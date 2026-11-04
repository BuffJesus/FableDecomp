function IsHeroWearingBeard(quest, me)
    local flags, scratchValue, predicateResult
    flags = 1
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_BEARD_01") then
        flags = 3
        local hero2 = quest:GetHero()
        if not quest:IsWearingHairstyle(hero2, "OBJECT_HERO_BEARD_02") then
            flags = 7
            local hero3 = quest:GetHero()
            if not quest:IsWearingHairstyle(hero3, "OBJECT_HERO_BEARD_03") then
                flags = 15
                local hero4 = quest:GetHero()
                if not quest:IsWearingHairstyle(hero4, "OBJECT_HERO_BEARD_MUTTON_01") then
                    flags = 31
                    local hero5 = quest:GetHero()
                    if not quest:IsWearingHairstyle(hero5, "OBJECT_HERO_BEARD_LONG_01") then
                        flags = 63
                        local hero6 = quest:GetHero()
                        if not quest:IsWearingHairstyle(hero6, "OBJECT_HERO_BEARD_CHIN_01") then
                            flags = 127
                            local hero7 = quest:GetHero()
                            if not quest:IsWearingHairstyle(hero7, "OBJECT_HERO_BEARD_TRAMP_01") then
                                flags = 255
                                local hero8 = quest:GetHero()
                                predicateResult = false
                                if not quest:IsWearingHairstyle(hero8, "OBJECT_HERO_BEARD_WATSON_01") then goto LAB_00e53856 end
                            end
                        end
                    end
                end
            end
        end
    end
    predicateResult = true
    ::LAB_00e53856::
    if (flags & 128) ~= 0 then
        flags = flags & 127
    end
    if (flags & 64) ~= 0 then
        flags = flags & 191
    end
    if (flags & 32) ~= 0 then
        flags = flags & 223
    end
    if (flags & 16) ~= 0 then
        flags = flags & 239
    end
    if (flags & 8) ~= 0 then
        flags = flags & 247
    end
    if (flags & 4) ~= 0 then
        flags = flags & 251
    end
    if (flags & 2) ~= 0 then
        scratchValue = flags & 253
    end
    return predicateResult
end

-- BB_BeardyBaldyMan.GetRandomSpeech (retail 0x00e53cb0)
-- E53CB0: bsim names this body NScript::CV_ArcheryCompetitionScript::CAC_Owner::GetRandomSpeech (a homologous script member); no PDB name
