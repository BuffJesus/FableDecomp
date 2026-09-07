Quest = nil

local EGP_INTRO                         = 0
local EGP_TRAINING                      = 100
local EGP_WAITING_FOR_WASP_BOSS         = 150
local EGP_WASP_BOSS                     = 200
local EGP_MAZE_MEETING                  = 300
local EGP_WAITING_FOR_ORCHARD_FARM      = 400
local EGP_ORCHARD_FARM                  = 450
local EGP_WAITING_FOR_TRADER_ESCORT     = 500
local EGP_TRADER_ESCORT                 = 550
local EGP_SECOND_MAZE_MEETING           = 600
local EGP_BANDIT_CAMP                   = 700
local EGP_MAZE_TELEPORT_TO_WW           = 800
local EGP_TROPHY_DEALER                 = 850
local EGP_WITCHWOOD_POST_TROPHY_DEALER  = 856
local EGP_WHITE_BALVERINE_WITCHWOOD     = 870
local EGP_WITCHWOOD_WAITING_FOR_ARENA   = 875
local EGP_ARENA                         = 900
local EGP_MEET_SISTER                   = 1000
local EGP_WAITING_FOR_MCC               = 1050
local EGP_MINION_CLIFFTOP_CHASE         = 1100
local EGP_GRAVEYARD                     = 1200
local EGP_PRISON                        = 1250
local EGP_HOOK_COAST                    = 1300
local EGP_WAITING_FOR_WIZARD_BATTLE     = 1450
local EGP_WIZARD_BATTLE                 = 1500
local EGP_WAITING_FOR_FOCAL_SITES       = 1550
local EGP_FOCAL_SITES                   = 1600
local EGP_JACK_BOSS_FIGHT               = 1700
local EGP_FIRE_HEART                    = 1900
local EGP_THE_SHIP                      = 2100
local EGP_THE_ORACLE                    = 2300
local EGP_SCYTHE_MEETING                = 2400
local EGP_HERO_SOULS                    = 2500
local EGP_DRAGON_FIGHT                  = 2600
local EGP_FINISHED_GAME                 = 2800


local TUTORIAL_CATEGORY_QUEST  = 27  -- 0x1B
local TUTORIAL_CATEGORY_RENOWN = 29  -- 0x1D

local GOSSIP_CATEGORIES = {
    "OV_INTRO",
    "GUILD_TRAINING",
    "WASP_BOSS",
    "DOING_WASP_BOSS",
    "VISIT_MAZE_1_GLOBAL",
    "VISIT_MAZE_1_BSSLUMS",
    "PRE_ORCH_FARM",
    "DOING_ORCH_FARM",
    "TRADER_ESCORT_GLOBAL",
    "TRADER_ESCORT_BSSLUMS",
    "VISIT_MAZE_2_GLOBAL",
    "VISIT_MAZE_2_BSSLUMS",
    "DOING_BANDIT_CAMP_GLOBAL",
    "DOING_BANDIT_CAMP_BANDITCAMP",
    "VISIT_MAZE_3_GLOBAL",
    "VISIT_MAZE_3_GUILD",
    "FIND_ARCHAEOLOGIST",
    "PRE_WHITE_BALV_GLOBAL",
    "PRE_WHITE_BALV_WITCHWOOD",
    "DOING_WHITE_BALV_GLOBAL",
    "DOING_WHITE_BALV_KHG",
    "PRE_ARENA_GLOBAL",
    "PRE_ARENA_KHG",
    "WHISPERS_FATE",
    "PRE_MCC_GLOBAL",
    "PRE_MCC_BSTONES",
    "DOING_MCC_GLOBAL",
    "DOING_GRAVEYARD_GLOBAL",
    "DOING_GRAVEYARD_GRAVEYARD",
    "HOOK_COAST_GATEWAY_GLOBAL",
    "HOOK_COAST_GATEWAY_DARKWOOD",
    "IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL",
    "IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST",
    "AFTER_WIZARD_BATTLE_GLOBAL",
    "DOING_FOCAL_SITES_GLOBAL",
    "DOING_FOCAL_SITES_GUILD",
    "FINAL_BATTLE_GLOBAL",
    "FINAL_BATTLE_GUILD",
    "AFTER_FINAL_BATTLE_GLOBAL",
    "AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL",
    "AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL",
    "AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL",
    "AFTER_FINAL_BATTLE_DIDNT_GET_SWORD_GLOBAL",
    "LOOKOUT_POINT_DEMON_DOOR_READY",
    "SUMMON_THE_SHIP",
    "NORTHERN_WASTES_OPEN",
    "SCARY_NECROPOLIS",
    "NECROPOLIS_FINISHED",
    "THUNDER_KILLED",
    "BRIAR_ROSE_KILLED",
    "GUILDMASTER_KILLED",
    "NONE_KILLED",
    "DRAGON_GATE_OPEN",
    "SNOWSPIRE_ARRIVAL",
}

local FINISHED_GAME_EXPRESSIONS = {
    "EXPRESSION_SHIT",
    "EXPRESSION_THANKS",
    "EXPRESSION_HEROIC_STANCE",
    "EXPRESSION_FLIRT",
    "EXPRESSION_APOLOGY",
    "EXPRESSION_SNEER",
    "EXPRESSION_EVIL_LAUGH",
    "EXPRESSION_BATTLE_CRY",
    "EXPRESSION_PELVIC_THRUST",
    "EXPRESSION_MIDDLE_FINGER",
    "EXPRESSION_BELCH",
    "EXPRESSION_FART",
    "EXPRESSION_VICTORY_PUMP",
    "EXPRESSION_COCK_A_DOODLE_DO",
    "EXPRESSION_CROTCH_GRAB",
    "EXPRESSION_KISS_MY_ASS",
    "EXPRESSION_FLAMENCO",
    "EXPRESSION_COSSACK",
    "EXPRESSION_AIR_GUITAR",
    "EXPRESSION_BALLET",
    "EXPRESSION_SATURDAY_NIGHT_FEVER",
    "EXPRESSION_TAP",
}

function DeclareGossipCategories(quest)
    for _, cat in ipairs(GOSSIP_CATEGORIES) do
        quest:AddRumourCategory(cat)
    end
end

function Init(questObject)
    Quest = questObject
    Quest:Log("LUAGameflow: Init started.")

    Quest:SetMasterGameState("PostSavePosition", EGP_INTRO)
    Quest:SetStateBool("CoreQuestWaiting", false)
    DeclareGossipCategories(Quest)

    Quest:Log("LUAGameflow: Init finished.")
end

function OnPersist(questObject, context)
    Quest = questObject

    local postSavePosition = Quest:GetMasterGameState("PostSavePosition") or EGP_INTRO
    postSavePosition = Quest:PersistTransferInt(context, "PostSavePosition", postSavePosition)
    Quest:SetMasterGameState("PostSavePosition", postSavePosition)

    local coreQuestWaiting = Quest:GetStateBool("CoreQuestWaiting") or false
    coreQuestWaiting = Quest:PersistTransferBool(context, "CoreQuestWaiting", coreQuestWaiting)
    Quest:SetStateBool("CoreQuestWaiting", coreQuestWaiting)
end

function CoreQuestReminder()
    local quest = Quest
    quest:Log("LUAGameflow: CoreQuestReminder worker started.")
    while true do
        if not quest:NewScriptFrame() then return end
        if quest:GetStateBool("CoreQuestWaiting") then
            local timerId = quest:RegisterTimer()
            if timerId and timerId >= 0 then
                quest:SetTimer(timerId, 300)
                while quest:GetTimer(timerId) > 0 do
                    if not quest:NewScriptFrame() then
                        quest:DeregisterTimer(timerId)
                        return
                    end
                end
                if quest:GetStateBool("CoreQuestWaiting") and not quest:IsHeroOnQuest() then
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_NEW_QUEST_AT_GUILD", "", true, false)
                end
                quest:DeregisterTimer(timerId)
            end
        end
    end
end

-- Restores guards to Barrow Fields after completing Q_TraderConflictEvil
function CheckBarrowFieldsGuards()
    local quest = Quest
    quest:Log("LUAGameflow: CheckBarrowFieldsGuards worker started.")

    if not quest:IsQuestCompleted("Q_TraderConflictEvil") and not quest:IsQuestCompleted("Q_TraderConflictGood") then
        while not quest:IsQuestActive("Q_TraderConflictEvil") and not quest:IsQuestActive("Q_TraderConflictGood") do
            if not quest:NewScriptFrame() then return end
        end

        if quest:IsQuestActive("Q_TraderConflictEvil") then
            while not quest:MsgOnQuestCompleted("Q_TraderConflictEvil") do
                if not quest:NewScriptFrame() then return end
            end

            -- If BarrowFields is currently loaded, wait for the player to leave it
            if quest:IsRegionLoaded("BarrowFields") then
                while quest:IsRegionLoaded("BarrowFields") do
                    if not quest:NewScriptFrame() then return end
                end
            end

            -- Wait for player to re-enter BarrowFields
            while not quest:IsRegionLoaded("BarrowFields") do
                if not quest:NewScriptFrame() then return end
            end

            local village = quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS")
            if village then
                quest:EnableGuards(village, true)
                quest:Log("LUAGameflow: CheckBarrowFieldsGuards re-enabled guards for VILLAGE_BARROWFIELDS.")
            end
        end
    end
end

-- Stage 0: EGP_INTRO
function RunStage_Intro(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_INTRO)
    quest:SetAllSoundsAsMuted(true)

    if not quest:IsXbox() then
        quest:GiveHeroObject("OBJECT_TATTOO_CARD_CHEST_CUSTOM_01", -1, true)
        quest:GiveHeroObject("OBJECT_TATTOO_CARD_BACK_CUSTOM_01", -1, true)
        quest:GiveHeroObject("OBJECT_TATTOO_CARD_ARMS_CUSTOM_01", -1, true)
        quest:GiveHeroObject("OBJECT_TATTOO_CARD_LEGS_CUSTOM_01", -1, true)
        quest:GiveHeroObject("OBJECT_TATTOO_CARD_FACE_CUSTOM_01", -1, true)
    end

    quest:AddLogbookStoryEntry(10)
    quest:SetAllowScreenFadingOnNextRegionChange(false)
    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_OAKVALE_INTRO", "Q_NewOakValeIntro", false)

    while not quest:MsgOnQuestCompleted("Q_NewOakValeIntro") do
        if not quest:NewScriptFrame() then return end
    end

    quest:ActivateMultipleQuestsWithoutLoadingResources({
        "Hook_Fresco_07_OakValeRaid",
        "Hook_Fresco_09_TimePassing",
        "Hook_Fresco_10_UneasyAlliance",
        "Q_GuildTraining",
    })

    quest:AddLogbookStoryEntry(30)
    quest:AddNewRumourToCategory("GUILD_TRAINING", "TEXT_AI_GOSSIP_GUILD_TRAINING_GUILD")
    quest:SetCategoryActivity("GUILD_TRAINING", true)
    quest:AddGossipVillage("GUILD_TRAINING", "VILLAGE_GUILD_COMPLEX_INSIDE")

    -- Initial batch activation of background and ambient quests
    local backgroundQuests = {
        "CreatureGenerators",
        "Q_ArenaHoldingScript",
        "V_AssassinAttacks",
        "V_BanditToll",
        "V_TravellingHeroes",
        "V_BeardyBaldy",
        "V_BodyGuard",
        "Q_BowerstoneTownLifeIntro",
        "V_ChapelOfEvil",
        "V_DemonDoors",
        "V_Fisherman",
        "V_FisticuffsClub",
        "V_HauntedHouse",
        "Q_HerosOldHouse",
        "V_HiddenBooty",
        "V_RandomPopulationSim",
        "Q_RansomVictimChiefsHouse",
        "V_RockTrollFirstEncounter",
        "V_StatueMaster",
        "V_SwordInTheStone",
        "V_TempleOfLight",
    }
    quest:ActivateMultipleQuestsWithoutLoadingResources(backgroundQuests)

    return EGP_TRAINING
end

-- Stage 100: EGP_TRAINING
function RunStage_Training(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_TRAINING)

    while not quest:MsgOnQuestCompleted("Q_GuildTraining") do
        if not quest:NewScriptFrame() then return end
    end

    quest:DeactivateQuest("Hook_Fresco_10_UneasyAlliance", 0)
    quest:AddLogbookStoryEntry(50)

    while not quest:IsLevelLoaded("HeroGuildComplex") do
        if not quest:NewScriptFrame() then return end
    end

    quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0)
    quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
    quest:DeactivateQuestLater("Q_GuildTrainingPreMelee", 0)
    quest:DeactivateQuestLater("Q_GuildTrainingMelee", 0)
    quest:DeactivateQuestLater("Q_GuildTrainingWill", 0)
    quest:DeactivateQuestLater("Q_GuildTrainingSkill", 0)
    quest:DeactivateQuestLater("Q_GuildTrainingDeparture", 0)

    local guildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
    local gmMarker = quest:GetThingWithScriptName("M_GuildmasterMarker")
    if guildmaster and gmMarker and guildmaster:IsAlive() then
        quest:EntityTeleportToThing(guildmaster, gmMarker)
    end

    quest:SetHeroGuideToShowQuestCardsWhenSpokenTo(true)

    local frontDoors = quest:GetThingWithScriptName("GuildDoors")
    if frontDoors and frontDoors:IsAlive() then
        quest:SetThingAsUsable(frontDoors, true)
    end

    quest:AddQuestCard("OBJECT_QUEST_CARD_WASP_MENACE", "Q_WaspBoss", false, false)
    quest:SetStateBool("CoreQuestWaiting", true)

    quest:AddQuestCard("OBJECT_DUMMY_QUEST_CARD_DEFEAT_SNOW_TROLL", "DUMMY_QUEST_HAS_NO_SCRIPT", false, false)
    quest:AddQuestCard("OBJECT_DUMMY_QUEST_CARD_SUPPRESS_UPRISING", "DUMMY_QUEST_2", false, false)
    quest:AddQuestCard("OBJECT_DUMMY_QUEST_CARD_MINION_CAMP", "DUMMY_QUEST_HAS_NO_SCRIPT", false, false)

    quest:ActivateQuest("GameflowAssistance")
    quest:ActivateQuest("CS_OakValeRevisited")
    quest:ActivateQuest("V_BeggarAndChild")
    quest:ActivateQuest("V_GuildMaster")
    quest:ActivateQuest("Q_OrchardFarm_Barricade")
    quest:ActivateQuest("V_SickChild")
    quest:ActivateQuest("V_BookCollecting")
    quest:ActivateQuest("V_ChickenKicking")
    quest:ActivateQuest("V_Bordello")

    local wwTeleporter = quest:GetThingWithScriptName("WitchwoodTeleporter")
    if wwTeleporter then
        quest:SetTeleporterAsActive(wwTeleporter, false)
    end

    quest:RemoveRumourCategory("GUILD_TRAINING")
    quest:AddNewRumourToCategory("WASP_BOSS", "TEXT_AI_GOSSIP_WASP_BOSS_GLOBAL")
    quest:SetCategoryActivity("WASP_BOSS", true)

    return EGP_WAITING_FOR_WASP_BOSS
end

-- Stage 150: EGP_WAITING_FOR_WASP_BOSS
function RunStage_WaitingForWaspBoss(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_WAITING_FOR_WASP_BOSS)
    quest:AutoSave()

    while not quest:IsQuestActive("Q_WaspBoss") do
        if not quest:NewScriptFrame() then return end
    end

    quest:SetStateBool("CoreQuestWaiting", false)

    if quest:DisplayTutorial(TUTORIAL_CATEGORY_QUEST) then
        while not quest:MsgIsTutorialClickedPast() do
            if not quest:NewScriptFrame() then return end
        end
    end

    quest:RemoveRumourCategory("WASP_BOSS")
    quest:AddNewRumourToCategory("DOING_WASP_BOSS", "TEXT_AI_GOSSIP_DOING_WASP_BOSS_GLOBAL")
    quest:SetCategoryActivity("DOING_WASP_BOSS", true)

    return EGP_WASP_BOSS
end

-- Stage 200: EGP_WASP_BOSS
function RunStage_WaspBoss(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_WASP_BOSS)

    while not quest:MsgOnQuestCompleted("Q_WaspBoss") do
        if not quest:NewScriptFrame() then return end
    end

    if quest:DisplayTutorial(TUTORIAL_CATEGORY_RENOWN) then
        while not quest:MsgIsTutorialClickedPast() do
            if not quest:NewScriptFrame() then return end
        end
    end

    if not quest:NewScriptFrame() then return end

    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_GUARDIAN_SISTER_INFO_FIRST", "QS_GuardianSisterInfo", false)
    quest:AddLogbookStoryEntry(60)
    quest:ActivateQuest("V_TourGuide")

    quest:RemoveRumourCategory("DOING_WASP_BOSS")
    quest:AddNewRumourToCategory("VISIT_MAZE_1_GLOBAL", "TEXT_AI_GOSSIP_VISIT_MAZE_1_GLOBAL")
    quest:AddNewRumourToCategory("VISIT_MAZE_1_GLOBAL", "TEXT_AI_GOSSIP_WASPBOSS_KILLED")
    quest:SetCategoryActivity("VISIT_MAZE_1_GLOBAL", true)
    quest:AddNewRumourToCategory("VISIT_MAZE_1_BSSLUMS", "TEXT_AI_GOSSIP_VISIT_MAZE_1_BSSLUMS")
    quest:SetCategoryActivity("VISIT_MAZE_1_BSSLUMS", true)
    quest:AddGossipVillage("VISIT_MAZE_1_BSSLUMS", "VILLAGE_BOWERSTONE_SLUMS")

    return EGP_MAZE_MEETING
end

-- Stage 300: EGP_MAZE_MEETING
function RunStage_MazeMeeting(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_MAZE_MEETING)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("QS_GuardianSisterInfo") do
        if not quest:NewScriptFrame() then return end
    end

    quest:ActivateQuest("Hook_BowerstoneTeleportTutorial")
    quest:AddLogbookStoryEntry(70)
    quest:ActivateQuest("V_PicnicAreaAfterWaspBoss")
    quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
    quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
    quest:DeactivateQuest("V_BeggarAndChild", 0)

    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
    quest:SetStateBool("CoreQuestWaiting", true)

    quest:RemoveRumourCategory("VISIT_MAZE_1_GLOBAL")
    quest:RemoveRumourCategory("VISIT_MAZE_1_BSSLUMS")
    quest:AddNewRumourToCategory("PRE_ORCH_FARM", "TEXT_AI_GOSSIP_PRE_ORCH_FARM_GLOBAL")
    quest:SetCategoryActivity("PRE_ORCH_FARM", true)

    return EGP_WAITING_FOR_ORCHARD_FARM
end

-- Stage 400: EGP_WAITING_FOR_ORCHARD_FARM
function RunStage_WaitingForOrchardFarm(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_WAITING_FOR_ORCHARD_FARM)
    quest:AutoSave()

    while true do
        if quest:IsQuestActive("Q_OrchardFarmRaidGood") then
            if not quest:NewScriptFrame() then return end
            quest:RemoveQuestCardFromGuild("Q_OrchardFarmRaidEvil")
            break
        elseif quest:IsQuestActive("Q_OrchardFarmRaidEvil") then
            if not quest:NewScriptFrame() then return end
            quest:RemoveQuestCardFromGuild("Q_OrchardFarmRaidGood")
            break
        end
        if not quest:NewScriptFrame() then return end
    end

    quest:SetStateBool("CoreQuestWaiting", false)
    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_GUILD_BOASTS")

    quest:RemoveRumourCategory("PRE_ORCH_FARM")
    quest:AddNewRumourToCategory("DOING_ORCH_FARM", "TEXT_AI_GOSSIP_DOING_ORCH_FARM_GLOBAL")
    quest:SetCategoryActivity("DOING_ORCH_FARM", true)

    return EGP_ORCHARD_FARM
end


-- Stage 450: EGP_ORCHARD_FARM
function RunStage_OrchardFarm(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_ORCHARD_FARM)
    quest:AutoSave()

    while true do
        if quest:MsgOnQuestCompleted("Q_OrchardFarmRaidEvil") or quest:MsgOnQuestCompleted("Q_OrchardFarmRaidGood") then
            break
        end
        if not quest:NewScriptFrame() then return end
    end

    quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", true)
    quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodEntrance", true)
    quest:ActivateQuest("V_IntroductionToTrophies")
    quest:ActivateQuest("QR_EscortTrader_Manager")

    quest:AddQuestCard("OBJECT_QUEST_CARD_DARKWOOD_TRADER_ESCORT", "Q_TraderEscort", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_HOBBE_CAVE", "Q_HobbeCave", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_HOBBE_CONTEST", "Q_HobbeToothContest", false, false)
    quest:SetStateBool("CoreQuestWaiting", true)

    quest:RemoveRumourCategory("DOING_ORCH_FARM")
    quest:AddNewRumourToCategory("TRADER_ESCORT_GLOBAL", "TEXT_AI_GOSSIP_TRADER_ESCORT_GLOBAL")
    quest:SetCategoryActivity("TRADER_ESCORT_GLOBAL", true)
    quest:AddNewRumourToCategory("TRADER_ESCORT_BSSLUMS", "TEXT_AI_GOSSIP_TRADER_ESCORT_BSSLUMS")
    quest:SetCategoryActivity("TRADER_ESCORT_BSSLUMS", true)
    quest:AddGossipVillage("TRADER_ESCORT_BSSLUMS", "VILLAGE_BOWERSTONE_SLUMS")

    return EGP_WAITING_FOR_TRADER_ESCORT
end

-- Stage 500: EGP_WAITING_FOR_TRADER_ESCORT
function RunStage_WaitingForTraderEscort(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_WAITING_FOR_TRADER_ESCORT)
    quest:AutoSave()

    while not quest:IsQuestActive("Q_TraderEscort") do
        if not quest:NewScriptFrame() then return end
    end

    quest:SetStateBool("CoreQuestWaiting", false)
    return EGP_TRADER_ESCORT
end

-- Stage 550: EGP_TRADER_ESCORT
function RunStage_TraderEscort(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_TRADER_ESCORT)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("Q_TraderEscort") do
        if not quest:NewScriptFrame() then return end
    end

    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_MAZE_AGAIN", "", true, true)
    if not quest:NewScriptFrame() then return end

    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_GUARDIAN_SISTER_INFO_SECOND", "QS_GuardianSisterInfo2_SisterInBanditCamp", false)
    quest:AddLogbookStoryEntry(90)
    quest:DeactivateQuest("V_IntroductionToTrophies", 0)
    quest:ActivateQuest("V_TalentlessBard")

    quest:RemoveRumourCategory("TRADER_ESCORT_GLOBAL")
    quest:RemoveRumourCategory("TRADER_ESCORT_BSSLUMS")
    quest:AddNewRumourToCategory("VISIT_MAZE_2_GLOBAL", "TEXT_AI_GOSSIP_VISIT_MAZE_2_GLOBAL")
    quest:SetCategoryActivity("VISIT_MAZE_2_GLOBAL", true)
    quest:AddNewRumourToCategory("VISIT_MAZE_2_BSSLUMS", "TEXT_AI_GOSSIP_VISIT_MAZE_2_BSSLUMS")
    quest:SetCategoryActivity("VISIT_MAZE_2_BSSLUMS", true)
    quest:AddGossipVillage("VISIT_MAZE_2_BSSLUMS", "VILLAGE_OAKVALE")

    return EGP_SECOND_MAZE_MEETING
end

-- Stage 600: EGP_SECOND_MAZE_MEETING
function RunStage_SecondMazeMeeting(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_SECOND_MAZE_MEETING)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("QS_GuardianSisterInfo2_SisterInBanditCamp") do
        if not quest:NewScriptFrame() then return end
    end

    quest:SetStateBool("CoreQuestWaiting", false)
    quest:AddLogbookStoryEntry(110)

    quest:RemoveRumourCategory("VISIT_MAZE_2_GLOBAL")
    quest:RemoveRumourCategory("VISIT_MAZE_2_BSSLUMS")
    quest:AddNewRumourToCategory("DOING_BANDIT_CAMP_GLOBAL", "TEXT_AI_GOSSIP_DOING_BANDIT_CAMP_GLOBAL")
    quest:SetCategoryActivity("DOING_BANDIT_CAMP_GLOBAL", true)
    quest:AddNewRumourToCategory("DOING_BANDIT_CAMP_BANDITCAMP", "TEXT_AI_GOSSIP_DOING_BANDIT_CAMP_BANDITCAMP")
    quest:SetCategoryActivity("DOING_BANDIT_CAMP_BANDITCAMP", true)
    quest:AddGossipVillage("DOING_BANDIT_CAMP_BANDITCAMP", "VILLAGE_BANDIT_CAMP_MAIN")
    quest:AddGossipVillage("DOING_BANDIT_CAMP_BANDITCAMP", "VILLAGE_BANDIT_CAMP_RESIDENTIAL")

    return EGP_BANDIT_CAMP
end

-- Stage 700: EGP_BANDIT_CAMP
function RunStage_BanditCamp(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_BANDIT_CAMP)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("Q_BanditCamp") do
        if not quest:NewScriptFrame() then return end
    end

    quest:AddLogbookStoryEntry(120)
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_MAZE_YET_AGAIN", "", true, true)
    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_GUARDIAN_TROPHY_DEALER_INFO", "QS_GuardianTrophyDealerInfo", false)
    quest:DeactivateQuest("V_BanditToll", 0)
    quest:ActivateQuest("Hook_Fresco_08_SistersStory")

    quest:RemoveRumourCategory("DOING_BANDIT_CAMP_GLOBAL")
    quest:RemoveRumourCategory("DOING_BANDIT_CAMP_BANDITCAMP")
    quest:AddNewRumourToCategory("VISIT_MAZE_3_GLOBAL", "TEXT_AI_GOSSIP_VISIT_MAZE_3_GLOBAL")
    quest:SetCategoryActivity("VISIT_MAZE_3_GLOBAL", true)
    quest:AddNewRumourToCategory("VISIT_MAZE_3_GUILD", "TEXT_AI_GOSSIP_VISIT_MAZE_3_GUILD")
    quest:SetCategoryActivity("VISIT_MAZE_3_GUILD", true)
    quest:AddGossipVillage("VISIT_MAZE_3_GUILD", "VILLAGE_GUILD_COMPLEX_INSIDE")

    quest:AddQuestCard("OBJECT_QUEST_CARD_TRADER_CONFLICT_EVIL", "Q_TraderConflictEvil", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_TRADER_CONFLICT_GOOD", "Q_TraderConflictGood", false, false)
    quest:ActivateQuest("V_MurderTwistFinal")

    return EGP_MAZE_TELEPORT_TO_WW
end

-- Stage 800: EGP_MAZE_TELEPORT_TO_WW
function RunStage_MazeTeleportToWW(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_MAZE_TELEPORT_TO_WW)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("QS_GuardianTrophyDealerInfo") do
        if not quest:NewScriptFrame() then return end
    end

    quest:ActivateQuestWithoutLoadingResources("Hook_BalverinesInKHGPreWhiteBalv")
    quest:RemoveRumourCategory("VISIT_MAZE_3_GLOBAL")
    quest:RemoveRumourCategory("VISIT_MAZE_3_GUILD")
    quest:AddNewRumourToCategory("FIND_ARCHAEOLOGIST", "TEXT_AI_GOSSIP_FIND_ARCHAEOLOGIST_GLOBAL")
    quest:SetCategoryActivity("FIND_ARCHAEOLOGIST", true)

    return EGP_TROPHY_DEALER
end

-- Stage 850: EGP_TROPHY_DEALER
function RunStage_TrophyDealer(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_TROPHY_DEALER)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("V_TrophyDealer") do
        if not quest:NewScriptFrame() then return end
    end

    quest:ActivateQuest("V_AmbushScam")
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_FOUND_TROPHY_DEALER", "", true, true)
    quest:AddLogbookStoryEntry(140)
    quest:AddQuestCard("OBJECT_QUEST_CARD_WHITE_BALVERINE_KNOTHOLE_GLADE", "Q_WhiteBalverineKnotholeGlade", false, false)

    quest:RemoveRumourCategory("FIND_ARCHAEOLOGIST")
    quest:AddNewRumourToCategory("PRE_WHITE_BALV_GLOBAL", "TEXT_AI_GOSSIP_PRE_WHITE_BALV_GLOBAL")
    quest:SetCategoryActivity("PRE_WHITE_BALV_GLOBAL", true)
    quest:SetCategoryActivity("PRE_WHITE_BALV_WITCHWOOD", true)

    return EGP_WITCHWOOD_POST_TROPHY_DEALER
end

-- Stage 856: EGP_WITCHWOOD_POST_TROPHY_DEALER
function RunStage_WitchwoodPostTrophyDealer(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_WITCHWOOD_POST_TROPHY_DEALER)
    quest:AutoSave()
    quest:SetStateBool("CoreQuestWaiting", true)

    local gaveMessage = false
    while not quest:IsQuestActive("Q_WhiteBalverineKnotholeGlade") do
        if quest:IsRegionLoaded("KnotholeGlade") then
            if not gaveMessage then
                quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_KNOTHOLE_GLADE_CLOSED", "", true, true)
                gaveMessage = true
            end
        else
            gaveMessage = false
        end
        if not quest:NewScriptFrame() then return end
    end

    quest:SetStateBool("CoreQuestWaiting", false)
    quest:DeactivateQuest("Hook_BalverinesInKHGPreWhiteBalv", 0)

    quest:RemoveRumourCategory("PRE_WHITE_BALV_GLOBAL")
    quest:RemoveRumourCategory("PRE_WHITE_BALV_WITCHWOOD")
    quest:AddNewRumourToCategory("DOING_WHITE_BALV_GLOBAL", "TEXT_AI_GOSSIP_DOING_WHITE_BALV_GLOBAL")
    quest:SetCategoryActivity("DOING_WHITE_BALV_GLOBAL", true)
    quest:AddNewRumourToCategory("DOING_WHITE_BALV_KHG", "TEXT_AI_GOSSIP_DOING_WHITE_BALV_KHG")
    quest:SetCategoryActivity("DOING_WHITE_BALV_KHG", true)
    quest:AddGossipVillage("DOING_WHITE_BALV_KHG", "VILLAGE_KNOTHOLE_GLADE")

    return EGP_WHITE_BALVERINE_WITCHWOOD
end

-- Stage 870: EGP_WHITE_BALVERINE_WITCHWOOD
function RunStage_WhiteBalverineWitchwood(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_WHITE_BALVERINE_WITCHWOOD)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("Q_WhiteBalverineKnotholeGlade") do
        if not quest:NewScriptFrame() then return end
    end

    quest:AddLogbookStoryEntry(150)
    quest:AddLogbookStoryEntry(160)
    quest:AddQuestCard("OBJECT_QUEST_CARD_ARENA", "Q_Arena", false, false)
    quest:SetStateBool("CoreQuestWaiting", true)
    quest:RemoveQuestCardFromGuild("Q_HobbeToothContest")
    quest:ActivateQuest("V_ArcheryCompetition")

    quest:RemoveRumourCategory("DOING_WHITE_BALV_GLOBAL")
    quest:RemoveRumourCategory("DOING_WHITE_BALV_KHG")
    quest:AddNewRumourToCategory("PRE_ARENA_GLOBAL", "TEXT_AI_GOSSIP_PRE_ARENA_GLOBAL")
    quest:SetCategoryActivity("PRE_ARENA_GLOBAL", true)
    quest:AddNewRumourToCategory("PRE_ARENA_KHG", "TEXT_AI_GOSSIP_PRE_ARENA_KHG")
    quest:SetCategoryActivity("PRE_ARENA_KHG", true)
    quest:AddGossipVillage("PRE_ARENA_KHG", "VILLAGE_KNOTHOLE_GLADE")

    return EGP_WITCHWOOD_WAITING_FOR_ARENA
end

-- Stage 875: EGP_WITCHWOOD_WAITING_FOR_ARENA
function RunStage_WitchwoodWaitingForArena(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_WITCHWOOD_WAITING_FOR_ARENA)
    quest:AutoSave()

    while not quest:IsQuestActive("Q_Arena") do
        if not quest:NewScriptFrame() then return end
    end

    quest:SetStateBool("CoreQuestWaiting", false)
    return EGP_ARENA
end

-- Stage 900: EGP_ARENA
function RunStage_Arena(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_ARENA)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("Q_Arena") do
        if not quest:NewScriptFrame() then return end
    end

    if quest:GetMasterGameState("WhisperKilledByHero") then
        quest:AddLogbookStoryEntry(175)
    else
        quest:AddLogbookStoryEntry(170)
    end
    quest:AddLogbookStoryEntry(180)

    quest:ActivateQuest("Hook_Fresco_05_MothersStory")
    quest:ActivateQuest("Hook_Fresco_13_KilledScorpion")
    quest:ActivateQuest("V_GhostGrannyNecklace")
    quest:ActivateQuest("V_MayorsInvitation")
    if not quest:NewScriptFrame() then return end

    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_MEET_SISTER", "QS_MeetSister", true)
    quest:AddQuestCard("OBJECT_QUEST_CARD_BREAK_SIEGE", "Q_BreakSiege", false, true)
    quest:AddQuestCard("OBJECT_QUEST_CARD_LOST_TRADER", "V_LostTrader", false, true)
    quest:RemoveQuestCardFromGuild("Q_HobbeCave")
    quest:DeactivateQuest("Q_HobbeToothContest", 0)

    if quest:GetMasterGameState("WhisperKilledByHero") then
        quest:AddNewRumourToCategory("WHISPERS_FATE", "TEXT_AI_GOSSIP_KILLED_WHISPER_GLOBAL")
        quest:SetCategoryActivity("WHISPERS_FATE", true)
    else
        quest:AddNewRumourToCategory("WHISPERS_FATE", "TEXT_AI_GOSSIP_SPARED_WHISPER_GLOBAL")
        quest:SetCategoryActivity("WHISPERS_FATE", true)
    end

    return EGP_MEET_SISTER
end

-- Stage 1000: EGP_MEET_SISTER
function RunStage_MeetSister(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_MEET_SISTER)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("QS_MeetSister") do
        if not quest:NewScriptFrame() then return end
    end

    quest:AddLogbookStoryEntry(190)
    if not quest:NewScriptFrame() then return end

    quest:AddQuestCard("OBJECT_QUEST_CARD_MINION_CLIFFTOP_CHASE", "Q_MinionClifftopChase", false, false)
    quest:SetStateBool("CoreQuestWaiting", true)

    quest:RemoveRumourCategory("PRE_ARENA_GLOBAL")
    quest:RemoveRumourCategory("PRE_ARENA_KHG")
    quest:AddNewRumourToCategory("PRE_MCC_GLOBAL", "TEXT_AI_GOSSIP_PRE_MCC_GLOBAL")
    quest:SetCategoryActivity("PRE_MCC_GLOBAL", true)
    quest:AddNewRumourToCategory("PRE_MCC_BSTONES", "TEXT_AI_GOSSIP_PRE_MCC_BSTONES")
    quest:SetCategoryActivity("PRE_MCC_BSTONES", true)
    quest:AddGossipVillage("PRE_MCC_BSTONES", "VILLAGE_BOWERSTONE_SLUMS")
    quest:AddGossipVillage("PRE_MCC_BSTONES", "VILLAGE_BOWERSTONE_POSH")

    return EGP_WAITING_FOR_MCC
end

-- Stage 1050: EGP_WAITING_FOR_MCC
function RunStage_WaitingForMCC(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_WAITING_FOR_MCC)
    quest:AutoSave()

    while not quest:IsQuestActive("Q_MinionClifftopChase") do
        if not quest:NewScriptFrame() then return end
    end

    quest:SetStateBool("CoreQuestWaiting", false)
    quest:RemoveRumourCategory("PRE_MCC_GLOBAL")
    quest:RemoveRumourCategory("PRE_MCC_STONES")
    quest:AddNewRumourToCategory("DOING_MCC_GLOBAL", "TEXT_AI_GOSSIP_DOING_MCC_GLOBAL")
    quest:SetCategoryActivity("DOING_MCC_GLOBAL", true)

    return EGP_MINION_CLIFFTOP_CHASE
end

-- Stage 1100: EGP_MINION_CLIFFTOP_CHASE
function RunStage_MinionClifftopChase(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_MINION_CLIFFTOP_CHASE)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("Q_MinionClifftopChase") do
        if not quest:NewScriptFrame() then return end
    end

    quest:AddQuestCard("OBJECT_QUEST_CARD_HANGING_TREE_EVIL", "Q_HangingTreeEvil", false, false)
    quest:AddQuestCard("OBJECT_QUEST_CARD_HANGING_TREE_GOOD", "Q_HangingTreeGood", false, false)
    quest:AddLogbookStoryEntry(200)
    if not quest:NewScriptFrame() then return end

    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_GRAVEYARD_SECRET_PASSAGE", "Q_OpeningGraveyardSecretPassage", false)
    quest:RemoveRumourCategory("DOING_MCC_GLOBAL")
    quest:AddNewRumourToCategory("DOING_GRAVEYARD_GLOBAL", "TEXT_AI_GOSSIP_DOING_GRAVEYARD_GLOBAL")
    quest:SetCategoryActivity("DOING_GRAVEYARD_GLOBAL", true)
    quest:SetCategoryActivity("DOING_GRAVEYARD_GRAVEYARD", true)
    quest:AddQuestCard("OBJECT_QUEST_CARD_MINION_CAMP", "Q_MinionCamp", false, false)

    return EGP_GRAVEYARD
end

-- Stage 1200: EGP_GRAVEYARD
function RunStage_Graveyard(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_GRAVEYARD)
    quest:AutoSave()
    if not quest:NewScriptFrame() then return end

    while not quest:MsgOnQuestCompleted("Q_OpeningGraveyardSecretPassage") do
        if not quest:NewScriptFrame() then return end
    end

    quest:AddQuestCard("OBJECT_QUEST_CARD_BOUNTY_HUNT", "Q_BountyHunt", false, false)
    quest:RemoveRumourCategory("DOING_GRAVEYARD_GLOBAL")
    quest:RemoveRumourCategory("DOING_GRAVEYARD_GRAVEYARD")
    quest:RemoveRumourCategory("WHISPERS_FATE")

    return EGP_PRISON
end

-- Stage 1250: EGP_PRISON
function RunStage_Prison(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_PRISON)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("Q_PrisonEscapeRescueMother") do
        if not quest:NewScriptFrame() then return end
    end

    quest:AddLogbookStoryEntry(230)
    quest:ActivateQuest("Hook_Fresco_03_JacksObsession")
    quest:ActivateQuest("Hook_Fresco_06_Prison")
    quest:ActivateQuest("Hook_Fresco_04_Kraken")
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_GUILD_SEAL_ACTIVE", "", true, true)

    quest:AddQuestCard("OBJECT_QUEST_CARD_AMBUSH_TRADERS", "Q_AmbushTraders", false, false)
    quest:DeactivateQuest("V_BeardyBaldy", 0)
    quest:DeactivateQuest("V_ParanoidWhispers", 0)

    quest:AddNewRumourToCategory("HOOK_COAST_GATEWAY_GLOBAL", "TEXT_AI_GOSSIP_HOOK_COAST_GATEWAY_GLOBAL")
    quest:AddNewRumourToCategory("HOOK_COAST_GATEWAY_GLOBAL", "TEXT_AI_GOSSIP_AFTER_PRISON_GLOBAL")
    quest:SetCategoryActivity("HOOK_COAST_GATEWAY_GLOBAL", true)
    if not quest:NewScriptFrame() then return end

    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_GATEWAY_TO_HOOK_COAST", "Q_EndGame", false)
    if not quest:NewScriptFrame() then return end

    return EGP_HOOK_COAST
end

-- Stage 1300: EGP_HOOK_COAST
function RunStage_HookCoast(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_HOOK_COAST)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("Q_EndGame") do
        if not quest:NewScriptFrame() then return end
    end

    return EGP_WAITING_FOR_WIZARD_BATTLE
end

-- Stage 1450: EGP_WAITING_FOR_WIZARD_BATTLE
function RunStage_WaitingForWizardBattle(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_WAITING_FOR_WIZARD_BATTLE)
    quest:SetStateBool("CoreQuestWaiting", true)

    local gaveMessage = false
    while not quest:IsQuestActive("Q_WizardBattle") do
        if quest:IsRegionLoaded("HeroGuildComplexInside") then
            if not gaveMessage then
                quest:AutoSave()
                quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MOTHER_LEFT_YOU_THIS", "", true, true)
                gaveMessage = true
            end
        else
            gaveMessage = false
        end
        if not quest:NewScriptFrame() then return end
    end

    quest:SetStateBool("CoreQuestWaiting", false)
    quest:RemoveRumourCategory("HOOK_COAST_GATEWAY_GLOBAL")
    quest:RemoveRumourCategory("HOOK_COAST_GATEWAY_DARKWOOD")

    quest:AddNewRumourToCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL", "TEXT_AI_GOSSIP_IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL")
    quest:SetCategoryActivity("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL", true)
    quest:AddNewRumourToCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST", "TEXT_AI_GOSSIP_IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST")
    quest:SetCategoryActivity("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST", true)
    quest:AddGossipVillage("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST", "VILLAGE_HOOK_COAST")

    quest:DeactivateQuest("Q_RansomVictimChiefsHouse", 0)
    quest:PrepareQuestsWhenFinalQuestIsActivated("Q_WizardBattle")
    quest:DeactivateQuest("V_RandomPopulationSim", 0)
    quest:ActivateQuest("Q_RansomVictimChiefsHouse")

    return EGP_WIZARD_BATTLE
end

-- Stage 1500: EGP_WIZARD_BATTLE
function RunStage_WizardBattle(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_WIZARD_BATTLE)

    while not quest:MsgOnQuestCompleted("Q_WizardBattle") do
        if not quest:NewScriptFrame() then return end
    end

    quest:RemoveRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL")
    quest:RemoveRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST")
    quest:AddNewRumourToCategory("AFTER_WIZARD_BATTLE_GLOBAL", "TEXT_AI_GOSSIP_AFTER_WIZARD_BATTLE_GLOBAL")
    quest:SetCategoryActivity("AFTER_WIZARD_BATTLE_GLOBAL", true)
    quest:ActivateQuest("Hook_Fresco_10_UneasyAlliance")

    return EGP_WAITING_FOR_FOCAL_SITES
end

-- Stage 1550: EGP_WAITING_FOR_FOCAL_SITES
function RunStage_WaitingForFocalSites(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_WAITING_FOR_FOCAL_SITES)
    quest:AutoSave()
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_GO_TO_WITCHWOOD_FOR_FOCAL_SITES", "", true, true)

    while not quest:IsRegionLoaded("HeroGuildComplexInside") do
        if not quest:NewScriptFrame() then return end
    end

    quest:ActivateQuest("Q_EndGameFocalSites")
    quest:AddLogbookStoryEntry(270)
    quest:DeactivateQuest("V_TourGuide", 0)
    quest:DeactivateQuest("QR_EscortTrader_Manager", 0)
    quest:DeactivateQuest("V_GhostGrannyNecklace", 0)
    quest:DeactivateQuest("V_SickChild", 0)
    quest:DeactivateQuest("V_SickChildBarrowFields", 0)
    quest:DeactivateQuest("V_SickChild_Activate", 0)

    quest:RemoveRumourCategory("AFTER_WIZARD_BATTLE_GLOBAL")
    quest:AddNewRumourToCategory("DOING_FOCAL_SITES_GLOBAL", "TEXT_AI_GOSSIP_DOING_FOCAL_SITES_GLOBAL")
    quest:SetCategoryActivity("DOING_FOCAL_SITES_GLOBAL", true)
    quest:AddNewRumourToCategory("DOING_FOCAL_SITES_GUILD", "TEXT_AI_GOSSIP_DOING_FOCAL_SITES_GUILD")
    quest:SetCategoryActivity("DOING_FOCAL_SITES_GUILD", true)
    quest:AddGossipVillage("DOING_FOCAL_SITES_GUILD", "VILLAGE_GUILD_COMPLEX_INSIDE")

    return EGP_FOCAL_SITES
end

-- Stage 1600: EGP_FOCAL_SITES
function RunStage_FocalSites(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_FOCAL_SITES)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("Q_EndGameFocalSites") do
        if not quest:NewScriptFrame() then return end
    end

    quest:AddLogbookStoryEntry(280)
    if not quest:NewScriptFrame() then return end

    quest:DeactivateQuest("V_GuildMaster", 0)
    quest:ActivateQuest("Q_EndGameBossBattle")

    quest:RemoveRumourCategory("DOING_FOCAL_SITES_GLOBAL")
    quest:RemoveRumourCategory("DOING_FOCAL_SITES_GUILD")
    quest:AddNewRumourToCategory("FINAL_BATTLE_GLOBAL", "TEXT_AI_GOSSIP_FINAL_BATTLE_GLOBAL")
    quest:SetCategoryActivity("FINAL_BATTLE_GLOBAL", true)
    quest:AddNewRumourToCategory("FINAL_BATTLE_GUILD", "TEXT_AI_GOSSIP_FINAL_BATTLE_GUILD")
    quest:SetCategoryActivity("FINAL_BATTLE_GUILD", true)
    quest:AddGossipVillage("FINAL_BATTLE_GUILD", "VILLAGE_GUILD_COMPLEX_INSIDE")

    return EGP_JACK_BOSS_FIGHT
end

-- Stage 1700: EGP_JACK_BOSS_FIGHT
function RunStage_JackBossFight(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_JACK_BOSS_FIGHT)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("Q_EndGameBossBattle") do
        if not quest:NewScriptFrame() then return end
    end
    while quest:IsQuestActive("Q_EndGameBossBattle") do
        if not quest:NewScriptFrame() then return end
    end

    quest:RemoveRumourCategory("FINAL_BATTLE_GLOBAL")
    quest:RemoveRumourCategory("FINAL_BATTLE_GUILD")
    quest:AddNewRumourToCategory("AFTER_FINAL_BATTLE_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_GLOBAL")
    quest:AddNewRumourToCategory("AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL")
    quest:AddNewRumourToCategory("AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL")
    quest:AddNewRumourToCategory("AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL")
    quest:AddNewRumourToCategory("AFTER_FINAL_BATTLE_DIDNT_GET_SWORD_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_NOTGOT_SWORD_GLOBAL")
    quest:SetCategoryActivity("AFTER_FINAL_BATTLE_GLOBAL", true)

    local jackResult = quest:GetMasterGameState("JackBossBattleResult")
    if jackResult == 1 then
        quest:SetCategoryActivity("AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL", true)
        quest:SetCategoryActivity("AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL", true)
        quest:AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_305")
    elseif jackResult == 2 then
        quest:SetCategoryActivity("AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL", true)
        quest:SetCategoryActivity("AFTER_FINAL_BATTLE_DIDNT_GET_SWORD_GLOBAL", true)
        quest:AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_300")
    end

    quest:ActivateQuest("V_RandomPopulationSim")
    quest:PrepareQuestsWhenFinalQuestIsCompleted()
    quest:SetStateBool("CoreQuestWaiting", false)

    quest:RemoveQuestCardFromGuild("Q_BreakSiege")
    quest:DeactivateQuest("Q_BreakSiege", 0)
    quest:SetMasterGameState("BreakSiegeFinished", true)

    quest:RemoveQuestCardFromGuild("Q_MinionCamp")
    quest:DeactivateQuest("Q_MinionCamp", 0)

    quest:AddQuestCard("OBJECT_QUEST_CARD_RANSOM_VICTIM", "Q_RansomVictim", false, false)
    quest:DeactivateQuest("V_AmbushScam", 0)
    quest:ActivateQuest("DummyQuestForScarletRoseStatue")

    quest:AddNewRumourToCategory("LOOKOUT_POINT_DEMON_DOOR_READY", "TEXT_AI_GOSSIP_LOOKOUT_POINT_DEMON_DOOR_READY")
    quest:SetCategoryActivity("LOOKOUT_POINT_DEMON_DOOR_READY", true)

    while not quest:IsRegionLoaded("OakvaleMemorialGarden") do
        if not quest:NewScriptFrame() then return end
    end

    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_FIRE_HEART", "Q_FireHeart", false)
    return EGP_FIRE_HEART
end

-- Stage 1900: EGP_FIRE_HEART
function RunStage_FireHeart(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_FIRE_HEART)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("Q_FireHeart") do
        if not quest:NewScriptFrame() then return end
    end

    quest:ActivateQuest("V_GuildMaster")
    quest:RemoveRumourCategory("LOOKOUT_POINT_DEMON_DOOR_READY")
    if not quest:NewScriptFrame() then return end

    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_SUMMONING_THE_SHIP", "Q_SummoningTheShip", false)

    if quest:GetMasterGameState("JackBossBattleResult") == 2 then
        quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_MAZE_RESEARCH", "V_MazeResearch", false)
    end

    quest:AddNewRumourToCategory("SUMMON_THE_SHIP", "TEXT_AI_GOSSIP_SUMMON_THE_SHIP")
    quest:SetCategoryActivity("SUMMON_THE_SHIP", true)
    quest:AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_310")

    return EGP_THE_SHIP
end

-- Stage 2100: EGP_THE_SHIP
function RunStage_TheShip(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_THE_SHIP)
    quest:AutoSave()
    quest:FadeScreenIn()

    while not quest:MsgOnQuestCompleted("Q_SummoningTheShip") do
        if not quest:NewScriptFrame() then return end
    end

    while not quest:IsRegionLoaded("LostBay") do
        if not quest:NewScriptFrame() then return end
    end

    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_ASK_ORACLE", "Q_AwakeningTheOracle", false)
    quest:RemoveRumourCategory("SUMMON_THE_SHIP")

    quest:AddNewRumourToCategory("NORTHERN_WASTES_OPEN", "TEXT_AI_GOSSIP_NORTHERN_WASTES_OPEN")
    quest:SetCategoryActivity("NORTHERN_WASTES_OPEN", true)
    quest:AddNewRumourToCategory("SCARY_NECROPOLIS", "TEXT_AI_GOSSIP_SCARY_NECROPOLIS")
    quest:SetCategoryActivity("SCARY_NECROPOLIS", true)
    quest:SetCategoryActivity("SNOWSPIRE_ARRIVAL", true)
    quest:AddGossipVillage("SNOWSPIRE_ARRIVAL", "VILLAGE_SNOWSPIRE")
    quest:AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_320")
    if not quest:NewScriptFrame() then return end

    return EGP_THE_ORACLE
end

-- Stage 2300: EGP_THE_ORACLE
function RunStage_TheOracle(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_THE_ORACLE)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("Q_AwakeningTheOracle") do
        if not quest:NewScriptFrame() then return end
    end

    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_SCYTHE_INFO", "QS_ScytheInfo", false)
    if not quest:NewScriptFrame() then return end

    return EGP_SCYTHE_MEETING
end

-- Stage 2400: EGP_SCYTHE_MEETING
function RunStage_ScytheMeeting(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_SCYTHE_MEETING)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("QS_ScytheInfo") do
        if not quest:NewScriptFrame() then return end
    end

    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_HERO_SOULS", "Q_HeroSouls", false)
    quest:RemoveRumourCategory("SCARY_NECROPOLIS")

    quest:AddNewRumourToCategory("NECROPOLIS_FINISHED", "TEXT_AI_GOSSIP_NECROPOLIS_FINISHED")
    quest:SetCategoryActivity("NECROPOLIS_FINISHED", true)
    quest:AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_330")
    quest:ActivateQuest("V_Oracle")
    if not quest:NewScriptFrame() then return end

    return EGP_HERO_SOULS
end

-- Stage 2500: EGP_HERO_SOULS
function RunStage_HeroSouls(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_HERO_SOULS)
    quest:AutoSave()

    while not quest:MsgOnQuestCompleted("Q_HeroSouls") do
        if not quest:NewScriptFrame() then return end
    end

    quest:AddNewRumourToCategory("DRAGON_GATE_OPEN", "TEXT_AI_GOSSIP_DRAGON_GATE_OPEN")
    quest:SetCategoryActivity("DRAGON_GATE_OPEN", true)
    if not quest:NewScriptFrame() then return end

    quest:GiveQuestCardDirectly("OBJECT_QUEST_CARD_DRAGON_BOSS", "Q_DragonBossFight", false)
    return EGP_DRAGON_FIGHT
end

-- Stage 2600: EGP_DRAGON_FIGHT
function RunStage_DragonFight(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_DRAGON_FIGHT)
    quest:AutoSave()
    quest:FadeScreenIn()

    while not quest:MsgOnQuestCompleted("Q_DragonBossFight") do
        if not quest:NewScriptFrame() then return end
    end
    while quest:IsQuestActive("Q_DragonBossFight") do
        if not quest:NewScriptFrame() then return end
    end

    quest:ActivateQuest("Hook_Fresco_12_KilledDragon")
    quest:SetRegionTextDisplayAsActive(false)
    quest:SetAllowScreenFadingOnNextRegionChange(false)
    quest:FadeScreenOutUntilNextCallToFadeScreenIn(1.0, 0.0)
    if not quest:NewScriptFrame() then return end

    local hero = quest:GetHero()
    local frescoHSP = quest:GetThingWithScriptName("FrescoDomeHSP")
    if hero and frescoHSP then
        quest:EntityTeleportToThing(hero, frescoHSP)
    end

    while not quest:IsLevelLoaded("FrescoDome") do
        if not quest:NewScriptFrame() then return end
    end

    -- Run end credits sequence
    if hero then
        quest:StartCutscene({ Hero = hero })
    else
        quest:StartCutscene()
    end
    quest:RunCutscene("CS_FABLE_CREDITS", true, false)
    quest:EndCutscene()

    local nw3Door = quest:GetThingWithScriptName("NW3BronzeDoorHSP")
    if hero and nw3Door then
        quest:EntityTeleportToThing(hero, nw3Door)
    end

    while not quest:IsRegionLoaded("NorthernWastes3") do
        if not quest:NewScriptFrame() then return end
    end

    quest:SetMasterGameState("BodyGuardsInLimbo", false)
    quest:TeleportAllFollowersToHeroPosition()
    quest:SetRegionTextDisplayAsActive(true)
    quest:SetAllowScreenFadingOnNextRegionChange(true)

    return EGP_FINISHED_GAME
end

-- Stage 2800: EGP_FINISHED_GAME
function RunStage_FinishedGame(quest)
    quest:SetMasterGameState("PostSavePosition", EGP_FINISHED_GAME)

    for _, expr in ipairs(FINISHED_GAME_EXPRESSIONS) do
        quest:GiveHeroExpression(expr, -1, false)
    end

    quest:AutoSave()
    quest:FadeScreenIn()
    quest:Log("LUAGameflow: Campaign completed! Free roam mode active.")

    -- Idle loop to keep the campaign manager alive during post-game free roam
    while true do
        if not quest:NewScriptFrame() then return end
    end
end

function Main(questObject)
    Quest = questObject
    Quest:Log("LUAGameflow: Main() started.")

    -- Spawn background worker threads
    Quest:CreateThread("CoreQuestReminder")
    Quest:CreateThread("CheckBarrowFieldsGuards")

    local STAGE_HANDLERS = {
        [EGP_INTRO]                        = RunStage_Intro,
        [EGP_TRAINING]                     = RunStage_Training,
        [EGP_WAITING_FOR_WASP_BOSS]        = RunStage_WaitingForWaspBoss,
        [EGP_WASP_BOSS]                    = RunStage_WaspBoss,
        [EGP_MAZE_MEETING]                 = RunStage_MazeMeeting,
        [EGP_WAITING_FOR_ORCHARD_FARM]     = RunStage_WaitingForOrchardFarm,
        [EGP_ORCHARD_FARM]                 = RunStage_OrchardFarm,
        [EGP_WAITING_FOR_TRADER_ESCORT]    = RunStage_WaitingForTraderEscort,
        [EGP_TRADER_ESCORT]                = RunStage_TraderEscort,
        [EGP_SECOND_MAZE_MEETING]          = RunStage_SecondMazeMeeting,
        [EGP_BANDIT_CAMP]                  = RunStage_BanditCamp,
        [EGP_MAZE_TELEPORT_TO_WW]          = RunStage_MazeTeleportToWW,
        [EGP_TROPHY_DEALER]                = RunStage_TrophyDealer,
        [EGP_WITCHWOOD_POST_TROPHY_DEALER] = RunStage_WitchwoodPostTrophyDealer,
        [EGP_WHITE_BALVERINE_WITCHWOOD]    = RunStage_WhiteBalverineWitchwood,
        [EGP_WITCHWOOD_WAITING_FOR_ARENA]  = RunStage_WitchwoodWaitingForArena,
        [EGP_ARENA]                        = RunStage_Arena,
        [EGP_MEET_SISTER]                  = RunStage_MeetSister,
        [EGP_WAITING_FOR_MCC]              = RunStage_WaitingForMCC,
        [EGP_MINION_CLIFFTOP_CHASE]        = RunStage_MinionClifftopChase,
        [EGP_GRAVEYARD]                    = RunStage_Graveyard,
        [EGP_PRISON]                       = RunStage_Prison,
        [EGP_HOOK_COAST]                   = RunStage_HookCoast,
        [EGP_WAITING_FOR_WIZARD_BATTLE]    = RunStage_WaitingForWizardBattle,
        [EGP_WIZARD_BATTLE]                = RunStage_WizardBattle,
        [EGP_WAITING_FOR_FOCAL_SITES]      = RunStage_WaitingForFocalSites,
        [EGP_FOCAL_SITES]                  = RunStage_FocalSites,
        [EGP_JACK_BOSS_FIGHT]              = RunStage_JackBossFight,
        [EGP_FIRE_HEART]                   = RunStage_FireHeart,
        [EGP_THE_SHIP]                     = RunStage_TheShip,
        [EGP_THE_ORACLE]                   = RunStage_TheOracle,
        [EGP_SCYTHE_MEETING]               = RunStage_ScytheMeeting,
        [EGP_HERO_SOULS]                   = RunStage_HeroSouls,
        [EGP_DRAGON_FIGHT]                 = RunStage_DragonFight,
        [EGP_FINISHED_GAME]                = RunStage_FinishedGame,
    }

    local currentStage = Quest:GetMasterGameState("PostSavePosition") or EGP_INTRO
    Quest:Log("LUAGameflow: Starting main campaign loop at stage " .. tostring(currentStage))

    while currentStage do
        local handler = STAGE_HANDLERS[currentStage]
        if handler then
            Quest:Log("LUAGameflow: Transitioning into stage " .. tostring(currentStage))
            currentStage = handler(Quest)
        else
            Quest:Log("LUAGameflow: Unknown or completed stage: " .. tostring(currentStage))
            break
        end
    end

    Quest:Log("LUAGameflow: Main loop exited.")
end
