-- SummoningTheShip.lua
-- Main quest script for Summoning The Ship (SummoningTheShip)
-- Functional parity port of NScript::CQ_SummoningTheShipScript

local QUEST_NAME = "SummoningTheShip"
local MUSIC_SET_BOSS = 0x17

function Init(quest)
    quest:Log("SummoningTheShip: Init() started.")

    quest:AddQuestRegion(QUEST_NAME, "HookCoast")
    quest:AddQuestRegion(QUEST_NAME, "LostBay")

    quest:SetStateInt("SummonersAlive", 0)
    quest:SetStateBool("LighthouseStarted", false)
    quest:SetStateBool("SummonerAttacksStarted", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateInt("CurrentAttackWave", 1)
    quest:SetStateInt("ConversationIndex", -1)

    local commTimer = quest:RegisterTimer()
    quest:SetStateInt("CommentaryTimer", commTimer)

    quest:Log("SummoningTheShip: Init() completed.")
end

function OnPersist(quest, context)
    local summonersAlive = quest:GetStateInt("SummonersAlive") or 0
    summonersAlive = quest:PersistTransferInt(context, "SummonersAlive", summonersAlive)
    quest:SetStateInt("SummonersAlive", summonersAlive)

    local lighthouseStarted = quest:GetStateBool("LighthouseStarted") or false
    lighthouseStarted = quest:PersistTransferBool(context, "LighthouseStarted", lighthouseStarted)
    quest:SetStateBool("LighthouseStarted", lighthouseStarted)

    local summonerAttacksStarted = quest:GetStateBool("SummonerAttacksStarted") or false
    summonerAttacksStarted = quest:PersistTransferBool(context, "SummonerAttacksStarted", summonerAttacksStarted)
    quest:SetStateBool("SummonerAttacksStarted", summonerAttacksStarted)

    local missionFailed = quest:GetStateBool("MissionFailed") or false
    missionFailed = quest:PersistTransferBool(context, "MissionFailed", missionFailed)
    quest:SetStateBool("MissionFailed", missionFailed)

    local currentAttackWave = quest:GetStateInt("CurrentAttackWave") or 1
    currentAttackWave = quest:PersistTransferInt(context, "CurrentAttackWave", currentAttackWave)
    quest:SetStateInt("CurrentAttackWave", currentAttackWave)
end

function Main(quest)
    quest:Log("SummoningTheShip: Main() started. Binding entities...")

    quest:AddEntityBinding("STS_BriarRose", "SummoningTheShip/Entities/STS_BriarRose")
    quest:AddEntityBinding("SummonerAttacker", "SummoningTheShip/Entities/SummonerAttacker")
    quest:AddEntityBinding("SummonerMinion", "SummoningTheShip/Entities/SummonerMinion")
    quest:AddEntityBinding("FireHeartHolder", "SummoningTheShip/Entities/FireHeartHolder")
    quest:AddEntityBinding("FireHeart", "SummoningTheShip/Entities/FireHeart")
    quest:AddEntityBinding("M_ActivateLighthouse", "SummoningTheShip/Entities/M_ActivateLighthouse")

    quest:FinalizeEntityBindings()

    quest:CreateThread("DoMission")
    quest:Log("SummoningTheShip: DoMission thread created.")
end

function DoMission(quest)
    quest:Log("SummoningTheShip: DoMission thread started.")

    local hero = quest:GetHero()
    local activeQuestName = quest:GetActiveQuestName()

    quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_01", "HookCoast", "HookCoast")

    -- Wait until HookCoast level is loaded
    while not quest:IsLevelLoaded("HookCoast") do
        if not quest:NewScriptFrame() then return end
    end

    quest:Log("SummoningTheShip: HookCoast loaded. Initializing arena...")

    quest:SetSummonerDeathExplosionAffectsHero(false)
    quest:SetWeaponOutCrimeEnabled(false)

    local door = quest:GetThingWithScriptName("LightHouseDoor")
    if door then quest:RemoveThing(door) end

    local ship1 = quest:GetThingWithScriptName("HookCoastShip1")
    if ship1 then quest:RemoveThing(ship1) end

    local ship2 = quest:GetThingWithScriptName("HookCoastShip2")
    if ship2 then quest:RemoveThing(ship2) end

    local village = quest:GetThingWithScriptName("HookCoastVillage")
    if village then quest:SetVillageLimbo(village, true) end

    quest:PlayCutscene("CS_SUMSHIP_PREINTRO", { HERO = hero })

    quest:KickOffQuestStartScreen(activeQuestName, true, false)
    quest:FadeScreenIn()

    -- Wait until Hero activates the lighthouse (FireHeartHolder)
    while not quest:GetStateBool("LighthouseStarted") do
        if not quest:NewScriptFrame() then return end
    end

    quest:Log("SummoningTheShip: Lighthouse started. Commencing battle sequence...")

    quest:OverrideMusic(MUSIC_SET_BOSS, false, false)
    quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_02", "HookCoast", "HookCoast")

    quest:PlayCutscene("CS_SUMSHIP_INTRO", { HERO = hero })

    quest:SetStateBool("SummonerAttacksStarted", true)
    quest:OverrideMusic(MUSIC_SET_BOSS, false, false)

    -- Wave 1 monitoring loop
    while quest:GetStateInt("CurrentAttackWave") <= 1 and not quest:GetStateBool("MissionFailed") do
        if not quest:NewScriptFrame() then return end
    end

    if quest:GetStateBool("MissionFailed") then
        quest:Log("SummoningTheShip: Mission failed during Wave 1.")
        quest:PlayCutscene("CS_SUMSHIP_OUTRO_FAILED", { HERO = hero })
        quest:SetQuestAsFailed(activeQuestName, true, "TEXT_QUEST_SUMMONING_THE_SHIP_FAILED", true)
        return
    else
        quest:Log("SummoningTheShip: Wave 1 cleared. Playing Wave 2 cutscene...")
        quest:PlayCutscene("CS_SUMSHIP_WAVE2", { HERO = hero })
    end

    quest:SetSummonerDeathExplosionAffectsHero(true)

    -- Wave 2 monitoring loop: wait until all summoners are dead or mission fails
    while (quest:GetStateInt("SummonersAlive") or 0) > 0 and not quest:GetStateBool("MissionFailed") do
        if not quest:NewScriptFrame() then return end
    end

    if quest:GetStateBool("MissionFailed") then
        quest:Log("SummoningTheShip: Mission failed during Wave 2.")
        quest:PlayCutscene("CS_SUMSHIP_OUTRO_FAILED", { HERO = hero })
        quest:SetQuestAsFailed(activeQuestName, true, "TEXT_QUEST_SUMMONING_THE_SHIP_FAILED", true)
        return
    end

    quest:Log("SummoningTheShip: Combat ended. Evaluating results...")

    local briarRose = quest:GetThingWithScriptName("STS_BriarRose")
    if briarRose and briarRose:IsAlive() then
        local convoID = quest:StartAmbientConversation(briarRose, hero, false, false)
        if convoID and convoID ~= -1 then
            quest:SetStateInt("ConversationIndex", convoID)
            quest:AddLineToConversation(convoID, "TEXT_QST_B02_BRIARROSE_FIREHEART_POWERED", briarRose, hero, false)
        end
    end

    quest:StopOverrideMusic(false)

    local summonersRemain = (quest:GetStateInt("SummonersAlive") or 0) > 0
    if summonersRemain or quest:GetStateBool("MissionFailed") then
        quest:Log("SummoningTheShip: Mission failed after Wave 2.")
        quest:PlayCutscene("CS_SUMSHIP_OUTRO_FAILED", { HERO = hero })
        quest:SetQuestAsFailed(activeQuestName, true, "TEXT_QUEST_SUMMONING_THE_SHIP_FAILED", true)
    else
        quest:Log("SummoningTheShip: Mission succeeded! Playing outro cutscene and video...")
        quest:Pause(3.0)
        quest:PlayCutscene("CS_SUMSHIP_OUTRO", { HERO = hero })
        quest:PlayAVIMovie("Data\\Video\\Summoning_The_Ship.xmv")
        quest:SetQuestAsCompleted(activeQuestName, true, false, false)
    end

    -- Cleanup routines
    if village then quest:SetVillageLimbo(village, false) end
    quest:SetWeaponOutCrimeEnabled(true)
    if briarRose then quest:RemoveThing(briarRose) end
    quest:StopOverrideMusic(false)

    local lostBayHSP = quest:GetThingWithScriptName("LostBayHSP")
    if lostBayHSP then
        quest:EntityTeleportToThing(hero, lostBayHSP)
    end

    local commTimer = quest:GetStateInt("CommentaryTimer")
    if commTimer and commTimer >= 0 then
        quest:DeregisterTimer(commTimer)
    end

    quest:DeactivateQuestLater(activeQuestName, 0)
    quest:Log("SummoningTheShip: DoMission thread finished.")
end
