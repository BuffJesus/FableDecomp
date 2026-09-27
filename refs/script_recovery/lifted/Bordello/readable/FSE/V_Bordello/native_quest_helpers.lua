-- Generated from the same native helper bodies as the quest draft.
local IsHeroWearingBeard, GetHeroStatusTextTag, PlayCutscene, IsHeroWearingTash
-- E3E320: bsim names this body NScript::CV_BordelloScript::IsHeroWearingBeard (a homologous script member); no PDB name
function IsHeroWearingBeard(quest, me)
    local predicateResult, predicateResult2, predicateResult3, scratchValue
local hero = quest:GetHero()
    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_DRESS") then
        goto LAB_00e3e3d9
    else
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_DRESS_GOOD") then goto LAB_00e3e3d9 end
        scratchValue = 0
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_TROUSERS_DRESS_EVIL") then goto LAB_00e3e3d9 end
    end
    goto FLOW_past_lab_00e3e3d9
    ::LAB_00e3e3d9::
    scratchValue = 1
    ::FLOW_past_lab_00e3e3d9::
    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_SHIRT_DRESS") then
        goto LAB_00e3e4c2
    else
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_SHIRT_DRESS_GOOD") then goto LAB_00e3e4c2 end
        predicateResult = false
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_SHIRT_DRESS_EVIL") then goto LAB_00e3e4c2 end
    end
    goto FLOW_past_lab_00e3e4c2
    ::LAB_00e3e4c2::
    predicateResult = true
    ::FLOW_past_lab_00e3e4c2::
    if predicateResult then
        scratchValue = scratchValue + 1
    end
    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_GLOVES_DRESS") then
        goto LAB_00e3e5ad
    else
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_GLOVES_DRESS_GOOD") then goto LAB_00e3e5ad end
        predicateResult2 = false
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_GLOVES_DRESS_EVIL") then goto LAB_00e3e5ad end
    end
    goto FLOW_past_lab_00e3e5ad
    ::LAB_00e3e5ad::
    predicateResult2 = true
    ::FLOW_past_lab_00e3e5ad::
    if predicateResult2 then
        scratchValue = scratchValue + 1
    end
    if quest:IsWearingClothingItem(hero, "OBJECT_HERO_HAT_WHOREWIG") then
        predicateResult3 = true
        if quest:IsWearingClothingItem(hero, "OBJECT_HERO_NO_BOOTS") then goto LAB_00e3e66d end
    end
    predicateResult3 = false
    ::LAB_00e3e66d::
    if predicateResult3 then
        scratchValue = scratchValue + 1
    end
    return scratchValue == '\x04'
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
    resources:TryAcquire(resource, quest:GetHero(), 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "BOSS", resources:MemberResource("seh_Boss"))
    resources:SetActor(actorMap, "GUARD", resources:MemberResource("seh_Guard"))
    resources:SetActor(actorMap, "MADAM", resources:MemberResource("seh_Madam"))
    resources:SetActor(actorMap, "WHORE", resources:MemberResource("seh_Whore"))
    quest:FixMovieSequenceCamera(true)
    if param3 then
        if quest:IsActiveThreadTerminating() then
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            return
        end
        -- TODO(native): SetCutsceneSkippableWhilePaused is not a ForgeFSE binding
        quest:SetCutsceneSkippableWhilePaused(true)
    end
    resources:RunMacroWithStrings(param2, actorMap, resources:MemberStringMap("csargs"), false, true)
    if param3 then
        if quest:IsActiveThreadTerminating() then
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            return
        end
        -- TODO(native): SetCutsceneSkippableWhilePaused is not a ForgeFSE binding
        quest:SetCutsceneSkippableWhilePaused(false)
    end
    quest:FixMovieSequenceCamera(false)
    quest:SetStateBool("CutscenePlaying", false)
    resources:ClearStringMap(resources:MemberStringMap("csargs"))
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

-- E44CC0: bsim names this body NScript::CV_BordelloScript::IsHeroWearingTash (a homologous script member); no PDB name
function IsHeroWearingTash(quest, me)
    local isWearingHairstyle
    local hero = quest:GetHero()
    isWearingHairstyle = quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMITH_01")
    if isWearingHairstyle then return end
    isWearingHairstyle = quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHTRADER_01")
    if isWearingHairstyle then return end
    isWearingHairstyle = quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHKHG_01")
    if isWearingHairstyle then return end
    isWearingHairstyle = quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSHERIFF_01")
    if isWearingHairstyle then return end
    isWearingHairstyle = quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHCHINESE_01")
    if isWearingHairstyle then return end
    isWearingHairstyle = quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMALL_01")
    if not isWearingHairstyle then return end
    return
end

return {IsHeroWearingBeard = IsHeroWearingBeard, GetHeroStatusTextTag = GetHeroStatusTextTag, PlayCutscene = PlayCutscene, IsHeroWearingTash = IsHeroWearingTash}
