-- Generated native draft: NOVI_AffairWife. Review coverage report before use.
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
    __native_entity_state:SetStateBool("GoingForHusband", false)
    __native_entity_state:SetStateBool("ForceFirstTimeSpeak", true)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsUseMovementInActions(me, false)
    quest:SetIsPushableByHero(me, false)
    __native_entity_state:SetStateBool("SaidRunningLine", false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

function Main(quest, me)
    local CVar29, CVar31, bVar4, cVar5, fVar22, fVar3, fVar33, iVar10, native_arg_sequence_1, native_arg_wife_ally_hero, native_arg_wife_animation_remainder, native_arg_wife_argument_id, native_arg_wife_hit, native_arg_wife_line_counter, native_arg_wife_line_key, native_arg_wife_participant, native_arg_wife_reply_remainder, native_arg_wife_reply_terminating, native_arg_wife_reverse_hero, pCVar1, pCVar13, pCVar17, pCVar24, pCVar27, pCVar28, pCVar30, pCVar35, pCVar8, pCVar9, paVar32, paVar34, pcVar26, piVar20, ppVar15, ppVar25, ppuVar2, puVar19, r1, r2, r3, r4, r5, r6, r7, uVar11, uVar12, uVar14, uVar21
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    native_arg_wife_argument_id = 0
    alive = not quest:IsActiveThreadTerminating()
    cVar5 = not alive
    uVar21 = 0
    repeat
        if cVar5 then
            -- LAB_00db33f3: (native jump target)
            return
        end
        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_a4);
        if bVar4 then
        end
        -- TODO(native): pppuVar36 = &ppuStack_a4;
        cVar5 = me:AcquireControl(3)
        while not cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                if true then return end  -- TODO(native): goto LAB_00db33f3
                -- TODO(native): goto LAB_00db33eb
            end
            -- TODO(native): pppuVar36 = &ppuStack_a4;
            cVar5 = me:AcquireControl(3)
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db3e16 end
        if __native_entity_state:GetStateBool("GoingForHusband") then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db3e16 end
            paVar34 = "NOVI_AffairMan"
            r1 = quest:GetThingWithScriptName("NOVI_AffairMan")
            pCVar27 = 0x1
            quest:EntitySetAsUseMovementInActions(me, true)
            if nil == nil then
            else
                pCVar9 = (**(*0x0 + 0x18))()
            end
            me:MoveToPosition(nil --[[missing]], 0x40000000, 0x1, false, true)
            -- TODO(native): ppVar23 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar1;
            quest:ClearThingHasInformation(me)
            bVar4 = quest:IsDistanceBetweenThingsUnder(me, r1, 3.0)
            if bVar4 then goto LAB_00db3593 end
            break
        end
        piVar20 = (uVar21 | 8)
        cVar5 = me:MsgIsHitByHero()
        if not cVar5 then
            piVar20 = (uVar21 | 0x18)
            cVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if cVar5 then
                piVar20 = (uVar21 | 0x38)
                cVar5 = me:MsgIsHitByHeroSpecialAbility(14)
                native_arg_wife_hit = not cVar5
            else
                native_arg_wife_hit = false
            end
        else
            native_arg_wife_hit = true
        end
        if (piVar20 & 0x20) ~= 0 then
            piVar20 = (piVar20 & 0xffffffdf)
        end
        if (piVar20 & 0x10) ~= 0 then
            piVar20 = (piVar20 & 0xffffffef)
        end
        if (piVar20 & 8) ~= 0 then
            -- TODO(native): piStack_98 = (int *)((uint)piVar20 & 0xfffffff7);
        end
        if native_arg_wife_hit then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db3e16 end
            native_arg_wife_ally_hero = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, native_arg_wife_ally_hero)
            native_arg_wife_reverse_hero = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(native_arg_wife_reverse_hero, me)
            require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
            -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff40);
            if bVar4 then
            end
            cVar5 = me:AcquireControl(4)
            while not cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db3e16 end
                cVar5 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db3e16 end
            pCVar27 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            -- TODO(native): CVar6 = (CScriptGameResourceObjectScriptedThingBase) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff44);
            fVar22 = quest:GetHealth(me)
            fVar3 = 0.0
            if fVar22 <= fVar3 then
                -- LAB_00db2e83: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00db32a5: (native jump target)
                quest:EndMovieSequence()
                goto LAB_00db32aa
            end
            CVar31 = 0x0
            pCVar30 = 0x1
            pCVar28 = 0x0
            pCVar27 = 0x0
            pcVar26 = "TEXT_QST_048_AFFAIR_WIFE_ON_HIT"
            pCVar8 = quest:GetHero()
            r2 = me:Speak(pCVar8, pcVar26, pCVar27, (pCVar28 ~= 0), (pCVar30 ~= 0), (CVar31 ~= 0))
            bVar4 = me:IsPerformingScriptTask()
            if bVar4 then
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        quest:PauseAllNonScriptedEntities(false)
                        quest:EndMovieSequence()
                        goto LAB_00db3e16
                    end
                    bVar4 = me:IsPerformingScriptTask()
                until not (bVar4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                quest:PauseAllNonScriptedEntities(false)
                quest:EndMovieSequence()
                goto LAB_00db32aa
            end
            quest:PauseAllNonScriptedEntities(false)
            quest:EndMovieSequence()
            goto LAB_00db3e16
        end
        cVar5 = me:IsTalkedToByHero()
        if cVar5 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db3e16 end
            quest:StartMovieSequence()
            ppuVar2 = *(param_1 + 4)
            quest:PauseAllNonScriptedEntities(true)
            -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff44);
            if bVar4 then
            end
            cVar5 = me:AcquireControl(4)
            while not cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    quest:PauseAllNonScriptedEntities(false)
                    quest:EndMovieSequence()
                    goto LAB_00db3e16
                end
                cVar5 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                -- TODO(native): uVar11 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff38);
                fVar22 = quest:GetHealth(me)
                fVar3 = 0.0
                if fVar3 < fVar22 then
                    bVar4 = false
                    pCVar30 = 0x1
                    pCVar28 = 0x0
                    pCVar27 = 0x0
                    pcVar26 = "TEXT_QST_048_AFFAIR_WIFE_LAYABOUT"
                    pCVar9 = quest:GetHero()
                    r3 = me:Speak(pCVar9, pcVar26, pCVar27, (pCVar28 ~= 0), (pCVar30 ~= 0), bVar4)
                    bVar4 = me:IsPerformingScriptTask()
                    if bVar4 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:EndMovieSequence()
                                goto LAB_00db3e16
                            end
                            bVar4 = me:IsPerformingScriptTask()
                        until not (bVar4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        quest:PauseAllNonScriptedEntities(false)
                        quest:EndMovieSequence()
                        goto LAB_00db3e16
                    end
                end
                if not quest:GetStateBool("HeroDiscoveredInfidelity") then
                    quest:PauseAllNonScriptedEntities(false)
                    quest:EndMovieSequence()
                    goto LAB_00db32aa
                end
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    quest:GiveHeroYesNoQuestion("TEXT_QST_048_AFFAIR_WIFE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    CVar29 = SUB41(me,0)
                    while iVar10 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db3e16
                        end
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        CVar29 = SUB41(me,0)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        quest:PauseAllNonScriptedEntities(false)
                        quest:EndMovieSequence()
                        goto LAB_00db3e16
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if iVar10 == 1 then
                        if not alive then
                            -- LAB_00db3def: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db3e16
                        end
                        -- TODO(native): uVar11 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff38);
                        fVar22 = quest:GetHealth(me)
                        fVar3 = 0.0
                        if fVar3 < fVar22 then
                            bVar4 = false
                            pCVar30 = 0x1
                            pCVar28 = 0x0
                            pCVar27 = 0x0
                            pcVar26 = "TEXT_QST_048_AFFAIR_WIFE_THANKYOU"
                            pCVar8 = quest:GetHero()
                            r4 = me:Speak(pCVar8, pcVar26, pCVar27, (pCVar28 ~= 0), (pCVar30 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db3e16
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:EndMovieSequence()
                                goto LAB_00db3e16
                            end
                        end
                        require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
                        __native_entity_state:SetStateBool("GoingForHusband", true)
                    else
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:EndMovieSequence()
                            goto LAB_00db3e16
                        end
                        -- TODO(native): uVar11 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff38);
                        fVar22 = quest:GetHealth(me)
                        fVar3 = 0.0
                        if fVar3 < fVar22 then
                            bVar4 = false
                            pCVar30 = 0x1
                            pCVar28 = 0x0
                            pCVar27 = 0x0
                            pcVar26 = "TEXT_QST_048_AFFAIR_WIFE_PLEA"
                            pCVar8 = quest:GetHero()
                            r5 = me:Speak(pCVar8, pcVar26, pCVar27, (pCVar28 ~= 0), (pCVar30 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        goto LAB_00db3e16
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:EndMovieSequence()
                                goto LAB_00db3e16
                            end
                        end
                    end
                    -- LAB_00db3293: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    quest:EndMovieSequence()
                    goto LAB_00db32aa
                end
                -- LAB_00db3dff: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                quest:EndMovieSequence()
                goto LAB_00db3e16
            end
            -- LAB_00db3de0: (native jump target)
            quest:PauseAllNonScriptedEntities(false)
            quest:EndMovieSequence()
            goto LAB_00db3e16
        end
        ::LAB_00db32aa::
        iVar10 = quest:GetTimer(quest:GetStateInt("TalkIntermittentTimer"))
        native_arg_sequence_1 = false
        if iVar10 == 0 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if native_arg_sequence_1 then
            if __native_entity_state:GetStateBool("ForceFirstTimeSpeak") then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                iVar10 = quest:RetailRandModulo(500)
                if iVar10 == 0 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
        end
        if native_arg_sequence_1 then
            fVar33 = 5.0
            pCVar8 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, 5.0)
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db3e16 end
                quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 3)
                __native_entity_state:SetStateBool("ForceFirstTimeSpeak", false)
                uVar11 = quest:AddNewConversation(me, false, false)
                native_arg_wife_participant = quest:GetHero()
                quest:AddPersonToConversation(uVar11, native_arg_wife_participant)
                uVar12 = quest:GetHero()
                quest:AddLineToConversation(uVar11, "TEXT_QST_048_AFFAIR_WIFE_WHERES_HUSBAND", me, uVar12, false)
            end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cVar5 = not alive
        uVar21 = 0
    until false
    while true do
        if not __native_entity_state:GetStateBool("SaidRunningLine") then
            fVar33 = 10.0
            pCVar13 = me:GetHomePos()
            bVar4 = (me ~= nil and me:IsDistanceFromPositionOver(pCVar13, fVar33))
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db3d90
                uVar11 = quest:AddNewConversation(me, false, false)
                quest:AddLineToConversation(uVar11, "TEXT_QST_048_AFFAIR_WIFE_RUNNING_TO_HUBBY", me, nil, false)
                __native_entity_state:SetStateBool("SaidRunningLine", true)
            end
        end
        bVar4 = quest:IsDistanceBetweenThingsUnder(me, r1, 3.0)
        if bVar4 then break end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00db3d90
    end
    ::LAB_00db3593::
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        -- LAB_00db3d90: (native jump target)
        return
    end
    me:ClearCommands()
    fVar33 = 0.0
    native_arg_wife_line_counter = 0
    quest:EntitySetAsUseMovementInActions(me, (native_arg_wife_line_counter ~= 0))
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        -- LAB_00db3d6a: (native jump target)
        return
    end
    repeat
        pCVar9 = quest:GetHero()
        bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar9, 15.0)
        if bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00db3d90
            native_arg_wife_animation_remainder = quest:RetailRandModulo(2)
            bVar4 = native_arg_wife_animation_remainder == 0
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db3d90
                me:PlayAnimation("ST_ARGUING_POINT_AWAY", false, false, false, true, DAT_01375748, false, false)
            else
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db3d90
                me:PlayAnimation("ST_ARGUING_POINT_AT", false, false, false, true, DAT_01375748, false, false)
                -- TODO(native): paVar16 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_70;
            end
            bVar4 = me:IsPerformingScriptTask()
            if bVar4 then
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00db3d90
                    quest:EntitySetFacingAngleTowardsThing(me, r1, false)
                    uVar14 = uVar21 | 1
                    cVar5 = me:MsgIsHitByHero()
                    if not cVar5 then
                        uVar14 = uVar21 | 3
                        cVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if cVar5 then
                            uVar14 = uVar21 | 7
                            cVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            bVar4 = not cVar5
                        else
                            bVar4 = false
                        end
                    else
                        bVar4 = true
                    end
                    if (uVar14 & 4) ~= 0 then
                        uVar14 = uVar14 & 0xfffffffb
                    end
                    uVar21 = uVar14
                    if (uVar14 & 2) ~= 0 then
                        uVar14 = uVar14 & 0xfffffffd
                        uVar21 = uVar14
                    end
                    if (uVar14 & 1) ~= 0 then
                        uVar21 = uVar14 & 0xfffffffe
                    end
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00db3d90
                        native_arg_wife_ally_hero = quest:GetHero()
                        quest:EntitySetThingAsAllyOfThing(me, native_arg_wife_ally_hero)
                        uVar11 = quest:GetHero()
                        quest:EntitySetThingAsAllyOfThing(uVar11, me)
                        require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
                        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff2c);
                        if bVar4 then
                        end
                        cVar5 = me:AcquireControl(4)
                        while not cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00db3d90
                            cVar5 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00db3d90
                        pCVar28 = ""
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff30);
                        fVar22 = quest:GetHealth(me)
                        fVar3 = 0.0
                        if fVar3 < fVar22 then
                            bVar4 = false
                            pCVar24 = 0x1
                            pCVar30 = 0x0
                            pCVar28 = 0x0
                            pcVar26 = "TEXT_QST_048_AFFAIR_WIFE_ON_HIT"
                            pCVar9 = quest:GetHero()
                            r6 = me:Speak(pCVar9, pcVar26, pCVar28, (pCVar30 ~= 0), (pCVar24 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        return
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:EndMovieSequence()
                                return
                            end
                        end
                        quest:EntitySetFacingAngleTowardsThing(me, r1, true)
                        quest:PauseAllNonScriptedEntities(false)
                        quest:EndMovieSequence()
                    end
                    cVar5 = me:IsTalkedToByHero()
                    if cVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00db3d90
                        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff3c);
                        if bVar4 then
                        end
                        cVar5 = me:AcquireControl(4)
                        while not cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00db3d90
                            cVar5 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00db3d90
                        me:ClearAllActions()
                        me:ClearCommands()
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff30);
                        fVar22 = quest:GetHealth(me)
                        fVar3 = 0.0
                        if fVar3 < fVar22 then
                            bVar4 = false
                            pCVar24 = 0x1
                            pCVar30 = 0x0
                            pCVar28 = 0x0
                            pcVar26 = "TEXT_QST_048_AFFAIR_WIFE_THANKYOU_SINGLE"
                            pCVar9 = quest:GetHero()
                            r7 = me:Speak(pCVar9, pcVar26, pCVar28, (pCVar30 ~= 0), (pCVar24 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        quest:EndMovieSequence()
                                        -- TODO(native): goto LAB_00db3d6a
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                -- LAB_00db3d61: (native jump target)
                                quest:EndMovieSequence()
                                -- TODO(native): goto LAB_00db3d6a
                            end
                        end
                        quest:EntitySetFacingAngleTowardsThing(me, r1, true)
                        quest:PauseAllNonScriptedEntities(false)
                        quest:EndMovieSequence()
                    end
                    cVar5 = quest:IsConversationActive(native_arg_wife_argument_id)
                    if not cVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00db3d90
                        ppVar15 = quest:AddNewConversation(me, false, false)
                        ppVar25 = ppVar15
                        native_arg_wife_argument_id = ppVar15
                        quest:AddPersonToConversation(ppVar15, r1)
                        native_arg_wife_line_counter = native_arg_wife_line_counter + 10
                        native_arg_wife_line_key = "TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_" .. tostring(native_arg_wife_line_counter)
                        if native_arg_wife_line_counter > 40 then
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                native_arg_wife_line_counter = 10
                                native_arg_wife_line_key = "TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_10"
                                goto LAB_00db3c11
                            end
                            -- LAB_00db3d8b: (native jump target)
                            -- TODO(native): goto LAB_00db3d90
                        end
                        ::LAB_00db3c11::
                        quest:AddLineToConversation(ppVar15, native_arg_wife_line_key, me, r1, false)
                        native_arg_wife_reply_remainder = quest:RetailRandModulo(2)
                        if native_arg_wife_reply_remainder == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            native_arg_wife_reply_terminating = not alive
                            if native_arg_wife_reply_terminating then return end  -- TODO(native): goto LAB_00db3d8b
                            quest:AddLineToConversation(ppVar15, "TEXT_QST_048_AFFAIRMAN_IN_TROUBLE", r1, me, false)
                        end
                    end
                    bVar4 = me:IsPerformingScriptTask()
                until not (bVar4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00db3d90
        end
        fVar33 = 2.0133748e-38
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
    until false
    -- LAB_00db3dd9: (native jump target)
    puVar19 = *ppuVar2
    -- LAB_00db3e05: (native jump target)
    quest:PauseAllNonScriptedEntities(false)
    -- LAB_00db3e0d: (native jump target)
    -- LAB_00db3e11: (native jump target)
    quest:EndMovieSequence()
    ::LAB_00db3e16::
end

