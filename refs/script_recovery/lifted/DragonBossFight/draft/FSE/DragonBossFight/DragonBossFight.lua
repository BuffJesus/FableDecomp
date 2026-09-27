-- Generated native draft: Q_DragonBossFight. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar2
    quest:AddEntityBinding("Dragon", "DragonBossFight/Entities/Dragon")
    quest:AddEntityBinding("DBMinion", "DragonBossFight/Entities/DBMinion")
    quest:AddEntityBinding("DBSummoner", "DragonBossFight/Entities/DBSummoner")
    quest:FinalizeEntityBindings()
    quest:CreateThread("DoMission")  -- native thread body DoMission: lift it as function DoMission(quest)
    if not bVar2 then
    end
end

function Init(quest)
    quest:SetStateInt("MinionSpawnDelay", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("SummonerSpawnDelay", quest:RegisterTimer())  -- native constructor: CTimer member
    quest:SetStateInt("DragonState", 0)
    quest:SetStateInt("NextSummonerSpawnPoint", 0)
    quest:SetStateInt("NumMinions", 0)
    quest:SetStateInt("NumSummoners", 0)
    quest:SetStateInt("NumFlyBysBetweenSummonerSpawns", quest:ReadGlobalGameData(0xbe0))
    quest:AddQuestRegion("Q_DragonBossFight", "DragonCliff2")
end

function DoMission(quest)
    local CVar15, bVar14, bVar3, cVar1, fVar16, fret_0, fret_00, iStack_18, iVar17, iVar9, pCVar13, pCVar4, pCVar5, pThing, pcVar7, r1, uVar10, uVar11
    local alive = true
    iVar9 = 0
    pCVar4 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pCVar4, "TEXT_QUEST_DRAGON_BOSS_OBJECTIVE_01", "DragonCliff2", "NorthernWastes3")
    bVar3 = quest:IsRegionLoaded("DragonCliff2")
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = quest:IsRegionLoaded("DragonCliff2")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    quest:FadeScreenOutUntilNextCallToFadeScreenIn(0.5, 0.0)
    quest:SetMasterGameState("BodyGuardsInLimbo", true)
    iStack_18 = quest:GetAllCreaturesExcludingHero()
    uVar10 = 0
    uVar11 = 0
    if #iStack_18 ~= 0 then
        repeat
            quest:EntitySetInLimbo(iStack_18[(iVar9) / 0xc + 1], true, true)
            uVar11 = #iStack_18
            uVar10 = uVar10 + 1
            iVar9 = iVar9 + 0xc
        until not (uVar10 < uVar11)
    end
    iVar9 = 0
    helper_D27050(quest, uVar11)
    quest:OverrideMusic(0x17, false, false)
    pCVar4 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pCVar4, "TEXT_QUEST_DRAGON_BOSS_OBJECTIVE_02", "DragonCliff2", "NorthernWastes3")
    CVar15 = 0x0
    bVar3 = true
    pCVar5 = quest:GetActiveQuestName()
    quest:KickOffQuestStartScreen(pCVar5, bVar3, (CVar15 ~= 0))
    quest:FadeScreenIn()
    quest:SetTeleportingAsActive(false)
    quest:DisplayQuestInfo(true)
    fVar16 = 1.0
    pCVar13 = {R = 255, G = 0, B = 0, A = 255}
    pThing = quest:GetThingWithScriptName("Dragon")
    r1 = quest:AddQuestInfoBarHealth(pThing, pCVar13, "HUD_QUEST_ICON_DRAGON", fVar16)
    pThing = nil
    quest:CreateThread("RunEnemySpawning")  -- native thread body 0x00D26BA0: lift it as function RunEnemySpawning(quest)
    quest:CreateThread("JackTaunts")  -- native thread body JackTaunts: lift it as function JackTaunts(quest)
    iVar17 = quest:GetStateInt("DragonState")
    while iVar17 ~= 4 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d26a3a end
        iVar17 = quest:GetStateInt("DragonState")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d26a3a end
    quest:SetTeleportingAsActive(true)
    iVar17 = extraout_ECX
    helper_D27050(quest, iVar17)
    quest:SetCutsceneActionMode(true, "TEXT_QST_B05_MASK_CHOICE")
    iVar17 = extraout_ECX_00
    helper_D27050(quest, iVar17)
    quest:SetCutsceneActionMode(false, "")
    cVar1 = quest:RetailFlags("cs_flags"):Get("PutMaskOn")
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not cVar1 then
        if bVar3 then goto LAB_00d26a3a end
        iVar17 = extraout_ECX_01
        helper_D27050(quest, iVar17)
        quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_380")
        quest:SetMasterGameState("HeroWoreMask", false)
        uVar11 = 0
        if #iStack_18 ~= 0 then
            repeat
                quest:EntitySetInLimbo(iStack_18[(iVar9) / 0xc + 1], false, true)
                uVar11 = uVar11 + 1
                iVar9 = iVar9 + 0xc
            until not (uVar11 < (#iStack_18))
        end
        CVar15 = 0x0
        bVar14 = false
        bVar3 = true
        pCVar5 = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(pCVar5, bVar3, bVar14, (CVar15 ~= 0))
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d26a3a end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d26a3a end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d26a3a end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d26a3a end
        fret_00 = quest:GetHeroMorality()
        if 0.5 <= fret_00 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d26a3a end
            pcVar7 = "Data\\Video\\dragon_good_no_mask.xmv"
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d26a3a end
            pcVar7 = "Data\\Video\\dragon_evil_no_mask.xmv"
        end
        quest:PlayAVIMovie(pcVar7)
        fVar16 = 0.5
    else
        if bVar3 then goto LAB_00d26a3a end
        iVar17 = extraout_ECX_01
        helper_D27050(quest, iVar17)
        quest:AddLogbookStoryEntry("TEXT_QST_LOG_STORY_PLATINUM_385")
        quest:SetMasterGameState("HeroWoreMask", true)
        uVar11 = 0
        if #iStack_18 ~= 0 then
            repeat
                quest:EntitySetInLimbo(iStack_18[(iVar9) / 0xc + 1], false, true)
                uVar11 = uVar11 + 1
                iVar9 = iVar9 + 0xc
            until not (uVar11 < (#iStack_18))
        end
        CVar15 = 0x0
        bVar14 = false
        bVar3 = true
        pCVar5 = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(pCVar5, bVar3, bVar14, (CVar15 ~= 0))
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d26a3a end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d26a3a end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d26a3a end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d26a3a end
        fret_0 = quest:GetHeroMorality()
        if 0.5 <= fret_0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d26a3a end
            pcVar7 = "Data\\Video\\dragon_good_mask.xmv"
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00d26a3a end
            pcVar7 = "Data\\Video\\dragon_evil_mask.xmv"
        end
        quest:PlayAVIMovie(pcVar7)
        fVar16 = -1.0
    end
    quest:GiveHeroMorality(fVar16)
    quest:StopOverrideMusic(false)
    uVar11 = 0
    pCVar5 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pCVar5, uVar11)
    ::LAB_00d26a3a::
end

function RunEnemySpawning(quest)
    local bVar1
    local alive = true
    local r1 = quest:GetAllThingsWithScriptName("DBMinionSpawn")
    local r2 = quest:GetAllThingsWithScriptName("DBSummonerSpawn")
    quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", quest:GetStateInt("NumFlyBysBetweenSummonerSpawns") + -1)
    local iVar2 = quest:GetStateInt("DragonState")
    quest:SetStateBool("MinionSpawningEnabled", false)
    quest:SetStateBool("SummonerSpawningEnabled", false)
    while true do
        if iVar2 == 4 then
            alive = not quest:IsActiveThreadTerminating()
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then break end
        local __native_condition_1 = quest:GetStateBool("MinionSpawningEnabled")
        if __native_condition_1 then
            iVar2 = quest:GetTimer(quest:GetStateInt("MinionSpawnDelay"))
            __native_condition_1 = iVar2 == 0
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            SpawnMinions(quest)
            quest:SetStateBool("MinionSpawningEnabled", false)
        end
        local __native_condition_2 = quest:GetStateBool("SummonerSpawningEnabled")
        if __native_condition_2 then
            iVar2 = quest:GetTimer(quest:GetStateInt("SummonerSpawnDelay"))
            __native_condition_2 = iVar2 == 0
        end
        if __native_condition_2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            SpawnSummoners(quest)
            quest:SetStateBool("SummonerSpawningEnabled", false)
        end
        iVar2 = quest:GetStateInt("DragonState")
    end
end

function JackTaunts(quest)
    local b2, bUnknown, bVar2, iVar3, iVar4, i_stk_4, pCVar5, pCVar6, pSpeaker, value
    local alive = true
    iVar3 = quest:RegisterTimer()
    value = 10
    i_stk_4 = iVar3
    quest:SetTimer(iVar3, 0x14)
    repeat
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d26b6e end
        iVar4 = quest:GetTimer(iVar3)
        if iVar4 == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                quest:DeregisterTimer(iVar3)
                return
            end
            b2 = false
            bVar2 = false
            pCVar5 = quest:GetHero()
            iVar4 = quest:AddNewConversation(pCVar5, bVar2, b2)
            pCVar5 = quest:GetHero()
            pSpeaker = quest:GetHero()
            bUnknown = false
            pCVar6 = tostring(value)
            pCVar6 = ("TEXT_QST_B05_JACK_TAUNT_" .. pCVar6)
            quest:AddLineToConversation(iVar4, pCVar6, pSpeaker, pCVar5, bUnknown)
            iVar3 = i_stk_4
            quest:SetTimer(i_stk_4, 0x14)
            value = value + 0xa
        end
    until not (value < 0x3c)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        -- LAB_00d26b7e: (native jump target)
        quest:DeregisterTimer(iVar3)
        return
    end
    ::LAB_00d26b6e::
    quest:DeregisterTimer(iVar3)
end

function SpawnMinions(quest)
    local bVar2, iVar4, pCVar3, r1, r2, v_stk_20
    local alive = true
    local v_stk_1c = quest:GetStateInt("TargetNumMinions") - quest:GetStateInt("NumMinions")
    v_stk_20 = 0
    if 0 < v_stk_1c then
        iVar4 = 0
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            bVar2 = false
            pCVar3 = quest:GetStateListAt("MinionSpawnPoints", (iVar4) / 0xc):GetPos()
            r1 = quest:CreateEffectAtPos("SUMMON_CREATURE_SUMMON_EVIL", pCVar3, 0.0, bVar2)
            r1 = nil
            bVar2 = false
            pCVar3 = quest:GetStateListAt("MinionSpawnPoints", (iVar4) / 0xc):GetPos()
            r2 = quest:CreateCreature("CREATURE_MINION_WARDOG", pCVar3, "DBMinion")
            r2 = nil
            v_stk_20 = v_stk_20 + 1
            iVar4 = iVar4 + 0xc
        until not (v_stk_20 < v_stk_1c)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
end

function SpawnSummoners(quest)
    local bVar2, iVar1, pCVar4, r1, r2, string, uVar3, v_stk_20
    local alive = true
    local v_stk_1c = quest:GetStateInt("TargetNumSummoners") - quest:GetStateInt("NumSummoners")
    v_stk_20 = 0
    if 0 < v_stk_1c then
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            uVar3 = math.random(0, 32767)
            uVar3 = uVar3 & 0x80000001
            bVar2 = uVar3 == 0
            if uVar3 < 0 then
                bVar2 = (uVar3 - 1 | 0xfffffffe) == 0xffffffff
            end
            string = "CREATURE_SUMMONER_01"
            if not bVar2 then
                string = "CREATURE_SUMMONER_02"
            end
            bVar2 = false
            -- TODO(native): pCVar4 = (**(*(quest:GetStateListRef("SummonerSpawnPoints") + quest:GetStateInt("NextSummonerSpawnPoint") * 0xc) + 0x18))()
            pCVar4 = nil --[[unresolved native value]]
            r1 = quest:CreateEffectAtPos("SUMMON_CREATURE_SUMMON_EVIL", nil --[[missing]], 0.0, bVar2)
            r1 = nil
            bVar2 = false
            -- TODO(native): pCVar4 = (**(*(quest:GetStateListRef("SummonerSpawnPoints") + quest:GetStateInt("NextSummonerSpawnPoint") * 0xc) + 0x18))()
            pCVar4 = nil --[[unresolved native value]]
            r2 = quest:CreateCreature(pCVar4, nil --[[missing]], "DBSummoner")
            r2 = nil
            iVar1 = quest:GetStateInt("NextSummonerSpawnPoint")
            quest:SetStateInt("NextSummonerSpawnPoint", iVar1 + 1)
            if quest:GetStateListCount("SummonerSpawnPoints") <= iVar1 + 1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                quest:SetStateInt("NextSummonerSpawnPoint", 0)
            end
            v_stk_20 = v_stk_20 + 1
        until not (v_stk_20 < v_stk_1c)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
end

function helper_D25C20(quest)
    local bVar2
    local alive = true
    local iVar1 = quest:GetStateInt("NumFlyBysSinceLastSummonerSpawn")
    quest:SetStateBool("MinionSpawningEnabled", true)
    quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", iVar1 + 1)
    if quest:GetStateInt("NumFlyBysBetweenSummonerSpawns") <= iVar1 + 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:SetStateBool("SummonerSpawningEnabled", true)
            quest:SetStateInt("NumFlyBysSinceLastSummonerSpawn", 0)
        end
    end
end

function helper_D27050(quest, native_arg_param_1)
    local resources = quest:RetailResources()
    local xStack_20 = resources:NewResource()
    local pScriptObject = xStack_20
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_2c = resources:NewActorMap()
    resources:SetActor(xStack_2c, "HERO", xStack_20)
    local xStack_10 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithFlags(native_arg_param_1, xStack_2c, quest:RetailFlags("cs_flags"), false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    resources:DestroyActorMap(xStack_2c)
    resources:ReleaseResource(xStack_20)
end

