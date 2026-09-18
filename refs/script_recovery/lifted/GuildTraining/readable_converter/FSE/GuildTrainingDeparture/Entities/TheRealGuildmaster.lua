-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local heroSpokenToMe, teleportToWoods

-- TheRealGuildmaster.Main (retail 0x00d50c60)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local actorMap, movie, resource, movie2, resource2
    quest:FadeScreenOut(0.5, 0.0)
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:SetIsPushableByHero(me, false)
    quest:SetHeroGuideToShowQuestCardsWhenSpokenTo(false)
    quest:EntityTeleportToThing(me, quest:GetThingWithScriptName("M_DepartureTeacherStand"), false)
    resource2 = resources:NewResource()
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource2); return end
    end
    if not quest:IsActiveThreadTerminating() then
        if not quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") then
            if quest:IsActiveThreadTerminating() then goto LAB_00d5134c end
            resource = resources:NewResource()
            while not resources:TryAcquire(resource, hero, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    resources:ReleaseResource(resource2)
                    return
                end
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource)
                resources:ReleaseResource(resource2)
                return
            end
            actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "GM", resource2)
            resources:SetActor(actorMap, "HERO", resource)
            movie = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_DEPARTURE_GM_DONE", actorMap, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_09", "GuildWoods", "")
            quest:ActivateQuest("Q_GuildTrainingWoodsDeparture")
            quest:SetQuestAsPersistent("Q_GuildTrainingWoodsDeparture", false)
            quest:SetMasterGameState("HeroTakingGuildTest", true)
        end
        while quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") do
            if not quest:NewScriptFrame(me) then goto LAB_00d5134c end
            if me:IsTalkedToByHero() then
                movie2 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                    if not me:Speak(hero, "TEXT_QST_028_GUILDMASTER_WOODS_DEPARTURE_MOAN", GROUP_SELECT_FIRST, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        resources:ReleaseResource(resource2)
                        return
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
        end
        if not quest:IsActiveThreadTerminating() then
            quest:ClearThingHasInformation(me)
        end
    end
    ::LAB_00d5134c::
    resources:ReleaseResource(resource2)
end

-- TheRealGuildmaster.Init (retail 0x00d50a80)
function Init(quest, me)
    heroSpokenToMe = false
    teleportToWoods = false
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

