param(
    [Parameter(Mandatory = $true)]
    [string]$Name,

    [ValidateSet('Symbol', 'Type')]
    [string]$Mode = 'Symbol',

    [string]$PdbPath = 'D:\Documents\FableTLC\debug_build\Ego_r.pdb',
    [string]$PdbUtilPath = 'C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\llvm-pdbutil.exe',
    [string]$DiaDirectory = 'C:\Program Files\Microsoft Visual Studio\2022\Community\DIA SDK\bin\amd64'
)

$ErrorActionPreference = 'Stop'
foreach ($path in @($PdbPath, $PdbUtilPath, $DiaDirectory)) {
    if (-not (Test-Path -LiteralPath $path)) {
        throw "PDB oracle dependency not found: $path"
    }
}

$oldPath = $env:PATH
try {
    $env:PATH = "$DiaDirectory;$oldPath"
    if ($Mode -eq 'Type') {
        $pattern = '^' + [regex]::Escape($Name) + '$'
        & $PdbUtilPath pretty --classes --class-definitions=layout "--include-types=$pattern" $PdbPath
    }
    else {
        & $PdbUtilPath pretty "--with-name=$Name" --load-address=0x400000 $PdbPath
    }
    if ($LASTEXITCODE -ne 0) {
        throw "llvm-pdbutil failed with exit code $LASTEXITCODE"
    }
}
finally {
    $env:PATH = $oldPath
}
