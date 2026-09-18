-- Readable native conversion: BanditExtra. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local helpers = require("TraderConflictGood.native_quest_helpers")

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- BanditExtra.Main (retail 0x00dfca90)
function Main(quest, me)
    local scratchValue, scratchValue2, banditGruntLevel
    local hero = quest:GetHero()
    quest:Pause(0.5)
    if 9 < quest:GetStateListCount("AllCreatures") then
        return
    end
    if not quest:IsDistanceBetweenThingsOver(me, hero, 14.0) then
        return
    end
    if quest:IsActiveThreadTerminating() then return end
    banditGruntLevel = quest:CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL2", me:GetPos(), "")
    scratchValue2 = quest:GetDistanceBetweenThings(banditGruntLevel, hero) ^ 2
    -- TODO(native): xStack_8 = (CCharString)(int)ROUND(fVar5 * _DAT_0126b7dc + 0.5);
    if scratchValue2 * 0.06666667014360428 == xStack_8 - 1.0 then
        -- TODO(native): xStack_8 = (CCharString)((int)xStack_8 - 1);
    end
    if 3 < xStack_8 then
        if quest:IsActiveThreadTerminating() then goto LAB_00dfcbf7 end
        scratchValue = 3
    end
    if iVar3 % scratchValue == 0 then
        if quest:IsActiveThreadTerminating() then goto LAB_00dfcbf7 end
        quest:GiveThingBestEnemyTarget(banditGruntLevel, hero)
    end
    helpers.UpdateLiveEnemies(quest, me)
    ::LAB_00dfcbf7::
end

-- BanditExtra.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- BanditExtra.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- BanditExtra.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

