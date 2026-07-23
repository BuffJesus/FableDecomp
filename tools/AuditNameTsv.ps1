param(
    [Parameter(Mandatory = $true)]
    [string]$Path,
    [int]$Sample = 8
)

$ErrorActionPreference = "Stop"

if (-not (Test-Path $Path)) {
    throw "Missing TSV: $Path"
}

$addrSeen = @{}
$nameSeen = @{}
$dupAddr = 0
$dupName = 0
$blankName = 0
$badAddr = 0
$whitespaceName = 0
$rows = 0
$examples = New-Object System.Collections.Generic.List[string]
$addrIndex = 0
$nameIndex = 1
$detected = "rename-plan"

Get-Content $Path | ForEach-Object {
    $line = $_
    if ([string]::IsNullOrWhiteSpace($line)) { return }
    if ($line.StartsWith("#")) {
        $header = $line.TrimStart("#") -split "`t"
        for ($i = 0; $i -lt $header.Count; $i++) {
            if ($header[$i] -eq "target_addr") { $addrIndex = $i }
            if ($header[$i] -eq "donor_name") { $nameIndex = $i; $detected = "bsim-audit" }
        }
        return
    }
    $cols = $line -split "`t"
    if ($cols.Count -le [math]::Max($addrIndex, $nameIndex)) { return }
    $addr = $cols[$addrIndex]
    $name = $cols[$nameIndex]
    $rows++

    if ($addr -notmatch "^(0x)?[0-9a-fA-F]+$") { $badAddr++ }
    if ([string]::IsNullOrWhiteSpace($name)) { $blankName++ }
    if ($name -match "\s") {
        $whitespaceName++
        if ($examples.Count -lt $Sample) { $examples.Add("$addr`t$name") }
    }
    if ($addrSeen.ContainsKey($addr)) { $dupAddr++ } else { $addrSeen[$addr] = $true }
    if ($nameSeen.ContainsKey($name)) { $dupName++ } else { $nameSeen[$name] = $true }
}

Write-Host "TSV audit: $Path"
Write-Host "Detected format: $detected"
Write-Host "Address column: $addrIndex"
Write-Host "Name column: $nameIndex"
Write-Host "Rows: $rows"
Write-Host "Unique addresses: $($addrSeen.Count)"
Write-Host "Unique names: $($nameSeen.Count)"
Write-Host "Duplicate address rows: $dupAddr"
Write-Host "Duplicate name rows: $dupName"
Write-Host "Bad address rows: $badAddr"
Write-Host "Blank names: $blankName"
Write-Host "Names containing whitespace: $whitespaceName"

if ($examples.Count -gt 0) {
    Write-Host ""
    Write-Host "Whitespace examples:"
    $examples
}
