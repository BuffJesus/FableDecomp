-- Readable native conversion: Q_GuildTrainingDeparture. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTrainingDeparture.Main (retail 0x00d50800)
function Main(quest)
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingDeparture/Entities/TheRealGuildmaster")
    quest:FinalizeEntityBindings()
    while not quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    while quest:IsQuestActive("Q_GuildTrainingWoodsDeparture") do
        if not quest:NewScriptFrame() then return end
    end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Q_GuildTrainingDeparture.Init (retail 0x00d506b0)
function Init(quest)
    quest:EntityTeleportToThing(quest:GetHero(), quest:GetThingWithScriptName("HeroDepartureStartMarker"), false)
    quest:FadeScreenIn()
    quest:SetStateBool("Finished", false)
    quest:SetStateInt("MeleeGrade", 0)
    quest:SetStateInt("SkillGrade", 0)
    quest:SetStateInt("WillGrade", 0)
end

