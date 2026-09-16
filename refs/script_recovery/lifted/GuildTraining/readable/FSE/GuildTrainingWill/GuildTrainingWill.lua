-- Reviewed native Init for Q_GuildTrainingWill.
-- Main remains excluded until its entity/resource state is converted.

function Init(questObject)
    Quest = questObject
    Quest:SetStateInt("TargetsHit", 0)
    Quest:SetStateInt("TutorialState", 1)
    Quest:SetStateInt("TotalTrainingDummiesCounter", 0)
    Quest:SetStateInt("GenericTutorialCounter", 0)
    Quest:SetStateBool("TestFinished", false)
    Quest:SetStateBool("BanditsDefeated", false)
end
