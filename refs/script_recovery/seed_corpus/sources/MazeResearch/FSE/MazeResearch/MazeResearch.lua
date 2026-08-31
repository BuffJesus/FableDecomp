Quest = nil

local QUEST_NAME = "MazeResearch"

function Init(questObject)
    Quest = questObject
    Quest:Log("MazeResearch: Init phase started.")
end

function Main(questObject)
    Quest = questObject
    Quest:Log("MazeResearch: Main() started. Setting up entity bindings...")

    Quest:AddEntityBinding("EmptyGrave", "MazeResearch/Entities/EmptyGrave")
    Quest:AddEntityBinding("HistoryBookcase", "MazeResearch/Entities/HistoryBookcase")
    Quest:FinalizeEntityBindings()

    local activeQuestName = Quest:GetActiveQuestName()
    Quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01", "HeroGuildComplexInside", "")
    Quest:Log("MazeResearch: Initial objective set.")
end

function OnPersist(questObject, context)
    Quest = questObject
    local swordTaken = Quest:GetStateBool("SwordTaken") or false
    swordTaken = Quest:PersistTransferBool(context, "SwordTaken", swordTaken)
    Quest:SetStateBool("SwordTaken", swordTaken)

    local bookRead = Quest:GetStateBool("BookRead") or false
    bookRead = Quest:PersistTransferBool(context, "BookRead", bookRead)
    Quest:SetStateBool("BookRead", bookRead)
end
