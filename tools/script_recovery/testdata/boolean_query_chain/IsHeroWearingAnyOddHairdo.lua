function IsHeroWearingAnyOddHairdo(quest, me)
    local predicateResult, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    predicateResult4 = false
    predicateResult3 = false
    predicateResult2 = false
    predicateResult = false
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_BUZZ_01") then
        predicateResult4 = true
        predicateResult3 = false
        predicateResult2 = false
        predicateResult = false
        local hero2 = quest:GetHero()
        if not quest:IsWearingHairstyle(hero2, "OBJECT_HERO_HAIR_BASIN_01") then
            predicateResult4 = true
            predicateResult3 = true
            predicateResult2 = false
            predicateResult = false
            local hero3 = quest:GetHero()
            if not quest:IsWearingHairstyle(hero3, "OBJECT_HERO_HAIR_MOHAWK_01") then
                predicateResult4 = true
                predicateResult3 = true
                predicateResult2 = true
                predicateResult = false
                local hero4 = quest:GetHero()
                if not quest:IsWearingHairstyle(hero4, "OBJECT_HERO_HAIR_PONYTAIL_01") then
                    predicateResult4 = true
                    predicateResult3 = true
                    predicateResult2 = true
                    predicateResult = true
                    local hero5 = quest:GetHero()
                    predicateResult5 = false
                    if not quest:IsWearingHairstyle(hero5, "OBJECT_HERO_HAIR_PLATS_01") then goto LAB_00e53c05 end
                end
            end
        end
    end
    predicateResult5 = true
    ::LAB_00e53c05::
    if predicateResult then
    end
    if predicateResult2 then
    end
    if predicateResult3 then
    end
    if predicateResult4 then
    end
    return predicateResult5
end

