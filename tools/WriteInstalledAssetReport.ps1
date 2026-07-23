param(
    [string]$IndexDir = "ghidra_out/installed_game",
    [string]$OutputPath = "docs/INSTALLED_GAME_ASSET_REPORT.md"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$summaryPath = Join-Path $IndexDir "summary.txt"
$extPath = Join-Path $IndexDir "extensions.csv"
$banksPath = Join-Path $IndexDir "big_banks.csv"
$typesPath = Join-Path $IndexDir "big_entry_types.csv"
$wadPath = Join-Path $IndexDir "finalalbion_wad_entries.csv"
$stbPath = Join-Path $IndexDir "finalalbion_stb_entries.csv"
$tngPath = Join-Path $IndexDir "loose_tng_summary.csv"

foreach ($path in @($summaryPath, $extPath, $banksPath, $typesPath, $wadPath, $tngPath)) {
    if (!(Test-Path -LiteralPath $path)) {
        throw "Missing input: $path"
    }
}

$summary = [ordered]@{}
Get-Content -LiteralPath $summaryPath | ForEach-Object {
    $idx = $_.IndexOf("=")
    if ($idx -gt 0) {
        $summary[$_.Substring(0, $idx)] = $_.Substring($idx + 1)
    }
}

$extensions = Import-Csv -LiteralPath $extPath
$banks = Import-Csv -LiteralPath $banksPath
$types = Import-Csv -LiteralPath $typesPath
$wad = Import-Csv -LiteralPath $wadPath
$stb = if (Test-Path -LiteralPath $stbPath) { Import-Csv -LiteralPath $stbPath } else { @() }
$tng = Import-Csv -LiteralPath $tngPath

$md = New-Object System.Collections.Generic.List[string]
$bt = [char]96

$graphicsTypes = @{
    "1" = "Static Mesh"; "2" = "Repeated Mesh"; "3" = "Physics (BBM)"; "4" = "Particle Mesh";
    "5" = "Animated Mesh"; "6" = "Animation"; "7" = "Delta Animation"; "8" = "Lipsync Animation";
    "9" = "Partial Animation"; "10" = "Relative Animation"
}
$textureTypes = @{
    "0" = "Graphic Single"; "1" = "Graphic Sequence"; "2" = "Bumpmap"; "3" = "Bumpmap Sequence";
    "4" = "Volume Texture"; "5" = "Sprite Sheet"
}
$textTypes = @{ "0" = "Text Entry"; "1" = "Group Text Entry"; "2" = "Narrator List" }
$md.Add("# Installed Game Asset Report")
$md.Add("")
$md.Add("*Generated from installed game indexes on $(Get-Date -Format yyyy-MM-dd).*")
$md.Add("")
$md.Add("## Install")
$md.Add("")
$md.Add("- Game root: $bt$($summary.game_root)$bt")
$md.Add("- Fable.exe present: $bt$($summary.fable_exe)$bt")
$md.Add("- FSE installed: $bt$($summary.fse_dll)$bt / folder $bt$($summary.fse_folder)$bt")
$md.Add("- FinalAlbion WAD present: $bt$($summary.finalalbion_wad)$bt")
$md.Add("- Loose TNGs: $bt$($summary.loose_tng_count)$bt")
$md.Add("- Loose LEVs: $bt$($summary.loose_lev_count)$bt")
$md.Add("")

$md.Add("## File Types")
$md.Add("")
$md.Add("| Extension | Count | Bytes |")
$md.Add("|---|---:|---:|")
foreach ($row in ($extensions | Select-Object -First 20)) {
    $md.Add("| $bt$($row.extension)$bt | $($row.count) | $($row.bytes) |")
}
$md.Add("")

$md.Add("## BIG Banks")
$md.Add("")
$md.Add("| Bank | Entries | Bytes |")
$md.Add("|---|---:|---:|")
foreach ($row in $banks) {
    $md.Add("| $bt$($row.bank)$bt | $($row.entries) | $($row.bytes) |")
}
$md.Add("")

$md.Add("## BIG Entry Types")
$md.Add("")
$md.Add("Names below are from EgoCore's `GetEntryTypeName` mapping where the bank context is unambiguous.")
$md.Add("")
$md.Add("| Type | Likely Meaning | Entries | Bytes |")
$md.Add("|---|---|---:|---:|")
foreach ($row in $types) {
    $typeName = @()
    if ($graphicsTypes.ContainsKey($row.type)) { $typeName += "graphics: $($graphicsTypes[$row.type])" }
    if ($textureTypes.ContainsKey($row.type)) { $typeName += "textures/frontend: $($textureTypes[$row.type])" }
    if ($textTypes.ContainsKey($row.type)) { $typeName += "text: $($textTypes[$row.type])" }
    if ($row.type -eq "0") { $typeName += "effects: Particle Entry; shaders: Vertex Shader; fonts: PC Font" }
    if ($row.type -eq "1") { $typeName += "dialogue: Lipsync Entry; shaders: Pixel Shader; fonts: Xbox Font" }
    if ($row.type -eq "2") { $typeName += "fonts: GlyphData" }
    if ($typeName.Count -eq 0) { $typeName += "bank-specific type $($row.type)" }
    $md.Add("| $bt$($row.type)$bt | $($typeName -join "; ") | $($row.entries) | $($row.bytes) |")
}
$md.Add("")

$md.Add("## FinalAlbion WAD")
$md.Add("")
$md.Add("| Extension | Count | Bytes |")
$md.Add("|---|---:|---:|")
foreach ($group in ($wad | Group-Object extension | Sort-Object Name)) {
    $bytes = ($group.Group | Measure-Object size -Sum).Sum
    $md.Add("| $bt$($group.Name)$bt | $($group.Count) | $bytes |")
}
$md.Add("")

if ($stb.Count -gt 0) {
    $md.Add("## FinalAlbion RT STB")
    $md.Add("")
    $md.Add("| Extension | Count | Bytes |")
    $md.Add("|---|---:|---:|")
    foreach ($group in ($stb | Group-Object extension | Sort-Object Name)) {
        $bytes = ($group.Group | Measure-Object size -Sum).Sum
        $name = if ([string]::IsNullOrWhiteSpace($group.Name)) { "(none)" } else { $group.Name }
        $md.Add("| $bt$name$bt | $($group.Count) | $bytes |")
    }
    $md.Add("")
}

$md.Add("## Largest Loose TNGs")
$md.Add("")
$md.Add("| File | Things | ScriptNames | Size |")
$md.Add("|---|---:|---:|---:|")
foreach ($row in ($tng | Sort-Object { [int]$_.new_things } -Descending | Select-Object -First 20)) {
    $md.Add("| $bt$($row.file)$bt | $($row.new_things) | $($row.script_names) | $($row.size) |")
}
$md.Add("")

$totalThings = 0
$totalScriptNames = 0
foreach ($row in $tng) {
    $totalThings += [int]$row.new_things
    $totalScriptNames += [int]$row.script_names
}
$md.Add("Loose TNG total: $bt$totalThings$bt things and $bt$totalScriptNames$bt ScriptName entries.")
$md.Add("")

$md.Add("## Editor Implications")
$md.Add("")
$md.Add("- WAD-backed LEV data is the missing level/terrain half; loose TNG data is already present for entity discovery.")
$md.Add("- BIG indexes provide the asset browser backbone: meshes, textures, animations, text, dialogue, fonts, effects, and shaders.")
$md.Add("- TNG summaries identify high-density regions that should become early stress tests for editor performance.")
$md.Add("- EgoCore should remain the read/compile oracle for BIG and resource banks.")
$md.Add("- SilverChest bridges should provide safe WAD/TNG/LEV operations until native behavior is validated against FableWin.")

$md | Set-Content -LiteralPath $OutputPath -Encoding UTF8

"report=$OutputPath"
