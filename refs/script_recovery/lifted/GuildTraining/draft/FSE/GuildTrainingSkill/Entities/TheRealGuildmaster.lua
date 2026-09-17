-- Generated native draft: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local CVar10, __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, __native_condition_5, __native_condition_6, __native_condition_7, __native_condition_8, bVar4, cVar5, c_stk_215, delay, fVar20, fVar3, iVar15, iVar16, iVar17, iVar19, ixVar13, native_arg_switch_2, p4, pCVar11, pCVar18, pCVar6, pCVar7, pCVar8, pcVar14, piVar2, r1, r2, r3, r4, r5, u_stk_200, xStack_164, xStack_170, xStack_180, xStack_190, xStack_1a0, xStack_1ac, xStack_1c, xStack_1c0, xStack_1d0, xStack_1d4, xStack_1e4, xStack_210, xStack_214_2, xStack_21c, xStack_2c
    local alive = true
    local function __cleanup_LAB_00d5dac4()
        resources:ReleaseResource(xStack_180)
        resources:DestroyActorMap(xStack_170)
        resources:DestroyMovie(xStack_2c)
        quest:DeregisterTimer(xStack_21c)
        resources:ReleaseResource(xStack_190)
    end
    local function __cleanup_LAB_00d5db05()
        quest:PauseAllNonScriptedEntities(false)
        resources:ReleaseResource(xStack_180)
        resources:DestroyActorMap(xStack_170)
        resources:DestroyMovie(xStack_2c)
        quest:DeregisterTimer(xStack_21c)
        resources:ReleaseResource(xStack_190)
    end
    local function __cleanup_LAB_00d5db37()
        resources:ReleaseResource(xStack_210)
        quest:DeregisterTimer(xStack_220)
        quest:DeregisterTimer(xStack_214)
        resources:DestroyMovie(xStack_1c)
    end
    local function __cleanup_LAB_00d5db90()
        quest:DeregisterTimer(xStack_214_2)
        quest:DeregisterTimer(xStack_220)
        quest:DeregisterTimer(xStack_214)
        resources:DestroyMovie(xStack_1c)
    end
    u_stk_200 = 0
    xStack_210 = resources:NewResource()
    bVar4 = false
    if bVar4 ~= 0 then
    end
    bVar4 = resources:TryAcquire(xStack_210, me, 4)
    while not bVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            resources:ReleaseResource(xStack_210)
            return
        end
        bVar4 = resources:TryAcquire(xStack_210, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        quest:EntitySetAsKillable(me, false, true)
        bVar4 = true
        pCVar6 = quest:GetHero()
        quest:EntitySetAlwaysBlockAttacksFromThing(me, pCVar6, bVar4)
        quest:SetIsPushableByHero(me, false)
        quest:SetThingHasInformation(me, false, false, false)
        pCVar7 = quest:GetThingWithScriptName("M_SkillTeacherStand")
        iVar19 = 1
        iVar17 = 0
        iVar16 = 0
        iVar15 = 0x40400000
        pCVar8 = pCVar7:GetPos()
        me:MoveToPosition(pCVar8, iVar15, iVar16, (iVar17 ~= 0), (iVar19 ~= 0))
        quest:SetPlayerUsingRangedDummies(true)
        iVar16 = quest:RegisterTimer()
        quest:SetTimer(iVar16, 0)
        iVar15 = quest:GetStateInt("TutorialState")
        while iVar15 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d5da96 end
            cVar5 = me:IsTalkedToByHero()
            if cVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00d5da96 end
                if quest:GetMasterGameState("HeroTakingGuildTest") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d5da96 end
                    me:ClearCommands()
                    quest:SetStateInt("TutorialState", 3)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d5da96 end
                    xStack_1c0 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    me:ClearCommands()
                    pCVar7 = resources:ScriptThing(xStack_210)
                    pCVar7 = pCVar7
                    r1 = quest:GetHealth(pCVar7)
                    fVar3 = 0.0
                    pCVar7 = nil
                    if fVar3 < fret_0 then
                        iVar19 = 0
                        iVar17 = 1
                        iVar16 = 0
                        iVar15 = 0
                        pcVar14 = "TEXT_QST_028_GUILDMASTER_SKILL_NOT_START"
                        pCVar7 = quest:GetHero()
                        r2 = me:Speak(pCVar7, pcVar14, iVar15, (iVar16 ~= 0), (iVar17 ~= 0), (iVar19 ~= 0))
                        iVar15 = me:IsPerformingScriptTask()
                        cVar5 = iVar15
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_1c0)
                                quest:DeregisterTimer(iVar16)
                                resources:ReleaseResource(xStack_210)
                                return
                            end
                            iVar15 = me:IsPerformingScriptTask()
                            cVar5 = iVar15
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1c0)
                            goto LAB_00d5da96
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_1c0)
                end
            end
            fVar20 = 5.5
            pCVar6 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar20)
            __native_condition_2 = bVar4
            if __native_condition_2 then
                iVar15 = quest:GetTimer(xStack_220)
                __native_condition_2 = iVar15 < 1
            end
            __native_condition_1 = __native_condition_2
            if __native_condition_1 then
                iVar15 = me:IsPerformingScriptTask()
                __native_condition_1 = not iVar15
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00d5da96 end
                iVar16 = quest:AddNewConversation(me, false, false)
                pCVar6 = quest:GetHero()
                quest:AddPersonToConversation(iVar16, pCVar6)
                quest:SetTimer(xStack_220, 10)
                if 0 == 0 then
                    bVar4 = false
                    pCVar6 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar4)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar16, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_FIRST", me, pCVar6, false)
                    -- LAB_00d5b380: (native jump target)
                else
                    if 0 == 1 then
                        bVar4 = false
                        pCVar6 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar4)
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(iVar16, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_SECOND", me, pCVar6, false)
                        goto FLOW_after_lab_00d5b380
                    end
                end
                ::FLOW_after_lab_00d5b380::
                -- TODO(native): xStack_1ec = 1 - xStack_1ec;
            end
            iVar15 = quest:GetStateInt("TutorialState")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            -- LAB_00d5dbab: (native jump target)
            quest:DeregisterTimer(xStack_214)
            resources:DestroyMovie(xStack_1c)
            return
        end
        quest:SetMasterGameState("SkillTrainingStarted", true)
        quest:SetMasterGameState("SkillRepeating", true)
        quest:SetMasterGameState("HeroTakingGuildTest", true)
        cVar5 = quest:GetMasterGameState("SkillRepeating")
        while cVar5 ~= 0 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00d5da96 end
            quest:SetStateInt("TutorialState", 3)
            quest:SetMasterGameState("SkillRepeatKnown", false)
            xStack_180 = resources:NewResource()
            bVar4 = false
            if bVar4 ~= 0 then
            end
            iVar16 = 4
            pCVar18 = xStack_180
            pCVar7 = quest:GetHero()
            bVar4 = resources:TryAcquire(pCVar18, pCVar7, iVar16)
            while not bVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    resources:DestroyMovie(xStack_2c)
                    quest:DeregisterTimer(xStack_21c)
                    resources:ReleaseResource(xStack_190)
                    return
                end
                iVar16 = 4
                pCVar18 = xStack_180
                pCVar7 = quest:GetHero()
                bVar4 = resources:TryAcquire(pCVar18, pCVar7, iVar16)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                resources:DestroyMovie(xStack_2c)
                quest:DeregisterTimer(xStack_21c)
                resources:ReleaseResource(xStack_190)
                return
            end
            xStack_170 = resources:NewActorMap()
            resources:SetActor(xStack_170, "HERO", xStack_180)
            resources:SetActor(xStack_170, "TEACHER", xStack_1c)
            quest:SetStateInt("TutorialState", 2)
            xStack_1c = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_SKILL_START", xStack_170, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetMasterGameState("SkillTrainingStarted", true)
            bVar4 = quest:IsXbox()
            if not bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_PC")
                    bVar4 = quest:MsgIsGameInfoClickedPast()
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            __cleanup_LAB_00d5dac4(); return
                        end
                        bVar4 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)xStack_9c);
                        goto LAB_00d5b7c0
                    end
                end
                __cleanup_LAB_00d5db05()
                return
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                -- LAB_00d5dab8: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                __cleanup_LAB_00d5dac4(); return
            end
            quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW")
            bVar4 = quest:MsgIsGameInfoClickedPast()
            while not bVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d5db05(); return end
                bVar4 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then return end  -- TODO(native): goto LAB_00d5dab8
            ::LAB_00d5b7c0::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_1c0)
            resources:DestroyActorMap(xStack_164)
            resources:ReleaseResource(xStack_1a0)
            xStack_1d4 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 3, 1.0)
            xStack_21c = quest:RegisterTimer()
            iVar15 = xStack_21c
            quest:SetTimer(xStack_21c, 0xf)
            quest:SetMasterGameState("SkillScore", 0)
            r3 = quest:GetThingWithScriptName("ArcheryRing")
            xStack_1d0 = quest:GetMasterGameState("SkillScore")
            quest:EntitySetTargetable(me, false)
            bVar4 = quest:IsXbox()
            if not bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick("GAME_ACTION_UNSHEATHE_RANGED_WEAPON", false, 1.0)
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick("GAME_ACTION_LOCK_TARGET", false, 1.0)
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick("GAME_ACTION_FIRE_RANGED_WEAPON", false, 1.0)
                    goto LAB_00d5ba3e
                end
                -- LAB_00d5db99: (native jump target)
                quest:DeregisterTimer(xStack_220)
                quest:DeregisterTimer(xStack_214)
                resources:DestroyMovie(xStack_1c)
                return
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                quest:DeregisterTimer(xStack_220)
                quest:DeregisterTimer(xStack_214)
                resources:DestroyMovie(xStack_1c)
                return
            end
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick("HUD_BLACK_BUTTON", false, 1.0)
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick("HUD_CONTROLLER_TRIGGER_LEFT", false, 1.0)
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick("HUD_CONTROLLER_X", false, 1.0)
            ::LAB_00d5ba3e::
            c_stk_215 = 0
            quest:DisplayQuestInfo(true)
            quest:SetTimer(iVar15, 5)
            iVar16 = 0
            iVar15 = quest:GetMasterGameState("SkillScore")
            while iVar15 < 3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    quest:DeregisterTimer(xStack_220)
                    quest:DeregisterTimer(xStack_214)
                    resources:DestroyMovie(xStack_1c)
                    return
                end
                u_stk_200 = u_stk_200 | 1
                bVar4 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                __native_condition_4 = bVar4
                if __native_condition_4 then
                    -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingLockTargetButton()
                    __native_condition_4 = bVar4
                end
                __native_condition_3 = __native_condition_4
                if __native_condition_3 then
                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingFireRangedWeaponButton()
                    __native_condition_3 = bVar4
                end
                if __native_condition_3 then
                    bVar4 = true
                else
                    bVar4 = false
                end
                if (u_stk_200 & 1) ~= 0 then
                    u_stk_200 = u_stk_200 & 0xfffffffe
                end
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(xStack_1c)
                        return
                    end
                    c_stk_215 = '\x01'
                    quest:RemoveQuestInfoElement(iVar16)
                    quest:RemoveQuestInfoElement(xStack_1ec)
                    quest:RemoveQuestInfoElement(xStack_1e8)
                else
                    if c_stk_215 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:DeregisterTimer(xStack_220)
                            quest:DeregisterTimer(xStack_214)
                            resources:DestroyMovie(xStack_1c)
                            return
                        end
                        bVar4 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                        quest:UpdateQuestInfoTick(bVar4, (fVar20 ~= 0))
                        -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                        quest:IsPlayerHoldingLockTargetButton()
                        quest:UpdateQuestInfoTick(xStack_1ec, bVar4)
                        -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                        quest:IsPlayerHoldingFireRangedWeaponButton()
                        quest:UpdateQuestInfoTick(xStack_1e8, bVar4)
                        iVar15 = quest:GetTimer(xStack_21c)
                        if iVar15 < 1 then
                            fVar20 = 6.0
                            pCVar7 = quest:GetHero()
                            bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar7, xStack_38, fVar20)
                            __native_condition_5 = bVar4
                            if __native_condition_5 then
                                bVar4 = quest:IsConversationActive(iVar16)
                                __native_condition_5 = not bVar4
                            end
                            if __native_condition_5 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:DeregisterTimer(xStack_220)
                                    quest:DeregisterTimer(xStack_214)
                                    resources:DestroyMovie(xStack_1c)
                                    return
                                end
                                bVar4 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                                if bVar4 then
                                    -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                                    quest:IsPlayerHoldingLockTargetButton()
                                    if bVar4 then
                                        -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                                        quest:IsPlayerHoldingFireRangedWeaponButton()
                                        if bVar4 then goto LAB_00d5bf6c end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_220)
                                            quest:DeregisterTimer(xStack_214)
                                            resources:DestroyMovie(xStack_1c)
                                            return
                                        end
                                        iVar16 = quest:AddNewConversation(me, false, false)
                                        pCVar6 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar16, pCVar6)
                                        bVar4 = quest:IsXbox()
                                        if bVar4 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar4 then
                                                quest:DeregisterTimer(xStack_220)
                                                quest:DeregisterTimer(xStack_214)
                                                resources:DestroyMovie(xStack_1c)
                                                return
                                            end
                                            pCVar6 = quest:GetHero()
                                            quest:AddLineToConversation(iVar16, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_30", me, pCVar6, false)
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar4 then
                                                quest:DeregisterTimer(xStack_220)
                                                quest:DeregisterTimer(xStack_214)
                                                resources:DestroyMovie(xStack_1c)
                                                return
                                            end
                                            pCVar6 = quest:GetHero()
                                            quest:AddLineToConversation(iVar16, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_30", me, pCVar6, false)
                                        end
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_220)
                                            quest:DeregisterTimer(xStack_214)
                                            resources:DestroyMovie(xStack_1c)
                                            return
                                        end
                                        iVar16 = quest:AddNewConversation(me, false, false)
                                        pCVar6 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar16, pCVar6)
                                        bVar4 = quest:IsXbox()
                                        if bVar4 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar4 then
                                                quest:DeregisterTimer(xStack_220)
                                                quest:DeregisterTimer(xStack_214)
                                                resources:DestroyMovie(xStack_1c)
                                                return
                                            end
                                            pCVar6 = quest:GetHero()
                                            quest:AddLineToConversation(iVar16, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_20", me, pCVar6, false)
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar4 then
                                                quest:DeregisterTimer(xStack_220)
                                                quest:DeregisterTimer(xStack_214)
                                                resources:DestroyMovie(xStack_1c)
                                                return
                                            end
                                            pCVar6 = quest:GetHero()
                                            quest:AddLineToConversation(iVar16, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_20", me, pCVar6, false)
                                        end
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:DeregisterTimer(xStack_220)
                                        quest:DeregisterTimer(xStack_214)
                                        resources:DestroyMovie(xStack_1c)
                                        return
                                    end
                                    iVar16 = quest:AddNewConversation(me, false, false)
                                    pCVar6 = quest:GetHero()
                                    quest:AddPersonToConversation(iVar16, pCVar6)
                                    bVar4 = quest:IsXbox()
                                    if bVar4 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_220)
                                            quest:DeregisterTimer(xStack_214)
                                            resources:DestroyMovie(xStack_1c)
                                            return
                                        end
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar16, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_10", me, pCVar6, false)
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_220)
                                            quest:DeregisterTimer(xStack_214)
                                            resources:DestroyMovie(xStack_1c)
                                            return
                                        end
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar16, "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_10", me, pCVar6, false)
                                    end
                                end
                                quest:SetTimer(xStack_21c, 10)
                            end
                        end
                    end
                end
                ::LAB_00d5bf6c::
                iVar15 = quest:GetMasterGameState("SkillScore")
                if xStack_1d0 < iVar15 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(xStack_1c)
                        return
                    end
                    xStack_1d0 = quest:GetMasterGameState("SkillScore")
                else
                    if iVar15 < xStack_1d0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:DeregisterTimer(xStack_220)
                            quest:DeregisterTimer(xStack_214)
                            resources:DestroyMovie(xStack_1c)
                            return
                        end
                        piVar2 = (__native_entity_state:GetStateInt("self_0x18") + 0xa4)
                        -- TODO(native): *piVar2 = *piVar2 + 1;
                        bVar4 = quest:IsConversationActive(iVar16)
                        if not bVar4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:DeregisterTimer(xStack_220)
                                quest:DeregisterTimer(xStack_214)
                                resources:DestroyMovie(xStack_1c)
                                return
                            end
                            iVar16 = quest:AddNewConversation(me, false, false)
                            pCVar7 = quest:GetHero()
                            quest:AddPersonToConversation(iVar16, pCVar7)
                            pCVar7 = quest:GetHero()
                            quest:AddLineToConversation(iVar16, "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", me, pCVar7, false)
                        end
                    end
                end
                quest:UpdateQuestInfoCounter(xStack_1d4, quest:GetMasterGameState("SkillScore"), -1)
                if __native_entity_state:GetStateBool("PlayerNotWarned") then
                    fVar20 = 6.0
                    pCVar7 = quest:GetHero()
                    bVar4 = quest:IsDistanceBetweenThingsOver(pCVar7, xStack_38, fVar20)
                    __native_condition_6 = bVar4
                    if __native_condition_6 then
                        bVar4 = quest:IsConversationActive(iVar16)
                        __native_condition_6 = not bVar4
                    end
                    if __native_condition_6 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:DeregisterTimer(xStack_220)
                            quest:DeregisterTimer(xStack_214)
                            resources:DestroyMovie(xStack_1c)
                            return
                        end
                        __native_entity_state:SetStateBool("PlayerNotWarned", false)
                        iVar16 = quest:AddNewConversation(me, false, false)
                        pCVar7 = quest:GetHero()
                        quest:AddPersonToConversation(iVar16, pCVar7)
                        pCVar7 = quest:GetHero()
                        quest:AddLineToConversation(iVar16, "TEXT_QST_028_MAZE_RING_OUT", me, pCVar7, false)
                    end
                end
                iVar15 = quest:GetMasterGameState("SkillScore")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                quest:DeregisterTimer(xStack_220)
                quest:DeregisterTimer(xStack_214)
                resources:DestroyMovie(xStack_1c)
                return
            end
            if c_stk_215 == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    quest:DeregisterTimer(xStack_220)
                    quest:DeregisterTimer(xStack_214)
                    resources:DestroyMovie(xStack_1c)
                    return
                end
                quest:RemoveQuestInfoElement(fVar20)
                quest:RemoveQuestInfoElement(xStack_1ec)
                quest:RemoveQuestInfoElement(xStack_1e8)
            end
            xStack_190 = resources:NewResource()
            bVar4 = false
            if bVar4 ~= 0 then
            end
            iVar16 = 4
            pCVar18 = xStack_190
            pCVar7 = quest:GetHero()
            bVar4 = resources:TryAcquire(pCVar18, pCVar7, iVar16)
            while not bVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d5db37(); return end
                iVar16 = 4
                pCVar18 = xStack_190
                pCVar7 = quest:GetHero()
                bVar4 = resources:TryAcquire(pCVar18, pCVar7, iVar16)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                __cleanup_LAB_00d5db37()
                return
            end
            xStack_164 = resources:NewActorMap()
            resources:SetActor(xStack_164, "HERO", xStack_210)
            resources:SetActor(xStack_164, "TEACHER", xStack_190)
            xStack_2c = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_SKILL_MOVE", xStack_164, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetStateInt("TutorialState", 4)
            quest:SetMasterGameState("MovingDummiesNeeded", true)
            quest:CameraDefault()
            quest:RemoveQuestInfoElement(xStack_1d4)
            bVar4 = quest:IsXbox()
            if not bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE_PC")
                    bVar4 = quest:MsgIsGameInfoClickedPast()
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): goto LAB_00d5db1f
                        end
                        bVar4 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)xStack_64);
                        goto LAB_00d5c4eb
                    end
                end
                -- LAB_00d5db45: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00d5db1f: (native jump target)
                resources:DestroyMovie(xStack_1c)
                resources:DestroyActorMap(xStack_1ac)
                __cleanup_LAB_00d5db37(); return
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                -- LAB_00d5db13: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): goto LAB_00d5db1f
            end
            quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE")
            bVar4 = quest:MsgIsGameInfoClickedPast()
            while not bVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00d5db45
                bVar4 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then return end  -- TODO(native): goto LAB_00d5db13
            ::LAB_00d5c4eb::
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(xStack_180)
            resources:DestroyActorMap(xStack_170)
            resources:ReleaseResource(xStack_210)
            bVar4 = quest:IsXbox()
            if not bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
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
                resources:DestroyMovie(xStack_1c)
                return
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                quest:DeregisterTimer(xStack_220)
                quest:DeregisterTimer(xStack_214)
                resources:DestroyMovie(xStack_1c)
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
            c_stk_215 = 0
            quest:DisplayQuestInfo(true)
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    quest:DeregisterTimer(xStack_220)
                    quest:DeregisterTimer(xStack_214)
                    resources:DestroyMovie(xStack_1c)
                    return
                end
                iVar15 = quest:GetTimer(xStack_21c)
                if iVar15 < 1 then
                    u_stk_200 = u_stk_200 | 2
                    bVar4 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                    if bVar4 then
                        bVar4 = false
                        goto FLOW_after_lab_00d5c757
                    end
                    fVar20 = 6.0
                    pCVar7 = quest:GetHero()
                    bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar7, xStack_38, fVar20)
                    if not bVar4 then
                        bVar4 = false
                        goto FLOW_after_lab_00d5c757
                    end
                    bVar4 = true
                else
                    -- LAB_00d5c757: (native jump target)
                    bVar4 = false
                end
                ::FLOW_after_lab_00d5c757::
                if (u_stk_200 & 2) ~= 0 then
                    u_stk_200 = u_stk_200 & 0xfffffffd
                end
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(xStack_1c)
                        return
                    end
                    iVar16 = quest:AddNewConversation(me, false, false)
                    pCVar7 = quest:GetHero()
                    quest:AddPersonToConversation(iVar16, pCVar7)
                    pCVar7 = quest:GetHero()
                    quest:AddLineToConversation(iVar16, "TEXT_QST_028_MAZE_BOW_UNSHEATH", me, pCVar7, false)
                    bVar4 = quest:IsXbox()
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:DeregisterTimer(xStack_220)
                            quest:DeregisterTimer(xStack_214)
                            resources:DestroyMovie(xStack_1c)
                            return
                        end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP")
                        bVar4 = quest:MsgIsGameInfoClickedPast()
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:DeregisterTimer(xStack_220)
                                quest:DeregisterTimer(xStack_214)
                                resources:DestroyMovie(xStack_1c)
                                return
                            end
                            bVar4 = quest:MsgIsGameInfoClickedPast()
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:DeregisterTimer(xStack_220)
                            quest:DeregisterTimer(xStack_214)
                            resources:DestroyMovie(xStack_1c)
                            return
                        end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC")
                        bVar4 = quest:MsgIsGameInfoClickedPast()
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:DeregisterTimer(xStack_220)
                                quest:DeregisterTimer(xStack_214)
                                resources:DestroyMovie(xStack_1c)
                                return
                            end
                            bVar4 = quest:MsgIsGameInfoClickedPast()
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(xStack_1c)
                        return
                    end
                    quest:SetTimer(xStack_21c, 0xf)
                end
                bVar4 = quest:MsgOnHeroFiredRangedWeapon()
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(xStack_1c)
                        return
                    end
                end
                u_stk_200 = u_stk_200 | 4
                bVar4 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                __native_condition_8 = bVar4
                if __native_condition_8 then
                    -- TODO(native): IsHeroInProjectileWeaponMode is not a ForgeFSE binding
                    quest:IsHeroInProjectileWeaponMode()
                    __native_condition_8 = bVar4
                end
                __native_condition_7 = __native_condition_8
                if __native_condition_7 then
                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingFireRangedWeaponButton()
                    __native_condition_7 = bVar4
                end
                if __native_condition_7 then
                    bVar4 = true
                else
                    bVar4 = false
                end
                if (u_stk_200 & 4) ~= 0 then
                    u_stk_200 = u_stk_200 & 0xfffffffb
                end
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:DeregisterTimer(xStack_220)
                        quest:DeregisterTimer(xStack_214)
                        resources:DestroyMovie(xStack_1c)
                        return
                    end
                    c_stk_215 = '\x01'
                    quest:RemoveQuestInfoElement(1)
                    quest:RemoveQuestInfoElement(xStack_1ec)
                    quest:RemoveQuestInfoElement(xStack_1e8)
                else
                    if c_stk_215 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:DeregisterTimer(xStack_220)
                            quest:DeregisterTimer(xStack_214)
                            resources:DestroyMovie(xStack_1c)
                            return
                        end
                        bVar4 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                        quest:UpdateQuestInfoTick(bVar4, (fVar20 ~= 0))
                        -- TODO(native): IsHeroInProjectileWeaponMode is not a ForgeFSE binding
                        quest:IsHeroInProjectileWeaponMode()
                        quest:UpdateQuestInfoTick(xStack_1ec, bVar4)
                        -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                        quest:IsPlayerHoldingFireRangedWeaponButton()
                        quest:UpdateQuestInfoTick(xStack_1e8, bVar4)
                    end
                end
            until not (xStack_1d4_b3 == 0)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                quest:DeregisterTimer(xStack_220)
                quest:DeregisterTimer(xStack_214)
                resources:DestroyMovie(xStack_1c)
                return
            end
            if c_stk_215 == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    quest:DeregisterTimer(xStack_220)
                    quest:DeregisterTimer(xStack_214)
                    resources:DestroyMovie(xStack_1c)
                    return
                end
                quest:RemoveQuestInfoElement(fVar3)
                quest:RemoveQuestInfoElement(xStack_1ec)
                quest:RemoveQuestInfoElement(xStack_1e8)
            end
            xStack_214_2 = quest:RegisterTimer()
            -- TODO(native): piVar2 = DAT_0143e8f8;
            iVar16 = (math.modf(quest:ReadGlobalGameDataFloat(0xf04)))
            quest:SetTimer(xStack_214_2, iVar16)
            quest:SetMasterGameState("SkillScore", 0)
            c_stk_215 = 0
            iVar15 = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
            xStack_1d0 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
            xStack_1e4 = quest:AddQuestInfoTimer(xStack_214_2, "HUD_CLOCK_ICON", 1.0)
            quest:DisplayQuestInfo(true)
            quest:UpdateQuestInfoCounter(iVar15, quest:GetMasterGameState("HighestSkillScore"), -1)
            quest:SetMasterGameState("SkillTestOccuring", true)
            quest:SetTimer(xStack_21c, 0xf)
            iVar15 = quest:GetTimer(xStack_214_2)
            while (0 < iVar15 and (c_stk_215 == 0)) do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d5db90(); return end
                if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d5db90(); return end
                    c_stk_215 = '\x01'
                end
                iVar15 = xStack_1d0
                quest:UpdateQuestInfoCounter(xStack_1d0, quest:GetMasterGameState("SkillScore"), -1)
                if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d5db90(); return end
                    quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                    quest:UpdateQuestInfoCounter(iVar15, quest:GetMasterGameState("HighestSkillScore"), -1)
                end
                iVar15 = quest:GetTimer(xStack_21c)
                if iVar15 < 1 then
                    u_stk_200 = u_stk_200 | 8
                    bVar4 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                    if bVar4 then
                        bVar4 = false
                        goto FLOW_after_lab_00d5cd47
                    end
                    bVar4 = true
                else
                    -- LAB_00d5cd47: (native jump target)
                    bVar4 = false
                end
                ::FLOW_after_lab_00d5cd47::
                if (u_stk_200 & 8) ~= 0 then
                    u_stk_200 = u_stk_200 & 0xfffffff7
                end
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d5db90(); return end
                    iVar16 = quest:AddNewConversation(me, false, false)
                    pCVar7 = quest:GetHero()
                    quest:AddPersonToConversation(iVar16, pCVar7)
                    pCVar7 = quest:GetHero()
                    quest:AddLineToConversation(iVar16, "TEXT_QST_028_MAZE_BOW_UNSHEATH", me, pCVar7, false)
                    bVar4 = quest:IsXbox()
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then __cleanup_LAB_00d5db90(); return end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP")
                        bVar4 = quest:MsgIsGameInfoClickedPast()
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then __cleanup_LAB_00d5db90(); return end
                            bVar4 = quest:MsgIsGameInfoClickedPast()
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then __cleanup_LAB_00d5db90(); return end
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC")
                        bVar4 = quest:MsgIsGameInfoClickedPast()
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then __cleanup_LAB_00d5db90(); return end
                            bVar4 = quest:MsgIsGameInfoClickedPast()
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d5db90(); return end
                    quest:SetTimer(xStack_21c, 0xf)
                end
                if not __native_entity_state:GetStateBool("PlayerNotWarned") then
                    -- LAB_00d5cf7a: (native jump target)
                    bVar4 = false
                else
                    u_stk_200 = u_stk_200 | 0x30
                    -- TODO(native): CVar10 = *(this + 4)
                    CVar10 = nil --[[unresolved native value]]
                    fVar20 = 6.0
                    -- TODO(native): pCVar7 = (**(*CVar10 + 0x120))(CVar10,"ArcheryRing","ArcheryRing")
                    pCVar7 = nil --[[unresolved native value]]
                    -- TODO(native): pCVar6 = (**(*CVar10 + 0x118))(CVar10)
                    pCVar6 = nil --[[unresolved native value]]
                    bVar4 = quest:IsDistanceBetweenThingsOver(pCVar6, pCVar7, fVar20)
                    if not bVar4 then
                        bVar4 = false
                        goto FLOW_after_lab_00d5cf7a
                    end
                    bVar4 = true
                end
                ::FLOW_after_lab_00d5cf7a::
                if (u_stk_200 & 0x20) ~= 0 then
                    u_stk_200 = u_stk_200 & 0xffffffdf
                end
                if (u_stk_200 & 0x10) ~= 0 then
                    u_stk_200 = u_stk_200 & 0xffffffef
                end
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d5db90(); return end
                    __native_entity_state:SetStateBool("PlayerNotWarned", false)
                    iVar16 = quest:AddNewConversation(me, false, false)
                    pCVar7 = quest:GetHero()
                    quest:AddPersonToConversation(iVar16, pCVar7)
                    pCVar7 = quest:GetHero()
                    quest:AddLineToConversation(iVar16, "TEXT_QST_028_MAZE_RING_OUT", me, pCVar7, false)
                end
                iVar15 = quest:GetTimer(xStack_214_2)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                __cleanup_LAB_00d5db90()
                return
            end
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            quest:SetMasterGameState("SkillTestOccuring", false)
            bVar4 = quest:IsHeroControlledByPlayer()
            while not bVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d5db90(); return end
                bVar4 = quest:IsHeroControlledByPlayer()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then __cleanup_LAB_00d5db90(); return end
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(iVar15)
            quest:RemoveQuestInfoElement(xStack_1d0)
            quest:RemoveQuestInfoElement(xStack_1e4)
            quest:EntitySetTargetable(me, true)
            xStack_1a0 = resources:NewResource()
            bVar4 = false
            if bVar4 ~= 0 then
            end
            iVar16 = 4
            pCVar18 = xStack_1a0
            pCVar7 = quest:GetHero()
            bVar4 = resources:TryAcquire(pCVar18, pCVar7, iVar16)
            while not bVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00d5db84
                iVar16 = 4
                pCVar18 = xStack_1a0
                pCVar7 = quest:GetHero()
                bVar4 = resources:TryAcquire(pCVar18, pCVar7, iVar16)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                -- LAB_00d5db84: (native jump target)
                resources:DestroyMovie(xStack_2c)
                __cleanup_LAB_00d5db90(); return
            end
            xStack_1ac = resources:NewActorMap()
            resources:SetActor(xStack_1ac, "HERO", xStack_2c)
            resources:SetActor(xStack_1ac, "TEACHER", xStack_210)
            xStack_1c0 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            if xStack_21c_b3 == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    xStack_1d4 = quest:GetMasterGameState("SkillScore")
                    ixVar13 = 0
                    iVar15 = 0
                    repeat
                        iVar16 = iVar15
                        if quest:ReadGlobalGameDataFloatAt(0xec0, ixVar13) < xStack_1d4 ~= (quest:ReadGlobalGameDataFloatAt(0xec0, ixVar13) == xStack_1d4) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then return end  -- TODO(native): goto LAB_00d5db53
                            break
                        end
                        iVar15 = iVar16 + 1
                        ixVar13 = ixVar13 + 1
                    until not (iVar15 < 7)
                    pCVar7 = resources:NewStringMap()
                    native_arg_switch_2 = iVar16
                    repeat
                        if native_arg_switch_2 == 0 then
                            pcVar14 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_APLUS"
                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_11c);
                            break
                        else
                            if native_arg_switch_2 == 1 then
                                pcVar14 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_A"
                                -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_114);
                                break
                            else
                                if native_arg_switch_2 == 2 then
                                    pcVar14 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_B"
                                    -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_10c);
                                    break
                                else
                                    if native_arg_switch_2 == 3 then
                                        pcVar14 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_C"
                                        -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_104);
                                        break
                                    else
                                        if native_arg_switch_2 == 4 then
                                            pcVar14 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_D"
                                            -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_fc);
                                            break
                                        else
                                            if native_arg_switch_2 == 5 then
                                                pcVar14 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_E"
                                                -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_f4);
                                                break
                                            else
                                                if native_arg_switch_2 == 6 then
                                                    pcVar14 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_F"
                                                    -- TODO(native): pCVar11 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)xStack_1cc,xStack_ec);
                                                    break
                                                else
                                                    goto FLOW_native_label_1
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    until not (false)
                    ::FLOW_native_label_1::
                    resources:RunMacroWithStrings("$GRADE", xStack_1ac, pCVar7, false, false)
                    resources:DestroyStringMap(pCVar7)
                    goto LAB_00d5d526
                end
                -- TODO(native): goto LAB_00d5db62
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                -- LAB_00d5db53: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00d5db6f: (native jump target)
                resources:ReleaseResource(xStack_190)
                resources:DestroyActorMap(xStack_164)
                -- TODO(native): goto LAB_00d5db84
            end
            resources:RunMacro("CS_GUILD_SKILL_DISQUALIFIED", xStack_1ac, false, true)
            ::LAB_00d5d526::
            quest:SetStateInt("TutorialState", 0)
            quest:RemoveQuestInfoElement(fVar20)
            quest:Pause(2.0)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_SKILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
            iVar15 = quest:MsgIsQuestionAnsweredYesOrNo()
            while iVar15 < 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00d5db62
                iVar15 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then return end  -- TODO(native): goto LAB_00d5db53
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if iVar15 == 1 then
                if not bVar4 then
                    resources:RunMacro("CS_GUILD_SKILL_CONTINUE", xStack_1ac, false, true)
                    quest:SetMasterGameState("SkillRepeating", false)
                    goto LAB_00d5d7a9
                end
                -- LAB_00d5db62: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): goto LAB_00d5db6f
            end
            if bVar4 then return end  -- TODO(native): goto LAB_00d5db53
            resources:RunMacro("CS_GUILD_MELEE_REPEAT", xStack_1ac, false, false)
            quest:SetMasterGameState("MovingDummiesNeeded", false)
            quest:SetMasterGameState("SkillRepeating", true)
            quest:SetMasterGameState("SkillRepeatKnown", true)
            pCVar7 = quest:GetHero()
            bVar4 = quest:IsObjectInThingsPossession("OBJECT_YEW_LONGBOW", pCVar7)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00d5db62
                quest:TakeObjectFromHero("OBJECT_YEW_LONGBOW")
            end
            cVar5 = quest:GetMasterGameState("SkillDummyReset")
            while cVar5 ~= '\x01' do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then return end  -- TODO(native): goto LAB_00d5db53
                cVar5 = quest:GetMasterGameState("SkillDummyReset")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then return end  -- TODO(native): goto LAB_00d5db62
            ::LAB_00d5d7a9::
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_1c0)
            resources:DestroyActorMap(xStack_1ac)
            resources:ReleaseResource(xStack_1a0)
            quest:DeregisterTimer(xStack_21c)
            quest:DeregisterTimer(xStack_220)
            iVar16 = xStack_21c
            cVar5 = quest:GetMasterGameState("SkillRepeating")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            pCVar7 = quest:GetThingWithScriptName("M_GuildmasterMarker")
            p4 = 1
            iVar19 = 0
            iVar17 = 0
            iVar15 = 0x40400000
            pCVar8 = pCVar7:GetPos()
            me:MoveToPosition(pCVar8, iVar15, iVar17, (iVar19 ~= 0), (p4 ~= 0))
            pCVar7 = quest:GetThingWithScriptName("SkillApprenticeMarker")
            bVar4 = false
            pCVar8 = pCVar7:GetPos()
            r4 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", pCVar8, "SkillApprentice")
            if (r4 ~= nil and not r4:IsNull()) then
                r4:SetToKillOnLevelUnload(0)
            end
            pCVar7 = quest:GetThingWithScriptName("BirdKillerMarker")
            bVar4 = false
            pCVar8 = pCVar7:GetPos()
            r5 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", pCVar8, "BirdKiller")
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_05", "", "")
            delay = 0
            pCVar11 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pCVar11, delay)
            quest:SetPlayerUsingRangedDummies(false)
        end
        ::LAB_00d5da96::
        quest:DeregisterTimer(iVar16)
    end
    resources:ReleaseResource(xStack_210)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("PlayerNotWarned", true)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

