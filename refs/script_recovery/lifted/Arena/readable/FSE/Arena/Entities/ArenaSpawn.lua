-- Readable native conversion: ArenaSpawn. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- per-entity fields (native class members; one Lua state per entity instance)
local self0X14

-- ArenaSpawn.Main (retail 0x00f1b690)
function Main(quest, me)
    local self_0x14
    tonumber(me:GetDataString())
    local timerId = quest:RegisterTimer()
    while not quest:IsActiveThreadTerminating() do
        -- TODO(native): cVar1 = *(self_0x14 + 0xe2 + iVar4)
        while not nil --[[unresolved native value]] do
            if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); return end
            -- TODO(native): cVar1 = *(self_0x14 + 0xe2 + iVar4)
        end
        if quest:IsActiveThreadTerminating() then break end
        if quest:GetStateInt("ArenaRound") ~= 3 then
            quest:CreateEffectAtPos("SUMMON_ARENA", me:GetPos(), timerId + 4, 0.0)
        end
        -- TODO(native): *(undefined1 *)(*(int *)(this + 0x14) + 0xe2 + iVar4) = 0;
        quest:NewScriptFrame(me)
    end
    quest:DeregisterTimer(timerId)
end

-- ArenaSpawn.Init (retail 0x00f1b660)
function Init(quest, me)
end

-- ArenaSpawn.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- ArenaSpawn.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

