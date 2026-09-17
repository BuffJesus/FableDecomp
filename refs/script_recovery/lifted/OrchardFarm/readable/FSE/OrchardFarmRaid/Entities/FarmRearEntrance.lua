-- Readable native conversion: FarmRearEntrance. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- FarmRearEntrance.Main (retail 0x00dcf4c0)
function Main(quest, me)
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() and quest:GetStateBool("DoneIntroduction") then
        if quest:IsActiveThreadTerminating() then return end
        quest:RemoveThing(me, true, true)
    end
end

-- FarmRearEntrance.Init (retail 0x00dcf480)
function Init(quest, me)
    quest:EntitySetTargetable(me, false)
end

-- FarmRearEntrance.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- FarmRearEntrance.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

