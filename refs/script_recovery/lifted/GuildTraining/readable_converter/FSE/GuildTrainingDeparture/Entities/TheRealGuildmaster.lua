-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- TheRealGuildmaster.Main (retail 0x00d50c60)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue2, scratchValue5, scratchValue6, scratchValue7, scratchValue8
    quest:FadeScreenOut(0.5, 0.0)
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:SetIsPushableByHero(me, false)
    quest:SetHeroGuideToShowQuestCardsWhenSpokenTo(false)
    quest:EntityTeleportToThing(me, quest:GetThingWithScriptName("M_DepartureTeacherStand"), false)
    scratchValue8 = resources:NewResource()
    while not resources:TryAcquire(scratchValue8, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(scratchValue8); return end
    end
    if not quest:IsActiveThreadTerminating() then
        if not quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") then
            if quest:IsActiveThreadTerminating() then goto LAB_00d5134c end
            scratchValue6 = resources:NewResource()
            while not resources:TryAcquire(scratchValue6, quest:GetHero(), 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(scratchValue6)
                    resources:ReleaseResource(scratchValue8)
                    return
                end
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(scratchValue6)
                resources:ReleaseResource(scratchValue8)
                return
            end
            scratchValue2 = resources:NewActorMap()
            resources:SetActor(scratchValue2, "GM", scratchValue8)
            resources:SetActor(scratchValue2, "HERO", scratchValue6)
            scratchValue5 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_DEPARTURE_GM_DONE", scratchValue2, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue5)
            resources:DestroyActorMap(scratchValue2)
            resources:ReleaseResource(scratchValue6)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_09", "GuildWoods", "")
            quest:ActivateQuest("Q_GuildTrainingWoodsDeparture")
            quest:SetQuestAsPersistent("Q_GuildTrainingWoodsDeparture", false)
            quest:SetMasterGameState("HeroTakingGuildTest", true)
        end
        while quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") do
            if not quest:NewScriptFrame(me) then goto LAB_00d5134c end
            if me:IsTalkedToByHero() then
                scratchValue7 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue8)) then
                    me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_WOODS_DEPARTURE_MOAN", 0, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue7)
                            resources:ReleaseResource(scratchValue8)
                            return
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(scratchValue7)
                        resources:DestroyMovie(scratchValue8)
                        return
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue7)
            end
        end
        if not quest:IsActiveThreadTerminating() then
            quest:ClearThingHasInformation(me)
        end
    end
    ::LAB_00d5134c::
    resources:ReleaseResource(scratchValue8)
end

-- TheRealGuildmaster.Init (retail 0x00d50a80)
function Init(quest, me)
    state:SetBool("HeroSpokenToMe", false)
    state:SetBool("TeleportToWoods", false)
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

