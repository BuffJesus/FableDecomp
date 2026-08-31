Quest = nil

local QUEST_NAME = "HerosOldHouse"

function Init(questObject)
    Quest = questObject
    Quest:Log("HerosOldHouse: Init phase started.")

    Quest:SetStateBool("Helping", false)
    Quest:SetStateBool("Helped", false)
    Quest:SetStateBool("BootyDugUp", false)
    Quest:SetStateBool("WifeAttacked", false)
    Quest:SetStateBool("WifeAttackedAndLeft", false)
    Quest:SetStateBool("MissionAborted", false)
end

function OnPersist(questObject, context)
    Quest = questObject
    local helping = Quest:GetStateBool("Helping") or false
    helping = Quest:PersistTransferBool(context, "Helping", helping)
    Quest:SetStateBool("Helping", helping)

    local helped = Quest:GetStateBool("Helped") or false
    helped = Quest:PersistTransferBool(context, "Helped", helped)
    Quest:SetStateBool("Helped", helped)

    local bootyDugUp = Quest:GetStateBool("BootyDugUp") or false
    bootyDugUp = Quest:PersistTransferBool(context, "BootyDugUp", bootyDugUp)
    Quest:SetStateBool("BootyDugUp", bootyDugUp)
end

function WatchForBooty(quest)
    quest:Log("HerosOldHouse: WatchForBooty thread started.")
    local hiddenBooty = quest:GetThingWithScriptName("HiddenBooty")
    if not hiddenBooty then
        quest:Log("!!! WARNING: HerosOldHouse WatchForBooty - HiddenBooty not found.")
        return
    end

    while quest:IsRegionLoaded("OakBay") do
        if not quest:IsDiggingSpotEnabled(hiddenBooty) then
            local activeQuestName = quest:GetActiveQuestName()
            quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_02", "", "OakBay")
            quest:Log("HerosOldHouse: HiddenBooty dug up! Objective 02 set.")
            return
        end

        if not quest:NewScriptFrame() then
            return
        end
    end
end

function Main(questObject)
    Quest = questObject
    Quest:Log("HerosOldHouse: Main() started.")

    while not Quest:IsRegionLoaded("OakBay") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    Quest:Log("HerosOldHouse: OakBay loaded. Setting up entity bindings...")
    Quest:AddEntityBinding("GhostFisherman", "HerosOldHouse/Entities/GhostFisherman")
    Quest:AddEntityBinding("FishermansWife", "HerosOldHouse/Entities/FishermansWife")
    Quest:AddEntityBinding("ExtraBooty", "HerosOldHouse/Entities/ExtraBooty")
    Quest:FinalizeEntityBindings()

    Quest:CreateThread("WatchForBooty")

    local haveSetQuestAsFailed = false

    -- Main quest monitor loop: wait for BootyDugUp while monitoring if wife attacked
    while not Quest:GetStateBool("BootyDugUp") do
        if not Quest:NewScriptFrame() then
            return
        end

        if not haveSetQuestAsFailed and Quest:IsRegionLoaded("OakBay") and Quest:GetStateBool("WifeAttacked") then
            if Quest:GetStateBool("Helping") then
                local activeQuestName = Quest:GetActiveQuestName()
                Quest:SetQuestAsFailed(activeQuestName, false, "", true)
                haveSetQuestAsFailed = true
                Quest:SetStateBool("Helping", false)
                Quest:Log("HerosOldHouse: Wife attacked while helping - quest failed.")
            end
        end
    end

    -- Once booty is dug up, deactivate quest
    local activeQuestName = Quest:GetActiveQuestName()
    Quest:DeactivateQuestLater(activeQuestName, 0)
    Quest:Log("HerosOldHouse: Main() finished and quest deactivated.")
end
