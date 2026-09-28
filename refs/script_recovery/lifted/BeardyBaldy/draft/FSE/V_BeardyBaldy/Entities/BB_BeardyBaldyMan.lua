-- Generated native draft: BB_BeardyBaldyMan. Review coverage report before use.
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
    local __native_condition_1, bVar5, bVar6, bVar8, cVar7, fVar4, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, fret_08, fret_09, fret_10, fret_11, fret_12, fret_13, fret_14, fret_15, fret_16, fret_17, fret_18, fret_19, fret_20, fret_21, fret_22, iVar13, iVar14, iVar15, iVar16, native_arg_sequence_1, p0, pCVar10, pCVar11, pCVar9, pThing, pcVar12, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r3, r4, r5, r6, r7, r8, r9, this_00, uVar1, xStack_124, xStack_12c, xStack_138, xStack_140, xStack_148, xStack_150, xStack_154, xStack_158, xStack_16c, xStack_170, xStack_174, xStack_178, xStack_184, xStack_188, xStack_18c, xStack_19c, xStack_1ac, xStack_1bc, xStack_1c8, xStack_1cc, xStack_1d4, xStack_1e4, xStack_1e8, xStack_1fc, xStack_20c, x_stk_108, x_stk_114, x_stk_120, x_stk_18, x_stk_24, x_stk_30, x_stk_3c, x_stk_48, x_stk_54, x_stk_60, x_stk_6c, x_stk_78, x_stk_84, x_stk_90, x_stk_9c, x_stk_a8, x_stk_b4, x_stk_c, x_stk_c0, x_stk_cc, x_stk_d8, x_stk_e4, x_stk_f0, x_stk_fc
    local alive = true
    bVar5 = false
    bVar8 = false
    bVar6 = false
    xStack_20c = resources:NewResource()
    if this_00 == nil then
        this_00 = 0x0
    else
        -- TODO(native): xStack_1c8._4_4_ = *(undefined4 *)(this + 0xc);
        bVar5 = true
        bVar8 = true
        bVar6 = true
        xStack_1c8 = nil
        if xStack_1c8 ~= nil then
            -- TODO(native): *xStack_1c8 = *xStack_1c8 + 1;
        end
        pCVar9 = extraout_EAX
        pCVar9 = (a .. pCVar9)
        -- TODO(native): CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript> (this_00,pCVar9,0);
        -- TODO(native): *(code **)(this_00 + 0x34) = NScript::CV_BeardyBaldyScript::WatchForAttack;
        -- TODO(native): *(undefined4 *)(this_00 + 0x38) = uVar1;
    end
    -- TODO(native): CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_00,sectionName);
    if bVar6 then
    end
    if bVar8 then
    end
    if bVar5 then
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    repeat
        if bVar6 then
            -- LAB_00e53541: (native jump target)
            resources:ReleaseResource(xStack_20c)
            return
        end
        if not quest:GetStateBool("AttackedByHero") then
            iVar13 = quest:GetTimer(quest:GetStateInt("RandomSpeechTimer"))
            if iVar13 == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                xStack_1e8 = helper_E53CB0(quest, me)
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if not bVar6 then
                        bVar6 = false
                        pThing = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(pThing, me, bVar6)
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if not bVar6 then
                                iVar14 = quest:AddNewConversation(me, false, false)
                                pCVar11 = quest:GetHero()
                                quest:AddPersonToConversation(iVar14, pCVar11)
                                pCVar11 = quest:GetHero()
                                quest:AddLineToConversation(iVar14, xStack_1e8, me, pCVar11, false)
                                require("V_BeardyBaldy.native_quest_helpers").helper_E53C70(quest, me)
                                goto LAB_00e51417
                            end
                        end
                    end
                end
                resources:ReleaseResource(xStack_20c)
                return
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                resources:ReleaseResource(xStack_20c)
                return
            end
            xStack_19c = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:PrepareResource(xStack_20c)
            bVar6 = resources:TryAcquire(xStack_20c, me, 4)
            while not bVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_19c)
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                bVar6 = resources:TryAcquire(xStack_20c, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_19c)
                resources:ReleaseResource(xStack_20c)
                return
            end
            x_stk_84 = resources:ScriptThing(xStack_20c)
            pCVar10 = x_stk_84
            fret_0 = quest:GetHealth(pCVar10)
            fVar4 = 0.0
            if fVar4 < fret_0 then
                iVar16 = 0
                iVar15 = 1
                iVar14 = 0
                iVar13 = 2
                pcVar12 = "TEXT_QST_014_ATTACK"
                pCVar10 = quest:GetHero()
                r1 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                iVar13 = me:IsPerformingScriptTask()
                cVar7 = iVar13
                while cVar7 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_19c)
                        resources:ReleaseResource(xStack_20c)
                        return
                    end
                    iVar13 = me:IsPerformingScriptTask()
                    cVar7 = iVar13
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_19c)
                    resources:ReleaseResource(xStack_20c)
                    return
                end
            end
            resources:PrepareResource(xStack_20c)
            quest:SetStateBool("AttackedByHero", false)
            require("V_BeardyBaldy.native_quest_helpers").helper_E53C70(quest, me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_19c)
        end
        ::LAB_00e51417::
        iVar13 = quest:GetStateInt("QuestPhase")
        if iVar13 == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                resources:ReleaseResource(xStack_20c)
                return
            end
            if quest:GetStateBool("InitialisePhase") then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                iVar13 = me:GetPos()
                -- TODO(native): helper_E53F60(quest, me, *(this + 0x14), me, iVar13)
                quest:SetStateBool("InitialisePhase", false)
            end
            cVar7 = me:IsTalkedToByHero()
            if cVar7 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                xStack_1e4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(xStack_20c)
                bVar6 = resources:TryAcquire(xStack_20c, me, 4)
                while not bVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e53600 end
                    bVar6 = resources:TryAcquire(xStack_20c, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    if quest:GetStateBool("Phase1RequirementsComplete") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            x_stk_120 = resources:ScriptThing(xStack_20c)
                            pCVar10 = x_stk_120
                            fret_00 = quest:GetHealth(pCVar10)
                            fVar4 = 0.0
                            if fVar4 < fret_00 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 2
                                pcVar12 = "TEXT_QST_014_PHASE1_REMINDER"
                                pCVar10 = quest:GetHero()
                                r2 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e53600 end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e53600 end
                            end
                            goto LAB_00e51c43
                        end
                        goto LAB_00e53600
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if not bVar6 then
                        if not quest:GetStateBool("IntroComplete") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e53600 end
                            x_stk_6c = resources:ScriptThing(xStack_20c)
                            pCVar10 = x_stk_6c
                            fret_01 = quest:GetHealth(pCVar10)
                            fVar4 = 0.0
                            if fVar4 < fret_01 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_014_PHASE1_INTRO1"
                                pCVar10 = quest:GetHero()
                                r3 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e53600 end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e53600 end
                            end
                            quest:SetStateBool("IntroComplete", true)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e53600 end
                            x_stk_54 = resources:ScriptThing(xStack_20c)
                            pCVar10 = x_stk_54
                            fret_02 = quest:GetHealth(pCVar10)
                            fVar4 = 0.0
                            if fVar4 < fret_02 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 2
                                pcVar12 = "TEXT_QST_014_PHASE1_INTRO_REMINDER"
                                pCVar10 = quest:GetHero()
                                r4 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e53600 end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e53600 end
                            end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_014_QUEST_ON_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar13 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar13 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e53600 end
                            iVar13 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            require("V_BeardyBaldy.native_quest_helpers").helper_E53C70(quest, me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if iVar13 == 1 then
                                if bVar6 then
                                    -- LAB_00e51633: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_1e4)
                                    resources:ReleaseResource(xStack_20c)
                                    return
                                end
                                x_stk_3c = resources:ScriptThing(xStack_20c)
                                pCVar10 = x_stk_3c
                                fret_03 = quest:GetHealth(pCVar10)
                                fVar4 = 0.0
                                if fVar4 < fret_03 then
                                    iVar16 = 0
                                    iVar15 = 1
                                    iVar14 = 0
                                    iVar13 = 0
                                    pcVar12 = "TEXT_QST_014_PHASE1_INTRO2"
                                    pCVar10 = quest:GetHero()
                                    r5 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                    while cVar7 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e53600 end
                                        iVar13 = me:IsPerformingScriptTask()
                                        cVar7 = iVar13
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e53600 end
                                end
                                xStack_174 = quest:ReadGlobalGameDataString(0x450)
                                pCVar10 = quest:GetHero()
                                bVar6 = quest:IsWearingHairstyle(pCVar10, xStack_174)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar8 = not alive
                                if bVar6 then
                                    if bVar8 then goto LAB_00e53600 end
                                    xStack_12c = quest:ReadGlobalGameDataString(0x45c)
                                    quest:GiveHeroObject(xStack_12c, -1, false)
                                    xStack_124 = quest:ReadGlobalGameDataString(0x458)
                                    quest:SetStateString("RequiredHairdo", xStack_124)
                                else
                                    if bVar8 then goto LAB_00e53600 end
                                    xStack_16c = quest:ReadGlobalGameDataString(0x454)
                                    quest:GiveHeroObject(xStack_16c, -1, false)
                                    xStack_154 = quest:ReadGlobalGameDataString(0x450)
                                    quest:SetStateString("RequiredHairdo", xStack_154)
                                end
                                x_stk_24 = resources:ScriptThing(xStack_20c)
                                pCVar10 = x_stk_24
                                fret_04 = quest:GetHealth(pCVar10)
                                fVar4 = 0.0
                                if fVar4 < fret_04 then
                                    iVar16 = 0
                                    iVar15 = 1
                                    iVar14 = 0
                                    iVar13 = 0
                                    pcVar12 = "TEXT_QST_014_PHASE1_INTRO3"
                                    pCVar10 = quest:GetHero()
                                    r6 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                    while cVar7 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e53600 end
                                        iVar13 = me:IsPerformingScriptTask()
                                        cVar7 = iVar13
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e53600 end
                                end
                                quest:SetStateBool("Phase1RequirementsComplete", true)
                            else
                                if bVar6 then goto LAB_00e53600 end
                                x_stk_c = resources:ScriptThing(xStack_20c)
                                pCVar10 = x_stk_c
                                fret_05 = quest:GetHealth(pCVar10)
                                fVar4 = 0.0
                                if fVar4 < fret_05 then
                                    iVar16 = 0
                                    iVar15 = 1
                                    iVar14 = 0
                                    iVar13 = 2
                                    pcVar12 = "TEXT_QST_014_PHASE1_DENIED"
                                    pCVar10 = quest:GetHero()
                                    r7 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                    while cVar7 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e53600 end
                                        iVar13 = me:IsPerformingScriptTask()
                                        cVar7 = iVar13
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e53600 end
                                end
                            end
                            goto LAB_00e51c43
                        end
                    end
                    goto FLOW_past_lab_00e51c43
                    ::LAB_00e51c43::
                    resources:PrepareResource(xStack_20c)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_1e4)
                    goto LAB_00e51c6f
                    ::FLOW_past_lab_00e51c43::
                end
                ::LAB_00e53600::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_1e4)
                resources:ReleaseResource(xStack_20c)
                return
            end
            ::LAB_00e51c6f::
            if quest:GetStateBool("Phase1RequirementsComplete") then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                quest:SetStateInt("QuestPhase", 1)
                quest:SetStateBool("InitialisePhase", true)
            end
        elseif iVar13 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                resources:ReleaseResource(xStack_20c)
                return
            end
            if quest:GetStateBool("InitialisePhase") then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                iVar13 = me:GetPos()
                -- TODO(native): helper_E53F60(quest, me, *(this + 0x14), me, iVar13)
                quest:SetStateBool("InitialisePhase", false)
            end
            cVar7 = me:IsTalkedToByHero()
            if cVar7 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                xStack_1bc = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(xStack_20c)
                bVar6 = resources:TryAcquire(xStack_20c, me, 4)
                while not bVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_1bc)
                        resources:ReleaseResource(xStack_20c)
                        return
                    end
                    bVar6 = resources:TryAcquire(xStack_20c, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    -- LAB_00e53618: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_1bc)
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                if not quest:GetStateBool("Phase2RequirementsComplete") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_1bc)
                        resources:ReleaseResource(xStack_20c)
                        return
                    end
                    pCVar9 = quest:GetStateString("RequiredHairdo")
                    pCVar10 = quest:GetHero()
                    bVar6 = quest:IsWearingHairstyle(pCVar10, pCVar9)
                    if bVar6 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1bc)
                            resources:ReleaseResource(xStack_20c)
                            return
                        end
                        x_stk_48 = resources:ScriptThing(xStack_20c)
                        pCVar10 = x_stk_48
                        fret_07 = quest:GetHealth(pCVar10)
                        fVar4 = 0.0
                        if fVar4 < fret_07 then
                            iVar16 = 0
                            iVar15 = 1
                            iVar14 = 0
                            iVar13 = 0
                            pcVar12 = "TEXT_QST_014_PHASE2_INTRO1"
                            pCVar10 = quest:GetHero()
                            r8 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            iVar13 = me:IsPerformingScriptTask()
                            cVar7 = iVar13
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_1bc)
                                    resources:ReleaseResource(xStack_20c)
                                    return
                                end
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_1bc)
                                resources:ReleaseResource(xStack_20c)
                                return
                            end
                        end
                        xStack_150 = quest:ReadGlobalGameDataString(0x460)
                        pCVar10 = quest:GetHero()
                        bVar6 = quest:IsWearingHairstyle(pCVar10, xStack_150)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar8 = not alive
                        if bVar6 then
                            if bVar8 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_1bc)
                                resources:ReleaseResource(xStack_20c)
                                return
                            end
                            xStack_170 = quest:ReadGlobalGameDataString(0x46c)
                            quest:GiveHeroObject(xStack_170, -1, false)
                            xStack_148 = quest:ReadGlobalGameDataString(0x468)
                            -- TODO(native): quest:SetStateString("RequiredBeard", &xStack_148)
                        else
                            if bVar8 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_1bc)
                                resources:ReleaseResource(xStack_20c)
                                return
                            end
                            xStack_178 = quest:ReadGlobalGameDataString(0x464)
                            quest:GiveHeroObject(xStack_178, -1, false)
                            xStack_138 = quest:ReadGlobalGameDataString(0x460)
                            -- TODO(native): quest:SetStateString("RequiredBeard", &xStack_138)
                        end
                        x_stk_f0 = resources:ScriptThing(xStack_20c)
                        pCVar10 = x_stk_f0
                        fret_08 = quest:GetHealth(pCVar10)
                        fVar4 = 0.0
                        if fVar4 < fret_08 then
                            iVar16 = 0
                            iVar15 = 1
                            iVar14 = 0
                            iVar13 = 0
                            pcVar12 = "TEXT_QST_014_PHASE2_INTRO2"
                            pCVar10 = quest:GetHero()
                            r9 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            iVar13 = me:IsPerformingScriptTask()
                            cVar7 = iVar13
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_1bc)
                                    resources:ReleaseResource(xStack_20c)
                                    return
                                end
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_1bc)
                                resources:ReleaseResource(xStack_20c)
                                return
                            end
                        end
                        quest:SetStateBool("Phase2RequirementsComplete", true)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1bc)
                            resources:ReleaseResource(xStack_20c)
                            return
                        end
                        x_stk_78 = resources:ScriptThing(xStack_20c)
                        pCVar10 = x_stk_78
                        fret_09 = quest:GetHealth(pCVar10)
                        fVar4 = 0.0
                        if fVar4 < fret_09 then
                            iVar16 = 0
                            iVar15 = 1
                            iVar14 = 0
                            iVar13 = 2
                            pcVar12 = "TEXT_QST_014_PHASE2_WRONG_HAIR"
                            pCVar10 = quest:GetHero()
                            r10 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            iVar13 = me:IsPerformingScriptTask()
                            cVar7 = iVar13
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_1bc)
                                    resources:ReleaseResource(xStack_20c)
                                    return
                                end
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_1bc)
                                resources:ReleaseResource(xStack_20c)
                                return
                            end
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        -- LAB_00e51eb4: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_1bc)
                        resources:ReleaseResource(xStack_20c)
                        return
                    end
                    x_stk_108 = resources:ScriptThing(xStack_20c)
                    pCVar10 = x_stk_108
                    fret_06 = quest:GetHealth(pCVar10)
                    fVar4 = 0.0
                    if fVar4 < fret_06 then
                        iVar16 = 0
                        iVar15 = 1
                        iVar14 = 0
                        iVar13 = 2
                        pcVar12 = "TEXT_QST_014_PHASE2_REMINDER"
                        pCVar10 = quest:GetHero()
                        r11 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                        iVar13 = me:IsPerformingScriptTask()
                        cVar7 = iVar13
                        while cVar7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_1bc)
                                resources:ReleaseResource(xStack_20c)
                                return
                            end
                            iVar13 = me:IsPerformingScriptTask()
                            cVar7 = iVar13
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1bc)
                            resources:ReleaseResource(xStack_20c)
                            return
                        end
                    end
                end
                resources:PrepareResource(xStack_20c)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_1bc)
            end
            if quest:GetStateBool("Phase2RequirementsComplete") then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                quest:SetStateInt("QuestPhase", 2)
                quest:SetStateBool("InitialisePhase", true)
            end
        elseif iVar13 == 2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                resources:ReleaseResource(xStack_20c)
                return
            end
            if quest:GetStateBool("InitialisePhase") then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                iVar13 = me:GetPos()
                -- TODO(native): helper_E53F60(quest, me, *(this + 0x14), me, iVar13)
                quest:SetStateBool("InitialisePhase", false)
            end
            cVar7 = me:IsTalkedToByHero()
            if cVar7 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                xStack_1fc = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(xStack_20c)
                bVar6 = resources:TryAcquire(xStack_20c, me, 4)
                while not bVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e5276b end
                    bVar6 = resources:TryAcquire(xStack_20c, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00e5276b end
                if not quest:GetStateBool("Phase3RequirementsComplete") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e5276b end
                    -- TODO(native): bVar6 = helper_E53670(quest, me, *(this + 0x14))
                    bVar6 = nil --[[unresolved native value]]
                    if not bVar6 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            x_stk_18 = resources:ScriptThing(xStack_20c)
                            pCVar10 = x_stk_18
                            fret_11 = quest:GetHealth(pCVar10)
                            fVar4 = 0.0
                            if fVar4 < fret_11 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 2
                                pcVar12 = "TEXT_QST_014_PHASE3_NO_BEARD"
                                pCVar10 = quest:GetHero()
                                r12 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e5276b end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                end
                                goto LAB_00e52d54
                            end
                            goto LAB_00e52d63
                        end
                        goto LAB_00e5276b
                    end
                    pCVar9 = quest:GetStateString("RequiredHairdo")
                    pCVar10 = quest:GetHero()
                    bVar6 = quest:IsWearingHairstyle(pCVar10, pCVar9)
                    if not bVar6 then
                        goto LAB_00e52a37
                    end
                    goto FLOW_past_lab_00e52a37
                    ::LAB_00e52a37::
                    pCVar9 = quest:GetStateString("RequiredHairdo")
                    pCVar10 = quest:GetHero()
                    bVar6 = quest:IsWearingHairstyle(pCVar10, pCVar9)
                    if bVar6 then
                        pCVar9 = quest:GetStateString("RequiredBeard")
                        pCVar10 = quest:GetHero()
                        bVar6 = quest:IsWearingHairstyle(pCVar10, pCVar9)
                        if not bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if not bVar6 then
                                x_stk_90 = resources:ScriptThing(xStack_20c)
                                pCVar10 = x_stk_90
                                fret_16 = quest:GetHealth(pCVar10)
                                fVar4 = 0.0
                                if fVar4 < fret_16 then
                                    iVar16 = 0
                                    iVar15 = 1
                                    iVar14 = 0
                                    iVar13 = 2
                                    pcVar12 = "TEXT_QST_014_PHASE3_WRONG_BEARD"
                                    pCVar10 = quest:GetHero()
                                    r13 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                    while cVar7 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e5276b end
                                        iVar13 = me:IsPerformingScriptTask()
                                        cVar7 = iVar13
                                    end
                                    goto LAB_00e52d54
                                end
                                goto LAB_00e52d63
                            end
                            goto LAB_00e5276b
                        end
                    end
                    pCVar9 = quest:GetStateString("RequiredHairdo")
                    pCVar10 = quest:GetHero()
                    bVar6 = quest:IsWearingHairstyle(pCVar10, pCVar9)
                    if bVar6 then
                        goto LAB_00e52c5e
                    else
                        pCVar9 = quest:GetStateString("RequiredBeard")
                        pCVar10 = quest:GetHero()
                        bVar6 = quest:IsWearingHairstyle(pCVar10, pCVar9)
                        if not bVar6 then goto LAB_00e52c5e end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            -- LAB_00e52c43: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1fc)
                            resources:ReleaseResource(xStack_20c)
                            return
                        end
                        x_stk_114 = resources:ScriptThing(xStack_20c)
                        pCVar10 = x_stk_114
                        fret_17 = quest:GetHealth(pCVar10)
                        fVar4 = 0.0
                        if fVar4 < fret_17 then
                            iVar16 = 0
                            iVar15 = 1
                            iVar14 = 0
                            iVar13 = 2
                            pcVar12 = "TEXT_QST_014_PHASE3_WRONG_HAIR"
                            pCVar10 = quest:GetHero()
                            r14 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            iVar13 = me:IsPerformingScriptTask()
                            cVar7 = iVar13
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e5276b end
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e5276b end
                        end
                    end
                    goto FLOW_past_lab_00e52c5e
                    ::LAB_00e52c5e::
                    pCVar9 = quest:GetStateString("RequiredHairdo")
                    pCVar10 = quest:GetHero()
                    bVar6 = quest:IsWearingHairstyle(pCVar10, pCVar9)
                    if not bVar6 then
                        pCVar9 = quest:GetStateString("RequiredBeard")
                        pCVar10 = quest:GetHero()
                        bVar6 = quest:IsWearingHairstyle(pCVar10, pCVar9)
                        if not bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e5276b end
                            x_stk_fc = resources:ScriptThing(xStack_20c)
                            pCVar10 = x_stk_fc
                            fret_18 = quest:GetHealth(pCVar10)
                            fVar4 = 0.0
                            if fVar4 < fret_18 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 2
                                pcVar12 = "TEXT_QST_014_PHASE3_WRONG_HAIR_AND_BEARD"
                                pCVar10 = quest:GetHero()
                                r15 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e5276b end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                end
                                goto LAB_00e52d54
                            end
                        end
                    end
                    ::FLOW_past_lab_00e52c5e::
                    goto LAB_00e52d63
                    ::FLOW_past_lab_00e52a37::
                    goto FLOW_past_lab_00e52d63
                    ::LAB_00e52d63::
                    resources:PrepareResource(xStack_20c)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_1fc)
                    goto LAB_00e52d8f
                    ::FLOW_past_lab_00e52d63::
                    pCVar9 = quest:GetStateString("RequiredBeard")
                    pCVar10 = quest:GetHero()
                    bVar6 = quest:IsWearingHairstyle(pCVar10, pCVar9)
                    if not bVar6 then goto LAB_00e52a37 end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e5276b end
                    x_stk_c0 = resources:ScriptThing(xStack_20c)
                    pCVar10 = x_stk_c0
                    fret_12 = quest:GetHealth(pCVar10)
                    fVar4 = 0.0
                    if fVar4 < fret_12 then
                        iVar16 = 0
                        iVar15 = 1
                        iVar14 = 0
                        iVar13 = 0
                        pcVar12 = "TEXT_QST_014_PHASE3_INTRO1"
                        pCVar10 = quest:GetHero()
                        r16 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                        iVar13 = me:IsPerformingScriptTask()
                        cVar7 = iVar13
                        while cVar7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e5276b end
                            iVar13 = me:IsPerformingScriptTask()
                            cVar7 = iVar13
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00e5276b end
                    end
                    iVar13 = require("V_BeardyBaldy.native_quest_helpers").IsHeroWearingAnyTash(quest, me)
                    if not iVar13 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            x_stk_a8 = resources:ScriptThing(xStack_20c)
                            pCVar10 = x_stk_a8
                            fret_14 = quest:GetHealth(pCVar10)
                            fVar4 = 0.0
                            if fVar4 < fret_14 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_014_PHASE3_INTRO3"
                                pCVar10 = quest:GetHero()
                                r17 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e5276b end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e5276b end
                            end
                            goto LAB_00e52838
                        end
                        goto LAB_00e5276b
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if not bVar6 then
                        x_stk_60 = resources:ScriptThing(xStack_20c)
                        pCVar10 = x_stk_60
                        fret_13 = quest:GetHealth(pCVar10)
                        fVar4 = 0.0
                        if fVar4 < fret_13 then
                            iVar16 = 0
                            iVar15 = 1
                            iVar14 = 0
                            iVar13 = 0
                            pcVar12 = "TEXT_QST_014_PHASE3_INTRO2"
                            pCVar10 = quest:GetHero()
                            r18 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                            iVar13 = me:IsPerformingScriptTask()
                            cVar7 = iVar13
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e5276b end
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e5276b end
                        end
                        goto LAB_00e52838
                    end
                    goto FLOW_past_lab_00e52838
                    ::LAB_00e52838::
                    xStack_18c = quest:ReadGlobalGameDataString(0x470)
                    pCVar10 = quest:GetHero()
                    bVar6 = quest:IsWearingHairstyle(pCVar10, xStack_18c)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                    if bVar6 then
                        if bVar8 then goto LAB_00e5276b end
                        xStack_188 = quest:ReadGlobalGameDataString(0x47c)
                        quest:GiveHeroObject(xStack_188, -1, false)
                        xStack_184 = quest:ReadGlobalGameDataString(0x478)
                        -- TODO(native): quest:SetStateString("RequiredTash", &xStack_184)
                    else
                        if bVar8 then goto LAB_00e5276b end
                        xStack_140 = quest:ReadGlobalGameDataString(0x474)
                        quest:GiveHeroObject(xStack_140, -1, false)
                        xStack_158 = quest:ReadGlobalGameDataString(0x470)
                        -- TODO(native): quest:SetStateString("RequiredTash", &xStack_158)
                    end
                    x_stk_30 = resources:ScriptThing(xStack_20c)
                    pCVar10 = x_stk_30
                    fret_15 = quest:GetHealth(pCVar10)
                    fVar4 = 0.0
                    if fVar4 < fret_15 then
                        iVar16 = 0
                        iVar15 = 1
                        iVar14 = 0
                        iVar13 = 0
                        pcVar12 = "TEXT_QST_014_PHASE3_INTRO4"
                        pCVar10 = quest:GetHero()
                        r19 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                        iVar13 = me:IsPerformingScriptTask()
                        cVar7 = iVar13
                        while cVar7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e5276b end
                            iVar13 = me:IsPerformingScriptTask()
                            cVar7 = iVar13
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00e5276b end
                    end
                    quest:SetStateBool("Phase3RequirementsComplete", true)
                    goto LAB_00e52d63
                    ::FLOW_past_lab_00e52838::
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e5276b end
                    x_stk_d8 = resources:ScriptThing(xStack_20c)
                    pCVar10 = x_stk_d8
                    fret_10 = quest:GetHealth(pCVar10)
                    fVar4 = 0.0
                    if fret_10 <= fVar4 then
                        -- LAB_00e52d63_c38: (native jump target)
                        resources:PrepareResource(xStack_20c)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_1fc)
                        goto LAB_00e52d8f
                    end
                    iVar16 = 0
                    iVar15 = 1
                    iVar14 = 0
                    iVar13 = 2
                    pcVar12 = "TEXT_QST_014_PHASE3_REMINDER"
                    pCVar10 = quest:GetHero()
                    r20 = me:Speak(pCVar10, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                    iVar13 = me:IsPerformingScriptTask()
                    cVar7 = iVar13
                    while cVar7 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00e5276b end
                        iVar13 = me:IsPerformingScriptTask()
                        cVar7 = iVar13
                    end
                    goto LAB_00e52d54
                end
                goto FLOW_past_lab_00e52d54
                ::LAB_00e52d54::
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if not bVar6 then
                    -- LAB_00e52d63_c39: (native jump target)
                    resources:PrepareResource(xStack_20c)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_1fc)
                    goto LAB_00e52d8f
                end
                ::FLOW_past_lab_00e52d54::
                ::LAB_00e5276b::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_1fc)
                -- LAB_00e53650: (native jump target)
                resources:ReleaseResource(xStack_20c)
                return
            end
            ::LAB_00e52d8f::
            if quest:GetStateBool("Phase3RequirementsComplete") then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                quest:SetStateInt("QuestPhase", 3)
                quest:SetStateBool("InitialisePhase", true)
            end
        elseif iVar13 == 3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                resources:ReleaseResource(xStack_20c)
                return
            end
            if quest:GetStateBool("InitialisePhase") then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                iVar13 = me:GetPos()
                -- TODO(native): helper_E53F60(quest, me, *(this + 0x14), me, iVar13)
                quest:SetStateBool("InitialisePhase", false)
            end
            cVar7 = me:IsTalkedToByHero()
            if cVar7 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                xStack_1ac = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(xStack_20c)
                bVar6 = resources:TryAcquire(xStack_20c, me, 4)
                while not bVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_1ac)
                        resources:ReleaseResource(xStack_20c)
                        return
                    end
                    bVar6 = resources:TryAcquire(xStack_20c, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_1ac)
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                if quest:GetStateBool("AllHairChanged") then
                    -- TODO(native): bVar6 = helper_E53670(quest, me, *(this + 0x14))
                    bVar6 = nil --[[unresolved native value]]
                    native_arg_sequence_1 = false
                    if bVar6 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if native_arg_sequence_1 then
                        iVar13 = require("V_BeardyBaldy.native_quest_helpers").IsHeroWearingAnyTash(quest, me)
                        if iVar13 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then
                        iVar13 = require("V_BeardyBaldy.native_quest_helpers").IsHeroWearingAnyOddHairdo(quest, me)
                        if iVar13 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            x_stk_cc = resources:ScriptThing(xStack_20c)
                            pCVar11 = x_stk_cc
                            fret_20 = quest:GetHealth(pCVar11)
                            fVar4 = 0.0
                            if fVar4 < fret_20 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 0
                                pcVar12 = "TEXT_QST_014_END_FINALE1"
                                pCVar11 = quest:GetHero()
                                r21 = me:Speak(pCVar11, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_1ac)
                                        resources:ReleaseResource(xStack_20c)
                                        return
                                    end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e53630 end
                            end
                            xStack_1cc = quest:ReadGlobalGameDataString(0x480)
                            quest:GiveHeroObject(xStack_1cc, -1, false)
                            quest:SetStateBool("QuestComplete", true)
                            goto LAB_00e532e3
                        end
                    else
                        if (3 < quest:GetStateInt("IncorrectHairComboCount")) and (quest:GetStateBool("AllHairChanged")) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if not bVar6 then
                                x_stk_b4 = resources:ScriptThing(xStack_20c)
                                pCVar11 = x_stk_b4
                                fret_21 = quest:GetHealth(pCVar11)
                                fVar4 = 0.0
                                if fVar4 < fret_21 then
                                    iVar16 = 0
                                    iVar15 = 1
                                    iVar14 = 0
                                    iVar13 = 0
                                    pcVar12 = "TEXT_QST_014_END_FINALE2"
                                    pCVar11 = quest:GetHero()
                                    r22 = me:Speak(pCVar11, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                    while cVar7 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e53630 end
                                        iVar13 = me:IsPerformingScriptTask()
                                        cVar7 = iVar13
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_1ac)
                                        resources:ReleaseResource(xStack_20c)
                                        return
                                    end
                                end
                                xStack_1d4 = quest:ReadGlobalGameDataString(0x484)
                                quest:GiveHeroObject(xStack_1d4, -1, false)
                                quest:SetStateBool("QuestComplete", true)
                                goto LAB_00e532e3
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1ac)
                            resources:ReleaseResource(xStack_20c)
                            return
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            quest:SetStateInt("IncorrectHairComboCount", quest:GetStateInt("IncorrectHairComboCount") + 1)
                            x_stk_9c = resources:ScriptThing(xStack_20c)
                            pCVar11 = x_stk_9c
                            fret_22 = quest:GetHealth(pCVar11)
                            fVar4 = 0.0
                            if fVar4 < fret_22 then
                                iVar16 = 0
                                iVar15 = 1
                                iVar14 = 0
                                iVar13 = 2
                                pcVar12 = "TEXT_QST_014_END_WRONG"
                                pCVar11 = quest:GetHero()
                                r23 = me:Speak(pCVar11, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                                iVar13 = me:IsPerformingScriptTask()
                                cVar7 = iVar13
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_1ac)
                                        resources:ReleaseResource(xStack_20c)
                                        return
                                    end
                                    iVar13 = me:IsPerformingScriptTask()
                                    cVar7 = iVar13
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e53630 end
                            end
                            goto LAB_00e532e3
                        end
                    end
                    ::LAB_00e53630::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_1ac)
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    -- LAB_00e52fd3: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_1ac)
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                x_stk_e4 = resources:ScriptThing(xStack_20c)
                pCVar11 = x_stk_e4
                fret_19 = quest:GetHealth(pCVar11)
                fVar4 = 0.0
                if fVar4 < fret_19 then
                    iVar16 = 0
                    iVar15 = 1
                    iVar14 = 0
                    iVar13 = 2
                    pcVar12 = "TEXT_QST_014_END_NO_TASH"
                    pCVar11 = quest:GetHero()
                    r24 = me:Speak(pCVar11, pcVar12, iVar13, (iVar14 ~= 0), (iVar15 ~= 0), (iVar16 ~= 0))
                    iVar13 = me:IsPerformingScriptTask()
                    cVar7 = iVar13
                    while cVar7 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1ac)
                            resources:ReleaseResource(xStack_20c)
                            return
                        end
                        iVar13 = me:IsPerformingScriptTask()
                        cVar7 = iVar13
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_1ac)
                        resources:ReleaseResource(xStack_20c)
                        return
                    end
                end
                ::LAB_00e532e3::
                resources:PrepareResource(xStack_20c)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_1ac)
            end
            if quest:GetStateBool("QuestComplete") then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                quest:GiveHeroExperience(quest:ReadGlobalGameData(0x488))
                quest:ClearThingHasInformation(me)
                resources:PrepareResource(xStack_20c)
                bVar6 = resources:TryAcquire(xStack_20c, me, 4)
                while not bVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        resources:ReleaseResource(xStack_20c)
                        return
                    end
                    bVar6 = resources:TryAcquire(xStack_20c, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                r25 = quest:GetNearestWithScriptName(me, "M_BB_ExitPoint")
                iVar13 = (r25 ~= nil and r25:IsAlive())
                if iVar13 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if not bVar6 then
                        bVar6 = (not resources:ScriptThing(xStack_20c):IsNull())
                        if bVar6 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e53647 end
                            if not (r25 ~= nil and not r25:IsNull()) then
                                p0 = {x = 0, y = 0, z = 0}
                            else
                                p0 = r25:GetPos()
                            end
                            me:MoveToPosition(p0, 1.0, 1, false, true)
                        end
                        bVar6 = quest:IsDistanceBetweenThingsUnder(me, r25, 2.0)
                        while true do
                            __native_condition_1 = not bVar6
                            if __native_condition_1 then
                                bVar6 = quest:MsgOnRegionLoaded()
                                __native_condition_1 = not bVar6
                            end
                            if not __native_condition_1 then break end
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e53647 end
                            bVar6 = quest:IsDistanceBetweenThingsUnder(me, r25, 2.0)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            quest:FadeOutAndKillEntity(me, false, 1.0, true)
                            quest:SetStateBool("BeardyBaldyLeft", true)
                            goto LAB_00e534c2
                        end
                    end
                    ::LAB_00e53647::
                    resources:ReleaseResource(xStack_20c)
                    return
                end
                ::LAB_00e534c2::
            end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
    until false
end

function Init(quest, me)
    quest:SetThingHasInformation(me, true, true, false)
    quest:SetThingPersistent(me, true)
    __native_entity_state:SetStateInt("lastRandomSpeechIdx1", 10)
    __native_entity_state:SetStateInt("lastRandomSpeechIdx2", 10)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

function helper_E53670(quest, me)
    local bVar3, bVar5, b_stk_21, pCVar4
    bVar5 = 1
    pCVar4 = quest:GetHero()
    bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_01")
    if not bVar3 then
        bVar5 = 3
        pCVar4 = quest:GetHero()
        bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_02")
        if not bVar3 then
            bVar5 = 7
            pCVar4 = quest:GetHero()
            bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_03")
            if not bVar3 then
                bVar5 = 0xf
                pCVar4 = quest:GetHero()
                bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_MUTTON_01")
                if not bVar3 then
                    bVar5 = 0x1f
                    pCVar4 = quest:GetHero()
                    bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_LONG_01")
                    if not bVar3 then
                        bVar5 = 0x3f
                        pCVar4 = quest:GetHero()
                        bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_CHIN_01")
                        if not bVar3 then
                            bVar5 = 0x7f
                            pCVar4 = quest:GetHero()
                            bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_TRAMP_01")
                            if not bVar3 then
                                bVar5 = 0xff
                                pCVar4 = quest:GetHero()
                                bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_WATSON_01")
                                b_stk_21 = false
                                if not bVar3 then goto LAB_00e53856 end
                            end
                        end
                    end
                end
            end
        end
    end
    b_stk_21 = true
    ::LAB_00e53856::
    if ((bVar5 & 0x80) ~= 0) then
        bVar5 = bVar5 & 0x7f
    end
    if (bVar5 & 0x40) ~= 0 then
        bVar5 = bVar5 & 0xbf
    end
    if (bVar5 & 0x20) ~= 0 then
        bVar5 = bVar5 & 0xdf
    end
    if (bVar5 & 0x10) ~= 0 then
        bVar5 = bVar5 & 0xef
    end
    if (bVar5 & 8) ~= 0 then
        bVar5 = bVar5 & 0xf7
    end
    if (bVar5 & 4) ~= 0 then
        bVar5 = bVar5 & 0xfb
    end
    if (bVar5 & 2) ~= 0 then
        bVar5 = bVar5 & 0xfd
    end
    return b_stk_21
end

function helper_E53CB0(quest, me)
    local bVar1, hiddenStringResult, iVar2, xStack_8
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    repeat
        if bVar1 then
            -- LAB_00e53d10: (native jump target)
            hiddenStringResult = ""
            -- LAB_00e53d52: (native jump target)
            return hiddenStringResult
        end
        ::FLOW_after_lab_00e53d52::
        iVar2 = math.random(0, 32767)
        iVar2 = iVar2 % 10
        if (iVar2 ~= __native_entity_state:GetStateInt("lastRandomSpeechIdx1")) and (iVar2 ~= __native_entity_state:GetStateInt("lastRandomSpeechIdx2")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                hiddenStringResult = ""
                do return hiddenStringResult end
                goto FLOW_after_lab_00e53d52
            end
            __native_entity_state:SetStateInt("lastRandomSpeechIdx2", __native_entity_state:GetStateInt("lastRandomSpeechIdx1"))
            __native_entity_state:SetStateInt("lastRandomSpeechIdx1", iVar2)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                xStack_8 = quest:GetStateString(("RandomSpeech_" .. iVar2))
                hiddenStringResult = xStack_8
                do return hiddenStringResult end
                goto FLOW_after_lab_00e53d52
            end
            hiddenStringResult = ""
            -- LAB_00e53d52_c3: (native jump target)
            do return hiddenStringResult end
            goto FLOW_after_lab_00e53d52
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
    until false
end

function helper_E53F60(quest, me, native_arg_param_1, native_arg_param_2)
    -- TODO(native): local center = *native_arg_param_2
    quest:SetWanderCentrePoint(nil --[[missing]], nil --[[missing]])
    local fVar3 = quest:ReadGlobalGameDataFloat(0x448)
    quest:SetWanderMinDistance(nil --[[missing]], native_arg_param_1)
    fVar3 = quest:ReadGlobalGameDataFloat(0x44c)
    quest:SetWanderMaxDistance(nil --[[missing]], native_arg_param_1)
    quest:SetScriptingStateGroup(nil --[[missing]], native_arg_param_1)
end

