-- Generated native draft: Global_ToggleTimeDisplay. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bDisplay = not quest:GetStateBool("TimeDisplayActive")
    quest:SetStateBool("TimeDisplayActive", bDisplay)
    quest:DisplayTime(bDisplay)
    local pQuestName = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pQuestName, 0)
end

function Init(quest)
    quest:SetStateBool("TimeDisplayActive", false)
end

function OnPersist(quest, context)
    local timeDisplayActive = quest:GetStateBool("TimeDisplayActive") or false
    timeDisplayActive = quest:PersistTransferBool(context, "TimeDisplayActive", timeDisplayActive)
    quest:SetStateBool("TimeDisplayActive", timeDisplayActive)
end

