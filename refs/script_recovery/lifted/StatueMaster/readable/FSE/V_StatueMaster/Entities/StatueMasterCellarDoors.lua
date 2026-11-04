-- Readable native conversion: StatueMasterCellarDoors. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local helpers = require("V_StatueMaster.native_quest_helpers")

-- StatueMasterCellarDoors.Main (retail 0x00ed45a0)
function Main(quest, me)
    local predicateResult, predicateResult2, predicateResult3, traderToEscort
    predicateResult = false
    predicateResult3 = false
    if helpers.GetStatuePointingPosition(quest, me) == 1 then
        predicateResult = true
        predicateResult3 = true
        traderToEscort = quest:GetThingWithScriptName("TraderToEscort")
        predicateResult2 = true
        if not (traderToEscort ~= nil and traderToEscort:IsAlive()) then goto LAB_00ed45ff end
    end
    predicateResult2 = false
    ::LAB_00ed45ff::
    if predicateResult3 then
        traderToEscort = nil
    end
    if predicateResult then
    end
    if predicateResult2 then
        if quest:IsActiveThreadTerminating() then return end
        quest:EntitySetAsLocked(me, false)
        return
    else
        if quest:IsActiveThreadTerminating() then return end
        quest:EntitySetAsLocked(me, true)
        while not quest:IsActiveThreadTerminating() do
            if not me:MsgIsUsedByHero() then
                quest:NewScriptFrame(me)
            else
                if quest:IsActiveThreadTerminating() then return end
                quest:DisplayGameInfo("TEXT_QST_061_CELLAR_LOCKED")
                while not quest:MsgIsGameInfoClickedPast() do
                    if not quest:NewScriptFrame(me) then return end
                end
                if quest:IsActiveThreadTerminating() then return end
                quest:NewScriptFrame(me)
            end
        end
    end
end

-- StatueMasterCellarDoors.Init (retail 0x00ed4570)
function Init(quest, me)
end

-- StatueMasterCellarDoors.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- StatueMasterCellarDoors.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

