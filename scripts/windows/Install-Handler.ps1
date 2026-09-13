param(
    [Parameter(Mandatory=$true)][ValidateSet('ss2-remaster-gog','nwn2-ee-gog')][string]$Game,
    [string]$NucleusDirectory = 'C:\NucleusCo-op'
)
$ErrorActionPreference = 'Stop'
$workspace = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$catalog = Get-Content -LiteralPath (Join-Path $workspace 'games\index.json') -Raw | ConvertFrom-Json
$entry = $catalog.games | Where-Object slug -eq $Game
if (!(Test-Path -LiteralPath (Join-Path $NucleusDirectory 'NucleusCoop.exe'))) { throw 'Nucleus installation not found.' }
$processName = [IO.Path]::GetFileNameWithoutExtension($entry.executable)
if (Get-Process NucleusCoop,$processName -ErrorAction SilentlyContinue) { throw 'End the affected session and close Nucleus normally before deployment.' }
$source = Join-Path $workspace "games\$Game\handler.js"
$text = Get-Content -LiteralPath $source -Raw
$title = [regex]::Match($text, 'Game.GameName = "([^"]+)"').Groups[1].Value
if (!$title) { throw 'Handler title missing.' }
$existing = @(Get-ChildItem -LiteralPath (Join-Path $NucleusDirectory 'handlers') -Filter '*.js' | Where-Object {
    (Get-Content -LiteralPath $_.FullName -Raw) -match ('Game.GUID\s*=\s*"' + [regex]::Escape($entry.guid) + '"')
})
if ($existing.Count -gt 1) { throw 'Multiple handlers use this GUID; inspect duplicates before deploying.' }
$name = if ($existing.Count) { $existing[0].BaseName } else { $title -replace '[\\/:*?"<>|]', '' }
$destination = Join-Path $NucleusDirectory "handlers\$name.js"
$assets = Join-Path $NucleusDirectory "handlers\$name"
$backup = Join-Path $workspace ('storage\backups\' + $Game + '-' + (Get-Date -Format 'yyyyMMdd-HHmmss-fff'))
New-Item -ItemType Directory -Force -Path $backup | Out-Null
foreach ($item in @($destination,$assets)) {
    if (Test-Path -LiteralPath $item) { Copy-Item -LiteralPath $item -Destination $backup -Recurse }
}
if ($entry.native_adapter) {
    $cache = Join-Path $workspace "storage\assets\$Game"
    $manifest = Get-Content -LiteralPath (Join-Path $workspace "games\$Game\dependencies.json") -Raw | ConvertFrom-Json
    foreach ($asset in $manifest.assets) {
        $file = Join-Path $cache $asset.file
        if (!(Test-Path -LiteralPath $file) -or (Get-FileHash -LiteralPath $file -Algorithm SHA256).Hash -ne $asset.sha256) { throw "Prepare the private pinned asset cache before deployment: $($asset.file)" }
    }
    New-Item -ItemType Directory -Force -Path $assets | Out-Null
    foreach ($asset in $manifest.assets) {
        $target = Join-Path $assets $asset.file
        if ((Test-Path -LiteralPath $target) -and ((Get-Item -LiteralPath $target).Attributes -band [IO.FileAttributes]::ReparsePoint)) { throw "Refusing to overwrite an asset symlink: $target" }
        Copy-Item -LiteralPath (Join-Path $cache $asset.file) -Destination $target -Force
    }
}
Copy-Item -LiteralPath $source -Destination $destination -Force
Write-Output "Installed $($entry.guid). Backup: $backup. Library registration and saved layouts were preserved; add the executable in Nucleus if needed."
