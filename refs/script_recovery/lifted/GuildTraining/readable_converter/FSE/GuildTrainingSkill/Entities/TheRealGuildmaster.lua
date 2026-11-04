-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

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
    local resources = quest:RetailResources()
    local addQuestInfoTickByText, ticked, c_stk_215_1, c_stk_215_2, c_stk_215_3, delay
    local tutorialState, scratchValue18, questionAnswer, scratchValue21, conversationId
    local addNewConversation, scratchValue30, i_stk_1d0_1, i_stk_1ec_1, i_stk_1ec_2, i_stk_1ec_3
    local timerId3, index, p4, hero3, scriptThing, hero29, guildEvilApprenticeMale2, actorMap
    local actorMap2, resource7, resource8, resource9, actorMap3, movie, movie2
    local addQuestInfoTickByText2, scratchValue40, addQuestInfoTickByText3, addQuestInfoTickByText4
    local resource10, timerId4, timerId5, movie3
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap2)
        resources:ReleaseResource(resource7)
        quest:DeregisterTimer(timerId3)
        resources:ReleaseResource(resource10)
    end
    local function ReleaseEverything2()
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap2)
        resources:ReleaseResource(resource7)
        quest:DeregisterTimer(timerId3)
        resources:ReleaseResource(resource10)
    end
    local function ReleaseEverything3()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie3)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource8)
        quest:DeregisterTimer(timerId5)
        quest:DeregisterTimer(timerId3)
        resources:ReleaseResource(resource10)
    end
    local function ReleaseEverything4()
        resources:DestroyMovie(movie3)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource8)
        quest:DeregisterTimer(timerId5)
        quest:DeregisterTimer(timerId3)
        resources:ReleaseResource(resource10)
    end
    local function ReleaseEverything5()
        resources:ReleaseResource(resource8)
        quest:DeregisterTimer(timerId5)
        quest:DeregisterTimer(timerId3)
        resources:ReleaseResource(resource10)
    end
    local function ReleaseEverything6()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        resources:DestroyActorMap(actorMap3)
        resources:ReleaseResource(resource9)
        quest:DeregisterTimer(timerId4)
        quest:DeregisterTimer(timerId5)
        quest:DeregisterTimer(timerId3)
        resources:ReleaseResource(resource10)
    end
    local function ReleaseEverything7()
        resources:ReleaseResource(resource9)
        quest:DeregisterTimer(timerId4)
        quest:DeregisterTimer(timerId5)
        quest:DeregisterTimer(timerId3)
        resources:ReleaseResource(resource10)
    end
    local function ReleaseEverything8()
        quest:DeregisterTimer(timerId4)
        quest:DeregisterTimer(timerId5)
        quest:DeregisterTimer(timerId3)
        resources:ReleaseResource(resource10)
    end
    resource10 = resources:NewResource()
    resources:PrepareResource(resource10)
    while not resources:TryAcquire(resource10, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource10)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource10); return end
    quest:EntitySetAsKillable(me, false, true)
    ticked = true
    local hero = quest:GetHero()
    quest:EntitySetAlwaysBlockAttacksFromThing(me, hero, true)
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    local position = quest:GetThingWithScriptName("M_SkillTeacherStand"):GetPos()
    me:MoveToPosition(position, 3.0, ENTITY_MOVE_WALK, false, true)
    quest:SetPlayerUsingRangedDummies(true)
    timerId3 = quest:RegisterTimer()
    quest:SetTimer(timerId3, 0)
    tutorialState = quest:GetStateInt("TutorialState")
    i_stk_1ec_1 = 0
    while tutorialState == 1 do
        quest:NewScriptFrame(me)
        ticked = quest:IsActiveThreadTerminating()
        scratchValue21 = timerId3
        if ticked then quest:DeregisterTimer(timerId3); resources:ReleaseResource(resource10); return end
        if me:IsTalkedToByHero() then
            ticked = quest:IsActiveThreadTerminating()
            scratchValue21 = timerId3
            if ticked then quest:DeregisterTimer(timerId3); resources:ReleaseResource(resource10); return end
            if not quest:GetMasterGameState("HeroTakingGuildTest") then
                ticked = quest:IsActiveThreadTerminating()
                scratchValue21 = timerId3
                if ticked then quest:DeregisterTimer(timerId3); resources:ReleaseResource(resource10); return end
                me:ClearCommands()
                quest:SetStateInt("TutorialState", 3)
            else
                ticked = quest:IsActiveThreadTerminating()
                scratchValue21 = timerId3
                if ticked then quest:DeregisterTimer(timerId3); resources:ReleaseResource(resource10); return end
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                me:ClearCommands()
                scriptThing = resources:ScriptThing(resource10)
                local fret_0 = quest:GetHealth(scriptThing)
                scriptThing = nil
                if 0.0 < fret_0 then
                    local line = "TEXT_QST_028_GUILDMASTER_SKILL_NOT_START"
                    local hero18 = quest:GetHero()
                    if not me:Speak(hero18, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(timerId3)
                        resources:ReleaseResource(resource10)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            quest:DeregisterTimer(timerId3)
                            resources:ReleaseResource(resource10)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        scratchValue21 = timerId3
                        quest:DeregisterTimer(timerId3)
                        resources:ReleaseResource(resource10)
                        return
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            end
        end
        local hero2 = quest:GetHero()
        ticked = quest:IsDistanceBetweenThingsUnder(hero2, me, 5.5)
        scratchValue21 = timerId3
        local scratchValue2 = ticked and quest:GetTimer(timerId3) < 1
        local scratchValue = scratchValue2 and not me:IsPerformingScriptTask()
        if not scratchValue then scratchValue21 = timerId3; tutorialState = quest:GetStateInt("TutorialState"); goto continue_3 end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId3); resources:ReleaseResource(resource10); return end
        conversationId = quest:AddNewConversation(me, false, false)
        hero3 = quest:GetHero()
        quest:AddPersonToConversation(conversationId, hero3)
        quest:SetTimer(timerId3, 10)
        if i_stk_1ec_1 == 0 then
            ticked = false
            local hero4 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, hero4, false)
            local hero5 = quest:GetHero()
            quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_FIRST", me, hero5, false)
        elseif i_stk_1ec_1 == 1 then
            ticked = false
            local hero6 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, hero6, false)
            local hero7 = quest:GetHero()
            quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_SECOND", me, hero7, false)
        end
        i_stk_1ec_1 = 1 - i_stk_1ec_1
        scratchValue21 = timerId3
        tutorialState = quest:GetStateInt("TutorialState")
        ::continue_3::
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId3)
        resources:ReleaseResource(resource10)
        return
    end
    quest:SetMasterGameState("SkillTrainingStarted", true)
    quest:SetMasterGameState("SkillRepeating", true)
    quest:SetMasterGameState("HeroTakingGuildTest", true)
    while quest:GetMasterGameState("SkillRepeating") do
        quest:NewScriptFrame(me)
        ticked = quest:IsActiveThreadTerminating()
        scratchValue21 = timerId3
        if ticked then quest:DeregisterTimer(timerId3); resources:ReleaseResource(resource10); return end
        quest:SetStateInt("TutorialState", 3)
        quest:SetMasterGameState("SkillRepeatKnown", false)
        resource7 = resources:NewResource()
        resources:PrepareResource(resource7)
        local resource = resource7
        local hero19 = quest:GetHero()
        ticked = resources:TryAcquire(resource, hero19, 4)
        while not ticked do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource7)
                quest:DeregisterTimer(timerId3)
                resources:ReleaseResource(resource10)
                return
            end
            local resource2 = resource7
            local hero20 = quest:GetHero()
            ticked = resources:TryAcquire(resource2, hero20, 4)
        end
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource7)
            quest:DeregisterTimer(timerId3)
            resources:ReleaseResource(resource10)
            return
        end
        actorMap2 = resources:NewActorMap()
        resources:SetActor(actorMap2, "HERO", resource7)
        resources:SetActor(actorMap2, "TEACHER", resource10)
        quest:SetStateInt("TutorialState", 2)
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_GUILD_SKILL_START", actorMap2, false, true)
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
        resources:DestroyActorMap(actorMap2)
        resources:ReleaseResource(resource7)
        local infoCounter2 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 3, 1.0)
        timerId5 = quest:RegisterTimer()
        local timerId = timerId5
        quest:SetTimer(timerId5, 15)
        quest:SetMasterGameState("SkillScore", 0)
        local archeryRing2 = quest:GetThingWithScriptName("ArcheryRing")
        i_stk_1d0_1 = quest:GetMasterGameState("SkillScore")
        quest:EntitySetTargetable(me, false)
        if not quest:IsXbox() then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId5); quest:DeregisterTimer(timerId3); resources:ReleaseResource(resource10); return end
            addQuestInfoTickByText2 = quest:AddQuestInfoTickByText("GAME_ACTION_UNSHEATHE_RANGED_WEAPON", false, 1.0)
            i_stk_1ec_2 = quest:AddQuestInfoTickByText("GAME_ACTION_LOCK_TARGET", false, 1.0)
            addQuestInfoTickByText3 = quest:AddQuestInfoTickByText("GAME_ACTION_FIRE_RANGED_WEAPON", false, 1.0)
            goto LAB_00d5ba3e
            quest:DeregisterTimer(timerId5)
            quest:DeregisterTimer(timerId3)
            resources:ReleaseResource(resource10)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId5)
            quest:DeregisterTimer(timerId3)
            resources:ReleaseResource(resource10)
            return
        end
        addQuestInfoTickByText2 = quest:AddQuestInfoTickByText("HUD_BLACK_BUTTON", false, 1.0)
        i_stk_1ec_2 = quest:AddQuestInfoTickByText("HUD_CONTROLLER_TRIGGER_LEFT", false, 1.0)
        addQuestInfoTickByText3 = quest:AddQuestInfoTickByText("HUD_CONTROLLER_X", false, 1.0)
        ::LAB_00d5ba3e::
        c_stk_215_1 = 0
        quest:DisplayQuestInfo(true)
        quest:SetTimer(timerId, 5)
        addNewConversation = 0
        while quest:GetMasterGameState("SkillScore") < 3 do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId5)
                quest:DeregisterTimer(timerId3)
                resources:ReleaseResource(resource10)
                return
            end
            local isPlayerCarryingItemOfType = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") and quest:IsPlayerHoldingLockTargetButton()
            local scratchValue3 = isPlayerCarryingItemOfType and quest:IsPlayerHoldingFireRangedWeaponButton()
            ticked = scratchValue3
            if ticked then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId3)
                    resources:ReleaseResource(resource10)
                    return
                end
                c_stk_215_1 = 1
                quest:RemoveQuestInfoElement(addQuestInfoTickByText2)
                quest:RemoveQuestInfoElement(i_stk_1ec_2)
                quest:RemoveQuestInfoElement(addQuestInfoTickByText3)
            elseif c_stk_215_1 == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId3)
                    resources:ReleaseResource(resource10)
                    return
                end
                ticked = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                quest:UpdateQuestInfoTick(addQuestInfoTickByText2, ticked)
                ticked = quest:IsPlayerHoldingLockTargetButton()
                quest:UpdateQuestInfoTick(i_stk_1ec_2, ticked)
                ticked = quest:IsPlayerHoldingFireRangedWeaponButton()
                quest:UpdateQuestInfoTick(addQuestInfoTickByText3, ticked)
                if quest:GetTimer(timerId5) < 1 then
                    local hero21 = quest:GetHero()
                    local isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(hero21, archeryRing2, 6.0) and not quest:IsConversationActive(addNewConversation)
                    if isDistanceBetweenThingsUnder then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId5)
                            quest:DeregisterTimer(timerId3)
                            resources:ReleaseResource(resource10)
                            return
                        end
                        if quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") then
                            if quest:IsPlayerHoldingLockTargetButton() then
                                if quest:IsPlayerHoldingFireRangedWeaponButton() then goto LAB_00d5bf6c end
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId5)
                                    quest:DeregisterTimer(timerId3)
                                    resources:ReleaseResource(resource10)
                                    return
                                end
                                addNewConversation = quest:AddNewConversation(me, false, false)
                                local hero8 = quest:GetHero()
                                quest:AddPersonToConversation(addNewConversation, hero8)
                                if quest:IsXbox() then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId5)
                                        quest:DeregisterTimer(timerId3)
                                        resources:ReleaseResource(resource10)
                                        return
                                    end
                                    local hero9 = quest:GetHero()
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_30", me, hero9, false)
                                else
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId5)
                                        quest:DeregisterTimer(timerId3)
                                        resources:ReleaseResource(resource10)
                                        return
                                    end
                                    local hero10 = quest:GetHero()
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_30", me, hero10, false)
                                end
                            else
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId5)
                                    quest:DeregisterTimer(timerId3)
                                    resources:ReleaseResource(resource10)
                                    return
                                end
                                addNewConversation = quest:AddNewConversation(me, false, false)
                                local hero11 = quest:GetHero()
                                quest:AddPersonToConversation(addNewConversation, hero11)
                                if quest:IsXbox() then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId5)
                                        quest:DeregisterTimer(timerId3)
                                        resources:ReleaseResource(resource10)
                                        return
                                    end
                                    local hero12 = quest:GetHero()
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_20", me, hero12, false)
                                else
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId5)
                                        quest:DeregisterTimer(timerId3)
                                        resources:ReleaseResource(resource10)
                                        return
                                    end
                                    local hero13 = quest:GetHero()
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_20", me, hero13, false)
                                end
                            end
                        else
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId5)
                                quest:DeregisterTimer(timerId3)
                                resources:ReleaseResource(resource10)
                                return
                            end
                            addNewConversation = quest:AddNewConversation(me, false, false)
                            local hero14 = quest:GetHero()
                            quest:AddPersonToConversation(addNewConversation, hero14)
                            if quest:IsXbox() then
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId5)
                                    quest:DeregisterTimer(timerId3)
                                    resources:ReleaseResource(resource10)
                                    return
                                end
                                local hero15 = quest:GetHero()
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_10", me, hero15, false)
                            else
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId5)
                                    quest:DeregisterTimer(timerId3)
                                    resources:ReleaseResource(resource10)
                                    return
                                end
                                local hero16 = quest:GetHero()
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_10", me, hero16, false)
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
                    quest:DeregisterTimer(timerId3)
                    resources:ReleaseResource(resource10)
                    return
                end
                i_stk_1d0_1 = quest:GetMasterGameState("SkillScore")
            elseif getMasterGameState5 < i_stk_1d0_1 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId3)
                    resources:ReleaseResource(resource10)
                    return
                end
                quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + 1)
                if not quest:IsConversationActive(addNewConversation) then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId5)
                        quest:DeregisterTimer(timerId3)
                        resources:ReleaseResource(resource10)
                        return
                    end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    local hero22 = quest:GetHero()
                    quest:AddPersonToConversation(addNewConversation, hero22)
                    local hero23 = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", me, hero23, false)
                end
            end
            quest:UpdateQuestInfoCounter(infoCounter2, quest:GetMasterGameState("SkillScore"), -1)
            if playerNotWarned then
                local hero24 = quest:GetHero()
                local isDistanceBetweenThingsOver = quest:IsDistanceBetweenThingsOver(hero24, archeryRing2, 6.0) and not quest:IsConversationActive(addNewConversation)
                if isDistanceBetweenThingsOver then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId5)
                        quest:DeregisterTimer(timerId3)
                        resources:ReleaseResource(resource10)
                        return
                    end
                    playerNotWarned = false
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    local hero25 = quest:GetHero()
                    quest:AddPersonToConversation(addNewConversation, hero25)
                    local hero26 = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_RING_OUT", me, hero26, false)
                end
            end
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId5)
            quest:DeregisterTimer(timerId3)
            resources:ReleaseResource(resource10)
            return
        end
        if c_stk_215_1 == 0 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId5)
                quest:DeregisterTimer(timerId3)
                resources:ReleaseResource(resource10)
                return
            end
            quest:RemoveQuestInfoElement(addQuestInfoTickByText2)
            quest:RemoveQuestInfoElement(i_stk_1ec_2)
            quest:RemoveQuestInfoElement(addQuestInfoTickByText3)
        end
        resource8 = resources:NewResource()
        resources:PrepareResource(resource8)
        local resource3 = resource8
        local hero27 = quest:GetHero()
        ticked = resources:TryAcquire(resource3, hero27, 4)
        while not ticked do
            if not quest:NewScriptFrame(me) then ReleaseEverything5(); return end
            local resource4 = resource8
            local hero28 = quest:GetHero()
            ticked = resources:TryAcquire(resource4, hero28, 4)
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything5(); return end
        actorMap = resources:NewActorMap()
        resources:SetActor(actorMap, "HERO", resource8)
        resources:SetActor(actorMap, "TEACHER", resource10)
        movie3 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_GUILD_SKILL_MOVE", actorMap, false, true)
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
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource8)
        if not quest:IsXbox() then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId5); quest:DeregisterTimer(timerId3); resources:ReleaseResource(resource10); return end
            addQuestInfoTickByText = quest:AddQuestInfoTickByText("GAME_ACTION_UNSHEATHE_RANGED_WEAPON", false, 1.0)
            scratchValue40 = addQuestInfoTickByText
            i_stk_1ec_3 = quest:AddQuestInfoTickByText("GAME_ACTION_TOGGLE_FIRST_PERSON_VIEW", false, 1.0)
            addQuestInfoTickByText4 = quest:AddQuestInfoTickByText("GAME_ACTION_FIRE_RANGED_WEAPON", false, 1.0)
            goto LAB_00d5c6b1
            quest:DeregisterTimer(timerId5)
            quest:DeregisterTimer(timerId3)
            resources:ReleaseResource(resource10)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId5)
            quest:DeregisterTimer(timerId3)
            resources:ReleaseResource(resource10)
            return
        end
        addQuestInfoTickByText = quest:AddQuestInfoTickByText("HUD_BLACK_BUTTON", false, 1.0)
        scratchValue40 = addQuestInfoTickByText
        i_stk_1ec_3 = quest:AddQuestInfoTickByText("HUD_CONTROLLER_THUMBSTICK_LEFT_CLICK", false, 1.0)
        addQuestInfoTickByText4 = quest:AddQuestInfoTickByText("HUD_CONTROLLER_X", false, 1.0)
        ::LAB_00d5c6b1::
        c_stk_215_2 = 0
        quest:DisplayQuestInfo(true)
        repeat
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId5)
                quest:DeregisterTimer(timerId3)
                resources:ReleaseResource(resource10)
                return
            end
            if not (quest:GetTimer(timerId5) < 1) then goto LAB_00d5c757 end
            if quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") then goto LAB_00d5c757 end
            hero29 = quest:GetHero()
            if not quest:IsDistanceBetweenThingsUnder(hero29, archeryRing2, 6.0) then goto LAB_00d5c757 end
            ticked = true
            goto FLOW_past_lab_00d5c757
            ::LAB_00d5c757::
            ticked = false
            ::FLOW_past_lab_00d5c757::
            if ticked then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId3)
                    resources:ReleaseResource(resource10)
                    return
                end
                local conversationId2 = quest:AddNewConversation(me, false, false)
                local hero30 = quest:GetHero()
                quest:AddPersonToConversation(conversationId2, hero30)
                local hero31 = quest:GetHero()
                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_MAZE_BOW_UNSHEATH", me, hero31, false)
                if quest:IsXbox() then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId5)
                        quest:DeregisterTimer(timerId3)
                        resources:ReleaseResource(resource10)
                        return
                    end
                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP")
                    while not quest:MsgIsGameInfoClickedPast() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId5)
                            quest:DeregisterTimer(timerId3)
                            resources:ReleaseResource(resource10)
                            do return end
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId5)
                        quest:DeregisterTimer(timerId3)
                        resources:ReleaseResource(resource10)
                        return
                    end
                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId5)
                            quest:DeregisterTimer(timerId3)
                            resources:ReleaseResource(resource10)
                            do return end
                        end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId3)
                    resources:ReleaseResource(resource10)
                    return
                end
                quest:SetTimer(timerId5, 15)
                addQuestInfoTickByText = scratchValue40
            end
            if quest:MsgOnHeroFiredRangedWeapon() then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId3)
                    resources:ReleaseResource(resource10)
                    return
                end
            end
            local isPlayerCarryingItemOfType2 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") and quest:IsHeroInProjectileWeaponMode()
            local scratchValue7 = isPlayerCarryingItemOfType2 and quest:IsPlayerHoldingFireRangedWeaponButton()
            ticked = scratchValue7
            if ticked then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId3)
                    resources:ReleaseResource(resource10)
                    return
                end
                c_stk_215_2 = 1
                quest:RemoveQuestInfoElement(addQuestInfoTickByText)
                quest:RemoveQuestInfoElement(i_stk_1ec_3)
                quest:RemoveQuestInfoElement(addQuestInfoTickByText4)
            elseif c_stk_215_2 == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId5)
                    quest:DeregisterTimer(timerId3)
                    resources:ReleaseResource(resource10)
                    return
                end
                ticked = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                quest:UpdateQuestInfoTick(addQuestInfoTickByText, ticked)
                ticked = quest:IsHeroInProjectileWeaponMode()
                quest:UpdateQuestInfoTick(i_stk_1ec_3, ticked)
                ticked = quest:IsPlayerHoldingFireRangedWeaponButton()
                quest:UpdateQuestInfoTick(addQuestInfoTickByText4, ticked)
            end
        until true
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId5)
            quest:DeregisterTimer(timerId3)
            resources:ReleaseResource(resource10)
            return
        end
        if c_stk_215_2 == 0 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId5)
                quest:DeregisterTimer(timerId3)
                resources:ReleaseResource(resource10)
                return
            end
            quest:RemoveQuestInfoElement(addQuestInfoTickByText)
            quest:RemoveQuestInfoElement(i_stk_1ec_3)
            quest:RemoveQuestInfoElement(addQuestInfoTickByText4)
        end
        timerId4 = quest:RegisterTimer()
        local scratchValue27 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_SkillTimer)))
        quest:SetTimer(timerId4, scratchValue27)
        quest:SetMasterGameState("SkillScore", 0)
        c_stk_215_3 = 0
        local infoCounter = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
        local i_stk_1ec_4 = infoCounter
        local i_stk_1d0_2 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
        local infoElement = quest:AddQuestInfoTimer(timerId4, "HUD_CLOCK_ICON", 1.0)
        quest:DisplayQuestInfo(true)
        quest:UpdateQuestInfoCounter(infoCounter, quest:GetMasterGameState("HighestSkillScore"), -1)
        quest:SetMasterGameState("SkillTestOccuring", true)
        quest:SetTimer(timerId5, 15)
        while 0 < quest:GetTimer(timerId4) and c_stk_215_3 == 0 do
            if not quest:NewScriptFrame(me) then ReleaseEverything8(); return end
            if quest:GetMasterGameState("GuildWarningOccuring") then
                if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
                c_stk_215_3 = 1
            end
            local scratchValue17 = i_stk_1d0_2
            quest:UpdateQuestInfoCounter(i_stk_1d0_2, quest:GetMasterGameState("SkillScore"), -1)
            if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
                quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                quest:UpdateQuestInfoCounter(scratchValue17, quest:GetMasterGameState("HighestSkillScore"), -1)
            end
            if not (quest:GetTimer(timerId5) < 1) then goto LAB_00d5cd47 end
            if quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") then goto LAB_00d5cd47 end
            ticked = true
            goto FLOW_past_lab_00d5cd47
            ::LAB_00d5cd47::
            ticked = false
            ::FLOW_past_lab_00d5cd47::
            if ticked then
                if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
                local conversationId3 = quest:AddNewConversation(me, false, false)
                local hero32 = quest:GetHero()
                quest:AddPersonToConversation(conversationId3, hero32)
                local hero33 = quest:GetHero()
                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_MAZE_BOW_UNSHEATH", me, hero33, false)
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
                local archeryRing = quest:GetThingWithScriptName("ArcheryRing")
                local hero17 = quest:GetHero()
                if not quest:IsDistanceBetweenThingsOver(hero17, archeryRing, 6.0) then goto LAB_00d5cf7a end
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
                local hero34 = quest:GetHero()
                quest:AddPersonToConversation(conversationId4, hero34)
                local hero35 = quest:GetHero()
                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_MAZE_RING_OUT", me, hero35, false)
            end
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
        quest:SetMasterGameState("HeroTakingGuildTest", false)
        quest:SetMasterGameState("SkillTestOccuring", false)
        while not quest:IsHeroControlledByPlayer() do
            if not quest:NewScriptFrame(me) then ReleaseEverything8(); return end
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
        quest:DisplayQuestInfo(false)
        quest:RemoveQuestInfoElement(i_stk_1ec_4)
        quest:RemoveQuestInfoElement(i_stk_1d0_2)
        quest:RemoveQuestInfoElement(infoElement)
        quest:EntitySetTargetable(me, true)
        resource9 = resources:NewResource()
        resources:PrepareResource(resource9)
        local resource5 = resource9
        local hero36 = quest:GetHero()
        ticked = resources:TryAcquire(resource5, hero36, 4)
        while not ticked do
            if not quest:NewScriptFrame(me) then ReleaseEverything7(); return end
            local resource6 = resource9
            local hero37 = quest:GetHero()
            ticked = resources:TryAcquire(resource6, hero37, 4)
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything7(); return end
        actorMap3 = resources:NewActorMap()
        resources:SetActor(actorMap3, "HERO", resource9)
        resources:SetActor(actorMap3, "TEACHER", resource10)
        movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        if c_stk_215_3 == 0 then
            if quest:IsActiveThreadTerminating() then ReleaseEverything6(); return end
            local getMasterGameState = quest:GetMasterGameState("SkillScore")
            index = 0
            scratchValue18 = 0
            repeat
                scratchValue30 = scratchValue18
                if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_SkillGrades, index) < getMasterGameState ~= (quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_SkillGrades, index) == getMasterGameState) then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything6(); return end
                    break
                end
                scratchValue18 = scratchValue30 + 1
                index = index + 1
            until not (scratchValue18 < 7)
            local actorMap4 = resources:NewStringMap()
            local switch = scratchValue30
            repeat
                if switch == 0 then
                    resources:SetString(actorMap4, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_APLUS")
                    break
                elseif switch == 1 then
                    resources:SetString(actorMap4, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_A")
                    break
                elseif switch == 2 then
                    resources:SetString(actorMap4, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_B")
                    break
                elseif switch == 3 then
                    resources:SetString(actorMap4, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_C")
                    break
                elseif switch == 4 then
                    resources:SetString(actorMap4, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_D")
                    break
                elseif switch == 5 then
                    resources:SetString(actorMap4, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_E")
                    break
                elseif switch == 6 then
                    resources:SetString(actorMap4, "$GRADE", "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_F")
                    break
                else
                    break
                end
            until true
            resources:RunMacroWithStrings("CS_GUILD_SKILL_WON_START", actorMap3, actorMap4, false, false)
            resources:DestroyStringMap(actorMap4)
        else
            if quest:IsActiveThreadTerminating() then ReleaseEverything6(); return end
            resources:RunMacro("CS_GUILD_SKILL_DISQUALIFIED", actorMap3, false, true)
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
        ticked = quest:IsActiveThreadTerminating()
        if questionAnswer == 1 then
            if not ticked then
                resources:RunMacro("CS_GUILD_SKILL_CONTINUE", actorMap3, false, true)
                quest:SetMasterGameState("SkillRepeating", false)
            else
                ReleaseEverything6()
                return
            end
        else
            if ticked then ReleaseEverything6(); return end
            resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap3, false, false)
            quest:SetMasterGameState("MovingDummiesNeeded", false)
            quest:SetMasterGameState("SkillRepeating", true)
            quest:SetMasterGameState("SkillRepeatKnown", true)
            local hero38 = quest:GetHero()
            if quest:IsObjectInThingsPossession("OBJECT_YEW_LONGBOW", hero38) then
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
        resources:DestroyActorMap(actorMap3)
        resources:ReleaseResource(resource9)
        quest:DeregisterTimer(timerId4)
        quest:DeregisterTimer(timerId5)
        scratchValue21 = timerId3
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId3); resources:ReleaseResource(resource10); return end
    p4 = 1
    local position2 = quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos()
    me:MoveToPosition(position2, 3.0, ENTITY_MOVE_WALK, false, true)
    ticked = false
    local position3 = quest:GetThingWithScriptName("SkillApprenticeMarker"):GetPos()
    local guildEvilApprenticeMale = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", position3, "SkillApprentice", false)
    if guildEvilApprenticeMale ~= nil then
        guildEvilApprenticeMale:SetToKillOnLevelUnload(false)
    end
    ticked = false
    local position4 = quest:GetThingWithScriptName("BirdKillerMarker"):GetPos()
    guildEvilApprenticeMale2 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", position4, "BirdKiller", false)
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_05", "", "")
    delay = 0
    local getActiveQuestName = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName, 0)
    quest:SetPlayerUsingRangedDummies(false)
    quest:DeregisterTimer(timerId3)
    resources:ReleaseResource(resource10)
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

