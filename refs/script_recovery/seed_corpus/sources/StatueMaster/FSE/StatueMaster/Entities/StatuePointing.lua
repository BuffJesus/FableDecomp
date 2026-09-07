local M = {}

local TOLERANCE = 0.04

function M.GetCurrentTimeFraction(quest)
    local timeOfDay = quest:GetTimeOfDay()
    local hour = math.floor(timeOfDay / 100)
    local minute = timeOfDay % 100
    return (minute / 60.0 + hour) / 24.0
end


function M.GetStatuePointingPosition(quest)
    local currentFraction = M.GetCurrentTimeFraction(quest)

    for i = 1, 3 do
        local angleStr = quest:GetStateString("SM_Angle" .. i)
        local angle = tonumber(angleStr)
        if angle and currentFraction > (angle - TOLERANCE) and currentFraction < (angle + TOLERANCE) then
            return i
        end
    end

    return 0
end

return M