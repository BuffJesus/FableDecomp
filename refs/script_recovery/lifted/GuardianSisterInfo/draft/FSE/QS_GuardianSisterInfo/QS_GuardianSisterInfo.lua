-- Generated native draft: QS_GuardianSisterInfo. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local alive = true
    quest:AddEntityBinding("MazeAtTavern", "QS_GuardianSisterInfo/Entities/MazeAtTavern")
    quest:FinalizeEntityBindings()
    local pQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_GUARDIAN_SISTER_INFO_FIRST_OBJECTIVE_01", "", "BowerstoneSlums")
    local bVar2 = quest:IsRegionLoaded("BowerstoneSlums")
    while true do
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:SetTimeOfDay(10.0)
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then break end
        bVar2 = quest:IsRegionLoaded("BowerstoneSlums")
    end
end

function Init(quest)
    quest:SetStateBool("GuardianSpokeToHero", false)
end

function OnPersist(quest, context)
    local guardianSpokeToHero = quest:GetStateBool("GuardianSpokeToHero") or false
    guardianSpokeToHero = quest:PersistTransferBool(context, "GuardianSpokeToHero", guardianSpokeToHero)
    quest:SetStateBool("GuardianSpokeToHero", guardianSpokeToHero)
end

