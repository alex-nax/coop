param(
    [string]$NucleusDirectory = 'C:\NucleusCo-op',
    [Parameter(Mandatory=$true)][string]$CacheDirectory,
    [string]$ProbeExecutable = ''
)
$ErrorActionPreference = 'Stop'
$workspace = Split-Path -Parent (Split-Path -Parent $PSScriptRoot)
$failures = @()
Get-ChildItem -LiteralPath $workspace -Filter '*.ps1' -Recurse | Where-Object { $_.FullName -notmatch '\\storage\\' } | ForEach-Object {
    $tokens = $null; $errors = $null
    [System.Management.Automation.Language.Parser]::ParseFile($_.FullName, [ref]$tokens, [ref]$errors) | Out-Null
    if ($errors.Count) { $failures += $errors }
}
if ($failures.Count) { throw ($failures | Out-String) }
Write-Output 'PowerShell syntax checks passed.'
$scratch = Join-Path $workspace ('storage\checks\' + [guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Force -Path $scratch | Out-Null
$archive = Join-Path $workspace 'dist\nwn2-ee-gog-0.1.0.nc'
$zip = Join-Path $scratch 'handler.zip'
Copy-Item -LiteralPath $archive -Destination $zip
Expand-Archive -LiteralPath $zip -DestinationPath (Join-Path $scratch 'imported')
$assets = Join-Path $scratch 'imported\assets'
& (Join-Path $assets 'Prepare-Assets.ps1') -NucleusDirectory $NucleusDirectory -CacheDirectory $CacheDirectory
& (Join-Path $assets 'Prepare-Assets.ps1') -NucleusDirectory $NucleusDirectory -CacheDirectory $CacheDirectory
& (Join-Path $assets 'Prepare-Assets.ps1') -VerifyOnly
Write-Output 'Packaged dependency setup and repeat-run verification passed in an isolated directory.'
if ($ProbeExecutable) {
    $probe = Join-Path $scratch 'probe'
    New-Item -ItemType Directory -Force -Path (Join-Path $probe 'ngalaxye_settings') | Out-Null
    Copy-Item -LiteralPath $ProbeExecutable -Destination (Join-Path $probe 'galaxy_probe.exe')
    foreach ($file in @('Galaxy64.dll','NemirtingasGalaxy64.dll')) { Copy-Item -LiteralPath (Join-Path $assets $file) -Destination (Join-Path $probe $file) }
    $settings = '{"api_version":"1.152.10.0","disable_online_networking":true,"enable_lan":true,"log_level":"warn","enable_overlay":false,"galaxyid":145496244628982949,"language":"en","productid":1993442013,"username":"CoopAbiProbe"}'
    [IO.File]::WriteAllText((Join-Path $probe 'ngalaxye_settings\NemirtingasGalaxyEmu.json'), $settings, (New-Object Text.UTF8Encoding($false)))
    Push-Location $probe
    try {
        $result = & '.\galaxy_probe.exe' compat 2>&1
        $code = $LASTEXITCODE
        $result | Set-Content -LiteralPath (Join-Path $scratch 'galaxy-probe.txt')
        $report = $result -join "`n"
        if ($code -ne 0 -or $report -notmatch 'IsLoggedOn=1' -or $report -notmatch 'STATS_RESULT GetAchievement unlocked=1' -or $report -notmatch 'OnEncryptedAppTicketRetrieveSuccess' -or $report -notmatch 'GetEncryptedAppTicket length=107') { throw "Isolated ABI probe failed; see $scratch" }
        Write-Output 'Isolated ABI probe passed: auth, logged-on state, ticket and achievements.'
    } finally { Pop-Location }
}
Write-Output "Private verification output: $scratch"
