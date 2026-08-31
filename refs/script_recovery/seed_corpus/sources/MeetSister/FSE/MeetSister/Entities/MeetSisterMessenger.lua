local MY_SCRIPT_NAME = "MeetSisterMessenger"

function Init(quest, me)
end

function Main(quest, me)
    local myThing = quest:GetThingWithScriptName(MY_SCRIPT_NAME)
    if not myThing then
        quest:Log("!!! ERROR: MeetSisterMessenger Main - could not self-lookup '" .. MY_SCRIPT_NAME .. "'. Aborting.")
        return
    end

    while not quest:IsRegionLoaded("ArenaExterior") do
        if not quest:NewScriptFrame(me) then
            return
        end
    end

    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: MeetSisterMessenger - could not get Hero.")
        return
    end

	quest:PlayCutscene("CS_ARENA_LEAVE_THERESA_01", { Hero = hero, Messenger = me }, {})

    quest:RemoveThing(myThing)
end