-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local CUTSCENE_BEHAVIOUR_NOT_PAUSED = 2  -- ECutsceneBehaviour (Ego_r.pdb)
local TUTORIAL_CATEGORY_QUEST_CARD = 28  -- ETutorialCategory (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local woodsEndPlayed, chatJumped

-- TheRealGuildmaster.Main (retail 0x00d52e90)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local addQuestInfoCounter, addQuestInfoCounter2, isActiveThreadTerminating, guildmasterTeleport
    local scratchValue6, scratchValue7, ctr_154, dummyHits, dummyHits2, questionAnswer
    local questionAnswer2, questionAnswer3, conversationId, switch, getPos, timerId, scratchValue21
    local newActorMap, movie, actorMap, resource2, timerId3, timerId4
    guildmasterTeleport = quest:GetStateBool("GuildmasterTeleport")
    scratchValue21 = 0
    while not guildmasterTeleport do
        if not quest:NewScriptFrame(me) then return end
        guildmasterTeleport = quest:GetStateBool("GuildmasterTeleport")
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_01", "", "")
    local resource3 = resources:NewResource()
    resources:PrepareResource(resource3)
    while not resources:TryAcquire(resource3, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource3); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); return end
    quest:EntitySetAsKillable(me, false, true)
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntityTeleportToThing(me, quest:GetThingWithScriptName("M_MeleeTeacherStand"), false)
    local timerId2 = quest:RegisterTimer()
    quest:SetTimer(timerId2, 0)
    scratchValue7 = 1
    repeat
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource3); return end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource3); return end
            scratchValue7 = 0
        end
        if not quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) or 0 < quest:GetTimer(timerId2) then goto FLOW_native_label_1 end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource3); return end
        conversationId = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId, hero)
        quest:SetTimer(timerId2, 5)
        switch = scratchValue21
        repeat
            if switch == 0 then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FIRST", me, hero, false)
                me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
                scratchValue21 = 1
                break
            else
                if switch == 1 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_SECOND", me, hero, false)
                    goto LAB_00d53316
                elseif switch == 2 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_THIRD", me, hero, false)
                    scratchValue21 = 3
                    break
                elseif switch == 3 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FOURTH", me, hero, false)
                    goto LAB_00d53316
                end
                goto FLOW_past_lab_00d53316
                ::LAB_00d53316::
                scratchValue21 = 2
                ::FLOW_past_lab_00d53316::
            end
        until true
        ::FLOW_native_label_1::
    until scratchValue7 == 0
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource3); return end
    quest:SetStateBool("WhisperStopFollowing", true)
    resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, hero, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d533bb end
    end
    if quest:IsActiveThreadTerminating() then
        goto LAB_00d533bb
    else
        actorMap = resources:NewActorMap()
        resources:SetActor(actorMap, "HERO", resource2)
        resources:SetActor(actorMap, "TEACHER", resource3)
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_GUILD_PREMELEE_PUNCH", actorMap, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:RemoveThing(quest:GetThingWithScriptName("PreMeleeWhisper"), false, true)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource2)
        if quest:IsXbox() then
            if not quest:IsActiveThreadTerminating() then
                quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH")
                while not quest:MsgIsGameInfoClickedPast() do
                    if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource3); return end
                end
                if not quest:IsActiveThreadTerminating() then
                    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_LOCKINGON")
                    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_PUNCHING")
                    goto LAB_00d536c0
                end
            end
        elseif not quest:IsActiveThreadTerminating() then
            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH_PC")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource3); return end
            end
            if not quest:IsActiveThreadTerminating() then
                quest:AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_LOCKINGON")
                quest:AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_PUNCHING")
                goto LAB_00d536c0
            end
        end
        goto FLOW_past_lab_00d536c0
        ::LAB_00d536c0::
        timerId4 = quest:RegisterTimer()
        timerId = timerId4
        quest:SetTimer(timerId4, 10)
        quest:SetStateInt("PreMeleeMode", 1)
        quest:SetStateInt("DummyHits", 0)
        addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
        quest:DisplayQuestInfo(true)
        dummyHits = quest:GetStateInt("DummyHits")
        timerId3 = 0
        while dummyHits < 7 do
            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
            quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("DummyHits"), -1)
            if timerId3 ~= quest:GetStateInt("DummyHits") then
                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                timerId3 = quest:GetStateInt("DummyHits")
                quest:SetTimer(timerId, 10)
            end
            if quest:GetTimer(timerId) >= 1 then
                dummyHits = quest:GetStateInt("DummyHits")
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                local conversationId2 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId2, hero)
                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_GUILDMASTER_PREMELEE_HIT_DUMMY", me, hero, false)
                if quest:IsXbox() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                quest:SetTimer(timerId4, 10)
                timerId = timerId4
                dummyHits = quest:GetStateInt("DummyHits")
            end
        end
        if not quest:IsActiveThreadTerminating() then
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            quest:DisplayQuestInfo(false)
            resource2 = resources:NewResource()
            resources:PrepareResource(resource2)
            while not resources:TryAcquire(resource2, hero, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00d53a0b end
            end
            if quest:IsActiveThreadTerminating() then
                goto LAB_00d53a0b
            else
                actorMap = resources:NewActorMap()
                resources:SetActor(actorMap, "HERO", resource2)
                resources:SetActor(actorMap, "TEACHER", resource3)
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_GUILD_PREMELEE_STICK", actorMap, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:DestroyActorMap(actorMap)
                resources:ReleaseResource(resource2)
                if quest:IsXbox() then
                    if not quest:IsActiveThreadTerminating() then
                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                        end
                        if not quest:IsActiveThreadTerminating() then quest:AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_MELEE"); goto LAB_00d53c7e end
                    end
                elseif not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                    end
                    if not quest:IsActiveThreadTerminating() then quest:AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_MELEE"); goto LAB_00d53c7e end
                end
                goto FLOW_past_lab_00d53c7e
                ::LAB_00d53c7e::
                quest:SetStateInt("PreMeleeMode", 2)
                quest:SetStateInt("DummyHits", 0)
                quest:SetTimer(timerId, 10)
                addQuestInfoCounter2 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                quest:DisplayQuestInfo(true)
                dummyHits2 = quest:GetStateInt("DummyHits")
                timerId3 = 0
                while dummyHits2 < 7 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                    quest:UpdateQuestInfoCounter(addQuestInfoCounter2, quest:GetStateInt("DummyHits"), -1)
                    if timerId3 ~= quest:GetStateInt("DummyHits") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                        timerId3 = quest:GetStateInt("DummyHits")
                        quest:SetTimer(timerId, 10)
                    end
                    if quest:GetTimer(timerId) >= 1 then
                        dummyHits2 = quest:GetStateInt("DummyHits")
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                        local conversationId3 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId3, hero)
                        quest:AddLineToConversation(conversationId3, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, hero, false)
                        if quest:IsXbox() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                            quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                            end
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                            quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                        quest:SetTimer(timerId4, 10)
                        timerId = timerId4
                        dummyHits2 = quest:GetStateInt("DummyHits")
                    end
                end
                if not quest:IsActiveThreadTerminating() then
                    quest:RemoveQuestInfoElement(addQuestInfoCounter2)
                    quest:DisplayQuestInfo(false)
                    movie = resources:StartMovie("")
                    resource2 = resources:NewResource()
                    resources:PrepareResource(resource2)
                    while not resources:TryAcquire(resource2, hero, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00d53ff2 end
                    end
                    if quest:IsActiveThreadTerminating() then
                        goto LAB_00d53ff2
                    else
                        local preMeleeDummy = quest:GetThingWithScriptName("PreMeleeDummy")
                        if preMeleeDummy == nil then
                            getPos = {x = 0, y = 0, z = 0}
                        else
                            getPos = preMeleeDummy:GetPos()
                        end
                        newActorMap = {x = getPos.x, y = getPos.y, z = getPos.z}
                        quest:CreateEffectAtPos("SMASH_DUMMY_01", newActorMap, 0.0, false)
                        quest:FadeOutAndKillEntity(preMeleeDummy, true, 1.0, true)
                        actorMap = resources:NewActorMap()
                        resources:SetActor(actorMap, "HERO", resource2)
                        resources:SetActor(actorMap, "TEACHER", resource3)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", actorMap, false, false)
                        quest:PauseAllNonScriptedEntities(true)
                        local scratchValue22 = quest:CreateExperienceOrb(newActorMap, 1)
                        quest:EntitySetCutsceneBehaviour(scratchValue22, CUTSCENE_BEHAVIOUR_NOT_PAUSED)
                        resources:RunMacro("CS_GUILD_PREMELEE_PASSED", actorMap, false, true)
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyActorMap(actorMap)
                        resources:ReleaseResource(resource2)
                        resources:DestroyMovie(movie)
                        if quest:IsXbox() then
                            if not quest:IsActiveThreadTerminating() then
                                quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                while not quest:MsgIsGameInfoClickedPast() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c34 end
                                end
                                if not quest:IsActiveThreadTerminating() then quest:AddLogbookTutorialEntry("TEXT_QST_LOG_HERO_EXPERIENCEORBS"); goto LAB_00d5439e end
                            end
                        elseif not quest:IsActiveThreadTerminating() then
                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_PC")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d55c34 end
                            end
                            if not quest:IsActiveThreadTerminating() then quest:AddLogbookTutorialEntryPC("TEXT_QST_LOG_HERO_EXPERIENCEORBS"); goto LAB_00d5439e end
                        end
                        goto FLOW_past_lab_00d5439e
                        ::LAB_00d5439e::
                        timerId3 = quest:RegisterTimer()
                        quest:SetTimer(timerId3, 10)
                        while scratchValue22 ~= nil and scratchValue22:IsAlive() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                            if quest:GetTimer(timerId3) < 1 then
                                local conversationId4 = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(conversationId4, hero)
                                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, hero, false)
                                quest:SetTimer(timerId3, 10)
                            end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            quest:Pause(0.5)
                            resource2 = resources:NewResource()
                            resources:PrepareResource(resource2)
                            while not resources:TryAcquire(resource2, hero, 4) do
                                if not quest:NewScriptFrame(me) then goto LAB_00d54dfa end
                            end
                            if quest:IsActiveThreadTerminating() then
                                goto LAB_00d54dfa
                            else
                                newActorMap = resources:NewActorMap()
                                resources:SetActor(newActorMap, "HERO", resource2)
                                resources:SetActor(newActorMap, "TEACHER", resource3)
                                movie = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_PREMELEE_ALARM", newActorMap, false, true)
                                quest:FixMovieSequenceCamera(false)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                resources:DestroyActorMap(newActorMap)
                                resources:ReleaseResource(resource2)
                                quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", "Q_GuildTrainingWoodsMelee", false)
                                quest:SetQuestCardObjective("Q_GuildTrainingWoodsMelee", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                quest:KickOffQuestStartScreen("Q_GuildTrainingWoodsMelee", true, false)
                                quest:NewScriptFrame(me)
                                if not quest:IsActiveThreadTerminating() then
                                    quest:NewScriptFrame(me)
                                    if not quest:IsActiveThreadTerminating() then
                                        quest:NewScriptFrame(me)
                                        if not quest:IsActiveThreadTerminating() then
                                            quest:NewScriptFrame(me)
                                            if not quest:IsActiveThreadTerminating() then
                                                quest:NewScriptFrame(me)
                                                if not quest:IsActiveThreadTerminating() then
                                                    if not quest:DisplayTutorial(TUTORIAL_CATEGORY_QUEST_CARD) then goto LAB_00d54846 end
                                                    if not quest:IsActiveThreadTerminating() then
                                                        while not quest:MsgIsTutorialClickedPast() do
                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                        end
                                                        if not quest:IsActiveThreadTerminating() then goto LAB_00d54846 end
                                                    end
                                                    goto FLOW_past_lab_00d54846
                                                    ::LAB_00d54846::
                                                    quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                    quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, ENTITY_MOVE_WALK, false, true)
                                                    scratchValue7 = 1
                                                    ctr_154 = 0
                                                    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                    scratchValue6 = 0
                                                    repeat
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                        if not quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") then goto LAB_00d54f9c end
                                                        if not woodsEndPlayed then
                                                            woodsEndPlayed = true
                                                            quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                            local resource = resources:NewResource()
                                                            resources:PrepareResource(resource)
                                                            while not resources:TryAcquire(resource, hero, 4) do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c91 end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c91 end
                                                            goto FLOW_past_lab_00d55c91
                                                            ::LAB_00d55c91::
                                                            resources:ReleaseResource(resource)
                                                            goto LAB_00d55c2b
                                                            ::FLOW_past_lab_00d55c91::
                                                            actorMap = resources:NewActorMap()
                                                            resources:SetActor(actorMap, "HERO", resource)
                                                            resources:SetActor(actorMap, "GUARD", resource3)
                                                            movie = resources:StartMovie("")
                                                            quest:PauseAllNonScriptedEntities(true)
                                                            quest:FixMovieSequenceCamera(true)
                                                            resources:RunMacro("CS_GUILD_MELEE_WOODSWON", actorMap, false, true)
                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                                                            while questionAnswer < 0 do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c72 end
                                                                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                                                            end
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c72 end
                                                            goto FLOW_past_lab_00d55c72
                                                            ::LAB_00d55c72::
                                                            quest:PauseAllNonScriptedEntities(false)
                                                            ::LAB_00d55c7f::
                                                            resources:DestroyMovie(movie)
                                                            resources:DestroyActorMap(actorMap)
                                                            goto LAB_00d55c91
                                                            ::FLOW_past_lab_00d55c72::
                                                            if questionAnswer == 1 then
                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                quest:Pause(1.0)
                                                                quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                scratchValue7 = 0
                                                            else
                                                                local fret_0 = quest:GetHealth(resources:ScriptThing(resource3))
                                                                if 0.0 < fret_0 then
                                                                    if not me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d55c72 end
                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c72 end
                                                                end
                                                                local mkGtmWdGuard = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                me:MoveToPosition(mkGtmWdGuard:GetPos(), 1.0, ENTITY_MOVE_WALK, false, true)
                                                                scratchValue6 = 0
                                                            end
                                                            quest:FixMovieSequenceCamera(false)
                                                            quest:PauseAllNonScriptedEntities(false)
                                                            resources:DestroyMovie(movie)
                                                            resources:DestroyActorMap(actorMap)
                                                            resources:ReleaseResource(resource)
                                                        else
                                                            if not quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") then
                                                                goto LAB_00d54f9c
                                                            else
                                                                if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then goto LAB_00d54f9c end
                                                                isActiveThreadTerminating = true
                                                            end
                                                            goto FLOW_hoist_lab_00d54f9c_1
                                                        end
                                                        goto FLOW_past_lab_00d54f9c
                                                        ::LAB_00d54f9c::
                                                        isActiveThreadTerminating = false
                                                        ::FLOW_hoist_lab_00d54f9c_1::
                                                        if isActiveThreadTerminating then
                                                            resources:PrepareResource(resource3)
                                                            while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                            resources:PrepareResource(resource3)
                                                            while not resources:TryAcquire(resource3, me, 4) do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                            resource2 = resources:StartMovie("")
                                                            quest:PauseAllNonScriptedEntities(true)
                                                            local fret_00 = quest:GetHealth(resources:ScriptThing(resource3))
                                                            if fret_00 <= 0.0 then
                                                                goto LAB_00d551d4
                                                            else
                                                                if not me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d55cba end
                                                                if not quest:IsActiveThreadTerminating() then goto LAB_00d551d4 end
                                                            end
                                                            goto FLOW_past_lab_00d551d4
                                                            ::LAB_00d551d4::
                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                            questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                            while questionAnswer2 < 0 do
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55cba end
                                                                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                            end
                                                            if not quest:IsActiveThreadTerminating() then
                                                                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                                                                if questionAnswer2 == 1 then
                                                                    if isActiveThreadTerminating then
                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                        resources:DestroyMovie(resource2)
                                                                        goto LAB_00d55c2b
                                                                    end
                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                    quest:Pause(1.0)
                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                    scratchValue7 = 0
                                                                else
                                                                    if isActiveThreadTerminating then goto LAB_00d55cba end
                                                                    local fret_01 = quest:GetHealth(resources:ScriptThing(resource3))
                                                                    if 0.0 < fret_01 then
                                                                        if not me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d55cba end
                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55cba end
                                                                    end
                                                                    local mkGtmWdGuard2 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                    me:MoveToPosition(mkGtmWdGuard2:GetPos(), 1.0, ENTITY_MOVE_WALK, false, true)
                                                                    scratchValue6 = 0
                                                                end
                                                                quest:PauseAllNonScriptedEntities(false)
                                                                resources:DestroyMovie(resource2)
                                                                goto LAB_00d55480
                                                            end
                                                            ::FLOW_past_lab_00d551d4::
                                                            ::LAB_00d55cba::
                                                            quest:PauseAllNonScriptedEntities(false)
                                                            resources:DestroyMovie(resource2)
                                                            goto LAB_00d55c2b
                                                        end
                                                        ::FLOW_past_lab_00d54f9c::
                                                        ::LAB_00d55480::
                                                        if me:IsTalkedToByHero() then
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                            local movie2 = resources:StartMovie("")
                                                            quest:PauseAllNonScriptedEntities(true)
                                                            me:ClearCommands()
                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") then
                                                                if not quest:IsActiveThreadTerminating() then
                                                                    local fret_03 = quest:GetHealth(resources:ScriptThing(resource3))
                                                                    if 0.0 < fret_03 then
                                                                        if not me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d555f3 end
                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55cd5 end
                                                                    end
                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                    questionAnswer3 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                    while questionAnswer3 < 0 do
                                                                        if not quest:NewScriptFrame(me) then goto LAB_00d555f3 end
                                                                        questionAnswer3 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                    end
                                                                    if not quest:IsActiveThreadTerminating() then
                                                                        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                                                                        if questionAnswer3 == 1 then
                                                                            if not isActiveThreadTerminating then
                                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                                quest:Pause(1.0)
                                                                                quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                scratchValue7 = 0
                                                                                goto LAB_00d5595a
                                                                            end
                                                                            goto LAB_00d555f3
                                                                        end
                                                                        if not isActiveThreadTerminating then
                                                                            local fret_04 = quest:GetHealth(resources:ScriptThing(resource3))
                                                                            if 0.0 < fret_04 then
                                                                                if not me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d555f3 end
                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55cd5 end
                                                                            end
                                                                            local mkGtmWdGuard3 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                            me:MoveToPosition(mkGtmWdGuard3:GetPos(), 1.0, ENTITY_MOVE_WALK, false, true)
                                                                            scratchValue6 = 0
                                                                            goto LAB_00d5595a
                                                                        end
                                                                    end
                                                                end
                                                                goto LAB_00d55cd5
                                                            end
                                                            goto FLOW_past_lab_00d55cd5
                                                            ::LAB_00d55cd5::
                                                            quest:PauseAllNonScriptedEntities(false)
                                                            resources:DestroyMovie(movie2)
                                                            goto LAB_00d55c2b
                                                            ::FLOW_past_lab_00d55cd5::
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d555f3 end
                                                            goto FLOW_past_lab_00d555f3
                                                            ::LAB_00d555f3::
                                                            quest:PauseAllNonScriptedEntities(false)
                                                            resources:DestroyMovie(movie2)
                                                            goto LAB_00d55c2b
                                                            ::FLOW_past_lab_00d555f3::
                                                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                                                                if not me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d55cd5 end
                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d555f3 end
                                                            end
                                                            ::LAB_00d5595a::
                                                            quest:PauseAllNonScriptedEntities(false)
                                                            resources:DestroyMovie(movie2)
                                                        end
                                                        if scratchValue7 ~= 0 then
                                                            if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId2) < 1) and not me:IsPerformingScriptTask() then
                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                local conversationId5 = quest:AddNewConversation(me, false, false)
                                                                quest:AddPersonToConversation(conversationId5, hero)
                                                                quest:SetTimer(timerId2, 10)
                                                                if not quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") then
                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                    if ctr_154 == 1 then
                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                        quest:AddLineToConversation(conversationId5, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                    elseif ctr_154 == 2 then
                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                        quest:AddLineToConversation(conversationId5, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, hero, false)
                                                                    end
                                                                    ctr_154 = 1 - ctr_154
                                                                else
                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                    quest:AddLineToConversation(conversationId5, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                end
                                                            end
                                                        end
                                                        if scratchValue6 == 0 then
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                                            if not me:IsPerformingScriptTask() then
                                                                scratchValue6 = 1
                                                                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                            end
                                                        end
                                                    until scratchValue7 == 0
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:SetStateBool("HeroSleeps", true)
                                                        quest:FadeScreenOut(0.5, 0.5)
                                                        quest:SetTimeOfDay(11.0)
                                                        quest:ChangeHeroHealthBy(1000.0, true, false)
                                                        quest:ResetPlayerCreatureCombatMultiplier()
                                                    end
                                                    ::FLOW_past_lab_00d54846::
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                            goto FLOW_past_lab_00d54dfa
                            ::LAB_00d54dfa::
                            resources:ReleaseResource(resource2)
                            ::FLOW_past_lab_00d54dfa::
                        end
                        ::LAB_00d55c2b::
                        quest:DeregisterTimer(timerId3)
                        ::FLOW_past_lab_00d5439e::
                    end
                    goto FLOW_past_lab_00d53ff2
                    ::LAB_00d53ff2::
                    resources:ReleaseResource(resource2)
                    resources:DestroyMovie(movie)
                    ::FLOW_past_lab_00d53ff2::
                    ::LAB_00d55c34::
                end
                ::FLOW_past_lab_00d53c7e::
            end
            goto FLOW_past_lab_00d53a0b
            ::LAB_00d53a0b::
            resources:ReleaseResource(resource2)
            ::FLOW_past_lab_00d53a0b::
        end
        ::LAB_00d55c3d::
        quest:DeregisterTimer(timerId4)
        ::FLOW_past_lab_00d536c0::
    end
    quest:DeregisterTimer(timerId2)
    resources:ReleaseResource(resource3)
    do return end
    ::LAB_00d533bb::
    resources:ReleaseResource(resource2)
    quest:DeregisterTimer(timerId2)
    resources:ReleaseResource(resource3)
end

-- TheRealGuildmaster.Init (retail 0x00d51ed0)
function Init(quest, me)
    chatJumped = false
    woodsEndPlayed = false
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

