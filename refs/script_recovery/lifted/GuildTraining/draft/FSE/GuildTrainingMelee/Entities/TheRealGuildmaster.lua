-- Generated native draft: TheRealGuildmaster. Review coverage report before use.
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
    local CVar1, __native_condition_1, aVar26, bVar2, cVar3, fVar16, fVar17, fVar28, iVar11, iVar6, native_arg_sequence_1, native_arg_switch_2, pCVar10, pCVar19, pCVar22, pCVar24, pCVar25, pCVar27, pCVar29, pCVar30, pCVar31, pCVar5, pVar4, paVar12, pcVar20, pfVar13, piVar8, ppVar21, ppVar23, ppuVar15, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r3, r30, r31, r32, r33, r34, r35, r36, r37, r38, r39, r4, r40, r5, r6, r7, r8, r9, uVar18, uVar9
    local alive = true
    -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_250);
    if bVar2 then
    end
    cVar3 = me:AcquireControl(4)
    while not cVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        cVar3 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00d5a8e2 end
    quest:EntitySetAsKillable(nil --[[missing]], false)
    r1 = quest:GetHero()
    quest:EntitySetAlwaysBlockAttacksFromThing(r1, nil --[[missing]], false)
    piVar8 = *(this + 0x10)
    if piVar8 ~= nil then
        -- TODO(native): *piVar8 = *piVar8 + 1;
    end
    quest:SetIsPushableByHero(nil --[[missing]], false)
    r2 = quest:GetThingWithScriptName("MeleeOpponent")
    quest:SetThingHasInformation(r2)
    r3 = quest:RegisterTimer()
    quest:SetTimer(0x0, 0x0)
    iVar6 = quest:GetStateInt("TutorialState")
    while iVar6 == 1 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00d5a8cb
        cVar3 = me:IsTalkedToByHero()
        if not cVar3 then
            native_arg_sequence_1 = false
            if not quest:GetStateBool("EarlyHitWhisper") then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                if not quest:GetStateBool("WhisperArrived") then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then
                -- TODO(native): ppStack_248 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *) ((uint)ppStack_248 | 1);
                cVar3 = me:MsgIsHitByHero()
                if cVar3 then return end  -- TODO(native): goto LAB_00d586db
                bVar2 = false
            else
                -- LAB_00d586db: (native jump target)
                bVar2 = true
            end
            if (ppStack_248 & 1) ~= 0 then
                -- TODO(native): ppStack_248 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *) ((uint)ppStack_248 & 0xfffffffe);
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a8cb
                pCVar22 = ""
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): pVar4 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry>) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd98);
                fVar16 = quest:GetHealth(nil --[[missing]])
                fVar17 = _DAT_0122dedc
                if fVar17 < fVar16 then
                    aVar26 = 0x0
                    pCVar25 = 0x1
                    pCVar24 = 0x0
                    pCVar22 = 0x0
                    pcVar20 = "TEXT_QST_028_GUILDMASTER_MELEE_HIT_WHISPER"
                    pCVar5 = quest:GetHero()
                    r4 = me:Speak(pCVar5, pcVar20, pCVar22, (pCVar24 ~= 0), (pCVar25 ~= 0), (aVar26 ~= 0))
                    bVar2 = me:IsPerformingScriptTask()
                    if bVar2 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities((aVar26 ~= 0))
                                goto LAB_00d5933c
                            end
                            bVar2 = me:IsPerformingScriptTask()
                        until not (bVar2)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): goto LAB_00d5a8cb
                    end
                end
                quest:SetStateInt("TutorialState", 2)
                quest:PauseAllNonScriptedEntities(false)
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5a8cb
            quest:SetStateInt("TutorialState", 2)
        end
        fVar28 = 5.5
        pCVar5 = quest:GetHero()
        bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar5, me, fVar28)
        __native_condition_1 = (bVar2) and (iVar6 = GSI->GetTimer(), iVar6 < 1)
        if __native_condition_1 then
            bVar2 = me:IsPerformingScriptTask()
            __native_condition_1 = not bVar2
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5a8cb
            r5 = quest:AddNewConversation(nil --[[missing]], false, false)
            r6 = quest:GetHero()
            quest:AddPersonToConversation(nil --[[missing]], r6)
            quest:SetTimer(nil --[[missing]], nil --[[missing]])
            if iStack_1f8 == 0 then
                r7 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(r7, nil --[[missing]])
                r8 = quest:GetHero()
                quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_FIRST", r8, nil --[[missing]])
            else
                if 1 == 1 then
                    r9 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(r9, nil --[[missing]])
                    r10 = quest:GetHero()
                    quest:AddLineToConversation(1, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_SECOND", r10, nil --[[missing]])
                end
            end
        end
        iVar6 = quest:GetStateInt("TutorialState")
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        quest:SetMasterGameState("HeroTakingGuildTest", true)
        quest:SetStateBool("WhisperStopWalking", true)
        quest:SetPlayerCreatureOnlyTarget(nil --[[missing]])
        cVar3 = quest:GetStateBool("MeleeRepeating")
        while cVar3 == '\x01' do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d5933c end
            quest:SetStateInt("TutorialState", 2)
            quest:SetStateBool("MeleeRepeatKnown", false)
            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_210);
            if bVar2 then
            end
            r11 = quest:GetHero()
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a902
                r12 = quest:GetHero()
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5a902: (native jump target)
                goto LAB_00d5a8d9
            end
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_a0);
            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_a0);
            if bVar2 then
            end
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a8f6
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5a8f6: (native jump target)
                -- TODO(native): goto LAB_00d5a902
            end
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_60,(CCharString *)&iStack_1f8);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7,pCVar27);
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_60,(CCharString *)aaStack_1f0);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7,pCVar27);
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_60,(CCharString *)aaStack_200);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7, (CScriptGameResourceObjectScriptedThingBase *)pCVar29);
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_44);
            pCVar22 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(false)
            quest:FixMovieSequenceCamera(false)
            ppVar23 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            quest:FixMovieSequenceCamera((ppVar23 ~= 0))
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            -- TODO(native): StdMap_Destroy_API();
            quest:SetStateInt("GenericTutorialCounter", 0)
            quest:DisplayQuestInfo(nil --[[missing]])
            pCVar22 = "HUD_WHISPER_ICON"
            uVar18 = quest:AddQuestInfoCounter("HUD_WHISPER_ICON", nil --[[missing]], nil --[[missing]])
            pCVar30 = uVar18
            quest:SetStateInt("TutorialState", 3)
            -- TODO(native): CTimer::CTimer((CTimer *)&stack0xfffffd5c);
            quest:SetTimer(nil --[[missing]], nil --[[missing]])
            iVar6 = quest:GetStateInt("GenericTutorialCounter")
            while iVar6 < 7 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a9b5
                iVar6 = quest:GetTimer(nil --[[missing]])
                if iVar6 < 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5a9b5
                    ppVar23 = quest:AddNewConversation(r12, nil --[[missing]], nil --[[missing]])
                    r13 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r13)
                    r14 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_HELP_ATTACK", r14, r11)
                    quest:SetTimer(nil --[[missing]], nil --[[missing]])
                end
                quest:UpdateQuestInfoCounter(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                -- TODO(native): iVar11 = DAT_0143e90c;
                r15 = quest:GetHero()
                fVar17 = quest:GetHealth(r15)
                if fVar17 < *(iVar11 + 0xed8) then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5a9b5
                    ppVar23 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                    r16 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r16)
                    r17 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_HEAL_HERO", r17, nil --[[missing]])
                    quest:ChangeHeroHealthBy(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                end
                iVar6 = quest:GetStateInt("GenericTutorialCounter")
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5a9b5
            quest:ClearAllRumbles()
            quest:DisplayQuestInfo(nil --[[missing]])
            quest:RemoveQuestInfoElement(nil --[[missing]])
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)afStack_e4);
            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)afStack_e4);
            if bVar2 then
            end
            r18 = quest:GetHero()
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a922
                r19 = quest:GetHero()
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5a922: (native jump target)
                -- LAB_00d5a9b5: (native jump target)
                -- TODO(native): goto LAB_00d5a9be
            end
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_90);
            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_90);
            if bVar2 then
            end
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a916
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5a916: (native jump target)
                -- TODO(native): goto LAB_00d5a922
            end
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_78,aCStack_18c);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7,pCVar27);
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_78,aCStack_140);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7,pCVar27);
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_78,aCStack_138);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7, (CScriptGameResourceObjectScriptedThingBase *)pCVar29);
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_34);
            pCVar22 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            quest:FixMovieSequenceCamera(nil --[[missing]])
            ppVar23 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            quest:FixMovieSequenceCamera((ppVar23 ~= 0))
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            -- TODO(native): StdMap_Destroy_API();
            quest:SetStateInt("GenericTutorialCounter", 0)
            cVar3 = quest:IsXbox()
            if not cVar3 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    pCVar22 = "TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK_PC"
                    quest:DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK_PC")
                    cVar3 = quest:MsgIsGameInfoClickedPast()
                    while not cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d5a9b5
                        cVar3 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_100;
                        goto LAB_00d5941c
                    end
                end
                -- TODO(native): goto LAB_00d5a9b5
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5a9b5
            pCVar22 = "TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK"
            quest:DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK")
            cVar3 = quest:MsgIsGameInfoClickedPast()
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a9b5
                cVar3 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5a9b5
            -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_118;
            ::LAB_00d5941c::
            quest:SetStateInt("TutorialState", 4)
            pCVar22 = "HUD_WHISPER_ICON"
            uVar18 = quest:AddQuestInfoCounter("HUD_WHISPER_ICON", nil --[[missing]], nil --[[missing]])
            quest:DisplayQuestInfo(nil --[[missing]])
            iVar6 = quest:GetStateInt("GenericTutorialCounter")
            while iVar6 < 5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a9b5
                quest:UpdateQuestInfoCounter(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                -- TODO(native): iVar11 = DAT_0143e90c;
                r20 = quest:GetHero()
                fVar17 = quest:GetHealth(r20)
                if fVar17 < *(iVar11 + 0xed8) then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5a9b5
                    ppVar23 = quest:AddNewConversation(r19, nil --[[missing]], nil --[[missing]])
                    r21 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r21)
                    r22 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_HEAL_HERO", r22, r18)
                    quest:ChangeHeroHealthBy(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                end
                iVar6 = quest:GetStateInt("GenericTutorialCounter")
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5a9b5
            quest:ClearAllRumbles()
            quest:SetStateInt("TutorialState", 5)
            piVar8 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_THUNDER")
            piVar8:GetPos()
            r23 = quest:CreateCreature("SkillApprenticeMarker", nil --[[missing]], "MeleeThunder")
            quest:EntitySetAppearanceMorphSeed(r23, nil --[[missing]])
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&uStack_244);
            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&uStack_244);
            if bVar2 then
            end
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a9a7
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5a9a7: (native jump target)
                -- TODO(native): goto LAB_00d5a9b5
            end
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_ec);
            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_ec);
            if bVar2 then
            end
            r24 = quest:GetHero()
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a93f
                r25 = quest:GetHero()
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5a93f: (native jump target)
                -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aCStack_ec;
                -- LAB_00d5a99e: (native jump target)
                -- TODO(native): goto LAB_00d5a9a7
            end
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_b0);
            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_b0);
            if bVar2 then
            end
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a933
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5a933: (native jump target)
                -- TODO(native): goto LAB_00d5a93f
            end
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_bc,(CCharString *)aaStack_1a0);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7, (CScriptGameResourceObjectScriptedThingBase *)pCVar29);
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_bc,aCStack_168);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7,pCVar27);
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_bc,aCStack_198);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7,pCVar27);
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_bc,aCStack_160);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7, (CScriptGameResourceObjectScriptedThingBase *)pCVar29);
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_54);
            pCVar22 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            quest:FixMovieSequenceCamera(nil --[[missing]])
            ppVar23 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            quest:FixMovieSequenceCamera((ppVar23 ~= 0))
            quest:ChangeHeroHealthBy(nil --[[missing]], nil --[[missing]], nil --[[missing]])
            quest:ModifyThingHealth(r25, nil --[[missing]])
            quest:SetStateInt("TutorialState", 6)
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            -- TODO(native): StdMap_Destroy_API();
            __native_entity_state:SetStateBool("HeroStanding", true)
            __native_entity_state:SetStateBool("WhisperStanding", true)
            quest:DisplayQuestInfo(nil --[[missing]])
            quest:RemoveQuestInfoElement(nil --[[missing]])
            quest:DisplayQuestInfo(nil --[[missing]])
            ppVar23 = quest:GetThingWithScriptName("MeleeOpponent")
            r26 = quest:AddQuestInfoBarHealth(ppVar23, nil --[[missing]], "HUD_WHISPER_ICON", nil --[[missing]])
            uVar9 = quest:GetHero()
            fVar17 = quest:GetHealth(uVar9)
            -- TODO(native): afStack_e4[0] = (float)fVar17;
            fVar17 = quest:GetHealth(r24)
            CVar1 = __native_entity_state:GetStateBool("HeroStanding")
            while (CVar1 and (__native_entity_state:GetStateBool("WhisperStanding"))) do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                -- TODO(native): iVar6 = DAT_0143e90c;
                if not alive then return end  -- TODO(native): goto LAB_00d5a9a7
                r27 = quest:GetHero()
                fVar17 = quest:GetHealth(r27)
                if *(iVar6 + 0xed8) <= fVar17 then
                    -- TODO(native): iVar11 = DAT_0143e90c;
                    r28 = quest:GetThingWithScriptName("MeleeOpponent")
                    fVar17 = quest:GetHealth(r28)
                    fVar28 = *(iVar11 + 0xed8)
                    if fVar17 < fVar28 then
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            __native_entity_state:SetStateBool("WhisperStanding", false)
                            r29 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                            r30 = quest:GetHero()
                            quest:AddPersonToConversation(nil --[[missing]], r30)
                            r31 = quest:GetHero()
                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_FIGHT_OVER", r31, nil --[[missing]])
                            -- TODO(native): goto LAB_00d59cc6
                        end
                        -- TODO(native): goto LAB_00d5a9a7
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5a9a7
                    __native_entity_state:SetStateBool("HeroStanding", false)
                    r32 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                    r33 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r33)
                    r34 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_FIGHT_OVER", r34, nil --[[missing]])
                    -- LAB_00d59cc6: (native jump target)
                end
                CVar1 = __native_entity_state:GetStateBool("HeroStanding")
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5a9a7
            quest:ClearAllRumbles()
            r35 = quest:GetHero()
            fVar17 = quest:GetHealth(r35)
            fVar17 = quest:GetHealth(nil --[[missing]])
            -- TODO(native): pfVar13 = *(float **)(DAT_0143e90c + 0xeb4);
            -- TODO(native): fStack_1e0 = (float)(((float10)fStack_80 - fVar17) - ((float10)fStack_7c - (float10)fStack_1e0));
            iVar6 = 0
            repeat
                iVar11 = iVar6
                if *pfVar13 < fStack_1e0 ~= (*pfVar13 == fStack_1e0) then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5a9a7
                    break
                end
                iVar6 = iVar11 + 1
                pfVar13 = pfVar13 + 1
            until not (iVar6 < 7)
            quest:ResetPlayerCreatureOnlyTarget()
            quest:SetStateInt("TutorialState", 7)
            quest:DisplayQuestInfo(nil --[[missing]])
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&fStack_1e0);
            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&fStack_1e0);
            if bVar2 then
            end
            r36 = quest:GetHero()
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a997
                r37 = quest:GetHero()
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5a997: (native jump target)
                -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aaStack_1dc;
                -- TODO(native): goto LAB_00d5a99e
            end
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_1cc);
            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_1cc);
            if bVar2 then
            end
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a98b
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5a98b: (native jump target)
                -- TODO(native): goto LAB_00d5a997
            end
            quest:RemoveQuestInfoElement(nil --[[missing]])
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_230,(CCharString *)aaStack_178);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7,pCVar27);
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_230,aCStack_130);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7,pCVar27);
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_230,aCStack_170);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7,pCVar27);
            -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_230,aCStack_10c);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar7, (CScriptGameResourceObjectScriptedThingBase *)pCVar22);
            -- TODO(native): Std_Deque_Construct();
            native_arg_switch_2 = iVar11
            repeat
                if native_arg_switch_2 == 0 then
                    pcVar20 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_APLUS"
                    -- TODO(native): pCVar22 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)aaStack_1f0,aCStack_168);
                    -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_168;
                    break
                else
                    if native_arg_switch_2 == 1 then
                        pcVar20 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_A"
                        -- TODO(native): pCVar22 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)aaStack_1f0,aCStack_128);
                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_128;
                        break
                    else
                        if native_arg_switch_2 == 2 then
                            pcVar20 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_B"
                            -- TODO(native): pCVar22 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)aaStack_1f0,aCStack_160);
                            -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_160;
                            break
                        else
                            if native_arg_switch_2 == 3 then
                                pcVar20 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_C"
                                -- TODO(native): pCVar22 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)aaStack_1f0,(CCharString *)aaStack_1b4);
                                paVar12 = "$GRADE"
                                break
                            else
                                if native_arg_switch_2 == 4 then
                                    pcVar20 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_D"
                                    -- TODO(native): pCVar22 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)aaStack_1f0,aCStack_158);
                                    -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_158;
                                    break
                                else
                                    if native_arg_switch_2 == 5 then
                                        pcVar20 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_E"
                                        -- TODO(native): pCVar22 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)aaStack_1f0,aCStack_120);
                                        -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_120;
                                        break
                                    else
                                        if native_arg_switch_2 == 6 then
                                            pcVar20 = "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_F"
                                            -- TODO(native): pCVar22 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)aaStack_1f0,aCStack_150);
                                            -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_150;
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
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_d0);
            pCVar22 = ""
            quest:StartMovieSequence()
            ppuVar15 = *(this + 4)
            -- TODO(native): (**(code **)((int)*ppuVar15 + 0x5ec))();
            ppVar23 = 0x1
            quest:FixMovieSequenceCamera((ppVar23 ~= 0))
            if not __native_entity_state:GetStateBool("WhisperStanding") then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    ppVar21 = 0x0
                    -- TODO(native): RunCutsceneMacro_Func();
                    -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_138;
                    goto LAB_00d5a28a
                end
                -- LAB_00d5a948: (native jump target)
                -- TODO(native): (**(code **)((int)*ppuVar15 + 0x5ec))();
                -- LAB_00d5a96a: (native jump target)
                -- TODO(native): LTextTreeWalkThrough__Dtor();
                -- TODO(native): StdMap_Destroy_API();
                -- TODO(native): goto LAB_00d5a98b
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5a956: (native jump target)
                iVar6 = *ppuVar15
                -- LAB_00d5a962: (native jump target)
                quest:PauseAllNonScriptedEntities((ppVar21 ~= 0))
                -- TODO(native): goto LAB_00d5a96a
            end
            ppVar21 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            -- TODO(native): paVar12 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_168;
            ::LAB_00d5a28a::
            quest:ChangeHeroHealthBy(ppVar21, nil --[[missing]], nil --[[missing]])
            quest:EntitySetInFaction(r37, "FACTION_HERO")
            me:SetFriendsWithEverythingFlag(nil --[[missing]])
            ppVar21 = quest:GetThingWithScriptName("MeleeThunder")
            quest:RemoveThing(ppVar21)
            quest:Pause(0x40000000)
            pCVar22 = "TEXT_QST_028_GUILDMASTER_MELEE_REPEAT_QUESTION"
            quest:GiveHeroYesNoQuestion(pCVar22, "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
            iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
            while iVar6 < 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a956
                iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5a948
            alive = not quest:IsActiveThreadTerminating()
            if iVar6 == 1 then
                if not alive then return end  -- TODO(native): goto LAB_00d5a956
                -- TODO(native): RunCutsceneMacro_Func();
                quest:SetStateBool("MeleeRepeating", false)
                quest:SetStateBool("MeleeRepeatKnown", true)
            else
                if not alive then return end  -- TODO(native): goto LAB_00d5a948
                ppVar21 = 0x0
                -- TODO(native): RunCutsceneMacro_Func();
                quest:SetStateBool("MeleeRepeating", true)
                quest:SetStateBool("MeleeRepeatKnown", true)
                r38 = quest:GetHero()
                cVar3 = quest:IsObjectInThingsPossession("OBJECT_IRON_LONGSWORD", r38)
                if cVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        iVar6 = *ppuStack_25c
                        -- TODO(native): goto LAB_00d5a962
                    end
                    quest:TakeObjectFromHero("OBJECT_IRON_LONGSWORD")
                end
            end
            quest:FixMovieSequenceCamera((ppVar21 ~= 0))
            -- TODO(native): (**(code **)((int)*ppuVar15 + 0x5ec))();
            -- TODO(native): LTextTreeWalkThrough__Dtor();
            -- TODO(native): StdMap_Destroy_API();
            cVar3 = quest:GetStateBool("MeleeOpponentReset")
            while cVar3 ~= '\x01' do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5a9a7
                cVar3 = quest:GetStateBool("MeleeOpponentReset")
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5a9a7
            cVar3 = quest:GetStateBool("MeleeRepeating")
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            ppVar23 = quest:GetThingWithScriptName("MeleeOpponent")
            quest:RemoveThing(ppVar23)
            pVar4 = SUB41(&"M_GuildmasterMarker",0)
            piVar8 = quest:GetThingWithScriptName("M_GuildmasterMarker")
            pCVar24 = 0x1
            pCVar22 = 0x0
            pCVar31 = 0x0
            pcVar20 = 0x40400000
            pCVar10 = piVar8:GetPos()
            me:MoveToPosition(pCVar10, SUB41(pCVar19,0))
            piVar8 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE")
            piVar8:GetPos()
            r39 = quest:CreateCreature("MeleeApprenticeMarker", nil --[[missing]], "MeleeApprentice")
            piVar8 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
            piVar8:GetPos()
            pCVar19 = ""
            r40 = quest:CreateCreature("CombatApprenticeMarker", nil --[[missing]], "CombatApprentice")
            ppVar23 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(ppVar23, pcVar20)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_04", "", "")
        end
        -- LAB_00d5a8cb: (native jump target)
        quest:DeregisterTimer(pCVar31)
    else
        -- LAB_00d5a9be: (native jump target)
    end
    ::LAB_00d5a8d9::
    ::LAB_00d5a8e2::
    do return end
    ::LAB_00d5933c::
    quest:DeregisterTimer(pCVar22)
    goto LAB_00d5a8d9
end

function Init(quest, me)
    __native_entity_state:SetStateBool("WhisperEarly", false)
    __native_entity_state:SetStateBool("WhisperLate", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

