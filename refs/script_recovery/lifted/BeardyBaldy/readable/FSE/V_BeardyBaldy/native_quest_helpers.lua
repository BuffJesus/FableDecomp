-- Generated from the same native helper bodies as the quest draft.
local IsHeroWearingAnyOddHairdo, IsHeroWearingAnyTash, ResetRandomSpeechTime
function IsHeroWearingAnyOddHairdo(quest, me)
    local predicateResult
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_BUZZ_01") then
        if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_BASIN_01") then
            if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_MOHAWK_01") then
                if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_PONYTAIL_01") then
                    predicateResult = false
                    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_PLATS_01") then goto LAB_00e53c05 end
                end
            end
        end
    end
    predicateResult = true
    ::LAB_00e53c05::
    return predicateResult
end

function IsHeroWearingAnyTash(quest, me)
    local predicateResult
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMITH_01") then
        if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHTRADER_01") then
            if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHKHG_01") then
                if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSHERIFF_01") then
                    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHCHINESE_01") then
                        predicateResult = false
                        if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMALL_01") then goto LAB_00e53a60 end
                    end
                end
            end
        end
    end
    predicateResult = true
    ::LAB_00e53a60::
    return predicateResult
end

-- E53C70: bsim names this body NScript::CV_AmbushScamScript::ResetRandomSpeechTime (a homologous script member); no PDB name
function ResetRandomSpeechTime(quest, me)
    local scratchValue = math.random(0, 32767)
    quest:SetTimer(quest:GetStateInt("RandomSpeechTimer"), quest:ReadGlobalGameData(1172) - scratchValue % quest:ReadGlobalGameData(1176))
end

return {IsHeroWearingAnyOddHairdo = IsHeroWearingAnyOddHairdo, IsHeroWearingAnyTash = IsHeroWearingAnyTash, ResetRandomSpeechTime = ResetRandomSpeechTime}
