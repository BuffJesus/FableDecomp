param(
    [string]$SdkVersion = '10.0.22621.0'
)

# Dot-source this file to activate the validated VC7.1 + Windows SDK x86
# environment in the current PowerShell process.
$vcRoot = 'D:\Tools\vc71'
$sdkRoot = 'C:\Program Files (x86)\Windows Kits\10'
$sdkInclude = Join-Path $sdkRoot "Include\$SdkVersion"
$sdkLib = Join-Path $sdkRoot "Lib\$SdkVersion"

$required = @(
    (Join-Path $vcRoot 'bin\cl.exe'),
    (Join-Path $sdkInclude 'um\Windows.h'),
    (Join-Path $sdkInclude 'shared\d3d9.h'),
    (Join-Path $sdkLib 'um\x86\kernel32.lib'),
    (Join-Path $sdkLib 'um\x86\d3d9.lib')
)
$missing = @($required | Where-Object { -not (Test-Path -LiteralPath $_) })
if ($missing.Count -gt 0) {
    throw "Validated VC7.1 SDK environment is incomplete: $($missing -join ', ')"
}

$env:PATH = "$(Join-Path $vcRoot 'bin');$env:PATH"
$env:INCLUDE = @(
    (Join-Path $vcRoot 'include'),
    (Join-Path $sdkInclude 'shared'),
    (Join-Path $sdkInclude 'um'),
    (Join-Path $sdkInclude 'winrt')
) -join ';'
$env:LIB = @(
    (Join-Path $vcRoot 'lib'),
    (Join-Path $sdkLib 'um\x86')
) -join ';'

Write-Output "VC71_SDK_ENV active sdk=$SdkVersion"
