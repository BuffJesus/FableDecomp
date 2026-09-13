-- Generated native draft: NOVI_LiveFather. Review coverage report before use.
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
    __native_entity_state:SetStateInt("PenniesGiven", 0)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

function Main(quest, me)
    local aVar22, bVar5, cVar6, fVar13, fVar3, iVar10, pCVar1, pCVar16, pCVar18, pCVar19, pCVar20, pCVar23, pCVar9, pcVar17, ppVar7, r1, r10, r11, r12, r13, r14, r15, r16, r2, r3, r4, r5, r6, r7, r8, r9, uVar12, uVar14, uVar15, uVar4
    local alive = true
    -- TODO(native): map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> amStack_d0 [4];
    -- TODO(native): allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_cc [4];
    -- TODO(native): uint uStack_c8;
    -- TODO(native): CCharString aCStack_c4 [8];
    -- TODO(native): undefined4 auStack_bc [2];
    -- TODO(native): allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_b4 [4];
    -- TODO(native): CCharString aCStack_b0 [4];
    -- TODO(native): CCharString aCStack_ac [4];
    -- TODO(native): CCharString aCStack_a8 [8];
    -- TODO(native): CCharString aCStack_a0 [4];
    -- TODO(native): CScriptGameResourceObjectMovieBase aCStack_9c [8];
    -- TODO(native): CCharString aCStack_94 [4];
    -- TODO(native): undefined **ppuStack_90;
    -- TODO(native): CScriptThing aCStack_8c [12];
    -- TODO(native): CScriptGameResourceObjectMovieBase aCStack_80 [4];
    -- TODO(native): allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> aaStack_7c [4];
    -- TODO(native): C3DClothPrimitive aCStack_78 [12];
    -- TODO(native): C3DClothPrimitive aCStack_6c [12];
    -- TODO(native): C3DClothPrimitive aCStack_60 [12];
    -- TODO(native): C3DClothPrimitive aCStack_54 [12];
    -- TODO(native): C3DClothPrimitive aCStack_48 [12];
    -- TODO(native): C3DClothPrimitive aCStack_3c [12];
    -- TODO(native): C3DClothPrimitive aCStack_30 [12];
    -- TODO(native): C3DClothPrimitive aCStack_24 [36];
    -- TODO(native): auStack_bc[0] = 0;
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    -- TODO(native): auStack_d8[0] = 0;
    -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_e4);
    if bVar5 then
    end
    cVar6 = me:AcquireControl(4)
    while not cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00db9785
        cVar6 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then return end  -- TODO(native): goto LAB_00db9785
    if quest:GetStateBool("DadFinishedIntro") then
        -- LAB_00db8aee: (native jump target)
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            -- LAB_00db8b00: (native jump target)
            -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff10);
            if bVar5 then
            end
            cVar6 = me:AcquireControl(4)
            while not cVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db9785 end
                cVar6 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                cVar6 = me:IsTalkedToByHero()
                if not cVar6 then
                    -- LAB_00db9499: (native jump target)
                    uVar12 = uStack_c8 | 1
                    cVar6 = me:MsgIsHitByHero()
                    if not cVar6 then
                        uVar12 = uVar4 | 3
                        cVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if cVar6 then
                            uVar12 = uVar4 | 7
                            cVar6 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                            if not cVar6 then return end  -- TODO(native): goto LAB_00db9527
                        end
                        -- TODO(native): uStack_dc = uStack_dc & 0xffffff;
                    else
                        -- LAB_00db9527: (native jump target)
                        -- TODO(native): uStack_dc = CONCAT13(1,(undefined3)uStack_dc);
                    end
                    if (uVar12 & 4) ~= 0 then
                        uVar12 = uVar12 & 0xfffffffb
                    end
                    if (uVar12 & 2) ~= 0 then
                        uVar12 = uVar12 & 0xfffffffd
                    end
                    if (uVar12 & 1) ~= 0 then
                        -- TODO(native): uStack_c8 = uVar12 & 0xfffffffe;
                    end
                    if uStack_dc._3_1_ == 0 then goto LAB_00db9720 end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        r1 = quest:GetHero()
                        quest:EntitySetThingAsAllyOfThing(r1, nil --[[missing]])
                        r2 = quest:GetHero()
                        quest:EntitySetThingAsAllyOfThing(r2, nil --[[missing]])
                        require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aCStack_ac);
                        pCVar18 = ""
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffef4);
                        if bVar5 then
                        end
                        cVar6 = me:AcquireControl(4)
                        uVar15 = SUB41(me,0)
                        while not cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db975e end
                            cVar6 = me:AcquireControl(4)
                            uVar15 = SUB41(me,0)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffee8);
                            fVar13 = quest:GetHealth(nil --[[missing]])
                            fVar3 = _DAT_0122dedc
                            if fVar3 < fVar13 then
                                aVar22 = 0x0
                                pCVar20 = 0x1
                                pCVar19 = 0x0
                                pCVar18 = 0x0
                                pcVar17 = "TEXT_QST_048_DAD_TEMPER"
                                pCVar9 = quest:GetHero()
                                r3 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), (aVar22 ~= 0))
                                bVar5 = me:IsPerformingScriptTask()
                                if bVar5 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00db975e end
                                        bVar5 = me:IsPerformingScriptTask()
                                    until not (bVar5)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00db976c end
                            end
                            quest:PauseAllNonScriptedEntities((aVar22 ~= 0))
                            goto LAB_00db9720
                        end
                        ::LAB_00db976c::
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        goto LAB_00db9778
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xffffff10);
                        if bVar5 then
                        end
                        cVar6 = me:AcquireControl(4)
                        while not cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00db9785 end
                            cVar6 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)auStack_d8);
                            pCVar18 = ""
                            quest:StartMovieSequence()
                            ppVar7 = 0x1
                            quest:PauseAllNonScriptedEntities((ppVar7 ~= 0))
                            if (quest:GetStateInt("GoodDeedsPerformed") == 0) and (quest:GetStateInt("BadDeedsPerformed") == 0) then
                                alive = not quest:IsActiveThreadTerminating()
                                aVar22 = SUB41(ppVar7,0)
                                if alive then
                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&pppuStack_fc);
                                    fVar13 = quest:GetHealth(nil --[[missing]])
                                    fVar3 = _DAT_0122dedc
                                    if fVar3 < fVar13 then
                                        bVar5 = false
                                        pCVar20 = 0x1
                                        pCVar19 = 0x0
                                        pCVar18 = 0x0
                                        pcVar17 = "TEXT_QST_048_DAD_DONE_NOTHING_YET"
                                        pCVar9 = quest:GetHero()
                                        r4 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar5)
                                        bVar5 = me:IsPerformingScriptTask()
                                        if bVar5 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then return end  -- TODO(native): goto LAB_00db974b
                                                bVar5 = me:IsPerformingScriptTask()
                                            until not (bVar5)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00db8d23 end
                                    end
                                    -- LAB_00db9483: (native jump target)
                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                    -- TODO(native): goto LAB_00db9499
                                end
                                ::LAB_00db8d23::
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)auStack_d8;
                            else
                                if quest:GetStateInt("GoodDeedsPerformed") <= __native_entity_state:GetStateInt("PenniesGiven") then
                                    if quest:GetStateInt("BadDeedsPerformed") < 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00db8d23
                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&pppuStack_fc);
                                        fVar13 = quest:GetHealth(nil --[[missing]])
                                        fVar3 = _DAT_0122dedc
                                        if fVar3 < fVar13 then
                                            bVar5 = false
                                            pCVar20 = 0x1
                                            pCVar19 = 0x0
                                            pCVar18 = 0x0
                                            pcVar17 = "TEXT_QST_048_DAD_DO_MORE"
                                            pCVar9 = quest:GetHero()
                                            r5 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar5)
                                            bVar5 = me:IsPerformingScriptTask()
                                            if bVar5 then
                                                repeat
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if not alive then goto LAB_00db974b end
                                                    bVar5 = me:IsPerformingScriptTask()
                                                until not (bVar5)
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00db8d23
                                        end
                                        r6 = quest:GetHero()
                                        -- TODO(native): IsObjectInThingsPossession is not a ForgeFSE binding
                                        quest:IsObjectInThingsPossession()
                                        aVar22 = SUB41(ppVar7,0)
                                        if cVar6 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then goto LAB_00db974b end
                                            quest:ClearThingHasInformation(r6)
                                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xffffff00);
                                            fVar13 = quest:GetHealth(nil --[[missing]])
                                            fVar3 = _DAT_0122dedc
                                            if fVar3 < fVar13 then
                                                bVar5 = false
                                                pCVar20 = 0x1
                                                pCVar19 = 0x0
                                                pCVar18 = 0x0
                                                pcVar17 = "TEXT_QST_048_DAD_GIVE_PRESENT_ALT"
                                                pCVar9 = quest:GetHero()
                                                r7 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar5)
                                                bVar5 = me:IsPerformingScriptTask()
                                                if bVar5 then
                                                    repeat
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then return end  -- TODO(native): goto LAB_00db8d23
                                                        bVar5 = me:IsPerformingScriptTask()
                                                    until not (bVar5)
                                                end
                                                -- TODO(native): goto LAB_00db9474
                                            end
                                        end
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        aVar22 = SUB41(ppVar7,0)
                                        if not alive then goto LAB_00db974b end
                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&pppuStack_fc);
                                        fVar13 = quest:GetHealth(nil --[[missing]])
                                        fVar3 = _DAT_0122dedc
                                        if fVar13 <= fVar3 then return end  -- TODO(native): goto LAB_00db9483
                                        bVar5 = false
                                        pCVar20 = 0x1
                                        pCVar19 = 0x0
                                        pCVar18 = 0x0
                                        pcVar17 = "TEXT_QST_048_DAD_ANTISOCIAL"
                                        pCVar9 = quest:GetHero()
                                        r8 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar5)
                                        bVar5 = me:IsPerformingScriptTask()
                                        if bVar5 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then return end  -- TODO(native): goto LAB_00db8d23
                                                bVar5 = me:IsPerformingScriptTask()
                                            until not (bVar5)
                                        end
                                        -- LAB_00db9474: (native jump target)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00db974b end
                                    end
                                    -- TODO(native): goto LAB_00db9483
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    __native_entity_state:SetStateInt("PenniesGiven", __native_entity_state:GetStateInt("PenniesGiven") + (quest:GetStateInt("GoodDeedsPerformed") - __native_entity_state:GetStateInt("PenniesGiven")))
                                    quest:GiveHeroGold(nil --[[missing]])
                                    if quest:GetStateInt("BadDeedsPerformed") == 0 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if alive then
                                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&pppuStack_fc);
                                            fVar13 = quest:GetHealth(nil --[[missing]])
                                            fVar3 = _DAT_0122dedc
                                            if fVar3 < fVar13 then
                                                bVar5 = false
                                                pCVar20 = 0x1
                                                pCVar19 = 0x0
                                                pCVar18 = 0x0
                                                pcVar17 = "TEXT_QST_048_DAD_GIVE_REWARD_JUST_GOOD"
                                                pCVar9 = quest:GetHero()
                                                r9 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar5)
                                                bVar5 = me:IsPerformingScriptTask()
                                                if bVar5 then
                                                    repeat
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then goto LAB_00db974b end
                                                        bVar5 = me:IsPerformingScriptTask()
                                                    until not (bVar5)
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then return end  -- TODO(native): goto LAB_00db8d23
                                            end
                                            -- LAB_00db8eea: (native jump target)
                                            r10 = quest:GetHero()
                                            -- TODO(native): IsObjectInThingsPossession is not a ForgeFSE binding
                                            quest:IsObjectInThingsPossession()
                                            if not cVar6 then
                                                iVar10 = quest:GetHeroGold()
                                                if 3 < iVar10 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    aVar22 = SUB41(ppVar7,0)
                                                    if alive then
                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&pppuStack_fc);
                                                        fVar13 = quest:GetHealth(r10)
                                                        fVar3 = _DAT_0122dedc
                                                        if fVar3 < fVar13 then
                                                            bVar5 = false
                                                            pCVar20 = 0x1
                                                            pCVar19 = 0x0
                                                            pCVar18 = 0x0
                                                            pcVar17 = "TEXT_QST_048_DAD_YOU_HAVE_ENOUGH"
                                                            pCVar9 = quest:GetHero()
                                                            r11 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar5)
                                                            bVar5 = me:IsPerformingScriptTask()
                                                            if bVar5 then
                                                                repeat
                                                                    alive = quest:NewScriptFrame(me)
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    if not alive then return end  -- TODO(native): goto LAB_00db8d23
                                                                    bVar5 = me:IsPerformingScriptTask()
                                                                until not (bVar5)
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            if not alive then goto LAB_00db974b end
                                                        end
                                                        quest:ClearThingHasInformation(nil --[[missing]])
                                                        -- TODO(native): goto LAB_00db9483
                                                    end
                                                    goto LAB_00db974b
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                if alive then
                                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&pppuStack_fc);
                                                    fVar13 = quest:GetHealth(nil --[[missing]])
                                                    fVar3 = _DAT_0122dedc
                                                    if fVar3 < fVar13 then
                                                        bVar5 = false
                                                        pCVar20 = 0x1
                                                        pCVar19 = 0x0
                                                        pCVar18 = 0x0
                                                        pcVar17 = "TEXT_QST_048_DAD_IS_ENOUGH"
                                                        pCVar9 = quest:GetHero()
                                                        r12 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar5)
                                                        bVar5 = me:IsPerformingScriptTask()
                                                        if bVar5 then
                                                            repeat
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                if not alive then goto LAB_00db974b end
                                                                bVar5 = me:IsPerformingScriptTask()
                                                            until not (bVar5)
                                                        end
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then return end  -- TODO(native): goto LAB_00db8d23
                                                    end
                                                    r13 = quest:GetActiveQuestName()
                                                    quest:SetQuestCardObjective(r13, "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01", "", "")
                                                    -- TODO(native): goto LAB_00db9483
                                                end
                                            else
                                                alive = not quest:IsActiveThreadTerminating()
                                                aVar22 = SUB41(ppVar7,0)
                                                if alive then
                                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&pppuStack_fc);
                                                    fVar13 = quest:GetHealth(nil --[[missing]])
                                                    fVar3 = _DAT_0122dedc
                                                    if fVar3 < fVar13 then
                                                        bVar5 = false
                                                        pCVar20 = 0x1
                                                        pCVar19 = 0x0
                                                        pCVar18 = 0x0
                                                        pcVar17 = "TEXT_QST_048_DAD_GIVE_PRESENT"
                                                        pCVar9 = quest:GetHero()
                                                        r14 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar5)
                                                        bVar5 = me:IsPerformingScriptTask()
                                                        if bVar5 then
                                                            repeat
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                if not alive then goto LAB_00db974b end
                                                                bVar5 = me:IsPerformingScriptTask()
                                                            until not (bVar5)
                                                        end
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then return end  -- TODO(native): goto LAB_00db8d23
                                                    end
                                                    quest:ClearThingHasInformation(nil --[[missing]])
                                                    -- TODO(native): goto LAB_00db9483
                                                end
                                            end
                                        end
                                        -- TODO(native): goto LAB_00db8d23
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&pppuStack_fc);
                                        fVar13 = quest:GetHealth(nil --[[missing]])
                                        fVar3 = _DAT_0122dedc
                                        if fVar3 < fVar13 then
                                            bVar5 = false
                                            pCVar20 = 0x1
                                            pCVar19 = 0x0
                                            pCVar18 = 0x0
                                            pcVar17 = "TEXT_QST_048_DAD_GIVE_REWARD_PART_BAD"
                                            pCVar9 = quest:GetHero()
                                            r15 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar5)
                                            bVar5 = me:IsPerformingScriptTask()
                                            if bVar5 then
                                                repeat
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if not alive then return end  -- TODO(native): goto LAB_00db8d23
                                                    bVar5 = me:IsPerformingScriptTask()
                                                until not (bVar5)
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then goto LAB_00db974b end
                                        end
                                        -- TODO(native): goto LAB_00db8eea
                                    end
                                end
                                ::LAB_00db974b::
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)auStack_d8;
                            end
                            goto LAB_00db977c
                        end
                    end
                end
            end
        end
        ::LAB_00db9785::
        return
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then return end  -- TODO(native): goto LAB_00db9785
    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)auStack_d8);
    ppVar7 = quest:GetHero()
    cVar6 = me:AcquireControl(4)
    while not cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00db8a66 end
        r16 = quest:GetHero()
        cVar6 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00db8a66 end
    -- TODO(native): StdMap_Construct_API();
    -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_d0,(CCharString *)&stack0xffffff14);
    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar21);
    -- TODO(native): pCVar8 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_d0,(CCharString *)&stack0xffffff14);
    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar8,pCVar21);
    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aCStack_8c);
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    quest:FixMovieSequenceCamera(nil --[[missing]])
    ppVar7 = 0x0
    -- TODO(native): RunCutsceneMacro_Func();
    quest:FixMovieSequenceCamera(false)
    quest:SetAllSoundsAsMuted(false)
    quest:SetStateBool("DadFinishedIntro", true)
    quest:Pause(0x3f800000)
    quest:CameraResetToViewBehindHero(0)
    quest:CameraDefault()
    cVar6 = quest:IsXbox()
    if not cVar6 then
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            quest:DisplayGameInfo("TEXT_QST_048_INSTRUCTION_HIGHLIGHTING_PC")
            cVar6 = quest:MsgIsGameInfoClickedPast()
            while not cVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00db8963 end
                cVar6 = quest:MsgIsGameInfoClickedPast()
            end
            -- TODO(native): goto LAB_00db8a3b
        end
        ::LAB_00db8963::
        quest:PauseAllNonScriptedEntities(false)
    else
        alive = not quest:IsActiveThreadTerminating()
        if not alive then return end  -- TODO(native): goto LAB_00db8963
        quest:DisplayGameInfo("TEXT_QST_048_INSTRUCTION_HIGHLIGHTING")
        cVar6 = quest:MsgIsGameInfoClickedPast()
        while not cVar6 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00db8963
            cVar6 = quest:MsgIsGameInfoClickedPast()
        end
        -- LAB_00db8a3b: (native jump target)
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            uVar14 = quest:AddQuestInfoCounter("HUD_DEED_GOOD_ICON", 0, 0x3f800000)
            quest:SetStateInt("GUIGoodDeedCounter", uVar14)
            quest:DisplayQuestInfo(true)
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): StdMap_Destroy_API();
            -- TODO(native): goto LAB_00db8aee
        end
        quest:PauseAllNonScriptedEntities(false)
    end
    -- TODO(native): StdMap_Destroy_API();
    ::LAB_00db8a66::
    do return end
    ::LAB_00db975e::
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    ::LAB_00db9778::
    -- TODO(native): this = aCStack_9c;
    ::LAB_00db977c::
    -- TODO(native): goto LAB_00db9785
    ::LAB_00db9720::
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    -- TODO(native): goto LAB_00db8b00
end

