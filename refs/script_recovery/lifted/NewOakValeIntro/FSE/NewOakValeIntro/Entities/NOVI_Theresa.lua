-- Generated native draft: NOVI_Theresa. Review coverage report before use.
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
    __native_entity_state:SetStateBool("DoneIntro", false)
    __native_entity_state:SetStateBool("spoken_already", false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
end

function Main(quest, me)
    local CVar24, CVar6, __native_condition_1, __native_condition_2, bVar4, cVar28, cVar5, fVar17, fVar2, fVar26, iVar12, pCVar13, pCVar18, pCVar20, pCVar21, pCVar22, pCVar23, pCVar27, pCVar8, paVar25, pcVar19, ppVar9, r1, r10, r11, r12, r13, r14, r15, r2, r3, r4, r5, r6, r7, r8, r9, uVar11, uVar3, uVar30, uVar7
    local alive = true
    -- TODO(native): map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> amStack_13c [4];
    -- TODO(native): CCharString aCStack_138 [4];
    -- TODO(native): undefined **ppuStack_134;
    -- TODO(native): CScriptThing aCStack_130 [4];
    -- TODO(native): CCharString aCStack_12c [8];
    -- TODO(native): allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_124 [4];
    -- TODO(native): CScriptGameResourceObjectMovieBase aCStack_120 [4];
    -- TODO(native): allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_11c [4];
    -- TODO(native): CCarriedReadableDef aCStack_118 [4];
    -- TODO(native): allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_114 [4];
    -- TODO(native): allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_110 [4];
    -- TODO(native): CCharString aCStack_10c [4];
    -- TODO(native): CCharString aCStack_108 [4];
    -- TODO(native): undefined4 uStack_104;
    -- TODO(native): undefined4 uStack_100;
    -- TODO(native): undefined4 uStack_fc;
    -- TODO(native): CScriptGameResourceObjectScriptedThingBase aCStack_f8 [4];
    -- TODO(native): CCarriedReadableDef aCStack_f4 [4];
    -- TODO(native): vector<CConversation,std::allocator<CConversation>_> avStack_f0 [4];
    -- TODO(native): vector<CScriptThing,std::allocator<CScriptThing>_> avStack_ec [4];
    -- TODO(native): CCarriedReadableDef aCStack_e8 [4];
    -- TODO(native): undefined4 uStack_e4;
    -- TODO(native): undefined4 uStack_e0;
    -- TODO(native): undefined4 uStack_dc;
    -- TODO(native): undefined4 uStack_d8;
    -- TODO(native): undefined4 uStack_d4;
    -- TODO(native): allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_cc [4];
    -- TODO(native): CScriptGameResourceObjectScriptedThingBase aCStack_c8 [12];
    -- TODO(native): CCarriedReadableDef aCStack_bc [4];
    -- TODO(native): pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> apStack_b8 [20];
    -- TODO(native): CCarriedReadableDef aCStack_a4 [16];
    -- TODO(native): C3DClothPrimitive aCStack_94 [16];
    -- TODO(native): CCarriedReadableDef aCStack_84 [20];
    -- TODO(native): C3DClothPrimitive aCStack_70 [8];
    -- TODO(native): C3DClothPrimitive aCStack_68 [36];
    -- TODO(native): C3DClothPrimitive aCStack_44 [12];
    -- TODO(native): C3DClothPrimitive aCStack_38 [56];
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    -- TODO(native): uStack_1ec = uStack_1ec & 0xffffff;
    -- TODO(native): pppuVar29 = &ppuStack_1f0;
    r1 = quest:GetThingWithScriptName("M_TriggerOutro")
    alive = not quest:IsActiveThreadTerminating()
    cVar5 = not alive
    repeat
        if cVar5 then
            -- LAB_00dbaebb: (native jump target)
            return
        end
        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_1f0);
        if bVar4 then
        end
        uVar11 = 0
        cVar5 = me:AcquireControl(4)
        while true do
            cVar28 = (uVar11 >> 0x18)
            if not (not cVar5) then break end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                if true then return end  -- TODO(native): goto LAB_00dbaebb
                -- TODO(native): goto LAB_00dbaeb3
            end
            uVar11 = 0
            cVar5 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00dbb2e8
        if not __native_entity_state:GetStateBool("DoneIntro") then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00dbb2e8
            while true do
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00dbb2e8
                me:PlayCombatAnimation("SKIP", true, false)
                fVar26 = 5.0
                pCVar8 = quest:GetHero()
                bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar26)
                while true do
                    __native_condition_1 = not bVar4
                    if __native_condition_1 then
                        bVar4 = me:IsPerformingScriptTask()
                        __native_condition_1 = bVar4
                    end
                    if not __native_condition_1 then break end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00dbb2e8
                    fVar26 = 5.0
                    pCVar8 = quest:GetHero()
                    bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar26)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00dbb2e8
                fVar26 = 5.0
                pCVar8 = quest:GetHero()
                bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar26)
                if bVar4 then break end
                alive = quest:NewScriptFrame(me)
            end
            alive = not quest:IsActiveThreadTerminating()
            if (not alive) or (function() alive = not quest:IsActiveThreadTerminating(); return not alive end)() then return end  -- TODO(native): goto LAB_00dbb2e8
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_bc);
            ppVar9 = quest:GetHero()
            me:AcquireControl(4)
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar10 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_13c,aCStack_1a0);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar10,pCVar18);
            -- TODO(native): pCVar10 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_13c,aCStack_144);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar10,pCVar18);
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_154);
            pCVar20 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities((uVar11 ~= 0))
            quest:FixMovieSequenceCamera(false)
            -- TODO(native): in_stack_fffffdc0 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
            -- TODO(native): RunCutsceneMacro_Func(0,0);
            uVar11 = quest:GetHero()
            pCVar20 = "OBJECT_CHOCOLATE_BOX_UNGIVEABLE"
            -- TODO(native): IsObjectInThingsPossession is not a ForgeFSE binding
            quest:IsObjectInThingsPossession(pCVar20, uVar11)
            if not cVar5 then goto LAB_00db9e86 end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00dbaf05: (native jump target)
                -- TODO(native): StdMap_Destroy_API();
                -- TODO(native): goto LAB_00dbb2e8
            end
            quest:GiveHeroYesNoQuestion("TEXT_QST_048_GIVE_CHOCOLATE_BOX", "TEXT_OBJECT_HERO_ANSWER_YES", "", "TEXT_OBJECT_HERO_ANSWER_NO", true)
            iVar12 = quest:MsgIsQuestionAnsweredYesOrNo()
            while iVar12 < 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00dbaefa
                iVar12 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): goto LAB_00dbaf05
            end
            if iVar12 == 1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00dbaefa: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): goto LAB_00dbaf05
                end
                r2 = quest:GetAllThingsWithScriptName("NOVI_Guard")
                -- TODO(native): KillAllThingsInVector (*(CGameScriptInterfaceBase **)(param_1 + 4), (vector<CScriptThing,std::allocator<CScriptThing>_> *)&uStack_e4,false);
                -- TODO(native): StdMap_Construct_API();
                -- TODO(native): pCVar10 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xfffffdec,aCStack_1b0);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar10,pCVar18);
                -- TODO(native): pCVar10 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xfffffdec,(CCharString *)&piStack_1e4);
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar10,pCVar18);
                ppVar9 = 0x0
                -- TODO(native): RunCutsceneMacro_Func(0,0,0,1);
                quest:SetStateBool("GivenTheresaChocs", true)
                quest:TakeObjectFromHero("OBJECT_CHOCOLATE_BOX_UNGIVEABLE")
                ppVar9 = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(ppVar9, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_05", "", "")
                quest:ClearThingHasInformation(me)
                -- TODO(native): StdMap_Destroy_API();
            end
            ::LAB_00db9e86::
            quest:FixMovieSequenceCamera(false)
            __native_entity_state:SetStateBool("DoneIntro", true)
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): StdMap_Destroy_API();
        else
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00dbb2e8
            uVar3 = "OBJECT_CHOCOLATE_BOX_UNGIVEABLE"
            -- TODO(native): uStack_1ec = uStack_1ec | 1;
            cVar5 = me:IsTalkedToByHero()
            if not cVar5 then
                -- LAB_00db9f53: (native jump target)
            else
                -- TODO(native): uStack_1ec = uVar3 | 3;
                r3 = quest:GetHero()
                -- TODO(native): IsObjectInThingsPossession is not a ForgeFSE binding
                quest:IsObjectInThingsPossession()
                if not cVar5 then return end  -- TODO(native): goto LAB_00db9f53
            end
            if (uStack_1ec & 2) ~= 0 then
                -- TODO(native): uStack_1ec = uStack_1ec & 0xfffffffd;
            end
            if (uStack_1ec & 1) ~= 0 then
                -- TODO(native): uStack_1ec = uStack_1ec & 0xfffffffe;
            end
            if (unaff_EDI >> 0x10) == 0 then
                cVar5 = me:MsgIsPresentedWithItem()
                if cVar5 then
                    if pppuVar29 ~= nil then
                        -- TODO(native): lVar14 = CBasicString<char>::Compare ((char *)*pppuVar29,"OBJECT_CHOCOLATE_BOX_UNGIVEABLE");
                        if lVar14 == 0 then goto LAB_00dba5a4 end
                        goto LAB_00dba4c8
                    end
                    iVar12 = 31
                    bVar4 = false
                    if not bVar4 then goto LAB_00dba4c8 end
                    ::LAB_00dba5a4::
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00dbb0d6 end
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_84);
                    pCVar20 = ""
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(bVar4)
                    pCVar20 = "NOVI_Guard"
                    r4 = quest:GetAllThingsWithScriptName("NOVI_Guard")
                    -- TODO(native): KillAllThingsInVector(*(CGameScriptInterfaceBase **)(param_1 + 4),avStack_ec,false);
                    -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&pppuStack_210);
                    fVar17 = quest:GetHealth(r3)
                    fVar2 = _DAT_0122dedc
                    if fVar2 < fVar17 then
                        bVar4 = false
                        pCVar22 = 0x1
                        pCVar21 = 0x0
                        pCVar20 = 0x0
                        pcVar19 = "TEXT_QST_048_THERESA_HELLO"
                        pCVar13 = quest:GetHero()
                        r5 = me:Speak(pCVar13, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- TODO(native): goto LAB_00dbb2e8
                                end
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): goto LAB_00dbb2e8
                        end
                    end
                    quest:SetStateBool("GivenTheresaChocs", true)
                    quest:TakeObjectFromHero("OBJECT_CHOCOLATE_BOX_UNGIVEABLE")
                    -- TODO(native): in_stack_fffffdc0 = apStack_b8;
                    ppVar9 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(ppVar9, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_05", "", "")
                    quest:ClearThingHasInformation(me)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): goto LAB_00dba3e3
                end
                ::LAB_00dba4c8::
                cVar5 = pCVar13:MsgIsPresentedWithItem()
                __native_condition_2 = not cVar5
                if not __native_condition_2 then
                    cVar5 = g_PresentedItemName ~= "OBJECT_CHOCOLATE_BOX_UNGIVEABLE"
                    __native_condition_2 = not cVar5
                end
                if __native_condition_2 then
                    cVar5 = pCVar13:IsTalkedToByHero()
                    if not cVar5 then
                        -- TODO(native): uStack_1ec = uStack_1ec | 4;
                        cVar5 = pCVar13:MsgIsHitByHero()
                        if not cVar5 then
                            -- TODO(native): uStack_1ec = uVar3 | 0xc;
                            cVar5 = pCVar13:MsgIsHitByAnySpecialAbilityFromHero()
                            if cVar5 then
                                -- TODO(native): uStack_1ec = uVar3 | 0x1c;
                                cVar5 = pCVar13:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                if not cVar5 then return end  -- TODO(native): goto LAB_00dbac3f
                            end
                            uVar30 = 0
                        else
                            -- LAB_00dbac3f: (native jump target)
                            uVar30 = 0x10000
                        end
                        if (uStack_1ec & 0x10) ~= 0 then
                            -- TODO(native): uStack_1ec = uStack_1ec & 0xffffffef;
                        end
                        if (uStack_1ec & 8) ~= 0 then
                            -- TODO(native): uStack_1ec = uStack_1ec & 0xfffffff7;
                        end
                        if (uStack_1ec & 4) ~= 0 then
                            -- TODO(native): uStack_1ec = uStack_1ec & 0xfffffffb;
                        end
                        if ((uVar30 & 0xffffff) >> 0x10) == 0 then
                            if bVar31 ~= 0 then return end  -- TODO(native): goto LAB_00dba3e3
                            bVar4 = me:IsPerformingScriptTask()
                            if not bVar4 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00dbb0d6 end
                                me:PlayCombatAnimation("SKIP", true, false)
                            end
                            goto LAB_00dba406
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            r6 = quest:GetHero()
                            quest:EntitySetThingAsAllyOfThing(r6, r1)
                            r7 = quest:GetHero()
                            quest:EntitySetThingAsAllyOfThing(r7, nil --[[missing]])
                            require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&uStack_dc);
                            pCVar20 = ""
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities((uVar30 ~= 0))
                            -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffde8);
                            fVar17 = quest:GetHealth(nil --[[missing]])
                            fVar2 = _DAT_0122dedc
                            if fVar2 < fVar17 then
                                CVar6 = 0x0
                                pCVar21 = 0x1
                                -- TODO(native): in_stack_fffffdc0 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0x0;
                                pCVar20 = 0x0
                                pcVar19 = "TEXT_QST_048_THERESA_DONT_HIT"
                                pCVar13 = quest:GetHero()
                                r8 = me:Speak(pCVar13, pcVar19, pCVar20, in_stack_fffffdc0, (pCVar21 ~= 0), (CVar6 ~= 0))
                                bVar4 = me:IsPerformingScriptTask()
                                if bVar4 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            quest:PauseAllNonScriptedEntities((CVar6 ~= 0))
                                            -- TODO(native): goto LAB_00dbb0ca
                                        end
                                        bVar4 = me:IsPerformingScriptTask()
                                    until not (bVar4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- LAB_00dbb0ca: (native jump target)
                                    -- TODO(native): goto LAB_00dbb0d6
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aaStack_cc;
                            goto LAB_00dba3d6
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_118);
                            paVar25 = ""
                            quest:StartMovieSequence()
                            uVar7 = 1
                            quest:PauseAllNonScriptedEntities((uVar7 ~= 0))
                            if cVar28 == 0 then
                                if not __native_entity_state:GetStateBool("spoken_already") then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        -- LAB_00dbaac3: (native jump target)
                                        quest:PauseAllNonScriptedEntities(false)
                                        -- TODO(native): goto LAB_00dbb2e8
                                    end
                                    __native_entity_state:SetStateBool("spoken_already", true)
                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffdf8);
                                    fVar17 = quest:GetHealth(nil --[[missing]])
                                    fVar2 = _DAT_0122dedc
                                    if fVar2 < fVar17 then
                                        CVar6 = 0x0
                                        pCVar22 = 0x1
                                        pCVar21 = 0x0
                                        pCVar20 = 0x0
                                        pcVar19 = "TEXT_QST_048_THERESA_GET_PRESENT"
                                        pCVar13 = quest:GetHero()
                                        r9 = me:Speak(pCVar13, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), (CVar6 ~= 0))
                                        bVar4 = me:IsPerformingScriptTask()
                                        if bVar4 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then return end  -- TODO(native): goto LAB_00dbb08a
                                                bVar4 = me:IsPerformingScriptTask()
                                            until not (bVar4)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00dbaac3
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dbb08a
                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffdf8);
                                    fVar17 = quest:GetHealth(nil --[[missing]])
                                    fVar2 = _DAT_0122dedc
                                    if fVar2 < fVar17 then
                                        CVar6 = 0x0
                                        pCVar22 = 0x1
                                        pCVar21 = 0x0
                                        pCVar20 = 0x0
                                        pcVar19 = "TEXT_QST_048_THERESA_REALLY_GET_PRESENT"
                                        pCVar13 = quest:GetHero()
                                        r10 = me:Speak(pCVar13, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), (CVar6 ~= 0))
                                        bVar4 = me:IsPerformingScriptTask()
                                        if bVar4 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then return end  -- TODO(native): goto LAB_00dbaac3
                                                bVar4 = me:IsPerformingScriptTask()
                                            until not (bVar4)
                                        end
                                        -- TODO(native): goto LAB_00dbab8c
                                    end
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    -- LAB_00dbb08a: (native jump target)
                                    quest:PauseAllNonScriptedEntities((CVar6 ~= 0))
                                    -- TODO(native): goto LAB_00dbb2e8
                                end
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffdf8);
                                fVar17 = quest:GetHealth(nil --[[missing]])
                                fVar2 = _DAT_0122dedc
                                if fVar2 < fVar17 then
                                    CVar6 = 0x0
                                    pCVar22 = 0x1
                                    pCVar21 = 0x0
                                    pCVar20 = 0x0
                                    pcVar19 = "TEXT_QST_048_THERESA_HELLO"
                                    pCVar13 = quest:GetHero()
                                    r11 = me:Speak(pCVar13, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), (CVar6 ~= 0))
                                    bVar4 = me:IsPerformingScriptTask()
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00dbaac3
                                            bVar4 = me:IsPerformingScriptTask()
                                        until not (bVar4)
                                    end
                                    -- LAB_00dbab8c: (native jump target)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dbb08a
                                end
                            end
                            quest:PauseAllNonScriptedEntities((CVar6 ~= 0))
                            -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aCStack_118;
                            goto LAB_00dba3d6
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aCStack_94);
                        paVar25 = ""
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): CVar6 = (CScriptGameResourceObjectScriptedThingBase) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffdf8);
                        fVar17 = quest:GetHealth(nil --[[missing]])
                        fVar2 = _DAT_0122dedc
                        if fVar2 < fVar17 then
                            CVar24 = 0x0
                            pCVar22 = 0x1
                            pCVar21 = 0x0
                            pCVar20 = 0x0
                            pcVar19 = "TEXT_QST_048_THERESA_BETTER_PRESENT"
                            pCVar13 = quest:GetHero()
                            -- TODO(native): in_stack_fffffdc0 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)0xdba84d;
                            r12 = me:Speak(pCVar13, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), (CVar24 ~= 0))
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities((CVar24 ~= 0))
                                        -- TODO(native): goto LAB_00dbb2e8
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                -- TODO(native): goto LAB_00dbb2e8
                            end
                        end
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aCStack_94;
                        goto LAB_00dba3d6
                    end
                end
                ::LAB_00dbb0d6::
                -- LAB_00dbb2e8: (native jump target)
                return
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00dbb0d6
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_f4);
            pCVar20 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            pCVar20 = "TEXT_QST_048_GIVE_CHOCOLATE_BOX"
            quest:GiveHeroYesNoQuestion("TEXT_QST_048_GIVE_CHOCOLATE_BOX", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
            iVar12 = quest:MsgIsQuestionAnsweredYesOrNo()
            while iVar12 < 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00dbaf2e
                iVar12 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00dbaf53: (native jump target)
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                -- TODO(native): goto LAB_00dbb2e8
            end
            alive = not quest:IsActiveThreadTerminating()
            if iVar12 == 1 then
                if not alive then
                    -- LAB_00dbaf2e: (native jump target)
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    -- TODO(native): goto LAB_00dbb2e8
                end
                pCVar23 = "NOVI_Guard"
                r13 = quest:GetAllThingsWithScriptName("NOVI_Guard")
                -- TODO(native): KillAllThingsInVector (*(CGameScriptInterfaceBase **)(param_1 + 4), (vector<CScriptThing,std::allocator<CScriptThing>_> *)aCStack_10c,false);
                -- TODO(native): in_stack_fffffdc0 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffddc);
                fVar17 = quest:GetHealth(nil --[[missing]])
                fVar2 = _DAT_0122dedc
                if fVar2 < fVar17 then
                    bVar4 = false
                    pCVar22 = 0x1
                    pCVar21 = 0x0
                    pCVar20 = 0x0
                    pcVar19 = "TEXT_QST_048_THERESA_HELLO"
                    pCVar13 = quest:GetHero()
                    r14 = me:Speak(pCVar13, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                    bVar4 = me:IsPerformingScriptTask()
                    if bVar4 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                -- TODO(native): goto LAB_00dbaf84
                            end
                            bVar4 = me:IsPerformingScriptTask()
                        until not (bVar4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        -- LAB_00dbafb6: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): goto LAB_00dbb2e8
                    end
                end
                quest:SetStateBool("GivenTheresaChocs", true)
                pCVar20 = "OBJECT_CHOCOLATE_BOX_UNGIVEABLE"
                quest:TakeObjectFromHero("OBJECT_CHOCOLATE_BOX_UNGIVEABLE")
                ppVar9 = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(ppVar9, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_05", "", "")
                quest:ClearThingHasInformation(me)
            else
                if not alive then return end  -- TODO(native): goto LAB_00dbaf53
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffde4);
                fVar17 = quest:GetHealth(nil --[[missing]])
                fVar2 = _DAT_0122dedc
                if fVar2 < fVar17 then
                    bVar4 = false
                    pCVar22 = 0x1
                    pCVar21 = 0x0
                    pCVar20 = 0x0
                    pcVar19 = "TEXT_QST_048_THERESA_REALLY_GET_PRESENT"
                    pCVar13 = quest:GetHero()
                    r15 = me:Speak(pCVar13, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar4)
                    bVar4 = me:IsPerformingScriptTask()
                    if bVar4 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00dbafb6
                            bVar4 = me:IsPerformingScriptTask()
                        until not (bVar4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        -- LAB_00dbaf84: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): goto LAB_00dbb2e8
                    end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aCStack_f4;
            ::LAB_00dba3d6::
            if (unaff_EDI >> 0x18) ~= 0 then
                -- LAB_00dba3e3: (native jump target)
                fVar26 = 2.0
                pCVar13 = quest:GetHero()
                bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar13, r1, fVar26)
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        quest:DisplayQuestInfo(false)
                        quest:RemoveQuestInfoElement(0)
                        quest:RemoveQuestInfoElement(0)
                        quest:RemoveQuestInfoElement(nil --[[missing]])
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_a4);
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_e8);
                        ppVar9 = quest:GetHero()
                        me:AcquireControl(4)
                        -- TODO(native): StdMap_Construct_API();
                        -- TODO(native): pCVar10 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xfffffe00,(CCharString *)&stack0xfffffdfc);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar10, (CScriptGameResourceObjectScriptedThingBase *)pCVar23);
                        -- TODO(native): pCVar10 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&stack0xfffffe00,(CCharString *)&stack0xfffffdfc);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar10,pCVar18);
                        ppVar9 = 0x0
                        -- TODO(native): RunCutsceneMacro_Func(0,0,0,1);
                        quest:PlayAVIMovie("Data\\\\Video\\\\1_raid_on_oak_vale_comp.xmv")
                        quest:FadeScreenOut(0x3f000000, 0)
                        quest:OverrideMusic(0x19, false, false)
                        quest:SetStateBool("AttackOver", true)
                        -- TODO(native): StdMap_Destroy_API();
                        quest:PauseAllNonScriptedEntities(false)
                        alive = not quest:IsActiveThreadTerminating()
                        -- TODO(native): goto LAB_00dbb2e8
                    end
                    -- TODO(native): goto LAB_00dbb0d6
                end
            end
            ::LAB_00dba406::
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cVar5 = not alive
    until false
end

