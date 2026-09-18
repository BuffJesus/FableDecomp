-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local woodsEndPlayed, chatJumped

-- TheRealGuildmaster.Main (retail 0x00d52e90)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local addQuestInfoCounter, scratchValue, scratchValue13, scratchValue14, scratchValue15, ctr_154
    local scratchValue19, addNewConversation, switch, scratchValue20, scratchValue21, scratchValue23
    local scratchValue24, scratchValue25, timerId, scratchValue26, resource, actorMap, movie
    local infoCounter, actorMap2, scratchValue31, scratchValue32, resource2, timerId2, timerId3
    local timerId4
    local function __region_LAB_00d555f3_c27()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d555f3_c28()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d555f3_c29()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d555f3_c6()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55c9f_c19()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55c9f_c27()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55c9f_c28()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55c9f_c29()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55c9f_c6()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55c9f_c7()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55cd5_c27()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55cd5_c28()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55cd5_c29()
        quest:PauseAllNonScriptedEntities(false)
    end
    local function __region_LAB_00d55cd5_c6()
        quest:PauseAllNonScriptedEntities(false)
    end
    scratchValue13 = quest:GetStateBool("GuildmasterTeleport")
    scratchValue26 = 0
    while not scratchValue13 do
        if not quest:NewScriptFrame(me) then return end
        scratchValue13 = quest:GetStateBool("GuildmasterTeleport")
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_01", "", "")
    resource2 = resources:NewResource()
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d55c4f end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d55c4f end
    quest:EntitySetAsKillable(me, false, true)
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    scratchValue20 = quest:GetThingWithScriptName("M_MeleeTeacherStand")
    quest:EntityTeleportToThing(me, scratchValue20, false)
    timerId4 = quest:RegisterTimer()
    quest:SetTimer(timerId4, 0)
    scratchValue15 = 1
    repeat
        if not quest:NewScriptFrame(me) then goto LAB_00d55c46 end
        if me:IsTalkedToByHero() then
            scratchValue15 = 0
        end
        if not quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) or 0 < quest:GetTimer(timerId4) then goto FLOW_native_label_1 end
        addNewConversation = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(addNewConversation, hero)
        quest:SetTimer(timerId4, 5)
        switch = scratchValue26
        repeat
            if switch == 0 then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FIRST", me, hero, false)
                me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
                scratchValue26 = 1
                break
            else
                if switch == 1 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_SECOND", me, hero, false)
                    scratchValue26 = 2
                    goto FLOW_after_lab_00d53316
                elseif switch == 2 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_THIRD", me, hero, false)
                    scratchValue26 = 3
                    break
                elseif switch == 3 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FOURTH", me, hero, false)
                    scratchValue26 = 2
                end
                ::FLOW_after_lab_00d53316::
            end
        until true
        ::FLOW_native_label_1::
    until scratchValue15 == 0
    if quest:IsActiveThreadTerminating() then goto LAB_00d55c46 end
    quest:SetStateBool("WhisperStopFollowing", true)
    scratchValue31 = resources:NewResource()
    while not resources:TryAcquire(scratchValue31, hero, 4) do
        quest:NewScriptFrame(me)
        if not quest:IsActiveThreadTerminating() then
            SUB(scratchValue31,0)
        else
            resources:DestroyMovie(scratchValue31)
            goto FLOW_after_lab_00d533bb
            SUB(scratchValue31,0)
        end
    end
    if quest:IsActiveThreadTerminating() then
        resources:DestroyMovie(scratchValue31)
    else
        actorMap2 = resources:NewActorMap()
        resources:SetActor(actorMap2, "HERO", scratchValue31)
        resources:SetActor(actorMap2, "TEACHER", resource2)
        movie = resources:StartMovie("")
        quest:StartMovieSequence()
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_GUILD_PREMELEE_PUNCH", actorMap2, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:RemoveThing(quest:GetThingWithScriptName("PreMeleeWhisper"), false, true)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap2)
        resources:DestroyMovie(scratchValue31)
        if quest:IsXbox() then
            if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d536c0 end
            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame(me) then goto LAB_00d55c46 end
            end
            if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d536c0 end
            timerId3 = quest:RegisterTimer()
            timerId = timerId3
            quest:SetTimer(timerId3, 10)
            quest:SetStateInt("PreMeleeMode", 1)
            quest:SetStateInt("DummyHits", 0)
            addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
            infoCounter = addQuestInfoCounter
            quest:DisplayQuestInfo(true)
            while quest:GetStateInt("DummyHits") < 7 do
                if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("DummyHits"), -1)
                if 0 ~= quest:GetStateInt("DummyHits") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                    -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                    quest:SetTimer(timerId, 10)
                end
                if quest:GetTimer(timerId) >= 1 then goto continue_2 end
                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_HIT_DUMMY", me, hero, false)
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
                quest:SetTimer(timerId3, 10)
                timerId = timerId3
                addQuestInfoCounter = infoCounter
                ::continue_2::
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            quest:DisplayQuestInfo(false)
            scratchValue31 = resources:NewResource()
            SUB(scratchValue31,0)
            while not resources:TryAcquire(scratchValue31, hero, 4) do
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then
                    SUB(scratchValue31,0)
                else
                    resources:DestroyMovie(scratchValue31)
                    goto FLOW_after_lab_00d53a0b
                    SUB(scratchValue31,0)
                end
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d53a0b: (native jump target)
                resources:DestroyMovie(scratchValue31)
            else
                actorMap2 = resources:NewActorMap()
                resources:SetActor(actorMap2, "HERO", scratchValue31)
                resources:SetActor(actorMap2, "TEACHER", resource2)
                movie = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_GUILD_PREMELEE_STICK", actorMap2, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:DestroyActorMap(actorMap2)
                resources:DestroyMovie(scratchValue31)
                if quest:IsXbox() then
                    if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d53c7e end
                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                    end
                    if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d53c7e end
                    -- LAB_00d53c7e: (native jump target)
                    quest:SetStateInt("PreMeleeMode", 2)
                    quest:SetStateInt("DummyHits", 0)
                    quest:SetTimer(timerId, 10)
                    addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                    infoCounter = addQuestInfoCounter
                    quest:DisplayQuestInfo(true)
                    while quest:GetStateInt("DummyHits") < 7 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                        quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("DummyHits"), -1)
                        if 0 ~= quest:GetStateInt("DummyHits") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                            -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                            quest:SetTimer(timerId, 10)
                        end
                        if quest:GetTimer(timerId) >= 1 then goto continue_4 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                        addNewConversation = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(addNewConversation, hero)
                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, hero, false)
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
                        quest:SetTimer(timerId3, 10)
                        timerId = timerId3
                        addQuestInfoCounter = infoCounter
                        ::continue_4::
                    end
                    if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d53c7e end
                    quest:RemoveQuestInfoElement(addQuestInfoCounter)
                    quest:DisplayQuestInfo(false)
                    scratchValue32 = nil
                    movie = resources:StartMovie("")
                    quest:StartMovieSequence()
                    scratchValue31 = resources:NewResource()
                    SUB(scratchValue31,0)
                    scratchValue = hero:AcquireControl(4)
                    while not scratchValue do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            resources:DestroyMovie(scratchValue31)
                            resources:DestroyMovie(movie)
                            goto FLOW_after_lab_00d53ff2
                        end
                        SUB(scratchValue31,0)
                        scratchValue = resources:TryAcquire(scratchValue31, hero, 4)
                    end
                    if quest:IsActiveThreadTerminating() then
                        -- LAB_00d53ff2: (native jump target)
                        resources:DestroyMovie(scratchValue31)
                        resources:DestroyMovie(movie)
                    else
                        scratchValue23 = quest:GetThingWithScriptName("PreMeleeDummy")
                        -- TODO(native): CStack_114._0_4_ = *puVar11;
                        -- TODO(native): CStack_114._4_4_ = puVar11[1];
                        -- TODO(native): CStack_114._8_4_ = puVar11[2];
                        -- TODO(native): CreateEffect is not a ForgeFSE binding
                        quest:CreateEffect(xStack_dc, "SMASH_DUMMY_01", actorMap, "", 0.0, false, false)
                        quest:FadeOutAndKillEntity(scratchValue23, true, 1.0, true)
                        actorMap2 = resources:NewActorMap()
                        resources:SetActor(actorMap2, "HERO", scratchValue31)
                        resources:SetActor(actorMap2, "TEACHER", resource2)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", actorMap2, false, false)
                        quest:PauseAllNonScriptedEntities(true)
                        quest:CreateExperienceOrb(actorMap, 1)
                        -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                        quest:EntitySetCutsceneBehaviour(nil, 2)
                        resources:RunMacro("CS_GUILD_PREMELEE_PASSED", actorMap2, false, true)
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyActorMap(actorMap2)
                        resources:DestroyMovie(scratchValue31)
                        resources:DestroyMovie(movie)
                        if quest:IsXbox() then
                            if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d5439e end
                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d55c34 end
                            end
                            if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d5439e end
                            -- LAB_00d5439e: (native jump target)
                            timerId2 = quest:RegisterTimer()
                            quest:SetTimer(timerId2, 10)
                            while scratchValue32 ~= nil and scratchValue32:IsAlive() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                if quest:GetTimer(timerId2) < 1 then
                                    addNewConversation = quest:AddNewConversation(me, false, false)
                                    quest:AddPersonToConversation(addNewConversation, hero)
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, hero, false)
                                    quest:SetTimer(timerId2, 10)
                                end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                            quest:Pause(0.5)
                            scratchValue31 = resources:NewResource()
                            SUB(scratchValue31,0)
                            while not resources:TryAcquire(0, hero, 4) do
                                if not quest:NewScriptFrame(me) then goto FLOW_after_lab_00d54dfa end
                                SUB(scratchValue31,0)
                            end
                            if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d54dfa end
                            actorMap = resources:NewActorMap()
                            resources:SetActor(actorMap, "HERO", scratchValue31)
                            resources:SetActor(actorMap, "TEACHER", resource2)
                            resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:FixMovieSequenceCamera(true)
                            resources:RunMacro("CS_GUILD_PREMELEE_ALARM", actorMap, false, true)
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyActorMap(actorMap)
                            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", "Q_GuildTrainingWoodsMelee", false)
                            quest:SetQuestCardObjective("Q_GuildTrainingWoodsMelee", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                            quest:KickOffQuestStartScreen("Q_GuildTrainingWoodsMelee", true, false)
                            if not quest:NewScriptFrame(me) then goto FLOW_after_lab_00d54dfa end
                            if not quest:NewScriptFrame(me) then goto FLOW_after_lab_00d54dfa end
                            if not quest:NewScriptFrame(me) then goto FLOW_after_lab_00d54dfa end
                            if not quest:NewScriptFrame(me) then goto FLOW_after_lab_00d54dfa end
                            if not quest:NewScriptFrame(me) then goto FLOW_after_lab_00d54dfa end
                            if quest:DisplayTutorial(28) then
                                if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d54846 end
                                while not quest:MsgIsTutorialClickedPast() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                end
                                quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                ctr_154 = 0
                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                scratchValue14 = 0
                                repeat
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c6
                                    if not woodsEndPlayed then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                        woodsEndPlayed = true
                                        quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                        quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                        resource = resources:NewResource()
                                        SUB(resource,0)
                                        scratchValue = hero:AcquireControl(4)
                                        while not scratchValue do
                                            quest:NewScriptFrame(me)
                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c6
                                            SUB(resource,0)
                                            scratchValue = me:AcquireControl(4)
                                        end
                                        if quest:IsActiveThreadTerminating() then
                                            -- LAB_00d55c91_c6: (native jump target)
                                            goto LAB_00d55c2b
                                        end
                                        actorMap2 = resources:NewActorMap()
                                        resources:SetActor(actorMap2, "HERO", resource)
                                        resources:SetActor(actorMap2, "GUARD", resource2)
                                        resources:StartMovie("")
                                        quest:StartMovieSequence()
                                        quest:PauseAllNonScriptedEntities(true)
                                        quest:FixMovieSequenceCamera(true)
                                        resources:RunMacro("CS_GUILD_MELEE_WOODSWON", actorMap2, false, true)
                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        while scratchValue19 < 0 do
                                            quest:NewScriptFrame(me)
                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c6
                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        end
                                        if quest:IsActiveThreadTerminating() then
                                            -- LAB_00d55c72_c6: (native jump target)
                                            quest:PauseAllNonScriptedEntities(false)
                                            -- LAB_00d55c7f_c6: (native jump target)
                                            resources:DestroyActorMap(actorMap2)
                                            -- TODO(native): goto LAB_00d55c91_c6
                                        end
                                        scratchValue = quest:IsActiveThreadTerminating()
                                        if scratchValue19 == 1 then
                                            if scratchValue then
                                                quest:PauseAllNonScriptedEntities(false)
                                                -- TODO(native): goto LAB_00d55c7f_c6
                                            end
                                            quest:FadeScreenOut(0.5, 0.5)
                                            quest:Pause(1.0)
                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                        else
                                            if scratchValue then return end  -- TODO(native): goto LAB_00d55c72_c6
                                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                while me:IsPerformingScriptTask() do
                                                    quest:NewScriptFrame(me)
                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                end
                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c6
                                            end
                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                        end
                                        quest:FixMovieSequenceCamera(false)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyActorMap(actorMap2)
                                    else
                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                            -- LAB_00d54f9c_c6: (native jump target)
                                            scratchValue = false
                                        else
                                            if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then return end  -- TODO(native): goto LAB_00d54f9c_c6
                                            scratchValue = true
                                        end
                                        if scratchValue then
                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                            while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                            end
                                            while not resources:TryAcquire(resource2, me, 4) do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                            end
                                            resources:StartMovie("")
                                            quest:StartMovieSequence()
                                            quest:PauseAllNonScriptedEntities(true)
                                            if quest:GetHealth(resources:ScriptThing(resource2)) <= 0.0 then
                                                -- LAB_00d551d4_c6: (native jump target)
                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                while scratchValue19 < 0 do
                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                    if scratchValue19 == 1 then
                                                        if scratchValue then
                                                            __region_LAB_00d55c9f_c6()
                                                            goto LAB_00d55c2b
                                                        end
                                                        quest:FadeScreenOut(0.5, 0.5)
                                                        quest:Pause(1.0)
                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                    else
                                                        if scratchValue then goto LAB_00d55cba_c6 end
                                                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                            while me:IsPerformingScriptTask() do
                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c6 end
                                                        end
                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                    end
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    goto LAB_00d55480_c6
                                                end
                                            else
                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                while me:IsPerformingScriptTask() do
                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                end
                                                if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c6
                                            end
                                            ::LAB_00d55cba_c6::
                                            quest:PauseAllNonScriptedEntities(false)
                                            goto LAB_00d55c2b
                                        end
                                    end
                                    ::LAB_00d55480_c6::
                                    if me:IsTalkedToByHero() then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                        resources:StartMovie("")
                                        quest:StartMovieSequence()
                                        quest:PauseAllNonScriptedEntities(true)
                                        me:ClearCommands()
                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                            if not quest:IsActiveThreadTerminating() then
                                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                    while me:IsPerformingScriptTask() do
                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                    end
                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                end
                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                while scratchValue19 < 0 do
                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                    if scratchValue19 == 1 then
                                                        if not scratchValue then
                                                            quest:FadeScreenOut(0.5, 0.5)
                                                            quest:Pause(1.0)
                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                            goto LAB_00d5595a_c6
                                                        end
                                                        __region_LAB_00d555f3_c6(); goto LAB_00d55c2b
                                                    end
                                                    if not scratchValue then
                                                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                            while me:IsPerformingScriptTask() do
                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                        end
                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                        goto LAB_00d5595a_c6
                                                    end
                                                end
                                            end
                                            __region_LAB_00d55cd5_c6()
                                            goto LAB_00d55c2b
                                        end
                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                            while me:IsPerformingScriptTask() do
                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                            end
                                            if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                        end
                                        ::LAB_00d5595a_c6::
                                        quest:PauseAllNonScriptedEntities(false)
                                    end
                                    if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                        addNewConversation = quest:AddNewConversation(me, false, false)
                                        quest:AddPersonToConversation(addNewConversation, hero)
                                        quest:SetTimer(timerId4, 10)
                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                            if ctr_154 == 1 then
                                                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, hero, false)
                                                -- LAB_00d55b4e_c6: (native jump target)
                                            elseif ctr_154 == 2 then
                                                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, hero, false)
                                                -- TODO(native): goto LAB_00d55b4e_c6
                                            end
                                            ctr_154 = 1 - ctr_154
                                        else
                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, hero, false)
                                        end
                                    end
                                    if scratchValue14 == 0 then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                        if not me:IsPerformingScriptTask() then
                                            scratchValue14 = 1
                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                    end
                                    end
                                until false
                                if not quest:IsActiveThreadTerminating() then
                                    quest:SetStateBool("HeroSleeps", true)
                                    quest:FadeScreenOut(0.5, 0.5)
                                    quest:SetTimeOfDay(11.0)
                                    quest:ChangeHeroHealthBy(1000.0, true, false)
                                    quest:ResetPlayerCreatureCombatMultiplier()
                                end
                                goto FLOW_after_lab_00d54846
                            else
                                -- LAB_00d54846: (native jump target)
                                quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                ctr_154 = 0
                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                scratchValue14 = 0
                                repeat
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                        if false then
                                            while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                            end
                                            while not resources:TryAcquire(resource2, me, 4) do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                            end
                                            resources:StartMovie("")
                                            quest:StartMovieSequence()
                                            quest:PauseAllNonScriptedEntities(true)
                                            if quest:GetHealth(resources:ScriptThing(resource2)) <= 0.0 then
                                                -- LAB_00d551d4_c7: (native jump target)
                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                while scratchValue19 < 0 do
                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                    if scratchValue19 == 1 then
                                                        if scratchValue then
                                                            __region_LAB_00d55c9f_c7()
                                                            goto LAB_00d55c2b
                                                        end
                                                        quest:FadeScreenOut(0.5, 0.5)
                                                        quest:Pause(1.0)
                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                    else
                                                        if scratchValue then goto LAB_00d55cba_c7 end
                                                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                            while me:IsPerformingScriptTask() do
                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c7 end
                                                        end
                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                    end
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    goto LAB_00d55480
                                                end
                                            else
                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                while me:IsPerformingScriptTask() do
                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                end
                                                if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c7
                                            end
                                            ::LAB_00d55cba_c7::
                                            quest:PauseAllNonScriptedEntities(false)
                                            goto LAB_00d55c2b
                                        end
                                        goto FLOW_after_lab_00d54f9c
                                    end
                                    if not woodsEndPlayed then
                                        woodsEndPlayed = true
                                        quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                        quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                        resource = resources:NewResource()
                                        SUB(resource,0)
                                        scratchValue = hero:AcquireControl(4)
                                        while not scratchValue do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                            SUB(resource,0)
                                            scratchValue = me:AcquireControl(4)
                                        end
                                        if quest:IsActiveThreadTerminating() then
                                            -- LAB_00d55c91: (native jump target)
                                            goto LAB_00d55c2b
                                        end
                                        actorMap2 = resources:NewActorMap()
                                        resources:SetActor(actorMap2, "HERO", resource)
                                        resources:SetActor(actorMap2, "GUARD", resource2)
                                        resources:StartMovie("")
                                        quest:StartMovieSequence()
                                        quest:PauseAllNonScriptedEntities(true)
                                        quest:FixMovieSequenceCamera(true)
                                        resources:RunMacro("CS_GUILD_MELEE_WOODSWON", actorMap2, false, true)
                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        while scratchValue19 < 0 do
                                            quest:NewScriptFrame(me)
                                            if not quest:IsActiveThreadTerminating() then
                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                            else
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyActorMap(actorMap2)
                                                goto LAB_00d55c2b
                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                            end
                                        end
                                        if quest:IsActiveThreadTerminating() then
                                            -- LAB_00d55c72: (native jump target)
                                            quest:PauseAllNonScriptedEntities(false)
                                            -- LAB_00d55c7f: (native jump target)
                                            resources:DestroyActorMap(actorMap2)
                                            goto LAB_00d55c2b
                                        end
                                        scratchValue = quest:IsActiveThreadTerminating()
                                        if scratchValue19 == 1 then
                                            if scratchValue then
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyActorMap(actorMap2)
                                                goto LAB_00d55c2b
                                            end
                                            quest:FadeScreenOut(0.5, 0.5)
                                            quest:Pause(1.0)
                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                        else
                                            if scratchValue then
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyActorMap(actorMap2)
                                                goto LAB_00d55c2b
                                            end
                                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                while me:IsPerformingScriptTask() do
                                                    quest:NewScriptFrame(me)
                                                    if quest:IsActiveThreadTerminating() then
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        resources:DestroyActorMap(actorMap2)
                                                        goto LAB_00d55c2b
                                                    end
                                                end
                                                if quest:IsActiveThreadTerminating() then
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    resources:DestroyActorMap(actorMap2)
                                                    goto LAB_00d55c2b
                                                end
                                            end
                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                        end
                                        quest:FixMovieSequenceCamera(false)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyActorMap(actorMap2)
                                    else
                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                            -- LAB_00d54f9c: (native jump target)
                                            scratchValue = false
                                        else
                                            scratchValue = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                        end
                                        if scratchValue then
                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                            while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                            end
                                            while not resources:TryAcquire(resource2, me, 4) do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                            end
                                            resources:StartMovie("")
                                            quest:StartMovieSequence()
                                            quest:PauseAllNonScriptedEntities(true)
                                            if quest:GetHealth(resources:ScriptThing(resource2)) <= 0.0 then
                                                -- LAB_00d551d4: (native jump target)
                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                while scratchValue19 < 0 do
                                                    quest:NewScriptFrame(me)
                                                    if not quest:IsActiveThreadTerminating() then
                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                    else
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        goto LAB_00d55c2b
                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                    end
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                    if scratchValue19 == 1 then
                                                        if scratchValue then
                                                            quest:PauseAllNonScriptedEntities(false)
                                                            goto LAB_00d55c2b
                                                        end
                                                        quest:FadeScreenOut(0.5, 0.5)
                                                        quest:Pause(1.0)
                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                    else
                                                        if scratchValue then goto LAB_00d55cba end
                                                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                            while me:IsPerformingScriptTask() do
                                                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d55c2b end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cba end
                                                        end
                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                    end
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    goto LAB_00d55480
                                                end
                                            else
                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                while me:IsPerformingScriptTask() do
                                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d55c2b end
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                    while scratchValue19 < 0 do
                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c19(); goto LAB_00d55c2b end
                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                    end
                                                    if not quest:IsActiveThreadTerminating() then
                                                        scratchValue = quest:IsActiveThreadTerminating()
                                                        if scratchValue19 == 1 then
                                                            if scratchValue then
                                                                __region_LAB_00d55c9f_c19()
                                                                goto LAB_00d55c2b
                                                            end
                                                            quest:FadeScreenOut(0.5, 0.5)
                                                            quest:Pause(1.0)
                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                        else
                                                            if scratchValue then goto LAB_00d55cba end
                                                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                while me:IsPerformingScriptTask() do
                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c19(); goto LAB_00d55c2b end
                                                                end
                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55cba end
                                                            end
                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                        end
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        goto LAB_00d55480
                                                    end
                                                end
                                            end
                                            ::LAB_00d55cba::
                                            quest:PauseAllNonScriptedEntities(false)
                                            goto LAB_00d55c2b
                                        end
                                    end
                                    ::FLOW_after_lab_00d54f9c::
                                    ::LAB_00d55480::
                                    if me:IsTalkedToByHero() then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                        resources:StartMovie("")
                                        quest:StartMovieSequence()
                                        quest:PauseAllNonScriptedEntities(true)
                                        me:ClearCommands()
                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                            if not quest:IsActiveThreadTerminating() then
                                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                    while me:IsPerformingScriptTask() do
                                                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d55c2b end
                                                    end
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55cd5 end
                                                end
                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                while scratchValue19 < 0 do
                                                    quest:NewScriptFrame(me)
                                                    if not quest:IsActiveThreadTerminating() then
                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                    else
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        goto LAB_00d55c2b
                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                    end
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                    if scratchValue19 == 1 then
                                                        if not scratchValue then
                                                            quest:FadeScreenOut(0.5, 0.5)
                                                            quest:Pause(1.0)
                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                            goto LAB_00d5595a
                                                        end
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        goto LAB_00d55c2b
                                                    end
                                                    if not scratchValue then
                                                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                            while me:IsPerformingScriptTask() do
                                                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d55c2b end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cd5 end
                                                        end
                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                        goto LAB_00d5595a
                                                    end
                                                end
                                            end
                                            ::LAB_00d55cd5::
                                            quest:PauseAllNonScriptedEntities(false)
                                            goto LAB_00d55c2b
                                        end
                                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d55c2b end
                                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                            while me:IsPerformingScriptTask() do
                                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d55c2b end
                                            end
                                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d55c2b end
                                        end
                                        ::LAB_00d5595a::
                                        quest:PauseAllNonScriptedEntities(false)
                                    end
                                    if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                        addNewConversation = quest:AddNewConversation(me, false, false)
                                        quest:AddPersonToConversation(addNewConversation, hero)
                                        quest:SetTimer(timerId4, 10)
                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                            if ctr_154 == 1 then
                                                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, hero, false)
                                                -- LAB_00d55b4e: (native jump target)
                                            elseif ctr_154 == 2 then
                                                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, hero, false)
                                            end
                                            ctr_154 = 1 - ctr_154
                                        else
                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, hero, false)
                                        end
                                    end
                                    if scratchValue14 == 0 then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                        if not me:IsPerformingScriptTask() then
                                            scratchValue14 = 1
                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                    end
                                    end
                                until false
                                if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d54846 end
                                quest:SetStateBool("HeroSleeps", true)
                                quest:FadeScreenOut(0.5, 0.5)
                                quest:SetTimeOfDay(11.0)
                                quest:ChangeHeroHealthBy(1000.0, true, false)
                                quest:ResetPlayerCreatureCombatMultiplier()
                            end
                            ::FLOW_after_lab_00d54846::
                            ::FLOW_after_lab_00d54dfa::
                            ::LAB_00d55c2b::
                            quest:DeregisterTimer(timerId2)
                        else
                            if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d5439e end
                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_PC")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d55c34 end
                            end
                            if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d5439e end
                            -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                            timerId2 = quest:RegisterTimer()
                            quest:SetTimer(timerId2, 10)
                            while scratchValue32 ~= nil and scratchValue32:IsAlive() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                if quest:GetTimer(timerId2) < 1 then
                                    addNewConversation = quest:AddNewConversation(me, false, false)
                                    quest:AddPersonToConversation(addNewConversation, hero)
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, hero, false)
                                    quest:SetTimer(timerId2, 10)
                                end
                            end
                            if not quest:IsActiveThreadTerminating() then
                                quest:Pause(0.5)
                                scratchValue31 = resources:NewResource()
                                SUB(scratchValue31,0)
                                while not resources:TryAcquire(0, hero, 4) do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54dfa_c27
                                    SUB(scratchValue31,0)
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    scratchValue20 = resources:NewActorMap()
                                    resources:SetActor(scratchValue20, "HERO", scratchValue31)
                                    resources:SetActor(scratchValue20, "TEACHER", resource2)
                                    resources:StartMovie("")
                                    quest:StartMovieSequence()
                                    quest:PauseAllNonScriptedEntities(true)
                                    quest:FixMovieSequenceCamera(true)
                                    resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue20, false, true)
                                    quest:FixMovieSequenceCamera(false)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyActorMap(scratchValue20)
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
                                                        if quest:DisplayTutorial(28) then
                                                            if not quest:IsActiveThreadTerminating() then
                                                                while not quest:MsgIsTutorialClickedPast() do
                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                end
                                                                if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54846_c27
                                                            end
                                                        else
                                                            -- LAB_00d54846_c27: (native jump target)
                                                            quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                            ctr_154 = 0
                                                            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                            scratchValue14 = 0
                                                            repeat
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c27
                                                                if not woodsEndPlayed then
                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                    woodsEndPlayed = true
                                                                    quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                    quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                    resource = resources:NewResource()
                                                                    SUB(resource,0)
                                                                    scratchValue = hero:AcquireControl(4)
                                                                    while not scratchValue do
                                                                        quest:NewScriptFrame(me)
                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c27
                                                                        SUB(resource,0)
                                                                        scratchValue = me:AcquireControl(4)
                                                                    end
                                                                    if quest:IsActiveThreadTerminating() then
                                                                        -- LAB_00d55c91_c27: (native jump target)
                                                                        goto LAB_00d55c2b_c27
                                                                    end
                                                                    actorMap2 = resources:NewActorMap()
                                                                    resources:SetActor(actorMap2, "HERO", resource)
                                                                    resources:SetActor(actorMap2, "GUARD", resource2)
                                                                    resources:StartMovie("")
                                                                    quest:StartMovieSequence()
                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                    quest:FixMovieSequenceCamera(true)
                                                                    resources:RunMacro("CS_GUILD_MELEE_WOODSWON", actorMap2, false, true)
                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                    while scratchValue19 < 0 do
                                                                        quest:NewScriptFrame(me)
                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                    end
                                                                    if quest:IsActiveThreadTerminating() then
                                                                        -- LAB_00d55c72_c27: (native jump target)
                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                        -- LAB_00d55c7f_c27: (native jump target)
                                                                        resources:DestroyActorMap(actorMap2)
                                                                        -- TODO(native): goto LAB_00d55c91_c27
                                                                    end
                                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                                    if scratchValue19 == 1 then
                                                                        if scratchValue then
                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                            -- TODO(native): goto LAB_00d55c7f_c27
                                                                        end
                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                        quest:Pause(1.0)
                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                    else
                                                                        if scratchValue then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                            while me:IsPerformingScriptTask() do
                                                                                quest:NewScriptFrame(me)
                                                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                            end
                                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                        end
                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                                    end
                                                                    quest:FixMovieSequenceCamera(false)
                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                    resources:DestroyActorMap(actorMap2)
                                                                else
                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                        -- LAB_00d54f9c_c27: (native jump target)
                                                                        scratchValue = false
                                                                    else
                                                                        if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then return end  -- TODO(native): goto LAB_00d54f9c_c27
                                                                        scratchValue = true
                                                                    end
                                                                    if scratchValue then
                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                        while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                        end
                                                                        while not resources:TryAcquire(resource2, me, 4) do
                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                        end
                                                                        resources:StartMovie("")
                                                                        quest:StartMovieSequence()
                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                        if quest:GetHealth(resources:ScriptThing(resource2)) <= 0.0 then
                                                                            -- LAB_00d551d4_c27: (native jump target)
                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                            while scratchValue19 < 0 do
                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                            end
                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                if scratchValue19 == 1 then
                                                                                    if scratchValue then
                                                                                        __region_LAB_00d55c9f_c27()
                                                                                        goto LAB_00d55c2b_c27
                                                                                    end
                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                    quest:Pause(1.0)
                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                else
                                                                                    if scratchValue then goto LAB_00d55cba_c27 end
                                                                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                        while me:IsPerformingScriptTask() do
                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                        end
                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c27 end
                                                                                    end
                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                                                end
                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                goto LAB_00d55480_c27
                                                                            end
                                                                        else
                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                            while me:IsPerformingScriptTask() do
                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                            end
                                                                            if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c27
                                                                        end
                                                                        ::LAB_00d55cba_c27::
                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                        goto LAB_00d55c2b_c27
                                                                    end
                                                                end
                                                                ::LAB_00d55480_c27::
                                                                if me:IsTalkedToByHero() then
                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                    resources:StartMovie("")
                                                                    quest:StartMovieSequence()
                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                    me:ClearCommands()
                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                        if not quest:IsActiveThreadTerminating() then
                                                                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                while me:IsPerformingScriptTask() do
                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                end
                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                            end
                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                            while scratchValue19 < 0 do
                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                            end
                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                if scratchValue19 == 1 then
                                                                                    if not scratchValue then
                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                        quest:Pause(1.0)
                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                        goto LAB_00d5595a_c27
                                                                                    end
                                                                                    __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27
                                                                                end
                                                                                if not scratchValue then
                                                                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                        while me:IsPerformingScriptTask() do
                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                        end
                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                                    end
                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                                                    goto LAB_00d5595a_c27
                                                                                end
                                                                            end
                                                                        end
                                                                        __region_LAB_00d55cd5_c27()
                                                                        goto LAB_00d55c2b_c27
                                                                    end
                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                        while me:IsPerformingScriptTask() do
                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                        end
                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                    end
                                                                    ::LAB_00d5595a_c27::
                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                end
                                                                if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                    addNewConversation = quest:AddNewConversation(me, false, false)
                                                                    quest:AddPersonToConversation(addNewConversation, hero)
                                                                    quest:SetTimer(timerId4, 10)
                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                        if ctr_154 == 1 then
                                                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                            -- LAB_00d55b4e_c27: (native jump target)
                                                                        elseif ctr_154 == 2 then
                                                                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, hero, false)
                                                                            -- TODO(native): goto LAB_00d55b4e_c27
                                                                        end
                                                                        ctr_154 = 1 - ctr_154
                                                                    else
                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                    end
                                                                end
                                                                if scratchValue14 == 0 then
                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                    if not me:IsPerformingScriptTask() then
                                                                        scratchValue14 = 1
                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                end
                                                                end
                                                            until false
                                                            if not quest:IsActiveThreadTerminating() then
                                                                quest:SetStateBool("HeroSleeps", true)
                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                quest:SetTimeOfDay(11.0)
                                                                quest:ChangeHeroHealthBy(1000.0, true, false)
                                                                quest:ResetPlayerCreatureCombatMultiplier()
                                                            end
                                                        end
                                                    end
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                            ::LAB_00d55c2b_c27::
                            quest:DeregisterTimer(timerId2)
                            goto FLOW_after_lab_00d5439e
                        end
                        ::FLOW_after_lab_00d5439e::
                    end
                    ::FLOW_after_lab_00d53ff2::
                    ::LAB_00d55c34::
                else
                    if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d53c7e end
                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                    end
                    if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d53c7e end
                    -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                    quest:SetStateInt("PreMeleeMode", 2)
                    quest:SetStateInt("DummyHits", 0)
                    quest:SetTimer(timerId, 10)
                    addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                    infoCounter = addQuestInfoCounter
                    quest:DisplayQuestInfo(true)
                    while quest:GetStateInt("DummyHits") < 7 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                        quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("DummyHits"), -1)
                        if 0 ~= quest:GetStateInt("DummyHits") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                            -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                            quest:SetTimer(timerId, 10)
                        end
                        if quest:GetTimer(timerId) >= 1 then goto continue_19 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                        addNewConversation = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(addNewConversation, hero)
                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, hero, false)
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
                        quest:SetTimer(timerId3, 10)
                        timerId = timerId3
                        addQuestInfoCounter = infoCounter
                        ::continue_19::
                    end
                    if not quest:IsActiveThreadTerminating() then
                        quest:RemoveQuestInfoElement(addQuestInfoCounter)
                        quest:DisplayQuestInfo(false)
                        scratchValue32 = nil
                        movie = resources:StartMovie("")
                        quest:StartMovieSequence()
                        scratchValue31 = resources:NewResource()
                        SUB(scratchValue31,0)
                        scratchValue21 = hero
                        scratchValue = hero:AcquireControl(4)
                        while not scratchValue do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d53ff2_c28
                            SUB(scratchValue31,0)
                            scratchValue21 = hero
                            scratchValue = resources:TryAcquire(scratchValue31, hero, 4)
                        end
                        if quest:IsActiveThreadTerminating() then
                            -- LAB_00d53ff2_c28: (native jump target)
                            resources:DestroyMovie(scratchValue31)
                            resources:DestroyMovie(movie)
                        else
                            scratchValue24 = quest:GetThingWithScriptName("PreMeleeDummy")
                            -- TODO(native): CStack_114._0_4_ = *puVar11;
                            -- TODO(native): CStack_114._4_4_ = puVar11[1];
                            -- TODO(native): CStack_114._8_4_ = puVar11[2];
                            -- TODO(native): CreateEffect is not a ForgeFSE binding
                            quest:CreateEffect(scratchValue21, "SMASH_DUMMY_01", scratchValue20, "", 0.0, false, false)
                            quest:FadeOutAndKillEntity(scratchValue24, true, 1.0, true)
                            actorMap2 = resources:NewActorMap()
                            resources:SetActor(actorMap2, "HERO", scratchValue31)
                            resources:SetActor(actorMap2, "TEACHER", resource2)
                            quest:FixMovieSequenceCamera(true)
                            resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", actorMap2, false, false)
                            quest:PauseAllNonScriptedEntities(true)
                            quest:CreateExperienceOrb(1, scratchValue20)
                            -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                            quest:EntitySetCutsceneBehaviour(nil, 2)
                            resources:RunMacro("CS_GUILD_PREMELEE_PASSED", actorMap2, false, true)
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyActorMap(actorMap2)
                            resources:DestroyMovie(scratchValue31)
                            resources:DestroyMovie(movie)
                            if quest:IsXbox() then
                                if not quest:IsActiveThreadTerminating() then
                                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                    while not quest:MsgIsGameInfoClickedPast() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c34_c28 end
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        -- LAB_00d5439e_c28: (native jump target)
                                        timerId2 = quest:RegisterTimer()
                                        quest:SetTimer(timerId2, 10)
                                        while scratchValue32 ~= nil and scratchValue32:IsAlive() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                            if quest:GetTimer(timerId2) < 1 then
                                                addNewConversation = quest:AddNewConversation(me, false, false)
                                                quest:AddPersonToConversation(addNewConversation, hero)
                                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, hero, false)
                                                quest:SetTimer(timerId2, 10)
                                            end
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            quest:Pause(0.5)
                                            scratchValue31 = resources:NewResource()
                                            SUB(scratchValue31,0)
                                            while not resources:TryAcquire(0, hero, 4) do
                                                quest:NewScriptFrame(me)
                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54dfa_c28
                                                SUB(scratchValue31,0)
                                            end
                                            if not quest:IsActiveThreadTerminating() then
                                                scratchValue20 = resources:NewActorMap()
                                                resources:SetActor(scratchValue20, "HERO", scratchValue31)
                                                resources:SetActor(scratchValue20, "TEACHER", resource2)
                                                resources:StartMovie("")
                                                quest:StartMovieSequence()
                                                quest:PauseAllNonScriptedEntities(true)
                                                quest:FixMovieSequenceCamera(true)
                                                resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue20, false, true)
                                                quest:FixMovieSequenceCamera(false)
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyActorMap(scratchValue20)
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
                                                                    if quest:DisplayTutorial(28) then
                                                                        if not quest:IsActiveThreadTerminating() then
                                                                            while not quest:MsgIsTutorialClickedPast() do
                                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                            end
                                                                            if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54846_c28
                                                                        end
                                                                    else
                                                                        -- LAB_00d54846_c28: (native jump target)
                                                                        quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                        quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                                        ctr_154 = 0
                                                                        quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                        scratchValue14 = 0
                                                                        repeat
                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c28
                                                                            if not woodsEndPlayed then
                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                woodsEndPlayed = true
                                                                                quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                resource = resources:NewResource()
                                                                                SUB(resource,0)
                                                                                scratchValue = hero:AcquireControl(4)
                                                                                while not scratchValue do
                                                                                    quest:NewScriptFrame(me)
                                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c28
                                                                                    SUB(resource,0)
                                                                                    scratchValue = me:AcquireControl(4)
                                                                                end
                                                                                if quest:IsActiveThreadTerminating() then
                                                                                    -- LAB_00d55c91_c28: (native jump target)
                                                                                    goto LAB_00d55c2b_c28
                                                                                end
                                                                                actorMap2 = resources:NewActorMap()
                                                                                resources:SetActor(actorMap2, "HERO", resource)
                                                                                resources:SetActor(actorMap2, "GUARD", resource2)
                                                                                resources:StartMovie("")
                                                                                quest:StartMovieSequence()
                                                                                quest:PauseAllNonScriptedEntities(true)
                                                                                quest:FixMovieSequenceCamera(true)
                                                                                resources:RunMacro("CS_GUILD_MELEE_WOODSWON", actorMap2, false, true)
                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                while scratchValue19 < 0 do
                                                                                    quest:NewScriptFrame(me)
                                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                end
                                                                                if quest:IsActiveThreadTerminating() then
                                                                                    -- LAB_00d55c72_c28: (native jump target)
                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                    -- LAB_00d55c7f_c28: (native jump target)
                                                                                    resources:DestroyActorMap(actorMap2)
                                                                                    -- TODO(native): goto LAB_00d55c91_c28
                                                                                end
                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                if scratchValue19 == 1 then
                                                                                    if scratchValue then
                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                        -- TODO(native): goto LAB_00d55c7f_c28
                                                                                    end
                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                    quest:Pause(1.0)
                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                else
                                                                                    if scratchValue then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                        while me:IsPerformingScriptTask() do
                                                                                            quest:NewScriptFrame(me)
                                                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                        end
                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                    end
                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                                                end
                                                                                quest:FixMovieSequenceCamera(false)
                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                resources:DestroyActorMap(actorMap2)
                                                                            else
                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                    -- LAB_00d54f9c_c28: (native jump target)
                                                                                    scratchValue = false
                                                                                else
                                                                                    if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then return end  -- TODO(native): goto LAB_00d54f9c_c28
                                                                                    scratchValue = true
                                                                                end
                                                                                if scratchValue then
                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                    while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                                    end
                                                                                    while not resources:TryAcquire(resource2, me, 4) do
                                                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                                    end
                                                                                    resources:StartMovie("")
                                                                                    quest:StartMovieSequence()
                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                    if quest:GetHealth(resources:ScriptThing(resource2)) <= 0.0 then
                                                                                        -- LAB_00d551d4_c28: (native jump target)
                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                        while scratchValue19 < 0 do
                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                        end
                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                            scratchValue = quest:IsActiveThreadTerminating()
                                                                                            if scratchValue19 == 1 then
                                                                                                if scratchValue then
                                                                                                    __region_LAB_00d55c9f_c28()
                                                                                                    goto LAB_00d55c2b_c28
                                                                                                end
                                                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                                                quest:Pause(1.0)
                                                                                                quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                            else
                                                                                                if scratchValue then goto LAB_00d55cba_c28 end
                                                                                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                                    while me:IsPerformingScriptTask() do
                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c28 end
                                                                                                end
                                                                                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                                                            end
                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                            goto LAB_00d55480_c28
                                                                                        end
                                                                                    else
                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                        while me:IsPerformingScriptTask() do
                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                        end
                                                                                        if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c28
                                                                                    end
                                                                                    ::LAB_00d55cba_c28::
                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                    goto LAB_00d55c2b_c28
                                                                                end
                                                                            end
                                                                            ::LAB_00d55480_c28::
                                                                            if me:IsTalkedToByHero() then
                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                resources:StartMovie("")
                                                                                quest:StartMovieSequence()
                                                                                quest:PauseAllNonScriptedEntities(true)
                                                                                me:ClearCommands()
                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                            while me:IsPerformingScriptTask() do
                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                            end
                                                                                            if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                        end
                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                        while scratchValue19 < 0 do
                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                        end
                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                            scratchValue = quest:IsActiveThreadTerminating()
                                                                                            if scratchValue19 == 1 then
                                                                                                if not scratchValue then
                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                    quest:Pause(1.0)
                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                    goto LAB_00d5595a_c28
                                                                                                end
                                                                                                __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28
                                                                                            end
                                                                                            if not scratchValue then
                                                                                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                                    while me:IsPerformingScriptTask() do
                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                                end
                                                                                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                                                                goto LAB_00d5595a_c28
                                                                                            end
                                                                                        end
                                                                                    end
                                                                                    __region_LAB_00d55cd5_c28()
                                                                                    goto LAB_00d55c2b_c28
                                                                                end
                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                    while me:IsPerformingScriptTask() do
                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                    end
                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                end
                                                                                ::LAB_00d5595a_c28::
                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                            end
                                                                            if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                addNewConversation = quest:AddNewConversation(me, false, false)
                                                                                quest:AddPersonToConversation(addNewConversation, hero)
                                                                                quest:SetTimer(timerId4, 10)
                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                    if ctr_154 == 1 then
                                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                        -- LAB_00d55b4e_c28: (native jump target)
                                                                                    elseif ctr_154 == 2 then
                                                                                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, hero, false)
                                                                                        -- TODO(native): goto LAB_00d55b4e_c28
                                                                                    end
                                                                                    ctr_154 = 1 - ctr_154
                                                                                else
                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                end
                                                                            end
                                                                            if scratchValue14 == 0 then
                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                if not me:IsPerformingScriptTask() then
                                                                                    scratchValue14 = 1
                                                                                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                            end
                                                                            end
                                                                        until false
                                                                        if not quest:IsActiveThreadTerminating() then
                                                                            quest:SetStateBool("HeroSleeps", true)
                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                            quest:SetTimeOfDay(11.0)
                                                                            quest:ChangeHeroHealthBy(1000.0, true, false)
                                                                            quest:ResetPlayerCreatureCombatMultiplier()
                                                                        end
                                                                    end
                                                                end
                                                            end
                                                        end
                                                    end
                                                end
                                            end
                                        end
                                        ::LAB_00d55c2b_c28::
                                        quest:DeregisterTimer(timerId2)
                                    end
                                end
                            elseif not quest:IsActiveThreadTerminating() then
                                quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_PC")
                                while not quest:MsgIsGameInfoClickedPast() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c34_c28 end
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                    -- TODO(native): goto LAB_00d5439e_c28
                                end
                            end
                        end
                        ::LAB_00d55c34_c28::
                    end
                    goto FLOW_after_lab_00d53c7e
                end
                ::FLOW_after_lab_00d53c7e::
            end
            ::FLOW_after_lab_00d53a0b::
            ::LAB_00d55c3d::
            quest:DeregisterTimer(timerId3)
        else
            if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d536c0 end
            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH_PC")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame(me) then goto LAB_00d55c46 end
            end
            if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d536c0 end
            -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
            -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
            timerId3 = quest:RegisterTimer()
            timerId = timerId3
            quest:SetTimer(timerId3, 10)
            quest:SetStateInt("PreMeleeMode", 1)
            quest:SetStateInt("DummyHits", 0)
            addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
            infoCounter = addQuestInfoCounter
            quest:DisplayQuestInfo(true)
            while quest:GetStateInt("DummyHits") < 7 do
                if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("DummyHits"), -1)
                if 0 ~= quest:GetStateInt("DummyHits") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                    -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                    quest:SetTimer(timerId, 10)
                end
                if quest:GetTimer(timerId) >= 1 then goto continue_22 end
                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_HIT_DUMMY", me, hero, false)
                if quest:IsXbox() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                quest:SetTimer(timerId3, 10)
                timerId = timerId3
                addQuestInfoCounter = infoCounter
                ::continue_22::
            end
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveQuestInfoElement(addQuestInfoCounter)
                quest:DisplayQuestInfo(false)
                scratchValue31 = resources:NewResource()
                SUB(scratchValue31,0)
                while not resources:TryAcquire(scratchValue31, hero, 4) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d53a0b_c29
                    SUB(scratchValue31,0)
                end
                if quest:IsActiveThreadTerminating() then
                    -- LAB_00d53a0b_c29: (native jump target)
                    resources:DestroyMovie(scratchValue31)
                else
                    actorMap2 = resources:NewActorMap()
                    resources:SetActor(actorMap2, "HERO", scratchValue31)
                    resources:SetActor(actorMap2, "TEACHER", resource2)
                    movie = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GUILD_PREMELEE_STICK", actorMap2, false, true)
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:DestroyActorMap(actorMap2)
                    resources:DestroyMovie(scratchValue31)
                    if quest:IsXbox() then
                        if not quest:IsActiveThreadTerminating() then
                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                            end
                            if not quest:IsActiveThreadTerminating() then
                                -- LAB_00d53c7e_c29: (native jump target)
                                quest:SetStateInt("PreMeleeMode", 2)
                                quest:SetStateInt("DummyHits", 0)
                                quest:SetTimer(timerId, 10)
                                addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                                infoCounter = addQuestInfoCounter
                                quest:DisplayQuestInfo(true)
                                while quest:GetStateInt("DummyHits") < 7 do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                    quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetStateInt("DummyHits"), -1)
                                    if 0 ~= quest:GetStateInt("DummyHits") then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                        -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                        quest:SetTimer(timerId, 10)
                                    end
                                    if quest:GetTimer(timerId) >= 1 then goto continue_23 end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                    addNewConversation = quest:AddNewConversation(me, false, false)
                                    quest:AddPersonToConversation(addNewConversation, hero)
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, hero, false)
                                    if quest:IsXbox() then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                                        while not quest:MsgIsGameInfoClickedPast() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                        end
                                    else
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                                        while not quest:MsgIsGameInfoClickedPast() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                        end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                    quest:SetTimer(timerId3, 10)
                                    timerId = timerId3
                                    addQuestInfoCounter = infoCounter
                                    ::continue_23::
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    quest:RemoveQuestInfoElement(addQuestInfoCounter)
                                    quest:DisplayQuestInfo(false)
                                    scratchValue32 = nil
                                    movie = resources:StartMovie("")
                                    quest:StartMovieSequence()
                                    scratchValue31 = resources:NewResource()
                                    SUB(scratchValue31,0)
                                    scratchValue21 = hero
                                    scratchValue = hero:AcquireControl(4)
                                    while not scratchValue do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d53ff2_c29
                                        SUB(scratchValue31,0)
                                        scratchValue21 = hero
                                        scratchValue = resources:TryAcquire(scratchValue31, hero, 4)
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        -- LAB_00d53ff2_c29: (native jump target)
                                        resources:DestroyMovie(scratchValue31)
                                        resources:DestroyMovie(movie)
                                    else
                                        scratchValue25 = quest:GetThingWithScriptName("PreMeleeDummy")
                                        -- TODO(native): CStack_114._0_4_ = *puVar11;
                                        -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                        -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                        -- TODO(native): CreateEffect is not a ForgeFSE binding
                                        quest:CreateEffect(scratchValue21, "SMASH_DUMMY_01", scratchValue20, "", 0.0, false, false)
                                        quest:FadeOutAndKillEntity(scratchValue25, true, 1.0, true)
                                        actorMap2 = resources:NewActorMap()
                                        resources:SetActor(actorMap2, "HERO", scratchValue31)
                                        resources:SetActor(actorMap2, "TEACHER", resource2)
                                        quest:FixMovieSequenceCamera(true)
                                        resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", actorMap2, false, false)
                                        quest:PauseAllNonScriptedEntities(true)
                                        quest:CreateExperienceOrb(1, scratchValue20)
                                        -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                        quest:EntitySetCutsceneBehaviour(nil, 2)
                                        resources:RunMacro("CS_GUILD_PREMELEE_PASSED", actorMap2, false, true)
                                        quest:FixMovieSequenceCamera(false)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyActorMap(actorMap2)
                                        resources:DestroyMovie(scratchValue31)
                                        resources:DestroyMovie(movie)
                                        if quest:IsXbox() then
                                            if not quest:IsActiveThreadTerminating() then
                                                quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                                while not quest:MsgIsGameInfoClickedPast() do
                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c34_c29 end
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    -- LAB_00d5439e_c29: (native jump target)
                                                    timerId2 = quest:RegisterTimer()
                                                    quest:SetTimer(timerId2, 10)
                                                    while scratchValue32 ~= nil and scratchValue32:IsAlive() do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                        if quest:GetTimer(timerId2) < 1 then
                                                            addNewConversation = quest:AddNewConversation(me, false, false)
                                                            quest:AddPersonToConversation(addNewConversation, hero)
                                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, hero, false)
                                                            quest:SetTimer(timerId2, 10)
                                                        end
                                                    end
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:Pause(0.5)
                                                        scratchValue31 = resources:NewResource()
                                                        SUB(scratchValue31,0)
                                                        while not resources:TryAcquire(0, hero, 4) do
                                                            quest:NewScriptFrame(me)
                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54dfa_c29
                                                            SUB(scratchValue31,0)
                                                        end
                                                        if not quest:IsActiveThreadTerminating() then
                                                            scratchValue20 = resources:NewActorMap()
                                                            resources:SetActor(scratchValue20, "HERO", scratchValue31)
                                                            resources:SetActor(scratchValue20, "TEACHER", resource2)
                                                            resources:StartMovie("")
                                                            quest:StartMovieSequence()
                                                            quest:PauseAllNonScriptedEntities(true)
                                                            quest:FixMovieSequenceCamera(true)
                                                            resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue20, false, true)
                                                            quest:FixMovieSequenceCamera(false)
                                                            quest:PauseAllNonScriptedEntities(false)
                                                            resources:DestroyActorMap(scratchValue20)
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
                                                                                if quest:DisplayTutorial(28) then
                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                        while not quest:MsgIsTutorialClickedPast() do
                                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                        end
                                                                                        if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54846_c29
                                                                                    end
                                                                                else
                                                                                    -- LAB_00d54846_c29: (native jump target)
                                                                                    quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                    quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_GREEN_SMALL")
                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                                                    ctr_154 = 0
                                                                                    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                    scratchValue14 = 0
                                                                                    repeat
                                                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c29
                                                                                        if not woodsEndPlayed then
                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                            woodsEndPlayed = true
                                                                                            quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                            resource = resources:NewResource()
                                                                                            SUB(resource,0)
                                                                                            scratchValue = hero:AcquireControl(4)
                                                                                            while not scratchValue do
                                                                                                quest:NewScriptFrame(me)
                                                                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c29
                                                                                                SUB(resource,0)
                                                                                                scratchValue = me:AcquireControl(4)
                                                                                            end
                                                                                            if quest:IsActiveThreadTerminating() then
                                                                                                -- LAB_00d55c91_c29: (native jump target)
                                                                                                goto LAB_00d55c2b_c29
                                                                                            end
                                                                                            actorMap2 = resources:NewActorMap()
                                                                                            resources:SetActor(actorMap2, "HERO", resource)
                                                                                            resources:SetActor(actorMap2, "GUARD", resource2)
                                                                                            resources:StartMovie("")
                                                                                            quest:StartMovieSequence()
                                                                                            quest:PauseAllNonScriptedEntities(true)
                                                                                            quest:FixMovieSequenceCamera(true)
                                                                                            resources:RunMacro("CS_GUILD_MELEE_WOODSWON", actorMap2, false, true)
                                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                            scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                            while scratchValue19 < 0 do
                                                                                                quest:NewScriptFrame(me)
                                                                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                            end
                                                                                            if quest:IsActiveThreadTerminating() then
                                                                                                -- LAB_00d55c72_c29: (native jump target)
                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                -- LAB_00d55c7f_c29: (native jump target)
                                                                                                resources:DestroyActorMap(actorMap2)
                                                                                                -- TODO(native): goto LAB_00d55c91_c29
                                                                                            end
                                                                                            scratchValue = quest:IsActiveThreadTerminating()
                                                                                            if scratchValue19 == 1 then
                                                                                                if scratchValue then
                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                    -- TODO(native): goto LAB_00d55c7f_c29
                                                                                                end
                                                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                                                quest:Pause(1.0)
                                                                                                quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                            else
                                                                                                if scratchValue then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                                    while me:IsPerformingScriptTask() do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                end
                                                                                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                                                            end
                                                                                            quest:FixMovieSequenceCamera(false)
                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                            resources:DestroyActorMap(actorMap2)
                                                                                        else
                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                -- LAB_00d54f9c_c29: (native jump target)
                                                                                                scratchValue = false
                                                                                            else
                                                                                                if not quest:IsQuestActive("Q_GuildTrainingWoodsMelee") then return end  -- TODO(native): goto LAB_00d54f9c_c29
                                                                                                scratchValue = true
                                                                                            end
                                                                                            if scratchValue then
                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                while quest:IsQuestActive("Q_GuildTrainingWoodsMelee") do
                                                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                                end
                                                                                                while not resources:TryAcquire(resource2, me, 4) do
                                                                                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                                end
                                                                                                resources:StartMovie("")
                                                                                                quest:StartMovieSequence()
                                                                                                quest:PauseAllNonScriptedEntities(true)
                                                                                                if quest:GetHealth(resources:ScriptThing(resource2)) <= 0.0 then
                                                                                                    -- LAB_00d551d4_c29: (native jump target)
                                                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    while scratchValue19 < 0 do
                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    end
                                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                                        scratchValue = quest:IsActiveThreadTerminating()
                                                                                                        if scratchValue19 == 1 then
                                                                                                            if scratchValue then
                                                                                                                __region_LAB_00d55c9f_c29()
                                                                                                                goto LAB_00d55c2b_c29
                                                                                                            end
                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                            quest:Pause(1.0)
                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                        else
                                                                                                            if scratchValue then goto LAB_00d55cba_c29 end
                                                                                                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                                                while me:IsPerformingScriptTask() do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c29 end
                                                                                                            end
                                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                                                                        end
                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                        goto LAB_00d55480_c29
                                                                                                    end
                                                                                                else
                                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                                    while me:IsPerformingScriptTask() do
                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                    end
                                                                                                    if not quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d551d4_c29
                                                                                                end
                                                                                                ::LAB_00d55cba_c29::
                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                goto LAB_00d55c2b_c29
                                                                                            end
                                                                                        end
                                                                                        ::LAB_00d55480_c29::
                                                                                        if me:IsTalkedToByHero() then
                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                            resources:StartMovie("")
                                                                                            quest:StartMovieSequence()
                                                                                            quest:PauseAllNonScriptedEntities(true)
                                                                                            me:ClearCommands()
                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                if not quest:IsActiveThreadTerminating() then
                                                                                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                                        while me:IsPerformingScriptTask() do
                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                    end
                                                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                    scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    while scratchValue19 < 0 do
                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                        scratchValue19 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    end
                                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                                        scratchValue = quest:IsActiveThreadTerminating()
                                                                                                        if scratchValue19 == 1 then
                                                                                                            if not scratchValue then
                                                                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                                                                quest:Pause(1.0)
                                                                                                                quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                goto LAB_00d5595a_c29
                                                                                                            end
                                                                                                            __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29
                                                                                                        end
                                                                                                        if not scratchValue then
                                                                                                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                                                while me:IsPerformingScriptTask() do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                            end
                                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, true)
                                                                                                            goto LAB_00d5595a_c29
                                                                                                        end
                                                                                                    end
                                                                                                end
                                                                                                __region_LAB_00d55cd5_c29()
                                                                                                goto LAB_00d55c2b_c29
                                                                                            end
                                                                                            if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", GROUP_SELECT_FIRST, false, true, CONCAT13(0,CONCAT12(0,CONCAT11( 0,0))))
                                                                                                while me:IsPerformingScriptTask() do
                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                end
                                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                            end
                                                                                            ::LAB_00d5595a_c29::
                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                        end
                                                                                        if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId4) < 1) and not me:IsPerformingScriptTask() then
                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                            addNewConversation = quest:AddNewConversation(me, false, false)
                                                                                            quest:AddPersonToConversation(addNewConversation, hero)
                                                                                            quest:SetTimer(timerId4, 10)
                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                if ctr_154 == 1 then
                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                                    -- LAB_00d55b4e_c29: (native jump target)
                                                                                                elseif ctr_154 == 2 then
                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, hero, false)
                                                                                                    -- TODO(native): goto LAB_00d55b4e_c29
                                                                                                end
                                                                                                ctr_154 = 1 - ctr_154
                                                                                            else
                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, hero, false)
                                                                                            end
                                                                                        end
                                                                                        if scratchValue14 == 0 then
                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                            if not me:IsPerformingScriptTask() then
                                                                                                scratchValue14 = 1
                                                                                                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                                                                                        end
                                                                                        end
                                                                                    until false
                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                        quest:SetStateBool("HeroSleeps", true)
                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                        quest:SetTimeOfDay(11.0)
                                                                                        quest:ChangeHeroHealthBy(1000.0, true, false)
                                                                                        quest:ResetPlayerCreatureCombatMultiplier()
                                                                                    end
                                                                                end
                                                                            end
                                                                        end
                                                                    end
                                                                end
                                                            end
                                                        end
                                                    end
                                                    ::LAB_00d55c2b_c29::
                                                    quest:DeregisterTimer(timerId2)
                                                end
                                            end
                                        elseif not quest:IsActiveThreadTerminating() then
                                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_PC")
                                            while not quest:MsgIsGameInfoClickedPast() do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c34_c29 end
                                            end
                                            if not quest:IsActiveThreadTerminating() then
                                                -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                                -- TODO(native): goto LAB_00d5439e_c29
                                            end
                                        end
                                    end
                                    ::LAB_00d55c34_c29::
                                end
                            end
                        end
                    elseif not quest:IsActiveThreadTerminating() then
                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK_PC")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                            -- TODO(native): goto LAB_00d53c7e_c29
                        end
                    end
                end
            end
            ::LAB_00d55c3d_c29::
            quest:DeregisterTimer(timerId3)
            goto FLOW_after_lab_00d536c0
        end
        ::FLOW_after_lab_00d536c0::
    end
    ::FLOW_after_lab_00d533bb::
    ::LAB_00d55c46::
    quest:DeregisterTimer(timerId4)
    ::LAB_00d55c4f::
    resources:ReleaseResource(resource2)
end

-- TheRealGuildmaster.Init (retail 0x00d51ed0)
function Init(quest, me)
    chatJumped = false
    woodsEndPlayed = false
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

