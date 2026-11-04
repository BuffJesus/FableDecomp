-- Readable native conversion: Q_DragonBossFight. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    DBNumMinionsHighHealth = 3008,  -- 0
    DBNumMinionsMediumHealth = 3012,  -- 2
    DBNumMinionsLowHealth = 3016,  -- 2
    DBNumMinionsVeryLowHealth = 3020,  -- 3
    DBNumSummonersHighHealth = 3024,  -- 0
    DBNumSummonersMediumHealth = 3028,  -- 0
    DBNumSummonersLowHealth = 3032,  -- 1
    DBNumSummonersVeryLowHealth = 3036,  -- 1
    DBNumFlyBysBetweenSummonerSpawns = 3040,  -- 2
}

-- Q_DragonBossFight.Main (retail 0x00d255e0)
function Main(quest)
    local scratchValue
    quest:AddEntityBinding("Dragon", "DragonBossFight/Entities/Dragon")
    quest:AddEntityBinding("DBMinion", "DragonBossFight/Entities/DBMinion")
    quest:AddEntityBinding("DBSummoner", "DragonBossFight/Entities/DBSummoner")
    quest:FinalizeEntityBindings()
    quest:CreateThread("DoMission")  -- native thread body DoMission: lift it as function DoMission(quest)
    if not scratchValue then
    end
end

-- Q_DragonBossFight.Init (retail 0x00d254b0)
function Init(quest)
    quest:SetStateInt("MinionSpawnDelay", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("SummonerSpawnDelay", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("DragonState", 0)
    quest:SetStateInt("NextSummonerSpawnPoint", 0)
    quest:SetStateInt("NumMinions", 0)
    quest:SetStateInt("NumSummoners", 0)
    quest:SetStateInt("NumFlyBysBetweenSummonerSpawns", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumFlyBysBetweenSummonerSpawns))
    quest:AddQuestRegion("Q_DragonBossFight", "DragonCliff2")
end

-- Q_DragonBossFight.DoMission (retail 0x00d26190)
function DoMission(quest)
    local scratchValue2, dragonState, scratchValue3, scratchValue4, pThing, scratchValue6
    local scratchValue7, healthBar, scratchValue8, scratchValue9, scratchValue10, scratchValue11
    scratchValue3 = 0
    local getActiveQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(getActiveQuestName, "TEXT_QUEST_DRAGON_BOSS_OBJECTIVE_01", "DragonCliff2", "NorthernWastes3")
    while not quest:IsRegionLoaded("DragonCliff2") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:FadeScreenOutUntilNextCallToFadeScreenIn(0.5, 0.0)
    quest:SetMasterGameState("BodyGuardsInLimbo", true)
    local creatures = quest:GetAllCreaturesExcludingHero()
    scratchValue8 = 0
    scratchValue9 = 0
    if #creatures ~= 0 then
        repeat
            quest:EntitySetInLimbo(creatures[scratchValue3 + 1], true, true)
            scratchValue9 = #creatures
            scratchValue8 = scratchValue8 + 1
            scratchValue3 = scratchValue3 + 1
        until not (scratchValue8 < scratchValue9)
    end
    scratchValue4 = 0
    helper_D27050(quest, scratchValue9)
    quest:OverrideMusic(23, false, false)
    local getActiveQuestName2 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(getActiveQuestName2, "TEXT_QUEST_DRAGON_BOSS_OBJECTIVE_02", "DragonCliff2", "NorthernWastes3")
    local getActiveQuestName3 = quest:GetActiveQuestName()
    quest:KickOffQuestStartScreen(getActiveQuestName3, true, false)
    quest:FadeScreenIn()
    quest:SetTeleportingAsActive(false)
    quest:DisplayQuestInfo(true)
    local scratchValue5 = {R = 255, G = 0, B = 0, A = 255}
    pThing = quest:GetThingWithScriptName("Dragon")
    healthBar = quest:AddQuestInfoBarHealth(pThing, scratchValue5, "HUD_QUEST_ICON_DRAGON", 1.0)
    pThing = nil
    quest:CreateThread("RunEnemySpawning")  -- native thread body 0x00D26BA0: lift it as function RunEnemySpawning(quest)
    quest:CreateThread("JackTaunts")  -- native thread body JackTaunts: lift it as function JackTaunts(quest)
    dragonState = quest:GetStateInt("DragonState")
    while dragonState ~= 4 do
        if not quest:NewScriptFrame() then return end
        dragonState = quest:GetStateInt("DragonState")
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetTeleportingAsActive(true)
    helper_D27050(quest, dragonState)
    quest:SetCutsceneActionMode(true, "TEXT_QST_B05_MASK_CHOICE")
    helper_D27050(quest, dragonState)
    quest:SetCutsceneActionMode(false, "")
    local scratchValue = quest:RetailFlags("cs_flags"):Get("PutMaskOn")
    local predicateResult = quest:IsActiveThreadTerminating()
    if not scratchValue then
        if predicateResult then return end
        helper_D27050(quest, dragonState)
        quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_380")
        quest:SetMasterGameState("HeroWoreMask", false)
        scratchValue10 = 0
        if #creatures ~= 0 then
            repeat
                quest:EntitySetInLimbo(creatures[scratchValue4 + 1], false, true)
                scratchValue10 = scratchValue10 + 1
                scratchValue4 = scratchValue4 + 1
            until not (scratchValue10 < (#creatures))
        end
        local getActiveQuestName4 = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(getActiveQuestName4, true, false, false)
        if not quest:NewScriptFrame() then return end
        if not quest:NewScriptFrame() then return end
        if not quest:NewScriptFrame() then return end
        if not quest:NewScriptFrame() then return end
        if 0.5 <= quest:GetHeroMorality() then
            if quest:IsActiveThreadTerminating() then return end
            scratchValue6 = "Data\\Video\\dragon_good_no_mask.xmv"
        else
            if quest:IsActiveThreadTerminating() then return end
            scratchValue6 = "Data\\Video\\dragon_evil_no_mask.xmv"
        end
        quest:PlayAVIMovie(scratchValue6)
        scratchValue2 = 0.5
    else
        if predicateResult then return end
        helper_D27050(quest, dragonState)
        quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_385")
        quest:SetMasterGameState("HeroWoreMask", true)
        scratchValue11 = 0
        if #creatures ~= 0 then
            repeat
                quest:EntitySetInLimbo(creatures[scratchValue4 + 1], false, true)
                scratchValue11 = scratchValue11 + 1
                scratchValue4 = scratchValue4 + 1
            until not (scratchValue11 < (#creatures))
        end
        local getActiveQuestName5 = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(getActiveQuestName5, true, false, false)
        if not quest:NewScriptFrame() then return end
        if not quest:NewScriptFrame() then return end
        if not quest:NewScriptFrame() then return end
        if not quest:NewScriptFrame() then return end
        if 0.5 <= quest:GetHeroMorality() then
            if quest:IsActiveThreadTerminating() then return end
            scratchValue7 = "Data\\Video\\dragon_good_mask.xmv"
        else
            if quest:IsActiveThreadTerminating() then return end
            scratchValue7 = "Data\\Video\\dragon_evil_mask.xmv"
        end
        quest:PlayAVIMovie(scratchValue7)
        scratchValue2 = -1.0
    end
    quest:GiveHeroMorality(scratchValue2)
    quest:StopOverrideMusic(false)
    local getActiveQuestName6 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName6, 0)
end

-- Q_DragonBossFight.RunEnemySpawning (retail 0x00d26ba0)
function RunEnemySpawning(quest)
    quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", quest:GetStateInt("NumFlyBysBetweenSummonerSpawns") - 1)
    local dragonState = quest:GetStateInt("DragonState")
    quest:SetStateBool("MinionSpawningEnabled", false)
    quest:SetStateBool("SummonerSpawningEnabled", false)
    while true do
        if dragonState == 4 then
            quest:IsActiveThreadTerminating()
            return
        end
        if not quest:NewScriptFrame() then break end
        local getStateBool = quest:GetStateBool("MinionSpawningEnabled") and quest:GetTimer(quest:GetStateInt("MinionSpawnDelay")) == 0
        if getStateBool then
            if quest:IsActiveThreadTerminating() then return end
            SpawnMinions(quest)
            quest:SetStateBool("MinionSpawningEnabled", false)
        end
        local getStateBool2 = quest:GetStateBool("SummonerSpawningEnabled") and quest:GetTimer(quest:GetStateInt("SummonerSpawnDelay")) == 0
        if not getStateBool2 then
            dragonState = quest:GetStateInt("DragonState")
        else
            if quest:IsActiveThreadTerminating() then return end
            SpawnSummoners(quest)
            quest:SetStateBool("SummonerSpawningEnabled", false)
            dragonState = quest:GetStateInt("DragonState")
        end
    end
end

-- Q_DragonBossFight.JackTaunts (retail 0x00d26a50)
function JackTaunts(quest)
    local registerTimer, value
    registerTimer = quest:RegisterTimer()
    value = 10
    local timerId = registerTimer
    quest:SetTimer(registerTimer, 20)
    repeat
        if not quest:NewScriptFrame() then quest:DeregisterTimer(registerTimer); return end
        if quest:GetTimer(registerTimer) == 0 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registerTimer); return end
            local hero = quest:GetHero()
            local conversationId = quest:AddNewConversation(hero, false, false)
            local hero2 = quest:GetHero()
            local pSpeaker = quest:GetHero()
            local scratchValue = "TEXT_QST_B05_JACK_TAUNT_" .. tostring(value)
            quest:AddLineToConversation(conversationId, scratchValue, pSpeaker, hero2, false)
            registerTimer = timerId
            quest:SetTimer(timerId, 20)
            value = value + 10
        end
    until not (value < 60)
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registerTimer); return end
    quest:DeregisterTimer(registerTimer)
    do return end
    quest:DeregisterTimer(registerTimer)
end

-- Q_DragonBossFight.SpawnMinions (retail 0x00d26cc0)
function SpawnMinions(quest)
    local minionSpawnPointsIndex, scratchValue
    local getStateInt = quest:GetStateInt("TargetNumMinions") - quest:GetStateInt("NumMinions")
    scratchValue = 0
    if not (0 < getStateInt) then return end
    minionSpawnPointsIndex = 0
    repeat
        if quest:IsActiveThreadTerminating() then return end
        local minionSpawnPointsItem = quest:GetStateListAt("MinionSpawnPoints", minionSpawnPointsIndex):GetPos()
        quest:CreateEffectAtPos("SUMMON_CREATURE_SUMMON_EVIL", minionSpawnPointsItem, 0.0, false)
        local minionSpawnPointsItem2 = quest:GetStateListAt("MinionSpawnPoints", minionSpawnPointsIndex):GetPos()
        quest:CreateCreature("CREATURE_MINION_WARDOG", minionSpawnPointsItem2, "DBMinion", false)
        scratchValue = scratchValue + 1
        minionSpawnPointsIndex = minionSpawnPointsIndex + 1
    until not (scratchValue < getStateInt)
end

-- Q_DragonBossFight.SpawnSummoners (retail 0x00d26e60)
function SpawnSummoners(quest)
    local scratchValue, string, scratchValue3
    local getStateInt = quest:GetStateInt("TargetNumSummoners") - quest:GetStateInt("NumSummoners")
    scratchValue3 = 0
    if not (0 < getStateInt) then return end
    repeat
        if quest:IsActiveThreadTerminating() then return end
        local randomChoice = quest:RetailRandModulo(32768) & 0x80000001
        scratchValue = randomChoice == 0
        if randomChoice < 0 then
            scratchValue = (randomChoice - 1 | 0xfffffffe) == 0xffffffff
        end
        string = "CREATURE_SUMMONER_01"
        if not scratchValue then
            string = "CREATURE_SUMMONER_02"
        end
        local summonerSpawnPointsItem = quest:GetStateListAt("SummonerSpawnPoints", quest:GetStateInt("NextSummonerSpawnPoint")):GetPos()
        quest:CreateEffectAtPos("SUMMON_CREATURE_SUMMON_EVIL", summonerSpawnPointsItem, 0.0, false)
        local summonerSpawnPointsItem2 = quest:GetStateListAt("SummonerSpawnPoints", quest:GetStateInt("NextSummonerSpawnPoint")):GetPos()
        quest:CreateCreature(string, summonerSpawnPointsItem2, "DBSummoner", false)
        local nextSummonerSpawnPoint = quest:GetStateInt("NextSummonerSpawnPoint")
        quest:SetStateInt("NextSummonerSpawnPoint", nextSummonerSpawnPoint + 1)
        if not (quest:GetStateListCount("SummonerSpawnPoints") <= nextSummonerSpawnPoint + 1) then
            scratchValue3 = scratchValue3 + 1
        else
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("NextSummonerSpawnPoint", 0)
            scratchValue3 = scratchValue3 + 1
        end
    until not (scratchValue3 < getStateInt)
end

-- Q_DragonBossFight.SpawnEnemies (retail 0x00d25c20)
-- D25C20: bsim names this body NScript::CQ_DragonBossFightScript::SpawnEnemies (a homologous script member); no PDB name
function SpawnEnemies(quest)
    local numFlyBysSinceLastSummonerSpawn = quest:GetStateInt("NumFlyBysSinceLastSummonerSpawn")
    quest:SetStateBool("MinionSpawningEnabled", true)
    quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", numFlyBysSinceLastSummonerSpawn + 1)
    if not (quest:GetStateInt("NumFlyBysBetweenSummonerSpawns") <= numFlyBysSinceLastSummonerSpawn + 1) then return end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetStateBool("SummonerSpawningEnabled", true)
    quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", 0)
end

-- Q_DragonBossFight.helper_D258C0 (retail 0x00d258c0)
function helper_D258C0(quest, param1)
    quest:SetStateInt("DragonState", param1)
    if param1 == 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumMinionsHighHealth))
        quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumSummonersHighHealth))
        return
    elseif param1 == 1 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumMinionsMediumHealth))
        quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumSummonersMediumHealth))
        return
    elseif param1 == 2 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumMinionsLowHealth))
        quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumSummonersLowHealth))
        return
    elseif param1 == 3 then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("TargetNumMinions", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumMinionsVeryLowHealth))
        quest:SetStateInt("TargetNumSummoners", quest:ReadGlobalGameData(SCRIPT_DEF.DBNumSummonersVeryLowHealth))
    end
end

-- Q_DragonBossFight.helper_D27050 (retail 0x00d27050)
function helper_D27050(quest, param1)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    local pThing = quest:GetHero()
    resources:TryAcquire(resource, pThing, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    local movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithFlags(param1, actorMap, quest:RetailFlags("cs_flags"), false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

