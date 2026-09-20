-- Generated native draft: Q_GuildTrainingWill. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingWill/Entities/TheRealGuildmaster", 1)
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
    local testFinished = quest:GetStateBool("TestFinished") or false
    testFinished = quest:PersistTransferBool(context, "TestFinished", testFinished)
    quest:SetStateBool("TestFinished", testFinished)
    local banditsDefeated = quest:GetStateBool("BanditsDefeated") or false
    banditsDefeated = quest:PersistTransferBool(context, "BanditsDefeated", banditsDefeated)
    quest:SetStateBool("BanditsDefeated", banditsDefeated)
end

