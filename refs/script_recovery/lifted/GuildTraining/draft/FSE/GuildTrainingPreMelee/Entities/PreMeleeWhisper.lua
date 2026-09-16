-- Generated native draft: PreMeleeWhisper. Review coverage report before use.
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
    local CVar23, bVar3, cVar4, fVar15, fVar2, fVar26, iVar9, native_arg_sequence_1, pCVar1, pCVar16, pCVar17, pCVar18, pCVar19, pCVar24, pCVar25, pCVar28, pCVar29, pCVar5, pCVar6, paVar12, pcVar22, piVar8, ppVar20, puVar14, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r2, r3, r4, r5, r6, r7, r8, r9, uVar10, uVar11, uVar21, uVar27
    local alive = true
    -- TODO(native): fStack_b4 = 1.4013e-45;
    quest:EntitySetAsKillable(me, false)
    me:SetFriendsWithEverythingFlag(nil --[[missing]])
    cVar4 = quest:GetStateBool("WhisperCutsceneFinished")
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        cVar4 = quest:GetStateBool("WhisperCutsceneFinished")
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff50);
        if bVar3 then
        end
        cVar4 = me:AcquireControl(4)
        while not cVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d52582
            cVar4 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            -- LAB_00d52582: (native jump target)
            return
        end
        pCVar5 = quest:RegisterTimer()
        pCVar28 = 0x0
        quest:SetTimer(pCVar5, pCVar28)
        pCVar24 = 0x1
        pCVar6 = quest:GetHero()
        cVar4 = quest:GetStateBool("WhisperStopFollowing")
        while not cVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d52d83
            cVar4 = me:IsTalkedToByHero()
            if cVar4 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d52e1b end
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_40);
                pCVar24 = ""
                quest:StartMovieSequence()
                pCVar19 = 0x1
                quest:PauseAllNonScriptedEntities((pCVar19 ~= 0))
                -- TODO(native): pCVar7 = (CCharString *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff30);
                fVar15 = quest:GetHealth(pCVar6)
                fVar2 = _DAT_0122dedc
                CVar23 = SUB41(pCVar25,0)
                uVar21 = SUB41(pCVar24,0)
                if fVar2 < fVar15 then
                    bVar3 = false
                    pCVar18 = 0x1
                    pCVar17 = 0x0
                    pCVar16 = 0x0
                    pcVar22 = "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CHAT"
                    pCVar6 = quest:GetHero()
                    r1 = me:Speak(pCVar6, pcVar22, pCVar16, (pCVar17 ~= 0), (pCVar18 ~= 0), bVar3)
                    bVar3 = me:IsPerformingScriptTask()
                    CVar23 = SUB41(pCVar25,0)
                    uVar21 = SUB41(pCVar24,0)
                    if bVar3 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                -- TODO(native): goto LAB_00d52dc0
                            end
                            bVar3 = me:IsPerformingScriptTask()
                            CVar23 = SUB41(pCVar25,0)
                            uVar21 = SUB41(pCVar24,0)
                        until not (bVar3)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        quest:PauseAllNonScriptedEntities(false)
                        -- LAB_00d52dc0: (native jump target)
                        return
                    end
                end
                pCVar24 = 0x1
                pCVar6 = quest:GetHero()
                me:FollowThing(pCVar6, pCVar7, (pCVar19 ~= 0))
                quest:PauseAllNonScriptedEntities(false)
            end
            r2 = quest:GetNearestWithScriptName(me, "PreMeleeChatMarker")
            piVar8 = quest:GetHero()
            if piStack_60 == nil then
            else
                puVar14 = (**(*piStack_60 + 0x18))()
            end
            iVar9 = piVar8:GetPos()
            -- TODO(native): fStack_b4 = (float)puVar14[2] - *(float *)(iVar9 + 8);
            bVar3 = quest:IsDistanceBetweenThingsUnder(me, r2, 7.0)
            if bVar3 then
                fVar26 = 7.0
                pCVar6 = quest:GetHero()
                bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar6, fVar26)
                native_arg_sequence_1 = false
                if not bVar3 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if not native_arg_sequence_1 then
                    iVar9 = quest:GetTimer(pCVar5)
                    if 5 < iVar9 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if not native_arg_sequence_1 then
                    if ABS(fStack_b4) < _DAT_0122ded8 == (ABS(fStack_b4) == _DAT_0122ded8) then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then goto LAB_00d52d56 end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00d52df2: (native jump target)
                    return
                end
                pCVar24 = r2:GetDataString()
                -- TODO(native): fStack_b4 = (float)GFCharStringToInt(pCVar24);
                r3 = quest:GetAllThingsWithScriptName("PreMeleeChatMarker")
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- TODO(native): goto LAB_00d52df2
                end
                uVar27 = 0
                uVar10 = quest:AddNewConversation(me, false, (uVar27 ~= 0))
                uVar11 = quest:GetHero()
                quest:AddPersonToConversation(uVar10, uVar11)
                quest:SetTimer(pCVar5, uVar27)
                -- TODO(native): switch(fStack_b4) {
                -- TODO(native): case 2.8026e-45:
                r4 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_LIBRARY", me, r4, false)
                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_98;
                break
                -- TODO(native): default:
                -- TODO(native): goto switchD_00d529d7_caseD_3;
                -- TODO(native): case 5.60519e-45:
                r5 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SHOP", me, r5, false)
                break
                -- TODO(native): case 7.00649e-45:
                r6 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CLOISTERS", me, r6, false)
                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_90;
                break
                -- TODO(native): case 8.40779e-45:
                r7 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE1", me, r7, false)
                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_7c;
                break
                -- TODO(native): case 9.80909e-45:
                r8 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE2", me, r8, false)
                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_88;
                break
                -- TODO(native): case 1.12104e-44:
                r9 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE3", me, r9, false)
                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_78;
                break
                -- TODO(native): case 1.26117e-44:
                r10 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE4", me, r10, false)
                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_80;
                break
                -- TODO(native): case 1.4013e-44:
                r11 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAZE", me, r11, false)
                break
                -- TODO(native): case 1.54143e-44:
                r12 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WILL", me, r12, false)
                break
                -- TODO(native): case 1.82169e-44:
                r13 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WOODS", me, r13, false)
                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_74;
                break
                -- TODO(native): case 1.96182e-44:
                r14 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SKILL", me, r14, false)
                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_a0;
                break
                -- TODO(native): case 2.10195e-44:
                r15 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SERVANTS", me, r15, false)
                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_9c;
                break
                -- TODO(native): case 2.24208e-44:
                r16 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAIN_DORM", me, r16, false)
                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_94;
                break
                -- TODO(native): case 2.66247e-44:
                r17 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DOOR", me, r17, false)
                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_8c;
                break
                -- TODO(native): case 2.8026e-44:
                r18 = quest:GetHero()
                quest:AddLineToConversation(uVar10, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DINING_ROOM", me, r18, false)
                -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_84;
            end
            -- TODO(native): switchD_00d529d7_caseD_3:
        end
        ::LAB_00d52d56::
        cVar4 = quest:GetStateBool("WhisperStopFollowing")
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        -- LAB_00d52d83: (native jump target)
        quest:DeregisterTimer(pCVar5)
        return
    end
    ::LAB_00d52e1b::
    quest:DeregisterTimer(pCVar5)
    end
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

