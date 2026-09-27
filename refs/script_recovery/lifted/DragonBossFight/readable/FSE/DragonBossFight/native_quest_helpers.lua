-- Generated from the same native helper bodies as the quest draft.
local SpawnEnemies
-- D25C20: bsim names this body NScript::CQ_DragonBossFightScript::SpawnEnemies (a homologous script member); no PDB name
function SpawnEnemies(quest, me)
    local numFlyBysSinceLastSummonerSpawn = quest:GetStateInt("NumFlyBysSinceLastSummonerSpawn")
    quest:SetStateBool("MinionSpawningEnabled", true)
    quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", numFlyBysSinceLastSummonerSpawn + 1)
    if not (quest:GetStateInt("NumFlyBysBetweenSummonerSpawns") <= numFlyBysSinceLastSummonerSpawn + 1) then return end
    local predicateResult = quest:IsActiveThreadTerminating()
    if predicateResult then return end
    quest:SetStateBool("SummonerSpawningEnabled", true)
    quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", 0)
end

return {SpawnEnemies = SpawnEnemies}
