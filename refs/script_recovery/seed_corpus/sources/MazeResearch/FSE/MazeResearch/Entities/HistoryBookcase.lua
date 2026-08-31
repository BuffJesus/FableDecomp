local MY_SCRIPT_NAME = "HistoryBookcase"

function Init(quest, me)
end

function Main(quest, me)
    local myThing = quest:GetThingWithScriptName(MY_SCRIPT_NAME)
    if not myThing then
        quest:Log("!!! ERROR: HistoryBookcase Main - could not self-lookup '" .. MY_SCRIPT_NAME .. "'. Aborting.")
        return
    end

    if quest:GetStateBool("BookRead") then
        while quest:NewScriptFrame(me) do end
        return
    end

    quest:SetThingAsUsable(myThing, true)
    quest:MiniMapAddMarker(myThing, "HUD_ORB_QUEST_VIGNETTE")

    while true do
        if me:MsgIsUsedByHero() then
            break
        end

        if not quest:NewScriptFrame(me) then
            return
        end
    end

    -- Update Quest Card Objective
    local activeQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_02", "HeroGuildComplexInside", "")

    -- Remove marker from bookcase, add marker to EmptyGrave
    quest:MiniMapRemoveMarker(myThing)
    local emptyGrave = quest:GetThingWithScriptName("EmptyGrave")
    if emptyGrave then
        quest:MiniMapAddMarker(emptyGrave, "HUD_ORB_QUEST_VIGNETTE")
    else
        quest:Log("!!! WARNING: HistoryBookcase - could not find EmptyGrave for minimap marker.")
    end

    quest:SetStateBool("BookRead", true)

    -- Display 4 journal entries sequentially
    local journalEntries = {
        "TEXT_QST_B01_JOURNAL_ENTRY_10",
        "TEXT_QST_B01_JOURNAL_ENTRY_20",
        "TEXT_QST_B01_JOURNAL_ENTRY_30",
        "TEXT_QST_B01_JOURNAL_ENTRY_40",
    }

    for _, entryTag in ipairs(journalEntries) do
        quest:DisplayGameInfo(entryTag)
        while not quest:MsgIsGameInfoClickedPast() do
            if not quest:NewScriptFrame(me) then
                return
            end
        end
    end

    while quest:NewScriptFrame(me) do end
end
