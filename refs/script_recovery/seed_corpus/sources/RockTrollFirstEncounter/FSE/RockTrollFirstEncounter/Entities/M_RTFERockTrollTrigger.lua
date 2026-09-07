-- M_RTFERockTrollTrigger.lua
-- Trigger entity script for spawning the Rock Troll in RockTrollFirstEncounter
-- Functional parity port of NScript::CV_RockTrollFirstEncounterScript::CM_RTFERockTrollTrigger

local TRIGGER_DISTANCE = 5.0

function Init(quest, me)
    quest:Log("M_RTFERockTrollTrigger: Init() started.")
end

function Main(quest, me)
    quest:Log("M_RTFERockTrollTrigger: Main() started.")

    -- Initial yield frame matching C++
    if not quest:NewScriptFrame(me) then return end

    if quest:GetStateBool("RockTrollTriggered") then
        quest:Log("M_RTFERockTrollTrigger: Rock Troll was already triggered. Removing trigger.")
        quest:RemoveThing(me)
        return
    end

    -- Cache Hero outside the monitoring loop to prevent memory leak
    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: M_RTFERockTrollTrigger could not find Hero!")
        return
    end

    local spawnMkr = quest:GetThingWithScriptName("M_RTFERockTrollSpawnPos")
    if not spawnMkr then
        quest:Log("!!! WARNING: M_RTFERockTrollTrigger could not find 'M_RTFERockTrollSpawnPos', fallback to me position.")
    end

    quest:Log("M_RTFERockTrollTrigger: Monitoring Hero proximity (distance < " .. tostring(TRIGGER_DISTANCE) .. ")...")

    -- Distance monitoring loop
    while true do
        if quest:IsDistanceBetweenThingsUnder(hero, me, TRIGGER_DISTANCE) then
            quest:Log("M_RTFERockTrollTrigger: Hero entered proximity threshold!")
            break
        end

        if not quest:NewScriptFrame(me) then return end
        if quest:GetStateBool("RockTrollTriggered") then return end
    end

    -- Determine spawn location
    local spawnPos = spawnMkr and spawnMkr:GetPos() or me:GetPos()
    quest:Log("M_RTFERockTrollTrigger: Spawning CREATURE_ROCK_TROLL_START_STANDING as 'RTFE_RockTroll'...")

    quest:CreateCreature("CREATURE_ROCK_TROLL_START_STANDING", spawnPos, "RTFE_RockTroll")
    quest:SetStateBool("RockTrollTriggered", true)

    -- Remove trigger entity
    quest:Log("M_RTFERockTrollTrigger: Trigger fulfilled. Removing self.")
    quest:RemoveThing(me)
end
