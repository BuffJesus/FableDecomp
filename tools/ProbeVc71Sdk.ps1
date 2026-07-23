param(
    [string]$Version = '10.0.22621.0'
)

$ErrorActionPreference = 'Stop'

$root = 'D:\Documents\FableTLC'
$vcRoot = 'D:\Tools\vc71'
$sdkRoot = 'C:\Program Files (x86)\Windows Kits\10'
$probeRoot = Join-Path $root 'rebuild\probes'
$source = Join-Path $probeRoot 'vc71_sdk_probe.cpp'
$outDir = Join-Path $probeRoot "out-$Version"
$reportPath = Join-Path $probeRoot "vc71-sdk-$Version.md"
$compileLog = Join-Path $outDir 'compile.log'
$linkLog = Join-Path $outDir 'link.log'

New-Item -ItemType Directory -Path $outDir -Force | Out-Null
$includeParts = @(
    (Join-Path $vcRoot 'include'),
    (Join-Path $sdkRoot "Include\$Version\shared"),
    (Join-Path $sdkRoot "Include\$Version\um"),
    (Join-Path $sdkRoot "Include\$Version\winrt")
)
$libParts = @(
    (Join-Path $vcRoot 'lib'),
    (Join-Path $sdkRoot "Lib\$Version\um\x86")
)

$missing = @($includeParts + $libParts | Where-Object { -not (Test-Path -LiteralPath $_) })
if ($missing.Count -gt 0) {
    throw "SDK probe paths missing: $($missing -join ', ')"
}

$oldPath = $env:PATH
$oldInclude = $env:INCLUDE
$oldLib = $env:LIB
try {
    $env:PATH = "$(Join-Path $vcRoot 'bin');$oldPath"
    $env:INCLUDE = $includeParts -join ';'
    $env:LIB = $libParts -join ';'
    Push-Location $outDir
    try {
        $compileOutput = & (Join-Path $vcRoot 'bin\cl.exe') /nologo /c /O2 /Oy /W3 `
            /D_WIN32_WINNT=0x0501 /DWINVER=0x0501 $source 2>&1
        $compileExit = $LASTEXITCODE
        Set-Content -LiteralPath $compileLog -Value $compileOutput -Encoding UTF8

        $linkExit = -1
        $runtimeExit = -1
        $linkOutput = @('link skipped because compilation failed')
        if ($compileExit -eq 0) {
            $object = Join-Path $outDir 'vc71_sdk_probe.obj'
            $linkOutput = & (Join-Path $vcRoot 'bin\link.exe') /nologo /subsystem:windows `
                /out:vc71_sdk_probe.exe $object kernel32.lib user32.lib d3d9.lib 2>&1
            $linkExit = $LASTEXITCODE
            if ($linkExit -eq 0) {
                $probeProcess = Start-Process -FilePath (Join-Path $outDir 'vc71_sdk_probe.exe') -Wait -PassThru
                $runtimeExit = $probeProcess.ExitCode
            }
        }
        Set-Content -LiteralPath $linkLog -Value $linkOutput -Encoding UTF8
    } finally {
        Pop-Location
    }
} finally {
    $env:PATH = $oldPath
    $env:INCLUDE = $oldInclude
    $env:LIB = $oldLib
}

$status = if ($compileExit -eq 0 -and $linkExit -eq 0 -and $runtimeExit -eq 0) { 'PASS' } elseif ($compileExit -eq 0) { 'PARTIAL' } else { 'FAIL' }
$lines = @(
    '# VC7.1 Platform SDK compatibility probe',
    '',
    "- SDK: Windows Kits 10 $Version",
    '- Compiler: D:\Tools\vc71\bin\cl.exe',
    "- Compile exit: $compileExit",
    "- Link exit: $linkExit",
    "- Runtime exit: $runtimeExit",
    "- Result: **$status**",
    '',
    '## Include path',
    '',
    '```text',
    ($includeParts -join "`n"),
    '```',
    '',
    '## Logs',
    '',
    "- $compileLog",
    "- $linkLog"
)
Set-Content -LiteralPath $reportPath -Value $lines -Encoding UTF8
Write-Output "VC71_SDK_PROBE result=$status compile=$compileExit link=$linkExit report=$reportPath"
if ($compileExit -ne 0) { exit 2 }
if ($linkExit -ne 0) { exit 3 }
if ($runtimeExit -ne 0) { exit 4 }
