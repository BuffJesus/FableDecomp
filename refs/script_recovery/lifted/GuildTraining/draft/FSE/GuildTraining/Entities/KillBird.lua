-- Generated native draft: KillBird. Review coverage report before use.
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
        bVar1 = false
        if bVar1 ~= 0 then
        end
        bVar1 = resources:TryAcquire(xStack_20, me, 4)
        while not bVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00d43263 end
            bVar1 = resources:TryAcquire(xStack_20, me, 4)
        end
        while true do
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not (not bVar1) then break end
            alive = quest:NewScriptFrame(me)
        end
        ::LAB_00d43263::
        resources:ReleaseResource(xStack_20)
    end
end

function Init(quest, me)
    local pThing2 = quest:GetHero()
    quest:EntitySetThingAsEnemyOfThing(me, pThing2)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
    local piVar1
    local cVar2 = me:MsgIsKilledBy("")
    if cVar2 then
        quest:SetStateBool("DisplayBirdKilledMessage", true)
        piVar1 = (__native_entity_state:GetStateInt("self_0x14") + 0x54)
        -- TODO(native): *piVar1 = *piVar1 + 1;
    end
end

