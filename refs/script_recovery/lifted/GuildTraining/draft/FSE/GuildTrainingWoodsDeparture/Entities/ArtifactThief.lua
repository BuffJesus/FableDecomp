-- Generated native draft: ArtifactThief. Review coverage report before use.
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
    local CVar2, __native_condition_1, bVar4, cVar5, fVar21, fVar22, fVar33, iVar9, pCVar1, pCVar13, pCVar14, pCVar16, pCVar24, pCVar25, pCVar27, pCVar28, pCVar29, pCVar31, pCVar8, paVar15, paVar32, pcVar11, pcVar23, piVar12, piVar35, ppVar10, ppuStack_144, ppuStack_18c, ppuVar20, r1, r10, r11, r12, r13, r14, r15, r16, r2, r3, r4, r5, r6, r7, r8, r9, uVar18, uVar19, uVar30, uVar7
    local alive = true
    uVar18 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_184);
    if bVar4 then
    end
    -- TODO(native): pppuVar34 = &ppuStack_184;
    cVar5 = me:AcquireControl(4)
    while not cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00d63c9f end
        cVar5 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        quest:EntitySetAsKillable(nil --[[missing]], false)
        quest:SetThingHasInformation(nil --[[missing]])
        __native_entity_state:SetStateBool("HoldingArtifact", true)
        __native_entity_state:SetStateBool("AlreadyTalkedTo", false)
        __native_entity_state:SetStateBool("NotAttacked", true)
        uVar7 = quest:RegisterTimer()
        quest:SetTimer(uVar7, 0)
        CVar2 = __native_entity_state:GetStateBool("HoldingArtifact")
        repeat
            if (not CVar2) or (not __native_entity_state:GetStateBool("NotAttacked")) then goto LAB_00d638ec end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d63c96 end
            fVar33 = 5.5
            pCVar8 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar8, me, fVar33)
            __native_condition_1 = bVar4
            if __native_condition_1 then
                iVar9 = quest:GetTimer(uVar7)
                __native_condition_1 = iVar9 < 1
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d63c96 end
                ppVar10 = quest:AddNewConversation(nil --[[missing]], false, (uVar18 ~= 0))
                r1 = quest:GetHero()
                quest:AddPersonToConversation(nil --[[missing]], r1)
                if not __native_entity_state:GetStateBool("AlreadyTalkedTo") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d63c96 end
                    r2 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(r2, nil --[[missing]])
                    me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, DAT_01375748, false)
                    r3 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_FIRST", r3, nil --[[missing]])
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d63c96 end
                    r4 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(r4, nil --[[missing]])
                    r5 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_COMMENT_SECOND", r5, nil --[[missing]])
                end
                quest:SetTimer(uVar7, 10)
            end
            uVar19 = uVar18 | 1
            cVar5 = me:MsgIsHitByHero()
            if not cVar5 then
                uVar19 = uVar18 | 3
                cVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if cVar5 then
                    uVar19 = uVar18 | 7
                    cVar5 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                    if not cVar5 then return end  -- TODO(native): goto LAB_00d6280c
                end
                bVar4 = false
            else
                -- LAB_00d6280c: (native jump target)
                bVar4 = true
            end
            if (uVar19 & 4) ~= 0 then
                uVar19 = uVar19 & 0xfffffffb
            end
            if (uVar19 & 2) ~= 0 then
                uVar19 = uVar19 & 0xfffffffd
            end
            if (uVar19 & 1) ~= 0 then
                uVar19 = uVar19 & 0xfffffffe
            end
            uVar18 = uVar19
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d63c96 end
                __native_entity_state:SetStateBool("NotAttacked", false)
                if not __native_entity_state:GetStateBool("AlreadyTalkedTo") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d63c96 end
                    pCVar28 = ""
                    quest:StartMovieSequence()
                    pCVar31 = 0x1
                    quest:PauseAllNonScriptedEntities((pCVar31 ~= 0))
                    -- TODO(native): pcVar11 = (char *)CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *) &stack0xfffffe64);
                    fVar21 = quest:GetHealth(nil --[[missing]])
                    fVar22 = _DAT_0122dedc
                    if fVar22 < fVar21 then
                        bVar4 = false
                        pCVar29 = 0x1
                        pCVar27 = 0x0
                        pCVar24 = 0x0
                        pcVar23 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_PRE_HIT"
                        pCVar8 = quest:GetHero()
                        r6 = me:Speak(pCVar8, pcVar23, pCVar24, (pCVar27 ~= 0), (pCVar29 ~= 0), bVar4)
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- TODO(native): goto LAB_00d63aab
                                end
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d63c96
                        end
                    end
                    pCVar24 = "OBJECT_HAND_LAMP"
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", nil --[[missing]])
                    quest:RemoveItemFromContainer(nil --[[missing]], "OBJECT_HAND_LAMP")
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): this = (CPhysicsMeshInfo *)&ppuStack_16c;
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d63c96 end
                    pCVar28 = ""
                    quest:StartMovieSequence()
                    pCVar31 = 0x1
                    quest:PauseAllNonScriptedEntities((pCVar31 ~= 0))
                    -- TODO(native): pcVar11 = (char *)CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *) &stack0xfffffe64);
                    fVar21 = quest:GetHealth(nil --[[missing]])
                    fVar22 = _DAT_0122dedc
                    if fVar22 < fVar21 then
                        bVar4 = false
                        pCVar29 = 0x1
                        pCVar27 = 0x0
                        pCVar24 = 0x0
                        pcVar23 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_HIT"
                        pCVar8 = quest:GetHero()
                        r7 = me:Speak(pCVar8, pcVar23, pCVar24, (pCVar27 ~= 0), (pCVar29 ~= 0), bVar4)
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- TODO(native): goto LAB_00d63aab
                                end
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d63c96
                        end
                    end
                    quest:GiveHeroObject("OBJECT_HAND_LAMP", nil --[[missing]])
                    quest:RemoveItemFromContainer(nil --[[missing]], "OBJECT_HAND_LAMP")
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): this = (CPhysicsMeshInfo *)&ppuStack_f0;
                end
                quest:EntitySetAsKillable(me, true, true)
                piVar12 = quest:GetThingWithScriptName("ArtifactThiefRunMarker")
                pCVar8 = piVar12:GetPos()
                me:MoveToPosition(pCVar8, pcVar11, pCVar31, pCVar25, SUB41(me,0))
                uVar18 = uVar19
            end
            cVar5 = me:IsTalkedToByHero()
            if cVar5 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d63c96 end
                if not __native_entity_state:GetStateBool("HoldingArtifact") then goto LAB_00d638ec end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d63c96 end
                if not __native_entity_state:GetStateBool("AlreadyTalkedTo") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d63c96 end
                    __native_entity_state:SetStateBool("AlreadyTalkedTo", true)
                    paVar32 = ""
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe64);
                    fVar22 = quest:GetHealth(nil --[[missing]])
                    uVar18 = CONCAT13(1,(int3)me)
                    if fVar22 <= _DAT_0122dedc then
                        uVar18 = me & 0xffffff
                    end
                    if (uVar18 >> 0x18) == 0 then
                        -- LAB_00d62e38: (native jump target)
                        pCVar25 = piVar12
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                        iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while true do
                            uVar30 = uVar7
                            if not (iVar9 < 0) then break end
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                -- LAB_00d63aa6: (native jump target)
                                -- LAB_00d63aab: (native jump target)
                                quest:DeregisterTimer(uVar7)
                                return
                            end
                            iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            alive = not quest:IsActiveThreadTerminating()
                            -- TODO(native): iVar3 = DAT_0143e90c;
                            if iVar9 == 1 then
                                if alive then
                                    ppuStack_18c = quest:GetHeroGold()
                                    if ppuStack_18c < *(iVar3 + 0xf14) then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if alive then
                                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe60);
                                            fVar22 = quest:GetHealth(nil --[[missing]])
                                            pCVar13 = CONCAT13(1,(int3)uVar18)
                                            if fVar22 <= _DAT_0122dedc then
                                                pCVar13 = (uVar18 & 0xffffff)
                                            end
                                            if (pCVar13 >> 0x18) ~= 0 then
                                                bVar4 = false
                                                pCVar31 = 0x1
                                                pCVar28 = 0x0
                                                pCVar25 = 0x0
                                                pcVar11 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD"
                                                pCVar8 = quest:GetHero()
                                                r8 = me:Speak(pCVar8, pcVar11, pCVar25, (pCVar28 ~= 0), (pCVar31 ~= 0), bVar4)
                                                bVar4 = me:IsPerformingScriptTask()
                                                if bVar4 then
                                                    repeat
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then goto LAB_00d63a62 end
                                                        bVar4 = me:IsPerformingScriptTask()
                                                    until not (bVar4)
                                                end
                                                -- TODO(native): goto LAB_00d632b4
                                            end
                                            -- TODO(native): goto LAB_00d632c3
                                        end
                                        goto LAB_00d63a7c
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe60);
                                        fVar22 = quest:GetHealth(nil --[[missing]])
                                        pCVar13 = CONCAT13(1,(int3)uVar18)
                                        if fVar22 <= _DAT_0122dedc then
                                            pCVar13 = (uVar18 & 0xffffff)
                                        end
                                        if (pCVar13 >> 0x18) ~= 0 then
                                            bVar4 = false
                                            pCVar31 = 0x1
                                            pCVar28 = 0x0
                                            pCVar25 = 0x0
                                            pcVar11 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES"
                                            pCVar8 = quest:GetHero()
                                            r9 = me:Speak(pCVar8, pcVar11, pCVar25, (pCVar28 ~= 0), (pCVar31 ~= 0), bVar4)
                                            bVar4 = me:IsPerformingScriptTask()
                                            if bVar4 then
                                                repeat
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if not alive then goto LAB_00d63a7c end
                                                    bVar4 = me:IsPerformingScriptTask()
                                                until not (bVar4)
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then goto LAB_00d63a62 end
                                        end
                                        quest:GiveHeroObject("OBJECT_HAND_LAMP", 0x0)
                                        quest:RemoveItemFromContainer(nil --[[missing]], "OBJECT_HAND_LAMP")
                                        uVar7 = __ftol2()
                                        quest:GiveHeroGold(0)
                                        uVar7 = __ftol2()
                                        quest:EntityGiveGold(me, nil --[[missing]])
                                        __native_entity_state:SetStateBool("HoldingArtifact", false)
                                        uVar30 = SUB41("ArtifactThiefRunMarker",0)
                                        piVar12 = quest:GetThingWithScriptName("ArtifactThiefRunMarker")
                                        pCVar31 = 0x1
                                        pCVar28 = 0x0
                                        pCVar25 = 0x0
                                        pCVar8 = piVar12:GetPos()
                                        me:MoveToPosition(pCVar8, SUB41(ppVar10,0))
                                        -- TODO(native): (**(code **)(*ppuStack_16c + 0x5ec))(0);
                                        goto LAB_00d638d8
                                    end
                                end
                                ::LAB_00d63a62::
                                quest:PauseAllNonScriptedEntities((pCVar25 ~= 0))
                                goto LAB_00d63c96
                            end
                            if alive then
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe60);
                                fVar22 = quest:GetHealth(piVar12)
                                pCVar13 = CONCAT13(1,(int3)uVar18)
                                if fVar22 <= _DAT_0122dedc then
                                    pCVar13 = (uVar18 & 0xffffff)
                                end
                                if (pCVar13 >> 0x18) ~= 0 then
                                    bVar4 = false
                                    pCVar31 = 0x1
                                    pCVar28 = 0x0
                                    pCVar25 = 0x0
                                    pcVar11 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO"
                                    pCVar8 = quest:GetHero()
                                    r10 = me:Speak(pCVar8, pcVar11, pCVar25, (pCVar28 ~= 0), (pCVar31 ~= 0), bVar4)
                                    bVar4 = me:IsPerformingScriptTask()
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d63a62
                                            bVar4 = me:IsPerformingScriptTask()
                                        until not (bVar4)
                                    end
                                    -- LAB_00d632b4: (native jump target)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00d63a7c end
                                end
                                -- LAB_00d632c3: (native jump target)
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                goto LAB_00d638d8
                            end
                        end
                    else
                        bVar4 = false
                        pCVar31 = 0x1
                        pCVar28 = 0x0
                        pCVar25 = 0x0
                        pcVar11 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_CHAT"
                        pCVar13 = quest:GetHero()
                        r11 = me:Speak(pCVar13, pcVar11, pCVar25, (pCVar28 ~= 0), (pCVar31 ~= 0), bVar4)
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d63a62
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then return end  -- TODO(native): goto LAB_00d62e38
                    end
                    ::LAB_00d63a7c::
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    goto LAB_00d63c96
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d63c96 end
                paVar15 = ""
                quest:StartMovieSequence()
                ppuVar20 = *(this + 4)
                uVar7 = 1
                -- TODO(native): (**(code **)(*ppuVar20 + 0x5ec))();
                -- TODO(native): ppVar10 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe64);
                fVar22 = quest:GetHealth(piVar12)
                pCVar8 = CONCAT13(1,(int3)pCVar13)
                if fVar22 <= _DAT_0122dedc then
                    pCVar8 = (pCVar13 & 0xffffff)
                end
                if (pCVar8 >> 0x18) ~= 0 then
                    bVar4 = false
                    pCVar31 = 0x1
                    pCVar28 = 0x0
                    pCVar25 = 0x0
                    pcVar11 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN"
                    pCVar13 = quest:GetHero()
                    r12 = me:Speak(pCVar13, pcVar11, pCVar25, (pCVar28 ~= 0), (pCVar31 ~= 0), bVar4)
                    bVar4 = me:IsPerformingScriptTask()
                    if bVar4 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d63ad2
                            bVar4 = me:IsPerformingScriptTask()
                        until not (bVar4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then goto LAB_00d633f7 end
                    -- LAB_00d635e3: (native jump target)
                    -- TODO(native): (**(code **)(*ppuVar20 + 0x5ec))();
                    goto LAB_00d63c96
                end
                ::LAB_00d633f7::
                pCVar25 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION"
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                pCVar13 = pCVar8
                while iVar9 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        -- TODO(native): (**(code **)(*ppuVar20 + 0x5ec))();
                        -- TODO(native): goto LAB_00d63aa6
                    end
                    iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d635e3
                alive = not quest:IsActiveThreadTerminating()
                -- TODO(native): iVar3 = DAT_0143e90c;
                if iVar9 ~= 1 then
                    if not alive then return end  -- TODO(native): goto LAB_00d635e3
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&piStack_b0);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities((uVar7 ~= 0))
                    -- TODO(native): CVar6 = (CScriptGameResourceObjectScriptedThingBase) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe54);
                    fVar21 = quest:GetHealth(nil --[[missing]])
                    fVar22 = _DAT_0122dedc
                    if fVar22 < fVar21 then
                        bVar4 = false
                        pCVar31 = 0x1
                        pCVar28 = 0x0
                        pCVar25 = 0x0
                        pcVar11 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_RETURN_QUESTION_NO"
                        pCVar8 = quest:GetHero()
                        r13 = me:Speak(pCVar8, pcVar11, pCVar25, (pCVar28 ~= 0), (pCVar31 ~= 0), bVar4)
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d63aec
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00d63aec: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d63c96
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    ppuVar20 = ppuStack_18c
                    goto LAB_00d638c8
                end
                if not alive then
                    -- LAB_00d63ad2: (native jump target)
                    -- TODO(native): (**(code **)(*ppuVar20 + 0x5ec))();
                    goto LAB_00d63c96
                end
                ppuStack_144 = quest:GetHeroGold()
                if *(iVar3 + 0xf14) <= ppuStack_144 then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe60);
                        fVar22 = quest:GetHealth(nil --[[missing]])
                        pCVar8 = CONCAT13(1,(int3)pCVar13)
                        if fVar22 <= _DAT_0122dedc then
                            pCVar8 = (pCVar13 & 0xffffff)
                        end
                        pCVar13 = pCVar8
                        uVar30 = uVar7
                        if (pCVar13 >> 0x18) ~= 0 then
                            bVar4 = false
                            pCVar31 = 0x1
                            pCVar28 = 0x0
                            pCVar25 = 0x0
                            pcVar11 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_YES"
                            pCVar8 = quest:GetHero()
                            r14 = me:Speak(pCVar8, pcVar11, pCVar25, (pCVar28 ~= 0), (pCVar31 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            uVar30 = uVar7
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d635e3
                                    bVar4 = me:IsPerformingScriptTask()
                                    uVar30 = uVar7
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d63ad2
                        end
                        pCVar31 = 0x0
                        pCVar28 = 0xffffffff
                        pCVar25 = "OBJECT_HAND_LAMP"
                        quest:GiveHeroObject("OBJECT_HAND_LAMP", pCVar28)
                        -- TODO(native): pppuVar26 = &ppuStack_184;
                        quest:RemoveItemFromContainer(nil --[[missing]], "OBJECT_HAND_LAMP")
                        uVar7 = __ftol2()
                        quest:GiveHeroGold(pCVar31)
                        uVar7 = __ftol2()
                        quest:EntityGiveGold(me, 0x0)
                        __native_entity_state:SetStateBool("HoldingArtifact", false)
                        piVar12 = quest:GetThingWithScriptName("ArtifactThiefRunMarker")
                        pCVar8 = piVar12:GetPos()
                        me:MoveToPosition(pCVar8, pppuVar26, pCVar28, (pCVar31 ~= 0), SUB41("",0))
                        ppuVar20 = ppuStack_18c
                        goto LAB_00d638c8
                    end
                    -- TODO(native): goto LAB_00d63ad2
                end
                alive = not quest:IsActiveThreadTerminating()
                uVar30 = SUB41("",0)
                if not alive then return end  -- TODO(native): goto LAB_00d635e3
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe60);
                fVar22 = quest:GetHealth(piVar12)
                pCVar8 = CONCAT13(1,(int3)pCVar13)
                if fVar22 <= _DAT_0122dedc then
                    pCVar8 = (pCVar13 & 0xffffff)
                end
                pCVar13 = pCVar8
                if (pCVar13 >> 0x18) ~= 0 then
                    bVar4 = false
                    pCVar31 = 0x1
                    pCVar28 = 0x0
                    pCVar25 = 0x0
                    pcVar11 = "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_QUESTION_NO_GOLD"
                    pCVar8 = quest:GetHero()
                    r15 = me:Speak(pCVar8, pcVar11, pCVar25, (pCVar28 ~= 0), (pCVar31 ~= 0), bVar4)
                    bVar4 = me:IsPerformingScriptTask()
                    if bVar4 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d63ad2
                            bVar4 = me:IsPerformingScriptTask()
                        until not (bVar4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d635e3
                end
                ::LAB_00d638c8::
                -- TODO(native): (**(code **)(*ppuVar20 + 0x5ec))();
                ::LAB_00d638d8::
                uVar18 = uVar19
            end
            CVar2 = __native_entity_state:GetStateBool("HoldingArtifact")
        until false
    end
    goto LAB_00d63c9f
    ::LAB_00d638ec::
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        bVar4 = me:IsPerformingScriptTask()
        if bVar4 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d63c96 end
                uVar19 = uVar18 | 8
                cVar5 = me:MsgIsHitByHero()
                if not cVar5 then
                    uVar19 = uVar18 | 0x18
                    cVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if cVar5 then
                        uVar19 = uVar18 | 0x38
                        cVar5 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                        if not cVar5 then return end  -- TODO(native): goto LAB_00d63b21
                    end
                    bVar4 = false
                else
                    -- LAB_00d63b21: (native jump target)
                    bVar4 = true
                end
                if (uVar19 & 0x20) ~= 0 then
                    uVar19 = uVar19 & 0xffffffdf
                end
                if (uVar19 & 0x10) ~= 0 then
                    uVar19 = uVar19 & 0xffffffef
                end
                if (uVar19 & 8) ~= 0 then
                    uVar19 = uVar19 & 0xfffffff7
                end
                uVar18 = uVar19
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d63c96 end
                    if __native_entity_state:GetStateBool("NotAttacked") then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d63c96 end
                        pCVar28 = 0x0
                        pCVar25 = 0x0
                        ppVar10 = quest:AddNewConversation(nil --[[missing]], (pCVar25 ~= 0), (pCVar28 ~= 0))
                        pcVar11 = quest:GetHero()
                        quest:AddPersonToConversation(bVar4, pcVar11)
                        r16 = quest:GetHero()
                        quest:AddLineToConversation(ppVar10, "TEXT_QST_028_MAZE_WOODS_ARTIFACT_THIEF_ATTACK", r16, nil --[[missing]])
                        __native_entity_state:SetStateBool("NotAttacked", false)
                        quest:EntitySetAsKillable(me, true, true)
                        piVar35 = quest:GetThingWithScriptName("ArtifactThiefRunMarker")
                        pCVar14 = piVar35:GetPos()
                        me:MoveToPosition(pCVar14, pCVar25, pCVar28, SUB41(pCVar13,0), pcVar11)
                        uVar18 = uVar19
                    end
                end
                bVar4 = me:IsPerformingScriptTask()
            until not (bVar4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
        end
    end
    ::LAB_00d63c96::
    ::LAB_00d63c9f::
end

function Init(quest, me)
    -- TODO(native): iStack_4 = this;
    -- TODO(native): ppVar1 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)(this + 8);
    quest:AddItemToContainer(nil --[[missing]], "OBJECT_HAND_LAMP")
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

