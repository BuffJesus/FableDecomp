-- Readable native conversion: QS_GuardianSisterInfo2_SisterInBanditCamp. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- QS_GuardianSisterInfo2_SisterInBanditCamp.Main (retail 0x00e268c0)
function Main(quest)
    quest:AddEntityBinding("MazeAtTavern", "QS_GuardianSisterInfo2_SisterInBanditCamp/Entities/MazeAtTavern")
    quest:FinalizeEntityBindings()
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_GUARDIAN_SISTER_INFO_SECOND_OBJECTIVE_01", "", "OakBay")
    while not quest:GetStateBool("GuardianSpokeToHero") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local mazeAtTavern = quest:GetThingWithScriptName("MazeAtTavern")
    while mazeAtTavern ~= nil and mazeAtTavern:IsAlive() do
        if not quest:NewScriptFrame() then goto LAB_00e26aca end
    end
    quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    ::LAB_00e26aca::
end

-- QS_GuardianSisterInfo2_SisterInBanditCamp.Init (retail 0x00e26810)
function Init(quest)
    quest:SetStateBool("GuardianSpokeToHero", false)
end

-- QS_GuardianSisterInfo2_SisterInBanditCamp.OnPersist (retail 0x00e26e00)
function OnPersist(quest, context)
    quest:SetStateBool("GuardianSpokeToHero", quest:PersistTransferBool(context, "GuardianSpokeToHero", quest:GetStateBool("GuardianSpokeToHero")))
end

