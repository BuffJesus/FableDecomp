-- Generated from the same native helper bodies as the quest draft.
local helper_E3E320, helper_E3E6B0, helper_E3E720, helper_E44A40, helper_E44CC0
function helper_E3E320(quest, me)
    local bVar3, bVar4, bVar5, bVar6, cVar8, pCVar7
    bVar5 = false
    bVar6 = false
    pCVar7 = quest:GetHero()
    bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_TROUSERS_DRESS")
    if bVar3 then
        goto LAB_00e3e3d9
    else
        bVar5 = true
        bVar6 = false
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_TROUSERS_DRESS_GOOD")
        if bVar3 then goto LAB_00e3e3d9 end
        bVar5 = true
        bVar6 = true
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_TROUSERS_DRESS_EVIL")
        cVar8 = 0
        if bVar3 then goto LAB_00e3e3d9 end
    end
    goto FLOW_past_lab_00e3e3d9
    ::LAB_00e3e3d9::
    cVar8 = 1
    ::FLOW_past_lab_00e3e3d9::
    if bVar6 then
    end
    if bVar5 then
    end
    bVar5 = false
    bVar6 = false
    pCVar7 = quest:GetHero()
    bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_SHIRT_DRESS")
    if bVar3 then
        goto LAB_00e3e4c2
    else
        bVar5 = true
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_SHIRT_DRESS_GOOD")
        if bVar3 then goto LAB_00e3e4c2 end
        bVar5 = true
        bVar6 = true
        pCVar7 = quest:GetHero()
        bVar4 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_SHIRT_DRESS_EVIL")
        bVar3 = false
        if bVar4 then goto LAB_00e3e4c2 end
    end
    goto FLOW_past_lab_00e3e4c2
    ::LAB_00e3e4c2::
    bVar3 = true
    ::FLOW_past_lab_00e3e4c2::
    if bVar6 then
    end
    if bVar5 then
    end
    bVar6 = false
    if bVar3 then
        cVar8 = cVar8 + 1
    end
    pCVar7 = quest:GetHero()
    bVar5 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_GLOVES_DRESS")
    if bVar5 then
        goto LAB_00e3e5ad
    else
        pCVar7 = quest:GetHero()
        bVar5 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_GLOVES_DRESS_GOOD")
        if bVar5 then goto LAB_00e3e5ad end
        bVar6 = true
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_GLOVES_DRESS_EVIL")
        bVar5 = false
        if bVar3 then goto LAB_00e3e5ad end
    end
    goto FLOW_past_lab_00e3e5ad
    ::LAB_00e3e5ad::
    bVar5 = true
    ::FLOW_past_lab_00e3e5ad::
    if bVar6 then
    end
    if bVar5 then
        cVar8 = cVar8 + 1
    end
    pCVar7 = quest:GetHero()
    bVar6 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_HAT_WHOREWIG")
    if bVar6 then
        pCVar7 = quest:GetHero()
        bVar5 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_NO_BOOTS")
        bVar6 = true
        if bVar5 then goto LAB_00e3e66d end
    end
    bVar6 = false
    ::LAB_00e3e66d::
    if bVar6 then
        cVar8 = cVar8 + 1
    end
    return cVar8 == '\x04'
end

function helper_E3E6B0(quest, me)
    local bVar1
    if quest:GetStateBool("HeroTricking") then
        bVar1 = helper_E3E320(quest, me)
        if bVar1 then
            return "_HEROWHORE"
        end
    end
    bVar1 = helper_E3E320(quest, me)
    if bVar1 then
        return "_HEROLADY"
    end
    return "_HEROMAN"
end

function helper_E3E720(quest, me, native_arg_param_2, native_arg_param_3)
    local resources = quest:RetailResources()
    local bVar6
    local alive = true
    quest:SetStateBool("CutscenePlaying", true)
    local xStack_10 = resources:NewResource()
    local pScriptObject = xStack_10
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_1c = resources:NewActorMap()
    resources:SetActor(xStack_1c, "HERO", xStack_10)
    resources:SetActor(xStack_1c, "BOSS", resources:MemberResource("seh_Boss"))
    resources:SetActor(xStack_1c, "GUARD", resources:MemberResource("seh_Guard"))
    resources:SetActor(xStack_1c, "MADAM", resources:MemberResource("seh_Madam"))
    resources:SetActor(xStack_1c, "WHORE", resources:MemberResource("seh_Whore"))
    quest:FixMovieSequenceCamera(true)
    local cVar5 = native_arg_param_3
    if native_arg_param_3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            resources:DestroyActorMap(xStack_1c)
            resources:ReleaseResource(xStack_10)
            return
        end
        -- TODO(native): SetCutsceneSkippableWhilePaused is not a ForgeFSE binding
        quest:SetCutsceneSkippableWhilePaused(true)
    end
    resources:RunMacroWithStrings(native_arg_param_2, xStack_1c, resources:MemberStringMap("csargs"), false, true)
    if cVar5 then
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            -- LAB_00e3ea29: (native jump target)
            resources:DestroyActorMap(xStack_1c)
            resources:ReleaseResource(xStack_10)
            return
        end
        -- TODO(native): SetCutsceneSkippableWhilePaused is not a ForgeFSE binding
        quest:SetCutsceneSkippableWhilePaused(false)
    end
    quest:FixMovieSequenceCamera(false)
    quest:SetStateBool("CutscenePlaying", false)
    resources:ClearStringMap(resources:MemberStringMap("csargs"))
    resources:DestroyActorMap(xStack_1c)
    resources:ReleaseResource(xStack_10)
end

function helper_E44A40(quest, me)
    local bVar3, bVar5, b_stk_21, pCVar4
    bVar5 = 1
    pCVar4 = quest:GetHero()
    bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_01")
    if not bVar3 then
        bVar5 = 3
        pCVar4 = quest:GetHero()
        bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_02")
        if not bVar3 then
            bVar5 = 7
            pCVar4 = quest:GetHero()
            bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_03")
            if not bVar3 then
                bVar5 = 0xf
                pCVar4 = quest:GetHero()
                bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_MUTTON_01")
                if not bVar3 then
                    bVar5 = 0x1f
                    pCVar4 = quest:GetHero()
                    bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_LONG_01")
                    if not bVar3 then
                        bVar5 = 0x3f
                        pCVar4 = quest:GetHero()
                        bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_CHIN_01")
                        if not bVar3 then
                            bVar5 = 0x7f
                            pCVar4 = quest:GetHero()
                            bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_TRAMP_01")
                            if not bVar3 then
                                bVar5 = 0xff
                                pCVar4 = quest:GetHero()
                                bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_WATSON_01")
                                b_stk_21 = false
                                if not bVar3 then goto LAB_00e44c26 end
                            end
                        end
                    end
                end
            end
        end
    end
    b_stk_21 = true
    ::LAB_00e44c26::
    if bVar5 < 0 then
        bVar5 = bVar5 & 0x7f
    end
    if (bVar5 & 0x40) ~= 0 then
        bVar5 = bVar5 & 0xbf
    end
    if (bVar5 & 0x20) ~= 0 then
        bVar5 = bVar5 & 0xdf
    end
    if (bVar5 & 0x10) ~= 0 then
        bVar5 = bVar5 & 0xef
    end
    if (bVar5 & 8) ~= 0 then
        bVar5 = bVar5 & 0xf7
    end
    if (bVar5 & 4) ~= 0 then
        bVar5 = bVar5 & 0xfb
    end
    if (bVar5 & 2) ~= 0 then
        bVar5 = bVar5 & 0xfd
    end
    return b_stk_21
end

function helper_E44CC0(quest, me)
    local bVar3, bVar4, bVar5, bVar6, bVar7, bVar8, b_stk_19, pCVar9
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
                        b_stk_19 = false
                        if not bVar8 then goto LAB_00e44e30 end
                    end
                end
            end
        end
    end
    b_stk_19 = true
    ::LAB_00e44e30::
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
    return b_stk_19
end

return {helper_E3E320 = helper_E3E320, helper_E3E6B0 = helper_E3E6B0, helper_E3E720 = helper_E3E720, helper_E44A40 = helper_E44A40, helper_E44CC0 = helper_E44CC0}
