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
            if quest:IsActiveThreadTerminating() then return end
            if quest:GetStateBool("Helping") then
                if quest:IsActiveThreadTerminating() then return end
                local pQuestName = quest:GetActiveQuestName()
                quest:SetQuestAsFailed(pQuestName, false, "", true)
                predicateResult = true
                quest:SetStateBool("Helping", false)
            end
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    local getActiveQuestName = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName, 0)
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
    local helping = quest:GetStateBool("Helping") or false
    helping = quest:PersistTransferBool(context, "Helping", helping)
    quest:SetStateBool("Helping", helping)
    local helped = quest:GetStateBool("Helped") or false
    helped = quest:PersistTransferBool(context, "Helped", helped)
    quest:SetStateBool("Helped", helped)
    local bootyDugUp = quest:GetStateBool("BootyDugUp") or false
    bootyDugUp = quest:PersistTransferBool(context, "BootyDugUp", bootyDugUp)
    quest:SetStateBool("BootyDugUp", bootyDugUp)
end

-- Q_HerosOldHouse.WatchForBooty (retail 0x00d8a240)
function WatchForBooty(quest)
    local hiddenBooty = quest:GetThingWithScriptName("HiddenBooty")
    local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    repeat
        if isActiveThreadTerminating then
            hiddenBooty = nil
            return
        end
        if not quest:IsRegionLoaded("OakBay") then
            if quest:IsActiveThreadTerminating() then hiddenBooty = nil; return end
            while not quest:IsRegionLoaded("OakBay") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsDiggingSpotEnabled(hiddenBooty) then
            quest:NewScriptFrame();
            isActiveThreadTerminating = quest:IsActiveThreadTerminating();
        else
            if not quest:IsActiveThreadTerminating() then
                local pQuestName = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_02", "", "OakBay")
                return
            end
            do return end
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        end
        ::continue_1::
    until false
end

