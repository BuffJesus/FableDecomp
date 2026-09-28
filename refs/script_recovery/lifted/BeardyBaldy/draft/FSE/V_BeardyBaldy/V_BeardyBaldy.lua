-- Generated native draft: V_BeardyBaldy. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local __native_condition_1, b3, bVar1, bVar9, cVar2, delay, iVar5, native_arg_sequence_1, pCVar4, pRelativeTo, r1, r2, xStack_24
    local alive = true
    quest:AddEntityBinding("BB_BeardyBaldyMan", "V_BeardyBaldy/Entities/BB_BeardyBaldyMan")
    quest:FinalizeEntityBindings()
    alive = not quest:IsActiveThreadTerminating()
    bVar9 = not alive
    if not bVar9 then
        repeat
            xStack_24 = quest:MsgOnRegionLoaded()
            bVar9 = (xStack_24 ~= nil)
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then goto LAB_00e500eb end
                xStack_24 = quest:MsgOnRegionLoaded()
                bVar9 = (xStack_24 ~= nil)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then break end
            if xStack_24 == nil then
                bVar9 = false
                if bVar9 then
                    goto LAB_00e4fb27
                end
            else
                iVar5 = ((xStack_24 == "BowerstoneSlumsWarehouses") and 0 or 1)
                if iVar5 == 0 then goto LAB_00e4fb27 end
            end
            goto FLOW_past_lab_00e4fb27
            ::LAB_00e4fb27::
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            native_arg_sequence_1 = false
            if bVar9 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then
                break
            end
            quest:CreateThread("WatchForQuestFinished")  -- native thread body CV_AmbushScamScript::WatchForQuestFinished: lift it as function WatchForQuestFinished(quest)
            quest:CreateThread("WatchForQuestCardConditions")  -- native thread body WatchForQuestCardConditions: lift it as function WatchForQuestCardConditions(quest)
            quest:CreateThread("WatchForBarberLadyDeath")  -- native thread body WatchForBarberLadyDeath: lift it as function WatchForBarberLadyDeath(quest)
            quest:CreateThread("WatchForBeardyBaldyDeath")  -- native thread body WatchForBeardyBaldyDeath: lift it as function WatchForBeardyBaldyDeath(quest)
            quest:CreateThread("WatchForNewHairdo")  -- native thread body 0x00E50150: lift it as function WatchForNewHairdo(quest)
            quest:CreateThread("WatchForNewBeard")  -- native thread body 0x00E50210: lift it as function WatchForNewBeard(quest)
            quest:CreateThread("WatchForNewTash")  -- native thread body 0x00E502E0: lift it as function WatchForNewTash(quest)
            bVar9 = false
            if not quest:GetStateBool("QuestActivated") then
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then break end
                quest:SetStateBool("QuestActivated", true)
            end
            GoTalkToBeardyBaldy(quest)
            goto LAB_00e4fef3
            ::FLOW_past_lab_00e4fb27::
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
        until false
    end
    ::LAB_00e500eb::
    do return end
    ::LAB_00e4fef3::
    if not quest:GetStateBool("QuestFinished") then
        if quest:GetStateBool("QuestComplete") then
            bVar9 = true
            bVar1 = quest:IsRegionLoaded("BowerstoneSlumsWarehouses")
            if not bVar1 then goto LAB_00e4ff2f end
        end
        bVar1 = true
    else
        goto LAB_00e4ff2f
    end
    goto FLOW_past_lab_00e4ff2f
    ::LAB_00e4ff2f::
    bVar1 = false
    ::FLOW_past_lab_00e4ff2f::
    if bVar9 then
        bVar9 = false
    end
    if not bVar1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar9 = not alive
        if not bVar9 then
            r1 = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
            pRelativeTo = quest:GetHero()
            r2 = quest:GetNearestWithScriptName(pRelativeTo, "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER")
            __native_condition_1 = not (r2 ~= nil and not r2:IsNull())
            if not __native_condition_1 then
                cVar2 = (r2 ~= nil and r2:IsAlive())
                __native_condition_1 = not cVar2
            end
            if __native_condition_1 then
                goto LAB_00e5002c
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if not bVar9 then
                    quest:MiniMapRemoveMarker(r2)
                    quest:ClearThingHasInformation(r2)
                    goto LAB_00e5002c
                end
            end
            goto FLOW_past_lab_00e5002c
            ::LAB_00e5002c::
            iVar5 = (r1 ~= nil and r1:IsAlive())
            if iVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then goto LAB_00e500d9 end
                quest:MiniMapRemoveMarker(r1)
                quest:ClearThingHasInformation(r1)
            end
            b3 = false
            bVar1 = false
            bVar9 = false
            pCVar4 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar4, bVar9, bVar1, b3)
            pCVar4 = quest:GetActiveQuestName()
            quest:RemoveQuestCardFromHero(pCVar4)
            delay = 0
            pCVar4 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(pCVar4, delay)
            ::FLOW_past_lab_00e5002c::
            ::LAB_00e500d9::
        end
        goto LAB_00e500eb
    end
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if bVar1 then
        return
    end
    goto LAB_00e4fef3
end

function Init(quest)
    quest:SetStateInt("RandomSpeechTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateBool("QuestFinished", false)
    quest:SetStateBool("QuestActivated", false)
    quest:SetStateBool("QuestComplete", false)
    quest:SetStateInt("QuestPhase", 0)
    quest:SetStateBool("InitialisePhase", true)
    quest:SetStateBool("IntroComplete", false)
    quest:SetStateBool("Phase1RequirementsComplete", false)
    quest:SetStateBool("Phase2RequirementsComplete", false)
    quest:SetStateBool("Phase3RequirementsComplete", false)
    quest:SetStateString("RequiredHairdo", "")
    quest:SetStateString("RequiredBeard", "")
    quest:SetStateString("RequiredTash", "")
    quest:SetStateBool("BeardyBaldyKilled", false)
    quest:SetStateBool("BarberLadyKilled", false)
    quest:SetStateBool("BeardyBaldyLeft", false)
    quest:SetStateBool("AcquiredNewHairdo", false)
    quest:SetStateBool("AcquiredNewBeard", false)
    quest:SetStateBool("AcquiredNewTash", false)
    quest:SetStateBool("AllHairChanged", false)
    quest:SetStateInt("IncorrectHairComboCount", 0)
    quest:SetStateBool("AttackedByHero", false)
    quest:SetStateString(("RandomSpeech_" .. 0), "TEXT_QST_014_RANDOM_00")
    quest:SetStateString(("RandomSpeech_" .. 1), "TEXT_QST_014_RANDOM_10")
    quest:SetStateString(("RandomSpeech_" .. 2), "TEXT_QST_014_RANDOM_20")
    quest:SetStateString(("RandomSpeech_" .. 3), "TEXT_QST_014_RANDOM_30")
    quest:SetStateString(("RandomSpeech_" .. 4), "TEXT_QST_014_RANDOM_40")
    quest:SetStateString(("RandomSpeech_" .. 5), "TEXT_QST_014_RANDOM_50")
    quest:SetStateString(("RandomSpeech_" .. 6), "TEXT_QST_014_RANDOM_60")
    quest:SetStateString(("RandomSpeech_" .. 7), "TEXT_QST_014_RANDOM_70")
    quest:SetStateString(("RandomSpeech_" .. 8), "TEXT_QST_014_RANDOM_80")
    quest:SetStateString(("RandomSpeech_" .. 9), "TEXT_QST_014_RANDOM_90")
end

function OnPersist(quest, context)
    local questFinished = quest:GetStateBool("QuestFinished") or false
    questFinished = quest:PersistTransferBool(context, "QuestFinished", questFinished)
    quest:SetStateBool("QuestFinished", questFinished)
    local questActivated = quest:GetStateBool("QuestActivated") or false
    questActivated = quest:PersistTransferBool(context, "QuestActivated", questActivated)
    quest:SetStateBool("QuestActivated", questActivated)
    local questComplete = quest:GetStateBool("QuestComplete") or false
    questComplete = quest:PersistTransferBool(context, "QuestComplete", questComplete)
    quest:SetStateBool("QuestComplete", questComplete)
    local questPhase = quest:GetStateInt("QuestPhase") or 0
    questPhase = quest:PersistTransferInt(context, "QuestPhase", questPhase)
    quest:SetStateInt("QuestPhase", questPhase)
    local initialisePhase = quest:GetStateBool("InitialisePhase") or false
    initialisePhase = quest:PersistTransferBool(context, "InitialisePhase", initialisePhase)
    quest:SetStateBool("InitialisePhase", initialisePhase)
    local introComplete = quest:GetStateBool("IntroComplete") or false
    introComplete = quest:PersistTransferBool(context, "IntroComplete", introComplete)
    quest:SetStateBool("IntroComplete", introComplete)
    local phase1RequirementsComplete = quest:GetStateBool("Phase1RequirementsComplete") or false
    phase1RequirementsComplete = quest:PersistTransferBool(context, "Phase1RequirementsComplete", phase1RequirementsComplete)
    quest:SetStateBool("Phase1RequirementsComplete", phase1RequirementsComplete)
    local phase2RequirementsComplete = quest:GetStateBool("Phase2RequirementsComplete") or false
    phase2RequirementsComplete = quest:PersistTransferBool(context, "Phase2RequirementsComplete", phase2RequirementsComplete)
    quest:SetStateBool("Phase2RequirementsComplete", phase2RequirementsComplete)
    local phase3RequirementsComplete = quest:GetStateBool("Phase3RequirementsComplete") or false
    phase3RequirementsComplete = quest:PersistTransferBool(context, "Phase3RequirementsComplete", phase3RequirementsComplete)
    quest:SetStateBool("Phase3RequirementsComplete", phase3RequirementsComplete)
    local beardyBaldyKilled = quest:GetStateBool("BeardyBaldyKilled") or false
    beardyBaldyKilled = quest:PersistTransferBool(context, "BeardyBaldyKilled", beardyBaldyKilled)
    quest:SetStateBool("BeardyBaldyKilled", beardyBaldyKilled)
    local barberLadyKilled = quest:GetStateBool("BarberLadyKilled") or false
    barberLadyKilled = quest:PersistTransferBool(context, "BarberLadyKilled", barberLadyKilled)
    quest:SetStateBool("BarberLadyKilled", barberLadyKilled)
    local acquiredNewHairdo = quest:GetStateBool("AcquiredNewHairdo") or false
    acquiredNewHairdo = quest:PersistTransferBool(context, "AcquiredNewHairdo", acquiredNewHairdo)
    quest:SetStateBool("AcquiredNewHairdo", acquiredNewHairdo)
    local acquiredNewBeard = quest:GetStateBool("AcquiredNewBeard") or false
    acquiredNewBeard = quest:PersistTransferBool(context, "AcquiredNewBeard", acquiredNewBeard)
    quest:SetStateBool("AcquiredNewBeard", acquiredNewBeard)
    local acquiredNewTash = quest:GetStateBool("AcquiredNewTash") or false
    acquiredNewTash = quest:PersistTransferBool(context, "AcquiredNewTash", acquiredNewTash)
    quest:SetStateBool("AcquiredNewTash", acquiredNewTash)
    local allHairChanged = quest:GetStateBool("AllHairChanged") or false
    allHairChanged = quest:PersistTransferBool(context, "AllHairChanged", allHairChanged)
    quest:SetStateBool("AllHairChanged", allHairChanged)
    local incorrectHairComboCount = quest:GetStateInt("IncorrectHairComboCount") or 0
    incorrectHairComboCount = quest:PersistTransferInt(context, "IncorrectHairComboCount", incorrectHairComboCount)
    quest:SetStateInt("IncorrectHairComboCount", incorrectHairComboCount)
    local attackedByHero = quest:GetStateBool("AttackedByHero") or false
    attackedByHero = quest:PersistTransferBool(context, "AttackedByHero", attackedByHero)
    quest:SetStateBool("AttackedByHero", attackedByHero)
end

function WatchForQuestFinished(quest)
    local bVar2
    local alive = true
    local CVar1 = quest:GetStateBool("BeardyBaldyKilled")
    while ((not CVar1 and (not quest:GetStateBool("BarberLadyKilled"))) and (not quest:GetStateBool("BeardyBaldyLeft"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        CVar1 = quest:GetStateBool("BeardyBaldyKilled")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:SetStateBool("QuestFinished", true)
    end
end

function WatchForQuestCardConditions(quest)
    local CVar1, bVar2, cVar3, iVar6, native_arg_sequence_1, p0, pCVar4, pCVar5, xStack_18, xStack_48
    local alive = true
    local function __cleanup_LAB_00e50dd6()
        xStack_18 = nil
    end
    CVar1 = quest:GetStateBool("Phase1RequirementsComplete")
    xStack_18 = nil
    while not CVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            xStack_18 = nil
            return
        end
        CVar1 = quest:GetStateBool("Phase1RequirementsComplete")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        -- LAB_00e5097e: (native jump target)
        return
    end
    bVar2 = true
    pCVar4 = quest:GetActiveQuestName()
    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BEARDY_BALDY", pCVar4, bVar2)
    pCVar5 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "")
    GoTalkToBeardyBaldy(quest)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        __cleanup_LAB_00e50dd6()
        return
    end
    ::LAB_00e50ab0::
    CVar1 = quest:GetStateBool("AcquiredNewHairdo")
    while ((not CVar1 and (not quest:GetStateBool("AcquiredNewBeard"))) and (not quest:GetStateBool("AcquiredNewTash"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e50cf5 end
        CVar1 = quest:GetStateBool("AcquiredNewHairdo")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        pCVar5 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_03", "", "")
        GoTalkToBeardyBaldy(quest)
        quest:SetStateBool("AcquiredNewHairdo", false)
        quest:SetStateBool("AcquiredNewBeard", false)
        quest:SetStateBool("AcquiredNewTash", false)
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                xStack_18 = nil
                goto LAB_00e50cf5
            end
            xStack_48 = quest:MsgOnRegionLoaded()
            bVar2 = (xStack_48 ~= nil)
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e50cf5 end
                xStack_48 = quest:MsgOnRegionLoaded()
                bVar2 = (xStack_48 ~= nil)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
            iVar6 = ((xStack_48 == "BowerstoneSlumsWarehouses") and 0 or 1)
            if iVar6 == 0 then goto LAB_00e50bfe end
            alive = quest:NewScriptFrame()
        until false
    end
    goto LAB_00e50cf5
    ::LAB_00e50bfe::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        p0 = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
        xStack_18 = p0
        native_arg_sequence_1 = false
        if not (xStack_18 ~= nil and not xStack_18:IsNull()) then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if not native_arg_sequence_1 then
            cVar3 = (xStack_18 ~= nil and xStack_18:IsAlive())
            if not cVar3 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then goto LAB_00e50db6 end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            while true do
                if not (xStack_18 ~= nil and not xStack_18:IsNull()) then
                    cVar3 = false
                else
                    cVar3 = xStack_18:IsTalkedToByHero()
                end
                if cVar3 then break end
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e50cf5 end
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                if not quest:GetStateBool("Phase3RequirementsComplete") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e50cf5 end
                    pCVar5 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(pCVar5, "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "")
                end
                goto LAB_00e50db6
            end
        end
        goto FLOW_past_lab_00e50db6
        ::LAB_00e50db6::
        GoTalkToBeardyBaldy(quest)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then __cleanup_LAB_00e50dd6(); return end
        goto LAB_00e50ab0
        ::FLOW_past_lab_00e50db6::
    end
    ::LAB_00e50cf5::
    return
end

function WatchForBarberLadyDeath(quest)
    local bVar3, cVar2
    local alive = true
    local pRelativeTo = quest:GetHero()
    local r1 = quest:GetNearestWithScriptName(pRelativeTo, "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER")
    while true do
        local __native_condition_1 = not (r1 ~= nil and not r1:IsNull())
        if not __native_condition_1 then
            cVar2 = (r1 ~= nil and r1:IsAlive())
            __native_condition_1 = not cVar2
        end
        if not __native_condition_1 then break end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            r1 = nil
            return
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    while true do
        if bVar3 then
            r1 = nil
            return
        end
        if not (r1 ~= nil and not r1:IsNull()) then
            cVar2 = false
        else
            cVar2 = (r1 ~= nil and r1:MsgIsKilledBy(""))
        end
        if cVar2 then break end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        quest:SetStateBool("BarberLadyKilled", true)
    end
end

function WatchForBeardyBaldyDeath(quest)
    local bVar2, cVar1
    local alive = true
    local r1 = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
    while true do
        if not (r1 ~= nil and not r1:IsNull()) then
            cVar1 = false
        else
            cVar1 = (r1 ~= nil and r1:MsgIsKilledBy(""))
        end
        if cVar1 then break end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            r1 = nil
            return
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:SetStateBool("BeardyBaldyKilled", true)
    end
end

function WatchForNewHairdo(quest)
    local bVar2, cVar1
    local alive = true
    cVar1 = quest:GetStateBool("Phase1RequirementsComplete")
    while not cVar1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e50201 end
        cVar1 = quest:GetStateBool("Phase1RequirementsComplete")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        while not bVar2 do
            bVar2 = quest:MsgOnHeroHairTypeChanged(0x10)
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e50201 end
                bVar2 = quest:MsgOnHeroHairTypeChanged(0x10)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
            quest:SetStateBool("AcquiredNewHairdo", true)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
        end
    end
    ::LAB_00e50201::
end

function WatchForNewBeard(quest)
    local bVar2, cVar1
    local alive = true
    cVar1 = quest:GetStateBool("Phase2RequirementsComplete")
    while (not cVar1 and (not quest:GetStateBool("AcquiredNewHairdo"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e502ce end
        cVar1 = quest:GetStateBool("Phase2RequirementsComplete")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        while not bVar2 do
            bVar2 = quest:MsgOnHeroHairTypeChanged(0x80)
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e502ce end
                bVar2 = quest:MsgOnHeroHairTypeChanged(0x80)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then break end
            quest:SetStateBool("AcquiredNewBeard", true)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
        end
    end
    ::LAB_00e502ce::
end

function WatchForNewTash(quest)
    local bVar2, cVar1
    local alive = true
    cVar1 = quest:GetStateBool("Phase3RequirementsComplete")
    while (not cVar1 and (not quest:GetStateBool("AcquiredNewBeard"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        cVar1 = quest:GetStateBool("Phase3RequirementsComplete")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        -- LAB_00e50320: (native jump target)
        return
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    while not bVar2 do
        bVar2 = quest:MsgOnHeroHairTypeChanged(0x100)
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e503b0 end
            bVar2 = quest:MsgOnHeroHairTypeChanged(0x100)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then break end
        quest:SetStateBool("AcquiredNewTash", true)
        quest:SetStateBool("AllHairChanged", true)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    end
    ::LAB_00e503b0::
end

function WatchForAttack(quest, native_arg_moi)
    local __native_condition_1, __native_condition_2, bVar1, bVar3, cVar2, fret_0, native_arg_sequence_1
    local alive = true
    bVar3 = false
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    repeat
        if bVar1 then
            return
        end
        __native_condition_1 = not (native_arg_moi ~= nil and not native_arg_moi:IsNull())
        if not __native_condition_1 then
            cVar2 = (native_arg_moi ~= nil and native_arg_moi:IsAlive())
            __native_condition_1 = not cVar2
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            return
        end
        __native_condition_2 = not (native_arg_moi ~= nil and not native_arg_moi:IsNull())
        if not __native_condition_2 then
            cVar2 = native_arg_moi:MsgIsHitByHero()
            __native_condition_2 = not cVar2
        end
        if __native_condition_2 then
            bVar3 = true
            native_arg_sequence_1 = false
            if (native_arg_moi ~= nil and not native_arg_moi:IsNull()) then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if native_arg_sequence_1 then
                -- TODO(native): MsgIsHitByAnyAggressiveSpecialAbilityFrom is not a ForgeFSE binding
                cVar2 = (native_arg_moi ~= nil and native_arg_moi:MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO"))
                if cVar2 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then goto LAB_00e53e21 end
            goto LAB_00e53e43
        else
            goto LAB_00e53e21
        end
        goto FLOW_past_lab_00e53e43
        ::LAB_00e53e43::
        bVar1 = false
        ::FLOW_past_lab_00e53e43::
        goto FLOW_past_lab_00e53e21
        ::LAB_00e53e21::
        fret_0 = quest:GetHealth(native_arg_moi)
        bVar1 = true
        if fret_0 <= 9.999999747378752e-05 then goto LAB_00e53e43 end
        ::FLOW_past_lab_00e53e21::
        if bVar3 then
            bVar3 = false
        end
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            quest:SetStateBool("AttackedByHero", true)
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
    until false
end

function GoTalkToBeardyBaldy(quest)
    local bVar2, cVar1
    local alive = true
    local r1 = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
    local pRelativeTo = quest:GetHero()
    local r2 = quest:GetNearestWithDefName(pRelativeTo, "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER")
    if (r1 ~= nil and not r1:IsNull()) then
        cVar1 = (r1 ~= nil and r1:IsAlive())
        if cVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            quest:MiniMapAddMarker(r1, "HUD_ORB_QUEST_VIGNETTE")
        end
    end
    if (r2 ~= nil and not r2:IsNull()) then
        cVar1 = (r2 ~= nil and r2:IsAlive())
        if cVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                -- LAB_00e5072f: (native jump target)
                return
            end
            quest:MiniMapRemoveMarker(r2)
        end
    end
    r2 = nil
    r1 = nil
end

function SetWanderPointAndDistance(quest, native_arg_param_1, native_arg_param_2)
    local center = {x = native_arg_param_2.x, y = native_arg_param_2.y, z = native_arg_param_2.z}
    quest:SetWanderCentrePoint(native_arg_param_1, center)
    local fVar3 = quest:ReadGlobalGameDataFloat(0x448)
    quest:SetWanderMinDistance(native_arg_param_1, fVar3)
    fVar3 = quest:ReadGlobalGameDataFloat(0x44c)
    quest:SetWanderMaxDistance(native_arg_param_1, fVar3)
    quest:SetScriptingStateGroup(native_arg_param_1, 4)
end

function IsHeroWearingAnyTash(quest)
    local bVar3, bVar4, bVar5, bVar6, bVar7, bVar8, pCVar9, u_stk_19
    bVar7 = false
    bVar6 = false
    bVar5 = false
    bVar4 = false
    bVar3 = false
    pCVar9 = quest:GetHero()
    bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHSMITH_01")
    if not bVar8 then
        bVar7 = true
        bVar6 = false
        bVar5 = false
        bVar4 = false
        bVar3 = false
        pCVar9 = quest:GetHero()
        bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHTRADER_01")
        if not bVar8 then
            bVar7 = true
            bVar6 = true
            bVar5 = false
            bVar4 = false
            bVar3 = false
            pCVar9 = quest:GetHero()
            bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHKHG_01")
            if not bVar8 then
                bVar7 = true
                bVar6 = true
                bVar5 = true
                bVar4 = false
                bVar3 = false
                pCVar9 = quest:GetHero()
                bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHSHERIFF_01")
                if not bVar8 then
                    bVar7 = true
                    bVar6 = true
                    bVar5 = true
                    bVar4 = true
                    bVar3 = false
                    pCVar9 = quest:GetHero()
                    bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHCHINESE_01")
                    if not bVar8 then
                        bVar7 = true
                        bVar6 = true
                        bVar5 = true
                        bVar4 = true
                        bVar3 = true
                        pCVar9 = quest:GetHero()
                        bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHSMALL_01")
                        u_stk_19 = false
                        if not bVar8 then goto LAB_00e53a60 end
                    end
                end
            end
        end
    end
    u_stk_19 = true
    ::LAB_00e53a60::
    if bVar3 then
    end
    if bVar4 then
    end
    if bVar5 then
    end
    if bVar6 then
    end
    if bVar7 then
    end
    return u_stk_19
end

function IsHeroWearingAnyOddHairdo(quest)
    local bVar3, bVar4, bVar5, bVar6, bVar7, pCVar8, u_stk_15
    bVar6 = false
    bVar5 = false
    bVar4 = false
    bVar3 = false
    pCVar8 = quest:GetHero()
    bVar7 = quest:IsWearingHairstyle(pCVar8, "OBJECT_HERO_HAIR_BUZZ_01")
    if not bVar7 then
        bVar6 = true
        bVar5 = false
        bVar4 = false
        bVar3 = false
        pCVar8 = quest:GetHero()
        bVar7 = quest:IsWearingHairstyle(pCVar8, "OBJECT_HERO_HAIR_BASIN_01")
        if not bVar7 then
            bVar6 = true
            bVar5 = true
            bVar4 = false
            bVar3 = false
            pCVar8 = quest:GetHero()
            bVar7 = quest:IsWearingHairstyle(pCVar8, "OBJECT_HERO_HAIR_MOHAWK_01")
            if not bVar7 then
                bVar6 = true
                bVar5 = true
                bVar4 = true
                bVar3 = false
                pCVar8 = quest:GetHero()
                bVar7 = quest:IsWearingHairstyle(pCVar8, "OBJECT_HERO_HAIR_PONYTAIL_01")
                if not bVar7 then
                    bVar6 = true
                    bVar5 = true
                    bVar4 = true
                    bVar3 = true
                    pCVar8 = quest:GetHero()
                    bVar7 = quest:IsWearingHairstyle(pCVar8, "OBJECT_HERO_HAIR_PLATS_01")
                    u_stk_15 = false
                    if not bVar7 then goto LAB_00e53c05 end
                end
            end
        end
    end
    u_stk_15 = true
    ::LAB_00e53c05::
    if bVar3 then
    end
    if bVar4 then
    end
    if bVar5 then
    end
    if bVar6 then
    end
    return u_stk_15
end

function GoTalkToBarber(quest)
    local bVar2, cVar1
    local alive = true
    local r1 = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
    local pRelativeTo = quest:GetHero()
    local r2 = quest:GetNearestWithDefName(pRelativeTo, "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER")
    if (r1 ~= nil and not r1:IsNull()) then
        cVar1 = (r1 ~= nil and r1:IsAlive())
        if cVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            quest:MiniMapRemoveMarker(r1)
        end
    end
    if (r2 ~= nil and not r2:IsNull()) then
        cVar1 = (r2 ~= nil and r2:IsAlive())
        if cVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                -- LAB_00e50f01: (native jump target)
                return
            end
            quest:MiniMapAddMarker(r2, "HUD_ORB_QUEST_VIGNETTE")
        end
    end
    r2 = nil
    r1 = nil
end

function helper_E53C70(quest)
    local iVar2 = math.random(0, 32767)
    quest:SetTimer(quest:GetStateInt("RandomSpeechTimer"), quest:ReadGlobalGameData(0x494) - iVar2 % quest:ReadGlobalGameData(0x498))
end

