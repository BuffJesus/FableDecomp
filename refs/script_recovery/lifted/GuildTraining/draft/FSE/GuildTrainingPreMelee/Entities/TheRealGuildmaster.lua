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
    local CVar10, __native_condition_1, __native_condition_10, __native_condition_11, __native_condition_2, __native_condition_3, __native_condition_4, __native_condition_5, __native_condition_6, __native_condition_7, __native_condition_8, __native_condition_9, b2, bVar3, cVar4, c_stk_161, c_stk_169, ctr_154, fVar2, fVar20, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, iVar23, iVar7, iVar8, native_arg_switch_2, pCVar12, pCVar5, pCVar6, pcVar15, puVar11, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r3, r30, r31, r32, r33, r34, r35, r36, r4, r5, r6, r7, r8, r9, timerId, uVar14, xStack_108, xStack_114_3, xStack_124, xStack_130, xStack_13c, xStack_14c, xStack_160, xStack_17c, xStack_180, xStack_184, xStack_188, xStack_f8, x_stk_24, x_stk_30, x_stk_48, x_stk_58, x_stk_c
    local alive = true
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
    cVar4 = quest:GetStateBool("GuildmasterTeleport")
    uVar14 = 0
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        cVar4 = quest:GetStateBool("GuildmasterTeleport")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_01", "", "")
    xStack_17c = resources:NewResource()
    bVar3 = false
    if bVar3 ~= 0 then
    end
    bVar3 = resources:TryAcquire(xStack_17c, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d55c4f end
        bVar3 = resources:TryAcquire(xStack_17c, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d55c4f end
    quest:EntitySetAsKillable(me, false, true)
    bVar3 = false
    quest:SetIsPushableByHero(me, bVar3)
    quest:SetThingHasInformation(me, false, false, false)
    bVar3 = false
    pCVar5 = quest:GetThingWithScriptName("M_MeleeTeacherStand")
    quest:EntityTeleportToThing(me, pCVar5, bVar3)
    xStack_188 = quest:RegisterTimer()
    quest:SetTimer(xStack_188, 0)
    c_stk_169 = 1
    repeat
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d55c46 end
        cVar4 = me:IsTalkedToByHero()
        if cVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d55c46 end
            c_stk_169 = 0
        end
        fVar20 = 5.5
        pCVar6 = quest:GetHero()
        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar20)
        __native_condition_1 = not bVar3
        if not __native_condition_1 then
            iVar7 = quest:GetTimer(xStack_188)
            __native_condition_1 = 0 < iVar7
        end
        if __native_condition_1 then goto FLOW_native_label_1 end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d55c46 end
        iVar8 = quest:AddNewConversation(me, false, false)
        pCVar6 = quest:GetHero()
        quest:AddPersonToConversation(iVar8, pCVar6)
        quest:SetTimer(xStack_188, 5)
        native_arg_switch_2 = uVar14
        repeat
            if native_arg_switch_2 == 0 then
                bVar3 = false
                pCVar6 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                pCVar6 = quest:GetHero()
                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FIRST", me, pCVar6, false)
                me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, true, false, false)
                uVar14 = 1
                break
            else
                if native_arg_switch_2 == 1 then
                    bVar3 = false
                    pCVar6 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_SECOND", me, pCVar6, false)
                    uVar14 = 2
                    goto FLOW_after_lab_00d53316
                else
                    if native_arg_switch_2 == 2 then
                        bVar3 = false
                        pCVar6 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_THIRD", me, pCVar6, false)
                        uVar14 = 3
                        break
                    else
                        if native_arg_switch_2 == 3 then
                            bVar3 = false
                            pCVar6 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FOURTH", me, pCVar6, false)
                            -- LAB_00d53316: (native jump target)
                            uVar14 = 2
                        end
                    end
                end
                ::FLOW_after_lab_00d53316::
            end
        until not (false)
        ::FLOW_native_label_1::
    until not (c_stk_169 ~= 0)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:SetStateBool("WhisperStopFollowing", true)
        xStack_14c = resources:NewResource()
        bVar3 = false
        if bVar3 ~= 0 then
        end
        iVar8 = 4
        pCVar6 = quest:GetHero()
        bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:DestroyMovie(xStack_14c)
                goto FLOW_after_lab_00d533bb
            end
            iVar8 = 4
            pCVar6 = quest:GetHero()
            bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00d533bb: (native jump target)
            resources:DestroyMovie(xStack_14c)
        else
            xStack_13c = resources:NewActorMap()
            resources:SetActor(xStack_13c, "HERO", xStack_14c)
            resources:SetActor(xStack_13c, "TEACHER", xStack_17c)
            xStack_124 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_PREMELEE_PUNCH", xStack_13c, false, true)
            quest:FixMovieSequenceCamera(false)
            b2 = true
            bVar3 = false
            pCVar6 = quest:GetThingWithScriptName("PreMeleeWhisper")
            quest:RemoveThing(pCVar6, bVar3, b2)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_124)
            resources:DestroyActorMap(xStack_13c)
            resources:DestroyMovie(xStack_14c)
            bVar3 = quest:IsXbox()
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH")
                    bVar3 = quest:MsgIsGameInfoClickedPast()
                    while not bVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d55c46 end
                        bVar3 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        -- LAB_00d536c0: (native jump target)
                        xStack_184 = quest:RegisterTimer()
                        timerId = xStack_184
                        quest:SetTimer(xStack_184, 10)
                        quest:SetStateInt("PreMeleeMode", 1)
                        quest:SetStateInt("DummyHits", 0)
                        CVar10 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                        xStack_130 = CVar10
                        quest:DisplayQuestInfo(true)
                        iVar7 = quest:GetStateInt("DummyHits")
                        while iVar7 < 7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d55c3d end
                            quest:UpdateQuestInfoCounter(CVar10, quest:GetStateInt("DummyHits"), -1)
                            if 0x0 ~= quest:GetStateInt("DummyHits") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d55c3d end
                                -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                quest:SetTimer(timerId, 10)
                            end
                            iVar7 = quest:GetTimer(timerId)
                            if iVar7 < 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d55c3d end
                                iVar8 = quest:AddNewConversation(me, false, false)
                                pCVar6 = quest:GetHero()
                                quest:AddPersonToConversation(iVar8, pCVar6)
                                pCVar6 = quest:GetHero()
                                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_HIT_DUMMY", me, pCVar6, false)
                                bVar3 = quest:IsXbox()
                                if bVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d55c3d end
                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP")
                                    bVar3 = quest:MsgIsGameInfoClickedPast()
                                    while not bVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d55c3d end
                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d55c3d end
                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP_PC")
                                    bVar3 = quest:MsgIsGameInfoClickedPast()
                                    while not bVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d55c3d end
                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                    end
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d55c3d end
                                quest:SetTimer(xStack_184, 10)
                                timerId = xStack_184
                                CVar10 = xStack_130
                            end
                            iVar7 = quest:GetStateInt("DummyHits")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            quest:RemoveQuestInfoElement(CVar10)
                            quest:DisplayQuestInfo(false)
                            xStack_14c = resources:NewResource()
                            bVar3 = false
                            if bVar3 ~= 0 then
                            end
                            iVar8 = 4
                            pCVar6 = quest:GetHero()
                            bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                            while not bVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    resources:DestroyMovie(xStack_14c)
                                    goto FLOW_after_lab_00d53a0b
                                end
                                iVar8 = 4
                                pCVar6 = quest:GetHero()
                                bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                -- LAB_00d53a0b: (native jump target)
                                resources:DestroyMovie(xStack_14c)
                            else
                                xStack_13c = resources:NewActorMap()
                                resources:SetActor(xStack_13c, "HERO", xStack_14c)
                                resources:SetActor(xStack_13c, "TEACHER", xStack_17c)
                                xStack_124 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_PREMELEE_STICK", xStack_13c, false, true)
                                quest:FixMovieSequenceCamera(false)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_124)
                                resources:DestroyActorMap(xStack_13c)
                                resources:DestroyMovie(xStack_14c)
                                bVar3 = quest:IsXbox()
                                if bVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if not bVar3 then
                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK")
                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                        while not bVar3 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00d55c3d end
                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if not bVar3 then
                                            -- LAB_00d53c7e: (native jump target)
                                            quest:SetStateInt("PreMeleeMode", 2)
                                            quest:SetStateInt("DummyHits", 0)
                                            quest:SetTimer(timerId, 10)
                                            CVar10 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                                            xStack_130 = CVar10
                                            quest:DisplayQuestInfo(true)
                                            iVar7 = quest:GetStateInt("DummyHits")
                                            while iVar7 < 7 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then goto LAB_00d55c3d end
                                                quest:UpdateQuestInfoCounter(CVar10, quest:GetStateInt("DummyHits"), -1)
                                                if 0x0 ~= quest:GetStateInt("DummyHits") then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d55c3d end
                                                    -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                                    quest:SetTimer(timerId, 10)
                                                end
                                                iVar7 = quest:GetTimer(timerId)
                                                if iVar7 < 1 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d55c3d end
                                                    iVar8 = quest:AddNewConversation(me, false, false)
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddPersonToConversation(iVar8, pCVar6)
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, pCVar6, false)
                                                    bVar3 = quest:IsXbox()
                                                    if bVar3 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then goto LAB_00d55c3d end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        while not bVar3 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then goto LAB_00d55c3d end
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then goto LAB_00d55c3d end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        while not bVar3 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then goto LAB_00d55c3d end
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d55c3d end
                                                    quest:SetTimer(xStack_184, 10)
                                                    timerId = xStack_184
                                                    CVar10 = xStack_130
                                                end
                                                iVar7 = quest:GetStateInt("DummyHits")
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if not bVar3 then
                                                quest:RemoveQuestInfoElement(CVar10)
                                                quest:DisplayQuestInfo(false)
                                                xStack_160 = nil
                                                xStack_124 = resources:StartMovie("")
                                                quest:StartMovieSequence()
                                                xStack_14c = resources:NewResource()
                                                bVar3 = false
                                                if bVar3 ~= 0 then
                                                end
                                                iVar8 = 4
                                                pCVar6 = quest:GetHero()
                                                bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                while not bVar3 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then
                                                        resources:DestroyMovie(xStack_14c)
                                                        resources:DestroyMovie(xStack_124)
                                                        goto FLOW_after_lab_00d53ff2
                                                    end
                                                    iVar8 = 4
                                                    pCVar6 = quest:GetHero()
                                                    bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then
                                                    -- LAB_00d53ff2: (native jump target)
                                                    resources:DestroyMovie(xStack_14c)
                                                    resources:DestroyMovie(xStack_124)
                                                else
                                                    r1 = quest:GetThingWithScriptName("PreMeleeDummy")
                                                    if not (r1 ~= nil and not r1:IsNull()) then
                                                    else
                                                        puVar11 = r1:GetPos()
                                                    end
                                                    -- TODO(native): CStack_114._0_4_ = *puVar11;
                                                    -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                                    -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                                    -- TODO(native): CreateEffect is not a ForgeFSE binding
                                                    quest:CreateEffect(xStack_dc, "SMASH_DUMMY_01", xStack_114_3, "", 0.0, false, false)
                                                    quest:FadeOutAndKillEntity(r1, true, 1.0, true)
                                                    xStack_13c = resources:NewActorMap()
                                                    resources:SetActor(xStack_13c, "HERO", xStack_14c)
                                                    resources:SetActor(xStack_13c, "TEACHER", xStack_17c)
                                                    quest:FixMovieSequenceCamera(true)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", xStack_13c, false, false)
                                                    quest:PauseAllNonScriptedEntities(true)
                                                    pCVar6 = quest:CreateExperienceOrb(xStack_114_3, 1)
                                                    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                                    quest:EntitySetCutsceneBehaviour(xStack_160, 2)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED", xStack_13c, false, true)
                                                    quest:FixMovieSequenceCamera(false)
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    resources:DestroyActorMap(xStack_13c)
                                                    resources:DestroyMovie(xStack_14c)
                                                    resources:DestroyMovie(xStack_124)
                                                    bVar3 = quest:IsXbox()
                                                    if bVar3 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if not bVar3 then
                                                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            while not bVar3 do
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if bVar3 then goto LAB_00d55c34 end
                                                                bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                -- LAB_00d5439e: (native jump target)
                                                                xStack_180 = quest:RegisterTimer()
                                                                quest:SetTimer(xStack_180, 10)
                                                                iVar7 = (xStack_160 ~= nil and xStack_160:IsAlive())
                                                                cVar4 = iVar7
                                                                while cVar4 do
                                                                    alive = quest:NewScriptFrame(me)
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00d55c2b end
                                                                    iVar7 = quest:GetTimer(xStack_180)
                                                                    if iVar7 < 1 then
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then goto LAB_00d55c2b end
                                                                        iVar8 = quest:AddNewConversation(me, false, false)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddPersonToConversation(iVar8, pCVar6)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, pCVar6, false)
                                                                        quest:SetTimer(xStack_180, 10)
                                                                    end
                                                                    iVar7 = (xStack_160 ~= nil and xStack_160:IsAlive())
                                                                    cVar4 = iVar7
                                                                end
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if not bVar3 then
                                                                    quest:Pause(0.5)
                                                                    xStack_14c = resources:NewResource()
                                                                    bVar3 = false
                                                                    if bVar3 ~= 0 then
                                                                    end
                                                                    iVar8 = 4
                                                                    pCVar6 = quest:GetHero()
                                                                    bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                                    while not bVar3 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then
                                                                            goto FLOW_after_lab_00d54dfa
                                                                        end
                                                                        iVar8 = 4
                                                                        pCVar6 = quest:GetHero()
                                                                        bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                                    end
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then
                                                                        -- LAB_00d54dfa: (native jump target)
                                                                    else
                                                                        xStack_114_3 = resources:NewActorMap()
                                                                        resources:SetActor(xStack_114_3, "HERO", xStack_14c)
                                                                        resources:SetActor(xStack_114_3, "TEACHER", xStack_17c)
                                                                        xStack_124 = resources:StartMovie("")
                                                                        quest:StartMovieSequence()
                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                        quest:FixMovieSequenceCamera(true)
                                                                        resources:RunMacro("CS_GUILD_PREMELEE_ALARM", xStack_114_3, false, true)
                                                                        quest:FixMovieSequenceCamera(false)
                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                        resources:DestroyActorMap(xStack_114_3)
                                                                        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", "Q_GuildTrainingWoodsMelee", false)
                                                                        quest:SetQuestCardObjective("Q_GuildTrainingWoodsMelee", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                        quest:KickOffQuestStartScreen("Q_GuildTrainingWoodsMelee", true, false)
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if not bVar3 then
                                                                            alive = quest:NewScriptFrame(me)
                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                            bVar3 = not alive
                                                                            if not bVar3 then
                                                                                alive = quest:NewScriptFrame(me)
                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                bVar3 = not alive
                                                                                if not bVar3 then
                                                                                    alive = quest:NewScriptFrame(me)
                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                    bVar3 = not alive
                                                                                    if not bVar3 then
                                                                                        alive = quest:NewScriptFrame(me)
                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                        bVar3 = not alive
                                                                                        if not bVar3 then
                                                                                            bVar3 = quest:DisplayTutorial(0x1c)
                                                                                            if bVar3 then
                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                bVar3 = not alive
                                                                                                if not bVar3 then
                                                                                                    bVar3 = quest:MsgIsTutorialClickedPast()
                                                                                                    while not bVar3 do
                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b end
                                                                                                        bVar3 = quest:MsgIsTutorialClickedPast()
                                                                                                    end
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    bVar3 = not alive
                                                                                                    if not bVar3 then
                                                                                                        pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                        quest:MiniMapRemoveMarker(pCVar6)
                                                                                                        pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                        quest:MiniMapAddMarker(pCVar6, "HUD_ORB_GREEN_SMALL")
                                                                                                        pCVar6 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                        iVar23 = 1
                                                                                                        iVar8 = 0
                                                                                                        iVar7 = 1.0
                                                                                                        pCVar12 = pCVar6:GetPos()
                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (xStack_14c ~= 0), (iVar23 ~= 0))
                                                                                                        c_stk_169 = 1
                                                                                                        ctr_154 = 0
                                                                                                        quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                                        c_stk_161 = 0
                                                                                                        repeat
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b end
                                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c6
                                                                                                            if not __native_entity_state:GetStateBool("WoodsEndPlayed") then
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b end
                                                                                                                __native_entity_state:SetStateBool("WoodsEndPlayed", true)
                                                                                                                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                                quest:MiniMapRemoveMarker(pCVar6)
                                                                                                                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                                quest:MiniMapAddMarker(pCVar6, "HUD_ORB_QUEST_CORE")
                                                                                                                xStack_108 = resources:NewResource()
                                                                                                                bVar3 = false
                                                                                                                if bVar3 ~= 0 then
                                                                                                                end
                                                                                                                iVar8 = 4
                                                                                                                pCVar6 = quest:GetHero()
                                                                                                                bVar3 = resources:TryAcquire(xStack_108, pCVar6, iVar8)
                                                                                                                while not bVar3 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d55c91_c6
                                                                                                                    iVar8 = 4
                                                                                                                    pCVar6 = quest:GetHero()
                                                                                                                    bVar3 = resources:TryAcquire(xStack_108, pCVar6, iVar8)
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then
                                                                                                                    -- LAB_00d55c91_c6: (native jump target)
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                                xStack_13c = resources:NewActorMap()
                                                                                                                resources:SetActor(xStack_13c, "HERO", xStack_108)
                                                                                                                resources:SetActor(xStack_13c, "GUARD", xStack_17c)
                                                                                                                xStack_124 = resources:StartMovie("")
                                                                                                                quest:StartMovieSequence()
                                                                                                                pCVar6 = 0x1
                                                                                                                quest:PauseAllNonScriptedEntities(true)
                                                                                                                quest:FixMovieSequenceCamera(true)
                                                                                                                resources:RunMacro("CS_GUILD_MELEE_WOODSWON", xStack_13c, false, true)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then
                                                                                                                    -- LAB_00d55c72_c6: (native jump target)
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    -- LAB_00d55c7f_c6: (native jump target)
                                                                                                                    resources:DestroyActorMap(xStack_13c)
                                                                                                                    -- TODO(native): goto LAB_00d55c91_c6
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if iVar7 == 1 then
                                                                                                                    if bVar3 then
                                                                                                                        -- LAB_00d55c63_c6: (native jump target)
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        -- TODO(native): goto LAB_00d55c7f_c6
                                                                                                                    end
                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                    quest:Pause(1.0)
                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                else
                                                                                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                                                                                    r1 = resources:ScriptThing(xStack_17c)
                                                                                                                    pCVar5 = r1
                                                                                                                    fret_0 = quest:GetHealth(pCVar5)
                                                                                                                    fVar2 = 0.0
                                                                                                                    if fVar2 < fret_0 then
                                                                                                                        iVar23 = 1
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0
                                                                                                                        pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        r2 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                        cVar4 = iVar7
                                                                                                                        while cVar4 do
                                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                        end
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c6
                                                                                                                    end
                                                                                                                    pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                    iVar23 = 0
                                                                                                                    iVar8 = 0
                                                                                                                    iVar7 = 1.0
                                                                                                                    pCVar12 = pCVar5:GetPos()
                                                                                                                    me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                end
                                                                                                                quest:FixMovieSequenceCamera(false)
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                resources:DestroyActorMap(xStack_13c)
                                                                                                            else
                                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                    -- LAB_00d54f9c_c6: (native jump target)
                                                                                                                    bVar3 = false
                                                                                                                else
                                                                                                                    bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                                    if not bVar3 then return end  -- TODO(native): goto LAB_00d54f9c_c6
                                                                                                                    bVar3 = true
                                                                                                                end
                                                                                                                if bVar3 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55c2b end
                                                                                                                    bVar3 = false
                                                                                                                    if bVar3 ~= 0 then
                                                                                                                    end
                                                                                                                    bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                                    if bVar3 then
                                                                                                                        repeat
                                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then goto LAB_00d55c2b end
                                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                                        until not (bVar3)
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55c2b end
                                                                                                                    bVar3 = false
                                                                                                                    if bVar3 ~= 0 then
                                                                                                                    end
                                                                                                                    bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                                    while not bVar3 do
                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then goto LAB_00d55c2b end
                                                                                                                        bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55c2b end
                                                                                                                    xStack_14c = resources:StartMovie("")
                                                                                                                    quest:StartMovieSequence()
                                                                                                                    pCVar6 = 0x1
                                                                                                                    quest:PauseAllNonScriptedEntities(true)
                                                                                                                    x_stk_30 = resources:ScriptThing(xStack_17c)
                                                                                                                    pCVar5 = x_stk_30
                                                                                                                    fret_00 = quest:GetHealth(pCVar5)
                                                                                                                    fVar2 = 0.0
                                                                                                                    if fret_00 <= fVar2 then
                                                                                                                        -- LAB_00d551d4_c6: (native jump target)
                                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        while iVar7 < 0 do
                                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                                                                                            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        end
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if not bVar3 then
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if iVar7 == 1 then
                                                                                                                                if bVar3 then
                                                                                                                                    __region_LAB_00d55c9f_c6()
                                                                                                                                    goto LAB_00d55c2b
                                                                                                                                end
                                                                                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                                                                                quest:Pause(1.0)
                                                                                                                                quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            else
                                                                                                                                if bVar3 then goto LAB_00d55cba_c6 end
                                                                                                                                x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                                pCVar5 = x_stk_58
                                                                                                                                fret_01 = quest:GetHealth(pCVar5)
                                                                                                                                fVar2 = 0.0
                                                                                                                                if fVar2 < fret_01 then
                                                                                                                                    iVar23 = 1
                                                                                                                                    iVar8 = 0
                                                                                                                                    iVar7 = 0
                                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                                    r3 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                                    cVar4 = iVar7
                                                                                                                                    while cVar4 do
                                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                                        bVar3 = not alive
                                                                                                                                        if bVar3 then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                                        cVar4 = iVar7
                                                                                                                                    end
                                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                                    bVar3 = not alive
                                                                                                                                    if bVar3 then goto LAB_00d55cba_c6 end
                                                                                                                                end
                                                                                                                                pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                                iVar23 = 0
                                                                                                                                iVar8 = 0
                                                                                                                                iVar7 = 1.0
                                                                                                                                pCVar12 = pCVar5:GetPos()
                                                                                                                                me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                            end
                                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                                            goto LAB_00d55480_c6
                                                                                                                        end
                                                                                                                    else
                                                                                                                        iVar23 = 1
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0
                                                                                                                        pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        r4 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                        cVar4 = iVar7
                                                                                                                        while cVar4 do
                                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then __region_LAB_00d55c9f_c6(); goto LAB_00d55c2b end
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                        end
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if not bVar3 then return end  -- TODO(native): goto LAB_00d551d4_c6
                                                                                                                    end
                                                                                                                    ::LAB_00d55cba_c6::
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                            end
                                                                                                            ::LAB_00d55480_c6::
                                                                                                            cVar4 = me:IsTalkedToByHero()
                                                                                                            if cVar4 then
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b end
                                                                                                                xStack_f8 = resources:StartMovie("")
                                                                                                                quest:StartMovieSequence()
                                                                                                                pCVar6 = 0x1
                                                                                                                quest:PauseAllNonScriptedEntities(true)
                                                                                                                me:ClearCommands()
                                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if not bVar3 then
                                                                                                                        x_stk_c = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_c
                                                                                                                        fret_03 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_03 then
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r5 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                                                                                        end
                                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        while iVar7 < 0 do
                                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        end
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if not bVar3 then
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if iVar7 == 1 then
                                                                                                                                if not bVar3 then
                                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                                    quest:Pause(1.0)
                                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                                    goto LAB_00d5595a_c6
                                                                                                                                end
                                                                                                                                __region_LAB_00d555f3_c6(); goto LAB_00d55c2b
                                                                                                                            end
                                                                                                                            if not bVar3 then
                                                                                                                                x_stk_48 = resources:ScriptThing(xStack_17c)
                                                                                                                                pCVar5 = x_stk_48
                                                                                                                                fret_04 = quest:GetHealth(pCVar5)
                                                                                                                                fVar2 = 0.0
                                                                                                                                if fVar2 < fret_04 then
                                                                                                                                    iVar23 = 1
                                                                                                                                    iVar8 = 0
                                                                                                                                    iVar7 = 0
                                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                                    r6 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                                    cVar4 = iVar7
                                                                                                                                    while cVar4 do
                                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                                        bVar3 = not alive
                                                                                                                                        if bVar3 then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                                        cVar4 = iVar7
                                                                                                                                    end
                                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                                    bVar3 = not alive
                                                                                                                                    if bVar3 then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                                                                                                end
                                                                                                                                pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                                iVar23 = 0
                                                                                                                                iVar8 = 0
                                                                                                                                iVar7 = 1.0
                                                                                                                                pCVar12 = pCVar5:GetPos()
                                                                                                                                me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                                goto LAB_00d5595a_c6
                                                                                                                            end
                                                                                                                        end
                                                                                                                    end
                                                                                                                    __region_LAB_00d55cd5_c6()
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then
                                                                                                                    __region_LAB_00d555f3_c6()
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                                x_stk_24 = resources:ScriptThing(xStack_17c)
                                                                                                                pCVar5 = x_stk_24
                                                                                                                fret_02 = quest:GetHealth(pCVar5)
                                                                                                                fVar2 = 0.0
                                                                                                                if fVar2 < fret_02 then
                                                                                                                    iVar23 = 1
                                                                                                                    iVar8 = 0
                                                                                                                    iVar7 = 0
                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD"
                                                                                                                    pCVar6 = quest:GetHero()
                                                                                                                    r7 = me:Speak(pCVar6, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                    while cVar4 do
                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then __region_LAB_00d55cd5_c6(); goto LAB_00d55c2b end
                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                        cVar4 = iVar7
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d555f3_c6(); goto LAB_00d55c2b end
                                                                                                                end
                                                                                                                ::LAB_00d5595a_c6::
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                            end
                                                                                                            if c_stk_169 ~= 0 then
                                                                                                                fVar20 = 5.5
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar20)
                                                                                                                __native_condition_3 = bVar3
                                                                                                                if __native_condition_3 then
                                                                                                                    iVar7 = quest:GetTimer(xStack_188)
                                                                                                                    __native_condition_3 = iVar7 < 1
                                                                                                                end
                                                                                                                __native_condition_2 = __native_condition_3
                                                                                                                if __native_condition_2 then
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    __native_condition_2 = not iVar7
                                                                                                                end
                                                                                                                if __native_condition_2 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55c2b end
                                                                                                                    iVar8 = quest:AddNewConversation(me, false, false)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddPersonToConversation(iVar8, pCVar5)
                                                                                                                    quest:SetTimer(xStack_188, 10)
                                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then goto LAB_00d55c2b end
                                                                                                                        if ctr_154 == 0x1 then
                                                                                                                            bVar3 = false
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                                            -- LAB_00d55b4e_c6: (native jump target)
                                                                                                                        else
                                                                                                                            if ctr_154 == 0x2 then
                                                                                                                                bVar3 = false
                                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, pCVar5, false)
                                                                                                                                -- TODO(native): goto LAB_00d55b4e_c6
                                                                                                                            end
                                                                                                                        end
                                                                                                                        ctr_154 = 1 - ctr_154
                                                                                                                    else
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then goto LAB_00d55c2b end
                                                                                                                        bVar3 = false
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            if c_stk_161 == 0 then
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b end
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                if not iVar7 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55c2b end
                                                                                                                    bVar3 = false
                                                                                                                    c_stk_161 = 1
                                                                                                                    pCVar6 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                                                                                                                end
                                                                                                            end
                                                                                                        until not (c_stk_169 ~= 0)
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if not bVar3 then
                                                                                                            quest:SetStateBool("HeroSleeps", true)
                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                            quest:SetTimeOfDay(11.0)
                                                                                                            quest:ChangeHeroHealthBy(1000.0, true, false)
                                                                                                            quest:ResetPlayerCreatureCombatMultiplier()
                                                                                                        end
                                                                                                        goto FLOW_after_lab_00d54846
                                                                                                    end
                                                                                                end
                                                                                            else
                                                                                                -- LAB_00d54846: (native jump target)
                                                                                                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                quest:MiniMapRemoveMarker(pCVar6)
                                                                                                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                quest:MiniMapAddMarker(pCVar6, "HUD_ORB_GREEN_SMALL")
                                                                                                pCVar6 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                iVar23 = 1
                                                                                                iVar8 = 0
                                                                                                iVar7 = 1.0
                                                                                                pCVar12 = pCVar6:GetPos()
                                                                                                me:MoveToPosition(pCVar12, iVar7, iVar8, (xStack_14c ~= 0), (iVar23 ~= 0))
                                                                                                c_stk_169 = 1
                                                                                                ctr_154 = 0
                                                                                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                                c_stk_161 = 0
                                                                                                repeat
                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    bVar3 = not alive
                                                                                                    if bVar3 then goto LAB_00d55c2b end
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        -- LAB_00d54f9c_c7: (native jump target)
                                                                                                        bVar3 = false
                                                                                                        if bVar3 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b end
                                                                                                            bVar3 = false
                                                                                                            if bVar3 ~= 0 then
                                                                                                            end
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if bVar3 then
                                                                                                                repeat
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55c2b end
                                                                                                                    bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                                until not (bVar3)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b end
                                                                                                            bVar3 = false
                                                                                                            if bVar3 ~= 0 then
                                                                                                            end
                                                                                                            bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                            while not bVar3 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b end
                                                                                                                bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b end
                                                                                                            xStack_14c = resources:StartMovie("")
                                                                                                            quest:StartMovieSequence()
                                                                                                            pCVar6 = 0x1
                                                                                                            quest:PauseAllNonScriptedEntities(true)
                                                                                                            x_stk_30 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = x_stk_30
                                                                                                            fret_00 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fret_00 <= fVar2 then
                                                                                                                -- LAB_00d551d4_c7: (native jump target)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if iVar7 == 1 then
                                                                                                                        if bVar3 then
                                                                                                                            __region_LAB_00d55c9f_c7()
                                                                                                                            goto LAB_00d55c2b
                                                                                                                        end
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                    else
                                                                                                                        if bVar3 then goto LAB_00d55cba_c7 end
                                                                                                                        x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_58
                                                                                                                        fret_01 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_01 then
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r8 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then goto LAB_00d55cba_c7 end
                                                                                                                        end
                                                                                                                        pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 1.0
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    goto LAB_00d55480
                                                                                                                end
                                                                                                            else
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r9 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55c9f_c7(); goto LAB_00d55c2b end
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then return end  -- TODO(native): goto LAB_00d551d4_c7
                                                                                                            end
                                                                                                            ::LAB_00d55cba_c7::
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        goto FLOW_after_lab_00d54f9c
                                                                                                    end
                                                                                                    if not __native_entity_state:GetStateBool("WoodsEndPlayed") then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b end
                                                                                                        __native_entity_state:SetStateBool("WoodsEndPlayed", true)
                                                                                                        pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                        quest:MiniMapRemoveMarker(pCVar6)
                                                                                                        pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                        quest:MiniMapAddMarker(pCVar6, "HUD_ORB_QUEST_CORE")
                                                                                                        xStack_108 = resources:NewResource()
                                                                                                        bVar3 = false
                                                                                                        if bVar3 ~= 0 then
                                                                                                        end
                                                                                                        iVar8 = 4
                                                                                                        pCVar6 = quest:GetHero()
                                                                                                        bVar3 = resources:TryAcquire(xStack_108, pCVar6, iVar8)
                                                                                                        while not bVar3 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            iVar8 = 4
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            bVar3 = resources:TryAcquire(xStack_108, pCVar6, iVar8)
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d55c91: (native jump target)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        xStack_13c = resources:NewActorMap()
                                                                                                        resources:SetActor(xStack_13c, "HERO", xStack_108)
                                                                                                        resources:SetActor(xStack_13c, "GUARD", xStack_17c)
                                                                                                        xStack_124 = resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        pCVar6 = 0x1
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        quest:FixMovieSequenceCamera(true)
                                                                                                        resources:RunMacro("CS_GUILD_MELEE_WOODSWON", xStack_13c, false, true)
                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        while iVar7 < 0 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                -- LAB_00d55c7f_c9: (native jump target)
                                                                                                                resources:DestroyActorMap(xStack_13c)
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d55c72: (native jump target)
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            -- LAB_00d55c7f: (native jump target)
                                                                                                            resources:DestroyActorMap(xStack_13c)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if iVar7 == 1 then
                                                                                                            if bVar3 then
                                                                                                                -- LAB_00d55c63: (native jump target)
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                resources:DestroyActorMap(xStack_13c)
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                            quest:Pause(1.0)
                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                        else
                                                                                                            if bVar3 then
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                -- LAB_00d55c7f_c12: (native jump target)
                                                                                                                resources:DestroyActorMap(xStack_13c)
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            r1 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = r1
                                                                                                            fret_0 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fVar2 < fret_0 then
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r10 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        -- LAB_00d55c7f_c13: (native jump target)
                                                                                                                        resources:DestroyActorMap(xStack_13c)
                                                                                                                        goto LAB_00d55c2b
                                                                                                                    end
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    -- LAB_00d55c7f_c14: (native jump target)
                                                                                                                    resources:DestroyActorMap(xStack_13c)
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                            end
                                                                                                            pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                            iVar23 = 0
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 1.0
                                                                                                            pCVar12 = pCVar5:GetPos()
                                                                                                            me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                        end
                                                                                                        quest:FixMovieSequenceCamera(false)
                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                        resources:DestroyActorMap(xStack_13c)
                                                                                                    else
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            -- LAB_00d54f9c: (native jump target)
                                                                                                            bVar3 = false
                                                                                                        else
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if not bVar3 then
                                                                                                                bVar3 = false
                                                                                                                goto FLOW_after_lab_00d54f9c_746
                                                                                                            end
                                                                                                            bVar3 = true
                                                                                                        end
                                                                                                        ::FLOW_after_lab_00d54f9c_746::
                                                                                                        if bVar3 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b end
                                                                                                            bVar3 = false
                                                                                                            if bVar3 ~= 0 then
                                                                                                            end
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if bVar3 then
                                                                                                                repeat
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55c2b end
                                                                                                                    bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                                until not (bVar3)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b end
                                                                                                            bVar3 = false
                                                                                                            if bVar3 ~= 0 then
                                                                                                            end
                                                                                                            bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                            while not bVar3 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b end
                                                                                                                bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b end
                                                                                                            xStack_14c = resources:StartMovie("")
                                                                                                            quest:StartMovieSequence()
                                                                                                            pCVar6 = 0x1
                                                                                                            quest:PauseAllNonScriptedEntities(true)
                                                                                                            x_stk_30 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = x_stk_30
                                                                                                            fret_00 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fret_00 <= fVar2 then
                                                                                                                -- LAB_00d551d4: (native jump target)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then
                                                                                                                        -- LAB_00d55c9f_c16: (native jump target)
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        goto LAB_00d55c2b
                                                                                                                    end
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if iVar7 == 1 then
                                                                                                                        if bVar3 then
                                                                                                                            -- LAB_00d55c9f: (native jump target)
                                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                                            goto LAB_00d55c2b
                                                                                                                        end
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                    else
                                                                                                                        if bVar3 then goto LAB_00d55cba end
                                                                                                                        x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_58
                                                                                                                        fret_01 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_01 then
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r11 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then
                                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                                    goto LAB_00d55c2b
                                                                                                                                end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then goto LAB_00d55cba end
                                                                                                                        end
                                                                                                                        pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 1.0
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    goto LAB_00d55480
                                                                                                                end
                                                                                                            else
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r12 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then
                                                                                                                        -- LAB_00d55c9f_c18: (native jump target)
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        goto LAB_00d55c2b
                                                                                                                    end
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then
                                                                                                                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                    while iVar7 < 0 do
                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then __region_LAB_00d55c9f_c19(); goto LAB_00d55c2b end
                                                                                                                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if not bVar3 then
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if iVar7 == 1 then
                                                                                                                            if bVar3 then
                                                                                                                                __region_LAB_00d55c9f_c19()
                                                                                                                                goto LAB_00d55c2b
                                                                                                                            end
                                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                                            quest:Pause(1.0)
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                        else
                                                                                                                            if bVar3 then goto LAB_00d55cba end
                                                                                                                            x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                            pCVar5 = x_stk_58
                                                                                                                            fret_01 = quest:GetHealth(pCVar5)
                                                                                                                            fVar2 = 0.0
                                                                                                                            if fVar2 < fret_01 then
                                                                                                                                iVar23 = 1
                                                                                                                                iVar8 = 0
                                                                                                                                iVar7 = 0
                                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                                r13 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                                while cVar4 do
                                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                                    bVar3 = not alive
                                                                                                                                    if bVar3 then __region_LAB_00d55c9f_c19(); goto LAB_00d55c2b end
                                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                                    cVar4 = iVar7
                                                                                                                                end
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then goto LAB_00d55cba end
                                                                                                                            end
                                                                                                                            pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                            iVar23 = 0
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 1.0
                                                                                                                            pCVar12 = pCVar5:GetPos()
                                                                                                                            me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                        end
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        goto LAB_00d55480
                                                                                                                    end
                                                                                                                    goto FLOW_after_lab_00d551d4
                                                                                                                end
                                                                                                            end
                                                                                                            ::FLOW_after_lab_00d551d4::
                                                                                                            ::LAB_00d55cba::
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                    end
                                                                                                    ::FLOW_after_lab_00d54f9c::
                                                                                                    ::LAB_00d55480::
                                                                                                    cVar4 = me:IsTalkedToByHero()
                                                                                                    if cVar4 then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b end
                                                                                                        xStack_f8 = resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        pCVar6 = 0x1
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        me:ClearCommands()
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if not bVar3 then
                                                                                                                x_stk_c = resources:ScriptThing(xStack_17c)
                                                                                                                pCVar5 = x_stk_c
                                                                                                                fret_03 = quest:GetHealth(pCVar5)
                                                                                                                fVar2 = 0.0
                                                                                                                if fVar2 < fret_03 then
                                                                                                                    iVar23 = 1
                                                                                                                    iVar8 = 0
                                                                                                                    iVar7 = 0
                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    r14 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                    while cVar4 do
                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then
                                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                                            goto LAB_00d55c2b
                                                                                                                        end
                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                        cVar4 = iVar7
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55cd5 end
                                                                                                                end
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        goto LAB_00d55c2b
                                                                                                                    end
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if iVar7 == 1 then
                                                                                                                        if not bVar3 then
                                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                                            quest:Pause(1.0)
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            goto LAB_00d5595a
                                                                                                                        end
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        goto LAB_00d55c2b
                                                                                                                    end
                                                                                                                    if not bVar3 then
                                                                                                                        x_stk_48 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_48
                                                                                                                        fret_04 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_04 then
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r15 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then
                                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                                    goto LAB_00d55c2b
                                                                                                                                end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then goto LAB_00d55cd5 end
                                                                                                                        end
                                                                                                                        pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 1.0
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                        goto LAB_00d5595a
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            ::LAB_00d55cd5::
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d555f3: (native jump target)
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        x_stk_24 = resources:ScriptThing(xStack_17c)
                                                                                                        pCVar5 = x_stk_24
                                                                                                        fret_02 = quest:GetHealth(pCVar5)
                                                                                                        fVar2 = 0.0
                                                                                                        if fVar2 < fret_02 then
                                                                                                            iVar23 = 1
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 0
                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD"
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            r16 = me:Speak(pCVar6, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            cVar4 = iVar7
                                                                                                            while cVar4 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                        end
                                                                                                        ::LAB_00d5595a::
                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                    end
                                                                                                    if c_stk_169 ~= 0 then
                                                                                                        fVar20 = 5.5
                                                                                                        pCVar5 = quest:GetHero()
                                                                                                        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar20)
                                                                                                        __native_condition_5 = bVar3
                                                                                                        if __native_condition_5 then
                                                                                                            iVar7 = quest:GetTimer(xStack_188)
                                                                                                            __native_condition_5 = iVar7 < 1
                                                                                                        end
                                                                                                        __native_condition_4 = __native_condition_5
                                                                                                        if __native_condition_4 then
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            __native_condition_4 = not iVar7
                                                                                                        end
                                                                                                        if __native_condition_4 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b end
                                                                                                            iVar8 = quest:AddNewConversation(me, false, false)
                                                                                                            pCVar5 = quest:GetHero()
                                                                                                            quest:AddPersonToConversation(iVar8, pCVar5)
                                                                                                            quest:SetTimer(xStack_188, 10)
                                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b end
                                                                                                                if ctr_154 == 0x1 then
                                                                                                                    bVar3 = false
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                                    -- LAB_00d55b4e: (native jump target)
                                                                                                                else
                                                                                                                    if ctr_154 == 0x2 then
                                                                                                                        bVar3 = false
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, pCVar5, false)
                                                                                                                        goto FLOW_after_lab_00d55b4e
                                                                                                                    end
                                                                                                                end
                                                                                                                ::FLOW_after_lab_00d55b4e::
                                                                                                                ctr_154 = 1 - ctr_154
                                                                                                            else
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b end
                                                                                                                bVar3 = false
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                            end
                                                                                                        end
                                                                                                    end
                                                                                                    if c_stk_161 == 0 then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b end
                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                        if not iVar7 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b end
                                                                                                            bVar3 = false
                                                                                                            c_stk_161 = 1
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                                                                                                        end
                                                                                                    end
                                                                                                until not (c_stk_169 ~= 0)
                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                bVar3 = not alive
                                                                                                if not bVar3 then
                                                                                                    quest:SetStateBool("HeroSleeps", true)
                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                    quest:SetTimeOfDay(11.0)
                                                                                                    quest:ChangeHeroHealthBy(1000.0, true, false)
                                                                                                    quest:ResetPlayerCreatureCombatMultiplier()
                                                                                                end
                                                                                            end
                                                                                            ::FLOW_after_lab_00d54846::
                                                                                        end
                                                                                    end
                                                                                end
                                                                            end
                                                                        end
                                                                    end
                                                                    ::FLOW_after_lab_00d54dfa::
                                                                end
                                                                ::LAB_00d55c2b::
                                                                quest:DeregisterTimer(xStack_180)
                                                            end
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if not bVar3 then
                                                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_PC")
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            while not bVar3 do
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if bVar3 then goto LAB_00d55c34 end
                                                                bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                                                xStack_180 = quest:RegisterTimer()
                                                                quest:SetTimer(xStack_180, 10)
                                                                iVar7 = (xStack_160 ~= nil and xStack_160:IsAlive())
                                                                cVar4 = iVar7
                                                                while cVar4 do
                                                                    alive = quest:NewScriptFrame(me)
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00d55c2b_c27 end
                                                                    iVar7 = quest:GetTimer(xStack_180)
                                                                    if iVar7 < 1 then
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then goto LAB_00d55c2b_c27 end
                                                                        iVar8 = quest:AddNewConversation(me, false, false)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddPersonToConversation(iVar8, pCVar6)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, pCVar6, false)
                                                                        quest:SetTimer(xStack_180, 10)
                                                                    end
                                                                    iVar7 = (xStack_160 ~= nil and xStack_160:IsAlive())
                                                                    cVar4 = iVar7
                                                                end
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if not bVar3 then
                                                                    quest:Pause(0.5)
                                                                    xStack_14c = resources:NewResource()
                                                                    bVar3 = false
                                                                    if bVar3 ~= 0 then
                                                                    end
                                                                    iVar8 = 4
                                                                    pCVar6 = quest:GetHero()
                                                                    bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                                    while not bVar3 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then return end  -- TODO(native): goto LAB_00d54dfa_c27
                                                                        iVar8 = 4
                                                                        pCVar6 = quest:GetHero()
                                                                        bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                                    end
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then
                                                                        -- LAB_00d54dfa_c27: (native jump target)
                                                                    else
                                                                        pCVar5 = resources:NewActorMap()
                                                                        resources:SetActor(pCVar5, "HERO", xStack_14c)
                                                                        resources:SetActor(pCVar5, "TEACHER", xStack_17c)
                                                                        xStack_124 = resources:StartMovie("")
                                                                        quest:StartMovieSequence()
                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                        quest:FixMovieSequenceCamera(true)
                                                                        resources:RunMacro("CS_GUILD_PREMELEE_ALARM", pCVar5, false, true)
                                                                        quest:FixMovieSequenceCamera(false)
                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                        resources:DestroyActorMap(pCVar5)
                                                                        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", "Q_GuildTrainingWoodsMelee", false)
                                                                        quest:SetQuestCardObjective("Q_GuildTrainingWoodsMelee", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                        quest:KickOffQuestStartScreen("Q_GuildTrainingWoodsMelee", true, false)
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if not bVar3 then
                                                                            alive = quest:NewScriptFrame(me)
                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                            bVar3 = not alive
                                                                            if not bVar3 then
                                                                                alive = quest:NewScriptFrame(me)
                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                bVar3 = not alive
                                                                                if not bVar3 then
                                                                                    alive = quest:NewScriptFrame(me)
                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                    bVar3 = not alive
                                                                                    if not bVar3 then
                                                                                        alive = quest:NewScriptFrame(me)
                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                        bVar3 = not alive
                                                                                        if not bVar3 then
                                                                                            bVar3 = quest:DisplayTutorial(0x1c)
                                                                                            if bVar3 then
                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                bVar3 = not alive
                                                                                                if not bVar3 then
                                                                                                    bVar3 = quest:MsgIsTutorialClickedPast()
                                                                                                    while not bVar3 do
                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                        bVar3 = quest:MsgIsTutorialClickedPast()
                                                                                                    end
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    bVar3 = not alive
                                                                                                    if not bVar3 then return end  -- TODO(native): goto LAB_00d54846_c27
                                                                                                end
                                                                                            else
                                                                                                -- LAB_00d54846_c27: (native jump target)
                                                                                                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                quest:MiniMapRemoveMarker(pCVar6)
                                                                                                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                quest:MiniMapAddMarker(pCVar6, "HUD_ORB_GREEN_SMALL")
                                                                                                pCVar6 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                iVar23 = 1
                                                                                                iVar8 = 0
                                                                                                iVar7 = 1.0
                                                                                                pCVar12 = pCVar6:GetPos()
                                                                                                me:MoveToPosition(pCVar12, iVar7, iVar8, (xStack_14c ~= 0), (iVar23 ~= 0))
                                                                                                c_stk_169 = 1
                                                                                                ctr_154 = 0
                                                                                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                                c_stk_161 = 0
                                                                                                repeat
                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    bVar3 = not alive
                                                                                                    if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c27
                                                                                                    if not __native_entity_state:GetStateBool("WoodsEndPlayed") then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                        __native_entity_state:SetStateBool("WoodsEndPlayed", true)
                                                                                                        pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                        quest:MiniMapRemoveMarker(pCVar6)
                                                                                                        pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                        quest:MiniMapAddMarker(pCVar6, "HUD_ORB_QUEST_CORE")
                                                                                                        xStack_108 = resources:NewResource()
                                                                                                        bVar3 = false
                                                                                                        if bVar3 ~= 0 then
                                                                                                        end
                                                                                                        iVar8 = 4
                                                                                                        pCVar6 = quest:GetHero()
                                                                                                        bVar3 = resources:TryAcquire(xStack_108, pCVar6, iVar8)
                                                                                                        while not bVar3 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c91_c27
                                                                                                            iVar8 = 4
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            bVar3 = resources:TryAcquire(xStack_108, pCVar6, iVar8)
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d55c91_c27: (native jump target)
                                                                                                            goto LAB_00d55c2b_c27
                                                                                                        end
                                                                                                        xStack_13c = resources:NewActorMap()
                                                                                                        resources:SetActor(xStack_13c, "HERO", xStack_108)
                                                                                                        resources:SetActor(xStack_13c, "GUARD", xStack_17c)
                                                                                                        xStack_124 = resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        pCVar6 = 0x1
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        quest:FixMovieSequenceCamera(true)
                                                                                                        resources:RunMacro("CS_GUILD_MELEE_WOODSWON", xStack_13c, false, true)
                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        while iVar7 < 0 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d55c72_c27: (native jump target)
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            -- LAB_00d55c7f_c27: (native jump target)
                                                                                                            resources:DestroyActorMap(xStack_13c)
                                                                                                            -- TODO(native): goto LAB_00d55c91_c27
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if iVar7 == 1 then
                                                                                                            if bVar3 then
                                                                                                                -- LAB_00d55c63_c27: (native jump target)
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                -- TODO(native): goto LAB_00d55c7f_c27
                                                                                                            end
                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                            quest:Pause(1.0)
                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                        else
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                            r1 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = r1
                                                                                                            fret_0 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fVar2 < fret_0 then
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r17 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                            end
                                                                                                            pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                            iVar23 = 0
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 1.0
                                                                                                            pCVar12 = pCVar5:GetPos()
                                                                                                            me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                        end
                                                                                                        quest:FixMovieSequenceCamera(false)
                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                        resources:DestroyActorMap(xStack_13c)
                                                                                                    else
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            -- LAB_00d54f9c_c27: (native jump target)
                                                                                                            bVar3 = false
                                                                                                        else
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if not bVar3 then return end  -- TODO(native): goto LAB_00d54f9c_c27
                                                                                                            bVar3 = true
                                                                                                        end
                                                                                                        if bVar3 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                            bVar3 = false
                                                                                                            if bVar3 ~= 0 then
                                                                                                            end
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if bVar3 then
                                                                                                                repeat
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                                    bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                                until not (bVar3)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                            bVar3 = false
                                                                                                            if bVar3 ~= 0 then
                                                                                                            end
                                                                                                            bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                            while not bVar3 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                                bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                            xStack_14c = resources:StartMovie("")
                                                                                                            quest:StartMovieSequence()
                                                                                                            pCVar6 = 0x1
                                                                                                            quest:PauseAllNonScriptedEntities(true)
                                                                                                            x_stk_30 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = x_stk_30
                                                                                                            fret_00 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fret_00 <= fVar2 then
                                                                                                                -- LAB_00d551d4_c27: (native jump target)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if iVar7 == 1 then
                                                                                                                        if bVar3 then
                                                                                                                            __region_LAB_00d55c9f_c27()
                                                                                                                            goto LAB_00d55c2b_c27
                                                                                                                        end
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                    else
                                                                                                                        if bVar3 then goto LAB_00d55cba_c27 end
                                                                                                                        x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_58
                                                                                                                        fret_01 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_01 then
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r18 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then goto LAB_00d55cba_c27 end
                                                                                                                        end
                                                                                                                        pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 1.0
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    goto LAB_00d55480_c27
                                                                                                                end
                                                                                                            else
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r19 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55c9f_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then return end  -- TODO(native): goto LAB_00d551d4_c27
                                                                                                            end
                                                                                                            ::LAB_00d55cba_c27::
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            goto LAB_00d55c2b_c27
                                                                                                        end
                                                                                                    end
                                                                                                    ::LAB_00d55480_c27::
                                                                                                    cVar4 = me:IsTalkedToByHero()
                                                                                                    if cVar4 then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                        xStack_f8 = resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        pCVar6 = 0x1
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        me:ClearCommands()
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if not bVar3 then
                                                                                                                x_stk_c = resources:ScriptThing(xStack_17c)
                                                                                                                pCVar5 = x_stk_c
                                                                                                                fret_03 = quest:GetHealth(pCVar5)
                                                                                                                fVar2 = 0.0
                                                                                                                if fVar2 < fret_03 then
                                                                                                                    iVar23 = 1
                                                                                                                    iVar8 = 0
                                                                                                                    iVar7 = 0
                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    r20 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                    while cVar4 do
                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                        cVar4 = iVar7
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                end
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if iVar7 == 1 then
                                                                                                                        if not bVar3 then
                                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                                            quest:Pause(1.0)
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            goto LAB_00d5595a_c27
                                                                                                                        end
                                                                                                                        __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27
                                                                                                                    end
                                                                                                                    if not bVar3 then
                                                                                                                        x_stk_48 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_48
                                                                                                                        fret_04 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_04 then
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r21 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                        end
                                                                                                                        pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 1.0
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                        goto LAB_00d5595a_c27
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            __region_LAB_00d55cd5_c27()
                                                                                                            goto LAB_00d55c2b_c27
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            __region_LAB_00d555f3_c27()
                                                                                                            goto LAB_00d55c2b_c27
                                                                                                        end
                                                                                                        x_stk_24 = resources:ScriptThing(xStack_17c)
                                                                                                        pCVar5 = x_stk_24
                                                                                                        fret_02 = quest:GetHealth(pCVar5)
                                                                                                        fVar2 = 0.0
                                                                                                        if fVar2 < fret_02 then
                                                                                                            iVar23 = 1
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 0
                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD"
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            r22 = me:Speak(pCVar6, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            cVar4 = iVar7
                                                                                                            while cVar4 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then __region_LAB_00d55cd5_c27(); goto LAB_00d55c2b_c27 end
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27 end
                                                                                                        end
                                                                                                        ::LAB_00d5595a_c27::
                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                    end
                                                                                                    if c_stk_169 ~= 0 then
                                                                                                        fVar20 = 5.5
                                                                                                        pCVar5 = quest:GetHero()
                                                                                                        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar20)
                                                                                                        __native_condition_7 = bVar3
                                                                                                        if __native_condition_7 then
                                                                                                            iVar7 = quest:GetTimer(xStack_188)
                                                                                                            __native_condition_7 = iVar7 < 1
                                                                                                        end
                                                                                                        __native_condition_6 = __native_condition_7
                                                                                                        if __native_condition_6 then
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            __native_condition_6 = not iVar7
                                                                                                        end
                                                                                                        if __native_condition_6 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                            iVar8 = quest:AddNewConversation(me, false, false)
                                                                                                            pCVar5 = quest:GetHero()
                                                                                                            quest:AddPersonToConversation(iVar8, pCVar5)
                                                                                                            quest:SetTimer(xStack_188, 10)
                                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                                if ctr_154 == 0x1 then
                                                                                                                    bVar3 = false
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                                    -- LAB_00d55b4e_c27: (native jump target)
                                                                                                                else
                                                                                                                    if ctr_154 == 0x2 then
                                                                                                                        bVar3 = false
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, pCVar5, false)
                                                                                                                        -- TODO(native): goto LAB_00d55b4e_c27
                                                                                                                    end
                                                                                                                end
                                                                                                                ctr_154 = 1 - ctr_154
                                                                                                            else
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                                bVar3 = false
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                            end
                                                                                                        end
                                                                                                    end
                                                                                                    if c_stk_161 == 0 then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                        if not iVar7 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                            bVar3 = false
                                                                                                            c_stk_161 = 1
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                                                                                                        end
                                                                                                    end
                                                                                                until not (c_stk_169 ~= 0)
                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                bVar3 = not alive
                                                                                                if not bVar3 then
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
                                                                quest:DeregisterTimer(xStack_180)
                                                                goto FLOW_after_lab_00d5439e
                                                            end
                                                        end
                                                    end
                                                    ::FLOW_after_lab_00d5439e::
                                                end
                                                ::FLOW_after_lab_00d53ff2::
                                                ::LAB_00d55c34::
                                            end
                                        end
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if not bVar3 then
                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK_PC")
                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                        while not bVar3 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00d55c3d end
                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if not bVar3 then
                                            -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                            quest:SetStateInt("PreMeleeMode", 2)
                                            quest:SetStateInt("DummyHits", 0)
                                            quest:SetTimer(timerId, 10)
                                            CVar10 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                                            xStack_130 = CVar10
                                            quest:DisplayQuestInfo(true)
                                            iVar7 = quest:GetStateInt("DummyHits")
                                            while iVar7 < 7 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then goto LAB_00d55c3d end
                                                quest:UpdateQuestInfoCounter(CVar10, quest:GetStateInt("DummyHits"), -1)
                                                if 0x0 ~= quest:GetStateInt("DummyHits") then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d55c3d end
                                                    -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                                    quest:SetTimer(timerId, 10)
                                                end
                                                iVar7 = quest:GetTimer(timerId)
                                                if iVar7 < 1 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d55c3d end
                                                    iVar8 = quest:AddNewConversation(me, false, false)
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddPersonToConversation(iVar8, pCVar6)
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, pCVar6, false)
                                                    bVar3 = quest:IsXbox()
                                                    if bVar3 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then goto LAB_00d55c3d end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        while not bVar3 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then goto LAB_00d55c3d end
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then goto LAB_00d55c3d end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        while not bVar3 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then goto LAB_00d55c3d end
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d55c3d end
                                                    quest:SetTimer(xStack_184, 10)
                                                    timerId = xStack_184
                                                    CVar10 = xStack_130
                                                end
                                                iVar7 = quest:GetStateInt("DummyHits")
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if not bVar3 then
                                                quest:RemoveQuestInfoElement(CVar10)
                                                quest:DisplayQuestInfo(false)
                                                xStack_160 = nil
                                                xStack_124 = resources:StartMovie("")
                                                quest:StartMovieSequence()
                                                xStack_14c = resources:NewResource()
                                                bVar3 = false
                                                if bVar3 ~= 0 then
                                                end
                                                iVar8 = 4
                                                pCVar6 = quest:GetHero()
                                                bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                while not bVar3 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d53ff2_c28
                                                    iVar8 = 4
                                                    pCVar6 = quest:GetHero()
                                                    bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then
                                                    -- LAB_00d53ff2_c28: (native jump target)
                                                    resources:DestroyMovie(xStack_14c)
                                                    resources:DestroyMovie(xStack_124)
                                                else
                                                    r23 = quest:GetThingWithScriptName("PreMeleeDummy")
                                                    if not (r23 ~= nil and not r23:IsNull()) then
                                                    else
                                                        puVar11 = r23:GetPos()
                                                    end
                                                    -- TODO(native): CStack_114._0_4_ = *puVar11;
                                                    -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                                    -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                                    -- TODO(native): CreateEffect is not a ForgeFSE binding
                                                    quest:CreateEffect(pCVar6, "SMASH_DUMMY_01", pCVar5, "", 0.0, false, false)
                                                    quest:FadeOutAndKillEntity(r23, true, 1.0, true)
                                                    xStack_13c = resources:NewActorMap()
                                                    resources:SetActor(xStack_13c, "HERO", xStack_14c)
                                                    resources:SetActor(xStack_13c, "TEACHER", xStack_17c)
                                                    quest:FixMovieSequenceCamera(true)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", xStack_13c, false, false)
                                                    quest:PauseAllNonScriptedEntities(true)
                                                    pCVar6 = quest:CreateExperienceOrb(1, pCVar5)
                                                    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                                    quest:EntitySetCutsceneBehaviour(xStack_160, 2)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED", xStack_13c, false, true)
                                                    quest:FixMovieSequenceCamera(false)
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    resources:DestroyActorMap(xStack_13c)
                                                    resources:DestroyMovie(xStack_14c)
                                                    resources:DestroyMovie(xStack_124)
                                                    bVar3 = quest:IsXbox()
                                                    if bVar3 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if not bVar3 then
                                                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            while not bVar3 do
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if bVar3 then goto LAB_00d55c34_c28 end
                                                                bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                -- LAB_00d5439e_c28: (native jump target)
                                                                xStack_180 = quest:RegisterTimer()
                                                                quest:SetTimer(xStack_180, 10)
                                                                iVar7 = (xStack_160 ~= nil and xStack_160:IsAlive())
                                                                cVar4 = iVar7
                                                                while cVar4 do
                                                                    alive = quest:NewScriptFrame(me)
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00d55c2b_c28 end
                                                                    iVar7 = quest:GetTimer(xStack_180)
                                                                    if iVar7 < 1 then
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then goto LAB_00d55c2b_c28 end
                                                                        iVar8 = quest:AddNewConversation(me, false, false)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddPersonToConversation(iVar8, pCVar6)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, pCVar6, false)
                                                                        quest:SetTimer(xStack_180, 10)
                                                                    end
                                                                    iVar7 = (xStack_160 ~= nil and xStack_160:IsAlive())
                                                                    cVar4 = iVar7
                                                                end
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if not bVar3 then
                                                                    quest:Pause(0.5)
                                                                    xStack_14c = resources:NewResource()
                                                                    bVar3 = false
                                                                    if bVar3 ~= 0 then
                                                                    end
                                                                    iVar8 = 4
                                                                    pCVar6 = quest:GetHero()
                                                                    bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                                    while not bVar3 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then return end  -- TODO(native): goto LAB_00d54dfa_c28
                                                                        iVar8 = 4
                                                                        pCVar6 = quest:GetHero()
                                                                        bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                                    end
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then
                                                                        -- LAB_00d54dfa_c28: (native jump target)
                                                                    else
                                                                        pCVar5 = resources:NewActorMap()
                                                                        resources:SetActor(pCVar5, "HERO", xStack_14c)
                                                                        resources:SetActor(pCVar5, "TEACHER", xStack_17c)
                                                                        xStack_124 = resources:StartMovie("")
                                                                        quest:StartMovieSequence()
                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                        quest:FixMovieSequenceCamera(true)
                                                                        resources:RunMacro("CS_GUILD_PREMELEE_ALARM", pCVar5, false, true)
                                                                        quest:FixMovieSequenceCamera(false)
                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                        resources:DestroyActorMap(pCVar5)
                                                                        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", "Q_GuildTrainingWoodsMelee", false)
                                                                        quest:SetQuestCardObjective("Q_GuildTrainingWoodsMelee", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                        quest:KickOffQuestStartScreen("Q_GuildTrainingWoodsMelee", true, false)
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if not bVar3 then
                                                                            alive = quest:NewScriptFrame(me)
                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                            bVar3 = not alive
                                                                            if not bVar3 then
                                                                                alive = quest:NewScriptFrame(me)
                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                bVar3 = not alive
                                                                                if not bVar3 then
                                                                                    alive = quest:NewScriptFrame(me)
                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                    bVar3 = not alive
                                                                                    if not bVar3 then
                                                                                        alive = quest:NewScriptFrame(me)
                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                        bVar3 = not alive
                                                                                        if not bVar3 then
                                                                                            bVar3 = quest:DisplayTutorial(0x1c)
                                                                                            if bVar3 then
                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                bVar3 = not alive
                                                                                                if not bVar3 then
                                                                                                    bVar3 = quest:MsgIsTutorialClickedPast()
                                                                                                    while not bVar3 do
                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                        bVar3 = quest:MsgIsTutorialClickedPast()
                                                                                                    end
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    bVar3 = not alive
                                                                                                    if not bVar3 then return end  -- TODO(native): goto LAB_00d54846_c28
                                                                                                end
                                                                                            else
                                                                                                -- LAB_00d54846_c28: (native jump target)
                                                                                                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                quest:MiniMapRemoveMarker(pCVar6)
                                                                                                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                quest:MiniMapAddMarker(pCVar6, "HUD_ORB_GREEN_SMALL")
                                                                                                pCVar6 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                iVar23 = 1
                                                                                                iVar8 = 0
                                                                                                iVar7 = 1.0
                                                                                                pCVar12 = pCVar6:GetPos()
                                                                                                me:MoveToPosition(pCVar12, iVar7, iVar8, (xStack_14c ~= 0), (iVar23 ~= 0))
                                                                                                c_stk_169 = 1
                                                                                                ctr_154 = 0
                                                                                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                                c_stk_161 = 0
                                                                                                repeat
                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    bVar3 = not alive
                                                                                                    if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c28
                                                                                                    if not __native_entity_state:GetStateBool("WoodsEndPlayed") then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                        __native_entity_state:SetStateBool("WoodsEndPlayed", true)
                                                                                                        pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                        quest:MiniMapRemoveMarker(pCVar6)
                                                                                                        pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                        quest:MiniMapAddMarker(pCVar6, "HUD_ORB_QUEST_CORE")
                                                                                                        xStack_108 = resources:NewResource()
                                                                                                        bVar3 = false
                                                                                                        if bVar3 ~= 0 then
                                                                                                        end
                                                                                                        iVar8 = 4
                                                                                                        pCVar6 = quest:GetHero()
                                                                                                        bVar3 = resources:TryAcquire(xStack_108, pCVar6, iVar8)
                                                                                                        while not bVar3 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c91_c28
                                                                                                            iVar8 = 4
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            bVar3 = resources:TryAcquire(xStack_108, pCVar6, iVar8)
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d55c91_c28: (native jump target)
                                                                                                            goto LAB_00d55c2b_c28
                                                                                                        end
                                                                                                        xStack_13c = resources:NewActorMap()
                                                                                                        resources:SetActor(xStack_13c, "HERO", xStack_108)
                                                                                                        resources:SetActor(xStack_13c, "GUARD", xStack_17c)
                                                                                                        xStack_124 = resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        pCVar6 = 0x1
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        quest:FixMovieSequenceCamera(true)
                                                                                                        resources:RunMacro("CS_GUILD_MELEE_WOODSWON", xStack_13c, false, true)
                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        while iVar7 < 0 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d55c72_c28: (native jump target)
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            -- LAB_00d55c7f_c28: (native jump target)
                                                                                                            resources:DestroyActorMap(xStack_13c)
                                                                                                            -- TODO(native): goto LAB_00d55c91_c28
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if iVar7 == 1 then
                                                                                                            if bVar3 then
                                                                                                                -- LAB_00d55c63_c28: (native jump target)
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                -- TODO(native): goto LAB_00d55c7f_c28
                                                                                                            end
                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                            quest:Pause(1.0)
                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                        else
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                            r23 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = r23
                                                                                                            fret_0 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fVar2 < fret_0 then
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r24 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                            end
                                                                                                            pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                            iVar23 = 0
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 1.0
                                                                                                            pCVar12 = pCVar5:GetPos()
                                                                                                            me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                        end
                                                                                                        quest:FixMovieSequenceCamera(false)
                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                        resources:DestroyActorMap(xStack_13c)
                                                                                                    else
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            -- LAB_00d54f9c_c28: (native jump target)
                                                                                                            bVar3 = false
                                                                                                        else
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if not bVar3 then return end  -- TODO(native): goto LAB_00d54f9c_c28
                                                                                                            bVar3 = true
                                                                                                        end
                                                                                                        if bVar3 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                            bVar3 = false
                                                                                                            if bVar3 ~= 0 then
                                                                                                            end
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if bVar3 then
                                                                                                                repeat
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                                    bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                                until not (bVar3)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                            bVar3 = false
                                                                                                            if bVar3 ~= 0 then
                                                                                                            end
                                                                                                            bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                            while not bVar3 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                                bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                            xStack_14c = resources:StartMovie("")
                                                                                                            quest:StartMovieSequence()
                                                                                                            pCVar6 = 0x1
                                                                                                            quest:PauseAllNonScriptedEntities(true)
                                                                                                            x_stk_30 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = x_stk_30
                                                                                                            fret_00 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fret_00 <= fVar2 then
                                                                                                                -- LAB_00d551d4_c28: (native jump target)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if iVar7 == 1 then
                                                                                                                        if bVar3 then
                                                                                                                            __region_LAB_00d55c9f_c28()
                                                                                                                            goto LAB_00d55c2b_c28
                                                                                                                        end
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                    else
                                                                                                                        if bVar3 then goto LAB_00d55cba_c28 end
                                                                                                                        x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_58
                                                                                                                        fret_01 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_01 then
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r25 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then goto LAB_00d55cba_c28 end
                                                                                                                        end
                                                                                                                        pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 1.0
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    goto LAB_00d55480_c28
                                                                                                                end
                                                                                                            else
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r26 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55c9f_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then return end  -- TODO(native): goto LAB_00d551d4_c28
                                                                                                            end
                                                                                                            ::LAB_00d55cba_c28::
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            goto LAB_00d55c2b_c28
                                                                                                        end
                                                                                                    end
                                                                                                    ::LAB_00d55480_c28::
                                                                                                    cVar4 = me:IsTalkedToByHero()
                                                                                                    if cVar4 then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                        xStack_f8 = resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        pCVar6 = 0x1
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        me:ClearCommands()
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if not bVar3 then
                                                                                                                x_stk_c = resources:ScriptThing(xStack_17c)
                                                                                                                pCVar5 = x_stk_c
                                                                                                                fret_03 = quest:GetHealth(pCVar5)
                                                                                                                fVar2 = 0.0
                                                                                                                if fVar2 < fret_03 then
                                                                                                                    iVar23 = 1
                                                                                                                    iVar8 = 0
                                                                                                                    iVar7 = 0
                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    r27 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                    while cVar4 do
                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                        cVar4 = iVar7
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                end
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if iVar7 == 1 then
                                                                                                                        if not bVar3 then
                                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                                            quest:Pause(1.0)
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            goto LAB_00d5595a_c28
                                                                                                                        end
                                                                                                                        __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28
                                                                                                                    end
                                                                                                                    if not bVar3 then
                                                                                                                        x_stk_48 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_48
                                                                                                                        fret_04 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_04 then
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r28 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                        end
                                                                                                                        pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 1.0
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                        goto LAB_00d5595a_c28
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            __region_LAB_00d55cd5_c28()
                                                                                                            goto LAB_00d55c2b_c28
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            __region_LAB_00d555f3_c28()
                                                                                                            goto LAB_00d55c2b_c28
                                                                                                        end
                                                                                                        x_stk_24 = resources:ScriptThing(xStack_17c)
                                                                                                        pCVar5 = x_stk_24
                                                                                                        fret_02 = quest:GetHealth(pCVar5)
                                                                                                        fVar2 = 0.0
                                                                                                        if fVar2 < fret_02 then
                                                                                                            iVar23 = 1
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 0
                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD"
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            r29 = me:Speak(pCVar6, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            cVar4 = iVar7
                                                                                                            while cVar4 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then __region_LAB_00d55cd5_c28(); goto LAB_00d55c2b_c28 end
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28 end
                                                                                                        end
                                                                                                        ::LAB_00d5595a_c28::
                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                    end
                                                                                                    if c_stk_169 ~= 0 then
                                                                                                        fVar20 = 5.5
                                                                                                        pCVar5 = quest:GetHero()
                                                                                                        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar20)
                                                                                                        __native_condition_9 = bVar3
                                                                                                        if __native_condition_9 then
                                                                                                            iVar7 = quest:GetTimer(xStack_188)
                                                                                                            __native_condition_9 = iVar7 < 1
                                                                                                        end
                                                                                                        __native_condition_8 = __native_condition_9
                                                                                                        if __native_condition_8 then
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            __native_condition_8 = not iVar7
                                                                                                        end
                                                                                                        if __native_condition_8 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                            iVar8 = quest:AddNewConversation(me, false, false)
                                                                                                            pCVar5 = quest:GetHero()
                                                                                                            quest:AddPersonToConversation(iVar8, pCVar5)
                                                                                                            quest:SetTimer(xStack_188, 10)
                                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                                if ctr_154 == 0x1 then
                                                                                                                    bVar3 = false
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                                    -- LAB_00d55b4e_c28: (native jump target)
                                                                                                                else
                                                                                                                    if ctr_154 == 0x2 then
                                                                                                                        bVar3 = false
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, pCVar5, false)
                                                                                                                        -- TODO(native): goto LAB_00d55b4e_c28
                                                                                                                    end
                                                                                                                end
                                                                                                                ctr_154 = 1 - ctr_154
                                                                                                            else
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                                bVar3 = false
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                            end
                                                                                                        end
                                                                                                    end
                                                                                                    if c_stk_161 == 0 then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                        if not iVar7 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                            bVar3 = false
                                                                                                            c_stk_161 = 1
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                                                                                                        end
                                                                                                    end
                                                                                                until not (c_stk_169 ~= 0)
                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                bVar3 = not alive
                                                                                                if not bVar3 then
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
                                                                quest:DeregisterTimer(xStack_180)
                                                            end
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if not bVar3 then
                                                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_PC")
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            while not bVar3 do
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if bVar3 then goto LAB_00d55c34_c28 end
                                                                bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                                                -- TODO(native): goto LAB_00d5439e_c28
                                                            end
                                                        end
                                                    end
                                                end
                                                ::LAB_00d55c34_c28::
                                            end
                                            goto FLOW_after_lab_00d53c7e
                                        end
                                    end
                                end
                                ::FLOW_after_lab_00d53c7e::
                            end
                            ::FLOW_after_lab_00d53a0b::
                        end
                        ::LAB_00d55c3d::
                        quest:DeregisterTimer(xStack_184)
                    end
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH_PC")
                    bVar3 = quest:MsgIsGameInfoClickedPast()
                    while not bVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d55c46 end
                        bVar3 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                        -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                        xStack_184 = quest:RegisterTimer()
                        timerId = xStack_184
                        quest:SetTimer(xStack_184, 10)
                        quest:SetStateInt("PreMeleeMode", 1)
                        quest:SetStateInt("DummyHits", 0)
                        CVar10 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                        xStack_130 = CVar10
                        quest:DisplayQuestInfo(true)
                        iVar7 = quest:GetStateInt("DummyHits")
                        while iVar7 < 7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d55c3d_c29 end
                            quest:UpdateQuestInfoCounter(CVar10, quest:GetStateInt("DummyHits"), -1)
                            if 0x0 ~= quest:GetStateInt("DummyHits") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d55c3d_c29 end
                                -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                quest:SetTimer(timerId, 10)
                            end
                            iVar7 = quest:GetTimer(timerId)
                            if iVar7 < 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d55c3d_c29 end
                                iVar8 = quest:AddNewConversation(me, false, false)
                                pCVar6 = quest:GetHero()
                                quest:AddPersonToConversation(iVar8, pCVar6)
                                pCVar6 = quest:GetHero()
                                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_HIT_DUMMY", me, pCVar6, false)
                                bVar3 = quest:IsXbox()
                                if bVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d55c3d_c29 end
                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP")
                                    bVar3 = quest:MsgIsGameInfoClickedPast()
                                    while not bVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d55c3d_c29 end
                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d55c3d_c29 end
                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP_PC")
                                    bVar3 = quest:MsgIsGameInfoClickedPast()
                                    while not bVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d55c3d_c29 end
                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                    end
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d55c3d_c29 end
                                quest:SetTimer(xStack_184, 10)
                                timerId = xStack_184
                                CVar10 = xStack_130
                            end
                            iVar7 = quest:GetStateInt("DummyHits")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            quest:RemoveQuestInfoElement(CVar10)
                            quest:DisplayQuestInfo(false)
                            xStack_14c = resources:NewResource()
                            bVar3 = false
                            if bVar3 ~= 0 then
                            end
                            iVar8 = 4
                            pCVar6 = quest:GetHero()
                            bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                            while not bVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then return end  -- TODO(native): goto LAB_00d53a0b_c29
                                iVar8 = 4
                                pCVar6 = quest:GetHero()
                                bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                -- LAB_00d53a0b_c29: (native jump target)
                                resources:DestroyMovie(xStack_14c)
                            else
                                xStack_13c = resources:NewActorMap()
                                resources:SetActor(xStack_13c, "HERO", xStack_14c)
                                resources:SetActor(xStack_13c, "TEACHER", xStack_17c)
                                xStack_124 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_PREMELEE_STICK", xStack_13c, false, true)
                                quest:FixMovieSequenceCamera(false)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_124)
                                resources:DestroyActorMap(xStack_13c)
                                resources:DestroyMovie(xStack_14c)
                                bVar3 = quest:IsXbox()
                                if bVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if not bVar3 then
                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK")
                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                        while not bVar3 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00d55c3d_c29 end
                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if not bVar3 then
                                            -- LAB_00d53c7e_c29: (native jump target)
                                            quest:SetStateInt("PreMeleeMode", 2)
                                            quest:SetStateInt("DummyHits", 0)
                                            quest:SetTimer(timerId, 10)
                                            CVar10 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                                            xStack_130 = CVar10
                                            quest:DisplayQuestInfo(true)
                                            iVar7 = quest:GetStateInt("DummyHits")
                                            while iVar7 < 7 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then goto LAB_00d55c3d_c29 end
                                                quest:UpdateQuestInfoCounter(CVar10, quest:GetStateInt("DummyHits"), -1)
                                                if 0x0 ~= quest:GetStateInt("DummyHits") then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d55c3d_c29 end
                                                    -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                                    quest:SetTimer(timerId, 10)
                                                end
                                                iVar7 = quest:GetTimer(timerId)
                                                if iVar7 < 1 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d55c3d_c29 end
                                                    iVar8 = quest:AddNewConversation(me, false, false)
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddPersonToConversation(iVar8, pCVar6)
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, pCVar6, false)
                                                    bVar3 = quest:IsXbox()
                                                    if bVar3 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then goto LAB_00d55c3d_c29 end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        while not bVar3 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then goto LAB_00d55c3d_c29 end
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then goto LAB_00d55c3d_c29 end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        while not bVar3 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then goto LAB_00d55c3d_c29 end
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d55c3d_c29 end
                                                    quest:SetTimer(xStack_184, 10)
                                                    timerId = xStack_184
                                                    CVar10 = xStack_130
                                                end
                                                iVar7 = quest:GetStateInt("DummyHits")
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if not bVar3 then
                                                quest:RemoveQuestInfoElement(CVar10)
                                                quest:DisplayQuestInfo(false)
                                                xStack_160 = nil
                                                xStack_124 = resources:StartMovie("")
                                                quest:StartMovieSequence()
                                                xStack_14c = resources:NewResource()
                                                bVar3 = false
                                                if bVar3 ~= 0 then
                                                end
                                                iVar8 = 4
                                                pCVar6 = quest:GetHero()
                                                bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                while not bVar3 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d53ff2_c29
                                                    iVar8 = 4
                                                    pCVar6 = quest:GetHero()
                                                    bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then
                                                    -- LAB_00d53ff2_c29: (native jump target)
                                                    resources:DestroyMovie(xStack_14c)
                                                    resources:DestroyMovie(xStack_124)
                                                else
                                                    r30 = quest:GetThingWithScriptName("PreMeleeDummy")
                                                    if not (r30 ~= nil and not r30:IsNull()) then
                                                    else
                                                        puVar11 = r30:GetPos()
                                                    end
                                                    -- TODO(native): CStack_114._0_4_ = *puVar11;
                                                    -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                                    -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                                    -- TODO(native): CreateEffect is not a ForgeFSE binding
                                                    quest:CreateEffect(pCVar6, "SMASH_DUMMY_01", pCVar5, "", 0.0, false, false)
                                                    quest:FadeOutAndKillEntity(r30, true, 1.0, true)
                                                    xStack_13c = resources:NewActorMap()
                                                    resources:SetActor(xStack_13c, "HERO", xStack_14c)
                                                    resources:SetActor(xStack_13c, "TEACHER", xStack_17c)
                                                    quest:FixMovieSequenceCamera(true)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", xStack_13c, false, false)
                                                    quest:PauseAllNonScriptedEntities(true)
                                                    pCVar6 = quest:CreateExperienceOrb(1, pCVar5)
                                                    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                                    quest:EntitySetCutsceneBehaviour(xStack_160, 2)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED", xStack_13c, false, true)
                                                    quest:FixMovieSequenceCamera(false)
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    resources:DestroyActorMap(xStack_13c)
                                                    resources:DestroyMovie(xStack_14c)
                                                    resources:DestroyMovie(xStack_124)
                                                    bVar3 = quest:IsXbox()
                                                    if bVar3 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if not bVar3 then
                                                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            while not bVar3 do
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if bVar3 then goto LAB_00d55c34_c29 end
                                                                bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                -- LAB_00d5439e_c29: (native jump target)
                                                                xStack_180 = quest:RegisterTimer()
                                                                quest:SetTimer(xStack_180, 10)
                                                                iVar7 = (xStack_160 ~= nil and xStack_160:IsAlive())
                                                                cVar4 = iVar7
                                                                while cVar4 do
                                                                    alive = quest:NewScriptFrame(me)
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00d55c2b_c29 end
                                                                    iVar7 = quest:GetTimer(xStack_180)
                                                                    if iVar7 < 1 then
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then goto LAB_00d55c2b_c29 end
                                                                        iVar8 = quest:AddNewConversation(me, false, false)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddPersonToConversation(iVar8, pCVar6)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, pCVar6, false)
                                                                        quest:SetTimer(xStack_180, 10)
                                                                    end
                                                                    iVar7 = (xStack_160 ~= nil and xStack_160:IsAlive())
                                                                    cVar4 = iVar7
                                                                end
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if not bVar3 then
                                                                    quest:Pause(0.5)
                                                                    xStack_14c = resources:NewResource()
                                                                    bVar3 = false
                                                                    if bVar3 ~= 0 then
                                                                    end
                                                                    iVar8 = 4
                                                                    pCVar6 = quest:GetHero()
                                                                    bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                                    while not bVar3 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then return end  -- TODO(native): goto LAB_00d54dfa_c29
                                                                        iVar8 = 4
                                                                        pCVar6 = quest:GetHero()
                                                                        bVar3 = resources:TryAcquire(xStack_14c, pCVar6, iVar8)
                                                                    end
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then
                                                                        -- LAB_00d54dfa_c29: (native jump target)
                                                                    else
                                                                        pCVar5 = resources:NewActorMap()
                                                                        resources:SetActor(pCVar5, "HERO", xStack_14c)
                                                                        resources:SetActor(pCVar5, "TEACHER", xStack_17c)
                                                                        xStack_124 = resources:StartMovie("")
                                                                        quest:StartMovieSequence()
                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                        quest:FixMovieSequenceCamera(true)
                                                                        resources:RunMacro("CS_GUILD_PREMELEE_ALARM", pCVar5, false, true)
                                                                        quest:FixMovieSequenceCamera(false)
                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                        resources:DestroyActorMap(pCVar5)
                                                                        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", "Q_GuildTrainingWoodsMelee", false)
                                                                        quest:SetQuestCardObjective("Q_GuildTrainingWoodsMelee", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                        quest:KickOffQuestStartScreen("Q_GuildTrainingWoodsMelee", true, false)
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if not bVar3 then
                                                                            alive = quest:NewScriptFrame(me)
                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                            bVar3 = not alive
                                                                            if not bVar3 then
                                                                                alive = quest:NewScriptFrame(me)
                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                bVar3 = not alive
                                                                                if not bVar3 then
                                                                                    alive = quest:NewScriptFrame(me)
                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                    bVar3 = not alive
                                                                                    if not bVar3 then
                                                                                        alive = quest:NewScriptFrame(me)
                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                        bVar3 = not alive
                                                                                        if not bVar3 then
                                                                                            bVar3 = quest:DisplayTutorial(0x1c)
                                                                                            if bVar3 then
                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                bVar3 = not alive
                                                                                                if not bVar3 then
                                                                                                    bVar3 = quest:MsgIsTutorialClickedPast()
                                                                                                    while not bVar3 do
                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                        bVar3 = quest:MsgIsTutorialClickedPast()
                                                                                                    end
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    bVar3 = not alive
                                                                                                    if not bVar3 then return end  -- TODO(native): goto LAB_00d54846_c29
                                                                                                end
                                                                                            else
                                                                                                -- LAB_00d54846_c29: (native jump target)
                                                                                                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                quest:MiniMapRemoveMarker(pCVar6)
                                                                                                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                quest:MiniMapAddMarker(pCVar6, "HUD_ORB_GREEN_SMALL")
                                                                                                pCVar6 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                iVar23 = 1
                                                                                                iVar8 = 0
                                                                                                iVar7 = 1.0
                                                                                                pCVar12 = pCVar6:GetPos()
                                                                                                me:MoveToPosition(pCVar12, iVar7, iVar8, (xStack_14c ~= 0), (iVar23 ~= 0))
                                                                                                c_stk_169 = 1
                                                                                                ctr_154 = 0
                                                                                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                                c_stk_161 = 0
                                                                                                repeat
                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    bVar3 = not alive
                                                                                                    if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c29
                                                                                                    if not __native_entity_state:GetStateBool("WoodsEndPlayed") then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                        __native_entity_state:SetStateBool("WoodsEndPlayed", true)
                                                                                                        pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                        quest:MiniMapRemoveMarker(pCVar6)
                                                                                                        pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                        quest:MiniMapAddMarker(pCVar6, "HUD_ORB_QUEST_CORE")
                                                                                                        xStack_108 = resources:NewResource()
                                                                                                        bVar3 = false
                                                                                                        if bVar3 ~= 0 then
                                                                                                        end
                                                                                                        iVar8 = 4
                                                                                                        pCVar6 = quest:GetHero()
                                                                                                        bVar3 = resources:TryAcquire(xStack_108, pCVar6, iVar8)
                                                                                                        while not bVar3 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c91_c29
                                                                                                            iVar8 = 4
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            bVar3 = resources:TryAcquire(xStack_108, pCVar6, iVar8)
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d55c91_c29: (native jump target)
                                                                                                            goto LAB_00d55c2b_c29
                                                                                                        end
                                                                                                        xStack_13c = resources:NewActorMap()
                                                                                                        resources:SetActor(xStack_13c, "HERO", xStack_108)
                                                                                                        resources:SetActor(xStack_13c, "GUARD", xStack_17c)
                                                                                                        xStack_124 = resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        pCVar6 = 0x1
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        quest:FixMovieSequenceCamera(true)
                                                                                                        resources:RunMacro("CS_GUILD_MELEE_WOODSWON", xStack_13c, false, true)
                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        while iVar7 < 0 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d55c72_c29: (native jump target)
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            -- LAB_00d55c7f_c29: (native jump target)
                                                                                                            resources:DestroyActorMap(xStack_13c)
                                                                                                            -- TODO(native): goto LAB_00d55c91_c29
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if iVar7 == 1 then
                                                                                                            if bVar3 then
                                                                                                                -- LAB_00d55c63_c29: (native jump target)
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                -- TODO(native): goto LAB_00d55c7f_c29
                                                                                                            end
                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                            quest:Pause(1.0)
                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                        else
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                            r30 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = r30
                                                                                                            fret_0 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fVar2 < fret_0 then
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r31 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c29
                                                                                                            end
                                                                                                            pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                            iVar23 = 0
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 1.0
                                                                                                            pCVar12 = pCVar5:GetPos()
                                                                                                            me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                        end
                                                                                                        quest:FixMovieSequenceCamera(false)
                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                        resources:DestroyActorMap(xStack_13c)
                                                                                                    else
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            -- LAB_00d54f9c_c29: (native jump target)
                                                                                                            bVar3 = false
                                                                                                        else
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if not bVar3 then return end  -- TODO(native): goto LAB_00d54f9c_c29
                                                                                                            bVar3 = true
                                                                                                        end
                                                                                                        if bVar3 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                            bVar3 = false
                                                                                                            if bVar3 ~= 0 then
                                                                                                            end
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if bVar3 then
                                                                                                                repeat
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                                    bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                                until not (bVar3)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                            bVar3 = false
                                                                                                            if bVar3 ~= 0 then
                                                                                                            end
                                                                                                            bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                            while not bVar3 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                                bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                            xStack_14c = resources:StartMovie("")
                                                                                                            quest:StartMovieSequence()
                                                                                                            pCVar6 = 0x1
                                                                                                            quest:PauseAllNonScriptedEntities(true)
                                                                                                            x_stk_30 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = x_stk_30
                                                                                                            fret_00 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fret_00 <= fVar2 then
                                                                                                                -- LAB_00d551d4_c29: (native jump target)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if iVar7 == 1 then
                                                                                                                        if bVar3 then
                                                                                                                            __region_LAB_00d55c9f_c29()
                                                                                                                            goto LAB_00d55c2b_c29
                                                                                                                        end
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                    else
                                                                                                                        if bVar3 then goto LAB_00d55cba_c29 end
                                                                                                                        x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_58
                                                                                                                        fret_01 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_01 then
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r32 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then goto LAB_00d55cba_c29 end
                                                                                                                        end
                                                                                                                        pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 1.0
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities(false)
                                                                                                                    goto LAB_00d55480_c29
                                                                                                                end
                                                                                                            else
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r33 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55c9f_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then return end  -- TODO(native): goto LAB_00d551d4_c29
                                                                                                            end
                                                                                                            ::LAB_00d55cba_c29::
                                                                                                            quest:PauseAllNonScriptedEntities(false)
                                                                                                            goto LAB_00d55c2b_c29
                                                                                                        end
                                                                                                    end
                                                                                                    ::LAB_00d55480_c29::
                                                                                                    cVar4 = me:IsTalkedToByHero()
                                                                                                    if cVar4 then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                        xStack_f8 = resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        pCVar6 = 0x1
                                                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                                                        me:ClearCommands()
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if not bVar3 then
                                                                                                                x_stk_c = resources:ScriptThing(xStack_17c)
                                                                                                                pCVar5 = x_stk_c
                                                                                                                fret_03 = quest:GetHealth(pCVar5)
                                                                                                                fVar2 = 0.0
                                                                                                                if fVar2 < fret_03 then
                                                                                                                    iVar23 = 1
                                                                                                                    iVar8 = 0
                                                                                                                    iVar7 = 0
                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    r34 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                    while cVar4 do
                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                        cVar4 = iVar7
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                end
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if iVar7 == 1 then
                                                                                                                        if not bVar3 then
                                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                                            quest:Pause(1.0)
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            goto LAB_00d5595a_c29
                                                                                                                        end
                                                                                                                        __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29
                                                                                                                    end
                                                                                                                    if not bVar3 then
                                                                                                                        x_stk_48 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_48
                                                                                                                        fret_04 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_04 then
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r35 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                        end
                                                                                                                        pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 1.0
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                                        goto LAB_00d5595a_c29
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            __region_LAB_00d55cd5_c29()
                                                                                                            goto LAB_00d55c2b_c29
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            __region_LAB_00d555f3_c29()
                                                                                                            goto LAB_00d55c2b_c29
                                                                                                        end
                                                                                                        x_stk_24 = resources:ScriptThing(xStack_17c)
                                                                                                        pCVar5 = x_stk_24
                                                                                                        fret_02 = quest:GetHealth(pCVar5)
                                                                                                        fVar2 = 0.0
                                                                                                        if fVar2 < fret_02 then
                                                                                                            iVar23 = 1
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 0
                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD"
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            r36 = me:Speak(pCVar6, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), (xStack_108 ~= 0))
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            cVar4 = iVar7
                                                                                                            while cVar4 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then __region_LAB_00d55cd5_c29(); goto LAB_00d55c2b_c29 end
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then __region_LAB_00d555f3_c29(); goto LAB_00d55c2b_c29 end
                                                                                                        end
                                                                                                        ::LAB_00d5595a_c29::
                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                    end
                                                                                                    if c_stk_169 ~= 0 then
                                                                                                        fVar20 = 5.5
                                                                                                        pCVar5 = quest:GetHero()
                                                                                                        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar20)
                                                                                                        __native_condition_11 = bVar3
                                                                                                        if __native_condition_11 then
                                                                                                            iVar7 = quest:GetTimer(xStack_188)
                                                                                                            __native_condition_11 = iVar7 < 1
                                                                                                        end
                                                                                                        __native_condition_10 = __native_condition_11
                                                                                                        if __native_condition_10 then
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            __native_condition_10 = not iVar7
                                                                                                        end
                                                                                                        if __native_condition_10 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                            iVar8 = quest:AddNewConversation(me, false, false)
                                                                                                            pCVar5 = quest:GetHero()
                                                                                                            quest:AddPersonToConversation(iVar8, pCVar5)
                                                                                                            quest:SetTimer(xStack_188, 10)
                                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                                if ctr_154 == 0x1 then
                                                                                                                    bVar3 = false
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                                    -- LAB_00d55b4e_c29: (native jump target)
                                                                                                                else
                                                                                                                    if ctr_154 == 0x2 then
                                                                                                                        bVar3 = false
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, pCVar5, false)
                                                                                                                        -- TODO(native): goto LAB_00d55b4e_c29
                                                                                                                    end
                                                                                                                end
                                                                                                                ctr_154 = 1 - ctr_154
                                                                                                            else
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                                bVar3 = false
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                            end
                                                                                                        end
                                                                                                    end
                                                                                                    if c_stk_161 == 0 then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                        if not iVar7 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c29 end
                                                                                                            bVar3 = false
                                                                                                            c_stk_161 = 1
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                                                                                                        end
                                                                                                    end
                                                                                                until not (c_stk_169 ~= 0)
                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                bVar3 = not alive
                                                                                                if not bVar3 then
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
                                                                quest:DeregisterTimer(xStack_180)
                                                            end
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if not bVar3 then
                                                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_PC")
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            while not bVar3 do
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if bVar3 then goto LAB_00d55c34_c29 end
                                                                bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                                                -- TODO(native): goto LAB_00d5439e_c29
                                                            end
                                                        end
                                                    end
                                                end
                                                ::LAB_00d55c34_c29::
                                            end
                                        end
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if not bVar3 then
                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK_PC")
                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                        while not bVar3 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00d55c3d_c29 end
                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if not bVar3 then
                                            -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                            -- TODO(native): goto LAB_00d53c7e_c29
                                        end
                                    end
                                end
                            end
                        end
                        ::LAB_00d55c3d_c29::
                        quest:DeregisterTimer(xStack_184)
                        goto FLOW_after_lab_00d536c0
                    end
                end
            end
            ::FLOW_after_lab_00d536c0::
        end
        ::FLOW_after_lab_00d533bb::
    end
    ::LAB_00d55c46::
    quest:DeregisterTimer(xStack_188)
    ::LAB_00d55c4f::
    resources:ReleaseResource(xStack_17c)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("ChatJumped", false)
    __native_entity_state:SetStateBool("WoodsEndPlayed", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

