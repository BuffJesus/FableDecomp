-- Readable native conversion: Q_DragonBossFight. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Q_DragonBossFight.Main (retail 0x00d255e0)
function Main(quest)
    quest:AddEntityBinding("Dragon", "DragonBossFight/Entities/Dragon")
    quest:AddEntityBinding("DBMinion", "DragonBossFight/Entities/DBMinion")
    quest:AddEntityBinding("DBSummoner", "DragonBossFight/Entities/DBSummoner")
    quest:FinalizeEntityBindings()
    quest:CreateThread("DoMission")  -- native thread body DoMission: lift it as function DoMission(quest)
end

-- Q_DragonBossFight.Init (retail 0x00d254b0)
function Init(quest)
    quest:SetStateInt("MinionSpawnDelay", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("SummonerSpawnDelay", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("DragonState", 0)
    quest:SetStateInt("NextSummonerSpawnPoint", 0)
    quest:SetStateInt("NumMinions", 0)
    quest:SetStateInt("NumSummoners", 0)
    quest:SetStateInt("NumFlyBysBetweenSummonerSpawns", quest:ReadGlobalGameData(3040))
    quest:AddQuestRegion("Q_DragonBossFight", "DragonCliff2")
end

-- Q_DragonBossFight.DoMission (retail 0x00d26190)
function DoMission(quest)
    local scratchValue2, dragonState, scratchValue3, scratchValue4, scratchValue6, scratchValue7
    local scratchValue8, scratchValue9, scratchValue, scratchValue11
    scratchValue3 = 0
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_DRAGON_BOSS_OBJECTIVE_01", "DragonCliff2", "NorthernWastes3")
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
        until scratchValue8 >= scratchValue9
    end
    scratchValue4 = 0
    helper_D27050(quest, scratchValue9)
    quest:OverrideMusic(23, false, false)
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_DRAGON_BOSS_OBJECTIVE_02", "DragonCliff2", "NorthernWastes3")
    quest:KickOffQuestStartScreen(quest:GetActiveQuestName(), true, false)
    quest:FadeScreenIn()
    quest:SetTeleportingAsActive(false)
    quest:DisplayQuestInfo(true)
    quest:AddQuestInfoBarHealth(quest:GetThingWithScriptName("Dragon"), {R = 255, G = 0, B = 0, A = 255}, "HUD_QUEST_ICON_DRAGON", 1.0)
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
    local predicateResult = quest:IsActiveThreadTerminating()
    if not quest:RetailFlags("cs_flags"):Get("PutMaskOn") then
        if predicateResult then return end
        helper_D27050(quest, dragonState)
        quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_380")
        quest:SetMasterGameState("HeroWoreMask", false)
        scratchValue = 0
        if #creatures ~= 0 then
            repeat
                quest:EntitySetInLimbo(creatures[scratchValue4 + 1], false, true)
                scratchValue = scratchValue + 1
                scratchValue4 = scratchValue4 + 1
            until scratchValue >= #creatures
        end
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
        if not quest:NewScriptFrame() then return end
        if not quest:NewScriptFrame() then return end
        if not quest:NewScriptFrame() then return end
        if not quest:NewScriptFrame() then return end
        if 0.5 <= quest:GetHeroMorality() then
            scratchValue6 = "Data\\Video\\dragon_good_no_mask.xmv"
        else
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
            until scratchValue11 >= #creatures
        end
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
        if not quest:NewScriptFrame() then return end
        if not quest:NewScriptFrame() then return end
        if not quest:NewScriptFrame() then return end
        if not quest:NewScriptFrame() then return end
        if 0.5 <= quest:GetHeroMorality() then
            scratchValue7 = "Data\\Video\\dragon_good_mask.xmv"
        else
            scratchValue7 = "Data\\Video\\dragon_evil_mask.xmv"
        end
        quest:PlayAVIMovie(scratchValue7)
        scratchValue2 = -1.0
    end
    quest:GiveHeroMorality(scratchValue2)
    quest:StopOverrideMusic(false)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Q_DragonBossFight.RunEnemySpawning (retail 0x00d26ba0)
function RunEnemySpawning(quest)
    quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", quest:GetStateInt("NumFlyBysBetweenSummonerSpawns") - 1)
    local dragonState = quest:GetStateInt("DragonState")
    quest:SetStateBool("MinionSpawningEnabled", false)
    quest:SetStateBool("SummonerSpawningEnabled", false)
    while true do
        if dragonState == 4 then
            return
        end
        if not quest:NewScriptFrame() then break end
        local getStateBool = quest:GetStateBool("MinionSpawningEnabled") and quest:GetTimer(quest:GetStateInt("MinionSpawnDelay")) == 0
        if getStateBool then
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
    local value
    local hero = quest:GetHero()
    local registerTimer = quest:RegisterTimer()
    value = 10
    quest:SetTimer(registerTimer, 20)
    repeat
        if not quest:NewScriptFrame() then quest:DeregisterTimer(registerTimer); return end
        if quest:GetTimer(registerTimer) == 0 then
            quest:AddLineToConversation(quest:AddNewConversation(hero, false, false), "TEXT_QST_B05_JACK_TAUNT_" .. tostring(value), hero, hero, false)
            quest:SetTimer(registerTimer, 20)
            value = value + 10
        end
    until value >= 60
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
    if 0 >= getStateInt then return end
    minionSpawnPointsIndex = 0
    repeat
        if quest:IsActiveThreadTerminating() then return end
        quest:CreateEffectAtPos("SUMMON_CREATURE_SUMMON_EVIL", quest:GetStateListAt("MinionSpawnPoints", minionSpawnPointsIndex):GetPos(), 0.0, false)
        quest:CreateCreature("CREATURE_MINION_WARDOG", quest:GetStateListAt("MinionSpawnPoints", minionSpawnPointsIndex):GetPos(), "DBMinion")
        scratchValue = scratchValue + 1
        minionSpawnPointsIndex = minionSpawnPointsIndex + 1
    until scratchValue >= getStateInt
end

-- Q_DragonBossFight.SpawnSummoners (retail 0x00d26e60)
function SpawnSummoners(quest)
    local createEffectAtPos, creature, scratchValue4
    local getStateInt = quest:GetStateInt("TargetNumSummoners") - quest:GetStateInt("NumSummoners")
    scratchValue4 = 0
    if 0 >= getStateInt then return end
    repeat
        if quest:IsActiveThreadTerminating() then return end
        local scratchValue3 = math.random(0, 32767) & 0x80000001
        -- TODO(native): pCVar4 = (**(*(quest:GetStateListRef("SummonerSpawnPoints") + quest:GetStateInt("NextSummonerSpawnPoint") * 0xc) + 0x18))()
--[[unresolved native value]]
        createEffectAtPos = quest:CreateEffectAtPos("SUMMON_CREATURE_SUMMON_EVIL", nil --[[missing]], 0.0, false)
        -- TODO(native): pCVar4 = (**(*(quest:GetStateListRef("SummonerSpawnPoints") + quest:GetStateInt("NextSummonerSpawnPoint") * 0xc) + 0x18))()
    --[[unresolved native value]]
        creature = quest:CreateCreature(nil, nil --[[missing]], "DBSummoner")
        local nextSummonerSpawnPoint = quest:GetStateInt("NextSummonerSpawnPoint")
        quest:SetStateInt("NextSummonerSpawnPoint", nextSummonerSpawnPoint + 1)
        if quest:GetStateListCount("SummonerSpawnPoints") <= nextSummonerSpawnPoint + 1 then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateInt("NextSummonerSpawnPoint", 0)
        end
        scratchValue4 = scratchValue4 + 1
    until scratchValue4 >= getStateInt
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

-- Q_DragonBossFight.helper_D27050 (retail 0x00d27050)
function helper_D27050(quest, param1)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
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

