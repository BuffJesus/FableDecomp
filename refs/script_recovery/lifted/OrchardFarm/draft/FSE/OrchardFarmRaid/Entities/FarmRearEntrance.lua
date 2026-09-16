-- Generated native draft: FarmRearEntrance. Review coverage report before use.
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
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    local bVar1 = not alive
    if (not bVar1) and (quest:GetStateBool("DoneIntroduction")) then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            quest:RemoveThing(me, true, true)
        end
    end
end

function Init(quest, me)
    quest:EntitySetTargetable(me, false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

