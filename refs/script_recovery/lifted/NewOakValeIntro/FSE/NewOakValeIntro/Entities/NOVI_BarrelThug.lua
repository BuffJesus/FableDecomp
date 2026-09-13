-- Generated native draft: NOVI_BarrelThug. Review coverage report before use.
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
    __native_entity_state:SetStateInt("LastTimeSpoken", 9999)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
end

function Main(quest, me)
    local CVar25, __native_condition_1, bVar5, cVar6, fVar16, fVar3, iVar10, pCVar1, pCVar18, pCVar19, pCVar20, pCVar21, pCVar22, pCVar24, pCVar8, pCVar9, paVar13, pcVar17, ppVar11, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r3, r4, r5, r6, r7, r8, r9, uVar12, uVar15, uVar23, uVar4, uVar7
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    alive = not quest:IsActiveThreadTerminating()
    cVar6 = not alive
    while true do
        if cVar6 then
            return
        end
        if not __native_entity_state:GetStateBool("DoneIntro") then break end
        -- LAB_00db6f4e: (native jump target)
        cVar6 = me:IsTalkedToByHero()
        if cVar6 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db7ce1 end
            pCVar21 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff0c);
            if bVar5 then
            end
            cVar6 = me:AcquireControl(4)
            while not cVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    quest:PauseAllNonScriptedEntities(false)
                    return
                end
                cVar6 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                -- LAB_00db7c90: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)appuStack_d0;
                goto LAB_00db7cd8
            end
            if not quest:GetStateBool("BarrelManSpokenToHeroOnReturn") then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00db7c90
                if not quest:GetStateBool("BarrelBrokenPersistent") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00db7c90
                    -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff00);
                    fVar16 = quest:GetHealth(nil --[[missing]])
                    fVar3 = _DAT_0122dedc
                    if fVar3 < fVar16 then
                        CVar25 = 0x0
                        pCVar22 = 0x1
                        pCVar8 = 0x0
                        pCVar21 = 0x2
                        pcVar17 = "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT"
                        pCVar9 = quest:GetHero()
                        r1 = me:Speak(pCVar9, pcVar17, pCVar21, (pCVar8 ~= 0), (pCVar22 ~= 0), (CVar25 ~= 0))
                        bVar5 = me:IsPerformingScriptTask()
                        if bVar5 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00db71e5
                                bVar5 = me:IsPerformingScriptTask()
                            until not (bVar5)
                        end
                        -- TODO(native): goto LAB_00db737b
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00db7c7e
                    -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff00);
                    fVar16 = quest:GetHealth(nil --[[missing]])
                    fVar3 = _DAT_0122dedc
                    if fVar3 < fVar16 then
                        CVar25 = 0x0
                        pCVar22 = 0x1
                        pCVar8 = 0x0
                        pCVar21 = 0x2
                        pcVar17 = "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE"
                        pCVar9 = quest:GetHero()
                        r2 = me:Speak(pCVar9, pcVar17, pCVar21, (pCVar8 ~= 0), (pCVar22 ~= 0), (CVar25 ~= 0))
                        bVar5 = me:IsPerformingScriptTask()
                        if bVar5 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00db7ca2
                                bVar5 = me:IsPerformingScriptTask()
                            until not (bVar5)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        cVar6 = not alive
                        -- TODO(native): goto joined_r0x00db72cd;
                    end
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00db7c7e: (native jump target)
                    quest:PauseAllNonScriptedEntities((CVar25 ~= 0))
                    -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)appuStack_d0;
                    goto LAB_00db7cd8
                end
                if not quest:GetStateBool("BarrelBrokenPersistent") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00db7c7e
                    -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff00);
                    fVar16 = quest:GetHealth(nil --[[missing]])
                    fVar3 = _DAT_0122dedc
                    if fVar3 < fVar16 then
                        CVar25 = 0x0
                        pCVar22 = 0x1
                        pCVar8 = 0x0
                        pCVar21 = 0x0
                        pcVar17 = "TEXT_QST_048_BARRELTHUG_WHY_NOT_SMASH"
                        pCVar9 = quest:GetHero()
                        r3 = me:Speak(pCVar9, pcVar17, pCVar21, (pCVar8 ~= 0), (pCVar22 ~= 0), (CVar25 ~= 0))
                        bVar5 = me:IsPerformingScriptTask()
                        if bVar5 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00db7ca2
                                bVar5 = me:IsPerformingScriptTask()
                            until not (bVar5)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        cVar6 = not alive
                        -- TODO(native): joined_r0x00db72cd:
                        if cVar6 then
                            -- LAB_00db71e5: (native jump target)
                            quest:PauseAllNonScriptedEntities((CVar25 ~= 0))
                            -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)appuStack_d0;
                            goto LAB_00db7cd8
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00db7c90
                    -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff00);
                    fVar16 = quest:GetHealth(nil --[[missing]])
                    fVar3 = _DAT_0122dedc
                    if fVar3 < fVar16 then
                        CVar25 = 0x0
                        pCVar22 = 0x1
                        pCVar8 = 0x0
                        pCVar21 = 0x0
                        pcVar17 = "TEXT_QST_048_BARRELTHUG_OUTRO"
                        pCVar9 = quest:GetHero()
                        r4 = me:Speak(pCVar9, pcVar17, pCVar21, (pCVar8 ~= 0), (pCVar22 ~= 0), (CVar25 ~= 0))
                        bVar5 = me:IsPerformingScriptTask()
                        if bVar5 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00db71e5
                                bVar5 = me:IsPerformingScriptTask()
                            until not (bVar5)
                        end
                        -- LAB_00db737b: (native jump target)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00db7ca2: (native jump target)
                            quest:PauseAllNonScriptedEntities((CVar25 ~= 0))
                            -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)appuStack_d0;
                            goto LAB_00db7cd8
                        end
                    end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
        end
        __native_condition_1 = not quest:GetStateBool("BarrelManSpokenToHeroOnReturn")
        if __native_condition_1 then
            iVar10 = quest:GetTimer(0)
            __native_condition_1 = 0 < iVar10
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db7ce1 end
            if not quest:GetStateBool("BarrelBrokenPersistent") then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db7ce1 end
                ppVar11 = quest:AddNewConversation(me, false, nil --[[missing]])
                r5 = quest:GetHero()
                quest:AddPersonToConversation(nil --[[missing]], r5)
                iVar10 = quest:GetTimer(nil --[[missing]])
                if (iVar10 < 10) and (10 < __native_entity_state:GetStateInt("LastTimeSpoken")) then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db7ce1 end
                    r6 = quest:GetHero()
                    quest:AddLineToConversation(0x0, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_10", r6, nil --[[missing]])
                else
                    iVar10 = quest:GetTimer(nil --[[missing]])
                    if (iVar10 < 0x14) and (0x14 < __native_entity_state:GetStateInt("LastTimeSpoken")) then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db7ce1 end
                        r7 = quest:GetHero()
                        quest:AddLineToConversation(0x0, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_20", r7, nil --[[missing]])
                    else
                        iVar10 = quest:GetTimer(nil --[[missing]])
                        if (iVar10 < 0x19) and (0x19 < __native_entity_state:GetStateInt("LastTimeSpoken")) then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db7ce1 end
                            r8 = quest:GetHero()
                            quest:AddLineToConversation(0x0, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_30", r8, nil --[[missing]])
                        else
                            iVar10 = quest:GetTimer(nil --[[missing]])
                            if (iVar10 < 0x1e) and (0x1e < __native_entity_state:GetStateInt("LastTimeSpoken")) then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00db7ce1 end
                                r9 = quest:GetHero()
                                quest:AddLineToConversation(0x0, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_40", r9, nil --[[missing]])
                            else
                                iVar10 = quest:GetTimer(nil --[[missing]])
                                if (iVar10 < 0x22) and (0x22 < __native_entity_state:GetStateInt("LastTimeSpoken")) then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00db7ce1 end
                                    r10 = quest:GetHero()
                                    quest:AddLineToConversation(0x0, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_50", r10, nil --[[missing]])
                                    -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_a4;
                                else
                                    iVar10 = quest:GetTimer(nil --[[missing]])
                                    if (iVar10 < 0x26) and (0x26 < __native_entity_state:GetStateInt("LastTimeSpoken")) then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00db7ce1 end
                                        r11 = quest:GetHero()
                                        quest:AddLineToConversation(0x0, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_60", r11, nil --[[missing]])
                                    else
                                        iVar10 = quest:GetTimer(nil --[[missing]])
                                        if (0x2c < iVar10) or (__native_entity_state:GetStateInt("LastTimeSpoken") < 0x2e) then goto LAB_00db7924 end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00db7ce1 end
                                        r12 = quest:GetHero()
                                        quest:AddLineToConversation(0x0, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_70", r12, nil --[[missing]])
                                        -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_b0;
                                    end
                                end
                            end
                        end
                    end
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db7ce1 end
                ppVar11 = quest:AddNewConversation(me, nil --[[missing]], nil --[[missing]])
                r13 = quest:GetHero()
                quest:AddPersonToConversation(nil --[[missing]], r13)
                iVar10 = quest:GetTimer(nil --[[missing]])
                if (iVar10 < 10) and (10 < __native_entity_state:GetStateInt("LastTimeSpoken")) then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db7ce1 end
                    r14 = quest:GetHero()
                    quest:AddLineToConversation(0x0, "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_10", r14, nil --[[missing]])
                    -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_98;
                else
                    iVar10 = quest:GetTimer(nil --[[missing]])
                    if (iVar10 < 0x19) and (0x19 < __native_entity_state:GetStateInt("LastTimeSpoken")) then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00db7ce1 end
                        r15 = quest:GetHero()
                        quest:AddLineToConversation(0x0, "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_20", r15, nil --[[missing]])
                        -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_a8;
                    else
                        iVar10 = quest:GetTimer(nil --[[missing]])
                        if (iVar10 < 0x23) and (0x23 < __native_entity_state:GetStateInt("LastTimeSpoken")) then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db7ce1 end
                            r16 = quest:GetHero()
                            quest:AddLineToConversation(0x0, "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_30", r16, nil --[[missing]])
                        else
                            iVar10 = quest:GetTimer(nil --[[missing]])
                            if (0x2c < iVar10) or (__native_entity_state:GetStateInt("LastTimeSpoken") < 0x2e) then goto LAB_00db7924 end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db7ce1 end
                            r17 = quest:GetHero()
                            quest:AddLineToConversation(0x0, "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_40", r17, nil --[[missing]])
                            -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_a0;
                        end
                    end
                end
            end
            uVar12 = quest:GetTimer(nil --[[missing]])
            __native_entity_state:SetStateInt("LastTimeSpoken", uVar12)
        else
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db7ce1 end
        end
        ::LAB_00db7924::
        uVar15 = uStack_d4 | 1
        cVar6 = me:MsgIsHitByHero()
        if not cVar6 then
            uVar15 = uVar4 | 3
            cVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if cVar6 then
                uVar15 = uVar4 | 7
                cVar6 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                if not cVar6 then return end  -- TODO(native): goto LAB_00db79ac
            end
            -- TODO(native): cStack_d5 = '\0';
        else
            -- LAB_00db79ac: (native jump target)
            -- TODO(native): cStack_d5 = '\x01';
        end
        if (uVar15 & 4) ~= 0 then
            uVar15 = uVar15 & 0xfffffffb
        end
        if (uVar15 & 2) ~= 0 then
            uVar15 = uVar15 & 0xfffffffd
        end
        if (uVar15 & 1) ~= 0 then
            -- TODO(native): uStack_d4 = uVar15 & 0xfffffffe;
        end
        if cStack_d5 ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00db7ce1 end
            r18 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(r18, nil --[[missing]])
            r19 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(r19, me)
            require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_88);
            paVar13 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&pCStack_104);
            if bVar5 then
            end
            cVar6 = me:AcquireControl(4)
            while true do
                uVar7 = SUB41(me,0)
                if not (not cVar6) then break end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db7cb5 end
                cVar6 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                -- TODO(native): uVar12 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffef0);
                fVar16 = quest:GetHealth(nil --[[missing]])
                fVar3 = _DAT_0122dedc
                if fVar3 < fVar16 then
                    bVar5 = false
                    pCVar22 = 0x1
                    pCVar8 = 0x0
                    pCVar21 = 0x0
                    pcVar17 = "TEXT_QST_048_BARRELTHUG_WHY_HIT"
                    pCVar9 = quest:GetHero()
                    r20 = me:Speak(pCVar9, pcVar17, pCVar21, (pCVar8 ~= 0), (pCVar22 ~= 0), bVar5)
                    bVar5 = me:IsPerformingScriptTask()
                    if bVar5 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db7cb5 end
                            bVar5 = me:IsPerformingScriptTask()
                        until not (bVar5)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00db7cc4 end
                end
                quest:PauseAllNonScriptedEntities(false)
                goto LAB_00db7bb0
            end
            ::LAB_00db7cc4::
            quest:PauseAllNonScriptedEntities(false)
            goto LAB_00db7cd1
        end
        ::LAB_00db7bb0::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cVar6 = not alive
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00db7ce1 end
    -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_e8);
    if bVar5 then
    end
    cVar6 = me:AcquireControl(4)
    while not cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db7ce1 end
        cVar6 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00db7ce1 end
    cVar6 = quest:GetStateBool("BarrelManLeftHeroInCharge")
    while not cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db7ce1 end
        cVar6 = quest:GetStateBool("BarrelManLeftHeroInCharge")
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00db7ce1 end
    -- TODO(native): pCStack_104 = aCStack_54;
    r21 = quest:GetThingWithScriptName("M_WHouse_ManStart")
    quest:EntityTeleportToThing(r21, nil --[[missing]])
    r22 = quest:GetHero()
    quest:EntitySetFacingAngleTowardsThing(r22, nil --[[missing]])
    quest:Pause(nil --[[missing]])
    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_88);
    pCVar21 = ""
    quest:StartMovieSequence()
    pCVar22 = 0x1
    quest:PauseAllNonScriptedEntities((pCVar22 ~= 0))
    -- TODO(native): pCVar8 = (CCharString *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffee8);
    fVar16 = quest:GetHealth(nil --[[missing]])
    fVar3 = _DAT_0122dedc
    uVar23 = SUB41(pCVar24,0)
    uVar7 = SUB41(pCVar21,0)
    if fVar16 <= fVar3 then
        -- LAB_00db6f15: (native jump target)
        pCVar21 = 0x1
        __native_entity_state:SetStateBool("DoneIntro", true)
        pCVar9 = quest:GetHero()
        me:FollowThing(pCVar9, pCVar22, (pCVar21 ~= 0))
        quest:PauseAllNonScriptedEntities(false)
        -- TODO(native): goto LAB_00db6f4e
    end
    bVar5 = false
    pCVar20 = 0x1
    pCVar19 = 0x0
    pCVar18 = 0x0
    pcVar17 = "TEXT_QST_048_BARRELTHUG_EXPLAIN"
    pCVar9 = quest:GetHero()
    r23 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar5)
    bVar5 = me:IsPerformingScriptTask()
    uVar23 = SUB41(pCVar24,0)
    uVar7 = SUB41(pCVar21,0)
    if bVar5 then
        repeat
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): this = aCStack_64;
                goto LAB_00db7cd8
            end
            bVar5 = me:IsPerformingScriptTask()
            uVar23 = SUB41(pCVar24,0)
            uVar7 = SUB41(pCVar21,0)
        until not (bVar5)
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then return end  -- TODO(native): goto LAB_00db6f15
    quest:PauseAllNonScriptedEntities(false)
    -- TODO(native): this = aCStack_64;
    ::LAB_00db7cd8::
    ::LAB_00db7ce1::
    do return end
    ::LAB_00db7cb5::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00db7cd1::
    -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)&ppuStack_78;
    goto LAB_00db7cd8
end

