<#
.SYNOPSIS
    Extract the FableTLC frontend atlases from a base-game install into loose
    files, recomp-style (Sonic Unleashed Recompiled / the Zelda decomps).

.DESCRIPTION
    The reconstruction ships asset-free: the frontend art (menus, backdrops,
    fonts, sprites) is not embedded in the executable.  This tool decodes it
    from the player's own base-game .big files into loose 32-bit .bmp atlases
    under <OutputDir>, which the runtime loads from disk at startup.

    It reuses the reconstruction's exact decode/compose pipeline (via
    build_bootstrap.ps1 -ExtractAssetsOnly), so the extracted atlases match the
    build byte-for-byte with no separate code path to drift out of sync.

.PARAMETER BaseGameDir
    Path to the installed "Fable The Lost Chapters" directory (the folder that
    contains data\graphics\pc\frontend.big).  Defaults to the standard Steam
    install location.

.PARAMETER OutputDir
    Where to write the loose atlases.  A "data\frontend" subfolder is created
    under it.  Point this at the folder next to the reconstruction executable.

.EXAMPLE
    ./extract_frontend_assets.ps1 -BaseGameDir "C:\Steam\...\Fable The Lost Chapters" -OutputDir "C:\FableTLC"
#>
param(
    [string]$BaseGameDir =
        'C:\Programs\Steam\steamapps\common\Fable The Lost Chapters',
    [string]$OutputDir = (Get-Location).Path
)

$ErrorActionPreference = 'Stop'

$frontendBank = Join-Path $BaseGameDir 'data\graphics\pc\frontend.big'
if (-not (Test-Path -LiteralPath $frontendBank)) {
    throw ("frontend.big not found under the base game directory. " +
        "Expected: $frontendBank")
}

$scriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$buildScript = Join-Path (Split-Path -Parent $scriptRoot) 'rebuild\build_bootstrap.ps1'
if (-not (Test-Path -LiteralPath $buildScript)) {
    throw "build_bootstrap.ps1 not found at $buildScript"
}

Write-Host "Extracting frontend atlases from: $BaseGameDir"
& $buildScript -RetailFrontendBank $frontendBank -ExtractAssetsOnly
if ($LASTEXITCODE -ne 0) {
    throw "asset extraction failed (build_bootstrap.ps1 exit $LASTEXITCODE)"
}

$stagedDir = Join-Path (Split-Path -Parent $scriptRoot) `
    'rebuild\build\bootstrap-Release\data\frontend'
if (-not (Test-Path -LiteralPath $stagedDir)) {
    throw "extraction produced no atlases at $stagedDir"
}

$destDir = Join-Path $OutputDir 'data\frontend'
New-Item -ItemType Directory -Force $destDir | Out-Null
Copy-Item (Join-Path $stagedDir '*.bmp') $destDir -Force

$count = (Get-ChildItem $destDir -Filter '*.bmp').Count
Write-Host "EXTRACT_FRONTEND_ASSETS PASS atlases=$count dir=$destDir"
