-- Readable native conversion: V_BeardyBaldy. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    BeardyBaldyWanderDistanceMin = 1096,  -- 0.0
    BeardyBaldyWanderDistanceMax = 1100,  -- 50.0
    BeardyBaldyRandomSpeechTimerMax = 1172,  -- 30
    BeardyBaldyRandomSpeechTimerVar = 1176,  -- 10
}

-- V_BeardyBaldy.Main (retail 0x00e4fa20)
function Main(quest)
    local getActiveQuestName, getActiveQuestName2, getActiveQuestName3, msgOnRegionLoaded
    quest:AddEntityBinding("BB_BeardyBaldyMan", "V_BeardyBaldy/Entities/BB_BeardyBaldyMan")
    quest:FinalizeEntityBindings()
    if not quest:IsActiveThreadTerminating() then
        repeat
            msgOnRegionLoaded = quest:MsgOnRegionLoaded()
            while msgOnRegionLoaded == nil do
                if not quest:NewScriptFrame() then goto LAB_00e500eb end
                msgOnRegionLoaded = quest:MsgOnRegionLoaded()
            end
            if quest:IsActiveThreadTerminating() then break end
            if msgOnRegionLoaded ~= nil and msgOnRegionLoaded == "BowerstoneSlumsWarehouses" then
                if quest:IsActiveThreadTerminating() or quest:IsActiveThreadTerminating() then break end
                quest:CreateThread("WatchForQuestFinished")  -- native thread body CV_AmbushScamScript::WatchForQuestFinished: lift it as function WatchForQuestFinished(quest)
                quest:CreateThread("WatchForQuestCardConditions")  -- native thread body WatchForQuestCardConditions: lift it as function WatchForQuestCardConditions(quest)
                quest:CreateThread("WatchForBarberLadyDeath")  -- native thread body WatchForBarberLadyDeath: lift it as function WatchForBarberLadyDeath(quest)
                quest:CreateThread("WatchForBeardyBaldyDeath")  -- native thread body WatchForBeardyBaldyDeath: lift it as function WatchForBeardyBaldyDeath(quest)
                quest:CreateThread("WatchForNewHairdo")  -- native thread body 0x00E50150: lift it as function WatchForNewHairdo(quest)
                quest:CreateThread("WatchForNewBeard")  -- native thread body 0x00E50210: lift it as function WatchForNewBeard(quest)
                quest:CreateThread("WatchForNewTash")  -- native thread body 0x00E502E0: lift it as function WatchForNewTash(quest)
                if not quest:GetStateBool("QuestActivated") then
                    if quest:IsActiveThreadTerminating() then break end
                    quest:SetStateBool("QuestActivated", true)
                end
                GoTalkToBeardyBaldy(quest)
                goto LAB_00e4fef3
            end
            if not quest:NewScriptFrame() then return end
        until false
    end
    ::LAB_00e500eb::
    do return end
    ::LAB_00e4fef3::
    if quest:GetStateBool("QuestFinished") or (quest:GetStateBool("QuestComplete") and (not quest:IsRegionLoaded("BowerstoneSlumsWarehouses"))) then
        if not quest:IsActiveThreadTerminating() then
            local beardyBaldyMan = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
            local pRelativeTo = quest:GetHero()
            local bowerstoneSlumsVillagerFemaleBarber = quest:GetNearestWithScriptName(pRelativeTo, "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER")
            if bowerstoneSlumsVillagerFemaleBarber == nil or not (bowerstoneSlumsVillagerFemaleBarber ~= nil and bowerstoneSlumsVillagerFemaleBarber:IsAlive()) then
                goto LAB_00e5002c
            elseif not quest:IsActiveThreadTerminating() then
                quest:MiniMapRemoveMarker(bowerstoneSlumsVillagerFemaleBarber)
                quest:ClearThingHasInformation(bowerstoneSlumsVillagerFemaleBarber)
                goto LAB_00e5002c
            end
            goto FLOW_past_lab_00e5002c
            ::LAB_00e5002c::
            if beardyBaldyMan ~= nil and beardyBaldyMan:IsAlive() then
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e5002c end
                quest:MiniMapRemoveMarker(beardyBaldyMan)
                quest:ClearThingHasInformation(beardyBaldyMan)
            end
            getActiveQuestName = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(getActiveQuestName, false, false, false)
            getActiveQuestName2 = quest:GetActiveQuestName()
            quest:RemoveQuestCardFromHero(getActiveQuestName2)
            getActiveQuestName3 = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(getActiveQuestName3, 0)
            ::FLOW_past_lab_00e5002c::
        end
        goto LAB_00e500eb
    end
    if not quest:NewScriptFrame() then return end
    goto LAB_00e4fef3
end

-- V_BeardyBaldy.Init (retail 0x00e4f620)
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
    quest:SetStateString("RandomSpeech_" .. 0, "TEXT_QST_014_RANDOM_00")
    quest:SetStateString("RandomSpeech_" .. 1, "TEXT_QST_014_RANDOM_10")
    quest:SetStateString("RandomSpeech_" .. 2, "TEXT_QST_014_RANDOM_20")
    quest:SetStateString("RandomSpeech_" .. 3, "TEXT_QST_014_RANDOM_30")
    quest:SetStateString("RandomSpeech_" .. 4, "TEXT_QST_014_RANDOM_40")
    quest:SetStateString("RandomSpeech_" .. 5, "TEXT_QST_014_RANDOM_50")
    quest:SetStateString("RandomSpeech_" .. 6, "TEXT_QST_014_RANDOM_60")
    quest:SetStateString("RandomSpeech_" .. 7, "TEXT_QST_014_RANDOM_70")
    quest:SetStateString("RandomSpeech_" .. 8, "TEXT_QST_014_RANDOM_80")
    quest:SetStateString("RandomSpeech_" .. 9, "TEXT_QST_014_RANDOM_90")
end

-- V_BeardyBaldy.OnPersist (retail 0x00e4f7f0)
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

-- V_BeardyBaldy.WatchForQuestFinished (retail 0x00e50100)
function WatchForQuestFinished(quest)
    while (not quest:GetStateBool("BeardyBaldyKilled") and (not quest:GetStateBool("BarberLadyKilled"))) and not quest:GetStateBool("BeardyBaldyLeft") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetStateBool("QuestFinished", true)
end

-- V_BeardyBaldy.WatchForQuestCardConditions (retail 0x00e50910)
function WatchForQuestCardConditions(quest)
    local phase1RequirementsComplete, scratchValue3, msgOnRegionLoaded
    local function ReleaseEverything()
        local scratchValue3 = nil
    end
    phase1RequirementsComplete = quest:GetStateBool("Phase1RequirementsComplete")
    scratchValue3 = nil
    while not phase1RequirementsComplete do
        quest:NewScriptFrame()
        if not quest:IsActiveThreadTerminating() then
            phase1RequirementsComplete = quest:GetStateBool("Phase1RequirementsComplete")
        else
            scratchValue3 = nil
            do return end
            phase1RequirementsComplete = quest:GetStateBool("Phase1RequirementsComplete")
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    local getActiveQuestName = quest:GetActiveQuestName()
    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BEARDY_BALDY", getActiveQuestName, true)
    local getActiveQuestName2 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(getActiveQuestName2, "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "")
    GoTalkToBeardyBaldy(quest)
    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
    ::LAB_00e50ab0::
    while (not quest:GetStateBool("AcquiredNewHairdo") and (not quest:GetStateBool("AcquiredNewBeard"))) and not quest:GetStateBool("AcquiredNewTash") do
        if not quest:NewScriptFrame() then return end
    end
    if not quest:IsActiveThreadTerminating() then
        local getActiveQuestName3 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(getActiveQuestName3, "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_03", "", "")
        GoTalkToBeardyBaldy(quest)
        quest:SetStateBool("AcquiredNewHairdo", false)
        quest:SetStateBool("AcquiredNewBeard", false)
        quest:SetStateBool("AcquiredNewTash", false)
        repeat
            if quest:IsActiveThreadTerminating() then scratchValue3 = nil; return end
            msgOnRegionLoaded = quest:MsgOnRegionLoaded()
            while msgOnRegionLoaded == nil do
                if not quest:NewScriptFrame() then return end
                msgOnRegionLoaded = quest:MsgOnRegionLoaded()
            end
            if quest:IsActiveThreadTerminating() then break end
            if msgOnRegionLoaded == "BowerstoneSlumsWarehouses" then goto LAB_00e50bfe end
            quest:NewScriptFrame()
        until false
    end
    do return end
    ::LAB_00e50bfe::
    if quest:IsActiveThreadTerminating() then return end
    scratchValue3 = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
    if scratchValue3 == nil or not (scratchValue3 ~= nil and scratchValue3:IsAlive()) then goto LAB_00e50db6 end
    if not quest:IsActiveThreadTerminating() then
        while true do
            if (scratchValue3 ~= nil and not scratchValue3:IsNull()) and scratchValue3:IsTalkedToByHero() then break end
            if not quest:NewScriptFrame() then return end
        end
        if not quest:IsActiveThreadTerminating() then
            if not quest:GetStateBool("Phase3RequirementsComplete") then
                if quest:IsActiveThreadTerminating() then return end
                local getActiveQuestName4 = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(getActiveQuestName4, "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "")
            end
            goto LAB_00e50db6
        end
    end
    do return end
    ::LAB_00e50db6::
    GoTalkToBeardyBaldy(quest)
    if not quest:NewScriptFrame() then ReleaseEverything(); return end
    goto LAB_00e50ab0
end

-- V_BeardyBaldy.WatchForBarberLadyDeath (retail 0x00e504c0)
function WatchForBarberLadyDeath(quest)
    local predicateResult
    local pRelativeTo = quest:GetHero()
    local bowerstoneSlumsVillagerFemaleBarber = quest:GetNearestWithScriptName(pRelativeTo, "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER")
    while true do
        local predicateResult4 = not (bowerstoneSlumsVillagerFemaleBarber ~= nil and not bowerstoneSlumsVillagerFemaleBarber:IsNull()) or not (bowerstoneSlumsVillagerFemaleBarber ~= nil and bowerstoneSlumsVillagerFemaleBarber:IsAlive())
        if not predicateResult4 then break end
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            bowerstoneSlumsVillagerFemaleBarber = nil
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    predicateResult = false
    while true do
        if predicateResult then
            bowerstoneSlumsVillagerFemaleBarber = nil
            return
        end
        if (bowerstoneSlumsVillagerFemaleBarber ~= nil and not bowerstoneSlumsVillagerFemaleBarber:IsNull()) and bowerstoneSlumsVillagerFemaleBarber ~= nil and bowerstoneSlumsVillagerFemaleBarber:MsgIsKilledBy("") then break end
        quest:NewScriptFrame()
        predicateResult = quest:IsActiveThreadTerminating()
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetStateBool("BarberLadyKilled", true)
end

-- V_BeardyBaldy.WatchForBeardyBaldyDeath (retail 0x00e503d0)
function WatchForBeardyBaldyDeath(quest)
    local beardyBaldyMan = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
    while true do
        if (beardyBaldyMan ~= nil and not beardyBaldyMan:IsNull()) and beardyBaldyMan ~= nil and beardyBaldyMan:MsgIsKilledBy("") then break end
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then
            beardyBaldyMan = nil
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetStateBool("BeardyBaldyKilled", true)
end

-- V_BeardyBaldy.WatchForNewHairdo (retail 0x00e50150)
function WatchForNewHairdo(quest)
    while not quest:GetStateBool("Phase1RequirementsComplete") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    while not quest:IsActiveThreadTerminating() do
        while not quest:MsgOnHeroHairTypeChanged(16) do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then break end
        quest:SetStateBool("AcquiredNewHairdo", true)
        quest:NewScriptFrame()
    end
end

-- V_BeardyBaldy.WatchForNewBeard (retail 0x00e50210)
function WatchForNewBeard(quest)
    while not quest:GetStateBool("Phase2RequirementsComplete") and not quest:GetStateBool("AcquiredNewHairdo") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    while not quest:IsActiveThreadTerminating() do
        while not quest:MsgOnHeroHairTypeChanged(128) do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then break end
        quest:SetStateBool("AcquiredNewBeard", true)
        quest:NewScriptFrame()
    end
end

-- V_BeardyBaldy.WatchForNewTash (retail 0x00e502e0)
function WatchForNewTash(quest)
    while not quest:GetStateBool("Phase3RequirementsComplete") and not quest:GetStateBool("AcquiredNewBeard") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    while not quest:IsActiveThreadTerminating() do
        while not quest:MsgOnHeroHairTypeChanged(256) do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then break end
        quest:SetStateBool("AcquiredNewTash", true)
        quest:SetStateBool("AllHairChanged", true)
        quest:NewScriptFrame()
    end
end

-- V_BeardyBaldy.WatchForAttack (retail 0x00e53d90)
function WatchForAttack(quest, moi)
    local predicateResult
    repeat
        if quest:IsActiveThreadTerminating() then return end
        if not (moi ~= nil and not moi:IsNull()) or not (moi ~= nil and moi:IsAlive()) then
            if quest:IsActiveThreadTerminating() then return end
            return
        end
        if (moi ~= nil and not moi:IsNull()) and moi:MsgIsHitByHero() then goto LAB_00e53e21 end
        if (moi ~= nil and not moi:IsNull()) and (moi ~= nil and moi:MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) then goto LAB_00e53e21 end
        goto LAB_00e53e43
        goto FLOW_past_lab_00e53e43
        ::LAB_00e53e43::
        predicateResult = false
        ::FLOW_past_lab_00e53e43::
        goto FLOW_past_lab_00e53e21
        ::LAB_00e53e21::
        predicateResult = true
        if quest:GetHealth(moi) <= 9.999999747378752e-05 then goto LAB_00e53e43 end
        ::FLOW_past_lab_00e53e21::
        if not predicateResult then
            quest:NewScriptFrame()
        else
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("AttackedByHero", true)
            quest:NewScriptFrame()
        end
    until false
end

-- V_BeardyBaldy.GoTalkToBeardyBaldy (retail 0x00e50650)
function GoTalkToBeardyBaldy(quest)
    local beardyBaldyMan = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
    local pRelativeTo = quest:GetHero()
    local getNearestWithDefName = quest:GetNearestWithDefName(pRelativeTo, "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER")
    if beardyBaldyMan ~= nil and not beardyBaldyMan:IsNull() then
        if beardyBaldyMan ~= nil and beardyBaldyMan:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            quest:MiniMapAddMarker(beardyBaldyMan, "HUD_ORB_QUEST_VIGNETTE")
        end
    end
    if not (getNearestWithDefName ~= nil and not getNearestWithDefName:IsNull()) then getNearestWithDefName = nil; beardyBaldyMan = nil; return end
    if not (getNearestWithDefName ~= nil and getNearestWithDefName:IsAlive()) then getNearestWithDefName = nil; beardyBaldyMan = nil; return end
    if quest:IsActiveThreadTerminating() then return end
    quest:MiniMapRemoveMarker(getNearestWithDefName)
    getNearestWithDefName = nil
    beardyBaldyMan = nil
end

-- V_BeardyBaldy.SetWanderPointAndDistance (retail 0x00e53f60)
function SetWanderPointAndDistance(quest, param1, param2)
    local center = {x = param2.x, y = param2.y, z = param2.z}
    quest:SetWanderCentrePoint(param1, center)
    local readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BeardyBaldyWanderDistanceMin)
    quest:SetWanderMinDistance(param1, readGlobalGameDataFloat)
    readGlobalGameDataFloat = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.BeardyBaldyWanderDistanceMax)
    quest:SetWanderMaxDistance(param1, readGlobalGameDataFloat)
    quest:SetScriptingStateGroup(param1, 4)
end

-- V_BeardyBaldy.IsHeroWearingAnyTash (retail 0x00e538f0)
function IsHeroWearingAnyTash(quest)
    local predicateResult, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    local predicateResult6
    predicateResult6 = true
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMITH_01") then
        local hero2 = quest:GetHero()
        if not quest:IsWearingHairstyle(hero2, "OBJECT_HERO_TASHTRADER_01") then
            local hero3 = quest:GetHero()
            if not quest:IsWearingHairstyle(hero3, "OBJECT_HERO_TASHKHG_01") then
                local hero4 = quest:GetHero()
                if not quest:IsWearingHairstyle(hero4, "OBJECT_HERO_TASHSHERIFF_01") then
                    local hero5 = quest:GetHero()
                    if not quest:IsWearingHairstyle(hero5, "OBJECT_HERO_TASHCHINESE_01") then
                        local hero6 = quest:GetHero()
                        predicateResult6 = not not quest:IsWearingHairstyle(hero6, "OBJECT_HERO_TASHSMALL_01")
                    end
                end
            end
        end
    end
    return predicateResult6
end

-- V_BeardyBaldy.IsHeroWearingAnyOddHairdo (retail 0x00e53ad0)
function IsHeroWearingAnyOddHairdo(quest)
    local predicateResult, predicateResult2, predicateResult3, predicateResult4, predicateResult5
    predicateResult5 = true
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_BUZZ_01") then
        local hero2 = quest:GetHero()
        if not quest:IsWearingHairstyle(hero2, "OBJECT_HERO_HAIR_BASIN_01") then
            local hero3 = quest:GetHero()
            if not quest:IsWearingHairstyle(hero3, "OBJECT_HERO_HAIR_MOHAWK_01") then
                local hero4 = quest:GetHero()
                if not quest:IsWearingHairstyle(hero4, "OBJECT_HERO_HAIR_PONYTAIL_01") then
                    local hero5 = quest:GetHero()
                    predicateResult5 = not not quest:IsWearingHairstyle(hero5, "OBJECT_HERO_HAIR_PLATS_01")
                end
            end
        end
    end
    return predicateResult5
end

-- V_BeardyBaldy.GoTalkToBarber (retail 0x00e50e40)
function GoTalkToBarber(quest)
    local beardyBaldyMan = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
    local pRelativeTo = quest:GetHero()
    local getNearestWithDefName = quest:GetNearestWithDefName(pRelativeTo, "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER")
    if beardyBaldyMan ~= nil and not beardyBaldyMan:IsNull() then
        if beardyBaldyMan ~= nil and beardyBaldyMan:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            quest:MiniMapRemoveMarker(beardyBaldyMan)
        end
    end
    if not (getNearestWithDefName ~= nil and not getNearestWithDefName:IsNull()) then getNearestWithDefName = nil; beardyBaldyMan = nil; return end
    if not (getNearestWithDefName ~= nil and getNearestWithDefName:IsAlive()) then getNearestWithDefName = nil; beardyBaldyMan = nil; return end
    if quest:IsActiveThreadTerminating() then return end
    quest:MiniMapAddMarker(getNearestWithDefName, "HUD_ORB_QUEST_VIGNETTE")
    getNearestWithDefName = nil
    beardyBaldyMan = nil
end

-- V_BeardyBaldy.ResetRandomSpeechTime (retail 0x00e53c70)
-- E53C70: bsim names this body NScript::CV_AmbushScamScript::ResetRandomSpeechTime (a homologous script member); no PDB name
function ResetRandomSpeechTime(quest)
    local randomChoice = quest:RetailRandModulo(32768)
    quest:SetTimer(quest:GetStateInt("RandomSpeechTimer"), quest:ReadGlobalGameData(SCRIPT_DEF.BeardyBaldyRandomSpeechTimerMax) - randomChoice % quest:ReadGlobalGameData(SCRIPT_DEF.BeardyBaldyRandomSpeechTimerVar))
end

