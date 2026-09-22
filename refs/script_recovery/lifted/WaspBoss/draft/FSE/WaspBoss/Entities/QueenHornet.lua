-- Generated native draft: QueenHornet. Review coverage report before use.
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
    local bVar1, p0, xStack_20
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        xStack_20 = resources:NewResource()
        quest:Pause(2.0)
        resources:PrepareResource(xStack_20)
        bVar1 = resources:TryAcquire(xStack_20, me, 0)
        while not bVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00e11cb7 end
            bVar1 = resources:TryAcquire(xStack_20, me, 0)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            resources:PrepareResource(xStack_20)
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
            until not (not bVar1)
        end
        ::LAB_00e11cb7::
        resources:ReleaseResource(xStack_20)
    end
end

function Init(quest, me)
    __native_entity_state:SetStateInt("DroneDeadCount", 0)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

