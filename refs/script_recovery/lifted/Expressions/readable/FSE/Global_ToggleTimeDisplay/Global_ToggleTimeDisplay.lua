-- Readable native conversion: Global_ToggleTimeDisplay. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Global_ToggleTimeDisplay.Main (retail 0x00eecf20)
function Main(quest)
    local bDisplay = not quest:GetStateBool("TimeDisplayActive")
    quest:SetStateBool("TimeDisplayActive", bDisplay)
    quest:DisplayTime(bDisplay)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Global_ToggleTimeDisplay.Init (retail 0x00eecf10)
function Init(quest)
    quest:SetStateBool("TimeDisplayActive", false)
end

-- Global_ToggleTimeDisplay.OnPersist (retail 0x00eed030)
function OnPersist(quest, context)
    quest:SetStateBool("TimeDisplayActive", quest:PersistTransferBool(context, "TimeDisplayActive", quest:GetStateBool("TimeDisplayActive")))
end

