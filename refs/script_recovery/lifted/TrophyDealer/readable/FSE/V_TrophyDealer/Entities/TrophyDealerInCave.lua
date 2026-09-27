-- Readable native conversion: TrophyDealerInCave. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- TrophyDealerInCave.Main (retail 0x00ee76f0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local resource2 = resources:NewResource()
    if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource2); return end
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAsDamageable(me, false)
    local resource = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); resources:ReleaseResource(resource2); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); resources:ReleaseResource(resource2); return end
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "Hero", resource)
    resources:SetActor(actorMap, "Trophy", resource2)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resources:RunMacro("CS_TROPHY_DEALER", actorMap, false, true)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
    quest:SetCreatureGeneratorsEnabled("Witchwood2", true)
    quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
    quest:FadeScreenIn()
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    resources:ReleaseResource(resource2)
    do return end
    resources:ReleaseResource(resource)
    resources:ReleaseResource(resource2)
end

-- TrophyDealerInCave.Init (retail 0x00ee73f0)
function Init(quest, me)
    quest:SetThingHasInformation(me, false, false, false)
end

-- TrophyDealerInCave.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TrophyDealerInCave.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

