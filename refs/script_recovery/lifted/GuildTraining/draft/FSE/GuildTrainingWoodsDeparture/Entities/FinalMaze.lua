-- Generated native draft: FinalMaze. Review coverage report before use.
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
    local CVar2, CVar35, CVar36, __native_condition_1, __native_condition_2, __native_condition_3, aVar24, bVar4, cVar34, cVar5, fVar17, fVar3, iVar11, pCStack_d4, pCVar1, pCVar12, pCVar20, pCVar21, pCVar22, pCVar23, pCVar27, pCVar28, pCVar30, pCVar31, pCVar33, paVar13, pcVar19, piVar29, ppVar7, puVar10, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r3, r30, r31, r4, r5, r6, r7, r8, r9, uVar15, uVar16, uVar18, uVar25, uVar6, uVar9
    local alive = true
    uVar15 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_70);
    if bVar4 then
    end
    -- TODO(native): pppuVar32 = &ppuStack_70;
    cVar5 = me:AcquireControl(4)
    while not cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00d664b9 end
        cVar5 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00d664b9 end
    __native_entity_state:SetStateBool("NotFighting", true)
    __native_entity_state:SetStateBool("NotBeaten", true)
    __native_entity_state:SetStateInt("BeenHit", 0)
    quest:EntitySetAsKillable(nil --[[missing]], false)
    quest:EntitySetInFaction(nil --[[missing]], "FACTION_HERO")
    quest:EntitySetAllowBossPhaseChanges(nil --[[missing]], false)
    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_60);
    if bVar4 then
    end
    ppVar7 = quest:GetHero()
    cVar5 = me:AcquireControl(4)
    while not cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00d649a2
        pCStack_d4 = quest:GetHero()
        cVar5 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        -- LAB_00d649a2: (native jump target)
        return
    end
    -- TODO(native): StdMap_Construct_API();
    -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_50,(CCharString *)&stack0xffffff50);
    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8, (CScriptGameResourceObjectScriptedThingBase *)pCVar27);
    -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_50,(CCharString *)&stack0xffffff50);
    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar28);
    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_44);
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(false)
    uVar25 = 0
    ppVar7 = 0x0
    -- TODO(native): RunCutsceneMacro_Func();
    quest:FixMovieSequenceCamera((ppVar7 ~= 0))
    quest:PauseAllNonScriptedEntities((uVar25 ~= 0))
    -- TODO(native): StdMap_Destroy_API();
    quest:EntitySetInFaction(nil --[[missing]], "FACTION_MONSTERS")
    r1 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 0, 0)
    quest:DisplayQuestInfo(true)
    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff1c);
    if bVar4 then
    end
    quest:EntitySetBossPhase(me, 0)
    uVar9 = quest:GetHero()
    quest:EntitySetAsDamageable(uVar9, (uVar15 ~= 0))
    quest:UpdateQuestInfoCounter(uVar25, __native_entity_state:GetStateInt("BeenHit"), -1)
    -- TODO(native): CTimer::CTimer((CTimer *)&pCStack_110);
    quest:SetTimer(pCStack_110, 0)
    puVar10 = me:GetPos()
    r2 = quest:EntityWillTeleportToArea(me, puVar10.x, puVar10.y, puVar10.z)
    quest:CacheMusicSet(0x2f)
    CVar2 = __native_entity_state:GetStateBool("NotBeaten")
    while CVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00d664b0 end
        uVar16 = uVar15 | 3
        cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD")
        if not cVar5 then
            uVar16 = uVar15 | 0xf
            cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA")
            cVar34 = 0
            if cVar5 then return end  -- TODO(native): goto LAB_00d64cba
        else
            -- LAB_00d64cba: (native jump target)
            cVar34 = '\x01'
        end
        if (uVar16 & 8) ~= 0 then
            uVar16 = uVar16 & 0xfffffff7
        end
        if (uVar16 & 4) ~= 0 then
            uVar16 = uVar16 & 0xfffffffb
        end
        if (uVar16 & 2) ~= 0 then
            uVar16 = uVar16 & 0xfffffffd
        end
        if (uVar16 & 1) ~= 0 then
            uVar16 = uVar16 & 0xfffffffe
        end
        if cVar34 == 0 then
            uVar15 = uVar16 | 0x30
            cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW")
            if not cVar5 then
                uVar15 = uVar16 | 0xf0
                cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW")
                if cVar5 then return end  -- TODO(native): goto LAB_00d64f41
                -- LAB_00d64f56: (native jump target)
                cVar34 = 0
            else
                -- LAB_00d64f41: (native jump target)
                cVar5 = quest:IsConversationActive(nil --[[missing]])
                cVar34 = '\x01'
                if cVar5 then return end  -- TODO(native): goto LAB_00d64f56
            end
            if uVar15 < 0 then
                uVar15 = uVar15 & 0xffffff7f
            end
            if (uVar15 & 0x40) ~= 0 then
                uVar15 = uVar15 & 0xffffffbf
            end
            if (uVar15 & 0x20) ~= 0 then
                uVar15 = uVar15 & 0xffffffdf
            end
            if (uVar15 & 0x10) ~= 0 then
                uVar15 = uVar15 & 0xffffffef
            end
            if cVar34 == 0 then
                cVar5 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                if cVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d664b0 end
                    cVar5 = quest:IsConversationActive(nil --[[missing]])
                    if not cVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d664b0 end
                        r3 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                        r4 = quest:GetHero()
                        quest:AddPersonToConversation(nil --[[missing]], r4)
                        r5 = quest:GetHero()
                        quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_LIGHTNING", r5, nil --[[missing]])
                        uVar15 = uVar15
                    end
                    quest:ModifyThingHealth(nil --[[missing]], nil --[[missing]])
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d664b0 end
                ppVar7 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                r6 = quest:GetHero()
                quest:AddPersonToConversation(nil --[[missing]], r6)
                r7 = quest:GetHero()
                quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_BOW", r7, nil --[[missing]])
                quest:ModifyThingHealth(me, nil --[[missing]])
                uVar15 = uVar15
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d664b0 end
            iVar11 = __native_entity_state:GetStateInt("BeenHit")
            __native_entity_state:SetStateInt("BeenHit", iVar11 + 1)
            if iVar11 + 1 == 7 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d664b0 end
                __native_entity_state:SetStateBool("NotBeaten", false)
                quest:ModifyThingHealth(nil --[[missing]], nil --[[missing]])
            end
            quest:UpdateQuestInfoCounter(nil --[[missing]], nil --[[missing]], nil --[[missing]])
            iVar11 = quest:GetTimer(nil --[[missing]])
            uVar15 = uVar16
            __native_condition_1 = iVar11 < 1
            if __native_condition_1 then
                cVar5 = quest:IsConversationActive(nil --[[missing]])
                __native_condition_1 = not cVar5
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d664b0 end
                uVar15 = rand()
                uVar15 = uVar15 & 0x80000001
                bVar4 = uVar15 == 0
                if uVar15 < 0 then
                    bVar4 = (uVar15 - 1 | 0xfffffffe) == 0xffffffff
                end
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d664b0 end
                    ppVar7 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                    r8 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r8)
                    r9 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", r9, nil --[[missing]])
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d664b0 end
                    ppVar7 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                    r10 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r10)
                    r11 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", r11, nil --[[missing]])
                end
                quest:SetTimer(nil --[[missing]], nil --[[missing]])
                uVar15 = uVar15
            end
        end
        CVar2 = __native_entity_state:GetStateBool("NotBeaten")
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        quest:RemoveQuestInfoElement(nil --[[missing]])
        quest:DisplayQuestInfo(nil --[[missing]])
        __native_entity_state:SetStateInt("BeenHit", 0)
        __native_entity_state:SetStateBool("NotBeaten", true)
        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&uStack_84);
        if bVar4 then
        end
        cVar5 = me:AcquireControl(4)
        while not cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d664b0 end
            cVar5 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_44);
            pCVar27 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            quest:FixMovieSequenceCamera(nil --[[missing]])
            quest:Pause(nil --[[missing]])
            r12 = quest:GetHero()
            quest:EntitySetAsDrawable(r12, nil --[[missing]])
            piVar29 = 0x0
            -- TODO(native): pCStack_d4 = (CScriptThing *)aCStack_44;
            ppVar7 = quest:GetThingWithScriptName("CAM_RC_MAZE")
            quest:CameraUseCameraPoint(ppVar7, nil --[[missing]], piVar29, nil --[[missing]], nil --[[missing]])
            -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff4c);
            fVar17 = quest:GetHealth(nil --[[missing]])
            fVar3 = _DAT_0122dedc
            if fVar3 < fVar17 then
                aVar24 = 0x0
                pCVar22 = 0x1
                pCVar21 = 0x0
                pCVar20 = 0x0
                pcVar19 = "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SKILL_FIRST"
                pCVar12 = quest:GetHero()
                r13 = me:Speak(pCVar12, pcVar19, pCVar20, (pCVar21 ~= 0), (pCVar22 ~= 0), (aVar24 ~= 0))
                bVar4 = me:IsPerformingScriptTask()
                if bVar4 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d65b83
                        bVar4 = me:IsPerformingScriptTask()
                    until not (bVar4)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00d65b9f: (native jump target)
                    quest:PauseAllNonScriptedEntities((aVar24 ~= 0))
                    goto LAB_00d664b0
                end
            end
            r14 = quest:GetHero()
            quest:EntitySetAsDrawable(r14, nil --[[missing]])
            quest:FixMovieSequenceCamera(nil --[[missing]])
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff38);
            if bVar4 then
            end
            quest:EntitySetBossPhase(nil --[[missing]], nil --[[missing]])
            uVar18 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", nil --[[missing]], nil --[[missing]])
            quest:DisplayQuestInfo(nil --[[missing]])
            -- TODO(native): uStack_10c = *(undefined4 *)(this + 0x20);
            quest:UpdateQuestInfoCounter(-1, nil --[[missing]], nil --[[missing]])
            puVar10 = me:GetPos()
            r15 = quest:EntityWillTeleportToArea(me, puVar10.x, puVar10.y, puVar10.z)
            CVar2 = __native_entity_state:GetStateBool("NotBeaten")
            uVar15 = uVar15
            while CVar2 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d664b0 end
                uVar16 = uVar15 | 0x300
                cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD")
                if not cVar5 then
                    uVar16 = uVar15 | 0xf00
                    cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA")
                    if cVar5 then return end  -- TODO(native): goto LAB_00d654d1
                    -- LAB_00d654e6: (native jump target)
                    cVar34 = 0
                else
                    -- LAB_00d654d1: (native jump target)
                    cVar5 = quest:IsConversationActive(nil --[[missing]])
                    cVar34 = '\x01'
                    if cVar5 then return end  -- TODO(native): goto LAB_00d654e6
                end
                if (uVar16 & 0x800) ~= 0 then
                    uVar16 = uVar16 & 0xfffff7ff
                end
                if (uVar16 & 0x400) ~= 0 then
                    uVar16 = uVar16 & 0xfffffbff
                end
                if (uVar16 & 0x200) ~= 0 then
                    uVar16 = uVar16 & 0xfffffdff
                end
                if (uVar16 & 0x100) ~= 0 then
                    uVar16 = uVar16 & 0xfffffeff
                end
                if cVar34 == 0 then
                    uVar15 = uVar16 | 0x3000
                    cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW")
                    if not cVar5 then
                        uVar15 = uVar16 | 0xf000
                        cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW")
                        cVar34 = 0
                        if cVar5 then return end  -- TODO(native): goto LAB_00d65670
                    else
                        -- LAB_00d65670: (native jump target)
                        cVar34 = '\x01'
                    end
                    if (uVar15 >> 8) < 0 then
                        uVar15 = uVar15 & 0xffff7fff
                    end
                    if (uVar15 & 0x4000) ~= 0 then
                        uVar15 = uVar15 & 0xffffbfff
                    end
                    if (uVar15 & 0x2000) ~= 0 then
                        uVar15 = uVar15 & 0xffffdfff
                    end
                    if (uVar15 & 0x1000) ~= 0 then
                        uVar15 = uVar15 & 0xffffefff
                    end
                    if cVar34 == 0 then
                        cVar5 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                        if cVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d664b0 end
                            cVar5 = quest:IsConversationActive(nil --[[missing]])
                            if not cVar5 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d664b0 end
                                r16 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                r17 = quest:GetHero()
                                quest:AddPersonToConversation(nil --[[missing]], r17)
                                r18 = quest:GetHero()
                                quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_LIGHTNING", r18, nil --[[missing]])
                                uVar15 = uVar15
                            end
                            quest:ModifyThingHealth(nil --[[missing]], nil --[[missing]])
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d664b0 end
                        iVar11 = __native_entity_state:GetStateInt("BeenHit")
                        __native_entity_state:SetStateInt("BeenHit", iVar11 + 1)
                        if iVar11 + 1 == 7 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d664b0 end
                            __native_entity_state:SetStateBool("NotBeaten", false)
                            quest:ModifyThingHealth(nil --[[missing]], nil --[[missing]])
                        end
                        quest:UpdateQuestInfoCounter(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                        iVar11 = quest:GetTimer(nil --[[missing]])
                        __native_condition_2 = iVar11 < 1
                        if __native_condition_2 then
                            cVar5 = quest:IsConversationActive(nil --[[missing]])
                            __native_condition_2 = not cVar5
                        end
                        if __native_condition_2 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d664b0 end
                            uVar16 = rand()
                            uVar16 = uVar16 & 0x80000001
                            bVar4 = uVar16 == 0
                            if uVar16 < 0 then
                                bVar4 = (uVar16 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if bVar4 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d664b0 end
                                ppVar7 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                -- TODO(native): pppuVar32 = (undefined ***)**(int **)(this + 4);
                                -- TODO(native): (*(code *)pppuVar32[0x46])();
                                -- TODO(native): (*(code *)pppuVar32[0x16e])();
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d664b0 end
                                ppVar7 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                -- TODO(native): pppuVar32 = (undefined ***)**(int **)(this + 4);
                                -- TODO(native): (*(code *)pppuVar32[0x46])();
                                -- TODO(native): (*(code *)pppuVar32[0x16e])();
                                -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_80;
                            end
                            quest:SetTimer(nil --[[missing]], nil --[[missing]])
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d664b0 end
                    ppVar7 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                    r19 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r19)
                    r20 = quest:GetHero()
                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_MELEE", r20, nil --[[missing]])
                    quest:ModifyThingHealth(me, nil --[[missing]])
                    uVar15 = uVar15
                end
                CVar2 = __native_entity_state:GetStateBool("NotBeaten")
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                quest:RemoveQuestInfoElement(nil --[[missing]])
                quest:DisplayQuestInfo(nil --[[missing]])
                __native_entity_state:SetStateInt("BeenHit", 0)
                __native_entity_state:SetStateBool("NotBeaten", true)
                -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&uStack_84);
                if bVar4 then
                end
                cVar5 = me:AcquireControl(4)
                while not cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d664b0 end
                    cVar5 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_44);
                    pCVar20 = ""
                    quest:StartMovieSequence()
                    pCVar20 = *(this + 4)
                    -- TODO(native): (**(code **)(*(int *)pCVar20 + 0x5ec))();
                    quest:FixMovieSequenceCamera(nil --[[missing]])
                    quest:Pause(nil --[[missing]])
                    r21 = quest:GetHero()
                    quest:EntitySetAsDrawable(r21, nil --[[missing]])
                    piVar29 = 0x0
                    -- TODO(native): pCStack_d4 = (CScriptThing *)aCStack_44;
                    ppVar7 = quest:GetThingWithScriptName("CAM_RC_MAZE")
                    quest:CameraUseCameraPoint(ppVar7, nil --[[missing]], piVar29, nil --[[missing]], nil --[[missing]])
                    -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff4c);
                    fVar17 = quest:GetHealth(nil --[[missing]])
                    fVar3 = _DAT_0122dedc
                    if fVar3 < fVar17 then
                        aVar24 = 0x0
                        pCVar23 = 0x1
                        pCVar22 = 0x0
                        pCVar21 = 0x0
                        pcVar19 = "TEXT_QST_028_MAZE_WOODS_DEPARTURE_LIGHTNING_FIRST"
                        pCVar12 = quest:GetHero()
                        r22 = me:Speak(pCVar12, pcVar19, pCVar21, (pCVar22 ~= 0), (pCVar23 ~= 0), (aVar24 ~= 0))
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d65b9f
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00d65b83: (native jump target)
                            quest:PauseAllNonScriptedEntities((aVar24 ~= 0))
                            goto LAB_00d664b0
                        end
                    end
                    r23 = quest:GetHero()
                    quest:EntitySetAsDrawable(r23, nil --[[missing]])
                    quest:FixMovieSequenceCamera(nil --[[missing]])
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff38);
                    if bVar4 then
                    end
                    quest:EntitySetBossPhase(nil --[[missing]], nil --[[missing]])
                    uVar18 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", nil --[[missing]], nil --[[missing]])
                    quest:DisplayQuestInfo(nil --[[missing]])
                    -- TODO(native): uStack_10c = *(undefined4 *)(this + 0x20);
                    quest:UpdateQuestInfoCounter(-1, nil --[[missing]], nil --[[missing]])
                    puVar10 = me:GetPos()
                    r24 = quest:EntityWillTeleportToArea(me, puVar10.x, puVar10.y, puVar10.z)
                    CVar2 = __native_entity_state:GetStateBool("NotBeaten")
                    uVar15 = uVar15
                    while CVar2 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d664b0 end
                        uVar16 = uVar15 | 0x30000
                        cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD")
                        if not cVar5 then
                            uVar16 = uVar15 | 0xf0000
                            cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA")
                            if cVar5 then return end  -- TODO(native): goto LAB_00d65d51
                            -- LAB_00d65d66: (native jump target)
                            pCVar20 = (pCVar20 & 0xffffff)
                        else
                            -- LAB_00d65d51: (native jump target)
                            cVar5 = quest:IsConversationActive(nil --[[missing]])
                            pCVar20 = CONCAT13(1,(int3)pCVar20)
                            if cVar5 then return end  -- TODO(native): goto LAB_00d65d66
                        end
                        if (uVar16 & 0x80000) ~= 0 then
                            uVar16 = uVar16 & 0xfff7ffff
                        end
                        if (uVar16 & 0x40000) ~= 0 then
                            uVar16 = uVar16 & 0xfffbffff
                        end
                        if (uVar16 & 0x20000) ~= 0 then
                            uVar16 = uVar16 & 0xfffdffff
                        end
                        if (uVar16 & 0x10000) ~= 0 then
                            uVar16 = uVar16 & 0xfffeffff
                        end
                        if (pCVar20 >> 0x18) == 0 then
                            uVar15 = uVar16 | 0x300000
                            cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW")
                            if not cVar5 then
                                uVar15 = uVar16 | 0xf00000
                                cVar5 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW")
                                if cVar5 then return end  -- TODO(native): goto LAB_00d65ef7
                                -- LAB_00d65f0c: (native jump target)
                                pCVar20 = (pCVar20 & 0xffffff)
                            else
                                -- LAB_00d65ef7: (native jump target)
                                cVar5 = quest:IsConversationActive(nil --[[missing]])
                                pCVar20 = CONCAT13(1,(int3)pCVar20)
                                if cVar5 then return end  -- TODO(native): goto LAB_00d65f0c
                            end
                            if (uVar15 & 0x800000) ~= 0 then
                                uVar15 = uVar15 & 0xff7fffff
                            end
                            if (uVar15 & 0x400000) ~= 0 then
                                uVar15 = uVar15 & 0xffbfffff
                            end
                            if (uVar15 & 0x200000) ~= 0 then
                                uVar15 = uVar15 & 0xffdfffff
                            end
                            if (uVar15 & 0x100000) ~= 0 then
                                uVar15 = uVar15 & 0xffefffff
                            end
                            if (pCVar20 >> 0x18) ~= 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    ppVar7 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                    r25 = quest:GetHero()
                                    quest:AddPersonToConversation(nil --[[missing]], r25)
                                    r26 = quest:GetHero()
                                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_BOW", r26, nil --[[missing]])
                                    -- TODO(native): goto LAB_00d65e4c
                                end
                                goto LAB_00d664b0
                            end
                            uVar6 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                            pCVar20 = CONCAT13(uVar6,(int3)pCVar20)
                            if (pCVar20 >> 0x18) ~= 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d664b0 end
                                iVar11 = __native_entity_state:GetStateInt("BeenHit")
                                __native_entity_state:SetStateInt("BeenHit", iVar11 + 1)
                                if iVar11 + 1 == 7 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00d664b0 end
                                    __native_entity_state:SetStateBool("NotBeaten", false)
                                    quest:ModifyThingHealth(nil --[[missing]], nil --[[missing]])
                                end
                                quest:UpdateQuestInfoCounter(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                iVar11 = quest:GetTimer(nil --[[missing]])
                                __native_condition_3 = iVar11 < 1
                                if __native_condition_3 then
                                    cVar5 = quest:IsConversationActive(nil --[[missing]])
                                    __native_condition_3 = not cVar5
                                end
                                if __native_condition_3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00d664b0 end
                                    uVar16 = rand()
                                    uVar16 = uVar16 & 0x80000001
                                    bVar4 = uVar16 == 0
                                    if uVar16 < 0 then
                                        bVar4 = (uVar16 - 1 | 0xfffffffe) == 0xffffffff
                                    end
                                    if bVar4 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00d664b0 end
                                        ppVar7 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                        -- TODO(native): pppuVar32 = (undefined ***)**(int **)(this + 4);
                                        -- TODO(native): (*(code *)pppuVar32[0x46])();
                                        -- TODO(native): (*(code *)pppuVar32[0x16e])();
                                        -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_80;
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00d664b0 end
                                        ppVar7 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                        -- TODO(native): pppuVar32 = (undefined ***)**(int **)(this + 4);
                                        -- TODO(native): (*(code *)pppuVar32[0x46])();
                                        -- TODO(native): (*(code *)pppuVar32[0x16e])();
                                        -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_5c;
                                    end
                                    quest:SetTimer(nil --[[missing]], nil --[[missing]])
                                end
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d664b0 end
                            ppVar7 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                            r27 = quest:GetHero()
                            quest:AddPersonToConversation(nil --[[missing]], r27)
                            r28 = quest:GetHero()
                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_MELEE", r28, nil --[[missing]])
                            -- LAB_00d65e4c: (native jump target)
                            quest:ModifyThingHealth(me, nil --[[missing]])
                            uVar15 = uVar15
                        end
                        CVar2 = __native_entity_state:GetStateBool("NotBeaten")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        quest:RemoveQuestInfoElement(nil --[[missing]])
                        quest:DisplayQuestInfo(nil --[[missing]])
                        -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&uStack_84);
                        if bVar4 then
                        end
                        cVar5 = me:AcquireControl(4)
                        CVar36 = SUB41(unaff_EBX,0)
                        CVar35 = SUB41(unaff_EBP,0)
                        while not cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d664b0 end
                            cVar5 = me:AcquireControl(4)
                            CVar36 = SUB41(unaff_EBX,0)
                            CVar35 = SUB41(unaff_EBP,0)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            quest:EntitySetInFaction(nil --[[missing]], "FACTION_HERO")
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aCStack_4c);
                            -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_4c);
                            if bVar4 then
                            end
                            r29 = quest:GetHero()
                            cVar5 = me:AcquireControl(4)
                            while not cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d6632b
                                r30 = quest:GetHero()
                                cVar5 = me:AcquireControl(4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                -- TODO(native): StdMap_Construct_API();
                                -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_28,aCStack_80);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8, (CScriptGameResourceObjectScriptedThingBase *)pCVar30);
                                -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_28,aCStack_80);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar28);
                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_1c);
                                pCVar33 = ""
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                quest:FixMovieSequenceCamera(nil --[[missing]])
                                ppVar7 = 0x0
                                -- TODO(native): RunCutsceneMacro_Func();
                                quest:FixMovieSequenceCamera((ppVar7 ~= 0))
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                -- TODO(native): StdMap_Destroy_API();
                                quest:SetStateBool("MissionSucceeded", true)
                                quest:ModifyThingHealth(me, nil --[[missing]])
                                quest:RemoveThing(r30)
                                r31 = quest:GetHero()
                                quest:EntitySetAsDamageable(r31, nil --[[missing]])
                            else
                                -- LAB_00d6632b: (native jump target)
                            end
                        end
                    end
                end
            end
        end
    end
    ::LAB_00d664b0::
    ::LAB_00d664b9::
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

