-- Generated native draft: Madame. Review coverage report before use.
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
    local CVar2, __push1, __push10, __push11, __push12, __push13, __push14, __push15, __push2, __push3, __push4, __push5, __push6, __push7, __push8, __push9, aC_stk_80, au_stk_9c, au_stk_b8, au_stk_e8, bVar3, cVar4, ctr_140, fVar14, fVar19, iVar15, iVar16, iVar17, iVar5, iVar7, i_stk_19c, i_stk_1a8, native_arg_sequence_1, native_arg_sequence_2, p0, p5, pCVar10, pCVar6, pCVar8, pcVar18, pvVar9, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r3, r4, r5, r6, r7, r8, r9, this_00, uVar11, uVar20, xStack_120, xStack_124, xStack_124_b3, xStack_12c, xStack_134, xStack_148, xStack_148_2, xStack_15c, xStack_160, xStack_164, xStack_174, xStack_178, xStack_17c, xStack_90, xStack_a0, xStack_a4, xStack_bc, xStack_cc, xStack_dc, xStack_ec, xStack_f8_b3, x_stk_14, x_stk_18, x_stk_20, x_stk_24, x_stk_44, x_stk_50, x_stk_54, x_stk_5c, x_stk_6c, x_stk_74, x_stk_78, x_stk_80
    local alive = true
    x_stk_80 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    if not quest:GetStateBool("BecomeNunnery") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        i_stk_19c = 1
        quest:SetThingHasInformation(me, true, false, false)
    end
    uVar20 = 0
    ctr_140 = nil
    -- TODO(native): xStack_174 = *(undefined ***)(this + 0x10);
    if xStack_174 ~= nil then
        -- TODO(native): *xStack_174 = *xStack_174 + 1;
    end
    quest:SetIsPushableByHero(nil --[[missing]], (uVar20 ~= 0))
    i_stk_1a8 = 0
    xStack_17c = p0
    quest:EntitySetAsKillable(me, false, false)
    -- TODO(native): xStack_124._0_3_ = 0;
    -- TODO(native): xStack_124_b3 = '\0';
    xStack_15c = p0
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    xStack_174 = resources:NewResource()
    resources:PrepareResource(xStack_174)
    xStack_164 = p0
    cVar4 = me:AcquireControl(4)
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e3e301 end
        xStack_12c = p0
        cVar4 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00e3e301 end
    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)(*(int *)(this + 0x14) + 0x94),xStack_174);
    iVar5 = quest:RegisterTimer()
    i_stk_1a8 = iVar5
    quest:SetTimer(iVar5, 2)
    if not __native_entity_state:GetStateBool("DoneIntro") then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            xStack_160 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            bVar3 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    pcVar18 = "TEXT_CS_B13_INTRO_HEROLADY_10"
                    goto LAB_00e3bde9
                end
                goto FLOW_hoist_lab_00e3bde9_1
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    pcVar18 = "TEXT_CS_B13_INTRO_10"
                    goto LAB_00e3bde9
                end
                quest:PauseAllNonScriptedEntities(false)
                pCVar10 = xStack_160
            end
            goto FLOW_past_lab_00e3bde9
            ::LAB_00e3bde9::
            -- TODO(native): pCVar6 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 0xb4),&xStack_150);
            -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
            __native_entity_state:SetStateBool("DoneIntro", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_160)
            goto LAB_00e3be37
            ::FLOW_hoist_lab_00e3bde9_1::
            quest:PauseAllNonScriptedEntities(false)
            pCVar10 = xStack_160
            ::FLOW_past_lab_00e3bde9::
            goto LAB_00e3e2ea
        end
    else
        goto LAB_00e3be37
    end
    goto FLOW_past_lab_00e3be37
    ::LAB_00e3be37::
    cVar4 = quest:GetStateBool("PlayerOwned")
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        iVar5 = i_stk_1a8
        if bVar3 then goto LAB_00e3e2ef end
        iVar7 = quest:GetTimer(i_stk_1a8)
        if iVar7 == 0 then
            fVar19 = 8.0
            pCVar8 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar19)
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00e3e2ef end
                quest:SetTimer(iVar5, 4)
                __push1 = quest:GetHero()
                xStack_12c = p0
                quest:EntitySetFacingAngleTowardsThing(me, __push1, false)
            end
        end
        bVar3 = me:MsgIsPresentedWithItem()
        if bVar3 then
            if xStack_178 == nil then
                bVar3 = false
                if bVar3 then
                    goto LAB_00e3bf28
                end
            else
                iVar5 = ((xStack_120 == "OBJECT_BEER_TANKARD") and 0 or 1)
                -- TODO(native): xStack_124_b3 = !(iVar5 != 0);
                if xStack_124_b3 ~= 0 then goto LAB_00e3bf28 end
            end
            goto FLOW_past_lab_00e3bf28
            ::LAB_00e3bf28::
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e3e2ef end
            xStack_dc = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            au_stk_9c = resources:ScriptThing(xStack_124)
            pCVar8 = au_stk_9c
            fVar14 = quest:GetHealth(pCVar8)
            CVar2 = xStack_164
            if fVar14 <= 0.0 then
                -- TODO(native): xStack_164 = CVar2 & 0xffffff;
            end
            if 1 ~= 0 then
                iVar17 = 0
                iVar16 = 1
                iVar15 = 0
                iVar7 = 0
                pcVar18 = "TEXT_QST_B13_MADAME_GIVEN_BEER"
                iVar5 = quest:GetHero()
                r1 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                iVar5 = me:IsPerformingScriptTask()
                cVar4 = iVar5
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_cc)
                        quest:DeregisterTimer(xStack_160)
                        resources:DestroyMovie(xStack_160)
                        return
                    end
                    iVar5 = me:IsPerformingScriptTask()
                    cVar4 = iVar5
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00e3e2ea
                end
            end
            quest:GiveHeroObject("OBJECT_BEER_TANKARD", -1, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_cc)
            ::FLOW_past_lab_00e3bf28::
        end
        xStack_124_b3 = me:IsTalkedToByHero()
        if xStack_124_b3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e3e2ef end
            native_arg_sequence_1 = false
            -- TODO(native): if (*(this + 0x14))[0x49] == nil then
            if false then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if native_arg_sequence_1 then
                bVar3 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                if bVar3 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00e3e2ef end
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                __push2 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, __push2, false)
                quest:Pause(1.0)
                __push3 = quest:GetHero()
                xStack_174 = p0
                quest:EntitySetFacingAngleTowardsThing(me, __push3, true)
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0x1, 1)
                au_stk_b8 = resources:ScriptThing(xStack_174)
                pCVar8 = au_stk_b8
                fVar14 = quest:GetHealth(pCVar8)
                cVar4 = 0.0 < fVar14
                if cVar4 then
                    iVar17 = 0
                    iVar16 = 1
                    iVar15 = 0
                    iVar7 = 0
                    pcVar18 = "TEXT_QST_B13_MADAME_WORK_QUESTION"
                    iVar5 = quest:GetHero()
                    r2 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                    iVar5 = me:IsPerformingScriptTask()
                    cVar4 = iVar5
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e3d063 end
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e3d473 end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_B13_MADAME_WORK_QUESTION_TEXT", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                while iVar5 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e3d063 end
                    iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00e3d473 end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if iVar5 == 1 then
                    if bVar3 then goto LAB_00e3d063 end
                    x_stk_18 = resources:ScriptThing(xStack_148)
                    iVar7 = x_stk_18
                    fVar14 = quest:GetHealth(iVar7)
                    cVar4 = 0.0 < fVar14
                    if cVar4 then
                        iVar17 = 0
                        iVar16 = 1
                        iVar15 = 0
                        iVar7 = 0
                        pcVar18 = "TEXT_QST_B13_MADAME_HERO_JOINED"
                        iVar5 = quest:GetHero()
                        r3 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e3d473 end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e3d063 end
                    end
                    quest:SetStateBool("HeroTricking", true)
                else
                    if bVar3 then goto LAB_00e3d473 end
                    x_stk_54 = resources:ScriptThing(xStack_148)
                    iVar7 = x_stk_54
                    fVar14 = quest:GetHealth(iVar7)
                    cVar4 = 0.0 < fVar14
                    if cVar4 then
                        iVar17 = 0
                        iVar16 = 1
                        iVar15 = 0
                        iVar7 = 0
                        pcVar18 = "TEXT_QST_B13_MADAME_HERO_DECLINED"
                        iVar5 = quest:GetHero()
                        r4 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e3d063 end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e3d473 end
                    end
                end
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00e3e2ef end
                __push4 = quest:GetHero()
                xStack_124_b3 = quest:IsObjectInThingsPossession("OBJECT_DEEDS_BORDELLO", __push4)
                if xStack_124_b3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        xStack_cc = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                        quest:SetStateBool("PlayerOwned", true)
                        pCVar8 = quest:GetThingWithScriptName("BordelloHouse")
                        quest:SetHouseOwnedByPlayer(pCVar8, true, true)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_cc)
                        goto LAB_00e3cab0
                    end
                    goto LAB_00e3e2ef
                end
                native_arg_sequence_2 = false
                -- TODO(native): if (*(this + 0x14))[0x49] == nil then
                if false then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
                if not native_arg_sequence_2 then
                    bVar3 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                    if not bVar3 then
                        native_arg_sequence_2 = true
                    else
                        native_arg_sequence_2 = false
                    end
                end
                if native_arg_sequence_2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e3e2ef end
                    xStack_a0 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    __push5 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, __push5, false)
                    quest:Pause(1.0)
                    __push6 = quest:GetHero()
                    xStack_174 = p0
                    quest:EntitySetFacingAngleTowardsThing(me, __push6, true)
                    alive = quest:NewScriptFrame(me)
                    quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0x1, fVar19)
                    if 5 < me then
                        i_stk_19c = 1
                    end
                    pCVar6 = tostring(i_stk_19c)
                    ("TEXT_QST_B13_MADAME_CHIT_CHAT_0" .. pCVar6)
                    au_stk_e8 = resources:ScriptThing(xStack_174)
                    pCVar8 = au_stk_e8
                    fVar14 = quest:GetHealth(pCVar8)
                    cVar4 = 0.0 < fVar14
                    if cVar4 then
                        iVar17 = 0
                        iVar16 = 1
                        iVar15 = 0
                        iVar7 = 0
                        iVar5 = quest:GetHero()
                        r5 = me:Speak(iVar5, pvVar9, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar10 = xStack_a0
                                goto LAB_00e3e2ea
                            end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar10 = xStack_a0
                            goto LAB_00e3e2ea
                        end
                    end
                    ctr_140 = ctr_140 + 1
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar10 = xStack_a0
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e3e2ef end
                    xStack_90 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    __push7 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, __push7, false)
                    quest:Pause(1.0)
                    __push8 = quest:GetHero()
                    xStack_174 = p0
                    quest:EntitySetFacingAngleTowardsThing(me, __push8, true)
                    alive = quest:NewScriptFrame(me)
                    quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0x1, i_stk_19c)
                    x_stk_80 = resources:ScriptThing(xStack_174)
                    pCVar8 = x_stk_80
                    fVar14 = quest:GetHealth(pCVar8)
                    cVar4 = 0.0 < fVar14
                    if cVar4 then
                        iVar17 = 0
                        iVar16 = 1
                        iVar15 = 0
                        iVar7 = 0
                        pcVar18 = "TEXT_QST_B13_MADAME_HERO_EARN_SOME"
                        iVar5 = quest:GetHero()
                        r6 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar10 = xStack_90
                                goto LAB_00e3e2ea
                            end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar10 = xStack_90
                            goto LAB_00e3e2ea
                        end
                    end
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar10 = xStack_90
                end
            end
            resources:ReleaseResource(pCVar10)
        end
        ::LAB_00e3cab0::
        uVar11 = uVar20 | 1
        bVar3 = me:MsgIsHitByHero()
        if bVar3 then
            goto LAB_00e3cb3c
        else
            uVar11 = uVar20 | 3
            bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar3 then
                uVar11 = uVar20 | 7
                bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar3 then goto LAB_00e3cb3c end
            end
            -- TODO(native): xStack_124_b3 = '\0';
        end
        goto FLOW_past_lab_00e3cb3c
        ::LAB_00e3cb3c::
        -- TODO(native): xStack_124_b3 = '\x01';
        ::FLOW_past_lab_00e3cb3c::
        if (uVar11 & 4) ~= 0 then
            uVar11 = uVar11 & 0xfffffffb
        end
        if (uVar11 & 2) ~= 0 then
            uVar11 = uVar11 & 0xfffffffd
        end
        if (uVar11 & 1) ~= 0 then
            uVar11 = uVar11 & 0xfffffffe
        end
        if xStack_124_b3 ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e3e2ef end
            xStack_ec = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            __push9 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, __push9, false)
            quest:Pause(1.0)
            __push10 = quest:GetHero()
            xStack_174 = p0
            quest:EntitySetFacingAngleTowardsThing(me, __push10, true)
            alive = quest:NewScriptFrame(me)
            iVar7 = -1
            quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, iVar7, 0x1)
            xStack_90 = resources:ScriptThing(xStack_174)
            pCVar8 = xStack_90
            fVar14 = quest:GetHealth(pCVar8)
            cVar4 = 0.0 < fVar14
            if cVar4 then
                p5 = 0
                iVar17 = 1
                iVar16 = 0
                iVar15 = 2
                pCVar6 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                pCVar6 = (a .. pCVar6)
                pvVar9 = pCVar6
                iVar5 = quest:GetHero()
                r7 = me:Speak(iVar5, pvVar9, iVar15, (iVar16 ~= 0), (iVar17 ~= 0), (p5 ~= 0))
                iVar5 = me:IsPerformingScriptTask()
                cVar4 = iVar5
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_ec)
                        quest:DeregisterTimer(iVar7)
                        resources:ReleaseResource(xStack_174)
                        return
                    end
                    iVar5 = me:IsPerformingScriptTask()
                    cVar4 = iVar5
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar10 = xStack_ec
                    goto LAB_00e3e2ea
                end
            end
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_ec)
        end
        uVar20 = uVar11
        cVar4 = quest:GetStateBool("PlayerOwned")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        while xStack_148_2 = iVar5, not bVar3 do
            iVar7 = quest:GetTimer(4)
            if iVar7 == 0 then
                fVar19 = 8.0
                pCVar8 = quest:GetHero()
                bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar19)
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                    quest:SetTimer(4, fVar19)
                    __push11 = quest:GetHero()
                    xStack_12c = p0
                    quest:EntitySetFacingAngleTowardsThing(me, __push11, false)
                end
            end
            xStack_124_b3 = me:IsTalkedToByHero()
            if xStack_124_b3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                xStack_160 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                __push12 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, __push12, false)
                quest:Pause(1.0)
                __push13 = quest:GetHero()
                xStack_174 = p0
                quest:EntitySetFacingAngleTowardsThing(me, __push13, true)
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(me, pCVar8, -1.0, 0x1, nil --[[missing]])
                if not quest:GetStateBool("BecomeNunnery") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e3d473 end
                    bVar3 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e3d063 end
                        if not quest:GetStateBool("HeroTricking") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e3d473 end
                            x_stk_74 = resources:ScriptThing(xStack_174)
                            iVar7 = x_stk_74
                            fVar14 = quest:GetHealth(iVar7)
                            cVar4 = 0.0 < fVar14
                            if cVar4 then
                                iVar17 = 0
                                iVar16 = 1
                                iVar15 = 0
                                iVar7 = 0
                                pcVar18 = "TEXT_QST_B13_MADAME_WORK_QUESTION"
                                iVar5 = quest:GetHero()
                                r8 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e3d063 end
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00e3d473 end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_B13_MADAME_WORK_QUESTION_TEXT", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar5 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00e3d063 end
                                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e3d473 end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if iVar5 == 1 then
                                if bVar3 then goto LAB_00e3d063 end
                                if not quest:GetStateBool("PlayerOwned") then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e3d063 end
                                    if not quest:GetStateBool("PlayerOwned") then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00e3d063 end
                                        x_stk_54 = resources:ScriptThing(xStack_148_2)
                                        iVar7 = x_stk_54
                                        fVar14 = quest:GetHealth(iVar7)
                                        cVar4 = 0.0 < fVar14
                                        if cVar4 then
                                            iVar17 = 0
                                            iVar16 = 1
                                            iVar15 = 0
                                            iVar7 = 0
                                            pcVar18 = "TEXT_QST_B13_MADAME_HERO_JOINED"
                                            iVar5 = quest:GetHero()
                                            r9 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar4 = iVar5
                                            while cVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then goto LAB_00e3d473 end
                                                iVar5 = me:IsPerformingScriptTask()
                                                cVar4 = iVar5
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00e3d063 end
                                        end
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00e3d473 end
                                        x_stk_24 = resources:ScriptThing(xStack_148_2)
                                        iVar7 = x_stk_24
                                        fVar14 = quest:GetHealth(iVar7)
                                        cVar4 = 0.0 < fVar14
                                        if cVar4 then
                                            iVar17 = 0
                                            iVar16 = 1
                                            iVar15 = 0
                                            iVar7 = 0
                                            pcVar18 = "TEXT_QST_B13_MADAME_HERO_JOINED_OWNED"
                                            iVar5 = quest:GetHero()
                                            r10 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar4 = iVar5
                                            while cVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then goto LAB_00e3d063 end
                                                iVar5 = me:IsPerformingScriptTask()
                                                cVar4 = iVar5
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if not bVar3 then
                                                quest:SetStateBool("HeroTricking", true)
                                                goto LAB_00e3dfb3
                                            end
                                            goto LAB_00e3d473
                                        end
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e3d473 end
                                    x_stk_6c = resources:ScriptThing(xStack_148_2)
                                    iVar7 = x_stk_6c
                                    fVar14 = quest:GetHealth(iVar7)
                                    cVar4 = 0.0 < fVar14
                                    if cVar4 then
                                        iVar17 = 0
                                        iVar16 = 1
                                        iVar15 = 0
                                        iVar7 = 0
                                        pcVar18 = "TEXT_QST_B13_MADAME_HERO_JOINED_OWNED"
                                        iVar5 = quest:GetHero()
                                        r11 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                        while cVar4 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00e3d063 end
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar4 = iVar5
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00e3d473 end
                                    end
                                end
                                quest:SetStateBool("HeroTricking", true)
                            else
                                if bVar3 then goto LAB_00e3d473 end
                                x_stk_18 = resources:ScriptThing(xStack_15c)
                                iVar7 = x_stk_18
                                fVar14 = quest:GetHealth(iVar7)
                                cVar4 = 0.0 < fVar14
                                if cVar4 then
                                    iVar17 = 0
                                    iVar16 = 1
                                    iVar15 = 0
                                    iVar7 = 0
                                    pcVar18 = "TEXT_QST_B13_MADAME_HERO_DECLINED"
                                    iVar5 = quest:GetHero()
                                    r12 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00e3d063 end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    goto LAB_00e3dfa4
                                end
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e3d063 end
                            x_stk_44 = resources:ScriptThing(xStack_148_2)
                            iVar7 = x_stk_44
                            fVar14 = quest:GetHealth(iVar7)
                            cVar4 = 0.0 < fVar14
                            if cVar4 then
                                iVar17 = 0
                                iVar16 = 1
                                iVar15 = 0
                                iVar7 = 0
                                pcVar18 = "TEXT_QST_B13_MADAME_HERO_EARN_SOME"
                                iVar5 = quest:GetHero()
                                r13 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e3d473 end
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00e3d063 end
                            end
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e3d473 end
                        if not quest:GetStateBool("BecomeNunnery") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e3d473 end
                            if not __native_entity_state:GetStateBool("MentionedNunnery") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00e3d063 end
                                __native_entity_state:SetStateBool("MentionedNunnery", true)
                                x_stk_20 = resources:ScriptThing(xStack_174)
                                iVar7 = x_stk_20
                                fVar14 = quest:GetHealth(iVar7)
                                cVar4 = 0.0 < fVar14
                                if cVar4 then
                                    iVar17 = 0
                                    iVar16 = 1
                                    iVar15 = 0
                                    iVar7 = 0
                                    pcVar18 = "TEXT_QST_B13_MADAME_BECOME_NUNNERY"
                                    iVar5 = quest:GetHero()
                                    r14 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00e3d473 end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e3d063 end
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00e3d473 end
                                x_stk_14 = resources:ScriptThing(xStack_15c)
                                iVar7 = x_stk_14
                                fVar14 = quest:GetHealth(iVar7)
                                cVar4 = 0.0 < fVar14
                                if cVar4 then
                                    iVar17 = 0
                                    iVar16 = 1
                                    iVar15 = 0
                                    iVar7 = 2
                                    pcVar18 = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_REPEAT"
                                    iVar5 = quest:GetHero()
                                    r15 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00e3d063 end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e3d473 end
                                end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_B13_MADAME_BECOME_NUNNERY_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar5 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00e3d063 end
                                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e3d473 end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if iVar5 == 1 then
                                if bVar3 then goto LAB_00e3d063 end
                                aC_stk_80 = resources:ScriptThing(xStack_148_2)
                                iVar7 = aC_stk_80
                                fVar14 = quest:GetHealth(iVar7)
                                cVar4 = 0.0 < fVar14
                                if cVar4 then
                                    iVar17 = 0
                                    iVar16 = 1
                                    iVar15 = 0
                                    iVar7 = 0
                                    pcVar18 = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_SURE"
                                    iVar5 = quest:GetHero()
                                    r16 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00e3d473 end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e3d063 end
                                end
                                quest:GiveHeroYesNoQuestion("TEXT_QST_B13_MADAME_BECOME_NUNNERY_SURE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                                while iVar5 < 0 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e3d473 end
                                    iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00e3d063 end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if iVar5 == 1 then
                                    if bVar3 then goto LAB_00e3d473 end
                                    x_stk_78 = resources:ScriptThing(xStack_134)
                                    iVar7 = x_stk_78
                                    fVar14 = quest:GetHealth(iVar7)
                                    cVar4 = 0.0 < fVar14
                                    if cVar4 then
                                        iVar17 = 0
                                        iVar16 = 1
                                        iVar15 = 0
                                        iVar7 = 0
                                        pcVar18 = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_THANKS"
                                        iVar5 = quest:GetHero()
                                        r17 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                        while cVar4 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00e3d063 end
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar4 = iVar5
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00e3d473 end
                                    end
                                    quest:TakeObjectFromHero("OBJECT_DEEDS_BORDELLO")
                                    pCVar8 = quest:GetThingWithScriptName("BordelloHouse")
                                    quest:SetHouseOwnedByPlayer(pCVar8, (xStack_cc ~= 0), false)
                                    quest:SetStateBool("BecomeNunnery", true)
                                    quest:ClearThingHasInformation(me)
                                    quest:GiveHeroRenownPoints(100)
                                    quest:GiveHeroMorality(0.20000000298023224)
                                else
                                    if bVar3 then goto LAB_00e3d063 end
                                    xStack_a4 = resources:ScriptThing(xStack_134)
                                    iVar7 = xStack_a4
                                    fVar14 = quest:GetHealth(iVar7)
                                    cVar4 = 0.0 < fVar14
                                    if cVar4 then
                                        iVar17 = 0
                                        iVar16 = 1
                                        iVar15 = 0
                                        iVar7 = 0
                                        pcVar18 = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_RECONSIDER"
                                        iVar5 = quest:GetHero()
                                        r18 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                        while cVar4 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00e3d473 end
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar4 = iVar5
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00e3d063 end
                                    end
                                end
                            else
                                if bVar3 then goto LAB_00e3d473 end
                                xStack_cc = resources:ScriptThing(xStack_134)
                                iVar7 = xStack_cc
                                fVar14 = quest:GetHealth(iVar7)
                                cVar4 = 0.0 < fVar14
                                if cVar4 then
                                    iVar17 = 0
                                    iVar16 = 1
                                    iVar15 = 0
                                    iVar7 = 0
                                    pcVar18 = "TEXT_QST_B13_MADAME_BECOME_NUNNERY_DECLINED"
                                    iVar5 = quest:GetHero()
                                    r19 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00e3d063 end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    goto LAB_00e3dfa4
                                end
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e3d063 end
                            x_stk_5c = resources:ScriptThing(xStack_148_2)
                            iVar7 = x_stk_5c
                            fVar14 = quest:GetHealth(iVar7)
                            cVar4 = 0.0 < fVar14
                            if cVar4 then
                                iVar17 = 0
                                iVar16 = 1
                                iVar15 = 0
                                iVar7 = 0
                                pcVar18 = "TEXT_QST_B13_MADAME_NUNNERY_THANKYOU"
                                iVar5 = quest:GetHero()
                                r20 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e3d473 end
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00e3d063 end
                            end
                        end
                    end
                    goto FLOW_past_lab_00e3dfa4
                    ::LAB_00e3dfa4::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e3d473 end
                    ::FLOW_past_lab_00e3dfa4::
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e3d063 end
                    x_stk_50 = resources:ScriptThing(xStack_174)
                    iVar7 = x_stk_50
                    fVar14 = quest:GetHealth(iVar7)
                    cVar4 = 0.0 < fVar14
                    if cVar4 then
                        iVar17 = 0
                        iVar16 = 1
                        iVar15 = 0
                        iVar7 = 0
                        pcVar18 = "TEXT_QST_B13_MADAME_NUNNERY_THANKYOU"
                        iVar5 = quest:GetHero()
                        r21 = me:Speak(iVar5, pcVar18, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e3d473 end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e3d063 end
                    end
                end
                ::LAB_00e3dfb3::
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_15c)
            end
            uVar11 = uVar20 | 8
            cVar4 = me:MsgIsHitByHero()
            if not cVar4 then
                uVar11 = uVar20 | 0x18
                cVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if cVar4 then
                    uVar11 = uVar20 | 0x38
                    cVar4 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not cVar4 then goto LAB_00e3e06a end
                end
                -- TODO(native): xStack_f8_b3 = '\0';
            else
                goto LAB_00e3e06a
            end
            goto FLOW_past_lab_00e3e06a
            ::LAB_00e3e06a::
            -- TODO(native): xStack_f8_b3 = '\x01';
            ::FLOW_past_lab_00e3e06a::
            if (uVar11 & 0x20) ~= 0 then
                uVar11 = uVar11 & 0xffffffdf
            end
            if (uVar11 & 0x10) ~= 0 then
                uVar11 = uVar11 & 0xffffffef
            end
            if (uVar11 & 8) ~= 0 then
                uVar11 = uVar11 & 0xfffffff7
            end
            if xStack_f8_b3 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                xStack_bc = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                __push14 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, __push14, false)
                quest:Pause(1.0)
                __push15 = quest:GetHero()
                xStack_174 = p0
                quest:EntitySetFacingAngleTowardsThing(me, __push15, true)
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0x1, nil --[[missing]])
                xStack_ec = resources:ScriptThing(xStack_174)
                pCVar8 = xStack_ec
                fVar14 = quest:GetHealth(pCVar8)
                cVar4 = 0.0 < fVar14
                if cVar4 then
                    iVar17 = 0
                    iVar16 = 1
                    iVar15 = 0
                    iVar7 = 2
                    pvVar9 = this_00
                    iVar5 = quest:GetHero()
                    r22 = me:Speak(iVar5, pvVar9, iVar7, (iVar15 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                    iVar5 = me:IsPerformingScriptTask()
                    cVar4 = iVar5
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00e3e2e3
                        end
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        goto LAB_00e3e2e3
                    end
                    goto FLOW_past_lab_00e3e2e3
                    ::LAB_00e3e2e3::
                    pCVar10 = xStack_bc
                    goto LAB_00e3e2ea
                    ::FLOW_past_lab_00e3e2e3::
                end
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_bc)
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            uVar20 = uVar11
        end
    end
    ::FLOW_past_lab_00e3be37::
    goto LAB_00e3e2ef
    ::LAB_00e3d473::
    quest:PauseAllNonScriptedEntities(false)
    pCVar10 = xStack_15c
    goto LAB_00e3e2ea
    ::LAB_00e3d063::
    quest:PauseAllNonScriptedEntities(false)
    pCVar10 = xStack_15c
    ::LAB_00e3e2ea::
    resources:DestroyMovie(pCVar10)
    ::LAB_00e3e2ef::
    quest:DeregisterTimer(i_stk_1a8)
    ::LAB_00e3e301::
    resources:ReleaseResource(xStack_174)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("DoneIntro", false)
    __native_entity_state:SetStateBool("MentionedNunnery", false)
end

function OnPersist(quest, me, context)
    local doneIntro = quest:GetStateBool("DoneIntro") or false
    doneIntro = quest:PersistTransferBool(context, "DoneIntro", doneIntro)
    quest:SetStateBool("DoneIntro", doneIntro)
end

function OnPredicateFail(quest, me)
end

