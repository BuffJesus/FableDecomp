-- Generated from the same native helper bodies as the quest draft.
local helper_D258C0, helper_D25C20
function helper_D258C0(quest, me, native_arg_param_1)
    local bVar1
    local alive = true
    quest:SetStateInt("DragonState", native_arg_param_1)
    if native_arg_param_1 == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(0xbc0))
            quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(0xbd0))
            return
        end
    elseif native_arg_param_1 == 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(0xbc4))
            quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(0xbd4))
            return
        end
    elseif native_arg_param_1 == 2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(0xbc8))
            quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(0xbd8))
            return
        end
    elseif native_arg_param_1 == 3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(0xbcc))
            quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(0xbdc))
        end
    end
end

function helper_D25C20(quest, me)
    local bVar2
    local alive = true
    local iVar1 = quest:GetStateInt("NumFlyBysSinceLastSummonerSpawn")
    quest:SetStateBool("MinionSpawningEnabled", true)
    quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", iVar1 + 1)
    if quest:GetStateInt("NumFlyBysBetweenSummonerSpawns") <= iVar1 + 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:SetStateBool("SummonerSpawningEnabled", true)
            quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", 0)
        end
    end
end

return {helper_D258C0 = helper_D258C0, helper_D25C20 = helper_D25C20}
