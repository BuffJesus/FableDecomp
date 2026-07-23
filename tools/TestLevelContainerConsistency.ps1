param(
    [string]$IndexDir = "ghidra_out/installed_game",
    [string]$OutputPath = "ghidra_out/installed_game/level_container_consistency.csv"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function BaseName([string]$Path) {
    return [IO.Path]::GetFileNameWithoutExtension($Path)
}

$maps = Import-Csv -LiteralPath (Join-Path $IndexDir "wld_maps.csv")
$wad = Import-Csv -LiteralPath (Join-Path $IndexDir "finalalbion_wad_entries.csv")
$stb = Import-Csv -LiteralPath (Join-Path $IndexDir "finalalbion_stb_entries.csv")
$tng = Import-Csv -LiteralPath (Join-Path $IndexDir "loose_tng_summary.csv")

$wadLev = @{}
$wadTng = @{}
foreach ($entry in $wad) {
    $base = (BaseName $entry.name).ToLowerInvariant()
    if ($entry.extension -eq ".lev") { $wadLev[$base] = $true }
    if ($entry.extension -eq ".tng") { $wadTng[$base] = $true }
}

$stbLev = @{}
foreach ($entry in $stb) {
    if ($entry.extension -eq ".lev") {
        $stbLev[(BaseName $entry.name).ToLowerInvariant()] = $true
    }
}

$looseTng = @{}
foreach ($entry in $tng) {
    $looseTng[(BaseName $entry.file).ToLowerInvariant()] = $true
}

$rows = New-Object System.Collections.Generic.List[object]
foreach ($map in $maps) {
    $levelBase = (BaseName $map.level_name).ToLowerInvariant()
    $scriptBase = $map.level_script_name.ToLowerInvariant()
    $rows.Add([pscustomobject]@{
        id = $map.id
        level_script_name = $map.level_script_name
        level_name = $map.level_name
        has_wad_lev = $wadLev.ContainsKey($levelBase)
        has_wad_tng = $wadTng.ContainsKey($scriptBase) -or $wadTng.ContainsKey($levelBase)
        has_stb_lev = $stbLev.ContainsKey($levelBase)
        has_loose_tng = $looseTng.ContainsKey($scriptBase) -or $looseTng.ContainsKey($levelBase)
    })
}

$rows | Export-Csv -LiteralPath $OutputPath -NoTypeInformation

$missingWadLev = ($rows | Where-Object { $_.has_wad_lev -ne $true } | Measure-Object).Count
$missingWadTng = ($rows | Where-Object { $_.has_wad_tng -ne $true } | Measure-Object).Count
$missingStbLev = ($rows | Where-Object { $_.has_stb_lev -ne $true } | Measure-Object).Count
$missingLooseTng = ($rows | Where-Object { $_.has_loose_tng -ne $true } | Measure-Object).Count

"maps=$($rows.Count) missing_wad_lev=$missingWadLev missing_wad_tng=$missingWadTng missing_stb_lev=$missingStbLev missing_loose_tng=$missingLooseTng output=$OutputPath"
