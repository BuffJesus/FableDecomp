-- ManWithDoorName.lua
-- Entity script for ManWithDoorName in Witchwood Stones (Witchwood2)
-- Functional parity port of NScript::CV_SingingStonesScript::CManWithDoorName

local MY_SCRIPT_NAME = "ManWithDoorName"
local REGION_NAME = "Witchwood2"

-- Speech selection methods
local GROUP_SELECT_FIRST             = 0
local GROUP_SELECT_RANDOM            = 1
local GROUP_SELECT_RANDOM_NO_REPEAT  = 2
local GROUP_SELECT_SEQUENTIAL        = 3
local GROUP_SELECT_NONE              = 4

-- Movement types
local ENTITY_MOVE_RUN = 1

function Init(quest, me)
    quest:SetThingPersistent(me, true)
    me:MakeBehavioral()
end

function Main(quest, me)
    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: ManWithDoorName Main - could not get Hero. Aborting.")
        return
    end

    local myThing = quest:GetThingWithScriptName(MY_SCRIPT_NAME)
    if not myThing then
        quest:Log("!!! ERROR: ManWithDoorName Main - could not self-lookup '" .. MY_SCRIPT_NAME .. "'. Aborting.")
        return
    end

    -- Setup info marker and wandering parameters
    quest:SetThingHasInformation(me, false, true, false)
    quest:SetWanderCentrePoint(myThing, me:GetPos())
    quest:SetWanderMinDistance(myThing, 0.0)
    quest:SetWanderMaxDistance(myThing, 10.0)
    quest:SetScriptingStateGroup(myThing, 4)

    local state = "WANDERING"
    local exitThing = nil

    while true do
        if state == "WANDERING" then
            -- 1. Check if hit by Hero
            local wasHit = me:MsgIsHitByHero()
                or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero())

            if wasHit then
                if quest:GetHealth(me) > 0.0001 then
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    me:AcquireControl()
                    if quest:GetHealth(me) > 0.0 then
                        me:Speak(hero, "TEXT_QST_060_MAN_WITH_DOOR_NAME_HIT", GROUP_SELECT_NONE)
                    end
                    me:ReleaseControl()
                    quest:PauseAllNonScriptedEntities(false)
                    quest:EndMovieSequence()
                end
                quest:SetStateBool("DoorManAttackedByHero", true)
            end

            -- 2. Check if bribe received and ready to depart
            if quest:GetStateBool("DoorManHasBribe") and not quest:GetStateBool("DoorManComplete") then
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                me:AcquireControl()
                if quest:GetHealth(me) > 0.0 then
                    me:Speak(hero, "TEXT_QST_060_MAN_WITH_DOOR_NAME_SPEAKS", GROUP_SELECT_NONE)
                end
                me:ReleaseControl()
                quest:PauseAllNonScriptedEntities(false)
                quest:EndMovieSequence()

                quest:SetStateBool("DoorManComplete", true)
                quest:ClearThingHasInformation(me)

                exitThing = quest:GetNearestWithDefName(me, "REGION_EXIT_POINT")
                if exitThing and exitThing:IsAlive() then
                    me:AcquireControl()
                    me:MoveToPosition(exitThing:GetPos(), 1.0, ENTITY_MOVE_RUN)
                    state = "FLEEING"
                else
                    me:FadeOutAndKillEntity(true, 1.0, true)
                    state = "DONE"
                    return
                end
            end

            -- 3. Check if talked to by Hero
            if state == "WANDERING" and me:IsTalkedToByHero() then
                local introComplete = quest:GetStateBool("DoorManIntroComplete")
                local attacked = quest:GetStateBool("DoorManAttackedByHero")

                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                me:AcquireControl()
                if quest:GetHealth(me) > 0.0 then
                    if introComplete then
                        if attacked then
                            me:Speak(hero, "TEXT_QST_060_MAN_WITH_DOOR_NAME_ATTACKED", GROUP_SELECT_RANDOM_NO_REPEAT)
                        else
                            me:Speak(hero, "TEXT_QST_060_MAN_WITH_DOOR_NAME_REMINDER", GROUP_SELECT_RANDOM_NO_REPEAT)
                        end
                    else
                        me:Speak(hero, "TEXT_QST_060_MAN_WITH_DOOR_NAME_INTRO", GROUP_SELECT_NONE)
                        quest:SetStateBool("DoorManIntroComplete", true)
                    end
                end
                me:ReleaseControl()
                quest:PauseAllNonScriptedEntities(false)
                quest:EndMovieSequence()
            end

            -- 4. Check if presented with an item
            if state == "WANDERING" and not quest:GetStateBool("DoorManHasBribe") then
                if me:MsgIsPresentedWithItem() then
                    local presentedItem = g_PresentedItemName
                    if presentedItem == "OBJECT_GEMSTONE_RUBY" then
                        quest:AddItemToContainer(me, "OBJECT_GEMSTONE_RUBY")
                        quest:SetStateBool("DoorManHasBribe", true)
                        -- Departure sequence will execute on the bribe check above
                    else
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        me:AcquireControl()
                        if quest:GetHealth(me) > 0.0 then
                            me:Speak(hero, "TEXT_QST_060_MAN_WITH_DOOR_WRONG_ITEM", GROUP_SELECT_RANDOM_NO_REPEAT)
                        end
                        me:ReleaseControl()
                        quest:PauseAllNonScriptedEntities(false)
                        quest:EndMovieSequence()
                    end
                end
            end

        elseif state == "FLEEING" then
            local reachedExit = exitThing and quest:IsDistanceBetweenThingsUnder(me, exitThing, 2.0)
            local regionLeft = not quest:IsRegionLoaded(REGION_NAME)

            if reachedExit or regionLeft then
                me:FadeOutAndKillEntity(true, 1.0, true)
                me:ReleaseControl()
                state = "DONE"
                return
            end
        end

        if not quest:NewScriptFrame(me) then
            me:ReleaseControl()
            return
        end
    end
end
