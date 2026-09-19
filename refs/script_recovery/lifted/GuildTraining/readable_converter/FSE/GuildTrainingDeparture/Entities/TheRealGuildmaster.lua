-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local heroSpokenToMe, teleportToWoods

-- TheRealGuildmaster.Main (retail 0x00d50c60)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local movie
    quest:FadeScreenOut(0.5, 0.0)
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:SetIsPushableByHero(me, false)
    quest:SetHeroGuideToShowQuestCardsWhenSpokenTo(false)
    quest:EntityTeleportToThing(me, quest:GetThingWithScriptName("M_DepartureTeacherStand"), false)
    local resource2 = resources:NewResource()
    while not resources:TryAcquire(resource2, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d5134c end
    if not quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") then
        local resource = resources:NewResource()
        while not resources:TryAcquire(resource, hero, 4) do
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
        quest:StartCutscene({GM = me, HERO = hero}, {}, true)
        quest:RunCutscene("CS_GUILD_DEPARTURE_GM_DONE", true, false)
        quest:EndCutscene()
        resources:ReleaseResource(resource)
        quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_09", "GuildWoods", "")
        quest:ActivateQuest("Q_GuildTrainingWoodsDeparture")
        quest:SetQuestAsPersistent("Q_GuildTrainingWoodsDeparture", false)
        quest:SetMasterGameState("HeroTakingGuildTest", true)
    end
    while quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") do
        if not quest:NewScriptFrame(me) then goto LAB_00d5134c end
        if not me:IsTalkedToByHero() then goto continue_2 end
        movie = resources:StartMovie("")
        quest:StartMovieSequence()
        quest:PauseAllNonScriptedEntities(true)
        if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_WOODS_DEPARTURE_MOAN", GROUP_SELECT_FIRST, false, true, false)
            while me:IsPerformingScriptTask() do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:EndCutscene()
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource2)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:EndCutscene()
                resources:DestroyMovie(movie)
                resources:ReleaseResource(resource2)
                return
            end
        end
        quest:EndCutscene()
        resources:DestroyMovie(movie)
        ::continue_2::
    end
    if not quest:IsActiveThreadTerminating() then
        quest:ClearThingHasInformation(me)
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

