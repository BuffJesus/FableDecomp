-- Readable native conversion: Q_GuildTrainingPreMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTrainingPreMelee.Main (retail 0x00d51520)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local preMeleeMaze, preMeleeWhisper, theRealGuildmaster, movie, resource, actorMap, resource4
    local resource5, resource6, timerId
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingPreMelee/Entities/TheRealGuildmaster")
    quest:AddEntityBinding("PreMeleeDummy", "GuildTrainingPreMelee/Entities/PreMeleeDummy")
    quest:AddEntityBinding("PreMeleeWhisper", "GuildTrainingPreMelee/Entities/PreMeleeWhisper")
    quest:FinalizeEntityBindings()
    quest:SetMasterGameState("GuildWarningOccuring", true)
    preMeleeMaze = quest:GetThingWithScriptName("PreMeleeMaze")
    preMeleeWhisper = quest:GetThingWithScriptName("PreMeleeWhisper")
    theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    quest:GiveHeroTitle("OBJECT_HERO_TITLE_CHICKEN_CHASER")
    quest:SetStateInt("PreMeleeMode", 0)
    resource6 = resources:NewResource()
    while not resources:TryAcquire(resource6, preMeleeMaze, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(resource6); return end
    end
    if not quest:IsActiveThreadTerminating() then
        resource5 = resources:NewResource()
        while not resources:TryAcquire(resource5, preMeleeWhisper, 4) do
            if not quest:NewScriptFrame() then goto LAB_00d51951 end
        end
        if not quest:IsActiveThreadTerminating() then
            resource4 = resources:NewResource()
            while not resources:TryAcquire(resource4, theRealGuildmaster, 4) do
                if not quest:NewScriptFrame() then goto LAB_00d51948 end
            end
            if not quest:IsActiveThreadTerminating() then
                resource = resources:NewResource()
                while not resources:TryAcquire(resource, hero, 4) do
                    if not quest:NewScriptFrame() then goto LAB_00d5193f end
                end
                if not quest:IsActiveThreadTerminating() then
                    actorMap = resources:NewActorMap()
                    resources:SetActor(actorMap, "HERO", resource)
                    resources:SetActor(actorMap, "MAZE", resource6)
                    resources:SetActor(actorMap, "WHISPER", resource5)
                    resources:SetActor(actorMap, "MASTER", resource4)
                    movie = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GUILD_PREMELEE_INTRO", actorMap, false, true)
                    resources:RunMacro("CS_GUILD_PREMELEE_BOOHOO", actorMap, false, true)
                    resources:RunMacro("CS_GUILD_PREMELEE_WAKEUP", actorMap, false, true)
                    quest:FixMovieSequenceCamera(false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                    quest:ResetPlayerCreatureCombatMultiplier()
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:DestroyActorMap(actorMap)
                    resources:ReleaseResource(resource)
                    resources:ReleaseResource(resource4)
                    resources:ReleaseResource(resource5)
                    resources:ReleaseResource(resource6)
                    quest:SetStateBool("WhisperCutsceneFinished", true)
                    quest:SetStateBool("GuildmasterTeleport", true)
                    quest:SetMasterGameState("GuildWarningOccuring", false)
                    timerId = quest:RegisterTimer()
                    quest:SetTimer(timerId, 5)
                    while 0 < quest:GetTimer(timerId) do
                        if not quest:NewScriptFrame() then goto LAB_00d51dd9 end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        quest:GiveHeroExpression("EXPRESSION_FART", -1, true)
                        quest:GiveHeroExpression("EXPRESSION_BELCH", -1, true)
                        quest:GiveHeroExpression("EXPRESSION_GIGGLE", -1, true)
                        while not quest:GetStateBool("HeroSleeps") do
                            if not quest:NewScriptFrame() then goto LAB_00d51dd9 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                        end
                    end
                    ::LAB_00d51dd9::
                    quest:DeregisterTimer(timerId)
                    goto LAB_00d51de2
                end
                ::LAB_00d5193f::
                resources:ReleaseResource(resource)
            end
            ::LAB_00d51948::
            resources:ReleaseResource(resource4)
        end
        ::LAB_00d51951::
        resources:ReleaseResource(resource5)
    end
    resources:ReleaseResource(resource6)
    ::LAB_00d51de2::
end

-- Q_GuildTrainingPreMelee.Init (retail 0x00d51450)
function Init(quest)
    quest:SetStateBool("HeroSleeps", false)
    quest:SetStateBool("WhisperCutsceneFinished", false)
    quest:SetStateBool("GuildmasterTeleport", false)
    quest:SetStateBool("WhisperStopFollowing", false)
end

