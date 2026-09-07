-- SummonerAttacker.lua
-- Entity script for Summoner attackers in Summoning The Ship
-- Functional parity port of NScript::CQ_SummoningTheShipScript::CSummonerAttacker

local HERO_ABILITY_HEAL_LIFE_SPELL = 14
local SCRIPT_AI_PRIORITY_HIGHEST = 5

function Init(quest, me)
    quest:Log("SummonerAttacker: Init() started.")

    local count = (quest:GetStateInt("SummonersAlive") or 0) + 1
    quest:SetStateInt("SummonersAlive", count)

    local dataStr = me:GetDataString()
    local waveID = (dataStr == "WAVE2") and 2 or 1

    quest:Log("SummonerAttacker: Init() completed. WaveID=" .. tostring(waveID) .. ", SummonersAlive=" .. tostring(count))
end

function OnPredicateFail(quest, me)
    quest:Log("SummonerAttacker: OnPredicateFail triggered.")

    local count = math.max(0, (quest:GetStateInt("SummonersAlive") or 1) - 1)
    quest:SetStateInt("SummonersAlive", count)

    local dataStr = me:GetDataString()
    local waveID = (dataStr == "WAVE2") and 2 or 1
    if waveID == 1 then
        local currentWave = quest:GetStateInt("CurrentAttackWave")
        if currentWave == 0 then currentWave = 1 end
        if currentWave <= 1 then
            quest:SetStateInt("CurrentAttackWave", 2)
            quest:Log("SummonerAttacker: OnPredicateFail - Advanced CurrentAttackWave to 2.")
        end
    end
end

function Main(quest, me)
    local hero = quest:GetHero()
    local dataStr = me:GetDataString()
    local waveID = (dataStr == "WAVE2") and 2 or 1
    quest:Log("SummonerAttacker: Main() started. WaveID=" .. tostring(waveID) .. " (dataStr='" .. tostring(dataStr) .. "')")

    me:MakeBehavioral()

    -- Initial yield frame
    if not quest:NewScriptFrame(me) then return end

    quest:EntitySetAlpha(me, 0.0)
    quest:EntitySetInLimbo(me, true)

    -- Wait until this summoner's wave begins
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

    quest:Log("SummonerAttacker: Wave " .. tostring(waveID) .. " active. Entering battlefield...")

    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
    quest:EntitySetInLimbo(me, false)
    quest:EntitySetAlpha(me, 1.0)
    quest:EntityFadeIn(me, 2.0)

    if waveID == 1 then
        quest:AddQuestInfoBarHealth(me, { red = 0, green = 255, blue = 255 }, "HUD_QUEST_ICON_SUMMONER_BLUE", 1.0)
    end

    local fireHeart = quest:GetThingWithScriptName("FireHeart")
    local minion1 = nil
    local minion2 = nil

    local currentState = "CHANNELING"

    -- State Machine loop
    while true do
        if me:IsDead() or not me:IsAlive() then break end

        if currentState == "CHANNELING" then
            me:AcquireControl(SCRIPT_AI_PRIORITY_HIGHEST)

            while not quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) do
                if not quest:NewScriptFrame(me) then break end
                if me:IsDead() or not me:IsAlive() then break end

                if not me:IsPerformingScriptTask() and fireHeart then
                    me:SummonerLightningOrbAttackTarget(fireHeart)
                end

                -- Check if attacked by Hero
                local hitByHero = me:MsgIsHitBy("SCRIPT_NAME_HERO")
                    or (me:MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                        and not me:MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))

                if hitByHero and hero then
                    if not minion1 or minion1:IsDead() or not minion1:IsAlive() then
                        local heroPos = hero:GetPos()
                        minion1 = quest:CreateCreatureNearby("CREATURE_MINION_WARDOG", heroPos, 15.0, "SummonerMinion")
                        quest:Log("SummonerAttacker: Spawned Minion 1 wardog.")
                    end

                    if not minion2 or minion2:IsDead() or not minion2:IsAlive() then
                        local heroPos = hero:GetPos()
                        minion2 = quest:CreateCreatureNearby("CREATURE_MINION_WARDOG", heroPos, 15.0, "SummonerMinion")
                        quest:Log("SummonerAttacker: Spawned Minion 2 wardog.")
                    end
                end
            end

            -- Hero closed in within 15 meters
            me:ReleaseControl()
            quest:MiniMapRemoveMarker(me)
            currentState = "MELEE_COMBAT"

        elseif currentState == "MELEE_COMBAT" then
            if hero then
                quest:GiveThingBestEnemyTarget(me, hero)
            end

            while quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) do
                if not quest:NewScriptFrame(me) then break end
                if me:IsDead() then break end
            end

            -- Hero moved away > 15m; resume channeling at Fire Heart
            quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
            currentState = "CHANNELING"
        end

        if me:IsDead() or not me:IsAlive() then break end
        if not quest:NewScriptFrame(me) then break end
    end

    local count = math.max(0, (quest:GetStateInt("SummonersAlive") or 1) - 1)
    quest:SetStateInt("SummonersAlive", count)

    if waveID == 1 then
        local currentWave = quest:GetStateInt("CurrentAttackWave")
        if currentWave == 0 then currentWave = 1 end
        if currentWave <= 1 then
            quest:SetStateInt("CurrentAttackWave", 2)
            quest:Log("SummonerAttacker: Wave 1 summoner dead. Advanced CurrentAttackWave to 2.")
        end
    end

    quest:Log("SummonerAttacker: Main() finished for Wave " .. tostring(waveID))
end
