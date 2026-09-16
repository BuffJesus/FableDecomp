-- Generated native draft: ApprenticeSpeedTest. Review coverage report before use.
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
    local CVar27, __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, bVar2, cVar3, fVar16, fVar17, fVar28, iVar7, iVar8, native_arg_switch_5, native_arg_switch_6, native_arg_switch_7, native_arg_switch_8, pCStack_280, pCVar1, pCVar13, pCVar20, pCVar21, pCVar24, pCVar25, pCVar26, pCVar29, pCVar6, pVar23, paVar12, pcVar22, piVar19, ppVar9, ppuVar15, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r3, r4, r5, r6, r7, r8, r9, uVar10, uVar11, uVar18, uVar4, uVar5
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_23c);
        if bVar2 then
        end
        cVar3 = me:AcquireControl(4)
        while not cVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d4075b end
            cVar3 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            -- TODO(native): pCStack_280 = *(CScriptThing **)(this + 0xc);
            piVar19 = *(this + 0x10)
            if piVar19 ~= nil then
                -- TODO(native): *piVar19 = *piVar19 + 1;
            end
            quest:SetIsPushableByHero(nil --[[missing]], false)
            r1 = quest:GetThingWithScriptName("SpeedFriend")
            r2 = quest:GetNearestWithDefName(r1, "VILLAGE_GUILD_COMPLEX_INSIDE")
            quest:EntityAttachToVillage(r2, nil --[[missing]])
            quest:EntityAttachToVillage(nil --[[missing]], nil --[[missing]])
            piVar19 = 0x0
            quest:EntitySetAsKillable(me, false, true)
            quest:EntitySetAsKillable(nil --[[missing]], false, true)
            quest:SetThingHasInformation(me, false, true, false)
            me:SetFriendsWithEverythingFlag(1)
            if piVar19 ~= nil then
                -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                quest:IsPlayerHoldingFireRangedWeaponButton(1)
            end
            __native_entity_state:SetStateInt("RaceMode", 0)
            r3 = quest:RegisterTimer()
            uVar5 = quest:RegisterTimer()
            r4 = quest:RegisterTimer()
            quest:SetTimer(uVar5, 1)
            -- TODO(native): uStack_22c = (undefined **)((uint)uStack_22c & 0xffffff);
            alive = not quest:IsActiveThreadTerminating()
            cVar3 = not alive
            while not cVar3 do
                if __native_entity_state:GetStateInt("RaceMode") == 0 then
                    -- TODO(native): iStack_21c = uStack_228 - 1;
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d4072e
                        if (quest:GetStateInt("GameState") == 3) and (uStack_22c._3_1_ == 0) then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4072e
                            quest:SetThingHasInformation(nil --[[missing]])
                            -- TODO(native): uStack_22c = (undefined **)CONCAT13(1,(undefined3)uStack_22c);
                        end
                        fVar28 = 10.0
                        pCVar6 = quest:GetHero()
                        bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar28)
                        __native_condition_1 = bVar2
                        if __native_condition_1 then
                            iVar7 = quest:GetTimer(uVar5)
                            __native_condition_1 = iVar7 < 1
                        end
                        if __native_condition_1 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4072e
                            r5 = quest:AddNewConversation(me, 0xffffffff, (piVar19 ~= 0))
                            quest:AddPersonToConversation(0, nil --[[missing]])
                            if iVar8 < 5 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d4072e
                                native_arg_switch_5 = iVar8
                                repeat
                                    if native_arg_switch_5 == 0 then
                                        quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", nil --[[missing]], nil --[[missing]])
                                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_134;
                                        break
                                    else
                                        if native_arg_switch_5 == 1 then
                                            quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", nil --[[missing]], nil --[[missing]])
                                            -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_174;
                                            break
                                        else
                                            if native_arg_switch_5 == 2 then
                                                quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", nil --[[missing]], nil --[[missing]])
                                                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_180;
                                                break
                                            else
                                                if native_arg_switch_5 == 3 then
                                                    quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", nil --[[missing]], nil --[[missing]])
                                                    -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_178;
                                                    break
                                                else
                                                    if native_arg_switch_5 == 4 then
                                                        quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", nil --[[missing]], nil --[[missing]])
                                                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_118;
                                                        break
                                                    else
                                                        goto FLOW_native_label_1
                                                    end
                                                end
                                            end
                                        end
                                    end
                                until not (false)
                            else
                                uVar11 = uStack_228 & 0x80000001
                                bVar2 = uVar11 == 0
                                if uVar11 < 0 then
                                    bVar2 = (uVar11 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not bVar2 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", nil --[[missing]], nil --[[missing]])
                                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_13c;
                                        goto LAB_00d3e7f7
                                    end
                                    -- TODO(native): goto LAB_00d4072e
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d4072e
                                quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", nil --[[missing]], nil --[[missing]])
                                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_110;
                            end
                            ::LAB_00d3e7f7::
                            ::FLOW_native_label_1::
                            quest:SetTimer(uVar5, nil --[[missing]])
                            -- TODO(native): uStack_228 = uStack_228 + 1;
                            -- TODO(native): iStack_21c = iVar8 + 1;
                        end
                        cVar3 = me:IsTalkedToByHero()
                        if cVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4072e
                            pCVar24 = ""
                            quest:StartMovieSequence()
                            ppuVar15 = *(this + 4)
                            -- TODO(native): (**(code **)(*ppuVar15 + 0x5ec))();
                            -- TODO(native): uVar4 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(aCStack_254);
                            fVar16 = quest:GetHealth(nil --[[missing]])
                            if fVar16 <= _DAT_0122dedc then
                            end
                            if (unaff_EDI >> 0x18) ~= 0 then
                                CVar27 = 0x0
                                pCVar26 = 0x1
                                pCVar25 = 0x0
                                pCVar24 = 0x0
                                pcVar22 = "TEXT_QST_028_FAST_APPRENTICE_BOAST"
                                pCVar6 = quest:GetHero()
                                r6 = me:Speak(pCVar6, pcVar22, pCVar24, (pCVar25 ~= 0), (pCVar26 ~= 0), (CVar27 ~= 0))
                                bVar2 = me:IsPerformingScriptTask()
                                if bVar2 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            -- TODO(native): (**(code **)(*ppuVar15 + 0x5ec))();
                                            quest:DeregisterTimer(uVar5)
                                            quest:DeregisterTimer(uVar5)
                                            quest:DeregisterTimer(uVar5)
                                            goto LAB_00d40749
                                        end
                                        bVar2 = me:IsPerformingScriptTask()
                                    until not (bVar2)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    -- TODO(native): (**(code **)(*ppuVar15 + 0x5ec))();
                                    -- TODO(native): goto LAB_00d40729
                                end
                            end
                            pCVar24 = "TEXT_OBJECT_HERO_ANSWER_NO"
                            pCVar25 = "TEXT_OBJECT_HERO_ANSWER_YES"
                            pCVar26 = "TEXT_QST_028_FAST_APPRENTICE_BOAST_QUESTION"
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_BOAST_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                            iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar8 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    -- TODO(native): (**(code **)(*ppuVar15 + 0x5ec))();
                                    quest:DeregisterTimer(uVar5)
                                    quest:DeregisterTimer(uVar5)
                                    quest:DeregisterTimer(uVar5)
                                    if (0x1 ~= nil) and (*0x1 = *0x1 + -1, *0x1 == 0) then
                                        -- TODO(native): (**(code **)(pCStack_280 + 4))();
                                    end
                                    if (pppuStack_270 ~= nil) and (*pppuStack_270 = (*pppuStack_270 + -1), *pppuStack_270 == nil) then
                                    end
                                    return
                                end
                                iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                -- TODO(native): (**(code **)(*ppuVar15 + 0x5ec))();
                                -- TODO(native): goto LAB_00d40729
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if iVar8 == 1 then
                                if not alive then
                                    -- LAB_00d3ee93: (native jump target)
                                    -- TODO(native): (**(code **)(*ppuVar15 + 0x5ec))();
                                    -- TODO(native): goto LAB_00d40729
                                end
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd94);
                                fVar16 = quest:GetHealth(nil --[[missing]])
                                cVar3 = _DAT_0122dedc < fVar16
                                if cVar3 ~= 0 then
                                    bVar2 = false
                                    pCVar21 = 0x1
                                    pCVar20 = 0x0
                                    pCVar24 = 0x0
                                    pcVar22 = "TEXT_QST_028_FAST_APPRENTICE_RUN"
                                    pCVar6 = quest:GetHero()
                                    r7 = me:Speak(pCVar6, pcVar22, pCVar24, (pCVar20 ~= 0), (pCVar21 ~= 0), bVar2)
                                    bVar2 = me:IsPerformingScriptTask()
                                    if bVar2 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then goto LAB_00d405d6 end
                                            bVar2 = me:IsPerformingScriptTask()
                                        until not (bVar2)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d3ee93
                                end
                                __native_entity_state:SetStateInt("RaceMode", 1)
                                quest:SetStateBool("ReachedPlatform", false)
                                ppVar9 = quest:GetThingWithScriptName("RaceMarker")
                                quest:MiniMapAddMarker(ppVar9, "HUD_ORB_QUEST_VIGNETTE")
                                uVar5 = __ftol2()
                                quest:SetTimer(uVar5, 0x0)
                                uVar5 = __ftol2()
                                quest:SetTimer(uVar5, CVar27)
                                uVar18 = quest:AddQuestInfoTimer(uVar5, ppVar9, 0x3f800000)
                                quest:DisplayQuestInfo(true)
                                if cStack_24d == 0 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        -- TODO(native): (**(code **)(*ppuStack_248 + 0x5ec))();
                                        -- TODO(native): goto LAB_00d40729
                                    end
                                    quest:SetThingHasInformation(me)
                                    -- TODO(native): cStack_24d = '\x01';
                                end
                            else
                                if not alive then return end  -- TODO(native): goto LAB_00d3ee93
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd94);
                                uVar4 = SUB41(pCVar26,0)
                                fVar16 = quest:GetHealth(nil --[[missing]])
                                cVar3 = _DAT_0122dedc < fVar16
                                if cVar3 ~= 0 then
                                    bVar2 = false
                                    pCVar26 = 0x1
                                    pCVar25 = 0x0
                                    pCVar24 = 0x0
                                    pcVar22 = "TEXT_QST_028_FAST_APPRENTICE_REFUSE"
                                    pCVar6 = quest:GetHero()
                                    r8 = me:Speak(pCVar6, pcVar22, pCVar24, (pCVar25 ~= 0), (pCVar26 ~= 0), bVar2)
                                    bVar2 = me:IsPerformingScriptTask()
                                    if bVar2 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then goto LAB_00d405d6 end
                                            bVar2 = me:IsPerformingScriptTask()
                                        until not (bVar2)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d3ee93
                                end
                            end
                            -- TODO(native): (**(code **)(*ppuVar15 + 0x5ec))();
                        end
                    until not (__native_entity_state:GetStateInt("RaceMode") == 0)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00d4072e: (native jump target)
                    goto LAB_00d40749
                end
                iVar8 = __native_entity_state:GetStateInt("RaceMode")
                while uStack_228 = uVar11, iVar8 == 1 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d4072e
                    fVar28 = 10.0
                    pCVar6 = quest:GetHero()
                    bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar28)
                    __native_condition_2 = bVar2
                    if __native_condition_2 then
                        iVar8 = quest:GetTimer(uVar5)
                        __native_condition_2 = iVar8 < 1
                    end
                    if __native_condition_2 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d4072e
                        r9 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(0, nil --[[missing]])
                        if uVar11 < 6 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4072e
                            native_arg_switch_6 = uVar11
                            repeat
                                if native_arg_switch_6 == 1 then
                                    quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", nil --[[missing]], nil --[[missing]])
                                    -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_144;
                                    break
                                else
                                    if native_arg_switch_6 == 2 then
                                        quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", nil --[[missing]], nil --[[missing]])
                                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_1c4;
                                        break
                                    else
                                        if native_arg_switch_6 == 3 then
                                            quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", nil --[[missing]], nil --[[missing]])
                                            -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_11c;
                                            break
                                        else
                                            if native_arg_switch_6 == 4 then
                                                quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", nil --[[missing]], nil --[[missing]])
                                                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_1bc;
                                                break
                                            else
                                                if native_arg_switch_6 == 5 then
                                                    quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", nil --[[missing]], nil --[[missing]])
                                                    -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_16c;
                                                    break
                                                else
                                                    goto FLOW_native_label_2
                                                end
                                            end
                                        end
                                    end
                                end
                            until not (false)
                        else
                            uVar10 = uVar11 & 0x80000001
                            bVar2 = uVar10 == 0
                            if uVar10 < 0 then
                                bVar2 = (uVar10 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if not bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", nil --[[missing]], nil --[[missing]])
                                    goto LAB_00d3f069
                                end
                                -- TODO(native): goto LAB_00d4072e
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4072e
                            quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", nil --[[missing]], nil --[[missing]])
                            -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_1b4 ;
                        end
                        ::LAB_00d3f069::
                        ::FLOW_native_label_2::
                        quest:SetTimer(uVar5, nil --[[missing]])
                        -- TODO(native): uStack_228 = uVar11 + 1;
                    end
                    cVar3 = me:IsTalkedToByHero()
                    if cVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d4072e
                        quest:RemoveQuestInfoElement(nil --[[missing]])
                        quest:DisplayQuestInfo(nil --[[missing]])
                        if not quest:GetStateBool("ReachedPlatform") then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4072e
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_d0);
                            paVar12 = ""
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            -- TODO(native): uVar4 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(aCStack_254);
                            fVar17 = quest:GetHealth(nil --[[missing]])
                            fVar16 = _DAT_0122dedc
                            if fVar16 < fVar17 then
                                CVar27 = 0x0
                                pCVar26 = 0x1
                                pCVar25 = 0x0
                                pCVar24 = 0x0
                                pcVar22 = "TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM"
                                pCVar6 = quest:GetHero()
                                r10 = me:Speak(pCVar6, pcVar22, pCVar24, (pCVar25 ~= 0), (pCVar26 ~= 0), (CVar27 ~= 0))
                                bVar2 = me:IsPerformingScriptTask()
                                if bVar2 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            quest:PauseAllNonScriptedEntities((CVar27 ~= 0))
                                            -- TODO(native): goto LAB_00d40729
                                        end
                                        bVar2 = me:IsPerformingScriptTask()
                                    until not (bVar2)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                    -- TODO(native): goto LAB_00d40729
                                end
                            end
                            __native_entity_state:SetStateInt("RaceMode", 2)
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        else
                            iVar8 = quest:GetTimer(uVar5)
                            if iVar8 < 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d4072e
                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_10c);
                                pCVar24 = ""
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                -- TODO(native): ppVar9 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(aCStack_254);
                                fVar16 = quest:GetHealth(nil --[[missing]])
                                if fVar16 <= _DAT_0122dedc then
                                end
                                if (unaff_EDI >> 0x18) ~= 0 then
                                    CVar27 = 0x0
                                    pCVar26 = 0x1
                                    pCVar25 = 0x0
                                    pCVar24 = 0x0
                                    pcVar22 = "TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW"
                                    pCVar6 = quest:GetHero()
                                    r11 = me:Speak(pCVar6, pcVar22, pCVar24, (pCVar25 ~= 0), (pCVar26 ~= 0), (CVar27 ~= 0))
                                    bVar2 = me:IsPerformingScriptTask()
                                    if bVar2 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d40684
                                            bVar2 = me:IsPerformingScriptTask()
                                        until not (bVar2)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities((CVar27 ~= 0))
                                        -- TODO(native): goto LAB_00d40729
                                    end
                                end
                                __native_entity_state:SetStateInt("RaceMode", 2)
                                if not quest:GetStateBool("ReachedPlatform") then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        -- LAB_00d40684: (native jump target)
                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                        -- TODO(native): goto LAB_00d40729
                                    end
                                    r12 = quest:GetThingWithScriptName("RaceMarker")
                                    quest:MiniMapRemoveMarker(r12)
                                end
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d4072e
                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_e0);
                                pCVar24 = ""
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                -- TODO(native): uVar4 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(aCStack_254);
                                fVar17 = quest:GetHealth(nil --[[missing]])
                                fVar16 = _DAT_0122dedc
                                if fVar16 < fVar17 then
                                    CVar27 = 0x0
                                    pCVar26 = 0x1
                                    pCVar25 = 0x0
                                    pCVar24 = 0x0
                                    pcVar22 = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED"
                                    pCVar6 = quest:GetHero()
                                    r13 = me:Speak(pCVar6, pcVar22, pCVar24, (pCVar25 ~= 0), (pCVar26 ~= 0), (CVar27 ~= 0))
                                    bVar2 = me:IsPerformingScriptTask()
                                    if bVar2 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                quest:PauseAllNonScriptedEntities((CVar27 ~= 0))
                                                -- TODO(native): goto LAB_00d40729
                                            end
                                            bVar2 = me:IsPerformingScriptTask()
                                        until not (bVar2)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                        -- TODO(native): goto LAB_00d40729
                                    end
                                end
                                __native_entity_state:SetStateInt("RaceMode", 3)
                                -- TODO(native): __ftol2();
                                quest:GiveHeroGold(nil --[[missing]])
                                quest:ClearThingHasInformation(nil --[[missing]])
                                quest:PauseAllNonScriptedEntities(false)
                            end
                        end
                    end
                    iVar8 = quest:GetTimer(uVar5)
                    if iVar8 < 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d4072e
                        quest:RemoveQuestInfoElement(nil --[[missing]])
                        quest:DisplayQuestInfo(nil --[[missing]])
                        if not quest:GetStateBool("ReachedPlatform") then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4072e
                            pCStack_280 = quest:GetThingWithScriptName("RaceMarker")
                            quest:MiniMapRemoveMarker(pCStack_280)
                        end
                        __native_entity_state:SetStateInt("RaceMode", 2)
                    end
                    iVar8 = __native_entity_state:GetStateInt("RaceMode")
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d4072e
                if __native_entity_state:GetStateInt("RaceMode") == 2 then
                    -- TODO(native): iStack_21c = uStack_228 - 1;
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d4072e
                        fVar28 = 10.0
                        pCVar6 = quest:GetHero()
                        bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar28)
                        __native_condition_3 = bVar2
                        if __native_condition_3 then
                            iVar8 = quest:GetTimer(uVar5)
                            __native_condition_3 = iVar8 < 1
                        end
                        if __native_condition_3 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4072e
                            r14 = quest:AddNewConversation(me, nil --[[missing]], nil --[[missing]])
                            quest:AddPersonToConversation(nil --[[missing]], nil --[[missing]])
                            if iStack_21c < 5 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d4072e
                                native_arg_switch_7 = iVar8
                                repeat
                                    if native_arg_switch_7 == 0 then
                                        quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", nil --[[missing]], nil --[[missing]])
                                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_194;
                                        break
                                    else
                                        if native_arg_switch_7 == 1 then
                                            quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", nil --[[missing]], nil --[[missing]])
                                            -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_148;
                                            break
                                        else
                                            if native_arg_switch_7 == 2 then
                                                quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", nil --[[missing]], nil --[[missing]])
                                                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_18c;
                                                break
                                            else
                                                if native_arg_switch_7 == 3 then
                                                    quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", nil --[[missing]], nil --[[missing]])
                                                    -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_140;
                                                    break
                                                else
                                                    if native_arg_switch_7 == 4 then
                                                        quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", nil --[[missing]], nil --[[missing]])
                                                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_184;
                                                        break
                                                    else
                                                        goto FLOW_native_label_3
                                                    end
                                                end
                                            end
                                        end
                                    end
                                until not (false)
                            else
                                uVar11 = uStack_228 & 0x80000001
                                bVar2 = uVar11 == 0
                                if uVar11 < 0 then
                                    bVar2 = (uVar11 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not bVar2 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", nil --[[missing]], nil --[[missing]])
                                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_17c;
                                        goto LAB_00d3f894
                                    end
                                    -- TODO(native): goto LAB_00d4072e
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d4072e
                                quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", nil --[[missing]], nil --[[missing]])
                            end
                            ::LAB_00d3f894::
                            ::FLOW_native_label_3::
                            quest:SetTimer(uVar5, nil --[[missing]])
                            -- TODO(native): uStack_228 = uStack_228 + 1;
                            -- TODO(native): iStack_21c = iVar8 + 1;
                        end
                        cVar3 = me:IsTalkedToByHero()
                        if cVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4072e
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aaStack_208);
                            pCVar24 = ""
                            quest:StartMovieSequence()
                            ppuVar15 = *(this + 4)
                            -- TODO(native): (**(code **)(*ppuVar15 + 0x5ec))();
                            -- TODO(native): uVar4 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(aCStack_254);
                            fVar17 = quest:GetHealth(nil --[[missing]])
                            fVar16 = _DAT_0122dedc
                            if fVar16 < fVar17 then
                                CVar27 = 0x0
                                pCVar26 = 0x1
                                pCVar25 = 0x0
                                pCVar24 = 0x0
                                pcVar22 = "TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL"
                                pCVar6 = quest:GetHero()
                                r15 = me:Speak(pCVar6, pcVar22, pCVar24, (pCVar25 ~= 0), (pCVar26 ~= 0), (CVar27 ~= 0))
                                bVar2 = me:IsPerformingScriptTask()
                                if bVar2 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            -- TODO(native): (**(code **)(*ppuVar15 + 0x5ec))();
                                            -- TODO(native): goto LAB_00d40729
                                        end
                                        bVar2 = me:IsPerformingScriptTask()
                                    until not (bVar2)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    -- TODO(native): (**(code **)(*ppuVar15 + 0x5ec))();
                                    -- TODO(native): goto LAB_00d40729
                                end
                            end
                            uVar5 = 1
                            paVar12 = "TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL_QUESTION"
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                            iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar8 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d406ee
                                iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                -- LAB_00d40702: (native jump target)
                                -- TODO(native): (**(code **)(*ppuStack_248 + 0x5ec))();
                                -- TODO(native): goto LAB_00d40729
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if iVar8 == 1 then
                                if not alive then
                                    -- LAB_00d406ee: (native jump target)
                                    -- TODO(native): (**(code **)(*ppuStack_248 + 0x5ec))();
                                    -- TODO(native): goto LAB_00d40729
                                end
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd94);
                                fVar17 = quest:GetHealth(nil --[[missing]])
                                fVar16 = _DAT_0122dedc
                                if fVar16 < fVar17 then
                                    bVar2 = false
                                    pCVar26 = 0x1
                                    pCVar25 = 0x0
                                    pCVar24 = 0x0
                                    pcVar22 = "TEXT_QST_028_FAST_APPRENTICE_RUN"
                                    pCVar6 = quest:GetHero()
                                    r16 = me:Speak(pCVar6, pcVar22, pCVar24, (pCVar25 ~= 0), (pCVar26 ~= 0), bVar2)
                                    bVar2 = me:IsPerformingScriptTask()
                                    if bVar2 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d40702
                                            bVar2 = me:IsPerformingScriptTask()
                                        until not (bVar2)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d406ee
                                end
                                __native_entity_state:SetStateInt("RaceMode", 1)
                                quest:SetStateBool("ReachedPlatform", false)
                                -- TODO(native): __ftol2();
                                quest:SetTimer(uVar5, CVar27)
                                -- TODO(native): __ftol2();
                                quest:SetTimer(uVar5, 0x1)
                                r17 = quest:AddQuestInfoTimer(uVar5, ppVar9, 0x3f800000)
                                uVar5 = quest:GetThingWithScriptName("RaceMarker")
                                quest:MiniMapAddMarker(uVar5, "HUD_ORB_QUEST_VIGNETTE")
                            else
                                if not alive then return end  -- TODO(native): goto LAB_00d40702
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd94);
                                uVar4 = SUB41(paVar12,0)
                                fVar17 = quest:GetHealth(nil --[[missing]])
                                fVar16 = _DAT_0122dedc
                                if fVar16 < fVar17 then
                                    bVar2 = false
                                    pCVar26 = 0x1
                                    pCVar25 = 0x0
                                    pCVar24 = 0x0
                                    pcVar22 = "TEXT_QST_028_FAST_APPRENTICE_REFUSE"
                                    pCVar6 = quest:GetHero()
                                    r18 = me:Speak(pCVar6, pcVar22, pCVar24, (pCVar25 ~= 0), (pCVar26 ~= 0), bVar2)
                                    bVar2 = me:IsPerformingScriptTask()
                                    if bVar2 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d406ee
                                            bVar2 = me:IsPerformingScriptTask()
                                        until not (bVar2)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d40702
                                end
                            end
                            -- TODO(native): (**(code **)(*ppuStack_248 + 0x5ec))();
                        end
                    until not (__native_entity_state:GetStateInt("RaceMode") == 2)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d4072e
                if __native_entity_state:GetStateInt("RaceMode") == 3 then
                    -- TODO(native): iStack_21c = uStack_228 - 1;
                    -- LAB_00d3fdc0: (native jump target)
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        fVar28 = 10.0
                        pCVar6 = quest:GetHero()
                        bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar28)
                        __native_condition_4 = bVar2
                        if __native_condition_4 then
                            iVar7 = quest:GetTimer(uVar5)
                            __native_condition_4 = iVar7 < 1
                        end
                        if __native_condition_4 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d4072e
                            r19 = quest:AddNewConversation(me, nil --[[missing]], nil --[[missing]])
                            quest:AddPersonToConversation(nil --[[missing]], nil --[[missing]])
                            if iVar8 < 5 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d4072e
                                native_arg_switch_8 = iVar8
                                repeat
                                    if native_arg_switch_8 == 0 then
                                        quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", nil --[[missing]], nil --[[missing]])
                                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_1f0;
                                        break
                                    else
                                        if native_arg_switch_8 == 1 then
                                            quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", nil --[[missing]], nil --[[missing]])
                                            -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_1e8;
                                            break
                                        else
                                            if native_arg_switch_8 == 2 then
                                                quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", nil --[[missing]], nil --[[missing]])
                                                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_1e0;
                                                break
                                            else
                                                if native_arg_switch_8 == 3 then
                                                    quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", nil --[[missing]], nil --[[missing]])
                                                    -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_1d8;
                                                    break
                                                else
                                                    if native_arg_switch_8 == 4 then
                                                        quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", nil --[[missing]], nil --[[missing]])
                                                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_1d0;
                                                        break
                                                    else
                                                        goto FLOW_native_label_4
                                                    end
                                                end
                                            end
                                        end
                                    end
                                until not (false)
                            else
                                uVar11 = uStack_228 & 0x80000001
                                bVar2 = uVar11 == 0
                                if uVar11 < 0 then
                                    bVar2 = (uVar11 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not bVar2 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", nil --[[missing]], nil --[[missing]])
                                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_1c0;
                                        goto LAB_00d40030
                                    end
                                    -- TODO(native): goto LAB_00d4072e
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d4072e
                                quest:AddLineToConversation(0x0, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", nil --[[missing]], nil --[[missing]])
                                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_1c8;
                            end
                            ::LAB_00d40030::
                            ::FLOW_native_label_4::
                            quest:SetTimer(uVar5, nil --[[missing]])
                            -- TODO(native): uStack_228 = uStack_228 + 1;
                            -- TODO(native): iStack_21c = iVar8 + 1;
                        end
                        cVar3 = me:IsTalkedToByHero()
                        if not cVar3 then goto LAB_00d403e1 end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&ppuStack_fc);
                            pCVar24 = ""
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            pCVar24 = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION"
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                            iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                            pVar23 = SUB41(pCVar24,0)
                            while iVar8 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d402b3
                                iVar8 = quest:MsgIsQuestionAnsweredYesOrNo()
                                pVar23 = SUB41(pCVar24,0)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                alive = not quest:IsActiveThreadTerminating()
                                if iVar8 ~= 1 then
                                    if alive then
                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd98);
                                        fVar16 = quest:GetHealth(nil --[[missing]])
                                        cVar3 = _DAT_0122dedc < fVar16
                                        if cVar3 ~= 0 then
                                            bVar2 = false
                                            pCVar26 = 0x1
                                            pCVar25 = 0x0
                                            pCVar24 = 0x0
                                            pcVar22 = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO"
                                            pCVar6 = quest:GetHero()
                                            r20 = me:Speak(pCVar6, pcVar22, pCVar24, (pCVar25 ~= 0), (pCVar26 ~= 0), bVar2)
                                            bVar2 = me:IsPerformingScriptTask()
                                            if bVar2 then
                                                repeat
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if not alive then goto LAB_00d402b3 end
                                                    bVar2 = me:IsPerformingScriptTask()
                                                until not (bVar2)
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d40716
                                        end
                                        -- TODO(native): goto LAB_00d40379
                                    end
                                    -- TODO(native): goto LAB_00d40716
                                end
                                if alive then
                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd98);
                                    fVar16 = quest:GetHealth(nil --[[missing]])
                                    cVar3 = _DAT_0122dedc < fVar16
                                    if cVar3 ~= 0 then
                                        bVar2 = false
                                        pCVar26 = 0x1
                                        pCVar25 = 0x0
                                        pCVar24 = 0x0
                                        pcVar22 = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES"
                                        pCVar6 = quest:GetHero()
                                        r21 = me:Speak(pCVar6, pcVar22, pCVar24, (pCVar25 ~= 0), (pCVar26 ~= 0), bVar2)
                                        bVar2 = me:IsPerformingScriptTask()
                                        if bVar2 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then return end  -- TODO(native): goto LAB_00d40716
                                                bVar2 = me:IsPerformingScriptTask()
                                            until not (bVar2)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00d402b3 end
                                    end
                                    -- LAB_00d40379: (native jump target)
                                    ppVar9 = quest:GetThingWithScriptName("RaceMarker")
                                    quest:MiniMapRemoveMarker(ppVar9)
                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                    goto LAB_00d403e1
                                end
                                ::LAB_00d402b3::
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            else
                                -- LAB_00d40716: (native jump target)
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            end
                            -- LAB_00d40729: (native jump target)
                        end
                    end
                    -- TODO(native): goto LAB_00d4072e
                end
                -- LAB_00d403eb: (native jump target)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d4072e
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                cVar3 = not alive
            end
            -- LAB_00d405fc: (native jump target)
            quest:DeregisterTimer(uVar5)
            quest:DeregisterTimer(uVar5)
            quest:DeregisterTimer(uVar5)
            ::LAB_00d40749::
        end
        ::LAB_00d4075b::
    end
    do return end
    ::LAB_00d405d6::
    -- TODO(native): (**(code **)(*ppuVar15 + 0x5ec))();
    -- TODO(native): goto LAB_00d405fc
    ::LAB_00d403e1::
    if __native_entity_state:GetStateInt("RaceMode") ~= 3 then return end  -- TODO(native): goto LAB_00d403eb
    -- TODO(native): goto LAB_00d3fdc0
end

function Init(quest, me)
end

function OnPersist(quest, context)
    local raceMode = quest:GetStateBool("RaceMode") or false
    raceMode = quest:PersistTransferBool(context, "RaceMode", raceMode)
    quest:SetStateBool("RaceMode", raceMode)
end

function OnPredicateFail(quest, me)
end

