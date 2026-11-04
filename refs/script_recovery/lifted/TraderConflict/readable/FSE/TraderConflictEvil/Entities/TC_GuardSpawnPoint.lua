-- Readable native conversion: TC_GuardSpawnPoint. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TCE_GuardRangeHighest = 3976,  -- 14
    TCE_GuardRangeHigh = 3980,  -- 11
    TCE_GuardRangeLow = 3984,  -- 5
}

-- TC_GuardSpawnPoint.Main (retail 0x00df7bf0)
function Main(quest, me)
    local scratchValue2, pOther, scratchValue5
    while not quest:GetStateBool("QuestStartScreened") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then return end
    while 25 - quest:GetStateInt("InitialNumberInRegion") ~= quest:GetStateInt("NumberSpawned") do
        if (((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 25 < quest:GetStateInt("NextTimeToSpawnGuards")) or quest:GetStateListCount("AllCreatures") < 7 then
            if quest:IsActiveThreadTerminating() then return end
            local position = me:GetPos()
            if not quest:IsCameraPosOnScreen(position) then
                if quest:IsActiveThreadTerminating() then return end
                scratchValue2 = 0
                repeat
                    local numberSpawned = quest:GetStateInt("NumberSpawned")
                    local scratchValue = 25 - quest:GetStateInt("InitialNumberInRegion")
                    if scratchValue == numberSpawned or scratchValue - numberSpawned < 0 then break end
                    if quest:IsActiveThreadTerminating() then return end
                    if quest:ReadGlobalGameData(SCRIPT_DEF.TCE_GuardRangeHighest) < (25 - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned") then
                        if quest:IsActiveThreadTerminating() then return end
                        pOther = "CREATURE_BS_GUARD_BLUE"
                    elseif quest:ReadGlobalGameData(SCRIPT_DEF.TCE_GuardRangeHigh) < (25 - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned") then
                        if quest:IsActiveThreadTerminating() then return end
                        pOther = "CREATURE_BS_GUARD_BLUE_CROSSBOW"
                    elseif quest:ReadGlobalGameData(SCRIPT_DEF.TCE_GuardRangeLow) < (25 - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned") then
                        if quest:IsActiveThreadTerminating() then return end
                        pOther = "CREATURE_BS_GUARD_RED"
                    else
                        if quest:IsActiveThreadTerminating() then return end
                        pOther = "CREATURE_BS_GUARD_BLACK"
                    end
                    scratchValue5 = pOther
                    if quest:RetailRandModulo(32768) % 500 == 0 then
                        if quest:IsActiveThreadTerminating() then return end
                        scratchValue5 = "CREATURE_BS_SHERIFF"
                    end
                    local pScriptName = "IsAGuard"
                    local position2 = me:GetPos()
                    local scratchValue4 = quest:CreateCreature(scratchValue5, position2, pScriptName, false)
                    quest:SetCombatNearbyBreakOffRange(scratchValue4, 10.0)
                    quest:EntitySetInFaction(scratchValue4, "FACTION_MONSTERS")
                    quest:MiniMapAddMarker(scratchValue4, "HUD_ORB_RED_SMALL")
                    local hero = quest:GetHero()
                    quest:GiveThingBestEnemyTarget(scratchValue4, hero)
                    quest:SetStateInt("NumberSpawned", quest:GetStateInt("NumberSpawned") + 1)
                    scratchValue2 = scratchValue2 + 1
                until not (scratchValue2 < 2)
                if quest:IsActiveThreadTerminating() then return end
            end
            quest:SetStateInt("NextTimeToSpawnGuards", ((quest:GetStateListCount("AllCreatures") - quest:GetStateInt("InitialNumberInRegion")) - quest:GetStateInt("NumberSpawned")) + 24)
        end
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:IsActiveThreadTerminating()
end

-- TC_GuardSpawnPoint.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- TC_GuardSpawnPoint.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TC_GuardSpawnPoint.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

