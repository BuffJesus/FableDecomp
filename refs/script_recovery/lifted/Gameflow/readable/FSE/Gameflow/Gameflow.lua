-- Readable native conversion: Gameflow. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local TUTORIAL_CATEGORY_QUEST = 27  -- ETutorialCategory (Ego_r.pdb)
local TUTORIAL_CATEGORY_RENOWN = 29  -- ETutorialCategory (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    CoreQuestReminderIntervalSeconds = 4040,  -- 300
}

-- Gameflow.Main (retail 0x00ce7670)
function Main(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, isActiveThreadTerminating, predicateResult4, predicateResult5
    local getMasterGameState, switch, actorMap, scratchValue
    quest:CreateThread("CoreQuestReminder")  -- native thread body NScript::CQ_HeroSoulsNostroScript::CNostro::Main: lift it as function CoreQuestReminder(quest)
    quest:CreateThread("CheckBarrowFieldsGuards")  -- native thread body NScript::CGameflowScript::CheckBarrowFieldsGuards: lift it as function CheckBarrowFieldsGuards(quest)
    isActiveThreadTerminating = false
    switch = quest:GetMasterGameState("PostSavePosition")
    if not (switch == 0 or switch == 100 or switch == 150 or switch == 200 or switch == 300 or switch == 400 or switch == 450 or switch == 500 or switch == 550 or switch == 600 or switch == 700 or switch == 800 or switch == 850 or switch == 856 or switch == 865 or switch == 870 or switch == 875 or switch == 900 or switch == 1000 or switch == 1050 or switch == 1100 or switch == 1200 or switch == 1250 or switch == 1300 or switch == 1450 or switch == 1500 or switch == 1550 or switch == 1600 or switch == 1700 or switch == 1900 or switch == 2100 or switch == 2300 or switch == 2400 or switch == 2500 or switch == 2600 or switch == 2800) then
        switch = 0x7ffffffe
    end
    repeat
        if switch == 0 then
            quest:SetMasterGameState("PostSavePosition", 0)
            quest:SetAllSoundsAsMuted(true)
            if not quest:IsXbox() then
                quest:GiveHeroObject("OBJECT_TATTOO_CARD_CHEST_CUSTOM_01", -1)
                quest:GiveHeroObject("OBJECT_TATTOO_CARD_BACK_CUSTOM_01", -1)
                quest:GiveHeroObject("OBJECT_TATTOO_CARD_ARMS_CUSTOM_01", -1)
                quest:GiveHeroObject("OBJECT_TATTOO_CARD_LEGS_CUSTOM_01", -1)
                quest:GiveHeroObject("OBJECT_TATTOO_CARD_FACE_CUSTOM_01", -1)
            end
            quest:AddLogbookStoryEntry(10)
            quest:SetAllowScreenFadingOnNextRegionChange(false)
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_OAKVALE_INTRO", "Q_NewOakValeIntro", false)
            while not quest:MsgOnQuestCompleted("Q_NewOakValeIntro") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            scratchValue = {}
            table.insert(scratchValue, "Hook_Fresco_07_OakValeRaid")
            table.insert(scratchValue, "Hook_Fresco_09_TimePassing")
            table.insert(scratchValue, "Hook_Fresco_10_UneasyAlliance")
            table.insert(scratchValue, "Q_GuildTraining")
            quest:ActivateMultipleQuestsWithoutLoadingResources(scratchValue)
            quest:AddLogbookStoryEntry(30)
            quest:AddNewRumourToCategory("GUILD_TRAINING", "TEXT_AI_GOSSIP_GUILD_TRAINING_GUILD")
            quest:SetCategoryActivity("GUILD_TRAINING", true)
            quest:AddGossipVillage("GUILD_TRAINING", "VILLAGE_GUILD_COMPLEX_INSIDE")
            scratchValue = {}
            table.insert(scratchValue, "CreatureGenerators")
            table.insert(scratchValue, "Q_ArenaHoldingScript")
            table.insert(scratchValue, "V_AssassinAttacks")
            table.insert(scratchValue, "V_BanditToll")
            table.insert(scratchValue, "V_TravellingHeroes")
            table.insert(scratchValue, "V_BeardyBaldy")
            table.insert(scratchValue, "V_BodyGuard")
            table.insert(scratchValue, "Q_BowerstoneTownLifeIntro")
            table.insert(scratchValue, "V_ChapelOfEvil")
            table.insert(scratchValue, "V_DemonDoors")
            table.insert(scratchValue, "V_Fisherman")
            table.insert(scratchValue, "V_FisticuffsClub")
            table.insert(scratchValue, "V_HauntedHouse")
            table.insert(scratchValue, "Q_HerosOldHouse")
            table.insert(scratchValue, "V_HiddenBooty")
            table.insert(scratchValue, "V_RandomPopulationSim")
            table.insert(scratchValue, "Q_RansomVictimChiefsHouse")
            table.insert(scratchValue, "V_RockTrollFirstEncounter")
            table.insert(scratchValue, "V_StatueMaster")
            table.insert(scratchValue, "V_SwordInTheStone")
            table.insert(scratchValue, "V_TempleOfLight")
            quest:ActivateMultipleQuestsWithoutLoadingResources(scratchValue)
            switch = 100
        end
        if switch == 100 then
            quest:SetMasterGameState("PostSavePosition", 100)
            while not quest:MsgOnQuestCompleted("Q_GuildTraining") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:DeactivateQuest("Hook_Fresco_10_UneasyAlliance", 0)
            quest:AddLogbookStoryEntry(50)
            while not quest:IsLevelLoaded("HeroGuildComplex") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0)
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
            quest:DeactivateQuestLater("Q_GuildTrainingPreMelee", 0)
            quest:DeactivateQuestLater("Q_GuildTrainingMelee", 0)
            quest:DeactivateQuestLater("Q_GuildTrainingWill", 0)
            quest:DeactivateQuestLater("Q_GuildTrainingSkill", 0)
            quest:DeactivateQuestLater("Q_GuildTrainingDeparture", 0)
            local theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
            if theRealGuildmaster ~= nil and theRealGuildmaster:IsAlive() then
                if quest:IsActiveThreadTerminating() then return end
                quest:EntityTeleportToThing(quest:GetThingWithScriptName("TheRealGuildmaster"), quest:GetThingWithScriptName("M_GuildmasterMarker"), false)
            end
            quest:SetHeroGuideToShowQuestCardsWhenSpokenTo(true)
            actorMap = quest:GetThingWithScriptName("GuildDoors")
            if actorMap ~= nil and actorMap:IsAlive() then
                if quest:IsActiveThreadTerminating() then return end
                quest:SetThingAsUsable(actorMap, true)
            end
            quest:AddQuestCard("OBJECT_QUEST_CARD_WASP_MENACE", "Q_WaspBoss", false, false)
            quest:SetStateInt("CoreQuestWaiting", 1)
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
            quest:SetTeleporterAsActive(quest:GetThingWithScriptName("WitchwoodTeleporter"), false)
            quest:RemoveRumourCategory("GUILD_TRAINING")
            quest:AddNewRumourToCategory("WASP_BOSS", "TEXT_AI_GOSSIP_WASP_BOSS_GLOBAL")
            quest:SetCategoryActivity("WASP_BOSS", true)
            switch = 150
        end
        if switch == 150 then
            quest:SetMasterGameState("PostSavePosition", 150)
            quest:AutoSave()
            while not quest:IsQuestActive("Q_WaspBoss") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("CoreQuestWaiting", 0)
            if quest:DisplayTutorial(TUTORIAL_CATEGORY_QUEST) then
                if quest:IsActiveThreadTerminating() then return end
                while not quest:MsgIsTutorialClickedPast() do
                    if not quest:NewScriptFrame() then return end
                end
            end
            quest:RemoveRumourCategory("WASP_BOSS")
            quest:AddNewRumourToCategory("DOING_WASP_BOSS", "TEXT_AI_GOSSIP_DOING_WASP_BOSS_GLOBAL")
            quest:SetCategoryActivity("DOING_WASP_BOSS", true)
            switch = 200
        end
        if switch == 200 then
            quest:SetMasterGameState("PostSavePosition", 200)
            while not quest:MsgOnQuestCompleted("Q_WaspBoss") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            if quest:DisplayTutorial(TUTORIAL_CATEGORY_RENOWN) then
                if quest:IsActiveThreadTerminating() then return end
                while not quest:MsgIsTutorialClickedPast() do
                    if not quest:NewScriptFrame() then return end
                end
            end
            if not quest:NewScriptFrame() then return end
            if not quest:NewScriptFrame() then return end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GUARDIAN_SISTER_INFO_FIRST", "QS_GuardianSisterInfo", false)
            quest:AddLogbookStoryEntry(60)
            quest:ActivateQuest("V_TourGuide")
            quest:RemoveRumourCategory("DOING_WASP_BOSS")
            quest:AddNewRumourToCategory("VISIT_MAZE_1_GLOBAL", "TEXT_AI_GOSSIP_VISIT_MAZE_1_GLOBAL")
            quest:AddNewRumourToCategory("VISIT_MAZE_1_GLOBAL", "TEXT_AI_GOSSIP_WASPBOSS_KILLED")
            quest:SetCategoryActivity("VISIT_MAZE_1_GLOBAL", true)
            quest:AddNewRumourToCategory("VISIT_MAZE_1_BSSLUMS", "TEXT_AI_GOSSIP_VISIT_MAZE_1_BSSLUMS")
            quest:SetCategoryActivity("VISIT_MAZE_1_BSSLUMS", true)
            quest:AddGossipVillage("VISIT_MAZE_1_BSSLUMS", "VILLAGE_BOWERSTONE_SLUMS")
            switch = 300
        end
        if switch == 300 then
            quest:SetMasterGameState("PostSavePosition", 300)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("QS_GuardianSisterInfo") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:ActivateQuest("Hook_BowerstoneTeleportTutorial")
            quest:AddLogbookStoryEntry(70)
            quest:ActivateQuest("V_PicnicAreaAfterWaspBoss")
            quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
            quest:RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT")
            quest:DeactivateQuest("V_BeggarAndChild", 0)
            quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false)
            quest:AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false)
            quest:SetStateInt("CoreQuestWaiting", 1)
            quest:RemoveRumourCategory("VISIT_MAZE_1_GLOBAL")
            quest:RemoveRumourCategory("VISIT_MAZE_1_BSSLUMS")
            quest:AddNewRumourToCategory("PRE_ORCH_FARM", "TEXT_AI_GOSSIP_PRE_ORCH_FARM_GLOBAL")
            quest:SetCategoryActivity("PRE_ORCH_FARM", true)
            switch = 400
        end
        if switch == 400 then
            quest:SetMasterGameState("PostSavePosition", 400)
            quest:AutoSave()
            if quest:IsActiveThreadTerminating() then return end
            while true do
                if quest:IsQuestActive("Q_OrchardFarmRaidGood") then break end
                if quest:IsQuestActive("Q_OrchardFarmRaidEvil") then
                    if not quest:NewScriptFrame() then return end
                    quest:RemoveQuestCardFromGuild("Q_OrchardFarmRaidGood")
                    goto LAB_00ce933c
                end
                if not quest:NewScriptFrame() then return end
            end
            if not quest:NewScriptFrame() then return end
            quest:RemoveQuestCardFromGuild("Q_OrchardFarmRaidEvil")
            ::LAB_00ce933c::
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("CoreQuestWaiting", 0)
            quest:AddLogbookTutorialEntry("TEXT_QST_LOG_GUILD_BOASTS")
            quest:RemoveRumourCategory("PRE_ORCH_FARM")
            quest:AddNewRumourToCategory("DOING_ORCH_FARM", "TEXT_AI_GOSSIP_DOING_ORCH_FARM_GLOBAL")
            quest:SetCategoryActivity("DOING_ORCH_FARM", true)
            switch = 450
        end
        if switch == 450 then
            quest:SetMasterGameState("PostSavePosition", 450)
            quest:AutoSave()
            repeat
                if quest:MsgOnQuestCompleted("Q_OrchardFarmRaidEvil") then
                    goto LAB_00ce94b6
                else
                    isActiveThreadTerminating = true
                    if quest:MsgOnQuestCompleted("Q_OrchardFarmRaidGood") then goto LAB_00ce94b6 end
                    predicateResult = true
                end
                goto FLOW_past_lab_00ce94b6
                ::LAB_00ce94b6::
                predicateResult = false
                ::FLOW_past_lab_00ce94b6::
                isActiveThreadTerminating = isActiveThreadTerminating and false
                if not predicateResult then
                    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                    if isActiveThreadTerminating then
                        return
                    end
                    quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", true)
                    quest:MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodEntrance", true)
                    quest:ActivateQuest("V_IntroductionToTrophies")
                    quest:ActivateQuest("QR_EscortTrader_Manager")
                    quest:AddQuestCard("OBJECT_QUEST_CARD_DARKWOOD_TRADER_ESCORT", "Q_TraderEscort", false, false)
                    quest:AddQuestCard("OBJECT_QUEST_CARD_HOBBE_CAVE", "Q_HobbeCave", false, false)
                    quest:AddQuestCard("OBJECT_QUEST_CARD_HOBBE_CONTEST", "Q_HobbeToothContest", false, false)
                    quest:SetStateInt("CoreQuestWaiting", 1)
                    quest:RemoveRumourCategory("DOING_ORCH_FARM")
                    quest:AddNewRumourToCategory("TRADER_ESCORT_GLOBAL", "TEXT_AI_GOSSIP_TRADER_ESCORT_GLOBAL")
                    quest:SetCategoryActivity("TRADER_ESCORT_GLOBAL", true)
                    quest:AddNewRumourToCategory("TRADER_ESCORT_BSSLUMS", "TEXT_AI_GOSSIP_TRADER_ESCORT_BSSLUMS")
                    quest:SetCategoryActivity("TRADER_ESCORT_BSSLUMS", true)
                    quest:AddGossipVillage("TRADER_ESCORT_BSSLUMS", "VILLAGE_BOWERSTONE_SLUMS")
                    switch = 500
                    goto FLOW_chain_next_3
                end
                if not quest:NewScriptFrame() then return end
            until false
            switch = 500
        end
        ::FLOW_chain_next_3::
        if switch == 500 then
            -- FLOW_native_label_1: (native jump target)
            quest:SetMasterGameState("PostSavePosition", 500)
            quest:AutoSave()
            while not quest:IsQuestActive("Q_TraderEscort") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:SetStateInt("CoreQuestWaiting", 0)
            switch = 550
        end
        if switch == 550 then
            quest:SetMasterGameState("PostSavePosition", 550)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("Q_TraderEscort") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_MAZE_AGAIN", "", true, true)
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GUARDIAN_SISTER_INFO_SECOND", "QS_GuardianSisterInfo2_SisterInBanditCamp", false)
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
            switch = 600
        end
        if switch == 600 then
            quest:SetMasterGameState("PostSavePosition", 600)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("QS_GuardianSisterInfo2_SisterInBanditCamp") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:SetStateInt("CoreQuestWaiting", 0)
            quest:AddLogbookStoryEntry(110)
            quest:RemoveRumourCategory("VISIT_MAZE_2_GLOBAL")
            quest:RemoveRumourCategory("VISIT_MAZE_2_BSSLUMS")
            quest:AddNewRumourToCategory("DOING_BANDIT_CAMP_GLOBAL", "TEXT_AI_GOSSIP_DOING_BANDIT_CAMP_GLOBAL")
            quest:SetCategoryActivity("DOING_BANDIT_CAMP_GLOBAL", true)
            quest:AddNewRumourToCategory("DOING_BANDIT_CAMP_BANDITCAMP", "TEXT_AI_GOSSIP_DOING_BANDIT_CAMP_BANDITCAMP")
            quest:SetCategoryActivity("DOING_BANDIT_CAMP_BANDITCAMP", true)
            quest:AddGossipVillage("DOING_BANDIT_CAMP_BANDITCAMP", "VILLAGE_BANDIT_CAMP_MAIN")
            quest:AddGossipVillage("DOING_BANDIT_CAMP_BANDITCAMP", "VILLAGE_BANDIT_CAMP_RESIDENTIAL")
            switch = 700
        end
        if switch == 700 then
            quest:SetMasterGameState("PostSavePosition", 700)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("Q_BanditCamp") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:AddLogbookStoryEntry(120)
            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_MAZE_YET_AGAIN", "", true, true)
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GUARDIAN_TROPHY_DEALER_INFO", "QS_GuardianTrophyDealerInfo", false)
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
            switch = 800
        end
        if switch == 800 then
            quest:SetMasterGameState("PostSavePosition", 800)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("QS_GuardianTrophyDealerInfo") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:ActivateQuestWithoutLoadingResources("Hook_BalverinesInKHGPreWhiteBalv")
            quest:RemoveRumourCategory("VISIT_MAZE_3_GLOBAL")
            quest:RemoveRumourCategory("VISIT_MAZE_3_GUILD")
            quest:AddNewRumourToCategory("FIND_ARCHAEOLOGIST", "TEXT_AI_GOSSIP_FIND_ARCHAEOLOGIST_GLOBAL")
            quest:SetCategoryActivity("FIND_ARCHAEOLOGIST", true)
            switch = 850
        end
        if switch == 850 then
            quest:SetMasterGameState("PostSavePosition", 850)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("V_TrophyDealer") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:ActivateQuest("V_AmbushScam")
            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_FOUND_TROPHY_DEALER", "", true, true)
            quest:AddLogbookStoryEntry(140)
            quest:AddQuestCard("OBJECT_QUEST_CARD_WHITE_BALVERINE_KNOTHOLE_GLADE", "Q_WhiteBalverineKnotholeGlade", false, false)
            quest:RemoveRumourCategory("FIND_ARCHAEOLOGIST")
            quest:AddNewRumourToCategory("PRE_WHITE_BALV_GLOBAL", "TEXT_AI_GOSSIP_PRE_WHITE_BALV_GLOBAL")
            quest:SetCategoryActivity("PRE_WHITE_BALV_GLOBAL", true)
            quest:SetCategoryActivity("PRE_WHITE_BALV_WITCHWOOD", true)
            switch = 856
        end
        if switch == 856 then
            quest:SetMasterGameState("PostSavePosition", 856)
            quest:AutoSave()
            quest:SetStateInt("CoreQuestWaiting", 1)
            predicateResult4 = false
            while not quest:IsQuestActive("Q_WhiteBalverineKnotholeGlade") do
                if not quest:NewScriptFrame() then return end
                if quest:IsRegionLoaded("KnotholeGlade") then
                    if predicateResult4 then goto continue_1 end
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_KNOTHOLE_GLADE_CLOSED", "", true, true)
                    predicateResult4 = true
                else
                    predicateResult4 = false
                end
                ::continue_1::
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:SetStateInt("CoreQuestWaiting", 0)
            quest:DeactivateQuest("Hook_BalverinesInKHGPreWhiteBalv", 0)
            quest:RemoveRumourCategory("PRE_WHITE_BALV_GLOBAL")
            quest:RemoveRumourCategory("PRE_WHITE_BALV_WITCHWOOD")
            quest:AddNewRumourToCategory("DOING_WHITE_BALV_GLOBAL", "TEXT_AI_GOSSIP_DOING_WHITE_BALV_GLOBAL")
            quest:SetCategoryActivity("DOING_WHITE_BALV_GLOBAL", true)
            quest:AddNewRumourToCategory("DOING_WHITE_BALV_KHG", "TEXT_AI_GOSSIP_DOING_WHITE_BALV_KHG")
            quest:SetCategoryActivity("DOING_WHITE_BALV_KHG", true)
            quest:AddGossipVillage("DOING_WHITE_BALV_KHG", "VILLAGE_KNOTHOLE_GLADE")
            switch = 865
        end
        if switch == 865 or switch == 870 then
            quest:SetMasterGameState("PostSavePosition", 870)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("Q_WhiteBalverineKnotholeGlade") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:AddLogbookStoryEntry(150)
            quest:AddLogbookStoryEntry(160)
            quest:AddQuestCard("OBJECT_QUEST_CARD_ARENA", "Q_Arena", false, false)
            quest:SetStateInt("CoreQuestWaiting", 1)
            quest:RemoveQuestCardFromGuild("Q_HobbeToothContest")
            quest:ActivateQuest("V_ArcheryCompetition")
            quest:RemoveRumourCategory("DOING_WHITE_BALV_GLOBAL")
            quest:RemoveRumourCategory("DOING_WHITE_BALV_KHG")
            quest:AddNewRumourToCategory("PRE_ARENA_GLOBAL", "TEXT_AI_GOSSIP_PRE_ARENA_GLOBAL")
            quest:SetCategoryActivity("PRE_ARENA_GLOBAL", true)
            quest:AddNewRumourToCategory("PRE_ARENA_KHG", "TEXT_AI_GOSSIP_PRE_ARENA_KHG")
            quest:SetCategoryActivity("PRE_ARENA_KHG", true)
            quest:AddGossipVillage("PRE_ARENA_KHG", "VILLAGE_KNOTHOLE_GLADE")
            switch = 875
        end
        if switch == 875 then
            quest:SetMasterGameState("PostSavePosition", 875)
            quest:AutoSave()
            while not quest:IsQuestActive("Q_Arena") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:SetStateInt("CoreQuestWaiting", 0)
            switch = 900
        end
        if switch == 900 then
            quest:SetMasterGameState("PostSavePosition", 900)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("Q_Arena") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            getMasterGameState = 175
            if not quest:GetMasterGameState("WhisperKilledByHero") then
                getMasterGameState = 170
            end
            quest:AddLogbookStoryEntry(getMasterGameState)
            quest:AddLogbookStoryEntry(180)
            quest:ActivateQuest("Hook_Fresco_05_MothersStory")
            quest:ActivateQuest("Hook_Fresco_13_KilledScorpion")
            quest:ActivateQuest("V_GhostGrannyNecklace")
            quest:ActivateQuest("V_MayorsInvitation")
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_MEET_SISTER", "QS_MeetSister", true)
            quest:AddQuestCard("OBJECT_QUEST_CARD_BREAK_SIEGE", "Q_BreakSiege", false, true)
            quest:AddQuestCard("OBJECT_QUEST_CARD_LOST_TRADER", "V_LostTrader", false, true)
            quest:RemoveQuestCardFromGuild("Q_HobbeCave")
            quest:DeactivateQuest("Q_HobbeToothContest", 0)
            if not quest:GetMasterGameState("WhisperKilledByHero") then
                quest:AddNewRumourToCategory("WHISPERS_FATE", "TEXT_AI_GOSSIP_SPARED_WHISPER_GLOBAL")
                quest:SetCategoryActivity("WHISPERS_FATE", true)
            else
                quest:AddNewRumourToCategory("WHISPERS_FATE", "TEXT_AI_GOSSIP_KILLED_WHISPER_GLOBAL")
                quest:SetCategoryActivity("WHISPERS_FATE", true)
            end
            switch = 1000
        end
        if switch == 1000 then
            quest:SetMasterGameState("PostSavePosition", 1000)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("QS_MeetSister") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:AddLogbookStoryEntry(190)
            if not quest:NewScriptFrame() then return end
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:AddQuestCard("OBJECT_QUEST_CARD_MINION_CLIFFTOP_CHASE", "Q_MinionClifftopChase", false, false)
            quest:SetStateInt("CoreQuestWaiting", 1)
            quest:RemoveRumourCategory("PRE_ARENA_GLOBAL")
            quest:RemoveRumourCategory("PRE_ARENA_KHG")
            quest:AddNewRumourToCategory("PRE_MCC_GLOBAL", "TEXT_AI_GOSSIP_PRE_MCC_GLOBAL")
            quest:SetCategoryActivity("PRE_MCC_GLOBAL", true)
            quest:AddNewRumourToCategory("PRE_MCC_BSTONES", "TEXT_AI_GOSSIP_PRE_MCC_BSTONES")
            quest:SetCategoryActivity("PRE_MCC_BSTONES", true)
            quest:AddGossipVillage("PRE_MCC_BSTONES", "VILLAGE_BOWERSTONE_SLUMS")
            quest:AddGossipVillage("PRE_MCC_BSTONES", "VILLAGE_BOWERSTONE_POSH")
            switch = 1050
        end
        if switch == 1050 then
            quest:SetMasterGameState("PostSavePosition", 1050)
            quest:AutoSave()
            while not quest:IsQuestActive("Q_MinionClifftopChase") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:SetStateInt("CoreQuestWaiting", 0)
            quest:RemoveRumourCategory("PRE_MCC_GLOBAL")
            quest:RemoveRumourCategory("PRE_MCC_STONES")
            quest:AddNewRumourToCategory("DOING_MCC_GLOBAL", "TEXT_AI_GOSSIP_DOING_MCC_GLOBAL")
            quest:SetCategoryActivity("DOING_MCC_GLOBAL", true)
            switch = 1100
        end
        if switch == 1100 then
            quest:SetMasterGameState("PostSavePosition", 1100)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("Q_MinionClifftopChase") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:AddQuestCard("OBJECT_QUEST_CARD_HANGING_TREE_EVIL", "Q_HangingTreeEvil", false, false)
            quest:AddQuestCard("OBJECT_QUEST_CARD_HANGING_TREE_GOOD", "Q_HangingTreeGood", false, false)
            quest:AddLogbookStoryEntry(200)
            if not quest:NewScriptFrame() then return end
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GRAVEYARD_SECRET_PASSAGE", "Q_OpeningGraveyardSecretPassage", false)
            quest:RemoveRumourCategory("DOING_MCC_GLOBAL")
            quest:AddNewRumourToCategory("DOING_GRAVEYARD_GLOBAL", "TEXT_AI_GOSSIP_DOING_GRAVEYARD_GLOBAL")
            quest:SetCategoryActivity("DOING_GRAVEYARD_GLOBAL", true)
            quest:SetCategoryActivity("DOING_GRAVEYARD_GRAVEYARD", true)
            quest:AddQuestCard("OBJECT_QUEST_CARD_MINION_CAMP", "Q_MinionCamp", false, false)
            switch = 1200
        end
        if switch == 1200 then
            quest:SetMasterGameState("PostSavePosition", 1200)
            quest:AutoSave()
            if not quest:NewScriptFrame() then return end
            if not quest:NewScriptFrame() then return end
            while not quest:MsgOnQuestCompleted("Q_OpeningGraveyardSecretPassage") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = false
            quest:AddQuestCard("OBJECT_QUEST_CARD_BOUNTY_HUNT", "Q_BountyHunt", false, false)
            quest:RemoveRumourCategory("DOING_GRAVEYARD_GLOBAL")
            quest:RemoveRumourCategory("DOING_GRAVEYARD_GRAVEYARD")
            quest:RemoveRumourCategory("WHISPERS_FATE")
            switch = 1250
        end
        if switch == 1250 then
            quest:SetMasterGameState("PostSavePosition", 1250)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("Q_PrisonEscapeRescueMother") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
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
            if not quest:NewScriptFrame() then return end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GATEWAY_TO_HOOK_COAST", "Q_EndGame", false)
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            switch = 1300
        end
        if switch == 1300 then
            quest:SetMasterGameState("PostSavePosition", 1300)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("Q_EndGame") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            switch = 1450
        end
        if switch == 1450 then
            quest:SetMasterGameState("PostSavePosition", 1450)
            quest:SetStateInt("CoreQuestWaiting", 1)
            predicateResult5 = false
            while not quest:IsQuestActive("Q_WizardBattle") do
                if not quest:NewScriptFrame() then return end
                if quest:IsRegionLoaded("HeroGuildComplexInside") then
                    if predicateResult5 then goto continue_2 end
                    quest:AutoSave()
                    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MOTHER_LEFT_YOU_THIS", "", true, true)
                    predicateResult5 = true
                else
                    predicateResult5 = false
                end
                ::continue_2::
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:SetStateInt("CoreQuestWaiting", 0)
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
            switch = 1500
        end
        if switch == 1500 then
            quest:SetMasterGameState("PostSavePosition", 1500)
            while not quest:MsgOnQuestCompleted("Q_WizardBattle") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:RemoveRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL")
            quest:RemoveRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST")
            quest:AddNewRumourToCategory("AFTER_WIZARD_BATTLE_GLOBAL", "TEXT_AI_GOSSIP_AFTER_WIZARD_BATTLE_GLOBAL")
            quest:SetCategoryActivity("AFTER_WIZARD_BATTLE_GLOBAL", true)
            quest:ActivateQuest("Hook_Fresco_10_UneasyAlliance")
            switch = 1550
        end
        if switch == 1550 then
            quest:SetMasterGameState("PostSavePosition", 1550)
            quest:AutoSave()
            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_GO_TO_WITCHWOOD_FOR_FOCAL_SITES", "", true, true)
            while not quest:IsRegionLoaded("HeroGuildComplexInside") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
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
            switch = 1600
        end
        if switch == 1600 then
            quest:SetMasterGameState("PostSavePosition", 1600)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("Q_EndGameFocalSites") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:AddLogbookStoryEntry(280)
            if not quest:NewScriptFrame() then return end
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:DeactivateQuest("V_GuildMaster", 0)
            quest:ActivateQuest("Q_EndGameBossBattle")
            quest:RemoveRumourCategory("DOING_FOCAL_SITES_GLOBAL")
            quest:RemoveRumourCategory("DOING_FOCAL_SITES_GUILD")
            quest:AddNewRumourToCategory("FINAL_BATTLE_GLOBAL", "TEXT_AI_GOSSIP_FINAL_BATTLE_GLOBAL")
            quest:SetCategoryActivity("FINAL_BATTLE_GLOBAL", true)
            quest:AddNewRumourToCategory("FINAL_BATTLE_GUILD", "TEXT_AI_GOSSIP_FINAL_BATTLE_GUILD")
            quest:SetCategoryActivity("FINAL_BATTLE_GUILD", true)
            quest:AddGossipVillage("FINAL_BATTLE_GUILD", "VILLAGE_GUILD_COMPLEX_INSIDE")
            switch = 1700
        end
        if switch == 1700 then
            quest:SetMasterGameState("PostSavePosition", 1700)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("Q_EndGameBossBattle") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
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
            getMasterGameState = quest:GetMasterGameState("JackBossBattleResult")
            if getMasterGameState == 1 then
                quest:SetCategoryActivity("AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL", true)
                quest:SetCategoryActivity("AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL", true)
                quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_305")
            elseif getMasterGameState == 2 then
                quest:SetCategoryActivity("AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL", true)
                quest:SetCategoryActivity("AFTER_FINAL_BATTLE_DIDNT_GET_SWORD_GLOBAL", true)
                quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_300")
            end
            quest:ActivateQuest("V_RandomPopulationSim")
            quest:PrepareQuestsWhenFinalQuestIsCompleted()
            quest:SetStateInt("CoreQuestWaiting", 0)
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
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_FIRE_HEART", "Q_FireHeart", false)
            switch = 1900
        end
        if switch == 1900 then
            quest:SetMasterGameState("PostSavePosition", 1900)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("Q_FireHeart") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:ActivateQuest("V_GuildMaster")
            quest:RemoveRumourCategory("LOOKOUT_POINT_DEMON_DOOR_READY")
            if not quest:NewScriptFrame() then return end
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SUMMONING_THE_SHIP", "Q_SummoningTheShip", false)
            if quest:GetMasterGameState("JackBossBattleResult") == 2 then
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if isActiveThreadTerminating then
                    return
                end
                quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_MAZE_RESEARCH", "V_MazeResearch", false)
            end
            quest:AddNewRumourToCategory("SUMMON_THE_SHIP", "TEXT_AI_GOSSIP_SUMMON_THE_SHIP")
            quest:SetCategoryActivity("SUMMON_THE_SHIP", true)
            quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_310")
            switch = 2100
        end
        if switch == 2100 then
            quest:SetMasterGameState("PostSavePosition", 2100)
            quest:AutoSave()
            quest:FadeScreenIn()
            while not quest:MsgOnQuestCompleted("Q_SummoningTheShip") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            while not quest:IsRegionLoaded("LostBay") do
                if not quest:NewScriptFrame() then return end
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_ASK_ORACLE", "Q_AwakeningTheOracle", false)
            quest:RemoveRumourCategory("SUMMON_THE_SHIP")
            quest:AddNewRumourToCategory("NORTHERN_WASTES_OPEN", "TEXT_AI_GOSSIP_NORTHERN_WASTES_OPEN")
            quest:SetCategoryActivity("NORTHERN_WASTES_OPEN", true)
            quest:AddNewRumourToCategory("SCARY_NECROPOLIS", "TEXT_AI_GOSSIP_SCARY_NECROPOLIS")
            quest:SetCategoryActivity("SCARY_NECROPOLIS", true)
            quest:SetCategoryActivity("SNOWSPIRE_ARRIVAL", true)
            quest:AddGossipVillage("SNOWSPIRE_ARRIVAL", "VILLAGE_SNOWSPIRE")
            quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_320")
            if not quest:NewScriptFrame() then return end
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            switch = 2300
        end
        if switch == 2300 then
            quest:SetMasterGameState("PostSavePosition", 2300)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("Q_AwakeningTheOracle") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SCYTHE_INFO", "QS_ScytheInfo", false)
            if not quest:NewScriptFrame() then return end
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            switch = 2400
        end
        if switch == 2400 then
            quest:SetMasterGameState("PostSavePosition", 2400)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("QS_ScytheInfo") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HERO_SOULS", "Q_HeroSouls", false)
            quest:RemoveRumourCategory("SCARY_NECROPOLIS")
            quest:AddNewRumourToCategory("NECROPOLIS_FINISHED", "TEXT_AI_GOSSIP_NECROPOLIS_FINISHED")
            quest:SetCategoryActivity("NECROPOLIS_FINISHED", true)
            quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_330")
            quest:ActivateQuest("V_Oracle")
            if not quest:NewScriptFrame() then return end
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            switch = 2500
        end
        if switch == 2500 then
            quest:SetMasterGameState("PostSavePosition", 2500)
            quest:AutoSave()
            while not quest:MsgOnQuestCompleted("Q_HeroSouls") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            quest:AddNewRumourToCategory("DRAGON_GATE_OPEN", "TEXT_AI_GOSSIP_DRAGON_GATE_OPEN")
            quest:SetCategoryActivity("DRAGON_GATE_OPEN", true)
            if not quest:NewScriptFrame() then return end
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_DRAGON_BOSS", "Q_DragonBossFight", false)
            switch = 2600
        end
        if switch == 2600 then
            quest:SetMasterGameState("PostSavePosition", 2600)
            quest:AutoSave()
            quest:FadeScreenIn()
            while not quest:MsgOnQuestCompleted("Q_DragonBossFight") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            while quest:IsQuestActive("Q_DragonBossFight") do
                if not quest:NewScriptFrame() then return end
            end
            quest:ActivateQuest("Hook_Fresco_12_KilledDragon")
            quest:SetRegionTextDisplayAsActive(false)
            quest:SetAllowScreenFadingOnNextRegionChange(false)
            quest:FadeScreenOutUntilNextCallToFadeScreenIn(1.0, 0.0)
            if not quest:NewScriptFrame() then return end
            quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("FrescoDomeHSP"), false)
            while not quest:IsLevelLoaded("FrescoDome") do
                if not quest:NewScriptFrame() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            local resource = resources:NewResource()
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, hero, 4) do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "Hero", resource)
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_FABLE_CREDITS", actorMap, false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("NW3BronzeDoorHSP"), false)
            while not quest:IsRegionLoaded("NorthernWastes3") do
                if not quest:NewScriptFrame() then return end
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            quest:SetMasterGameState("BodyGuardsInLimbo", false)
            quest:TeleportAllFollowersToHeroPosition()
            quest:SetRegionTextDisplayAsActive(true)
            quest:SetAllowScreenFadingOnNextRegionChange(true)
            quest:SetMasterGameState("PostSavePosition", 2800)
            quest:GiveHeroExpression("EXPRESSION_SHIT", -1, false)
            quest:GiveHeroExpression("EXPRESSION_THANKS", -1, false)
            quest:GiveHeroExpression("EXPRESSION_HEROIC_STANCE", -1, false)
            quest:GiveHeroExpression("EXPRESSION_FLIRT", -1, false)
            quest:GiveHeroExpression("EXPRESSION_APOLOGY", -1, false)
            quest:GiveHeroExpression("EXPRESSION_SNEER", -1, false)
            quest:GiveHeroExpression("EXPRESSION_EVIL_LAUGH", -1, false)
            quest:GiveHeroExpression("EXPRESSION_BATTLE_CRY", -1, false)
            quest:GiveHeroExpression("EXPRESSION_PELVIC_THRUST", -1, false)
            quest:GiveHeroExpression("EXPRESSION_MIDDLE_FINGER", -1, false)
            quest:GiveHeroExpression("EXPRESSION_BELCH", -1, false)
            quest:GiveHeroExpression("EXPRESSION_FART", -1, false)
            quest:GiveHeroExpression("EXPRESSION_VICTORY_PUMP", -1, false)
            quest:GiveHeroExpression("EXPRESSION_COCK_A_DOODLE_DO", -1, false)
            quest:GiveHeroExpression("EXPRESSION_CROTCH_GRAB", -1, false)
            quest:GiveHeroExpression("EXPRESSION_KISS_MY_ASS", -1, false)
            quest:GiveHeroExpression("EXPRESSION_FLAMENCO", -1, false)
            quest:GiveHeroExpression("EXPRESSION_COSSACK", -1, false)
            quest:GiveHeroExpression("EXPRESSION_AIR_GUITAR", -1, false)
            quest:GiveHeroExpression("EXPRESSION_BALLET", -1, false)
            quest:GiveHeroExpression("EXPRESSION_SATURDAY_NIGHT_FEVER", -1, false)
            quest:GiveHeroExpression("EXPRESSION_TAP", -1, false)
            quest:AutoSave()
            quest:FadeScreenIn()
            switch = 2800
        end
        if switch == 2800 then
            repeat
                quest:NewScriptFrame()
            until quest:IsActiveThreadTerminating()
            return
        end
        if switch == 0x7ffffffe then
            return
        end
    until true
end

-- Gameflow.Init (retail 0x00ce6cf0)
function Init(quest)
    quest:SetMasterGameState("PostSavePosition", 0)
    quest:SetStateInt("CoreQuestWaiting", 0)
    quest:AddRumourCategory("OV_INTRO")
    quest:AddRumourCategory("GUILD_TRAINING")
    quest:AddRumourCategory("WASP_BOSS")
    quest:AddRumourCategory("DOING_WASP_BOSS")
    quest:AddRumourCategory("VISIT_MAZE_1_GLOBAL")
    quest:AddRumourCategory("VISIT_MAZE_1_BSSLUMS")
    quest:AddRumourCategory("PRE_ORCH_FARM")
    quest:AddRumourCategory("DOING_ORCH_FARM")
    quest:AddRumourCategory("TRADER_ESCORT_GLOBAL")
    quest:AddRumourCategory("TRADER_ESCORT_BSSLUMS")
    quest:AddRumourCategory("VISIT_MAZE_2_GLOBAL")
    quest:AddRumourCategory("VISIT_MAZE_2_BSSLUMS")
    quest:AddRumourCategory("DOING_BANDIT_CAMP_GLOBAL")
    quest:AddRumourCategory("DOING_BANDIT_CAMP_BANDITCAMP")
    quest:AddRumourCategory("VISIT_MAZE_3_GLOBAL")
    quest:AddRumourCategory("VISIT_MAZE_3_GUILD")
    quest:AddRumourCategory("FIND_ARCHAEOLOGIST")
    quest:AddRumourCategory("PRE_WHITE_BALV_GLOBAL")
    quest:AddRumourCategory("PRE_WHITE_BALV_WITCHWOOD")
    quest:AddRumourCategory("DOING_WHITE_BALV_GLOBAL")
    quest:AddRumourCategory("DOING_WHITE_BALV_KHG")
    quest:AddRumourCategory("PRE_ARENA_GLOBAL")
    quest:AddRumourCategory("PRE_ARENA_KHG")
    quest:AddRumourCategory("WHISPERS_FATE")
    quest:AddRumourCategory("PRE_MCC_GLOBAL")
    quest:AddRumourCategory("PRE_MCC_BSTONES")
    quest:AddRumourCategory("DOING_MCC_GLOBAL")
    quest:AddRumourCategory("DOING_GRAVEYARD_GLOBAL")
    quest:AddRumourCategory("DOING_GRAVEYARD_GRAVEYARD")
    quest:AddRumourCategory("HOOK_COAST_GATEWAY_GLOBAL")
    quest:AddRumourCategory("HOOK_COAST_GATEWAY_DARKWOOD")
    quest:AddRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL")
    quest:AddRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST")
    quest:AddRumourCategory("AFTER_WIZARD_BATTLE_GLOBAL")
    quest:AddRumourCategory("DOING_FOCAL_SITES_GLOBAL")
    quest:AddRumourCategory("DOING_FOCAL_SITES_GUILD")
    quest:AddRumourCategory("FINAL_BATTLE_GLOBAL")
    quest:AddRumourCategory("FINAL_BATTLE_GUILD")
    quest:AddRumourCategory("AFTER_FINAL_BATTLE_GLOBAL")
    quest:AddRumourCategory("AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL")
    quest:AddRumourCategory("AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL")
    quest:AddRumourCategory("AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL")
    quest:AddRumourCategory("AFTER_FINAL_BATTLE_DIDNT_GET_SWORD_GLOBAL")
    quest:AddRumourCategory("LOOKOUT_POINT_DEMON_DOOR_READY")
    quest:AddRumourCategory("SUMMON_THE_SHIP")
    quest:AddRumourCategory("NORTHERN_WASTES_OPEN")
    quest:AddRumourCategory("SCARY_NECROPOLIS")
    quest:AddRumourCategory("NECROPOLIS_FINISHED")
    quest:AddRumourCategory("THUNDER_KILLED")
    quest:AddRumourCategory("BRIAR_ROSE_KILLED")
    quest:AddRumourCategory("GUILDMASTER_KILLED")
    quest:AddRumourCategory("NONE_KILLED")
    quest:AddRumourCategory("DRAGON_GATE_OPEN")
    quest:AddRumourCategory("SNOWSPIRE_ARRIVAL")
end

-- Gameflow.OnPersist (retail 0x00cef8e0)
function OnPersist(quest, context)
    quest:SetMasterGameState("PostSavePosition", quest:PersistTransferInt(context, "PostSavePosition", quest:GetMasterGameState("PostSavePosition") or 0))
    quest:SetStateInt("CoreQuestWaiting", quest:PersistTransferUInt(context, "CoreQuestWaiting", quest:GetStateInt("CoreQuestWaiting") or 0))
    local savedScriptNames = quest:PersistTransferStringList(context, "SavedScriptNames", {})  -- vector<CCharString> member `SavedScriptNames` (this + 0x4c)
    local savedCardDefNames = quest:PersistTransferStringList(context, "SavedCardDefNames", {})  -- vector<CCharString> member `SavedCardDefNames` (this + 0x58)
end

-- Gameflow.CoreQuestReminder (retail 0x00cef3b0)
function CoreQuestReminder(quest)
    local timerId
    if quest:IsActiveThreadTerminating() then return end
    while true do
        while quest:GetStateInt("CoreQuestWaiting") == 0 do
            if not quest:NewScriptFrame() then return end
        end
        timerId = quest:RegisterTimer()
        quest:SetTimer(timerId, quest:ReadGlobalGameData(SCRIPT_DEF.CoreQuestReminderIntervalSeconds))
        while quest:GetTimer(timerId) ~= 0 do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then break end
        local getStateInt = quest:GetStateInt("CoreQuestWaiting") ~= 0 and not quest:IsHeroOnQuest()
        if getStateInt then
            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_NEW_QUEST_AT_GUILD", "", true, false)
        end
        quest:DeregisterTimer(timerId)
        if not quest:NewScriptFrame() then return end
    end
    quest:DeregisterTimer(timerId)
end

-- Gameflow.CheckBarrowFieldsGuards (retail 0x00cef550)
function CheckBarrowFieldsGuards(quest)
    local predicateResult, predicateResult3
    if not quest:IsQuestCompleted("Q_TraderConflictEvil") then
        predicateResult = true
        if not quest:IsQuestCompleted("Q_TraderConflictGood") then goto LAB_00cef5bb end
    end
    predicateResult = false
    ::LAB_00cef5bb::
    if not predicateResult or quest:IsActiveThreadTerminating() then return end
    repeat
        if quest:IsQuestActive("Q_TraderConflictEvil") then
            goto LAB_00cef653
        else
            predicateResult3 = true
            if quest:IsQuestActive("Q_TraderConflictGood") then goto LAB_00cef653 end
        end
        goto FLOW_past_lab_00cef653
        ::LAB_00cef653::
        predicateResult3 = false
        ::FLOW_past_lab_00cef653::
        if not predicateResult3 then
            if quest:IsActiveThreadTerminating() then return end
            if not quest:IsQuestActive("Q_TraderConflictEvil") then
                return
            end
            while not quest:MsgOnQuestCompleted("Q_TraderConflictEvil") do
                if not quest:NewScriptFrame() then return end
            end
            while quest:IsRegionLoaded("BarrowFields") do
                if not quest:NewScriptFrame() then return end
            end
            while not quest:IsRegionLoaded("BarrowFields") do
                if not quest:NewScriptFrame() then return end
            end
            quest:EnableGuards(quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS"), true)
            do return end
            return
        end
        if not quest:NewScriptFrame() then return end
    until false
end

