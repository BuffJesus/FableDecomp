-- Readable native conversion: QS_GuardianSisterInfo2_SisterInBanditCamp. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- QS_GuardianSisterInfo2_SisterInBanditCamp.Main (retail 0x00e268c0)
function Main(quest)
    quest:AddEntityBinding("MazeAtTavern", "QS_GuardianSisterInfo2_SisterInBanditCamp/Entities/MazeAtTavern")
    quest:FinalizeEntityBindings()
    local pQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_GUARDIAN_SISTER_INFO_SECOND_OBJECTIVE_01", "", "OakBay")
    while not quest:GetStateBool("GuardianSpokeToHero") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local mazeAtTavern = quest:GetThingWithScriptName("MazeAtTavern")
    while mazeAtTavern ~= nil and mazeAtTavern:IsAlive() do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local getActiveQuestName = quest:GetActiveQuestName()
    quest:SetQuestAsCompleted(getActiveQuestName, false, false, false)
    local getActiveQuestName2 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName2, 0)
end

-- QS_GuardianSisterInfo2_SisterInBanditCamp.Init (retail 0x00e26810)
function Init(quest)
    quest:SetStateBool("GuardianSpokeToHero", false)
end

-- QS_GuardianSisterInfo2_SisterInBanditCamp.OnPersist (retail 0x00e26e00)
function OnPersist(quest, context)
    local guardianSpokeToHero = quest:GetStateBool("GuardianSpokeToHero") or false
    guardianSpokeToHero = quest:PersistTransferBool(context, "GuardianSpokeToHero", guardianSpokeToHero)
    quest:SetStateBool("GuardianSpokeToHero", guardianSpokeToHero)
end

