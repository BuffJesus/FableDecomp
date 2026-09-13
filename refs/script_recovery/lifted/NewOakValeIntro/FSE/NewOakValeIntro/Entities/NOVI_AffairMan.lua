-- Generated native draft: NOVI_AffairMan. Review coverage report before use.
-- Not copied from the working port; registration remains disabled.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Init(quest, me)
    __native_entity_state:SetStateInt("BadgerIndex", 0)
    __native_entity_state:SetStateBool("EncounterOver", false)
    __native_entity_state:SetStateBool("SaidFirstRangedComment", false)
    __native_entity_state:SetStateBool("HeroAgreedToKeepQuiet", false)
    __native_entity_state:SetStateBool("HeroSaidHeWouldReportMan", false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
    quest:SetThingHasInformation(me, false, false, false)
end

function Main(quest, me)
    local bVar4, cVar5, fVar18, fVar25, fVar3, iVar10, native_arg_man_cleanup_mask, native_arg_man_conversation_id, native_arg_man_hit, native_arg_man_home_position, native_arg_man_line_index, native_arg_man_line_terminating, native_arg_man_near_key, native_arg_man_ranged_index, native_arg_man_ranged_key, native_arg_man_talk_terminating, native_arg_sequence_1, native_arg_woman_near_key, native_arg_woman_ranged_key, pCVar1, pCVar12, pCVar13, pCVar20, pCVar21, pCVar22, pCVar26, pCVar7, pcVar19, ppVar24, r1, r10, r2, r3, r4, r5, r6, r7, r8, r9, uVar14, uVar17, uVar6, uVar8, uVar9
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_140);
        if bVar4 then
        end
        -- TODO(native): pppuVar27 = &ppuStack_140;
        cVar5 = me:AcquireControl(4)
        while not cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db1d8e end
            cVar5 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            r1 = quest:GetThingWithScriptName("NOVI_AffairWoman")
            r2 = quest:GetThingWithScriptName("NOVI_AffairWife")
            native_arg_man_conversation_id = 0
            alive = not quest:IsActiveThreadTerminating()
            uVar14 = 0
            if alive then
                ::LAB_00db0b30::
                uVar17 = uVar14 | 1
                native_arg_man_cleanup_mask = uVar17
                cVar5 = me:MsgIsHitByHero()
                if not cVar5 then
                    uVar17 = uVar14 | 3
                    native_arg_man_cleanup_mask = uVar17
                    cVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if cVar5 then
                        uVar17 = uVar14 | 7
                        native_arg_man_cleanup_mask = uVar17
                        cVar5 = me:MsgIsHitByHeroSpecialAbility(14)
                        native_arg_man_hit = not cVar5
                    else
                        native_arg_man_hit = false
                    end
                else
                    native_arg_man_hit = true
                end
                if (uVar17 & 4) ~= 0 then
                    uVar17 = uVar17 & 0xfffffffb
                    native_arg_man_cleanup_mask = uVar17
                end
                if (uVar17 & 2) ~= 0 then
                    uVar17 = uVar17 & 0xfffffffd
                    native_arg_man_cleanup_mask = uVar17
                end
                if (uVar17 & 1) ~= 0 then
                    native_arg_man_cleanup_mask = uVar17 & 0xfffffffe
                end
                if native_arg_man_hit then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    pCVar20 = ""
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffea8);
                    if bVar4 then
                    end
                    cVar5 = me:AcquireControl(4)
                    while not cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db1d7c
                        end
                        cVar5 = me:AcquireControl(4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                        fVar18 = quest:GetHealth(me)
                        fVar3 = 0.0
                        if fVar3 < fVar18 then
                            bVar4 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_AFFAIRMAN_ON_HIT"
                            pCVar7 = quest:GetHero()
                            r3 = me:Speak(pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db1d7c
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:EndMovieSequence()
                                goto LAB_00db1d7c
                            end
                        end
                        require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
                        quest:PauseAllNonScriptedEntities(false)
                        quest:EndMovieSequence()
                        goto LAB_00db1c71
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    quest:EndMovieSequence()
                    goto LAB_00db1d7c
                end
                cVar5 = me:IsTalkedToByHero()
                if cVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    pCVar20 = ""
                    quest:StartMovieSequence()
                    pCVar21 = 0x1
                    -- TODO(native): piStack_12c = piVar2;
                    quest:PauseAllNonScriptedEntities((pCVar21 ~= 0))
                    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffea8);
                    if bVar4 then
                    end
                    cVar5 = me:AcquireControl(4)
                    uVar6 = SUB41(pCVar13,0)
                    while not cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db1d7c
                        end
                        cVar5 = me:AcquireControl(4)
                        uVar6 = SUB41(pCVar13,0)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        quest:PauseAllNonScriptedEntities(false)
                        quest:EndMovieSequence()
                        goto LAB_00db1d7c
                    end
                    me:ClearAllActions()
                    me:ClearCommands()
                    uVar6 = SUB41(me,0)
                    bVar4 = quest:IsDistanceBetweenThingsUnder(me, r2, 5.0)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db1d7c
                        end
                        if not __native_entity_state:GetStateBool("HeroAgreedToKeepQuiet") then
                            if __native_entity_state:GetStateBool("HeroSaidHeWouldReportMan") then
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    -- TODO(native): uVar8 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                                    fVar18 = quest:GetHealth(me)
                                    fVar3 = 0.0
                                    if fVar3 < fVar18 then
                                        bVar4 = false
                                        pCVar22 = 0x1
                                        pCVar21 = 0x0
                                        pCVar20 = 0x0
                                        pcVar19 = "TEXT_QST_048_AFFAIRMAN_SOME_NERVE"
                                        pCVar7 = quest:GetHero()
                                        r4 = me:Speak(pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                        bVar4 = me:IsPerformingScriptTask()
                                        if bVar4 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    quest:EndMovieSequence()
                                                    goto LAB_00db1d7c
                                                end
                                                bVar4 = me:IsPerformingScriptTask()
                                            until not (bVar4)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            quest:PauseAllNonScriptedEntities(false)
                                            quest:EndMovieSequence()
                                            goto LAB_00db1d7c
                                        end
                                    end
                                    quest:ClearThingHasInformation(me)
                                    goto LAB_00db1593
                                end
                                -- LAB_00db1085: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                quest:EndMovieSequence()
                                goto LAB_00db1d7c
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                -- TODO(native): uVar8 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                                fVar18 = quest:GetHealth(me)
                                fVar3 = 0.0
                                if fVar3 < fVar18 then
                                    bVar4 = false
                                    pCVar22 = 0x1
                                    pCVar21 = 0x0
                                    pCVar20 = 0x0
                                    pcVar19 = "TEXT_QST_048_AFFAIRMAN_HOW_FIND_OUT"
                                    pCVar7 = quest:GetHero()
                                    r5 = me:Speak(pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                    bVar4 = me:IsPerformingScriptTask()
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                quest:PauseAllNonScriptedEntities(false)
                                                quest:EndMovieSequence()
                                                goto LAB_00db1d7c
                                            end
                                            bVar4 = me:IsPerformingScriptTask()
                                        until not (bVar4)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    native_arg_man_talk_terminating = not alive
                                    if native_arg_man_talk_terminating then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db1d7c
                                    end
                                    quest:ClearThingHasInformation(me)
                                    goto LAB_00db1593
                                end
                                quest:ClearThingHasInformation(me)
                                goto LAB_00db1593
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                -- TODO(native): uVar8 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                                fVar18 = quest:GetHealth(me)
                                fVar3 = 0.0
                                if fVar3 < fVar18 then
                                    bVar4 = false
                                    pCVar22 = 0x1
                                    pCVar21 = 0x0
                                    pCVar20 = 0x0
                                    pcVar19 = "TEXT_QST_048_AFFAIRMAN_HAD_A_DEAL"
                                    pCVar7 = quest:GetHero()
                                    r6 = me:Speak(pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                    bVar4 = me:IsPerformingScriptTask()
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                quest:PauseAllNonScriptedEntities(false)
                                                quest:EndMovieSequence()
                                                goto LAB_00db1d7c
                                            end
                                            bVar4 = me:IsPerformingScriptTask()
                                        until not (bVar4)
                                    end
                                    -- LAB_00db1144: (native jump target)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db1d7c
                                    end
                                end
                                -- LAB_00db1153: (native jump target)
                                quest:ClearThingHasInformation(me)
                                goto LAB_00db1593
                            end
                        end
                        -- LAB_00db1cf3: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        quest:EndMovieSequence()
                        goto LAB_00db1d7c
                    end
                    if not __native_entity_state:GetStateBool("EncounterOver") then
                        alive = not quest:IsActiveThreadTerminating()
                        uVar8 = 0
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db1d7c
                        end
                        uVar9 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(r1, uVar9, false)
                        -- TODO(native): uVar9 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe90);
                        fVar18 = quest:GetHealth(me)
                        fVar3 = 0.0
                        if fVar3 < fVar18 then
                            bVar4 = false
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_AFFAIRMAN_INTRO"
                            pCVar7 = quest:GetHero()
                            r7 = me:Speak(pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db1d7c
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:EndMovieSequence()
                                goto LAB_00db1d7c
                            end
                        end
                        quest:SetStateBool("HeroDiscoveredInfidelity", true)
                        pCVar20 = "TEXT_QST_048_AFFAIRMAN_QUESTION_WILL_YOU_TELL"
                        quest:GiveHeroYesNoQuestion("TEXT_QST_048_AFFAIRMAN_QUESTION_WILL_YOU_TELL", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        uVar6 = uVar8
                        while iVar10 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:EndMovieSequence()
                                goto LAB_00db1d7c
                            end
                            iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                            uVar6 = uVar8
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            alive = not quest:IsActiveThreadTerminating()
                            if iVar10 == 1 then
                                if not alive then
                                    -- LAB_00db1d23: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    quest:EndMovieSequence()
                                    goto LAB_00db1d7c
                                end
                                -- TODO(native): uVar8 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe98);
                                fVar18 = quest:GetHealth(me)
                                fVar3 = 0.0
                                if fVar3 < fVar18 then
                                    bVar4 = false
                                    pCVar22 = 0x1
                                    pCVar21 = 0x0
                                    pCVar20 = 0x0
                                    pcVar19 = "TEXT_QST_048_AFFAIRMAN_GOOD_LAD"
                                    pCVar7 = quest:GetHero()
                                    r8 = me:Speak(pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                    bVar4 = me:IsPerformingScriptTask()
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                quest:PauseAllNonScriptedEntities(false)
                                                quest:EndMovieSequence()
                                                goto LAB_00db1d7c
                                            end
                                            bVar4 = me:IsPerformingScriptTask()
                                        until not (bVar4)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db1d7c
                                    end
                                end
                                require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 4)
                                quest:GiveHeroGold(1)
                                __native_entity_state:SetStateBool("HeroAgreedToKeepQuiet", true)
                            else
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    quest:EndMovieSequence()
                                    goto LAB_00db1d7c
                                end
                                -- TODO(native): uVar8 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe98);
                                fVar18 = quest:GetHealth(me)
                                fVar3 = 0.0
                                if fVar3 < fVar18 then
                                    bVar4 = false
                                    pCVar22 = 0x1
                                    pCVar21 = 0x0
                                    pCVar20 = 0x0
                                    pcVar19 = "TEXT_QST_048_AFFAIRMAN_DONT_TELL"
                                    pCVar7 = quest:GetHero()
                                    r9 = me:Speak(pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                                    bVar4 = me:IsPerformingScriptTask()
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                quest:PauseAllNonScriptedEntities(false)
                                                quest:EndMovieSequence()
                                                goto LAB_00db1d7c
                                            end
                                            bVar4 = me:IsPerformingScriptTask()
                                        until not (bVar4)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db1d7c
                                    end
                                end
                                __native_entity_state:SetStateBool("HeroSaidHeWouldReportMan", true)
                            end
                            quest:ClearThingHasInformation(me)
                            __native_entity_state:SetStateBool("EncounterOver", true)
                            goto LAB_00db1593
                        end
                        -- LAB_00db1d0a: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        quest:EndMovieSequence()
                        goto LAB_00db1d7c
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        quest:PauseAllNonScriptedEntities(false)
                        quest:EndMovieSequence()
                        goto LAB_00db1d7c
                    end
                    -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe9c);
                    fVar18 = quest:GetHealth(me)
                    fVar3 = 0.0
                    if fVar3 < fVar18 then
                        bVar4 = false
                        pCVar22 = 0x1
                        pCVar21 = 0x0
                        pCVar20 = 0x0
                        pcVar19 = "TEXT_QST_048_AFFAIRMAN_SHOO"
                        pCVar7 = quest:GetHero()
                        r10 = me:Speak(pCVar7, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    quest:EndMovieSequence()
                                    goto LAB_00db1d7c
                                end
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db1d7c
                        end
                    end
                    ::LAB_00db1593::
                    bVar4 = (r1 ~= nil and r1:IsAlive())
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db1d7c
                        end
                        quest:EntitySetFacingAngleTowardsThing(r1, me)
                    else
                        bVar4 = (r2 ~= nil and r2:IsAlive())
                        if bVar4 then
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                quest:EntitySetFacingAngleTowardsThing(r2, me)
                                goto LAB_00db15e6
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db1d7c
                        end
                    end
                    ::LAB_00db15e6::
                    quest:PauseAllNonScriptedEntities(false)
                    quest:EndMovieSequence()
                    goto LAB_00db1c71
                end
                bVar4 = me:IsPerformingScriptTask()
                if bVar4 then goto LAB_00db1c71 end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db1d7c end
                bVar4 = quest:IsDistanceBetweenThingsUnder(me, r2, 5.0)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    quest:EntitySetFacingAngleTowardsThing(me, r2, false)
                    iVar10 = quest:RetailRandModulo(50)
                    if iVar10 ~= 0 then goto LAB_00db1c71 end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    me:PlayAnimation("ST_OPINION_FEAR_IDLE_COWERING", false, false, false, true, DAT_01375748, false, false)
                    -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_b8;
                else
                    fVar25 = 0.1
                    pCVar12 = me:GetHomePos()
                    bVar4 = (me ~= nil and me:IsDistanceFromPositionOver(pCVar12, fVar25))
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db1d7c end
                        native_arg_man_home_position = me:GetHomePos()
                        fVar25 = 2.0
                        bVar4 = me:IsDistanceFromPositionOver(native_arg_man_home_position, 2.0)
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00db1d7c end
                                me:MoveToPosition(native_arg_man_home_position, 0.0, 0, false, true)
                                bVar4 = me:IsPerformingScriptTask()
                                if bVar4 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00db1d7c end
                                        bVar4 = me:IsPerformingScriptTask()
                                    until not (bVar4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00db1d7c end
                                fVar25 = 2.0
                                bVar4 = me:IsDistanceFromPositionOver(native_arg_man_home_position, 2.0)
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db1d7c end
                        goto LAB_00db1c71
                    end
                    cVar5 = quest:IsConversationActive(native_arg_man_conversation_id)
                    native_arg_sequence_1 = false
                    if cVar5 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if not native_arg_sequence_1 then
                        if __native_entity_state:GetStateBool("SaidFirstRangedComment") then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                        if native_arg_sequence_1 then
                            iVar10 = quest:RetailRandModulo(100)
                            if iVar10 ~= 0 then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                        end
                        if not native_arg_sequence_1 then
                            bVar4 = quest:IsDistanceBetweenThingsUnder(me, r1, 2.0)
                            if not bVar4 then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                        end
                    end
                    if native_arg_sequence_1 then goto LAB_00db1c71 end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    fVar25 = 5.0
                    pCVar7 = quest:GetHero()
                    bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, 5.0)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db1d7c end
                        uVar8 = quest:AddNewConversation(me, false, false)
                        native_arg_man_conversation_id = uVar8
                        quest:AddPersonToConversation(uVar8, r1)
                        iVar10 = __native_entity_state:GetStateInt("BadgerIndex")
                        __native_entity_state:SetStateInt("BadgerIndex", iVar10 + 10)
                        if 0x32 < iVar10 + 10 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db1d7c end
                            __native_entity_state:SetStateInt("BadgerIndex", 10)
                        end
                        native_arg_man_line_index = __native_entity_state:GetStateInt("BadgerIndex")
                        native_arg_man_near_key = "TEXT_QST_048_AFFAIRMAN_DIRTY_WORDS_NEAR_" .. tostring(native_arg_man_line_index)
                        native_arg_woman_near_key = "TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_NEAR_" .. tostring(native_arg_man_line_index)
                        if native_arg_man_line_index == 10 then
                            alive = not quest:IsActiveThreadTerminating()
                            native_arg_man_line_terminating = not alive
                            if native_arg_man_line_terminating then goto LAB_00db1d7c end
                            quest:AddLineToConversation(uVar8, native_arg_woman_near_key, r1, me, false)
                            quest:AddLineToConversation(uVar8, native_arg_man_near_key, me, r1, false)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            native_arg_man_line_terminating = not alive
                            if native_arg_man_line_terminating then goto LAB_00db1d7c end
                            quest:AddLineToConversation(uVar8, native_arg_man_near_key, me, r1, false)
                            quest:AddLineToConversation(uVar8, native_arg_woman_near_key, r1, me, false)
                        end
                        quest:SetStateBool("HeroDiscoveredInfidelity", true)
                    else
                        fVar25 = 13.0
                        pCVar7 = quest:GetHero()
                        bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, 13.0)
                        if bVar4 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db1d7c end
                            __native_entity_state:SetStateBool("SaidFirstRangedComment", true)
                            uVar8 = quest:AddNewConversation(me, false, false)
                            native_arg_man_conversation_id = uVar8
                            quest:AddPersonToConversation(uVar8, r1)
                            iVar10 = __native_entity_state:GetStateInt("BadgerIndex")
                            __native_entity_state:SetStateInt("BadgerIndex", iVar10 + 10)
                            if 0x32 < iVar10 + 10 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00db1d7c end
                                __native_entity_state:SetStateInt("BadgerIndex", 10)
                            end
                            native_arg_man_ranged_index = __native_entity_state:GetStateInt("BadgerIndex")
                            native_arg_man_ranged_key = "TEXT_QST_048_AFFAIRMAN_SCRMSG_DIRTY_WORDS_" .. tostring(native_arg_man_ranged_index)
                            native_arg_woman_ranged_key = "TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_" .. tostring(native_arg_man_ranged_index)
                            quest:AddLineToConversation(uVar8, native_arg_man_ranged_key, me, r1, false)
                            quest:AddLineToConversation(uVar8, native_arg_woman_ranged_key, r1, me, false)
                        end
                    end
                    if quest:GetStateBool("TalkingToWoman") then goto LAB_00db1c71 end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    quest:Pause(0.4000000059604645)
                    quest:EntitySetFacingAngleTowardsThing(me, r1, true)
                    quest:EntitySetFacingAngleTowardsThing(r1, me, true)
                    uVar14 = quest:RetailRandModulo(2)
                    bVar4 = uVar14 == 0
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            quest:SetStateBool("ReceiveKiss", true)
                            me:PlayAnimation("GIVE_KISS", false, true, false, true, DAT_01375748, false, false)
                            -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_e4;
                            goto LAB_00db1c6c
                        end
                        goto LAB_00db1d7c
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db1d7c end
                    quest:SetStateBool("ReceiveHug", true)
                    me:PlayAnimation("GIVE_HUG", false, true, false, true, DAT_01375748, false, false)
                    -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_dc;
                end
                ::LAB_00db1c6c::
                ::LAB_00db1c71::
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                uVar14 = native_arg_man_cleanup_mask
                if not alive then goto LAB_00db1d7c end
                goto LAB_00db0b30
            end
            ::LAB_00db1d7c::
        end
        ::LAB_00db1d8e::
    end
end

