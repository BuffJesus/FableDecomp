param(
    [string]$Configuration = 'Release'
)

$ErrorActionPreference = 'Stop'
$rebuildRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
$vcRoot = 'D:\Tools\vc71'
$sourceRoot = Join-Path $rebuildRoot 'src\tier0'
$outDir = Join-Path $rebuildRoot "build\tier0-$Configuration"
New-Item -ItemType Directory -Path $outDir -Force | Out-Null

$oldPath = $env:PATH
$oldInclude = $env:INCLUDE
$oldLib = $env:LIB
try {
    $env:PATH = "$(Join-Path $vcRoot 'bin');$oldPath"
    $env:INCLUDE = "$(Join-Path $vcRoot 'include');$(Join-Path $rebuildRoot 'include')"
    $env:LIB = Join-Path $vcRoot 'lib'
    foreach ($source in Get-ChildItem -LiteralPath $sourceRoot -Filter '*.cpp' -File) {
        $object = Join-Path $outDir "$($source.BaseName).obj"
        & (Join-Path $vcRoot 'bin\cl.exe') /nologo /c /O2 /Oy /W3 /Fo$object $source.FullName
        if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $object)) {
            throw "Tier-0 compile failed: $($source.Name)"
        }
    }
} finally {
    $env:PATH = $oldPath
    $env:INCLUDE = $oldInclude
    $env:LIB = $oldLib
}
Write-Output "TIER0_BUILD PASS objects=$(@(Get-ChildItem -LiteralPath $outDir -Filter '*.obj').Count) out=$outDir"
