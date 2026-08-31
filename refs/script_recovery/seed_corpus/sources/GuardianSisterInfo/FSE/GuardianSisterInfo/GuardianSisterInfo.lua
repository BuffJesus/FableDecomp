Quest = nil

local QUEST_NAME = "GuardianSisterInfo"

function Init(questObject)
    Quest = questObject
    Quest:Log("GuardianSisterInfo: Init phase started.")
    Quest:SetStateBool("GuardianSpokeToHero", false)
end

function Main(questObject)
    Quest = questObject
    Quest:Log("GuardianSisterInfo: Main() started. Setting up entity bindings...")

    Quest:AddEntityBinding("MazeAtTavern", "GuardianSisterInfo/Entities/MazeAtTavern")
    Quest:FinalizeEntityBindings()

    local activeQuestName = Quest:GetActiveQuestName()
    Quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_GUARDIAN_SISTER_INFO_FIRST_OBJECTIVE_01", "", "BowerstoneSlums")
    Quest:Log("GuardianSisterInfo: Initial objective set.")

    -- Wait until BowerstoneSlums region is loaded before setting time of day
    while not Quest:IsRegionLoaded("BowerstoneSlums") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    Quest:SetTimeOfDay(10.0)

    -- Wait until interaction is completed
    while not Quest:GetStateBool("GuardianSpokeToHero") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    -- Wait until Maze is no longer alive / removed
    local maze = Quest:GetThingWithScriptName("MazeAtTavern")
    if maze and maze:IsAlive() then
        while maze:IsAlive() do
            if not Quest:NewScriptFrame() then
                return
            end
        end
    end

    -- Complete and deactivate the quest
    activeQuestName = Quest:GetActiveQuestName()
    Quest:SetQuestAsCompleted(activeQuestName, false, false, false)
    Quest:DeactivateQuestLater(activeQuestName, 0)
end

function OnPersist(questObject, context)
    Quest = questObject
    local guardianSpokeToHero = Quest:GetStateBool("GuardianSpokeToHero") or false
    guardianSpokeToHero = Quest:PersistTransferBool(context, "GuardianSpokeToHero", guardianSpokeToHero)
    Quest:SetStateBool("GuardianSpokeToHero", guardianSpokeToHero)
end
