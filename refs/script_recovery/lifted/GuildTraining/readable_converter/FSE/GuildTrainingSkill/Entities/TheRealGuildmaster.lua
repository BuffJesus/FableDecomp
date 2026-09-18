-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_SkillGrades = 3776,  -- '07000000000016430000f0420000a042000048420000c8410000204100000000'
    GUI_SkillTimer = 3844,  -- 60.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local playerNotWarned

-- TheRealGuildmaster.Main (retail 0x00d5ae70)
function Main(quest, me)
    local hero_ = quest:GetHero()
    local resources = quest:RetailResources()
    local addQuestInfoTickByText, scratchValue3, scratchValue4, scratchValue7, scratchValue8
    local scratchValue9, scratchValue, scratchValue12, scratchValue14, scratchValue15
    local scratchValue18, switch, hero, scratchValue19, archeryRing, guildEvilApprenticeMale
    local actorMap, actorMap2, resource, resource3, resource4, actorMap3, movie, movie2
    local scratchValue22, scratchValue23, scratchValue24, addQuestInfoTickByText2
    local addQuestInfoTickByText3, resource5, timerId, timerId2, scratchValue25, movie3
    local function __cleanup_LAB_00d5dac4()
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap2)
        resources:ReleaseResource(resource)
        quest:DeregisterTimer(scratchValue25)
        resources:ReleaseResource(resource5)
    end
    local function __cleanup_LAB_00d5db05()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap2)
        resources:ReleaseResource(resource)
        quest:DeregisterTimer(scratchValue25)
        resources:ReleaseResource(resource5)
    end
    local function __cleanup_LAB_00d5db37()
        resources:ReleaseResource(resource3)
        quest:DeregisterTimer(timerId2)
        quest:DeregisterTimer(scratchValue25)
        resources:ReleaseResource(resource5)
    end
    local function __cleanup_LAB_00d5db90()
        quest:DeregisterTimer(timerId)
        quest:DeregisterTimer(timerId2)
        quest:DeregisterTimer(scratchValue25)
        resources:ReleaseResource(resource5)
    end
    resource5 = resources:NewResource()
    while not resources:TryAcquire(resource5, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource5); return end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:EntitySetAsKillable(me, false, true)
        quest:EntitySetAlwaysBlockAttacksFromThing(me, hero_, true)
        quest:SetIsPushableByHero(me, false)
        quest:SetThingHasInformation(me, false, false, false)
        me:MoveToPosition(quest:GetThingWithScriptName("M_SkillTeacherStand"):GetPos(), 0x40400000, 0, false, true)
        quest:SetPlayerUsingRangedDummies(true)
        scratchValue25 = quest:RegisterTimer()
        quest:SetTimer(scratchValue25, 0)
        while quest:GetStateInt("TutorialState") == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d5da96 end
            if me:IsTalkedToByHero() then
                if quest:GetMasterGameState("HeroTakingGuildTest") == 0 then
                    me:ClearCommands()
                    quest:SetStateInt("TutorialState", 3)
                else
                    movie2 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    me:ClearCommands()
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource5)) then
                        if not me:Speak(hero_, "TEXT_QST_028_GUILDMASTER_SKILL_NOT_START", GROUP_SELECT_FIRST, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            quest:DeregisterTimer(scratchValue25)
                            resources:ReleaseResource(resource5)
                            return
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
            scratchValue12 = 5.5
            if (quest:IsDistanceBetweenThingsUnder(hero_, me, 5.5) and quest:GetTimer(scratchValue25) < 1) and not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d5da96 end
                scratchValue15 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue15, hero_)
                quest:SetTimer(scratchValue25, 10)
                if true then
                    quest:EntitySetFacingAngleTowardsThing(me, hero_, false)
                    quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_FIRST", me, hero_, false)
                elseif 0 == 1 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero_, false)
                    quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_SECOND", me, hero_, false)
                end
                -- TODO(native): xStack_1ec = 1 - xStack_1ec;
            end
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue25)
            resources:ReleaseResource(resource5)
            return
        end
        quest:SetMasterGameState("SkillTrainingStarted", true)
        quest:SetMasterGameState("SkillRepeating", true)
        quest:SetMasterGameState("HeroTakingGuildTest", true)
        while quest:GetMasterGameState("SkillRepeating") ~= 0 do
            if not quest:NewScriptFrame(me) then goto LAB_00d5da96 end
            quest:SetStateInt("TutorialState", 3)
            quest:SetMasterGameState("SkillRepeatKnown", false)
            resource = resources:NewResource()
            while not resources:TryAcquire(resource, hero_, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    quest:DeregisterTimer(scratchValue25)
                    resources:ReleaseResource(resource5)
                    return
                end
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource)
                quest:DeregisterTimer(scratchValue25)
                resources:ReleaseResource(resource5)
                return
            end
            actorMap2 = resources:NewActorMap()
            resources:SetActor(actorMap2, "HERO", resource)
            resources:SetActor(actorMap2, "TEACHER", resource5)
            quest:SetStateInt("TutorialState", 2)
            movie = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_SKILL_START", actorMap2, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetMasterGameState("SkillTrainingStarted", true)
            if not quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            __cleanup_LAB_00d5dac4(); return
                        end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)xStack_8c);
                        goto LAB_00d5b7c0
                    end
                end
                __cleanup_LAB_00d5db05()
                return
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5dab8: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                __cleanup_LAB_00d5dac4(); return
            end
            quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5db05(); return end
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5dab8
            ::LAB_00d5b7c0::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap2)
            resources:ReleaseResource(resource)
            scratchValue23 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 3, 1.0)
            timerId2 = quest:RegisterTimer()
            quest:SetTimer(timerId2, 15)
            quest:SetMasterGameState("SkillScore", 0)
            archeryRing = quest:GetThingWithScriptName("ArcheryRing")
            scratchValue22 = quest:GetMasterGameState("SkillScore")
            quest:EntitySetTargetable(me, false)
            if not quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    scratchValue24 = quest:AddQuestInfoTickByText("GAME_ACTION_UNSHEATHE_RANGED_WEAPON", false, 1.0)
                    addQuestInfoTickByText3 = quest:AddQuestInfoTickByText("GAME_ACTION_LOCK_TARGET", false, 1.0)
                    addQuestInfoTickByText2 = quest:AddQuestInfoTickByText("GAME_ACTION_FIRE_RANGED_WEAPON", false, 1.0)
                    goto LAB_00d5ba3e
                end
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(scratchValue25)
                resources:ReleaseResource(resource5)
                return
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(scratchValue25)
                resources:ReleaseResource(resource5)
                return
            end
            scratchValue24 = quest:AddQuestInfoTickByText("HUD_BLACK_BUTTON", false, 1.0)
            addQuestInfoTickByText3 = quest:AddQuestInfoTickByText("HUD_CONTROLLER_TRIGGER_LEFT", false, 1.0)
            addQuestInfoTickByText2 = quest:AddQuestInfoTickByText("HUD_CONTROLLER_X", false, 1.0)
            ::LAB_00d5ba3e::
            scratchValue = 0
            quest:DisplayQuestInfo(true)
            quest:SetTimer(timerId2, 5)
            scratchValue15 = 0
            scratchValue14 = quest:GetMasterGameState("SkillScore")
            while scratchValue14 < 3 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    quest:DeregisterTimer(scratchValue25)
                    resources:ReleaseResource(resource5)
                    return
                end
                scratchValue9 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                scratchValue4 = scratchValue9
                if scratchValue4 then
                    -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingLockTargetButton()
                    scratchValue4 = scratchValue9
                end
                scratchValue3 = scratchValue4
                if scratchValue3 then
                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingFireRangedWeaponButton()
                    scratchValue3 = scratchValue9
                end
                if scratchValue3 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        quest:DeregisterTimer(scratchValue25)
                        resources:ReleaseResource(resource5)
                        return
                    end
                    scratchValue = 1
                    quest:RemoveQuestInfoElement(scratchValue24)
                    quest:RemoveQuestInfoElement(addQuestInfoTickByText3)
                    quest:RemoveQuestInfoElement(addQuestInfoTickByText2)
                elseif scratchValue == 0 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        quest:DeregisterTimer(scratchValue25)
                        resources:ReleaseResource(resource5)
                        return
                    end
                    scratchValue9 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                    quest:UpdateQuestInfoTick(scratchValue24, scratchValue9)
                    -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingLockTargetButton()
                    quest:UpdateQuestInfoTick(addQuestInfoTickByText3, scratchValue9)
                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingFireRangedWeaponButton()
                    quest:UpdateQuestInfoTick(addQuestInfoTickByText2, scratchValue9)
                    if quest:GetTimer(timerId2) < 1 then
                        scratchValue12 = 6.0
                        if quest:IsDistanceBetweenThingsUnder(hero_, archeryRing, 6.0) and not quest:IsConversationActive(scratchValue15) then
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId2)
                                quest:DeregisterTimer(scratchValue25)
                                resources:ReleaseResource(resource5)
                                return
                            end
                            scratchValue9 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                            if scratchValue9 then
                                -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                                quest:IsPlayerHoldingLockTargetButton()
                                if scratchValue9 then
                                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                                    quest:IsPlayerHoldingFireRangedWeaponButton()
                                    if scratchValue9 then goto LAB_00d5bf6c end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId2)
                                        quest:DeregisterTimer(scratchValue25)
                                        resources:ReleaseResource(resource5)
                                        return
                                    end
                                    scratchValue15 = quest:AddNewConversation(me, false, false)
                                    quest:AddPersonToConversation(scratchValue15, hero_)
                                    if quest:IsXbox() then
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId2)
                                            quest:DeregisterTimer(scratchValue25)
                                            resources:ReleaseResource(resource5)
                                            return
                                        end
                                        quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_30", me, hero_, false)
                                    else
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId2)
                                            quest:DeregisterTimer(scratchValue25)
                                            resources:ReleaseResource(resource5)
                                            return
                                        end
                                        quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_30", me, hero_, false)
                                    end
                                else
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId2)
                                        quest:DeregisterTimer(scratchValue25)
                                        resources:ReleaseResource(resource5)
                                        return
                                    end
                                    scratchValue15 = quest:AddNewConversation(me, false, false)
                                    quest:AddPersonToConversation(scratchValue15, hero_)
                                    if quest:IsXbox() then
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId2)
                                            quest:DeregisterTimer(scratchValue25)
                                            resources:ReleaseResource(resource5)
                                            return
                                        end
                                        quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_20", me, hero_, false)
                                    else
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId2)
                                            quest:DeregisterTimer(scratchValue25)
                                            resources:ReleaseResource(resource5)
                                            return
                                        end
                                        quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_20", me, hero_, false)
                                    end
                                end
                            else
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId2)
                                    quest:DeregisterTimer(scratchValue25)
                                    resources:ReleaseResource(resource5)
                                    return
                                end
                                scratchValue15 = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(scratchValue15, hero_)
                                if quest:IsXbox() then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId2)
                                        quest:DeregisterTimer(scratchValue25)
                                        resources:ReleaseResource(resource5)
                                        return
                                    end
                                    quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_10", me, hero_, false)
                                else
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId2)
                                        quest:DeregisterTimer(scratchValue25)
                                        resources:ReleaseResource(resource5)
                                        return
                                    end
                                    quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_10", me, hero_, false)
                                end
                            end
                            quest:SetTimer(timerId2, 10)
                        end
                    end
                end
                ::LAB_00d5bf6c::
                scratchValue14 = quest:GetMasterGameState("SkillScore")
                if scratchValue22 < scratchValue14 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        quest:DeregisterTimer(scratchValue25)
                        resources:ReleaseResource(resource5)
                        return
                    end
                    scratchValue22 = quest:GetMasterGameState("SkillScore")
                elseif scratchValue14 < scratchValue22 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        quest:DeregisterTimer(scratchValue25)
                        resources:ReleaseResource(resource5)
                        return
                    end
                    -- TODO(native): *piVar2 = *piVar2 + 1;
                    if not quest:IsConversationActive(scratchValue15) then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId2)
                            quest:DeregisterTimer(scratchValue25)
                            resources:ReleaseResource(resource5)
                            return
                        end
                        scratchValue15 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue15, hero_)
                        quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", me, hero_, false)
                    end
                end
                quest:UpdateQuestInfoCounter(scratchValue23, quest:GetMasterGameState("SkillScore"), -1)
                if playerNotWarned then
                    scratchValue12 = 6.0
                    if quest:IsDistanceBetweenThingsOver(hero_, archeryRing, 6.0) and not quest:IsConversationActive(scratchValue15) then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId2)
                            quest:DeregisterTimer(scratchValue25)
                            resources:ReleaseResource(resource5)
                            return
                        end
                        playerNotWarned = false
                        scratchValue15 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue15, hero_)
                        quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_MAZE_RING_OUT", me, hero_, false)
                    end
                end
                scratchValue14 = quest:GetMasterGameState("SkillScore")
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(scratchValue25)
                resources:ReleaseResource(resource5)
                return
            end
            if scratchValue == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    quest:DeregisterTimer(scratchValue25)
                    resources:ReleaseResource(resource5)
                    return
                end
                quest:RemoveQuestInfoElement(scratchValue24)
                quest:RemoveQuestInfoElement(addQuestInfoTickByText3)
                quest:RemoveQuestInfoElement(addQuestInfoTickByText2)
            end
            resource3 = resources:NewResource()
            while not resources:TryAcquire(resource3, hero_, 4) do
                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5db37(); return end
            end
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db37(); return end
            actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "HERO", resource3)
            resources:SetActor(actorMap, "TEACHER", resource5)
            movie3 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_SKILL_MOVE", actorMap, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetStateInt("TutorialState", 4)
            quest:SetMasterGameState("MovingDummiesNeeded", true)
            quest:CameraDefault()
            quest:RemoveQuestInfoElement(scratchValue23)
            if not quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): goto LAB_00d5db1f
                        end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)xStack_98);
                        goto LAB_00d5c4eb
                    end
                end
                -- LAB_00d5db45: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00d5db1f: (native jump target)
                resources:DestroyMovie(movie3)
                resources:DestroyActorMap(actorMap)
                __cleanup_LAB_00d5db37(); return
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5db13: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): goto LAB_00d5db1f
            end
            quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE")
            while not quest:MsgIsGameInfoClickedPast() do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db45
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db13
            ::LAB_00d5c4eb::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource3)
            if not quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    addQuestInfoTickByText = quest:AddQuestInfoTickByText("GAME_ACTION_UNSHEATHE_RANGED_WEAPON", false, 1.0)
                    scratchValue24 = addQuestInfoTickByText
                    addQuestInfoTickByText3 = quest:AddQuestInfoTickByText("GAME_ACTION_TOGGLE_FIRST_PERSON_VIEW", false, 1.0)
                    addQuestInfoTickByText2 = quest:AddQuestInfoTickByText("GAME_ACTION_FIRE_RANGED_WEAPON", false, 1.0)
                    goto LAB_00d5c6b1
                end
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(scratchValue25)
                resources:ReleaseResource(resource5)
                return
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(scratchValue25)
                resources:ReleaseResource(resource5)
                return
            end
            addQuestInfoTickByText = quest:AddQuestInfoTickByText("HUD_BLACK_BUTTON", false, 1.0)
            scratchValue24 = addQuestInfoTickByText
            addQuestInfoTickByText3 = quest:AddQuestInfoTickByText("HUD_CONTROLLER_THUMBSTICK_LEFT_CLICK", false, 1.0)
            addQuestInfoTickByText2 = quest:AddQuestInfoTickByText("HUD_CONTROLLER_X", false, 1.0)
            ::LAB_00d5c6b1::
            scratchValue = 0
            quest:DisplayQuestInfo(true)
            repeat
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    quest:DeregisterTimer(scratchValue25)
                    resources:ReleaseResource(resource5)
                    return
                end
                if quest:GetTimer(timerId2) < 1 then
                    if quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") then
                        scratchValue9 = false
                        goto FLOW_after_lab_00d5c757
                    end
                    scratchValue12 = 6.0
                    if not quest:IsDistanceBetweenThingsUnder(hero_, archeryRing, 6.0) then
                        scratchValue9 = false
                        goto FLOW_after_lab_00d5c757
                    end
                    scratchValue9 = true
                else
                    scratchValue9 = false
                end
                ::FLOW_after_lab_00d5c757::
                if scratchValue9 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        quest:DeregisterTimer(scratchValue25)
                        resources:ReleaseResource(resource5)
                        return
                    end
                    scratchValue15 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue15, hero_)
                    quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_MAZE_BOW_UNSHEATH", me, hero_, false)
                    if quest:IsXbox() then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId2)
                            quest:DeregisterTimer(scratchValue25)
                            resources:ReleaseResource(resource5)
                            return
                        end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP")
                        while not quest:MsgIsGameInfoClickedPast() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId2)
                                quest:DeregisterTimer(scratchValue25)
                                resources:ReleaseResource(resource5)
                                return
                            end
                        end
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId2)
                            quest:DeregisterTimer(scratchValue25)
                            resources:ReleaseResource(resource5)
                            return
                        end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC")
                        while not quest:MsgIsGameInfoClickedPast() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId2)
                                quest:DeregisterTimer(scratchValue25)
                                resources:ReleaseResource(resource5)
                                return
                            end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        quest:DeregisterTimer(scratchValue25)
                        resources:ReleaseResource(resource5)
                        return
                    end
                    quest:SetTimer(timerId2, 15)
                    addQuestInfoTickByText = scratchValue24
                end
                if quest:MsgOnHeroFiredRangedWeapon() then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        quest:DeregisterTimer(scratchValue25)
                        resources:ReleaseResource(resource5)
                        return
                    end
                end
                scratchValue9 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                scratchValue8 = scratchValue9
                if scratchValue8 then
                    -- TODO(native): IsHeroInProjectileWeaponMode is not a ForgeFSE binding
                    quest:IsHeroInProjectileWeaponMode()
                    scratchValue8 = scratchValue9
                end
                scratchValue7 = scratchValue8
                if scratchValue7 then
                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingFireRangedWeaponButton()
                    scratchValue7 = scratchValue9
                end
                if scratchValue7 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        quest:DeregisterTimer(scratchValue25)
                        resources:ReleaseResource(resource5)
                        return
                    end
                    scratchValue = 1
                    quest:RemoveQuestInfoElement(addQuestInfoTickByText)
                    quest:RemoveQuestInfoElement(addQuestInfoTickByText3)
                    quest:RemoveQuestInfoElement(addQuestInfoTickByText2)
                elseif scratchValue == 0 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        quest:DeregisterTimer(scratchValue25)
                        resources:ReleaseResource(resource5)
                        return
                    end
                    scratchValue9 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                    quest:UpdateQuestInfoTick(addQuestInfoTickByText, scratchValue9)
                    -- TODO(native): IsHeroInProjectileWeaponMode is not a ForgeFSE binding
                    quest:IsHeroInProjectileWeaponMode()
                    quest:UpdateQuestInfoTick(addQuestInfoTickByText3, scratchValue9)
                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingFireRangedWeaponButton()
                    quest:UpdateQuestInfoTick(addQuestInfoTickByText2, scratchValue9)
                end
            until true
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(scratchValue25)
                resources:ReleaseResource(resource5)
                return
            end
            if scratchValue == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    quest:DeregisterTimer(scratchValue25)
                    resources:ReleaseResource(resource5)
                    return
                end
                quest:RemoveQuestInfoElement(addQuestInfoTickByText)
                quest:RemoveQuestInfoElement(addQuestInfoTickByText3)
                quest:RemoveQuestInfoElement(addQuestInfoTickByText2)
            end
            timerId = quest:RegisterTimer()
            -- TODO(native): piVar2 = DAT_0143e8f8;
            quest:SetTimer(timerId, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_SkillTimer))))
            quest:SetMasterGameState("SkillScore", 0)
            scratchValue = 0
            scratchValue14 = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
            addQuestInfoTickByText3 = scratchValue14
            scratchValue22 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
            scratchValue24 = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
            quest:DisplayQuestInfo(true)
            quest:UpdateQuestInfoCounter(scratchValue14, quest:GetMasterGameState("HighestSkillScore"), -1)
            quest:SetMasterGameState("SkillTestOccuring", true)
            quest:SetTimer(timerId2, 15)
            scratchValue14 = quest:GetTimer(timerId)
            while 0 < scratchValue14 and scratchValue == 0 do
                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5db90(); return end
                if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                    scratchValue = 1
                end
                scratchValue14 = scratchValue22
                quest:UpdateQuestInfoCounter(scratchValue22, quest:GetMasterGameState("SkillScore"), -1)
                if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
                    quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                    quest:UpdateQuestInfoCounter(scratchValue14, quest:GetMasterGameState("HighestSkillScore"), -1)
                end
                if quest:GetTimer(timerId2) < 1 then
                    if quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") then
                        scratchValue9 = false
                        goto FLOW_after_lab_00d5cd47
                    end
                    scratchValue9 = true
                else
                    scratchValue9 = false
                end
                ::FLOW_after_lab_00d5cd47::
                if scratchValue9 then
                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
                    scratchValue15 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue15, hero_)
                    quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_MAZE_BOW_UNSHEATH", me, hero_, false)
                    if quest:IsXbox() then
                        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5db90(); return end
                        end
                    else
                        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5db90(); return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
                    quest:SetTimer(timerId2, 15)
                end
                if not playerNotWarned then
                    scratchValue9 = false
                else
                    -- TODO(native): CVar10 = *(this + 4)
                    addQuestInfoTickByText = nil --[[unresolved native value]]
                    scratchValue12 = 6.0
                    -- TODO(native): pCVar7 = (**(*CVar10 + 0x120))(CVar10,"ArcheryRing","ArcheryRing")
                    scratchValue19 = nil --[[unresolved native value]]
                    -- TODO(native): pCVar6 = (**(*CVar10 + 0x118))(CVar10)
                    hero = nil --[[unresolved native value]]
                    scratchValue9 = quest:IsDistanceBetweenThingsOver(hero, scratchValue19, 6.0)
                end
                if scratchValue9 then
                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
                    playerNotWarned = false
                    scratchValue15 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue15, hero_)
                    quest:AddLineToConversation(scratchValue15, "TEXT_QST_028_MAZE_RING_OUT", me, hero_, false)
                end
                scratchValue14 = quest:GetTimer(timerId)
            end
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            quest:SetMasterGameState("SkillTestOccuring", false)
            while not quest:IsHeroControlledByPlayer() do
                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5db90(); return end
            end
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(addQuestInfoTickByText3)
            quest:RemoveQuestInfoElement(scratchValue22)
            quest:RemoveQuestInfoElement(scratchValue24)
            quest:EntitySetTargetable(me, true)
            resource4 = resources:NewResource()
            while not resources:TryAcquire(resource4, hero_, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db84
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5db84: (native jump target)
                resources:ReleaseResource(resource4)
                __cleanup_LAB_00d5db90(); return
            end
            actorMap3 = resources:NewActorMap()
            resources:SetActor(actorMap3, "HERO", resource4)
            resources:SetActor(actorMap3, "TEACHER", resource5)
            movie2 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            if xStack_21c_b3 == 0 then
                if not quest:IsActiveThreadTerminating() then
                    scratchValue23 = quest:GetMasterGameState("SkillScore")
                    scratchValue18 = 0
                    scratchValue14 = 0
                    repeat
                        scratchValue15 = scratchValue14
                        if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_SkillGrades, scratchValue18) < scratchValue23 ~= (quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_SkillGrades, scratchValue18) == scratchValue23) then
                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db53
                            break
                        end
                        scratchValue14 = scratchValue15 + 1
                        scratchValue18 = scratchValue18 + 1
                    until scratchValue14 >= 7
                    scratchValue19 = resources:NewStringMap()
                    switch = scratchValue15
                    repeat
                        if switch == 0 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_114);
                            break
                        elseif switch == 1 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_10c);
                            break
                        elseif switch == 2 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_104);
                            break
                        elseif switch == 3 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_fc);
                            break
                        elseif switch == 4 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_f4);
                            break
                        elseif switch == 5 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_ec);
                            break
                        elseif switch == 6 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_e4);
                            break
                        else
                            goto FLOW_native_label_1
                        end
                    until true
                    ::FLOW_native_label_1::
                    resources:RunMacroWithStrings("CS_GUILD_SKILL_WON_START", actorMap3, scratchValue19, false, false)
                    resources:DestroyStringMap(scratchValue19)
                    goto LAB_00d5d526
                end
                -- TODO(native): goto LAB_00d5db62
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5db53: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00d5db6f: (native jump target)
                resources:DestroyMovie(movie2)
                resources:DestroyActorMap(actorMap3)
                -- TODO(native): goto LAB_00d5db84
            end
            resources:RunMacro("CS_GUILD_SKILL_DISQUALIFIED", actorMap3, false, true)
            ::LAB_00d5d526::
            quest:SetStateInt("TutorialState", 0)
            quest:RemoveQuestInfoElement(scratchValue12)
            quest:Pause(2.0)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_SKILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
            scratchValue14 = quest:MsgIsQuestionAnsweredYesOrNo()
            while scratchValue14 < 0 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db62
                scratchValue14 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db53
            if scratchValue14 == 1 then
                resources:RunMacro("CS_GUILD_SKILL_CONTINUE", actorMap3, false, true)
                quest:SetMasterGameState("SkillRepeating", false)
                goto LAB_00d5d7a9
                -- LAB_00d5db62: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): goto LAB_00d5db6f
            end
            if false then return end  -- TODO(native): goto LAB_00d5db53
            resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap3, false, false)
            quest:SetMasterGameState("MovingDummiesNeeded", false)
            quest:SetMasterGameState("SkillRepeating", true)
            quest:SetMasterGameState("SkillRepeatKnown", true)
            if quest:IsObjectInThingsPossession("OBJECT_YEW_LONGBOW", hero_) then
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db62
                quest:TakeObjectFromHero("OBJECT_YEW_LONGBOW")
            end
            while quest:GetMasterGameState("SkillDummyReset") ~= 1 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db53
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db62
            ::LAB_00d5d7a9::
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            resources:DestroyActorMap(actorMap3)
            resources:ReleaseResource(resource4)
            quest:DeregisterTimer(timerId)
            quest:DeregisterTimer(timerId2)
        end
        if not quest:IsActiveThreadTerminating() then
            me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 0x40400000, 0, false, true)
            guildEvilApprenticeMale = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("SkillApprenticeMarker"):GetPos(), "SkillApprentice")
            if guildEvilApprenticeMale ~= nil then
                guildEvilApprenticeMale:SetToKillOnLevelUnload(0)
            end
            quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("BirdKillerMarker"):GetPos(), "BirdKiller")
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_05", "", "")
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            quest:SetPlayerUsingRangedDummies(false)
        end
        ::LAB_00d5da96::
        quest:DeregisterTimer(scratchValue25)
    end
    resources:ReleaseResource(resource5)
end

-- TheRealGuildmaster.Init (retail 0x00d5ac90)
function Init(quest, me)
    playerNotWarned = true
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

