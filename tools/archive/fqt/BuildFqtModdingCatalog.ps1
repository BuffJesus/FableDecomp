param(
    [string]$IndexDir = "ghidra_out/installed_game",
    [string]$OutputPath = "ghidra_out/installed_game/fqt_modding_catalog.json"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

function CountCsv([string]$Path) {
    if (!(Test-Path -LiteralPath $Path)) { return 0 }
    return ((Import-Csv -LiteralPath $Path) | Measure-Object).Count
}

function TopGroups([object[]]$Rows, [string]$Property, [int]$Limit) {
    return @(
        $Rows |
            Group-Object $Property |
            Sort-Object Count -Descending |
            Select-Object -First $Limit |
            ForEach-Object { [ordered]@{ name = $_.Name; count = $_.Count } }
    )
}

$summaryPath = Join-Path $IndexDir "summary.txt"
$summary = [ordered]@{}
if (Test-Path -LiteralPath $summaryPath) {
    Get-Content -LiteralPath $summaryPath | ForEach-Object {
        $idx = $_.IndexOf("=")
        if ($idx -gt 0) {
            $summary[$_.Substring(0, $idx)] = $_.Substring($idx + 1)
        }
    }
}

$banks = Import-Csv -LiteralPath (Join-Path $IndexDir "big_banks.csv")
$types = Import-Csv -LiteralPath (Join-Path $IndexDir "big_entry_types.csv")
$maps = Import-Csv -LiteralPath (Join-Path $IndexDir "wld_maps.csv")
$regions = Import-Csv -LiteralPath (Join-Path $IndexDir "wld_regions.csv")
$entities = Import-Csv -LiteralPath (Join-Path $IndexDir "chest_key_reward_entities.csv")
$tng = Import-Csv -LiteralPath (Join-Path $IndexDir "loose_tng_summary.csv")

$catalog = [ordered]@{
    generated_on = (Get-Date -Format "yyyy-MM-ddTHH:mm:ssK")
    game_root = $summary.game_root
    inputs = [ordered]@{
        big_entries = "ghidra_out/installed_game/big_entries.csv"
        big_banks = "ghidra_out/installed_game/big_banks.csv"
        big_entry_types = "ghidra_out/installed_game/big_entry_types.csv"
        finalalbion_wad_entries = "ghidra_out/installed_game/finalalbion_wad_entries.csv"
        finalalbion_stb_entries = "ghidra_out/installed_game/finalalbion_stb_entries.csv"
        loose_tng_summary = "ghidra_out/installed_game/loose_tng_summary.csv"
        wld_maps = "ghidra_out/installed_game/wld_maps.csv"
        wld_regions = "ghidra_out/installed_game/wld_regions.csv"
        chest_key_reward_entities = "ghidra_out/installed_game/chest_key_reward_entities.csv"
        level_container_consistency = "ghidra_out/installed_game/level_container_consistency.csv"
        header_enums = "ghidra_out/installed_game/header_enums.csv"
        header_enum_counts = "ghidra_out/installed_game/header_enum_counts.csv"
        compiled_game_entries = "ghidra_out/installed_game/compiled_game_entries.csv"
        compiled_script_entries = "ghidra_out/installed_game/compiled_script_entries.csv"
        compiled_frontend_entries = "ghidra_out/installed_game/compiled_frontend_entries.csv"
        gameplay_entity_definition_validation = "ghidra_out/installed_game/gameplay_entity_definition_validation.csv"
        fablewin_editor_targets = "ghidra_out/fablewin_fqt_editor_targets.tsv"
        native_gameplay_targets = "ghidra_out/native_gameplay_targets.tsv"
    }
    counts = [ordered]@{
        big_banks = CountCsv (Join-Path $IndexDir "big_banks.csv")
        big_entries = CountCsv (Join-Path $IndexDir "big_entries.csv")
        wad_entries = CountCsv (Join-Path $IndexDir "finalalbion_wad_entries.csv")
        stb_entries = CountCsv (Join-Path $IndexDir "finalalbion_stb_entries.csv")
        loose_tng_files = CountCsv (Join-Path $IndexDir "loose_tng_summary.csv")
        wld_maps = CountCsv (Join-Path $IndexDir "wld_maps.csv")
        wld_regions = CountCsv (Join-Path $IndexDir "wld_regions.csv")
        chest_key_reward_entities = CountCsv (Join-Path $IndexDir "chest_key_reward_entities.csv")
        level_container_consistency_rows = CountCsv (Join-Path $IndexDir "level_container_consistency.csv")
        header_enum_constants = CountCsv (Join-Path $IndexDir "header_enums.csv")
        compiled_game_entries = CountCsv (Join-Path $IndexDir "compiled_game_entries.csv")
        compiled_script_entries = CountCsv (Join-Path $IndexDir "compiled_script_entries.csv")
        compiled_frontend_entries = CountCsv (Join-Path $IndexDir "compiled_frontend_entries.csv")
        gameplay_entity_definition_validation_rows = CountCsv (Join-Path $IndexDir "gameplay_entity_definition_validation.csv")
        native_gameplay_targets = if (Test-Path "ghidra_out/native_gameplay_targets.tsv") { ((Import-Csv -LiteralPath "ghidra_out/native_gameplay_targets.tsv" -Delimiter "`t") | Measure-Object).Count } else { 0 }
    }
    top = [ordered]@{
        big_banks = @($banks | Sort-Object { [int64]$_.bytes } -Descending | Select-Object -First 8 bank,entries,bytes)
        big_entry_types = @($types | Select-Object type,entries,bytes)
        densest_tngs = @($tng | Sort-Object { [int]$_.new_things } -Descending | Select-Object -First 20 file,new_things,script_names,size)
        entity_categories = TopGroups $entities "category" 20
        entity_definition_types = TopGroups $entities "definition_type" 40
        densest_regions = @($regions | Sort-Object { [int]$_.contains_count } -Descending | Select-Object -First 20 id,region_name,display_name,contains_count,sees_count)
    }
    editor_modules = @(
        [ordered]@{ id = "world_browser"; sources = @("wld_maps", "wld_regions", "FinalAlbion.wad"); first_tasks = @("map grid", "region visibility", "level open routing") }
        [ordered]@{ id = "level_scene"; sources = @("LEV from WAD", "loose TNG", "SilverChest.LevBridge", "SilverChest.TngBridge"); first_tasks = @("terrain theme grid", "entity list", "placement editing") }
        [ordered]@{ id = "asset_browser"; sources = @("EgoCore BIG indexes", "SilverChest.Modern"); first_tasks = @("mesh/texture/text/dialogue filters", "preview extraction", "mod package staging") }
        [ordered]@{ id = "chest_reward_editor"; sources = @("chest_key_reward_entities", "FableWin CScriptThing/opened-chest symbols"); first_tasks = @("chest list", "reward properties", "runtime validation") }
        [ordered]@{ id = "decompilation_oracle"; sources = @("FableWin PDB", "ego_r PDB", "FSE hooks"); first_tasks = @("name porting", "format behavior validation", "script/native API reconciliation") }
    )
}

$json = $catalog | ConvertTo-Json -Depth 8
$json | Set-Content -LiteralPath $OutputPath -Encoding UTF8
"catalog=$OutputPath"
