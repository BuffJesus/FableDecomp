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

    # Ghidra can only export an oracle for an address it knows is a function. Thousands of
    # landed candidates sit at starts DISCOVERED from the binary itself (crawl/manifest_gaps
    # -> crawl/xrefs -> crawl/manifest_add_gaps), which the Ghidra DB has never heard of, so
    # this regeneration silently drops their rows -- 3,584 of them on 2026-08-20, which
    # quietly reverted those functions to "compiled but never byte-compared" and deflated the
    # parity totals with no error anywhere.
    #
    # backfill_oracles.py re-derives those rows straight from Fable.exe using the manifest
    # boundaries. It is idempotent and never invents an oracle for an address that is not an
    # authoritative function start, so it is safe on every refresh. A failure here is logged
    # and tolerated: a thinner ledger is recoverable, a broken refresh is worse.
    $backfill = Join-Path $root 'tools\decomp_pipeline\backfill_oracles.py'
    $python = 'C:\Users\Cornelio\AppData\Local\Programs\Python\Python314\python.exe'
    if ((Test-Path -LiteralPath $backfill) -and (Test-Path -LiteralPath $python)) {
        $backfillOutput = & $python $backfill '--write'
        if ($LASTEXITCODE -ne 0) {
            Write-Output "WARNING: oracle backfill failed; landed candidates at discovered function starts are NOT byte-compared this cycle"
        }
        $backfillOutput | Where-Object { $_ -match 'APPENDED|oracle_missing' } | ForEach-Object {
            Write-Output "  backfill: $_"
        }
    }

    # Some Ghidra function bodies include trailing alignment or the beginning of the next
    # function. Apply only reviewed, length-guarded corrections; never run the generic tail
    # heuristic over the whole ledger (0059aa64 is a legitimate counterexample).
    $boundaryFixups = Join-Path $root 'tools\decomp_pipeline\apply_oracle_boundary_overrides.py'
    if ((Test-Path -LiteralPath $boundaryFixups) -and (Test-Path -LiteralPath $python)) {
        & $python $boundaryFixups $output '--write'
        if ($LASTEXITCODE -ne 0) {
            throw 'oracle boundary override validation failed'
        }
    }
}
finally {
    Remove-Item -LiteralPath $addressFile -Force -ErrorAction SilentlyContinue
    Remove-Item -LiteralPath $temp -Force -ErrorAction SilentlyContinue
}
Write-Output "CANDIDATE_ORACLES PASS functions=$($addresses.Count) output=$output"
