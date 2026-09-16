-- Reviewed native Main/registration slice for Q_GuildTrainingDeparture.

function Main(questObject)
    Quest = questObject
    Quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingDeparture/Entities/TheRealGuildmaster")
    Quest:FinalizeEntityBindings()

    while not Quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") do
        if not Quest:NewScriptFrame() then return end
        if Quest:IsActiveThreadTerminating() then return end
    end
    while Quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") do
        if not Quest:NewScriptFrame() then return end
        if Quest:IsActiveThreadTerminating() then return end
    end
    if Quest:IsActiveThreadTerminating() then return end
    Quest:DeactivateQuestLater(Quest:GetActiveQuestName(), 0)
end
