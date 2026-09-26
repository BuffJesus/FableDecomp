-- Readable native conversion: Q_ArenaHoldingScript. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_ArenaHoldingScript.Main (retail 0x00cf0790)
function Main(quest)
    local timerId = quest:RegisterTimer()
    while not quest:IsRegionLoaded("ArenaExterior") do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
    quest:AddEntityBinding("EntranceGuard", "ArenaHoldingScript/Entities/EntranceGuard", 1)
    quest:AddEntityBinding("EntranceGuard2", "ArenaHoldingScript/Entities/EntranceGuard2", 1)
    quest:AddEntityBinding("OakvaleFan", "ArenaHoldingScript/Entities/OakvaleFan", 1)
    quest:AddEntityBinding("BowerstoneFan", "ArenaHoldingScript/Entities/BowerstoneFan", 1)
    quest:AddEntityBinding("HookCoastFan", "ArenaHoldingScript/Entities/HookCoastFan", 1)
    quest:AddEntityBinding("FanMarker", "ArenaHoldingScript/Entities/FanMarker", 1)
    quest:FinalizeEntityBindings()
    local arenaExtToHallOfHeroes = quest:GetThingWithScriptName("ArenaExtToHallOfHeroes")
    quest:SetThingAsUsable(arenaExtToHallOfHeroes, false)
    quest:SetThingPersistent(arenaExtToHallOfHeroes, true)
    quest:DeregisterTimer(timerId)
    do return end
    quest:DeregisterTimer(timerId)
end

-- Q_ArenaHoldingScript.Init (retail 0x00cf06d0)
function Init(quest)
    quest:SetStateBool("PlayerAttackedFans", false)
    quest:SetStateBool("FansCanLeaveNow", false)
    quest:SetStateBool("OakvaleInsult", false)
    quest:SetStateBool("ArenaOver", false)
    quest:SetStateBool("InHitCutsceneAlready", false)
end

-- Q_ArenaHoldingScript.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

