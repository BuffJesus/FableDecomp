-- Readable native conversion: Q_GuildTrainingPreMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTrainingPreMelee.Main (retail 0x00d51520)
function Main(quest)
    local resources = quest:RetailResources()
    local preMeleeMaze, r2_1, r3_1, scratchValue3, scratchValue4, scratchValue5, scratchValue6
    local scratchValue7, scratchValue8, timerId
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingPreMelee/Entities/TheRealGuildmaster")
    quest:AddEntityBinding("PreMeleeDummy", "GuildTrainingPreMelee/Entities/PreMeleeDummy")
    quest:AddEntityBinding("PreMeleeWhisper", "GuildTrainingPreMelee/Entities/PreMeleeWhisper")
    quest:FinalizeEntityBindings()
    quest:SetMasterGameState("GuildWarningOccuring", true)
    preMeleeMaze = quest:GetThingWithScriptName("PreMeleeMaze")
    r2_1 = quest:GetThingWithScriptName("PreMeleeWhisper")
    r3_1 = quest:GetThingWithScriptName("TheRealGuildmaster")
    quest:GiveHeroTitle("OBJECT_HERO_TITLE_CHICKEN_CHASER")
    quest:SetStateInt("PreMeleeMode", 0)
    scratchValue8 = resources:NewResource()
    while not resources:TryAcquire(scratchValue8, preMeleeMaze, 4) do
        if not quest:NewScriptFrame() then resources:ReleaseResource(scratchValue8); return end
    end
    if not quest:IsActiveThreadTerminating() then
        scratchValue7 = resources:NewResource()
        while not resources:TryAcquire(scratchValue7, r2_1, 4) do
            if not quest:NewScriptFrame() then goto LAB_00d51951 end
        end
        if not quest:IsActiveThreadTerminating() then
            scratchValue6 = resources:NewResource()
            while not resources:TryAcquire(scratchValue6, r3_1, 4) do
                if not quest:NewScriptFrame() then goto LAB_00d51948 end
            end
            if not quest:IsActiveThreadTerminating() then
                scratchValue4 = resources:NewResource()
                while not resources:TryAcquire(scratchValue4, quest:GetHero(), 4) do
                    if not quest:NewScriptFrame() then goto LAB_00d5193f end
                end
                if not quest:IsActiveThreadTerminating() then
                    scratchValue5 = resources:NewActorMap()
                    resources:SetActor(scratchValue5, "HERO", scratchValue4)
                    resources:SetActor(scratchValue5, "MAZE", scratchValue8)
                    resources:SetActor(scratchValue5, "WHISPER", scratchValue7)
                    resources:SetActor(scratchValue5, "MASTER", scratchValue6)
                    scratchValue3 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GUILD_PREMELEE_INTRO", scratchValue5, false, true)
                    resources:RunMacro("CS_GUILD_PREMELEE_BOOHOO", scratchValue5, false, true)
                    resources:RunMacro("CS_GUILD_PREMELEE_WAKEUP", scratchValue5, false, true)
                    quest:FixMovieSequenceCamera(false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                    quest:ResetPlayerCreatureCombatMultiplier()
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(scratchValue3)
                    resources:DestroyActorMap(scratchValue5)
                    resources:ReleaseResource(scratchValue4)
                    resources:ReleaseResource(scratchValue6)
                    resources:ReleaseResource(scratchValue7)
                    resources:DestroyMovie(scratchValue8)
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
                resources:ReleaseResource(scratchValue4)
            end
            ::LAB_00d51948::
            resources:ReleaseResource(scratchValue6)
        end
        ::LAB_00d51951::
        resources:ReleaseResource(scratchValue7)
    end
    resources:ReleaseResource(scratchValue8)
    ::LAB_00d51de2::
end

-- Q_GuildTrainingPreMelee.Init (retail 0x00d51450)
function Init(quest)
    quest:SetStateBool("HeroSleeps", false)
    quest:SetStateBool("WhisperCutsceneFinished", false)
    quest:SetStateBool("GuildmasterTeleport", false)
    quest:SetStateBool("WhisperStopFollowing", false)
end

