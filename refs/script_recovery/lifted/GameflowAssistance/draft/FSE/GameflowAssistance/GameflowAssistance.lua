-- Generated native draft: GameflowAssistance. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar2
    quest:CreateThread("Tutorial_CombatMultiplier")  -- native thread body NScript::CGameflowAssistanceScript::Tutorial_LowHealth: lift it as function Tutorial_CombatMultiplier(quest)
    if not bVar2 then
    end
    quest:CreateThread("Tutorial_LowHealth")  -- native thread body NScript::CGameflowAssistanceScript::Tutorial_LowHealth: lift it as function Tutorial_LowHealth(quest)
    if not bVar2 then
    end
    quest:CreateThread("Tutorial_LowMagic")  -- native thread body NScript::CGameflowAssistanceScript::Tutorial_LowHealth: lift it as function Tutorial_LowMagic(quest)
    if not bVar2 then
    end
    quest:CreateThread("Tutorial_RenownLevelIncrease")  -- native thread body NScript::CGameflowAssistanceScript::Tutorial_RenownLevelIncrease: lift it as function Tutorial_RenownLevelIncrease(quest)
    if not bVar2 then
    end
end

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

function Tutorial_CombatMultiplier(quest)
    local iVar2
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    local bVar1 = not alive
    if bVar1 then
        return
    end
    repeat
        if quest:GetMasterGameState("KilledGM") then
            alive = not quest:IsActiveThreadTerminating()
            return
        end
        bVar1 = quest:IsPlayerWieldingWeapon()
        local __native_condition_1 = bVar1
        if __native_condition_1 then
            bVar1 = quest:IsHeroControlledByPlayer()
            __native_condition_1 = bVar1
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            iVar2 = quest:GetPlayerCreatureCombatMultiplier()
            if quest:GetStateInt("CombatMultHighestLevel") < iVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                quest:SetStateInt("CombatMultHighestLevel", iVar2)
                quest:SetStateInt("CombatMultFramesSinceChange", 0)
                if quest:ReadGlobalGameData(0xfdc) < iVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        return
                    end
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_MASTER", "", true, false)
                    return
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                iVar2 = quest:GetStateInt("CombatMultHighestLevel")
                if iVar2 == 0xf then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        return
                    end
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_EXCELLENT", "", true, false)
                elseif iVar2 == 10 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        return
                    end
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_ADEPT", "", true, false)
                elseif iVar2 == 5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        return
                    end
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_PROGRESS", "", true, false)
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                iVar2 = quest:GetStateInt("CombatMultFramesSinceChange")
                quest:SetStateInt("CombatMultFramesSinceChange", iVar2 + 1)
                if quest:ReadGlobalGameData(0xfd8) * quest:GetStateInt("CombatMultHighestLevel") < iVar2 + 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        return
                    end
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_TRY", "", true, false)
                    quest:SetStateInt("CombatMultFramesSinceChange", 0)
                end
            end
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
    until false
end

function Tutorial_LowHealth(quest)
    local bVar3, fret_0, fret_00, iVar4, iVar5
    local alive = true
    iVar5 = 0
    iVar4 = 0
    if quest:GetStateInt("HealthWarningsGiven") < quest:ReadGlobalGameData(0xff0) then
        repeat
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            if quest:GetMasterGameState("KilledGM") then break end
            bVar3 = quest:IsHeroControlledByPlayer()
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                fret_0 = quest:GetHeroHealth()
                if quest:ReadGlobalGameData(0xfe4) <= fret_0 then
                    fret_00 = quest:GetHeroHealth()
                    if fret_00 < quest:ReadGlobalGameData(0xfe0) then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        if iVar5 < 1 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_HEALTH_LOW", "", true, false)
                            iVar5 = quest:ReadGlobalGameData(0xfe8)
                            goto LAB_00cf0155
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        return
                    end
                    if iVar4 < 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_HEALTH_CRIT", "", true, false)
                        iVar4 = quest:ReadGlobalGameData(0xfec)
                        goto LAB_00cf0155
                    end
                end
                goto FLOW_past_lab_00cf0155
                ::LAB_00cf0155::
                quest:SetStateInt("HealthWarningsGiven", quest:GetStateInt("HealthWarningsGiven") + 1)
                ::FLOW_past_lab_00cf0155::
                iVar4 = iVar4 + -1
                iVar5 = iVar5 + -1
            end
        until not (quest:GetStateInt("HealthWarningsGiven") < quest:ReadGlobalGameData(0xff0))
    end
    alive = not quest:IsActiveThreadTerminating()
end

function Tutorial_LowMagic(quest)
    local bVar2, fret_0, iVar3
    local alive = true
    iVar3 = 0
    if quest:GetStateInt("WillEnergyWarningsGiven") < quest:ReadGlobalGameData(0xffc) then
        repeat
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            if quest:GetMasterGameState("KilledGM") then break end
            bVar2 = quest:IsHeroControlledByPlayer()
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                local __native_condition_1 = iVar3 < 1
                if __native_condition_1 then
                    fret_0 = quest:GetHeroWillEnergyLevel()
                    __native_condition_1 = fret_0 < quest:ReadGlobalGameDataFloat(0xff4)
                end
                if __native_condition_1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_080_WILL_LOW", "", true, false)
                    iVar3 = quest:ReadGlobalGameData(0xff8)
                    quest:SetStateInt("WillEnergyWarningsGiven", quest:GetStateInt("WillEnergyWarningsGiven") + 1)
                end
                iVar3 = iVar3 + -1
            end
        until not (quest:GetStateInt("WillEnergyWarningsGiven") < quest:ReadGlobalGameData(0xffc))
    end
    alive = not quest:IsActiveThreadTerminating()
end

function Tutorial_RenownLevelIncrease(quest)
    local iVar2, register0x00000010
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    local bVar1 = not alive
    while true do
        if bVar1 then
            iVar2 = 8
            repeat
                register0x00000010 = (register0x00000010 + -4)
                iVar2 = iVar2 + -1
            until not (iVar2 ~= 0)
            return
        end
        if quest:GetMasterGameState("KilledGM") then break end
        iVar2 = quest:GetHeroRenownLevel()
        bVar1 = quest:IsHeroControlledByPlayer()
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                iVar2 = 8
                repeat
                    register0x00000010 = (register0x00000010 + -4)
                    iVar2 = iVar2 + -1
                until not (iVar2 ~= 0)
                return
            end
            if quest:GetStateInt("SavedRenownLevel") < iVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    -- TODO(native): _vector_constructor_iterator_((int)xStack_20,4,8,0x99eae0);
                    return
                end
                quest:SetStateInt("SavedRenownLevel", iVar2)
                if iVar2 < 8 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        -- TODO(native): _vector_constructor_iterator_((int)xStack_20,4,8,0x99eae0);
                        return
                    end
                    quest:HeroReceiveMessageFromGuildMaster("", "TEXT_QST_080_RENOWN_LEVEL_SEVEN", true, true)
                end
                if 1 < iVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        -- TODO(native): _vector_constructor_iterator_((int)xStack_20,4,8,0x99eae0);
                        return
                    end
                    quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
                    quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
                    quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
                end
            end
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    iVar2 = 8
    if bVar1 then
        repeat
            register0x00000010 = (register0x00000010 + -4)
            iVar2 = iVar2 + -1
        until not (iVar2 ~= 0)
        return
    end
    repeat
        register0x00000010 = (register0x00000010 + -4)
        iVar2 = iVar2 + -1
    until not (iVar2 ~= 0)
end

