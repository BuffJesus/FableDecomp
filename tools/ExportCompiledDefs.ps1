param(
    [string]$GameRoot = "C:\Programs\Steam\steamapps\common\Fable The Lost Chapters",
    [string]$OutputDir = "ghidra_out/installed_game"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$compiledDefs = Join-Path $GameRoot "data\CompiledDefs"
$names = Join-Path $compiledDefs "names.bin"
$bins = @("game.bin", "script.bin", "frontend.bin")

foreach ($binName in $bins) {
    $binPath = Join-Path $compiledDefs $binName
    if (!(Test-Path -LiteralPath $binPath)) {
        continue
    }
    $stem = [IO.Path]::GetFileNameWithoutExtension($binName)
    $out = Join-Path $OutputDir "compiled_${stem}_entries.csv"
    dotnet run --project tools/bin_dump/BinDump.csproj -- $names $binPath $out
}

$summaryRows = New-Object System.Collections.Generic.List[object]
foreach ($csv in Get-ChildItem -LiteralPath $OutputDir -File -Filter "compiled_*_entries.csv") {
    $rows = Import-Csv -LiteralPath $csv.FullName
    $summaryRows.Add([pscustomobject]@{
        file = $csv.Name
        entries = ($rows | Measure-Object).Count
        unique_definitions = ($rows | Group-Object definition | Measure-Object).Count
        named_entries = ($rows | Where-Object { ![string]::IsNullOrWhiteSpace($_.name) } | Measure-Object).Count
    })
}

$summaryRows | Export-Csv -LiteralPath (Join-Path $OutputDir "compiled_defs_summary.csv") -NoTypeInformation
"compiled_defs=$($summaryRows.Count)"
