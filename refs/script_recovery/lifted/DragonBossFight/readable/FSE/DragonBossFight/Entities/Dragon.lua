-- Readable native conversion: Dragon. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local helpers = require("DragonBossFight.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local mediumHealth, lowHealth, veryLowHealth, self0X48, self0X68, self0X6C

-- Dragon.Main (retail 0x00d25a00)
function Main(quest, me)
    local getStateInt, getStateInt2, getStateInt3, predicateResult, this_00
    if not quest:NewScriptFrame(me) then return end
    predicateResult = false
    while quest:GetStateInt("DragonState") ~= 4 do
        if not quest:NewScriptFrame(me) then return end
        if predicateResult then
            if not quest:IsCreatureFlying(me) then
                predicateResult = false
            end
        elseif quest:IsCreatureFlying(me) then
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
                quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(3012))
                quest:ReadGlobalGameData(3028)
                goto LAB_00d25bfd
            end
            -- TODO(native): } else {
            getStateInt2 = quest:GetStateInt("DragonState") == 1
            if getStateInt2 then
                local fret_00 = quest:GetHealth(me)
                getStateInt2 = fret_00 < lowHealth ~= (fret_00 == lowHealth)
            end
            if getStateInt2 then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateInt("DragonState", 2)
                if not quest:IsActiveThreadTerminating() then
                    quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(3016))
                    quest:ReadGlobalGameData(3032)
                    goto LAB_00d25bfd
                end
                -- TODO(native): } else {
                getStateInt3 = quest:GetStateInt("DragonState") == 2
                if getStateInt3 then
                    local fret_01 = quest:GetHealth(me)
                    getStateInt3 = fret_01 < veryLowHealth ~= (fret_01 == veryLowHealth)
                end
                if getStateInt3 then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:SetStateInt("DragonState", 3)
                    if not quest:IsActiveThreadTerminating() then
                        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(3020))
                        quest:ReadGlobalGameData(3036)
                        goto LAB_00d25bfd
                    end
                end
            end
        end
        goto FLOW_past_lab_00d25bfd
        ::LAB_00d25bfd::
        -- TODO(native): *(undefined4 *)(this_00 + 0x6c) = uVar4;
        ::FLOW_past_lab_00d25bfd::
    end
end

-- Dragon.Init (retail 0x00d25880)
function Init(quest, me)
    -- TODO(native): helper_D258C0(quest, me, *(this + 0x14), 0)
    mediumHealth = quest:ReadGlobalGameData(2996)
    lowHealth = quest:ReadGlobalGameData(3000)
    veryLowHealth = quest:ReadGlobalGameData(3004)
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

-- helper 0xD258C0 (named after the state it writes)
function Setself_0x48(quest, me, param1)
    -- TODO(native): name field 0x48 (int)
    self0X48 = param1
    if param1 == 0 then
        if quest:IsActiveThreadTerminating() then return end
        -- TODO(native): name field 0x68 (undefined4)
        self0X68 = quest:ReadGlobalGameData(3008)
        -- TODO(native): name field 0x6c (undefined4)
        self0X6C = quest:ReadGlobalGameData(3024)
        return
    elseif param1 == 1 then
        if quest:IsActiveThreadTerminating() then return end
        -- TODO(native): name field 0x68 (undefined4)
        self0X68 = quest:ReadGlobalGameData(3012)
        -- TODO(native): name field 0x6c (undefined4)
        self0X6C = quest:ReadGlobalGameData(3028)
        return
    elseif param1 == 2 then
        if quest:IsActiveThreadTerminating() then return end
        -- TODO(native): name field 0x68 (undefined4)
        self0X68 = quest:ReadGlobalGameData(3016)
        -- TODO(native): name field 0x6c (undefined4)
        self0X6C = quest:ReadGlobalGameData(3032)
        return
    elseif param1 == 3 then
        if quest:IsActiveThreadTerminating() then return end
        -- TODO(native): name field 0x68 (undefined4)
        self0X68 = quest:ReadGlobalGameData(3020)
        -- TODO(native): name field 0x6c (undefined4)
        self0X6C = quest:ReadGlobalGameData(3036)
    end
end

