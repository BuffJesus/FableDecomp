local TOLERANCE = 0.04
local MY_SCRIPT_NAME = "StatueMasterStatue"

local function GetCurrentTimeFraction(quest)
    local timeOfDay = quest:GetTimeOfDay()
    local hour = math.floor(timeOfDay / 100)
    local minute = timeOfDay % 100
    return (minute / 60.0 + hour) / 24.0
end

local function GetStatuePointingPosition(quest)
    local currentFraction = GetCurrentTimeFraction(quest)

    for i = 1, 3 do
        local angleStr = quest:GetStateString("SM_Angle" .. i)
        local angle = tonumber(angleStr)
        if angle and currentFraction > (angle - TOLERANCE) and currentFraction < (angle + TOLERANCE) then
            return i
        end
    end

    return 0
end

function Init(quest, me)
    local myThing = quest:GetThingWithScriptName(MY_SCRIPT_NAME)
    if myThing then
        quest:SetReadableObjectTextTag(myThing, "TEXT_QST_061_STATUE_NOWHERE")
    else
        quest:Log("!!! ERROR: StatueMasterStatue Init - could not self-lookup '" .. MY_SCRIPT_NAME .. "'")
    end
end

function Main(quest, me)
    local myThing = quest:GetThingWithScriptName(MY_SCRIPT_NAME)
    if not myThing then
        quest:Log("!!! ERROR: StatueMasterStatue Main - could not self-lookup '" .. MY_SCRIPT_NAME .. "'. Aborting.")
        return
    end

    quest:EntitySetCutsceneBehaviour(myThing, 2) -- CUTSCENE_BEHAVIOUR_NOT_PAUSED

    local lastPointingPosition = 0

    repeat
        local currentFraction = GetCurrentTimeFraction(quest)
        local facingAngle = math.fmod(currentFraction + 0.5, 1.0)
        quest:EntitySetFacingAngle(myThing, facingAngle, true)

        local pointingPosition = GetStatuePointingPosition(quest)
        if pointingPosition ~= lastPointingPosition then
            lastPointingPosition = pointingPosition

            if pointingPosition == 1 then
                quest:SetReadableObjectTextTag(myThing, "TEXT_QST_061_STATUE_BOWERSTONE")
            elseif pointingPosition == 3 then
                quest:SetReadableObjectTextTag(myThing, "TEXT_QST_061_STATUE_GREATWOOD")
            else
                quest:SetReadableObjectTextTag(myThing, "TEXT_QST_061_STATUE_NOWHERE")
            end
        end
    until not quest:NewScriptFrame(me)
end