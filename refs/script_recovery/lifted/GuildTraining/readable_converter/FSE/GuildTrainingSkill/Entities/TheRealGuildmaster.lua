-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_SkillGrades = 3776,  -- '07000000000016430000f0420000a042000048420000c8410000204100000000'
    GUI_SkillTimer = 3844,  -- 60.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local playerNotWarned

-- TheRealGuildmaster.Main (retail 0x00d5ae70)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local addQuestInfoTickByText, ticked, c_stk_215_1, c_stk_215_2, c_stk_215_3, tutorialState
    local scratchValue, questionAnswer, addNewConversation, scratchValue29, i_stk_1d0_1, i_stk_1ec_1
    local i_stk_1ec_2, i_stk_1ec_3, timerId, index, guildEvilApprenticeMale, actorMap2, actorMap3
    local resource7, resource8, resource9, actorMap4, movie, movie2, addQuestInfoTickByText2
    local addQuestInfoTickByText3, addQuestInfoTickByText4, resource, timerId4, timerId5, movie3
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap3)
        resources:ReleaseResource(resource7)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything2()
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap3)
        resources:ReleaseResource(resource7)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything3()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie3)
        resources:DestroyActorMap(actorMap2)
        resources:ReleaseResource(resource8)
        quest:DeregisterTimer(timerId5)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything4()
        resources:DestroyMovie(movie3)
        resources:DestroyActorMap(actorMap2)
        resources:ReleaseResource(resource8)
        quest:DeregisterTimer(timerId5)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything5()
        resources:ReleaseResource(resource8)
        quest:DeregisterTimer(timerId5)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything6()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        resources:DestroyActorMap(actorMap4)
        resources:ReleaseResource(resource9)
        quest:DeregisterTimer(timerId4)
        quest:DeregisterTimer(timerId5)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything7()
        resources:ReleaseResource(resource9)
        quest:DeregisterTimer(timerId4)
        quest:DeregisterTimer(timerId5)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything8()
        quest:DeregisterTimer(timerId4)
        quest:DeregisterTimer(timerId5)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
    end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAlwaysBlockAttacksFromThing(me, hero, true)
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    me:MoveToPosition(quest:GetThingWithScriptName("M_SkillTeacherStand"):GetPos(), 3.0, ENTITY_MOVE_WALK, false, true)
    quest:SetPlayerUsingRangedDummies(true)
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    tutorialState = quest:GetStateInt("TutorialState")
    i_stk_1ec_1 = 0
    while tutorialState == 1 do
        if not quest:NewScriptFrame(me) then goto LAB_00d5da96 end
        if me:IsTalkedToByHero() then
            if not quest:GetMasterGameState("HeroTakingGuildTest") then
                me:ClearCommands()
                quest:SetStateInt("TutorialState", 3)
            else
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                me:ClearCommands()
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_SKILL_NOT_START", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        goto LAB_00d5da96
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            end
        end
        if not ((quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1) and not me:IsPerformingScriptTask()) then
            tutorialState = quest:GetStateInt("TutorialState")
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00d5da96 end
            local conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:SetTimer(timerId, 10)
            if i_stk_1ec_1 == 0 then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_FIRST", me, hero, false)
            elseif i_stk_1ec_1 == 1 then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_SECOND", me, hero, false)
            end
            i_stk_1ec_1 = 1 - i_stk_1ec_1
            tutorialState = quest:GetStateInt("TutorialState")
        end
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
        return
    end
    quest:SetMasterGameState("SkillTrainingStarted", true)
    quest:SetMasterGameState("SkillRepeating", true)
    quest:SetMasterGameState("HeroTakingGuildTest", true)
    while quest:GetMasterGameState("SkillRepeating") do
        if not quest:NewScriptFrame(me) then goto LAB_00d5da96 end
        quest:SetStateInt("TutorialState", 3)
        quest:SetMasterGameState("SkillRepeatKnown", false)
        resource7 = resources:NewResource()
        resources:PrepareResource(resource7)
        while not resources:TryAcquire(resource7, hero, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource7)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource7)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        actorMap3 = resources:NewActorMap()
        resources:SetActor(actorMap3, "HERO", resource7)
        resources:SetActor(actorMap3, "TEACHER", resource)
        quest:SetStateInt("TutorialState", 2)
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_GUILD_SKILL_START", actorMap3, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:SetMasterGameState("SkillTrainingStarted", true)
        if not quest:IsXbox() then
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_PC")
            while not quest:MsgIsGameInfoClickedPast() do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    ReleaseEverything2(); do return end
                end
            end
            if not quest:IsActiveThreadTerminating() then
                quest:AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_RANGED")
            else
                ReleaseEverything()
                return
            end
        else
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            quest:AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_RANGED")
        end
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap3)
        resources:ReleaseResource(resource7)
        local infoCounter2 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 3, 1.0)
        timerId5 = quest:RegisterTimer()
        quest:SetTimer(timerId5, 15)
        quest:SetMasterGameState("SkillScore", 0)
        local archeryRing = quest:GetThingWithScriptName("ArcheryRing")
        i_stk_1d0_1 = quest:GetMasterGameState("SkillScore")
        quest:EntitySetTargetable(me, false)
        if not quest:IsXbox() then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId5); quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            addQuestInfoTickByText2 = quest:AddQuestInfoTickByText("GAME_ACTION_UNSHEATHE_RANGED_WEAPON", false, 1.0)
            i_stk_1ec_2 = quest:AddQuestInfoTickByText("GAME_ACTION_LOCK_TARGET", false, 1.0)
            addQuestInfoTickByText3 = quest:AddQuestInfoTickByText("GAME_ACTION_FIRE_RANGED_WEAPON", false, 1.0)
            goto LAB_00d5ba3e
            quest:DeregisterTimer(timerId5)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId5)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        addQuestInfoTickByText2 = quest:AddQuestInfoTickByText("HUD_BLACK_BUTTON", false, 1.0)
        i_stk_1ec_2 = quest:AddQuestInfoTickByText("HUD_CONTROLLER_TRIGGER_LEFT", false, 1.0)
        addQuestInfoTickByText3 = quest:AddQuestInfoTickByText("HUD_CONTROLLER_X", false, 1.0)
        ::LAB_00d5ba3e::
        c_stk_215_1 = 0
        quest:DisplayQuestInfo(true)
        quest:SetTimer(timerId5, 5)
        addNewConversation = 0
        while quest:GetMasterGameState("SkillScore") < 3 do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId5)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            if (quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") and quest:IsPlayerHoldingLockTargetButton()) and quest:IsPlayerHoldingFireRangedWeaponButton() then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
                c_stk_215_1 = 1
                quest:RemoveQuestInfoElement(addQuestInfoTickByText2)
                quest:RemoveQuestInfoElement(i_stk_1ec_2)
                quest:RemoveQuestInfoElement(addQuestInfoTickByText3)
            elseif c_stk_215_1 == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
                quest:UpdateQuestInfoTick(addQuestInfoTickByText2, quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW"))
                quest:UpdateQuestInfoTick(i_stk_1ec_2, quest:IsPlayerHoldingLockTargetButton())
                quest:UpdateQuestInfoTick(addQuestInfoTickByText3, quest:IsPlayerHoldingFireRangedWeaponButton())
                if quest:GetTimer(timerId5) < 1 then
                    if quest:IsDistanceBetweenThingsUnder(hero, archeryRing, 6.0) and not quest:IsConversationActive(addNewConversation) then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId5)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            return
                        end
                        if quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") then
                            if quest:IsPlayerHoldingLockTargetButton() then
                                if quest:IsPlayerHoldingFireRangedWeaponButton() then goto LAB_00d5bf6c end
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId5)
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                addNewConversation = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(addNewConversation, hero)
                                if quest:IsXbox() then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId5)
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_30", me, hero, false)
                                else
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId5)
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_30", me, hero, false)
                                end
                            else
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId5)
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                addNewConversation = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(addNewConversation, hero)
                                if quest:IsXbox() then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId5)
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_20", me, hero, false)
                                else
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId5)
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_20", me, hero, false)
                                end
                            end
                        else
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId5)
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(resource)
                                return
                            end
                            addNewConversation = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(addNewConversation, hero)
                            if quest:IsXbox() then
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId5)
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_10", me, hero, false)
                            else
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId5)
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_10", me, hero, false)
                            end
                        end
                        quest:SetTimer(timerId5, 10)
                    end
                end
            end
            ::LAB_00d5bf6c::
            local getMasterGameState5 = quest:GetMasterGameState("SkillScore")
            if i_stk_1d0_1 < getMasterGameState5 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
                i_stk_1d0_1 = quest:GetMasterGameState("SkillScore")
            elseif getMasterGameState5 < i_stk_1d0_1 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
                quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + 1)
                if not quest:IsConversationActive(addNewConversation) then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId5)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        return
                    end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", me, hero, false)
                end
            end
            quest:UpdateQuestInfoCounter(infoCounter2, quest:GetMasterGameState("SkillScore"), -1)
            if playerNotWarned then
                if quest:IsDistanceBetweenThingsOver(hero, archeryRing, 6.0) and not quest:IsConversationActive(addNewConversation) then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId5)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        return
                    end
                    playerNotWarned = false
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_RING_OUT", me, hero, false)
                end
            end
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId5)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        if c_stk_215_1 == 0 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId5)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            quest:RemoveQuestInfoElement(addQuestInfoTickByText2)
            quest:RemoveQuestInfoElement(i_stk_1ec_2)
            quest:RemoveQuestInfoElement(addQuestInfoTickByText3)
        end
        resource8 = resources:NewResource()
        resources:PrepareResource(resource8)
        while not resources:TryAcquire(resource8, hero, 4) do
            if not quest:NewScriptFrame(me) then ReleaseEverything5(); return end
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything5(); return end
        actorMap2 = resources:NewActorMap()
        resources:SetActor(actorMap2, "HERO", resource8)
        resources:SetActor(actorMap2, "TEACHER", resource)
        movie3 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_GUILD_SKILL_MOVE", actorMap2, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:SetStateInt("TutorialState", 4)
        quest:SetMasterGameState("MovingDummiesNeeded", true)
        quest:CameraDefault()
        quest:RemoveQuestInfoElement(infoCounter2)
        if not quest:IsXbox() then
            if quest:IsActiveThreadTerminating() then ReleaseEverything3(); return end
            quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE_PC")
            while not quest:MsgIsGameInfoClickedPast() do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    ReleaseEverything4(); do return end
                end
            end
            if not quest:IsActiveThreadTerminating() then
                quest:AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_RANGED_FIRSTPERSON")
            else
                ReleaseEverything3()
                return
            end
        else
            if quest:IsActiveThreadTerminating() then ReleaseEverything3(); return end
            quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame(me) then ReleaseEverything3(); return end
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything3(); return end
            quest:AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_RANGED_FIRSTPERSON")
        end
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie3)
        resources:DestroyActorMap(actorMap2)
        resources:ReleaseResource(resource8)
        if not quest:IsXbox() then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId5); quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            addQuestInfoTickByText = quest:AddQuestInfoTickByText("GAME_ACTION_UNSHEATHE_RANGED_WEAPON", false, 1.0)
            i_stk_1ec_3 = quest:AddQuestInfoTickByText("GAME_ACTION_TOGGLE_FIRST_PERSON_VIEW", false, 1.0)
            addQuestInfoTickByText4 = quest:AddQuestInfoTickByText("GAME_ACTION_FIRE_RANGED_WEAPON", false, 1.0)
            goto LAB_00d5c6b1
            quest:DeregisterTimer(timerId5)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId5)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        addQuestInfoTickByText = quest:AddQuestInfoTickByText("HUD_BLACK_BUTTON", false, 1.0)
        i_stk_1ec_3 = quest:AddQuestInfoTickByText("HUD_CONTROLLER_THUMBSTICK_LEFT_CLICK", false, 1.0)
        addQuestInfoTickByText4 = quest:AddQuestInfoTickByText("HUD_CONTROLLER_X", false, 1.0)
        ::LAB_00d5c6b1::
        c_stk_215_2 = 0
        quest:DisplayQuestInfo(true)
        repeat
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId5)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            if quest:GetTimer(timerId5) >= 1 then goto LAB_00d5c757 end
            if quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") then goto LAB_00d5c757 end
            if not quest:IsDistanceBetweenThingsUnder(hero, archeryRing, 6.0) then goto LAB_00d5c757 end
            ticked = true
            goto FLOW_past_lab_00d5c757
            ::LAB_00d5c757::
            ticked = false
            ::FLOW_past_lab_00d5c757::
            if ticked then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
                local conversationId2 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId2, hero)
                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_MAZE_BOW_UNSHEATH", me, hero, false)
                if quest:IsXbox() then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId5)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        return
                    end
                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP")
                    while not quest:MsgIsGameInfoClickedPast() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId5)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId5)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        return
                    end
                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId5)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
                quest:SetTimer(timerId5, 15)
                addQuestInfoTickByText = addQuestInfoTickByText2
            end
            if quest:MsgOnHeroFiredRangedWeapon() then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
            end
            if (quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") and quest:IsHeroInProjectileWeaponMode()) and quest:IsPlayerHoldingFireRangedWeaponButton() then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
                c_stk_215_2 = 1
                quest:RemoveQuestInfoElement(addQuestInfoTickByText)
                quest:RemoveQuestInfoElement(i_stk_1ec_3)
                quest:RemoveQuestInfoElement(addQuestInfoTickByText4)
            elseif c_stk_215_2 == 0 then
                if not quest:IsActiveThreadTerminating() then quest:UpdateQuestInfoTick(addQuestInfoTickByText, quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")); quest:UpdateQuestInfoTick(i_stk_1ec_3, quest:IsHeroInProjectileWeaponMode()); quest:UpdateQuestInfoTick(addQuestInfoTickByText4, quest:IsPlayerHoldingFireRangedWeaponButton()); goto continue_9 end
                quest:DeregisterTimer(timerId5)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                do return end
                quest:UpdateQuestInfoTick(addQuestInfoTickByText, quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW"))
                quest:UpdateQuestInfoTick(i_stk_1ec_3, quest:IsHeroInProjectileWeaponMode())
                quest:UpdateQuestInfoTick(addQuestInfoTickByText4, quest:IsPlayerHoldingFireRangedWeaponButton())
            end
            ::continue_9::
        until true
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId5)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        if c_stk_215_2 == 0 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId5)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            quest:RemoveQuestInfoElement(addQuestInfoTickByText)
            quest:RemoveQuestInfoElement(i_stk_1ec_3)
            quest:RemoveQuestInfoElement(addQuestInfoTickByText4)
        end
        timerId4 = quest:RegisterTimer()
        quest:SetTimer(timerId4, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_SkillTimer))))
        quest:SetMasterGameState("SkillScore", 0)
        c_stk_215_3 = 0
        local infoCounter = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
        local i_stk_1d0_2 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
        local infoElement = quest:AddQuestInfoTimer(timerId4, "HUD_CLOCK_ICON", 1.0)
        quest:DisplayQuestInfo(true)
        quest:UpdateQuestInfoCounter(infoCounter, quest:GetMasterGameState("HighestSkillScore"), -1)
        quest:SetMasterGameState("SkillTestOccuring", true)
        quest:SetTimer(timerId5, 15)
        while 0 < quest:GetTimer(timerId4) and c_stk_215_3 == 0 do
            if not quest:NewScriptFrame(me) then ReleaseEverything8(); return end
            if quest:GetMasterGameState("GuildWarningOccuring") then
                c_stk_215_3 = 1
            end
            quest:UpdateQuestInfoCounter(i_stk_1d0_2, quest:GetMasterGameState("SkillScore"), -1)
            if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
                quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                quest:UpdateQuestInfoCounter(i_stk_1d0_2, quest:GetMasterGameState("HighestSkillScore"), -1)
            end
            if quest:GetTimer(timerId5) >= 1 then goto LAB_00d5cd47 end
            if quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") then goto LAB_00d5cd47 end
            ticked = true
            goto FLOW_past_lab_00d5cd47
            ::LAB_00d5cd47::
            ticked = false
            ::FLOW_past_lab_00d5cd47::
            if ticked then
                if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
                local conversationId3 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId3, hero)
                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_MAZE_BOW_UNSHEATH", me, hero, false)
                if quest:IsXbox() then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then ReleaseEverything8(); return end
                    end
                else
                    if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then ReleaseEverything8(); return end
                    end
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
                quest:SetTimer(timerId5, 15)
            end
            if not playerNotWarned then
                goto LAB_00d5cf7a
            else
                if not quest:IsDistanceBetweenThingsOver(hero, quest:GetThingWithScriptName("ArcheryRing"), 6.0) then goto LAB_00d5cf7a end
                ticked = true
            end
            goto FLOW_past_lab_00d5cf7a
            ::LAB_00d5cf7a::
            ticked = false
            ::FLOW_past_lab_00d5cf7a::
            if ticked then
                if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
                playerNotWarned = false
                local conversationId4 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId4, hero)
                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_MAZE_RING_OUT", me, hero, false)
            end
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
        quest:SetMasterGameState("HeroTakingGuildTest", false)
        quest:SetMasterGameState("SkillTestOccuring", false)
        while not quest:IsHeroControlledByPlayer() do
            if not quest:NewScriptFrame(me) then ReleaseEverything8(); return end
        end
        quest:DisplayQuestInfo(false)
        quest:RemoveQuestInfoElement(infoCounter)
        quest:RemoveQuestInfoElement(i_stk_1d0_2)
        quest:RemoveQuestInfoElement(infoElement)
        quest:EntitySetTargetable(me, true)
        resource9 = resources:NewResource()
        resources:PrepareResource(resource9)
        while not resources:TryAcquire(resource9, hero, 4) do
            if not quest:NewScriptFrame(me) then ReleaseEverything7(); return end
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything7(); return end
        actorMap4 = resources:NewActorMap()
        resources:SetActor(actorMap4, "HERO", resource9)
        resources:SetActor(actorMap4, "TEACHER", resource)
        movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        if c_stk_215_3 == 0 then
            if not quest:IsActiveThreadTerminating() then
                local getMasterGameState = quest:GetMasterGameState("SkillScore")
                index = 0
                scratchValue = 0
                repeat
                    scratchValue29 = scratchValue
                    if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_SkillGrades, index) < getMasterGameState ~= (quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_SkillGrades, index) == getMasterGameState) then
                        if quest:IsActiveThreadTerminating() then ReleaseEverything6(); return end
                        break
                    end
                    scratchValue = scratchValue29 + 1
                    index = index + 1
                until scratchValue >= 7
                local actorMap = resources:NewStringMap()
                repeat
                    if scratchValue29 == 0 then
                        resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_APLUS")
                        break
                    elseif scratchValue29 == 1 then
                        resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_A")
                        break
                    elseif scratchValue29 == 2 then
                        resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_B")
                        break
                    elseif scratchValue29 == 3 then
                        resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_C")
                        break
                    elseif scratchValue29 == 4 then
                        resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_D")
                        break
                    elseif scratchValue29 == 5 then
                        resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_E")
                        break
                    elseif scratchValue29 == 6 then
                        resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_F")
                        break
                    else
                        goto FLOW_native_label_1
                    end
                until true
                ::FLOW_native_label_1::
                resources:RunMacroWithStrings("CS_GUILD_SKILL_WON_START", actorMap4, actorMap, false, false)
                resources:DestroyStringMap(actorMap)
            else
                ReleaseEverything6(); return
            end
        else
            if quest:IsActiveThreadTerminating() then ReleaseEverything6(); return end
            resources:RunMacro("CS_GUILD_SKILL_DISQUALIFIED", actorMap4, false, true)
        end
        quest:SetStateInt("TutorialState", 0)
        quest:RemoveQuestInfoElement(infoCounter2)
        quest:Pause(2.0)
        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_SKILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
        while questionAnswer < 0 do
            if not quest:NewScriptFrame(me) then ReleaseEverything6(); return end
            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything6(); return end
        if questionAnswer == 1 then
            if true then
                resources:RunMacro("CS_GUILD_SKILL_CONTINUE", actorMap4, false, true)
                quest:SetMasterGameState("SkillRepeating", false)
            else
                ReleaseEverything6()
                return
            end
        else
            resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap4, false, false)
            quest:SetMasterGameState("MovingDummiesNeeded", false)
            quest:SetMasterGameState("SkillRepeating", true)
            quest:SetMasterGameState("SkillRepeatKnown", true)
            if quest:IsObjectInThingsPossession("OBJECT_YEW_LONGBOW", hero) then
                if quest:IsActiveThreadTerminating() then ReleaseEverything6(); return end
                quest:TakeObjectFromHero("OBJECT_YEW_LONGBOW")
            end
            while not quest:GetMasterGameState("SkillDummyReset") do
                if not quest:NewScriptFrame(me) then ReleaseEverything6(); return end
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything6(); return end
        end
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        resources:DestroyActorMap(actorMap4)
        resources:ReleaseResource(resource9)
        quest:DeregisterTimer(timerId4)
        quest:DeregisterTimer(timerId5)
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d5da96 end
    me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 3.0, ENTITY_MOVE_WALK, false, true)
    guildEvilApprenticeMale = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("SkillApprenticeMarker"):GetPos(), "SkillApprentice")
    if guildEvilApprenticeMale ~= nil then
        guildEvilApprenticeMale:SetToKillOnLevelUnload(false)
    end
    quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("BirdKillerMarker"):GetPos(), "BirdKiller")
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_05", "", "")
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    quest:SetPlayerUsingRangedDummies(false)
    ::LAB_00d5da96::
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
end

-- TheRealGuildmaster.Init (retail 0x00d5ac90)
function Init(quest, me)
    playerNotWarned = true
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

