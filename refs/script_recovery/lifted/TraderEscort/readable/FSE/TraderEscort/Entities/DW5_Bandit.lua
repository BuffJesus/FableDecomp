-- Readable native conversion: DW5_Bandit. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- DW5_Bandit.Main (retail 0x00e047d0)
function Main(quest, me)
    if not quest:NewScriptFrame(me) then return end
    while not me:IsAwareOfHero() do
        if not quest:NewScriptFrame(me) then return end
    end
    quest:GiveThingBestEnemyTarget(me, quest:GetHero())
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
end

-- DW5_Bandit.Init (retail 0x00e047a0)
function Init(quest, me)
end

-- DW5_Bandit.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- DW5_Bandit.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

