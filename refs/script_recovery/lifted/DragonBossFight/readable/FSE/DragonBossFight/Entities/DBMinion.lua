-- Readable native conversion: DBMinion. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- DBMinion.Main (retail 0x00d25d80)
function Main(quest, me)
    if not quest:NewScriptFrame(me) then return end
    quest:GiveThingBestEnemyTarget(me, quest:GetHero())
    while quest:GetStateInt("DragonState") ~= 4 do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveThing(me, true, true)
end

-- DBMinion.Init (retail 0x00d25d10)
function Init(quest, me)
    quest:SetStateInt("NumMinions", quest:GetStateInt("NumMinions") + 1)
end

-- DBMinion.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- DBMinion.OnPredicateFail (retail 0x00d25d20)
function OnPredicateFail(quest, me)
    local minionSpawnDelay = quest:GetStateInt("MinionSpawnDelay")
    quest:SetStateInt("NumMinions", quest:GetStateInt("NumMinions") - 1)
    quest:SetTimer(minionSpawnDelay, quest:GetTimer(minionSpawnDelay) + 20)
end

