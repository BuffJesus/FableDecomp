param(
    [Parameter(Position=0)][string]$Command,
    [Parameter(Position=1)][string]$Target
)

$ErrorActionPreference = 'Stop'
$headless = 'D:\Subuwu\tools\ghidra-public\support\analyzeHeadless.bat'
$projectDir = 'D:\Documents\FableTLC\ghidra_proj'
$scriptDir = 'D:\Documents\FableTLC\tools\ghidra_scripts'

if ($Command -eq 'decompile') {
    if ([string]::IsNullOrWhiteSpace($Target)) { throw 'decompile needs an address' }
    $started = $false
    & $headless $projectDir FableTLC -process Fable.exe -noanalysis `
        -scriptPath $scriptDir -postScript DumpFuncBytesAndC.java $Target 2>&1 |
        ForEach-Object {
            $line = $_.ToString()
            if ($line -match 'DumpFuncBytesAndC\.java> ====(.*)$') {
                $started = $true
                "====" + $Matches[1]
            } elseif ($started) {
                if ($line -match '^INFO  DumpFuncBytesAndC\.java> (.*?)(?: \(GhidraScript\))?$') {
                    $Matches[1]
                } elseif ($line -notmatch '^INFO  (REPORT|HEADLESS|Opening|SCRIPT):') {
                    $line -replace ' \(GhidraScript\)$',''
                }
            }
        }
    exit $LASTEXITCODE
}

[Console]::Error.WriteLine("unknown command: $Command")
exit 2
