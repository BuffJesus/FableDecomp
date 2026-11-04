-- Readable native conversion: DemonDoorFace. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- per-entity fields (native class members; one Lua state per entity instance)
local timesSpoken

-- DemonDoorFace.Main (retail 0x00ee7a20)
function Main(quest, me)
    local resources = quest:RetailResources()
    local resource2 = resources:NewResource()
    if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource2); return end
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    while true do
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
            if not quest:GetMasterGameState("SingingStonesInSync") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue = resources:ScriptThing(resource2)
                local fret_0 = quest:GetHealth(scratchValue)
                if 0.0 < fret_0 then
                    local p1 = "TEXT_QST_071_DOOR_LOCKED1"
                    local hero = quest:GetHero()
                    if not me:Speak(hero, p1, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        resources:ReleaseResource(resource2)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            resources:ReleaseResource(resource2)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        resources:ReleaseResource(resource2)
                        return
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            end
            if not quest:GetMasterGameState("TrophyDealerHeroSpokenToDemonDoors") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
                quest:SetMasterGameState("TrophyDealerHeroSpokenToDemonDoors", true)
                local getActiveQuestName = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(getActiveQuestName, "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_03", "Witchwood2", "")
            end
        end
        if quest:GetMasterGameState("SingingStonesInSync") then break end
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    local resource = resources:NewResource()
    local hero2 = quest:GetHero()
    resources:TryAcquire(resource, hero2, 4)
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource)
        resources:ReleaseResource(resource2)
        return
    end
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "DOOR", resource2)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resources:RunMacro("CS_TROPHY_DEALER_DOOR_OPENS", actorMap, false, true)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
    local getActiveQuestName2 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(getActiveQuestName2, "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_04", "WitchwoodCavern", "")
    local demonDoorDoor = quest:GetThingWithScriptName("DemonDoorDoor")
    quest:SetThingPersistent(demonDoorDoor, true)
    quest:SetMasterGameState("SingingStonesInSync", false)
    quest:RemoveThing(me, false, true)
    resources:ReleaseResource(resource2)
end

-- DemonDoorFace.Init (retail 0x00ee74e0)
function Init(quest, me)
    timesSpoken = 0
    quest:SetThingHasInformation(me, false, true, false)
    quest:SetThingPersistent(me, true)
    quest:EntitySetAsDamageable(me, false)
end

-- DemonDoorFace.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- DemonDoorFace.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

