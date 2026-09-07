-- DemonDoorFace.lua
-- Entity script for DemonDoorFace in Witchwood
-- Functional parity port of NScript::CV_TrophyDealerScript::CDemonDoorFace

local MY_SCRIPT_NAME = "DemonDoorFace"

local STATE_WAIT_FOR_TALK_OR_SYNC = 1
local STATE_OPENING = 2
local STATE_DONE = 3

function Init(quest, me)
    quest:Log("DemonDoorFace: Init() started.")

    -- Setup info icon, persistence, and damage immunity
    quest:SetThingHasInformation(me, false, true, false)
    quest:SetThingPersistent(me, true)
    quest:EntitySetAsDamageable(me, false)

    quest:SetStateInt("DemonDoorState", STATE_WAIT_FOR_TALK_OR_SYNC)
end

function Main(quest, me)
    quest:Log("DemonDoorFace: Main() started.")

    -- Get Hero reference once outside any loops (prevents memory leaks)
    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: DemonDoorFace Main - could not get Hero.")
        return
    end

    -- Acquire script control
    if not me:AcquireControl() then
        quest:Log("!!! ERROR: DemonDoorFace Main - failed to acquire control.")
        return
    end

    -- Add minimap objective marker
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")

    while true do
        local currentState = quest:GetStateInt("DemonDoorState") or STATE_WAIT_FOR_TALK_OR_SYNC

        if currentState == STATE_WAIT_FOR_TALK_OR_SYNC then
            -- Check if Hero interacted with the Demon Door
            if me:IsTalkedToByHero() then
                local stonesInSync = quest:GetMasterGameState("SingingStonesInSync") or false
                if not stonesInSync then
                    if quest:GetHealth(me) > 0.0 then
                        -- Speak locked dialogue line (blocking variation for single-task entity)
                        me:SpeakAndWait("TEXT_QST_071_DOOR_LOCKED1", 0)
                    end

                    local spokenBefore = quest:GetMasterGameState("TrophyDealerHeroSpokenToDemonDoors") or false
                    if not spokenBefore then
                        quest:SetMasterGameState("TrophyDealerHeroSpokenToDemonDoors", true)
                        local activeQuestName = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_03", "Witchwood2", "")
                        quest:Log("DemonDoorFace: Hero spoke to locked Demon Door. Objective 03 set.")
                    end
                end
            end

            -- Check if Singing Stones have been solved
            if quest:GetMasterGameState("SingingStonesInSync") then
                quest:Log("DemonDoorFace: SingingStonesInSync is true. Transitioning to STATE_OPENING.")
                quest:SetStateInt("DemonDoorState", STATE_OPENING)
                currentState = STATE_OPENING
            end
        end

        if currentState == STATE_OPENING then
            -- Release script control before playing cutscene
            me:ReleaseControl()

            quest:Log("DemonDoorFace: Playing CS_TROPHY_DEALER_DOOR_OPENS cutscene...")
            quest:PlayCutscene("CS_TROPHY_DEALER_DOOR_OPENS", { HERO = hero, DOOR = me })
            quest:Log("DemonDoorFace: Cutscene finished.")

            -- Update quest card objective
            local activeQuestName = quest:GetActiveQuestName()
            quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_04", "WitchwoodCavern", "")

            -- Ensure the cavern door is persistent
            local door = quest:GetThingWithScriptName("DemonDoorDoor")
            if door then
                quest:SetThingPersistent(door, true)
            else
                quest:Log("!!! WARNING: DemonDoorDoor thing not found.")
            end

            -- Reset SingingStonesInSync flag in master game state
            quest:SetMasterGameState("SingingStonesInSync", false)

            -- Remove marker and remove the door face entity
            quest:MiniMapRemoveMarker(me)
            quest:RemoveThing(me)
            quest:Log("DemonDoorFace: Removed entity. Door is open.")

            quest:SetStateInt("DemonDoorState", STATE_DONE)
            return
        end

        if currentState == STATE_DONE then
            return
        end

        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end
end

function OnPersist(quest, me, context)
    local state = quest:GetStateInt("DemonDoorState") or STATE_WAIT_FOR_TALK_OR_SYNC
    state = quest:PersistTransferInt(context, "DemonDoorState", state)
    quest:SetStateInt("DemonDoorState", state)
end
