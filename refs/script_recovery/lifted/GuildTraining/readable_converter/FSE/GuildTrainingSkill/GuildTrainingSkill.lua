-- Readable native conversion: Q_GuildTrainingSkill. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTrainingSkill.Main (retail 0x00d5ab40)
function Main(quest)
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingSkill/Entities/TheRealGuildmaster")
    quest:FinalizeEntityBindings()
end

-- Q_GuildTrainingSkill.Init (retail 0x00d5aa80)
function Init(quest)
    quest:SetStateInt("TargetsHit", 0)
    quest:SetStateInt("TutorialState", 1)
    quest:SetStateInt("GenericTutorialCounter", 0)
    quest:SetStateInt("TotalTrainingDummiesCounter", 3)
end

