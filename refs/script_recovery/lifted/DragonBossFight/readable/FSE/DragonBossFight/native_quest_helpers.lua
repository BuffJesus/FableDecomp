-- Generated from the same native helper bodies as the quest draft.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    DBNumMinionsHighHealth = 3008,  -- 0
    DBNumMinionsMediumHealth = 3012,  -- 2
    DBNumMinionsLowHealth = 3016,  -- 2
    DBNumMinionsVeryLowHealth = 3020,  -- 3
    DBNumSummonersHighHealth = 3024,  -- 0
    DBNumSummonersMediumHealth = 3028,  -- 0
    DBNumSummonersLowHealth = 3032,  -- 1
    DBNumSummonersVeryLowHealth = 3036,  -- 1
}
local SetDragonState, SpawnEnemies
-- helper 0xD258C0 (named after the state it writes)
function SetDragonState(quest, me, param1)
    quest:SetStateInt("DragonState", param1)
    if param1 == 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumMinionsHighHealth))
        quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumSummonersHighHealth))
        return
    elseif param1 == 1 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumMinionsMediumHealth))
        quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumSummonersMediumHealth))
        return
    elseif param1 == 2 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumMinionsLowHealth))
        quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumSummonersLowHealth))
        return
    elseif param1 == 3 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumMinionsVeryLowHealth))
        quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumSummonersVeryLowHealth))
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
