-- Readable native conversion: GameflowAssistance. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    CombatMultTooLongUnchanged = 4056,  -- 1000
    CombatMultStopBotheringAfter = 4060,  -- 30
    LowHealthWarning = 4064,  -- 10
    VeryLowHealthWarning = 4068,  -- 5
    LowHealthMessageInterval = 4072,  -- 300
    VeryLowHealthMessageInterval = 4076,  -- 200
    HealthMessageLimit = 4080,  -- 30
    LowWillEnergyWarningLevel = 4084,  -- 0.20000000298023224
    LowWillEnergyMessageInterval = 4088,  -- 300
    WillEnergyMessageLimit = 4092,  -- 30
}

-- GameflowAssistance.Main (retail 0x00cefac0)
function Main(quest)
    quest:CreateThread("Tutorial_CombatMultiplier")  -- native thread body NScript::CGameflowAssistanceScript::Tutorial_LowHealth: lift it as function Tutorial_CombatMultiplier(quest)
    quest:CreateThread("Tutorial_LowHealth")  -- native thread body NScript::CGameflowAssistanceScript::Tutorial_LowHealth: lift it as function Tutorial_LowHealth(quest)
    quest:CreateThread("Tutorial_LowMagic")  -- native thread body NScript::CGameflowAssistanceScript::Tutorial_LowHealth: lift it as function Tutorial_LowMagic(quest)
    quest:CreateThread("Tutorial_RenownLevelIncrease")  -- native thread body NScript::CGameflowAssistanceScript::Tutorial_RenownLevelIncrease: lift it as function Tutorial_RenownLevelIncrease(quest)
end

-- GameflowAssistance.Init (retail 0x00cefa00)
function Init(quest)
    quest:SetStateInt("FlourishesOffered", 0)
    quest:SetStateInt("FlourishesPerformed", 0)
    quest:SetStateInt("CombatMultHighestLevel", 1)
    quest:SetStateInt("CombatMultFramesSinceChange", 0)
    quest:SetStateInt("HealthWarningsGiven", 0)
    quest:SetStateInt("WillEnergyWarningsGiven", 0)
    quest:SetStateInt("SaveWarningsGiven", 0)
    quest:SetStateInt("SavedRenownLevel", 0)
end

-- GameflowAssistance.OnPersist (retail 0x00cf0560)
function OnPersist(quest, context)
    quest:SetStateInt("FlourishesOffered", quest:PersistTransferInt(context, "FlourishesOffered", quest:GetStateInt("FlourishesOffered") or 0))
    quest:SetStateInt("FlourishesPerformed", quest:PersistTransferInt(context, "FlourishesPerformed", quest:GetStateInt("FlourishesPerformed") or 0))
    quest:SetStateInt("CombatMultHighestLevel", quest:PersistTransferInt(context, "CombatMultHighestLevel", quest:GetStateInt("CombatMultHighestLevel") or 0))
    quest:SetStateInt("CombatMultFramesSinceChange", quest:PersistTransferInt(context, "CombatMultFramesSinceChange", quest:GetStateInt("CombatMultFramesSinceChange") or 0))
    quest:SetStateInt("HealthWarningsGiven", quest:PersistTransferInt(context, "HealthWarningsGiven", quest:GetStateInt("HealthWarningsGiven") or 0))
    quest:SetStateInt("WillEnergyWarningsGiven", quest:PersistTransferInt(context, "WillEnergyWarningsGiven", quest:GetStateInt("WillEnergyWarningsGiven") or 0))
    quest:SetStateInt("SaveWarningsGiven", quest:PersistTransferInt(context, "SaveWarningsGiven", quest:GetStateInt("SaveWarningsGiven") or 0))
    quest:SetStateInt("SavedRenownLevel", quest:PersistTransferInt(context, "SavedRenownLevel", quest:GetStateInt("SavedRenownLevel") or 0))
end

-- GameflowAssistance.Tutorial_CombatMultiplier (retail 0x00cefcc0)
function Tutorial_CombatMultiplier(quest)
    if quest:IsActiveThreadTerminating() then return end
    repeat
        if quest:GetMasterGameState("KilledGM") then
            return
        end
        local isPlayerWieldingWeapon = quest:IsPlayerWieldingWeapon() and quest:IsHeroControlledByPlayer()
        if isPlayerWieldingWeapon then
            local getPlayerCreatureCombatMultiplier = quest:GetPlayerCreatureCombatMultiplier()
            if quest:GetStateInt("CombatMultHighestLevel") < getPlayerCreatureCombatMultiplier then
                quest:SetStateInt("CombatMultHighestLevel", getPlayerCreatureCombatMultiplier)
                quest:SetStateInt("CombatMultFramesSinceChange", 0)
                if quest:ReadGlobalGameData(SCRIPT_DEF.CombatMultStopBotheringAfter) < getPlayerCreatureCombatMultiplier then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_MASTER", "", true, false)
                    return
                end
                local combatMultHighestLevel = quest:GetStateInt("CombatMultHighestLevel")
                if combatMultHighestLevel == 15 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_EXCELLENT", "", true, false)
                elseif combatMultHighestLevel == 10 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_ADEPT", "", true, false)
                elseif combatMultHighestLevel == 5 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_PROGRESS", "", true, false)
                end
            else
                local combatMultFramesSinceChange = quest:GetStateInt("CombatMultFramesSinceChange")
                quest:SetStateInt("CombatMultFramesSinceChange", combatMultFramesSinceChange + 1)
                if quest:ReadGlobalGameData(SCRIPT_DEF.CombatMultTooLongUnchanged) * quest:GetStateInt("CombatMultHighestLevel") < combatMultFramesSinceChange + 1 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_TRY", "", true, false)
                    quest:SetStateInt("CombatMultFramesSinceChange", 0)
                end
            end
        end
        if not quest:NewScriptFrame() then return end
    until false
end

-- GameflowAssistance.Tutorial_LowHealth (retail 0x00ceffb0)
function Tutorial_LowHealth(quest)
    local fret_0, readGlobalGameData, readGlobalGameData2
    readGlobalGameData2 = 0
    readGlobalGameData = 0
    while quest:GetStateInt("HealthWarningsGiven") < quest:ReadGlobalGameData(SCRIPT_DEF.HealthMessageLimit) do
        if not quest:NewScriptFrame() then return end
        if quest:GetMasterGameState("KilledGM") then break end
        if not quest:IsHeroControlledByPlayer() then goto continue_1 end
        fret_0 = quest:GetHeroHealth()
        if quest:ReadGlobalGameData(SCRIPT_DEF.VeryLowHealthWarning) <= fret_0 then
            if quest:GetHeroHealth() < quest:ReadGlobalGameData(SCRIPT_DEF.LowHealthWarning) then
                if readGlobalGameData2 < 1 then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_HEALTH_LOW", "", true, false)
                    readGlobalGameData2 = quest:ReadGlobalGameData(SCRIPT_DEF.LowHealthMessageInterval)
                    goto LAB_00cf0155
                end
            end
        elseif readGlobalGameData < 1 then
            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_HEALTH_CRIT", "", true, false)
            readGlobalGameData = quest:ReadGlobalGameData(SCRIPT_DEF.VeryLowHealthMessageInterval)
            goto LAB_00cf0155
        end
        goto FLOW_past_lab_00cf0155
        ::LAB_00cf0155::
        quest:SetStateInt("HealthWarningsGiven", quest:GetStateInt("HealthWarningsGiven") + 1)
        ::FLOW_past_lab_00cf0155::
        readGlobalGameData = readGlobalGameData - 1
        readGlobalGameData2 = readGlobalGameData2 - 1
        ::continue_1::
    end
end

-- GameflowAssistance.Tutorial_LowMagic (retail 0x00cf0180)
function Tutorial_LowMagic(quest)
    local readGlobalGameData = 0
    while quest:GetStateInt("WillEnergyWarningsGiven") < quest:ReadGlobalGameData(SCRIPT_DEF.WillEnergyMessageLimit) do
        if not quest:NewScriptFrame() then return end
        if quest:GetMasterGameState("KilledGM") then break end
        if quest:IsHeroControlledByPlayer() then
            local scratchValue = readGlobalGameData < 1 and quest:GetHeroWillEnergyLevel() < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.LowWillEnergyWarningLevel)
            if scratchValue then
                quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_WILL_LOW", "", true, false)
                readGlobalGameData = quest:ReadGlobalGameData(SCRIPT_DEF.LowWillEnergyMessageInterval)
                quest:SetStateInt("WillEnergyWarningsGiven", quest:GetStateInt("WillEnergyWarningsGiven") + 1)
            end
            readGlobalGameData = readGlobalGameData - 1
        end
    end
end

-- GameflowAssistance.Tutorial_RenownLevelIncrease (retail 0x00cf02a0)
function Tutorial_RenownLevelIncrease(quest)
    local scratchValue, scratchValue2, scratchValue3
    local isHeroControlledByPlayer = quest:IsActiveThreadTerminating()
    while true do
        if isHeroControlledByPlayer then
            scratchValue = 8
            repeat
                scratchValue = scratchValue - 1
            until scratchValue == 0
            return
        end
        if quest:GetMasterGameState("KilledGM") then break end
        local getHeroRenownLevel = quest:GetHeroRenownLevel()
        if not quest:IsHeroControlledByPlayer() then quest:NewScriptFrame(); isHeroControlledByPlayer = quest:IsActiveThreadTerminating(); goto continue_1 end
        if quest:IsActiveThreadTerminating() then
            scratchValue2 = 8
            repeat
                scratchValue2 = scratchValue2 - 1
            until scratchValue2 == 0
            return
        end
        if quest:GetStateInt("SavedRenownLevel") < getHeroRenownLevel then
            if quest:IsActiveThreadTerminating() then
                -- TODO(native): _vector_constructor_iterator_((int)xStack_20,4,8,0x99eae0);
                return
            end
            quest:SetStateInt("SavedRenownLevel", getHeroRenownLevel)
            if getHeroRenownLevel < 8 then
                if quest:IsActiveThreadTerminating() then
                    -- TODO(native): _vector_constructor_iterator_((int)xStack_20,4,8,0x99eae0);
                    return
                end
                quest:HeroReceiveMessageFromGuildMaster("", "TEXT_QST_080_RENOWN_LEVEL_SEVEN", true, true)
            end
            if 1 < getHeroRenownLevel then
                if quest:IsActiveThreadTerminating() then
                    -- TODO(native): _vector_constructor_iterator_((int)xStack_20,4,8,0x99eae0);
                    return
                end
                quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
                quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
                quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
            end
        end
        quest:NewScriptFrame()
        isHeroControlledByPlayer = quest:IsActiveThreadTerminating()
        ::continue_1::
    end
    scratchValue3 = 8
    if quest:IsActiveThreadTerminating() then
        repeat
            scratchValue3 = scratchValue3 - 1
        until scratchValue3 == 0
        return
    end
    repeat
        scratchValue3 = scratchValue3 - 1
    until scratchValue3 == 0
end

