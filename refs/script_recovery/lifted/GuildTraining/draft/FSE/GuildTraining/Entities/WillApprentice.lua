-- Generated native draft: WillApprentice. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, aVar22, bVar4, cVar5, cVar7, fStack_114, fVar18, fVar29, fVar3, iVar11, iVar14, native_arg_sequence_1, native_arg_switch_2, pCVar1, pCVar10, pCVar21, pCVar25, pCVar26, pCVar27, pCVar30, pCVar9, pVar28, paVar15, pcVar23, pfVar16, piVar2, ppVar12, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r2, r3, r4, r5, r6, r7, r8, r9, uVar19, uVar20, uVar6, uVar8
    local alive = true
    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_148);
    if bVar4 then
    end
    -- TODO(native): pppuVar31 = &ppuStack_148;
    cVar5 = me:AcquireControl(4)
    while not cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            if (ppuStack_148 == nil) or (*ppuStack_148 = *ppuStack_148 + -1, *ppuStack_148 ~= nil) then return end  -- TODO(native): goto LAB_00d50495
            -- TODO(native): goto LAB_00d5048d
        end
        cVar5 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        -- LAB_00d505c1: (native jump target)
        return
    end
    piVar2 = *(this + 0x10)
    if piVar2 ~= nil then
        -- TODO(native): *piVar2 = *piVar2 + 1;
    end
    quest:SetIsPushableByHero(nil --[[missing]], false)
    quest:SetThingHasInformation(nil --[[missing]])
    quest:EntitySetAsKillable(nil --[[missing]], false)
    r1 = quest:GetNearestWithDefName(nil --[[missing]], "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:EntityAttachToVillage(me, r1)
    uVar20 = 1
    me:SetFriendsWithEverythingFlag(1)
    r2 = quest:GetThingWithScriptName("WillApprenticeTargetMarker")
    uVar8 = quest:RegisterTimer()
    quest:SetTimer(uVar8, 10)
    alive = not quest:IsActiveThreadTerminating()
    cVar5 = not alive
    repeat
        if cVar5 then
            quest:DeregisterTimer(uVar8)
            -- LAB_00d50495: (native jump target)
            return
        end
        cVar5 = quest:IsQuestActive("Q_GuildTrainingDeparture")
        if not cVar5 then goto LAB_00d4f285 end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            -- LAB_00d504b9: (native jump target)
            quest:DeregisterTimer(uVar8)
            -- LAB_00d505af: (native jump target)
            -- TODO(native): goto LAB_00d505c1
        end
        iVar11 = __native_entity_state:GetStateInt("self_0x18")
        if ((3 < *(iVar11 + 0xb4)) or (3 < *(iVar11 + 0xb8))) or (3 < *(iVar11 + 0xbc)) then
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                if uStack_140._3_1_ ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d505a6
                    quest:ClearThingHasInformation(r2)
                    -- TODO(native): uStack_140 = uStack_140 & 0xffffff;
                end
                goto LAB_00d4f285
            end
            -- TODO(native): goto LAB_00d504b9
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00d505a6
        if uStack_140._3_1_ == 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d505a6
            quest:SetThingHasInformation(nil --[[missing]])
            -- TODO(native): uStack_140 = CONCAT13(1,(undefined3)uStack_140);
        end
        ::LAB_00d4f285::
        bVar4 = IsDistanceBetweenThingsOver(me,&ppuStack_13c,4.0)
        __native_condition_1 = not bVar4
        if not __native_condition_1 then
            bVar4 = me:IsPerformingScriptTask()
            __native_condition_1 = bVar4
        end
        if __native_condition_1 then
            bVar4 = me:IsPerformingScriptTask()
            if bVar4 then goto LAB_00d4f49e end
            fVar29 = 10.0
            pCVar9 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar9, fVar29)
            native_arg_sequence_1 = false
            if not bVar4 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                iVar11 = quest:GetTimer(uVar8)
                if 0 < iVar11 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then goto LAB_00d4f49e end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                r3 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(r3, nil --[[missing]])
                quest:SetTimer(uVar8, 0x0)
                r4 = quest:AddNewConversation(nil --[[missing]], false, false)
                r5 = quest:GetHero()
                quest:AddPersonToConversation(0x0, r5)
                if quest:GetMasterGameState("GlobalWillGrade") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        r6 = quest:GetHero()
                        quest:AddLineToConversation(0x0, "TEXT_QST_028_APPRENTICE_WILL_EARLY_COMMENT", r6, nil --[[missing]])
                        -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_ac;
                        -- TODO(native): goto LAB_00d4f499
                    end
                else
                    if quest:GetMasterGameState("GlobalWillGrade") == 7 then
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            r7 = quest:GetHero()
                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_APPRENTICE_WILL_APLUS_COMMENT", r7, nil --[[missing]])
                            -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_9c;
                            -- TODO(native): goto LAB_00d4f499
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            r8 = quest:GetHero()
                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_APPRENTICE_WILL_NOT_APLUS_COMMENT", r8, nil --[[missing]])
                            -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_a4;
                            -- LAB_00d4f499: (native jump target)
                            goto LAB_00d4f49e
                        end
                    end
                end
            end
            -- TODO(native): goto LAB_00d505a6
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00d504b9
        if piStack_138 == nil then
        else
            pCVar9 = (**(*piStack_138 + 0x18))()
        end
        me:MoveToPosition(nil --[[missing]], pCVar9, 0x40400000, false, false)
        ::LAB_00d4f49e::
        cVar5 = me:IsTalkedToByHero()
        if cVar5 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d504b9
            bVar4 = me:IsPerformingScriptTask()
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_58);
                    pCVar25 = ""
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffea0);
                    fVar18 = quest:GetHealth(nil --[[missing]])
                    fVar3 = _DAT_0122dedc
                    if fVar3 < fVar18 then
                        pVar28 = 0x0
                        pCVar27 = 0x1
                        pCVar26 = 0x0
                        pCVar25 = 0x0
                        pcVar23 = "TEXT_QST_028_APPRENTICE_WILL_EARLY"
                        pCVar10 = quest:GetHero()
                        r9 = me:Speak(pCVar10, pcVar23, pCVar25, (pCVar26 ~= 0), (pCVar27 ~= 0), (pVar28 ~= 0))
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities((pVar28 ~= 0))
                                    -- TODO(native): goto LAB_00d505a6
                                end
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            -- TODO(native): goto LAB_00d505a6
                        end
                    end
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    goto LAB_00d503cb
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d505a6 end
                if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_68);
                        pCVar25 = ""
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffea0);
                        fVar18 = quest:GetHealth(nil --[[missing]])
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar18 then
                            pVar28 = 0x0
                            pCVar27 = 0x1
                            pCVar26 = 0x0
                            pCVar25 = 0x0
                            pcVar23 = "TEXT_QST_028_APPRENTICE_WILL_MULTI_GRADE"
                            pCVar10 = quest:GetHero()
                            r10 = me:Speak(pCVar10, pcVar23, pCVar25, (pCVar26 ~= 0), (pCVar27 ~= 0), (pVar28 ~= 0))
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities((pVar28 ~= 0))
                                        -- TODO(native): goto LAB_00d505a6
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                -- TODO(native): goto LAB_00d505a6
                            end
                        end
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        goto LAB_00d503cb
                    end
                    -- TODO(native): goto LAB_00d505a6
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d505a6 end
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_110);
                pCVar25 = ""
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                pCVar25 = "TEXT_QST_028_APPRENTICE_WILL_HELLO"
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_WILL_HELLO", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                iVar11 = quest:MsgIsQuestionAnsweredYesOrNo()
                while iVar11 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d50542
                    iVar11 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d4fa43
                alive = not quest:IsActiveThreadTerminating()
                uVar6 = SUB41(pCVar25,0)
                if iVar11 ~= 1 then
                    if alive then
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe8c);
                        fVar18 = quest:GetHealth(nil --[[missing]])
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar18 then
                            aVar22 = 0x0
                            pCVar27 = 0x1
                            pCVar26 = 0x0
                            pCVar25 = 0x0
                            pcVar23 = "TEXT_QST_028_APPRENTICE_WILL_RETURN"
                            pCVar10 = quest:GetHero()
                            r11 = me:Speak(pCVar10, pcVar23, pCVar25, (pCVar26 ~= 0), (pCVar27 ~= 0), (aVar22 ~= 0))
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d4fa43
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d50542 end
                        end
                        goto LAB_00d4fb0a
                    end
                    ::LAB_00d50542::
                    quest:PauseAllNonScriptedEntities((aVar22 ~= 0))
                    -- TODO(native): goto LAB_00d505a6
                end
                if not alive then return end  -- TODO(native): goto LAB_00d50542
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe8c);
                fVar18 = quest:GetHealth(nil --[[missing]])
                fVar3 = _DAT_0122dedc
                uVar6 = SUB41(pCVar25,0)
                if fVar3 < fVar18 then
                    aVar22 = 0x0
                    pCVar21 = 0x1
                    pCVar27 = 0x0
                    pCVar26 = 0x0
                    pcVar23 = "TEXT_QST_028_APPRENTICE_WILL_INTRO"
                    pCVar10 = quest:GetHero()
                    r12 = me:Speak(pCVar10, pcVar23, pCVar26, (pCVar27 ~= 0), (pCVar21 ~= 0), (aVar22 ~= 0))
                    bVar4 = me:IsPerformingScriptTask()
                    uVar6 = SUB41(pCVar25,0)
                    if bVar4 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4fa43
                            bVar4 = me:IsPerformingScriptTask()
                            uVar6 = SUB41(pCVar25,0)
                        until not (bVar4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d50542
                end
                if quest:GetMasterGameState("GlobalWillGrade") ~= 7 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        -- LAB_00d4fa43: (native jump target)
                        quest:PauseAllNonScriptedEntities((aVar22 ~= 0))
                        -- TODO(native): goto LAB_00d505a6
                    end
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe8c);
                    fVar18 = quest:GetHealth(nil --[[missing]])
                    fVar3 = _DAT_0122dedc
                    if fVar3 < fVar18 then
                        aVar22 = 0x0
                        pCVar27 = 0x1
                        pCVar26 = 0x0
                        pCVar25 = 0x0
                        pcVar23 = "TEXT_QST_028_APPRENTICE_WILL_INTRO_APLUS"
                        pCVar10 = quest:GetHero()
                        r13 = me:Speak(pCVar10, pcVar23, pCVar25, (pCVar26 ~= 0), (pCVar27 ~= 0), (aVar22 ~= 0))
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d50542
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d4fa43
                    end
                end
                ::LAB_00d4fb0a::
                quest:PauseAllNonScriptedEntities((aVar22 ~= 0))
                if iVar11 ~= 1 then goto LAB_00d503cb end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d505a6 end
                quest:SetPlayerUsingWillDummies(nil --[[missing]])
                quest:SetMasterGameState("HeroTakingGuildTest", true)
                cVar5 = quest:MsgOnHeroCastSpell()
                while cVar5 == 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d505a6 end
                    cVar5 = quest:MsgOnHeroCastSpell()
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d505a6 end
                -- TODO(native): CTimer::CTimer(aCStack_144);
                -- TODO(native): __ftol2();
                quest:SetTimer(uVar8, nil --[[missing]])
                quest:SetMasterGameState("WillScore", 0)
                quest:SetTimer(uVar8, nil --[[missing]])
                pCVar25 = "HUD_ICON_ARROW"
                uVar19 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", nil --[[missing]], nil --[[missing]])
                uVar19 = quest:AddQuestInfoTimer(uVar8, "HUD_CLOCK_ICON", nil --[[missing]])
                quest:DisplayQuestInfo(nil --[[missing]])
                cVar5 = 0
                -- TODO(native): CTimer::CTimer((CTimer *)&stack0xfffffe7c);
                quest:SetTimer(uVar8, nil --[[missing]])
                iVar11 = quest:GetTimer(uVar8)
                while (0 < iVar11 and (cVar5 == 0)) do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d50594 end
                    iVar11 = quest:GetTimer(uVar8)
                    if iVar11 < 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d50594 end
                        r14 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(r14, nil --[[missing]])
                        quest:SetTimer(uVar8, nil --[[missing]])
                    end
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d50594 end
                        cVar5 = '\x01'
                        -- TODO(native): cStack_121 = '\x01';
                    end
                    iVar11 = quest:GetHeroWillEnergy()
                    __native_condition_2 = iVar11 == 0
                    if __native_condition_2 then
                        iVar11 = quest:GetTimer(uVar8)
                        __native_condition_2 = iVar11 < 1
                    end
                    if __native_condition_2 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d50594 end
                        ppVar12 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                        r15 = quest:GetHero()
                        quest:AddPersonToConversation(nil --[[missing]], r15)
                        r16 = quest:GetHero()
                        quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_APPRENTICE_WILL_NO_WILL", r16, nil --[[missing]])
                        quest:SetTimer(uVar8, nil --[[missing]])
                    end
                    fVar29 = 30.0
                    pCVar10 = quest:GetHero()
                    bVar4 = IsDistanceBetweenThingsOver(pCVar10,(me),fVar29)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d50594 end
                        cVar5 = '\x01'
                        -- TODO(native): cStack_121 = '\x01';
                    end
                    quest:UpdateQuestInfoCounter(fVar29, nil --[[missing]], nil --[[missing]])
                    iVar11 = quest:GetTimer(uVar8)
                end
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    cVar7 = quest:IsHeroControlledByPlayer()
                    while not cVar7 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d50594 end
                        cVar7 = quest:IsHeroControlledByPlayer()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d50594 end
                    quest:DisplayQuestInfo(nil --[[missing]])
                    quest:RemoveQuestInfoElement(nil --[[missing]])
                    quest:RemoveQuestInfoElement(nil --[[missing]])
                    if cVar5 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d50594 end
                        fStack_114 = quest:GetMasterGameState("WillScore")
                        -- TODO(native): pfVar16 = *(float **)(DAT_0143e90c + 0xecc);
                        iVar11 = 0
                        repeat
                            iVar14 = iVar11
                            if *pfVar16 < fStack_114 ~= (*pfVar16 == fStack_114) then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d50594 end
                                break
                            end
                            pfVar16 = pfVar16 + 1
                            iVar11 = iVar14 + 1
                        until not (iVar14 + 1 < 7)
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aaStack_e4);
                        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aaStack_e4);
                        if bVar4 then
                        end
                        r17 = quest:GetHero()
                        cVar5 = me:AcquireControl(4)
                        while not cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d50588
                            r18 = quest:GetHero()
                            cVar5 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00d50588: (native jump target)
                            goto LAB_00d50594
                        end
                        -- TODO(native): StdMap_Construct_API();
                        -- TODO(native): pCVar13 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_120,aCStack_a8);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar13,pCVar24);
                        -- TODO(native): pCVar13 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_120,aCStack_c8);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar13, (CScriptGameResourceObjectScriptedThingBase *)paVar15);
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_78);
                        pCVar25 = ""
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        quest:FixMovieSequenceCamera(nil --[[missing]])
                        ppVar12 = 0x0
                        -- TODO(native): RunCutsceneMacro_Func();
                        -- TODO(native): pCVar13 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_140,(CCharString *)amStack_120);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar13,pCVar24);
                        native_arg_switch_2 = iVar14
                        repeat
                            if native_arg_switch_2 == 0 then
                                if quest:GetMasterGameState("GlobalWillGrade") ~= 7 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        ppVar12 = 0x0
                                        -- TODO(native): RunCutsceneMacro_Func();
                                        quest:ClearThingHasInformation(r18)
                                        goto FLOW_native_label_1
                                    end
                                    -- TODO(native): goto LAB_00d50567
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    -- TODO(native): RunCutsceneMacro_Func();
                                    -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_b0;
                                    break
                                end
                                quest:PauseAllNonScriptedEntities((ppVar12 ~= 0))
                                -- TODO(native): goto LAB_00d50573
                            else
                                if native_arg_switch_2 == 1 then
                                    -- TODO(native): RunCutsceneMacro_Func();
                                    -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_d0;
                                    break
                                else
                                    if native_arg_switch_2 == 2 then
                                        -- TODO(native): RunCutsceneMacro_Func();
                                        -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_108 ;
                                        break
                                    else
                                        if native_arg_switch_2 == 3 then
                                            -- TODO(native): RunCutsceneMacro_Func();
                                            -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_f4;
                                            break
                                        else
                                            if native_arg_switch_2 == 4 then
                                                -- TODO(native): RunCutsceneMacro_Func();
                                                -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_11c ;
                                                break
                                            else
                                                if native_arg_switch_2 == 5 then
                                                    -- TODO(native): RunCutsceneMacro_Func();
                                                    -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_118 ;
                                                    break
                                                else
                                                    if native_arg_switch_2 == 6 then
                                                        -- TODO(native): RunCutsceneMacro_Func();
                                                        -- TODO(native): paVar15 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_110 ;
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
                        if quest:GetMasterGameState("GlobalWillGrade") < 7 - iVar14 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                -- LAB_00d50567: (native jump target)
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                -- LAB_00d50573: (native jump target)
                                -- TODO(native): StdMap_Destroy_API();
                                -- TODO(native): goto LAB_00d50588
                            end
                            quest:SetMasterGameState("GlobalWillGrade", 7 - iVar14)
                        end
                        quest:FixMovieSequenceCamera(nil --[[missing]])
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        -- TODO(native): StdMap_Destroy_API();
                    end
                    quest:SetMasterGameState("HeroTakingGuildTest", false)
                    quest:SetPlayerUsingWillDummies(nil --[[missing]])
                    goto LAB_00d503cb
                end
                ::LAB_00d50594::
            end
            ::LAB_00d505a6::
            -- TODO(native): goto LAB_00d505af
        end
        ::LAB_00d503cb::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cVar5 = not alive
    until false
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

