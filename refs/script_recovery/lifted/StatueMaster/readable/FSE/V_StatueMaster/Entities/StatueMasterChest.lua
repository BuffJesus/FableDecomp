-- Readable native conversion: StatueMasterChest. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local helpers = require("V_StatueMaster.native_quest_helpers")

-- StatueMasterChest.Main (retail 0x00ed4830)
function Main(quest, me)
    if helpers.GetStatuePointingPosition(quest, me) == 3 then
        return
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:RemoveThing(me, false, true)
end

-- StatueMasterChest.Init (retail 0x00ed4820)
function Init(quest, me)
end

-- StatueMasterChest.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- StatueMasterChest.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

