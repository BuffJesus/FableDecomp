-- Generated native draft: NOVI_Villager. Review coverage report before use.
-- Not copied from the working port; registration remains disabled.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

local __native_vectors = {
    [0xd8] = {"TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_10", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_20", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_30", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_40", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_50", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_60"},
    [0xa8] = {"TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_10", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_20", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_30", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_40", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_50", "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_60"},
    [0xe4] = {"TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_10", "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_20", "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_30", "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_40"},
    [0xb4] = {"TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_10", "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_20", "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_30", "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_40"},
    [0xcc] = {"TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_10", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_20", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_30", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_40", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_50", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_60"},
    [0x9c] = {"TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_10", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_20", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_30", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_40", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_50", "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_60"},
    [0xf0] = {"TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_10", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_20", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_30", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_40", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_50"},
    [0xc0] = {"TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_10", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_20", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_30", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_40", "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_50"},
}

function Init(quest, me)
    __native_entity_state:SetStateBool("HeroDidHitMe", false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    local hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, hero)
end

function Main(quest, me)
    local __native_condition_1, bVar5, cVar6, fVar16, fVar24, fVar3, iVar12, iVar8, pCVar1, pCVar18, pCVar19, pCVar20, pCVar25, pCVar9, pcVar17, ppVar10, ppVar21, r1, r2, r3, uVar11, uVar15, uVar22, uVar23, uVar4, uVar7
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    alive = not quest:IsActiveThreadTerminating()
    uVar4 = 0
    cVar6 = extraout_AL_00
    repeat
        if cVar6 ~= 0 then
            return
        end
        -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)appuStack_20);
        if bVar5 then
        end
        pCVar25 = "SCRIPT_NAME_HERO"
        uVar15 = uVar4 | 1
        cVar6 = me:MsgIsHitByHero()
        if not cVar6 then
            uVar15 = uVar4 | 3
            cVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if cVar6 then
                uVar15 = uVar4 | 7
                cVar6 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not cVar6 then return end  -- TODO(native): goto LAB_00dae0aa
            end
            bVar5 = false
        else
            -- LAB_00dae0aa: (native jump target)
            bVar5 = true
        end
        if (uVar15 & 4) ~= 0 then
            uVar15 = uVar15 & 0xfffffffb
        end
        if (uVar15 & 2) ~= 0 then
            uVar15 = uVar15 & 0xfffffffd
        end
        if (uVar15 & 1) ~= 0 then
            -- TODO(native): uStack_64 = uVar15 & 0xfffffffe;
        end
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00daea49
            uVar7 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, uVar7)
            uVar7 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(uVar7, nil --[[missing]])
            require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
            __native_entity_state:SetStateBool("HeroDidHitMe", true)
            -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh(aCStack_34);
            if bVar5 then
            end
            cVar6 = me:AcquireControl(4)
            while not cVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00daea49
                cVar6 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00daea49
            pCVar18 = ""
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            iVar8 = quest:EntityGetSex(nil --[[missing]])
            uVar22 = SUB41(me,0)
            if iVar8 == 1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): goto LAB_00daea49
                end
                -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)aCStack_34);
                fVar16 = quest:GetHealth(nil --[[missing]])
                fVar3 = 0.0
                if fVar3 < fVar16 then
                    bVar5 = false
                    pCVar20 = 0x1
                    pCVar19 = 0x0
                    pCVar18 = 0x1
                    pcVar17 = "TEXT_QST_048_VILLAGER_ATTACKED_MALE"
                    pCVar9 = quest:GetHero()
                    r1 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar5)
                    bVar5 = me:IsPerformingScriptTask()
                    if bVar5 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00daea2a
                            bVar5 = me:IsPerformingScriptTask()
                        until not (bVar5)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): goto LAB_00daea49
                    end
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00daea2a: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): goto LAB_00daea49
                end
                -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)aCStack_34);
                fVar16 = quest:GetHealth(nil --[[missing]])
                fVar3 = 0.0
                if fVar3 < fVar16 then
                    bVar5 = false
                    pCVar20 = 0x1
                    pCVar19 = 0x0
                    pCVar18 = 0x1
                    pcVar17 = "TEXT_QST_048_VILLAGER_ATTACKED_FEMALE"
                    pCVar9 = quest:GetHero()
                    r2 = me:Speak(pCVar9, pcVar17, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar5)
                    bVar5 = me:IsPerformingScriptTask()
                    if bVar5 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                -- TODO(native): goto LAB_00daea49
                            end
                            bVar5 = me:IsPerformingScriptTask()
                        until not (bVar5)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00daea2a
                end
            end
            quest:PauseAllNonScriptedEntities(false)
        else
            cVar6 = me:IsTalkedToByHero()
            alive = not quest:IsActiveThreadTerminating()
            if cVar6 then
                if not alive then return end  -- TODO(native): goto LAB_00daea49
                iVar8 = quest:EntityGetSex(me)
                if iVar8 == 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        pcVar17 = "_MALE"
                        -- LAB_00dae43d: (native jump target)
                        -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh(aCStack_24);
                        if bVar5 then
                        end
                        cVar6 = me:AcquireControl(4)
                        while not cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00daea44 end
                            cVar6 = me:AcquireControl(4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            uVar7 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(me, uVar7)
                            ppVar10 = quest:AddNewConversation(me, false, false)
                            uVar7 = quest:GetHero()
                            quest:AddPersonToConversation(ppVar10, uVar7)
                            ppVar21 = ppVar10
                            if not __native_entity_state:GetStateBool("HeroDidHitMe") then
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    uVar7 = quest:GetHero()
                                    uVar23 = 0
                                    pCVar18 = pcVar17
                                    uVar11 = CCharString__AppendData(pCVar18)
                                    quest:AddLineToConversation(ppVar10, uVar11, me, uVar7, (uVar23 ~= 0))
                                    -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_54;
                                    -- TODO(native): goto LAB_00dae59d
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    uVar7 = quest:GetHero()
                                    uVar23 = 0
                                    pCVar18 = pcVar17
                                    uVar11 = CCharString__AppendData(pCVar18)
                                    quest:AddLineToConversation(ppVar10, uVar11, me, uVar7, (uVar23 ~= 0))
                                    -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *) aCStack_5c;
                                    -- LAB_00dae59d: (native jump target)
                                    cVar6 = quest:IsConversationActive(ppVar10)
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00daea44 end
                                        cVar6 = quest:IsConversationActive(ppVar10)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        -- TODO(native): bVar5 = C3DMeshInfo::HasPhysicsMesh(aCStack_24);
                                        if bVar5 then
                                        end
                                        goto LAB_00dae98e
                                    end
                                end
                            end
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        pcVar17 = "_FEMALE"
                        -- TODO(native): goto LAB_00dae43d
                    end
                end
                ::LAB_00daea44::
                -- LAB_00daea49: (native jump target)
                return
            end
            if not alive then return end  -- TODO(native): goto LAB_00daea49
            iVar8 = quest:GetTimer(quest:GetStateInt("TalkIntermittentTimer"))
            __native_condition_1 = iVar8 == 0
            if __native_condition_1 then
                iVar8 = quest:RetailRandModulo(100)
                __native_condition_1 = iVar8 == 0
            end
            if __native_condition_1 then
                fVar24 = 5.0
                pCVar9 = quest:GetHero()
                bVar5 = quest:IsDistanceBetweenThingsUnder(me, pCVar9, fVar24)
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00daea49
                    quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 3)
                    uVar7 = quest:AddNewConversation(me, false, false)
                    uVar11 = quest:GetHero()
                    quest:AddPersonToConversation(uVar7, uVar11)
                    iVar12 = quest:GetStateInt("BadDeedsPerformed")
                    if (iVar12 == 0) and (0 < quest:GetStateInt("GoodDeedsPerformed")) then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00daea49
                        iVar8 = quest:EntityGetSex(me)
                        if iVar8 == 1 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00daea49
                            iVar12 = GetVillagerSpeechIndex(quest, me, 6)
                            iVar8 = __native_vectors[0x9c]
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00daea49
                            iVar12 = GetVillagerSpeechIndex(quest, me, 6)
                            iVar8 = __native_vectors[0xcc]
                        end
                    elseif (iVar12 < 1) or (quest:GetStateInt("GoodDeedsPerformed") ~= 0) then
                        if (iVar12 < 1) or (quest:GetStateInt("GoodDeedsPerformed") < 1) then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00daea49
                            iVar8 = quest:EntityGetSex(me)
                            if iVar8 == 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00daea49
                                iVar12 = GetVillagerSpeechIndex(quest, me, 5)
                                iVar8 = __native_vectors[0xc0]
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00daea49
                                iVar12 = GetVillagerSpeechIndex(quest, me, 5)
                                iVar8 = __native_vectors[0xf0]
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00daea49
                            iVar8 = quest:EntityGetSex(me)
                            if iVar8 == 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00daea49
                                iVar12 = GetVillagerSpeechIndex(quest, me, 4)
                                iVar8 = __native_vectors[0xb4]
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00daea49
                                iVar12 = GetVillagerSpeechIndex(quest, me, 4)
                                iVar8 = __native_vectors[0xe4]
                            end
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00daea49
                        iVar8 = quest:EntityGetSex(me)
                        if iVar8 == 1 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00daea49
                            iVar12 = GetVillagerSpeechIndex(quest, me, 6)
                            iVar8 = __native_vectors[0xa8]
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00daea49
                            iVar12 = GetVillagerSpeechIndex(quest, me, 6)
                            iVar8 = __native_vectors[0xd8]
                        end
                    end
                    r3 = iVar8[iVar12 + 1]
                    uVar11 = quest:GetHero()
                    quest:AddLineToConversation(uVar7, r3, me, uVar11, false)
                end
            end
        end
        ::LAB_00dae98e::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cVar6 = extraout_AL_32
    until false
end

function GetVillagerSpeechIndex(quest, me, native_arg_param_1)
    local uVar3
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    local cVar1 = not alive
    while true do
        if cVar1 then
            return 0
        end
        uVar3 = quest:RetailRandModulo(native_arg_param_1)
        if uVar3 ~= quest:GetStateInt("lastVillagerSpeechIdx") then break end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cVar1 = not alive
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return 0
    end
    quest:SetStateInt("lastVillagerSpeechIdx", uVar3)
    alive = not quest:IsActiveThreadTerminating()
    return ((not (not alive)) and uVar3 or 0)
end

