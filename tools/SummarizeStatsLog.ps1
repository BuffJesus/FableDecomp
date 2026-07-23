param(
    [Parameter(Mandatory = $true)]
    [string[]]$Path
)

$ErrorActionPreference = "Stop"

foreach ($p in $Path) {
    if (-not (Test-Path $p)) {
        Write-Warning "Missing log: $p"
        continue
    }

    $line = Select-String -Path $p -Pattern "STATS program=" | Select-Object -Last 1
    if (-not $line) {
        Write-Warning "No STATS line in $p"
        continue
    }

    $values = @{}
    foreach ($m in [regex]::Matches($line.Line, "([A-Za-z_]+)=([^ ]+)")) {
        $values[$m.Groups[1].Value] = $m.Groups[2].Value
    }

    $total = [int]$values["total_functions"]
    $named = [int]$values["named"]
    $default = [int]$values["default_named"]
    $pct = if ($total -gt 0) { 100.0 * $named / $total } else { 0 }

    [pscustomobject]@{
        Log = $p
        Program = $values["program"]
        TotalFunctions = $total
        Named = $named
        DefaultNamed = $default
        NamedPercent = "{0:n2}" -f $pct
        Thunks = [int]$values["thunks"]
        TotalSymbols = [int64]$values["total_symbols"]
    }
}
