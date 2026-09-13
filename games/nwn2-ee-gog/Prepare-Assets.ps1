param(
    [string]$NucleusDirectory = 'C:\NucleusCo-op',
    [string]$AssetDirectory = $PSScriptRoot,
    [string]$CacheDirectory = '',
    [string]$SevenZip = '',
    [switch]$VerifyOnly
)
$ErrorActionPreference = 'Stop'
$manifest = Get-Content -LiteralPath (Join-Path $PSScriptRoot 'dependencies.json') -Raw | ConvertFrom-Json
function Test-AssetHash([string]$Path, [string]$Expected) {
    return (Test-Path -LiteralPath $Path -PathType Leaf) -and ((Get-FileHash -LiteralPath $Path -Algorithm SHA256).Hash -eq $Expected)
}
if ($VerifyOnly) {
    foreach ($asset in $manifest.assets) {
        if (!(Test-AssetHash (Join-Path $AssetDirectory $asset.file) $asset.sha256)) { throw "Missing or incorrect asset: $($asset.file)" }
    }
    Write-Output 'All four NWN2 assets match the pinned manifest.'
    return
}
if (Get-Process NucleusCoop,nwn2 -ErrorAction SilentlyContinue) { throw 'End the NWN2 session and close Nucleus normally before preparing assets.' }
New-Item -ItemType Directory -Force -Path $AssetDirectory | Out-Null
$temporary = Join-Path ([IO.Path]::GetTempPath()) ('coop-assets-' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $temporary | Out-Null
try {
    foreach ($asset in $manifest.assets) {
        $destination = Join-Path $AssetDirectory $asset.file
        if (Test-AssetHash $destination $asset.sha256) { Write-Output "Verified $($asset.file)"; continue }
        $source = $null
        if ($CacheDirectory) {
            $candidate = Join-Path $CacheDirectory $asset.file
            if (Test-AssetHash $candidate $asset.sha256) { $source = $candidate }
        }
        if (!$source -and $asset.provision -eq 'nucleus') {
            $source = Join-Path $NucleusDirectory 'utils\NemirtingasGalaxyEmu\x64\Galaxy64.dll'
        }
        if (!$source -and $asset.provision -eq 'bundled-original') {
            $source = Join-Path $PSScriptRoot $asset.file
        }
        if (!$source -and $asset.provision -eq 'download') {
            $downloadFolder = Join-Path $temporary ([IO.Path]::GetFileNameWithoutExtension($asset.file))
            New-Item -ItemType Directory -Path $downloadFolder | Out-Null
            $archive = Join-Path $downloadFolder ([IO.Path]::GetFileName(([uri]$asset.download).AbsolutePath))
            [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
            Invoke-WebRequest -UseBasicParsing -Uri $asset.download -OutFile $archive
            if ($asset.archive_sha256 -and !(Test-AssetHash $archive $asset.archive_sha256)) { throw "Archive hash mismatch: $archive" }
            $expanded = Join-Path $downloadFolder 'expanded'
            if ([IO.Path]::GetExtension($archive) -eq '.zip') {
                Expand-Archive -LiteralPath $archive -DestinationPath $expanded
            } else {
                if (!$SevenZip) {
                    $tool = Get-Command 7z.exe,7zz.exe -ErrorAction SilentlyContinue | Select-Object -First 1
                    if ($tool) { $SevenZip = $tool.Source }
                    elseif (Test-Path -LiteralPath 'C:\Program Files\7-Zip\7z.exe') { $SevenZip = 'C:\Program Files\7-Zip\7z.exe' }
                }
                if (!$SevenZip) { throw 'Install 7-Zip or pass -SevenZip with its executable path, then retry.' }
                & $SevenZip x $archive ('-o' + $expanded) -y | Out-Null
                if ($LASTEXITCODE -ne 0) { throw '7-Zip extraction failed.' }
            }
            $matches = @(Get-ChildItem -LiteralPath $expanded -Recurse -File -Filter $asset.file | Where-Object { Test-AssetHash $_.FullName $asset.sha256 })
            if (!$matches.Count) { throw "The upstream archive has no matching $($asset.file)." }
            $source = $matches[0].FullName
        }
        if (!$source -or !(Test-AssetHash $source $asset.sha256)) { throw "Cannot provision pinned $($asset.file). Check Nucleus 2.4.2, cache and package versions; no substitute was installed." }
        if (Test-Path -LiteralPath $destination) {
            if ((Get-Item -LiteralPath $destination).Attributes -band [IO.FileAttributes]::ReparsePoint) { throw "Refusing to replace a symlink: $destination" }
            $backup = $destination + '.backup-' + (Get-Date -Format 'yyyyMMdd-HHmmss-fff')
            Copy-Item -LiteralPath $destination -Destination $backup
        }
        $pending = $destination + '.pending-' + [guid]::NewGuid().ToString('N')
        Copy-Item -LiteralPath $source -Destination $pending
        if (!(Test-AssetHash $pending $asset.sha256)) { throw "Copy verification failed: $pending" }
        Move-Item -LiteralPath $pending -Destination $destination -Force
        Write-Output "Prepared $($asset.file)"
    }
} finally {
    Remove-Item -LiteralPath $temporary -Recurse -Force
}
Write-Output 'NWN2 assets ready. Open Nucleus and use the left/right two-gamepad layout.'
