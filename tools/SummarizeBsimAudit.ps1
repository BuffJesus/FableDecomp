param(
    [Parameter(Mandatory = $true)]
    [string]$Path
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path $Path)) {
    throw "Missing BSim audit TSV: $Path"
}

$rows = 0
$applied = 0
$failed = 0
$targets = @{}
$names = @{}
$bands = [ordered]@{
    "0.95-1.00" = 0
    "0.90-0.95" = 0
    "0.80-0.90" = 0
    "0.70-0.80" = 0
    "0.60-0.70" = 0
    "0.50-0.60" = 0
    "<0.50" = 0
}
$sigMin = [double]::PositiveInfinity
$sigMax = [double]::NegativeInfinity
$simMin = [double]::PositiveInfinity
$simMax = [double]::NegativeInfinity

Get-Content $Path | ForEach-Object {
    $line = $_
    if ([string]::IsNullOrWhiteSpace($line) -or $line.StartsWith("#")) { return }
    $c = $line -split "`t"
    if ($c.Count -lt 6) { return }
    $rows++

    $target = $c[0]
    $name = $c[2]
    $sim = [double]$c[3]
    $sig = [double]$c[4]
    $ok = $c[5] -eq "true"

    if ($ok) { $applied++ } else { $failed++ }
    if (-not $targets.ContainsKey($target)) { $targets[$target] = 0 }
    $targets[$target]++
    if (-not $names.ContainsKey($name)) { $names[$name] = 0 }
    $names[$name]++

    $simMin = [math]::Min($simMin, $sim)
    $simMax = [math]::Max($simMax, $sim)
    $sigMin = [math]::Min($sigMin, $sig)
    $sigMax = [math]::Max($sigMax, $sig)

    if ($sim -ge 0.95) { $bands["0.95-1.00"]++ }
    elseif ($sim -ge 0.90) { $bands["0.90-0.95"]++ }
    elseif ($sim -ge 0.80) { $bands["0.80-0.90"]++ }
    elseif ($sim -ge 0.70) { $bands["0.70-0.80"]++ }
    elseif ($sim -ge 0.60) { $bands["0.60-0.70"]++ }
    elseif ($sim -ge 0.50) { $bands["0.50-0.60"]++ }
    else { $bands["<0.50"]++ }
}

$dupTargetRows = ($targets.GetEnumerator() | Where-Object { $_.Value -gt 1 } | Measure-Object).Count
$dupNameRows = ($names.GetEnumerator() | Where-Object { $_.Value -gt 1 } | Measure-Object).Count

Write-Host "BSim audit: $Path"
Write-Host "Rows: $rows"
Write-Host "Applied: $applied"
Write-Host "Failed: $failed"
Write-Host "Unique targets: $($targets.Count)"
Write-Host "Repeated target addresses: $dupTargetRows"
Write-Host "Unique donor names: $($names.Count)"
Write-Host "Repeated donor names: $dupNameRows"
Write-Host "Similarity min/max: $([math]::Round($simMin, 6)) / $([math]::Round($simMax, 6))"
Write-Host "Significance min/max: $([math]::Round($sigMin, 6)) / $([math]::Round($sigMax, 6))"
Write-Host ""
Write-Host "Similarity bands:"
foreach ($kv in $bands.GetEnumerator()) {
    Write-Host ("  {0}: {1}" -f $kv.Key, $kv.Value)
}
