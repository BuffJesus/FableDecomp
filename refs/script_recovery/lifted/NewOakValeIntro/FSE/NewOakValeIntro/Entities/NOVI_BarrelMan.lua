-- Generated native draft: NOVI_BarrelMan. Review coverage report before use.
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
    -- TODO(native): uStack_20 = *(undefined4 *)(*(int *)(this + 0x14) + 0x108);
    quest:SetTimer(0, nil --[[missing]])
    __native_entity_state:SetStateBool("ComplainedAboutStock", false)
    __native_entity_state:SetStateInt("MyPhase", 0)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, true)
    local puVar3 = me:GetHomePos()
    -- TODO(native): *(undefined4 *)(iVar2 + 0x85) = *puVar3;
    -- TODO(native): *(undefined4 *)(iVar2 + 0x89) = puVar3[1];
    -- TODO(native): *(undefined4 *)(iVar2 + 0x8d) = puVar3[2];
    __native_entity_state:SetStateBool("HeroLetMeDown", false)
    __native_entity_state:SetStateBool("OverheardYet", false)
    quest:EntitySetSightRadius(me, 0x41200000)
end

function Main(quest, me)
    local bVar3, cVar2, fVar19, fVar20, fVar25, iVar11, native_arg_sequence_1, pCVar1, pCVar15, pCVar22, pCVar23, pCVar24, pCVar26, pCVar7, pCVar9, pcVar21, piVar10, piVar8, ppVar12, ppuVar17, ppuVar18, puVar14, puVar28, puVar5, r1, r10, r11, r12, r2, r3, r4, r5, r6, r7, r8, r9, uVar4, uVar6
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    quest:SetCreatureBrain(nil --[[missing]], "BRAIN_PASSIVE_OVERRIDE")
    -- TODO(native): puStack_1bc = aCStack_1b8;
    me:GetHomePos()
    -- TODO(native): uStack_1c4 = *(undefined4 *)(param_1 + 0xc);
    -- TODO(native): piStack_1c0 = *(int **)(param_1 + 0x10);
    if piStack_1c0 ~= nil then
        -- TODO(native): *piStack_1c0 = *piStack_1c0 + 1;
    end
    quest:SetWanderCentrePoint(nil --[[missing]], nil --[[missing]])
    -- TODO(native): ppuStack_1d4 = *(undefined ***)(param_1 + 0xc);
    -- TODO(native): piStack_1d0 = *(int **)(param_1 + 0x10);
    if piStack_1d0 ~= nil then
        -- TODO(native): *piStack_1d0 = *piStack_1d0 + 1;
    end
    quest:SetWanderMinDistance(nil --[[missing]], 0)
    -- TODO(native): ppuStack_1e4 = *(undefined ***)(param_1 + 0xc);
    -- TODO(native): piStack_1e0 = *(int **)(param_1 + 0x10);
    if piStack_1e0 ~= nil then
        -- TODO(native): *piStack_1e0 = *piStack_1e0 + 1;
    end
    quest:SetWanderMaxDistance(nil --[[missing]], 0)
    -- TODO(native): ppuStack_1f4 = *(undefined ***)(param_1 + 0xc);
    -- TODO(native): piStack_1f0 = *(int **)(param_1 + 0x10);
    if piStack_1f0 ~= nil then
        -- TODO(native): *piStack_1f0 = *piStack_1f0 + 1;
    end
    quest:SetScriptingStateGroup(nil --[[missing]], 4)
    r1 = quest:GetThingWithScriptName("M_WHouse_ManStart")
    r2 = quest:GetThingWithScriptName("M_WHouse_GuardPoint")
    alive = not quest:IsActiveThreadTerminating()
    cVar2 = not alive
    repeat
        if cVar2 then
            return
        end
        -- TODO(native): if (*(int *)(param_1 + 0x20) == 0) goto switchD_00db5526_default;
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db6afd end
        -- TODO(native): switch(*(undefined4 *)(param_1 + 0x20)) {
        -- TODO(native): case 1:
        quest:SetTimer(quest:GetStateInt("WatchTimer"), 0x2d)
        quest:EntitySetTargetable(me, false)
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffdfc);
        if bVar3 then
        end
        cVar2 = me:AcquireControl(4)
        while not cVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db6afd end
            cVar2 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db6afd end
        r3 = quest:GetThingWithScriptName("M_BarrelManWalkOff")
        if piStack_158 == nil then
        else
            puVar5 = (**(*piStack_158 + 0x18))()
        end
        -- TODO(native): uStack_150 = *puVar5;
        -- TODO(native): ppuStack_14c = (undefined **)puVar5[1];
        fVar25 = 2.0
        -- TODO(native): uStack_148 = puVar5[2];
        -- TODO(native): bVar3 = IsDistanceFromThingToPositionOver(pCVar7,(C3DVector *)&uStack_150,fVar25);
        bVar3 = nil --[[unresolved native result]]
        if bVar3 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00db6a0e: (native jump target)
                    goto LAB_00db6afd
                end
                me:MoveToPosition(nil --[[missing]], 0x0, 0x1, false, false)
                bVar3 = me:IsPerformingScriptTask()
                if bVar3 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00db6a0e
                        bVar3 = me:IsPerformingScriptTask()
                    until not (bVar3)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db6a0e
                fVar25 = 2.0
                -- TODO(native): bVar3 = IsDistanceFromThingToPositionOver(pCVar7,(C3DVector *)&uStack_150,fVar25);
                bVar3 = nil --[[unresolved native result]]
            until not (bVar3)
        end
        alive = not quest:IsActiveThreadTerminating()
        -- TODO(native): this = aCStack_15c;
        if not alive then
            -- LAB_00db6a32: (native jump target)
            goto LAB_00db6afd
        end
        __native_entity_state:SetStateInt("MyPhase", 2)
        break
        -- TODO(native): case 2:
        iVar11 = quest:GetTimer(quest:GetStateInt("WatchTimer"))
        while iVar11 ~= 0xf do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db6afd end
            iVar11 = quest:GetTimer(quest:GetStateInt("WatchTimer"))
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db6afd end
        r4 = quest:GetThingWithScriptName("M_BarrelManWalkOff")
        r5 = quest:GetThingWithScriptName("M_BarrelManWalkOffAlt")
        if piStack_144 == nil then
        else
            puVar5 = (**(*piStack_144 + 0x18))()
        end
        cVar2 = quest:IsCameraPosOnScreen(puVar5)
        if not cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00db6a1f: (native jump target)
                -- TODO(native): this = (C3DClothPrimitive *)apStack_138;
                -- TODO(native): goto LAB_00db6a32
            end
            ppVar12 = r4
        else
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00db6a1f
        end
        quest:EntityTeleportToThing(me, r5, false)
        __native_entity_state:SetStateInt("MyPhase", 3)
        break
        -- TODO(native): case 3:
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_1f4);
        if bVar3 then
        end
        cVar2 = me:AcquireControl(4)
        while not cVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db6afd end
            cVar2 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db6afd end
        quest:EntitySetCutsceneBehaviour(me, 1)
        quest:EntitySetTargetable(me, false)
        if nil == nil then
        else
            puVar5 = (**(*0x0 + 0x18))()
        end
        -- TODO(native): piStack_144 = (int *)*puVar5;
        -- TODO(native): uStack_140 = puVar5[1];
        fVar25 = 2.0
        -- TODO(native): uStack_13c = puVar5[2];
        -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&ppuStack_1f4);
        -- TODO(native): bVar3 = IsDistanceFromThingToPositionOver(pCVar7,(C3DVector *)&piStack_144,fVar25);
        bVar3 = nil --[[unresolved native result]]
        if bVar3 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db6afd end
                me:MoveToPosition(nil --[[missing]], 0x0, 0x1, false, false)
                bVar3 = me:IsPerformingScriptTask()
                if bVar3 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db6afd end
                        bVar3 = me:IsPerformingScriptTask()
                    until not (bVar3)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db6afd end
                fVar25 = 2.0
                -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&ppuStack_1f4);
                -- TODO(native): bVar3 = IsDistanceFromThingToPositionOver(pCVar7,(C3DVector *)&piStack_144,fVar25);
                bVar3 = nil --[[unresolved native result]]
            until not (bVar3)
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db6afd end
        __native_entity_state:SetStateInt("MyPhase", 4)
        break
        -- TODO(native): case 4:
        quest:SetStateBool("BarrelManSpokenToHeroOnReturn", true)
        uVar6 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(me, uVar6)
        quest:EntitySetCutsceneBehaviour(me, 2)
        quest:EntitySetTargetable(me, true)
        uVar6 = quest:GetHero()
        cVar2 = quest:CanThingBe_Seen_ByOtherThing(uVar6, r4)
        if cVar2 then
            -- LAB_00db5c28: (native jump target)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db6afd end
            -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_1f4);
            if bVar3 then
            end
            cVar2 = me:AcquireControl(4)
            while not cVar2 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db6afd end
                cVar2 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db6afd end
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_108);
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            -- TODO(native): uVar4 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe00);
            fVar19 = quest:GetHealth(r3)
            fVar20 = _DAT_0122dedc
            if fVar20 < fVar19 then
                bVar3 = false
                pCVar24 = 0x1
                pCVar23 = 0x0
                pCVar22 = 0x0
                pcVar21 = "TEXT_QST_048_BARRELMAN_THANKS"
                pCVar7 = quest:GetHero()
                r6 = me:Speak(pCVar7, pcVar21, pCVar22, (pCVar23 ~= 0), (pCVar24 ~= 0), bVar3)
                bVar3 = me:IsPerformingScriptTask()
                if bVar3 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00db6af8
                        end
                        bVar3 = me:IsPerformingScriptTask()
                    until not (bVar3)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00db6af8
                end
            end
            require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
            quest:PauseAllNonScriptedEntities(false)
            -- LAB_00db5daf: (native jump target)
            quest:SetCreatureBrain(me, "BRAIN_GOOD_VILLAGER_BASE")
            __native_entity_state:SetStateInt("MyPhase", 5)
            break
        end
        fVar25 = 10.0
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, fVar25)
        if bVar3 then return end  -- TODO(native): goto LAB_00db5c28
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db6afd end
        ppVar12 = quest:AddNewConversation(me, false, false)
        -- TODO(native): uVar6 = CCarriedReadableDef::CCarriedReadableDef(aCStack_88);
        quest:AddLineToConversation(ppVar12, uVar6, me, r2, false)
        __native_entity_state:SetStateBool("HeroLetMeDown", true)
        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_118);
        pCVar22 = ""
        quest:StartMovieSequence()
        quest:PauseAllNonScriptedEntities(true)
        quest:DisplayGameInfo("TEXT_QST_048_INSTRUCTION_LEFT_WAREHOUSE_UNATTENDED")
        cVar2 = quest:MsgIsGameInfoClickedPast()
        while not cVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                quest:PauseAllNonScriptedEntities(false)
                goto LAB_00db6af8
            end
            cVar2 = quest:MsgIsGameInfoClickedPast()
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 1)
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): goto LAB_00db5daf
        end
        quest:PauseAllNonScriptedEntities(false)
        goto LAB_00db6af8
    end
    -- TODO(native): switchD_00db5526_default:
    ppuVar17 = (ppuStack_1c8 | 1)
    cVar2 = me:MsgIsHitByHero()
    if not cVar2 then
        ppuVar17 = (ppuVar18 | 3)
        cVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
        if cVar2 then
            ppuVar17 = (ppuVar18 | 7)
            cVar2 = me:MsgIsHitByHeroSpecialAbility(0xe)
            if not cVar2 then return end  -- TODO(native): goto LAB_00db5e85
        end
        bVar3 = false
    else
        -- LAB_00db5e85: (native jump target)
        bVar3 = true
    end
    if (ppuVar17 & 4) ~= 0 then
        ppuVar17 = (ppuVar17 & 0xfffffffb)
    end
    if (ppuVar17 & 2) ~= 0 then
        ppuVar17 = (ppuVar17 & 0xfffffffd)
    end
    if (ppuVar17 & 1) ~= 0 then
        ppuVar17 = (ppuVar17 & 0xfffffffe)
    end
    if bVar3 then
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db6afd end
        uVar6 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(me, uVar6)
        uVar6 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(uVar6, r1)
        require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffdfc);
        if bVar3 then
        end
        cVar2 = me:AcquireControl(4)
        while not cVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db6afd end
            cVar2 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db6afd end
        quest:StartMovieSequence()
        quest:PauseAllNonScriptedEntities(true)
        -- TODO(native): uVar4 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe00);
        fVar19 = quest:GetHealth(nil --[[missing]])
        fVar20 = _DAT_0122dedc
        if fVar19 <= fVar20 then
            -- LAB_00db6088: (native jump target)
            quest:PauseAllNonScriptedEntities(false)
            goto LAB_00db6933
        end
        bVar3 = false
        pCVar24 = 0x1
        pCVar23 = 0x0
        pCVar22 = 0x0
        pcVar21 = "TEXT_QST_048_BARRELMAN_CAREFUL"
        pCVar7 = quest:GetHero()
        r7 = me:Speak(pCVar7, pcVar21, pCVar22, (pCVar23 ~= 0), (pCVar24 ~= 0), bVar3)
        bVar3 = me:IsPerformingScriptTask()
        if bVar3 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    quest:PauseAllNonScriptedEntities(false)
                    goto LAB_00db6af8
                end
                bVar3 = me:IsPerformingScriptTask()
            until not (bVar3)
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then return end  -- TODO(native): goto LAB_00db6088
        quest:PauseAllNonScriptedEntities(false)
        goto LAB_00db6af8
    end
    if __native_entity_state:GetStateInt("MyPhase") == 0 then
        -- TODO(native): fVar25 = DAT_013ac858;
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, 0x0)
        if not bVar3 then return end  -- TODO(native): goto LAB_00db60c9
        -- LAB_00db60fb: (native jump target)
        bVar3 = true
    else
        -- LAB_00db60c9: (native jump target)
        ppuVar17 = (ppuVar17 | 8)
        cVar2 = me:IsTalkedToByHero()
        bVar3 = false
        if cVar2 then return end  -- TODO(native): goto LAB_00db60fb
    end
    if (ppuVar17 & 8) ~= 0 then
        -- TODO(native): ppuStack_1c8 = (undefined **)((uint)ppuVar17 & 0xfffffff7);
    end
    if bVar3 then
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db6afd end
        -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_1f4);
        if bVar3 then
        end
        cVar2 = me:AcquireControl(4)
        while not cVar2 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db6afd end
            cVar2 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db6afd end
        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&piStack_1c0);
        pCVar22 = ""
        quest:StartMovieSequence()
        ppuVar18 = *(param_1 + 4)
        uVar6 = 1
        -- TODO(native): (**(code **)(*ppuVar18 + 0x5ec))();
        uVar4 = uVar6
        iVar11 = __native_entity_state:GetStateInt("MyPhase")
        if iVar11 ~= 0 then
            if 3 < iVar11 then
                if iVar11 ~= 5 then goto LAB_00db684c end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db6ae0 end
                if not __native_entity_state:GetStateBool("HeroLetMeDown") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db6ae0 end
                    -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe00);
                    fVar19 = quest:GetHealth(nil --[[missing]])
                    fVar20 = _DAT_0122dedc
                    if fVar20 < fVar19 then
                        bVar3 = false
                        pCVar24 = 0x1
                        pCVar23 = 0x0
                        pCVar22 = 0x0
                        pcVar21 = "TEXT_QST_048_BARRELMAN_NO_TIME"
                        pCVar7 = quest:GetHero()
                        r8 = me:Speak(pCVar7, pcVar21, pCVar22, (pCVar23 ~= 0), (pCVar24 ~= 0), bVar3)
                        bVar3 = me:IsPerformingScriptTask()
                        if bVar3 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00db6783
                                bVar3 = me:IsPerformingScriptTask()
                            until not (bVar3)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00db6ae6: (native jump target)
                            puVar14 = *ppuStack_1d8
                            goto LAB_00db6aec
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db6ac9 end
                    if quest:GetStateBool("BarrelBrokenPersistent") then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db6ae0 end
                        -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe00);
                        fVar20 = quest:GetHealth(nil --[[missing]])
                        puVar5 = CONCAT13(1,(int3)puVar28)
                        if fVar20 <= _DAT_0122dedc then
                            puVar5 = (puVar28 & 0xffffff)
                        end
                        puVar28 = puVar5
                        if (puVar28 >> 0x18) ~= 0 then
                            bVar3 = false
                            pCVar24 = 0x1
                            pCVar23 = 0x0
                            pCVar22 = 0x0
                            pcVar21 = "TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN"
                            pCVar7 = quest:GetHero()
                            r9 = me:Speak(pCVar7, pcVar21, pCVar22, (pCVar23 ~= 0), (pCVar24 ~= 0), bVar3)
                            bVar3 = me:IsPerformingScriptTask()
                            if bVar3 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00db6ac9 end
                                    bVar3 = me:IsPerformingScriptTask()
                                until not (bVar3)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db6ae0 end
                        end
                        -- TODO(native): goto LAB_00db684c
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db6ac9 end
                    -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe00);
                    fVar19 = quest:GetHealth(nil --[[missing]])
                    fVar20 = _DAT_0122dedc
                    if fVar20 < fVar19 then
                        bVar3 = false
                        pCVar24 = 0x1
                        pCVar23 = 0x0
                        pCVar22 = 0x0
                        pcVar21 = "TEXT_QST_048_BARRELMAN_LETDOWN_NOT_BROKE"
                        pCVar7 = quest:GetHero()
                        r10 = me:Speak(pCVar7, pcVar21, pCVar22, (pCVar23 ~= 0), (pCVar24 ~= 0), bVar3)
                        bVar3 = me:IsPerformingScriptTask()
                        if bVar3 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00db6ae6
                                bVar3 = me:IsPerformingScriptTask()
                            until not (bVar3)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00db6783: (native jump target)
                            -- TODO(native): (**(code **)(*ppuStack_1d8 + 0x5ec))(0);
                            goto LAB_00db6af4
                        end
                    end
                end
                ::LAB_00db684c::
                -- TODO(native): bVar3 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffe00);
                if bVar3 then
                end
                -- TODO(native): (**(code **)(*ppuVar18 + 0x5ec))(0);
                goto LAB_00db6933
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe00);
                fVar20 = quest:GetHealth(nil --[[missing]])
                puVar5 = CONCAT13(1,(int3)puVar28)
                if fVar20 <= _DAT_0122dedc then
                    puVar5 = (puVar28 & 0xffffff)
                end
                puVar28 = puVar5
                if (puVar28 >> 0x18) ~= 0 then
                    bVar3 = false
                    pCVar24 = 0x1
                    pCVar23 = 0x0
                    pCVar22 = 0x0
                    pcVar21 = "TEXT_QST_048_BARRELMAN_NOT_LARKING"
                    pCVar7 = quest:GetHero()
                    r11 = me:Speak(pCVar7, pcVar21, pCVar22, (pCVar23 ~= 0), (pCVar24 ~= 0), bVar3)
                    bVar3 = me:IsPerformingScriptTask()
                    if bVar3 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db6ae0 end
                            bVar3 = me:IsPerformingScriptTask()
                        until not (bVar3)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db6ac9 end
                end
                -- TODO(native): goto LAB_00db684c
            end
            goto LAB_00db6ac9
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db6ae0 end
        -- TODO(native): CTimer::CTimer((CTimer *)&piStack_1d0);
        piVar8 = quest:GetHero()
        fVar25 = 4.0
        pCVar9 = piVar8:GetPos()
        bVar3 = (me ~= nil and me:IsDistanceFromPositionOver(pCVar9, fVar25))
        piVar8 = 0x0
        uVar4 = uVar6
        if bVar3 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db6ac0 end
                piVar10 = quest:GetHero()
                pCVar26 = 0x1
                pCVar24 = 0x0
                pCVar23 = 0x1
                pcVar21 = 0x40000000
                pCVar7 = piVar10:GetPos()
                me:MoveToPosition(pCVar7, SUB41(uVar6,0), SUB41(pCVar22,0))
                quest:SetTimer(piVar8, 2)
                bVar3 = me:IsPerformingScriptTask()
                if bVar3 then
                    repeat
                        iVar11 = quest:GetTimer(piVar8)
                        if iVar11 < 1 then break end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db6ad7 end
                        bVar3 = me:IsPerformingScriptTask()
                    until not (bVar3)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db6ac0 end
                piVar10 = quest:GetHero()
                fVar25 = 4.0
                pCVar9 = piVar10:GetPos()
                bVar3 = (me ~= nil and me:IsDistanceFromPositionOver(pCVar9, fVar25))
                uVar4 = uVar6
            until not (bVar3)
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffe00);
            fVar20 = quest:GetHealth(piVar10)
            puVar5 = CONCAT13(1,(int3)puVar28)
            if fVar20 <= _DAT_0122dedc then
                puVar5 = (puVar28 & 0xffffff)
            end
            puVar28 = puVar5
            if (puVar28 >> 0x18) ~= 0 then
                bVar3 = false
                pCVar24 = 0x1
                pCVar23 = 0x0
                pCVar22 = 0x0
                pcVar21 = "TEXT_QST_048_BARRELMAN_FAVOUR"
                pCVar7 = quest:GetHero()
                r12 = me:Speak(pCVar7, pcVar21, pCVar22, (pCVar23 ~= 0), (pCVar24 ~= 0), bVar3)
                bVar3 = me:IsPerformingScriptTask()
                if bVar3 then
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db6ac0 end
                        bVar3 = me:IsPerformingScriptTask()
                    until not (bVar3)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db6ad7 end
            end
            quest:FadeScreenOut(0x3f800000, 0x3f800000)
            quest:Pause(0x40000000)
            iVar11 = *piVar8
            uVar6 = quest:GetThingWithScriptName("M_WHouse_GuardPoint")
            ppVar12 = quest:GetHero()
            quest:EntityTeleportToThing(ppVar12, uVar6)
            uVar6 = quest:GetThingWithScriptName("M_BarrelManHiddenPos")
            quest:EntityTeleportToThing(me, uVar6)
            __native_entity_state:SetStateInt("MyPhase", 2)
            quest:ClearThingHasInformation(me)
            quest:FadeScreenIn()
            quest:SetTimer(quest:GetStateInt("WatchTimer"), 0x2d)
            quest:SetStateBool("BarrelManLeftHeroInCharge", true)
            -- TODO(native): goto LAB_00db684c
        end
        ::LAB_00db6ad7::
        ::LAB_00db6ae0::
        puVar14 = *ppuVar18
        ::LAB_00db6aec::
        -- TODO(native): (**(code **)(puVar14 + 0x5ec))(0);
        goto LAB_00db6af4
    end
    native_arg_sequence_1 = false
    if __native_entity_state:GetStateInt("MyPhase") == 0 then
        native_arg_sequence_1 = true
    else
        native_arg_sequence_1 = false
    end
    if native_arg_sequence_1 then
        if not __native_entity_state:GetStateBool("OverheardYet") then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if not native_arg_sequence_1 then
            iVar11 = rand()
            if iVar11 % DAT_013ac854 == 0 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
    end
    if native_arg_sequence_1 then
        fVar25 = 15.0
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, fVar25)
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db6afd end
            __native_entity_state:SetStateBool("OverheardYet", true)
            uVar6 = quest:AddNewConversation(me, false, false)
            -- TODO(native): uVar13 = CCarriedReadableDef::CCarriedReadableDef(aCStack_e8);
            quest:AddLineToConversation(uVar6, "TEXT_QST_048_BARRELMAN_OVERHEAR", me, nil --[[missing]], false)
        end
    end
    ::LAB_00db6933::
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    cVar2 = not alive
    until false
    ::LAB_00db6ac0::
    ::LAB_00db6ac9::
    -- TODO(native): (**(code **)(*ppuVar18 + 0x5ec))(0);
    ::LAB_00db6af4::
    ::LAB_00db6af8::
    ::LAB_00db6afd::
end

