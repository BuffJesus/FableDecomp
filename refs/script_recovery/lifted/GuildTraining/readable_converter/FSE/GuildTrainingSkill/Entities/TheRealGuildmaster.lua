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

-- TheRealGuildmaster.Main (retail 0x00d5ae70)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue, scratchValue4, scratchValue5, scratchValue8, scratchValue9, scratchValue10
    local scratchValue11, scratchValue12, scratchValue13, scratchValue14, scratchValue15
    local scratchValue16, scratchValue19, switch2, scratchValue21, scratchValue22, scratchValue25
    local scratchValue27, scratchValue28, scratchValue29, scratchValue30, scratchValue31
    local scratchValue32, scratchValue33, scratchValue34, scratchValue35, scratchValue36
    local scratchValue37, scratchValue38, scratchValue39, timerId, timerId2, scratchValue40
    local function __cleanup_LAB_00d5dac4()
        resources:ReleaseResource(scratchValue30)
        resources:DestroyActorMap(scratchValue29)
        resources:DestroyMovie(scratchValue40)
        quest:DeregisterTimer(timerId2)
        resources:ReleaseResource(scratchValue31)
    end
    local function __cleanup_LAB_00d5db05()
        quest:PauseAllNonScriptedEntities(false)
        resources:ReleaseResource(scratchValue30)
        resources:DestroyActorMap(scratchValue29)
        resources:DestroyMovie(scratchValue40)
        quest:DeregisterTimer(timerId2)
        resources:ReleaseResource(scratchValue31)
    end
    local function __cleanup_LAB_00d5db37()
        resources:ReleaseResource(scratchValue39)
        quest:DeregisterTimer(xStack_220)
        quest:DeregisterTimer(xStack_214)
        resources:DestroyMovie(scratchValue34)
    end
    local function __cleanup_LAB_00d5db90()
        quest:DeregisterTimer(timerId)
        quest:DeregisterTimer(xStack_220)
        quest:DeregisterTimer(xStack_214)
        resources:DestroyMovie(scratchValue34)
    end
    scratchValue27 = 0
    scratchValue39 = resources:NewResource()
    scratchValue10 = resources:TryAcquire(scratchValue39, me, 4)
    while not scratchValue10 do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(scratchValue39); return end
        scratchValue10 = resources:TryAcquire(scratchValue39, me, 4)
    end
    if not quest:IsActiveThreadTerminating() then
        quest:EntitySetAsKillable(me, false, true)
        quest:EntitySetAlwaysBlockAttacksFromThing(me, quest:GetHero(), true)
        quest:SetIsPushableByHero(me, false)
        quest:SetThingHasInformation(me, false, false, false)
        me:MoveToPosition(quest:GetThingWithScriptName("M_SkillTeacherStand"):GetPos(), 0x40400000, 0, false, true)
        quest:SetPlayerUsingRangedDummies(true)
        scratchValue16 = quest:RegisterTimer()
        quest:SetTimer(scratchValue16, 0)
        scratchValue15 = quest:GetStateInt("TutorialState")
        while scratchValue15 == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d5da96 end
            if me:IsTalkedToByHero() then
                if quest:GetMasterGameState("HeroTakingGuildTest") == 0 then
                    me:ClearCommands()
                    quest:SetStateInt("TutorialState", 3)
                else
                    scratchValue35 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    me:ClearCommands()
                    quest:GetHealth(resources:ScriptThing(scratchValue39))
                    scratchValue14 = 0.0
                    if 0.0 < fret_0 then
                        scratchValue16 = 0
                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_SKILL_NOT_START", 0, false, true, false)
                        scratchValue11 = me:IsPerformingScriptTask()
                        while scratchValue11 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue35)
                                quest:DeregisterTimer(0)
                                resources:ReleaseResource(scratchValue39)
                                return
                            end
                            scratchValue11 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue35)
                            goto LAB_00d5da96
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue35)
                end
            end
            scratchValue13 = 5.5
            if (quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(xStack_220) < 1) and not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d5da96 end
                scratchValue16 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue16, quest:GetHero())
                quest:SetTimer(xStack_220, 10)
                if true then
                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                    quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_FIRST", me, quest:GetHero(), false)
                elseif 0 == 1 then
                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                    quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_SECOND", me, quest:GetHero(), false)
                end
                -- TODO(native): xStack_1ec = 1 - xStack_1ec;
            end
            scratchValue15 = quest:GetStateInt("TutorialState")
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(xStack_214)
            resources:DestroyMovie(scratchValue34)
            return
        end
        quest:SetMasterGameState("SkillTrainingStarted", true)
        quest:SetMasterGameState("SkillRepeating", true)
        quest:SetMasterGameState("HeroTakingGuildTest", true)
        scratchValue11 = quest:GetMasterGameState("SkillRepeating")
        while scratchValue11 ~= 0 do
            if not quest:NewScriptFrame(me) then goto LAB_00d5da96 end
            quest:SetStateInt("TutorialState", 3)
            quest:SetMasterGameState("SkillRepeatKnown", false)
            scratchValue30 = resources:NewResource()
            scratchValue10 = resources:TryAcquire(scratchValue30, quest:GetHero(), 4)
            while not scratchValue10 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:DestroyMovie(scratchValue40)
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(scratchValue31)
                    return
                end
                scratchValue10 = resources:TryAcquire(scratchValue30, quest:GetHero(), 4)
            end
            if quest:IsActiveThreadTerminating() then
                resources:DestroyMovie(scratchValue40)
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(scratchValue31)
                return
            end
            scratchValue29 = resources:NewActorMap()
            resources:SetActor(scratchValue29, "HERO", scratchValue30)
            resources:SetActor(scratchValue29, "TEACHER", scratchValue34)
            quest:SetStateInt("TutorialState", 2)
            scratchValue34 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_SKILL_START", scratchValue29, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetMasterGameState("SkillTrainingStarted", true)
            if not quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_PC")
                    scratchValue10 = quest:MsgIsGameInfoClickedPast()
                    while not scratchValue10 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            __cleanup_LAB_00d5dac4(); return
                        end
                        scratchValue10 = quest:MsgIsGameInfoClickedPast()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)xStack_9c);
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
            scratchValue10 = quest:MsgIsGameInfoClickedPast()
            while not scratchValue10 do
                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5db05(); return end
                scratchValue10 = quest:MsgIsGameInfoClickedPast()
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5dab8
            ::LAB_00d5b7c0::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue35)
            resources:DestroyActorMap(scratchValue28)
            resources:ReleaseResource(scratchValue32)
            scratchValue37 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 3, 1.0)
            timerId2 = quest:RegisterTimer()
            scratchValue15 = timerId2
            quest:SetTimer(timerId2, 15)
            quest:SetMasterGameState("SkillScore", 0)
            scratchValue36 = quest:GetMasterGameState("SkillScore")
            quest:EntitySetTargetable(me, false)
            if not quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick("GAME_ACTION_UNSHEATHE_RANGED_WEAPON", false, 1.0)
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick("GAME_ACTION_LOCK_TARGET", false, 1.0)
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick("GAME_ACTION_FIRE_RANGED_WEAPON", false, 1.0)
                    goto LAB_00d5ba3e
                end
                quest:DeregisterTimer(xStack_220)
                quest:DeregisterTimer(xStack_214)
                resources:DestroyMovie(scratchValue34)
                return
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(xStack_220)
                quest:DeregisterTimer(xStack_214)
                resources:DestroyMovie(scratchValue34)
                return
            end
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick("HUD_BLACK_BUTTON", false, 1.0)
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick("HUD_CONTROLLER_TRIGGER_LEFT", false, 1.0)
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick("HUD_CONTROLLER_X", false, 1.0)
            ::LAB_00d5ba3e::
            scratchValue12 = 0
            quest:DisplayQuestInfo(true)
            quest:SetTimer(scratchValue15, 5)
            scratchValue16 = 0
            scratchValue15 = quest:GetMasterGameState("SkillScore")
            while scratchValue15 < 3 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(xStack_220)
                    quest:DeregisterTimer(xStack_214)
                    resources:DestroyMovie(scratchValue34)
                    return
                end
                scratchValue27 = scratchValue27 | 1
                scratchValue10 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                scratchValue5 = scratchValue10
                if scratchValue5 then
                    -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingLockTargetButton()
                    scratchValue5 = scratchValue10
                end
                scratchValue4 = scratchValue5
                if scratchValue4 then
                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingFireRangedWeaponButton()
                    scratchValue4 = scratchValue10
                end
                scratchValue10 = scratchValue4
                if scratchValue27 & 1 ~= 0 then
                    scratchValue27 = scratchValue27 & 0xfffffffe
                end
                if scratchValue10 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(scratchValue34)
                        return
                    end
                    scratchValue12 = '\x01'
                    quest:RemoveQuestInfoElement(scratchValue16)
                    quest:RemoveQuestInfoElement(xStack_1ec)
                    quest:RemoveQuestInfoElement(xStack_1e8)
                elseif scratchValue12 == 0 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(scratchValue34)
                        return
                    end
                    scratchValue10 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                    quest:UpdateQuestInfoTick(scratchValue10, scratchValue13 ~= 0)
                    -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingLockTargetButton()
                    quest:UpdateQuestInfoTick(xStack_1ec, scratchValue10)
                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingFireRangedWeaponButton()
                    quest:UpdateQuestInfoTick(xStack_1e8, scratchValue10)
                    if quest:GetTimer(timerId2) < 1 then
                        scratchValue13 = 6.0
                        if quest:IsDistanceBetweenThingsUnder(quest:GetHero(), xStack_38, 6.0) and not quest:IsConversationActive(scratchValue16) then
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(xStack_220)
                                quest:DeregisterTimer(xStack_214)
                                resources:DestroyMovie(scratchValue34)
                                return
                            end
                            scratchValue10 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                            if scratchValue10 then
                                -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                                quest:IsPlayerHoldingLockTargetButton()
                                if scratchValue10 then
                                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                                    quest:IsPlayerHoldingFireRangedWeaponButton()
                                    if scratchValue10 then goto LAB_00d5bf6c end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(xStack_220)
                                        quest:DeregisterTimer(xStack_214)
                                        resources:DestroyMovie(scratchValue34)
                                        return
                                    end
                                    scratchValue16 = quest:AddNewConversation(me, false, false)
                                    quest:AddPersonToConversation(scratchValue16, quest:GetHero())
                                    if quest:IsXbox() then
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(xStack_220)
                                            quest:DeregisterTimer(xStack_214)
                                            resources:DestroyMovie(scratchValue34)
                                            return
                                        end
                                        quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_30", me, quest:GetHero(), false)
                                    else
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(xStack_220)
                                            quest:DeregisterTimer(xStack_214)
                                            resources:DestroyMovie(scratchValue34)
                                            return
                                        end
                                        quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_30", me, quest:GetHero(), false)
                                    end
                                else
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(xStack_220)
                                        quest:DeregisterTimer(xStack_214)
                                        resources:DestroyMovie(scratchValue34)
                                        return
                                    end
                                    scratchValue16 = quest:AddNewConversation(me, false, false)
                                    quest:AddPersonToConversation(scratchValue16, quest:GetHero())
                                    if quest:IsXbox() then
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(xStack_220)
                                            quest:DeregisterTimer(xStack_214)
                                            resources:DestroyMovie(scratchValue34)
                                            return
                                        end
                                        quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_20", me, quest:GetHero(), false)
                                    else
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(xStack_220)
                                            quest:DeregisterTimer(xStack_214)
                                            resources:DestroyMovie(scratchValue34)
                                            return
                                        end
                                        quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_20", me, quest:GetHero(), false)
                                    end
                                end
                            else
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(xStack_220)
                                    quest:DeregisterTimer(xStack_214)
                                    resources:DestroyMovie(scratchValue34)
                                    return
                                end
                                scratchValue16 = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(scratchValue16, quest:GetHero())
                                if quest:IsXbox() then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(xStack_220)
                                        quest:DeregisterTimer(xStack_214)
                                        resources:DestroyMovie(scratchValue34)
                                        return
                                    end
                                    quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_10", me, quest:GetHero(), false)
                                else
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(xStack_220)
                                        quest:DeregisterTimer(xStack_214)
                                        resources:DestroyMovie(scratchValue34)
                                        return
                                    end
                                    quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_10", me, quest:GetHero(), false)
                                end
                            end
                            quest:SetTimer(timerId2, 10)
                        end
                    end
                end
                ::LAB_00d5bf6c::
                scratchValue15 = quest:GetMasterGameState("SkillScore")
                if scratchValue36 < scratchValue15 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(scratchValue34)
                        return
                    end
                    scratchValue36 = quest:GetMasterGameState("SkillScore")
                elseif scratchValue15 < scratchValue36 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(scratchValue34)
                        return
                    end
                    -- TODO(native): *piVar2 = *piVar2 + 1;
                    if not quest:IsConversationActive(scratchValue16) then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(xStack_220)
                            quest:DeregisterTimer(xStack_214)
                            resources:DestroyMovie(scratchValue34)
                            return
                        end
                        scratchValue16 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue16, quest:GetHero())
                        quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", me, quest:GetHero(), false)
                    end
                end
                quest:UpdateQuestInfoCounter(scratchValue37, quest:GetMasterGameState("SkillScore"), -1)
                if state:GetBool("PlayerNotWarned") then
                    scratchValue13 = 6.0
                    if quest:IsDistanceBetweenThingsOver(quest:GetHero(), xStack_38, 6.0) and not quest:IsConversationActive(scratchValue16) then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(xStack_220)
                            quest:DeregisterTimer(xStack_214)
                            resources:DestroyMovie(scratchValue34)
                            return
                        end
                        state:SetBool("PlayerNotWarned", false)
                        scratchValue16 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue16, quest:GetHero())
                        quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_MAZE_RING_OUT", me, quest:GetHero(), false)
                    end
                end
                scratchValue15 = quest:GetMasterGameState("SkillScore")
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(xStack_220)
                quest:DeregisterTimer(xStack_214)
                resources:DestroyMovie(scratchValue34)
                return
            end
            if scratchValue12 == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(xStack_220)
                    quest:DeregisterTimer(xStack_214)
                    resources:DestroyMovie(scratchValue34)
                    return
                end
                quest:RemoveQuestInfoElement(scratchValue13)
                quest:RemoveQuestInfoElement(xStack_1ec)
                quest:RemoveQuestInfoElement(xStack_1e8)
            end
            scratchValue31 = resources:NewResource()
            scratchValue10 = resources:TryAcquire(scratchValue31, quest:GetHero(), 4)
            while not scratchValue10 do
                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5db37(); return end
                scratchValue10 = resources:TryAcquire(scratchValue31, quest:GetHero(), 4)
            end
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db37(); return end
            scratchValue28 = resources:NewActorMap()
            resources:SetActor(scratchValue28, "HERO", scratchValue39)
            resources:SetActor(scratchValue28, "TEACHER", scratchValue31)
            scratchValue40 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_SKILL_MOVE", scratchValue28, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetStateInt("TutorialState", 4)
            quest:SetMasterGameState("MovingDummiesNeeded", true)
            quest:CameraDefault()
            quest:RemoveQuestInfoElement(scratchValue37)
            if not quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE_PC")
                    scratchValue10 = quest:MsgIsGameInfoClickedPast()
                    while not scratchValue10 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): goto LAB_00d5db1f
                        end
                        scratchValue10 = quest:MsgIsGameInfoClickedPast()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)xStack_64);
                        goto LAB_00d5c4eb
                    end
                end
                -- LAB_00d5db45: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00d5db1f: (native jump target)
                resources:DestroyMovie(scratchValue34)
                resources:DestroyActorMap(scratchValue33)
                __cleanup_LAB_00d5db37(); return
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5db13: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): goto LAB_00d5db1f
            end
            quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE")
            scratchValue10 = quest:MsgIsGameInfoClickedPast()
            while not scratchValue10 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db45
                scratchValue10 = quest:MsgIsGameInfoClickedPast()
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db13
            ::LAB_00d5c4eb::
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(scratchValue30)
            resources:DestroyActorMap(scratchValue29)
            resources:ReleaseResource(scratchValue39)
            if not quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick("GAME_ACTION_UNSHEATHE_RANGED_WEAPON", false, 1.0)
                    -- TODO(native): xStack_1e4 = CVar10;
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick("GAME_ACTION_TOGGLE_FIRST_PERSON_VIEW", false, 1.0)
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick("GAME_ACTION_FIRE_RANGED_WEAPON", false, 1.0)
                    goto LAB_00d5c6b1
                end
                quest:DeregisterTimer(xStack_220)
                quest:DeregisterTimer(xStack_214)
                resources:DestroyMovie(scratchValue34)
                return
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(xStack_220)
                quest:DeregisterTimer(xStack_214)
                resources:DestroyMovie(scratchValue34)
                return
            end
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick("HUD_BLACK_BUTTON", false, 1.0)
            -- TODO(native): xStack_1e4 = CVar10;
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick("HUD_CONTROLLER_THUMBSTICK_LEFT_CLICK", false, 1.0)
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick("HUD_CONTROLLER_X", false, 1.0)
            ::LAB_00d5c6b1::
            scratchValue12 = 0
            quest:DisplayQuestInfo(true)
            repeat
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(xStack_220)
                    quest:DeregisterTimer(xStack_214)
                    resources:DestroyMovie(scratchValue34)
                    return
                end
                if quest:GetTimer(timerId2) < 1 then
                    scratchValue27 = scratchValue27 | 2
                    if quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") then
                        scratchValue10 = false
                        goto FLOW_after_lab_00d5c757
                    end
                    scratchValue13 = 6.0
                    if not quest:IsDistanceBetweenThingsUnder(quest:GetHero(), xStack_38, 6.0) then
                        scratchValue10 = false
                        goto FLOW_after_lab_00d5c757
                    end
                    scratchValue10 = true
                else
                    scratchValue10 = false
                end
                ::FLOW_after_lab_00d5c757::
                if scratchValue27 & 2 ~= 0 then
                    scratchValue27 = scratchValue27 & 0xfffffffd
                end
                if scratchValue10 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(scratchValue34)
                        return
                    end
                    scratchValue16 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue16, quest:GetHero())
                    quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_MAZE_BOW_UNSHEATH", me, quest:GetHero(), false)
                    if quest:IsXbox() then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(xStack_220)
                            quest:DeregisterTimer(xStack_214)
                            resources:DestroyMovie(scratchValue34)
                            return
                        end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP")
                        scratchValue10 = quest:MsgIsGameInfoClickedPast()
                        while not scratchValue10 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(xStack_220)
                                quest:DeregisterTimer(xStack_214)
                                resources:DestroyMovie(scratchValue34)
                                return
                            end
                            scratchValue10 = quest:MsgIsGameInfoClickedPast()
                        end
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(xStack_220)
                            quest:DeregisterTimer(xStack_214)
                            resources:DestroyMovie(scratchValue34)
                            return
                        end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC")
                        scratchValue10 = quest:MsgIsGameInfoClickedPast()
                        while not scratchValue10 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(xStack_220)
                                quest:DeregisterTimer(xStack_214)
                                resources:DestroyMovie(scratchValue34)
                                return
                            end
                            scratchValue10 = quest:MsgIsGameInfoClickedPast()
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(scratchValue34)
                        return
                    end
                    quest:SetTimer(timerId2, 15)
                end
                if quest:MsgOnHeroFiredRangedWeapon() then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(scratchValue34)
                        return
                    end
                end
                scratchValue27 = scratchValue27 | 4
                scratchValue10 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                scratchValue9 = scratchValue10
                if scratchValue9 then
                    -- TODO(native): IsHeroInProjectileWeaponMode is not a ForgeFSE binding
                    quest:IsHeroInProjectileWeaponMode()
                    scratchValue9 = scratchValue10
                end
                scratchValue8 = scratchValue9
                if scratchValue8 then
                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingFireRangedWeaponButton()
                    scratchValue8 = scratchValue10
                end
                scratchValue10 = scratchValue8
                if scratchValue27 & 4 ~= 0 then
                    scratchValue27 = scratchValue27 & 0xfffffffb
                end
                if scratchValue10 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(scratchValue34)
                        return
                    end
                    scratchValue12 = '\x01'
                    quest:RemoveQuestInfoElement(1)
                    quest:RemoveQuestInfoElement(xStack_1ec)
                    quest:RemoveQuestInfoElement(xStack_1e8)
                elseif scratchValue12 == 0 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(scratchValue34)
                        return
                    end
                    scratchValue10 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                    quest:UpdateQuestInfoTick(scratchValue10, scratchValue13 ~= 0)
                    -- TODO(native): IsHeroInProjectileWeaponMode is not a ForgeFSE binding
                    quest:IsHeroInProjectileWeaponMode()
                    quest:UpdateQuestInfoTick(xStack_1ec, scratchValue10)
                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingFireRangedWeaponButton()
                    quest:UpdateQuestInfoTick(xStack_1e8, scratchValue10)
                end
            until xStack_1d4_b3 ~= 0
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(xStack_220)
                quest:DeregisterTimer(xStack_214)
                resources:DestroyMovie(scratchValue34)
                return
            end
            if scratchValue12 == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(xStack_220)
                    quest:DeregisterTimer(xStack_214)
                    resources:DestroyMovie(scratchValue34)
                    return
                end
                quest:RemoveQuestInfoElement(scratchValue14)
                quest:RemoveQuestInfoElement(xStack_1ec)
                quest:RemoveQuestInfoElement(xStack_1e8)
            end
            timerId = quest:RegisterTimer()
            -- TODO(native): piVar2 = DAT_0143e8f8;
            quest:SetTimer(timerId, math.modf(quest:ReadGlobalGameDataFloat(3844)))
            quest:SetMasterGameState("SkillScore", 0)
            scratchValue12 = 0
            scratchValue15 = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
            scratchValue36 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
            scratchValue38 = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
            quest:DisplayQuestInfo(true)
            quest:UpdateQuestInfoCounter(scratchValue15, quest:GetMasterGameState("HighestSkillScore"), -1)
            quest:SetMasterGameState("SkillTestOccuring", true)
            quest:SetTimer(timerId2, 15)
            scratchValue15 = quest:GetTimer(timerId)
            while 0 < scratchValue15 and scratchValue12 == 0 do
                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5db90(); return end
                if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                    scratchValue12 = '\x01'
                end
                scratchValue15 = scratchValue36
                quest:UpdateQuestInfoCounter(scratchValue36, quest:GetMasterGameState("SkillScore"), -1)
                if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
                    quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                    quest:UpdateQuestInfoCounter(scratchValue15, quest:GetMasterGameState("HighestSkillScore"), -1)
                end
                if quest:GetTimer(timerId2) < 1 then
                    scratchValue27 = scratchValue27 | 8
                    if quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW") then
                        scratchValue10 = false
                        goto FLOW_after_lab_00d5cd47
                    end
                    scratchValue10 = true
                else
                    scratchValue10 = false
                end
                ::FLOW_after_lab_00d5cd47::
                if scratchValue27 & 8 ~= 0 then
                    scratchValue27 = scratchValue27 & 0xfffffff7
                end
                if scratchValue10 then
                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
                    scratchValue16 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue16, quest:GetHero())
                    quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_MAZE_BOW_UNSHEATH", me, quest:GetHero(), false)
                    if quest:IsXbox() then
                        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP")
                        scratchValue10 = quest:MsgIsGameInfoClickedPast()
                        while not scratchValue10 do
                            if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5db90(); return end
                            scratchValue10 = quest:MsgIsGameInfoClickedPast()
                        end
                    else
                        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC")
                        scratchValue10 = quest:MsgIsGameInfoClickedPast()
                        while not scratchValue10 do
                            if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5db90(); return end
                            scratchValue10 = quest:MsgIsGameInfoClickedPast()
                        end
                    end
                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
                    quest:SetTimer(timerId2, 15)
                end
                if not state:GetBool("PlayerNotWarned") then
                    scratchValue10 = false
                else
                    scratchValue27 = scratchValue27 | 48
                    -- TODO(native): CVar10 = *(this + 4)
                    scratchValue = nil --[[unresolved native value]]
                    scratchValue13 = 6.0
                    -- TODO(native): pCVar7 = (**(*CVar10 + 0x120))(CVar10,"ArcheryRing","ArcheryRing")
                    scratchValue22 = nil --[[unresolved native value]]
                    -- TODO(native): pCVar6 = (**(*CVar10 + 0x118))(CVar10)
                    scratchValue21 = nil --[[unresolved native value]]
                    scratchValue10 = quest:IsDistanceBetweenThingsOver(scratchValue21, scratchValue22, 6.0)
                end
                if scratchValue27 & 32 ~= 0 then
                    scratchValue27 = scratchValue27 & 0xffffffdf
                end
                if scratchValue27 & 16 ~= 0 then
                    scratchValue27 = scratchValue27 & 0xffffffef
                end
                if scratchValue10 then
                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
                    state:SetBool("PlayerNotWarned", false)
                    scratchValue16 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue16, quest:GetHero())
                    quest:AddLineToConversation(scratchValue16, "TEXT_QST_028_MAZE_RING_OUT", me, quest:GetHero(), false)
                end
                scratchValue15 = quest:GetTimer(timerId)
            end
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5db90(); return end
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            quest:SetMasterGameState("SkillTestOccuring", false)
            scratchValue10 = quest:IsHeroControlledByPlayer()
            while not scratchValue10 do
                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5db90(); return end
                scratchValue10 = quest:IsHeroControlledByPlayer()
            end
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(scratchValue15)
            quest:RemoveQuestInfoElement(scratchValue36)
            quest:RemoveQuestInfoElement(scratchValue38)
            quest:EntitySetTargetable(me, true)
            scratchValue32 = resources:NewResource()
            scratchValue10 = resources:TryAcquire(scratchValue32, quest:GetHero(), 4)
            while not scratchValue10 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db84
                scratchValue10 = resources:TryAcquire(scratchValue32, quest:GetHero(), 4)
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5db84: (native jump target)
                resources:DestroyMovie(scratchValue40)
                __cleanup_LAB_00d5db90(); return
            end
            scratchValue33 = resources:NewActorMap()
            resources:SetActor(scratchValue33, "HERO", scratchValue40)
            resources:SetActor(scratchValue33, "TEACHER", scratchValue39)
            scratchValue35 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            if xStack_21c_b3 == 0 then
                if not quest:IsActiveThreadTerminating() then
                    scratchValue37 = quest:GetMasterGameState("SkillScore")
                    scratchValue19 = 0
                    scratchValue15 = 0
                    repeat
                        scratchValue16 = scratchValue15
                        if quest:ReadGlobalGameDataFloatAt(3776, scratchValue19) < scratchValue37 ~= (quest:ReadGlobalGameDataFloatAt(3776, scratchValue19) == scratchValue37) then
                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db53
                            break
                        end
                        scratchValue15 = scratchValue16 + 1
                        scratchValue19 = scratchValue19 + 1
                    until scratchValue15 >= 7
                    scratchValue22 = resources:NewStringMap()
                    switch2 = scratchValue16
                    repeat
                        if switch2 == 0 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_11c);
                            break
                        elseif switch2 == 1 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_114);
                            break
                        elseif switch2 == 2 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_10c);
                            break
                        elseif switch2 == 3 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_104);
                            break
                        elseif switch2 == 4 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_fc);
                            break
                        elseif switch2 == 5 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_f4);
                            break
                        elseif switch2 == 6 then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_ec);
                            break
                        else
                            goto FLOW_native_label_1
                        end
                    until true
                    ::FLOW_native_label_1::
                    resources:RunMacroWithStrings("$GRADE", scratchValue33, scratchValue22, false, false)
                    resources:DestroyStringMap(scratchValue22)
                    goto LAB_00d5d526
                end
                -- TODO(native): goto LAB_00d5db62
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5db53: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00d5db6f: (native jump target)
                resources:ReleaseResource(scratchValue31)
                resources:DestroyActorMap(scratchValue28)
                -- TODO(native): goto LAB_00d5db84
            end
            resources:RunMacro("CS_GUILD_SKILL_DISQUALIFIED", scratchValue33, false, true)
            ::LAB_00d5d526::
            quest:SetStateInt("TutorialState", 0)
            quest:RemoveQuestInfoElement(scratchValue13)
            quest:Pause(2.0)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_SKILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
            scratchValue15 = quest:MsgIsQuestionAnsweredYesOrNo()
            while scratchValue15 < 0 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db62
                scratchValue15 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db53
            if scratchValue15 == 1 then
                resources:RunMacro("CS_GUILD_SKILL_CONTINUE", scratchValue33, false, true)
                quest:SetMasterGameState("SkillRepeating", false)
                goto LAB_00d5d7a9
                -- LAB_00d5db62: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): goto LAB_00d5db6f
            end
            if false then return end  -- TODO(native): goto LAB_00d5db53
            resources:RunMacro("CS_GUILD_MELEE_REPEAT", scratchValue33, false, false)
            quest:SetMasterGameState("MovingDummiesNeeded", false)
            quest:SetMasterGameState("SkillRepeating", true)
            quest:SetMasterGameState("SkillRepeatKnown", true)
            if quest:IsObjectInThingsPossession("OBJECT_YEW_LONGBOW", quest:GetHero()) then
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db62
                quest:TakeObjectFromHero("OBJECT_YEW_LONGBOW")
            end
            scratchValue11 = quest:GetMasterGameState("SkillDummyReset")
            while scratchValue11 ~= '\x01' do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db53
                scratchValue11 = quest:GetMasterGameState("SkillDummyReset")
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5db62
            ::LAB_00d5d7a9::
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue35)
            resources:DestroyActorMap(scratchValue33)
            resources:ReleaseResource(scratchValue32)
            quest:DeregisterTimer(timerId2)
            quest:DeregisterTimer(xStack_220)
            scratchValue16 = timerId2
            scratchValue11 = quest:GetMasterGameState("SkillRepeating")
        end
        if not quest:IsActiveThreadTerminating() then
            me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 0x40400000, 0, false, true)
            scratchValue25 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("SkillApprenticeMarker"):GetPos(), "SkillApprentice")
            if scratchValue25 ~= nil and not scratchValue25:IsNull() then
                scratchValue25:SetToKillOnLevelUnload(0)
            end
            quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("BirdKillerMarker"):GetPos(), "BirdKiller")
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_05", "", "")
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            quest:SetPlayerUsingRangedDummies(false)
        end
        ::LAB_00d5da96::
        quest:DeregisterTimer(scratchValue16)
    end
    resources:ReleaseResource(scratchValue39)
end

-- TheRealGuildmaster.Init (retail 0x00d5ac90)
function Init(quest, me)
    state:SetBool("PlayerNotWarned", true)
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

