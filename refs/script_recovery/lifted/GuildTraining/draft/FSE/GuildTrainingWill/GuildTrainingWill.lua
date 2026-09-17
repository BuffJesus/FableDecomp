-- Generated native draft: Q_GuildTrainingWill. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingWill/Entities/TheRealGuildmaster")
    quest:FinalizeEntityBindings()
end

function Init(quest)
    quest:SetStateInt("TargetsHit", 0)
    quest:SetStateInt("TutorialState", 1)
    quest:SetStateInt("TotalTrainingDummiesCounter", 0)
    quest:SetStateInt("GenericTutorialCounter", 0)
    quest:SetStateBool("TestFinished", false)
    quest:SetStateBool("BanditsDefeated", false)
end

function OnPersist(quest, context)
end

