-- Generated native draft: Q_ArenaHoldingScript. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local r1
    local alive = true
    local timerId = quest:RegisterTimer()
    local i_stk_10 = timerId
    local bVar1 = quest:IsRegionLoaded("ArenaExterior")
    while not bVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            quest:DeregisterTimer(i_stk_10)
            return
        end
        bVar1 = quest:IsRegionLoaded("ArenaExterior")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        quest:AddEntityBinding("EntranceGuard", "ArenaHoldingScript/Entities/EntranceGuard", 1)
        quest:AddEntityBinding("EntranceGuard2", "ArenaHoldingScript/Entities/EntranceGuard2", 1)
        quest:AddEntityBinding("OakvaleFan", "ArenaHoldingScript/Entities/OakvaleFan", 1)
        quest:AddEntityBinding("BowerstoneFan", "ArenaHoldingScript/Entities/BowerstoneFan", 1)
        quest:AddEntityBinding("HookCoastFan", "ArenaHoldingScript/Entities/HookCoastFan", 1)
        quest:AddEntityBinding("FanMarker", "ArenaHoldingScript/Entities/FanMarker", 1)
        quest:FinalizeEntityBindings()
        r1 = quest:GetThingWithScriptName("ArenaExtToHallOfHeroes")
        quest:SetThingAsUsable(r1, false)
        quest:SetThingPersistent(r1, true)
        quest:DeregisterTimer(i_stk_10)
        return
    end
    quest:DeregisterTimer(i_stk_10)
end

function Init(quest)
    quest:SetStateBool("PlayerAttackedFans", false)
    quest:SetStateBool("FansCanLeaveNow", false)
    quest:SetStateBool("OakvaleInsult", false)
    quest:SetStateBool("ArenaOver", false)
    quest:SetStateBool("InHitCutsceneAlready", false)
end

function OnPersist(quest, context)
end

