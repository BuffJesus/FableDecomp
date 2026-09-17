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
    local CVar10, __native_condition_1, __native_condition_10, __native_condition_2, __native_condition_3, __native_condition_4, __native_condition_5, __native_condition_6, __native_condition_7, __native_condition_8, __native_condition_9, b2, bVar3, cVar4, c_stk_161, c_stk_169, fVar2, fVar20, iVar23, iVar7, iVar8, pCVar12, pCVar21, pCVar5, pCVar6, pcVar15, pppuVar22, puVar11, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r3, r30, r31, r32, r33, r34, r35, r36, r37, r38, r39, r4, r40, r41, r42, r43, r44, r45, r46, r47, r48, r49, r5, r50, r51, r52, r53, r54, r55, r56, r57, r58, r59, r6, r60, r61, r62, r63, r64, r65, r66, r67, r68, r69, r7, r8, r9, thing_b10, thing_b11, thing_b8, thing_b9, timerId, uVar14, uVar16, uVar17, uVar18, uVar19, u_stk_128, xStack_108, xStack_114_2, xStack_124, xStack_13c, xStack_14c, xStack_160, xStack_17c, xStack_188, xStack_24, xStack_30, xStack_c, xStack_f8, x_stk_58
    local alive = true
    local function __region_LAB_00d555f3_c26()
        quest:PauseAllNonScriptedEntities((uVar18 ~= 0))
    end
    local function __region_LAB_00d555f3_c27()
        quest:PauseAllNonScriptedEntities((uVar18 ~= 0))
    end
    local function __region_LAB_00d555f3_c28()
        quest:PauseAllNonScriptedEntities((uVar18 ~= 0))
    end
    local function __region_LAB_00d555f3_c5()
        quest:PauseAllNonScriptedEntities((uVar18 ~= 0))
    end
    local function __region_LAB_00d55c9f_c18()
        quest:PauseAllNonScriptedEntities((uVar18 ~= 0))
    end
    local function __region_LAB_00d55c9f_c26()
        quest:PauseAllNonScriptedEntities((fVar2 ~= 0))
    end
    local function __region_LAB_00d55c9f_c27()
        quest:PauseAllNonScriptedEntities((fVar2 ~= 0))
    end
    local function __region_LAB_00d55c9f_c28()
        quest:PauseAllNonScriptedEntities((fVar2 ~= 0))
    end
    local function __region_LAB_00d55c9f_c5()
        quest:PauseAllNonScriptedEntities((fVar2 ~= 0))
    end
    local function __region_LAB_00d55c9f_c6()
        quest:PauseAllNonScriptedEntities((fVar2 ~= 0))
    end
    local function __region_LAB_00d55cd5_c26()
        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
    end
    local function __region_LAB_00d55cd5_c27()
        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
    end
    local function __region_LAB_00d55cd5_c28()
        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
    end
    local function __region_LAB_00d55cd5_c5()
        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
    end
    cVar4 = quest:GetStateBool("GuildmasterTeleport")
    uVar14 = 0
    u_stk_128 = 0
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
    uVar16 = 0
    uVar17 = 0
    uVar18 = 0
    uVar19 = 0
    quest:EntitySetAsKillable(me, false, true)
    bVar3 = false
    -- TODO(native): thing._4_4_ = pCVar5;
    -- TODO(native): thing._0_4_ = p1;
    thing_b8 = uVar16
    thing_b9 = uVar17
    thing_b10 = uVar18
    thing_b11 = uVar19
    quest:SetIsPushableByHero(nil --[[missing]], thing)
    quest:SetThingHasInformation(me, false, false, false)
    bVar3 = false
    pCVar5 = quest:GetThingWithScriptName("M_MeleeTeacherStand")
    quest:EntityTeleportToThing(me, pCVar5, bVar3)
    xStack_188 = quest:RegisterTimer()
    quest:SetTimer(xStack_188, 0)
    c_stk_169 = '\x01'
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
        -- TODO(native): if ((!bVar3) || (iVar7 = GSI->GetTimer(xStack_188), 0 < iVar7)) goto switchD_00d5317f_default;
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d55c46 end
        iVar8 = quest:AddNewConversation(me, false, false)
        pCVar6 = quest:GetHero()
        quest:AddPersonToConversation(iVar8, pCVar6)
        quest:SetTimer(xStack_188, 5)
        -- TODO(native): switch(uVar14) {
        -- TODO(native): case 0:
        bVar3 = false
        pCVar6 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
        pCVar6 = quest:GetHero()
        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FIRST", me, pCVar6, false)
        me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, DAT_01375748, false, false)
        uVar14 = 1
        break
        -- TODO(native): case 1:
        bVar3 = false
        pCVar6 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
        pCVar6 = quest:GetHero()
        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_SECOND", me, pCVar6, false)
        goto LAB_00d53316
        -- TODO(native): case 2:
        bVar3 = false
        pCVar6 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
        pCVar6 = quest:GetHero()
        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_THIRD", me, pCVar6, false)
        uVar14 = 3
        break
        -- TODO(native): case 3:
        bVar3 = false
        pCVar6 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
        pCVar6 = quest:GetHero()
        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FOURTH", me, pCVar6, false)
        ::LAB_00d53316::
        uVar14 = 2
    end
    -- TODO(native): switchD_00d5317f_default:
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
        uVar16 = SUB41(xStack_14c,0)
        uVar17 = (xStack_14c >> 8)
        uVar18 = (xStack_14c >> 0x10)
        uVar19 = (xStack_14c >> 0x18)
        pCVar6 = quest:GetHero()
        bVar3 = me:AcquireControl(4)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:DestroyMovie(xStack_14c)
                goto FLOW_after_lab_00d533bb
            end
            iVar8 = 4
            uVar16 = SUB41(xStack_14c,0)
            uVar17 = (xStack_14c >> 8)
            uVar18 = (xStack_14c >> 0x10)
            uVar19 = (xStack_14c >> 0x18)
            pCVar6 = quest:GetHero()
            bVar3 = me:AcquireControl(4)
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
                        -- TODO(native): CTimer::CTimer((CTimer *)&xStack_184);
                        quest:SetTimer(xStack_188, xStack_184)
                        quest:SetStateInt("PreMeleeMode", 1)
                        quest:SetStateInt("DummyHits", 0)
                        CVar10 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
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
                                quest:SetTimer(xStack_188, 10)
                            end
                            iVar7 = quest:GetTimer(xStack_188)
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
                                quest:SetTimer(xStack_188, xStack_184)
                                CVar10 = CVar10
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
                            uVar16 = SUB41(xStack_14c,0)
                            uVar17 = (xStack_14c >> 8)
                            uVar18 = (xStack_14c >> 0x10)
                            uVar19 = (xStack_14c >> 0x18)
                            pCVar6 = quest:GetHero()
                            bVar3 = me:AcquireControl(4)
                            while not bVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    resources:DestroyMovie(xStack_14c)
                                    goto FLOW_after_lab_00d53a0b
                                end
                                iVar8 = 4
                                uVar16 = SUB41(xStack_14c,0)
                                uVar17 = (xStack_14c >> 8)
                                uVar18 = (xStack_14c >> 0x10)
                                uVar19 = (xStack_14c >> 0x18)
                                pCVar6 = quest:GetHero()
                                bVar3 = me:AcquireControl(4)
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
                                        quest:DisplayGameInfo("")
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
                                            quest:SetTimer(xStack_188, 10)
                                            CVar10 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
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
                                                    quest:SetTimer(xStack_188, 10)
                                                end
                                                iVar7 = quest:GetTimer(xStack_188)
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
                                                    quest:SetTimer(xStack_188, xStack_184)
                                                    CVar10 = CVar10
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
                                                uVar16 = SUB41(xStack_14c,0)
                                                uVar17 = (xStack_14c >> 8)
                                                uVar18 = (xStack_14c >> 0x10)
                                                uVar19 = (xStack_14c >> 0x18)
                                                pCVar6 = quest:GetHero()
                                                bVar3 = me:AcquireControl(4)
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
                                                    uVar16 = SUB41(xStack_14c,0)
                                                    uVar17 = (xStack_14c >> 8)
                                                    uVar18 = (xStack_14c >> 0x10)
                                                    uVar19 = (xStack_14c >> 0x18)
                                                    pCVar6 = quest:GetHero()
                                                    bVar3 = me:AcquireControl(4)
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
                                                    quest:CreateEffect(xStack_dc, "SMASH_DUMMY_01", (xStack_114_2 + 4), "", 0.0, false, false)
                                                    quest:FadeOutAndKillEntity(r1, true, 1.0, true)
                                                    xStack_13c = resources:NewActorMap()
                                                    resources:SetActor(xStack_13c, "HERO", xStack_14c)
                                                    resources:SetActor(xStack_13c, "TEACHER", xStack_17c)
                                                    quest:FixMovieSequenceCamera(true)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", xStack_13c, false, false)
                                                    quest:PauseAllNonScriptedEntities(true)
                                                    pCVar6 = quest:CreateExperienceOrb(xStack_114_2, 1)
                                                    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                                    quest:EntitySetCutsceneBehaviour(xStack_160, 2)
                                                    resources:RunMacro(xStack_18c, xStack_13c, false, true)
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
                                                                -- TODO(native): CTimer::CTimer((CTimer *)&xStack_180);
                                                                quest:SetTimer(xStack_188, xStack_180)
                                                                iVar7 = (xStack_160 ~= nil and xStack_160:IsAlive())
                                                                cVar4 = iVar7
                                                                while cVar4 do
                                                                    alive = quest:NewScriptFrame(me)
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00d55c2b end
                                                                    iVar7 = quest:GetTimer(xStack_188)
                                                                    if iVar7 < 1 then
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then goto LAB_00d55c2b end
                                                                        iVar8 = quest:AddNewConversation(me, false, false)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddPersonToConversation(iVar8, pCVar6)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, pCVar6, false)
                                                                        quest:SetTimer(xStack_188, xStack_180)
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
                                                                    uVar16 = SUB41(xStack_14c,0)
                                                                    uVar17 = (xStack_14c >> 8)
                                                                    uVar18 = (xStack_14c >> 0x10)
                                                                    uVar19 = (xStack_14c >> 0x18)
                                                                    pCVar6 = quest:GetHero()
                                                                    bVar3 = me:AcquireControl(4)
                                                                    while not bVar3 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then
                                                                            goto FLOW_after_lab_00d54dfa
                                                                        end
                                                                        iVar8 = 4
                                                                        uVar16 = SUB41(xStack_14c,0)
                                                                        uVar17 = (xStack_14c >> 8)
                                                                        uVar18 = (xStack_14c >> 0x10)
                                                                        uVar19 = (xStack_14c >> 0x18)
                                                                        pCVar6 = quest:GetHero()
                                                                        bVar3 = me:AcquireControl(4)
                                                                    end
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then
                                                                        -- LAB_00d54dfa: (native jump target)
                                                                    else
                                                                        xStack_114_2 = resources:NewActorMap()
                                                                        pCVar21 = xStack_124
                                                                        -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](( map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)(xStack_114_2 + 4),&xStack_18c);
                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar9,pCVar21);
                                                                        pppuVar22 = xStack_17c
                                                                        -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](( map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)(xStack_114_2 + 4),&xStack_18c);
                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar9,pppuVar22);
                                                                        xStack_124 = resources:StartMovie("")
                                                                        quest:StartMovieSequence()
                                                                        quest:PauseAllNonScriptedEntities(true)
                                                                        quest:FixMovieSequenceCamera(true)
                                                                        resources:RunMacro("CS_GUILD_PREMELEE_ALARM", xStack_114_2, false, true)
                                                                        quest:FixMovieSequenceCamera(false)
                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                        resources:DestroyActorMap(xStack_114_2)
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
                                                                                            bVar3 = (**(**(this + 4) + 0x1d8 ))(*(this + 4),0x1c)
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
                                                                                                        pCVar6 = quest:GetThingWithScriptName("HUD_ORB_GREEN_SMALL")
                                                                                                        quest:MiniMapAddMarker(pCVar6, "TheRealGuildmaster")
                                                                                                        pCVar6 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                        iVar23 = 1
                                                                                                        uVar16 = 0
                                                                                                        uVar17 = 0
                                                                                                        uVar18 = 0
                                                                                                        uVar19 = 0
                                                                                                        iVar8 = 0
                                                                                                        iVar7 = 0x3f800000
                                                                                                        pCVar12 = pCVar6:GetPos()
                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))), (iVar23 ~= 0))
                                                                                                        c_stk_169 = '\x01'
                                                                                                        quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                                        c_stk_161 = 0
                                                                                                        repeat
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b end
                                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c5
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
                                                                                                                uVar16 = SUB41(xStack_108,0)
                                                                                                                uVar17 = (xStack_108 >> 8)
                                                                                                                uVar18 = (xStack_108 >> 0x10)
                                                                                                                uVar19 = (xStack_108 >> 0x18)
                                                                                                                pCVar6 = quest:GetHero()
                                                                                                                bVar3 = me:AcquireControl(4)
                                                                                                                while not bVar3 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d55c91_c5
                                                                                                                    iVar8 = 4
                                                                                                                    uVar16 = SUB41(xStack_108,0)
                                                                                                                    uVar17 = (xStack_108 >> 8)
                                                                                                                    uVar18 = (xStack_108 >> 0x10)
                                                                                                                    uVar19 = (xStack_108 >> 0x18)
                                                                                                                    pCVar6 = quest:GetHero()
                                                                                                                    bVar3 = me:AcquireControl(4)
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then
                                                                                                                    -- LAB_00d55c91_c5: (native jump target)
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                                xStack_13c = resources:NewActorMap()
                                                                                                                resources:SetActor(xStack_13c, "HERO", xStack_108)
                                                                                                                resources:SetActor(xStack_13c, "GUARD", xStack_17c)
                                                                                                                xStack_124 = resources:StartMovie("")
                                                                                                                quest:StartMovieSequence()
                                                                                                                pCVar6 = 0x1
                                                                                                                quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                                quest:FixMovieSequenceCamera(true)
                                                                                                                resources:RunMacro(xStack_c4, xStack_13c, false, true)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", xStack_b4)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c5
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then
                                                                                                                    -- LAB_00d55c72_c5: (native jump target)
                                                                                                                    quest:PauseAllNonScriptedEntities((iVar8 ~= 0))
                                                                                                                    -- LAB_00d55c7f_c5: (native jump target)
                                                                                                                    resources:DestroyActorMap(xStack_13c)
                                                                                                                    -- TODO(native): goto LAB_00d55c91_c5
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if iVar7 == 1 then
                                                                                                                    if bVar3 then
                                                                                                                        -- LAB_00d55c63_c5: (native jump target)
                                                                                                                        quest:PauseAllNonScriptedEntities(false)
                                                                                                                        -- TODO(native): goto LAB_00d55c7f_c5
                                                                                                                    end
                                                                                                                    quest:FadeScreenOut(0.5, 0.5)
                                                                                                                    quest:Pause(1.0)
                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                    -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                else
                                                                                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c5
                                                                                                                    r1 = resources:ScriptThing(xStack_17c)
                                                                                                                    pCVar5 = r1
                                                                                                                    r2 = quest:GetHealth(pCVar5)
                                                                                                                    fVar2 = 0.0
                                                                                                                    if fVar2 < fret_0 then
                                                                                                                        uVar16 = 0
                                                                                                                        uVar17 = 0
                                                                                                                        uVar18 = 0
                                                                                                                        uVar19 = 0
                                                                                                                        iVar23 = 1
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0
                                                                                                                        pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        r3 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                        cVar4 = iVar7
                                                                                                                        while cVar4 do
                                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c5
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                        end
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c5
                                                                                                                    end
                                                                                                                    pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                    uVar16 = 1
                                                                                                                    uVar17 = 0
                                                                                                                    uVar18 = 0
                                                                                                                    uVar19 = 0
                                                                                                                    iVar23 = 0
                                                                                                                    iVar8 = 0
                                                                                                                    iVar7 = 0x3f800000
                                                                                                                    pCVar12 = pCVar5:GetPos()
                                                                                                                    me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                    -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                                end
                                                                                                                quest:FixMovieSequenceCamera(false)
                                                                                                                quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                                resources:DestroyActorMap(xStack_13c)
                                                                                                            else
                                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                    -- LAB_00d54f9c_c5: (native jump target)
                                                                                                                    bVar3 = false
                                                                                                                else
                                                                                                                    u_stk_128 = u_stk_128 | 1
                                                                                                                    bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                                    if not bVar3 then return end  -- TODO(native): goto LAB_00d54f9c_c5
                                                                                                                    bVar3 = true
                                                                                                                end
                                                                                                                if (u_stk_128 & 1) ~= 0 then
                                                                                                                    u_stk_128 = u_stk_128 & 0xfffffffe
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
                                                                                                                            -- TODO(native): (**(code **)(**(int **)(this + 4) + 0x1c) )(*(int **)(this + 4));
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
                                                                                                                    bVar3 = me:AcquireControl(4)
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
                                                                                                                    quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                                    xStack_30 = resources:ScriptThing(xStack_17c)
                                                                                                                    pCVar5 = xStack_30
                                                                                                                    r4 = quest:GetHealth(pCVar5)
                                                                                                                    fVar2 = 0.0
                                                                                                                    if fret_00 <= fVar2 then
                                                                                                                        -- LAB_00d551d4_c5: (native jump target)
                                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        while iVar7 < 0 do
                                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then __region_LAB_00d55c9f_c5(); goto LAB_00d55c2b end
                                                                                                                            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        end
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if not bVar3 then
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if iVar7 == 1 then
                                                                                                                                if bVar3 then
                                                                                                                                    __region_LAB_00d55c9f_c5()
                                                                                                                                    goto LAB_00d55c2b
                                                                                                                                end
                                                                                                                                quest:FadeScreenOut(0.5, 0.5)
                                                                                                                                quest:Pause(1.0)
                                                                                                                                quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                                -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                            else
                                                                                                                                if bVar3 then goto LAB_00d55cba_c5 end
                                                                                                                                x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                                pCVar5 = x_stk_58
                                                                                                                                r5 = quest:GetHealth(pCVar5)
                                                                                                                                fVar2 = 0.0
                                                                                                                                if fVar2 < fret_01 then
                                                                                                                                    uVar16 = 0
                                                                                                                                    uVar17 = 0
                                                                                                                                    uVar18 = 0
                                                                                                                                    uVar19 = 0
                                                                                                                                    iVar23 = 1
                                                                                                                                    iVar8 = 0
                                                                                                                                    iVar7 = 0
                                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                                    r6 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                                    cVar4 = iVar7
                                                                                                                                    while cVar4 do
                                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                                        bVar3 = not alive
                                                                                                                                        if bVar3 then __region_LAB_00d55c9f_c5(); goto LAB_00d55c2b end
                                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                                        cVar4 = iVar7
                                                                                                                                    end
                                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                                    bVar3 = not alive
                                                                                                                                    if bVar3 then goto LAB_00d55cba_c5 end
                                                                                                                                end
                                                                                                                                pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                                uVar16 = 1
                                                                                                                                uVar17 = 0
                                                                                                                                uVar18 = 0
                                                                                                                                uVar19 = 0
                                                                                                                                iVar23 = 0
                                                                                                                                iVar8 = 0
                                                                                                                                iVar7 = 0x3f800000
                                                                                                                                pCVar12 = pCVar5:GetPos()
                                                                                                                                me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                                -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                                            end
                                                                                                                            quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                                            goto LAB_00d55480_c5
                                                                                                                        end
                                                                                                                    else
                                                                                                                        uVar16 = 0
                                                                                                                        uVar17 = 0
                                                                                                                        uVar18 = 0
                                                                                                                        uVar19 = 0
                                                                                                                        iVar23 = 1
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0
                                                                                                                        pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        r7 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                        cVar4 = iVar7
                                                                                                                        while cVar4 do
                                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then __region_LAB_00d55c9f_c5(); goto LAB_00d55c2b end
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                        end
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if not bVar3 then return end  -- TODO(native): goto LAB_00d551d4_c5
                                                                                                                    end
                                                                                                                    ::LAB_00d55cba_c5::
                                                                                                                    quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                            end
                                                                                                            ::LAB_00d55480_c5::
                                                                                                            cVar4 = me:IsTalkedToByHero()
                                                                                                            if cVar4 then
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b end
                                                                                                                xStack_f8 = resources:StartMovie("")
                                                                                                                quest:StartMovieSequence()
                                                                                                                pCVar6 = 0x1
                                                                                                                quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                                me:ClearCommands()
                                                                                                                if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if not bVar3 then
                                                                                                                        xStack_24 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = xStack_24
                                                                                                                        r8 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_03 then
                                                                                                                            uVar16 = 0
                                                                                                                            uVar17 = 0
                                                                                                                            uVar18 = 0
                                                                                                                            uVar19 = 0
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r9 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then __region_LAB_00d555f3_c5(); goto LAB_00d55c2b end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then __region_LAB_00d55cd5_c5(); goto LAB_00d55c2b end
                                                                                                                        end
                                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", xStack_8c)
                                                                                                                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        while iVar7 < 0 do
                                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then __region_LAB_00d555f3_c5(); goto LAB_00d55c2b end
                                                                                                                            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                        end
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if not bVar3 then
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if iVar7 == 1 then
                                                                                                                                if not bVar3 then
                                                                                                                                    quest:FadeScreenOut(*(this + 4), 0.5)
                                                                                                                                    quest:Pause(*(this + 4))
                                                                                                                                    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                                    -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                                    goto LAB_00d5595a_c5
                                                                                                                                end
                                                                                                                                __region_LAB_00d555f3_c5(); goto LAB_00d55c2b
                                                                                                                            end
                                                                                                                            if not bVar3 then
                                                                                                                                xStack_c = resources:ScriptThing(xStack_17c)
                                                                                                                                pCVar5 = xStack_c
                                                                                                                                r10 = quest:GetHealth(pCVar5)
                                                                                                                                fVar2 = 0.0
                                                                                                                                if fVar2 < fret_04 then
                                                                                                                                    uVar16 = 0
                                                                                                                                    uVar17 = 0
                                                                                                                                    uVar18 = 0
                                                                                                                                    uVar19 = 0
                                                                                                                                    iVar23 = 1
                                                                                                                                    iVar8 = 0
                                                                                                                                    iVar7 = 0
                                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                                    r11 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                                    cVar4 = iVar7
                                                                                                                                    while cVar4 do
                                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                                        bVar3 = not alive
                                                                                                                                        if bVar3 then __region_LAB_00d555f3_c5(); goto LAB_00d55c2b end
                                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                                        cVar4 = iVar7
                                                                                                                                    end
                                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                                    bVar3 = not alive
                                                                                                                                    if bVar3 then __region_LAB_00d55cd5_c5(); goto LAB_00d55c2b end
                                                                                                                                end
                                                                                                                                pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                                uVar16 = 1
                                                                                                                                uVar17 = 0
                                                                                                                                uVar18 = 0
                                                                                                                                uVar19 = 0
                                                                                                                                iVar23 = 0
                                                                                                                                iVar8 = 0
                                                                                                                                iVar7 = 0x3f800000
                                                                                                                                pCVar12 = pCVar5:GetPos()
                                                                                                                                me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                                -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                                                goto LAB_00d5595a_c5
                                                                                                                            end
                                                                                                                        end
                                                                                                                    end
                                                                                                                    __region_LAB_00d55cd5_c5()
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then
                                                                                                                    __region_LAB_00d555f3_c5()
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                                pCVar5 = resources:ScriptThing(xStack_17c)
                                                                                                                pCVar5 = pCVar5
                                                                                                                r12 = quest:GetHealth(pCVar5)
                                                                                                                fVar2 = 0.0
                                                                                                                if fVar2 < fret_02 then
                                                                                                                    uVar16 = 0
                                                                                                                    uVar17 = 0
                                                                                                                    uVar18 = 0
                                                                                                                    uVar19 = 0
                                                                                                                    iVar23 = 1
                                                                                                                    iVar8 = 0
                                                                                                                    iVar7 = 0
                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD"
                                                                                                                    pCVar6 = quest:GetHero()
                                                                                                                    r13 = me:Speak(pCVar6, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                    while cVar4 do
                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then __region_LAB_00d55cd5_c5(); goto LAB_00d55c2b end
                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                        cVar4 = iVar7
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d555f3_c5(); goto LAB_00d55c2b end
                                                                                                                end
                                                                                                                ::LAB_00d5595a_c5::
                                                                                                                quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                            end
                                                                                                            if c_stk_169 ~= 0 then
                                                                                                                fVar20 = 5.5
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar20)
                                                                                                                __native_condition_2 = bVar3
                                                                                                                if __native_condition_2 then
                                                                                                                    __native_condition_2 = iVar7 < 1
                                                                                                                end
                                                                                                                __native_condition_1 = __native_condition_2
                                                                                                                if __native_condition_1 then
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    __native_condition_1 = not iVar7
                                                                                                                end
                                                                                                                if __native_condition_1 then
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
                                                                                                                        if xStack_18c_3 == 0x1 then
                                                                                                                            bVar3 = false
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                                            -- LAB_00d55b4e_c5: (native jump target)
                                                                                                                        elseif xStack_18c_3 == 0x2 then
                                                                                                                            bVar3 = false
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, pCVar5, false)
                                                                                                                            -- TODO(native): goto LAB_00d55b4e_c5
                                                                                                                        end
                                                                                                                        -- TODO(native): xStack_18c_3 = (CCharString)(1 - (int)xStack_18c_3);
                                                                                                                    else
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then goto LAB_00d55c2b end
                                                                                                                        bVar3 = false
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                        pCVar5 = quest:GetHero()
                                                                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, pCVar5, xStack_130)
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
                                                                                                                    c_stk_161 = '\x01'
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
                                                                                                pCVar6 = quest:GetThingWithScriptName("HUD_ORB_GREEN_SMALL")
                                                                                                quest:MiniMapAddMarker(pCVar6, "TheRealGuildmaster")
                                                                                                pCVar6 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                iVar23 = 1
                                                                                                uVar16 = 0
                                                                                                uVar17 = 0
                                                                                                uVar18 = 0
                                                                                                uVar19 = 0
                                                                                                iVar8 = 0
                                                                                                iVar7 = 0x3f800000
                                                                                                pCVar12 = pCVar6:GetPos()
                                                                                                me:MoveToPosition(pCVar12, iVar7, iVar8, CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))), (iVar23 ~= 0))
                                                                                                c_stk_169 = '\x01'
                                                                                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                                c_stk_161 = 0
                                                                                                repeat
                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    bVar3 = not alive
                                                                                                    if bVar3 then goto LAB_00d55c2b end
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                        -- LAB_00d54f9c_c6: (native jump target)
                                                                                                        bVar3 = false
                                                                                                        if (u_stk_128 & 1) ~= 0 then
                                                                                                            u_stk_128 = u_stk_128 & 0xfffffffe
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
                                                                                                                    -- TODO(native): (**(code **)(**(int **)(this + 4) + 0x1c) )(*(int **)(this + 4));
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
                                                                                                            bVar3 = me:AcquireControl(4)
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
                                                                                                            quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                            xStack_30 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = xStack_30
                                                                                                            r14 = quest:GetHealth(pCVar5)
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
                                                                                                                        -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                    else
                                                                                                                        if bVar3 then goto LAB_00d55cba_c6 end
                                                                                                                        x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_58
                                                                                                                        r15 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_01 then
                                                                                                                            uVar16 = 0
                                                                                                                            uVar17 = 0
                                                                                                                            uVar18 = 0
                                                                                                                            uVar19 = 0
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r16 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                                        uVar16 = 1
                                                                                                                        uVar17 = 0
                                                                                                                        uVar18 = 0
                                                                                                                        uVar19 = 0
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0x3f800000
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                        -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                                    goto LAB_00d55480
                                                                                                                end
                                                                                                            else
                                                                                                                uVar16 = 0
                                                                                                                uVar17 = 0
                                                                                                                uVar18 = 0
                                                                                                                uVar19 = 0
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r17 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                            quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
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
                                                                                                        uVar16 = SUB41(xStack_108,0)
                                                                                                        uVar17 = (xStack_108 >> 8)
                                                                                                        uVar18 = (xStack_108 >> 0x10)
                                                                                                        uVar19 = (xStack_108 >> 0x18)
                                                                                                        pCVar6 = quest:GetHero()
                                                                                                        bVar3 = me:AcquireControl(4)
                                                                                                        while not bVar3 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            iVar8 = 4
                                                                                                            uVar16 = SUB41(xStack_108,0)
                                                                                                            uVar17 = (xStack_108 >> 8)
                                                                                                            uVar18 = (xStack_108 >> 0x10)
                                                                                                            uVar19 = (xStack_108 >> 0x18)
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            bVar3 = me:AcquireControl(4)
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
                                                                                                        quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                        quest:FixMovieSequenceCamera(true)
                                                                                                        resources:RunMacro(xStack_c4, xStack_13c, false, true)
                                                                                                        quest:GiveHeroYesNoQuestion("", "TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", xStack_7c)
                                                                                                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        while iVar7 < 0 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then
                                                                                                                quest:PauseAllNonScriptedEntities((iVar8 ~= 0))
                                                                                                                -- LAB_00d55c7f_c8: (native jump target)
                                                                                                                resources:DestroyActorMap(xStack_13c)
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d55c72: (native jump target)
                                                                                                            quest:PauseAllNonScriptedEntities((fVar2 ~= 0))
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
                                                                                                            -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                        else
                                                                                                            if bVar3 then
                                                                                                                quest:PauseAllNonScriptedEntities((fVar20 ~= 0))
                                                                                                                -- LAB_00d55c7f_c11: (native jump target)
                                                                                                                resources:DestroyActorMap(xStack_13c)
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                            r1 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = r1
                                                                                                            r18 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fVar2 < fret_0 then
                                                                                                                uVar16 = 0
                                                                                                                uVar17 = 0
                                                                                                                uVar18 = 0
                                                                                                                uVar19 = 0
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r19 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then
                                                                                                                        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                                        -- LAB_00d55c7f_c12: (native jump target)
                                                                                                                        resources:DestroyActorMap(xStack_13c)
                                                                                                                        goto LAB_00d55c2b
                                                                                                                    end
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then
                                                                                                                    quest:PauseAllNonScriptedEntities((uVar18 ~= 0))
                                                                                                                    -- LAB_00d55c7f_c13: (native jump target)
                                                                                                                    resources:DestroyActorMap(xStack_13c)
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                            end
                                                                                                            pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                            uVar16 = 1
                                                                                                            uVar17 = 0
                                                                                                            uVar18 = 0
                                                                                                            uVar19 = 0
                                                                                                            iVar23 = 0
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 0x3f800000
                                                                                                            pCVar12 = pCVar5:GetPos()
                                                                                                            me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                            -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                        end
                                                                                                        quest:FixMovieSequenceCamera(false)
                                                                                                        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                        resources:DestroyActorMap(xStack_13c)
                                                                                                    else
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            -- LAB_00d54f9c: (native jump target)
                                                                                                            bVar3 = false
                                                                                                        else
                                                                                                            u_stk_128 = u_stk_128 | 1
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if not bVar3 then
                                                                                                                bVar3 = false
                                                                                                                goto FLOW_after_lab_00d54f9c_811
                                                                                                            end
                                                                                                            bVar3 = true
                                                                                                        end
                                                                                                        ::FLOW_after_lab_00d54f9c_811::
                                                                                                        if (u_stk_128 & 1) ~= 0 then
                                                                                                            u_stk_128 = u_stk_128 & 0xfffffffe
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
                                                                                                                    -- TODO(native): (**(code **)(**(int **)(this + 4) + 0x1c) )(*(int **)(this + 4));
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
                                                                                                            bVar3 = me:AcquireControl(4)
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
                                                                                                            quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                            xStack_30 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = xStack_30
                                                                                                            r20 = quest:GetHealth(pCVar5)
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
                                                                                                                        -- LAB_00d55c9f_c15: (native jump target)
                                                                                                                        quest:PauseAllNonScriptedEntities((fVar2 ~= 0))
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
                                                                                                                            quest:PauseAllNonScriptedEntities((uVar18 ~= 0))
                                                                                                                            goto LAB_00d55c2b
                                                                                                                        end
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                        -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                    else
                                                                                                                        if bVar3 then goto LAB_00d55cba end
                                                                                                                        x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_58
                                                                                                                        r21 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_01 then
                                                                                                                            uVar16 = 0
                                                                                                                            uVar17 = 0
                                                                                                                            uVar18 = 0
                                                                                                                            uVar19 = 0
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r22 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then
                                                                                                                                    quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
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
                                                                                                                        uVar16 = 1
                                                                                                                        uVar17 = 0
                                                                                                                        uVar18 = 0
                                                                                                                        uVar19 = 0
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0x3f800000
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                        -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                                    goto LAB_00d55480
                                                                                                                end
                                                                                                            else
                                                                                                                uVar16 = 0
                                                                                                                uVar17 = 0
                                                                                                                uVar18 = 0
                                                                                                                uVar19 = 0
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r23 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then
                                                                                                                        -- LAB_00d55c9f_c17: (native jump target)
                                                                                                                        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
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
                                                                                                                        if bVar3 then __region_LAB_00d55c9f_c18(); goto LAB_00d55c2b end
                                                                                                                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if not bVar3 then
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if iVar7 == 1 then
                                                                                                                            if bVar3 then
                                                                                                                                __region_LAB_00d55c9f_c18()
                                                                                                                                goto LAB_00d55c2b
                                                                                                                            end
                                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                                            quest:Pause(1.0)
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                        else
                                                                                                                            if bVar3 then goto LAB_00d55cba end
                                                                                                                            x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                            pCVar5 = x_stk_58
                                                                                                                            r24 = quest:GetHealth(pCVar5)
                                                                                                                            fVar2 = 0.0
                                                                                                                            if fVar2 < fret_01 then
                                                                                                                                uVar16 = 0
                                                                                                                                uVar17 = 0
                                                                                                                                uVar18 = 0
                                                                                                                                uVar19 = 0
                                                                                                                                iVar23 = 1
                                                                                                                                iVar8 = 0
                                                                                                                                iVar7 = 0
                                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                                r25 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                                while cVar4 do
                                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                                    bVar3 = not alive
                                                                                                                                    if bVar3 then __region_LAB_00d55c9f_c18(); goto LAB_00d55c2b end
                                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                                    cVar4 = iVar7
                                                                                                                                end
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then goto LAB_00d55cba end
                                                                                                                            end
                                                                                                                            pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                            uVar16 = 1
                                                                                                                            uVar17 = 0
                                                                                                                            uVar18 = 0
                                                                                                                            uVar19 = 0
                                                                                                                            iVar23 = 0
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0x3f800000
                                                                                                                            pCVar12 = pCVar5:GetPos()
                                                                                                                            me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                            -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                                        end
                                                                                                                        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                                        goto LAB_00d55480
                                                                                                                    end
                                                                                                                    goto FLOW_after_lab_00d551d4
                                                                                                                end
                                                                                                            end
                                                                                                            ::FLOW_after_lab_00d551d4::
                                                                                                            ::LAB_00d55cba::
                                                                                                            quest:PauseAllNonScriptedEntities((uVar18 ~= 0))
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
                                                                                                        quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                        me:ClearCommands()
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if not bVar3 then
                                                                                                                xStack_24 = resources:ScriptThing(xStack_17c)
                                                                                                                pCVar5 = xStack_24
                                                                                                                r26 = quest:GetHealth(pCVar5)
                                                                                                                fVar2 = 0.0
                                                                                                                if fVar2 < fret_03 then
                                                                                                                    uVar16 = 0
                                                                                                                    uVar17 = 0
                                                                                                                    uVar18 = 0
                                                                                                                    uVar19 = 0
                                                                                                                    iVar23 = 1
                                                                                                                    iVar8 = 0
                                                                                                                    iVar7 = 0
                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    r27 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                    while cVar4 do
                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then
                                                                                                                            quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                                            goto LAB_00d55c2b
                                                                                                                        end
                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                        cVar4 = iVar7
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55cd5 end
                                                                                                                end
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", xStack_8c)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then
                                                                                                                        quest:PauseAllNonScriptedEntities((uVar18 ~= 0))
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
                                                                                                                            quest:FadeScreenOut(*(this + 4), 0.5)
                                                                                                                            quest:Pause(*(this + 4))
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                            goto LAB_00d5595a
                                                                                                                        end
                                                                                                                        quest:PauseAllNonScriptedEntities((uVar17 ~= 0))
                                                                                                                        goto LAB_00d55c2b
                                                                                                                    end
                                                                                                                    if not bVar3 then
                                                                                                                        xStack_c = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = xStack_c
                                                                                                                        r28 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_04 then
                                                                                                                            uVar16 = 0
                                                                                                                            uVar17 = 0
                                                                                                                            uVar18 = 0
                                                                                                                            uVar19 = 0
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r29 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then
                                                                                                                                    quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
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
                                                                                                                        uVar16 = 1
                                                                                                                        uVar17 = 0
                                                                                                                        uVar18 = 0
                                                                                                                        uVar19 = 0
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0x3f800000
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                        -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                                        goto LAB_00d5595a
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            ::LAB_00d55cd5::
                                                                                                            quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d555f3: (native jump target)
                                                                                                            quest:PauseAllNonScriptedEntities((uVar18 ~= 0))
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        pCVar5 = resources:ScriptThing(xStack_17c)
                                                                                                        pCVar5 = pCVar5
                                                                                                        r30 = quest:GetHealth(pCVar5)
                                                                                                        fVar2 = 0.0
                                                                                                        if fVar2 < fret_02 then
                                                                                                            uVar16 = 0
                                                                                                            uVar17 = 0
                                                                                                            uVar18 = 0
                                                                                                            uVar19 = 0
                                                                                                            iVar23 = 1
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 0
                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD"
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            r31 = me:Speak(pCVar6, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            cVar4 = iVar7
                                                                                                            while cVar4 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then
                                                                                                                    quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                                    goto LAB_00d55c2b
                                                                                                                end
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then
                                                                                                                quest:PauseAllNonScriptedEntities((uVar18 ~= 0))
                                                                                                                goto LAB_00d55c2b
                                                                                                            end
                                                                                                        end
                                                                                                        ::LAB_00d5595a::
                                                                                                        quest:PauseAllNonScriptedEntities((uVar17 ~= 0))
                                                                                                    end
                                                                                                    if c_stk_169 ~= 0 then
                                                                                                        fVar20 = 5.5
                                                                                                        pCVar5 = quest:GetHero()
                                                                                                        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar20)
                                                                                                        __native_condition_4 = bVar3
                                                                                                        if __native_condition_4 then
                                                                                                            __native_condition_4 = iVar7 < 1
                                                                                                        end
                                                                                                        __native_condition_3 = __native_condition_4
                                                                                                        if __native_condition_3 then
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            __native_condition_3 = not iVar7
                                                                                                        end
                                                                                                        if __native_condition_3 then
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
                                                                                                                if xStack_18c_3 == 0x1 then
                                                                                                                    bVar3 = false
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                                    -- LAB_00d55b4e: (native jump target)
                                                                                                                elseif xStack_18c_3 == 0x2 then
                                                                                                                    bVar3 = false
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, pCVar5, false)
                                                                                                                    goto FLOW_after_lab_00d55b4e
                                                                                                                end
                                                                                                                ::FLOW_after_lab_00d55b4e::
                                                                                                                -- TODO(native): xStack_18c_3 = (CCharString)(1 - (int)xStack_18c_3);
                                                                                                            else
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b end
                                                                                                                bVar3 = false
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, pCVar5, xStack_130)
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
                                                                                                            c_stk_161 = '\x01'
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
                                                                -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_18c);
                                                                -- TODO(native): CTimer::CTimer((CTimer *)&xStack_180);
                                                                quest:SetTimer(xStack_188, xStack_180)
                                                                iVar7 = (xStack_160 ~= nil and xStack_160:IsAlive())
                                                                cVar4 = iVar7
                                                                while cVar4 do
                                                                    alive = quest:NewScriptFrame(me)
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00d55c2b_c26 end
                                                                    iVar7 = quest:GetTimer(xStack_188)
                                                                    if iVar7 < 1 then
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then goto LAB_00d55c2b_c26 end
                                                                        iVar8 = quest:AddNewConversation(me, false, false)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddPersonToConversation(iVar8, pCVar6)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, pCVar6, false)
                                                                        quest:SetTimer(xStack_188, xStack_180)
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
                                                                    uVar16 = SUB41(xStack_14c,0)
                                                                    uVar17 = (xStack_14c >> 8)
                                                                    uVar18 = (xStack_14c >> 0x10)
                                                                    uVar19 = (xStack_14c >> 0x18)
                                                                    pCVar6 = quest:GetHero()
                                                                    bVar3 = me:AcquireControl(4)
                                                                    while not bVar3 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then return end  -- TODO(native): goto LAB_00d54dfa_c26
                                                                        iVar8 = 4
                                                                        uVar16 = SUB41(xStack_14c,0)
                                                                        uVar17 = (xStack_14c >> 8)
                                                                        uVar18 = (xStack_14c >> 0x10)
                                                                        uVar19 = (xStack_14c >> 0x18)
                                                                        pCVar6 = quest:GetHero()
                                                                        bVar3 = me:AcquireControl(4)
                                                                    end
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then
                                                                        -- LAB_00d54dfa_c26: (native jump target)
                                                                    else
                                                                        pCVar5 = resources:NewActorMap()
                                                                        pCVar21 = xStack_124
                                                                        -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](( map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)(xStack_114_2 + 4),&xStack_18c);
                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar9,pCVar21);
                                                                        pppuVar22 = xStack_17c
                                                                        -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](( map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)(xStack_114_2 + 4),&xStack_18c);
                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar9,pppuVar22);
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
                                                                                            bVar3 = (**(**(this + 4) + 0x1d8 ))(*(this + 4),0x1c)
                                                                                            if bVar3 then
                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                bVar3 = not alive
                                                                                                if not bVar3 then
                                                                                                    bVar3 = quest:MsgIsTutorialClickedPast()
                                                                                                    while not bVar3 do
                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                        bVar3 = quest:MsgIsTutorialClickedPast()
                                                                                                    end
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    bVar3 = not alive
                                                                                                    if not bVar3 then return end  -- TODO(native): goto LAB_00d54846_c26
                                                                                                end
                                                                                            else
                                                                                                -- LAB_00d54846_c26: (native jump target)
                                                                                                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                quest:MiniMapRemoveMarker(pCVar6)
                                                                                                pCVar6 = quest:GetThingWithScriptName("HUD_ORB_GREEN_SMALL")
                                                                                                quest:MiniMapAddMarker(pCVar6, "TheRealGuildmaster")
                                                                                                pCVar6 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                iVar23 = 1
                                                                                                uVar16 = 0
                                                                                                uVar17 = 0
                                                                                                uVar18 = 0
                                                                                                uVar19 = 0
                                                                                                iVar8 = 0
                                                                                                iVar7 = 0x3f800000
                                                                                                pCVar12 = pCVar6:GetPos()
                                                                                                me:MoveToPosition(pCVar12, iVar7, iVar8, CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))), (iVar23 ~= 0))
                                                                                                c_stk_169 = '\x01'
                                                                                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods", "")
                                                                                                c_stk_161 = 0
                                                                                                repeat
                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    bVar3 = not alive
                                                                                                    if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c_c26
                                                                                                    if not __native_entity_state:GetStateBool("WoodsEndPlayed") then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c26 end
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
                                                                                                        uVar16 = SUB41(xStack_108,0)
                                                                                                        uVar17 = (xStack_108 >> 8)
                                                                                                        uVar18 = (xStack_108 >> 0x10)
                                                                                                        uVar19 = (xStack_108 >> 0x18)
                                                                                                        pCVar6 = quest:GetHero()
                                                                                                        bVar3 = me:AcquireControl(4)
                                                                                                        while not bVar3 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c91_c26
                                                                                                            iVar8 = 4
                                                                                                            uVar16 = SUB41(xStack_108,0)
                                                                                                            uVar17 = (xStack_108 >> 8)
                                                                                                            uVar18 = (xStack_108 >> 0x10)
                                                                                                            uVar19 = (xStack_108 >> 0x18)
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            bVar3 = me:AcquireControl(4)
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d55c91_c26: (native jump target)
                                                                                                            goto LAB_00d55c2b_c26
                                                                                                        end
                                                                                                        xStack_13c = resources:NewActorMap()
                                                                                                        resources:SetActor(xStack_13c, "HERO", xStack_108)
                                                                                                        resources:SetActor(xStack_13c, "GUARD", xStack_17c)
                                                                                                        xStack_124 = resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        pCVar6 = 0x1
                                                                                                        quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                        quest:FixMovieSequenceCamera(true)
                                                                                                        resources:RunMacro(xStack_c4, xStack_13c, false, true)
                                                                                                        quest:GiveHeroYesNoQuestion("", "TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", xStack_7c)
                                                                                                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        while iVar7 < 0 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c26
                                                                                                            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            -- LAB_00d55c72_c26: (native jump target)
                                                                                                            quest:PauseAllNonScriptedEntities((iVar8 ~= 0))
                                                                                                            -- LAB_00d55c7f_c26: (native jump target)
                                                                                                            resources:DestroyActorMap(xStack_13c)
                                                                                                            -- TODO(native): goto LAB_00d55c91_c26
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if iVar7 == 1 then
                                                                                                            if bVar3 then
                                                                                                                -- LAB_00d55c63_c26: (native jump target)
                                                                                                                quest:PauseAllNonScriptedEntities(false)
                                                                                                                -- TODO(native): goto LAB_00d55c7f_c26
                                                                                                            end
                                                                                                            quest:FadeScreenOut(0.5, 0.5)
                                                                                                            quest:Pause(1.0)
                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                            -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                        else
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c26
                                                                                                            r1 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = r1
                                                                                                            r32 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fVar2 < fret_0 then
                                                                                                                uVar16 = 0
                                                                                                                uVar17 = 0
                                                                                                                uVar18 = 0
                                                                                                                uVar19 = 0
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r33 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c26
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c26
                                                                                                            end
                                                                                                            pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                            uVar16 = 1
                                                                                                            uVar17 = 0
                                                                                                            uVar18 = 0
                                                                                                            uVar19 = 0
                                                                                                            iVar23 = 0
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 0x3f800000
                                                                                                            pCVar12 = pCVar5:GetPos()
                                                                                                            me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                            -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                        end
                                                                                                        quest:FixMovieSequenceCamera(false)
                                                                                                        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                        resources:DestroyActorMap(xStack_13c)
                                                                                                    else
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            -- LAB_00d54f9c_c26: (native jump target)
                                                                                                            bVar3 = false
                                                                                                        else
                                                                                                            u_stk_128 = u_stk_128 | 1
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if not bVar3 then return end  -- TODO(native): goto LAB_00d54f9c_c26
                                                                                                            bVar3 = true
                                                                                                        end
                                                                                                        if (u_stk_128 & 1) ~= 0 then
                                                                                                            u_stk_128 = u_stk_128 & 0xfffffffe
                                                                                                        end
                                                                                                        if bVar3 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                            bVar3 = false
                                                                                                            if bVar3 ~= 0 then
                                                                                                            end
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if bVar3 then
                                                                                                                repeat
                                                                                                                    -- TODO(native): (**(code **)(**(int **)(this + 4) + 0x1c) )(*(int **)(this + 4));
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                                    bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                                until not (bVar3)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                            bVar3 = false
                                                                                                            if bVar3 ~= 0 then
                                                                                                            end
                                                                                                            bVar3 = me:AcquireControl(4)
                                                                                                            while not bVar3 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                                bVar3 = resources:TryAcquire(xStack_17c, me, 4)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                            xStack_14c = resources:StartMovie("")
                                                                                                            quest:StartMovieSequence()
                                                                                                            pCVar6 = 0x1
                                                                                                            quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                            xStack_30 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = xStack_30
                                                                                                            r34 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fret_00 <= fVar2 then
                                                                                                                -- LAB_00d551d4_c26: (native jump target)
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55c9f_c26(); goto LAB_00d55c2b_c26 end
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if iVar7 == 1 then
                                                                                                                        if bVar3 then
                                                                                                                            __region_LAB_00d55c9f_c26()
                                                                                                                            goto LAB_00d55c2b_c26
                                                                                                                        end
                                                                                                                        quest:FadeScreenOut(0.5, 0.5)
                                                                                                                        quest:Pause(1.0)
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                        -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                    else
                                                                                                                        if bVar3 then goto LAB_00d55cba_c26 end
                                                                                                                        x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_58
                                                                                                                        r35 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_01 then
                                                                                                                            uVar16 = 0
                                                                                                                            uVar17 = 0
                                                                                                                            uVar18 = 0
                                                                                                                            uVar19 = 0
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r36 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then __region_LAB_00d55c9f_c26(); goto LAB_00d55c2b_c26 end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then goto LAB_00d55cba_c26 end
                                                                                                                        end
                                                                                                                        pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                        uVar16 = 1
                                                                                                                        uVar17 = 0
                                                                                                                        uVar18 = 0
                                                                                                                        uVar19 = 0
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0x3f800000
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                        -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                                    goto LAB_00d55480_c26
                                                                                                                end
                                                                                                            else
                                                                                                                uVar16 = 0
                                                                                                                uVar17 = 0
                                                                                                                uVar18 = 0
                                                                                                                uVar19 = 0
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r37 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                                while cVar4 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55c9f_c26(); goto LAB_00d55c2b_c26 end
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then return end  -- TODO(native): goto LAB_00d551d4_c26
                                                                                                            end
                                                                                                            ::LAB_00d55cba_c26::
                                                                                                            quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                            goto LAB_00d55c2b_c26
                                                                                                        end
                                                                                                    end
                                                                                                    ::LAB_00d55480_c26::
                                                                                                    cVar4 = me:IsTalkedToByHero()
                                                                                                    if cVar4 then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                        xStack_f8 = resources:StartMovie("")
                                                                                                        quest:StartMovieSequence()
                                                                                                        pCVar6 = 0x1
                                                                                                        quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                        me:ClearCommands()
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if not bVar3 then
                                                                                                                xStack_24 = resources:ScriptThing(xStack_17c)
                                                                                                                pCVar5 = xStack_24
                                                                                                                r38 = quest:GetHealth(pCVar5)
                                                                                                                fVar2 = 0.0
                                                                                                                if fVar2 < fret_03 then
                                                                                                                    uVar16 = 0
                                                                                                                    uVar17 = 0
                                                                                                                    uVar18 = 0
                                                                                                                    uVar19 = 0
                                                                                                                    iVar23 = 1
                                                                                                                    iVar8 = 0
                                                                                                                    iVar7 = 0
                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    r39 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                    iVar7 = me:IsPerformingScriptTask()
                                                                                                                    cVar4 = iVar7
                                                                                                                    while cVar4 do
                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        bVar3 = not alive
                                                                                                                        if bVar3 then __region_LAB_00d555f3_c26(); goto LAB_00d55c2b_c26 end
                                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                                        cVar4 = iVar7
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d55cd5_c26(); goto LAB_00d55c2b_c26 end
                                                                                                                end
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_LOG_HERO_EXPERIENCEORBS", "TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", xStack_68)
                                                                                                                iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar7 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if bVar3 then __region_LAB_00d555f3_c26(); goto LAB_00d55c2b_c26 end
                                                                                                                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if not bVar3 then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    bVar3 = not alive
                                                                                                                    if iVar7 == 1 then
                                                                                                                        if not bVar3 then
                                                                                                                            quest:FadeScreenOut(*(this + 4), 0.5)
                                                                                                                            quest:Pause(*(this + 4))
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                            goto LAB_00d5595a_c26
                                                                                                                        end
                                                                                                                        __region_LAB_00d555f3_c26(); goto LAB_00d55c2b_c26
                                                                                                                    end
                                                                                                                    if not bVar3 then
                                                                                                                        xStack_c = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = xStack_c
                                                                                                                        r40 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_04 then
                                                                                                                            uVar16 = 0
                                                                                                                            uVar17 = 0
                                                                                                                            uVar18 = 0
                                                                                                                            uVar19 = 0
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r41 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                                            cVar4 = iVar7
                                                                                                                            while cVar4 do
                                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                                bVar3 = not alive
                                                                                                                                if bVar3 then __region_LAB_00d555f3_c26(); goto LAB_00d55c2b_c26 end
                                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                                cVar4 = iVar7
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            bVar3 = not alive
                                                                                                                            if bVar3 then __region_LAB_00d55cd5_c26(); goto LAB_00d55c2b_c26 end
                                                                                                                        end
                                                                                                                        pCVar5 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                                        uVar16 = 1
                                                                                                                        uVar17 = 0
                                                                                                                        uVar18 = 0
                                                                                                                        uVar19 = 0
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0x3f800000
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                        -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                                        goto LAB_00d5595a_c26
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            __region_LAB_00d55cd5_c26()
                                                                                                            goto LAB_00d55c2b_c26
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then
                                                                                                            __region_LAB_00d555f3_c26()
                                                                                                            goto LAB_00d55c2b_c26
                                                                                                        end
                                                                                                        pCVar5 = resources:ScriptThing(xStack_17c)
                                                                                                        pCVar5 = pCVar5
                                                                                                        r42 = quest:GetHealth(pCVar5)
                                                                                                        fVar2 = 0.0
                                                                                                        if fVar2 < fret_02 then
                                                                                                            uVar16 = 0
                                                                                                            uVar17 = 0
                                                                                                            uVar18 = 0
                                                                                                            uVar19 = 0
                                                                                                            iVar23 = 1
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 0
                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD"
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            r43 = me:Speak(pCVar6, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            cVar4 = iVar7
                                                                                                            while cVar4 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then __region_LAB_00d55cd5_c26(); goto LAB_00d55c2b_c26 end
                                                                                                                iVar7 = me:IsPerformingScriptTask()
                                                                                                                cVar4 = iVar7
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then __region_LAB_00d555f3_c26(); goto LAB_00d55c2b_c26 end
                                                                                                        end
                                                                                                        ::LAB_00d5595a_c26::
                                                                                                        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                    end
                                                                                                    if c_stk_169 ~= 0 then
                                                                                                        fVar20 = 5.5
                                                                                                        pCVar5 = quest:GetHero()
                                                                                                        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar20)
                                                                                                        __native_condition_6 = bVar3
                                                                                                        if __native_condition_6 then
                                                                                                            __native_condition_6 = iVar7 < 1
                                                                                                        end
                                                                                                        __native_condition_5 = __native_condition_6
                                                                                                        if __native_condition_5 then
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            __native_condition_5 = not iVar7
                                                                                                        end
                                                                                                        if __native_condition_5 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                            iVar8 = quest:AddNewConversation(me, false, false)
                                                                                                            pCVar5 = quest:GetHero()
                                                                                                            quest:AddPersonToConversation(iVar8, pCVar5)
                                                                                                            quest:SetTimer(xStack_188, 10)
                                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                                if xStack_18c_3 == 0x1 then
                                                                                                                    bVar3 = false
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                                    -- LAB_00d55b4e_c26: (native jump target)
                                                                                                                elseif xStack_18c_3 == 0x2 then
                                                                                                                    bVar3 = false
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, pCVar5, false)
                                                                                                                    -- TODO(native): goto LAB_00d55b4e_c26
                                                                                                                end
                                                                                                                -- TODO(native): xStack_18c_3 = (CCharString)(1 - (int)xStack_18c_3);
                                                                                                            else
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                                bVar3 = false
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, pCVar5, xStack_130)
                                                                                                            end
                                                                                                        end
                                                                                                    end
                                                                                                    if c_stk_161 == 0 then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        bVar3 = not alive
                                                                                                        if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                        iVar7 = me:IsPerformingScriptTask()
                                                                                                        if not iVar7 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then goto LAB_00d55c2b_c26 end
                                                                                                            bVar3 = false
                                                                                                            c_stk_161 = '\x01'
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
                                                                ::LAB_00d55c2b_c26::
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
                                            quest:SetTimer(xStack_188, 10)
                                            CVar10 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
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
                                                    quest:SetTimer(xStack_188, 10)
                                                end
                                                iVar7 = quest:GetTimer(xStack_188)
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
                                                    quest:SetTimer(xStack_188, xStack_184)
                                                    CVar10 = CVar10
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
                                                uVar16 = SUB41(xStack_14c,0)
                                                uVar17 = (xStack_14c >> 8)
                                                uVar18 = (xStack_14c >> 0x10)
                                                uVar19 = (xStack_14c >> 0x18)
                                                pCVar6 = quest:GetHero()
                                                bVar3 = me:AcquireControl(4)
                                                while not bVar3 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d53ff2_c27
                                                    iVar8 = 4
                                                    uVar16 = SUB41(xStack_14c,0)
                                                    uVar17 = (xStack_14c >> 8)
                                                    uVar18 = (xStack_14c >> 0x10)
                                                    uVar19 = (xStack_14c >> 0x18)
                                                    pCVar6 = quest:GetHero()
                                                    bVar3 = me:AcquireControl(4)
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then
                                                    -- LAB_00d53ff2_c27: (native jump target)
                                                    resources:DestroyMovie(xStack_14c)
                                                    resources:DestroyMovie(xStack_124)
                                                else
                                                    r44 = quest:GetThingWithScriptName("PreMeleeDummy")
                                                    if not (r44 ~= nil and not r44:IsNull()) then
                                                    else
                                                        puVar11 = r44:GetPos()
                                                    end
                                                    -- TODO(native): CStack_114._0_4_ = *puVar11;
                                                    -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                                    -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                                    -- TODO(native): CreateEffect is not a ForgeFSE binding
                                                    quest:CreateEffect(pCVar6, "SMASH_DUMMY_01", (pCVar5 + 4), "", 0.0, false, false)
                                                    quest:FadeOutAndKillEntity(r44, true, 1.0, true)
                                                    xStack_13c = resources:NewActorMap()
                                                    resources:SetActor(xStack_13c, "HERO", xStack_14c)
                                                    resources:SetActor(xStack_13c, "TEACHER", xStack_17c)
                                                    quest:FixMovieSequenceCamera(true)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", xStack_13c, false, false)
                                                    quest:PauseAllNonScriptedEntities(true)
                                                    pCVar6 = quest:CreateExperienceOrb(1, pCVar5)
                                                    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                                    quest:EntitySetCutsceneBehaviour(xStack_160, 2)
                                                    resources:RunMacro(xStack_18c, xStack_13c, false, true)
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
                                                                if bVar3 then goto LAB_00d55c34_c27 end
                                                                bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                -- LAB_00d5439e_c27: (native jump target)
                                                                -- TODO(native): CTimer::CTimer((CTimer *)&xStack_180);
                                                                quest:SetTimer(xStack_188, xStack_180)
                                                                iVar7 = (xStack_160 ~= nil and xStack_160:IsAlive())
                                                                cVar4 = iVar7
                                                                while cVar4 do
                                                                    alive = quest:NewScriptFrame(me)
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00d55c2b_c27 end
                                                                    iVar7 = quest:GetTimer(xStack_188)
                                                                    if iVar7 < 1 then
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then goto LAB_00d55c2b_c27 end
                                                                        iVar8 = quest:AddNewConversation(me, false, false)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddPersonToConversation(iVar8, pCVar6)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, pCVar6, false)
                                                                        quest:SetTimer(xStack_188, xStack_180)
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
                                                                    uVar16 = SUB41(xStack_14c,0)
                                                                    uVar17 = (xStack_14c >> 8)
                                                                    uVar18 = (xStack_14c >> 0x10)
                                                                    uVar19 = (xStack_14c >> 0x18)
                                                                    pCVar6 = quest:GetHero()
                                                                    bVar3 = me:AcquireControl(4)
                                                                    while not bVar3 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then return end  -- TODO(native): goto LAB_00d54dfa_c27
                                                                        iVar8 = 4
                                                                        uVar16 = SUB41(xStack_14c,0)
                                                                        uVar17 = (xStack_14c >> 8)
                                                                        uVar18 = (xStack_14c >> 0x10)
                                                                        uVar19 = (xStack_14c >> 0x18)
                                                                        pCVar6 = quest:GetHero()
                                                                        bVar3 = me:AcquireControl(4)
                                                                    end
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then
                                                                        -- LAB_00d54dfa_c27: (native jump target)
                                                                    else
                                                                        pCVar5 = resources:NewActorMap()
                                                                        pCVar21 = xStack_124
                                                                        -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](( map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)(xStack_114_2 + 4),&xStack_18c);
                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar9,pCVar21);
                                                                        pppuVar22 = xStack_17c
                                                                        -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](( map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)(xStack_114_2 + 4),&xStack_18c);
                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar9,pppuVar22);
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
                                                                                            bVar3 = (**(**(this + 4) + 0x1d8 ))(*(this + 4),0x1c)
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
                                                                                                pCVar6 = quest:GetThingWithScriptName("HUD_ORB_GREEN_SMALL")
                                                                                                quest:MiniMapAddMarker(pCVar6, "TheRealGuildmaster")
                                                                                                pCVar6 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                iVar23 = 1
                                                                                                uVar16 = 0
                                                                                                uVar17 = 0
                                                                                                uVar18 = 0
                                                                                                uVar19 = 0
                                                                                                iVar8 = 0
                                                                                                iVar7 = 0x3f800000
                                                                                                pCVar12 = pCVar6:GetPos()
                                                                                                me:MoveToPosition(pCVar12, iVar7, iVar8, CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))), (iVar23 ~= 0))
                                                                                                c_stk_169 = '\x01'
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
                                                                                                        uVar16 = SUB41(xStack_108,0)
                                                                                                        uVar17 = (xStack_108 >> 8)
                                                                                                        uVar18 = (xStack_108 >> 0x10)
                                                                                                        uVar19 = (xStack_108 >> 0x18)
                                                                                                        pCVar6 = quest:GetHero()
                                                                                                        bVar3 = me:AcquireControl(4)
                                                                                                        while not bVar3 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c91_c27
                                                                                                            iVar8 = 4
                                                                                                            uVar16 = SUB41(xStack_108,0)
                                                                                                            uVar17 = (xStack_108 >> 8)
                                                                                                            uVar18 = (xStack_108 >> 0x10)
                                                                                                            uVar19 = (xStack_108 >> 0x18)
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            bVar3 = me:AcquireControl(4)
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
                                                                                                        quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                        quest:FixMovieSequenceCamera(true)
                                                                                                        resources:RunMacro(xStack_c4, xStack_13c, false, true)
                                                                                                        quest:GiveHeroYesNoQuestion("", "TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", xStack_7c)
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
                                                                                                            quest:PauseAllNonScriptedEntities((iVar8 ~= 0))
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
                                                                                                            -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                        else
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c27
                                                                                                            r44 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = r44
                                                                                                            r45 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fVar2 < fret_0 then
                                                                                                                uVar16 = 0
                                                                                                                uVar17 = 0
                                                                                                                uVar18 = 0
                                                                                                                uVar19 = 0
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r46 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                            uVar16 = 1
                                                                                                            uVar17 = 0
                                                                                                            uVar18 = 0
                                                                                                            uVar19 = 0
                                                                                                            iVar23 = 0
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 0x3f800000
                                                                                                            pCVar12 = pCVar5:GetPos()
                                                                                                            me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                            -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                        end
                                                                                                        quest:FixMovieSequenceCamera(false)
                                                                                                        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                        resources:DestroyActorMap(xStack_13c)
                                                                                                    else
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            -- LAB_00d54f9c_c27: (native jump target)
                                                                                                            bVar3 = false
                                                                                                        else
                                                                                                            u_stk_128 = u_stk_128 | 1
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if not bVar3 then return end  -- TODO(native): goto LAB_00d54f9c_c27
                                                                                                            bVar3 = true
                                                                                                        end
                                                                                                        if (u_stk_128 & 1) ~= 0 then
                                                                                                            u_stk_128 = u_stk_128 & 0xfffffffe
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
                                                                                                                    -- TODO(native): (**(code **)(**(int **)(this + 4) + 0x1c) )(*(int **)(this + 4));
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
                                                                                                            bVar3 = me:AcquireControl(4)
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
                                                                                                            quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                            xStack_30 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = xStack_30
                                                                                                            r47 = quest:GetHealth(pCVar5)
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
                                                                                                                        -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                    else
                                                                                                                        if bVar3 then goto LAB_00d55cba_c27 end
                                                                                                                        x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_58
                                                                                                                        r48 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_01 then
                                                                                                                            uVar16 = 0
                                                                                                                            uVar17 = 0
                                                                                                                            uVar18 = 0
                                                                                                                            uVar19 = 0
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r49 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                                        uVar16 = 1
                                                                                                                        uVar17 = 0
                                                                                                                        uVar18 = 0
                                                                                                                        uVar19 = 0
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0x3f800000
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                        -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                                    goto LAB_00d55480_c27
                                                                                                                end
                                                                                                            else
                                                                                                                uVar16 = 0
                                                                                                                uVar17 = 0
                                                                                                                uVar18 = 0
                                                                                                                uVar19 = 0
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r50 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                            quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
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
                                                                                                        quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                        me:ClearCommands()
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if not bVar3 then
                                                                                                                xStack_24 = resources:ScriptThing(xStack_17c)
                                                                                                                pCVar5 = xStack_24
                                                                                                                r51 = quest:GetHealth(pCVar5)
                                                                                                                fVar2 = 0.0
                                                                                                                if fVar2 < fret_03 then
                                                                                                                    uVar16 = 0
                                                                                                                    uVar17 = 0
                                                                                                                    uVar18 = 0
                                                                                                                    uVar19 = 0
                                                                                                                    iVar23 = 1
                                                                                                                    iVar8 = 0
                                                                                                                    iVar7 = 0
                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    r52 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", xStack_8c)
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
                                                                                                                            quest:FadeScreenOut(*(this + 4), 0.5)
                                                                                                                            quest:Pause(*(this + 4))
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                            goto LAB_00d5595a_c27
                                                                                                                        end
                                                                                                                        __region_LAB_00d555f3_c27(); goto LAB_00d55c2b_c27
                                                                                                                    end
                                                                                                                    if not bVar3 then
                                                                                                                        xStack_c = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = xStack_c
                                                                                                                        r53 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_04 then
                                                                                                                            uVar16 = 0
                                                                                                                            uVar17 = 0
                                                                                                                            uVar18 = 0
                                                                                                                            uVar19 = 0
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r54 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                                        uVar16 = 1
                                                                                                                        uVar17 = 0
                                                                                                                        uVar18 = 0
                                                                                                                        uVar19 = 0
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0x3f800000
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                        -- TODO(native): xStack_150_2._3_1_ = 0;
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
                                                                                                        pCVar5 = resources:ScriptThing(xStack_17c)
                                                                                                        pCVar5 = pCVar5
                                                                                                        r55 = quest:GetHealth(pCVar5)
                                                                                                        fVar2 = 0.0
                                                                                                        if fVar2 < fret_02 then
                                                                                                            uVar16 = 0
                                                                                                            uVar17 = 0
                                                                                                            uVar18 = 0
                                                                                                            uVar19 = 0
                                                                                                            iVar23 = 1
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 0
                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD"
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            r56 = me:Speak(pCVar6, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                    end
                                                                                                    if c_stk_169 ~= 0 then
                                                                                                        fVar20 = 5.5
                                                                                                        pCVar5 = quest:GetHero()
                                                                                                        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar20)
                                                                                                        __native_condition_8 = bVar3
                                                                                                        if __native_condition_8 then
                                                                                                            __native_condition_8 = iVar7 < 1
                                                                                                        end
                                                                                                        __native_condition_7 = __native_condition_8
                                                                                                        if __native_condition_7 then
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            __native_condition_7 = not iVar7
                                                                                                        end
                                                                                                        if __native_condition_7 then
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
                                                                                                                if xStack_18c_3 == 0x1 then
                                                                                                                    bVar3 = false
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                                    -- LAB_00d55b4e_c27: (native jump target)
                                                                                                                elseif xStack_18c_3 == 0x2 then
                                                                                                                    bVar3 = false
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", me, pCVar5, false)
                                                                                                                    -- TODO(native): goto LAB_00d55b4e_c27
                                                                                                                end
                                                                                                                -- TODO(native): xStack_18c_3 = (CCharString)(1 - (int)xStack_18c_3);
                                                                                                            else
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c27 end
                                                                                                                bVar3 = false
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", me, pCVar5, xStack_130)
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
                                                                                                            c_stk_161 = '\x01'
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
                                                                if bVar3 then goto LAB_00d55c34_c27 end
                                                                bVar3 = quest:MsgIsGameInfoClickedPast()
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_18c);
                                                                -- TODO(native): goto LAB_00d5439e_c27
                                                            end
                                                        end
                                                    end
                                                end
                                                ::LAB_00d55c34_c27::
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
                        -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_18c);
                        -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                        -- TODO(native): CTimer::CTimer((CTimer *)&xStack_184);
                        quest:SetTimer(xStack_188, xStack_184)
                        quest:SetStateInt("PreMeleeMode", 1)
                        quest:SetStateInt("DummyHits", 0)
                        CVar10 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                        quest:DisplayQuestInfo(true)
                        iVar7 = quest:GetStateInt("DummyHits")
                        while iVar7 < 7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d55c3d_c28 end
                            quest:UpdateQuestInfoCounter(CVar10, quest:GetStateInt("DummyHits"), -1)
                            if 0x0 ~= quest:GetStateInt("DummyHits") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d55c3d_c28 end
                                -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                quest:SetTimer(xStack_188, 10)
                            end
                            iVar7 = quest:GetTimer(xStack_188)
                            if iVar7 < 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d55c3d_c28 end
                                iVar8 = quest:AddNewConversation(me, false, false)
                                pCVar6 = quest:GetHero()
                                quest:AddPersonToConversation(iVar8, pCVar6)
                                pCVar6 = quest:GetHero()
                                quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_HIT_DUMMY", me, pCVar6, false)
                                bVar3 = quest:IsXbox()
                                if bVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d55c3d_c28 end
                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP")
                                    bVar3 = quest:MsgIsGameInfoClickedPast()
                                    while not bVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d55c3d_c28 end
                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d55c3d_c28 end
                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP_PC")
                                    bVar3 = quest:MsgIsGameInfoClickedPast()
                                    while not bVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d55c3d_c28 end
                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                    end
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d55c3d_c28 end
                                quest:SetTimer(xStack_188, xStack_184)
                                CVar10 = CVar10
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
                            uVar16 = SUB41(xStack_14c,0)
                            uVar17 = (xStack_14c >> 8)
                            uVar18 = (xStack_14c >> 0x10)
                            uVar19 = (xStack_14c >> 0x18)
                            pCVar6 = quest:GetHero()
                            bVar3 = me:AcquireControl(4)
                            while not bVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then return end  -- TODO(native): goto LAB_00d53a0b_c28
                                iVar8 = 4
                                uVar16 = SUB41(xStack_14c,0)
                                uVar17 = (xStack_14c >> 8)
                                uVar18 = (xStack_14c >> 0x10)
                                uVar19 = (xStack_14c >> 0x18)
                                pCVar6 = quest:GetHero()
                                bVar3 = me:AcquireControl(4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                -- LAB_00d53a0b_c28: (native jump target)
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
                                        quest:DisplayGameInfo("")
                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                        while not bVar3 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00d55c3d_c28 end
                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if not bVar3 then
                                            -- LAB_00d53c7e_c28: (native jump target)
                                            quest:SetStateInt("PreMeleeMode", 2)
                                            quest:SetStateInt("DummyHits", 0)
                                            quest:SetTimer(xStack_188, 10)
                                            CVar10 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", 7, 1.0)
                                            quest:DisplayQuestInfo(true)
                                            iVar7 = quest:GetStateInt("DummyHits")
                                            while iVar7 < 7 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then goto LAB_00d55c3d_c28 end
                                                quest:UpdateQuestInfoCounter(CVar10, quest:GetStateInt("DummyHits"), -1)
                                                if 0x0 ~= quest:GetStateInt("DummyHits") then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d55c3d_c28 end
                                                    -- TODO(native): xStack_180 = *(CCharString *)(*(int *)(this + 0x14) + 0x50);
                                                    quest:SetTimer(xStack_188, 10)
                                                end
                                                iVar7 = quest:GetTimer(xStack_188)
                                                if iVar7 < 1 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d55c3d_c28 end
                                                    iVar8 = quest:AddNewConversation(me, false, false)
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddPersonToConversation(iVar8, pCVar6)
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", me, pCVar6, false)
                                                    bVar3 = quest:IsXbox()
                                                    if bVar3 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then goto LAB_00d55c3d_c28 end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        while not bVar3 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then goto LAB_00d55c3d_c28 end
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then goto LAB_00d55c3d_c28 end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                                                        bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        while not bVar3 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then goto LAB_00d55c3d_c28 end
                                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00d55c3d_c28 end
                                                    quest:SetTimer(xStack_188, xStack_184)
                                                    CVar10 = CVar10
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
                                                uVar16 = SUB41(xStack_14c,0)
                                                uVar17 = (xStack_14c >> 8)
                                                uVar18 = (xStack_14c >> 0x10)
                                                uVar19 = (xStack_14c >> 0x18)
                                                pCVar6 = quest:GetHero()
                                                bVar3 = me:AcquireControl(4)
                                                while not bVar3 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d53ff2_c28
                                                    iVar8 = 4
                                                    uVar16 = SUB41(xStack_14c,0)
                                                    uVar17 = (xStack_14c >> 8)
                                                    uVar18 = (xStack_14c >> 0x10)
                                                    uVar19 = (xStack_14c >> 0x18)
                                                    pCVar6 = quest:GetHero()
                                                    bVar3 = me:AcquireControl(4)
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then
                                                    -- LAB_00d53ff2_c28: (native jump target)
                                                    resources:DestroyMovie(xStack_14c)
                                                    resources:DestroyMovie(xStack_124)
                                                else
                                                    r57 = quest:GetThingWithScriptName("PreMeleeDummy")
                                                    if not (r57 ~= nil and not r57:IsNull()) then
                                                    else
                                                        puVar11 = r57:GetPos()
                                                    end
                                                    -- TODO(native): CStack_114._0_4_ = *puVar11;
                                                    -- TODO(native): CStack_114._4_4_ = puVar11[1];
                                                    -- TODO(native): CStack_114._8_4_ = puVar11[2];
                                                    -- TODO(native): CreateEffect is not a ForgeFSE binding
                                                    quest:CreateEffect(pCVar6, "SMASH_DUMMY_01", (pCVar5 + 4), "", 0.0, false, false)
                                                    quest:FadeOutAndKillEntity(r57, true, 1.0, true)
                                                    xStack_13c = resources:NewActorMap()
                                                    resources:SetActor(xStack_13c, "HERO", xStack_14c)
                                                    resources:SetActor(xStack_13c, "TEACHER", xStack_17c)
                                                    quest:FixMovieSequenceCamera(true)
                                                    resources:RunMacro("CS_GUILD_PREMELEE_PASSED_SETUP", xStack_13c, false, false)
                                                    quest:PauseAllNonScriptedEntities(true)
                                                    pCVar6 = quest:CreateExperienceOrb(1, pCVar5)
                                                    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)xStack_160, (int)&*(int *)(pCVar6 + 0x4));
                                                    quest:EntitySetCutsceneBehaviour(xStack_160, 2)
                                                    resources:RunMacro(xStack_18c, xStack_13c, false, true)
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
                                                                -- TODO(native): CTimer::CTimer((CTimer *)&xStack_180);
                                                                quest:SetTimer(xStack_188, xStack_180)
                                                                iVar7 = (xStack_160 ~= nil and xStack_160:IsAlive())
                                                                cVar4 = iVar7
                                                                while cVar4 do
                                                                    alive = quest:NewScriptFrame(me)
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00d55c2b_c28 end
                                                                    iVar7 = quest:GetTimer(xStack_188)
                                                                    if iVar7 < 1 then
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then goto LAB_00d55c2b_c28 end
                                                                        iVar8 = quest:AddNewConversation(me, false, false)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddPersonToConversation(iVar8, pCVar6)
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", me, pCVar6, false)
                                                                        quest:SetTimer(xStack_188, xStack_180)
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
                                                                    uVar16 = SUB41(xStack_14c,0)
                                                                    uVar17 = (xStack_14c >> 8)
                                                                    uVar18 = (xStack_14c >> 0x10)
                                                                    uVar19 = (xStack_14c >> 0x18)
                                                                    pCVar6 = quest:GetHero()
                                                                    bVar3 = me:AcquireControl(4)
                                                                    while not bVar3 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then return end  -- TODO(native): goto LAB_00d54dfa_c28
                                                                        iVar8 = 4
                                                                        uVar16 = SUB41(xStack_14c,0)
                                                                        uVar17 = (xStack_14c >> 8)
                                                                        uVar18 = (xStack_14c >> 0x10)
                                                                        uVar19 = (xStack_14c >> 0x18)
                                                                        pCVar6 = quest:GetHero()
                                                                        bVar3 = me:AcquireControl(4)
                                                                    end
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then
                                                                        -- LAB_00d54dfa_c28: (native jump target)
                                                                    else
                                                                        pCVar5 = resources:NewActorMap()
                                                                        pCVar21 = xStack_124
                                                                        -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](( map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)(xStack_114_2 + 4),&xStack_18c);
                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar9,pCVar21);
                                                                        pppuVar22 = xStack_17c
                                                                        -- TODO(native): pCVar9 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](( map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)(xStack_114_2 + 4),&xStack_18c);
                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar9,pppuVar22);
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
                                                                                            bVar3 = (**(**(this + 4) + 0x1d8 ))(*(this + 4),0x1c)
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
                                                                                                pCVar6 = quest:GetThingWithScriptName("HUD_ORB_GREEN_SMALL")
                                                                                                quest:MiniMapAddMarker(pCVar6, "TheRealGuildmaster")
                                                                                                pCVar6 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                iVar23 = 1
                                                                                                uVar16 = 0
                                                                                                uVar17 = 0
                                                                                                uVar18 = 0
                                                                                                uVar19 = 0
                                                                                                iVar8 = 0
                                                                                                iVar7 = 0x3f800000
                                                                                                pCVar12 = pCVar6:GetPos()
                                                                                                me:MoveToPosition(pCVar12, iVar7, iVar8, CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))), (iVar23 ~= 0))
                                                                                                c_stk_169 = '\x01'
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
                                                                                                        uVar16 = SUB41(xStack_108,0)
                                                                                                        uVar17 = (xStack_108 >> 8)
                                                                                                        uVar18 = (xStack_108 >> 0x10)
                                                                                                        uVar19 = (xStack_108 >> 0x18)
                                                                                                        pCVar6 = quest:GetHero()
                                                                                                        bVar3 = me:AcquireControl(4)
                                                                                                        while not bVar3 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c91_c28
                                                                                                            iVar8 = 4
                                                                                                            uVar16 = SUB41(xStack_108,0)
                                                                                                            uVar17 = (xStack_108 >> 8)
                                                                                                            uVar18 = (xStack_108 >> 0x10)
                                                                                                            uVar19 = (xStack_108 >> 0x18)
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            bVar3 = me:AcquireControl(4)
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
                                                                                                        quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                        quest:FixMovieSequenceCamera(true)
                                                                                                        resources:RunMacro(xStack_c4, xStack_13c, false, true)
                                                                                                        quest:GiveHeroYesNoQuestion("", "TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", xStack_7c)
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
                                                                                                            quest:PauseAllNonScriptedEntities((iVar8 ~= 0))
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
                                                                                                            -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                        else
                                                                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d55c72_c28
                                                                                                            r57 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = r57
                                                                                                            r58 = quest:GetHealth(pCVar5)
                                                                                                            fVar2 = 0.0
                                                                                                            if fVar2 < fret_0 then
                                                                                                                uVar16 = 0
                                                                                                                uVar17 = 0
                                                                                                                uVar18 = 0
                                                                                                                uVar19 = 0
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r59 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                            uVar16 = 1
                                                                                                            uVar17 = 0
                                                                                                            uVar18 = 0
                                                                                                            uVar19 = 0
                                                                                                            iVar23 = 0
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 0x3f800000
                                                                                                            pCVar12 = pCVar5:GetPos()
                                                                                                            me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                            -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                        end
                                                                                                        quest:FixMovieSequenceCamera(false)
                                                                                                        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                        resources:DestroyActorMap(xStack_13c)
                                                                                                    else
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            -- LAB_00d54f9c_c28: (native jump target)
                                                                                                            bVar3 = false
                                                                                                        else
                                                                                                            u_stk_128 = u_stk_128 | 1
                                                                                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if not bVar3 then return end  -- TODO(native): goto LAB_00d54f9c_c28
                                                                                                            bVar3 = true
                                                                                                        end
                                                                                                        if (u_stk_128 & 1) ~= 0 then
                                                                                                            u_stk_128 = u_stk_128 & 0xfffffffe
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
                                                                                                                    -- TODO(native): (**(code **)(**(int **)(this + 4) + 0x1c) )(*(int **)(this + 4));
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
                                                                                                            bVar3 = me:AcquireControl(4)
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
                                                                                                            quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                            xStack_30 = resources:ScriptThing(xStack_17c)
                                                                                                            pCVar5 = xStack_30
                                                                                                            r60 = quest:GetHealth(pCVar5)
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
                                                                                                                        -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                    else
                                                                                                                        if bVar3 then goto LAB_00d55cba_c28 end
                                                                                                                        x_stk_58 = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = x_stk_58
                                                                                                                        r61 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_01 then
                                                                                                                            uVar16 = 0
                                                                                                                            uVar17 = 0
                                                                                                                            uVar18 = 0
                                                                                                                            uVar19 = 0
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r62 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                                        uVar16 = 1
                                                                                                                        uVar17 = 0
                                                                                                                        uVar18 = 0
                                                                                                                        uVar19 = 0
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0x3f800000
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                        -- TODO(native): xStack_150_2._3_1_ = 0;
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                                    goto LAB_00d55480_c28
                                                                                                                end
                                                                                                            else
                                                                                                                uVar16 = 0
                                                                                                                uVar17 = 0
                                                                                                                uVar18 = 0
                                                                                                                uVar19 = 0
                                                                                                                iVar23 = 1
                                                                                                                iVar8 = 0
                                                                                                                iVar7 = 0
                                                                                                                pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                r63 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                            quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
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
                                                                                                        quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                                                                                                        me:ClearCommands()
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            bVar3 = not alive
                                                                                                            if not bVar3 then
                                                                                                                xStack_24 = resources:ScriptThing(xStack_17c)
                                                                                                                pCVar5 = xStack_24
                                                                                                                r64 = quest:GetHealth(pCVar5)
                                                                                                                fVar2 = 0.0
                                                                                                                if fVar2 < fret_03 then
                                                                                                                    uVar16 = 0
                                                                                                                    uVar17 = 0
                                                                                                                    uVar18 = 0
                                                                                                                    uVar19 = 0
                                                                                                                    iVar23 = 1
                                                                                                                    iVar8 = 0
                                                                                                                    iVar7 = 0
                                                                                                                    pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    r65 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_LOG_HERO_EXPERIENCEORBS", "TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", xStack_68)
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
                                                                                                                            quest:FadeScreenOut(*(this + 4), 0.5)
                                                                                                                            quest:Pause(*(this + 4))
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            -- TODO(native): xStack_17c = xStack_17c & 0xffffff;
                                                                                                                            goto LAB_00d5595a_c28
                                                                                                                        end
                                                                                                                        __region_LAB_00d555f3_c28(); goto LAB_00d55c2b_c28
                                                                                                                    end
                                                                                                                    if not bVar3 then
                                                                                                                        xStack_c = resources:ScriptThing(xStack_17c)
                                                                                                                        pCVar5 = xStack_c
                                                                                                                        r66 = quest:GetHealth(pCVar5)
                                                                                                                        fVar2 = 0.0
                                                                                                                        if fVar2 < fret_04 then
                                                                                                                            uVar16 = 0
                                                                                                                            uVar17 = 0
                                                                                                                            uVar18 = 0
                                                                                                                            uVar19 = 0
                                                                                                                            iVar23 = 1
                                                                                                                            iVar8 = 0
                                                                                                                            iVar7 = 0
                                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar5 = quest:GetHero()
                                                                                                                            r67 = me:Speak(pCVar5, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                                        uVar16 = 1
                                                                                                                        uVar17 = 0
                                                                                                                        uVar18 = 0
                                                                                                                        uVar19 = 0
                                                                                                                        iVar23 = 0
                                                                                                                        iVar8 = 0
                                                                                                                        iVar7 = 0x3f800000
                                                                                                                        pCVar12 = pCVar5:GetPos()
                                                                                                                        me:MoveToPosition(pCVar12, iVar7, iVar8, (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11(uVar17, uVar16))))
                                                                                                                        -- TODO(native): xStack_150_2._3_1_ = 0;
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
                                                                                                        pCVar5 = resources:ScriptThing(xStack_17c)
                                                                                                        pCVar5 = pCVar5
                                                                                                        r68 = quest:GetHealth(pCVar5)
                                                                                                        fVar2 = 0.0
                                                                                                        if fVar2 < fret_02 then
                                                                                                            uVar16 = 0
                                                                                                            uVar17 = 0
                                                                                                            uVar18 = 0
                                                                                                            uVar19 = 0
                                                                                                            iVar23 = 1
                                                                                                            iVar8 = 0
                                                                                                            iVar7 = 0
                                                                                                            pcVar15 = "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD"
                                                                                                            pCVar6 = quest:GetHero()
                                                                                                            r69 = me:Speak(pCVar6, pcVar15, iVar7, (iVar8 ~= 0), (iVar23 ~= 0), CONCAT13(uVar19,CONCAT12(uVar18,CONCAT11( uVar17,uVar16))))
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
                                                                                                        quest:PauseAllNonScriptedEntities((uVar19 ~= 0))
                                                                                                    end
                                                                                                    if c_stk_169 ~= 0 then
                                                                                                        fVar20 = 5.5
                                                                                                        pCVar5 = quest:GetHero()
                                                                                                        bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar20)
                                                                                                        __native_condition_10 = bVar3
                                                                                                        if __native_condition_10 then
                                                                                                            __native_condition_10 = iVar7 < 1
                                                                                                        end
                                                                                                        __native_condition_9 = __native_condition_10
                                                                                                        if __native_condition_9 then
                                                                                                            iVar7 = me:IsPerformingScriptTask()
                                                                                                            __native_condition_9 = not iVar7
                                                                                                        end
                                                                                                        if __native_condition_9 then
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
                                                                                                                if xStack_18c_3 == 0x1 then
                                                                                                                    bVar3 = false
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", me, pCVar5, false)
                                                                                                                    -- LAB_00d55b4e_c28: (native jump target)
                                                                                                                elseif xStack_18c_3 == 0x2 then
                                                                                                                    bVar3 = false
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                    pCVar5 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_LOG_COMBAT_LOCKINGON", me, pCVar5, false)
                                                                                                                    -- TODO(native): goto LAB_00d55b4e_c28
                                                                                                                end
                                                                                                                -- TODO(native): xStack_18c_3 = (CCharString)(1 - (int)xStack_18c_3);
                                                                                                            else
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                bVar3 = not alive
                                                                                                                if bVar3 then goto LAB_00d55c2b_c28 end
                                                                                                                bVar3 = false
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:EntitySetFacingAngleTowardsThing(me, pCVar5, bVar3)
                                                                                                                pCVar5 = quest:GetHero()
                                                                                                                quest:AddLineToConversation(iVar8, "TEXT_QST_LOG_COMBAT_PUNCHING", me, pCVar5, false)
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
                                                                                                            c_stk_161 = '\x01'
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
                                                                -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_18c);
                                                                -- TODO(native): goto LAB_00d5439e_c28
                                                            end
                                                        end
                                                    end
                                                end
                                                ::LAB_00d55c34_c28::
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
                                            if bVar3 then goto LAB_00d55c3d_c28 end
                                            bVar3 = quest:MsgIsGameInfoClickedPast()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if not bVar3 then
                                            -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)&xStack_18c);
                                            -- TODO(native): goto LAB_00d53c7e_c28
                                        end
                                    end
                                end
                            end
                        end
                        ::LAB_00d55c3d_c28::
                        goto FLOW_after_lab_00d536c0
                    end
                end
            end
            ::FLOW_after_lab_00d536c0::
        end
        ::FLOW_after_lab_00d533bb::
    end
    ::LAB_00d55c46::
    ::LAB_00d55c4f::
    resources:DestroyMovie(xStack_f8)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("ChatJumped", false)
    __native_entity_state:SetStateBool("WoodsEndPlayed", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

