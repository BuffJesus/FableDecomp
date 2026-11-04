-- Generated from the same native helper bodies as the quest draft.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    BeardyBaldyWanderDistanceMin = 1096,  -- 0.0
    BeardyBaldyWanderDistanceMax = 1100,  -- 50.0
    BeardyBaldyRandomSpeechTimerMax = 1172,  -- 30
    BeardyBaldyRandomSpeechTimerVar = 1176,  -- 10
}
local IsHeroWearingAnyOddHairdo, IsHeroWearingAnyTash, SetWanderPointAndDistance, ResetRandomSpeechTime
function IsHeroWearingAnyOddHairdo(quest, me)
    local predicateResult, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    predicateResult5 = true
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_BUZZ_01") then
        local hero2 = quest:GetHero()
        if not quest:IsWearingHairstyle(hero2, "OBJECT_HERO_HAIR_BASIN_01") then
            local hero3 = quest:GetHero()
            if not quest:IsWearingHairstyle(hero3, "OBJECT_HERO_HAIR_MOHAWK_01") then
                local hero4 = quest:GetHero()
                if not quest:IsWearingHairstyle(hero4, "OBJECT_HERO_HAIR_PONYTAIL_01") then
                    local hero5 = quest:GetHero()
                    predicateResult5 = not not quest:IsWearingHairstyle(hero5, "OBJECT_HERO_HAIR_PLATS_01")
                end
            end
        end
    end
    return predicateResult5
end

function IsHeroWearingAnyTash(quest, me)
    local predicateResult, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    local predicateResult6
    predicateResult6 = true
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMITH_01") then
        local hero2 = quest:GetHero()
        if not quest:IsWearingHairstyle(hero2, "OBJECT_HERO_TASHTRADER_01") then
            local hero3 = quest:GetHero()
            if not quest:IsWearingHairstyle(hero3, "OBJECT_HERO_TASHKHG_01") then
                local hero4 = quest:GetHero()
                if not quest:IsWearingHairstyle(hero4, "OBJECT_HERO_TASHSHERIFF_01") then
                    local hero5 = quest:GetHero()
                    if not quest:IsWearingHairstyle(hero5, "OBJECT_HERO_TASHCHINESE_01") then
                        local hero6 = quest:GetHero()
                        predicateResult6 = not not quest:IsWearingHairstyle(hero6, "OBJECT_HERO_TASHSMALL_01")
                    end
                end
            end
        end
    end
    return predicateResult6
end

function SetWanderPointAndDistance(quest, me, param1, param2)
    local center = {x = param2.x, y = param2.y, z = param2.z}
    quest:SetWanderCentrePoint(param1, center)
    local readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BeardyBaldyWanderDistanceMin)
    quest:SetWanderMinDistance(param1, readGlobalGameDataFloat)
    readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BeardyBaldyWanderDistanceMax)
    quest:SetWanderMaxDistance(param1, readGlobalGameDataFloat)
    quest:SetScriptingStateGroup(param1, 4)
end

-- E53C70: bsim names this body NScript::CV_AmbushScamScript::ResetRandomSpeechTime (a homologous script member); no PDB name
function ResetRandomSpeechTime(quest, me)
    local randomChoice = quest:RetailRandModulo(32768)
    quest:SetTimer(quest:GetStateInt("RandomSpeechTimer"), quest:ReadGlobalGameData(SCRIPT_DEF.BeardyBaldyRandomSpeechTimerMax) - randomChoice % quest:ReadGlobalGameData(SCRIPT_DEF.BeardyBaldyRandomSpeechTimerVar))
end

return {IsHeroWearingAnyOddHairdo = IsHeroWearingAnyOddHairdo, IsHeroWearingAnyTash = IsHeroWearingAnyTash, SetWanderPointAndDistance = SetWanderPointAndDistance, ResetRandomSpeechTime = ResetRandomSpeechTime}
