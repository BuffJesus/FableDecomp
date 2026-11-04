-- Readable native conversion: M_HeroDownstairs. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- M_HeroDownstairs.Main (retail 0x00e3b320)
function Main(quest, me)
    while not quest:IsActiveThreadTerminating() do
        local thing = quest:GetHero()
        if not quest:IsDistanceBetweenThingsUnder(me, thing, 3.0) then
            quest:NewScriptFrame(me)
        else
            if quest:IsActiveThreadTerminating() then return end
            if quest:GetStateBool("HeroPartying") then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateBool("HeroPartying", false)
            end
            if not quest:GetStateBool("MagicianSleeping") then
                quest:NewScriptFrame(me)
            else
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateBool("MagicianSleeping", false)
                quest:NewScriptFrame(me)
            end
        end
    end
end

-- M_HeroDownstairs.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- M_HeroDownstairs.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- M_HeroDownstairs.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

