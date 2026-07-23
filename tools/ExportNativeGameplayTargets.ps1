param(
    [string]$PdbNamesPath = "ghidra_out/fablewin_pdb_names.tsv",
    [string]$OutputPath = "ghidra_out/native_gameplay_targets.tsv",
    [string]$ReportPath = "docs/NATIVE_GAMEPLAY_TARGETS.md"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$patterns = [ordered]@{
    chest = "Chest|CChest|OpenedChest"
    inventory = "Inventory|Inventor|GiveHeroObject|Item"
    reward = "Reward|Treasure|Augment|Gift"
    definition = "Definition|CDef|DefManager|CompiledDef"
    script_thing = "CScriptThing|ScriptThing|Thing"
    shop = "Shop|Trader|Vendor"
}

$rows = New-Object System.Collections.Generic.List[object]
foreach ($line in Get-Content -LiteralPath $PdbNamesPath) {
    $parts = $line -split "`t", 2
    if ($parts.Count -lt 2) { continue }
    $addr = $parts[0]
    $name = $parts[1]
    foreach ($key in $patterns.Keys) {
        if ($name -match $patterns[$key]) {
            $rows.Add([pscustomobject]@{ category=$key; address=$addr; name=$name })
            break
        }
    }
}

$rows | Export-Csv -LiteralPath $OutputPath -NoTypeInformation -Delimiter "`t"

$bt = [char]96
$md = New-Object System.Collections.Generic.List[string]
$md.Add("# Native Gameplay Targets")
$md.Add("")
$md.Add("*Generated from " + $bt + "fablewin_pdb_names.tsv" + $bt + " on $(Get-Date -Format yyyy-MM-dd).*")
$md.Add("")
$md.Add("## Counts")
$md.Add("")
$md.Add("| Category | Symbols |")
$md.Add("|---|---:|")
foreach ($group in ($rows | Group-Object category | Sort-Object Count -Descending)) {
    $md.Add("| $bt$($group.Name)$bt | $($group.Count) |")
}
$md.Add("")
$md.Add("## First Targets")
$md.Add("")
$md.Add("| Category | Address | Symbol |")
$md.Add("|---|---:|---|")
foreach ($row in ($rows | Sort-Object category,name | Select-Object -First 80)) {
    $md.Add("| $bt$($row.category)$bt | $bt$($row.address)$bt | $bt$($row.name)$bt |")
}
$md.Add("")
$md.Add("## Use")
$md.Add("")
$md.Add("- Inspect these after the current BSim pass settles and names are demangled in retail `Fable.exe`.")
$md.Add("- Start with chest/reward symbols and compare behavior against `gameplay_entity_definition_validation.csv`.")
$md.Add("- Definition-manager symbols should drive compiled-def validation and object picker correctness.")

$md | Set-Content -LiteralPath $ReportPath -Encoding UTF8
"targets=$($rows.Count) report=$ReportPath"
