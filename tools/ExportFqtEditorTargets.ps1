param(
    [string]$InputPath = "ghidra_out/fablewin_pdb_names.tsv",
    [string]$OutputTsv = "ghidra_out/fablewin_fqt_editor_targets.tsv",
    [string]$OutputMd = "docs/FQT_EDITOR_TARGETS.md"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$groups = [ordered]@{
    "level_world_map" = @(
        "SaveLevel@CEditControlCentre",
        "SaveAllLevels@CEditControlCentre",
        "CreateAndSaveNewLevel@CEditControlCentre",
        "LoadSingleLevelFromFiles@CEditWorldMap",
        "SaveSingleLevel@CEditWorldMap",
        "SaveToFile@CEditWorldMap",
        "LoadFromFile@CEditWorldMap",
        "CreateNewRegion@CEditWorldMap",
        "DeleteRegion@CEditWorldMap",
        "EditSaveRegionsToString@CEditWorldMap",
        "GetRegion@CEditWorldMap",
        "SetMapPos@CEditWorldMap"
    )
    "tng_lev_merge" = @(
        "LoadLevFile@CEditLevelMerger",
        "ParseTngFile@CEditLevelMerger",
        "GetConflicts@CEditLevelMerger",
        "ResolveNextFile@CEditLevelMerger",
        "CConflict@CEditLevelMerger",
        "CThingText@CEditLevelMerger"
    )
    "region_nav_mapwho" = @(
        "ConsoleGenerateRegionGraph",
        "ConsoleLoadRegionGraphData",
        "ConsoleSetDrawNavigationMeshes",
        "ConsoleSetDrawMapwhoInfo",
        "RegionGraph",
        "NavigationMesh",
        "Mapwho"
    )
    "definitions" = @(
        "SetCompiledDefDirectory",
        "SetUsingCompiledDefs",
        "SetCreatingCompiledDefs",
        "ValidateDefinitions",
        "InitAndCompile@CGameDefinitionManager",
        "CGameDefinitionManager",
        "CDefinitionManager"
    )
    "chests_items_rewards" = @(
        "MsgOpenedChest",
        "OpenChest",
        "Chest",
        "SilverKey",
        "Inventory",
        "QuestItem",
        "Reward"
    )
    "archives_formats" = @(
        "FinalAlbion",
        "WAD",
        "LEV",
        "TNG",
        "LoadLev",
        "ParseTng",
        "SaveTng"
    )
}

if (!(Test-Path -LiteralPath $InputPath)) {
    throw "Input not found: $InputPath"
}

$rows = New-Object System.Collections.Generic.List[object]
Get-Content -LiteralPath $InputPath | ForEach-Object {
    if ([string]::IsNullOrWhiteSpace($_) -or $_.StartsWith("#")) {
        return
    }

    $parts = $_ -split "`t", 2
    if ($parts.Count -ne 2) {
        return
    }

    $addr = $parts[0].Trim()
    $name = $parts[1].Trim()
    foreach ($group in $groups.Keys) {
        foreach ($pattern in $groups[$group]) {
            if ($name -match $pattern) {
                $rows.Add([pscustomobject]@{
                    category = $group
                    address = $addr
                    name = $name
                })
                return
            }
        }
    }
}

$tsv = New-Object System.Collections.Generic.List[string]
$tsv.Add("category`taddress`tname")
$rows |
    Sort-Object category, address, name -Unique |
    ForEach-Object { $tsv.Add("$($_.category)`t$($_.address)`t$($_.name)") }
$tsv | Set-Content -LiteralPath $OutputTsv -Encoding UTF8

$md = New-Object System.Collections.Generic.List[string]
$md.Add("# FQT Editor Targets")
$md.Add("")
$md.Add("*Generated from `$InputPath` on $(Get-Date -Format yyyy-MM-dd).*".Replace('$InputPath', $InputPath))
$md.Add("")
$md.Add("These are the first FableWin editor/debug symbols to inspect after the donor import is saved and PDB names are applied.")
$md.Add("")

foreach ($group in $groups.Keys) {
    $items = @($rows | Where-Object { $_.category -eq $group } | Sort-Object address, name -Unique)
    $md.Add("## $group")
    $md.Add("")
    $md.Add("Count: $($items.Count)")
    $md.Add("")
    $md.Add("| Address | Symbol |")
    $md.Add("|---|---|")
    foreach ($item in ($items | Select-Object -First 80)) {
        $symbol = $item.name.Replace("|", "\|")
        $md.Add("| $($item.address) | ``$symbol`` |")
    }
    if ($items.Count -gt 80) {
        $md.Add("")
        $md.Add("Showing first 80 of $($items.Count); full list is in ``$OutputTsv``.".Replace('$OutputTsv', $OutputTsv))
    }
    $md.Add("")
}

$md | Set-Content -LiteralPath $OutputMd -Encoding UTF8

"targets=$($rows.Count)"
"tsv=$OutputTsv"
"md=$OutputMd"
