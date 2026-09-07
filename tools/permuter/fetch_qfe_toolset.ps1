<#
    fetch_qfe_toolset.ps1 — download the VC7.1 13.10.4035 (QFE) compiler used to build
    ~11 of retail Fable.exe's object files, which our RTM 3077 toolchain cannot reproduce
    byte-exact. See docs/pipeline/QFE4035_COMPILER_GATE.md for the evidence (PE Rich header).

    SOURCE (review before running): the WinDDK 3790.1830 (Windows Server 2003 SP1 DDK)
    x86 compiler binaries, from the community mirror below. These are Microsoft binaries;
    you are responsible for your license to use them. This script only DOWNLOADS and
    verifies file versions — it does not execute anything.

        https://github.com/9176324/WinDDK  (tree: 3790.1830/bin/x86)

    Usage:  powershell -ExecutionPolicy Bypass -File tools\permuter\fetch_qfe_toolset.ps1
    Then:   python tools\permuter\permuter_score.py --selfcheck --qfe
#>
param(
    [string]$Dest = $(if ($env:VC71_QFE) { Join-Path $env:VC71_QFE 'bin' } else { 'D:\Tools\vc71-qfe4035\bin' }),
    [string]$Base = 'https://raw.githubusercontent.com/9176324/WinDDK/master/3790.1830/bin/x86'
)
$ErrorActionPreference = 'Stop'

# c1.dll = C frontend, c1xx.dll = C++ frontend, c2.dll = codegen backend; cl.exe drives.
# msobj71/mspdb71/msvcr71/msvcp71 are the compiler's own runtime deps.
$files = 'cl.exe','c1.dll','c1xx.dll','c2.dll','msobj71.dll','mspdb71.dll','msvcr71.dll','msvcp71.dll','link.exe'

New-Item -ItemType Directory -Force -Path $Dest | Out-Null
Write-Host "Downloading VC7.1 QFE-4035 compiler to $Dest" -ForegroundColor Cyan
Write-Host "Source: $Base" -ForegroundColor DarkGray
foreach ($f in $files) {
    $out = Join-Path $Dest $f
    Invoke-WebRequest -Uri "$Base/$f" -OutFile $out -UseBasicParsing
    Write-Host ("  {0,-14} {1,10:n0} bytes" -f $f, (Get-Item $out).Length)
}

# Version gate: refuse the toolset unless the codegen-relevant binaries are 13.10.4035.
Write-Host "`nVerifying build number (must be 13.10.4035):" -ForegroundColor Cyan
$ok = $true
foreach ($f in 'cl.exe','c1xx.dll','c2.dll') {
    $v = (Get-Item (Join-Path $Dest $f)).VersionInfo.FileVersion
    $good = $v -match '13\.10\.4035'
    if (-not $good) { $ok = $false }
    Write-Host ("  {0,-10} {1}  {2}" -f $f, $v, $(if ($good) { 'OK' } else { 'WRONG BUILD' })) `
        -ForegroundColor $(if ($good) { 'Green' } else { 'Red' })
}
if (-not $ok) {
    Write-Host "`nABORT: downloaded binaries are not 13.10.4035. Do not use this toolset." -ForegroundColor Red
    Write-Host "The mirror layout may have changed; source the exact 3790.1830 SP1 DDK compiler." -ForegroundColor Red
    exit 1
}
Write-Host "`n13.10.4035 toolset ready at $Dest" -ForegroundColor Green
Write-Host "Next: python tools\permuter\permuter_score.py --selfcheck --qfe" -ForegroundColor Green
