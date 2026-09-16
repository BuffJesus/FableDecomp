-- Reviewed native Init for Q_GuildTrainingSkill.
-- Main remains excluded until its entity/resource state is converted.

function Init(questObject)
    Quest = questObject
    Quest:SetStateInt("TargetsHit", 0)
    Quest:SetStateInt("TutorialState", 1)
    Quest:SetStateInt("GenericTutorialCounter", 0)
    Quest:SetStateInt("TotalTrainingDummies", 3)
end
