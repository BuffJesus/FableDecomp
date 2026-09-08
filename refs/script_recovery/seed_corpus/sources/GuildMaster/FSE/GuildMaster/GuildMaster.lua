-- GuildMaster.lua
-- Main quest script for the Guildmaster ambient quest (V_GuildMaster.c)
-- Functional parity port of NScript::CV_GuildMasterScript

Quest = nil

local QUEST_NAME = "GuildMaster"

function Init(questObject)
    Quest = questObject
    Quest:Log("GuildMaster: Init() started.")

    -- Initialize speech trackers matching CV_GuildMasterScript::Init
    Quest:SetStateString("LastDialogueSaid", "")
    Quest:SetStateBool("GMDial_1", false) -- Hero Title (Chicken Chaser) hint
    Quest:SetStateBool("GMDial_2", false) -- Teleporters hint
    Quest:SetStateBool("GMDial_3", false) -- Weapons hint
    Quest:SetStateBool("GMDial_4", false) -- Log book hint

    Quest:SetStateBool("FocalSitesSetupDone", false)

    Quest:Log("GuildMaster: Init() completed.")
end

function OnPersist(questObject, context)
    Quest = questObject

    local lastDialogue = Quest:GetStateString("LastDialogueSaid") or ""
    lastDialogue = Quest:PersistTransferString(context, "LastDialogueSaid", lastDialogue)
    Quest:SetStateString("LastDialogueSaid", lastDialogue)

    local d1 = Quest:GetStateBool("GMDial_1") or false
    d1 = Quest:PersistTransferBool(context, "GMDial_1", d1)
    Quest:SetStateBool("GMDial_1", d1)

    local d2 = Quest:GetStateBool("GMDial_2") or false
    d2 = Quest:PersistTransferBool(context, "GMDial_2", d2)
    Quest:SetStateBool("GMDial_2", d2)

    local d3 = Quest:GetStateBool("GMDial_3") or false
    d3 = Quest:PersistTransferBool(context, "GMDial_3", d3)
    Quest:SetStateBool("GMDial_3", d3)

    local d4 = Quest:GetStateBool("GMDial_4") or false
    d4 = Quest:PersistTransferBool(context, "GMDial_4", d4)
    Quest:SetStateBool("GMDial_4", d4)

    local focalDone = Quest:GetStateBool("FocalSitesSetupDone") or false
    focalDone = Quest:PersistTransferBool(context, "FocalSitesSetupDone", focalDone)
    Quest:SetStateBool("FocalSitesSetupDone", focalDone)
end

function Main(questObject)
    Quest = questObject
    Quest:Log("GuildMaster: Main() started. Binding entities...")

    -- Bind Guildmaster entity script
    Quest:AddEntityBinding("GuildMasterGameFlow", "GuildMaster/Entities/GuildMasterGameFlow")
    Quest:FinalizeEntityBindings()

    Quest:Log("GuildMaster: Entity bindings finalized. Entering quest life cycle loop...")

    -- Persistent vignette / ambient quest: stays active as long as the quest is loaded
    while true do
        if not Quest:NewScriptFrame() then
            return
        end
    end
end
