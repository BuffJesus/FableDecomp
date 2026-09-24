-- Readable native conversion: DW5_Balv. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- DW5_Balv.Main (retail 0x00e04970)
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

-- DW5_Balv.Init (retail 0x00e04940)
function Init(quest, me)
end

-- DW5_Balv.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- DW5_Balv.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

