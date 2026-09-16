-- Generated native draft: SkillApprentice. Review coverage report before use.
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
    local __native_condition_1, bVar4, cVar5, fStack_138, fVar16, fVar26, fVar3, iVar10, iVar12, native_arg_sequence_1, native_arg_switch_2, pCVar1, pCVar22, pCVar24, pCVar25, pCVar27, pCVar29, pCVar8, pCVar9, paVar13, pcVar20, pfVar14, piVar2, ppVar23, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r2, r3, r4, r5, r6, r7, r8, r9, uVar17, uVar18, uVar19, uVar6, uVar7
    local alive = true
    -- TODO(native): apiStack_164[0] = (int *)0x0;
    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_170);
    if bVar4 then
    end
    -- TODO(native): pppuVar28 = &ppuStack_170;
    cVar5 = me:AcquireControl(4)
    while not cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00d4dcdb
        cVar5 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        -- LAB_00d4de61: (native jump target)
        return
    end
    piVar2 = *(this + 0x10)
    if piVar2 ~= nil then
        -- TODO(native): *piVar2 = *piVar2 + 1;
    end
    quest:SetIsPushableByHero(nil --[[missing]], false)
    quest:SetThingHasInformation(nil --[[missing]])
    pcVar20 = 0x1
    quest:EntitySetAsKillable(nil --[[missing]], (pcVar20 ~= 0))
    r1 = quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:EntityAttachToVillage(me, r1)
    uVar19 = 1
    me:SetFriendsWithEverythingFlag(1)
    __native_entity_state:SetStateBool("PlayerNotWarned", true)
    uVar7 = quest:RegisterTimer()
    quest:SetTimer(uVar7, 10)
    r2 = quest:GetThingWithScriptName("SkillApprenticeTargetMarker")
    alive = not quest:IsActiveThreadTerminating()
    cVar5 = not alive
    repeat
        if cVar5 then
            if (pcVar20 ~= nil) and (*pcVar20 = *pcVar20 + -1, *pcVar20 == 0) then
                -- TODO(native): (**(code **)(pcVar20 + 4))();
            end
            quest:DeregisterTimer(uVar7)
            -- LAB_00d4dcdb: (native jump target)
            return
        end
        cVar5 = quest:IsQuestActive("Q_GuildTrainingDeparture")
        if not cVar5 then goto LAB_00d4c9a2 end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            -- LAB_00d4dd41: (native jump target)
            quest:DeregisterTimer(uVar7)
            -- LAB_00d4de58: (native jump target)
            -- TODO(native): goto LAB_00d4de61
        end
        iVar10 = __native_entity_state:GetStateInt("self_0x18")
        if ((3 < *(iVar10 + 0xb4)) or (3 < *(iVar10 + 0xb8))) or (3 < *(iVar10 + 0xbc)) then
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                if cStack_15a ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d4de46
                    quest:ClearThingHasInformation(r2)
                    -- TODO(native): cStack_15a = '\0';
                end
                goto LAB_00d4c9a2
            end
            quest:DeregisterTimer(uVar7)
            -- TODO(native): goto LAB_00d4de58
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00d4de46
        if cStack_15a == 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d4de46
            quest:SetThingHasInformation(nil --[[missing]])
            -- TODO(native): cStack_15a = '\x01';
        end
        ::LAB_00d4c9a2::
        bVar4 = IsDistanceBetweenThingsOver(me,&uStack_168,4.0)
        __native_condition_1 = not bVar4
        if not __native_condition_1 then
            bVar4 = me:IsPerformingScriptTask()
            __native_condition_1 = bVar4
        end
        if __native_condition_1 then
            bVar4 = me:IsPerformingScriptTask()
            if bVar4 then goto LAB_00d4cbac end
            fVar26 = 10.0
            pCVar8 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar26)
            native_arg_sequence_1 = false
            if not bVar4 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                iVar10 = quest:GetTimer(uVar7)
                if 0 < iVar10 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then goto LAB_00d4cbac end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                r3 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(r3, nil --[[missing]])
                quest:SetTimer(uVar7, 0x0)
                r4 = quest:AddNewConversation(nil --[[missing]], false, false)
                pcVar20 = quest:GetHero()
                quest:AddPersonToConversation(0, pcVar20)
                if quest:GetMasterGameState("GlobalSkillGrade") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        r5 = quest:GetHero()
                        quest:AddLineToConversation(uVar19, "TEXT_QST_028_APPRENTICE_SKILL_EARLY_COMMENT", r5, nil --[[missing]])
                        -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_c0;
                        -- TODO(native): goto LAB_00d4cba7
                    end
                else
                    if quest:GetMasterGameState("GlobalSkillGrade") == 7 then
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            r6 = quest:GetHero()
                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_APPRENTICE_SKILL_APLUS_COMMENT", r6, nil --[[missing]])
                            -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_b0;
                            -- TODO(native): goto LAB_00d4cba7
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            r7 = quest:GetHero()
                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_APPRENTICE_SKILL_NOT_APLUS_COMMENT", r7, nil --[[missing]])
                            -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_b8;
                            -- LAB_00d4cba7: (native jump target)
                            goto LAB_00d4cbac
                        end
                    end
                end
            end
            -- TODO(native): goto LAB_00d4de46
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00d4dd41
        if apiStack_164[0] == nil then
        else
            pCVar8 = (**(*apiStack_164[0] + 0x18))()
        end
        me:MoveToPosition(nil --[[missing]], 0x40400000, 0x0, false, true)
        ::LAB_00d4cbac::
        cVar5 = me:IsTalkedToByHero()
        if cVar5 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                quest:DeregisterTimer(uVar7)
                -- TODO(native): goto LAB_00d4de58
            end
            bVar4 = me:IsPerformingScriptTask()
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_70);
                    pCVar22 = ""
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe78);
                    fVar16 = quest:GetHealth(nil --[[missing]])
                    fVar3 = _DAT_0122dedc
                    if fVar3 < fVar16 then
                        bVar4 = false
                        pCVar25 = 0x1
                        pCVar24 = 0x0
                        pCVar22 = 0x0
                        pcVar20 = "TEXT_QST_028_APPRENTICE_SKILL_EARLY"
                        pCVar9 = quest:GetHero()
                        r8 = me:Speak(pCVar9, pcVar20, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), bVar4)
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                    -- TODO(native): goto LAB_00d4de46
                                end
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            -- TODO(native): goto LAB_00d4de46
                        end
                    end
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    goto LAB_00d4dc36
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d4de46 end
                if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_80);
                        pCVar22 = ""
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe78);
                        fVar16 = quest:GetHealth(nil --[[missing]])
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar16 then
                            bVar4 = false
                            pCVar25 = 0x1
                            pCVar24 = 0x0
                            pCVar22 = 0x0
                            pcVar20 = "TEXT_QST_028_APPRENTICE_SKILL_TEST_ALREADY"
                            pCVar9 = quest:GetHero()
                            r9 = me:Speak(pCVar9, pcVar20, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                        -- TODO(native): goto LAB_00d4de46
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                -- TODO(native): goto LAB_00d4de46
                            end
                        end
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        goto LAB_00d4dc36
                    end
                    -- TODO(native): goto LAB_00d4de46
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d4de46 end
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_134);
                pCVar22 = ""
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe78);
                fVar16 = quest:GetHealth(nil --[[missing]])
                fVar3 = _DAT_0122dedc
                if fVar3 < fVar16 then
                    bVar4 = false
                    pCVar25 = 0x1
                    pCVar24 = 0x0
                    pCVar22 = 0x0
                    pcVar20 = "TEXT_QST_028_APPRENTICE_SKILL_HELLO"
                    pCVar9 = quest:GetHero()
                    r10 = me:Speak(pCVar9, pcVar20, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), bVar4)
                    bVar4 = me:IsPerformingScriptTask()
                    if bVar4 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4dded
                            bVar4 = me:IsPerformingScriptTask()
                        until not (bVar4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then goto LAB_00d4cf79 end
                    -- LAB_00d4d1f3: (native jump target)
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    -- TODO(native): goto LAB_00d4de46
                end
                ::LAB_00d4cf79::
                pcVar20 = "TEXT_QST_028_APPRENTICE_SKILL_QUESTION"
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_SKILL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                while iVar10 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d4dded
                    iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d4d1f3
                alive = not quest:IsActiveThreadTerminating()
                uVar6 = uVar7
                if iVar10 == 1 then
                    if alive then
                        if quest:GetMasterGameState("GlobalSkillGrade") ~= 7 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4d1f3
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe74);
                            fVar16 = quest:GetHealth(nil --[[missing]])
                            fVar3 = _DAT_0122dedc
                            uVar6 = uVar7
                            if fVar3 < fVar16 then
                                bVar4 = false
                                pCVar25 = 0x1
                                pCVar24 = 0x0
                                pCVar22 = 0x0
                                pcVar20 = "TEXT_QST_028_APPRENTICE_SKILL_TIME_LIMIT_APLUS"
                                pCVar9 = quest:GetHero()
                                r11 = me:Speak(pCVar9, pcVar20, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), bVar4)
                                bVar4 = me:IsPerformingScriptTask()
                                uVar6 = uVar7
                                if bVar4 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00d4dded end
                                        bVar4 = me:IsPerformingScriptTask()
                                        uVar6 = uVar7
                                    until not (bVar4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d4d1f3
                            end
                        end
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe74);
                        fVar16 = quest:GetHealth(nil --[[missing]])
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar16 then
                            bVar4 = false
                            pCVar25 = 0x1
                            pCVar24 = 0x0
                            pCVar22 = 0x0
                            pcVar20 = "TEXT_QST_028_APPRENTICE_SKILL_TIME_LIMIT"
                            pCVar9 = quest:GetHero()
                            r12 = me:Speak(pCVar9, pcVar20, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00d4dded end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4d1f3
                        end
                        goto LAB_00d4d2bb
                    end
                    ::LAB_00d4dded::
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    -- TODO(native): goto LAB_00d4de46
                end
                if not alive then return end  -- TODO(native): goto LAB_00d4dded
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe74);
                fVar16 = quest:GetHealth(nil --[[missing]])
                fVar3 = _DAT_0122dedc
                if fVar3 < fVar16 then
                    bVar4 = false
                    pCVar25 = 0x1
                    pCVar24 = 0x0
                    pCVar22 = 0x0
                    pcVar20 = "TEXT_QST_028_APPRENTICE_SKILL_RETURN"
                    pCVar9 = quest:GetHero()
                    r13 = me:Speak(pCVar9, pcVar20, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), bVar4)
                    bVar4 = me:IsPerformingScriptTask()
                    if bVar4 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4d1f3
                            bVar4 = me:IsPerformingScriptTask()
                        until not (bVar4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d4dded
                end
                ::LAB_00d4d2bb::
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                if iVar10 ~= 1 then goto LAB_00d4dc36 end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d4de46 end
                quest:SetPlayerUsingRangedDummies(nil --[[missing]])
                quest:SetMasterGameState("HeroTakingGuildTest", true)
                cVar5 = quest:MsgOnHeroFiredRangedWeapon()
                while not cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4de46 end
                    cVar5 = quest:MsgOnHeroFiredRangedWeapon()
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d4de46 end
                -- TODO(native): CTimer::CTimer((CTimer *)&uStack_16c);
                -- TODO(native): __ftol2();
                quest:SetTimer(uVar7, nil --[[missing]])
                quest:SetMasterGameState("SkillScore", 0)
                -- TODO(native): apiStack_164[0] = (int *)((uint)apiStack_164[0] & 0xffffff);
                pCVar22 = "HUD_ICON_MULTI_ARROW"
                uVar17 = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", nil --[[missing]], nil --[[missing]])
                -- TODO(native): auStack_a8[0] = (int)uVar17;
                uVar7 = 0x3f800000
                paVar13 = "HUD_ICON_ARROW"
                uVar18 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", uVar7, nil --[[missing]])
                uVar18 = quest:AddQuestInfoTimer(uVar7, "HUD_CLOCK_ICON", nil --[[missing]])
                -- TODO(native): apiStack_164[0] = (int *)uVar18;
                quest:DisplayQuestInfo(true)
                quest:UpdateQuestInfoCounter(uVar17, quest:GetMasterGameState("HighestSkillScore"), -1)
                pCVar29 = (pCVar29 & 0xffffff)
                uVar19 = 0xd4d482
                iVar10 = quest:GetTimer(uVar7)
                while (0 < iVar10 and (cStack_159 == 0)) do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4de3d end
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d4de3d end
                        -- TODO(native): cStack_159 = '\x01';
                    end
                    if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d4de3d end
                        quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                        quest:UpdateQuestInfoCounter(1, nil --[[missing]], nil --[[missing]])
                    end
                    fVar26 = -NAN
                    quest:UpdateQuestInfoCounter(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                    pCVar9 = quest:GetThingWithScriptName("ArcheryRing")
                    pCVar8 = quest:GetHero()
                    bVar4 = IsDistanceBetweenThingsOver(pCVar8,pCVar9,fVar26)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d4de3d end
                        if __native_entity_state:GetStateBool("PlayerNotWarned") then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d4de3d end
                            __native_entity_state:SetStateBool("PlayerNotWarned", false)
                            r14 = quest:AddNewConversation(pCVar8, nil --[[missing]], nil --[[missing]])
                            r15 = quest:GetHero()
                            quest:AddPersonToConversation(nil --[[missing]], r15)
                            r16 = quest:GetHero()
                            pcVar20 = "TEXT_QST_028_APPRENTICE_SKILL_RING_OUT"
                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_APPRENTICE_SKILL_RING_OUT", r16, pCVar9)
                        end
                        -- TODO(native): cStack_159 = '\x01';
                    end
                    iVar10 = quest:GetTimer(uVar7)
                end
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    cVar5 = quest:IsHeroControlledByPlayer()
                    while not cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d4de3d end
                        cVar5 = quest:IsHeroControlledByPlayer()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4de3d end
                    quest:DisplayQuestInfo(nil --[[missing]])
                    quest:RemoveQuestInfoElement(nil --[[missing]])
                    quest:RemoveQuestInfoElement(nil --[[missing]])
                    quest:RemoveQuestInfoElement(nil --[[missing]])
                    if cStack_159 ~= 0 then
                        -- LAB_00d4dc16: (native jump target)
                        quest:SetMasterGameState("HeroTakingGuildTest", false)
                        quest:SetPlayerUsingRangedDummies(nil --[[missing]])
                        goto LAB_00d4dc36
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4de3d end
                    fStack_138 = quest:GetMasterGameState("SkillScore")
                    -- TODO(native): pfVar14 = *(float **)(DAT_0143e90c + 0xec0);
                    iVar10 = 0
                    repeat
                        iVar12 = iVar10
                        if *pfVar14 < fStack_138 ~= (*pfVar14 == fStack_138) then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d4de3d end
                            break
                        end
                        pfVar14 = pfVar14 + 1
                        iVar10 = iVar12 + 1
                    until not (iVar12 + 1 < 7)
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aaStack_124);
                    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aaStack_124);
                    if bVar4 then
                    end
                    r17 = quest:GetHero()
                    cVar5 = me:AcquireControl(4)
                    while not cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d4de34 end
                        r18 = quest:GetHero()
                        cVar5 = me:AcquireControl(4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): StdMap_Construct_API();
                        -- TODO(native): pCVar11 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_144,aCStack_ac);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar11,pCVar21);
                        -- TODO(native): pCVar11 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_144,aCStack_e4);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar11, (CScriptGameResourceObjectScriptedThingBase *)paVar13);
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_90);
                        pCVar22 = ""
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        quest:FixMovieSequenceCamera(nil --[[missing]])
                        ppVar23 = 0x0
                        -- TODO(native): RunCutsceneMacro_Func();
                        if uStack_16c._3_1_ == 0 then
                            -- LAB_00d4d979: (native jump target)
                            -- TODO(native): pCVar11 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)apiStack_164,aCStack_d4);
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar11,pCVar21);
                            native_arg_switch_2 = iVar12
                            repeat
                                if native_arg_switch_2 == 0 then
                                    if quest:GetMasterGameState("GlobalSkillGrade") == 7 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00d4de05 end
                                        -- TODO(native): RunCutsceneMacro_Func(0);
                                        -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_c4;
                                        break
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        ppVar23 = 0x0
                                        -- TODO(native): RunCutsceneMacro_Func();
                                        quest:ClearThingHasInformation(me)
                                        goto FLOW_native_label_1
                                    end
                                    -- TODO(native): goto LAB_00d4de13
                                else
                                    if native_arg_switch_2 == 1 then
                                        -- TODO(native): RunCutsceneMacro_Func(0);
                                        -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_114;
                                        break
                                    else
                                        if native_arg_switch_2 == 2 then
                                            -- TODO(native): RunCutsceneMacro_Func(0);
                                            -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_110;
                                            break
                                        else
                                            if native_arg_switch_2 == 3 then
                                                -- TODO(native): RunCutsceneMacro_Func(0);
                                                -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_130;
                                                break
                                            else
                                                if native_arg_switch_2 == 4 then
                                                    -- TODO(native): RunCutsceneMacro_Func(0);
                                                    paVar13 = "CS_GUILD_DEPARTURE_SKILL_TEST_D"
                                                    break
                                                else
                                                    if native_arg_switch_2 == 5 then
                                                        -- TODO(native): RunCutsceneMacro_Func(0);
                                                        paVar13 = "CS_GUILD_DEPARTURE_SKILL_TEST_E"
                                                        break
                                                    else
                                                        if native_arg_switch_2 == 6 then
                                                            -- TODO(native): RunCutsceneMacro_Func(0);
                                                            -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_11c;
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
                            if quest:GetMasterGameState("GlobalSkillGrade") < 7 - iVar12 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    -- LAB_00d4de13: (native jump target)
                                    quest:PauseAllNonScriptedEntities((ppVar23 ~= 0))
                                    goto LAB_00d4de1f
                                end
                                quest:SetMasterGameState("GlobalSkillGrade", 7 - iVar12)
                            end
                            pcVar20 = 0x0
                            quest:FixMovieSequenceCamera((pcVar20 ~= 0))
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            -- TODO(native): StdMap_Destroy_API();
                            -- TODO(native): goto LAB_00d4dc16
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): pCVar11 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)apiStack_164,(CCharString *)aCStack_134);
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar11,pCVar21);
                            -- TODO(native): RunCutsceneMacro_Func(0);
                            -- TODO(native): goto LAB_00d4d979
                        end
                        ::LAB_00d4de05::
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        ::LAB_00d4de1f::
                        -- TODO(native): StdMap_Destroy_API();
                    end
                    ::LAB_00d4de34::
                end
                ::LAB_00d4de3d::
            end
            ::LAB_00d4de46::
            -- TODO(native): goto LAB_00d4de58
        end
        ::LAB_00d4dc36::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        uVar7 = uVar19
        cVar5 = extraout_AL_52
    until false
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

