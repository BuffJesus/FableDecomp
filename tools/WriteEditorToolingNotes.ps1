param(
    [string]$PdbNames = "ghidra_out\fablewin_pdb_names.tsv",
    [string]$OutPath = "docs\EDITOR_TOOLING_NOTES.md"
)

$ErrorActionPreference = "Stop"

$Root = "D:\Documents\FableTLC"
Set-Location $Root

$queries = [ordered]@{
    "Editor activation and mode switching" = @(
        "GConsoleSetEditor",
        "IsEditorActive",
        "SetAsEditingLevel",
        "SetAsEditingWorld",
        "CEditComponent"
    )
    "Level save/load and world map editing" = @(
        "SaveLevel",
        "SaveAllLevels",
        "SaveSingleLevel",
        "LoadSingleLevel",
        "EditLoadLevel",
        "CreateAndSaveNewLevel",
        "CEditWorldMap"
    )
    "TNG/LEV merge and conflict tooling" = @(
        "CEditLevelMerger",
        "ParseTngFile",
        "LoadLevFile",
        "ResolveNextFile",
        "GetConflicts",
        "CThingText"
    )
    "Region graph, mapwho, navigation" = @(
        "ConsoleGenerateRegionGraph",
        "ConsoleLoadRegionGraphData",
        "ConsoleSetDrawNavigationMeshes",
        "ConsoleSetDrawMapwhoInfo",
        "GetNavNodeLevel",
        "Navigation"
    )
    "Definitions and compiled defs" = @(
        "SetCompiledDefDirectory",
        "SetUsingCompiledDefs",
        "SetCreatingCompiledDefs",
        "ValidateDefinitions",
        "InitDefinitions",
        "CGameDefinitionManager"
    )
    "Chests, silver keys, inventory rewards" = @(
        "MsgOpenedChest",
        "Chest",
        "SilverKey",
        "Inventory",
        "Chocolate",
        "Chocolates"
    )
}

$all = Get-Content $PdbNames | Where-Object { $_ -and -not $_.StartsWith("#") }

function Find-Matches($terms) {
    $items = New-Object System.Collections.Generic.List[object]
    foreach ($line in $all) {
        $cols = $line -split "`t"
        if ($cols.Count -lt 2) { continue }
        $name = $cols[1]
        foreach ($term in $terms) {
            if ($name -match [regex]::Escape($term)) {
                $items.Add([pscustomobject]@{ Address = $cols[0]; Name = $name; Term = $term })
                break
            }
        }
    }
    $items | Sort-Object Address,Name -Unique
}

$md = New-Object System.Collections.Generic.List[string]
$md.Add("# Editor/tooling notes from FableWin symbols")
$md.Add("")
$md.Add("Generated: $(Get-Date -Format s)")
$md.Add("Source: $PdbNames")
$md.Add("")
$md.Add("## Reading")
$md.Add("")
$md.Add("`FableWin.exe` appears to include Lionhead's internal editor/runtime tool surface, not just")
$md.Add("a debug build of the retail game. These names are useful in two ways:")
$md.Add("")
$md.Add("- They can be ported through BSim to name matching retail `Fable.exe` engine functions.")
$md.Add("- Editor-only code documents the native semantics behind community tools such as ChocolateBox")
$md.Add("  and the local FQT/SilverChest WAD/TNG pipeline.")
$md.Add("")
$md.Add("ChocolateBox is still best treated as the community GUI/front-end lineage; the FableWin symbols")
$md.Add("are lower-level engine/editor ground truth for level save/load, TNG parsing, region graph,")
$md.Add("definitions, and object/chest semantics.")
$md.Add("")

foreach ($section in $queries.Keys) {
    $items = @(Find-Matches $queries[$section])
    $md.Add("## $section")
    $md.Add("")
    $md.Add("Matches: $($items.Count)")
    $md.Add("")
    $md.Add("| Address | Symbol |")
    $md.Add("|---|---|")
    foreach ($m in ($items | Select-Object -First 40)) {
        $safe = $m.Name.Replace("|", "\|")
        $md.Add("| $($m.Address) | $safe |")
    }
    if ($items.Count -gt 40) {
        $md.Add("| ... | $($items.Count - 40) more omitted |")
    }
    $md.Add("")
}

$md.Add("## Local tool overlap")
$md.Add("")
$md.Add("- FQT calls `SilverChest.WadBridge.exe` via `FableQuestTool/Services/WadBridgeClient.cs` to")
$md.Add("  list/extract `FinalAlbion.wad` content and feed TNG scanning.")
$md.Add("- FQT currently notes that vanilla TNG data is stored in LEV/WAD paths and that extraction")
$md.Add("  through LEV structures is incomplete. FableWin editor symbols identify native functions")
$md.Add("  around `CEditWorldMap`, `CEditLevelMerger::LoadLevFile`, and `ParseTngFile` that can guide")
$md.Add("  a better extractor/writer.")
$md.Add("- ChocolateBox maps to the same domain at the tool level: region editing, lev/tng edits,")
$md.Add("  FinalAlbion WAD/loose level workflows, and definition metadata. The PDB names give us the")
$md.Add("  engine/editor side of those workflows.")

$md | Set-Content -Path $OutPath -Encoding UTF8
Write-Host "Wrote $OutPath"
