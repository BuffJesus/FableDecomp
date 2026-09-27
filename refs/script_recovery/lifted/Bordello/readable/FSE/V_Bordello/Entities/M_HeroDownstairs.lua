-- Readable native conversion: M_HeroDownstairs. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- M_HeroDownstairs.Main (retail 0x00e3b320)
function Main(quest, me)
    while not quest:IsActiveThreadTerminating() do
        if not quest:IsDistanceBetweenThingsUnder(me, quest:GetHero(), 3.0) then
            quest:NewScriptFrame(me)
        else
            if quest:GetStateBool("HeroPartying") then
                quest:SetStateBool("HeroPartying", false)
            end
            if not quest:GetStateBool("MagicianSleeping") then
                quest:NewScriptFrame(me)
            else
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

