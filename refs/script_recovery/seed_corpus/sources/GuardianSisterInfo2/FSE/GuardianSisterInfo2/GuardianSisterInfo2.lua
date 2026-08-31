Quest = nil

local QUEST_NAME = "GuardianSisterInfo2"

function Init(questObject)
    Quest = questObject
    Quest:Log("GuardianSisterInfo2: Init phase started.")
    Quest:SetStateBool("GuardianSpokeToHero", false)
end

function Main(questObject)
    Quest = questObject
    Quest:Log("GuardianSisterInfo2: Main() started. Setting up entity bindings...")

    Quest:AddEntityBinding("MazeAtTavern", "GuardianSisterInfo2/Entities/MazeAtTavern")
    Quest:FinalizeEntityBindings()

    local activeQuestName = Quest:GetActiveQuestName()
    -- NOTE: 3rd arg ("First2" in the decompile) is uncertain -- see write-up. Using "" (no location override)
    -- to match the pattern established in MazeResearch's SetQuestCardObjective call.
    Quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_GUARDIAN_SISTER_INFO_SECOND_OBJECTIVE_01", "", "OakBay")
    Quest:Log("GuardianSisterInfo2: Initial objective set.")

    while not Quest:GetStateBool("GuardianSpokeToHero") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    local maze = Quest:GetThingWithScriptName("MazeAtTavern")
    if maze and maze:IsAlive() then
        while maze:IsAlive() do
            if not Quest:NewScriptFrame() then
                return
            end
        end
    end

    -- Complete the quest. NOTE: MazeAtTavern's own Main() also calls this pair of functions
    -- after the teleport-out sequence -- this duplication is faithful to the decompiled source,
    -- both scripts independently call SetQuestAsCompleted/DeactivateQuestLater.
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