Quest = nil

local QUEST_NAME = "Q_WaspBoss"

function Init(questObject)
    Quest = questObject
    Quest:Log("Q_WaspBoss: Init() started.")

    Quest:AddQuestRegion(QUEST_NAME, "PicnicArea")
    Quest:SetQuestWorldMapOffset(QUEST_NAME, 10, 10)

    Quest:SetStateBool("MissionSucceeded", false)
    Quest:SetStateBool("MissionFailed", false)
    Quest:SetStateBool("CutsceneFinished", false)
    Quest:SetStateBool("StartChase", false)
    Quest:SetStateBool("QuestStartScreened", false)
    Quest:SetStateBool("QueenHornetAttacks", false)
    Quest:SetStateBool("PanickedVillagersScene", false)
    Quest:SetStateInt("SavedVillagerCount", 0)
    Quest:SetStateInt("ReachedWaspHelperTimer", 120)
    Quest:SetStateInt("ReachedWaspHelperTimerID", -1)

    Quest:Log("Q_WaspBoss: Init() finished.")
end

function OnPersist(questObject, context)
    Quest = questObject
    Quest:Log("Q_WaspBoss: OnPersist() triggered.")

    local questStartScreened = Quest:GetStateBool("QuestStartScreened") or false
    questStartScreened = Quest:PersistTransferBool(context, "QuestStartScreened", questStartScreened)
    Quest:SetStateBool("QuestStartScreened", questStartScreened)

    local savedVillagerCount = Quest:GetStateInt("SavedVillagerCount") or 0
    savedVillagerCount = Quest:PersistTransferInt(context, "SavedVillagerCount", savedVillagerCount)
    Quest:SetStateInt("SavedVillagerCount", savedVillagerCount)
end

function WatchForCutscene(quest)
    quest:Log("Q_WaspBoss: WatchForCutscene thread started.")

    while not quest:GetStateBool("QueenHornetAttacks") do
        if not quest:IsLevelLoaded("PicnicArea") then
            quest:Log("Q_WaspBoss: WatchForCutscene exiting - PicnicArea not loaded.")
            return
        end
        if not quest:NewScriptFrame() then
            return
        end
    end

    quest:Log("Q_WaspBoss: QueenHornetAttacks detected. Starting Queen Cutscene...")

    local th_hornet = quest:GetThingWithScriptName("QueenHornet")
    if th_hornet then
        quest:EntitySetCutsceneBehaviour(th_hornet, 2) -- CUTSCENE_BEHAVIOUR_NOT_PAUSED
    end

    local hero = quest:GetHero()
    local actors = { HERO = hero }
    quest:StartCutscene(actors)
    quest:RunCutscene("CS_WASPBOSS_QUEEN", true, false)
    quest:EndCutscene()

    if th_hornet and hero then
        quest:EntitySetFacingAngleTowardsThing(hero, th_hornet)
    end

    quest:CameraDefault()
    quest:SetStateBool("CutsceneFinished", true)
    quest:Log("Q_WaspBoss: Queen cutscene finished, CutsceneFinished set to true.")
end

function GuildmasterHelp(quest)
    quest:Log("Q_WaspBoss: GuildmasterHelp thread started. Waiting for cutscene to finish...")

    while not quest:GetStateBool("CutsceneFinished") do
        if not quest:IsLevelLoaded("PicnicArea") then
            quest:Log("Q_WaspBoss: GuildmasterHelp exiting - PicnicArea not loaded.")
            return
        end
        if not quest:NewScriptFrame() then
            return
        end
    end

    quest:Log("Q_WaspBoss: Delivering initial Guildmaster guidance 10...")
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_10", "", true, true)

    local waspQueen = quest:GetThingWithScriptName("QueenHornet")
    while not waspQueen do
        if not quest:NewScriptFrame() then return end
        waspQueen = quest:GetThingWithScriptName("QueenHornet")
    end

    -- Phase 3 guidance (Q_WaspBoss.c:221-237)
    while quest:EntityGetBossPhase(waspQueen) ~= 3 do
        if not quest:NewScriptFrame() then return end
    end
    quest:Pause(1.0)
    quest:Log("Q_WaspBoss: Queen Boss Phase 3 reached. Delivering guidance line 20.")
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_20", "", true, true)

    -- Phase 4 guidance (Q_WaspBoss.c:238-254)
    while quest:EntityGetBossPhase(waspQueen) ~= 4 do
        if not quest:NewScriptFrame() then return end
    end
    quest:Pause(1.0)
    quest:Log("Q_WaspBoss: Queen Boss Phase 4 reached. Delivering guidance line 30.")
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_30", "", true, true)

    -- Phase 6 guidance (Q_WaspBoss.c:255-271)
    while quest:EntityGetBossPhase(waspQueen) ~= 6 do
        if not quest:NewScriptFrame() then return end
    end
    quest:Pause(1.0)
    quest:Log("Q_WaspBoss: Queen Boss Phase 6 reached. Delivering guidance line 40.")
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_40", "", true, true)

    -- Low health guidance: WB_LowHealth = 4 (Q_WaspBoss.c:272-297)
    while quest:GetHealth(waspQueen) > 4 do
        if not quest:NewScriptFrame() then return end
    end
    quest:Pause(1.0)
    quest:Log("Q_WaspBoss: Queen reached WB_LowHealth (<=4). Delivering guidance line 50.")
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_50", "", true, true)
end

function WaspIntro(quest)
    quest:Log("Q_WaspBoss: WaspIntro() started. Setting StartChase to true.")
    quest:SetStateBool("StartChase", true)

    local hero = quest:GetHero()
    local victim = quest:GetThingWithScriptName("WaspVictim")
    local actors = { HERO = hero }
    if victim then
        actors.VICTIM = victim
    end

    quest:Log("Q_WaspBoss: Playing CS_WASPBOSS_INTRO cutscene...")
    quest:StartCutscene(actors)
    quest:RunCutscene("CS_WASPBOSS_INTRO", true, false)
    quest:EndCutscene()

    if not quest:GetStateBool("QuestStartScreened") then
        quest:Log("Q_WaspBoss: Displaying quest start screen...")
        quest:KickOffQuestStartScreen(QUEST_NAME, true, false)
        quest:SetStateBool("QuestStartScreened", true)
    end
end

function Outro(quest)
    quest:Log("Q_WaspBoss: Outro() started.")
    local hero = quest:GetHero()
    if not hero then return end
    local actors = { HERO = hero }

    quest:StartCutscene(actors)
    quest:Pause(3.0)
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(0.5)

    local queenPos1 = quest:GetThingWithScriptName("QueenDepositPos1")
    if queenPos1 then
        local deadQueen = quest:GetNearestWithDefName(queenPos1, "CREATURE_HORNET_QUEEN_01")
        if deadQueen then
            quest:Log("Q_WaspBoss: Removing dead Queen Hornet creature from world.")
            quest:RemoveThing(deadQueen)
        end
    end

    quest:Log("Q_WaspBoss: Playing CS_WASPBOSS_OUTRO cutscene...")
    quest:RunCutscene("CS_WASPBOSS_OUTRO", true, false)
    quest:EndCutscene()

    quest:SetStateBool("PanickedVillagersScene", true)

    for i = 1, 3 do
        local vill = quest:GetThingWithScriptName("VILL" .. i)
        if vill then
            quest:EntitySetInFaction(vill, "FACTION_PICNIC_AREA")
        end
    end
    quest:Log("Q_WaspBoss: Outro() finished.")
end

function EndMission(quest)
    quest:Log("Q_WaspBoss: EndMission() started.")
    local waspQueen = quest:GetThingWithScriptName("QueenHornet")
    quest:DisplayQuestInfo(true)

    local infoBarID = -1
    if waspQueen then
        quest:Log("Q_WaspBoss: Adding Queen Hornet boss health bar.")
        infoBarID = quest:AddQuestInfoBarHealth(waspQueen, { r = 255, g = 0, b = 0 }, "HUD_ICON_WASP_HEAD", 1.0)
    end

    quest:SetQuestCardObjective(QUEST_NAME, "TEXT_QUEST_WASP_MENACE_OBJECTIVE_02", "", "HeroGuildComplexInside")

    while quest:IsLevelLoaded("PicnicArea") do
        if not waspQueen or not waspQueen:IsAlive() or quest:GetHealth(waspQueen) <= 0 then
            quest:Log("Q_WaspBoss: Queen Hornet defeated!")
            break
        end
        if not quest:NewScriptFrame() then
            return
        end
    end

    if infoBarID ~= -1 then
        quest:RemoveQuestInfoElement(infoBarID)
    end
    quest:DisplayQuestInfo(false)

    Outro(quest)
    quest:SetStateBool("MissionSucceeded", true)
    quest:Log("Q_WaspBoss: MissionSucceeded set to true.")
end

function DoMission(quest)
    quest:Log("Q_WaspBoss: DoMission thread started. Registering gossip/rumours...")

    quest:AddRumourCategory("Post waspboss killed")
    quest:AddNewRumourToCategory("Post waspboss killed", "TEXT_GOSSIP_Q_WASPBOSS_SUCCESS")
    quest:AddGossipFactionToCategory("Post waspboss killed", "FACTION_PICNIC_AREA")
    quest:AddGossipFactionToCategory("Post waspboss killed", "FACTION_BOWERSTONE_SOUTH")
    quest:AddGossipFactionToCategory("Post waspboss killed", "FACTION_HERO_HERO_GUILD")
    quest:AddGossipFactionToCategory("Post waspboss killed", "FACTION_LOOKOUT_POINT")
    quest:AddGossipFactionToCategory("Post waspboss killed", "FACTION_GREATWOOD_ENTRANCE")

    quest:AddRumourCategory("Post waspboss active")
    quest:AddNewRumourToCategory("Post waspboss active", "TEXT_GOSSIP_Q_WASPBOSS_ACCEPTED")
    quest:AddGossipFactionToCategory("Post waspboss active", "FACTION_PICNIC_AREA")
    quest:AddGossipFactionToCategory("Post waspboss active", "FACTION_BOWERSTONE_SOUTH")
    quest:AddGossipFactionToCategory("Post waspboss active", "FACTION_HERO_HERO_GUILD")
    quest:AddGossipFactionToCategory("Post waspboss active", "FACTION_LOOKOUT_POINT")
    quest:AddGossipFactionToCategory("Post waspboss active", "FACTION_GREATWOOD_ENTRANCE")
    quest:SetCategoryActivity("Post waspboss active", true)

    -- Spawn initial hornet drones
    quest:Log("Q_WaspBoss: Spawning 5 initial HornetDrones at deposit positions...")
    for i = 1, 5 do
        local posThing = quest:GetThingWithScriptName("QueenDepositPos" .. i)
        if posThing then
            local pos = posThing:GetPos()
            quest:CreateCreature("CREATURE_HORNET_PICNIC", { x = pos.x, y = pos.y, z = pos.z }, "HornetDrone")
        else
            quest:Log("!!! WARNING: QueenDepositPos" .. i .. " not found!")
        end
    end

    WaspIntro(quest)

    -- Wait until all initial drones, chasers, and attackers are defeated
    quest:Log("Q_WaspBoss: Gathering initial wasps and waiting for them to be defeated...")
    local initialWasps = {}
    local drones = quest:GetAllThingsWithScriptName("HornetDrone")
    if drones then for _, d in ipairs(drones) do table.insert(initialWasps, d) end end
    local chasers = quest:GetAllThingsWithScriptName("WaspChaser")
    if chasers then for _, c in ipairs(chasers) do table.insert(initialWasps, c) end end
    local attackers = quest:GetAllThingsWithScriptName("WaspAttacker")
    if attackers then for _, a in ipairs(attackers) do table.insert(initialWasps, a) end end

    while true do
        local anyAlive = false
        for _, wasp in ipairs(initialWasps) do
            if wasp and wasp:IsAlive() and quest:GetHealth(wasp) > 0 then
                anyAlive = true
                break
            end
        end

        if not anyAlive then
            quest:Log("Q_WaspBoss: All initial wasps defeated!")
            break
        end

        if not quest:NewScriptFrame() then
            return
        end
    end

    quest:Log("Q_WaspBoss: Pausing 4.0s before spawning Queen Hornet...")
    quest:Pause(4.0)

    -- Spawn Queen Hornet
    local queenPosThing = quest:GetThingWithScriptName("MK_WQ_STARTING")
    if queenPosThing then
        local qpos = queenPosThing:GetPos()
        quest:Log("Q_WaspBoss: Spawning CREATURE_HORNET_QUEEN_01 at MK_WQ_STARTING.")
        quest:CreateCreature("CREATURE_HORNET_QUEEN_01", { x = qpos.x, y = qpos.y, z = qpos.z }, "QueenHornet")
    else
        quest:Log("!!! ERROR: MK_WQ_STARTING not found!")
    end

    quest:SetStateBool("QueenHornetAttacks", true)
    quest:CreateThread("GuildmasterHelp")
    EndMission(quest)
end

function WatchForTermination(quest)
    quest:Log("Q_WaspBoss: WatchForTermination thread started.")

    while not quest:GetStateBool("MissionSucceeded") and not quest:GetStateBool("MissionFailed") do
        if not quest:NewScriptFrame() then
            return
        end
    end

    if quest:GetStateBool("MissionSucceeded") then
        quest:Log("Q_WaspBoss: Mission succeeded! Giving 800 XP (WB_ExperienceReward) and marking completed.")
        quest:GiveHeroExperience(800)
        quest:SetQuestAsCompleted(QUEST_NAME, true, false, false)
        quest:ResetPlayerCreatureCombatMultiplier()
        quest:StopOverrideMusic(false)
        quest:FadeScreenIn()

        quest:Log("Q_WaspBoss: Waiting for Hero to leave PicnicArea...")
        while quest:IsLevelLoaded("PicnicArea") do
            if not quest:NewScriptFrame() then
                return
            end
        end

        quest:Log("Q_WaspBoss: Hero left PicnicArea. Sending GM completion message and deactivating quest.")
        quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_FIRST", "", true, true)
        quest:DeactivateQuestLater(QUEST_NAME, 0)
    elseif quest:GetStateBool("MissionFailed") then
        quest:Log("Q_WaspBoss: Mission failed. Marking failed.")
        quest:SetQuestAsFailed(QUEST_NAME, true, true, true)
        quest:StopOverrideMusic(false)
        quest:FadeScreenIn()
        quest:DeactivateQuestLater(QUEST_NAME, 0)
    end
end

function Main(questObject)
    Quest = questObject
    Quest:Log("Q_WaspBoss: Main() started.")
    
    local reachedHelperTimer = Quest:RegisterTimer()
    Quest:SetTimer(reachedHelperTimer, 120)
    Quest:SetStateInt("ReachedWaspHelperTimerID", reachedHelperTimer)
    Quest:SetStateInt("ReachedWaspHelperTimer", 120)

    Quest:Log("Q_WaspBoss: Waiting for LookoutPoint region to load...")
    while not Quest:IsRegionLoaded("LookoutPoint") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    Quest:Log("Q_WaspBoss: LookoutPoint loaded. Pausing 1.0s and giving tutorial...")
    Quest:Pause(1.0)
    Quest:GiveHeroTutorial(1) -- TUTORIAL_CATEGORY_CAMERA

    Quest:Log("Q_WaspBoss: Setting up entity bindings...")
    Quest:AddEntityBinding("GratefulVillagerSpawn", "WaspBoss/Entities/GratefulVillagerSpawn")
    Quest:AddEntityBinding("WaspChaser",            "WaspBoss/Entities/WaspChaser")
    Quest:AddEntityBinding("WaspChaseWoman",        "WaspBoss/Entities/WaspChaseWoman")
    Quest:AddEntityBinding("WaspAttacker",          "WaspBoss/Entities/WaspAttacker")
    Quest:AddEntityBinding("WaspVictim",            "WaspBoss/Entities/WaspVictim")
    Quest:AddEntityBinding("FleeingWoman",          "WaspBoss/Entities/FleeingWoman")
    Quest:AddEntityBinding("WaspHelper",            "WaspBoss/Entities/WaspHelper")
    Quest:AddEntityBinding("QueenHornet",           "WaspBoss/Entities/QueenHornet")
    Quest:AddEntityBinding("HornetDrone",           "WaspBoss/Entities/HornetDrone")
    Quest:FinalizeEntityBindings()

    Quest:SetQuestCardObjective(QUEST_NAME, "TEXT_QUEST_WASP_MENACE_OBJECTIVE_01", "", "HeroGuildComplexInside")
    Quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_028_GUILDSEAL_WASP_MAP", "", true, true)

    Quest:Log("Q_WaspBoss: Waiting for PicnicArea level to load...")
    while not Quest:IsLevelLoaded("PicnicArea") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    Quest:Log("Q_WaspBoss: PicnicArea level loaded. Overriding music and starting threads...")
    Quest:OverrideMusic(1, false, false) -- Boss music
    Quest:CreateThread("WatchForTermination")
    Quest:CreateThread("DoMission")
    Quest:CreateThread("WatchForCutscene")

    Quest:Log("Q_WaspBoss: Main() thread setup complete.")
end
