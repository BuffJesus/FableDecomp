-- Readable native conversion: Q_WaspBoss. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_NOT_PAUSED = 2  -- ECutsceneBehaviour (Ego_r.pdb)
local TUTORIAL_CATEGORY_CAMERA = 6  -- ETutorialCategory (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    WB_LowHealth = 3636,  -- 4
    WB_CreaturesSpawned = 3644,  -- 'CREATURE_HORNET_PICNIC'
    WB_WaspBossName = 3648,  -- 'CREATURE_HORNET_QUEEN_01'
    WB_ExperienceReward = 3672,  -- 800
}

local function __native_all_dead(list)
    local down = 0
    for _, thing in ipairs(list) do
        if not thing:IsAlive() or thing:IsUnconscious() then down = down + 1 end
    end
    return down == #list
end

-- Q_WaspBoss.Main (retail 0x00e0ea40)
function Main(quest)
    quest:SetTimer(quest:GetStateInt("ReachedWaspHelper"), 120)
    while not quest:IsRegionLoaded("LookoutPoint") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:Pause(1.0)
    quest:GiveHeroTutorial(TUTORIAL_CATEGORY_CAMERA)
    quest:AddEntityBinding("GratefulVillagerSpawn", "WaspBoss/Entities/GratefulVillagerSpawn", 1)
    quest:AddEntityBinding("WaspChaser", "WaspBoss/Entities/WaspChaser", 1)
    quest:AddEntityBinding("WaspChaseWoman", "WaspBoss/Entities/WaspChaseWoman", 1)
    quest:AddEntityBinding("WaspAttacker", "WaspBoss/Entities/WaspAttacker", 1)
    quest:AddEntityBinding("WaspVictim", "WaspBoss/Entities/WaspVictim", 1)
    quest:AddEntityBinding("FleeingWoman", "WaspBoss/Entities/FleeingWoman", 1)
    quest:AddEntityBinding("WaspHelper", "WaspBoss/Entities/WaspHelper", 1)
    quest:AddEntityBinding("QueenHornet", "WaspBoss/Entities/QueenHornet", 1)
    quest:AddEntityBinding("HornetDrone", "WaspBoss/Entities/HornetDrone", 1)
    quest:FinalizeEntityBindings()
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_WASP_MENACE_OBJECTIVE_01", "", "HeroGuildComplexInside")
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_028_GUILDSEAL_WASP_MAP", "", true, true)
    while not quest:IsLevelLoaded("PicnicArea") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:OverrideMusic(23, false, false)
    quest:CreateThread("WatchForTermination")  -- native thread body CV_ChapelOfEvilScript::EndMission: lift it as function WatchForTermination(quest)
    quest:CreateThread("DoMission")  -- native thread body CQ_WaspBossScript::DoMission: lift it as function DoMission(quest)
    quest:CreateThread("WatchForCutscene")  -- native thread body CScriptGameResourceObjectScriptedThingBase_HandleQueenHornetCutscene: lift it as function WatchForCutscene(quest)
end

-- Q_WaspBoss.Init (retail 0x00e0e820)
function Init(quest)
    quest:SetStateInt("ReachedWaspHelper", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:AddQuestRegion("Q_WaspBoss", "PicnicArea")
    quest:SetQuestWorldMapOffset("Q_WaspBoss", 10, nil --[[missing]])
    quest:SetStateBool("PanickedVillagersScene", false)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("DronesCreated", false)
    quest:SetStateBool("SaidLineOnce", false)
    quest:SetStateBool("QueenHornetAttacks", false)
    quest:SetStateBool("CutsceneFinished", false)
    quest:SetStateBool("StartChase", false)
    quest:SetStateBool("QuestStartScreened", false)
    quest:SetStateInt("SavedVillagerCount", 0)
    quest:SetTimer(quest:GetStateInt("ReachedWaspHelper"), 0)
end

-- Q_WaspBoss.OnPersist (retail 0x00e0e9a0)
function OnPersist(quest, context)
    quest:SetStateBool("QuestStartScreened", quest:PersistTransferBool(context, "QuestStartScreened", quest:GetStateBool("QuestStartScreened")))
    quest:SetStateInt("SavedVillagerCount", quest:PersistTransferInt(context, "SavedVillagerCount", quest:GetStateInt("SavedVillagerCount") or 0))
end

-- Q_WaspBoss.WatchForTermination (retail 0x00e0f1d0)
function WatchForTermination(quest)
    local missionFailed = quest:GetStateBool("MissionFailed")
    while not missionFailed and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    if not missionFailed then
        quest:ResetPlayerCreatureCombatMultiplier()
        quest:GiveHeroExperience(quest:ReadGlobalGameData(SCRIPT_DEF.WB_ExperienceReward))
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
        quest:StopOverrideMusic(false)
        quest:FadeScreenIn()
        while quest:IsLevelLoaded("PicnicArea") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_078_GM_MSG_FIRST", "", true, true)
    else
        quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "", true)
        quest:StopOverrideMusic(false)
    end
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Q_WaspBoss.DoMission (retail 0x00e12580)
function DoMission(quest)
    local append_xStack_24_0, append_xStack_24_1, isActiveThreadTerminating, scratchValue
    local scratchValue2, position, scratchValue3, scratchValue4, scratchValue5, hornetDrone
    local readGlobalGameDataString, scratchValue6
    quest:AddRumourCategory("Post waspboss killed")
    quest:AddNewRumourToCategory("Post waspboss killed", "TEXT_AI_GOSSIP_WASPBOSS_KILLED")
    quest:AddGossipFactionToCategory("Post waspboss killed", "FACTION_PICNIC_AREA")
    scratchValue6 = nil
    scratchValue2 = 1
    repeat
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            scratchValue6 = nil
            goto LAB_00e12a9c
        end
        readGlobalGameDataString = quest:ReadGlobalGameDataString(SCRIPT_DEF.WB_CreaturesSpawned)
        scratchValue3 = tostring(scratchValue2)
        scratchValue3 = "QueenDepositPos" .. scratchValue3
        scratchValue4 = quest:GetThingWithScriptName(scratchValue3)
        isActiveThreadTerminating = false
        position = scratchValue4:GetPos()
        scratchValue4 = quest:CreateCreature(readGlobalGameDataString, position, "HornetDrone")
        scratchValue6 = scratchValue4
        scratchValue4 = nil
        scratchValue4 = nil
        scratchValue2 = scratchValue2 + 1
    until scratchValue2 >= 6
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    if isActiveThreadTerminating then
        scratchValue6 = nil
        scratchValue6 = 0
        return
    end
    hornetDrone = quest:GetAllThingsWithScriptName("HornetDrone")
    append_xStack_24_0 = quest:GetAllThingsWithScriptName("WaspChaser")
    for _, appendedThing in ipairs(append_xStack_24_0) do hornetDrone[#hornetDrone + 1] = appendedThing end
    append_xStack_24_1 = quest:GetAllThingsWithScriptName("WaspAttacker")
    for _, appendedThing in ipairs(append_xStack_24_1) do hornetDrone[#hornetDrone + 1] = appendedThing end
    WaspIntro(quest)
    scratchValue2 = __native_all_dead(hornetDrone)
    scratchValue = scratchValue2
    while true do
        if scratchValue then
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then return end
            quest:Pause(4.0)
            readGlobalGameDataString = quest:ReadGlobalGameDataString(SCRIPT_DEF.WB_WaspBossName)
            scratchValue4 = quest:GetThingWithScriptName("MK_WQ_STARTING")
            isActiveThreadTerminating = false
            position = scratchValue4:GetPos()
            scratchValue5 = quest:CreateCreature(readGlobalGameDataString, position, "QueenHornet")
            quest:SetStateBool("QueenHornetAttacks", true)
            quest:CreateThread("GuildmasterHelp")  -- native thread body CScriptGameResourceObjectScriptedThingBase_HandleQueenHornetDialogue: lift it as function GuildmasterHelp(quest)
            quest:NewScriptFrame()
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if not isActiveThreadTerminating and not quest:GetStateBool("MissionFailed") then
                helper_E13310(quest)
            end
            return
        end
        quest:NewScriptFrame()
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then break end
        scratchValue2 = __native_all_dead(hornetDrone)
        scratchValue = scratchValue2
    end
    scratchValue6 = nil
    ::LAB_00e12a9c::
    scratchValue6 = 0
end

-- Q_WaspBoss.WatchForCutscene (retail 0x00e12330)
function WatchForCutscene(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local queenHornetAttacks = quest:GetStateBool("QueenHornetAttacks")
    while true do
        if queenHornetAttacks then
            if quest:IsActiveThreadTerminating() then return end
            local queenHornet = quest:GetThingWithScriptName("QueenHornet")
            quest:EntitySetCutsceneBehaviour(queenHornet, CUTSCENE_BEHAVIOUR_NOT_PAUSED)
            local resource = resources:NewResource()
            resources:TryAcquire(resource, hero, 4)
            local actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "HERO", resource)
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_WASPBOSS_QUEEN", actorMap, false, true)
            quest:EntitySetFacingAngleTowardsThing(hero, queenHornet, true)
            quest:CameraDefault()
            quest:SetStateBool("CutsceneFinished", true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            return
        end
        if not quest:NewScriptFrame() then break end
        queenHornetAttacks = quest:GetStateBool("QueenHornetAttacks")
    end
end

-- Q_WaspBoss.GuildmasterHelp (retail 0x00e12b30)
function GuildmasterHelp(quest)
    local fret_00
    while not quest:GetStateBool("CutsceneFinished") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local queenHornet = quest:GetThingWithScriptName("QueenHornet")
    if not quest:NewScriptFrame() then return end
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_10", "", true, true)
    while quest:EntityGetBossPhase(queenHornet) ~= 3 do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:Pause(1.0)
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_20", "", true, true)
    while quest:EntityGetBossPhase(queenHornet) ~= 4 do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:Pause(1.0)
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_30", "", true, true)
    while quest:EntityGetBossPhase(queenHornet) ~= 6 do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:Pause(1.0)
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_40", "", true, true)
    local fret_0 = quest:GetHealth(queenHornet)
    if quest:ReadGlobalGameData(SCRIPT_DEF.WB_LowHealth) ~= fret_0 then
        repeat
            if not quest:NewScriptFrame() then return end
            fret_00 = quest:GetHealth(queenHornet)
        until quest:ReadGlobalGameData(SCRIPT_DEF.WB_LowHealth) == fret_00
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:Pause(1.0)
    quest:HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_50", "", true, true)
end

-- Q_WaspBoss.WaspIntro (retail 0x00e12f20)
-- E12F20: bsim names this body NScript::CQ_WaspBossScript::WaspIntro (a homologous script member); no PDB name
function WaspIntro(quest)
    local resources = quest:RetailResources()
    quest:SetStateBool("StartChase", true)
    local waspVictim = quest:GetThingWithScriptName("WaspVictim")
    local resource = resources:NewResource()
    local resource2 = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
    resources:TryAcquire(resource2, waspVictim, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "VICTIM", resource2)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resources:RunMacro("CS_WASPBOSS_INTRO", actorMap, false, true)
    if not quest:GetStateBool("QuestStartScreened") then
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource2)
            resources:ReleaseResource(resource)
            return
        end
        quest:KickOffQuestStartScreen(quest:GetActiveQuestName(), true, false)
        quest:SetStateBool("QuestStartScreened", true)
    end
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource2)
    resources:ReleaseResource(resource)
end

-- Q_WaspBoss.helper_E13310 (retail 0x00e13310)
function helper_E13310(quest)
    local predicateResult, healthBar, queenHornet
    queenHornet = quest:GetThingWithScriptName("QueenHornet")
    quest:DisplayQuestInfo(true)
    while not quest:GetStateBool("QueenHornetAttacks") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_WASP_MENACE_OBJECTIVE_02", "", "HeroGuildComplexInside")
    healthBar = quest:AddQuestInfoBarHealth(queenHornet, {R = 255, G = 255, B = 0, A = 255}, "HUD_ICON_WASP_HEAD", 1.0)
    while true do
        while true do
            if not quest:NewScriptFrame() then return end
            predicateResult = false
            if not quest:IsLevelLoaded("PicnicArea") then break end
            if (queenHornet ~= nil and not queenHornet:IsNull()) and queenHornet ~= nil and queenHornet:MsgIsKilledBy("") then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayQuestInfo(false)
                    quest:RemoveQuestInfoElement(healthBar)
                    helper_E137B0(quest)
                    quest:SetStateBool("MissionSucceeded", true)
                end
                do return end
            end
        end
        if predicateResult then break end
        while not quest:IsLevelLoaded("PicnicArea") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then break end
        queenHornet = quest:GetThingWithScriptName("QueenHornet")
        quest:DisplayQuestInfo(true)
        healthBar = quest:AddQuestInfoBarHealth(queenHornet, {R = 255, G = 255, B = 0, A = 255}, "HUD_ICON_WASP_HEAD", 1.0)
    end
end

-- Q_WaspBoss.helper_E137B0 (retail 0x00e137b0)
function helper_E137B0(quest)
    local resources = quest:RetailResources()
    if not quest:NewScriptFrame() then return end
    quest:SetStateBool("PanickedVillagersScene", true)
    local resource = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:Pause(3.0)
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(0.5)
    local getNearestWithDefName = quest:GetNearestWithDefName(quest:GetThingWithScriptName("QueenDepositPos1"), "CREATURE_HORNET_QUEEN_01")
    if getNearestWithDefName ~= nil then
        if getNearestWithDefName ~= nil and getNearestWithDefName:IsAlive() then
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:ReleaseResource(resource)
                return
            end
            quest:RemoveThing(getNearestWithDefName, false, true)
        end
    end
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:RunMacro("CS_WASPBOSS_OUTRO", actorMap, false, true)
    resources:DestroyActorMap(actorMap)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:ReleaseResource(resource)
    quest:EntitySetInFaction(quest:GetThingWithScriptName("VILL1"), "FACTION_PICNIC_AREA")
    quest:EntitySetInFaction(quest:GetThingWithScriptName("VILL2"), "FACTION_PICNIC_AREA")
    quest:EntitySetInFaction(quest:GetThingWithScriptName("VILL3"), "FACTION_PICNIC_AREA")
end

