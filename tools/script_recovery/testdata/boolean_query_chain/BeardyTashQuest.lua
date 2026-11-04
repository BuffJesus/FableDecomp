function IsHeroWearingAnyTash(quest)
    local predicateResult, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    local predicateResult6
    predicateResult5 = false
    predicateResult4 = false
    predicateResult3 = false
    predicateResult2 = false
    predicateResult = false
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMITH_01") then
        predicateResult5 = true
        predicateResult4 = false
        predicateResult3 = false
        predicateResult2 = false
        predicateResult = false
        local hero2 = quest:GetHero()
        if not quest:IsWearingHairstyle(hero2, "OBJECT_HERO_TASHTRADER_01") then
            predicateResult5 = true
            predicateResult4 = true
            predicateResult3 = false
            predicateResult2 = false
            predicateResult = false
            local hero3 = quest:GetHero()
            if not quest:IsWearingHairstyle(hero3, "OBJECT_HERO_TASHKHG_01") then
                predicateResult5 = true
                predicateResult4 = true
                predicateResult3 = true
                predicateResult2 = false
                predicateResult = false
                local hero4 = quest:GetHero()
                if not quest:IsWearingHairstyle(hero4, "OBJECT_HERO_TASHSHERIFF_01") then
                    predicateResult5 = true
                    predicateResult4 = true
                    predicateResult3 = true
                    predicateResult2 = true
                    predicateResult = false
                    local hero5 = quest:GetHero()
                    if not quest:IsWearingHairstyle(hero5, "OBJECT_HERO_TASHCHINESE_01") then
                        predicateResult5 = true
                        predicateResult4 = true
                        predicateResult3 = true
                        predicateResult2 = true
                        predicateResult = true
                        local hero6 = quest:GetHero()
                        predicateResult6 = false
                        if not quest:IsWearingHairstyle(hero6, "OBJECT_HERO_TASHSMALL_01") then goto LAB_00e53a60 end
                    end
                end
            end
        end
    end
    predicateResult6 = true
    ::LAB_00e53a60::
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

-- V_BeardyBaldy.IsHeroWearingAnyOddHairdo (retail 0x00e53ad0)
