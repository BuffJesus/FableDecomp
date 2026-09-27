-- Readable native conversion: V_BeardyBaldy. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- V_BeardyBaldy.Main (retail 0x00e4fa20)
function Main(quest)
    local predicateResult, msgOnRegionLoaded
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
    if quest:GetStateBool("QuestFinished") then goto LAB_00e4ff2f end
    if quest:GetStateBool("QuestComplete") then
        if not quest:IsRegionLoaded("BowerstoneSlumsWarehouses") then goto LAB_00e4ff2f end
    end
    predicateResult = true
    goto FLOW_past_lab_00e4ff2f
    ::LAB_00e4ff2f::
    predicateResult = false
    ::FLOW_past_lab_00e4ff2f::
    if not predicateResult then
        if not quest:IsActiveThreadTerminating() then
            local beardyBaldyMan = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
            local bowerstoneSlumsVillagerFemaleBarber = quest:GetNearestWithScriptName(quest:GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER")
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
                if quest:IsActiveThreadTerminating() then goto LAB_00e500d9 end
                quest:MiniMapRemoveMarker(beardyBaldyMan)
                quest:ClearThingHasInformation(beardyBaldyMan)
            end
            quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
            quest:RemoveQuestCardFromHero(quest:GetActiveQuestName())
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            ::FLOW_past_lab_00e5002c::
            ::LAB_00e500d9::
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
    quest:SetStateBool("QuestFinished", quest:PersistTransferBool(context, "QuestFinished", quest:GetStateBool("QuestFinished")))
    quest:SetStateBool("QuestActivated", quest:PersistTransferBool(context, "QuestActivated", quest:GetStateBool("QuestActivated")))
    quest:SetStateBool("QuestComplete", quest:PersistTransferBool(context, "QuestComplete", quest:GetStateBool("QuestComplete")))
    quest:SetStateInt("QuestPhase", quest:PersistTransferInt(context, "QuestPhase", quest:GetStateInt("QuestPhase") or 0))
    quest:SetStateBool("InitialisePhase", quest:PersistTransferBool(context, "InitialisePhase", quest:GetStateBool("InitialisePhase")))
    quest:SetStateBool("IntroComplete", quest:PersistTransferBool(context, "IntroComplete", quest:GetStateBool("IntroComplete")))
    quest:SetStateBool("Phase1RequirementsComplete", quest:PersistTransferBool(context, "Phase1RequirementsComplete", quest:GetStateBool("Phase1RequirementsComplete")))
    quest:SetStateBool("Phase2RequirementsComplete", quest:PersistTransferBool(context, "Phase2RequirementsComplete", quest:GetStateBool("Phase2RequirementsComplete")))
    quest:SetStateBool("Phase3RequirementsComplete", quest:PersistTransferBool(context, "Phase3RequirementsComplete", quest:GetStateBool("Phase3RequirementsComplete")))
    quest:SetStateBool("BeardyBaldyKilled", quest:PersistTransferBool(context, "BeardyBaldyKilled", quest:GetStateBool("BeardyBaldyKilled")))
    quest:SetStateBool("BarberLadyKilled", quest:PersistTransferBool(context, "BarberLadyKilled", quest:GetStateBool("BarberLadyKilled")))
    quest:SetStateBool("AcquiredNewHairdo", quest:PersistTransferBool(context, "AcquiredNewHairdo", quest:GetStateBool("AcquiredNewHairdo")))
    quest:SetStateBool("AcquiredNewBeard", quest:PersistTransferBool(context, "AcquiredNewBeard", quest:GetStateBool("AcquiredNewBeard")))
    quest:SetStateBool("AcquiredNewTash", quest:PersistTransferBool(context, "AcquiredNewTash", quest:GetStateBool("AcquiredNewTash")))
    quest:SetStateBool("AllHairChanged", quest:PersistTransferBool(context, "AllHairChanged", quest:GetStateBool("AllHairChanged")))
    quest:SetStateInt("IncorrectHairComboCount", quest:PersistTransferInt(context, "IncorrectHairComboCount", quest:GetStateInt("IncorrectHairComboCount") or 0))
    quest:SetStateBool("AttackedByHero", quest:PersistTransferBool(context, "AttackedByHero", quest:GetStateBool("AttackedByHero")))
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
    local scratchValue3, msgOnRegionLoaded
    local function ReleaseEverything()
        local scratchValue3 = nil
    end
    while not quest:GetStateBool("Phase1RequirementsComplete") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BEARDY_BALDY", quest:GetActiveQuestName(), true)
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "")
    GoTalkToBeardyBaldy(quest)
    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
    ::LAB_00e50ab0::
    while (not quest:GetStateBool("AcquiredNewHairdo") and (not quest:GetStateBool("AcquiredNewBeard"))) and not quest:GetStateBool("AcquiredNewTash") do
        if not quest:NewScriptFrame() then return end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_03", "", "")
        GoTalkToBeardyBaldy(quest)
        quest:SetStateBool("AcquiredNewHairdo", false)
        quest:SetStateBool("AcquiredNewBeard", false)
        quest:SetStateBool("AcquiredNewTash", false)
        repeat
            if quest:IsActiveThreadTerminating() then return end
            msgOnRegionLoaded = quest:MsgOnRegionLoaded()
            while msgOnRegionLoaded == nil do
                if not quest:NewScriptFrame() then return end
                msgOnRegionLoaded = quest:MsgOnRegionLoaded()
            end
            if msgOnRegionLoaded == "BowerstoneSlumsWarehouses" then goto LAB_00e50bfe end
            quest:NewScriptFrame()
        until false
    end
    do return end
    ::LAB_00e50bfe::
    scratchValue3 = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
    if scratchValue3 == nil or not (scratchValue3 ~= nil and scratchValue3:IsAlive()) then goto LAB_00e50db6 end
    if not quest:IsActiveThreadTerminating() then
        while true do
            if (scratchValue3 ~= nil and not scratchValue3:IsNull()) and scratchValue3:IsTalkedToByHero() then break end
            if not quest:NewScriptFrame() then return end
        end
        if not quest:IsActiveThreadTerminating() then
            if not quest:GetStateBool("Phase3RequirementsComplete") then
                quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "")
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
    local bowerstoneSlumsVillagerFemaleBarber = quest:GetNearestWithScriptName(quest:GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER")
    while true do
        local predicateResult4 = not (bowerstoneSlumsVillagerFemaleBarber ~= nil and not bowerstoneSlumsVillagerFemaleBarber:IsNull()) or not (bowerstoneSlumsVillagerFemaleBarber ~= nil and bowerstoneSlumsVillagerFemaleBarber:IsAlive())
        if not predicateResult4 then break end
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    predicateResult = false
    while true do
        if predicateResult then
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
        if not quest:NewScriptFrame() then return end
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
        quest:SetStateBool("AcquiredNewTash", true)
        quest:SetStateBool("AllHairChanged", true)
        quest:NewScriptFrame()
    end
end

-- V_BeardyBaldy.WatchForAttack (retail 0x00e53d90)
function WatchForAttack(quest, moi)
    local scratchValue, scratchValue2, scratchValue3, predicateResult, sequence
    repeat
        if quest:IsActiveThreadTerminating() then goto LAB_00e53ea8 end
        goto FLOW_past_lab_00e53ea8
        ::LAB_00e53ea8::
        moi = nil
        scratchValue = moi._8_4_ ~= 0
        if scratchValue then
            -- TODO(native): *native_arg_moi._8_4_ = *native_arg_moi._8_4_ - 1;
            -- TODO(native): __native_condition_1 = *native_arg_moi._8_4_ == 0
            scratchValue = nil --[[unresolved native value]]
        end
        if scratchValue then
            -- TODO(native): (**(code **)(native_arg_moi._8_4_ + 4))();
        end
        ::FLOW_after_lab_00e53ec1::
        ::LAB_00e53ec9::
        do return end
        ::FLOW_past_lab_00e53ea8::
        if not (moi ~= nil and not moi:IsNull()) or not (moi ~= nil and moi:IsAlive()) then
            if quest:IsActiveThreadTerminating() then
                moi = nil
                scratchValue2 = moi._8_4_ == 0
                if not scratchValue2 then
                    -- TODO(native): *native_arg_moi._8_4_ = *native_arg_moi._8_4_ - 1;
                    -- TODO(native): __native_condition_3 = *native_arg_moi._8_4_ ~= 0
                    scratchValue2 = nil --[[unresolved native value]]
                end
                if scratchValue2 then goto LAB_00e53ec9 end
                -- TODO(native): (**(code **)(native_arg_moi._8_4_ + 4))();
                goto FLOW_after_lab_00e53ec1
            end
            moi = nil
            if quest:IsActiveThreadTerminating() then
                scratchValue3 = moi._8_4_ == 0
                if not scratchValue3 then
                    -- TODO(native): *native_arg_moi._8_4_ = *native_arg_moi._8_4_ - 1;
                    -- TODO(native): __native_condition_4 = *native_arg_moi._8_4_ ~= 0
                    scratchValue3 = nil --[[unresolved native value]]
                end
                if scratchValue3 then goto LAB_00e53ec9 end
                -- TODO(native): (**(code **)(native_arg_moi._8_4_ + 4))();
                goto FLOW_after_lab_00e53ec1
            end
            goto LAB_00e53ea8
        end
        if (moi ~= nil and not moi:IsNull()) and moi:MsgIsHitByHero() then goto LAB_00e53e21 end
        sequence = moi ~= nil and not moi:IsNull()
        if sequence then
            -- TODO(native): MsgIsHitByAnyAggressiveSpecialAbilityFrom is not a ForgeFSE binding
            sequence = moi ~= nil and moi:MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")
        end
        if sequence then goto LAB_00e53e21 end
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
    local getNearestWithDefName = quest:GetNearestWithDefName(quest:GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER")
    if beardyBaldyMan ~= nil and not beardyBaldyMan:IsNull() then
        if beardyBaldyMan ~= nil and beardyBaldyMan:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            quest:MiniMapAddMarker(beardyBaldyMan, "HUD_ORB_QUEST_VIGNETTE")
        end
    end
    if not (getNearestWithDefName ~= nil and not getNearestWithDefName:IsNull()) then return end
    if not (getNearestWithDefName ~= nil and getNearestWithDefName:IsAlive()) then return end
    if quest:IsActiveThreadTerminating() then return end
    quest:MiniMapRemoveMarker(getNearestWithDefName)
end

-- V_BeardyBaldy.IsHeroWearingAnyTash (retail 0x00e538f0)
function IsHeroWearingAnyTash(quest)
    local predicateResult
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMITH_01") then
        if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHTRADER_01") then
            if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHKHG_01") then
                if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSHERIFF_01") then
                    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHCHINESE_01") then
                        predicateResult = false
                        if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_TASHSMALL_01") then goto LAB_00e53a60 end
                    end
                end
            end
        end
    end
    predicateResult = true
    ::LAB_00e53a60::
    return predicateResult
end

-- V_BeardyBaldy.IsHeroWearingAnyOddHairdo (retail 0x00e53ad0)
function IsHeroWearingAnyOddHairdo(quest)
    local predicateResult
    local hero = quest:GetHero()
    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_BUZZ_01") then
        if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_BASIN_01") then
            if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_MOHAWK_01") then
                if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_PONYTAIL_01") then
                    predicateResult = false
                    if not quest:IsWearingHairstyle(hero, "OBJECT_HERO_HAIR_PLATS_01") then goto LAB_00e53c05 end
                end
            end
        end
    end
    predicateResult = true
    ::LAB_00e53c05::
    return predicateResult
end

-- V_BeardyBaldy.GoTalkToBarber (retail 0x00e50e40)
function GoTalkToBarber(quest)
    local beardyBaldyMan = quest:GetThingWithScriptName("BB_BeardyBaldyMan")
    local getNearestWithDefName = quest:GetNearestWithDefName(quest:GetHero(), "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER")
    if beardyBaldyMan ~= nil and not beardyBaldyMan:IsNull() then
        if beardyBaldyMan ~= nil and beardyBaldyMan:IsAlive() then
            if quest:IsActiveThreadTerminating() then return end
            quest:MiniMapRemoveMarker(beardyBaldyMan)
        end
    end
    if not (getNearestWithDefName ~= nil and not getNearestWithDefName:IsNull()) then return end
    if not (getNearestWithDefName ~= nil and getNearestWithDefName:IsAlive()) then return end
    if quest:IsActiveThreadTerminating() then return end
    quest:MiniMapAddMarker(getNearestWithDefName, "HUD_ORB_QUEST_VIGNETTE")
end

-- V_BeardyBaldy.ResetRandomSpeechTime (retail 0x00e53c70)
-- E53C70: bsim names this body NScript::CV_AmbushScamScript::ResetRandomSpeechTime (a homologous script member); no PDB name
function ResetRandomSpeechTime(quest)
    local scratchValue = math.random(0, 32767)
    quest:SetTimer(quest:GetStateInt("RandomSpeechTimer"), quest:ReadGlobalGameData(1172) - scratchValue % quest:ReadGlobalGameData(1176))
end

