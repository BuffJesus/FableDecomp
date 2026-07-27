[CmdletBinding()]
param(
    [string]$SourceDirectory = '',

    [switch]$InstallSource,

    [string]$RetailMovie = ''
)

$ErrorActionPreference = 'Stop'

$rebuildRoot = Split-Path -Parent $PSCommandPath
$commit = '77f217b3f89d4dac7864a62cc91ff7b569f26a50'
$relativeBaseClasses =
    'Samples/Win7Samples/multimedia/directshow/baseclasses'
$sourceRepository = Join-Path (
    Join-Path $rebuildRoot 'build\tools'
) "windows-classic-samples-$($commit.Substring(0, 12))"

if ([string]::IsNullOrWhiteSpace($SourceDirectory)) {
    $SourceDirectory = Join-Path $sourceRepository $relativeBaseClasses
}

if (-not (Test-Path -LiteralPath $SourceDirectory)) {
    if (-not $InstallSource) {
        throw (
            'Microsoft DirectShow base-class source is not installed. ' +
            'Re-run with -InstallSource, or pass -SourceDirectory.'
        )
    }
    New-Item -ItemType Directory -Force -Path $sourceRepository |
        Out-Null
    & git -C $sourceRepository init
    if ($LASTEXITCODE -ne 0) {
        throw 'Could not initialise the DirectShow source repository.'
    }
    & git -C $sourceRepository remote add origin `
        'https://github.com/microsoft/Windows-classic-samples.git'
    if ($LASTEXITCODE -ne 0) {
        throw 'Could not configure the Microsoft sample repository.'
    }
    & git -C $sourceRepository sparse-checkout init --cone
    & git -C $sourceRepository sparse-checkout set $relativeBaseClasses
    & git -C $sourceRepository fetch --depth 1 origin $commit
    if ($LASTEXITCODE -ne 0) {
        throw "Could not fetch pinned Microsoft commit $commit."
    }
    & git -C $sourceRepository checkout --detach FETCH_HEAD
    if ($LASTEXITCODE -ne 0) {
        throw "Could not check out pinned Microsoft commit $commit."
    }
}

$SourceDirectory = (Resolve-Path -LiteralPath $SourceDirectory).Path
$requiredSource = Join-Path $SourceDirectory 'renbase.cpp'
if (-not (Test-Path -LiteralPath $requiredSource)) {
    throw "DirectShow base classes were not found in $SourceDirectory"
}

. (Join-Path $rebuildRoot 'vc71_sdk_env.ps1') | Out-Null

$outputDirectory = Join-Path (
    Join-Path $rebuildRoot 'build\tools'
) 'directshow-baseclasses-vc71'
New-Item -ItemType Directory -Force -Path $outputDirectory |
    Out-Null

$compiler = 'D:\Tools\vc71\bin\cl.exe'
$librarian = 'D:\Tools\vc71\bin\lib.exe'
$compileOptions = @(
    '/nologo',
    '/w',
    '/c',
    '/O2',
    '/MT',
    '/DWIN32',
    '/DWIN32_LEAN_AND_MEAN',
    '/DNDEBUG',
    '/D_LIB',
    "/I$SourceDirectory"
)
$objects = [System.Collections.Generic.List[string]]::new()
$sources = Get-ChildItem -LiteralPath $SourceDirectory -Filter '*.cpp' |
    Sort-Object Name
foreach ($source in $sources) {
    $object = Join-Path $outputDirectory "$($source.BaseName).obj"
    & $compiler @compileOptions "/Fo$object" $source.FullName
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $object)) {
        throw "Failed to compile Microsoft base class $($source.Name)."
    }
    $objects.Add($object)
}

$library = Join-Path $outputDirectory 'strmbase.lib'
& $librarian /nologo "/out:$library" $objects
if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $library)) {
    throw 'Failed to build the VC7.1 DirectShow base-class library.'
}

$layoutProbeSource = Join-Path (
    Join-Path $rebuildRoot 'probes'
) 'directshow_baseclasses_layout.cpp'
$layoutProbe = Join-Path $outputDirectory 'directshow-layout-probe.exe'
& $compiler @(
    '/nologo',
    '/w',
    '/O2',
    '/MT',
    '/DWIN32',
    '/DWIN32_LEAN_AND_MEAN',
    '/DNDEBUG',
    '/D_LIB',
    "/I$SourceDirectory",
    "/Fe$layoutProbe",
    $layoutProbeSource
)
if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $layoutProbe)) {
    throw 'Failed to build the DirectShow layout probe.'
}

$layoutOutput = & $layoutProbe
$layoutExitCode = $LASTEXITCODE
$layoutOutput | Write-Output
if (
    $layoutExitCode -ne 0 -or
    (($layoutOutput -join "`n") -notmatch
        'CBaseVideoRenderer=0x160')
) {
    throw (
        'Microsoft CBaseVideoRenderer does not match the retail ' +
        'CTextureRenderer base extent.'
    )
}

$textureProbeSource = Join-Path (
    Join-Path $rebuildRoot 'probes'
) 'fable_texture_renderer_layout.cpp'
$textureProbe = Join-Path (
    $outputDirectory
) 'fable-texture-renderer-layout-probe.exe'
& $compiler @(
    '/nologo',
    '/w',
    '/O2',
    '/MT',
    '/DWIN32',
    '/DWIN32_LEAN_AND_MEAN',
    '/DNDEBUG',
    '/D_LIB',
    "/I$SourceDirectory",
    "/Fe$textureProbe",
    $textureProbeSource,
    $library,
    'strmiids.lib',
    'winmm.lib',
    'ole32.lib',
    'oleaut32.lib',
    'user32.lib',
    'gdi32.lib',
    'advapi32.lib'
)
if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $textureProbe)) {
    throw 'Failed to build the Fable texture-renderer layout probe.'
}

$textureOutput = & $textureProbe
$textureExitCode = $LASTEXITCODE
$textureOutput | Write-Output
if (
    $textureExitCode -ne 0 -or
    (($textureOutput -join "`n") -notmatch 'size=0x180')
) {
    throw (
        'The reconstructed CTextureRenderer inheritance layout does not ' +
        'match the retail 0x180-byte allocation.'
    )
}

$graphProbeSource = Join-Path (
    Join-Path $rebuildRoot 'probes'
) 'directshow_texture_renderer_graph.cpp'
$graphProbe = Join-Path (
    $outputDirectory
) 'directshow-texture-renderer-graph-probe.exe'
& $compiler @(
    '/nologo',
    '/w',
    '/O2',
    '/MT',
    '/DWIN32',
    '/DWIN32_LEAN_AND_MEAN',
    '/DNDEBUG',
    '/D_LIB',
    "/I$SourceDirectory",
    "/Fe$graphProbe",
    $graphProbeSource,
    $library,
    'strmiids.lib',
    'winmm.lib',
    'ole32.lib',
    'oleaut32.lib',
    'user32.lib',
    'gdi32.lib',
    'advapi32.lib'
)
if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $graphProbe)) {
    throw 'Failed to build the DirectShow texture-renderer graph probe.'
}

$graphProof = ''
if (-not [string]::IsNullOrWhiteSpace($RetailMovie)) {
    $RetailMovie = (Resolve-Path -LiteralPath $RetailMovie).Path
    $graphOutput = & $graphProbe $RetailMovie
    $graphExitCode = $LASTEXITCODE
    $graphOutput | Write-Output
    if (
        $graphExitCode -ne 0 -or
        (($graphOutput -join "`n") -notmatch
            'DIRECTSHOW_TEXTURE_RENDERER_GRAPH')
    ) {
        throw (
            'The recovered-shape texture renderer did not receive ' +
            'changing decoded retail frames.'
        )
    }
    $graphProof = ' graph=decoded-retail-frames'
}

Write-Output (
    "DIRECTSHOW_BASECLASSES PASS commit=$commit " +
    "sources=$($sources.Count) base-video-renderer=0x160 " +
    "texture-renderer=0x180 " +
    "library=$library$graphProof"
)
