param(
    [string]$InstalledDir = "ghidra_out/installed_game",
    [string]$OutJson = "ghidra_out/installed_game/object_families.json",
    [string]$OutCsv = "ghidra_out/installed_game/object_families.csv"
)

# Decode OBJECT_FAMILY and CContainerRewardHeroDef payload hex dumps into a
# reward-picker table for FableForge. Layouts (evidence: docs/FINDINGS.md
# 2026-07-18 "Reward payload fully decoded"):
#   OBJECT_FAMILY:            9-byte header, u32 count, count x (u32 objDefIdx, f32 weight)
#   CContainerRewardHeroDef:  7-byte header, u32 count, count x (u32 familyDefIdx)
# Def indices are game.bin entry indices (rows of compiled_game_entries.csv).

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$entries = Import-Csv (Join-Path $InstalledDir "compiled_game_entries.csv")
$nameByIndex = @{}
for ($i = 0; $i -lt $entries.Count; $i++) { $nameByIndex[$i] = $entries[$i].name }

function ConvertFrom-Hex([string]$hex) {
    $bytes = New-Object byte[] ($hex.Length / 2)
    for ($i = 0; $i -lt $bytes.Length; $i++) {
        $bytes[$i] = [Convert]::ToByte($hex.Substring($i * 2, 2), 16)
    }
    return ,$bytes
}

$families = New-Object System.Collections.Generic.List[object]
foreach ($row in (Import-Csv (Join-Path $InstalledDir "objectfamily_payloads.tsv") -Delimiter "`t")) {
    $bytes = ConvertFrom-Hex $row.hex
    $count = [BitConverter]::ToUInt32($bytes, 9)
    $members = New-Object System.Collections.Generic.List[object]
    for ($i = 0; $i -lt $count; $i++) {
        $off = 13 + $i * 8
        $idx = [BitConverter]::ToUInt32($bytes, $off)
        $weight = [BitConverter]::ToSingle($bytes, $off + 4)
        $members.Add([pscustomobject]@{
            object_index = $idx
            object_name = if ($idx -eq 0) { "" } else { $nameByIndex[[int]$idx] }
            weight = $weight
        })
    }
    $total = 0.0
    foreach ($m in $members) { $total += $m.weight }
    $families.Add([pscustomobject]@{
        family_index = [int]$row.index
        family_name = $row.name
        member_count = [int]$count
        weight_total = $total
        members = $members
    })
}

$rewardDefs = New-Object System.Collections.Generic.List[object]
foreach ($row in (Import-Csv (Join-Path $InstalledDir "rewarddef_payloads.tsv") -Delimiter "`t")) {
    $bytes = ConvertFrom-Hex $row.hex
    $count = [BitConverter]::ToUInt32($bytes, 7)
    $refs = New-Object System.Collections.Generic.List[object]
    for ($i = 0; $i -lt $count; $i++) {
        $idx = [BitConverter]::ToUInt32($bytes, 11 + $i * 4)
        $refs.Add([pscustomobject]@{
            family_index = $idx
            family_name = $nameByIndex[[int]$idx]
        })
    }
    $rewardDefs.Add([pscustomobject]@{
        entry_index = [int]$row.index
        family_count = [int]$count
        families = $refs
    })
}

[pscustomobject]@{
    generated_on = (Get-Date -Format o)
    source = "game.bin via tools/bin_dump hex mode"
    object_families = $families
    container_reward_defs = $rewardDefs
} | ConvertTo-Json -Depth 6 | Set-Content -Encoding utf8 $OutJson

$flat = foreach ($fam in $families) {
    foreach ($m in $fam.members) {
        [pscustomobject]@{
            family_index = $fam.family_index
            family_name = $fam.family_name
            object_index = $m.object_index
            object_name = $m.object_name
            weight = $m.weight
            weight_total = $fam.weight_total
        }
    }
}
$flat | Export-Csv -NoTypeInformation -Encoding utf8 $OutCsv

"families=$($families.Count) reward_defs=$($rewardDefs.Count) json=$OutJson csv=$OutCsv"
