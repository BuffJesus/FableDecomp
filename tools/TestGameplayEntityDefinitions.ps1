param(
    [string]$IndexDir = "ghidra_out/installed_game",
    [string]$OutputPath = "ghidra_out/installed_game/gameplay_entity_definition_validation.csv"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$entities = Import-Csv -LiteralPath (Join-Path $IndexDir "chest_key_reward_entities.csv")
$game = Import-Csv -LiteralPath (Join-Path $IndexDir "compiled_game_entries.csv")

$byName = @{}
foreach ($entry in $game) {
    if (![string]::IsNullOrWhiteSpace($entry.name)) {
        $byName[$entry.name.ToUpperInvariant()] = $entry
    }
}

$rows = New-Object System.Collections.Generic.List[object]
foreach ($entity in $entities) {
    $key = $entity.definition_type.ToUpperInvariant()
    $resolved = $byName.ContainsKey($key)
    $entry = if ($resolved) { $byName[$key] } else { $null }
    $rows.Add([pscustomobject]@{
        file = $entity.file
        line = $entity.line
        category = $entity.category
        entity_type = $entity.entity_type
        definition_type = $entity.definition_type
        uid = $entity.uid
        x = $entity.x
        y = $entity.y
        z = $entity.z
        resolved_in_game_bin = $resolved
        game_bin_index = if ($entry) { $entry.index } else { "" }
        game_bin_definition = if ($entry) { $entry.definition } else { "" }
        game_bin_length = if ($entry) { $entry.length } else { "" }
    })
}

$rows | Export-Csv -LiteralPath $OutputPath -NoTypeInformation

$missing = ($rows | Where-Object { $_.resolved_in_game_bin -ne $true } | Measure-Object).Count
"entities=$($rows.Count) unresolved=$missing output=$OutputPath"
