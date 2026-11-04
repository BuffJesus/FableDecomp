-- Readable native conversion: Q_GuildTrainingWill. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_GuildTrainingWill.Main (retail 0x00d5dd40)
function Main(quest)
    quest:AddEntityBinding("TheRealGuildmaster", "GuildTrainingWill/Entities/TheRealGuildmaster", 1)
    quest:FinalizeEntityBindings()
end

-- Q_GuildTrainingWill.Init (retail 0x00d5dc80)
function Init(quest)
    quest:SetStateInt("TargetsHit", 0)
    quest:SetStateInt("TutorialState", 1)
    quest:SetStateInt("TotalTrainingDummiesCounter", 0)
    quest:SetStateInt("GenericTutorialCounter", 0)
    quest:SetStateBool("TestFinished", false)
    quest:SetStateBool("BanditsDefeated", false)
end

-- Q_GuildTrainingWill.OnPersist (retail 0x00d5e070)
function OnPersist(quest, context)
    local testFinished = quest:GetStateBool("TestFinished") or false
    testFinished = quest:PersistTransferBool(context, "TestFinished", testFinished)
    quest:SetStateBool("TestFinished", testFinished)
    local banditsDefeated = quest:GetStateBool("BanditsDefeated") or false
    banditsDefeated = quest:PersistTransferBool(context, "BanditsDefeated", banditsDefeated)
    quest:SetStateBool("BanditsDefeated", banditsDefeated)
end

