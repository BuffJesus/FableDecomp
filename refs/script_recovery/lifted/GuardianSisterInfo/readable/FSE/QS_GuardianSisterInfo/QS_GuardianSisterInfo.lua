-- Readable native conversion: QS_GuardianSisterInfo. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- QS_GuardianSisterInfo.Main (retail 0x00e25ab0)
function Main(quest)
    quest:AddEntityBinding("MazeAtTavern", "QS_GuardianSisterInfo/Entities/MazeAtTavern")
    quest:FinalizeEntityBindings()
    local pQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_GUARDIAN_SISTER_INFO_FIRST_OBJECTIVE_01", "", "BowerstoneSlums")
    local isRegionLoaded = quest:IsRegionLoaded("BowerstoneSlums")
    while true do
        if isRegionLoaded then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetTimeOfDay(10.0)
            return
        end
        if not quest:NewScriptFrame() then break end
        isRegionLoaded = quest:IsRegionLoaded("BowerstoneSlums")
    end
end

-- QS_GuardianSisterInfo.Init (retail 0x00e25a00)
function Init(quest)
    quest:SetStateBool("GuardianSpokeToHero", false)
end

-- QS_GuardianSisterInfo.OnPersist (retail 0x00e25f40)
function OnPersist(quest, context)
    local guardianSpokeToHero = quest:GetStateBool("GuardianSpokeToHero") or false
    guardianSpokeToHero = quest:PersistTransferBool(context, "GuardianSpokeToHero", guardianSpokeToHero)
    quest:SetStateBool("GuardianSpokeToHero", guardianSpokeToHero)
end

