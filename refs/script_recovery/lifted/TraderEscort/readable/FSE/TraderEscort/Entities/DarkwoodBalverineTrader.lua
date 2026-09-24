-- Readable native conversion: DarkwoodBalverineTrader. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- DarkwoodBalverineTrader.Main (retail 0x00e03480)
function Main(quest, me)
    if not quest:NewScriptFrame(me) then return end
    quest:GiveThingBestEnemyTarget(me, quest:GetHero())
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
end

-- DarkwoodBalverineTrader.Init (retail 0x00e03450)
function Init(quest, me)
end

-- DarkwoodBalverineTrader.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- DarkwoodBalverineTrader.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

