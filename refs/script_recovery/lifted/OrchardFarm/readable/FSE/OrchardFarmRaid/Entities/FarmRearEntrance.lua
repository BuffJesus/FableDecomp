-- Readable native conversion: FarmRearEntrance. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() and quest:GetStateBool("DoneIntroduction") then
        if quest:IsActiveThreadTerminating() then return end
        quest:RemoveThing(me, true, true)
    end
end

function Init(quest, me)
    quest:EntitySetTargetable(me, false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

