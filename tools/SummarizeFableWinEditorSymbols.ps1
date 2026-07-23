param(
    [string]$PdbNames = "ghidra_out\fablewin_pdb_names.tsv",
    [string]$OutMarkdown = "ghidra_out\fablewin_editor_symbols.md",
    [string]$OutTsv = "ghidra_out\fablewin_editor_symbols.tsv"
)

$ErrorActionPreference = "Stop"

$Root = "D:\Documents\FableTLC"
Set-Location $Root

if (-not (Test-Path $PdbNames)) {
    throw "Missing parsed PDB names TSV: $PdbNames"
}

$categories = [ordered]@{
    "editor_classes" = "CEdit|Editor|EditBox|GConsoleSetEditor|IsEditorActive|SetEditor"
    "level_region" = "Region|Level|Map|MiniMap|StaticMap|Mapwho|RegionGraph"
    "resource_defs" = "Definition|CompiledDef|CDef|DefManager|ResourceDirectory|DefDir|GameDefinition"
    "terrain_landscape" = "Terrain|Landscape|Height|EHF|Ground|NavMesh|NavigationMesh"
    "objects_placement" = "Thing|ScriptThing|Marker|Building|Object|Placement|Position|Transform"
    "chest_keys_items" = "Chest|SilverKey|Silver|Inventory|Gift|Chocolate|Chocolates|Item"
    "archives_formats" = "BIG|BIN|BNK|WAD|LEV|TNG|STB|GDB|BBB|BBBEntry|Bnk|Archive"
    "console_debug" = "Console|Debug|Draw|Reload|Generate|Validate|Dump|Trace"
}

$rows = New-Object System.Collections.Generic.List[object]
$classCounts = @{}

Get-Content $PdbNames | ForEach-Object {
    if ([string]::IsNullOrWhiteSpace($_) -or $_.StartsWith("#")) { return }
    $cols = $_ -split "`t"
    if ($cols.Count -lt 2) { return }
    $addr = $cols[0]
    $name = $cols[1]

    foreach ($cat in $categories.Keys) {
        if ($name -match $categories[$cat]) {
            $rows.Add([pscustomobject]@{
                Category = $cat
                Address = $addr
                Name = $name
            })
        }
    }

    $m = [regex]::Match($name, "\?[^@]*@(?<class>[^@?][^@]*)@@")
    if ($m.Success) {
        $class = $m.Groups["class"].Value
        if ($class -match "^(CEdit|CEditor|CLevel|CRegion|CMap|CGameDefinition|CDefinition|CDef|CThing|CScriptThing|CChest|CInventory|CResource)") {
            if (-not $classCounts.ContainsKey($class)) { $classCounts[$class] = 0 }
            $classCounts[$class]++
        }
    }
}

$rows = $rows | Sort-Object Category,Address,Name -Unique

$tsv = New-Object System.Collections.Generic.List[string]
$tsv.Add("#category`taddress`tname")
foreach ($r in $rows) {
    $tsv.Add("$($r.Category)`t$($r.Address)`t$($r.Name)")
}
$tsv | Set-Content -Path $OutTsv -Encoding UTF8

$md = New-Object System.Collections.Generic.List[string]
$md.Add("# FableWin editor/tooling symbol survey")
$md.Add("")
$md.Add("Generated: $(Get-Date -Format s)")
$md.Add(("Source: {0}" -f $PdbNames))
$md.Add("")
$md.Add("## Category counts")
$md.Add("")
$md.Add("| Category | Matches |")
$md.Add("|---|---:|")
foreach ($cat in $categories.Keys) {
    $count = ($rows | Where-Object { $_.Category -eq $cat } | Measure-Object).Count
    $md.Add("| $cat | $count |")
}

$md.Add("")
$md.Add("## Top class prefixes")
$md.Add("")
$md.Add("| Class | Count |")
$md.Add("|---|---:|")
$classCounts.GetEnumerator() |
    Sort-Object Value -Descending |
    Select-Object -First 40 |
    ForEach-Object { $md.Add("| $($_.Key) | $($_.Value) |") }

$md.Add("")
$md.Add("## Representative symbols")
foreach ($cat in $categories.Keys) {
    $md.Add("")
    $md.Add("### $cat")
    $sample = $rows | Where-Object { $_.Category -eq $cat } | Select-Object -First 30
    if (-not $sample) {
        $md.Add("")
        $md.Add("No matches.")
        continue
    }
    $md.Add("")
    $md.Add("| Address | Name |")
    $md.Add("|---|---|")
    foreach ($r in $sample) {
        $safeName = $r.Name.Replace("|", "\|")
        $md.Add("| $($r.Address) | $safeName |")
    }
}

$md | Set-Content -Path $OutMarkdown -Encoding UTF8

Write-Host "Wrote $OutMarkdown"
Write-Host "Wrote $OutTsv"
Write-Host "Matches: $($rows.Count)"
