-- RockTrollFirstEncounter.lua
-- Main quest script for the Witchwood Rock Troll First Encounter
-- Functional parity port of NScript::CV_RockTrollFirstEncounterScript

Quest = nil

local QUEST_NAME = "RockTrollFirstEncounter"
local SUBQUEST_ACTIVATE_NAME = "V_RockTrollFirstEncounter_Activate"
local REGION_NAME = "Witchwood1"

function Init(questObject)
    Quest = questObject
    Quest:Log("RockTrollFirstEncounter: Init() started.")

    -- Disable creature generators in Witchwood1 during the encounter
    Quest:SetCreatureGeneratorsEnabled(REGION_NAME, false)

    Quest:SetStateBool("MissionSucceeded", false)
    Quest:SetStateBool("MissionOver", false)
    Quest:SetStateBool("Activated", false)
    Quest:SetStateBool("RockTrollTriggered", false)
    Quest:SetStateBool("PlayedExhumeCutScene", false)
    Quest:SetStateBool("AddedItemsToRockTroll", false)
    Quest:SetStateInt("RockTrollHealthBarID", 0)

    Quest:Log("RockTrollFirstEncounter: Init() completed.")
end

function OnPersist(questObject, context)
    Quest = questObject

    local activated = Quest:GetStateBool("Activated") or false
    activated = Quest:PersistTransferBool(context, "Activated", activated)
    Quest:SetStateBool("Activated", activated)

    local triggered = Quest:GetStateBool("RockTrollTriggered") or false
    triggered = Quest:PersistTransferBool(context, "RockTrollTriggered", triggered)
    Quest:SetStateBool("RockTrollTriggered", triggered)

    local exhumePlayed = Quest:GetStateBool("PlayedExhumeCutScene") or false
    exhumePlayed = Quest:PersistTransferBool(context, "PlayedExhumeCutScene", exhumePlayed)
    Quest:SetStateBool("PlayedExhumeCutScene", exhumePlayed)

    local itemsAdded = Quest:GetStateBool("AddedItemsToRockTroll") or false
    itemsAdded = Quest:PersistTransferBool(context, "AddedItemsToRockTroll", itemsAdded)
    Quest:SetStateBool("AddedItemsToRockTroll", itemsAdded)

    local healthBarID = Quest:GetStateInt("RockTrollHealthBarID") or 0
    healthBarID = Quest:PersistTransferInt(context, "RockTrollHealthBarID", healthBarID)
    Quest:SetStateInt("RockTrollHealthBarID", healthBarID)
end

function Main(questObject)
    Quest = questObject
    Quest:Log("RockTrollFirstEncounter: Main() started. Binding entities...")

    Quest:AddEntityBinding("RTFE_Sparrow", "RockTrollFirstEncounter/Entities/RTFE_Sparrow")
    Quest:AddEntityBinding("RTFE_RockTroll", "RockTrollFirstEncounter/Entities/RTFE_RockTroll")
    Quest:AddEntityBinding("M_RTFERockTrollTrigger", "RockTrollFirstEncounter/Entities/M_RTFERockTrollTrigger")
    Quest:FinalizeEntityBindings()

    Quest:Log("RockTrollFirstEncounter: Entity bindings finalized. Waiting for region '" .. REGION_NAME .. "' to load...")

    -- Wait until Witchwood1 is loaded
    while not Quest:IsRegionLoaded(REGION_NAME) do
        if not Quest:NewScriptFrame() then return end
    end

    Quest:Log("RockTrollFirstEncounter: Region '" .. REGION_NAME .. "' loaded.")

    -- Spawn WatchForRegionExit thread
    Quest:CreateThread("WatchForRegionExit")

    -- Activate quest marker / subquest if not already activated
    if not Quest:GetStateBool("Activated") then
        Quest:Log("RockTrollFirstEncounter: Activating subquest '" .. SUBQUEST_ACTIVATE_NAME .. "'...")
        Quest:ActivateQuest(SUBQUEST_ACTIVATE_NAME)
        Quest:SetStateBool("Activated", true)
    end

    -- Wait until the Rock Troll is defeated
    while not Quest:GetStateBool("MissionOver") do
        if not Quest:NewScriptFrame() then return end
    end

    Quest:Log("RockTrollFirstEncounter: MissionOver is true. Cleaning up HUD and waiting for region exit...")

    local healthBarID = Quest:GetStateInt("RockTrollHealthBarID")
    if healthBarID and healthBarID ~= 0 then
        Quest:RemoveQuestInfoElement(healthBarID)
        Quest:SetStateInt("RockTrollHealthBarID", 0)
    end
    Quest:DisplayQuestInfo(false)

    -- Wait for region transition (player leaves the region)
    while not Quest:MsgOnRegionLoaded() do
        if not Quest:NewScriptFrame() then return end
    end

    Quest:Log("RockTrollFirstEncounter: Region exit detected. Re-enabling generators and deactivating quest...")

    -- Re-enable creature generators in Witchwood1
    Quest:SetCreatureGeneratorsEnabled(REGION_NAME, true)

    -- Deactivate subquest and active quest
    Quest:DeactivateQuestLater(SUBQUEST_ACTIVATE_NAME, 0)
    local activeQuestName = Quest:GetActiveQuestName()
    if activeQuestName and activeQuestName ~= "" then
        Quest:DeactivateQuestLater(activeQuestName, 0)
    end

    Quest:Log("RockTrollFirstEncounter: Quest finished and deactivated.")
end

function WatchForRegionExit(quest)
    quest:Log("RockTrollFirstEncounter: WatchForRegionExit thread started.")

    while not quest:IsRegionLoaded(REGION_NAME) do
        if not quest:NewScriptFrame() then return end
    end

    while quest:IsRegionLoaded(REGION_NAME) do
        if not quest:NewScriptFrame() then return end
    end

    quest:Log("RockTrollFirstEncounter: WatchForRegionExit detected player left '" .. REGION_NAME .. "'. Re-enabling creature generators.")
    quest:SetCreatureGeneratorsEnabled(REGION_NAME, true)
end

function WatchForRockTrollHit(quest)
    quest:Log("RockTrollFirstEncounter: WatchForRockTrollHit thread started.")

    local troll = quest:GetThingWithScriptName("RTFE_RockTroll")
    while not troll do
        if not quest:NewScriptFrame() then return end
        troll = quest:GetThingWithScriptName("RTFE_RockTroll")
    end

    while true do
        local isHit = troll:MsgIsHitBy("SCRIPT_NAME_HERO")
        if not isHit and troll.MsgIsHitByAnyAggressiveSpecialAbilityFrom then
            isHit = troll:MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")
        end

        if isHit then
            quest:Log("RockTrollFirstEncounter: Rock Troll was hit by Hero!")
            break
        end

        if not quest:NewScriptFrame() then return end
    end

    local currentHealth = quest:GetHealth(troll)
    if currentHealth > 0.0001 then
        quest:Log("RockTrollFirstEncounter: Adding health bar for Rock Troll (Health=" .. tostring(currentHealth) .. ")...")
        local healthBarID = quest:AddQuestInfoBarHealth(troll, { red = 255, green = 0, blue = 0 }, "HUD_QUEST_ICON_ROCK_TROLL", 1.0)
        quest:SetStateInt("RockTrollHealthBarID", healthBarID)
        quest:DisplayQuestInfo(true)
    end
end

function WatchForRockTrollKilled(quest)
    quest:Log("RockTrollFirstEncounter: WatchForRockTrollKilled thread started.")

    local troll = quest:GetThingWithScriptName("RTFE_RockTroll")
    while not troll do
        if not quest:NewScriptFrame() then return end
        troll = quest:GetThingWithScriptName("RTFE_RockTroll")
    end

    -- Initial yield frame matching C++
    if not quest:NewScriptFrame() then return end

    while true do
        local isKilled = troll:MsgIsKilledBy("") or (quest:GetHealth(troll) <= 0.0001)
        if isKilled then
            quest:Log("RockTrollFirstEncounter: Rock Troll killed!")
            break
        end

        if not quest:NewScriptFrame() then return end
    end

    local healthBarID = quest:GetStateInt("RockTrollHealthBarID")
    if healthBarID and healthBarID ~= 0 then
        quest:RemoveQuestInfoElement(healthBarID)
        quest:SetStateInt("RockTrollHealthBarID", 0)
    end
    quest:DisplayQuestInfo(false)
    quest:SetStateBool("MissionOver", true)
    quest:Log("RockTrollFirstEncounter: WatchForRockTrollKilled completed. MissionOver set to true.")
end
