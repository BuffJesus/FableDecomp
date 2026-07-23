param(
    [string]$GameRoot = "C:\Programs\Steam\steamapps\common\Fable The Lost Chapters",
    [string]$OutputDir = "ghidra_out/installed_game"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

function Unquote([string]$Value) {
    $v = $Value.Trim()
    if ($v.Length -ge 2 -and $v.StartsWith('"') -and $v.EndsWith('"')) {
        return $v.Substring(1, $v.Length - 2)
    }
    return $v
}

function CsvSafe([string]$Value) {
    if ($null -eq $Value) { return '""' }
    return '"' + $Value.Replace('"', '""') + '"'
}

function GetEntityCategory([string]$Text) {
    if ($Text -match '(?i)silver.*key|silverkey') { return "silver_key" }
    if ($Text -match '(?i)chest') { return "chest" }
    if ($Text -match '(?i)reward|treasure|augment') { return "reward" }
    if ($Text -match '(?i)shop|trader') { return "shop_trader" }
    if ($Text -match '(?i)key') { return "key" }
    if ($Text -match '(?i)container') { return "container" }
    return "other"
}

$wldPath = Join-Path $GameRoot "data\Levels\FinalAlbion.wld"
if (Test-Path -LiteralPath $wldPath) {
    $maps = New-Object System.Collections.Generic.List[object]
    $regions = New-Object System.Collections.Generic.List[object]
    $current = $null
    $state = "basic"

    foreach ($raw in Get-Content -LiteralPath $wldPath) {
        $line = ($raw -replace "//.*$", "").Trim()
        if ($line.Length -eq 0) { continue }
        if ($line.EndsWith(";")) { $line = $line.Substring(0, $line.Length - 1).Trim() }
        if ($line.Length -eq 0) { continue }

        if ($line -match '^NewMap\s+(\d+)$') {
            $current = [ordered]@{ id=$Matches[1]; x=""; y=""; level_name=""; level_script_name=""; uid=""; is_sea=""; loaded_on_proximity="" }
            $state = "map"
            continue
        }
        if ($line -ieq "EndMap") {
            if ($current) { $maps.Add([pscustomobject]$current) }
            $current = $null
            $state = "basic"
            continue
        }
        if ($line -match '^NewRegion\s+(\d+)$') {
            $current = [ordered]@{
                id=$Matches[1]; region_name=""; display_name=""; region_def=""; minimap="";
                minimap_scale=""; appear_on_world_map=""; contains_maps=New-Object System.Collections.Generic.List[string];
                sees_maps=New-Object System.Collections.Generic.List[string]
            }
            $state = "region"
            continue
        }
        if ($line -ieq "EndRegion") {
            if ($current) {
                $regions.Add([pscustomobject]@{
                    id=$current.id
                    region_name=$current.region_name
                    display_name=$current.display_name
                    region_def=$current.region_def
                    minimap=$current.minimap
                    minimap_scale=$current.minimap_scale
                    appear_on_world_map=$current.appear_on_world_map
                    contains_count=$current.contains_maps.Count
                    sees_count=$current.sees_maps.Count
                    contains_maps=($current.contains_maps -join ";")
                    sees_maps=($current.sees_maps -join ";")
                })
            }
            $current = $null
            $state = "basic"
            continue
        }

        if ($state -eq "map" -and $current -and $line -match '^(\S+)\s+(.+)$') {
            $key = $Matches[1]
            $value = Unquote $Matches[2]
            switch -Regex ($key) {
                '^MapX$' { $current.x = $value }
                '^MapY$' { $current.y = $value }
                '^LevelName$' { $current.level_name = $value }
                '^LevelScriptName$' { $current.level_script_name = $value }
                '^MapUID$' { $current.uid = $value }
                '^IsSea$' { $current.is_sea = $value }
                '^LoadedOnPlayerProximity$' { $current.loaded_on_proximity = $value }
            }
            continue
        }

        if ($state -eq "region" -and $current -and $line -match '^(\S+)\s+(.+)$') {
            $key = $Matches[1]
            $value = Unquote $Matches[2]
            switch -Regex ($key) {
                '^RegionName$' { $current.region_name = $value }
                '^NewDisplayName$' { $current.display_name = $value }
                '^RegionDef$' { $current.region_def = $value }
                '^MiniMapGraphic$' { $current.minimap = $value }
                '^MiniMapScale$' { $current.minimap_scale = $value }
                '^AppearOnWorldMap$' { $current.appear_on_world_map = $value }
                '^ContainsMap$' { $current.contains_maps.Add($value) }
                '^SeesMap$' { $current.sees_maps.Add($value) }
            }
        }
    }

    $maps | Export-Csv -LiteralPath (Join-Path $OutputDir "wld_maps.csv") -NoTypeInformation
    $regions | Export-Csv -LiteralPath (Join-Path $OutputDir "wld_regions.csv") -NoTypeInformation
}

$tngRoot = Join-Path $GameRoot "data\Levels\FinalAlbion"
$entityRows = New-Object System.Collections.Generic.List[string]
$entityRows.Add("file,line,category,entity_type,script_name,definition_type,uid,x,y,z,matched_text")
$patterns = "(?i)(chest|silver|key|reward|augment|treasure|shop|trader|container)"

if (Test-Path -LiteralPath $tngRoot) {
    foreach ($file in Get-ChildItem -LiteralPath $tngRoot -Recurse -File -Filter *.tng) {
        $currentType = ""
        $scriptName = ""
        $definitionType = ""
        $uid = ""
        $x = ""
        $y = ""
        $z = ""
        $startLine = 0
        $lineNo = 0
        foreach ($raw in Get-Content -LiteralPath $file.FullName) {
            $lineNo++
            $line = $raw.Trim()
            if ($line -match '^NewThing\s+(.+)$') {
                $currentType = $Matches[1].Trim().TrimEnd(";")
                $scriptName = ""
                $definitionType = ""
                $uid = ""
                $x = ""
                $y = ""
                $z = ""
                $startLine = $lineNo
                continue
            }
            if ($line -match '^UID\s+(.+?);?$') {
                $uid = Unquote $Matches[1]
            }
            if ($line -match '^ScriptName\s+(.+?);?$') {
                $scriptName = Unquote $Matches[1]
            }
            if ($line -match '^DefinitionType\s+(.+?);?$') {
                $definitionType = Unquote $Matches[1]
            }
            if ($line -match '^PositionX\s+(.+?);?$') {
                $x = Unquote $Matches[1]
            }
            if ($line -match '^PositionY\s+(.+?);?$') {
                $y = Unquote $Matches[1]
            }
            if ($line -match '^PositionZ\s+(.+?);?$') {
                $z = Unquote $Matches[1]
            }
            if ($line -match '^EndThing') {
                $haystack = "$currentType $scriptName $definitionType"
                if ($haystack -match $patterns) {
                    $rel = $file.FullName.Substring($tngRoot.Length).TrimStart("\", "/")
                    $category = GetEntityCategory $haystack
                    $entityRows.Add((@(
                        CsvSafe $rel
                        $startLine
                        CsvSafe $category
                        CsvSafe $currentType
                        CsvSafe $scriptName
                        CsvSafe $definitionType
                        CsvSafe $uid
                        CsvSafe $x
                        CsvSafe $y
                        CsvSafe $z
                        CsvSafe $haystack
                    ) -join ","))
                }
            }
        }
    }
}

$entityRows | Set-Content -LiteralPath (Join-Path $OutputDir "chest_key_reward_entities.csv") -Encoding UTF8

$summaryPath = Join-Path $OutputDir "world_chest_summary.txt"
$mapCount = if (Test-Path (Join-Path $OutputDir "wld_maps.csv")) { ((Import-Csv (Join-Path $OutputDir "wld_maps.csv")) | Measure-Object).Count } else { 0 }
$regionCount = if (Test-Path (Join-Path $OutputDir "wld_regions.csv")) { ((Import-Csv (Join-Path $OutputDir "wld_regions.csv")) | Measure-Object).Count } else { 0 }
$entityCount = [Math]::Max(0, $entityRows.Count - 1)
@(
    "wld_maps=$mapCount"
    "wld_regions=$regionCount"
    "chest_key_reward_entities=$entityCount"
) | Set-Content -LiteralPath $summaryPath -Encoding ASCII

"maps=$mapCount regions=$regionCount chest_key_reward_entities=$entityCount"
