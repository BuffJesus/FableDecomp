local MY_SCRIPT_NAME = "MeetSisterSister"

function Init(quest, me)
end

function Main(quest, me)
    local myThing = quest:GetThingWithScriptName(MY_SCRIPT_NAME)
    if not myThing then
        quest:Log("!!! ERROR: MeetSisterSister Main - could not self-lookup '" .. MY_SCRIPT_NAME .. "'. Aborting.")
        return
    end

    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: MeetSisterSister - could not get Hero.")
        return
    end

    quest:PlayCutscene("CS_ARENA_LEAVE_THERESA_02", { Hero = hero, Sister = me }, {})

    quest:RemoveThing(myThing)

    local activeQuestName = quest:GetActiveQuestName()
    quest:SetQuestAsCompleted(activeQuestName, false, false, false)
    quest:DeactivateQuestLater(activeQuestName, 0)
    quest:FadeScreenIn()
end