-- Generated native draft: CombatApprentice. Review coverage report before use.
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
    local __native_condition_1, bVar3, cVar4, fVar17, fVar18, fVar32, iVar10, iVar13, native_arg_switch_2, pCVar1, pCVar22, pCVar25, pCVar26, pCVar27, pCVar28, pCVar29, pCVar30, pCVar33, pCVar8, pCVar9, pVar5, paVar14, pcVar24, pfVar15, piVar31, ppVar11, ppVar21, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r3, r4, r5, r6, r7, r8, r9, uVar19, uVar20, uVar7
    local alive = true
    -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_254);
    if bVar3 then
    end
    -- TODO(native): pppuVar34 = &ppuStack_254;
    cVar4 = me:AcquireControl(4)
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            if (ppuStack_254 == nil) or (*ppuStack_254 = *ppuStack_254 + -1, *ppuStack_254 ~= nil) then return end  -- TODO(native): goto LAB_00d4c4a5
            -- TODO(native): goto LAB_00d4c49d
        end
        cVar4 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        -- LAB_00d4c6ec: (native jump target)
        return
    end
    pCVar30 = *(this + 0x10)
    if pCVar30 ~= nil then
        -- TODO(native): *(int *)pCVar30 = *(int *)pCVar30 + 1;
    end
    quest:SetIsPushableByHero(nil --[[missing]], false)
    quest:SetThingHasInformation(nil --[[missing]])
    quest:EntitySetInFaction(nil --[[missing]], "FACTION_HERO")
    quest:EntitySetAsKillable(nil --[[missing]], false)
    uVar20 = 1
    me:SetFriendsWithEverythingFlag(1)
    uVar7 = quest:RegisterTimer()
    quest:SetTimer(uVar7, 10)
    r1 = quest:GetThingWithScriptName("CombatApprenticeTargetMarker")
    alive = not quest:IsActiveThreadTerminating()
    cVar4 = not alive
    repeat
        if cVar4 then
            if (pCVar30 ~= nil) and (*pCVar30 = *pCVar30 + -1, *pCVar30 == 0) then
                -- TODO(native): (**(code **)(pCVar30 + 4))();
            end
            quest:DeregisterTimer(uVar7)
            -- LAB_00d4c4a5: (native jump target)
            return
        end
        cVar4 = quest:IsQuestActive("Q_GuildTrainingDeparture")
        if not cVar4 then goto LAB_00d4a512 end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00d4c4f5
        iVar10 = __native_entity_state:GetStateInt("self_0x18")
        if ((*(iVar10 + 0xb4) < 4) and (*(iVar10 + 0xb8) < 4)) and (*(iVar10 + 0xbc) < 4) then
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                if cStack_236 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d4c6da
                    quest:SetThingHasInformation(r1)
                    -- TODO(native): cStack_236 = '\x01';
                end
                goto LAB_00d4a512
            end
            -- TODO(native): goto LAB_00d4c6da
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            quest:DeregisterTimer(uVar7)
            return
        end
        if cStack_236 ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d4c6da
            quest:ClearThingHasInformation(nil --[[missing]])
            -- TODO(native): cStack_236 = '\0';
        end
        ::LAB_00d4a512::
        pCVar30 = 0xd4a525
        bVar3 = IsDistanceBetweenThingsOver(me,aCStack_244,4.0)
        __native_condition_1 = not bVar3
        if not __native_condition_1 then
            bVar3 = me:IsPerformingScriptTask()
            __native_condition_1 = bVar3
        end
        if __native_condition_1 then
            bVar3 = me:IsPerformingScriptTask()
            if bVar3 then goto LAB_00d4a71c end
            fVar32 = 10.0
            pCVar8 = quest:GetHero()
            pCVar30 = 0xd4a59d
            bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar32)
            if not bVar3 then goto LAB_00d4a71c end
            pCVar30 = 0xd4a5b8
            uVar19 = quest:GetTimer(uVar7)
            -- TODO(native): ppVar16 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar19 >> 0x20);
            if 0 < uVar19 then goto LAB_00d4a71c end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                r2 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(r2, nil --[[missing]])
                quest:SetTimer(uVar7, 0x0)
                r3 = quest:AddNewConversation(nil --[[missing]], false, (uVar20 ~= 0))
                r4 = quest:GetHero()
                quest:AddPersonToConversation(0x0, r4)
                if quest:GetMasterGameState("GlobalMeleeGrade") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        r5 = quest:GetHero()
                        quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_COMMENT", r5, nil --[[missing]])
                        -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_1a8;
                        -- TODO(native): goto LAB_00d4a717
                    end
                else
                    if quest:GetMasterGameState("GlobalMeleeGrade") == 7 then
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            r6 = quest:GetHero()
                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_APPRENTICE_MELEE_APLUS_COMMENT", r6, nil --[[missing]])
                            -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_13c;
                            -- TODO(native): goto LAB_00d4a717
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            r7 = quest:GetHero()
                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_APPRENTICE_MELEE_NOT_APLUS_COMMENT", r7, nil --[[missing]])
                            -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_144;
                            -- LAB_00d4a717: (native jump target)
                            goto LAB_00d4a71c
                        end
                    end
                end
            end
            -- TODO(native): goto LAB_00d4c6da
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            -- LAB_00d4c4f5: (native jump target)
            quest:DeregisterTimer(uVar7)
            return
        end
        if piStack_240 == nil then
        else
            pCVar8 = (**(*piStack_240 + 0x18))()
        end
        pCVar30 = 0x0
        me:MoveToPosition(nil --[[missing]], pCVar8, 0x40400000, true, false)
        ::LAB_00d4a71c::
        if not __native_entity_state:GetStateBool("WaitingForFight") then
            -- LAB_00d4a758: (native jump target)
            bVar3 = false
        else
            -- TODO(native): uStack_234 = uStack_234 | 1;
            pCVar30 = 0xd4a750
            cVar4 = me:IsTalkedToByHero()
            if not cVar4 then return end  -- TODO(native): goto LAB_00d4a758
            bVar3 = true
        end
        if (uStack_234 & 1) ~= 0 then
            -- TODO(native): uStack_234 = uStack_234 & 0xfffffffe;
        end
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                quest:DeregisterTimer(uVar7)
                return
            end
            bVar3 = me:IsPerformingScriptTask()
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d4c6da end
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_f8);
                pCVar30 = ""
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                -- TODO(native): pVar5 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd94);
                fVar17 = quest:GetHealth(nil --[[missing]])
                fVar18 = _DAT_0122dedc
                if fVar18 < fVar17 then
                    bVar3 = false
                    pCVar29 = 0x1
                    pCVar28 = 0x0
                    pCVar27 = 0x0
                    pcVar24 = "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_EARLY"
                    pCVar9 = quest:GetHero()
                    r8 = me:Speak(pCVar9, pcVar24, pCVar27, (pCVar28 ~= 0), (pCVar29 ~= 0), bVar3)
                    bVar3 = me:IsPerformingScriptTask()
                    if bVar3 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                -- TODO(native): goto LAB_00d4c6da
                            end
                            bVar3 = me:IsPerformingScriptTask()
                        until not (bVar3)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        -- TODO(native): goto LAB_00d4c6da
                    end
                end
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                goto LAB_00d4c416
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_e8);
                        pCVar30 = ""
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        -- TODO(native): pVar5 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd94);
                        fVar17 = quest:GetHealth(nil --[[missing]])
                        fVar18 = _DAT_0122dedc
                        if fVar18 < fVar17 then
                            bVar3 = false
                            pCVar29 = 0x1
                            pCVar28 = 0x0
                            pCVar27 = 0x0
                            pcVar24 = "TEXT_QST_028_APPRENTICE_MELEE_OTHER_MELEE_GRADE"
                            pCVar9 = quest:GetHero()
                            r9 = me:Speak(pCVar9, pcVar24, pCVar27, (pCVar28 ~= 0), (pCVar29 ~= 0), bVar3)
                            bVar3 = me:IsPerformingScriptTask()
                            if bVar3 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                        -- TODO(native): goto LAB_00d4c6da
                                    end
                                    bVar3 = me:IsPerformingScriptTask()
                                until not (bVar3)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                -- TODO(native): goto LAB_00d4c6da
                            end
                        end
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        goto LAB_00d4c416
                    end
                    -- TODO(native): goto LAB_00d4c6da
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d4c6da end
                r10 = quest:GetThingWithScriptName("MeleeApprentice")
                bVar3 = aCStack_250:IsAlive()
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4c6d1 end
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_218);
                    pCVar30 = ""
                    quest:StartMovieSequence()
                    -- TODO(native): piStack_228 = piVar31;
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd94);
                    fVar17 = quest:GetHealth(r10)
                    fVar18 = _DAT_0122dedc
                    if fVar18 < fVar17 then
                        bVar3 = false
                        pCVar29 = 0x1
                        pCVar28 = 0x0
                        pCVar27 = 0x0
                        pcVar24 = "TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_HELLO"
                        pCVar9 = quest:GetHero()
                        r11 = me:Speak(pCVar9, pcVar24, pCVar27, (pCVar28 ~= 0), (pCVar29 ~= 0), bVar3)
                        bVar3 = me:IsPerformingScriptTask()
                        if bVar3 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d4ae53
                                bVar3 = me:IsPerformingScriptTask()
                            until not (bVar3)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then goto LAB_00d4ac95 end
                        -- LAB_00d4c5ff: (native jump target)
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        goto LAB_00d4c6d1
                    end
                    ::LAB_00d4ac95::
                    pCVar27 = "TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_QUESTION"
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                    iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar10 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d4ae53
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    pVar5 = SUB41(uVar7,0)
                    if not alive then return end  -- TODO(native): goto LAB_00d4c5ff
                    if iVar10 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00d4ae53: (native jump target)
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            goto LAB_00d4c6d1
                        end
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd90);
                        fVar18 = quest:GetHealth(nil --[[missing]])
                        -- TODO(native): pppuVar2 = (undefined ***)CONCAT13(1,(int3)pppuVar34);
                        if fVar18 <= _DAT_0122dedc then
                            -- TODO(native): pppuVar2 = (undefined ***)((uint)pppuVar34 & 0xffffff);
                        end
                        -- TODO(native): pppuVar34 = pppuVar2;
                        if (pppuVar34 >> 0x18) ~= 0 then
                            bVar3 = false
                            pCVar29 = 0x1
                            pCVar28 = 0x0
                            pCVar27 = 0x0
                            pcVar24 = "TEXT_QST_028_APPRENTICE_MELEE_RETURN"
                            pCVar9 = quest:GetHero()
                            r12 = me:Speak(pCVar9, pcVar24, pCVar27, (pCVar28 ~= 0), (pCVar29 ~= 0), bVar3)
                            bVar3 = me:IsPerformingScriptTask()
                            if bVar3 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d4c5ff
                                    bVar3 = me:IsPerformingScriptTask()
                                until not (bVar3)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4ae53
                        end
                    else
                        if iVar10 ~= 1 then goto LAB_00d4b11f end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d4c5ff
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd90);
                        fVar18 = quest:GetHealth(nil --[[missing]])
                        -- TODO(native): pppuVar2 = (undefined ***)CONCAT13(1,(int3)pppuVar34);
                        if fVar18 <= _DAT_0122dedc then
                            -- TODO(native): pppuVar2 = (undefined ***)((uint)pppuVar34 & 0xffffff);
                        end
                        -- TODO(native): pppuVar34 = pppuVar2;
                        pVar5 = SUB41(uVar7,0)
                        if (pppuVar34 >> 0x18) ~= 0 then
                            bVar3 = false
                            pCVar29 = 0x1
                            pCVar28 = 0x0
                            pCVar27 = 0x0
                            pcVar24 = "TEXT_QST_028_APPRENTICE_MELEE_START"
                            pCVar9 = quest:GetHero()
                            r13 = me:Speak(pCVar9, pcVar24, pCVar27, (pCVar28 ~= 0), (pCVar29 ~= 0), bVar3)
                            bVar3 = me:IsPerformingScriptTask()
                            pVar5 = SUB41(uVar7,0)
                            if bVar3 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d4ae53
                                    bVar3 = me:IsPerformingScriptTask()
                                    pVar5 = SUB41(uVar7,0)
                                until not (bVar3)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4c5ff
                        end
                        if quest:GetMasterGameState("GlobalMeleeGrade") ~= 7 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4ae53
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd90);
                            fVar18 = quest:GetHealth(nil --[[missing]])
                            -- TODO(native): pppuVar2 = (undefined ***)CONCAT13(1,(int3)pppuVar34);
                            if fVar18 <= _DAT_0122dedc then
                                -- TODO(native): pppuVar2 = (undefined ***)((uint)pppuVar34 & 0xffffff);
                            end
                            -- TODO(native): pppuVar34 = pppuVar2;
                            if (pppuVar34 >> 0x18) ~= 0 then
                                bVar3 = false
                                pCVar29 = 0x1
                                pCVar28 = 0x0
                                pCVar27 = 0x0
                                pcVar24 = "TEXT_QST_028_APPRENTICE_MELEE_START_APLUS"
                                pCVar9 = quest:GetHero()
                                r14 = me:Speak(pCVar9, pcVar24, pCVar27, (pCVar28 ~= 0), (pCVar29 ~= 0), bVar3)
                                bVar3 = me:IsPerformingScriptTask()
                                if bVar3 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d4c5ff
                                        bVar3 = me:IsPerformingScriptTask()
                                    until not (bVar3)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d4ae53
                            end
                        end
                        quest:FadeScreenOut(nil --[[missing]], nil --[[missing]])
                        quest:Pause(nil --[[missing]])
                        iVar13 = *piVar31
                        r15 = quest:GetThingWithScriptName("HeroMeleeStart")
                        ppVar11 = quest:GetHero()
                        quest:EntityTeleportToThing(ppVar11, r15)
                        uVar20 = 0
                        uVar7 = quest:GetThingWithScriptName("WhisperMeleeStart")
                        quest:EntityTeleportToThing(uVar7, nil --[[missing]])
                        uVar7 = quest:GetHero()
                        quest:EntityUnsheatheMeleeWeapon(uVar7)
                        quest:EntityUnsheatheWeapons(nil --[[missing]], false)
                    end
                    ::LAB_00d4b11f::
                    quest:PauseAllNonScriptedEntities((uVar20 ~= 0))
                    if iVar10 ~= 1 then return end  -- TODO(native): goto LAB_00d4c40d
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4c6d1 end
                    quest:FadeScreenIn()
                    quest:SetPlayerCreatureOnlyTarget(nil --[[missing]])
                    quest:SetMasterGameState("HeroTakingGuildTest", true)
                    quest:SetStateBool("StartedMeleeTesting", true)
                    piVar31 = 0x0
                    __native_entity_state:SetStateBool("WaitingForFight", false)
                    quest:ChangeHeroHealthBy(piVar31, nil --[[missing]], nil --[[missing]])
                    quest:ModifyThingHealth(nil --[[missing]], nil --[[missing]])
                    quest:EntitySetAsKillable(nil --[[missing]], nil --[[missing]])
                    quest:EntitySetCombatType(nil --[[missing]], "HERO_WHISPER_TUTORIAL_ATTACK_STYLE")
                    quest:EntitySetInFaction(nil --[[missing]], "FACTION_BANDITS")
                    if piVar31 ~= nil then
                        -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                        quest:IsPlayerHoldingFireRangedWeaponButton(0)
                    end
                    uVar20 = quest:GetHero()
                    quest:GiveThingBestEnemyTarget(uVar20, nil --[[missing]])
                    quest:SetStateBool("FightFinished", false)
                    -- TODO(native): CTimer::CTimer((CTimer *)&stack0xfffffd5c);
                    quest:SetTimer(uVar7, puVar23)
                    quest:DisplayQuestInfo(true)
                    r16 = quest:AddQuestInfoBarHealth(nil --[[missing]], 0x3f800000, ppVar11, nil --[[missing]])
                    uVar20 = quest:GetHero()
                    fVar18 = quest:GetHealth(uVar20)
                    pCVar33 = fVar18
                    fVar18 = quest:GetHealth(nil --[[missing]])
                    cVar4 = quest:GetStateBool("FightFinished")
                    -- TODO(native): unaff_EBP = unaff_EBP & 0xffffff;
                    -- TODO(native): cStack_235 = '\0';
                    while not cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d4c6c8 end
                        if quest:GetMasterGameState("GuildWarningOccuring") == '\x01' then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d4c6c8 end
                            -- TODO(native): cStack_235 = '\x01';
                            -- TODO(native): unaff_EBP = CONCAT13(1,(int3)unaff_EBP);
                            quest:SetStateBool("FightFinished", true)
                        end
                        if piStack_24c == nil then
                            cVar4 = 0
                        else
                            cVar4 = (**(*piStack_24c + 100))()
                        end
                        if cVar4 == 0 then
                            if piStack_24c == nil then
                                cVar4 = 0
                            else
                                cVar4 = (**(*piStack_24c + 0xa4))()
                            end
                            if cVar4 ~= 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d4c6c8 end
                                quest:EntitySetInFaction(nil --[[missing]], "FACTION_HERO")
                                if ppuStack_254 ~= nil then
                                    -- TODO(native): (**(code **)(*ppuStack_254 + 0x10c))();
                                end
                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_d0);
                                pCVar30 = ""
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(false)
                                -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd8c);
                                fVar17 = quest:GetHealth(nil --[[missing]])
                                fVar18 = _DAT_0122dedc
                                if fVar18 < fVar17 then
                                    bVar3 = false
                                    pCVar28 = 0x1
                                    pCVar27 = 0x0
                                    pCVar30 = 0x0
                                    pcVar24 = "TEXT_QST_028_APPRENTICE_MELEE_NO_LIGHTNING"
                                    pCVar9 = quest:GetHero()
                                    r17 = me:Speak(pCVar9, pcVar24, pCVar30, (pCVar27 ~= 0), (pCVar28 ~= 0), bVar3)
                                    bVar3 = me:IsPerformingScriptTask()
                                    if bVar3 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                goto LAB_00d4c6c8
                                            end
                                            bVar3 = me:IsPerformingScriptTask()
                                        until not (bVar3)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                        goto LAB_00d4c6c8
                                    end
                                end
                                quest:SetStateBool("FightFinished", true)
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                -- TODO(native): this = aCStack_c8;
                                -- TODO(native): goto LAB_00d4b6f6
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d4c6c8 end
                            quest:EntitySetInFaction(nil --[[missing]], "FACTION_HERO")
                            if ppuStack_254 ~= nil then
                                -- TODO(native): (**(code **)(*ppuStack_254 + 0x10c))();
                            end
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_c0);
                            pCVar30 = ""
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd8c);
                            fVar17 = quest:GetHealth(nil --[[missing]])
                            fVar18 = _DAT_0122dedc
                            if fVar18 < fVar17 then
                                bVar3 = false
                                pCVar28 = 0x1
                                pCVar27 = 0x0
                                pCVar30 = 0x0
                                pcVar24 = "TEXT_QST_028_APPRENTICE_MELEE_NO_BOW"
                                pCVar9 = quest:GetHero()
                                r18 = me:Speak(pCVar9, pcVar24, pCVar30, (pCVar27 ~= 0), (pCVar28 ~= 0), bVar3)
                                bVar3 = me:IsPerformingScriptTask()
                                if bVar3 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                            goto LAB_00d4c6c8
                                        end
                                        bVar3 = me:IsPerformingScriptTask()
                                    until not (bVar3)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                    goto LAB_00d4c6c8
                                end
                            end
                            quest:SetStateBool("FightFinished", true)
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            -- TODO(native): this = aCStack_b8;
                            -- LAB_00d4b6f6: (native jump target)
                        end
                        -- TODO(native): iVar13 = DAT_0143e90c;
                        r19 = quest:GetHero()
                        fVar18 = quest:GetHealth(r19)
                        -- TODO(native): iVar10 = DAT_0143e90c;
                        if *(iVar13 + 0xed8) <= fVar18 then
                            fVar18 = quest:GetHealth(nil --[[missing]])
                            if fVar18 < *(iVar10 + 0xed8) then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d4c6c8 end
                                quest:SetStateBool("FightFinished", true)
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d4c6c8 end
                            quest:SetStateBool("FightFinished", true)
                        end
                        if piStack_24c == nil then
                            cVar4 = 0
                        else
                            cVar4 = (**(*piStack_24c + 0x54))()
                        end
                        if cVar4 == 0 then
                            cVar4 = quest:IsPlayerCreatureBlocking()
                            if not cVar4 then
                                -- LAB_00d4b8c5: (native jump target)
                                bVar3 = false
                            else
                                -- TODO(native): uStack_234 = uStack_234 | 2;
                                piVar31 = quest:GetHero()
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                cVar4 = piVar31:MsgIsHitBy("MeleeOpponent")
                                if not cVar4 then return end  -- TODO(native): goto LAB_00d4b8c5
                                bVar3 = true
                            end
                            if (uStack_234 & 2) ~= 0 then
                                -- TODO(native): uStack_234 = uStack_234 & 0xfffffffd;
                            end
                            if bVar3 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d4c6c8 end
                                iVar10 = quest:GetTimer(uVar7)
                                if iVar10 < 1 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        ppVar11 = quest:AddNewConversation(piVar31, bVar3, nil --[[missing]])
                                        r20 = quest:GetHero()
                                        quest:AddPersonToConversation(nil --[[missing]], r20)
                                        r21 = quest:GetHero()
                                        quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", r21, nil --[[missing]])
                                        -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_1c0;
                                        -- TODO(native): goto LAB_00d4b857
                                    end
                                    goto LAB_00d4c6c8
                                end
                            else
                                piVar31 = quest:GetHero()
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                cVar4 = piVar31:MsgIsHitBy("MeleeOpponent")
                                if cVar4 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00d4c6c8 end
                                    iVar10 = quest:GetTimer(uVar7)
                                    if iVar10 < 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00d4c6c8 end
                                        ppVar11 = quest:AddNewConversation(piVar31, nil --[[missing]], nil --[[missing]])
                                        r22 = quest:GetHero()
                                        quest:AddPersonToConversation(nil --[[missing]], r22)
                                        r23 = quest:GetHero()
                                        quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", r23, nil --[[missing]])
                                        quest:SetTimer(uVar7, nil --[[missing]])
                                    end
                                end
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d4c6c8 end
                            iVar10 = quest:GetTimer(uVar7)
                            if iVar10 < 9 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d4c6c8 end
                                ppVar11 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                r24 = quest:GetHero()
                                quest:AddPersonToConversation(nil --[[missing]], r24)
                                r25 = quest:GetHero()
                                quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", r25, nil --[[missing]])
                                -- LAB_00d4b857: (native jump target)
                                quest:SetTimer(uVar7, nil --[[missing]])
                            end
                        end
                        cVar4 = quest:GetStateBool("FightFinished")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4c6c8 end
                    quest:ResetPlayerCreatureOnlyTarget()
                    quest:RemoveQuestInfoElement(nil --[[missing]])
                    quest:EntitySetInFaction(nil --[[missing]], "FACTION_HERO")
                    if piStack_258 ~= nil then
                        -- TODO(native): (**(code **)(*piStack_258 + 0x10c))();
                    end
                    quest:DisplayQuestInfo(nil --[[missing]])
                    if (unaff_EBP >> 0x18) == '\x01' then
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            if cStack_235 ~= '\x01' then
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    quest:FadeScreenOut(nil --[[missing]], nil --[[missing]])
                                    goto LAB_00d4bbeb
                                end
                                goto LAB_00d4c6c8
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d4c6c8 end
                            quest:Pause(nil --[[missing]])
                            pCVar30 = 0xff000000
                            quest:FadeScreenOut(pCVar30, nil --[[missing]])
                            quest:SheatheHeroWeapons()
                            quest:EntitySheatheWeapons(nil --[[missing]])
                            quest:Pause(nil --[[missing]])
                            ::LAB_00d4bbeb::
                            quest:ChangeHeroHealthBy(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                            quest:ModifyThingHealth(nil --[[missing]], nil --[[missing]])
                            iVar10 = *piVar31
                            uVar20 = 0
                            uVar7 = quest:GetThingWithScriptName("M_MeleeHeroStand")
                            ppVar11 = quest:GetHero()
                            quest:EntityTeleportToThing(ppVar11, uVar7)
                            uVar7 = quest:GetThingWithScriptName("M_MeleeOpponentStand")
                            quest:EntityTeleportToThing(uVar7, nil --[[missing]])
                            quest:EntitySetInFaction(nil --[[missing]], ppVar11)
                            quest:FadeScreenIn()
                            __native_entity_state:SetStateBool("WaitingForFight", true)
                            -- LAB_00d4c3f3: (native jump target)
                            quest:SetStateBool("StartedMeleeTesting", false)
                            quest:SetMasterGameState("HeroTakingGuildTest", false)
                            -- TODO(native): goto LAB_00d4c40d
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d4c6c8 end
                        r26 = quest:GetHero()
                        fVar18 = quest:GetHealth(r26)
                        fVar18 = quest:GetHealth(nil --[[missing]])
                        -- TODO(native): pfVar15 = *(float **)(DAT_0143e90c + 0xeb4);
                        -- TODO(native): fStack_21c = (float)(((float10)fStack_10c - fVar18) - ((float10)fStack_21c - (float10)fStack_220));
                        iVar10 = 0
                        repeat
                            iVar13 = iVar10
                            if *pfVar15 < fStack_21c ~= (*pfVar15 == fStack_21c) then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d4c6c8 end
                                break
                            end
                            pfVar15 = pfVar15 + 1
                            iVar10 = iVar13 + 1
                        until not (iVar13 + 1 < 7)
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_208);
                        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_208);
                        if bVar3 then
                        end
                        cVar4 = me:AcquireControl(4)
                        while not cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d4c6bf end
                            cVar4 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aaStack_1f8);
                            -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aaStack_1f8);
                            if bVar3 then
                            end
                            r27 = quest:GetHero()
                            cVar4 = me:AcquireControl(4)
                            while not cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d4c6b3 end
                                r28 = quest:GetHero()
                                cVar4 = me:AcquireControl(4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                -- TODO(native): StdMap_Construct_API();
                                -- TODO(native): pCVar12 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_22c,aCStack_148);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar12,pCVar25);
                                -- TODO(native): pCVar12 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_22c,aCStack_178);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar12, (CScriptGameResourceObjectScriptedThingBase *)paVar14);
                                -- TODO(native): pCVar12 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_22c,aCStack_120);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar12, (CScriptGameResourceObjectScriptedThingBase *)pCVar26);
                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_108);
                                pCVar30 = ""
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities((uVar20 ~= 0))
                                quest:FixMovieSequenceCamera(nil --[[missing]])
                                ppVar11 = 0x0
                                -- TODO(native): RunCutsceneMacro_Func();
                                -- TODO(native): pCVar12 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&piStack_24c,aCStack_188);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar12,pCVar25);
                                native_arg_switch_2 = iVar13
                                repeat
                                    if native_arg_switch_2 == 0 then
                                        if quest:GetMasterGameState("GlobalMeleeGrade") ~= 7 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            if alive then
                                                ppVar21 = 0x0
                                                -- TODO(native): RunCutsceneMacro_Func();
                                                quest:ClearThingHasInformation(r28)
                                                goto FLOW_native_label_1
                                            end
                                            -- TODO(native): goto LAB_00d4c692
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if alive then
                                            -- TODO(native): RunCutsceneMacro_Func();
                                            -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_180;
                                            break
                                        end
                                        quest:PauseAllNonScriptedEntities((ppVar21 ~= 0))
                                        -- TODO(native): goto LAB_00d4c69e
                                    else
                                        if native_arg_switch_2 == 1 then
                                            -- TODO(native): RunCutsceneMacro_Func();
                                            -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_158;
                                            break
                                        else
                                            if native_arg_switch_2 == 2 then
                                                -- TODO(native): RunCutsceneMacro_Func();
                                                -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_1c0;
                                                break
                                            else
                                                if native_arg_switch_2 == 3 then
                                                    -- TODO(native): RunCutsceneMacro_Func();
                                                    -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_1bc;
                                                    break
                                                else
                                                    if native_arg_switch_2 == 4 then
                                                        -- TODO(native): RunCutsceneMacro_Func();
                                                        -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_204;
                                                        break
                                                    else
                                                        if native_arg_switch_2 == 5 then
                                                            -- TODO(native): RunCutsceneMacro_Func();
                                                            paVar14 = "CS_GUILD_DEPARTURE_MELEE_TEST_E"
                                                            break
                                                        else
                                                            if native_arg_switch_2 == 6 then
                                                                -- TODO(native): RunCutsceneMacro_Func();
                                                                paVar14 = "CS_GUILD_DEPARTURE_MELEE_TEST_F"
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
                                if quest:GetMasterGameState("GlobalMeleeGrade") < 7 - iVar13 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        -- LAB_00d4c692: (native jump target)
                                        quest:PauseAllNonScriptedEntities((ppVar11 ~= 0))
                                        -- LAB_00d4c69e: (native jump target)
                                        -- TODO(native): StdMap_Destroy_API();
                                        goto LAB_00d4c6b3
                                    end
                                    quest:SetMasterGameState("GlobalMeleeGrade", 7 - iVar13)
                                end
                                -- TODO(native): pCVar12 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&piStack_24c,(CCharString *)aaStack_1f0);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar12,pCVar25);
                                -- TODO(native): pCVar12 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&piStack_24c,aCStack_1e8);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar12, (CScriptGameResourceObjectScriptedThingBase *)pCVar26);
                                -- TODO(native): pCVar12 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&piStack_24c,aCStack_1e0);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar12,pCVar25);
                                ppVar11 = 0x0
                                -- TODO(native): RunCutsceneMacro_Func();
                                quest:FixMovieSequenceCamera((ppVar11 ~= 0))
                                uVar20 = 0
                                __native_entity_state:SetStateBool("WaitingForFight", true)
                                quest:ChangeHeroHealthBy(0x447a0000, true, false)
                                quest:ModifyThingHealth(r27, 0x447a0000, false)
                                quest:EntitySetInFaction(nil --[[missing]], ppVar11)
                                quest:PauseAllNonScriptedEntities(false)
                                -- TODO(native): StdMap_Destroy_API();
                                -- TODO(native): goto LAB_00d4c3f3
                            end
                            ::LAB_00d4c6b3::
                        end
                        ::LAB_00d4c6bf::
                    end
                    ::LAB_00d4c6c8::
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_d8);
                        pCVar30 = ""
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities((uVar20 ~= 0))
                        -- TODO(native): pVar5 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd94);
                        fVar17 = quest:GetHealth(nil --[[missing]])
                        fVar18 = _DAT_0122dedc
                        if fVar18 < fVar17 then
                            bVar3 = false
                            pCVar29 = 0x1
                            pCVar28 = 0x0
                            pCVar27 = 0x0
                            pcVar24 = "TEXT_QST_028_APPRENTICE_MELEE_NO_WHISPER"
                            pCVar9 = quest:GetHero()
                            r29 = me:Speak(pCVar9, pcVar24, pCVar27, (pCVar28 ~= 0), (pCVar29 ~= 0), bVar3)
                            bVar3 = me:IsPerformingScriptTask()
                            if bVar3 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                        goto LAB_00d4c6d1
                                    end
                                    bVar3 = me:IsPerformingScriptTask()
                                until not (bVar3)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                goto LAB_00d4c6d1
                            end
                        end
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        -- LAB_00d4c40d: (native jump target)
                        goto LAB_00d4c416
                    end
                end
                ::LAB_00d4c6d1::
            end
            ::LAB_00d4c6da::
            -- TODO(native): goto LAB_00d4c6ec
        end
        ::LAB_00d4c416::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        uVar7 = uVar20
        cVar4 = extraout_AL_68
    until false
end

function Init(quest, me)
    __native_entity_state:SetStateBool("WaitingForFight", true)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

