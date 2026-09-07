-- ScytheInfo.lua
-- Main quest script for Scythe Info (QS_ScytheInfo.c)
-- Functional parity port of NScript::CQS_ScytheInfoScript

Quest = nil

local QUEST_NAME = "ScytheInfo"

function Init(questObject)
    Quest = questObject
    Quest:Log("ScytheInfo: Init() started.")
    Quest:SetStateBool("MissionSucceeded", false)
end

function OnPersist(questObject, context)
    Quest = questObject
    local missionSucceeded = Quest:GetStateBool("MissionSucceeded") or false
    missionSucceeded = Quest:PersistTransferBool(context, "MissionSucceeded", missionSucceeded)
    Quest:SetStateBool("MissionSucceeded", missionSucceeded)
end

function Main(questObject)
    Quest = questObject
    Quest:Log("ScytheInfo: Main() started. Setting up entity bindings...")

    Quest:AddEntityBinding("ScytheMarker", "ScytheInfo/Entities/ScytheMarker")
    Quest:AddEntityBinding("ScytheNearOracle", "ScytheInfo/Entities/ScytheNearOracle")
    Quest:FinalizeEntityBindings()

    local activeQuestName = Quest:GetActiveQuestName()
    Quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_AWAKEN_ORACLE_OBJECTIVE_03", "", "NorthernWastes3")
    Quest:Log("ScytheInfo: Initial objective set for NorthernWastes3.")

    -- 100-second reminder timer telling Hero to return to Snowspire / Archon's Shrine
    local snowspireReminder = Quest:RegisterTimer()
    Quest:SetTimer(snowspireReminder, 100)

    -- If NorthernWastes2 is currently loaded, remind Hero periodically until they leave NorthernWastes2
    if Quest:IsRegionLoaded("NorthernWastes2") then
        while true do
            if not Quest:NewScriptFrame() then
                Quest:DeregisterTimer(snowspireReminder)
                return
            end

            if Quest:GetTimer(snowspireReminder) <= 0 then
                Quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_B03_SCYTHE_RETURN_TO_SNOWSPIRE_REMINDER_10", "", true, true)
                Quest:SetTimer(snowspireReminder, 100)
            end

            if not Quest:IsRegionLoaded("NorthernWastes2") then
                break
            end
        end
    end

    -- Wait until the mission succeeds (cutscene triggered by ScytheNearOracle)
    while not Quest:GetStateBool("MissionSucceeded") do
        if not Quest:NewScriptFrame() then
            Quest:DeregisterTimer(snowspireReminder)
            return
        end
    end

    -- Mission succeeded: complete quest, deactivate, fade screen in, and deregister timer
    activeQuestName = Quest:GetActiveQuestName()
    Quest:SetQuestAsCompleted(activeQuestName, false, false, false)
    Quest:DeactivateQuestLater(activeQuestName, 0)
    Quest:FadeScreenIn()
    Quest:DeregisterTimer(snowspireReminder)
    Quest:Log("ScytheInfo: Quest completed and deactivated.")
end
