-- Generated native draft: Cham. Review coverage report before use.
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
    local bVar1, cVar2, iVar3, p0, p2, p3, p4, pThing, this_00, xStack_10
    local alive = true
    xStack_10 = resources:NewResource()
    quest:EntityUnsetAsOpinionSource(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    cVar2 = quest:GetStateBool("ChamLeaving")
    while not cVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            resources:ReleaseResource(xStack_10)
            return
        end
        cVar2 = quest:GetStateBool("ChamLeaving")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        resources:PrepareResource(xStack_10)
        bVar1 = resources:TryAcquire(xStack_10, me, 4)
        while not bVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00f1492e end
            bVar1 = resources:TryAcquire(xStack_10, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if not bVar1 then
            this_00 = quest:GetThingWithScriptName("GuardingDoorMarkerLeft")
            p4 = 1
            p3 = 0
            p2 = 1
            iVar3 = 1.0
            p0 = this_00:GetPos()
            me:MoveToPosition(p0, iVar3, p2, (p3 ~= 0), (p4 ~= 0))
            iVar3 = me:IsPerformingScriptTask()
            cVar2 = iVar3
            while cVar2 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then goto LAB_00f1492e end
                iVar3 = me:IsPerformingScriptTask()
                cVar2 = iVar3
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:RemoveThing(me, false, true)
            end
        end
    end
    ::LAB_00f1492e::
    resources:ReleaseResource(xStack_10)
end

function Init(quest, me)
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

