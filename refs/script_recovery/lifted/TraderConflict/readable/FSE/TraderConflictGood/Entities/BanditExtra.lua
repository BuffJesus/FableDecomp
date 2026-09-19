-- Readable native conversion: BanditExtra. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local helpers = require("TraderConflictGood.native_quest_helpers")

-- BanditExtra.Main (retail 0x00dfca90)
function Main(quest, me)
    local scratchValue, scratchValue4
    local hero = quest:GetHero()
    quest:Pause(0.5)
    if 9 < quest:GetStateListCount("AllCreatures") then
        return
    end
    if not quest:IsDistanceBetweenThingsOver(me, hero, 14.0) then
        return
    end
    if quest:IsActiveThreadTerminating() then return end
    local banditGruntLevel = quest:CreateCreature("CREATURE_BANDIT_GRUNT_LEVEL2", me:GetPos(), "")
    local getDistanceBetweenThings = quest:GetDistanceBetweenThings(banditGruntLevel, hero) ^ 2
    -- TODO(native): xStack_8 = (CCharString)(int)ROUND(fVar5 * _DAT_0126b7dc + 0.5);
    if getDistanceBetweenThings * 0.06666667014360428 == scratchValue4 - 1.0 then
        -- TODO(native): xStack_8 = (CCharString)((int)xStack_8 - 1);
    end
    if 3 < scratchValue4 then
        if quest:IsActiveThreadTerminating() then goto LAB_00dfcbf7 end
        scratchValue = 3
    end
    if not (math.random(0, 32767) % scratchValue == 0) then helpers.UpdateLiveEnemies(quest, me); goto LAB_00dfcbf7 end
    if quest:IsActiveThreadTerminating() then goto LAB_00dfcbf7 end
    quest:GiveThingBestEnemyTarget(banditGruntLevel, hero)
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

