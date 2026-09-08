-- LUAGameflowAssistance.lua
-- Background assistance and tutorial quest
-- Functional parity port of NScript::CGameflowAssistanceScript (CGameflowAssistance.c)

Quest = nil

local QUEST_NAME = "LUAGameflowAssistance"

-- Game definition constants (from main.def lines 2450-2468)
local COMBAT_MULT_TOO_LONG_UNCHANGED       = 1000
local COMBAT_MULT_STOP_BOTHERING_AFTER     = 30

local LOW_HEALTH_WARNING                   = 10.0
local VERY_LOW_HEALTH_WARNING              = 5.0
local LOW_HEALTH_MESSAGE_INTERVAL          = 300
local VERY_LOW_HEALTH_MESSAGE_INTERVAL     = 200
local HEALTH_MESSAGE_LIMIT                 = 30

local LOW_WILL_ENERGY_WARNING_LEVEL        = 0.2
local LOW_WILL_ENERGY_MESSAGE_INTERVAL     = 300
local WILL_ENERGY_MESSAGE_LIMIT            = 30

local RENOWN_LEVEL_MESSAGES = {
    [1] = "TEXT_QST_080_RENOWN_LEVEL_ONE",
    [2] = "TEXT_QST_080_RENOWN_LEVEL_TWO",
    [3] = "TEXT_QST_080_RENOWN_LEVEL_THREE",
    [4] = "TEXT_QST_080_RENOWN_LEVEL_FOUR",
    [5] = "TEXT_QST_080_RENOWN_LEVEL_FIVE",
    [6] = "TEXT_QST_080_RENOWN_LEVEL_SIX",
    [7] = "TEXT_QST_080_RENOWN_LEVEL_SEVEN",
}

function Init(questObject)
    Quest = questObject
    Quest:Log("LUAGameflowAssistance: Init phase started.")

    Quest:SetStateInt("FlourishesOffered", 0)
    Quest:SetStateInt("FlourishesPerformed", 0)
    Quest:SetStateInt("CombatMultHighestLevel", 1)
    Quest:SetStateInt("CombatMultFramesSinceChange", 0)
    Quest:SetStateInt("HealthWarningsGiven", 0)
    Quest:SetStateInt("WillEnergyWarningsGiven", 0)
    Quest:SetStateInt("SaveWarningsGiven", 0)
    Quest:SetStateInt("SavedRenownLevel", 0)

    Quest:Log("LUAGameflowAssistance: Init finished.")
end

function OnPersist(questObject, context)
    Quest = questObject

    local flourishesOffered = Quest:GetStateInt("FlourishesOffered") or 0
    flourishesOffered = Quest:PersistTransferInt(context, "FlourishesOffered", flourishesOffered)
    Quest:SetStateInt("FlourishesOffered", flourishesOffered)

    local flourishesPerformed = Quest:GetStateInt("FlourishesPerformed") or 0
    flourishesPerformed = Quest:PersistTransferInt(context, "FlourishesPerformed", flourishesPerformed)
    Quest:SetStateInt("FlourishesPerformed", flourishesPerformed)

    local combatMultHighestLevel = Quest:GetStateInt("CombatMultHighestLevel") or 1
    combatMultHighestLevel = Quest:PersistTransferInt(context, "CombatMultHighestLevel", combatMultHighestLevel)
    Quest:SetStateInt("CombatMultHighestLevel", combatMultHighestLevel)

    local combatMultFramesSinceChange = Quest:GetStateInt("CombatMultFramesSinceChange") or 0
    combatMultFramesSinceChange = Quest:PersistTransferInt(context, "CombatMultFramesSinceChange", combatMultFramesSinceChange)
    Quest:SetStateInt("CombatMultFramesSinceChange", combatMultFramesSinceChange)

    local healthWarningsGiven = Quest:GetStateInt("HealthWarningsGiven") or 0
    healthWarningsGiven = Quest:PersistTransferInt(context, "HealthWarningsGiven", healthWarningsGiven)
    Quest:SetStateInt("HealthWarningsGiven", healthWarningsGiven)

    local willEnergyWarningsGiven = Quest:GetStateInt("WillEnergyWarningsGiven") or 0
    willEnergyWarningsGiven = Quest:PersistTransferInt(context, "WillEnergyWarningsGiven", willEnergyWarningsGiven)
    Quest:SetStateInt("WillEnergyWarningsGiven", willEnergyWarningsGiven)

    local saveWarningsGiven = Quest:GetStateInt("SaveWarningsGiven") or 0
    saveWarningsGiven = Quest:PersistTransferInt(context, "SaveWarningsGiven", saveWarningsGiven)
    Quest:SetStateInt("SaveWarningsGiven", saveWarningsGiven)

    local savedRenownLevel = Quest:GetStateInt("SavedRenownLevel") or 0
    savedRenownLevel = Quest:PersistTransferInt(context, "SavedRenownLevel", savedRenownLevel)
    Quest:SetStateInt("SavedRenownLevel", savedRenownLevel)
end

function Tutorial_CombatMultiplier(quest)
    quest:Log("LUAGameflowAssistance: Tutorial_CombatMultiplier thread started.")

    while true do
        if quest:GetMasterGameState("KilledGM") then
            quest:Log("LUAGameflowAssistance: KilledGM is true, ending Tutorial_CombatMultiplier.")
            return
        end

        if quest:IsPlayerWieldingWeapon() and quest:IsHeroControlledByPlayer() then
            local currentMult = quest:GetPlayerCreatureCombatMultiplier() or 1
            local highestLevel = quest:GetStateInt("CombatMultHighestLevel") or 1
            local framesSinceChange = quest:GetStateInt("CombatMultFramesSinceChange") or 0

            if currentMult <= highestLevel then
                framesSinceChange = framesSinceChange + 1
                if framesSinceChange > (highestLevel * COMBAT_MULT_TOO_LONG_UNCHANGED) then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_TRY", "", true, false)
                    framesSinceChange = 0
                end
                quest:SetStateInt("CombatMultFramesSinceChange", framesSinceChange)
            else
                highestLevel = currentMult
                framesSinceChange = 0
                quest:SetStateInt("CombatMultHighestLevel", highestLevel)
                quest:SetStateInt("CombatMultFramesSinceChange", framesSinceChange)

                if currentMult > COMBAT_MULT_STOP_BOTHERING_AFTER then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_MASTER", "", true, false)
                    quest:Log("LUAGameflowAssistance: Combat multiplier exceeded limit (30), stopping multiplier tutorial.")
                    return
                end

                if highestLevel == 15 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_EXCELLENT", "", true, false)
                elseif highestLevel == 10 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_ADEPT", "", true, false)
                elseif highestLevel == 5 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_PROGRESS", "", true, false)
                end
            end
        end

        if not quest:NewScriptFrame() then
            return
        end
    end
end

function Tutorial_LowHealth(quest)
    quest:Log("LUAGameflowAssistance: Tutorial_LowHealth thread started.")

    local lowHealthCooldown = 0
    local critHealthCooldown = 0

    while (quest:GetStateInt("HealthWarningsGiven") or 0) < HEALTH_MESSAGE_LIMIT do
        if not quest:NewScriptFrame() then
            return
        end

        if quest:GetMasterGameState("KilledGM") then
            quest:Log("LUAGameflowAssistance: KilledGM is true, ending Tutorial_LowHealth.")
            return
        end

        if quest:IsHeroControlledByPlayer() then
            local health = quest:GetHeroHealth() or 0.0
            local warningsGiven = quest:GetStateInt("HealthWarningsGiven") or 0

            if health >= VERY_LOW_HEALTH_WARNING then
                if health < LOW_HEALTH_WARNING then
                    if lowHealthCooldown <= 0 then
                        quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_HEALTH_LOW", "", true, false)
                        lowHealthCooldown = LOW_HEALTH_MESSAGE_INTERVAL
                        warningsGiven = warningsGiven + 1
                        quest:SetStateInt("HealthWarningsGiven", warningsGiven)
                    end
                end
            else
                if critHealthCooldown <= 0 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_HEALTH_CRIT", "", true, false)
                    critHealthCooldown = VERY_LOW_HEALTH_MESSAGE_INTERVAL
                    warningsGiven = warningsGiven + 1
                    quest:SetStateInt("HealthWarningsGiven", warningsGiven)
                end
            end

            critHealthCooldown = critHealthCooldown - 1
            lowHealthCooldown = lowHealthCooldown - 1
        end
    end

    quest:Log("LUAGameflowAssistance: Tutorial_LowHealth limit reached or finished.")
end

function Tutorial_LowMagic(quest)
    quest:Log("LUAGameflowAssistance: Tutorial_LowMagic thread started.")

    local willCooldown = 0

    while (quest:GetStateInt("WillEnergyWarningsGiven") or 0) < WILL_ENERGY_MESSAGE_LIMIT do
        if not quest:NewScriptFrame() then
            return
        end

        if quest:GetMasterGameState("KilledGM") then
            quest:Log("LUAGameflowAssistance: KilledGM is true, ending Tutorial_LowMagic.")
            return
        end

        if quest:IsHeroControlledByPlayer() then
            if willCooldown <= 0 then
                local willLevel = quest:GetHeroWillEnergyLevel() or 1.0
                if willLevel < LOW_WILL_ENERGY_WARNING_LEVEL then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_WILL_LOW", "", true, false)
                    willCooldown = LOW_WILL_ENERGY_MESSAGE_INTERVAL
                    local warnings = (quest:GetStateInt("WillEnergyWarningsGiven") or 0) + 1
                    quest:SetStateInt("WillEnergyWarningsGiven", warnings)
                end
            end
            willCooldown = willCooldown - 1
        end
    end

    quest:Log("LUAGameflowAssistance: Tutorial_LowMagic limit reached or finished.")
end

function Tutorial_RenownLevelIncrease(quest)
    quest:Log("LUAGameflowAssistance: Tutorial_RenownLevelIncrease thread started.")

    while not quest:GetMasterGameState("KilledGM") do
        if quest:IsHeroControlledByPlayer() then
            local renownLevel = quest:GetHeroRenownLevel() or 0
            local savedLevel = quest:GetStateInt("SavedRenownLevel") or 0

            if renownLevel > savedLevel then
                quest:SetStateInt("SavedRenownLevel", renownLevel)

                if renownLevel < 8 and RENOWN_LEVEL_MESSAGES[renownLevel] then
                    quest:HeroReceiveMessageFromGuildMaster(RENOWN_LEVEL_MESSAGES[renownLevel], "", true, true)
                end

                if renownLevel > 1 then
                    quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
                    quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
                    quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
                end
            end
        end

        if not quest:NewScriptFrame() then
            return
        end
    end

    quest:Log("LUAGameflowAssistance: KilledGM is true, ending Tutorial_RenownLevelIncrease.")
end

function Main(questObject)
    Quest = questObject
    Quest:Log("LUAGameflowAssistance: Main() started.")

    Quest:CreateThread("Tutorial_CombatMultiplier")
    Quest:CreateThread("Tutorial_LowHealth")
    Quest:CreateThread("Tutorial_LowMagic")
    Quest:CreateThread("Tutorial_RenownLevelIncrease")

    Quest:Log("LUAGameflowAssistance: All 4 tutorial threads created.")
end
