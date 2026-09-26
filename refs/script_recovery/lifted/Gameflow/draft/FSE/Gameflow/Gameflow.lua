-- Generated native draft: Gameflow. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local bVar2, bVar3, bVar9, iVar12, iVar8, native_arg_switch_2, pCVar10, pCVar5, pCVar6, r1, uVar11, xStack_58, xStack_68, xStack_81c
    local alive = true
    quest:CreateThread("CoreQuestReminder")  -- native thread body NScript::CQ_HeroSoulsNostroScript::CNostro::Main: lift it as function CoreQuestReminder(quest)
    if not bVar9 then
    end
    quest:CreateThread("CheckBarrowFieldsGuards")  -- native thread body NScript::CGameflowScript::CheckBarrowFieldsGuards: lift it as function CheckBarrowFieldsGuards(quest)
    if not bVar9 then
    end
    bVar9 = false
    iVar8 = quest:GetMasterGameState("PostSavePosition")
    native_arg_switch_2 = iVar8
    if not (native_arg_switch_2 == 0 or native_arg_switch_2 == 100 or native_arg_switch_2 == 0x96 or native_arg_switch_2 == 200 or native_arg_switch_2 == 300 or native_arg_switch_2 == 400 or native_arg_switch_2 == 0x1c2 or native_arg_switch_2 == 500 or native_arg_switch_2 == 0x226 or native_arg_switch_2 == 600 or native_arg_switch_2 == 700 or native_arg_switch_2 == 800 or native_arg_switch_2 == 0x352 or native_arg_switch_2 == 0x358 or native_arg_switch_2 == 0x361 or native_arg_switch_2 == 0x366 or native_arg_switch_2 == 0x36b or native_arg_switch_2 == 900 or native_arg_switch_2 == 1000 or native_arg_switch_2 == 0x41a or native_arg_switch_2 == 0x44c or native_arg_switch_2 == 0x4b0 or native_arg_switch_2 == 0x4e2 or native_arg_switch_2 == 0x514 or native_arg_switch_2 == 0x5aa or native_arg_switch_2 == 0x5dc or native_arg_switch_2 == 0x60e or native_arg_switch_2 == 0x640 or native_arg_switch_2 == 0x6a4 or native_arg_switch_2 == 0x76c or native_arg_switch_2 == 0x834 or native_arg_switch_2 == 0x8fc or native_arg_switch_2 == 0x960 or native_arg_switch_2 == 0x9c4 or native_arg_switch_2 == 0xa28 or native_arg_switch_2 == 0xaf0) then
        native_arg_switch_2 = 0x7ffffffe
    end
    repeat
        if native_arg_switch_2 == 0 then
            quest:SetMasterGameState("PostSavePosition", 0)
            quest:SetAllSoundsAsMuted(true)
            bVar2 = quest:IsXbox()
            if not bVar2 then
                quest:GiveHeroObject("OBJECT_TATTOO_CARD_CHEST_CUSTOM_01", -1, true)
                quest:GiveHeroObject("OBJECT_TATTOO_CARD_BACK_CUSTOM_01", -1, true)
                quest:GiveHeroObject("OBJECT_TATTOO_CARD_ARMS_CUSTOM_01", -1, true)
                quest:GiveHeroObject("OBJECT_TATTOO_CARD_LEGS_CUSTOM_01", -1, true)
                quest:GiveHeroObject("OBJECT_TATTOO_CARD_FACE_CUSTOM_01", -1, true)
            end
            quest:AddLogbookStoryEntry(10)
            quest:SetAllowScreenFadingOnNextRegionChange(false)
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_OAKVALE_INTRO", "Q_NewOakValeIntro", false)
            bVar2 = quest:MsgOnQuestCompleted("Q_NewOakValeIntro")
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:MsgOnQuestCompleted("Q_NewOakValeIntro")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            xStack_81c = {}
            table.insert(xStack_81c, "Hook_Fresco_07_OakValeRaid")
            table.insert(xStack_81c, "Hook_Fresco_09_TimePassing")
            table.insert(xStack_81c, "Hook_Fresco_10_UneasyAlliance")
            table.insert(xStack_81c, "Q_GuildTraining")
            quest:ActivateMultipleQuestsWithoutLoadingResources(xStack_81c)
            quest:AddLogbookStoryEntry(30)
            quest:AddNewRumourToCategory("GUILD_TRAINING", "TEXT_AI_GOSSIP_GUILD_TRAINING_GUILD")
            quest:SetCategoryActivity("GUILD_TRAINING", true)
            quest:AddGossipVillage("GUILD_TRAINING", "VILLAGE_GUILD_COMPLEX_INSIDE")
            xStack_81c = {}
            table.insert(xStack_81c, "CreatureGenerators")
            table.insert(xStack_81c, "Q_ArenaHoldingScript")
            table.insert(xStack_81c, "V_AssassinAttacks")
            table.insert(xStack_81c, "V_BanditToll")
            table.insert(xStack_81c, "V_TravellingHeroes")
            table.insert(xStack_81c, "V_BeardyBaldy")
            table.insert(xStack_81c, "V_BodyGuard")
            table.insert(xStack_81c, "Q_BowerstoneTownLifeIntro")
            table.insert(xStack_81c, "V_ChapelOfEvil")
            table.insert(xStack_81c, "V_DemonDoors")
            table.insert(xStack_81c, "V_Fisherman")
            table.insert(xStack_81c, "V_FisticuffsClub")
            table.insert(xStack_81c, "V_HauntedHouse")
            table.insert(xStack_81c, "Q_HerosOldHouse")
            table.insert(xStack_81c, "V_HiddenBooty")
            table.insert(xStack_81c, "V_RandomPopulationSim")
            table.insert(xStack_81c, "Q_RansomVictimChiefsHouse")
            table.insert(xStack_81c, "V_RockTrollFirstEncounter")
            table.insert(xStack_81c, "V_StatueMaster")
            table.insert(xStack_81c, "V_SwordInTheStone")
            table.insert(xStack_81c, "V_TempleOfLight")
            quest:ActivateMultipleQuestsWithoutLoadingResources(xStack_81c)
            native_arg_switch_2 = 100
        end
        if native_arg_switch_2 == 100 then
            quest:SetMasterGameState("PostSavePosition", 100)
            bVar2 = quest:MsgOnQuestCompleted("Q_GuildTraining")
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:MsgOnQuestCompleted("Q_GuildTraining")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            quest:DeactivateQuest("Hook_Fresco_10_UneasyAlliance", 0)
            quest:AddLogbookStoryEntry(50)
            bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:IsLevelLoaded("HeroGuildComplex")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsMelee", 0)
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
            quest:DeactivateQuestLater("Q_GuildTrainingPreMelee", 0)
            quest:DeactivateQuestLater("Q_GuildTrainingMelee", 0)
            quest:DeactivateQuestLater("Q_GuildTrainingWill", 0)
            quest:DeactivateQuestLater("Q_GuildTrainingSkill", 0)
            quest:DeactivateQuestLater("Q_GuildTrainingDeparture", 0)
            pCVar5 = quest:GetThingWithScriptName("TheRealGuildmaster")
            bVar2 = (pCVar5 ~= nil and pCVar5:IsAlive())
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                uVar11 = false
                pCVar5 = quest:GetThingWithScriptName("M_GuildmasterMarker")
                pCVar6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                quest:EntityTeleportToThing(pCVar6, pCVar5, uVar11)
            end
            quest:SetHeroGuideToShowQuestCardsWhenSpokenTo(true)
            r1 = quest:GetThingWithScriptName("GuildDoors")
            iVar8 = (r1 ~= nil and r1:IsAlive())
            if iVar8 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                quest:SetThingAsUsable(r1, true)
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
            bVar2 = false
            pCVar5 = quest:GetThingWithScriptName("WitchwoodTeleporter")
            quest:SetTeleporterAsActive(pCVar5, bVar2)
            quest:RemoveRumourCategory("GUILD_TRAINING")
            quest:AddNewRumourToCategory("WASP_BOSS", "TEXT_AI_GOSSIP_WASP_BOSS_GLOBAL")
            quest:SetCategoryActivity("WASP_BOSS", true)
            native_arg_switch_2 = 0x96
        end
        if native_arg_switch_2 == 0x96 then
            quest:SetMasterGameState("PostSavePosition", 0x96)
            quest:AutoSave()
            bVar2 = quest:IsQuestActive("Q_WaspBoss")
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:IsQuestActive("Q_WaspBoss")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            quest:SetStateInt("CoreQuestWaiting", 0)
            bVar2 = quest:DisplayTutorial(0x1b)
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:MsgIsTutorialClickedPast()
                while not bVar2 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    bVar2 = quest:MsgIsTutorialClickedPast()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
            end
            quest:RemoveRumourCategory("WASP_BOSS")
            quest:AddNewRumourToCategory("DOING_WASP_BOSS", "TEXT_AI_GOSSIP_DOING_WASP_BOSS_GLOBAL")
            quest:SetCategoryActivity("DOING_WASP_BOSS", true)
            native_arg_switch_2 = 200
        end
        if native_arg_switch_2 == 200 then
            quest:SetMasterGameState("PostSavePosition", 200)
            bVar2 = quest:MsgOnQuestCompleted("Q_WaspBoss")
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:MsgOnQuestCompleted("Q_WaspBoss")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            bVar2 = quest:DisplayTutorial(0x1d)
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:MsgIsTutorialClickedPast()
                while not bVar2 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    bVar2 = quest:MsgIsTutorialClickedPast()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
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
            native_arg_switch_2 = 300
        end
        if native_arg_switch_2 == 300 then
            quest:SetMasterGameState("PostSavePosition", 300)
            quest:AutoSave()
            bVar2 = quest:MsgOnQuestCompleted("QS_GuardianSisterInfo")
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:MsgOnQuestCompleted("QS_GuardianSisterInfo")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
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
            native_arg_switch_2 = 400
        end
        if native_arg_switch_2 == 400 then
            quest:SetMasterGameState("PostSavePosition", 400)
            quest:AutoSave()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            while true do
                bVar2 = quest:IsQuestActive("Q_OrchardFarmRaidGood")
                if bVar2 then break end
                bVar2 = quest:IsQuestActive("Q_OrchardFarmRaidEvil")
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        return
                    end
                    quest:RemoveQuestCardFromGuild("Q_OrchardFarmRaidGood")
                    goto LAB_00ce933c
                end
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            quest:RemoveQuestCardFromGuild("Q_OrchardFarmRaidEvil")
            ::LAB_00ce933c::
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            quest:SetStateInt("CoreQuestWaiting", 0)
            quest:AddLogbookTutorialEntry("TEXT_QST_LOG_GUILD_BOASTS")
            quest:RemoveRumourCategory("PRE_ORCH_FARM")
            quest:AddNewRumourToCategory("DOING_ORCH_FARM", "TEXT_AI_GOSSIP_DOING_ORCH_FARM_GLOBAL")
            quest:SetCategoryActivity("DOING_ORCH_FARM", true)
            native_arg_switch_2 = 0x1c2
        end
        if native_arg_switch_2 == 0x1c2 then
            quest:SetMasterGameState("PostSavePosition", 0x1c2)
            quest:AutoSave()
            repeat
                bVar2 = quest:MsgOnQuestCompleted("Q_OrchardFarmRaidEvil")
                if bVar2 then
                    goto LAB_00ce94b6
                else
                    bVar9 = true
                    bVar2 = quest:MsgOnQuestCompleted("Q_OrchardFarmRaidGood")
                    if bVar2 then goto LAB_00ce94b6 end
                    bVar2 = true
                end
                goto FLOW_past_lab_00ce94b6
                ::LAB_00ce94b6::
                bVar2 = false
                ::FLOW_past_lab_00ce94b6::
                if bVar9 then
                    bVar9 = false
                end
                if not bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar9 = not alive
                    if bVar9 then
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
                    native_arg_switch_2 = 500
                    goto FLOW_chain_next_3
                end
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
            until false
            native_arg_switch_2 = 500
        end
        ::FLOW_chain_next_3::
        if native_arg_switch_2 == 500 then
            -- FLOW_native_label_1: (native jump target)
            quest:SetMasterGameState("PostSavePosition", 500)
            quest:AutoSave()
            bVar9 = quest:IsQuestActive("Q_TraderEscort")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:IsQuestActive("Q_TraderEscort")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:SetStateInt("CoreQuestWaiting", 0)
            native_arg_switch_2 = 0x226
        end
        if native_arg_switch_2 == 0x226 then
            quest:SetMasterGameState("PostSavePosition", 0x226)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("Q_TraderEscort")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_TraderEscort")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_MAZE_AGAIN", "", true, true)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
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
            native_arg_switch_2 = 600
        end
        if native_arg_switch_2 == 600 then
            quest:SetMasterGameState("PostSavePosition", 600)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("QS_GuardianSisterInfo2_SisterInBanditCamp")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("QS_GuardianSisterInfo2_SisterInBanditCamp")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
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
            native_arg_switch_2 = 700
        end
        if native_arg_switch_2 == 700 then
            quest:SetMasterGameState("PostSavePosition", 700)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("Q_BanditCamp")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_BanditCamp")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
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
            native_arg_switch_2 = 800
        end
        if native_arg_switch_2 == 800 then
            quest:SetMasterGameState("PostSavePosition", 800)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("QS_GuardianTrophyDealerInfo")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("QS_GuardianTrophyDealerInfo")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:ActivateQuestWithoutLoadingResources("Hook_BalverinesInKHGPreWhiteBalv")
            quest:RemoveRumourCategory("VISIT_MAZE_3_GLOBAL")
            quest:RemoveRumourCategory("VISIT_MAZE_3_GUILD")
            quest:AddNewRumourToCategory("FIND_ARCHAEOLOGIST", "TEXT_AI_GOSSIP_FIND_ARCHAEOLOGIST_GLOBAL")
            quest:SetCategoryActivity("FIND_ARCHAEOLOGIST", true)
            native_arg_switch_2 = 0x352
        end
        if native_arg_switch_2 == 0x352 then
            quest:SetMasterGameState("PostSavePosition", 0x352)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("V_TrophyDealer")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("V_TrophyDealer")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
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
            native_arg_switch_2 = 0x358
        end
        if native_arg_switch_2 == 0x358 then
            quest:SetMasterGameState("PostSavePosition", 0x358)
            quest:AutoSave()
            quest:SetStateInt("CoreQuestWaiting", 1)
            bVar9 = false
            bVar2 = quest:IsQuestActive("Q_WhiteBalverineKnotholeGlade")
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:IsRegionLoaded("KnotholeGlade")
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar2 then
                    if bVar3 then
                        return
                    end
                    if not bVar9 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar9 = not alive
                        if bVar9 then
                            return
                        end
                        quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_KNOTHOLE_GLADE_CLOSED", "", true, true)
                        bVar9 = true
                    end
                else
                    if bVar3 then
                        return
                    end
                    bVar9 = false
                end
                bVar2 = quest:IsQuestActive("Q_WhiteBalverineKnotholeGlade")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
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
            native_arg_switch_2 = 0x361
        end
        if native_arg_switch_2 == 0x361 or native_arg_switch_2 == 0x366 then
            quest:SetMasterGameState("PostSavePosition", 0x366)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("Q_WhiteBalverineKnotholeGlade")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_WhiteBalverineKnotholeGlade")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
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
            native_arg_switch_2 = 0x36b
        end
        if native_arg_switch_2 == 0x36b then
            quest:SetMasterGameState("PostSavePosition", 0x36b)
            quest:AutoSave()
            bVar9 = quest:IsQuestActive("Q_Arena")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:IsQuestActive("Q_Arena")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:SetStateInt("CoreQuestWaiting", 0)
            native_arg_switch_2 = 900
        end
        if native_arg_switch_2 == 900 then
            quest:SetMasterGameState("PostSavePosition", 900)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("Q_Arena")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_Arena")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            iVar8 = 0xaf
            if not quest:GetMasterGameState("WhisperKilledByHero") then
                iVar8 = 0xaa
            end
            quest:AddLogbookStoryEntry(iVar8)
            quest:AddLogbookStoryEntry(180)
            quest:ActivateQuest("Hook_Fresco_05_MothersStory")
            quest:ActivateQuest("Hook_Fresco_13_KilledScorpion")
            quest:ActivateQuest("V_GhostGrannyNecklace")
            quest:ActivateQuest("V_MayorsInvitation")
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
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
            native_arg_switch_2 = 1000
        end
        if native_arg_switch_2 == 1000 then
            quest:SetMasterGameState("PostSavePosition", 1000)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("QS_MeetSister")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("QS_MeetSister")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:AddLogbookStoryEntry(190)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
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
            native_arg_switch_2 = 0x41a
        end
        if native_arg_switch_2 == 0x41a then
            quest:SetMasterGameState("PostSavePosition", 0x41a)
            quest:AutoSave()
            bVar9 = quest:IsQuestActive("Q_MinionClifftopChase")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:IsQuestActive("Q_MinionClifftopChase")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:SetStateInt("CoreQuestWaiting", 0)
            quest:RemoveRumourCategory("PRE_MCC_GLOBAL")
            quest:RemoveRumourCategory("PRE_MCC_STONES")
            quest:AddNewRumourToCategory("DOING_MCC_GLOBAL", "TEXT_AI_GOSSIP_DOING_MCC_GLOBAL")
            quest:SetCategoryActivity("DOING_MCC_GLOBAL", true)
            native_arg_switch_2 = 0x44c
        end
        if native_arg_switch_2 == 0x44c then
            quest:SetMasterGameState("PostSavePosition", 0x44c)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("Q_MinionClifftopChase")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_MinionClifftopChase")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:AddQuestCard("OBJECT_QUEST_CARD_HANGING_TREE_EVIL", "Q_HangingTreeEvil", false, false)
            quest:AddQuestCard("OBJECT_QUEST_CARD_HANGING_TREE_GOOD", "Q_HangingTreeGood", false, false)
            quest:AddLogbookStoryEntry(200)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GRAVEYARD_SECRET_PASSAGE", "Q_OpeningGraveyardSecretPassage", false)
            quest:RemoveRumourCategory("DOING_MCC_GLOBAL")
            quest:AddNewRumourToCategory("DOING_GRAVEYARD_GLOBAL", "TEXT_AI_GOSSIP_DOING_GRAVEYARD_GLOBAL")
            quest:SetCategoryActivity("DOING_GRAVEYARD_GLOBAL", true)
            quest:SetCategoryActivity("DOING_GRAVEYARD_GRAVEYARD", true)
            quest:AddQuestCard("OBJECT_QUEST_CARD_MINION_CAMP", "Q_MinionCamp", false, false)
            native_arg_switch_2 = 0x4b0
        end
        if native_arg_switch_2 == 0x4b0 then
            quest:SetMasterGameState("PostSavePosition", 0x4b0)
            quest:AutoSave()
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            bVar9 = quest:MsgOnQuestCompleted("Q_OpeningGraveyardSecretPassage")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_OpeningGraveyardSecretPassage")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:AddQuestCard("OBJECT_QUEST_CARD_BOUNTY_HUNT", "Q_BountyHunt", false, false)
            quest:RemoveRumourCategory("DOING_GRAVEYARD_GLOBAL")
            quest:RemoveRumourCategory("DOING_GRAVEYARD_GRAVEYARD")
            quest:RemoveRumourCategory("WHISPERS_FATE")
            native_arg_switch_2 = 0x4e2
        end
        if native_arg_switch_2 == 0x4e2 then
            quest:SetMasterGameState("PostSavePosition", 0x4e2)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("Q_PrisonEscapeRescueMother")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_PrisonEscapeRescueMother")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
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
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_GATEWAY_TO_HOOK_COAST", "Q_EndGame", false)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            native_arg_switch_2 = 0x514
        end
        if native_arg_switch_2 == 0x514 then
            quest:SetMasterGameState("PostSavePosition", 0x514)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("Q_EndGame")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_EndGame")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            native_arg_switch_2 = 0x5aa
        end
        if native_arg_switch_2 == 0x5aa then
            quest:SetMasterGameState("PostSavePosition", 0x5aa)
            quest:SetStateInt("CoreQuestWaiting", 1)
            bVar9 = false
            bVar2 = quest:IsQuestActive("Q_WizardBattle")
            while not bVar2 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:IsRegionLoaded("HeroGuildComplexInside")
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar2 then
                    if bVar3 then
                        return
                    end
                    if not bVar9 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar9 = not alive
                        if bVar9 then
                            return
                        end
                        quest:AutoSave()
                        quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MOTHER_LEFT_YOU_THIS", "", true, true)
                        bVar9 = true
                    end
                else
                    if bVar3 then
                        return
                    end
                    bVar9 = false
                end
                bVar2 = quest:IsQuestActive("Q_WizardBattle")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
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
            native_arg_switch_2 = 0x5dc
        end
        if native_arg_switch_2 == 0x5dc then
            quest:SetMasterGameState("PostSavePosition", 0x5dc)
            bVar9 = quest:MsgOnQuestCompleted("Q_WizardBattle")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_WizardBattle")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:RemoveRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_GLOBAL")
            quest:RemoveRumourCategory("IN_HOOK_COAST_POST_DRAGON_PRE_BATTLE_HOOKCOAST")
            quest:AddNewRumourToCategory("AFTER_WIZARD_BATTLE_GLOBAL", "TEXT_AI_GOSSIP_AFTER_WIZARD_BATTLE_GLOBAL")
            quest:SetCategoryActivity("AFTER_WIZARD_BATTLE_GLOBAL", true)
            quest:ActivateQuest("Hook_Fresco_10_UneasyAlliance")
            native_arg_switch_2 = 0x60e
        end
        if native_arg_switch_2 == 0x60e then
            quest:SetMasterGameState("PostSavePosition", 0x60e)
            quest:AutoSave()
            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_GO_TO_WITCHWOOD_FOR_FOCAL_SITES", "", true, true)
            bVar9 = quest:IsRegionLoaded("HeroGuildComplexInside")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:IsRegionLoaded("HeroGuildComplexInside")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
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
            native_arg_switch_2 = 0x640
        end
        if native_arg_switch_2 == 0x640 then
            quest:SetMasterGameState("PostSavePosition", 0x640)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("Q_EndGameFocalSites")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_EndGameFocalSites")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:AddLogbookStoryEntry(280)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
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
            native_arg_switch_2 = 0x6a4
        end
        if native_arg_switch_2 == 0x6a4 then
            quest:SetMasterGameState("PostSavePosition", 0x6a4)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("Q_EndGameBossBattle")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_EndGameBossBattle")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            bVar9 = quest:IsQuestActive("Q_EndGameBossBattle")
            if bVar9 then
                repeat
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar9 = not alive
                    if bVar9 then
                        return
                    end
                    bVar9 = quest:IsQuestActive("Q_EndGameBossBattle")
                until not (bVar9)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:RemoveRumourCategory("FINAL_BATTLE_GLOBAL")
            quest:RemoveRumourCategory("FINAL_BATTLE_GUILD")
            quest:AddNewRumourToCategory("AFTER_FINAL_BATTLE_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_GLOBAL")
            quest:AddNewRumourToCategory("AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL")
            quest:AddNewRumourToCategory("AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL")
            quest:AddNewRumourToCategory("AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL")
            quest:AddNewRumourToCategory("AFTER_FINAL_BATTLE_DIDNT_GET_SWORD_GLOBAL", "TEXT_AI_GOSSIP_AFTER_FINAL_BATTLE_NOTGOT_SWORD_GLOBAL")
            quest:SetCategoryActivity("AFTER_FINAL_BATTLE_GLOBAL", true)
            iVar8 = quest:GetMasterGameState("JackBossBattleResult")
            if iVar8 == 1 then
                quest:SetCategoryActivity("AFTER_FINAL_BATTLE_KILLED_SISTER_GLOBAL", true)
                quest:SetCategoryActivity("AFTER_FINAL_BATTLE_GOT_SWORD_GLOBAL", true)
                quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_305")
            else
                if iVar8 == 2 then
                    quest:SetCategoryActivity("AFTER_FINAL_BATTLE_SPARED_SISTER_GLOBAL", true)
                    quest:SetCategoryActivity("AFTER_FINAL_BATTLE_DIDNT_GET_SWORD_GLOBAL", true)
                    quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_300")
                    goto LAB_00ced66c
                end
            end
            ::LAB_00ced66c::
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
            bVar9 = quest:IsRegionLoaded("OakvaleMemorialGarden")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:IsRegionLoaded("OakvaleMemorialGarden")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_FIRE_HEART", "Q_FireHeart", false)
            native_arg_switch_2 = 0x76c
        end
        if native_arg_switch_2 == 0x76c then
            quest:SetMasterGameState("PostSavePosition", 0x76c)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("Q_FireHeart")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_FireHeart")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:ActivateQuest("V_GuildMaster")
            quest:RemoveRumourCategory("LOOKOUT_POINT_DEMON_DOOR_READY")
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SUMMONING_THE_SHIP", "Q_SummoningTheShip", false)
            if quest:GetMasterGameState("JackBossBattleResult") == 2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_MAZE_RESEARCH", "V_MazeResearch", false)
            end
            quest:AddNewRumourToCategory("SUMMON_THE_SHIP", "TEXT_AI_GOSSIP_SUMMON_THE_SHIP")
            quest:SetCategoryActivity("SUMMON_THE_SHIP", true)
            quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_310")
            native_arg_switch_2 = 0x834
        end
        if native_arg_switch_2 == 0x834 then
            quest:SetMasterGameState("PostSavePosition", 0x834)
            quest:AutoSave()
            quest:FadeScreenIn()
            bVar9 = quest:MsgOnQuestCompleted("Q_SummoningTheShip")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_SummoningTheShip")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            bVar9 = quest:IsRegionLoaded("LostBay")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:IsRegionLoaded("LostBay")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
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
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            native_arg_switch_2 = 0x8fc
        end
        if native_arg_switch_2 == 0x8fc then
            quest:SetMasterGameState("PostSavePosition", 0x8fc)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("Q_AwakeningTheOracle")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_AwakeningTheOracle")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_SCYTHE_INFO", "QS_ScytheInfo", false)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            native_arg_switch_2 = 0x960
        end
        if native_arg_switch_2 == 0x960 then
            quest:SetMasterGameState("PostSavePosition", 0x960)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("QS_ScytheInfo")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("QS_ScytheInfo")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_HERO_SOULS", "Q_HeroSouls", false)
            quest:RemoveRumourCategory("SCARY_NECROPOLIS")
            quest:AddNewRumourToCategory("NECROPOLIS_FINISHED", "TEXT_AI_GOSSIP_NECROPOLIS_FINISHED")
            quest:SetCategoryActivity("NECROPOLIS_FINISHED", true)
            quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_330")
            quest:ActivateQuest("V_Oracle")
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            native_arg_switch_2 = 0x9c4
        end
        if native_arg_switch_2 == 0x9c4 then
            quest:SetMasterGameState("PostSavePosition", 0x9c4)
            quest:AutoSave()
            bVar9 = quest:MsgOnQuestCompleted("Q_HeroSouls")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_HeroSouls")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:AddNewRumourToCategory("DRAGON_GATE_OPEN", "TEXT_AI_GOSSIP_DRAGON_GATE_OPEN")
            quest:SetCategoryActivity("DRAGON_GATE_OPEN", true)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_DRAGON_BOSS", "Q_DragonBossFight", false)
            native_arg_switch_2 = 0xa28
        end
        if native_arg_switch_2 == 0xa28 then
            quest:SetMasterGameState("PostSavePosition", 0xa28)
            quest:AutoSave()
            quest:FadeScreenIn()
            bVar9 = quest:MsgOnQuestCompleted("Q_DragonBossFight")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:MsgOnQuestCompleted("Q_DragonBossFight")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            bVar9 = quest:IsQuestActive("Q_DragonBossFight")
            if bVar9 then
                repeat
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar9 = not alive
                    if bVar9 then
                        return
                    end
                    bVar9 = quest:IsQuestActive("Q_DragonBossFight")
                until not (bVar9)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:ActivateQuest("Hook_Fresco_12_KilledDragon")
            quest:SetRegionTextDisplayAsActive(false)
            quest:SetAllowScreenFadingOnNextRegionChange(false)
            quest:FadeScreenOutUntilNextCallToFadeScreenIn(1.0, 0.0)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            uVar11 = false
            pCVar5 = quest:GetThingWithScriptName("FrescoDomeHSP")
            pCVar6 = quest:GetHero()
            quest:EntityTeleportToThing(pCVar6, pCVar5, uVar11)
            bVar9 = quest:IsLevelLoaded("FrescoDome")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:IsLevelLoaded("FrescoDome")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            xStack_68 = resources:NewResource()
            resources:PrepareResource(xStack_68)
            iVar12 = 4
            pCVar10 = xStack_68
            pCVar5 = quest:GetHero()
            bVar9 = resources:TryAcquire(pCVar10, pCVar5, iVar12)
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    resources:ReleaseResource(xStack_68)
                    return
                end
                iVar12 = 4
                pCVar10 = xStack_68
                pCVar5 = quest:GetHero()
                bVar9 = resources:TryAcquire(pCVar10, pCVar5, iVar12)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                -- LAB_00cef03c: (native jump target)
                resources:ReleaseResource(xStack_68)
                return
            end
            xStack_81c = resources:NewActorMap()
            resources:SetActor(xStack_81c, "Hero", xStack_68)
            xStack_58 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_FABLE_CREDITS", xStack_81c, false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_58)
            resources:DestroyActorMap(xStack_81c)
            resources:ReleaseResource(xStack_68)
            uVar11 = false
            pCVar5 = quest:GetThingWithScriptName("NW3BronzeDoorHSP")
            pCVar6 = quest:GetHero()
            quest:EntityTeleportToThing(pCVar6, pCVar5, uVar11)
            bVar9 = quest:IsRegionLoaded("NorthernWastes3")
            while not bVar9 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                if bVar9 then
                    return
                end
                bVar9 = quest:IsRegionLoaded("NorthernWastes3")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            quest:SetMasterGameState("BodyGuardsInLimbo", false)
            quest:TeleportAllFollowersToHeroPosition()
            quest:SetRegionTextDisplayAsActive(true)
            quest:SetAllowScreenFadingOnNextRegionChange(true)
            quest:SetMasterGameState("PostSavePosition", 0xaf0)
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
            native_arg_switch_2 = 0xaf0
        end
        if native_arg_switch_2 == 0xaf0 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar9 = not alive
                bVar9 = not bVar9
            until not (bVar9)
            return
        end
        if native_arg_switch_2 == 0x7ffffffe then
            return
        end
    until not (false)
end

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

function OnPersist(quest, context)
    local postSavePosition = quest:GetMasterGameState("PostSavePosition") or 0
    postSavePosition = quest:PersistTransferInt(context, "PostSavePosition", postSavePosition)
    quest:SetMasterGameState("PostSavePosition", postSavePosition)
    local coreQuestWaiting = quest:GetStateInt("CoreQuestWaiting") or 0
    coreQuestWaiting = quest:PersistTransferUInt(context, "CoreQuestWaiting", coreQuestWaiting)
    quest:SetStateInt("CoreQuestWaiting", coreQuestWaiting)
    local savedScriptNames = quest:PersistTransferStringList(context, "SavedScriptNames", {})  -- vector<CCharString> member `SavedScriptNames` (this + 0x4c)
    local savedCardDefNames = quest:PersistTransferStringList(context, "SavedCardDefNames", {})  -- vector<CCharString> member `SavedCardDefNames` (this + 0x58)
end

function CoreQuestReminder(quest)
    local iVar2, iVar3, i_stk_4
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    local bVar1 = not alive
    if bVar1 then
        return
    end
    while true do
        iVar2 = quest:GetStateInt("CoreQuestWaiting")
        while iVar2 == 0 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            iVar2 = quest:GetStateInt("CoreQuestWaiting")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        iVar2 = quest:RegisterTimer()
        i_stk_4 = iVar2
        quest:SetTimer(i_stk_4, quest:ReadGlobalGameData(0xfc8))
        iVar3 = quest:GetTimer(i_stk_4)
        while iVar3 ~= 0 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                quest:DeregisterTimer(i_stk_4)
                return
            end
            iVar3 = quest:GetTimer(i_stk_4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then break end
        local __native_condition_1 = quest:GetStateInt("CoreQuestWaiting") ~= 0
        if __native_condition_1 then
            bVar1 = quest:IsHeroOnQuest()
            __native_condition_1 = not bVar1
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                quest:DeregisterTimer(i_stk_4)
                return
            end
            quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_NEW_QUEST_AT_GUILD", "", true, false)
        end
        quest:DeregisterTimer(i_stk_4)
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
    end
    quest:DeregisterTimer(i_stk_4)
end

function CheckBarrowFieldsGuards(quest)
    local bVar2, bVar3, bVar4, native_arg_sequence_1, pVillage
    local alive = true
    bVar4 = false
    bVar2 = quest:IsQuestCompleted("Q_TraderConflictEvil")
    if not bVar2 then
        bVar4 = true
        bVar3 = quest:IsQuestCompleted("Q_TraderConflictGood")
        bVar2 = true
        if not bVar3 then goto LAB_00cef5bb end
    end
    bVar2 = false
    ::LAB_00cef5bb::
    if bVar4 then
    end
    bVar4 = false
    native_arg_sequence_1 = false
    if not bVar2 then
        native_arg_sequence_1 = true
    else
        native_arg_sequence_1 = false
    end
    if not native_arg_sequence_1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
    end
    if native_arg_sequence_1 then
        return
    end
    repeat
        bVar2 = quest:IsQuestActive("Q_TraderConflictEvil")
        if bVar2 then
            goto LAB_00cef653
        else
            bVar4 = true
            bVar3 = quest:IsQuestActive("Q_TraderConflictGood")
            bVar2 = true
            if bVar3 then goto LAB_00cef653 end
        end
        goto FLOW_past_lab_00cef653
        ::LAB_00cef653::
        bVar2 = false
        ::FLOW_past_lab_00cef653::
        if bVar4 then
            bVar4 = false
        end
        if not bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar4 = quest:IsQuestActive("Q_TraderConflictEvil")
            if not bVar4 then
                return
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar4 = quest:MsgOnQuestCompleted("Q_TraderConflictEvil")
            while not bVar4 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
                bVar4 = quest:MsgOnQuestCompleted("Q_TraderConflictEvil")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar4 = quest:IsRegionLoaded("BarrowFields")
            if bVar4 then
                repeat
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        return
                    end
                    bVar4 = quest:IsRegionLoaded("BarrowFields")
                until not (bVar4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            bVar4 = quest:IsRegionLoaded("BarrowFields")
            while not bVar4 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
                bVar4 = quest:IsRegionLoaded("BarrowFields")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                bVar4 = true
                pVillage = quest:GetThingWithScriptName("VILLAGE_BARROWFIELDS")
                quest:EnableGuards(pVillage, bVar4)
                return
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
    until false
end

