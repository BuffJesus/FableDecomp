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
    local scratchValue
    quest:CreateThread("Tutorial_CombatMultiplier")  -- native thread body NScript::CGameflowAssistanceScript::Tutorial_LowHealth: lift it as function Tutorial_CombatMultiplier(quest)
    if not scratchValue then
    end
    quest:CreateThread("Tutorial_LowHealth")  -- native thread body NScript::CGameflowAssistanceScript::Tutorial_LowHealth: lift it as function Tutorial_LowHealth(quest)
    if not scratchValue then
    end
    quest:CreateThread("Tutorial_LowMagic")  -- native thread body NScript::CGameflowAssistanceScript::Tutorial_LowHealth: lift it as function Tutorial_LowMagic(quest)
    if not scratchValue then
    end
    quest:CreateThread("Tutorial_RenownLevelIncrease")  -- native thread body NScript::CGameflowAssistanceScript::Tutorial_RenownLevelIncrease: lift it as function Tutorial_RenownLevelIncrease(quest)
    if not scratchValue then
    end
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
    local flourishesOffered = quest:GetStateInt("FlourishesOffered") or 0
    flourishesOffered = quest:PersistTransferInt(context, "FlourishesOffered", flourishesOffered)
    quest:SetStateInt("FlourishesOffered", flourishesOffered)
    local flourishesPerformed = quest:GetStateInt("FlourishesPerformed") or 0
    flourishesPerformed = quest:PersistTransferInt(context, "FlourishesPerformed", flourishesPerformed)
    quest:SetStateInt("FlourishesPerformed", flourishesPerformed)
    local combatMultHighestLevel = quest:GetStateInt("CombatMultHighestLevel") or 0
    combatMultHighestLevel = quest:PersistTransferInt(context, "CombatMultHighestLevel", combatMultHighestLevel)
    quest:SetStateInt("CombatMultHighestLevel", combatMultHighestLevel)
    local combatMultFramesSinceChange = quest:GetStateInt("CombatMultFramesSinceChange") or 0
    combatMultFramesSinceChange = quest:PersistTransferInt(context, "CombatMultFramesSinceChange", combatMultFramesSinceChange)
    quest:SetStateInt("CombatMultFramesSinceChange", combatMultFramesSinceChange)
    local healthWarningsGiven = quest:GetStateInt("HealthWarningsGiven") or 0
    healthWarningsGiven = quest:PersistTransferInt(context, "HealthWarningsGiven", healthWarningsGiven)
    quest:SetStateInt("HealthWarningsGiven", healthWarningsGiven)
    local willEnergyWarningsGiven = quest:GetStateInt("WillEnergyWarningsGiven") or 0
    willEnergyWarningsGiven = quest:PersistTransferInt(context, "WillEnergyWarningsGiven", willEnergyWarningsGiven)
    quest:SetStateInt("WillEnergyWarningsGiven", willEnergyWarningsGiven)
    local saveWarningsGiven = quest:GetStateInt("SaveWarningsGiven") or 0
    saveWarningsGiven = quest:PersistTransferInt(context, "SaveWarningsGiven", saveWarningsGiven)
    quest:SetStateInt("SaveWarningsGiven", saveWarningsGiven)
    local savedRenownLevel = quest:GetStateInt("SavedRenownLevel") or 0
    savedRenownLevel = quest:PersistTransferInt(context, "SavedRenownLevel", savedRenownLevel)
    quest:SetStateInt("SavedRenownLevel", savedRenownLevel)
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
            if quest:IsActiveThreadTerminating() then return end
            local getPlayerCreatureCombatMultiplier = quest:GetPlayerCreatureCombatMultiplier()
            if quest:GetStateInt("CombatMultHighestLevel") < getPlayerCreatureCombatMultiplier then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateInt("CombatMultHighestLevel", getPlayerCreatureCombatMultiplier)
                quest:SetStateInt("CombatMultFramesSinceChange", 0)
                if quest:ReadGlobalGameData(SCRIPT_DEF.CombatMultStopBotheringAfter) < getPlayerCreatureCombatMultiplier then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_MASTER", "", true, false)
                    return
                end
                if quest:IsActiveThreadTerminating() then return end
                local combatMultHighestLevel = quest:GetStateInt("CombatMultHighestLevel")
                if combatMultHighestLevel == 15 then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_EXCELLENT", "", true, false)
                elseif combatMultHighestLevel == 10 then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_ADEPT", "", true, false)
                elseif combatMultHighestLevel == 5 then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_PROGRESS", "", true, false)
                end
            else
                if quest:IsActiveThreadTerminating() then return end
                local combatMultFramesSinceChange = quest:GetStateInt("CombatMultFramesSinceChange")
                quest:SetStateInt("CombatMultFramesSinceChange", combatMultFramesSinceChange + 1)
                if quest:ReadGlobalGameData(SCRIPT_DEF.CombatMultTooLongUnchanged) * quest:GetStateInt("CombatMultHighestLevel") < combatMultFramesSinceChange + 1 then
                    if quest:IsActiveThreadTerminating() then return end
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
        if quest:IsActiveThreadTerminating() then return end
        fret_0 = quest:GetHeroHealth()
        if quest:ReadGlobalGameData(SCRIPT_DEF.VeryLowHealthWarning) <= fret_0 then
            if quest:GetHeroHealth() < quest:ReadGlobalGameData(SCRIPT_DEF.LowHealthWarning) then
                if quest:IsActiveThreadTerminating() then return end
                if readGlobalGameData2 < 1 then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_HEALTH_LOW", "", true, false)
                    readGlobalGameData2 = quest:ReadGlobalGameData(SCRIPT_DEF.LowHealthMessageInterval)
                    goto LAB_00cf0155
                end
            end
        else
            if quest:IsActiveThreadTerminating() then return end
            if readGlobalGameData < 1 then
                if quest:IsActiveThreadTerminating() then return end
                quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_HEALTH_CRIT", "", true, false)
                readGlobalGameData = quest:ReadGlobalGameData(SCRIPT_DEF.VeryLowHealthMessageInterval)
                goto LAB_00cf0155
            end
        end
        goto FLOW_past_lab_00cf0155
        ::LAB_00cf0155::
        quest:SetStateInt("HealthWarningsGiven", (quest:GetStateInt("HealthWarningsGiven") + 1 + 2147483648) % 4294967296 - 2147483648)
        ::FLOW_past_lab_00cf0155::
        readGlobalGameData = readGlobalGameData - 1
        readGlobalGameData2 = readGlobalGameData2 - 1
        ::continue_1::
    end
    quest:IsActiveThreadTerminating()
end

-- GameflowAssistance.Tutorial_LowMagic (retail 0x00cf0180)
function Tutorial_LowMagic(quest)
    local readGlobalGameData = 0
    while quest:GetStateInt("WillEnergyWarningsGiven") < quest:ReadGlobalGameData(SCRIPT_DEF.WillEnergyMessageLimit) do
        if not quest:NewScriptFrame() then return end
        if quest:GetMasterGameState("KilledGM") then break end
        if quest:IsHeroControlledByPlayer() then
            if quest:IsActiveThreadTerminating() then return end
            local scratchValue = readGlobalGameData < 1 and quest:GetHeroWillEnergyLevel() < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.LowWillEnergyWarningLevel)
            if scratchValue then
                if quest:IsActiveThreadTerminating() then return end
                quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_WILL_LOW", "", true, false)
                readGlobalGameData = quest:ReadGlobalGameData(SCRIPT_DEF.LowWillEnergyMessageInterval)
                quest:SetStateInt("WillEnergyWarningsGiven", quest:GetStateInt("WillEnergyWarningsGiven") + 1)
            end
            readGlobalGameData = readGlobalGameData - 1
        end
    end
    quest:IsActiveThreadTerminating()
end

-- GameflowAssistance.Tutorial_RenownLevelIncrease (retail 0x00cf02a0)
function Tutorial_RenownLevelIncrease(quest)
    local renownMessages = {
        [1] = "TEXT_QST_080_RENOWN_LEVEL_ONE",
        [2] = "TEXT_QST_080_RENOWN_LEVEL_TWO",
        [3] = "TEXT_QST_080_RENOWN_LEVEL_THREE",
        [4] = "TEXT_QST_080_RENOWN_LEVEL_FOUR",
        [5] = "TEXT_QST_080_RENOWN_LEVEL_FIVE",
        [6] = "TEXT_QST_080_RENOWN_LEVEL_SIX",
        [7] = "TEXT_QST_080_RENOWN_LEVEL_SEVEN",
    }
    local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    while true do
        if isActiveThreadTerminating then
            return
        end
        if quest:GetMasterGameState("KilledGM") then break end
        local getHeroRenownLevel = quest:GetHeroRenownLevel()
        if not quest:IsHeroControlledByPlayer() then
            quest:NewScriptFrame();
            isActiveThreadTerminating = quest:IsActiveThreadTerminating();
        else
            if quest:IsActiveThreadTerminating() then return end
            if quest:GetStateInt("SavedRenownLevel") < getHeroRenownLevel then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetStateInt("SavedRenownLevel", getHeroRenownLevel)
                if getHeroRenownLevel < 8 then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:HeroReceiveMessageFromGuildMaster("", renownMessages[getHeroRenownLevel] or "TEXT_QST_080_RENOWN_LEVEL_SEVEN", true, true)
                end
                if 1 < getHeroRenownLevel then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
                    quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
                    quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
                end
            end
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        end
        ::continue_1::
    end
    if quest:IsActiveThreadTerminating() then return end
end

