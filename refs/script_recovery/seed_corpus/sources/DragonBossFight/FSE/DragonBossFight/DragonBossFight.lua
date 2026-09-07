-- DragonBossFight.lua
-- Main quest script for the Jack of Blades Dragon Boss Fight

local QUEST_NAME = "DragonBossFight"

-- Dragon Boss States
local DS_HEALTH_HIGH     = 0
local DS_HEALTH_MEDIUM   = 1
local DS_HEALTH_LOW      = 2
local DS_HEALTH_VERY_LOW = 3
local DS_DEAD            = 4

-- Music enum from EMusicSetType
local MUSIC_SET_BOSS = 0x17 -- 23 decimal (MUSIC_SET_BOSS)

function Init(questObject)
    Quest = questObject
    Quest:Log("DragonBossFight: Init() started.")

    Quest:SetStateInt("DragonState", DS_HEALTH_HIGH)
    Quest:SetStateInt("TargetNumMinions", 2)
    Quest:SetStateInt("TargetNumSummoners", 0)
    Quest:SetStateInt("NumMinions", 0)
    Quest:SetStateInt("NumSummoners", 0)
    Quest:SetStateInt("NumFlyBysBetweenSummonerSpawns", 1)
    Quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", 0)
    Quest:SetStateBool("MinionSpawningEnabled", false)
    Quest:SetStateBool("SummonerSpawningEnabled", false)
    Quest:SetStateInt("NextSummonerSpawnPoint", 1)

    Quest:AddQuestRegion(QUEST_NAME, "NorthernWastes3")
    Quest:AddQuestRegion(QUEST_NAME, "DragonCliff2")
    Quest:Log("DragonBossFight: Region 'DragonCliff2' added.")
end

function OnPersist(questObject, context)
    Quest = questObject
    local dragonState = Quest:GetStateInt("DragonState") or DS_HEALTH_HIGH
    dragonState = Quest:PersistTransferInt(context, "DragonState", dragonState)
    Quest:SetStateInt("DragonState", dragonState)
end

function Main(questObject)
    Quest = questObject
    Quest:Log("DragonBossFight: Main() started. Binding entities...")

    Quest:AddEntityBinding("Dragon", "DragonBossFight/Entities/Dragon")
    Quest:FinalizeEntityBindings()

    Quest:CreateThread("DoMission")
    Quest:Log("DragonBossFight: Entity bindings finalized and DoMission thread spawned.")
end

function SpawnMinions(quest, minionSpawnPoints)
    local target = quest:GetStateInt("TargetNumMinions") or 2
    local current = quest:GetStateInt("NumMinions") or 0
    local toSpawn = target - current
    quest:Log("SpawnMinions: target=" .. tostring(target) .. ", current=" .. tostring(current) .. ", toSpawn=" .. tostring(toSpawn))
    if toSpawn <= 0 or not minionSpawnPoints or #minionSpawnPoints == 0 then return end

    for i = 1, toSpawn do
        local spIdx = ((i - 1) % #minionSpawnPoints) + 1
        local sp = minionSpawnPoints[spIdx]
        if sp then
            local pos = sp:GetPos()
            quest:Log("SpawnMinions [" .. tostring(i) .. "]: pos={" .. tostring(pos.x) .. "," .. tostring(pos.y) .. "," .. tostring(pos.z) .. "}")
            quest:CreateEffectAtPos("SUMMON_CREATURE_SUMMON_EVIL", pos)
            quest:Log("SpawnMinions [" .. tostring(i) .. "]: Effect created. Calling CreateCreature...")
            quest:CreateCreature("CREATURE_MINION_WARDOG", pos, "")
            quest:Log("SpawnMinions [" .. tostring(i) .. "]: CreateCreature completed.")
        end
    end
    quest:SetStateInt("NumMinions", target)
    quest:Log("SpawnMinions: Finished. NumMinions updated to " .. tostring(target))
end

function SpawnSummoners(quest, summonerSpawnPoints)
    local target = quest:GetStateInt("TargetNumSummoners") or 0
    local current = quest:GetStateInt("NumSummoners") or 0
    local toSpawn = target - current
    quest:Log("SpawnSummoners: target=" .. tostring(target) .. ", current=" .. tostring(current) .. ", toSpawn=" .. tostring(toSpawn))
    if toSpawn <= 0 or not summonerSpawnPoints or #summonerSpawnPoints == 0 then return end

    local nextIdx = quest:GetStateInt("NextSummonerSpawnPoint") or 1
    for i = 1, toSpawn do
        local sp = summonerSpawnPoints[nextIdx]
        if sp then
            local creatureName = (math.random(1, 2) == 1) and "CREATURE_SUMMONER_01" or "CREATURE_SUMMONER_02"
            local pos = sp:GetPos()
            quest:Log("SpawnSummoners [" .. tostring(i) .. "]: Spawning " .. creatureName .. " at spawn point index " .. tostring(nextIdx))
            quest:CreateEffectAtPos("SUMMON_CREATURE_SUMMON_EVIL", pos)
            quest:Log("SpawnSummoners [" .. tostring(i) .. "]: Effect created. Calling CreateCreature...")
            quest:CreateCreature(creatureName, pos, "")
            quest:Log("SpawnSummoners [" .. tostring(i) .. "]: CreateCreature completed.")
        end
        nextIdx = (nextIdx % #summonerSpawnPoints) + 1
    end
    quest:SetStateInt("NextSummonerSpawnPoint", nextIdx)
    quest:SetStateInt("NumSummoners", target)
    quest:Log("SpawnSummoners: Finished. NumSummoners updated to " .. tostring(target))
end

function RunEnemySpawning(quest)
    quest:Log("DragonBossFight: RunEnemySpawning thread started.")
    local minionSpawnPoints = quest:GetAllThingsWithScriptName("DBMinionSpawn")
    local summonerSpawnPoints = quest:GetAllThingsWithScriptName("DBSummonerSpawn")
    local minionTimer = quest:RegisterTimer()
    local summonerTimer = quest:RegisterTimer()

    while quest:GetStateInt("DragonState") ~= DS_DEAD do
        if not quest:NewScriptFrame() then return end

        if quest:GetStateBool("MinionSpawningEnabled") and quest:GetTimer(minionTimer) == 0 then
            quest:Log("RunEnemySpawning: Spawning minions...")
            SpawnMinions(quest, minionSpawnPoints)
            quest:SetStateBool("MinionSpawningEnabled", false)
        end

        if quest:GetStateBool("SummonerSpawningEnabled") and quest:GetTimer(summonerTimer) == 0 then
            quest:Log("RunEnemySpawning: Spawning summoners...")
            SpawnSummoners(quest, summonerSpawnPoints)
            quest:SetStateBool("SummonerSpawningEnabled", false)
        end
    end

    quest:DeregisterTimer(minionTimer)
    quest:DeregisterTimer(summonerTimer)
    quest:Log("DragonBossFight: RunEnemySpawning thread finished.")
end

function JackTaunts(quest)
    quest:Log("DragonBossFight: JackTaunts thread started.")
    local hero = quest:GetHero()
    local tauntsTimer = quest:RegisterTimer()
    quest:SetTimer(tauntsTimer, 20)
    local tauntIdx = 10

    while quest:GetStateInt("DragonState") ~= DS_DEAD do
        if not quest:NewScriptFrame() then return end

        if quest:GetTimer(tauntsTimer) == 0 then
            if hero then
                local convoID = quest:StartAmbientConversation(hero, hero)
                if convoID and convoID ~= -1 then
                    local lineKey = "TEXT_QST_B05_JACK_TAUNT_" .. tostring(tauntIdx)
                    quest:Log("JackTaunts: Playing taunt '" .. lineKey .. "'")
                    quest:AddLineToConversation(convoID, lineKey, hero, hero, false)
                    tauntIdx = tauntIdx + 10
                    if tauntIdx >= 60 then
                        tauntIdx = 10
                    end
                end
            end
            quest:SetTimer(tauntsTimer, 20)
        end
    end

    quest:DeregisterTimer(tauntsTimer)
    quest:Log("DragonBossFight: JackTaunts thread finished.")
end

function DoMission(quest)
    quest:Log("DragonBossFight: DoMission thread started.")

    local activeQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_DRAGON_BOSS_OBJECTIVE_01", "DragonCliff2", "NorthernWastes3")

    -- Wait until DragonCliff2 is loaded
    while not quest:IsRegionLoaded("DragonCliff2") do
        if not quest:NewScriptFrame() then return end
    end

    quest:Log("DragonBossFight: DragonCliff2 loaded. Setting up boss arena...")
    quest:FadeScreenOutUntilNextCallToFadeScreenIn(0.5, 0.0)

    quest:SetMasterGameState("BodyGuardsInLimbo", true)

    local followers = quest:GetAllCreaturesExcludingHero()
    if followers then
        for i = 1, #followers do
            quest:EntitySetInLimbo(followers[i], true)
        end
    end

    local hero = quest:GetHero()

    quest:PlayCutscene("CS_DRAGON_INTRO", { HERO = hero })
    quest:OverrideMusic(MUSIC_SET_BOSS, false, false)

    quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_DRAGON_BOSS_OBJECTIVE_02", "DragonCliff2", "NorthernWastes3")
    quest:KickOffQuestStartScreen(activeQuestName, true, false)

    quest:FadeScreenIn()
    quest:SetTeleportingAsActive(false)
    quest:DisplayQuestInfo(true)

    local dragon = quest:GetThingWithScriptName("Dragon")
    if dragon then
        quest:AddQuestInfoBarHealth(dragon, { red = 0, green = 255, blue = 0 }, "HUD_QUEST_ICON_DRAGON", 1.0)
    else
        quest:Log("!!! WARNING: DragonBossFight - Dragon thing not found for health bar.")
    end

    -- Spawn dedicated concurrent threads for enemy spawning and Jack taunts
    quest:CreateThread("RunEnemySpawning")
    quest:CreateThread("JackTaunts")

    quest:Log("DragonBossFight: Entering main combat monitoring loop in DoMission...")

    while quest:GetStateInt("DragonState") ~= DS_DEAD do
        if not quest:NewScriptFrame() then return end

        local dragonThing = quest:GetThingWithScriptName("Dragon")
        if dragonThing then
            local hp = quest:GetHealth(dragonThing)
            if hp <= 0 or dragonThing:IsDead() then
                quest:Log("DoMission: Dragon defeated! (HP=" .. tostring(hp) .. ", IsDead=" .. tostring(dragonThing:IsDead()) .. ")")
                quest:SetStateInt("DragonState", DS_DEAD)
                break
            end
        end
    end

    quest:Log("DragonBossFight: Dragon defeated! Starting outro sequences...")
    quest:SetTeleportingAsActive(true)

    -- Clean up any remaining minions / summoners in the arena
    local minions = quest:GetAllThingsWithDefName("CREATURE_MINION_WARDOG")
    if minions then
        for i = 1, #minions do
            quest:RemoveThing(minions[i])
        end
    end
    local sum1 = quest:GetAllThingsWithDefName("CREATURE_SUMMONER_01")
    if sum1 then
        for i = 1, #sum1 do
            quest:RemoveThing(sum1[i])
        end
    end
    local sum2 = quest:GetAllThingsWithDefName("CREATURE_SUMMONER_02")
    if sum2 then
        for i = 1, #sum2 do
            quest:RemoveThing(sum2[i])
        end
    end

    quest:PlayCutscene("CS_DRAGON_DEATH", { HERO = hero })

    quest:SetCutsceneActionMode(true, "TEXT_QST_B05_MASK_CHOICE")
    quest:StartCutscene({ HERO = hero }, { PutMaskOn = false })
    quest:RunCutscene("CS_DRAGON_OUTRO_CHOICE")
    quest:SetCutsceneActionMode(false, "")

    local putMaskOn = quest:GetCutsceneFlag("PutMaskOn")
    quest:EndCutscene()

    if putMaskOn then
        quest:Log("DragonBossFight: Player chose EVIL branch (Wear Mask).")
        quest:PlayCutscene("CS_DRAGON_OUTRO_EVIL", { HERO = hero })
        quest:AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_385")
        quest:SetMasterGameState("HeroWoreMask", true)

        if followers then
            for i = 1, #followers do
                quest:EntitySetInLimbo(followers[i], false)
            end
        end

        quest:SetQuestAsCompleted(activeQuestName, true, false, false)

        for i = 1, 4 do
            if not quest:NewScriptFrame() then return end
        end

        if quest:GetHeroMorality() >= 0.5 then
            quest:PlayAVIMovie("Data\\Video\\dragon_good_mask.xmv")
        else
            quest:PlayAVIMovie("Data\\Video\\dragon_evil_mask.xmv")
        end

        quest:GiveHeroMorality(-1.0)
        quest:StopOverrideMusic(false)
        quest:SetHUDEnabled(true)
        quest:DisplayQuestInfo(true)
        quest:DeactivateQuestLater(activeQuestName, 0)
    else
        quest:Log("DragonBossFight: Player chose GOOD branch (Throw Mask).")
        quest:PlayCutscene("CS_DRAGON_OUTRO_GOOD", { HERO = hero })
        quest:AddLogbookStoryEntryString("TEXT_QST_LOG_STORY_PLATINUM_380")
        quest:SetMasterGameState("HeroWoreMask", false)

        if followers then
            for i = 1, #followers do
                quest:EntitySetInLimbo(followers[i], false)
            end
        end

        quest:SetQuestAsCompleted(activeQuestName, true, false, false)

        for i = 1, 4 do
            if not quest:NewScriptFrame() then return end
        end

        if quest:GetHeroMorality() >= 0.5 then
            quest:PlayAVIMovie("Data\\Video\\dragon_good_no_mask.xmv")
        else
            quest:PlayAVIMovie("Data\\Video\\dragon_evil_no_mask.xmv")
        end

        quest:GiveHeroMorality(0.5)
        quest:StopOverrideMusic(false)
        quest:SetHUDEnabled(true)
        quest:DisplayQuestInfo(true)
        quest:DeactivateQuestLater(activeQuestName, 0)
    end

    quest:Log("DragonBossFight: DoMission finished and quest deactivated.")
end
