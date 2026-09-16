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
    local CVar29, CVar3, CVar32, __native_condition_1, __native_condition_2, aVar25, bVar1, bVar4, cVar2, fStack_1e0, fVar14, fVar15, fVar34, iVar10, iVar7, native_arg_switch_5, pCVar17, pCVar19, pCVar21, pCVar26, pCVar27, pCVar33, pCVar35, pCVar36, pCVar6, paVar11, paVar20, paVar23, pcVar30, pfVar12, piVar5, piVar9, ppVar24, ppVar31, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r3, r30, r31, r32, r33, r34, r35, r4, r5, r6, r7, r8, r9, uVar16, uVar22, uVar28
    local alive = true
    -- TODO(native): bVar1 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_228);
    if bVar1 then
    end
    cVar2 = me:AcquireControl(4)
    while not cVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        cVar2 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00d61b7b end
    quest:EntitySetAsKillable(nil --[[missing]], false)
    r1 = quest:GetHero()
    quest:EntitySetAlwaysBlockAttacksFromThing(r1, nil --[[missing]], false)
    quest:SetThingHasInformation(nil --[[missing]])
    quest:SetPlayerUsingWillDummies(false)
    piVar5 = *(this + 0x10)
    if piVar5 ~= nil then
        -- TODO(native): *piVar5 = *piVar5 + 1;
    end
    quest:SetIsPushableByHero(nil --[[missing]], false)
    r2 = quest:RegisterTimer()
    r3 = quest:RegisterTimer()
    quest:SetTimer(0, nil --[[missing]])
    quest:EntitySetTargetingType(nil --[[missing]], nil --[[missing]])
    if not quest:GetStateBool("TestFinished") then
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            quest:DeregisterTimer(nil --[[missing]])
            quest:DeregisterTimer(nil --[[missing]])
            return
        end
        piVar5 = quest:GetThingWithScriptName("M_WillTeacherStand")
        pCVar33 = 0x1
        pcVar30 = 0x0
        pCVar6 = piVar5:GetPos()
        me:MoveToPosition(pCVar6, pCVar26, SUB41(me,0), SUB41(pCVar18,0))
        iVar7 = quest:GetStateInt("TutorialState")
        while iVar7 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d61b69 end
            cVar2 = me:IsTalkedToByHero()
            if cVar2 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d61b69 end
                if quest:GetMasterGameState("HeroTakingGuildTest") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d61b69 end
                    quest:SetStateInt("TutorialState", 2)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d61b69 end
                    quest:StartMovieSequence()
                    pCVar6 = 0x1
                    quest:PauseAllNonScriptedEntities((pCVar6 ~= 0))
                    -- TODO(native): CVar3 = (CScriptGameResourceObjectScriptedThingBase) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffdc0);
                    fVar14 = quest:GetHealth(nil --[[missing]])
                    fVar15 = _DAT_0122dedc
                    if fVar15 < fVar14 then
                        CVar29 = 0x0
                        pCVar33 = 0x1
                        pCVar27 = 0x0
                        pCVar26 = 0x0
                        pcVar30 = "TEXT_QST_028_GUILDMASTER_WILL_NOT_START"
                        pCVar6 = quest:GetHero()
                        r4 = me:Speak(pCVar6, pcVar30, pCVar26, (pCVar27 ~= 0), (pCVar33 ~= 0), (CVar29 ~= 0))
                        bVar1 = me:IsPerformingScriptTask()
                        if bVar1 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities((CVar29 ~= 0))
                                    -- TODO(native): goto LAB_00d61b0a
                                end
                                bVar1 = me:IsPerformingScriptTask()
                            until not (bVar1)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            iVar7 = *piVar5
                            -- TODO(native): goto LAB_00d61b5a
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                end
            end
            fVar34 = 5.5
            pCVar6 = quest:GetHero()
            bVar1 = quest:IsDistanceBetweenThingsUnder(pCVar6, me, fVar34)
            __native_condition_1 = (bVar1) and (iVar7 = GSI->GetTimer(), iVar7 < 1)
            if __native_condition_1 then
                bVar1 = me:IsPerformingScriptTask()
                __native_condition_1 = not bVar1
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d61b69 end
                r5 = quest:AddNewConversation(nil --[[missing]], false, nil --[[missing]])
                r6 = quest:GetHero()
                quest:AddPersonToConversation(nil --[[missing]], r6)
                quest:SetTimer(nil --[[missing]], nil --[[missing]])
                if iStack_238 == 0 then
                    r7 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(r7, nil --[[missing]])
                    r8 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_FIRST", r8, nil --[[missing]])
                    -- LAB_00d5e63f: (native jump target)
                else
                    if iStack_238 == 1 then
                        r9 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(r9, nil --[[missing]])
                        r10 = quest:GetHero()
                        quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_SECOND", r10, nil --[[missing]])
                        -- TODO(native): goto LAB_00d5e63f
                    end
                end
                -- TODO(native): iStack_238 = 1 - iStack_238;
            end
            iVar7 = quest:GetStateInt("TutorialState")
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            quest:SetMasterGameState("HeroTakingGuildTest", true)
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d61b69 end
                quest:SetStateInt("TutorialState", 2)
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_44);
                -- TODO(native): bVar1 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_44);
                if bVar1 then
                end
                r11 = quest:GetHero()
                cVar2 = me:AcquireControl(4)
                while not cVar2 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d60aee
                    r12 = quest:GetHero()
                    cVar2 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00d60aee: (native jump target)
                    goto LAB_00d61b69
                end
                -- TODO(native): StdMap_Construct_API();
                -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_60,(CCharString *)aaStack_19c);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8, (CScriptGameResourceObjectScriptedThingBase *)pCVar35);
                -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_60,(CCharString *)aaStack_184);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                quest:GiveHeroAbility(nil --[[missing]], nil --[[missing]])
                quest:SetMasterGameState("WillTrainingStarted", true)
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_34);
                pCVar26 = ""
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                quest:FixMovieSequenceCamera(nil --[[missing]])
                ppVar24 = 0x0
                -- TODO(native): RunCutsceneMacro_Func();
                quest:FixMovieSequenceCamera((ppVar24 ~= 0))
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                -- TODO(native): StdMap_Destroy_API();
                cVar2 = quest:IsXbox()
                if not cVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d61b69 end
                    pCVar26 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC"
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                    cVar2 = quest:MsgIsGameInfoClickedPast()
                    while not cVar2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d61b69 end
                        cVar2 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d61b69 end
                    -- TODO(native): paVar11 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_140;
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d61b69 end
                    pCVar26 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAP"
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                    cVar2 = quest:MsgIsGameInfoClickedPast()
                    while not cVar2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d61b69 end
                        cVar2 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d61b69 end
                    -- TODO(native): paVar11 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_150;
                end
                quest:SetMasterGameState("WillScore", 0)
                -- TODO(native): CTimer::CTimer((CTimer *)&stack0xfffffdb8);
                quest:SetTimer(nil --[[missing]], nil --[[missing]])
                iVar7 = quest:GetStateInt("TutorialState")
                -- TODO(native): cStack_239 = '\0';
                while iVar7 == 2 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d6138b end
                    while (cStack_239 == 0 and (quest:GetMasterGameState("WillScore") == 0)) do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d6138b end
                        cVar2 = me:IsTalkedToByHero()
                        if cVar2 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d6138b end
                            -- TODO(native): cStack_239 = '\x01';
                        end
                        iVar7 = quest:GetTimer(nil --[[missing]])
                        if iVar7 < 1 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d6138b end
                            r13 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(r13, r12)
                            quest:SetTimer(nil --[[missing]], nil --[[missing]])
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d6138b end
                    if quest:GetMasterGameState("WillScore") ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d6138b end
                        quest:SetStateInt("TutorialState", 3)
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_54);
                        -- TODO(native): bVar1 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_54);
                        if bVar1 then
                        end
                        r14 = quest:GetHero()
                        cVar2 = me:AcquireControl(4)
                        while not cVar2 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d60aff
                            r15 = quest:GetHero()
                            cVar2 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00d60aff: (native jump target)
                            goto LAB_00d61b69
                        end
                        -- TODO(native): StdMap_Construct_API();
                        -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)aCStack_1c0,aCStack_130);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8, (CScriptGameResourceObjectScriptedThingBase *)pCVar35);
                        -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)aCStack_1c0,aCStack_128);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_1c);
                        pCVar26 = ""
                        quest:StartMovieSequence()
                        quest:FixMovieSequenceCamera(nil --[[missing]])
                        ppVar24 = 0x0
                        -- TODO(native): RunCutsceneMacro_Func();
                        quest:FixMovieSequenceCamera((ppVar24 ~= 0))
                        -- TODO(native): StdMap_Destroy_API();
                    end
                    if cStack_239 ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d6138b end
                        -- TODO(native): cStack_239 = extraout_AL_27;
                        cVar2 = quest:IsXbox()
                        if not cVar2 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d6138b end
                            pCVar26 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC"
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            cVar2 = quest:MsgIsGameInfoClickedPast()
                            while not cVar2 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d6138b end
                                cVar2 = quest:MsgIsGameInfoClickedPast()
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d6138b end
                            pCVar26 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAP"
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            cVar2 = quest:MsgIsGameInfoClickedPast()
                            while not cVar2 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d6138b end
                                cVar2 = quest:MsgIsGameInfoClickedPast()
                            end
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d6138b end
                    end
                    iVar7 = quest:GetStateInt("TutorialState")
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d6138b end
                cVar2 = quest:IsXbox()
                if not cVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d6138b end
                    pCVar26 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC"
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                    cVar2 = quest:MsgIsGameInfoClickedPast()
                    while not cVar2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d6138b end
                        cVar2 = quest:MsgIsGameInfoClickedPast()
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d6138b end
                    pCVar26 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST"
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                    cVar2 = quest:MsgIsGameInfoClickedPast()
                    while not cVar2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d6138b end
                        cVar2 = quest:MsgIsGameInfoClickedPast()
                    end
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d6138b end
                cVar2 = quest:MsgOnHeroCastSpell()
                while cVar2 == 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d6138b end
                    cVar2 = me:IsTalkedToByHero()
                    if cVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d6138b end
                        cVar2 = quest:IsXbox()
                        if not cVar2 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d6138b end
                            pCVar26 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC"
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            pCVar26 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC"
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                            cVar2 = quest:MsgIsGameInfoClickedPast()
                            while not cVar2 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d6138b end
                                cVar2 = quest:MsgIsGameInfoClickedPast()
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d6138b end
                            pCVar26 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAP"
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            paVar11 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST"
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                            cVar2 = quest:MsgIsGameInfoClickedPast()
                            while not cVar2 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d6138b end
                                cVar2 = quest:MsgIsGameInfoClickedPast()
                            end
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d6138b end
                    end
                    cVar2 = quest:MsgOnHeroCastSpell()
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d6138b end
                -- TODO(native): CTimer::CTimer((CTimer *)&stack0xfffffdc0);
                -- TODO(native): __ftol2();
                quest:SetTimer(nil --[[missing]], nil --[[missing]])
                quest:SetStateInt("TutorialState", 3)
                quest:SetMasterGameState("WillScore", 0)
                -- TODO(native): CTimer::CTimer((CTimer *)&stack0xfffffdc0);
                quest:SetTimer(nil --[[missing]], nil --[[missing]])
                pCVar26 = "HUD_ICON_ARROW"
                uVar16 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", nil --[[missing]], nil --[[missing]])
                uVar16 = quest:AddQuestInfoTimer(nil --[[missing]], "HUD_CLOCK_ICON", nil --[[missing]])
                quest:DisplayQuestInfo(nil --[[missing]])
                quest:SetTimer(nil --[[missing]], nil --[[missing]])
                iVar7 = quest:GetTimer(nil --[[missing]])
                while (0 < iVar7 and (cStack_239 == 0)) do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d61379 end
                    iVar7 = quest:GetTimer(nil --[[missing]])
                    if iVar7 < 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d61379 end
                        r16 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(r16, r15)
                        quest:SetTimer(nil --[[missing]], nil --[[missing]])
                    end
                    quest:SetMasterGameState("WillTestOccuring", true)
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d61379 end
                        -- TODO(native): cStack_239 = '\x01';
                    end
                    iVar7 = quest:GetHeroWillEnergy()
                    __native_condition_2 = iVar7 == 0
                    if __native_condition_2 then
                        iVar7 = quest:GetTimer(nil --[[missing]])
                        __native_condition_2 = iVar7 < 1
                    end
                    if __native_condition_2 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d61379 end
                        ppVar24 = quest:AddNewConversation(r14, nil --[[missing]], nil --[[missing]])
                        r17 = quest:GetHero()
                        quest:AddPersonToConversation(nil --[[missing]], r17)
                        r18 = quest:GetHero()
                        quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_WILL_NO_WILL", r18, r11)
                        quest:SetTimer(nil --[[missing]], nil --[[missing]])
                    end
                    cVar2 = me:IsTalkedToByHero()
                    if cVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d61379 end
                        cVar2 = quest:IsXbox()
                        if not cVar2 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d61379 end
                            paVar11 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC"
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            paVar11 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC"
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                            cVar2 = quest:MsgIsGameInfoClickedPast()
                            while not cVar2 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d61379 end
                                cVar2 = quest:MsgIsGameInfoClickedPast()
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d61379 end
                            pCVar26 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAP"
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            pCVar26 = "TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST"
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                            cVar2 = quest:MsgIsGameInfoClickedPast()
                            while not cVar2 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d61379 end
                                cVar2 = quest:MsgIsGameInfoClickedPast()
                            end
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d61379 end
                    end
                    quest:UpdateQuestInfoCounter(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                    iVar7 = quest:GetTimer(nil --[[missing]])
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d61379 end
                quest:SetMasterGameState("WillTestOccuring", false)
                cVar2 = quest:IsHeroControlledByPlayer()
                while not cVar2 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d61379 end
                    cVar2 = quest:IsHeroControlledByPlayer()
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d61379 end
                quest:DisplayQuestInfo(nil --[[missing]])
                quest:RemoveQuestInfoElement(nil --[[missing]])
                quest:RemoveQuestInfoElement(nil --[[missing]])
                quest:SetStateInt("TutorialState", 0)
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_218);
                -- TODO(native): bVar1 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_218);
                if bVar1 then
                end
                r19 = quest:GetHero()
                cVar2 = me:AcquireControl(4)
                while not cVar2 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d61370 end
                    r20 = quest:GetHero()
                    cVar2 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d61370 end
                -- TODO(native): StdMap_Construct_API();
                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_1dc);
                paVar11 = ""
                quest:StartMovieSequence()
                quest:FixMovieSequenceCamera(nil --[[missing]])
                if cStack_239 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d6135b end
                    fStack_1e0 = quest:GetMasterGameState("WillScore")
                    -- TODO(native): pfVar12 = *(float **)(DAT_0143e90c + 0xecc);
                    iVar7 = 0
                    repeat
                        iVar10 = iVar7
                        if *pfVar12 < fStack_1e0 ~= (*pfVar12 == fStack_1e0) then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d6135b end
                            break
                        end
                        iVar7 = iVar10 + 1
                        pfVar12 = pfVar12 + 1
                    until not (iVar7 < 7)
                    -- TODO(native): Std_Deque_Construct();
                    native_arg_switch_5 = iVar10
                    repeat
                        if native_arg_switch_5 == 0 then
                            pcVar30 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_APLUS"
                            -- TODO(native): pCVar26 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](amStack_1cc,(CCharString *)aaStack_88);
                            paVar11 = "$GRADE"
                            break
                        else
                            if native_arg_switch_5 == 1 then
                                pcVar30 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_A"
                                -- TODO(native): pCVar26 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](amStack_1cc,(CCharString *)aaStack_fc);
                                paVar11 = "$GRADE"
                                break
                            else
                                if native_arg_switch_5 == 2 then
                                    pcVar30 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_B"
                                    -- TODO(native): pCVar26 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](amStack_1cc,aCStack_e4);
                                    -- TODO(native): paVar11 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_e4;
                                    break
                                else
                                    if native_arg_switch_5 == 3 then
                                        pcVar30 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_C"
                                        -- TODO(native): pCVar26 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](amStack_1cc,aCStack_f4);
                                        -- TODO(native): paVar11 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_f4;
                                        break
                                    else
                                        if native_arg_switch_5 == 4 then
                                            pcVar30 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_D"
                                            -- TODO(native): pCVar26 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](amStack_1cc,aCStack_e8);
                                            -- TODO(native): paVar11 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_e8;
                                            break
                                        else
                                            if native_arg_switch_5 == 5 then
                                                pcVar30 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_E"
                                                -- TODO(native): pCVar26 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](amStack_1cc,aCStack_ec);
                                                -- TODO(native): paVar11 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_ec;
                                                break
                                            else
                                                if native_arg_switch_5 == 6 then
                                                    pcVar30 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_F"
                                                    -- TODO(native): pCVar26 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](amStack_1cc,aCStack_d8);
                                                    -- TODO(native): paVar11 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_d8;
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
                    -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_21c,aCStack_d0);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                    -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_21c,aCStack_e0);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                    ppVar24 = 0x0
                    -- TODO(native): RunCutsceneMacro_Func();
                    quest:PauseAllNonScriptedEntities((ppVar24 ~= 0))
                    ppVar24 = 0x0
                    -- TODO(native): RunCutsceneMacro_Func();
                    quest:Pause(ppVar24)
                    uVar28 = 1
                    pCVar26 = ""
                    pCVar27 = "TEXT_OBJECT_HERO_ANSWER_RETAKE"
                    pCVar33 = "TEXT_OBJECT_HERO_ANSWER_CONTINUE"
                    pCVar19 = "TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION"
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "")
                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar7 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d61578
                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d61342
                    alive = not quest:IsActiveThreadTerminating()
                    if iVar7 == 1 then
                        if not alive then return end  -- TODO(native): goto LAB_00d61578
                        quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", true)
                        r21 = quest:GetThingWithScriptName("MeleeApprentice")
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&stack0xfffffdc0);
                        -- TODO(native): bVar1 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffdc0);
                        if bVar1 then
                        end
                        cVar2 = me:AcquireControl(4)
                        while not cVar2 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d6132d
                            cVar2 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d61563
                        -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xfffffda8,(CCharString *)aCStack_1d0);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                        -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xfffffda8,aCStack_1c8);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                        -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xfffffda8,aCStack_1e4);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                        ppVar24 = 0x0
                        -- TODO(native): RunCutsceneMacro_Func();
                        bVar1 = false
                        -- TODO(native): in_stack_fffffd44 = aCStack_1ec;
                        -- TODO(native): in_stack_fffffd40 = aCStack_1c0;
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "")
                        -- TODO(native): in_stack_fffffd3c = (CCharString *)0xd604f2;
                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar7 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d6132d
                            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d61563
                        alive = not quest:IsActiveThreadTerminating()
                        if iVar7 == 1 then
                            if not alive then return end  -- TODO(native): goto LAB_00d61563
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd90);
                            fVar14 = quest:GetHealth(r21)
                            fVar15 = _DAT_0122dedc
                            uVar22 = uVar28
                            aVar25 = SUB41(pCVar26,0)
                            if fVar14 <= fVar15 then return end  -- TODO(native): goto LAB_00d610d3
                            r22 = me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_NO", 0x0, false, true, false)
                            bVar1 = me:IsPerformingScriptTask()
                            uVar22 = uVar28
                            aVar25 = SUB41(pCVar26,0)
                            goto FLOW_native_label_3
                        end
                        if not alive then return end  -- TODO(native): goto LAB_00d6132d
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffd90);
                        uVar22 = SUB41(pCVar19,0)
                        fVar15 = quest:GetHealth(r20)
                        cVar2 = _DAT_0122dedc < fVar15
                        if cVar2 ~= 0 then
                            r23 = me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_YES", 0x0, false, true, false)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d61563
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d6132d
                        end
                        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&iStack_238);
                        if bVar4 then
                        end
                    else
                        if not alive then return end  -- TODO(native): goto LAB_00d61342
                        quest:SetHeroWillEnergyLevel(uVar28)
                        -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xfffffda4,(CCharString *)&stack0xfffffd9c);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                        -- TODO(native): RunCutsceneMacro_Func();
                        bVar1 = true
                    end
                    quest:PauseAllNonScriptedEntities(bVar1)
                    -- TODO(native): LTextTreeWalkThrough__Dtor();
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d6135b end
                    ppVar24 = 0x1
                    quest:PauseAllNonScriptedEntities((ppVar24 ~= 0))
                    -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_220,aCStack_d0);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                    -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_220,aCStack_148);
                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                    ppVar24 = 0x0
                    -- TODO(native): RunCutsceneMacro_Func();
                    quest:Pause(ppVar24)
                    uVar28 = 1
                    pCVar26 = ""
                    paVar11 = "TEXT_OBJECT_HERO_ANSWER_RETAKE"
                    pCVar27 = "TEXT_OBJECT_HERO_ANSWER_CONTINUE"
                    paVar23 = "TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION"
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "")
                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar7 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d6101c end
                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d60b31
                    alive = not quest:IsActiveThreadTerminating()
                    if iVar7 == 1 then
                        if not alive then goto LAB_00d6101c end
                        quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", true)
                        paVar20 = "HERO"
                        r24 = quest:GetThingWithScriptName("MeleeApprentice")
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_198);
                        -- TODO(native): bVar1 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_198);
                        if bVar1 then
                        end
                        cVar2 = me:AcquireControl(4)
                        while not cVar2 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d60b19
                            cVar2 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d61004 end
                        -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xfffffdb8,aCStack_e8);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                        -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xfffffdb8,aCStack_150);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                        -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xfffffdb8,aCStack_bc);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8, (CScriptGameResourceObjectScriptedThingBase *)pCVar33);
                        ppVar24 = 0x0
                        -- TODO(native): RunCutsceneMacro_Func();
                        bVar1 = false
                        pCVar33 = "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION"
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "")
                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar7 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d60b19
                            iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d61004 end
                        alive = not quest:IsActiveThreadTerminating()
                        if iVar7 == 1 then
                            if not alive then goto LAB_00d61004 end
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffda0);
                            fVar14 = quest:GetHealth(r24)
                            fVar15 = _DAT_0122dedc
                            uVar22 = uVar28
                            aVar25 = SUB41(pCVar26,0)
                            if fVar14 <= fVar15 then return end  -- TODO(native): goto LAB_00d60be3
                            r25 = me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_NO", 0x0, false, true, false)
                            bVar1 = me:IsPerformingScriptTask()
                            uVar22 = uVar28
                            aVar25 = SUB41(pCVar26,0)
                            goto FLOW_native_label_4
                        end
                        if not alive then return end  -- TODO(native): goto LAB_00d60b19
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffda0);
                        uVar22 = SUB41(paVar23,0)
                        fVar15 = quest:GetHealth(r19)
                        cVar2 = _DAT_0122dedc < fVar15
                        if cVar2 ~= 0 then
                            r26 = me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_YES", 0x0, false, true, false)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00d61004 end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d60b19
                        end
                        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_190);
                        if bVar4 then
                        end
                        quest:PauseAllNonScriptedEntities((ppVar24 ~= 0))
                    else
                        if not alive then return end  -- TODO(native): goto LAB_00d60b31
                        quest:SetHeroWillEnergyLevel(uVar28)
                        -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xfffffdb4,aCStack_dc);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar17);
                        ppVar24 = 0x0
                        -- TODO(native): RunCutsceneMacro_Func();
                        bVar1 = true
                        quest:PauseAllNonScriptedEntities(bVar1)
                    end
                end
                quest:FixMovieSequenceCamera((ppVar24 ~= 0))
                quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", false)
                -- TODO(native): StdMap_Destroy_API();
            until not (bVar1)
            alive = not quest:IsActiveThreadTerminating()
            if alive then return end  -- TODO(native): goto LAB_00d6070e
        end
    else
        -- LAB_00d6070e: (native jump target)
        quest:EntitySetTargetingType(nil --[[missing]], nil --[[missing]])
        quest:SetMasterGameState("HeroTakingGuildTest", false)
        quest:SetStateInt("TutorialState", 4)
        quest:SetStateBool("TestFinished", true)
        CVar32 = SUB41(&"M_GuildmasterMarker",0)
        piVar5 = quest:GetThingWithScriptName("M_GuildmasterMarker")
        pCVar33 = 0x1
        pCVar27 = 0x0
        pCVar26 = 0x0
        pCVar6 = piVar5:GetPos()
        me:MoveToPosition(pCVar6, SUB41(ppVar24,0))
        piVar5 = quest:GetThingWithScriptName("WillApprentice")
        cVar2 = (piVar5 ~= nil and piVar5:IsAlive())
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00d61b0a: (native jump target)
                quest:DeregisterTimer(pCVar26)
                quest:DeregisterTimer(pCVar27)
                return
            end
            piVar5 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
            piVar5:GetPos()
            r27 = quest:CreateCreature("WillApprenticeMarker", nil --[[missing]], "WillApprentice")
            if piStack_1bc ~= nil then
                -- TODO(native): (**(code **)(*piStack_1bc + 0x118))();
            end
        end
        if quest:GetStateBool("BanditsDefeated") then
            -- LAB_00d609de: (native jump target)
            pCVar17 = ""
            pCVar27 = "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06"
            quest:SetQuestCardObjective("Q_GuildTraining", "", "Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06")
            quest:SetPlayerUsingWillDummies((pCVar33 ~= 0))
            alive = not quest:IsActiveThreadTerminating()
            cVar2 = extraout_AL_88
            -- FLOW_native_label_2: (native jump target)
            if cVar2 == 0 then
                if not quest:GetStateBool("BanditsDefeated") then
                    -- TODO(native): uStack_188 = uStack_188 | 1;
                    cVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
                    if cVar2 then return end  -- TODO(native): goto LAB_00d6158a
                    bVar1 = true
                else
                    -- LAB_00d6158a: (native jump target)
                    bVar1 = false
                end
                if (uStack_188 & 1) ~= 0 then
                    -- TODO(native): uStack_188 = uStack_188 & 0xfffffffe;
                end
                if bVar1 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d61b0a
                    quest:SetStateBool("BanditsDefeated", true)
                end
                cVar2 = me:IsTalkedToByHero()
                if not cVar2 then goto LAB_00d61af3 end
                alive = not quest:IsActiveThreadTerminating()
                if extraout_AL_x00100 == 0 then
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&ppuStack_1fc);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(bVar1)
                    ppVar31 = 0x1
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO")
                    iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar7 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if extraout_AL_x00101 ~= 0 then return end  -- TODO(native): goto LAB_00d61b53
                        iVar7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if extraout_AL_x00102 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        if iVar7 == 1 then
                            if extraout_AL_x00103 == 0 then
                                quest:FadeScreenOut(ppVar31, nil --[[missing]])
                                quest:Pause(nil --[[missing]])
                                pCVar26 = "Data\\Video\\2_guild_split_2_comp.xmv"
                                quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
                                quest:ConfiscateAllHeroWeapons()
                                cVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
                                if cVar2 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if extraout_AL_x00104 ~= 0 then return end  -- TODO(native): goto LAB_00d61b44
                                    quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", nil --[[missing]])
                                end
                                ppVar24 = quest:GetActiveQuestName()
                                quest:DeactivateQuestLater(ppVar24, nil --[[missing]])
                                quest:SetTimeOfDay(nil --[[missing]])
                                quest:ResetPlayerCreatureCombatMultiplier()
                                quest:SetHeroAsTeenager(nil --[[missing]])
                                quest:ChangeHeroHealthBy(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                ppVar24 = quest:GetThingWithScriptName("MeleeApprentice")
                                quest:RemoveThing(ppVar24)
                                piVar9 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE")
                                piVar9:GetPos()
                                -- TODO(native): in_stack_fffffd44 = (CCharString *)amStack_1cc;
                                -- TODO(native): in_stack_fffffd40 = aCStack_b8;
                                r28 = quest:CreateCreature("MeleeApprenticeMarker", nil --[[missing]], "MeleeApprentice")
                                -- LAB_00d61ad8: (native jump target)
                                -- TODO(native): in_stack_fffffd3c = (CCharString *)0x0;
                                -- TODO(native): in_stack_fffffd38 = (CCharString *)0xd61aea;
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                goto LAB_00d61af3
                            end
                        else
                            if extraout_AL_x00103 == 0 then
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffdbc);
                                CVar3 = SUB41(ppVar31,0)
                                fVar14 = quest:GetHealth(r28)
                                fVar15 = _DAT_0122dedc
                                if fVar15 < fVar14 then
                                    CVar29 = 0x0
                                    pCVar33 = 0x1
                                    pCVar27 = 0x0
                                    pCVar26 = 0x0
                                    pcVar30 = "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO"
                                    pCVar6 = quest:GetHero()
                                    r29 = me:Speak(pCVar6, pcVar30, pCVar26, (pCVar27 ~= 0), (pCVar33 ~= 0), (CVar29 ~= 0))
                                    bVar1 = me:IsPerformingScriptTask()
                                    if bVar1 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if extraout_AL_x00105 ~= 0 then return end  -- TODO(native): goto LAB_00d61b44
                                            bVar1 = me:IsPerformingScriptTask()
                                        until not (bVar1)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if extraout_AL_x00106 ~= 0 then goto LAB_00d61b53 end
                                end
                                piVar9 = quest:GetThingWithScriptName("M_GuildmasterMarker")
                                pCVar6 = piVar9:GetPos()
                                me:MoveToPosition(pCVar6, in_stack_fffffd30, in_stack_fffffd34, in_stack_fffffd38, in_stack_fffffd3c)
                                -- TODO(native): goto LAB_00d61ad8
                            end
                        end
                        ::LAB_00d61b53::
                        iVar7 = *piVar5
                        -- LAB_00d61b5a: (native jump target)
                        quest:PauseAllNonScriptedEntities((CVar29 ~= 0))
                    else
                        -- LAB_00d61b44: (native jump target)
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    end
                    goto LAB_00d61b69
                end
            end
            -- TODO(native): goto LAB_00d61b0a
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            quest:ActivateQuest("Q_GuildTrainingWoodsWill")
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", nil --[[missing]])
            quest:SetQuestAsPersistent("Q_GuildTrainingWoodsWill", nil --[[missing]])
            quest:SetQuestCardObjective("Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_01", "GuildWoods", "")
            -- TODO(native): goto LAB_00d609de
        end
    end
    goto LAB_00d61b69
    ::FLOW_native_label_3::
    if not bVar1 then goto LAB_00d610c4 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then return end  -- TODO(native): goto LAB_00d6132d
    bVar1 = me:IsPerformingScriptTask()
    uVar22 = uVar28
    aVar25 = SUB41(pCVar26,0)
    goto FLOW_native_label_3
    ::LAB_00d610c4::
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        -- LAB_00d610d3: (native jump target)
        quest:FadeScreenOut(nil --[[missing]], nil --[[missing]])
        quest:Pause(nil --[[missing]])
        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
        quest:ConfiscateAllHeroWeapons()
        cVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d61563
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", nil --[[missing]])
        end
        quest:SetTimeOfDay(nil --[[missing]])
        quest:ResetPlayerCreatureCombatMultiplier()
        quest:SetHeroAsTeenager(nil --[[missing]])
        quest:ChangeHeroHealthBy(nil --[[missing]], nil --[[missing]], nil --[[missing]])
        ppVar24 = quest:GetThingWithScriptName("MeleeApprentice")
        quest:RemoveThing(ppVar24)
        piVar9 = quest:GetThingWithScriptName("HERO")
        uVar28 = piVar9:GetPos()
        r30 = quest:CreateCreature(ppVar24, uVar28, "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE")
        piVar9 = quest:GetThingWithScriptName("WillApprentice")
        cVar2 = (piVar9 ~= nil and piVar9:IsAlive())
        if cVar2 then
            -- LAB_00d6144d: (native jump target)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
            -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)&stack0xfffffd70,(CFontBank *)ppVar24,pCVar26,pCVar37,pCVar18, pCVar19,(ulong)pCVar33,(float)pCVar27,(bool)aVar25,(bool)uVar22);
            quest:SetPlayerUsingWillDummies(nil --[[missing]])
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            quest:SetStateInt("TutorialState", 4)
            quest:SetStateBool("TestFinished", true)
            r31 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(r31, nil --[[missing]])
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
            until not (alive)
            -- TODO(native): goto LAB_00d61563
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            piVar9 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
            piVar9:GetPos()
            r32 = quest:CreateCreature("WillApprenticeMarker", nil --[[missing]], "WillApprentice")
            if piStack_1f8 ~= nil then
                -- TODO(native): (**(code **)(*piStack_1f8 + 0x118))();
            end
            -- TODO(native): goto LAB_00d6144d
        end
        -- LAB_00d6132d: (native jump target)
        -- LAB_00d61342: (native jump target)
        quest:PauseAllNonScriptedEntities(nil --[[missing]])
    else
        -- LAB_00d61563: (native jump target)
        -- LAB_00d61578: (native jump target)
        quest:PauseAllNonScriptedEntities(nil --[[missing]])
    end
    -- TODO(native): LTextTreeWalkThrough__Dtor();
    goto LAB_00d6135b
    ::FLOW_native_label_4::
    if not bVar1 then goto LAB_00d60bd4 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then return end  -- TODO(native): goto LAB_00d60b19
    bVar1 = me:IsPerformingScriptTask()
    uVar22 = uVar28
    aVar25 = SUB41(pCVar26,0)
    goto FLOW_native_label_4
    ::LAB_00d61af3::
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    cVar2 = extraout_AL_x00107
    -- TODO(native): goto FLOW_native_label_2
    ::LAB_00d60bd4::
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        -- LAB_00d60be3: (native jump target)
        quest:FadeScreenOut(nil --[[missing]], nil --[[missing]])
        quest:Pause(nil --[[missing]])
        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
        quest:ConfiscateAllHeroWeapons()
        cVar2 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d61004 end
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", nil --[[missing]])
        end
        quest:SetTimeOfDay(nil --[[missing]])
        quest:ResetPlayerCreatureCombatMultiplier()
        quest:SetHeroAsTeenager(nil --[[missing]])
        quest:ChangeHeroHealthBy(nil --[[missing]], nil --[[missing]], nil --[[missing]])
        ppVar24 = quest:GetThingWithScriptName("MeleeApprentice")
        quest:RemoveThing(ppVar24)
        piVar9 = quest:GetThingWithScriptName("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE")
        uVar28 = piVar9:GetPos()
        r33 = quest:CreateCreature(ppVar24, uVar28, "MeleeApprenticeMarker")
        piVar9 = quest:GetThingWithScriptName("WillApprentice")
        cVar2 = (piVar9 ~= nil and piVar9:IsAlive())
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                piVar9 = quest:GetThingWithScriptName("CREATURE_GUILD_EVIL_APPRENTICE_MALE")
                piVar9:GetPos()
                r34 = quest:CreateCreature("WillApprenticeMarker", nil --[[missing]], "WillApprentice")
                if piStack_1e8 ~= nil then
                    -- TODO(native): (**(code **)(*piStack_1e8 + 0x118))();
                end
                goto LAB_00d60ef5
            end
            -- LAB_00d60b19: (native jump target)
            -- LAB_00d60b31: (native jump target)
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            goto LAB_00d6135b
        end
        ::LAB_00d60ef5::
        pCVar21 = ""
        quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "Q_GuildTraining")
        -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)&stack0xfffffd80,(CFontBank *)ppVar24,pCVar37,pCVar18, (CCharString *)pCVar21,(CCharString *)paVar23,(ulong)pCVar27,(float)paVar11, (bool)aVar25,(bool)uVar22);
        quest:SetPlayerUsingWillDummies(nil --[[missing]])
        quest:SetMasterGameState("HeroTakingGuildTest", false)
        quest:SetStateInt("TutorialState", 4)
        quest:SetStateBool("TestFinished", true)
        r35 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(r35, nil --[[missing]])
        repeat
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
        until not (alive)
    end
    ::LAB_00d61004::
    ::LAB_00d6101c::
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    ::LAB_00d6135b::
    -- TODO(native): StdMap_Destroy_API();
    ::LAB_00d61370::
    ::LAB_00d61379::
    ::LAB_00d6138b::
    ::LAB_00d61b69::
    ::LAB_00d61b7b::
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

