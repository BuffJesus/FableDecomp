-- Generated native draft: Q_GuildTrainingPreMelee. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local bVar6, cVar1, delay, iVar5, iVar8, pCVar3, pCVar7, pQuestName, r1, r2, r3, xStack_10, xStack_20, xStack_2c, xStack_3c, xStack_4c, xStack_5c, xStack_88
    local alive = true
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingPreMelee/Entities/TheRealGuildmaster")
    quest:AddEntityBinding("PreMeleeDummy", "GuildTrainingPreMelee/Entities/PreMeleeDummy")
    quest:AddEntityBinding("PreMeleeWhisper", "GuildTrainingPreMelee/Entities/PreMeleeWhisper")
    quest:FinalizeEntityBindings()
    quest:SetMasterGameState("GuildWarningOccuring", true)
    r1 = quest:GetThingWithScriptName("PreMeleeMaze")
    r2 = quest:GetThingWithScriptName("PreMeleeWhisper")
    r3 = quest:GetThingWithScriptName("TheRealGuildmaster")
    quest:GiveHeroTitle("OBJECT_HERO_TITLE_CHICKEN_CHASER")
    quest:SetStateInt("PreMeleeMode", 0)
    xStack_5c = resources:NewResource()
    bVar6 = false
    if bVar6 ~= 0 then
    end
    bVar6 = resources:TryAcquire(xStack_5c, r1, 4)
    while not bVar6 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            resources:ReleaseResource(xStack_5c)
            r3 = nil
            r2 = nil
            r1 = nil
            return
        end
        bVar6 = resources:TryAcquire(xStack_5c, r1, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if not bVar6 then
        xStack_4c = resources:NewResource()
        bVar6 = false
        if bVar6 ~= 0 then
        end
        bVar6 = resources:TryAcquire(xStack_4c, r2, 4)
        while not bVar6 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then goto LAB_00d51951 end
            bVar6 = resources:TryAcquire(xStack_4c, r2, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if not bVar6 then
            xStack_3c = resources:NewResource()
            bVar6 = false
            if bVar6 ~= 0 then
            end
            bVar6 = resources:TryAcquire(xStack_3c, r3, 4)
            while not bVar6 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00d51948 end
                bVar6 = resources:TryAcquire(xStack_3c, r3, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                xStack_20 = resources:NewResource()
                bVar6 = false
                if bVar6 ~= 0 then
                end
                iVar8 = 4
                pCVar7 = xStack_20
                pCVar3 = quest:GetHero()
                bVar6 = resources:TryAcquire(pCVar7, pCVar3, iVar8)
                while not bVar6 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00d5193f end
                    iVar8 = 4
                    pCVar7 = xStack_20
                    pCVar3 = quest:GetHero()
                    bVar6 = resources:TryAcquire(pCVar7, pCVar3, iVar8)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    xStack_2c = resources:NewActorMap()
                    resources:SetActor(xStack_2c, "HERO", xStack_20)
                    resources:SetActor(xStack_2c, "MAZE", xStack_5c)
                    resources:SetActor(xStack_2c, "WHISPER", xStack_4c)
                    resources:SetActor(xStack_2c, "MASTER", xStack_3c)
                    xStack_10 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GUILD_PREMELEE_INTRO", xStack_2c, false, true)
                    resources:RunMacro("CS_GUILD_PREMELEE_BOOHOO", xStack_2c, false, true)
                    resources:RunMacro("CS_GUILD_PREMELEE_WAKEUP", xStack_2c, false, true)
                    quest:FixMovieSequenceCamera(false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                    quest:ResetPlayerCreatureCombatMultiplier()
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_10)
                    resources:DestroyActorMap(xStack_2c)
                    resources:ReleaseResource(xStack_20)
                    resources:ReleaseResource(xStack_3c)
                    resources:ReleaseResource(xStack_4c)
                    resources:ReleaseResource(xStack_5c)
                    quest:SetStateBool("WhisperCutsceneFinished", true)
                    quest:SetStateBool("GuildmasterTeleport", true)
                    quest:SetMasterGameState("GuildWarningOccuring", false)
                    xStack_88 = quest:RegisterTimer()
                    quest:SetTimer(xStack_88, 5)
                    iVar5 = quest:GetTimer(xStack_88)
                    while 0 < iVar5 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00d51dd9 end
                        iVar5 = quest:GetTimer(xStack_88)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if not bVar6 then
                        quest:GiveHeroExpression("EXPRESSION_FART", -1, true)
                        quest:GiveHeroExpression("EXPRESSION_BELCH", -1, true)
                        quest:GiveHeroExpression("EXPRESSION_GIGGLE", -1, true)
                        cVar1 = quest:GetStateBool("HeroSleeps")
                        while not cVar1 do
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00d51dd9 end
                            cVar1 = quest:GetStateBool("HeroSleeps")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            delay = 0
                            pQuestName = quest:GetActiveQuestName()
                            quest:DeactivateQuestLater(pQuestName, delay)
                        end
                    end
                    ::LAB_00d51dd9::
                    quest:DeregisterTimer(xStack_88)
                    goto LAB_00d51de2
                end
                ::LAB_00d5193f::
                resources:ReleaseResource(xStack_20)
            end
            ::LAB_00d51948::
            resources:ReleaseResource(xStack_3c)
        end
        ::LAB_00d51951::
        resources:ReleaseResource(xStack_4c)
    end
    resources:ReleaseResource(xStack_5c)
    ::LAB_00d51de2::
end

function Init(quest)
    quest:SetStateBool("HeroSleeps", false)
    quest:SetStateBool("WhisperCutsceneFinished", false)
    quest:SetStateBool("GuildmasterTeleport", false)
    quest:SetStateBool("WhisperStopFollowing", false)
end

