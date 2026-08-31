local MY_SCRIPT_NAME = "MazeAtTavern"

local TalkCounter = 0
local HitCount = 0

function Init(quest, me)
    TalkCounter = 0
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

    -- Setup mini-map marker and disable pushing by Hero
    quest:MiniMapAddMarker(myThing, "HUD_ORB_QUEST_CORE")
    quest:SetIsPushableByHero(myThing, false)

    -- Acquire scripted control handle for animations and speech
    me:AcquireControl()
    quest:SetThingHasInformation(me, true)

    local wavingTimer = quest:RegisterTimer()

    while true do
        -- Wave the hero over if they are nearby and the waving timer has elapsed
        if quest:GetTimer(wavingTimer) <= 0 then
            if quest:IsDistanceBetweenThingsUnder(hero, myThing, 30.0) then
                local convoID = quest:StartAmbientConversation(myThing, hero, false, false)
                quest:AddLineToConversation(convoID, "TEXT_QST_027_MAZE_CALL_HERO_OVER_10", myThing, hero, false)
                me:PlayAnimation("ST_HELLO")
                quest:SetTimer(wavingTimer, 5)
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
            quest:EntitySetThingAsAllyOfThing(myThing, hero)
            quest:EntitySetThingAsAllyOfThing(hero, myThing)

            if quest:GetHealth(me) > 0.0 then
                me:SpeakAndWait("TEXT_QST_076_MAZE_ON_HIT_10")
            end

            HitCount = HitCount + 1
        end

        -- First talk triggers the cutscene and completes the meeting beat
        if me:IsTalkedToByHero() then
            if TalkCounter == 0 then
                quest:ForceShipsVisible()
                -- Release our scripted control handle before playing cutscene so PlayCutscene can acquire MAZE & HERO
                me:ReleaseControl()
                quest:PlayCutscene("CS_GUARDIAN_SISTER_2", { MAZE = me, HERO = hero }, {})
                quest:SetStateBool("GuardianSpokeToHero", true)
                quest:AddQuestCard("OBJECT_QUEST_CARD_BANDIT_CAMP", "Q_BanditCamp", false, false)
                -- Re-acquire control for teleport animation
                me:AcquireControl()
            end
            TalkCounter = TalkCounter + 1
            quest:ClearThingHasInformation(me)
        end

        if quest:GetStateBool("GuardianSpokeToHero") then
            break
        end

        if not quest:NewScriptFrame(me) then
            quest:DeregisterTimer(wavingTimer)
            me:ReleaseControl()
            return
        end
    end

    -- Teleport-out and complete the quest
    me:ClearCommands()
    me:PlayAnimation("ST_TELEPORT_OUT")
    quest:CreateEffectAtPos("MAZE_TELEPORT_OUT_01", me:GetPos())
    quest:FadeOutAndKillEntity(me, true, 1.0, true)
    me:ReleaseControl()

    local activeQuestName = quest:GetActiveQuestName()
    quest:SetQuestAsCompleted(activeQuestName, false, false, false)
    quest:DeactivateQuestLater(activeQuestName, 0)

    quest:DeregisterTimer(wavingTimer)
end