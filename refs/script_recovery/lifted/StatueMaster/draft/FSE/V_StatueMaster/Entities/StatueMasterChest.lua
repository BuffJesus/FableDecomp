-- Generated native draft: StatueMasterChest. Review coverage report before use.
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
    local iVar2 = require("V_StatueMaster.native_quest_helpers").GetStatuePointingPosition(quest, me)
    if iVar2 == 3 then
        alive = not quest:IsActiveThreadTerminating()
        return
    end
    alive = not quest:IsActiveThreadTerminating()
    local bVar1 = not alive
    if not bVar1 then
        quest:RemoveThing(me, false, true)
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

