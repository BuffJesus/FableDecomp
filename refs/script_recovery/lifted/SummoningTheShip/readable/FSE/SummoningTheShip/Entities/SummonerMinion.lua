-- Readable native conversion: SummonerMinion. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- per-entity fields (native class members; one Lua state per entity instance)
local waveID

-- SummonerMinion.Main (retail 0x00df2ab0)
function Main(quest, me)
    if not quest:NewScriptFrame(me) then return end
    quest:EntitySetAlpha(me, 0.0, true)
    quest:EntitySetInLimbo(me, true, true)
    while not quest:GetStateBool("SummonerAttacksStarted") or quest:GetStateInt("CurrentAttackWave") ~= waveID do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if waveID == 1 then
        quest:GiveThingBestEnemyTarget(me, quest:GetHero())
    end
    quest:EntitySetInLimbo(me, false, true)
    -- TODO(native): FadeInThing(p0,0x40000000);
    while quest:GetStateInt("SummonersAlive") ~= 0 do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:Pause(1.0)
    quest:RemoveThing(me, true, true)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
end

-- SummonerMinion.Init (retail 0x00df2a40)
function Init(quest, me)
    local predicateResult = me:GetDataString() == "WAVE2"
    waveID = ((predicateResult and predicateResult ~= nil and predicateResult ~= 0) and 1 or 0) + 1
end

-- SummonerMinion.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- SummonerMinion.OnPredicateFail (retail 0x00df2a10)
function OnPredicateFail(quest, me)
end

