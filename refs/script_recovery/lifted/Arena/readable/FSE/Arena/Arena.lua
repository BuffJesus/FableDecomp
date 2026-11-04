-- Readable native conversion: Q_Arena. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local CUTSCENE_BEHAVIOUR_PAUSED = 1  -- ECutsceneBehaviour (Ego_r.pdb)
local CUTSCENE_BEHAVIOUR_NOT_PAUSED = 2  -- ECutsceneBehaviour (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    ArenaNakedBoastCost = 112,  -- 500
    ArenaNakedBoastReward = 116,  -- 4000
    ArenaNoDamageBoastCost = 120,  -- 400
    ArenaNoDamageBoastReward = 124,  -- 10000
    ArenaCrowdShoutDelay = 2660,  -- 5
    ArenaCrowdShoutDelayRandom = 2664,  -- 3
    RewardObjects = 2668,  -- '060000008ab102009db10200b0b1020077b1020077b10200c58b0200'
    ArenaShowNoMercyBoastAmount = 2780,  -- 1000
    ArenaShowMercyBoastAmount = 2784,  -- 1000
    ArenaShowNoMercyBoastReward = 2788,  -- 2000
    ArenaShowMercyBoastReward = 2792,  -- 2000
    ArenaSpareWhisperMorality = 2796,  -- 0.07999999821186066
}

-- Q_Arena.Main (retail 0x00f0fb70)
function Main(quest)
    local resources = quest:RetailResources()
    local angle, scratchValue, getHeroTitle, switch, position, hero10Control, resource
    local arenaMainExit, centrePoint, hero2, whisperAlly, arenaCellEntrance2, arenaInsideHSP3, roth
    local hero6, hero10, hero12, hero16, hero17, hero18, hero19, scratchValue7, actorMap, pppuVar
    local shadow, arenaCellDoorGuard, cellsToHallOfHeroesEntrance, flick, needle, cham, shadow2
    local arenaCellDoorGuard22, flick2, needle2, this_00, retailRandModulo, resource3, resource4
    local resource5, resource6, actorMap2, resource7, actorMap3, scratchValue8, resource8, resource9
    local actorMap5, actorMap6, movie, movie2
    local getActiveQuestName = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(getActiveQuestName, "TEXT_QUEST_ARENA_OBJECTIVE_01", "Arena", "KnotholeGlade")
    InitialiseVariables(quest)
    quest:AddEntityBinding("Cham", "Arena/Entities/Cham", 1)
    quest:AddEntityBinding("Flick", "Arena/Entities/Flick", 1)
    quest:AddEntityBinding("Shadow", "Arena/Entities/Shadow", 1)
    quest:AddEntityBinding("Needle", "Arena/Entities/Needle", 1)
    quest:AddEntityBinding("Roth", "Arena/Entities/Roth", 1)
    quest:AddEntityBinding("CellWhisper", "Arena/Entities/CellWhisper", 1)
    quest:AddEntityBinding("ArenaCellDoorGuard", "Arena/Entities/ArenaCellDoorGuard", 1)
    quest:AddEntityBinding("ArenaCellDoorGuard2", "Arena/Entities/ArenaCellDoorGuard2", 1)
    quest:AddEntityBinding("SUMMONED_CREATURE", "Arena/Entities/SUMMONED_CREATURE")
    quest:AddEntityBinding("ArenaEnemy", "Arena/Entities/ArenaEnemy")
    quest:AddEntityBinding("ArenaSpawn", "Arena/Entities/ArenaSpawn")
    quest:AddEntityBinding("CagedBalverine", "Arena/Entities/CagedBalverine")
    quest:AddEntityBinding("ArenaCellExitGuard", "Arena/Entities/ArenaCellExitGuard", 1)
    quest:AddEntityBinding("WhisperAlly", "Arena/Entities/WhisperAlly", 1)
    quest:FinalizeEntityBindings()
    scratchValue = quest:IsQuestActive("Q_ArenaHoldingScript")
    if (1 - (scratchValue and 1 or 0)) ~= 0 then
        scratchValue = quest:IsActiveThreadTerminating()
        if scratchValue then
            return
        end
        quest:ActivateQuest("Q_ArenaHoldingScript")
    end
    quest:CreateThread("WatchForTermination")  -- native thread body Script_Arena_Teleport_Thread: lift it as function WatchForTermination(quest)
    if not scratchValue then
    end
    quest:CreateThread("CrowdChecker")  -- native thread body CQ_ArenaScript__CrowdChecker: lift it as function CrowdChecker(quest)
    if quest:GetHeroTitle() == 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:GiveHeroTitle("OBJECT_HERO_TITLE_CHICKEN_CHASER")
    end
    if quest:GetStateInt("ArenaState") == 0 then
        if quest:IsActiveThreadTerminating() then return end
        while not quest:IsLevelLoaded("ArenaHallOfHeroes") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateInt("ArenaState", 1)
    end
    quest:SetTimeOfDay(12.0)
    quest:SetTimeAsStopped(true)
    quest:SetTeleportingAsActive(false)
    if quest:GetStateInt("ArenaState") == 1 then
        if quest:IsActiveThreadTerminating() then return end
        resource8 = resources:NewResource()
        pppuVar = resource8
        local hero = quest:GetHero()
        resources:TryAcquire(pppuVar, hero, 4)
        local nav1 = quest:GetThingWithScriptName("Q_Nav1")
        local hero8 = quest:GetHero()
        quest:EntityTeleportToThing(hero8, nav1, false)
        actorMap5 = resources:NewActorMap()
        resources:SetActor(actorMap5, "Hero", resource8)
        resource9 = resources:StartMovie("")
        resources:RunMacro("CS_ARENA_HOH_INTRO", actorMap5, false, true)
        resources:DestroyMovie(resource9)
        resources:DestroyActorMap(actorMap5)
        resources:ReleaseResource(resource8)
        local arenaCellEntrance = quest:GetThingWithScriptName("ArenaCellEntrance")
        local hero9 = quest:GetHero()
        quest:EntityTeleportToThing(hero9, arenaCellEntrance, false)
        while not quest:IsLevelLoaded("ArenaCells") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        local cellsExitToArena = quest:GetThingWithScriptName("CellsExitToArena")
        quest:SetThingAsUsable(cellsExitToArena, false)
        local cellsExitToHOH = quest:GetThingWithScriptName("CellsExitToHOH")
        quest:SetThingAsUsable(cellsExitToHOH, false)
        local cellsExitToArena2 = quest:GetThingWithScriptName("CellsExitToArena")
        quest:SetThingPersistent(cellsExitToArena2, true)
        local cellsExitToHOH2 = quest:GetThingWithScriptName("CellsExitToHOH")
        quest:SetThingPersistent(cellsExitToHOH2, true)
        quest:FadeScreenIn()
        quest:SetStateInt("ArenaState", 2)
    end
    if quest:GetStateInt("ArenaState") == 2 then
        if quest:IsActiveThreadTerminating() then return end
        local roth3 = quest:GetThingWithScriptName("Roth")
        resource6 = resources:NewResource()
        resources:PrepareResource(resource6)
        while not resources:TryAcquire(resource6, roth3, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f10d3a end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f10d3a end
        flick = quest:GetThingWithScriptName("Flick")
        resource5 = resources:NewResource()
        resources:PrepareResource(resource5)
        while not resources:TryAcquire(resource5, flick, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f10d22 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f10d22 end
        needle = quest:GetThingWithScriptName("Needle")
        resource9 = resources:NewResource()
        resources:PrepareResource(resource9)
        while not resources:TryAcquire(resource9, needle, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f10d0d end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f10d0d end
        cham = quest:GetThingWithScriptName("Cham")
        resource8 = resources:NewResource()
        resources:PrepareResource(resource8)
        while not resources:TryAcquire(resource8, cham, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f10cf8 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f10cf8 end
        shadow2 = quest:GetThingWithScriptName("Shadow")
        resource7 = resources:NewResource()
        resources:PrepareResource(resource7)
        while not resources:TryAcquire(resource7, shadow2, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f10ce3 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f10ce3 end
        arenaCellDoorGuard22 = quest:GetThingWithScriptName("ArenaCellDoorGuard2")
        resource4 = resources:NewResource()
        resources:PrepareResource(resource4)
        while not resources:TryAcquire(resource4, arenaCellDoorGuard22, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f10cce end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f10cce end
        resource3 = resources:NewResource()
        resources:PrepareResource(resource3)
        hero10Control = resource3
        hero10 = quest:GetHero()
        scratchValue = resources:TryAcquire(hero10Control, hero10, 4)
        while not scratchValue do
            if not quest:NewScriptFrame() then goto LAB_00f10cc2 end
            local hero11Control = resource3
            local hero11 = quest:GetHero()
            scratchValue = resources:TryAcquire(hero11Control, hero11, 4)
        end
        if not (quest:IsActiveThreadTerminating()) then
            actorMap3 = resources:NewActorMap()
            resources:SetActor(actorMap3, "Hero", resource3)
            resources:SetActor(actorMap3, "Roth", resource6)
            resources:SetActor(actorMap3, "Flick", resource5)
            resources:SetActor(actorMap3, "Cham", resource8)
            resources:SetActor(actorMap3, "Guard", resource4)
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_ARENA_CELLS_INTRO", actorMap3, false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap3)
            resources:ReleaseResource(resource3)
            resources:ReleaseResource(resource4)
            resources:ReleaseResource(resource7)
            resources:ReleaseResource(resource8)
            resources:ReleaseResource(resource9)
            resources:ReleaseResource(resource5)
            resources:ReleaseResource(resource6)
            quest:SetStateBool("ChamLeaving", true)
            quest:SetStateInt("ArenaState", 3)
            goto LAB_00f10fa6
        end
        ::LAB_00f10cc2::
        resources:ReleaseResource(resource3)
        ::LAB_00f10cce::
        resources:ReleaseResource(resource4)
        ::LAB_00f10ce3::
        resources:ReleaseResource(resource7)
        ::LAB_00f10cf8::
        resources:ReleaseResource(resource8)
        ::LAB_00f10d0d::
        resources:ReleaseResource(resource9)
        ::LAB_00f10d22::
        resources:ReleaseResource(resource5)
        ::LAB_00f10d3a::
        resources:ReleaseResource(resource6)
        return
    end
    ::LAB_00f10fa6::
    local timerId = quest:RegisterTimer()
    if quest:GetStateInt("ArenaState") == 3 then
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        local getActiveQuestName2 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(getActiveQuestName2, "TEXT_QUEST_ARENA_OBJECTIVE_02", "ArenaCells", "KnotholeGlade")
        quest:SetTimer(timerId, 60)
        while 0 < quest:GetTimer(timerId) do
            if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        while not quest:IsHeroControlledByPlayer() do
            if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        quest:SetStateInt("ArenaState", 4)
    end
    if quest:GetStateInt("ArenaState") == 4 then
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        local getActiveQuestName3 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(getActiveQuestName3, "TEXT_QUEST_ARENA_OBJECTIVE_03", "Arena", "KnotholeGlade")
        local roth4 = quest:GetThingWithScriptName("Roth")
        resource6 = resources:NewResource()
        resources:PrepareResource(resource6)
        while not resources:TryAcquire(resource6, roth4, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f11602 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f11602 end
        flick2 = quest:GetThingWithScriptName("Flick")
        resource8 = resources:NewResource()
        resources:PrepareResource(resource8)
        while not resources:TryAcquire(resource8, flick2, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f115ed end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f115ed end
        needle2 = quest:GetThingWithScriptName("Needle")
        resource9 = resources:NewResource()
        resources:PrepareResource(resource9)
        while not resources:TryAcquire(resource9, needle2, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f115d8 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f115d8 end
        shadow = quest:GetThingWithScriptName("Shadow")
        resource7 = resources:NewResource()
        resources:PrepareResource(resource7)
        while not resources:TryAcquire(resource7, shadow, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f115c3 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f115c3 end
        arenaCellDoorGuard = quest:GetThingWithScriptName("ArenaCellDoorGuard2")
        resource5 = resources:NewResource()
        resources:PrepareResource(resource5)
        while not resources:TryAcquire(resource5, arenaCellDoorGuard, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f115ae end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f115ae end
        resource4 = resources:NewResource()
        resources:PrepareResource(resource4)
        resource = resource4
        hero12 = quest:GetHero()
        scratchValue = resources:TryAcquire(resource, hero12, 4)
        while not scratchValue do
            if not quest:NewScriptFrame() then goto LAB_00f115a2 end
            local resource2 = resource4
            local hero13 = quest:GetHero()
            scratchValue = resources:TryAcquire(resource2, hero13, 4)
        end
        if not (quest:IsActiveThreadTerminating()) then
            actorMap3 = resources:NewActorMap()
            resources:SetActor(actorMap3, "Hero", resource4)
            resources:SetActor(actorMap3, "Roth", resource6)
            resources:SetActor(actorMap3, "Flick", resource8)
            resources:SetActor(actorMap3, "Guard", resource5)
            resource3 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:RunMacro("CS_ARENA_CELLS_INTRO_CHAMDEAD", actorMap3, false, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(resource3)
            resources:DestroyActorMap(actorMap3)
            resources:ReleaseResource(resource4)
            resources:ReleaseResource(resource5)
            resources:ReleaseResource(resource7)
            resources:ReleaseResource(resource9)
            resources:ReleaseResource(resource8)
            resources:ReleaseResource(resource6)
            quest:SetStateInt("ArenaState", 5)
            goto LAB_00f11828
        end
        ::LAB_00f115a2::
        resources:ReleaseResource(resource4)
        ::LAB_00f115ae::
        resources:ReleaseResource(resource5)
        ::LAB_00f115c3::
        resources:ReleaseResource(resource7)
        ::LAB_00f115d8::
        resources:ReleaseResource(resource9)
        ::LAB_00f115ed::
        resources:ReleaseResource(resource8)
        ::LAB_00f11602::
        resources:ReleaseResource(resource6)
        quest:DeregisterTimer(timerId)
        return
    end
    ::LAB_00f11828::
    if quest:GetStateInt("ArenaState") == 5 then
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        if quest:GetStateInt("ArenaRound") < 3 then
            local cellWhisper = quest:GetThingWithScriptName("CellWhisper")
            if cellWhisper ~= nil and cellWhisper:IsAlive() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                local cellWhisper2 = quest:GetThingWithScriptName("CellWhisper")
                quest:RemoveThing(cellWhisper2, false, true)
            end
        else
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
            local cellWhisper3 = quest:GetThingWithScriptName("CellWhisper")
            if not (cellWhisper3 ~= nil and cellWhisper3:IsAlive()) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
                position = quest:GetThingWithScriptName("FlickPoint"):GetPos()
                quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", position, "CellWhisper", false)
            end
        end
        while quest:GetStateInt("ArenaState") ~= 6 do
            if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        local arenaInsideHSP = quest:GetThingWithScriptName("ArenaInsideHSP")
        local hero14 = quest:GetHero()
        quest:EntityTeleportToThing(hero14, arenaInsideHSP, false)
        while not quest:IsLevelLoaded("Arena") do
            if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        quest:MiniMapSetAsEnabled(false)
        quest:SetStateBool("PlayerLeaving", false)
        local arenaHeroGate = quest:GetThingWithScriptName("ArenaHeroGate")
        quest:OpenDoor(arenaHeroGate)
        quest:SetThingPersistent(arenaHeroGate, true)
    end
    local actorMap4 = resources:NewActorMap()
    if quest:GetStateInt("ArenaState") == 7 then
        if not quest:IsActiveThreadTerminating() then
            if quest:GetStateInt("ArenaRound") < 3 then
                local cellWhisper4 = quest:GetThingWithScriptName("CellWhisper")
                if cellWhisper4 ~= nil and cellWhisper4:IsAlive() then
                    if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
                    local cellWhisper5 = quest:GetThingWithScriptName("CellWhisper")
                    quest:RemoveThing(cellWhisper5, false, true)
                end
                goto LAB_00f11d57
            elseif not quest:IsActiveThreadTerminating() then
                local cellWhisper6 = quest:GetThingWithScriptName("CellWhisper")
                if cellWhisper6 ~= nil and cellWhisper6:IsAlive() then goto LAB_00f11d5c end
                if not quest:IsActiveThreadTerminating() then
                    position = quest:GetThingWithScriptName("FlickPoint"):GetPos()
                    quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", position, "CellWhisper", false)
                    goto LAB_00f11d57
                end
            end
            goto FLOW_past_lab_00f11d57
            ::LAB_00f11d57::
            goto LAB_00f11d5c
            ::FLOW_past_lab_00f11d57::
            goto FLOW_past_lab_00f11d5c
            ::LAB_00f11d5c::
            while quest:GetStateInt("ArenaState") ~= 6 do
                if not quest:NewScriptFrame() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
            end
            if not quest:IsActiveThreadTerminating() then
                quest:FadeScreenOut(0.5, 0.5)
                local arenaInsideHSP2 = quest:GetThingWithScriptName("ArenaInsideHSP")
                local hero15 = quest:GetHero()
                quest:EntityTeleportToThing(hero15, arenaInsideHSP2, false)
                while not quest:IsLevelLoaded("Arena") do
                    if not quest:NewScriptFrame() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
                end
                if not quest:IsActiveThreadTerminating() then
                    quest:MiniMapSetAsEnabled(false)
                    quest:SetStateInt("GoldMultiplier", 0)
                    quest:SetStateBool("PlayerLeaving", false)
                    goto LAB_00f11eec
                end
            end
            ::FLOW_past_lab_00f11d5c::
        end
    elseif not quest:IsActiveThreadTerminating() then
        quest:FadeScreenOut(0.5, 0.5)
        goto LAB_00f11eec
    end
    resources:DestroyActorMap(actorMap4)
    quest:DeregisterTimer(timerId)
    do return end
    ::LAB_00f11eec::
    AnimateCrowd(quest)
    while not quest:GetStateBool("PlayerWon") do
        if not quest:NewScriptFrame() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
        while not quest:GetStateBool("PlayerLeaving") and not quest:GetStateBool("PlayerWon") do
            if not quest:NewScriptFrame() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
            if quest:GetStateInt("ArenaRoundWave") ~= 0 then goto LAB_00f13472 end
            if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
            quest:AutoSaveCheckPoint()
            quest:SetStateBool("WhisperNeededForCutscene", true)
            arenaMainExit = quest:GetThingWithScriptName("ArenaMainExit")
            quest:SetThingAsUsable(arenaMainExit, false)
            centrePoint = quest:GetThingWithScriptName("ARENA_CentrePoint")
            hero16 = quest:GetHero()
            quest:EntityTeleportToThing(hero16, centrePoint, false)
            resource9 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resource8 = resources:NewResource()
            resource7 = resources:NewResource()
            resources:PrepareResource(resource8)
            pppuVar = resource8
            hero2 = quest:GetHero()
            scratchValue = resources:TryAcquire(pppuVar, hero2, 4)
            while not scratchValue do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource7)
                    resources:ReleaseResource(resource8)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(resource9)
                    resources:DestroyActorMap(actorMap4)
                    quest:DeregisterTimer(timerId)
                    return
                end
                pppuVar = resource8
                local hero3 = quest:GetHero()
                scratchValue = resources:TryAcquire(pppuVar, hero3, 4)
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00f13e6c end
            scratchValue8 = "CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC"
            retailRandModulo = quest:RetailRandModulo(32768) & 0x80000003
            if retailRandModulo < 0 then
                retailRandModulo = (retailRandModulo - 1 | 0xfffffffc) + 1
            end
            if retailRandModulo ~= 1 then
                if retailRandModulo ~= 2 then goto LAB_00f121a5 end
                if quest:IsActiveThreadTerminating() then goto LAB_00f13e5e end
                scratchValue7 = "CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC3"
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00f13e6c end
                scratchValue7 = "CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC2"
            end
            scratchValue8 = scratchValue7
            ::LAB_00f121a5::
            actorMap2 = resources:NewActorMap()
            resources:SetActor(actorMap2, "Hero", resource8)
            actorMap6 = resources:NewStringMap()
            resources:SetString(actorMap6, "$THEME", "NULL")
            getHeroTitle = quest:GetHeroTitle()
            if not (quest:GetStateInt("ArenaRound") < 3) then
                if quest:IsActiveThreadTerminating() then goto LAB_00f13e9c end
                whisperAlly = quest:GetThingWithScriptName("WhisperAlly")
                quest:SetStateThing("Whisper", whisperAlly)
                this_00 = quest:GetStateThing("Whisper")
                if not (this_00 ~= nil and this_00:IsAlive()) then
                    if not quest:IsActiveThreadTerminating() then
                        position = quest:GetThingWithScriptName("ARENA_WhisperStartPoint"):GetPos()
                        local rivalHeroWhisperArena = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", position, "WhisperAlly", false)
                        quest:SetStateThing("Whisper", rivalHeroWhisperArena)
                        goto LAB_00f123aa
                    end
                    goto LAB_00f13e45
                end
            end
            ::LAB_00f123aa::
            switch = quest:GetStateInt("ArenaRound")
            repeat
                if switch == 0 then
                    local getActiveQuestName4 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(getActiveQuestName4, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_01", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 0)
                    local switch4 = getHeroTitle
                    repeat
                        if switch4 == 1 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_17")
                            break
                        elseif switch4 == 2 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_20")
                            break
                        elseif switch4 == 3 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_11")
                            break
                        elseif switch4 == 4 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_06")
                            break
                        elseif switch4 == 5 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_03")
                            break
                        elseif switch4 == 6 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_12")
                            break
                        elseif switch4 == 7 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_04")
                            break
                        elseif switch4 == 8 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_15")
                            break
                        elseif switch4 == 9 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_10")
                            break
                        elseif switch4 == 10 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_13")
                            break
                        elseif switch4 == 11 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_07")
                            break
                        elseif switch4 == 12 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_16")
                            break
                        elseif switch4 == 13 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_18")
                            break
                        elseif switch4 == 14 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_09")
                            break
                        elseif switch4 == 15 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_08")
                            break
                        elseif switch4 == 16 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_19")
                            break
                        elseif switch4 == 17 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_01")
                            break
                        elseif switch4 == 18 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_14")
                            break
                        elseif switch4 == 19 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_05")
                            break
                        elseif switch4 == 20 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_02")
                            break
                        else
                            break
                        end
                    until true
                    scratchValue8 = "CS_ARENA_ROUND_1"
                    local randomChoice = quest:RetailRandModulo(32768)
                    local scratchValue3 = GetFanfareMusic(quest, randomChoice % 10)
                    quest:OverrideMusic(scratchValue3, false, false)
                    break
                elseif switch == 1 then
                    local getActiveQuestName5 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(getActiveQuestName5, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_02", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 0)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND2_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND2_20")
                    scratchValue8 = "CS_ARENA_ROUND_BEGIN_GENERIC"
                    break
                elseif switch == 2 then
                    local getActiveQuestName6 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(getActiveQuestName6, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_03", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 1)
                    scratchValue8 = "CS_ARENA_ROUND_WHISPER_INTRO"
                    resources:SetString(actorMap6, "$THEME", "ENVIRONMENT_WITCHWOOD")
                    break
                elseif switch == 3 then
                    local getActiveQuestName7 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(getActiveQuestName7, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_04", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 1)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND3_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND3_20")
                    resources:SetString(actorMap6, "$THEME", "ENVIRONMENT_HAUNTED")
                    break
                elseif switch == 4 then
                    local getActiveQuestName8 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(getActiveQuestName8, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_05", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 2)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND4_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND4_20")
                    resources:SetString(actorMap6, "$THEME", "RESET")
                    break
                elseif switch == 5 then
                    local getActiveQuestName9 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(getActiveQuestName9, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_06", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 2)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND5_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND5_20")
                    break
                elseif switch == 6 then
                    local getActiveQuestName10 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(getActiveQuestName10, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_07", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 3)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND6_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND6_20")
                    break
                elseif switch == 7 then
                    local getActiveQuestName11 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(getActiveQuestName11, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_08", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 3)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND7_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND7_20")
                    resources:SetString(actorMap6, "$THEME", "ENVIRONMENT_EOW_04")
                    quest:OverrideMusic(23, false, false)
                    break
                elseif switch == 8 then
                    local getActiveQuestName12 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(getActiveQuestName12, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_09", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 3)
                    quest:SetStateInt("ArenaState", 8)
                    scratchValue8 = "CS_ARENA_ROUND_BOTH_WINNERS"
                    resources:SetString(actorMap6, "$THEME", "RESET")
                    break
                else
                    break
                end
            until true
            if quest:GetStateThing("Whisper"):IsAlive() then
                resources:PrepareResource(resource7)
                while not resources:TryAcquire(resource7, quest:GetStateThing("Whisper"), 4) do
                    if not quest:NewScriptFrame() then goto LAB_00f13e9c end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00f13e45 end
                goto FLOW_hoist_lab_00f13e45_1
            end
            goto FLOW_past_lab_00f13e45
            ::LAB_00f13e45::
            resources:DestroyStringMap(actorMap6)
            resources:DestroyActorMap(actorMap2)
            goto LAB_00f13e5e
            ::FLOW_hoist_lab_00f13e45_1::
            goto FLOW_hoist_lab_00f13e5e_1
            ::FLOW_past_lab_00f13e45::
            goto FLOW_past_lab_00f13e5e
            ::LAB_00f13e5e::
            goto LAB_00f13e6c
            ::FLOW_hoist_lab_00f13e5e_1::
            goto FLOW_hoist_lab_00f13e6c_1
            ::FLOW_past_lab_00f13e5e::
            goto FLOW_past_lab_00f13e6c
            ::LAB_00f13e6c::
            resources:ReleaseResource(resource7)
            resources:ReleaseResource(resource8)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(resource9)
            resources:DestroyActorMap(actorMap4)
            quest:DeregisterTimer(timerId)
            do return end
            ::FLOW_hoist_lab_00f13e6c_1::
            resources:SetActor(actorMap2, "Whisper", resource7)
            ::FLOW_past_lab_00f13e6c::
            resources:RunMacroWithStrings(scratchValue8, actorMap2, actorMap6, false, true)
            quest:StopOverrideMusic(false)
            if scratchValue8 ~= nil and scratchValue8 == "CS_ARENA_ROUND_BOTH_WINNERS" then
                if quest:IsActiveThreadTerminating() then goto LAB_00f13e9c end
                goto FLOW_hoist_lab_00f13e9c_1
            end
            goto FLOW_past_lab_00f13e9c
            ::LAB_00f13e9c::
            resources:DestroyStringMap(actorMap6)
            resources:DestroyActorMap(actorMap2)
            resources:ReleaseResource(resource7)
            resources:ReleaseResource(resource8)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(resource9)
            resources:DestroyActorMap(actorMap4)
            quest:DeregisterTimer(timerId)
            do return end
            ::FLOW_hoist_lab_00f13e9c_1::
            quest:SetStateBool("FinalBattleCS", true)
            ::FLOW_past_lab_00f13e9c::
            quest:SetStateBool("WhisperNeededForCutscene", false)
            resources:DestroyStringMap(actorMap6)
            resources:DestroyActorMap(actorMap2)
            resources:ReleaseResource(resource7)
            resources:ReleaseResource(resource8)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(resource9)
            ::LAB_00f13472::
            if not quest:GetStateBool("PlayerWon") then
                if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
                if quest:GetStateInt("ArenaState") == 8 then
                    if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
                    while not quest:IsLevelLoaded("ArenaCells") do
                        if not quest:NewScriptFrame() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
                    end
                    if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
                    local cellWhisper7 = quest:GetThingWithScriptName("CellWhisper")
                    if cellWhisper7 ~= nil and cellWhisper7:IsAlive() then
                        if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
                        local cellWhisper8 = quest:GetThingWithScriptName("CellWhisper")
                        quest:RemoveThing(cellWhisper8, false, true)
                    end
                    quest:SetStateBool("PlayerWon", true)
                else
                    if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
                    PlayWave(quest)
                end
            end
        end
        if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
        if not quest:GetStateBool("PlayerLeaving") then goto continue_2 end
        if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
        movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(0.5)
        quest:ResetToDefaultTheme(0.0)
        arenaCellEntrance2 = quest:GetThingWithScriptName("ArenaCellEntrance2")
        hero17 = quest:GetHero()
        quest:EntityTeleportToThing(hero17, arenaCellEntrance2, false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        while not quest:IsLevelLoaded("ArenaCells") do
            if not quest:NewScriptFrame() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
        end
        if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
        quest:FadeScreenIn()
        quest:MiniMapSetAsEnabled(true)
        quest:SetStateInt("ArenaState", 7)
        if quest:GetStateInt("ArenaRound") < 3 then
            local cellWhisper9 = quest:GetThingWithScriptName("CellWhisper")
            if cellWhisper9 ~= nil and cellWhisper9:IsAlive() then
                if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
                local cellWhisper10 = quest:GetThingWithScriptName("CellWhisper")
                quest:RemoveThing(cellWhisper10, false, true)
                goto LAB_00f13a09
            end
        else
            if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
            local cellWhisper11 = quest:GetThingWithScriptName("CellWhisper")
            if not (cellWhisper11 ~= nil and cellWhisper11:IsAlive()) then
                if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
                position = quest:GetThingWithScriptName("FlickPoint"):GetPos()
                quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", position, "CellWhisper", false)
            end
        end
        ::LAB_00f13a09::
        quest:AutoSaveCheckPoint()
        while quest:GetStateInt("ArenaState") ~= 6 do
            if not quest:NewScriptFrame() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
        end
        if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
        arenaInsideHSP3 = quest:GetThingWithScriptName("ArenaInsideHSP")
        hero18 = quest:GetHero()
        quest:EntityTeleportToThing(hero18, arenaInsideHSP3, false)
        quest:FadeScreenOut(0.5, 0.5)
        while not quest:IsLevelLoaded("Arena") do
            if not quest:NewScriptFrame() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
        end
        if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
        AnimateCrowd(quest)
        quest:MiniMapSetAsEnabled(false)
        quest:SetStateInt("GoldMultiplier", 0)
        ::continue_2::
    end
    if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
    resource7 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resource8 = resources:NewResource()
    resource9 = resources:NewResource()
    resources:PrepareResource(resource8)
    pppuVar = resource8
    local hero4 = quest:GetHero()
    scratchValue = resources:TryAcquire(pppuVar, hero4, 4)
    while not scratchValue do
        if not quest:NewScriptFrame() then goto LAB_00f13f09 end
        pppuVar = resource8
        local hero5 = quest:GetHero()
        scratchValue = resources:TryAcquire(pppuVar, hero5, 4)
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f13f09 end
    resources:PrepareResource(resource9)
    pppuVar = resource9
    roth = quest:GetThingWithScriptName("Roth")
    scratchValue = resources:TryAcquire(pppuVar, roth, 4)
    while not scratchValue do
        if not quest:NewScriptFrame() then goto LAB_00f13f09 end
        pppuVar = resource9
        local roth2 = quest:GetThingWithScriptName("Roth")
        scratchValue = resources:TryAcquire(pppuVar, roth2, 4)
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f13f09 end
    actorMap5 = resources:NewActorMap()
    resources:SetActor(actorMap5, "Hero", resource8)
    resources:SetActor(actorMap5, "Roth", resource9)
    if quest:GetMasterGameState("WhisperKilledByHero") then
        if quest:IsActiveThreadTerminating() then goto LAB_00f13efc end
        goto FLOW_hoist_lab_00f13efc_1
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00f13efc end
        actorMap = "CS_ARENA_CELLS_LADYGREY_ONE"
    end
    goto FLOW_past_lab_00f13efc
    ::LAB_00f13efc::
    resources:DestroyActorMap(actorMap5)
    goto LAB_00f13f09
    ::FLOW_hoist_lab_00f13efc_1::
    actorMap = "CS_ARENA_CELLS_LADYGREY_TWO"
    ::FLOW_past_lab_00f13efc::
    resources:RunMacro(actorMap, actorMap5, false, true)
    resources:DestroyActorMap(actorMap5)
    resources:ReleaseResource(resource9)
    resources:ReleaseResource(resource8)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(resource7)
    cellsToHallOfHeroesEntrance = quest:GetThingWithScriptName("CellsToHallOfHeroesEntrance")
    hero19 = quest:GetHero()
    quest:EntityTeleportToThing(hero19, cellsToHallOfHeroesEntrance, false)
    angle = cellsToHallOfHeroesEntrance:GetAngleXY()
    hero6 = quest:GetHero()
    quest:EntitySetFacingAngle(hero6, angle, true)
    while not quest:IsLevelLoaded("ArenaHallOfHeroes") do
        if not quest:NewScriptFrame() then goto LAB_00f141b5 end
    end
    if not quest:IsActiveThreadTerminating() then
        resource9 = resources:NewResource()
        pppuVar = resource9
        local hero7 = quest:GetHero()
        resources:TryAcquire(pppuVar, hero7, 4)
        actorMap3 = resources:NewActorMap()
        resources:SetActor(actorMap3, "Hero", resource9)
        movie2 = resources:StartMovie("")
        resources:RunMacro("CS_ARENA_HOH_OUTRO", actorMap3, false, true)
        resources:DestroyMovie(movie2)
        resources:DestroyActorMap(actorMap3)
        quest:FadeScreenOut(0.5, 0.0)
        quest:ReturnAllConfiscatedItemsToHero()
        quest:MiniMapSetAsEnabled(true)
        quest:SetStateBool("MissionSucceeded", true)
        resources:ReleaseResource(resource9)
    end
    ::LAB_00f141b5::
    resources:DestroyActorMap(actorMap4)
    quest:DeregisterTimer(timerId)
    do return end
    ::LAB_00f13f09::
    resources:ReleaseResource(resource9)
    resources:ReleaseResource(resource8)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(resource7)
    resources:DestroyActorMap(actorMap4)
    quest:DeregisterTimer(timerId)
end

-- Q_Arena.Init (retail 0x00cfa700)
function Init(quest)
    quest:SetStateInt("GlobalCrowdTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    local scratchValue
    quest:AddQuestRegion("Q_Arena", "ArenaExterior")
    quest:AddQuestRegion("Q_Arena", "ArenaHallOfHeroes")
    quest:AddQuestRegion("Q_Arena", "ArenaCells")
    quest:AddQuestRegion("Q_Arena", "Arena")
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(SCRIPT_DEF.ArenaNakedBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.ArenaNakedBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(SCRIPT_DEF.ArenaNoDamageBoastCost), quest:ReadGlobalGameData(SCRIPT_DEF.ArenaNoDamageBoastReward), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLARENAOPPONENTS", 30, quest:ReadGlobalGameData(SCRIPT_DEF.ArenaShowNoMercyBoastAmount), quest:ReadGlobalGameData(SCRIPT_DEF.ArenaShowNoMercyBoastReward), true, "", 1)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_SPAREARENAOPPONENTS", 31, quest:ReadGlobalGameData(SCRIPT_DEF.ArenaShowMercyBoastAmount), quest:ReadGlobalGameData(SCRIPT_DEF.ArenaShowMercyBoastReward), false, "", 1)
    quest:SetStateBool("ChamLeaving", false)
    quest:SetStateInt("ArenaRound", 0)
    quest:SetStateInt("ArenaRoundWave", 0)
    quest:SetStateBool("NeedBertForSpeech", false)
    quest:SetStateBool("PlayerLeaving", false)
    quest:SetStateBool("PlayerWon", false)
    quest:SetStateInt("TotalCreatures_0", 0)
    quest:SetStateInt("TotalCreatures_1", 0)
    quest:SetStateInt("TotalCreatures_2", 0)
    quest:SetStateInt("ExtraCreatures", 0)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateInt("GoldMultiplier", 0)
    quest:SetStateInt("GoldTotal", 0)
    quest:SetStateBool("InHitCutsceneAlready", false)
    quest:SetStateInt("NewCrowdHappiness", 0)
    quest:SetStateInt("NewCrowdBaseLevel", 0)
    quest:SetStateInt("NewCrowdPoints", 200)
    quest:SetStateBool("PauseCrowdChecker", true)
    scratchValue = 0
    repeat
        if quest:IsActiveThreadTerminating() then return end
        quest:SetStateBool("ArenaSpawnNeeded_" .. scratchValue, false)
        scratchValue = scratchValue + 1
    until not (scratchValue < 16)
    if quest:IsActiveThreadTerminating() then return end
    quest:SetStateBool("FinalBattleCS", false)
    quest:SetStateInt("ArenaState", 0)
    quest:SetStateBool("WhisperNeededForCutscene", true)
end

-- Q_Arena.OnPersist (retail 0x00cfab10)
function OnPersist(quest, context)
    local arenaState = quest:GetStateInt("ArenaState") or 0
    arenaState = quest:PersistTransferInt(context, "ArenaState", arenaState)
    quest:SetStateInt("ArenaState", arenaState)
    local arenaRound = quest:GetStateInt("ArenaRound") or 0
    arenaRound = quest:PersistTransferInt(context, "ArenaRound", arenaRound)
    quest:SetStateInt("ArenaRound", arenaRound)
    local goldMultiplier = quest:GetStateInt("GoldMultiplier") or 0
    goldMultiplier = quest:PersistTransferInt(context, "GoldMultiplier", goldMultiplier)
    quest:SetStateInt("GoldMultiplier", goldMultiplier)
    local goldTotal = quest:GetStateInt("GoldTotal") or 0
    goldTotal = quest:PersistTransferInt(context, "GoldTotal", goldTotal)
    quest:SetStateInt("GoldTotal", goldTotal)
    local newCrowdPoints = quest:GetStateInt("NewCrowdPoints") or 0
    newCrowdPoints = quest:PersistTransferInt(context, "NewCrowdPoints", newCrowdPoints)
    quest:SetStateInt("NewCrowdPoints", newCrowdPoints)
    local chamLeaving = quest:GetStateBool("ChamLeaving") or false
    chamLeaving = quest:PersistTransferBool(context, "ChamLeaving", chamLeaving)
    quest:SetStateBool("ChamLeaving", chamLeaving)
    local playerLeaving = quest:GetStateBool("PlayerLeaving") or false
    playerLeaving = quest:PersistTransferBool(context, "PlayerLeaving", playerLeaving)
    quest:SetStateBool("PlayerLeaving", playerLeaving)
end

-- Q_Arena.WatchForTermination (retail 0x00f14300)
function WatchForTermination(quest)
    while not quest:GetStateBool("MissionFailed") and not quest:GetStateBool("MissionSucceeded") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetTimeAsStopped(false)
    if not quest:GetStateBool("MissionFailed") then
        if quest:IsActiveThreadTerminating() then return end
        quest:GiveHeroGold(quest:GetStateInt("GoldTotal"))
        if not quest:GetMasterGameState("WhisperKilledByHero") then
            if quest:IsActiveThreadTerminating() then return end
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.ArenaSpareWhisperMorality))
        end
        local arenaExtEntrance = quest:GetThingWithScriptName("ArenaExtEntrance")
        quest:SetAllowScreenFadingOnNextRegionChange(false)
        local pThingToMove = quest:GetHero()
        quest:EntityTeleportToThing(pThingToMove, arenaExtEntrance, false)
        while not quest:IsRegionLoaded("ArenaExterior") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        local getActiveQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsCompleted(getActiveQuestName, true, false, false)
        quest:FadeScreenIn()
        quest:SetAllowScreenFadingOnNextRegionChange(true)
        quest:SetMasterGameState("ArenaFinished", true)
    else
        if quest:IsActiveThreadTerminating() then return end
    end
    quest:SetTeleportingAsActive(true)
    local getActiveQuestName2 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName2, 0)
end

-- Q_Arena.CrowdChecker (retail 0x00f1cce0)
function CrowdChecker(quest)
    local scratchValue2, scratchValue3, arenaAudience01, scratchValue4, scratchValue5
    local conversationId, scratchValue6, infoElement, sequence1, switch4, hero, scratchValue7
    local scratchValue8, cellsExitToArena, scratchValue9, scratchValue10, scratchValue11
    local crowdSoundSource, scratchValue12
    local scratchValue = {0, 200, 400}
    scratchValue5 = quest:RegisterTimer()
    local timerId = scratchValue5
    quest:SetTimer(timerId, 0)
    local timerId2 = quest:RegisterTimer()
    quest:SetTimer(timerId2, 15)
    scratchValue6 = -1
    scratchValue2 = quest:IsActiveThreadTerminating()
    ::FLOW_loop_head_lab_00f1e382::
    scratchValue8 = scratchValue7
    if scratchValue2 then goto FLOW_loop_exit_lab_00f1e382 end
    scratchValue2 = quest:IsLevelLoaded("ArenaCells")
    while scratchValue2 do
        quest:NewScriptFrame()
        scratchValue2 = quest:IsActiveThreadTerminating()
        scratchValue7 = 0
        if scratchValue2 then goto LAB_00f1e423 end
        scratchValue10 = 0
        cellsExitToArena = quest:GetThingWithScriptName("CellsExitToArena")
        scratchValue2 = quest:IsLevelLoaded("ArenaCells")
        while scratchValue2 do
            quest:NewScriptFrame()
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then
                cellsExitToArena = nil
                goto LAB_00f1eba3
            end
            conversationId = quest:RetailRandModulo(3)
            if conversationId == 0 then
                scratchValue10 = quest:PlayCriteriaSoundOnThing(cellsExitToArena, "ARENA_HECKLE_BIG_MUFFLED_LOOP")
            else
                if conversationId == 1 then
                    scratchValue10 = quest:PlayCriteriaSoundOnThing(cellsExitToArena, "ARENA_APPLAUSE_MEDIUM_MUFFLED_LOOP")
                    goto LAB_00f1cf00
                end
                if conversationId == 2 then
                    scratchValue10 = quest:PlayCriteriaSoundOnThing(cellsExitToArena, "ARENA_CHANT_MUFFLED_LOOP")
                    goto LAB_00f1cf00
                end
            end
            ::LAB_00f1cf00::
            infoElement = quest:RetailRandModulo(32768)
            infoElement = infoElement % 10
            quest:Pause(infoElement + 2.0)
            quest:StopSound(scratchValue10)
            scratchValue2 = quest:IsLevelLoaded("ArenaCells")
        end
        scratchValue2 = quest:IsActiveThreadTerminating()
        cellsExitToArena = nil
        if scratchValue2 then
            goto LAB_00f1eba3
        end
        scratchValue2 = quest:IsLevelLoaded("ArenaCells")
    end
    scratchValue2 = quest:IsActiveThreadTerminating()
    if scratchValue2 then
        quest:DeregisterTimer(timerId2)
        quest:DeregisterTimer(timerId)
        return
    end
    scratchValue2 = quest:IsLevelLoaded("Arena")
    if scratchValue2 then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if not scratchValue2 then
            infoElement = quest:AddQuestInfoBar(400.0, 0.0, {R = 255, G = 255, B = 0, A = 255}, {R = 255, G = 255, B = 0, A = 255}, "HUD_EXPR_CLAP", "", 1.0)
            quest:DisplayQuestInfo(true)
            crowdSoundSource = quest:GetAllThingsWithScriptName("CrowdSoundSource")
            scratchValue11 = (function(n) local t = {}; for i = 1, n do t[i] = 0 end; return t end)(#crowdSoundSource)
            arenaAudience01 = quest:GetAllThingsWithDefName("OBJECT_ARENA_AUDIENCE_01")
            scratchValue3 = 0
            scratchValue2 = quest:IsLevelLoaded("Arena")
            if scratchValue2 then
                goto LAB_00f1d1e0
            end
            goto FLOW_hoist_lab_00f1d1e0_1
        end
        goto FLOW_hoist_lab_00f1d1e0_2
    end
    goto FLOW_past_lab_00f1d1e0
    ::LAB_00f1d1e0::
    quest:NewScriptFrame()
    scratchValue2 = quest:IsActiveThreadTerminating()
    if scratchValue2 then
        quest:DeregisterTimer(timerId2)
        quest:DeregisterTimer(timerId)
        return
    end
    if not quest:GetStateBool("PauseCrowdChecker") or scratchValue3 == 0 then goto LAB_00f1d2e6 end
    scratchValue2 = quest:IsActiveThreadTerminating()
    if not scratchValue2 then
        repeat
            if not quest:GetStateBool("PauseCrowdChecker") then
                goto LAB_00f1d260
            else
                scratchValue2 = quest:IsLevelLoaded("Arena")
                if not scratchValue2 then goto LAB_00f1d260 end
                scratchValue2 = true
            end
            goto FLOW_past_lab_00f1d260
            ::LAB_00f1d260::
            scratchValue2 = false
            ::FLOW_past_lab_00f1d260::
            if not scratchValue2 then goto LAB_00f1d2d7 end
            quest:NewScriptFrame()
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(timerId)
                do return end
            end
        until false
    end
    goto LAB_00f1eb80
    ::FLOW_hoist_lab_00f1d1e0_1::
    scratchValue2 = quest:IsActiveThreadTerminating()
    if not scratchValue2 then
        quest:RemoveQuestInfoElement(infoElement)
        quest:DisplayQuestInfo(false)
        goto LAB_00f1e3c7
    end
    ::LAB_00f1eb80::
    ::FLOW_hoist_lab_00f1d1e0_2::
    goto LAB_00f1eba3
    ::FLOW_past_lab_00f1d1e0::
    ::LAB_00f1e3c7::
    quest:NewScriptFrame()
    scratchValue2 = quest:IsActiveThreadTerminating()
    goto FLOW_loop_head_lab_00f1e382
    ::FLOW_loop_exit_lab_00f1e382::
    ::LAB_00f1eba3::
    quest:DeregisterTimer(timerId2)
    quest:DeregisterTimer(timerId)
    do return end
    ::LAB_00f1e423::
    goto LAB_00f1eba3
    ::LAB_00f1d2d7::
    scratchValue2 = quest:IsActiveThreadTerminating()
    if scratchValue2 then
        quest:DeregisterTimer(timerId2)
        quest:DeregisterTimer(timerId)
        return
    end
    ::LAB_00f1d2e6::
    scratchValue2 = quest:IsLevelLoaded("Arena")
    if not scratchValue2 then goto LAB_00f1e350 end
    scratchValue2 = quest:IsActiveThreadTerminating()
    if scratchValue2 then
        quest:DeregisterTimer(timerId2)
        quest:DeregisterTimer(timerId)
        return
    end
    quest:UpdateQuestInfoBar(infoElement, quest:GetStateInt("NewCrowdPoints"), -1.0, -1.0)
    conversationId = quest:GetTimer(timerId)
    if conversationId < 1 then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then
            quest:DeregisterTimer(timerId2)
            quest:DeregisterTimer(timerId)
            return
        end
        quest:SetTimer(timerId, 5)
        scratchValue5 = 2
        repeat
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(timerId)
                return
            end
            if not ((scratchValue[scratchValue5 + 1] <= quest:GetStateInt("NewCrowdPoints")) or (scratchValue5 == 0)) then scratchValue5 = scratchValue5 - 1; conversationId = scratchValue6; goto continue_2 end
            scratchValue2 = quest:IsActiveThreadTerminating()
            conversationId = scratchValue5
            if scratchValue2 then
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(timerId)
                return
            end
            break
            scratchValue5 = scratchValue5 - 1
            conversationId = scratchValue6
            ::continue_2::
        until not (-1 < scratchValue5)
        scratchValue6 = conversationId
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then
            quest:DeregisterTimer(timerId2)
            quest:DeregisterTimer(timerId)
            return
        end
        scratchValue5 = quest:GetStateInt("NewCrowdPoints") - 4
        quest:SetStateInt("NewCrowdPoints", scratchValue5)
        if 400 < scratchValue5 then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(timerId)
                return
            end
            quest:SetStateInt("NewCrowdPoints", 400)
        end
        if scratchValue6 ~= quest:GetStateInt("NewCrowdHappiness") or (-1 ~= quest:GetStateInt("NewCrowdBaseLevel")) then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(timerId)
                return
            end
            scratchValue5 = quest:GetStateInt("NewCrowdBaseLevel")
            quest:SetStateInt("NewCrowdHappiness", scratchValue6)
            scratchValue10 = 0
            scratchValue4 = scratchValue5
            if #crowdSoundSource ~= 0 then
                conversationId = 0
                repeat
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then
                        quest:DeregisterTimer(timerId2)
                        quest:DeregisterTimer(timerId)
                        return
                    end
                    quest:StopSound(scratchValue11[scratchValue10 + 1])
                    quest:Pause(0.2)
                    scratchValue9 = quest:PlayCriteriaSoundOnThing(crowdSoundSource[conversationId / 12 + 1], quest:GetStateString("CrowdLoopTags_" .. scratchValue5 .. "_" .. scratchValue6))
                    scratchValue11[scratchValue10 + 1] = scratchValue9
                    scratchValue3 = 0
                    quest:Pause(0.3)
                    scratchValue10 = scratchValue10 + 1
                    conversationId = conversationId + 12
                until not (scratchValue10 < (#crowdSoundSource))
            end
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(timerId)
                return
            end
        end
        scratchValue10 = 0
        if #arenaAudience01 ~= 0 then
            scratchValue5 = 0
            repeat
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then
                    quest:DeregisterTimer(timerId2)
                    quest:DeregisterTimer(timerId)
                    return
                end
                local elem_1 = arenaAudience01[scratchValue5 / 12 + 1]
                local pPos = elem_1:GetPos()
                scratchValue2 = quest:IsCameraPosOnScreen(pPos)
                if not scratchValue2 then scratchValue10 = scratchValue10 + 1; scratchValue5 = scratchValue5 + 12; goto continue_4 end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then
                    quest:DeregisterTimer(timerId2)
                    quest:DeregisterTimer(timerId)
                    return
                end
                if scratchValue6 == 0 then
                    scratchValue9 = quest:RetailRandModulo(32768)
                    scratchValue9 = scratchValue9 & 0x80000001
                    scratchValue2 = scratchValue9 == 0
                    if scratchValue9 < 0 then
                        scratchValue2 = (scratchValue9 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if scratchValue2 then
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then
                            quest:DeregisterTimer(timerId2)
                            quest:DeregisterTimer(timerId)
                            return
                        end
                        quest:EntityPlayObjectAnimation(arenaAudience01[scratchValue5 / 12 + 1], "CLAP_LOOP_02", true)
                    else
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then
                            quest:DeregisterTimer(timerId2)
                            quest:DeregisterTimer(timerId)
                            return
                        end
                        quest:EntityPlayObjectAnimation(arenaAudience01[scratchValue5 / 12 + 1], "IDLE_BREATHE_01", true)
                    end
                elseif scratchValue6 == 1 then
                    scratchValue9 = quest:RetailRandModulo(32768)
                    scratchValue9 = scratchValue9 & 0x80000001
                    scratchValue2 = scratchValue9 == 0
                    if scratchValue9 < 0 then
                        scratchValue2 = (scratchValue9 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if scratchValue2 then
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then
                            quest:DeregisterTimer(timerId2)
                            quest:DeregisterTimer(timerId)
                            return
                        end
                        quest:EntityPlayObjectAnimation(arenaAudience01[scratchValue5 / 12 + 1], "CLAP_LOOP_02", true)
                    else
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then
                            quest:DeregisterTimer(timerId2)
                            quest:DeregisterTimer(timerId)
                            return
                        end
                        quest:EntityPlayObjectAnimation(arenaAudience01[scratchValue5 / 12 + 1], "CHEER_LOOP_01", true)
                    end
                elseif scratchValue6 == 2 then
                    quest:EntityPlayObjectAnimation(arenaAudience01[scratchValue5 / 12 + 1], "CHEER_LOOP_01", true)
                end
                scratchValue10 = scratchValue10 + 1
                scratchValue5 = scratchValue5 + 12
                ::continue_4::
            until not (scratchValue10 < (#arenaAudience01))
        end
        scratchValue2 = quest:IsActiveThreadTerminating()
        scratchValue5 = timerId
        if scratchValue2 then
            quest:DeregisterTimer(timerId2)
            quest:DeregisterTimer(timerId)
            return
        end
    end
    conversationId = quest:GetTimer(timerId2)
    if conversationId < 1 then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then
            quest:DeregisterTimer(timerId2)
            quest:DeregisterTimer(timerId)
            return
        end
        sequence1 = false
        sequence1 = scratchValue3 ~= 0
        if sequence1 then
            scratchValue2 = quest:IsSoundPlaying(0)
            sequence1 = scratchValue2
        end
        if sequence1 then goto LAB_00f1e2fb end
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then
            quest:DeregisterTimer(timerId2)
            quest:DeregisterTimer(timerId)
            return
        end
        scratchValue3 = 1
        local singleShouter = quest:GetThingWithScriptName("SingleShouter")
        scratchValue12 = quest:PlayCriteriaSoundOnThing(singleShouter, quest:GetStateString("CrowdLoopTags_" .. scratchValue4 .. "_" .. scratchValue6))
        conversationId = quest:RetailRandModulo(32768)
        quest:SetTimer(timerId2, conversationId % quest:ReadGlobalGameData(SCRIPT_DEF.ArenaCrowdShoutDelayRandom) + quest:ReadGlobalGameData(SCRIPT_DEF.ArenaCrowdShoutDelay))
        conversationId = quest:AddNewConversation(singleShouter, false, false)
        hero = quest:GetHero()
        quest:AddPersonToConversation(conversationId, hero)
        if quest:GetStateInt("NewCrowdHappiness") < 1 then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(timerId)
                return
            end
            local switch = quest:GetStateInt("ArenaRound")
            repeat
                if switch == 0 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_WASPS", singleShouter, hero, false)
                    break
                elseif switch == 1 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_HOBBES", singleShouter, hero, false)
                    break
                elseif switch == 2 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_BALVERINES", singleShouter, hero, false)
                    break
                elseif switch == 3 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_UNDEAD", singleShouter, hero, false)
                    break
                elseif switch == 4 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_BANDITS", singleShouter, hero, false)
                    break
                elseif switch == 5 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_EARTH_TROLL", singleShouter, hero, false)
                    break
                elseif switch == 6 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_ROCK_TROLL", singleShouter, hero, false)
                    break
                elseif switch == 7 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_SCORPION", singleShouter, hero, false)
                    break
                elseif switch == 8 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_WASPS", singleShouter, hero, false)
                    break
                else
                    goto FLOW_native_label_1
                end
            until true
            goto LAB_00f1e2e9
        end
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then
            quest:DeregisterTimer(timerId2)
            quest:DeregisterTimer(timerId)
            return
        end
        scratchValue10 = quest:RetailRandModulo(32768)
        scratchValue10 = scratchValue10 & 0x80000001
        scratchValue2 = scratchValue10 == 0
        if scratchValue10 < 0 then
            scratchValue2 = (scratchValue10 - 1 | 0xfffffffe) == 0xffffffff
        end
        if scratchValue2 then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then
                quest:DeregisterTimer(timerId2)
                quest:DeregisterTimer(timerId)
                return
            end
            local switch3 = quest:GetStateInt("ArenaRound")
            repeat
                if switch3 == 0 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_WASPS", singleShouter, hero, false)
                    break
                elseif switch3 == 1 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_HOBBES", singleShouter, hero, false)
                    break
                elseif switch3 == 2 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_BALVERINES", singleShouter, hero, false)
                    break
                elseif switch3 == 3 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_UNDEAD", singleShouter, hero, false)
                    break
                elseif switch3 == 4 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_BANDITS", singleShouter, hero, false)
                    break
                elseif switch3 == 5 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_EARTH_TROLL", singleShouter, hero, false)
                    break
                elseif switch3 == 6 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_ROCK_TROLL", singleShouter, hero, false)
                    break
                elseif switch3 == 7 then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_SCORPION", singleShouter, hero, false)
                    break
                else
                    goto FLOW_native_label_1
                end
            until true
            goto LAB_00f1e2e9
        end
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then
            quest:DeregisterTimer(timerId2)
            quest:DeregisterTimer(timerId)
            return
        end
        scratchValue5 = quest:GetHeroTitle()
        switch4 = scratchValue5
        repeat
            if switch4 == 1 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_17", singleShouter, hero, false)
                break
            elseif switch4 == 2 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_20", singleShouter, hero, false)
                break
            elseif switch4 == 3 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_11", singleShouter, hero, false)
                break
            elseif switch4 == 4 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_06", singleShouter, hero, false)
                break
            elseif switch4 == 5 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_03", singleShouter, hero, false)
                break
            elseif switch4 == 6 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_12", singleShouter, hero, false)
                break
            elseif switch4 == 7 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_04", singleShouter, hero, false)
                break
            elseif switch4 == 8 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_15", singleShouter, hero, false)
                break
            elseif switch4 == 9 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_10", singleShouter, hero, false)
                break
            elseif switch4 == 10 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_13", singleShouter, hero, false)
                break
            elseif switch4 == 11 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_07", singleShouter, hero, false)
                break
            elseif switch4 == 12 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_16", singleShouter, hero, false)
                break
            elseif switch4 == 13 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_18", singleShouter, hero, false)
                break
            elseif switch4 == 14 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_09", singleShouter, hero, false)
                break
            elseif switch4 == 15 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_08", singleShouter, hero, false)
                break
            elseif switch4 == 16 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_19", singleShouter, hero, false)
                break
            elseif switch4 == 17 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_01", singleShouter, hero, false)
                break
            elseif switch4 == 18 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_14", singleShouter, hero, false)
                break
            elseif switch4 == 19 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_05", singleShouter, hero, false)
                break
            elseif switch4 == 20 then
                hero = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_02", singleShouter, hero, false)
                break
            else
                goto FLOW_native_label_1
            end
        until true
        ::LAB_00f1e2e9::
        ::FLOW_native_label_1::
        scratchValue5 = timerId
    end
    ::LAB_00f1e2fb::
    hero = quest:GetHero()
    scratchValue2 = hero ~= nil and hero:MsgIsHitBy("")
    if scratchValue2 then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if scratchValue2 then
            quest:DeregisterTimer(timerId2)
            quest:DeregisterTimer(timerId)
            return
        end
        quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") - 5)
    end
    ::LAB_00f1e350::
    scratchValue2 = quest:IsLevelLoaded("Arena")
    if not scratchValue2 then
        scratchValue2 = quest:IsActiveThreadTerminating()
        if not scratchValue2 then
            quest:RemoveQuestInfoElement(infoElement)
            quest:DisplayQuestInfo(false)
            goto LAB_00f1e3c7
        end
        goto LAB_00f1eba3
    end
    goto LAB_00f1d1e0
end

-- Q_Arena.InitialiseVariables (retail 0x00f25840)
function InitialiseVariables(quest)
    quest:InitialiseArenaRounds()
    quest:SetStateString("CrowdLoopTags_0_0", "ARENA_HECKLE_SMALL_LOOP")
    quest:SetStateString("CrowdLoopTags_0_1", "ARENA_CHANT_LOOP")
    quest:SetStateString("CrowdLoopTags_0_2", "ARENA_APPLAUSE_SMALL_LOOP")
    quest:SetStateString("CrowdLoopTags_0_3", "ARENA_CHEER_CROWD")
    quest:SetStateString("CrowdLoopTags_0_4", "ARENA_AWWW")
    quest:SetStateString("CrowdLoopTags_1_0", "ARENA_HECKLE_SMALL_LOOP")
    quest:SetStateString("CrowdLoopTags_1_1", "ARENA_CHANT_LOOP")
    quest:SetStateString("CrowdLoopTags_1_2", "ARENA_APPLAUSE_MEDIUM_LOOP")
    quest:SetStateString("CrowdLoopTags_1_3", "ARENA_CHEER_CROWD")
    quest:SetStateString("CrowdLoopTags_1_4", "ARENA_AWWW")
    quest:SetStateString("CrowdLoopTags_2_0", "ARENA_HECKLE_MEDIUM_LOOP")
    quest:SetStateString("CrowdLoopTags_2_1", "ARENA_CHANT_LOOP")
    quest:SetStateString("CrowdLoopTags_2_2", "ARENA_APPLAUSE_BIG_LOOP")
    quest:SetStateString("CrowdLoopTags_2_3", "ARENA_CHEER_CROWD")
    quest:SetStateString("CrowdLoopTags_2_4", "ARENA_AWWW")
    quest:SetStateString("CrowdLoopTags_3_0", "ARENA_HECKLE_BIG_LOOP")
    quest:SetStateString("CrowdLoopTags_3_1", "ARENA_CHANT_LOOP")
    quest:SetStateString("CrowdLoopTags_3_2", "ARENA_APPLAUSE_BIG_LOOP")
    quest:SetStateString("CrowdLoopTags_3_3", "ARENA_CHEER_CROWD")
    quest:SetStateString("CrowdLoopTags_3_4", "ARENA_AWWW")
end

-- Q_Arena.AnimateCrowd (retail 0x00f1ec70)
function AnimateCrowd(quest)
    local scratchValue, i_stk_1c_1, scratchValue2
    local audienceMember = quest:GetAllThingsWithScriptName("AudienceMember")
    local retailRandModulo = #audienceMember
    if not (0 < retailRandModulo) then return end
    scratchValue = 0
    i_stk_1c_1 = retailRandModulo
    repeat
        scratchValue2 = audienceMember[scratchValue / 12 + 1]
        local switch1 = quest:RetailRandModulo(32768) % 6
        repeat
            if switch1 == 0 then
                quest:EntityPlayObjectAnimation(scratchValue2, "CHEER_LOOP_01", true)
                break
            elseif switch1 == 1 then
                quest:EntityPlayObjectAnimation(scratchValue2, "CHEER_LOOP_02", true)
                break
            elseif switch1 == 2 then
                quest:EntityPlayObjectAnimation(scratchValue2, "CLAP_LOOP_02", true)
                break
            elseif switch1 == 3 then
                quest:EntityPlayObjectAnimation(scratchValue2, "SHOUT_LOOP_01", true)
                break
            elseif switch1 == 4 then
                quest:EntityPlayObjectAnimation(scratchValue2, "THUMBS_DOWN_LOOP_01", true)
                break
            else
                quest:EntityPlayObjectAnimation(scratchValue2, "WAVE_LOOP_01", true)
            end
        until true
        quest:EntitySetCutsceneBehaviour(scratchValue2, CUTSCENE_BEHAVIOUR_NOT_PAUSED)
        scratchValue2 = nil
        scratchValue = scratchValue + 12
        i_stk_1c_1 = i_stk_1c_1 - 1
    until i_stk_1c_1 == 0
end

-- Q_Arena.GetFanfareMusic (retail 0x00f14270)
function GetFanfareMusic(quest, fanfare)
    local switch1 = fanfare
    repeat
        if switch1 == 1 then
            return 36
        elseif switch1 == 2 then
            return 37
        elseif switch1 == 3 then
            return 38
        elseif switch1 == 4 then
            return 39
        elseif switch1 == 5 then
            return 40
        elseif switch1 == 6 then
            return 41
        elseif switch1 == 7 then
            return 42
        elseif switch1 == 8 then
            return 43
        elseif switch1 == 9 then
            return 44
        else
            return 35
        end
    until true
end

-- Q_Arena.PlayWave (retail 0x00f1eed0)
function PlayWave(quest)
    local resources = quest:RetailResources()
    local function parseGameInteger(text)
        local value, negative = 0, false
        for position = 1, #text do
            local character = text:sub(position, position)
            if character == "." then break end
            if character == "-" then
                negative = true
            elseif character >= "0" and character <= "9" then
                value = (value * 10 + tonumber(character)) % 4294967296
            end
        end
        if negative then value = (-value) % 4294967296 end
        -- Match the game's signed 32-bit result, including overflow.
        if value >= 2147483648 then value = value - 4294967296 end
        return value
    end
    local b1, b2, scratchValue, scratchValue2, predicateResult38, scratchValue4, creatureCounterIds
    local creatureGroupIndex, creatureType, ctr_10c, ctr_fc, timerId2, timerId3, timerId4
    local scratchValue7, scratchValue8, scratchValue9, scratchValue10, scratchValue11
    local scratchValue12, scratchValue13, scratchValue14, arenaRoundWave, questionAnswer
    local conversationId, count, scratchValue16, scratchValue17, scratchValue18, scratchValue19
    local scratchValue20, initialCreatureCounts, sequence1, getPos, hero9, whisperAlly, hero10
    local whisperAlly3, hero11, resource, resource2, hero15, scratchValue29
    local bigCreatureSpawnPoint, arenaSpawn, savedCreatureGroupIndex, totalCreaturesIndex
    local totalCreaturesIndex2, retailRandModulo, scratchValue30, multipleBigCreatureSpawnPoint
    local arenaEnemy, arenaSpawn2, movie, getDataString, resource3, scratchValue31, scratchValue32
    local createCreatureNearby, scratchValue34
    bigCreatureSpawnPoint = quest:GetThingWithScriptName("BigCreatureSpawnPoint")
    local timerId = quest:RegisterTimer()
    quest:SetStateBool("PauseCrowdChecker", false)
    local timerId5 = timerId
    if not quest:GetStateBool("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_ShortWave") and (quest:GetStateInt("ArenaRound") ~= 0 or (quest:GetStateInt("ArenaRoundWave") ~= 0)) then
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); goto LAB_00f2119d end
        local hero = quest:GetHero()
        conversationId = quest:AddNewConversation(hero, false, false)
        local hero2 = quest:GetHero()
        hero15 = quest:GetHero()
        quest:AddLineToConversation(conversationId, "TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_THREE", hero15, hero2, false)
        timerId2 = timerId
        quest:SetTimer(timerId, 2)
        while 0 < quest:GetTimer(timerId5) do
            if not quest:NewScriptFrame() then goto LAB_00f21195 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f21195 end
        local hero3 = quest:GetHero()
        conversationId = quest:AddNewConversation(hero3, false, false)
        local hero4 = quest:GetHero()
        hero15 = quest:GetHero()
        quest:AddLineToConversation(conversationId, "TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_TWO", hero15, hero4, false)
        timerId3 = timerId
        quest:SetTimer(timerId, 2)
        while 0 < quest:GetTimer(timerId5) do
            if not quest:NewScriptFrame() then goto LAB_00f20cc5 end
        end
        if not quest:IsActiveThreadTerminating() then
            local hero5 = quest:GetHero()
            conversationId = quest:AddNewConversation(hero5, false, false)
            local hero6 = quest:GetHero()
            hero15 = quest:GetHero()
            quest:AddLineToConversation(conversationId, "TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_ONE", hero15, hero6, false)
            timerId4 = timerId
            quest:SetTimer(timerId, 2)
            while 0 < quest:GetTimer(timerId5) do
                if not quest:NewScriptFrame() then goto LAB_00f20cc5 end
            end
            if not quest:IsActiveThreadTerminating() then
                local hero7 = quest:GetHero()
                conversationId = quest:AddNewConversation(hero7, false, false)
                local hero8 = quest:GetHero()
                hero15 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_GO", hero15, hero8, false)
                goto LAB_00f1f251
            end
        end
        quest:DeregisterTimer(timerId)
        goto LAB_00f2119d
    end
    ::LAB_00f1f251::
    multipleBigCreatureSpawnPoint = quest:GetAllThingsWithScriptName("MultipleBigCreatureSpawnPoint")
    count = #multipleBigCreatureSpawnPoint
    arenaSpawn2 = quest:GetAllThingsWithScriptName("ArenaSpawn")
    conversationId = #arenaSpawn2
    scratchValue18 = conversationId
    hero9 = quest:GetHero()
    arenaSpawn = quest:GetFurthestWithScriptName(hero9, "ArenaSpawn")
    if arenaSpawn == nil then
        getDataString = ""
    else
        getDataString = arenaSpawn:GetDataString()
    end
    scratchValue7 = parseGameInteger(getDataString)
    scratchValue16 = scratchValue7
    scratchValue34 = (scratchValue7 - 1) & 0xffffffff
    scratchValue4 = 1
    if scratchValue34 == 0xffffffff then
        if quest:IsActiveThreadTerminating() then goto LAB_00f2116f end
        scratchValue34 = (conversationId - 1) & 0xffffffff
    end
    arenaEnemy = quest:GetAllThingsWithScriptName("ArenaEnemy")
    sequence1 = quest:GetStateInt("ArenaRound") == 0 and quest:GetStateInt("ArenaRoundWave") == 0
    if sequence1 then
        scratchValue8 = #arenaEnemy * 12 >> 31
        sequence1 = #arenaEnemy ~= 0
    end
    if sequence1 then
        if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
        quest:SetStateInt("TotalCreatures_0", #arenaEnemy)
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
        ctr_10c = 0
        if 0 < quest:GetStateInt("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_NumWaveCreatures") then
            totalCreaturesIndex = 0
            creatureGroupIndex = 0
            repeat
                savedCreatureGroupIndex = creatureGroupIndex
                if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
                quest:SetStateInt("TotalCreatures_" .. totalCreaturesIndex, 0)
                scratchValue17 = 0
                if 0 < quest:GetStateInt("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_NumCreatures") then
                    repeat
                        if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
                        if quest:GetStateInt("ArenaRound") < 5 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
                            if scratchValue4 == 0 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
                                scratchValue20 = 0
                                if 0 < conversationId then
                                    scratchValue19 = 0
                                    repeat
                                        scratchValue9 = scratchValue19
                                        scratchValue29 = arenaSpawn2[scratchValue19 / 12 + 1]:GetDataString()
                                        if parseGameInteger(scratchValue29) == scratchValue34 then
                                            if not quest:IsActiveThreadTerminating() then
                                                scratchValue32 = nil
                                                if quest:GetStateInt("ArenaRound") == 3 then
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:SetCreatureCreationDelayFrames(1)
                                                        creatureType = quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_CreatureType")
                                                        getPos = arenaSpawn2[scratchValue9 / 12 + 1]:GetPos()
                                                        scratchValue32 = quest:CreateCreature(creatureType, getPos, "ArenaEnemy", true)
                                                        goto LAB_00f1fcce
                                                    end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    quest:SetCreatureCreationDelayFrames(1)
                                                    creatureType = quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_CreatureType")
                                                    getPos = arenaSpawn2[scratchValue9 / 12 + 1]:GetPos()
                                                    scratchValue32 = quest:CreateCreatureNearby(creatureType, getPos, 1.0, "ArenaEnemy")
                                                    goto LAB_00f1fcce
                                                end
                                                goto FLOW_past_lab_00f1fcce
                                                ::LAB_00f1fcce::
                                                quest:ResetCreatureCreationDelayFrames()
                                                quest:EntitySetCutsceneBehaviour(scratchValue32, CUTSCENE_BEHAVIOUR_PAUSED)
                                                whisperAlly = quest:GetThingWithScriptName("WhisperAlly")
                                                if not (whisperAlly ~= nil and whisperAlly:IsAlive()) then
                                                    if not quest:IsActiveThreadTerminating() then hero10 = quest:GetHero(); goto LAB_00f1fe19 end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    retailRandModulo = quest:RetailRandModulo(32768) & 0x80000001
                                                    scratchValue = retailRandModulo == 0
                                                    if retailRandModulo < 0 then
                                                        scratchValue = (retailRandModulo - 1 | 0xfffffffe) == 0xffffffff
                                                    end
                                                    if scratchValue then
                                                        if not quest:IsActiveThreadTerminating() then
                                                            local whisperAlly2 = quest:GetThingWithScriptName("WhisperAlly")
                                                            quest:GiveThingBestEnemyTarget(scratchValue32, whisperAlly2)
                                                            goto LAB_00f1fe22
                                                        end
                                                    elseif not quest:IsActiveThreadTerminating() then
                                                        hero10 = quest:GetHero()
                                                        goto LAB_00f1fe19
                                                    end
                                                end
                                                goto FLOW_past_lab_00f1fe19
                                                ::LAB_00f1fe19::
                                                quest:GiveThingBestEnemyTarget(scratchValue32, hero10)
                                                goto LAB_00f1fe22
                                                ::FLOW_past_lab_00f1fe19::
                                                goto FLOW_past_lab_00f1fe22
                                                ::LAB_00f1fe22::
                                                quest:SetStateBool("ArenaSpawnNeeded_" .. scratchValue34, true)
                                                scratchValue9 = scratchValue19
                                                conversationId = scratchValue18
                                                goto LAB_00f1fe48
                                                ::FLOW_past_lab_00f1fe22::
                                                ::FLOW_past_lab_00f1fcce::
                                            end
                                            goto LAB_00f20cc5
                                        end
                                        ::LAB_00f1fe48::
                                        scratchValue20 = scratchValue20 + 1
                                        scratchValue19 = scratchValue9 + 12
                                    until not (scratchValue20 < conversationId)
                                end
                                scratchValue34 = (scratchValue34 - 1) & 0xffffffff
                                if scratchValue34 == 0xffffffff then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
                                    scratchValue34 = (conversationId - 1) & 0xffffffff
                                end
                                scratchValue4 = 1
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
                                scratchValue19 = 0
                                if 0 < conversationId then
                                    scratchValue20 = 0
                                    repeat
                                        scratchValue10 = scratchValue20
                                        scratchValue29 = arenaSpawn2[scratchValue20 / 12 + 1]:GetDataString()
                                        if parseGameInteger(scratchValue29) == scratchValue16 then
                                            if not quest:IsActiveThreadTerminating() then
                                                if quest:GetStateInt("ArenaRound") == 3 then
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:SetCreatureCreationDelayFrames(1)
                                                        creatureType = quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_CreatureType")
                                                        getPos = arenaSpawn2[scratchValue10 / 12 + 1]:GetPos()
                                                        scratchValue31 = quest:CreateCreature(creatureType, getPos, "ArenaEnemy", true)
                                                        goto LAB_00f1f934
                                                    end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    quest:SetCreatureCreationDelayFrames(1)
                                                    creatureType = quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_CreatureType")
                                                    getPos = arenaSpawn2[scratchValue10 / 12 + 1]:GetPos()
                                                    scratchValue31 = quest:CreateCreatureNearby(creatureType, getPos, 1.0, "ArenaEnemy")
                                                    goto LAB_00f1f934
                                                end
                                                goto FLOW_past_lab_00f1f934
                                                ::LAB_00f1f934::
                                                quest:ResetCreatureCreationDelayFrames()
                                                quest:EntitySetCutsceneBehaviour(scratchValue31, CUTSCENE_BEHAVIOUR_PAUSED)
                                                whisperAlly3 = quest:GetThingWithScriptName("WhisperAlly")
                                                if not (whisperAlly3 ~= nil and whisperAlly3:IsAlive()) then
                                                    if not quest:IsActiveThreadTerminating() then hero11 = quest:GetHero(); goto LAB_00f1fa88 end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    retailRandModulo = quest:RetailRandModulo(32768) & 0x80000001
                                                    scratchValue2 = retailRandModulo == 0
                                                    if retailRandModulo < 0 then
                                                        scratchValue2 = (retailRandModulo - 1 | 0xfffffffe) == 0xffffffff
                                                    end
                                                    if scratchValue2 then
                                                        if not quest:IsActiveThreadTerminating() then
                                                            local whisperAlly4 = quest:GetThingWithScriptName("WhisperAlly")
                                                            quest:GiveThingBestEnemyTarget(scratchValue31, whisperAlly4)
                                                            goto LAB_00f1fa91
                                                        end
                                                    elseif not quest:IsActiveThreadTerminating() then
                                                        hero11 = quest:GetHero()
                                                        goto LAB_00f1fa88
                                                    end
                                                end
                                                goto FLOW_past_lab_00f1fa88
                                                ::LAB_00f1fa88::
                                                quest:GiveThingBestEnemyTarget(scratchValue31, hero11)
                                                goto LAB_00f1fa91
                                                ::FLOW_past_lab_00f1fa88::
                                                goto FLOW_past_lab_00f1fa91
                                                ::LAB_00f1fa91::
                                                quest:SetStateBool("ArenaSpawnNeeded_" .. scratchValue16, true)
                                                scratchValue10 = scratchValue20
                                                conversationId = scratchValue18
                                                goto LAB_00f1faba
                                                ::FLOW_past_lab_00f1fa91::
                                                ::FLOW_past_lab_00f1f934::
                                            end
                                            goto LAB_00f20cc5
                                        end
                                        ::LAB_00f1faba::
                                        scratchValue19 = scratchValue19 + 1
                                        scratchValue20 = scratchValue10 + 12
                                    until not (scratchValue19 < conversationId)
                                end
                                scratchValue16 = (scratchValue16 + 1) % conversationId
                                scratchValue4 = 0
                            end
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
                            createCreatureNearby = nil
                            local predicateResult = quest:IsActiveThreadTerminating()
                            if conversationId == 1 then
                                if predicateResult then
                                    goto LAB_00f20cc5
                                end
                                quest:SetCreatureCreationDelayFrames(1)
                                if not (bigCreatureSpawnPoint ~= nil and not bigCreatureSpawnPoint:IsNull()) then
                                    getPos = {x = 0, y = 0, z = 0}
                                else
                                    getPos = bigCreatureSpawnPoint:GetPos()
                                end
                                createCreatureNearby = quest:CreateCreatureNearby(quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_CreatureType"), getPos, 1.0, "ArenaEnemy")
                                quest:ResetCreatureCreationDelayFrames()
                                conversationId = scratchValue18
                            else
                                if predicateResult then
                                    goto LAB_00f20cc5
                                end
                                ctr_fc = 0
                                if 0 < count then
                                    scratchValue11 = 0
                                    repeat
                                        scratchValue29 = multipleBigCreatureSpawnPoint[scratchValue11 + 1]:GetDataString()
                                        if parseGameInteger(scratchValue29) ~= scratchValue17 then ctr_fc = ctr_fc + 1; scratchValue11 = scratchValue11 + 1; goto continue_1 end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
                                        quest:SetCreatureCreationDelayFrames(1)
                                        creatureType = quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_CreatureType")
                                        scratchValue30 = multipleBigCreatureSpawnPoint[scratchValue11 + 1]:GetPos()
                                        savedCreatureGroupIndex = creatureGroupIndex
                                        createCreatureNearby = quest:CreateCreatureNearby(creatureType, scratchValue30, 1.0, "ArenaEnemy")
                                        quest:ResetCreatureCreationDelayFrames()
                                        conversationId = scratchValue18
                                        ctr_fc = ctr_fc + 1
                                        scratchValue11 = scratchValue11 + 1
                                        ::continue_1::
                                    until not (ctr_fc < count)
                                end
                            end
                            quest:EntitySetCutsceneBehaviour(createCreatureNearby, CUTSCENE_BEHAVIOUR_PAUSED)
                            local hero12 = quest:GetHero()
                            quest:GiveThingBestEnemyTarget(createCreatureNearby, hero12)
                        end
                        quest:SetStateInt("TotalCreatures_" .. totalCreaturesIndex, quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex) + 1)
                        scratchValue17 = scratchValue17 + 1
                    until not (scratchValue17 < quest:GetStateInt("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_NumCreatures"))
                end
                if not quest:IsActiveThreadTerminating() then ctr_10c = ctr_10c + 1; totalCreaturesIndex = totalCreaturesIndex + 1; creatureGroupIndex = savedCreatureGroupIndex + 1; goto continue_2 end
                arenaSpawn = nil
                quest:DeregisterTimer(timerId)
                bigCreatureSpawnPoint = nil
                do return end
                ctr_10c = ctr_10c + 1
                totalCreaturesIndex = totalCreaturesIndex + 1
                creatureGroupIndex = savedCreatureGroupIndex + 1
                ::continue_2::
            until not (ctr_10c < quest:GetStateInt("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_NumWaveCreatures"))
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
    end
    creatureGroupIndex = 0
    initialCreatureCounts = {}
    creatureCounterIds = {}
    totalCreaturesIndex2 = 0
    scratchValue12 = 0
    repeat
        savedCreatureGroupIndex = creatureGroupIndex
        if quest:IsActiveThreadTerminating() then
            arenaSpawn = nil
            quest:DeregisterTimer(timerId)
            bigCreatureSpawnPoint = nil
            return
        end
        conversationId = quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex2)
        initialCreatureCounts[(scratchValue12 / 4) + 1] = conversationId
        creatureCounterIds[(scratchValue12 / 4) + 1] = -1
        if 0 < conversationId then
            savedCreatureGroupIndex = creatureGroupIndex
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            conversationId = quest:AddQuestInfoCounterList(quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. creatureGroupIndex .. "_HUDType"), quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex2), 1.0)
            creatureCounterIds[(scratchValue12 / 4) + 1] = conversationId
        end
        if 0 < initialCreatureCounts[(scratchValue12 / 4) + 1] then
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            quest:UpdateQuestInfoCounterList(creatureCounterIds[(scratchValue12 / 4) + 1], quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex2), -1)
        end
        creatureGroupIndex = savedCreatureGroupIndex + 1
        scratchValue12 = scratchValue12 + 4
        totalCreaturesIndex2 = totalCreaturesIndex2 + 1
    until not (creatureGroupIndex < 3)
    if quest:IsActiveThreadTerminating() then
        arenaSpawn = nil
        quest:DeregisterTimer(timerId)
        bigCreatureSpawnPoint = nil
        return
    end
    quest:DisplayQuestInfo(true)
    scratchValue13 = 0
    conversationId = 0
    totalCreaturesIndex2 = 0
    repeat
        if quest:IsActiveThreadTerminating() then goto LAB_00f2049f end
        scratchValue13 = scratchValue13 + quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex2)
        conversationId = conversationId + 1
        totalCreaturesIndex2 = totalCreaturesIndex2 + 1
    until not (conversationId < 3)
    if quest:IsActiveThreadTerminating() then
        arenaSpawn = nil
        quest:DeregisterTimer(timerId)
        bigCreatureSpawnPoint = nil
        return
    end
    while 0 < scratchValue13 do
        quest:NewScriptFrame()
        scratchValue13 = 0
        if quest:IsActiveThreadTerminating() then goto LAB_00f207f4 end
        totalCreaturesIndex2 = 0
        conversationId = 0
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00f208af end
            scratchValue13 = scratchValue13 + quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex2)
            if 0 < initialCreatureCounts[(conversationId / 4) + 1] then
                if quest:IsActiveThreadTerminating() then goto LAB_00f2096f end
                quest:UpdateQuestInfoCounterList(creatureCounterIds[(conversationId / 4) + 1], quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex2), -1)
            end
            conversationId = conversationId + 4
            totalCreaturesIndex2 = totalCreaturesIndex2 + 1
        until not (conversationId < 12)
        if quest:IsActiveThreadTerminating() then goto LAB_00f20a2f end
        if quest:GetStateInt("ExtraCreatures") ~= 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            quest:RemoveQuestInfoElement(creatureCounterIds[0 + 1])
            initialCreatureCounts[0 + 1] = initialCreatureCounts[0 + 1] + quest:GetStateInt("ExtraCreatures")
            local replacementCounterId = quest:AddQuestInfoCounterList(quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. 0 .. "_HUDType"), initialCreatureCounts[0 + 1], 1.0)
            creatureCounterIds[0 + 1] = replacementCounterId
            quest:SetStateInt("ExtraCreatures", 0)
        end
        if not quest:IsLevelLoaded("Arena") then
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            quest:SetStateBool("PlayerLeaving", true)
            conversationId = 0
            totalCreaturesIndex2 = 0
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
                quest:SetStateInt("TotalCreatures_" .. totalCreaturesIndex2, 0)
                conversationId = conversationId + 1
                totalCreaturesIndex2 = totalCreaturesIndex2 + 1
            until not (conversationId < 3)
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
        end
    end
    if not quest:IsActiveThreadTerminating() then scratchValue14 = 0; goto LAB_00f20700 end
    ::LAB_00f20ca1::
    ::LAB_00f20cc5::
    quest:DeregisterTimer(timerId)
    goto LAB_00f2119d
    ::LAB_00f2049f::
    arenaSpawn = nil
    quest:DeregisterTimer(timerId)
    bigCreatureSpawnPoint = nil
    do return end
    ::LAB_00f207f4::
    arenaSpawn = nil
    goto LAB_00f20ae2
    ::LAB_00f208af::
    arenaSpawn = nil
    goto LAB_00f20ae2
    ::LAB_00f2096f::
    arenaSpawn = nil
    goto LAB_00f20ae2
    ::LAB_00f20b8b::
    timerId2 = timerId
    goto LAB_00f21195
    ::LAB_00f20700::
    while true do
        if quest:IsActiveThreadTerminating() then goto LAB_00f20b8b end
        if 0 < initialCreatureCounts[(scratchValue14 / 4) + 1] then
            if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
            quest:RemoveQuestInfoElement(creatureCounterIds[(scratchValue14 / 4) + 1])
        end
        scratchValue14 = scratchValue14 + 4
        if 11 < scratchValue14 then break end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
    quest:DisplayQuestInfo(false)
    arenaRoundWave = quest:GetStateInt("ArenaRoundWave")
    quest:SetStateInt("ArenaRoundWave", arenaRoundWave + 1)
    quest:SetStateBool("PauseCrowdChecker", true)
    if arenaRoundWave + 1 == quest:GetStateInt("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_NumWaves") then
        if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
        quest:SetStateInt("ArenaRound", quest:GetStateInt("ArenaRound") + 1)
        quest:SetStateInt("ArenaRoundWave", 0)
        if quest:GetStateInt("NewCrowdPoints") < 600 then
            if 100 <= quest:GetStateInt("NewCrowdPoints") then goto LAB_00f20ce5 end
            goto LAB_00f20e68
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            goto LAB_00f20ce5
        end
        goto FLOW_past_lab_00f20e68
        ::LAB_00f20e68::
        if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
        quest:Pause(1.0)
        b2 = false
        b1 = false
        conversationId = quest:RetailRandModulo(32768)
        conversationId = GetFanfareMusic(quest, conversationId % 10)
        quest:OverrideMusic(conversationId, false, false)
        quest:Pause(5.0)
        quest:StopOverrideMusic(false)
        GivePrizeFund(quest)
        while not quest:IsHeroControlledByPlayer() do
            if not quest:NewScriptFrame() then goto LAB_00f212cc end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f212cc end
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        resource3 = resources:NewResource()
        resources:PrepareResource(resource3)
        conversationId = 4
        resource = resource3
        hero15 = quest:GetHero()
        while not resources:TryAcquire(resource3, hero15, 4) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource3)
                quest:PauseAllNonScriptedEntities(false)
                goto FLOW_after_lab_00f211ba
            end
            conversationId = 4
            resource2 = resource3
            hero15 = quest:GetHero()
        end
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource3)
            quest:PauseAllNonScriptedEntities(false)
            goto FLOW_after_lab_00f211ba
        end
        if 7 < quest:GetStateInt("ArenaRound") then
            quest:FadeScreenOut(0.5, 0.5)
            quest:Pause(0.5)
            resources:ReleaseResource(resource3)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            goto LAB_00f21166
        end
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource3)
            quest:PauseAllNonScriptedEntities(false)
        else
            local pQuestionText = GetEndRoundQuestion(quest)
            quest:GiveHeroYesNoQuestion(pQuestionText, "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_YES", "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_NO", "", true)
            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer < 0 do
                if not quest:NewScriptFrame() then goto LAB_00f212a6 end
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource3)
                quest:PauseAllNonScriptedEntities(false)
                goto FLOW_after_lab_00f211ba
            end
            predicateResult38 = quest:IsActiveThreadTerminating()
            if questionAnswer == 1 then
                if not predicateResult38 then
                    quest:SetStateInt("GoldMultiplier", (quest:GetStateInt("GoldMultiplier") + 1 + 2147483648) % 4294967296 - 2147483648)
                    quest:FadeScreenOut(0.5, 0.5)
                    quest:Pause(0.5)
                    resources:ReleaseResource(resource3)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00f21166
                end
            else
                if predicateResult38 then
                    resources:ReleaseResource(resource3)
                    quest:PauseAllNonScriptedEntities(false)
                    goto FLOW_after_lab_00f211ba
                end
                quest:SetStateBool("PlayerLeaving", true)
                quest:GiveHeroGold(quest:GetStateInt("GoldTotal"))
                quest:SetStateInt("GoldTotal", 0)
                local hero13 = quest:GetHero()
                conversationId = quest:AddNewConversation(hero13, false, false)
                local hero14 = quest:GetHero()
                hero15 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_V2_ARENA_KEEPER_RETURN", hero15, hero14, false)
                while quest:IsConversationActive(conversationId) do
                    quest:NewScriptFrame()
                    if quest:IsActiveThreadTerminating() then
                        resources:ReleaseResource(resource3)
                        quest:PauseAllNonScriptedEntities(false)
                        goto FLOW_after_lab_00f211ba
                    end
                end
                if not quest:IsActiveThreadTerminating() then
                    quest:FadeScreenOut(0.5, 0.5)
                    quest:Pause(0.5)
                    resources:ReleaseResource(resource3)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00f21166
                end
            end
            ::LAB_00f212a6::
            resources:ReleaseResource(resource3)
            quest:PauseAllNonScriptedEntities(false)
        end
        ::FLOW_after_lab_00f211ba::
        resources:DestroyMovie(movie)
        ::FLOW_past_lab_00f20e68::
        goto FLOW_past_lab_00f20ce5
        ::LAB_00f20ce5::
        if not quest:IsActiveThreadTerminating() then
            local scratchValue15 = quest:ReadGlobalGameData(2672)
            conversationId = quest:ReadGlobalGameData(SCRIPT_DEF.RewardObjects)
            local rewardIndex = quest:RetailRandModulo(32768) % ((scratchValue15 - conversationId) >> 2)
            local rewardTurns = (quest:RetailRandModulo(32768) % 360) * 0.0027777778450399637
            local rewardOffset = quest:RotateVectorHorizontal({x = 14.0, y = 0.0, z = 5.0}, rewardTurns)
            getPos = quest:GetThingWithScriptName("ARENA_CentrePoint"):GetPos()
            local rewardPosition = {x = rewardOffset.x + getPos.x, y = rewardOffset.y + getPos.y, z = rewardOffset.z + getPos.z}
            local rewardDefinition = quest:ReadGlobalGameDataDefStringAt(2668, rewardIndex)
            quest:CreateObject(rewardDefinition, rewardPosition, "")
            goto LAB_00f20e68
        end
        ::FLOW_past_lab_00f20ce5::
        ::LAB_00f212cc::
        goto LAB_00f20cc5
    end
    ::LAB_00f21166::
    ::LAB_00f2116f::
    timerId2 = timerId
    ::LAB_00f21195::
    quest:DeregisterTimer(timerId2)
    ::LAB_00f2119d::
    do return end
    ::LAB_00f20a2f::
    arenaSpawn = nil
    ::LAB_00f20ae2::
    quest:DeregisterTimer(timerId)
    bigCreatureSpawnPoint = nil
end

-- Q_Arena.GivePrizeFund (retail 0x00f21300)
function GivePrizeFund(quest)
    local pOther, scratchValue, scratchValue2
    local hero = quest:GetHero()
    local conversationID = quest:AddNewConversation(hero, true, false)
    local hero2 = quest:GetHero()
    quest:AddPersonToConversation(conversationID, hero2)
    scratchValue = ""
    scratchValue2 = ""
    local switch = quest:GetStateInt("GoldMultiplier")
    repeat
        if switch == 0 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 100)
            scratchValue = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND1_PRIZE"
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND1_PRIZE_LATER"
            break
        elseif switch == 1 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 500)
            scratchValue = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND2_PRIZE"
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND2_PRIZE_LATER"
            break
        elseif switch == 2 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 1000)
            scratchValue = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND3_PRIZE"
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND3_PRIZE_LATER"
            break
        elseif switch == 3 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 2000)
            scratchValue = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND4_PRIZE"
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND4_PRIZE_LATER"
            break
        elseif switch == 4 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 3000)
            scratchValue = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND5_PRIZE"
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND5_PRIZE_LATER"
            break
        elseif switch == 5 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 4000)
            scratchValue = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND6_PRIZE"
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND6_PRIZE_LATER"
            break
        elseif switch == 6 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 8000)
            scratchValue = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND7_PRIZE"
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND7_PRIZE_LATER"
            break
        elseif switch == 7 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 10000)
            scratchValue = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND8_PRIZE"
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND8_PRIZE_LATER"
            break
        else
            goto FLOW_native_label_1
        end
    until true
    scratchValue2 = pOther
    ::FLOW_native_label_1::
    local hero3 = quest:GetHero()
    local hero5 = quest:GetHero()
    quest:AddLineToConversation(conversationID, scratchValue, hero5, hero3, false)
    local hero4 = quest:GetHero()
    local hero6 = quest:GetHero()
    quest:AddLineToConversation(conversationID, scratchValue2, hero6, hero4, false)
    if not quest:IsConversationActive(conversationID) then
        quest:IsActiveThreadTerminating()
    else
        repeat
            if not quest:NewScriptFrame() then return end
        until not quest:IsConversationActive(conversationID)
        quest:IsActiveThreadTerminating()
    end
end

-- Q_Arena.GetEndRoundQuestion (retail 0x00f21590)
function GetEndRoundQuestion(quest)
    local string
    local switch1 = quest:GetStateInt("GoldMultiplier")
    repeat
        if switch1 == 0 then
            string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_1"
            break
        elseif switch1 == 1 then
            string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_2"
            break
        elseif switch1 == 2 then
            string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_3"
            break
        elseif switch1 == 3 then
            string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_4"
            break
        elseif switch1 == 4 then
            string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_5"
            break
        elseif switch1 == 5 then
            string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_6"
            break
        elseif switch1 == 6 then
            string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_7"
            break
        elseif switch1 == 7 then
            string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_8"
            break
        else
            string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION"
        end
    until true
    return string
end

-- Q_Arena.DrawGetEnvironment (retail 0x00f14250)
-- F14250: bsim names this body CWorld::DrawGetEnvironment (a homologous script member); no PDB name
function DrawGetEnvironment(quest)
    -- TODO(native): return *(this + 0x44)
end

