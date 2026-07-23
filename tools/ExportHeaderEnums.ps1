param(
    [string]$DefsRoot = "C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\Defs",
    [string]$OutputDir = "ghidra_out/installed_game"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

function CsvSafe([string]$Value) {
    if ($null -eq $Value) { return '""' }
    return '"' + $Value.Replace('"', '""') + '"'
}

$enumRows = New-Object System.Collections.Generic.List[string]
$enumRows.Add("file,enum,name,value")

foreach ($file in Get-ChildItem -LiteralPath $DefsRoot -Recurse -File -Filter *.h) {
    $rel = $file.FullName.Substring($DefsRoot.Length).TrimStart("\", "/")
    $enumName = ""
    $inEnum = $false
    $nextValue = 0L

    foreach ($raw in Get-Content -LiteralPath $file.FullName) {
        $line = ($raw -replace "//.*$", "").Trim()
        if (!$inEnum) {
            if ($line -match '^enum\s+([A-Za-z_][A-Za-z0-9_]*)?') {
                $enumName = if ($Matches[1]) { $Matches[1] } else { "(anonymous)" }
                $inEnum = $true
                $nextValue = 0
            }
            continue
        }

        if ($line -match '^\};') {
            $inEnum = $false
            $enumName = ""
            continue
        }

        if ($line -match '^([A-Za-z_][A-Za-z0-9_]*)(\s*=\s*([^,\s]+))?\s*,?') {
            $name = $Matches[1]
            $valueText = $Matches[3]
            $value = $nextValue
            if (![string]::IsNullOrWhiteSpace($valueText)) {
                if ($valueText -match '^0x[0-9A-Fa-f]+$') {
                    $value = [Convert]::ToInt64($valueText.Substring(2), 16)
                }
                elseif ($valueText -match '^-?\d+$') {
                    $value = [int64]$valueText
                }
            }
            $enumRows.Add((@(CsvSafe $rel; CsvSafe $enumName; CsvSafe $name; $value) -join ","))
            $nextValue = $value + 1
        }
    }
}

$enumPath = Join-Path $OutputDir "header_enums.csv"
$enumRows | Set-Content -LiteralPath $enumPath -Encoding UTF8

$enums = Import-Csv -LiteralPath $enumPath
$summaryPath = Join-Path $OutputDir "header_enum_summary.txt"
$byEnumPath = Join-Path $OutputDir "header_enum_counts.csv"
$enums |
    Group-Object enum |
    Sort-Object Count -Descending |
    Select-Object @{Name="enum";Expression={$_.Name}},Count |
    Export-Csv -LiteralPath $byEnumPath -NoTypeInformation

@(
    "rows=$($enums.Count)"
    "enum_count=$(($enums | Group-Object enum | Measure-Object).Count)"
    "source=$DefsRoot"
) | Set-Content -LiteralPath $summaryPath -Encoding ASCII

"rows=$($enums.Count) enums=$(($enums | Group-Object enum | Measure-Object).Count)"
