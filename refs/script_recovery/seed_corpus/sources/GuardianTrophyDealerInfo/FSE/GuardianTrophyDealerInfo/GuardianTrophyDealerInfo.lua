Quest = nil

local QUEST_NAME = "GuardianTrophyDealerInfo"

function Init(questObject)
    Quest = questObject
    Quest:Log("GuardianTrophyDealerInfo: Init phase started.")
    Quest:SetStateBool("PieceOver", false)
end

function Main(questObject)
    Quest = questObject
    Quest:Log("GuardianTrophyDealerInfo: Main() started. Setting up entity bindings...")

    Quest:AddEntityBinding("GTDI_Maze", "GuardianTrophyDealerInfo/Entities/GTDI_Maze")
    Quest:FinalizeEntityBindings()

    local activeQuestName = Quest:GetActiveQuestName()
    Quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_GUARDIAN_TROPHY_DEALER_INFO_OBJECTIVE_01", "HeroGuildComplexInside", "HeroGuildComplexInside")
    Quest:Log("GuardianTrophyDealerInfo: Initial objective set.")

    -- Wait until the trophy dealer info cutscene sequence has completed
    while not Quest:GetStateBool("PieceOver") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    -- Wait until GTDI_Maze is no longer alive / unloaded
    local maze = Quest:GetThingWithScriptName("GTDI_Maze")
    if maze and maze:IsAlive() then
        while maze:IsAlive() do
            if not Quest:NewScriptFrame() then
                return
            end
        end
    end

    activeQuestName = Quest:GetActiveQuestName()
    Quest:DeactivateQuestLater(activeQuestName, 0)
end

function OnPersist(questObject, context)
    Quest = questObject
    local pieceOver = Quest:GetStateBool("PieceOver") or false
    pieceOver = Quest:PersistTransferBool(context, "PieceOver", pieceOver)
    Quest:SetStateBool("PieceOver", pieceOver)
end
