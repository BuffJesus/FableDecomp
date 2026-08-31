Quest = nil

local QUEST_NAME = "MeetSister"

function Init(questObject)
    Quest = questObject
    Quest:Log("MeetSister: Init phase started.")
end

function Main(questObject)
    Quest = questObject
    Quest:Log("MeetSister: Main() started. Setting up entity bindings...")

    Quest:AddEntityBinding("MeetSisterMessenger", "MeetSister/Entities/MeetSisterMessenger")
    Quest:AddEntityBinding("MeetSisterSister", "MeetSister/Entities/MeetSisterSister")
    Quest:FinalizeEntityBindings()

    local activeQuestName = Quest:GetActiveQuestName()
    Quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY", "HauntedHouse", "BarrowFields")
    Quest:Log("MeetSister: Initial objective set.")
end