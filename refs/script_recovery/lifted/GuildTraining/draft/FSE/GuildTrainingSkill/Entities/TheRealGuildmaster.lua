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
    local __native_condition_1, __native_condition_2, __native_condition_3, aVar4, bVar2, bVar5, cVar3, fVar1, fVar18, fVar37, iVar13, iVar9, native_arg_sequence_1, native_arg_sequence_2, native_arg_switch_2, pCStack_250, pCVar12, pCVar17, pCVar21, pCVar23, pCVar25, pCVar28, pCVar29, pCVar30, pCVar33, pCVar35, pCVar38, pCVar40, pCVar41, pCVar7, pVar27, paVar14, pcVar36, pfVar15, piVar6, pmVar26, ppVar10, ppVar22, ppVar39, ppVar42, ppVar8, pppuStack_238, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r3, r30, r31, r32, r33, r34, r35, r36, r37, r38, r39, r4, r40, r5, r6, r7, r8, r9, uVar19, uVar20, uVar24, uVar31, uVar32, uVar34, unaff_EBP
    local alive = true
    -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_210);
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
    if alive then
        ppVar42 = 0x1
        quest:EntitySetAsKillable(me, false)
        pCStack_250 = quest:GetHero()
        quest:EntitySetAlwaysBlockAttacksFromThing(me, pCStack_250, true)
        -- TODO(native): uStack_260 = *(undefined4 *)(this + 0xc);
        -- TODO(native): piStack_25c = *(int **)(this + 0x10);
        if piStack_25c ~= nil then
            -- TODO(native): *piStack_25c = *piStack_25c + 1;
        end
        quest:SetIsPushableByHero(me, false)
        quest:SetThingHasInformation(nil --[[missing]])
        piVar6 = quest:GetThingWithScriptName("M_SkillTeacherStand")
        pCVar41 = 0x1
        pCVar40 = 0x0
        pCVar38 = 0x0
        pcVar36 = 0x40400000
        pCVar7 = piVar6:GetPos()
        me:MoveToPosition(pCVar7, SUB41(0x0,0), SUB41(pCVar28,0))
        pCVar38 = 0x1
        quest:SetPlayerUsingRangedDummies((pCVar38 ~= 0))
        ppVar8 = quest:RegisterTimer()
        pCVar40 = 0x0
        ppVar39 = ppVar8
        quest:SetTimer(ppVar8, pCVar40)
        iVar9 = quest:GetStateInt("TutorialState")
        while iVar9 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d5da96 end
            cVar3 = me:IsTalkedToByHero()
            if cVar3 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d5da96 end
                if quest:GetMasterGameState("HeroTakingGuildTest") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d5da96 end
                    me:ClearCommands()
                    quest:SetStateInt("TutorialState", 3)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d5da96 end
                    quest:StartMovieSequence()
                    pCVar7 = 0x1
                    quest:PauseAllNonScriptedEntities((pCVar7 ~= 0))
                    me:ClearCommands()
                    -- TODO(native): aVar4 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_>) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd8c);
                    fVar18 = quest:GetHealth(nil --[[missing]])
                    fVar1 = _DAT_0122dedc
                    -- TODO(native): unaff_ESI = (int *)0x0;
                    if fVar1 < fVar18 then
                        pVar27 = 0x0
                        pCVar25 = 0x1
                        pCVar23 = 0x0
                        pCVar41 = 0x0
                        pcVar36 = "TEXT_QST_028_GUILDMASTER_SKILL_NOT_START"
                        pCVar7 = quest:GetHero()
                        r1 = me:Speak(pCVar7, pcVar36, pCVar41, (pCVar23 ~= 0), (pCVar25 ~= 0), (pVar27 ~= 0))
                        bVar2 = me:IsPerformingScriptTask()
                        if bVar2 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities((pVar27 ~= 0))
                                    quest:DeregisterTimer(ppVar8)
                                    return
                                end
                                bVar2 = me:IsPerformingScriptTask()
                            until not (bVar2)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d5da96
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                end
            end
            fVar37 = 5.5
            pCVar7 = quest:GetHero()
            bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar7, me, fVar37)
            __native_condition_1 = (bVar2) and (iVar9 = GSI->GetTimer(), iVar9 < 1)
            if __native_condition_1 then
                bVar2 = me:IsPerformingScriptTask()
                __native_condition_1 = not bVar2
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d5da96 end
                r2 = quest:AddNewConversation(nil --[[missing]], false, false)
                r3 = quest:GetHero()
                quest:AddPersonToConversation(0, r3)
                quest:SetTimer(ppVar8, 0)
                if pppuStack_244 == nil then
                    r4 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(r4, nil --[[missing]])
                    r5 = quest:GetHero()
                    quest:AddLineToConversation(ppVar42, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_FIRST", r5, nil --[[missing]])
                    -- LAB_00d5b380: (native jump target)
                else
                    if pppuStack_244 == 0x1 then
                        r6 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(r6, nil --[[missing]])
                        r7 = quest:GetHero()
                        quest:AddLineToConversation(0, "TEXT_QST_028_GUILDMASTER_SKILL_COMMENT_SECOND", r7, nil --[[missing]])
                        -- TODO(native): goto LAB_00d5b380
                    end
                end
                -- TODO(native): pppuStack_244 = (undefined ***)(1 - (int)pppuStack_244);
            end
            iVar9 = quest:GetStateInt("TutorialState")
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            -- LAB_00d5dbab: (native jump target)
            return
        end
        quest:SetMasterGameState("SkillTrainingStarted", true)
        quest:SetMasterGameState("SkillRepeating", true)
        quest:SetMasterGameState("HeroTakingGuildTest", true)
        cVar3 = quest:GetMasterGameState("SkillRepeating")
        while cVar3 ~= 0 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d5da96 end
            quest:SetStateInt("TutorialState", 3)
            quest:SetMasterGameState("SkillRepeatKnown", false)
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_1d8);
            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_1d8);
            if bVar2 then
            end
            ppVar10 = quest:GetHero()
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5dadc
                r8 = quest:GetHero()
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5dadc
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar11 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_1d4,aCStack_d0);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar11,pCVar29);
            -- TODO(native): pCVar11 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_1d4,aCStack_154);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar11,pCVar29);
            quest:SetStateInt("TutorialState", 2)
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_80);
            pCVar28 = ""
            quest:StartMovieSequence()
            fVar37 = 1.4013e-45
            quest:PauseAllNonScriptedEntities(false)
            uVar24 = 1
            quest:FixMovieSequenceCamera((uVar24 ~= 0))
            pCVar23 = 0x1
            pCVar41 = 0x0
            pCVar40 = 0x0
            ppVar10 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            pCVar21 = 0x0
            quest:FixMovieSequenceCamera((pCVar21 ~= 0))
            quest:SetMasterGameState("SkillTrainingStarted", true)
            cVar3 = quest:IsXbox()
            if not cVar3 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    pCVar25 = "TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_PC"
                    quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_PC")
                    cVar3 = quest:MsgIsGameInfoClickedPast()
                    while true do
                        aVar4 = SUB41(pCVar30,0)
                        pVar27 = SUB41(pCVar28,0)
                        if cVar3 then break end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d5dab8
                        cVar3 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)aCStack_114,pCVar21,(CCharString *)ppVar10,pCVar40, pCVar41,pCVar23,uVar24,fVar37,(bool)pVar27,(bool)aVar4);
                        -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_114;
                        goto LAB_00d5b7c0
                    end
                end
                -- LAB_00d5db05: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00d5dac4: (native jump target)
                -- TODO(native): StdMap_Destroy_API();
                -- LAB_00d5dadc: (native jump target)
                return
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5dab8: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): goto LAB_00d5dac4
            end
            pCVar25 = "TEXT_QST_028_SKILL_INSTRUCTIONS_BOW"
            quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW")
            cVar3 = quest:MsgIsGameInfoClickedPast()
            while true do
                aVar4 = SUB41(pCVar30,0)
                pVar27 = SUB41(pCVar28,0)
                if cVar3 then break end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db05
                cVar3 = quest:MsgIsGameInfoClickedPast()
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5dab8
            -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)aCStack_124,pCVar21,(CCharString *)ppVar10,pCVar40,pCVar41, pCVar23,uVar24,fVar37,(bool)pVar27,(bool)aVar4);
            -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_124;
            ::LAB_00d5b7c0::
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): StdMap_Destroy_API();
            pCVar28 = "HUD_QUEST_ICON_TARGET_DUMMY"
            uVar19 = quest:AddQuestInfoCounter(pCVar28, 3, 0x3f800000)
            -- TODO(native): CTimer::CTimer((CTimer *)&stack0xfffffd4c);
            quest:SetTimer(ppVar8, ppVar10)
            quest:SetMasterGameState("SkillScore", 0)
            r9 = quest:GetThingWithScriptName("ArcheryRing")
            quest:EntitySetTargetable(me, false)
            cVar3 = quest:IsXbox()
            if not cVar3 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    pCVar28 = "GAME_ACTION_UNSHEATHE_RANGED_WEAPON"
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick()
                    pCVar28 = "GAME_ACTION_LOCK_TARGET"
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick()
                    pCVar28 = "GAME_ACTION_FIRE_RANGED_WEAPON"
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick()
                    -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_bc;
                    goto LAB_00d5ba3e
                end
                -- LAB_00d5db99: (native jump target)
                -- TODO(native): goto LAB_00d5dbab
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5db99
            pCVar28 = "HUD_BLACK_BUTTON"
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick()
            pCVar28 = "HUD_CONTROLLER_TRIGGER_LEFT"
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick()
            pCVar28 = "HUD_CONTROLLER_X"
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick()
            ::LAB_00d5ba3e::
            quest:DisplayQuestInfo((pCVar40 ~= 0))
            quest:SetTimer(ppVar8, ppVar10)
            iVar9 = quest:GetMasterGameState("SkillScore")
            while cStack_26d = (ppVar8 >> 0x18), ppStack_270 = ppVar8, iVar9 < 3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db99
                -- TODO(native): uStack_258 = uStack_258 | 1;
                cVar3 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                native_arg_sequence_1 = false
                if not cVar3 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if not native_arg_sequence_1 then
                    -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingLockTargetButton()
                    if not cVar3 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if not native_arg_sequence_1 then
                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingFireRangedWeaponButton()
                    if not cVar3 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then
                    bVar2 = false
                else
                    bVar2 = true
                end
                if (uStack_258 & 1) ~= 0 then
                    -- TODO(native): uStack_258 = uStack_258 & 0xfffffffe;
                end
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5db99
                    quest:RemoveQuestInfoElement(bVar2)
                    quest:RemoveQuestInfoElement(pCVar41)
                    quest:RemoveQuestInfoElement(pCVar23)
                    ppVar10 = ppVar42
                else
                    bVar2 = cStack_26d == 0
                    ppVar10 = ppVar42
                    if bVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d5db99
                        r10 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                        ppVar10 = ppVar42
                        quest:UpdateQuestInfoTick(0, false)
                        -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                        quest:IsPlayerHoldingLockTargetButton()
                        quest:UpdateQuestInfoTick(nil --[[missing]], nil --[[missing]])
                        -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                        quest:IsPlayerHoldingFireRangedWeaponButton()
                        quest:UpdateQuestInfoTick(nil --[[missing]], nil --[[missing]])
                        iVar9 = quest:GetTimer(ppVar8)
                        if iVar9 < 1 then
                            fVar37 = 6.0
                            pCVar7 = quest:GetHero()
                            bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar7, r9, &pCStack_254)
                            __native_condition_2 = bVar2
                            if __native_condition_2 then
                                cVar3 = quest:IsConversationActive(nil --[[missing]])
                                __native_condition_2 = not cVar3
                            end
                            if __native_condition_2 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d5db99
                                cVar3 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                                if not cVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d5db99
                                    ppVar42 = quest:AddNewConversation(r8, nil --[[missing]], nil --[[missing]])
                                    r11 = quest:GetHero()
                                    quest:AddPersonToConversation(nil --[[missing]], r11)
                                    cVar3 = quest:IsXbox()
                                    if not cVar3 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d5db99
                                        r12 = quest:GetHero()
                                        quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_10", r12, nil --[[missing]])
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d5db99
                                        r13 = quest:GetHero()
                                        quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_10", r13, nil --[[missing]])
                                        -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_124;
                                    end
                                else
                                    -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                                    quest:IsPlayerHoldingLockTargetButton()
                                    if not cVar3 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d5db99
                                        ppVar42 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                        r14 = quest:GetHero()
                                        quest:AddPersonToConversation(nil --[[missing]], r14)
                                        cVar3 = quest:IsXbox()
                                        if not cVar3 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d5db99
                                            r15 = quest:GetHero()
                                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_20", r15, nil --[[missing]])
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d5db99
                                            r16 = quest:GetHero()
                                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_20", r16, nil --[[missing]])
                                        end
                                    else
                                        -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                                        quest:IsPlayerHoldingFireRangedWeaponButton()
                                        if cVar3 then goto LAB_00d5bf6c end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d5db99
                                        ppVar42 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                        r17 = quest:GetHero()
                                        quest:AddPersonToConversation(nil --[[missing]], r17)
                                        cVar3 = quest:IsXbox()
                                        if not cVar3 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d5db99
                                            r18 = quest:GetHero()
                                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_PC_30", r18, nil --[[missing]])
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d5db99
                                            r19 = quest:GetHero()
                                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_BOW1_INSTRUCTIONS_30", r19, nil --[[missing]])
                                            -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_11c;
                                        end
                                    end
                                end
                                quest:SetTimer(ppVar8, nil --[[missing]])
                            end
                        end
                    end
                end
                ::LAB_00d5bf6c::
                if unaff_EBP < quest:GetMasterGameState("SkillScore") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5db99
                    unaff_EBP = quest:GetMasterGameState("SkillScore")
                else
                    if quest:GetMasterGameState("SkillScore") < unaff_EBP then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d5db99
                        quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + 1)
                        cVar3 = quest:IsConversationActive(nil --[[missing]])
                        if not cVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d5db99
                            r20 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                            r21 = quest:GetHero()
                            quest:AddPersonToConversation(nil --[[missing]], r21)
                            r22 = quest:GetHero()
                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", r22, nil --[[missing]])
                        end
                    end
                end
                quest:UpdateQuestInfoCounter(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                if __native_entity_state:GetStateBool("PlayerNotWarned") then
                    fVar37 = 6.0
                    pCVar7 = quest:GetHero()
                    bVar2 = IsDistanceBetweenThingsOver(pCVar7,&pCStack_254,fVar37)
                    __native_condition_3 = bVar2
                    if __native_condition_3 then
                        cVar3 = quest:IsConversationActive(fVar37)
                        __native_condition_3 = not cVar3
                    end
                    if __native_condition_3 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d5db99
                        __native_entity_state:SetStateBool("PlayerNotWarned", false)
                        r23 = quest:AddNewConversation(pCVar7, nil --[[missing]], nil --[[missing]])
                        r24 = quest:GetHero()
                        quest:AddPersonToConversation(nil --[[missing]], r24)
                        r25 = quest:GetHero()
                        quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_RING_OUT", r25, nil --[[missing]])
                    end
                end
                ppVar42 = ppVar10
                iVar9 = quest:GetMasterGameState("SkillScore")
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5db99
            if cStack_26d == 0 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db99
                quest:RemoveQuestInfoElement(nil --[[missing]])
                quest:RemoveQuestInfoElement(nil --[[missing]])
                quest:RemoveQuestInfoElement(nil --[[missing]])
            end
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_1e8);
            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_1e8);
            if bVar2 then
            end
            r26 = quest:GetHero()
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db37
                r27 = quest:GetHero()
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5db37: (native jump target)
                -- TODO(native): goto LAB_00d5db99
            end
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar11 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_1bc,aCStack_178);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar11, (CScriptGameResourceObjectScriptedThingBase *)pCVar30);
            -- TODO(native): pCVar11 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_1bc,aCStack_c0);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar11,pCVar29);
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&piStack_84);
            pCVar28 = ""
            quest:StartMovieSequence()
            uVar32 = 1
            quest:PauseAllNonScriptedEntities((uVar32 ~= 0))
            fVar37 = 1.4013e-45
            quest:FixMovieSequenceCamera(nil --[[missing]])
            uVar24 = 1
            pCVar23 = 0x0
            pCVar41 = 0x0
            ppVar10 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            pCVar40 = 0x0
            quest:FixMovieSequenceCamera((pCVar40 ~= 0))
            quest:SetStateInt("TutorialState", 4)
            quest:SetMasterGameState("MovingDummiesNeeded", true)
            quest:CameraDefault()
            pCVar21 = pCStack_250
            quest:RemoveQuestInfoElement(ppVar10)
            cVar3 = quest:IsXbox()
            if not cVar3 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    pCVar25 = "TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE_PC"
                    quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE_PC")
                    cVar3 = quest:MsgIsGameInfoClickedPast()
                    uVar34 = SUB41(pCVar28,0)
                    uVar31 = uVar32
                    while not cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d5db13
                        cVar3 = quest:MsgIsGameInfoClickedPast()
                        uVar34 = SUB41(pCVar28,0)
                        uVar31 = uVar32
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)aCStack_118,pCVar21,pCVar40,(CCharString *)ppVar10, pCVar41,pCVar23,uVar24,fVar37,(bool)uVar31,(bool)uVar34);
                        -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_118;
                        goto LAB_00d5c4eb
                    end
                end
                -- LAB_00d5db45: (native jump target)
                quest:PauseAllNonScriptedEntities((pCVar41 ~= 0))
                -- LAB_00d5db1f: (native jump target)
                -- TODO(native): StdMap_Destroy_API();
                -- TODO(native): goto LAB_00d5db37
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5db13: (native jump target)
                quest:PauseAllNonScriptedEntities((pCVar23 ~= 0))
                -- TODO(native): goto LAB_00d5db1f
            end
            pCVar25 = "TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE"
            quest:DisplayGameInfo("TEXT_QST_028_SKILL_INSTRUCTIONS_BOW_SNIPE")
            cVar3 = quest:MsgIsGameInfoClickedPast()
            uVar34 = SUB41(pCVar28,0)
            uVar31 = uVar32
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db45
                cVar3 = quest:MsgIsGameInfoClickedPast()
                uVar34 = SUB41(pCVar28,0)
                uVar31 = uVar32
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5db13
            -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)aCStack_e4,pCVar21,pCVar40,(CCharString *)ppVar10,pCVar41, pCVar23,uVar24,fVar37,(bool)uVar31,(bool)uVar34);
            -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_e4;
            ::LAB_00d5c4eb::
            quest:PauseAllNonScriptedEntities((uVar24 ~= 0))
            -- TODO(native): StdMap_Destroy_API();
            cVar3 = quest:IsXbox()
            if not cVar3 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    pCVar28 = "GAME_ACTION_UNSHEATHE_RANGED_WEAPON"
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick()
                    pCVar17 = uVar19
                    pmVar26 = "GAME_ACTION_TOGGLE_FIRST_PERSON_VIEW"
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick()
                    pCVar28 = "GAME_ACTION_FIRE_RANGED_WEAPON"
                    -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
                    quest:AddQuestInfoTick()
                    goto LAB_00d5c6b1
                end
                -- TODO(native): goto LAB_00d5db99
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5db99
            pCVar28 = "HUD_BLACK_BUTTON"
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick()
            pCVar17 = uVar19
            paVar14 = "HUD_CONTROLLER_THUMBSTICK_LEFT_CLICK"
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick()
            pCVar28 = "HUD_CONTROLLER_X"
            -- TODO(native): AddQuestInfoTick is not a ForgeFSE binding
            quest:AddQuestInfoTick()
            ::LAB_00d5c6b1::
            quest:DisplayQuestInfo(nil --[[missing]])
            bVar2 = false
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db99
                uVar19 = quest:GetTimer(ppVar8)
                -- TODO(native): ppVar16 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar19 >> 0x20);
                if uVar19 < 1 then
                    -- TODO(native): uStack_258 = uStack_258 | 2;
                    cVar3 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                    if cVar3 then return end  -- TODO(native): goto LAB_00d5c757
                    fVar37 = 6.0
                    pCVar7 = quest:GetHero()
                    bVar5 = quest:IsDistanceBetweenThingsUnder(pCVar7, r27, &pCStack_254)
                    if not bVar5 then return end  -- TODO(native): goto LAB_00d5c757
                    bVar5 = true
                else
                    -- LAB_00d5c757: (native jump target)
                    bVar5 = false
                end
                if (uStack_258 & 2) ~= 0 then
                    -- TODO(native): uStack_258 = uStack_258 & 0xfffffffd;
                end
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5db99
                    ppVar10 = quest:AddNewConversation(r26, bVar5, bVar2)
                    r28 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r28)
                    r29 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_BOW_UNSHEATH", r29, nil --[[missing]])
                    cVar3 = quest:IsXbox()
                    if not cVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d5db99
                        pCVar28 = "TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC"
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC")
                        cVar3 = quest:MsgIsGameInfoClickedPast()
                        while not cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d5db99
                            cVar3 = quest:MsgIsGameInfoClickedPast()
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d5db99
                        pCVar28 = "TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP"
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP")
                        cVar3 = quest:MsgIsGameInfoClickedPast()
                        while not cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d5db99
                            cVar3 = quest:MsgIsGameInfoClickedPast()
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5db99
                    quest:SetTimer(ppVar8, nil --[[missing]])
                    pCVar17 = ""
                end
                cVar3 = quest:MsgOnHeroFiredRangedWeapon()
                if cVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5db99
                    bVar2 = true
                end
                -- TODO(native): uStack_258 = uStack_258 | 4;
                cVar3 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                native_arg_sequence_2 = false
                if not cVar3 then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
                if not native_arg_sequence_2 then
                    -- TODO(native): IsHeroInProjectileWeaponMode is not a ForgeFSE binding
                    quest:IsHeroInProjectileWeaponMode()
                    if not cVar3 then
                        native_arg_sequence_2 = true
                    else
                        native_arg_sequence_2 = false
                    end
                end
                if not native_arg_sequence_2 then
                    -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                    quest:IsPlayerHoldingFireRangedWeaponButton()
                    if not cVar3 then
                        native_arg_sequence_2 = true
                    else
                        native_arg_sequence_2 = false
                    end
                end
                if native_arg_sequence_2 then
                    bVar5 = false
                else
                    bVar5 = true
                end
                if (uStack_258 & 4) ~= 0 then
                    -- TODO(native): uStack_258 = uStack_258 & 0xfffffffb;
                end
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5db99
                    quest:RemoveQuestInfoElement(bVar5)
                    quest:RemoveQuestInfoElement(bVar2)
                    quest:RemoveQuestInfoElement(nil --[[missing]])
                else
                    if cStack_26d == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d5db99
                        r30 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                        pCVar33 = pCVar17
                        quest:UpdateQuestInfoTick(nil --[[missing]], nil --[[missing]])
                        -- TODO(native): IsHeroInProjectileWeaponMode is not a ForgeFSE binding
                        quest:IsHeroInProjectileWeaponMode()
                        quest:UpdateQuestInfoTick(nil --[[missing]], nil --[[missing]])
                        -- TODO(native): IsPlayerHoldingFireRangedWeaponButton is not a ForgeFSE binding
                        quest:IsPlayerHoldingFireRangedWeaponButton()
                        quest:UpdateQuestInfoTick(nil --[[missing]], nil --[[missing]])
                    end
                end
            until not (not bVar2)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5db99
            if cStack_26d == 0 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db99
                quest:RemoveQuestInfoElement(nil --[[missing]])
                quest:RemoveQuestInfoElement(nil --[[missing]])
                quest:RemoveQuestInfoElement(nil --[[missing]])
            end
            -- TODO(native): CTimer::CTimer((CTimer *)&uStack_26c);
            -- TODO(native): __ftol2();
            quest:SetTimer(ppVar8, nil --[[missing]])
            quest:SetMasterGameState("SkillScore", 0)
            paVar14 = "HUD_ICON_MULTI_ARROW"
            uVar19 = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", nil --[[missing]], nil --[[missing]])
            paVar14 = "HUD_ICON_ARROW"
            uVar20 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", nil --[[missing]], nil --[[missing]])
            pCVar28 = "HUD_CLOCK_ICON"
            uVar20 = quest:AddQuestInfoTimer(ppVar8, "HUD_CLOCK_ICON", nil --[[missing]])
            quest:DisplayQuestInfo(true)
            quest:UpdateQuestInfoCounter(uVar19, quest:GetMasterGameState("HighestSkillScore"), -1)
            quest:SetMasterGameState("SkillTestOccuring", true)
            quest:SetTimer(ppVar8, 0xf)
            iVar9 = quest:GetTimer(ppVar8)
            while (0 < iVar9 and (cStack_26d = (ppVar8 >> 0x18), cStack_26d == 0)) do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db90
                if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5db90
                end
                quest:UpdateQuestInfoCounter(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5db90
                    quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                    quest:UpdateQuestInfoCounter(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                end
                uVar19 = quest:GetTimer(ppVar8)
                -- TODO(native): ppVar16 = (pair<EHeroMorphType,CParticleMorphs::CEntry> *)((ulonglong)uVar19 >> 0x20);
                if uVar19 < 1 then
                    -- TODO(native): uStack_258 = uStack_258 | 8;
                    cVar3 = quest:IsPlayerCarryingItemOfType("OBJECT_YEW_LONGBOW")
                    if cVar3 then return end  -- TODO(native): goto LAB_00d5cd47
                    bVar2 = true
                else
                    -- LAB_00d5cd47: (native jump target)
                    bVar2 = false
                end
                if (uStack_258 & 8) ~= 0 then
                    -- TODO(native): uStack_258 = uStack_258 & 0xfffffff7;
                end
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5db90
                    ppVar10 = quest:AddNewConversation(nil --[[missing]], bVar2, nil --[[missing]])
                    r31 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r31)
                    r32 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_BOW_UNSHEATH", r32, nil --[[missing]])
                    cVar3 = quest:IsXbox()
                    if not cVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d5db90
                        paVar14 = "TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC"
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP_PC")
                        cVar3 = quest:MsgIsGameInfoClickedPast()
                        while not cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d5db90
                            cVar3 = quest:MsgIsGameInfoClickedPast()
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d5db90
                        paVar14 = "TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP"
                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BOWWIELD_HELP")
                        cVar3 = quest:MsgIsGameInfoClickedPast()
                        while not cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d5db90
                            cVar3 = quest:MsgIsGameInfoClickedPast()
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5db90
                    quest:SetTimer(ppVar8, nil --[[missing]])
                end
                if not __native_entity_state:GetStateBool("PlayerNotWarned") then
                    -- LAB_00d5cf7a: (native jump target)
                    bVar2 = false
                else
                    -- TODO(native): uStack_258 = uStack_258 | 0x30;
                    -- TODO(native): ppVar10 = *(pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> **)(this + 4);
                    fVar37 = 6.0
                    ppVar42 = ppVar10
                    pCVar7 = (**(*ppVar10 + 0x120))()
                    pCVar12 = (**(*ppVar10 + 0x118))()
                    bVar2 = IsDistanceBetweenThingsOver(pCVar12,pCVar7,fVar37)
                    if not bVar2 then return end  -- TODO(native): goto LAB_00d5cf7a
                    bVar2 = true
                end
                if (uStack_258 & 0x20) ~= 0 then
                    -- TODO(native): uStack_258 = uStack_258 & 0xffffffdf;
                end
                if (uStack_258 & 0x10) ~= 0 then
                    -- TODO(native): uStack_258 = uStack_258 & 0xffffffef;
                end
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d5db90
                    __native_entity_state:SetStateBool("PlayerNotWarned", false)
                    r33 = quest:AddNewConversation(nil --[[missing]], bVar2, (fVar37 ~= 0))
                    r34 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r34)
                    r35 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_RING_OUT", r35, nil --[[missing]])
                end
                iVar9 = quest:GetTimer(ppVar8)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5db90: (native jump target)
                -- TODO(native): goto LAB_00d5db99
            end
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            quest:SetMasterGameState("SkillTestOccuring", false)
            cVar3 = quest:IsHeroControlledByPlayer()
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db90
                cVar3 = quest:IsHeroControlledByPlayer()
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5db90
            quest:DisplayQuestInfo(nil --[[missing]])
            quest:RemoveQuestInfoElement(nil --[[missing]])
            quest:RemoveQuestInfoElement(nil --[[missing]])
            quest:RemoveQuestInfoElement(nil --[[missing]])
            quest:EntitySetTargetable(nil --[[missing]], nil --[[missing]])
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&ppuStack_210);
            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_210);
            if bVar2 then
            end
            r36 = quest:GetHero()
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db84
                r37 = quest:GetHero()
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5db84: (native jump target)
                -- TODO(native): goto LAB_00d5db90
            end
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar11 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_204,(CCharString *)aaStack_18c);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar11,pCVar29);
            -- TODO(native): pCVar11 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_204,aCStack_184);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar11,pCVar29);
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&ppuStack_218);
            pCVar28 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            ppVar10 = 0x1
            quest:FixMovieSequenceCamera((ppVar10 ~= 0))
            if (pCVar38 >> 0x18) == 0 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    pppuStack_238 = quest:GetMasterGameState("SkillScore")
                    -- TODO(native): pfVar15 = *(float **)(DAT_0143e90c + 0xec0);
                    iVar9 = 0
                    repeat
                        iVar13 = iVar9
                        if *pfVar15 < pppuStack_238 ~= (*pfVar15 == pppuStack_238) then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d5db53
                            break
                        end
                        iVar9 = iVar13 + 1
                        pfVar15 = pfVar15 + 1
                    until not (iVar9 < 7)
                    -- TODO(native): Std_Deque_Construct();
                    native_arg_switch_2 = iVar13
                    repeat
                        if native_arg_switch_2 == 0 then
                            pcVar36 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_APLUS"
                            -- TODO(native): pCVar28 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)&ppuStack_234,aCStack_17c);
                            -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_17c;
                            break
                        else
                            if native_arg_switch_2 == 1 then
                                pcVar36 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_A"
                                -- TODO(native): pCVar28 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)&ppuStack_234,(CCharString *)aaStack_174);
                                paVar14 = "$GRADE"
                                break
                            else
                                if native_arg_switch_2 == 2 then
                                    pcVar36 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_B"
                                    -- TODO(native): pCVar28 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)&ppuStack_234,aCStack_16c);
                                    -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_16c;
                                    break
                                else
                                    if native_arg_switch_2 == 3 then
                                        pcVar36 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_C"
                                        -- TODO(native): pCVar28 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)&ppuStack_234,(CCharString *)aaStack_164);
                                        paVar14 = "$GRADE"
                                        break
                                    else
                                        if native_arg_switch_2 == 4 then
                                            pcVar36 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_D"
                                            -- TODO(native): pCVar28 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)&ppuStack_234,aCStack_15c);
                                            -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_15c;
                                            break
                                        else
                                            if native_arg_switch_2 == 5 then
                                                pcVar36 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_E"
                                                -- TODO(native): pCVar28 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)&ppuStack_234,aCStack_154);
                                                -- TODO(native): paVar14 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_154;
                                                break
                                            else
                                                if native_arg_switch_2 == 6 then
                                                    pcVar36 = "TEXT_QST_028_GUILDMASTER_SKILL_GRADE_F"
                                                    -- TODO(native): pCVar28 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[]((map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)&ppuStack_234,(CCharString *)aaStack_14c);
                                                    paVar14 = "$GRADE"
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
                    ppVar22 = 0x0
                    -- TODO(native): RunCutsceneMacro_Func();
                    -- TODO(native): LTextTreeWalkThrough__Dtor();
                    goto LAB_00d5d526
                end
                -- TODO(native): goto LAB_00d5db62
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d5db53: (native jump target)
                quest:PauseAllNonScriptedEntities((ppVar22 ~= 0))
                -- LAB_00d5db6f: (native jump target)
                -- TODO(native): StdMap_Destroy_API();
                -- TODO(native): goto LAB_00d5db84
            end
            ppVar22 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            ::LAB_00d5d526::
            quest:SetStateInt("TutorialState", 0)
            quest:RemoveQuestInfoElement(ppVar22)
            quest:Pause(nil --[[missing]])
            pCVar28 = "TEXT_QST_028_GUILDMASTER_SKILL_REPEAT_QUESTION"
            quest:GiveHeroYesNoQuestion(pCVar28, "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "")
            iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
            while iVar9 < 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db62
                iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5db53
            alive = not quest:IsActiveThreadTerminating()
            if iVar9 == 1 then
                if alive then
                    -- TODO(native): RunCutsceneMacro_Func();
                    quest:SetMasterGameState("SkillRepeating", false)
                    goto LAB_00d5d7a9
                end
                -- LAB_00d5db62: (native jump target)
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                -- TODO(native): goto LAB_00d5db6f
            end
            if not alive then return end  -- TODO(native): goto LAB_00d5db53
            ppVar22 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            quest:SetMasterGameState("MovingDummiesNeeded", false)
            quest:SetMasterGameState("SkillRepeating", true)
            quest:SetMasterGameState("SkillRepeatKnown", true)
            r38 = quest:GetHero()
            cVar3 = quest:IsObjectInThingsPossession("OBJECT_YEW_LONGBOW", r38)
            if cVar3 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db62
                quest:TakeObjectFromHero("OBJECT_YEW_LONGBOW")
            end
            cVar3 = quest:GetMasterGameState("SkillDummyReset")
            while cVar3 ~= '\x01' do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d5db53
                cVar3 = quest:GetMasterGameState("SkillDummyReset")
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d5db62
            ::LAB_00d5d7a9::
            quest:FixMovieSequenceCamera((ppVar22 ~= 0))
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            -- TODO(native): StdMap_Destroy_API();
            cVar3 = quest:GetMasterGameState("SkillRepeating")
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            pVar27 = SUB41(&"M_GuildmasterMarker",0)
            piVar6 = quest:GetThingWithScriptName("M_GuildmasterMarker")
            pCVar40 = 0x1
            pCVar38 = 0x0
            pCVar28 = 0x0
            pcVar36 = 0x40400000
            pCVar7 = piVar6:GetPos()
            me:MoveToPosition(pCVar7, SUB41(pCVar35,0))
            piVar6 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
            piVar6:GetPos()
            r39 = quest:CreateCreature("SkillApprenticeMarker", nil --[[missing]], "SkillApprentice")
            if piStack_84 ~= nil then
                -- TODO(native): (**(code **)(*piStack_84 + 0x118))();
            end
            piVar6 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
            uVar32 = piVar6:GetPos()
            pCVar28 = "HUD_CONTROLLER_X"
            r40 = quest:CreateCreature(pCVar28, uVar32, "BirdKillerMarker")
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_05", "", "")
            ppVar42 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(ppVar42, pcVar36)
            quest:SetPlayerUsingRangedDummies(false)
        end
        ::LAB_00d5da96::
        quest:DeregisterTimer(ppVar8)
    end
end

function Init(quest, me)
    __native_entity_state:SetStateBool("PlayerNotWarned", true)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

