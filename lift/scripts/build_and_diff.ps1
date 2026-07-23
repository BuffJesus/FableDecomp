# Lift harness — compile a lifted TU with VC7.1 and diff its .text against the
# retail bytes. Usage: powershell -File build_and_diff.ps1 <name.cpp> "<retail hex>"
# The bar-#2 oracle: a MATCH proves the lifted C++ recompiles to the original code.
param(
    [Parameter(Mandatory=$true)][string]$Source,
    [Parameter(Mandatory=$true)][string]$RetailHex   # e.g. "8b81e0000000d94078c3"
)
$vc = "D:\Tools\vc71"
$env:PATH = "$vc\bin;$env:PATH"; $env:INCLUDE = "$vc\include"; $env:LIB = "$vc\lib"
$liftRoot = Split-Path -Parent $PSScriptRoot
$buildRoot = Join-Path $liftRoot 'proofs\build'
New-Item -ItemType Directory -Path $buildRoot -Force | Out-Null
$obj = Join-Path $buildRoot "$([System.IO.Path]::GetFileNameWithoutExtension($Source)).obj"

& "$vc\bin\cl.exe" /nologo /c /O2 /Oy "/Fo$obj" $Source | Out-Null
if (-not (Test-Path $obj)) { Write-Error "compile failed"; exit 1 }

# Extract .text bytes via mingw objdump (dumpbin disasm needs msdis, absent in the portable extract).
$objdump = "C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\objdump.exe"
$raw = & $objdump -s -j .text $obj 2>$null
# Lines look like: " 0000 8b81e000 0000d940 78c3  ...."; keep the hex groups.
$hex = ($raw | Select-String -Pattern '^\s[0-9a-f]{4}\s' | ForEach-Object {
    ($_ -replace '^\s[0-9a-f]{4}\s','') -replace '\s{2,}.*$','' -replace '\s',''
}) -join ''
$hex = $hex.ToLower()
$retail = ($RetailHex -replace '\s','').ToLower()

Write-Output "lifted : $hex"
Write-Output "retail : $retail"
if ($hex -eq $retail) { Write-Output "RESULT : *** MATCH (byte-identical .text) ***" }
else { Write-Output "RESULT : DIFFER (len lifted=$($hex.Length/2) retail=$($retail.Length/2))" }
