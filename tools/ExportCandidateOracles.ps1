param()

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
$headless = 'D:\Subuwu\tools\ghidra-public\support\analyzeHeadless.bat'
$project = Join-Path $root 'ghidra_proj'
$scripts = Join-Path $root 'tools\ghidra_scripts'
$candidateManifest = Join-Path $root 'rebuild\compile-gate\vc71-compiled.tsv'
$output = Join-Path $root 'rebuild\oracles\auto-re-candidates.tsv'
$temp = "$output.tmp.$PID"

if (-not (Test-Path -LiteralPath $candidateManifest)) {
    throw "Candidate compile manifest is missing: $candidateManifest"
}
$addresses = @(
    Import-Csv -LiteralPath $candidateManifest -Delimiter "`t" |
        Where-Object { $_.status -eq 'PASS' } |
        ForEach-Object { "0x$($_.address)" }
)
if ($addresses.Count -eq 0) {
    throw 'Candidate compile manifest contains no addresses'
}

$arguments = @(
    $project,
    'FableTLC',
    '-process', 'Fable.exe',
    '-noanalysis',
    '-scriptPath', $scripts,
    '-postScript', 'ExportFunctionOracle.java', $temp
) + $addresses

& $headless @arguments
if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $temp)) {
    throw "Candidate oracle export failed with exit code $LASTEXITCODE"
}
Move-Item -LiteralPath $temp -Destination $output -Force
Write-Output "CANDIDATE_ORACLES PASS functions=$($addresses.Count) output=$output"
