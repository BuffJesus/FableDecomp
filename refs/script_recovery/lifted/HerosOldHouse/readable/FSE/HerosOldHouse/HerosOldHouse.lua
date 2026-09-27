-- Readable native conversion: Q_HerosOldHouse. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_HerosOldHouse.Main (retail 0x00d89e60)
function Main(quest)
    local predicateResult, isRegionLoaded
    while not quest:IsRegionLoaded("OakBay") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:AddEntityBinding("GhostFisherman", "HerosOldHouse/Entities/GhostFisherman", 1)
    quest:AddEntityBinding("FishermansWife", "HerosOldHouse/Entities/FishermansWife", 1)
    quest:AddEntityBinding("ExtraBooty", "HerosOldHouse/Entities/ExtraBooty", 1)
    quest:FinalizeEntityBindings()
    quest:CreateThread("WatchForBooty")  -- native thread body NScript::CQ_HerosOldHouseScript::WatchForBooty: lift it as function WatchForBooty(quest)
    quest:SetStateBool("MissionAborted", false)
    predicateResult = false
    while not quest:GetStateBool("BootyDugUp") do
        if not quest:NewScriptFrame() then return end
        if predicateResult then
            goto LAB_00d8a156
        else
            isRegionLoaded = quest:IsRegionLoaded("OakBay")
            if not isRegionLoaded or not quest:GetStateBool("WifeAttacked") then goto LAB_00d8a156 end
        end
        goto FLOW_past_lab_00d8a156
        ::LAB_00d8a156::
        isRegionLoaded = false
        ::FLOW_past_lab_00d8a156::
        if isRegionLoaded then
            if quest:GetStateBool("Helping") then
                quest:SetQuestAsFailed(quest:GetActiveQuestName(), false, "", true)
                predicateResult = true
                quest:SetStateBool("Helping", false)
            end
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Q_HerosOldHouse.Init (retail 0x00d89da0)
function Init(quest)
    quest:SetStateBool("Helping", false)
    quest:SetStateBool("Helped", false)
    quest:SetStateBool("WifeAttacked", false)
    quest:SetStateBool("WifeAttackedAndLeft", false)
    quest:SetStateBool("BootyDugUp", false)
end

-- Q_HerosOldHouse.OnPersist (retail 0x00d8ab10)
function OnPersist(quest, context)
    quest:SetStateBool("Helping", quest:PersistTransferBool(context, "Helping", quest:GetStateBool("Helping")))
    quest:SetStateBool("Helped", quest:PersistTransferBool(context, "Helped", quest:GetStateBool("Helped")))
    quest:SetStateBool("BootyDugUp", quest:PersistTransferBool(context, "BootyDugUp", quest:GetStateBool("BootyDugUp")))
end

-- Q_HerosOldHouse.WatchForBooty (retail 0x00d8a240)
function WatchForBooty(quest)
    local hiddenBooty = quest:GetThingWithScriptName("HiddenBooty")
    repeat
        if quest:IsActiveThreadTerminating() then return end
        while not quest:IsRegionLoaded("OakBay") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsDiggingSpotEnabled(hiddenBooty) then
            quest:NewScriptFrame()
        else
            if not quest:IsActiveThreadTerminating() then quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_02", "", "OakBay"); return end
            do return end
            quest:NewScriptFrame()
        end
    until false
end

