-- Generated native draft: Q_GuildTrainingSkill. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingSkill/Entities/TheRealGuildmaster", 1)
    quest:FinalizeEntityBindings()
end

function Init(quest)
    quest:SetStateInt("TargetsHit", 0)
    quest:SetStateInt("TutorialState", 1)
    quest:SetStateInt("GenericTutorialCounter", 0)
    quest:SetStateInt("TotalTrainingDummiesCounter", 3)
end

