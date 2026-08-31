local MY_SCRIPT_NAME = "MazeAtTavern"

local TalkCounter = 0
local WavedOver = false
local HitCount = 0

function Init(quest, me)
    TalkCounter = 0
    WavedOver = false
    HitCount = 0
end

function Main(quest, me)
    local myThing = quest:GetThingWithScriptName(MY_SCRIPT_NAME)
    if not myThing then
        quest:Log("!!! ERROR: MazeAtTavern Main - could not self-lookup '" .. MY_SCRIPT_NAME .. "'. Aborting.")
        return
    end

    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: MazeAtTavern Main - could not get Hero. Aborting.")
        return
    end

    -- Acquire scripted control handle for animations and speech
    me:AcquireControl()

    -- Setup opinion source for Bowerstone pub
    quest:EntitySetAsOpinionSourceByString(myThing, "OPINION_SOURCE_MAZE_BS_PUB")

    -- Setup mini-map marker, information icon, and disable pushing by Hero
    quest:MiniMapAddMarker(myThing, "HUD_ORB_QUEST_CORE")
    quest:SetThingHasInformation(me, false, true, false)
    quest:SetIsPushableByHero(myThing, false)

    while true do
        -- Wave the hero over if they are nearby and haven't spoken yet (fires once)
        if not quest:GetStateBool("GuardianSpokeToHero") and not WavedOver then
            if quest:IsDistanceBetweenThingsUnder(hero, myThing, 30.0) then
                local convoID = quest:StartAmbientConversation(myThing, hero, false, false)
                quest:AddLineToConversation(convoID, "TEXT_QST_027_MAZE_CALL_HERO_OVER_10", myThing, hero, false)
                me:PlayAnimation("ST_HELLO")
                WavedOver = true
            end
        end

        -- Face the hero until they have spoken to us
        if not quest:GetStateBool("GuardianSpokeToHero") then
            quest:EntitySetFacingAngleTowardsThing(me, hero)
        end

        -- React to being hit, but not to being healed
        local wasHit = me:MsgIsHitByHero()
            or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero())

        if wasHit then
            if quest:GetHealth(me) > 0.0 then
                me:SpeakAndWait("TEXT_QST_027_MAZE_ON_HIT_10")
            end

            me:SetFriendsWithEverythingFlag(true)
            quest:ClearThingBestEnemyTarget(myThing)
            HitCount = HitCount + 1
        end

        -- Talk triggers the cutscene and completes the meeting beat
        if me:IsTalkedToByHero() then
            -- Release our scripted control handle before playing cutscene so PlayCutscene can acquire ME & HERO
            me:ReleaseControl()
            quest:PlayCutscene("CS_GUARDIANSISTER_BOWERSTONE", { ME = me, HERO = hero }, {})
            quest:SetStateBool("GuardianSpokeToHero", true)
            quest:ClearThingHasInformation(me)

            local activeQuestName = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(activeQuestName, false, false, false)
            quest:DeactivateQuestLater(activeQuestName, 0)
            quest:RemoveThing(me)
            break
        end

        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end
end
