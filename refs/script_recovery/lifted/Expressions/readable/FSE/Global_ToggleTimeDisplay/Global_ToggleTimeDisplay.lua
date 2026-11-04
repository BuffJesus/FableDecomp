-- Readable native conversion: Global_ToggleTimeDisplay. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Global_ToggleTimeDisplay.Main (retail 0x00eecf20)
function Main(quest)
    local bDisplay = not quest:GetStateBool("TimeDisplayActive")
    quest:SetStateBool("TimeDisplayActive", bDisplay)
    quest:DisplayTime(bDisplay)
    local pQuestName = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pQuestName, 0)
end

-- Global_ToggleTimeDisplay.Init (retail 0x00eecf10)
function Init(quest)
    quest:SetStateBool("TimeDisplayActive", false)
end

-- Global_ToggleTimeDisplay.OnPersist (retail 0x00eed030)
function OnPersist(quest, context)
    local timeDisplayActive = quest:GetStateBool("TimeDisplayActive") or false
    timeDisplayActive = quest:PersistTransferBool(context, "TimeDisplayActive", timeDisplayActive)
    quest:SetStateBool("TimeDisplayActive", timeDisplayActive)
end

