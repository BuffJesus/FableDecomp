-- Generated from the same native helper bodies as the quest draft.
local IsHeroWearingBeard, GetHeroStatusTextTag, PlayCutscene, helper_E44A40, IsHeroWearingTash
-- E3E320: bsim names this body NScript::CV_BordelloScript::IsHeroWearingBeard (a homologous script member); no PDB name
function IsHeroWearingBeard(quest, me)
    local predicateResult, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    local predicateResult6, predicateResult7, predicateResult8, scratchValue
    predicateResult2 = false
    predicateResult5 = false
    local hero = quest:GetHero()
    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_DRESS") then
        goto LAB_00e3e3d9
    else
        predicateResult2 = true
        predicateResult5 = false
        local hero2 = quest:GetHero()
        if quest:IsWearingClothingItem(hero2, "OBJECT_HERO_TROUSERS_DRESS_GOOD") then goto LAB_00e3e3d9 end
        predicateResult2 = true
        predicateResult5 = true
        local hero3 = quest:GetHero()
        scratchValue = 0
        if quest:IsWearingClothingItem(hero3, "OBJECT_HERO_TROUSERS_DRESS_EVIL") then goto LAB_00e3e3d9 end
    end
    goto FLOW_past_lab_00e3e3d9
    ::LAB_00e3e3d9::
    scratchValue = 1
    ::FLOW_past_lab_00e3e3d9::
    if predicateResult5 then
    end
    if predicateResult2 then
    end
    predicateResult3 = false
    predicateResult6 = false
    local hero4 = quest:GetHero()
    if quest:IsWearingClothingItem(hero4, "OBJECT_HERO_SHIRT_DRESS") then
        goto LAB_00e3e4c2
    else
        predicateResult3 = true
        local hero5 = quest:GetHero()
        if quest:IsWearingClothingItem(hero5, "OBJECT_HERO_SHIRT_DRESS_GOOD") then goto LAB_00e3e4c2 end
        predicateResult3 = true
        predicateResult6 = true
        local hero6 = quest:GetHero()
        predicateResult = false
        if quest:IsWearingClothingItem(hero6, "OBJECT_HERO_SHIRT_DRESS_EVIL") then goto LAB_00e3e4c2 end
    end
    goto FLOW_past_lab_00e3e4c2
    ::LAB_00e3e4c2::
    predicateResult = true
    ::FLOW_past_lab_00e3e4c2::
    if predicateResult6 then
    end
    if predicateResult3 then
    end
    predicateResult7 = false
    if predicateResult then
        scratchValue = scratchValue + 1
    end
    local hero7 = quest:GetHero()
    if quest:IsWearingClothingItem(hero7, "OBJECT_HERO_GLOVES_DRESS") then
        goto LAB_00e3e5ad
    else
        local hero8 = quest:GetHero()
        if quest:IsWearingClothingItem(hero8, "OBJECT_HERO_GLOVES_DRESS_GOOD") then goto LAB_00e3e5ad end
        predicateResult7 = true
        local hero9 = quest:GetHero()
        predicateResult4 = false
        if quest:IsWearingClothingItem(hero9, "OBJECT_HERO_GLOVES_DRESS_EVIL") then goto LAB_00e3e5ad end
    end
    goto FLOW_past_lab_00e3e5ad
    ::LAB_00e3e5ad::
    predicateResult4 = true
    ::FLOW_past_lab_00e3e5ad::
    if predicateResult7 then
    end
    if predicateResult4 then
        scratchValue = scratchValue + 1
    end
    local hero10 = quest:GetHero()
    if quest:IsWearingClothingItem(hero10, "OBJECT_HERO_HAT_WHOREWIG") then
        local hero11 = quest:GetHero()
        predicateResult8 = true
        if quest:IsWearingClothingItem(hero11, "OBJECT_HERO_NO_BOOTS") then goto LAB_00e3e66d end
    end
    predicateResult8 = false
    ::LAB_00e3e66d::
    if predicateResult8 then
        scratchValue = scratchValue + 1
    end
    return scratchValue == 4
end

-- E3E6B0: bsim names this body NScript::CV_BordelloScript::GetHeroStatusTextTag (a homologous script member); no PDB name
function GetHeroStatusTextTag(quest, me)
    if quest:GetStateBool("HeroTricking") then
        if IsHeroWearingBeard(quest, me) then
            return "_HEROWHORE"
        end
    end
    if IsHeroWearingBeard(quest, me) then
        return "_HEROLADY"
    end
    return "_HEROMAN"
end

-- E3E720: bsim names this body NScript::CV_BordelloScript::PlayCutscene (a homologous script member); no PDB name
function PlayCutscene(quest, me, param2, param3)
    local resources = quest:RetailResources()
    quest:SetStateBool("CutscenePlaying", true)
    local resource = resources:NewResource()
    local pThing = quest:GetHero()
    resources:TryAcquire(resource, pThing, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "BOSS", resources:MemberResource("seh_Boss"))
    resources:SetActor(actorMap, "GUARD", resources:MemberResource("seh_Guard"))
    resources:SetActor(actorMap, "MADAM", resources:MemberResource("seh_Madam"))
    resources:SetActor(actorMap, "WHORE", resources:MemberResource("seh_Whore"))
    quest:FixMovieSequenceCamera(true)
    local scratchValue = param3
    if param3 then
        if quest:IsActiveThreadTerminating() then
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            return
        end
        quest:SetCutsceneSkippableWhilePaused(true)
    end
    resources:RunMacroWithStrings(param2, actorMap, resources:MemberStringMap("csargs"), false, true)
    if scratchValue then
        if quest:IsActiveThreadTerminating() then
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            return
        end
        quest:SetCutsceneSkippableWhilePaused(false)
    end
    quest:FixMovieSequenceCamera(false)
    quest:SetStateBool("CutscenePlaying", false)
    resources:ClearStringMap(resources:MemberStringMap("csargs"))
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

function helper_E44A40(quest, me)
    local flags, scratchValue, predicateResult
    predicateResult = true
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_01") then
        local hero2 = quest:GetHero()
        if not quest:IsWearingHairstyle(hero2, "OBJECT_HERO_BEARD_02") then
            local hero3 = quest:GetHero()
            if not quest:IsWearingHairstyle(hero3, "OBJECT_HERO_BEARD_03") then
                local hero4 = quest:GetHero()
                if not quest:IsWearingHairstyle(hero4, "OBJECT_HERO_BEARD_MUTTON_01") then
                    local hero5 = quest:GetHero()
                    if not quest:IsWearingHairstyle(hero5, "OBJECT_HERO_BEARD_LONG_01") then
                        local hero6 = quest:GetHero()
                        if not quest:IsWearingHairstyle(hero6, "OBJECT_HERO_BEARD_CHIN_01") then
                            local hero7 = quest:GetHero()
                            if not quest:IsWearingHairstyle(hero7, "OBJECT_HERO_BEARD_TRAMP_01") then
                                local hero8 = quest:GetHero()
                                predicateResult = not not quest:IsWearingHairstyle(hero8, "OBJECT_HERO_BEARD_WATSON_01")
                            end
                        end
                    end
                end
            end
        end
    end
    return predicateResult
end

-- E44CC0: bsim names this body NScript::CV_BordelloScript::IsHeroWearingTash (a homologous script member); no PDB name
function IsHeroWearingTash(quest, me)
    local predicateResult, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    local isWearingHairstyle, predicateResult6, hero
    predicateResult5 = false
    predicateResult4 = false
    predicateResult3 = false
    predicateResult2 = false
    predicateResult = false
    hero = quest:GetHero()
    isWearingHairstyle = quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMITH_01")
    if not isWearingHairstyle then
        predicateResult5 = true
        predicateResult4 = false
        predicateResult3 = false
        predicateResult2 = false
        predicateResult = false
        hero = quest:GetHero()
        isWearingHairstyle = quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHTRADER_01")
        if not isWearingHairstyle then
            predicateResult5 = true
            predicateResult4 = true
            predicateResult3 = false
            predicateResult2 = false
            predicateResult = false
            hero = quest:GetHero()
            isWearingHairstyle = quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHKHG_01")
            if not isWearingHairstyle then
                predicateResult5 = true
                predicateResult4 = true
                predicateResult3 = true
                predicateResult2 = false
                predicateResult = false
                hero = quest:GetHero()
                isWearingHairstyle = quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSHERIFF_01")
                if not isWearingHairstyle then
                    predicateResult5 = true
                    predicateResult4 = true
                    predicateResult3 = true
                    predicateResult2 = true
                    predicateResult = false
                    hero = quest:GetHero()
                    isWearingHairstyle = quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHCHINESE_01")
                    if not isWearingHairstyle then
                        predicateResult5 = true
                        predicateResult4 = true
                        predicateResult3 = true
                        predicateResult2 = true
                        predicateResult = true
                        hero = quest:GetHero()
                        isWearingHairstyle = quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMALL_01")
                        predicateResult6 = false
                        if not isWearingHairstyle then goto LAB_00e44e30 end
                    end
                end
            end
        end
    end
    predicateResult6 = true
    ::LAB_00e44e30::
    if predicateResult then
    end
    if predicateResult2 then
    end
    if predicateResult3 then
    end
    if predicateResult4 then
    end
    if predicateResult5 then
    end
    return predicateResult6
end

return {IsHeroWearingBeard = IsHeroWearingBeard, GetHeroStatusTextTag = GetHeroStatusTextTag, PlayCutscene = PlayCutscene, helper_E44A40 = helper_E44A40, IsHeroWearingTash = IsHeroWearingTash}
