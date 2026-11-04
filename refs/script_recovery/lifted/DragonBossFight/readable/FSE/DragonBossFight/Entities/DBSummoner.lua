-- Readable native conversion: DBSummoner. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- DBSummoner.Main (retail 0x00d25f50)
function Main(quest, me)
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then return end
    local pTarget = quest:GetHero()
    quest:GiveThingBestEnemyTarget(me, pTarget)
    while quest:GetStateInt("DragonState") ~= 4 do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveThing(me, true, true)
end

-- DBSummoner.Init (retail 0x00d25ee0)
function Init(quest, me)
    quest:SetStateInt("NumSummoners", quest:GetStateInt("NumSummoners") + 1)
end

-- DBSummoner.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- DBSummoner.OnPredicateFail (retail 0x00d25ef0)
function OnPredicateFail(quest, me)
    local summonerSpawnDelay = quest:GetStateInt("SummonerSpawnDelay")
    quest:SetStateInt("NumSummoners", quest:GetStateInt("NumSummoners") - 1)
    local timeRemaining = quest:GetTimer(summonerSpawnDelay)
    quest:SetTimer(summonerSpawnDelay, timeRemaining + 20)
end

