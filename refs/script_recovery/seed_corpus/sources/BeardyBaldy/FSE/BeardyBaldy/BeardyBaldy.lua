-- BeardyBaldy.lua
-- Main quest script for Beardy Baldy (V_BeardyBaldy)
-- Functional parity port of NScript::CV_BeardyBaldyScript

Quest = nil

local QUEST_NAME = "BeardyBaldy"
local REGION_NAME = "BowerstoneSlumsWarehouses"
local BARBER_DEF_NAME = "CREATURE_BOWERSTONE_SLUMS_VILLAGER_FEMALE_BARBER"
local BEARDY_BALDY_SCRIPT_NAME = "BB_BeardyBaldyMan"

-- Constants from main.def
local DEF = {
    WANDER_MIN                  = 0.0,
    WANDER_MAX                  = 50.0,
    HAIRCUT_STYLE_1             = "OBJECT_HERO_HAIR_BASIN_01",
    HAIRCUT_CARD_1              = "OBJECT_HAIRCUT_CARD_MOP",
    HAIRCUT_STYLE_2             = "OBJECT_HERO_HAIR_MOHAWK_01",
    HAIRCUT_CARD_2              = "OBJECT_HAIRCUT_CARD_07",
    BEARD_STYLE_1               = "OBJECT_HERO_BEARD_MUTTON_01",
    BEARD_CARD_1                = "OBJECT_BEARD_CARD_04",
    BEARD_STYLE_2               = "OBJECT_HERO_BEARD_LONG_01",
    BEARD_CARD_2                = "OBJECT_BEARD_CARD_05",
    TASH_STYLE_1                = "OBJECT_HERO_TASHTRADER_01",
    TASH_CARD_1                 = "OBJECT_TASH_CARD_02",
    TASH_STYLE_2                = "OBJECT_HERO_TASHSHERIFF_01",
    TASH_CARD_2                 = "OBJECT_TASH_CARD_04",
    REWARD_FULL                 = "OBJECT_TATTOO_CARD_BS_BACK_01",
    REWARD_HALF                 = "OBJECT_BEARD_CARD_08",
    EXPERIENCE                  = 100,
    RENOWN                      = 0,
    MORALITY                    = 0.0,
    RANDOM_SPEECH_TIMER_MAX     = 30,
    RANDOM_SPEECH_TIMER_VAR     = 10,
}

-- Odd hairdos list matching IsHeroWearingAnyOddHairdo
local ODD_HAIRDOS = {
    "OBJECT_HERO_HAIR_BUZZ_01",
    "OBJECT_HERO_HAIR_BASIN_01",
    "OBJECT_HERO_HAIR_MOHAWK_01",
    "OBJECT_HERO_HAIR_PONYTAIL_01",
    "OBJECT_HERO_HAIR_PLATS_01",
}

-- Tash styles list matching IsHeroWearingAnyTash
local TASH_STYLES = {
    "OBJECT_HERO_TASHSMITH_01",
    "OBJECT_HERO_TASHTRADER_01",
    "OBJECT_HERO_TASHKHG_01",
    "OBJECT_HERO_TASHSHERIFF_01",
    "OBJECT_HERO_TASHCHINESE_01",
    "OBJECT_HERO_TASHSMALL_01",
}

-- Beard styles list matching CV_BordelloScript::IsHeroWearingBeard
local BEARD_STYLES = {
    "OBJECT_HERO_BEARD_01",
    "OBJECT_HERO_BEARD_02",
    "OBJECT_HERO_BEARD_03",
    "OBJECT_HERO_BEARD_MUTTON_01",
    "OBJECT_HERO_BEARD_LONG_01",
    "OBJECT_HERO_BEARD_CHIN_01",
    "OBJECT_HERO_BEARD_TRAMP_01",
    "OBJECT_HERO_BEARD_WATSON_01",
}

-- Area codes for MsgOnHeroHairTypeChanged
local COVERS_BODY_AREA_HEAD      = 0x10
local COVERS_BODY_AREA_FACE      = 0x80
local COVERS_BODY_AREA_MOUSTACHE = 0x100

-- Helper: Check if hero is wearing any odd hairdo
function IsHeroWearingAnyOddHairdo(quest, hero)
    if not hero then return false end
    for _, style in ipairs(ODD_HAIRDOS) do
        if quest:IsWearingHairstyle(hero, style) then
            return true
        end
    end
    return false
end

-- Helper: Check if hero is wearing any moustache
function IsHeroWearingAnyTash(quest, hero)
    if not hero then return false end
    for _, style in ipairs(TASH_STYLES) do
        if quest:IsWearingHairstyle(hero, style) then
            return true
        end
    end
    return false
end

-- Helper: Check if hero is wearing any beard (matching CV_BordelloScript::IsHeroWearingBeard)
function IsHeroWearingBeard(quest, hero)
    if not hero then return false end
    for _, style in ipairs(BEARD_STYLES) do
        if quest:IsWearingHairstyle(hero, style) then
            return true
        end
    end
    return false
end

-- Helper: Move minimap marker to Barber Lady
function GoTalkToBarber(quest)
    quest:Log("BeardyBaldy: GoTalkToBarber() called.")
    local beardyBaldy = quest:GetThingWithScriptName(BEARDY_BALDY_SCRIPT_NAME)
    if beardyBaldy and beardyBaldy:IsAlive() then
        quest:MiniMapRemoveMarker(beardyBaldy)
    end

    local hero = quest:GetHero()
    if hero and quest:IsRegionLoaded("BowerstoneSlums") then
        local barber = quest:GetNearestWithDefName(hero, BARBER_DEF_NAME)
        if barber and barber:IsAlive() then
            quest:MiniMapAddMarker(barber, "HUD_ORB_QUEST_VIGNETTE")
        end
    end
    quest:Log("BeardyBaldy: GoTalkToBarber() finished.")
end

-- Helper: Move minimap marker to Beardy Baldy
function GoTalkToBeardyBaldy(quest)
    quest:Log("BeardyBaldy: GoTalkToBeardyBaldy() called.")
    local beardyBaldy = quest:GetThingWithScriptName(BEARDY_BALDY_SCRIPT_NAME)
    if beardyBaldy and beardyBaldy:IsAlive() then
        quest:MiniMapAddMarker(beardyBaldy, "HUD_ORB_QUEST_VIGNETTE")
    end

    local hero = quest:GetHero()
    if hero and quest:IsRegionLoaded("BowerstoneSlums") then
        local barber = quest:GetNearestWithDefName(hero, BARBER_DEF_NAME)
        if barber and barber:IsAlive() then
            quest:MiniMapRemoveMarker(barber)
        end
    end
    quest:Log("BeardyBaldy: GoTalkToBeardyBaldy() finished.")
end

function Init(questObject)
    Quest = questObject
    Quest:Log("BeardyBaldy: Init() started.")

    Quest:SetStateBool("QuestFinished", false)
    Quest:SetStateBool("QuestActivated", false)
    Quest:SetStateBool("QuestComplete", false)
    Quest:SetStateInt("QuestPhase", 0)
    Quest:SetStateBool("InitialisePhase", true)
    Quest:SetStateBool("IntroComplete", false)
    Quest:SetStateBool("Phase1RequirementsComplete", false)
    Quest:SetStateBool("Phase2RequirementsComplete", false)
    Quest:SetStateBool("Phase3RequirementsComplete", false)
    Quest:SetStateString("RequiredHairdo", "")
    Quest:SetStateString("RequiredBeard", "")
    Quest:SetStateString("RequiredTash", "")
    Quest:SetStateBool("BeardyBaldyKilled", false)
    Quest:SetStateBool("BarberLadyKilled", false)
    Quest:SetStateBool("BeardyBaldyLeft", false)
    Quest:SetStateBool("AcquiredNewHairdo", false)
    Quest:SetStateBool("AcquiredNewBeard", false)
    Quest:SetStateBool("AcquiredNewTash", false)
    Quest:SetStateBool("AllHairChanged", false)
    Quest:SetStateInt("IncorrectHairComboCount", 0)
    Quest:SetStateBool("AttackedByHero", false)

    Quest:Log("BeardyBaldy: Init() completed.")
end

function OnPersist(questObject, context)
    Quest = questObject

    local questFinished = Quest:GetStateBool("QuestFinished") or false
    questFinished = Quest:PersistTransferBool(context, "QuestFinished", questFinished)
    Quest:SetStateBool("QuestFinished", questFinished)

    local questActivated = Quest:GetStateBool("QuestActivated") or false
    questActivated = Quest:PersistTransferBool(context, "QuestActivated", questActivated)
    Quest:SetStateBool("QuestActivated", questActivated)

    local questComplete = Quest:GetStateBool("QuestComplete") or false
    questComplete = Quest:PersistTransferBool(context, "QuestComplete", questComplete)
    Quest:SetStateBool("QuestComplete", questComplete)

    local questPhase = Quest:GetStateInt("QuestPhase") or 0
    questPhase = Quest:PersistTransferInt(context, "QuestPhase", questPhase)
    Quest:SetStateInt("QuestPhase", questPhase)

    local initialisePhase = Quest:GetStateBool("InitialisePhase") or false
    initialisePhase = Quest:PersistTransferBool(context, "InitialisePhase", initialisePhase)
    Quest:SetStateBool("InitialisePhase", initialisePhase)

    local introComplete = Quest:GetStateBool("IntroComplete") or false
    introComplete = Quest:PersistTransferBool(context, "IntroComplete", introComplete)
    Quest:SetStateBool("IntroComplete", introComplete)

    local phase1Req = Quest:GetStateBool("Phase1RequirementsComplete") or false
    phase1Req = Quest:PersistTransferBool(context, "Phase1RequirementsComplete", phase1Req)
    Quest:SetStateBool("Phase1RequirementsComplete", phase1Req)

    local phase2Req = Quest:GetStateBool("Phase2RequirementsComplete") or false
    phase2Req = Quest:PersistTransferBool(context, "Phase2RequirementsComplete", phase2Req)
    Quest:SetStateBool("Phase2RequirementsComplete", phase2Req)

    local phase3Req = Quest:GetStateBool("Phase3RequirementsComplete") or false
    phase3Req = Quest:PersistTransferBool(context, "Phase3RequirementsComplete", phase3Req)
    Quest:SetStateBool("Phase3RequirementsComplete", phase3Req)

    local reqHairdo = Quest:GetStateString("RequiredHairdo") or ""
    reqHairdo = Quest:PersistTransferString(context, "RequiredHairdo", reqHairdo)
    Quest:SetStateString("RequiredHairdo", reqHairdo)

    local reqBeard = Quest:GetStateString("RequiredBeard") or ""
    reqBeard = Quest:PersistTransferString(context, "RequiredBeard", reqBeard)
    Quest:SetStateString("RequiredBeard", reqBeard)

    local reqTash = Quest:GetStateString("RequiredTash") or ""
    reqTash = Quest:PersistTransferString(context, "RequiredTash", reqTash)
    Quest:SetStateString("RequiredTash", reqTash)

    local bbKilled = Quest:GetStateBool("BeardyBaldyKilled") or false
    bbKilled = Quest:PersistTransferBool(context, "BeardyBaldyKilled", bbKilled)
    Quest:SetStateBool("BeardyBaldyKilled", bbKilled)

    local barberKilled = Quest:GetStateBool("BarberLadyKilled") or false
    barberKilled = Quest:PersistTransferBool(context, "BarberLadyKilled", barberKilled)
    Quest:SetStateBool("BarberLadyKilled", barberKilled)

    local acqHair = Quest:GetStateBool("AcquiredNewHairdo") or false
    acqHair = Quest:PersistTransferBool(context, "AcquiredNewHairdo", acqHair)
    Quest:SetStateBool("AcquiredNewHairdo", acqHair)

    local acqBeard = Quest:GetStateBool("AcquiredNewBeard") or false
    acqBeard = Quest:PersistTransferBool(context, "AcquiredNewBeard", acqBeard)
    Quest:SetStateBool("AcquiredNewBeard", acqBeard)

    local acqTash = Quest:GetStateBool("AcquiredNewTash") or false
    acqTash = Quest:PersistTransferBool(context, "AcquiredNewTash", acqTash)
    Quest:SetStateBool("AcquiredNewTash", acqTash)

    local allHair = Quest:GetStateBool("AllHairChanged") or false
    allHair = Quest:PersistTransferBool(context, "AllHairChanged", allHair)
    Quest:SetStateBool("AllHairChanged", allHair)

    local incorrectCount = Quest:GetStateInt("IncorrectHairComboCount") or 0
    incorrectCount = Quest:PersistTransferInt(context, "IncorrectHairComboCount", incorrectCount)
    Quest:SetStateInt("IncorrectHairComboCount", incorrectCount)

    local attacked = Quest:GetStateBool("AttackedByHero") or false
    attacked = Quest:PersistTransferBool(context, "AttackedByHero", attacked)
    Quest:SetStateBool("AttackedByHero", attacked)
end

-- Thread 1: Watch for quest finish condition
function WatchForQuestFinished(quest)
    quest:Log("BeardyBaldy: Thread 1 (WatchForQuestFinished) started.")
    while not quest:GetStateBool("BeardyBaldyKilled")
      and not quest:GetStateBool("BarberLadyKilled")
      and not quest:GetStateBool("BeardyBaldyLeft") do
        if not quest:NewScriptFrame() then return end
    end
    quest:SetStateBool("QuestFinished", true)
    quest:Log("BeardyBaldy: WatchForQuestFinished - quest marked finished.")
end

-- Thread 2: Watch for quest card conditions and marker switching
function WatchForQuestCardConditions(quest)
    quest:Log("BeardyBaldy: Thread 2 (WatchForQuestCardConditions) started.")
    -- Wait until Phase 1 requirements are complete
    while not quest:GetStateBool("Phase1RequirementsComplete") do
        if not quest:NewScriptFrame() then return end
    end

    local activeQuestName = quest:GetActiveQuestName()
    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_BEARDY_BALDY", activeQuestName, true)
    quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "")
    GoTalkToBarber(quest)

    while true do
        -- Wait until Hero acquired new hairdo, beard, or tash
        while not quest:GetStateBool("AcquiredNewHairdo")
          and not quest:GetStateBool("AcquiredNewBeard")
          and not quest:GetStateBool("AcquiredNewTash") do
            if not quest:NewScriptFrame() then return end
        end

        activeQuestName = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_03", "", "")
        GoTalkToBeardyBaldy(quest)

        quest:SetStateBool("AcquiredNewHairdo", false)
        quest:SetStateBool("AcquiredNewBeard", false)
        quest:SetStateBool("AcquiredNewTash", false)

        -- Wait for region loaded
        while not quest:IsRegionLoaded(REGION_NAME) do
            if not quest:NewScriptFrame() then return end
        end

        local beardyBaldy = quest:GetThingWithScriptName(BEARDY_BALDY_SCRIPT_NAME)
        if beardyBaldy and beardyBaldy:IsAlive() then
            -- Wait until talked to by Hero
            while not beardyBaldy:IsTalkedToByHero() do
                if not quest:NewScriptFrame() then return end
            end

            if not quest:GetStateBool("Phase3RequirementsComplete") then
                activeQuestName = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_BEARDY_BALDY_OBJECTIVE_02", "", "")
            end
        end

        GoTalkToBarber(quest)
    end
end

-- Thread 3: Watch for barber lady death
function WatchForBarberLadyDeath(quest)
    quest:Log("BeardyBaldy: Thread 3 (WatchForBarberLadyDeath) started.")
    local hero = quest:GetHero()
    if not hero then return end

    local barber = nil
    while not barber or not barber:IsAlive() do
        if quest:IsRegionLoaded("BowerstoneSlums") then
            barber = quest:GetNearestWithDefName(hero, BARBER_DEF_NAME)
        end
        if not quest:NewScriptFrame() then return end
    end

    while not barber:MsgIsKilledBy("") do
        if not quest:NewScriptFrame() then return end
    end

    quest:SetStateBool("BarberLadyKilled", true)
    quest:Log("BeardyBaldy: BarberLady killed!")
end

-- Thread 4: Watch for Beardy Baldy death
function WatchForBeardyBaldyDeath(quest)
    quest:Log("BeardyBaldy: Thread 4 (WatchForBeardyBaldyDeath) started.")
    local beardyBaldy = quest:GetThingWithScriptName(BEARDY_BALDY_SCRIPT_NAME)
    while not beardyBaldy or not beardyBaldy:IsAlive() do
        if not quest:NewScriptFrame() then return end
        beardyBaldy = quest:GetThingWithScriptName(BEARDY_BALDY_SCRIPT_NAME)
    end

    while not beardyBaldy:MsgIsKilledBy("") do
        if not quest:NewScriptFrame() then return end
    end

    quest:SetStateBool("BeardyBaldyKilled", true)
    quest:Log("BeardyBaldy: BeardyBaldy killed!")
end

-- Thread 5: Watch for new hairdo change
function WatchForNewHairdo(quest)
    quest:Log("BeardyBaldy: Thread 5 (WatchForNewHairdo) started.")
    while not quest:GetStateBool("Phase1RequirementsComplete") do
        if not quest:NewScriptFrame() then return end
    end

    while true do
        if quest:MsgOnHeroHairTypeChanged(COVERS_BODY_AREA_HEAD) then
            quest:SetStateBool("AcquiredNewHairdo", true)
            quest:Log("BeardyBaldy: AcquiredNewHairdo event fired.")
        end
        if not quest:NewScriptFrame() then return end
    end
end

-- Thread 6: Watch for new beard change
function WatchForNewBeard(quest)
    quest:Log("BeardyBaldy: Thread 6 (WatchForNewBeard) started.")
    while not quest:GetStateBool("Phase2RequirementsComplete") and not quest:GetStateBool("AcquiredNewHairdo") do
        if not quest:NewScriptFrame() then return end
    end

    while true do
        if quest:MsgOnHeroHairTypeChanged(COVERS_BODY_AREA_FACE) then
            quest:SetStateBool("AcquiredNewBeard", true)
            quest:Log("BeardyBaldy: AcquiredNewBeard event fired.")
        end
        if not quest:NewScriptFrame() then return end
    end
end

-- Thread 7: Watch for new moustache change
function WatchForNewTash(quest)
    quest:Log("BeardyBaldy: Thread 7 (WatchForNewTash) started.")
    while not quest:GetStateBool("Phase3RequirementsComplete") and not quest:GetStateBool("AcquiredNewBeard") do
        if not quest:NewScriptFrame() then return end
    end

    while true do
        if quest:MsgOnHeroHairTypeChanged(COVERS_BODY_AREA_MOUSTACHE) then
            quest:SetStateBool("AcquiredNewTash", true)
            quest:SetStateBool("AllHairChanged", true)
            quest:Log("BeardyBaldy: AcquiredNewTash and AllHairChanged fired.")
        end
        if not quest:NewScriptFrame() then return end
    end
end

function Main(questObject)
    Quest = questObject
    Quest:Log("BeardyBaldy: Main() started.")

    -- Entity binding
    Quest:AddEntityBinding(BEARDY_BALDY_SCRIPT_NAME, "BeardyBaldy/Entities/BB_BeardyBaldyMan")
    Quest:FinalizeEntityBindings()

    -- Wait for region loaded
    while not Quest:IsRegionLoaded(REGION_NAME) do
        if not Quest:NewScriptFrame() then return end
    end

    Quest:Log("BeardyBaldy: Main() region loaded. Creating threads...")
    -- Spawn watcher threads
    Quest:CreateThread("WatchForQuestFinished")
    Quest:CreateThread("WatchForQuestCardConditions")
    Quest:CreateThread("WatchForBarberLadyDeath")
    Quest:CreateThread("WatchForBeardyBaldyDeath")
    Quest:CreateThread("WatchForNewHairdo")
    Quest:CreateThread("WatchForNewBeard")
    Quest:CreateThread("WatchForNewTash")
    Quest:Log("BeardyBaldy: All 7 threads created.")

    if not Quest:GetStateBool("QuestActivated") then
        Quest:SetStateBool("QuestActivated", true)
    end

    Quest:Log("BeardyBaldy: Main calling GoTalkToBeardyBaldy...")
    GoTalkToBeardyBaldy(Quest)
    Quest:Log("BeardyBaldy: Main entering wait loop...")

    -- Main wait loop: wait until finished OR (complete and player leaves region)
    while true do
        local questFinished = Quest:GetStateBool("QuestFinished")
        local questComplete = Quest:GetStateBool("QuestComplete")
        local inRegion = Quest:IsRegionLoaded(REGION_NAME)

        if questFinished or (questComplete and not inRegion) then
            break
        end

        if not Quest:NewScriptFrame() then return end
    end

    Quest:Log("BeardyBaldy: Quest finishing sequence reached. Performing cleanup...")

    -- Clean up markers and information on Barber and Beardy Baldy
    local hero = Quest:GetHero()
    if hero then
        local barber = Quest:GetNearestWithDefName(hero, BARBER_DEF_NAME)
        if barber and barber:IsAlive() then
            Quest:MiniMapRemoveMarker(barber)
            Quest:ClearThingHasInformation(barber)
        end
    end

    local beardyBaldy = Quest:GetThingWithScriptName(BEARDY_BALDY_SCRIPT_NAME)
    if beardyBaldy and beardyBaldy:IsAlive() then
        Quest:MiniMapRemoveMarker(beardyBaldy)
        Quest:ClearThingHasInformation(beardyBaldy)
    end

    -- Quest completion and card removal
    local activeQuestName = Quest:GetActiveQuestName()
    Quest:SetQuestAsCompleted(activeQuestName, false, false, false)
    Quest:RemoveQuestCardFromHero(activeQuestName)
    Quest:DeactivateQuestLater(activeQuestName, 0)

    Quest:Log("BeardyBaldy: Quest completed and deactivated.")
end
