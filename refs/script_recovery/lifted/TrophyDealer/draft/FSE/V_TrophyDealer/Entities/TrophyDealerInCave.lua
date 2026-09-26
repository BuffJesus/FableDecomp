-- Generated native draft: TrophyDealerInCave. Review coverage report before use.
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
    local b2, b3, bVar3, delay, ePriority, p0, pCVar5, pThing, pppuVar6, xStack_10, xStack_20, xStack_30, xStack_3c
    local alive = true
    xStack_30 = resources:NewResource()
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        resources:ReleaseResource(xStack_30)
        return
    end
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAsDamageable(me, false)
    xStack_20 = resources:NewResource()
    ePriority = 4
    pppuVar6 = xStack_20
    pThing = quest:GetHero()
    resources:TryAcquire(pppuVar6, pThing, ePriority)
    resources:PrepareResource(xStack_30)
    bVar3 = resources:TryAcquire(xStack_30, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00ee7842 end
        bVar3 = resources:TryAcquire(xStack_30, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        xStack_3c = resources:NewActorMap()
        resources:SetActor(xStack_3c, "Hero", xStack_20)
        resources:SetActor(xStack_3c, "Trophy", xStack_30)
        xStack_10 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        resources:RunMacro("CS_TROPHY_DEALER", xStack_3c, false, true)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_10)
        resources:DestroyActorMap(xStack_3c)
        resources:ReleaseResource(xStack_20)
        quest:SetCreatureGeneratorsEnabled("Witchwood2", true)
        b3 = false
        b2 = false
        bVar3 = true
        pCVar5 = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(pCVar5, bVar3, b2, b3)
        quest:FadeScreenIn()
        delay = 0
        pCVar5 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pCVar5, delay)
        resources:ReleaseResource(xStack_30)
        return
    end
    ::LAB_00ee7842::
    resources:ReleaseResource(xStack_20)
    resources:ReleaseResource(xStack_30)
end

function Init(quest, me)
    quest:SetThingHasInformation(me, false, false, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

