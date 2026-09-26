-- Generated native draft: WB_Villager. Review coverage report before use.
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
    local resources = quest:RetailResources()
    local xStack_20
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    local bVar1 = not alive
    if not bVar1 then
        xStack_20 = resources:NewResource()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        while not bVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
        end
        resources:ReleaseResource(xStack_20)
    end
end

function Init(quest, me)
    __native_entity_state:SetStateBool("Done", false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

