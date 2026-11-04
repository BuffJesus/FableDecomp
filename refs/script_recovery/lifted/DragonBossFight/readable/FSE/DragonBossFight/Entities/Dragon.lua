-- Readable native conversion: Dragon. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    DBDragonMediumHealth = 2996,  -- 1400.0
    DBDragonLowHealth = 3000,  -- 900.0
    DBDragonVeryLowHealth = 3004,  -- 500.0
    DBNumMinionsMediumHealth = 3012,  -- 2
    DBNumMinionsLowHealth = 3016,  -- 2
    DBNumMinionsVeryLowHealth = 3020,  -- 3
    DBNumSummonersMediumHealth = 3028,  -- 0
    DBNumSummonersLowHealth = 3032,  -- 1
    DBNumSummonersVeryLowHealth = 3036,  -- 1
}

local helpers = require("DragonBossFight.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local mediumHealth_, lowHealth_, veryLowHealth_

-- Dragon.Main (retail 0x00d25a00)
function Main(quest, me)
    local getStateInt, getStateInt2, getStateInt3, predicateResult, readGlobalGameData
    local mediumHealth = mediumHealth_
    local lowHealth = lowHealth_
    local veryLowHealth = veryLowHealth_
    if not quest:NewScriptFrame(me) then return end
    predicateResult = false
    while quest:GetStateInt("DragonState") ~= 4 do
        if not quest:NewScriptFrame(me) then return end
        if predicateResult then
            if not quest:IsCreatureFlying(me) then
                if quest:IsActiveThreadTerminating() then return end
                predicateResult = false
            end
        elseif quest:IsCreatureFlying(me) then
            if quest:IsActiveThreadTerminating() then return end
            helpers.SpawnEnemies(quest, me)
            predicateResult = true
        end
        getStateInt = quest:GetStateInt("DragonState") == 0
        if getStateInt then
            local fret_0 = quest:GetHealth(me)
            getStateInt = fret_0 < mediumHealth ~= (fret_0 == mediumHealth)
        end
        if getStateInt then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("DragonState", 1)
            if not quest:IsActiveThreadTerminating() then
                quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumMinionsMediumHealth))
                readGlobalGameData = quest:ReadGlobalGameData(SCRIPT_DEF.DBNumSummonersMediumHealth)
                goto LAB_00d25bfd
            end
        else
            getStateInt2 = quest:GetStateInt("DragonState") == 1
            if getStateInt2 then
                local fret_00 = quest:GetHealth(me)
                getStateInt2 = fret_00 < lowHealth ~= (fret_00 == lowHealth)
            end
            if getStateInt2 then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateInt("DragonState", 2)
                if not quest:IsActiveThreadTerminating() then
                    quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumMinionsLowHealth))
                    readGlobalGameData = quest:ReadGlobalGameData(SCRIPT_DEF.DBNumSummonersLowHealth)
                    goto LAB_00d25bfd
                end
            else
                getStateInt3 = quest:GetStateInt("DragonState") == 2
                if getStateInt3 then
                    local fret_01 = quest:GetHealth(me)
                    getStateInt3 = fret_01 < veryLowHealth ~= (fret_01 == veryLowHealth)
                end
                if getStateInt3 then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:SetStateInt("DragonState", 3)
                    if not quest:IsActiveThreadTerminating() then
                        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumMinionsVeryLowHealth))
                        readGlobalGameData = quest:ReadGlobalGameData(SCRIPT_DEF.DBNumSummonersVeryLowHealth)
                        goto LAB_00d25bfd
                    end
                end
            end
        end
        goto FLOW_past_lab_00d25bfd
        ::LAB_00d25bfd::
        quest:SetStateInt("TargetNumSummoners", readGlobalGameData)
        ::FLOW_past_lab_00d25bfd::
    end
    quest:IsActiveThreadTerminating()
end

-- Dragon.Init (retail 0x00d25880)
function Init(quest, me)
    helpers.helper_D258C0(quest, me, 0)
    mediumHealth_ = quest:ReadGlobalGameData(SCRIPT_DEF.DBDragonMediumHealth)
    lowHealth_ = quest:ReadGlobalGameData(SCRIPT_DEF.DBDragonLowHealth)
    veryLowHealth_ = quest:ReadGlobalGameData(SCRIPT_DEF.DBDragonVeryLowHealth)
end

-- Dragon.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- Dragon.OnPredicateFail (retail 0x00d259a0)
function OnPredicateFail(quest, me)
    if me:MsgIsKilledBy("") then
        quest:SetStateInt("DragonState", 4)
    end
end

