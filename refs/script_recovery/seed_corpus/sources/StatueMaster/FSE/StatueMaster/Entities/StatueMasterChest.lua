local TOLERANCE = 0.04
local MY_SCRIPT_NAME = "StatueMasterChest"

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

function Main(quest, me)
    if GetStatuePointingPosition(quest) ~= 3 then
        local myThing = quest:GetThingWithScriptName(MY_SCRIPT_NAME)
        if myThing then
            quest:RemoveThing(myThing)
        else
            quest:Log("!!! ERROR: StatueMasterChest Main - could not self-lookup '" .. MY_SCRIPT_NAME .. "'")
        end
    end
end