param()

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent (Split-Path -Parent $MyInvocation.MyCommand.Path)
$headless = 'D:\Subuwu\tools\ghidra-public\support\analyzeHeadless.bat'
$project = Join-Path $root 'ghidra_proj'
$scripts = Join-Path $root 'tools\ghidra_scripts'
$candidateManifest = Join-Path $root 'rebuild\compile-gate\vc71-compiled.tsv'
$output = Join-Path $root 'rebuild\oracles\auto-re-candidates.tsv'
$supplement = Join-Path $root 'rebuild\oracles\manual-re-candidates.tsv'
$temp = "$output.tmp.$PID"
$addressFile = "$output.addresses.$PID.txt"

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

try {
    [System.IO.File]::WriteAllLines(
        $addressFile,
        [string[]]$addresses,
        [System.Text.UTF8Encoding]::new($false)
    )
    $arguments = @(
        $project,
        'FableTLC',
        '-process', 'Fable.exe',
        '-noanalysis',
        '-scriptPath', $scripts,
        '-postScript', 'ExportFunctionOracle.java', $temp, "@$addressFile"
    )

    & $headless @arguments
    if ($LASTEXITCODE -ne 0 -or -not (Test-Path -LiteralPath $temp)) {
        throw "Candidate oracle export failed with exit code $LASTEXITCODE"
    }

    # Some VC7.1 template helpers are emitted as adjacent instruction regions
    # that Ghidra either does not mark as function starts or truncates at the
    # first return. Preserve their independently verified complete retail
    # spans across automatic oracle refreshes.
    if (Test-Path -LiteralPath $supplement) {
        $generatedRows = @(Import-Csv -LiteralPath $temp -Delimiter "`t")
        $supplementRows = @(Import-Csv -LiteralPath $supplement -Delimiter "`t")
        $overrides = @{}
        foreach ($row in $supplementRows) {
            $overrides[$row.address.ToLowerInvariant()] = $row
        }

        $lines = [System.Collections.Generic.List[string]]::new()
        $lines.Add("address`tname`tlength`tbytes")
        foreach ($row in $generatedRows) {
            $key = $row.address.ToLowerInvariant()
            if ($overrides.ContainsKey($key)) {
                $row = $overrides[$key]
                $overrides.Remove($key)
            }
            $lines.Add(
                "$($row.address)`t$($row.name)`t$($row.length)`t$($row.bytes)"
            )
        }
        foreach ($key in @($overrides.Keys | Sort-Object)) {
            $row = $overrides[$key]
            $lines.Add(
                "$($row.address)`t$($row.name)`t$($row.length)`t$($row.bytes)"
            )
        }
        [System.IO.File]::WriteAllLines(
            $temp,
            [string[]]$lines,
            [System.Text.UTF8Encoding]::new($false)
        )
    }
    Move-Item -LiteralPath $temp -Destination $output -Force
}
finally {
    Remove-Item -LiteralPath $addressFile -Force -ErrorAction SilentlyContinue
    Remove-Item -LiteralPath $temp -Force -ErrorAction SilentlyContinue
}
Write-Output "CANDIDATE_ORACLES PASS functions=$($addresses.Count) output=$output"
