-- Readable native conversion: TC_GuardSpawnPoint. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TCE_GuardRangeHighest = 3976,  -- 14
    TCE_GuardRangeHigh = 3980,  -- 11
    TCE_GuardRangeLow = 3984,  -- 5
}

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- TC_GuardSpawnPoint.Main (retail 0x00df7bf0)
function Main(quest, me)
    local numberSpawned, scratchValue, scratchValue3, pOther, scratchValue5, scratchValue6
    while not quest:GetStateBool("QuestStartScreened") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
    while 25 - quest:GetStateInt("InitialNumberInRegion") ~= quest:GetStateInt("NumberSpawned") do
        if (((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25 < quest:GetStateInt("NextTimeToSpawnGuards")) or quest:GetStateListCount("AllCreatures") < 7 then
            if quest:IsActiveThreadTerminating() then return end
            if not quest:IsCameraPosOnScreen(me:GetPos()) then
                scratchValue3 = 0
                repeat
                    numberSpawned = quest:GetStateInt("NumberSpawned")
                    scratchValue = 25 - quest:GetStateInt("InitialNumberInRegion")
                    if scratchValue == numberSpawned or scratchValue - numberSpawned < 0 then break end
                    if quest:IsActiveThreadTerminating() then return end
                    if quest:ReadGlobalGameData(SCRIPT_DEF.TCE_GuardRangeHighest) < (25 - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned") then
                        pOther = "CREATURE_BS_GUARD_BLUE"
                    elseif quest:ReadGlobalGameData(SCRIPT_DEF.TCE_GuardRangeHigh) < (25 - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned") then
                        pOther = "CREATURE_BS_GUARD_BLUE_CROSSBOW"
                    elseif quest:ReadGlobalGameData(SCRIPT_DEF.TCE_GuardRangeLow) < (25 - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned") then
                        pOther = "CREATURE_BS_GUARD_RED"
                    else
                        pOther = "CREATURE_BS_GUARD_BLACK"
                    end
                    scratchValue6 = pOther
                    if math.random(0, 32767) % 500 == 0 then
                        if quest:IsActiveThreadTerminating() then return end
                        scratchValue6 = "CREATURE_BS_SHERIFF"
                    end
                    scratchValue5 = quest:CreateCreature(scratchValue6, me:GetPos(), "IsAGuard")
                    quest:SetCombatNearbyBreakOffRange(scratchValue5, pThing)
                    quest:EntitySetInFaction(scratchValue5, "FACTION_MONSTERS")
                    quest:MiniMapAddMarker(scratchValue5, "HUD_ORB_RED_SMALL")
                    quest:GiveThingBestEnemyTarget(scratchValue5, quest:GetHero())
                    quest:SetStateInt("NumberSpawned", quest:GetStateInt("NumberSpawned") + 1)
                    scratchValue3 = scratchValue3 + 1
                until scratchValue3 >= 2
                if quest:IsActiveThreadTerminating() then return end
            end
            quest:SetStateInt("NextTimeToSpawnGuards", ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 24)
        end
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
end

-- TC_GuardSpawnPoint.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- TC_GuardSpawnPoint.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TC_GuardSpawnPoint.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

