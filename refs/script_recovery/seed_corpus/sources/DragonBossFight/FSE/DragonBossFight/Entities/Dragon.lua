-- Dragon.lua
-- Script for Jack of Blades in Dragon form

local DS_HEALTH_HIGH     = 0
local DS_HEALTH_MEDIUM   = 1
local DS_HEALTH_LOW      = 2
local DS_HEALTH_VERY_LOW = 3
local DS_DEAD            = 4

function Init(quest, me)
    quest:Log("Dragon: Init phase started.")
end

function OnPredicateFail(quest, me)
    quest:Log("Dragon: OnPredicateFail triggered - Dragon killed.")
    quest:SetStateInt("DragonState", DS_DEAD)
end

function Main(quest, me)
    quest:Log("Dragon: Main() started.")

    quest:SetStateInt("DragonState", DS_HEALTH_HIGH)
    quest:SetStateInt("TargetNumMinions", 2)
    quest:SetStateInt("TargetNumSummoners", 0)

    -- Wait until dragon has valid health initialized by the game engine
    local maxHealth = quest:GetHealth(me)
    while maxHealth <= 0 do
        if not quest:NewScriptFrame(me) then return end
        maxHealth = quest:GetHealth(me)
    end
    quest:Log("Dragon: Initial Max Health detected = " .. tostring(maxHealth))

    -- Realistic boss phase thresholds:
    -- Phase 1 (High): 100% -> 60% (ground combat + minion wardogs)
    -- Phase 2 (Medium): 60% -> 30% (first summoner waves)
    -- Phase 3 (Low): 30% -> 15% (increased minion & summoner waves)
    -- Phase 4 (Very Low): < 15% (furious final phase)
    local healthMedium   = maxHealth * 0.75
    local healthLow      = maxHealth * 0.50
    local healthVeryLow  = maxHealth * 0.25

    local wasFlying = false
    local flyBysBetweenSummoners = quest:GetStateInt("NumFlyBysBetweenSummonerSpawns") or 1

    while true do
        if not quest:NewScriptFrame(me) then
            quest:Log("Dragon: Script frame terminated.")
            break
        end

        local currentHealth = quest:GetHealth(me)
        local dragonState = quest:GetStateInt("DragonState")

        -- Check death condition (both health <= 0 and IsDead check)
        if currentHealth <= 0 or me:IsDead() or dragonState == DS_DEAD then
            quest:SetStateInt("DragonState", DS_DEAD)
            quest:Log("Dragon: Dragon is dead (HP=" .. tostring(currentHealth) .. "). Exiting Main loop.")
            break
        end

        local isFlying = quest:IsCreatureFlying(me)
        if wasFlying then
            if not isFlying then
                wasFlying = false
                quest:Log("Dragon: Landed on ground.")
            end
        else
            if isFlying then
                wasFlying = true
                quest:Log("Dragon: Took off into flight.")

                local flyBys = (quest:GetStateInt("NumFlyBysSinceLastSummonerSpawn") or 0) + 1
                quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", flyBys)
                quest:SetStateBool("MinionSpawningEnabled", true)

                if flyBys >= flyBysBetweenSummoners and (quest:GetStateInt("TargetNumSummoners") or 0) > 0 then
                    quest:SetStateBool("SummonerSpawningEnabled", true)
                    quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", 0)
                    quest:Log("Dragon: Summoner spawning enabled on fly-by threshold reached.")
                end
            end
        end

        if dragonState == DS_HEALTH_HIGH and currentHealth <= healthMedium then
            quest:SetStateInt("DragonState", DS_HEALTH_MEDIUM)
            quest:SetStateInt("TargetNumMinions", 4)
            quest:SetStateInt("TargetNumSummoners", 1)
            quest:SetStateBool("SummonerSpawningEnabled", true)
            quest:Log("Dragon: Transitioned to DS_HEALTH_MEDIUM (TargetMinions=4, TargetSummoners=1, Health=" .. tostring(currentHealth) .. ")")
        elseif dragonState == DS_HEALTH_MEDIUM and currentHealth <= healthLow then
            quest:SetStateInt("DragonState", DS_HEALTH_LOW)
            quest:SetStateInt("TargetNumMinions", 4)
            quest:SetStateInt("TargetNumSummoners", 2)
            quest:SetStateBool("SummonerSpawningEnabled", true)
            quest:Log("Dragon: Transitioned to DS_HEALTH_LOW (TargetMinions=4, TargetSummoners=2, Health=" .. tostring(currentHealth) .. ")")
        elseif dragonState == DS_HEALTH_LOW and currentHealth <= healthVeryLow then
            quest:SetStateInt("DragonState", DS_HEALTH_VERY_LOW)
            quest:SetStateInt("TargetNumMinions", 6)
            quest:SetStateInt("TargetNumSummoners", 2)
            quest:SetStateBool("SummonerSpawningEnabled", true)
            quest:Log("Dragon: Transitioned to DS_HEALTH_VERY_LOW (TargetMinions=6, TargetSummoners=2, Health=" .. tostring(currentHealth) .. ")")
        end
    end

    quest:Log("Dragon: Main() ended.")
end
