local TOLERANCE = 0.04
local MY_SCRIPT_NAME = "StatueMasterCellarDoors"

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
    local myThing = quest:GetThingWithScriptName(MY_SCRIPT_NAME)
    if not myThing then
        quest:Log("!!! ERROR: StatueMasterCellarDoors Main - could not self-lookup '" .. MY_SCRIPT_NAME .. "'. Aborting.")
        return
    end

    local unlockCellar = false

    if GetStatuePointingPosition(quest) == 1 then
        --local trader = quest:GetThingWithScriptName("TraderToEscort")
        --if trader and not trader:IsNull() then
            unlockCellar = true
        --end
    end

    if unlockCellar then
        quest:EntitySetAsLocked(myThing, false)
        return
    end

    quest:EntitySetAsLocked(myThing, true)

    while true do
        if me:MsgIsUsedByHero() then
            quest:DisplayGameInfo("TEXT_QST_061_CELLAR_LOCKED")

            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame(me) then
                    return
                end
            end
        end

        if not quest:NewScriptFrame(me) then
            return
        end
    end
end