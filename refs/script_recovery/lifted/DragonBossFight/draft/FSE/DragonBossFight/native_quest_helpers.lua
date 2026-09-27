-- Generated from the same native helper bodies as the quest draft.
local helper_D25C20
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

return {helper_D25C20 = helper_D25C20}
