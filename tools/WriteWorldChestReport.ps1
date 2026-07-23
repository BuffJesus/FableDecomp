param(
    [string]$IndexDir = "ghidra_out/installed_game",
    [string]$OutputPath = "docs/WORLD_AND_CHEST_INDEX.md"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$mapsPath = Join-Path $IndexDir "wld_maps.csv"
$regionsPath = Join-Path $IndexDir "wld_regions.csv"
$entitiesPath = Join-Path $IndexDir "chest_key_reward_entities.csv"
foreach ($path in @($mapsPath, $regionsPath, $entitiesPath)) {
    if (!(Test-Path -LiteralPath $path)) {
        throw "Missing input: $path"
    }
}

$maps = Import-Csv -LiteralPath $mapsPath
$regions = Import-Csv -LiteralPath $regionsPath
$entities = Import-Csv -LiteralPath $entitiesPath
$bt = [char]96
$md = New-Object System.Collections.Generic.List[string]

$md.Add("# World and Chest Index")
$md.Add("")
$md.Add("*Generated from installed `FinalAlbion.wld` and loose TNGs on $(Get-Date -Format yyyy-MM-dd).*")
$md.Add("")
$md.Add("## Totals")
$md.Add("")
$md.Add("- WLD maps: $bt$(($maps | Measure-Object).Count)$bt")
$md.Add("- WLD regions: $bt$(($regions | Measure-Object).Count)$bt")
$md.Add("- Chest/key/reward/shop/container-like TNG entities: $bt$(($entities | Measure-Object).Count)$bt")
$md.Add("")

$md.Add("## Entity Categories")
$md.Add("")
$md.Add("| Category | Count |")
$md.Add("|---|---:|")
foreach ($group in ($entities | Group-Object category | Sort-Object Count -Descending)) {
    $md.Add("| $bt$($group.Name)$bt | $($group.Count) |")
}
$md.Add("")

$md.Add("## Top Definitions")
$md.Add("")
$md.Add("| DefinitionType | Count |")
$md.Add("|---|---:|")
foreach ($group in ($entities | Group-Object definition_type | Sort-Object Count -Descending | Select-Object -First 30)) {
    $name = if ([string]::IsNullOrWhiteSpace($group.Name)) { "(blank)" } else { $group.Name }
    $md.Add("| $bt$name$bt | $($group.Count) |")
}
$md.Add("")

$md.Add("## Densest Regions")
$md.Add("")
$md.Add("| Region | Display | Contains | Sees |")
$md.Add("|---|---|---:|---:|")
foreach ($row in ($regions | Sort-Object { [int]$_.contains_count } -Descending | Select-Object -First 20)) {
    $md.Add("| $bt$($row.region_name)$bt | $bt$($row.display_name)$bt | $($row.contains_count) | $($row.sees_count) |")
}
$md.Add("")

$md.Add("## First Editor Targets")
$md.Add("")
$md.Add("| File | Line | Category | Entity | DefinitionType | UID | X | Y | Z |")
$md.Add("|---|---:|---|---|---|---|---:|---:|---:|")
foreach ($row in ($entities | Where-Object { $_.category -in @("chest", "silver_key", "reward", "key") } | Select-Object -First 30)) {
    $md.Add("| $bt$($row.file)$bt | $($row.line) | $bt$($row.category)$bt | $bt$($row.entity_type)$bt | $bt$($row.definition_type)$bt | $bt$($row.uid)$bt | $($row.x) | $($row.y) | $($row.z) |")
}
$md.Add("")
$md.Add("## Editor Implications")
$md.Add("")
$md.Add("- `wld_maps.csv` gives world-map coordinates and level script names for the scene browser.")
$md.Add("- `wld_regions.csv` gives region grouping and visibility edges for map navigation and streaming/load previews.")
$md.Add("- `chest_key_reward_entities.csv` is the first gameplay-object index for chest/key reward editing and validation.")
$md.Add("- FableWin symbols around `CScriptThing::MsgOpenedChest`, inventory, rewards, and definitions should be inspected against these fixtures.")

$md | Set-Content -LiteralPath $OutputPath -Encoding UTF8
"report=$OutputPath"
