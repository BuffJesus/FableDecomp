-- SummonerMinion.lua
-- Entity script for Minion wardogs spawned during Summoning The Ship
-- Functional parity port of NScript::CQ_SummoningTheShipScript::CSummonerMinion

function Init(quest, me)
    local dataStr = me:GetDataString()
    local waveID = (dataStr == "WAVE2") and 2 or 1
    quest:Log("SummonerMinion: Init() completed. WaveID=" .. tostring(waveID))
end

function Main(quest, me)
    local hero = quest:GetHero()
    local dataStr = me:GetDataString()
    local waveID = (dataStr == "WAVE2") and 2 or 1
    quest:Log("SummonerMinion: Main() started. WaveID=" .. tostring(waveID) .. " (dataStr='" .. tostring(dataStr) .. "')")

    me:MakeBehavioral()

    -- Initial yield frame
    if not quest:NewScriptFrame(me) then return end

    quest:EntitySetAlpha(me, 0.0)
    quest:EntitySetInLimbo(me, true)

    -- Wait until this minion's wave begins
    while true do
        local attacksStarted = quest:GetStateBool("SummonerAttacksStarted")
        local currentWave = quest:GetStateInt("CurrentAttackWave")
        if currentWave == 0 then currentWave = 1 end
        if attacksStarted and currentWave >= waveID then
            break
        end

        if not quest:NewScriptFrame(me) then return end
        if me:IsDead() or not me:IsAlive() then return end
    end

    quest:Log("SummonerMinion: Wave " .. tostring(waveID) .. " active. Entering battlefield...")

    if waveID == 1 and hero then
        quest:GiveThingBestEnemyTarget(me, hero)
    end

    quest:EntitySetInLimbo(me, false)
    quest:EntitySetAlpha(me, 1.0)
    quest:EntityFadeIn(me, 2.0)

    -- Active combat loop while summoners are alive
    while (quest:GetStateInt("SummonersAlive") or 0) > 0 do
        if not quest:NewScriptFrame(me) then break end
        if me:IsDead() then break end
    end

    -- Despawn once all summoners have been defeated
    if not me:IsDead() then
        quest:Log("SummonerMinion: All summoners dead. Despawning minion...")
        quest:Pause(1.0)
        quest:RemoveThing(me)
        while true do
            if not quest:NewScriptFrame(me) then break end
        end
    end

    quest:Log("SummonerMinion: Main() finished.")
end
