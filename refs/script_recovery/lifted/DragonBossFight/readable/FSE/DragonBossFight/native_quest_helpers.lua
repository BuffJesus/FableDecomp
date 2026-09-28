-- Generated from the same native helper bodies as the quest draft.
local SetDragonState, SpawnEnemies
-- helper 0xD258C0 (named after the state it writes)
function SetDragonState(quest, me, param1)
    quest:SetStateInt("DragonState", param1)
    if param1 == 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(3008))
        quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(3024))
        return
    elseif param1 == 1 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(3012))
        quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(3028))
        return
    elseif param1 == 2 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(3016))
        quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(3032))
        return
    elseif param1 == 3 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(3020))
        quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(3036))
    end
end

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

return {helper_D258C0 = SetDragonState, SpawnEnemies = SpawnEnemies}
