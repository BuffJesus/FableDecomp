-- Generated native draft: WaspAttacker. Review coverage report before use.
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
    local bVar2, cVar3, p0, pTarget, r1, xStack_20
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        xStack_20 = resources:NewResource()
        r1 = quest:GetThingWithScriptName("WaspVictim")
        quest:GiveThingBestEnemyTarget(me, r1)
        cVar3 = (r1 ~= nil and r1:IsAlive())
        while cVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e1155e end
            cVar3 = (r1 ~= nil and r1:IsAlive())
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            pTarget = quest:GetHero()
            quest:GiveThingBestEnemyTarget(me, pTarget)
        end
        ::LAB_00e1155e::
        resources:ReleaseResource(xStack_20)
    end
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

