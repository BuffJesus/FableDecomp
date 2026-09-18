-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local woodsEndPlayed, chatJumped

-- TheRealGuildmaster.Main (retail 0x00d52e90)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local addQuestInfoCounter, addQuestInfoCounter2, addQuestInfoCounter3, addQuestInfoCounter4
    local addQuestInfoCounter5, scratchValue, guildmasterTeleport, c_stk_161_1, c_stk_161_2
    local c_stk_161_3, c_stk_161_4, c_stk_161_5, c_stk_169_1, ctr_154, scratchValue58
    local addNewConversation, switch, scratchValue59, scratchValue60, preMeleeDummy, preMeleeDummy2
    local scratchValue66, timerId, scratchValue67, resource, actorMap, movie, actorMap2
    local scratchValue68, scratchValue69, resource2, timerId2, timerId3, timerId4
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
    guildmasterTeleport = quest:GetStateBool("GuildmasterTeleport")
    scratchValue67 = 0
    while not guildmasterTeleport do
        if not quest:NewScriptFrame(me) then return end
        guildmasterTeleport = quest:GetStateBool("GuildmasterTeleport")
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
    scratchValue59 = quest:GetThingWithScriptName("M_MeleeTeacherStand")
    quest:EntityTeleportToThing(me, scratchValue59, false)
    timerId4 = quest:RegisterTimer()
    quest:SetTimer(timerId4, 0)
    c_stk_169_1 = 1
    repeat
        if not quest:NewScriptFrame(me) then goto LAB_00d55c46 end
        if me:IsTalkedToByHero() then
            c_stk_169_1 = 0
        end
        if not quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) or 0 < quest:GetTimer(timerId4) then goto FLOW_native_label_1 end
        addNewConversation = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(addNewConversation, hero)
        quest:SetTimer(timerId4, 5)
        switch = scratchValue67
        repeat
            if switch == 0 then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FIRST", me, hero, false)
                me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
                scratchValue67 = 1
                break
            else
                if switch == 1 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_SECOND", me, hero, false)
                    scratchValue67 = 2
                    goto FLOW_after_lab_00d53316
                elseif switch == 2 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_THIRD", me, hero, false)
                    scratchValue67 = 3
                    break
                elseif switch == 3 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FOURTH", me, hero, false)
                    scratchValue67 = 2
                end
                ::FLOW_after_lab_00d53316::
            end
        until true
        ::FLOW_native_label_1::
    until c_stk_169_1 == 0
    if quest:IsActiveThreadTerminating() then goto LAB_00d55c46 end
    quest:SetStateBool("WhisperStopFollowing", true)
    scratchValue68 = resources:NewResource()
    while not resources:TryAcquire(scratchValue68, hero, 4) do
        if not quest:NewScriptFrame(me) then resources:DestroyMovie(scratchValue68); goto FLOW_after_lab_00d533bb end
    end
    if quest:IsActiveThreadTerminating() then
        resources:DestroyMovie(scratchValue68)
    else
        actorMap2 = resources:NewActorMap()
        resources:SetActor(actorMap2, "HERO", scratchValue68)
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
        resources:DestroyMovie(scratchValue68)
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
                ::continue_2::
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
            quest:RemoveQuestInfoElement(addQuestInfoCounter)
            quest:DisplayQuestInfo(false)
            scratchValue68 = resources:NewResource()
            while not resources:TryAcquire(scratchValue68, hero, 4) do
                if not quest:NewScriptFrame(me) then resources:DestroyMovie(scratchValue68); goto FLOW_after_lab_00d53a0b end
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d53a0b: (native jump target)
                resources:DestroyMovie(scratchValue68)
            else
                actorMap2 = resources:NewActorMap()
                resources:SetActor(actorMap2, "HERO", scratchValue68)
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
                resources:DestroyMovie(scratchValue68)
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
                    addQuestInfoCounter2 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                    quest:DisplayQuestInfo(true)
                    while quest:GetStateInt("DummyHits") < 7 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                        quest:UpdateQuestInfoCounter(addQuestInfoCounter2, quest:GetStateInt("DummyHits"), -1)
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
                        ::continue_4::
                    end
                    if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d53c7e end
                    quest:RemoveQuestInfoElement(addQuestInfoCounter2)
                    quest:DisplayQuestInfo(false)
                    scratchValue69 = nil
                    movie = resources:StartMovie("")
                    quest:StartMovieSequence()
                    scratchValue68 = resources:NewResource()
                    while not resources:TryAcquire(scratchValue68, hero, 4) do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            resources:DestroyMovie(scratchValue68)
                            resources:DestroyMovie(movie)
                            goto FLOW_after_lab_00d53ff2
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        -- LAB_00d53ff2: (native jump target)
                        resources:DestroyMovie(scratchValue68)
                        resources:DestroyMovie(movie)
                    else
                        preMeleeDummy = quest:GetThingWithScriptName("PreMeleeDummy")
                        -- TODO(native): CStack_114._0_4_ = *puVar11;
                        -- TODO(native): CStack_114._4_4_ = puVar11[1];
                        -- TODO(native): CStack_114._8_4_ = puVar11[2];
                        -- TODO(native): CreateEffect is not a ForgeFSE binding
                        quest:CreateEffect(xStack_dc, "SMASH_DUMMY_01", actorMap, "", 0.0, false, false)
                        quest:FadeOutAndKillEntity(preMeleeDummy, true, 1.0, true)
                        actorMap2 = resources:NewActorMap()
                        resources:SetActor(actorMap2, "HERO", scratchValue68)
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
                        resources:DestroyMovie(scratchValue68)
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
                            while scratchValue69 ~= nil and scratchValue69:IsAlive() do
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
                            scratchValue68 = resources:NewResource()
                            while not resources:TryAcquire(scratchValue68, hero, 4) do
                                if not quest:NewScriptFrame(me) then goto FLOW_after_lab_00d54dfa end
                            end
                            if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d54dfa end
                            actorMap = resources:NewActorMap()
                            resources:SetActor(actorMap, "HERO", scratchValue68)
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
                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, scratchValue68 ~= 0, true)
                                ctr_154 = 0
                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                c_stk_161_1 = 0
                                repeat
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c6
                                    if not woodsEndPlayed then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                        woodsEndPlayed = true
                                        quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                        quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                        resource = resources:NewResource()
                                        while not resources:TryAcquire(resource, hero, 4) do
                                            quest:NewScriptFrame(me)
                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c6
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
                                        scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        while scratchValue58 < 0 do
                                            quest:NewScriptFrame(me)
                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c6
                                            scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        end
                                        if quest:IsActiveThreadTerminating() then
                                            -- LAB_00d55c72_c6: (native jump target)
                                            quest:PauseAllNonScriptedEntities(false)
                                            -- LAB_00d55c7f_c6: (native jump target)
                                            resources:DestroyActorMap(actorMap2)
                                            -- TODO(native): goto LAB_00d55c91_c6
                                        end
                                        scratchValue = quest:IsActiveThreadTerminating()
                                        if scratchValue58 == 1 then
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
                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                while me:IsPerformingScriptTask() do
                                                    quest:NewScriptFrame(me)
                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                end
                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c6
                                            end
                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
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
                                                scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                while scratchValue58 < 0 do
                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                    scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                    if scratchValue58 == 1 then
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
                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                            while me:IsPerformingScriptTask() do
                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c6 end
                                                        end
                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
                                                    end
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    goto LAB_00d55480_c6
                                                end
                                            else
                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, resource ~= 0)
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
                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                    while me:IsPerformingScriptTask() do
                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                    end
                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                end
                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                while scratchValue58 < 0 do
                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                    scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                    if scratchValue58 == 1 then
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
                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                            while me:IsPerformingScriptTask() do
                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                        end
                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
                                                        goto LAB_00d5595a_c6
                                                    end
                                                end
                                            end
                                            __region_LAB_00d55cd5_c6()
                                            goto LAB_00d55c2b
                                        end
                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", GROUP_SELECT_FIRST, false, true, resource ~= 0)
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
                                    if c_stk_161_1 == 0 then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                        if not me:IsPerformingScriptTask() then
                                            c_stk_161_1 = 1
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
                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, scratchValue68 ~= 0, true)
                                ctr_154 = 0
                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                c_stk_161_2 = 0
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
                                                scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                while scratchValue58 < 0 do
                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                    scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                    if scratchValue58 == 1 then
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
                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                            while me:IsPerformingScriptTask() do
                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c7 end
                                                        end
                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
                                                    end
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    goto LAB_00d55480
                                                end
                                            else
                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, resource ~= 0)
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
                                        while not resources:TryAcquire(resource, hero, 4) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b end
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
                                        scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        while scratchValue58 < 0 do
                                            quest:NewScriptFrame(me)
                                            if not quest:IsActiveThreadTerminating() then
                                                scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                            else
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyActorMap(actorMap2)
                                                goto LAB_00d55c2b
                                                scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
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
                                        if scratchValue58 == 1 then
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
                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
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
                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
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
                                                scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                while scratchValue58 < 0 do
                                                    quest:NewScriptFrame(me)
                                                    if not quest:IsActiveThreadTerminating() then
                                                        scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                    else
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        goto LAB_00d55c2b
                                                        scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                    end
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                    if scratchValue58 == 1 then
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
                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                            while me:IsPerformingScriptTask() do
                                                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d55c2b end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cba end
                                                        end
                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
                                                    end
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    goto LAB_00d55480
                                                end
                                            else
                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                while me:IsPerformingScriptTask() do
                                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d55c2b end
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                    scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                    while scratchValue58 < 0 do
                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c19(); goto LAB_00d55c2b end
                                                        scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                    end
                                                    if not quest:IsActiveThreadTerminating() then
                                                        scratchValue = quest:IsActiveThreadTerminating()
                                                        if scratchValue58 == 1 then
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
                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                while me:IsPerformingScriptTask() do
                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c19(); goto LAB_00d55c2b end
                                                                end
                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55cba end
                                                            end
                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
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
                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                    while me:IsPerformingScriptTask() do
                                                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d55c2b end
                                                    end
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55cd5 end
                                                end
                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                while scratchValue58 < 0 do
                                                    quest:NewScriptFrame(me)
                                                    if not quest:IsActiveThreadTerminating() then
                                                        scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                    else
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        goto LAB_00d55c2b
                                                        scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                    end
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                    if scratchValue58 == 1 then
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
                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                            while me:IsPerformingScriptTask() do
                                                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d55c2b end
                                                            end
                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55cd5 end
                                                        end
                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
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
                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", GROUP_SELECT_FIRST, false, true, resource ~= 0)
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
                                    if c_stk_161_2 == 0 then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b end
                                        if not me:IsPerformingScriptTask() then
                                            c_stk_161_2 = 1
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
                            while scratchValue69 ~= nil and scratchValue69:IsAlive() do
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
                                scratchValue68 = resources:NewResource()
                                while not resources:TryAcquire(scratchValue68, hero, 4) do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54dfa_c27
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    scratchValue59 = resources:NewActorMap()
                                    resources:SetActor(scratchValue59, "HERO", scratchValue68)
                                    resources:SetActor(scratchValue59, "TEACHER", resource2)
                                    resources:StartMovie("")
                                    quest:StartMovieSequence()
                                    quest:PauseAllNonScriptedEntities(true)
                                    quest:FixMovieSequenceCamera(true)
                                    resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue59, false, true)
                                    quest:FixMovieSequenceCamera(false)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyActorMap(scratchValue59)
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
                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, scratchValue68 ~= 0, true)
                                                            ctr_154 = 0
                                                            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                            c_stk_161_3 = 0
                                                            repeat
                                                                if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c27 end
                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c27
                                                                if not woodsEndPlayed then
                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                    woodsEndPlayed = true
                                                                    quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                    quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                    resource = resources:NewResource()
                                                                    while not resources:TryAcquire(resource, hero, 4) do
                                                                        quest:NewScriptFrame(me)
                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c27
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
                                                                    scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                    while scratchValue58 < 0 do
                                                                        quest:NewScriptFrame(me)
                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                        scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                    end
                                                                    if quest:IsActiveThreadTerminating() then
                                                                        -- LAB_00d55c72_c27: (native jump target)
                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                        -- LAB_00d55c7f_c27: (native jump target)
                                                                        resources:DestroyActorMap(actorMap2)
                                                                        -- TODO(native): goto LAB_00d55c91_c27
                                                                    end
                                                                    scratchValue = quest:IsActiveThreadTerminating()
                                                                    if scratchValue58 == 1 then
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
                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                            while me:IsPerformingScriptTask() do
                                                                                quest:NewScriptFrame(me)
                                                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                            end
                                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                        end
                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
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
                                                                            scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                            while scratchValue58 < 0 do
                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                            end
                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                if scratchValue58 == 1 then
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
                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                                        while me:IsPerformingScriptTask() do
                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                        end
                                                                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c27 end
                                                                                    end
                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
                                                                                end
                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                goto LAB_00d55480_c27
                                                                            end
                                                                        else
                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, resource ~= 0)
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
                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                                while me:IsPerformingScriptTask() do
                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                end
                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                            end
                                                                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                            scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                            while scratchValue58 < 0 do
                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                            end
                                                                            if not quest:IsActiveThreadTerminating() then
                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                if scratchValue58 == 1 then
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
                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                                        while me:IsPerformingScriptTask() do
                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                        end
                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                                    end
                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
                                                                                    goto LAB_00d5595a_c27
                                                                                end
                                                                            end
                                                                        end
                                                                        __region_LAB_00d55cd5_c27()
                                                                        goto LAB_00d55c2b_c27
                                                                    end
                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", GROUP_SELECT_FIRST, false, true, resource ~= 0)
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
                                                                if c_stk_161_3 == 0 then
                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c27 end
                                                                    if not me:IsPerformingScriptTask() then
                                                                        c_stk_161_3 = 1
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
                    addQuestInfoCounter3 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                    quest:DisplayQuestInfo(true)
                    while quest:GetStateInt("DummyHits") < 7 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d55c3d end
                        quest:UpdateQuestInfoCounter(addQuestInfoCounter3, quest:GetStateInt("DummyHits"), -1)
                        if 0 ~= quest:GetStateInt("DummyHits") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d end
                            -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                            quest:SetTimer(timerId, 10)
                        end
                        if quest:GetTimer(timerId) >= 1 then goto continue_20 end
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
                        ::continue_20::
                    end
                    if not quest:IsActiveThreadTerminating() then
                        quest:RemoveQuestInfoElement(addQuestInfoCounter3)
                        quest:DisplayQuestInfo(false)
                        scratchValue69 = nil
                        movie = resources:StartMovie("")
                        quest:StartMovieSequence()
                        scratchValue68 = resources:NewResource()
                        scratchValue60 = hero
                        while not resources:TryAcquire(scratchValue68, scratchValue60, 4) do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d53ff2_c28
                            scratchValue60 = hero
                        end
                        if quest:IsActiveThreadTerminating() then
                            -- LAB_00d53ff2_c28: (native jump target)
                            resources:DestroyMovie(scratchValue68)
                            resources:DestroyMovie(movie)
                        else
                            preMeleeDummy2 = quest:GetThingWithScriptName("PreMeleeDummy")
                            -- TODO(native): CStack_114._0_4_ = *puVar11;
                            -- TODO(native): CStack_114._4_4_ = puVar11[1];
                            -- TODO(native): CStack_114._8_4_ = puVar11[2];
                            -- TODO(native): CreateEffect is not a ForgeFSE binding
                            quest:CreateEffect(scratchValue60, "SMASH_DUMMY_01", scratchValue59, "", 0.0, false, false)
                            quest:FadeOutAndKillEntity(preMeleeDummy2, true, 1.0, true)
                            actorMap2 = resources:NewActorMap()
                            resources:SetActor(actorMap2, "HERO", scratchValue68)
                            resources:SetActor(actorMap2, "TEACHER", resource2)
                            quest:FixMovieSequenceCamera(true)
                            resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", actorMap2, false, false)
                            quest:PauseAllNonScriptedEntities(true)
                            quest:CreateExperienceOrb(1, scratchValue59)
                            -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                            quest:EntitySetCutsceneBehaviour(nil, 2)
                            resources:RunMacro("CS_GUILD_PREMELEE_PASSED", actorMap2, false, true)
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyActorMap(actorMap2)
                            resources:DestroyMovie(scratchValue68)
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
                                        while scratchValue69 ~= nil and scratchValue69:IsAlive() do
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
                                            scratchValue68 = resources:NewResource()
                                            while not resources:TryAcquire(scratchValue68, hero, 4) do
                                                quest:NewScriptFrame(me)
                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54dfa_c28
                                            end
                                            if not quest:IsActiveThreadTerminating() then
                                                scratchValue59 = resources:NewActorMap()
                                                resources:SetActor(scratchValue59, "HERO", scratchValue68)
                                                resources:SetActor(scratchValue59, "TEACHER", resource2)
                                                resources:StartMovie("")
                                                quest:StartMovieSequence()
                                                quest:PauseAllNonScriptedEntities(true)
                                                quest:FixMovieSequenceCamera(true)
                                                resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue59, false, true)
                                                quest:FixMovieSequenceCamera(false)
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyActorMap(scratchValue59)
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
                                                                        me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, scratchValue68 ~= 0, true)
                                                                        ctr_154 = 0
                                                                        quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                        c_stk_161_4 = 0
                                                                        repeat
                                                                            if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c28 end
                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c28
                                                                            if not woodsEndPlayed then
                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                woodsEndPlayed = true
                                                                                quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                resource = resources:NewResource()
                                                                                while not resources:TryAcquire(resource, hero, 4) do
                                                                                    quest:NewScriptFrame(me)
                                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c28
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
                                                                                scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                while scratchValue58 < 0 do
                                                                                    quest:NewScriptFrame(me)
                                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                    scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                end
                                                                                if quest:IsActiveThreadTerminating() then
                                                                                    -- LAB_00d55c72_c28: (native jump target)
                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                    -- LAB_00d55c7f_c28: (native jump target)
                                                                                    resources:DestroyActorMap(actorMap2)
                                                                                    -- TODO(native): goto LAB_00d55c91_c28
                                                                                end
                                                                                scratchValue = quest:IsActiveThreadTerminating()
                                                                                if scratchValue58 == 1 then
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
                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                                        while me:IsPerformingScriptTask() do
                                                                                            quest:NewScriptFrame(me)
                                                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                        end
                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                    end
                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
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
                                                                                        scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                        while scratchValue58 < 0 do
                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                            scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                        end
                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                            scratchValue = quest:IsActiveThreadTerminating()
                                                                                            if scratchValue58 == 1 then
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
                                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                                                    while me:IsPerformingScriptTask() do
                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c28 end
                                                                                                end
                                                                                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
                                                                                            end
                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                            goto LAB_00d55480_c28
                                                                                        end
                                                                                    else
                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, resource ~= 0)
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
                                                                                            me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                                            while me:IsPerformingScriptTask() do
                                                                                                if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                            end
                                                                                            if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                        end
                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                        scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                        while scratchValue58 < 0 do
                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                            scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                        end
                                                                                        if not quest:IsActiveThreadTerminating() then
                                                                                            scratchValue = quest:IsActiveThreadTerminating()
                                                                                            if scratchValue58 == 1 then
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
                                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                                                    while me:IsPerformingScriptTask() do
                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                                end
                                                                                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
                                                                                                goto LAB_00d5595a_c28
                                                                                            end
                                                                                        end
                                                                                    end
                                                                                    __region_LAB_00d55cd5_c28()
                                                                                    goto LAB_00d55c2b_c28
                                                                                end
                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", GROUP_SELECT_FIRST, false, true, resource ~= 0)
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
                                                                            if c_stk_161_4 == 0 then
                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c28 end
                                                                                if not me:IsPerformingScriptTask() then
                                                                                    c_stk_161_4 = 1
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
            addQuestInfoCounter4 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
            quest:DisplayQuestInfo(true)
            while quest:GetStateInt("DummyHits") < 7 do
                if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                quest:UpdateQuestInfoCounter(addQuestInfoCounter4, quest:GetStateInt("DummyHits"), -1)
                if 0 ~= quest:GetStateInt("DummyHits") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                    -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                    quest:SetTimer(timerId, 10)
                end
                if quest:GetTimer(timerId) >= 1 then goto continue_23 end
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
                ::continue_23::
            end
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveQuestInfoElement(addQuestInfoCounter4)
                quest:DisplayQuestInfo(false)
                scratchValue68 = resources:NewResource()
                while not resources:TryAcquire(scratchValue68, hero, 4) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d53a0b_c29
                end
                if quest:IsActiveThreadTerminating() then
                    -- LAB_00d53a0b_c29: (native jump target)
                    resources:DestroyMovie(scratchValue68)
                else
                    actorMap2 = resources:NewActorMap()
                    resources:SetActor(actorMap2, "HERO", scratchValue68)
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
                    resources:DestroyMovie(scratchValue68)
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
                                addQuestInfoCounter5 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                                quest:DisplayQuestInfo(true)
                                while quest:GetStateInt("DummyHits") < 7 do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d55c3d_c29 end
                                    quest:UpdateQuestInfoCounter(addQuestInfoCounter5, quest:GetStateInt("DummyHits"), -1)
                                    if 0 ~= quest:GetStateInt("DummyHits") then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d55c3d_c29 end
                                        -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                        quest:SetTimer(timerId, 10)
                                    end
                                    if quest:GetTimer(timerId) >= 1 then goto continue_24 end
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
                                    ::continue_24::
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    quest:RemoveQuestInfoElement(addQuestInfoCounter5)
                                    quest:DisplayQuestInfo(false)
                                    scratchValue69 = nil
                                    movie = resources:StartMovie("")
                                    quest:StartMovieSequence()
                                    scratchValue68 = resources:NewResource()
                                    scratchValue60 = hero
                                    while not resources:TryAcquire(scratchValue68, scratchValue60, 4) do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d53ff2_c29
                                        scratchValue60 = hero
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        -- LAB_00d53ff2_c29: (native jump target)
                                        resources:DestroyMovie(scratchValue68)
                                        resources:DestroyMovie(movie)
                                    else
                                        scratchValue66 = quest:GetThingWithScriptName("PreMeleeDummy")
                                        -- TODO(native): CStack_114._0_4_ = *puVar11;
                                        -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                        -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                        -- TODO(native): CreateEffect is not a ForgeFSE binding
                                        quest:CreateEffect(scratchValue60, "SMASH_DUMMY_01", scratchValue59, "", 0.0, false, false)
                                        quest:FadeOutAndKillEntity(scratchValue66, true, 1.0, true)
                                        actorMap2 = resources:NewActorMap()
                                        resources:SetActor(actorMap2, "HERO", scratchValue68)
                                        resources:SetActor(actorMap2, "TEACHER", resource2)
                                        quest:FixMovieSequenceCamera(true)
                                        resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", actorMap2, false, false)
                                        quest:PauseAllNonScriptedEntities(true)
                                        quest:CreateExperienceOrb(1, scratchValue59)
                                        -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                        quest:EntitySetCutsceneBehaviour(nil, 2)
                                        resources:RunMacro("CS_GUILD_PREMELEE_PASSED", actorMap2, false, true)
                                        quest:FixMovieSequenceCamera(false)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyActorMap(actorMap2)
                                        resources:DestroyMovie(scratchValue68)
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
                                                    while scratchValue69 ~= nil and scratchValue69:IsAlive() do
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
                                                        scratchValue68 = resources:NewResource()
                                                        while not resources:TryAcquire(scratchValue68, hero, 4) do
                                                            quest:NewScriptFrame(me)
                                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d54dfa_c29
                                                        end
                                                        if not quest:IsActiveThreadTerminating() then
                                                            scratchValue59 = resources:NewActorMap()
                                                            resources:SetActor(scratchValue59, "HERO", scratchValue68)
                                                            resources:SetActor(scratchValue59, "TEACHER", resource2)
                                                            resources:StartMovie("")
                                                            quest:StartMovieSequence()
                                                            quest:PauseAllNonScriptedEntities(true)
                                                            quest:FixMovieSequenceCamera(true)
                                                            resources:RunMacro("CS_GUILD_PREMELEE_ALARM", scratchValue59, false, true)
                                                            quest:FixMovieSequenceCamera(false)
                                                            quest:PauseAllNonScriptedEntities(false)
                                                            resources:DestroyActorMap(scratchValue59)
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
                                                                                    me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, scratchValue68 ~= 0, true)
                                                                                    ctr_154 = 0
                                                                                    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                    c_stk_161_5 = 0
                                                                                    repeat
                                                                                        if not quest:NewScriptFrame(me) then goto LAB_00d55c2b_c29 end
                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c29
                                                                                        if not woodsEndPlayed then
                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                            woodsEndPlayed = true
                                                                                            quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("TheRealGuildmaster"))
                                                                                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("TheRealGuildmaster"), "HUD_ORB_QUEST_CORE")
                                                                                            resource = resources:NewResource()
                                                                                            while not resources:TryAcquire(resource, hero, 4) do
                                                                                                quest:NewScriptFrame(me)
                                                                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c91_c29
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
                                                                                            scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                            while scratchValue58 < 0 do
                                                                                                quest:NewScriptFrame(me)
                                                                                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                            end
                                                                                            if quest:IsActiveThreadTerminating() then
                                                                                                -- LAB_00d55c72_c29: (native jump target)
                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                -- LAB_00d55c7f_c29: (native jump target)
                                                                                                resources:DestroyActorMap(actorMap2)
                                                                                                -- TODO(native): goto LAB_00d55c91_c29
                                                                                            end
                                                                                            scratchValue = quest:IsActiveThreadTerminating()
                                                                                            if scratchValue58 == 1 then
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
                                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                                                    while me:IsPerformingScriptTask() do
                                                                                                        quest:NewScriptFrame(me)
                                                                                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                    end
                                                                                                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                end
                                                                                                me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
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
                                                                                                    scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    while scratchValue58 < 0 do
                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                        scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    end
                                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                                        scratchValue = quest:IsActiveThreadTerminating()
                                                                                                        if scratchValue58 == 1 then
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
                                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                                                                while me:IsPerformingScriptTask() do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then goto LAB_00d55cba_c29 end
                                                                                                            end
                                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
                                                                                                        end
                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                        goto LAB_00d55480_c29
                                                                                                    end
                                                                                                else
                                                                                                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, resource ~= 0)
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
                                                                                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                                                        while me:IsPerformingScriptTask() do
                                                                                                            if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                        end
                                                                                                        if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                    end
                                                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                    scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    while scratchValue58 < 0 do
                                                                                                        if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                        scratchValue58 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                    end
                                                                                                    if not quest:IsActiveThreadTerminating() then
                                                                                                        scratchValue = quest:IsActiveThreadTerminating()
                                                                                                        if scratchValue58 == 1 then
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
                                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO", GROUP_SELECT_FIRST, false, true, resource ~= 0)
                                                                                                                while me:IsPerformingScriptTask() do
                                                                                                                    if not quest:NewScriptFrame(me) then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                end
                                                                                                                if quest:IsActiveThreadTerminating() then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                            end
                                                                                                            me:MoveToPosition(quest:GetThingWithScriptName("MK_GTM_WD_GUARD"):GetPos(), 1.0, 0, false, resource ~= 0)
                                                                                                            goto LAB_00d5595a_c29
                                                                                                        end
                                                                                                    end
                                                                                                end
                                                                                                __region_LAB_00d55cd5_c29()
                                                                                                goto LAB_00d55c2b_c29
                                                                                            end
                                                                                            if quest:IsActiveThreadTerminating() then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
                                                                                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD", GROUP_SELECT_FIRST, false, true, resource ~= 0)
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
                                                                                        if c_stk_161_5 == 0 then
                                                                                            if quest:IsActiveThreadTerminating() then goto LAB_00d55c2b_c29 end
                                                                                            if not me:IsPerformingScriptTask() then
                                                                                                c_stk_161_5 = 1
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

