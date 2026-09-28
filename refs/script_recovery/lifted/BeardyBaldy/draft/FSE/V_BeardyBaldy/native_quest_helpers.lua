-- Generated from the same native helper bodies as the quest draft.
local IsHeroWearingAnyOddHairdo, IsHeroWearingAnyTash, SetWanderPointAndDistance, helper_E53C70
function IsHeroWearingAnyOddHairdo(quest, me)
    local bVar3, bVar4, bVar5, bVar6, bVar7, pCVar8, u_stk_15
    bVar6 = false
    bVar5 = false
    bVar4 = false
    bVar3 = false
    pCVar8 = quest:GetHero()
    bVar7 = quest:IsWearingHairstyle(pCVar8, "OBJECT_HERO_HAIR_BUZZ_01")
    if not bVar7 then
        bVar6 = true
        bVar5 = false
        bVar4 = false
        bVar3 = false
        pCVar8 = quest:GetHero()
        bVar7 = quest:IsWearingHairstyle(pCVar8, "OBJECT_HERO_HAIR_BASIN_01")
        if not bVar7 then
            bVar6 = true
            bVar5 = true
            bVar4 = false
            bVar3 = false
            pCVar8 = quest:GetHero()
            bVar7 = quest:IsWearingHairstyle(pCVar8, "OBJECT_HERO_HAIR_MOHAWK_01")
            if not bVar7 then
                bVar6 = true
                bVar5 = true
                bVar4 = true
                bVar3 = false
                pCVar8 = quest:GetHero()
                bVar7 = quest:IsWearingHairstyle(pCVar8, "OBJECT_HERO_HAIR_PONYTAIL_01")
                if not bVar7 then
                    bVar6 = true
                    bVar5 = true
                    bVar4 = true
                    bVar3 = true
                    pCVar8 = quest:GetHero()
                    bVar7 = quest:IsWearingHairstyle(pCVar8, "OBJECT_HERO_HAIR_PLATS_01")
                    u_stk_15 = false
                    if not bVar7 then goto LAB_00e53c05 end
                end
            end
        end
    end
    u_stk_15 = true
    ::LAB_00e53c05::
    if bVar3 then
    end
    if bVar4 then
    end
    if bVar5 then
    end
    if bVar6 then
    end
    return u_stk_15
end

function IsHeroWearingAnyTash(quest, me)
    local bVar3, bVar4, bVar5, bVar6, bVar7, bVar8, pCVar9, u_stk_19
    bVar7 = false
    bVar6 = false
    bVar5 = false
    bVar4 = false
    bVar3 = false
    pCVar9 = quest:GetHero()
    bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHSMITH_01")
    if not bVar8 then
        bVar7 = true
        bVar6 = false
        bVar5 = false
        bVar4 = false
        bVar3 = false
        pCVar9 = quest:GetHero()
        bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHTRADER_01")
        if not bVar8 then
            bVar7 = true
            bVar6 = true
            bVar5 = false
            bVar4 = false
            bVar3 = false
            pCVar9 = quest:GetHero()
            bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHKHG_01")
            if not bVar8 then
                bVar7 = true
                bVar6 = true
                bVar5 = true
                bVar4 = false
                bVar3 = false
                pCVar9 = quest:GetHero()
                bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHSHERIFF_01")
                if not bVar8 then
                    bVar7 = true
                    bVar6 = true
                    bVar5 = true
                    bVar4 = true
                    bVar3 = false
                    pCVar9 = quest:GetHero()
                    bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHCHINESE_01")
                    if not bVar8 then
                        bVar7 = true
                        bVar6 = true
                        bVar5 = true
                        bVar4 = true
                        bVar3 = true
                        pCVar9 = quest:GetHero()
                        bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHSMALL_01")
                        u_stk_19 = false
                        if not bVar8 then goto LAB_00e53a60 end
                    end
                end
            end
        end
    end
    u_stk_19 = true
    ::LAB_00e53a60::
    if bVar3 then
    end
    if bVar4 then
    end
    if bVar5 then
    end
    if bVar6 then
    end
    if bVar7 then
    end
    return u_stk_19
end

function SetWanderPointAndDistance(quest, me, native_arg_param_1, native_arg_param_2)
    local center = {x = native_arg_param_2.x, y = native_arg_param_2.y, z = native_arg_param_2.z}
    quest:SetWanderCentrePoint(native_arg_param_1, center)
    local fVar3 = quest:ReadGlobalGameDataFloat(0x448)
    quest:SetWanderMinDistance(native_arg_param_1, fVar3)
    fVar3 = quest:ReadGlobalGameDataFloat(0x44c)
    quest:SetWanderMaxDistance(native_arg_param_1, fVar3)
    quest:SetScriptingStateGroup(native_arg_param_1, 4)
end

function helper_E53C70(quest, me)
    local iVar2 = math.random(0, 32767)
    quest:SetTimer(quest:GetStateInt("RandomSpeechTimer"), quest:ReadGlobalGameData(0x494) - iVar2 % quest:ReadGlobalGameData(0x498))
end

return {IsHeroWearingAnyOddHairdo = IsHeroWearingAnyOddHairdo, IsHeroWearingAnyTash = IsHeroWearingAnyTash, SetWanderPointAndDistance = SetWanderPointAndDistance, helper_E53C70 = helper_E53C70}
