local MY_SCRIPT_NAME = "GTDI_Maze"

local InitialAngle = 0.0

function Init(quest, me)
    InitialAngle = me:GetAngleXY()
end

function Main(quest, me)
    local myThing = quest:GetThingWithScriptName(MY_SCRIPT_NAME)
    if not myThing then
        quest:Log("!!! ERROR: GTDI_Maze Main - could not self-lookup '" .. MY_SCRIPT_NAME .. "'. Aborting.")
        return
    end

    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: GTDI_Maze Main - could not get Hero. Aborting.")
        return
    end

    -- Setup info icon, control handle, entity properties, and quest start screen
    quest:SetThingHasInformation(me, false, true, false)
    me:AcquireControl()

    quest:EntitySetAsKillable(me, false)
    quest:EntitySetAsDamageable(me, false)
    quest:SetIsPushableByHero(me, false)

    local activeQuestName = quest:GetActiveQuestName()
    quest:KickOffQuestStartScreen(activeQuestName, false, false)

    quest:MiniMapAddMarker(myThing, "HUD_ORB_QUEST_CORE")

    -- Pre-interaction loop: wait for Hero to either hit or talk to Maze
    while true do
        local wasHit = me:MsgIsHitByHero()
            or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero())

        if wasHit then
            quest:EntitySetThingAsAllyOfThing(myThing, hero)
            quest:EntitySetThingAsAllyOfThing(hero, myThing)

            -- Release scripted control before playing cutscenes so the cutscene system can acquire MAZE & HERO
            me:ReleaseControl()
            quest:PlayCutscene({ "CS_MAZE_TROPHY_INFO_SETUP", "CS_MAZE_TROPHY_INFO_HITME", "CS_MAZE_TROPHY_INFO" }, { MAZE = me, HERO = hero })
            me:AcquireControl()
            break
        end

        if me:IsTalkedToByHero() then
            me:ReleaseControl()
            quest:PlayCutscene({ "CS_MAZE_TROPHY_INFO_SETUP", "CS_MAZE_TROPHY_INFO" }, { MAZE = me, HERO = hero })
            me:AcquireControl()
            break
        end

        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end

    -- Quest Completion Beat
    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_FIND_TROPHY_DEALER", "V_TrophyDealer", false)

    local teleporter = quest:GetThingWithScriptName("WitchwoodTeleporter")
    if teleporter then
        quest:SetTeleporterAsActive(teleporter, true)
    else
        quest:Log("!!! WARNING: WitchwoodTeleporter not found!")
    end
    quest:SetTeleportingAsActive(true)

    quest:ClearThingHasInformation(me)
    quest:MiniMapRemoveMarker(myThing)
    quest:EntitySetFacingAngle(me, InitialAngle)
    quest:AddLogbookStoryEntry(130)

    activeQuestName = quest:GetActiveQuestName()
    quest:SetQuestAsCompleted(activeQuestName, false, false, false)
    quest:SetStateBool("PieceOver", true)

    -- Post-meeting Repeat Loop: Maze remains in Guild and responds to repeated hits or talks
    while true do
        local wasHit = me:MsgIsHitByHero()
            or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero())

        if wasHit then
            quest:EntitySetThingAsAllyOfThing(myThing, hero)
            quest:EntitySetThingAsAllyOfThing(hero, myThing)
            quest:EntitySetFacingAngleTowardsThing(me, hero)

            if quest:GetHealth(me) > 0.0 then
                me:SpeakAndWait("TEXT_QST_077_MAZE_ON_HIT")
            end
        elseif me:IsTalkedToByHero() then
            quest:EntitySetFacingAngleTowardsThing(me, hero)
            if quest:GetHealth(me) > 0.0 then
                me:SpeakAndWait("TEXT_QST_077_MAZE_REPEAT")
            end
        end

        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end
end
