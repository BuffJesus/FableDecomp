-- TentacleKrakenBossFight.lua
-- Main quest script for the Tentacle Kraken Boss Fight (Q_TentacleKrakenBossFight)
-- Functional parity port of NScript::CQ_TentacleKrakenBossFightScript

local QUEST_NAME = "TentacleKrakenBossFight"
local MUSIC_SET_BOSS = 0x17 -- 23 decimal (MUSIC_SET_BOSS)
local MOTHER_SAFETY_DISTANCE = 6.0 -- MotherFollowingHeroKrakenSafetyDistance 6.0

local KRAKEN_ALIVE = 0
local KRAKEN_DEAD  = 1

function Init(quest)
    quest:Log("TentacleKrakenBossFight: Init() started.")

    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("KrakenAutoSaveCheckPoint", false)
    quest:SetStateInt("KrakenState", KRAKEN_ALIVE)

    -- MasterScriptData->MotherDodgingKraken = 0
    quest:SetMasterGameState("MotherDodgingKraken", false)

    quest:Log("TentacleKrakenBossFight: Init() completed.")
end

function OnPersist(quest, context)
    local krakenAutoSaveCheckPoint = quest:GetStateBool("KrakenAutoSaveCheckPoint") or false
    krakenAutoSaveCheckPoint = quest:PersistTransferBool(context, "KrakenAutoSaveCheckPoint", krakenAutoSaveCheckPoint)
    quest:SetStateBool("KrakenAutoSaveCheckPoint", krakenAutoSaveCheckPoint)

    local krakenState = quest:GetStateInt("KrakenState") or KRAKEN_ALIVE
    krakenState = quest:PersistTransferInt(context, "KrakenState", krakenState)
    quest:SetStateInt("KrakenState", krakenState)
end

function Main(quest)
    quest:Log("TentacleKrakenBossFight: Main() started. Binding entities...")

    quest:AddEntityBinding("KrakenMinion", "TentacleKrakenBossFight/Entities/KrakenMinion")
    quest:AddEntityBinding("M_Experience", "TentacleKrakenBossFight/Entities/M_Experience")
    quest:AddEntityBinding("TheKraken", "TentacleKrakenBossFight/Entities/TheKraken")

    quest:FinalizeEntityBindings()

    quest:CreateThread("DoMission")

    quest:Log("TentacleKrakenBossFight: Entity bindings finalized, DoMission thread spawned.")
end

function DoMission(quest)
    quest:Log("TentacleKrakenBossFight: DoMission thread started.")

    -- Wait until KrakenChamber is loaded
    while not quest:IsLevelLoaded("KrakenChamber") do
        if not quest:NewScriptFrame() then return end
    end

    quest:Log("TentacleKrakenBossFight: KrakenChamber loaded.")

    if not quest:GetStateBool("KrakenAutoSaveCheckPoint") then
        quest:SetStateBool("KrakenAutoSaveCheckPoint", true)
        quest:AutoSaveCheckPoint()
        quest:Log("TentacleKrakenBossFight: Checkpoint auto-saved.")
    end

    PhaseOne(quest)

    if not quest:NewScriptFrame() then return end

    if not quest:GetStateBool("MissionFailed") then
        quest:StopOverrideMusic(false)
        quest:SetStateBool("MissionSucceeded", true)
        quest:Log("TentacleKrakenBossFight: PhaseOne complete. MissionSucceeded = true.")
    end

    local activeQuestName = quest:GetActiveQuestName()
    quest:Log("TentacleKrakenBossFight: Deactivating quest '" .. tostring(activeQuestName) .. "'...")
    quest:DeactivateQuestLater(activeQuestName, 0)
    quest:Log("TentacleKrakenBossFight: DoMission thread finished.")
end

function PhaseOne(quest)
    quest:Log("TentacleKrakenBossFight: PhaseOne() started.")

    local hero = quest:GetHero()
    local startMissionMarker = quest:GetThingWithScriptName("TKBF_MainChamberEntranceMarker")

    if startMissionMarker and hero then
        while not quest:IsDistanceBetweenThingsUnder(hero, startMissionMarker, 10.0) do
            if not quest:NewScriptFrame() then return end
        end
    end

    quest:Log("TentacleKrakenBossFight: Hero reached entrance marker.")

    local activeQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_05", "KrakenChamber", "")

    -- Setup barrier doors and effects
    local doorBlockVec = quest:GetAllThingsWithScriptName("DoorBlock")
    local invisoDoors = {}
    local barrierEffects = {}

    if doorBlockVec then
        for i = 1, #doorBlockVec do
            local marker = doorBlockVec[i]
            local pos = marker:GetPos()
            local angle = marker:GetAngleXY()

            local invisoDoor = quest:CreateObject("OBJECT_ACTIVATION_RECEPTOR_DOOR_TEST", pos, angle, "InvisoDoor")
            if invisoDoor then
                quest:EntitySetFacingAngle(invisoDoor, angle)
                quest:EntitySetAlpha(invisoDoor, 0.0)
                table.insert(invisoDoors, invisoDoor)
            end

            local effect = quest:CreateEffectAtPos("HK_MAGIC_BARRIER", pos, angle)
            if effect then
                quest:EntitySetFacingAngle(effect, angle)
                table.insert(barrierEffects, effect)
            end
        end
    end

    quest:Log("TentacleKrakenBossFight: Barrier doors created (" .. tostring(#invisoDoors) .. " doors, " .. tostring(#barrierEffects) .. " effects).")

    -- Intro cutscene
    quest:PlayCutscene("CS_KRAKEN_INTRO", { Hero = hero })
    quest:OverrideMusic(MUSIC_SET_BOSS, false, false)

    -- Teleport Mother to torture spot
    local motherKraken = quest:GetThingWithScriptName("MK_MOTHER_KRAKEN")
    local motherTorture = quest:GetThingWithScriptName("MotherTorture")
    if motherTorture and motherKraken then
        quest:EntityTeleportToThing(motherTorture, motherKraken)
    end

    -- Re-fetch TheKraken after cutscene to ensure a fresh, valid entity pointer
    local theKraken = quest:GetThingWithScriptName("TheKraken")
    if not theKraken then
        for i = 1, 30 do
            if not quest:NewScriptFrame() then break end
            theKraken = quest:GetThingWithScriptName("TheKraken")
            if theKraken then break end
        end
    end

    -- Enable Quest Info HUD overlay so the boss health bar is rendered
    quest:DisplayQuestInfo(true)

    local krakenHealthBarId = -1
    if theKraken then
        quest:MiniMapAddMarker(theKraken, "HUD_ORB_QUEST_CORE")
        krakenHealthBarId = quest:AddQuestInfoBarHealth(theKraken, { red = 255, green = 0, blue = 0 }, "HUD_QUEST_ICON_KRAKEN", 1.0)
        quest:Log("TentacleKrakenBossFight: Added Kraken health bar (ID=" .. tostring(krakenHealthBarId) .. ") and minimap marker.")
    else
        quest:Log("!!! ERROR: TentacleKrakenBossFight - TheKraken thing not found for health bar.")
    end

    quest:SetMasterGameState("MotherDodgingKraken", true)

    -- Mother follows hero
    if motherTorture and hero then
        quest:EntityFollowThing(motherTorture, hero, MOTHER_SAFETY_DISTANCE, true)
    end

    quest:Log("TentacleKrakenBossFight: Combat started. Waiting for Kraken defeat...")

    -- Combat monitoring loop: wait until Kraken is defeated
    while quest:GetStateInt("KrakenState") ~= KRAKEN_DEAD do
        if not quest:NewScriptFrame() then return end

        if theKraken then
            local hp = quest:GetHealth(theKraken)
            if hp <= 0.0 or theKraken:IsDead() then
                quest:Log("TentacleKrakenBossFight: Kraken dead detected in DoMission loop (HP=" .. tostring(hp) .. ").")
                quest:SetStateInt("KrakenState", KRAKEN_DEAD)
                break
            end
        end
    end

    quest:Log("TentacleKrakenBossFight: The Kraken defeated! Cleaning up arena...")

    -- Clean up health bar and hide quest info overlay
    if krakenHealthBarId and krakenHealthBarId ~= -1 then
        quest:RemoveQuestInfoElement(krakenHealthBarId)
    end
    quest:DisplayQuestInfo(false)

    -- Outro cutscene
    if hero then
        quest:PlayCutscene("CS_KRAKEN_OUTRO", { Hero = hero })
    end

    -- Spawn experience orbs at all M_Experience markers (20 XP each)
    local expMarkers = quest:GetAllThingsWithScriptName("M_Experience")
    if expMarkers then
        for i = 1, #expMarkers do
            local markerPos = expMarkers[i]:GetPos()
            if markerPos then
                local orbPos = { x = markerPos.x, y = markerPos.y, z = markerPos.z + 5.0 }
                quest:CreateExperienceOrb(orbPos, 20)
            end
        end
        quest:Log("TentacleKrakenBossFight: Experience orbs spawned at " .. tostring(#expMarkers) .. " markers.")
    end

    -- Cleanup doors and effects
    for i = 1, #invisoDoors do
        quest:RemoveThing(invisoDoors[i])
    end

    for i = 1, #barrierEffects do
        local fx = barrierEffects[i]
        local pos = fx:GetPos()
        quest:CreateEffectAtPos("HK_MAGIC_BARRIER_OFF", pos)
        quest:RemoveThing(fx)
    end

    quest:SetMasterGameState("MotherDodgingKraken", false)

    quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_PRISON_ESCAPE_OBJECTIVE_06", "Graveyard", "")

    -- Wait until Hero reaches exit trigger
    local exitTrigger = quest:GetThingWithScriptName("S_ExitTrigger")
    if exitTrigger and hero then
        while not quest:IsDistanceBetweenThingsUnder(hero, exitTrigger, 6.0) do
            if not quest:NewScriptFrame() then return end
        end
    end

    quest:Log("TentacleKrakenBossFight: Exit trigger reached. Transitioning to Graveyard...")

    quest:AddQuestRegion("Q_PrisonEscapeRescueMother", "Graveyard")

    if not quest:NewScriptFrame() then return end

    local entrance = quest:GetThingWithScriptName("GraveyardFromPrisonRegionEntrance")
    if entrance and hero then
        quest:EntityTeleportToThing(hero, entrance)
        quest:Log("TentacleKrakenBossFight: Hero teleported to Graveyard entrance.")
    end

    quest:Log("TentacleKrakenBossFight: PhaseOne() completed successfully.")
end
