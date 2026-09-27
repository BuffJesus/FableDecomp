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
}

-- Q_Arena.Main (retail 0x00f0fb70)
function Main(quest)
    local whisper = quest:GetStateThing("Whisper")
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local getHeroTitle, switch, scratchValue, actorMap, shadow, arenaCellDoorGuard
    local cellsToHallOfHeroesEntrance, flick, needle, cham, shadow2, arenaCellDoorGuard22, flick2
    local needle2, this_00, scratchValue11, resource, resource4, resource5, resource6, actorMap2
    local resource7, actorMap3, scratchValue12, resource8, resource9, actorMap5, actorMap6, movie
    local movie2
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_01", "Arena", "KnotholeGlade")
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
    if (1 - (quest:IsQuestActive("Q_ArenaHoldingScript") and 1 or 0)) ~= 0 then
        if quest:IsActiveThreadTerminating() then return end
        quest:ActivateQuest("Q_ArenaHoldingScript")
    end
    quest:CreateThread("WatchForTermination")  -- native thread body Script_Arena_Teleport_Thread: lift it as function WatchForTermination(quest)
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
        quest:SetStateInt("ArenaState", 1)
    end
    quest:SetTimeOfDay(12.0)
    quest:SetTimeAsStopped(true)
    quest:SetTeleportingAsActive(false)
    if quest:GetStateInt("ArenaState") == 1 then
        if quest:IsActiveThreadTerminating() then return end
        resource8 = resources:NewResource()
        resources:TryAcquire(resource8, hero, 4)
        quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("Q_Nav1"), false)
        actorMap5 = resources:NewActorMap()
        resources:SetActor(actorMap5, "Hero", resource8)
        resource9 = resources:StartMovie("")
        resources:RunMacro("CS_ARENA_HOH_INTRO", actorMap5, false, true)
        resources:DestroyMovie(resource9)
        resources:DestroyActorMap(actorMap5)
        resources:ReleaseResource(resource8)
        quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("ArenaCellEntrance"), false)
        while not quest:IsLevelLoaded("ArenaCells") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetThingAsUsable(quest:GetThingWithScriptName("CellsExitToArena"), false)
        quest:SetThingAsUsable(quest:GetThingWithScriptName("CellsExitToHOH"), false)
        quest:SetThingPersistent(quest:GetThingWithScriptName("CellsExitToArena"), true)
        quest:SetThingPersistent(quest:GetThingWithScriptName("CellsExitToHOH"), true)
        quest:FadeScreenIn()
        quest:SetStateInt("ArenaState", 2)
    end
    if quest:GetStateInt("ArenaState") == 2 then
        if quest:IsActiveThreadTerminating() then return end
        local roth = quest:GetThingWithScriptName("Roth")
        resource6 = resources:NewResource()
        resources:PrepareResource(resource6)
        while not resources:TryAcquire(resource6, roth, 4) do
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
        resource = resources:NewResource()
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, hero, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f10cc2 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f10cc2 end
        actorMap3 = resources:NewActorMap()
        resources:SetActor(actorMap3, "Hero", resource)
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
        resources:ReleaseResource(resource)
        resources:ReleaseResource(resource4)
        resources:ReleaseResource(resource7)
        resources:ReleaseResource(resource8)
        resources:ReleaseResource(resource9)
        resources:ReleaseResource(resource5)
        resources:ReleaseResource(resource6)
        quest:SetStateBool("ChamLeaving", true)
        quest:SetStateInt("ArenaState", 3)
        goto LAB_00f10fa6
        ::LAB_00f10cc2::
        resources:ReleaseResource(resource)
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
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_02", "ArenaCells", "KnotholeGlade")
        quest:SetTimer(timerId, 60)
        while 0 < quest:GetTimer(timerId) do
            if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        while not quest:IsHeroControlledByPlayer() do
            if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
        end
        quest:SetStateInt("ArenaState", 4)
    end
    if quest:GetStateInt("ArenaState") == 4 then
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_03", "Arena", "KnotholeGlade")
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
        while not resources:TryAcquire(resource4, hero, 4) do
            if not quest:NewScriptFrame() then goto LAB_00f115a2 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f115a2 end
        actorMap3 = resources:NewActorMap()
        resources:SetActor(actorMap3, "Hero", resource4)
        resources:SetActor(actorMap3, "Roth", resource6)
        resources:SetActor(actorMap3, "Flick", resource8)
        resources:SetActor(actorMap3, "Guard", resource5)
        resource = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        resources:RunMacro("CS_ARENA_CELLS_INTRO_CHAMDEAD", actorMap3, false, true)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(resource)
        resources:DestroyActorMap(actorMap3)
        resources:ReleaseResource(resource4)
        resources:ReleaseResource(resource5)
        resources:ReleaseResource(resource7)
        resources:ReleaseResource(resource9)
        resources:ReleaseResource(resource8)
        resources:ReleaseResource(resource6)
        quest:SetStateInt("ArenaState", 5)
        goto LAB_00f11828
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
                quest:RemoveThing(quest:GetThingWithScriptName("CellWhisper"), false, true)
            end
        else
            local cellWhisper3 = quest:GetThingWithScriptName("CellWhisper")
            if not (cellWhisper3 ~= nil and cellWhisper3:IsAlive()) then
                quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", quest:GetThingWithScriptName("FlickPoint"):GetPos(), "CellWhisper")
            end
        end
        while quest:GetStateInt("ArenaState") ~= 6 do
            if not quest:NewScriptFrame() then quest:DeregisterTimer(timerId); return end
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); return end
        quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("ArenaInsideHSP"), false)
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
                    quest:RemoveThing(quest:GetThingWithScriptName("CellWhisper"), false, true)
                end
                goto FLOW_hoist_lab_00f11d57_1
            elseif not quest:IsActiveThreadTerminating() then
                local cellWhisper6 = quest:GetThingWithScriptName("CellWhisper")
                if cellWhisper6 ~= nil and cellWhisper6:IsAlive() then goto LAB_00f11d5c end
                if not quest:IsActiveThreadTerminating() then quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", quest:GetThingWithScriptName("FlickPoint"):GetPos(), "CellWhisper"); goto LAB_00f11d57 end
            end
            goto FLOW_past_lab_00f11d57
            ::LAB_00f11d57::
            ::FLOW_hoist_lab_00f11d57_1::
            goto LAB_00f11d5c
            ::FLOW_past_lab_00f11d57::
            goto FLOW_past_lab_00f11d5c
            ::LAB_00f11d5c::
            while quest:GetStateInt("ArenaState") ~= 6 do
                if not quest:NewScriptFrame() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
            end
            if not quest:IsActiveThreadTerminating() then
                quest:FadeScreenOut(0.5, 0.5)
                quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("ArenaInsideHSP"), false)
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
            quest:SetThingAsUsable(quest:GetThingWithScriptName("ArenaMainExit"), false)
            quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("ARENA_CentrePoint"), false)
            resource9 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resource8 = resources:NewResource()
            resource7 = resources:NewResource()
            resources:PrepareResource(resource8)
            while not resources:TryAcquire(resource8, hero, 4) do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource7)
                    resources:ReleaseResource(resource8)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(resource9)
                    resources:DestroyActorMap(actorMap4)
                    quest:DeregisterTimer(timerId)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00f13e6c end
            scratchValue11 = math.random(0, 32767) & 0x80000003
            if scratchValue11 < 0 then
                scratchValue11 = (scratchValue11 - 1 | 0xfffffffc) + 1
            end
            if scratchValue11 ~= 1 then
                if scratchValue11 ~= 2 then goto LAB_00f121a5 end
                if quest:IsActiveThreadTerminating() then goto LAB_00f13e5e end
                scratchValue = "CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC3"
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00f13e6c end
                scratchValue = "CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC2"
            end
            scratchValue12 = scratchValue
            ::LAB_00f121a5::
            actorMap2 = resources:NewActorMap()
            resources:SetActor(actorMap2, "Hero", resource8)
            actorMap6 = resources:NewStringMap()
            resources:SetString(actorMap6, "$THEME", "NULL")
            getHeroTitle = quest:GetHeroTitle()
            if quest:GetStateInt("ArenaRound") < 3 then goto LAB_00f123aa end
            if quest:IsActiveThreadTerminating() then goto LAB_00f13e9c end
            -- TODO(native): CScriptThing::operator=(this_00,(int)pCVar7);
            if not (whisper ~= nil and whisper:IsAlive()) then
                if not quest:IsActiveThreadTerminating() then
                    quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", quest:GetThingWithScriptName("ARENA_WhisperStartPoint"):GetPos(), "WhisperAlly")
                    -- TODO(native): CScriptThing::operator=(this_00,(int)pCVar7);
                    goto LAB_00f123aa
                end
                goto LAB_00f13e45
            end
            ::LAB_00f123aa::
            switch = quest:GetStateInt("ArenaRound")
            repeat
                if switch == 0 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_01", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 0)
                    repeat
                        if getHeroTitle == 1 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_17")
                            break
                        elseif getHeroTitle == 2 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_20")
                            break
                        elseif getHeroTitle == 3 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_11")
                            break
                        elseif getHeroTitle == 4 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_06")
                            break
                        elseif getHeroTitle == 5 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_03")
                            break
                        elseif getHeroTitle == 6 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_12")
                            break
                        elseif getHeroTitle == 7 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_04")
                            break
                        elseif getHeroTitle == 8 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_15")
                            break
                        elseif getHeroTitle == 9 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_10")
                            break
                        elseif getHeroTitle == 10 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_13")
                            break
                        elseif getHeroTitle == 11 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_07")
                            break
                        elseif getHeroTitle == 12 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_16")
                            break
                        elseif getHeroTitle == 13 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_18")
                            break
                        elseif getHeroTitle == 14 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_09")
                            break
                        elseif getHeroTitle == 15 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_08")
                            break
                        elseif getHeroTitle == 16 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_19")
                            break
                        elseif getHeroTitle == 17 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_01")
                            break
                        elseif getHeroTitle == 18 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_14")
                            break
                        elseif getHeroTitle == 19 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_05")
                            break
                        elseif getHeroTitle == 20 then
                            resources:SetString(actorMap6, "$HEROTITLE", "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_02")
                            break
                        else
                            break
                        end
                    until true
                    scratchValue12 = "CS_ARENA_ROUND_1"
                    quest:OverrideMusic(GetFanfareMusic(quest, math.random(0, 32767) % 10), false, false)
                    break
                elseif switch == 1 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_02", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 0)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND2_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND2_20")
                    scratchValue12 = "CS_ARENA_ROUND_BEGIN_GENERIC"
                    break
                elseif switch == 2 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_03", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 1)
                    scratchValue12 = "CS_ARENA_ROUND_WHISPER_INTRO"
                    resources:SetString(actorMap6, "$THEME", "ENVIRONMENT_WITCHWOOD")
                    break
                elseif switch == 3 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_04", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 1)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND3_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND3_20")
                    resources:SetString(actorMap6, "$THEME", "ENVIRONMENT_HAUNTED")
                    break
                elseif switch == 4 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_05", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 2)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND4_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND4_20")
                    resources:SetString(actorMap6, "$THEME", "RESET")
                    break
                elseif switch == 5 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_06", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 2)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND5_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND5_20")
                    break
                elseif switch == 6 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_07", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 3)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND6_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND6_20")
                    break
                elseif switch == 7 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_08", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 3)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND7_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND7_20")
                    resources:SetString(actorMap6, "$THEME", "ENVIRONMENT_EOW_04")
                    quest:OverrideMusic(23, false, false)
                    break
                elseif switch == 8 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_09", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 3)
                    quest:SetStateInt("ArenaState", 8)
                    scratchValue12 = "CS_ARENA_ROUND_BOTH_WINNERS"
                    resources:SetString(actorMap6, "$THEME", "RESET")
                    break
                else
                    break
                end
            until true
            if whisper:IsAlive() then
                resources:PrepareResource(resource7)
                while not resources:TryAcquire(resource7, whisper, 4) do
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
            resources:RunMacroWithStrings(scratchValue12, actorMap2, actorMap6, false, true)
            quest:StopOverrideMusic(false)
            if scratchValue12 ~= nil and scratchValue12 == "CS_ARENA_ROUND_BOTH_WINNERS" then
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
                        quest:RemoveThing(quest:GetThingWithScriptName("CellWhisper"), false, true)
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
        quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("ArenaCellEntrance2"), false)
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
                if not quest:IsActiveThreadTerminating() then quest:RemoveThing(quest:GetThingWithScriptName("CellWhisper"), false, true); goto LAB_00f13a09 end
                resources:DestroyActorMap(actorMap4)
                quest:DeregisterTimer(timerId)
                return
            end
        else
            if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
            local cellWhisper11 = quest:GetThingWithScriptName("CellWhisper")
            if not (cellWhisper11 ~= nil and cellWhisper11:IsAlive()) then
                if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
                quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", quest:GetThingWithScriptName("FlickPoint"):GetPos(), "CellWhisper")
            end
        end
        ::LAB_00f13a09::
        quest:AutoSaveCheckPoint()
        while quest:GetStateInt("ArenaState") ~= 6 do
            if not quest:NewScriptFrame() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
        end
        if quest:IsActiveThreadTerminating() then resources:DestroyActorMap(actorMap4); quest:DeregisterTimer(timerId); return end
        quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("ArenaInsideHSP"), false)
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
    while not resources:TryAcquire(resource8, hero, 4) do
        if not quest:NewScriptFrame() then goto LAB_00f13f09 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f13f09 end
    resources:PrepareResource(resource9)
    while not resources:TryAcquire(resource9, quest:GetThingWithScriptName("Roth"), 4) do
        if not quest:NewScriptFrame() then goto LAB_00f13f09 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f13f09 end
    actorMap5 = resources:NewActorMap()
    resources:SetActor(actorMap5, "Hero", resource8)
    resources:SetActor(actorMap5, "Roth", resource9)
    if DrawGetEnvironment(quest)[116] == 1 then
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
    quest:EntityTeleportToThing(hero, cellsToHallOfHeroesEntrance, false)
    quest:EntitySetFacingAngle(hero, cellsToHallOfHeroesEntrance:GetAngleXY(), true)
    while not quest:IsLevelLoaded("ArenaHallOfHeroes") do
        if not quest:NewScriptFrame() then goto LAB_00f141b5 end
    end
    if not quest:IsActiveThreadTerminating() then
        resource9 = resources:NewResource()
        resources:TryAcquire(resource9, hero, 4)
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
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLARENAOPPONENTS", 30, quest:ReadGlobalGameData(2780), quest:ReadGlobalGameData(2788), true, "", 1)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_SPAREARENAOPPONENTS", 31, quest:ReadGlobalGameData(2784), quest:ReadGlobalGameData(2792), false, "", 1)
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
        -- TODO(native): this[iVar2 + 0xe2] = (CQ_ArenaScript)0x0;
        scratchValue = scratchValue + 1
    until scratchValue >= 16
    if quest:IsActiveThreadTerminating() then return end
    quest:SetStateBool("FinalBattleCS", false)
    quest:SetStateInt("ArenaState", 0)
    quest:SetStateBool("WhisperNeededForCutscene", true)
end

-- Q_Arena.OnPersist (retail 0x00cfab10)
function OnPersist(quest, context)
    quest:SetStateInt("ArenaState", quest:PersistTransferInt(context, "ArenaState", quest:GetStateInt("ArenaState") or 0))
    quest:SetStateInt("ArenaRound", quest:PersistTransferInt(context, "ArenaRound", quest:GetStateInt("ArenaRound") or 0))
    quest:SetStateInt("GoldMultiplier", quest:PersistTransferInt(context, "GoldMultiplier", quest:GetStateInt("GoldMultiplier") or 0))
    quest:SetStateInt("GoldTotal", quest:PersistTransferInt(context, "GoldTotal", quest:GetStateInt("GoldTotal") or 0))
    quest:SetStateInt("NewCrowdPoints", quest:PersistTransferInt(context, "NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") or 0))
    quest:SetStateBool("ChamLeaving", quest:PersistTransferBool(context, "ChamLeaving", quest:GetStateBool("ChamLeaving")))
    quest:SetStateBool("PlayerLeaving", quest:PersistTransferBool(context, "PlayerLeaving", quest:GetStateBool("PlayerLeaving")))
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
            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(2796))
        end
        local arenaExtEntrance = quest:GetThingWithScriptName("ArenaExtEntrance")
        quest:SetAllowScreenFadingOnNextRegionChange(false)
        quest:EntityTeleportToThing(quest:GetHero(), arenaExtEntrance, false)
        while not quest:IsRegionLoaded("ArenaExterior") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        quest:SetQuestAsCompleted(quest:GetActiveQuestName(), true, false, false)
        quest:FadeScreenIn()
        quest:SetAllowScreenFadingOnNextRegionChange(true)
        quest:SetMasterGameState("ArenaFinished", true)
    else
        if quest:IsActiveThreadTerminating() then return end
    end
    quest:SetTeleportingAsActive(true)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Q_Arena.CrowdChecker (retail 0x00f1cce0)
function CrowdChecker(quest)
    local scratchValue, predicateResult, predicateResult9, scratchValue2, scratchValue3
    local scratchValue4, c_stk_151_1, arenaAudience01, newCrowdBaseLevel, scratchValue6
    local scratchValue7, scratchValue10, scratchValue11, scratchValue13, timerId, timerId3
    local i_stk_fc_3, switch4, hero39, playCriteriaSoundOnThing, scratchValue22, scratchValue23
    local scratchValue25, crowdSoundSource
    local hero = quest:GetHero()
    -- TODO(native): aiStack_c[0] = 0;
    -- TODO(native): aiStack_c[1] = 200;
    -- TODO(native): aiStack_c[2] = 400;
    local function DeregisterTimers()
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId)
    end
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    timerId3 = quest:RegisterTimer()
    quest:SetTimer(timerId3, 15)
    scratchValue13 = -1
    predicateResult = quest:IsActiveThreadTerminating()
    while true do
        if predicateResult then break end
        while quest:IsLevelLoaded("ArenaCells") do
            if not quest:NewScriptFrame() then goto LAB_00f1e423 end
            playCriteriaSoundOnThing = 0
            local cellsExitToArena = quest:GetThingWithScriptName("CellsExitToArena")
            while quest:IsLevelLoaded("ArenaCells") do
                if not quest:NewScriptFrame() then goto LAB_00f1eba3 end
                local scratchValue9 = math.random(0, 32767) % 3
                if scratchValue9 == 0 then
                    playCriteriaSoundOnThing = quest:PlayCriteriaSoundOnThing(cellsExitToArena, "ARENA_HECKLE_BIG_MUFFLED_LOOP")
                else
                    if scratchValue9 == 1 then
                        playCriteriaSoundOnThing = quest:PlayCriteriaSoundOnThing(cellsExitToArena, "ARENA_APPLAUSE_MEDIUM_MUFFLED_LOOP")
                        goto LAB_00f1cf00
                    end
                    if scratchValue9 == 2 then
                        playCriteriaSoundOnThing = quest:PlayCriteriaSoundOnThing(cellsExitToArena, "ARENA_CHANT_MUFFLED_LOOP")
                        goto LAB_00f1cf00
                    end
                end
                ::LAB_00f1cf00::
                quest:Pause((math.random(0, 32767) % 10) + 2.0)
                quest:StopSound(playCriteriaSoundOnThing)
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00f1eba3 end
        end
        if quest:IsActiveThreadTerminating() then DeregisterTimers(); return end
        if quest:IsLevelLoaded("Arena") then
            if not quest:IsActiveThreadTerminating() then
                i_stk_fc_3 = quest:AddQuestInfoBar(400.0, 0.0, {R = 255, G = 255, B = 0, A = 255}, {R = 255, G = 255, B = 0, A = 255}, "HUD_EXPR_CLAP", "", 1.0)
                quest:DisplayQuestInfo(true)
                crowdSoundSource = quest:GetAllThingsWithScriptName("CrowdSoundSource")
                -- TODO(native): Vector_ZeroInit(#xStack_150);
                arenaAudience01 = quest:GetAllThingsWithDefName("OBJECT_ARENA_AUDIENCE_01")
                c_stk_151_1 = 0
                if quest:IsLevelLoaded("Arena") then
                    -- LAB_00f1d1e0: (native jump target)
                    quest:NewScriptFrame()
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId3)
                        quest:DeregisterTimer(timerId)
                        return
                    end
                    if not quest:GetStateBool("PauseCrowdChecker") or true then goto LAB_00f1d2e6 end
                    if not quest:IsActiveThreadTerminating() then
                        repeat
                            if not quest:GetStateBool("PauseCrowdChecker") then
                                goto LAB_00f1d260
                            else
                                if not quest:IsLevelLoaded("Arena") then goto LAB_00f1d260 end
                                predicateResult9 = true
                            end
                            goto FLOW_past_lab_00f1d260
                            ::LAB_00f1d260::
                            predicateResult9 = false
                            ::FLOW_past_lab_00f1d260::
                            if not predicateResult9 then goto LAB_00f1d2d7 end
                            quest:NewScriptFrame()
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId3)
                                quest:DeregisterTimer(timerId)
                                do return end
                            end
                        until false
                    end
                elseif not quest:IsActiveThreadTerminating() then
                    quest:RemoveQuestInfoElement(i_stk_fc_3)
                    quest:DisplayQuestInfo(false)
                    goto LAB_00f1e3c7
                end
            end
            goto LAB_00f1eba3
        end
        ::LAB_00f1e3c7::
        quest:NewScriptFrame()
        predicateResult = quest:IsActiveThreadTerminating()
    end
    ::FLOW_after_lab_00f1e382::
    ::LAB_00f1eba3::
    quest:DeregisterTimer(timerId3)
    quest:DeregisterTimer(timerId)
    do return end
    ::LAB_00f1e423::
    goto LAB_00f1eba3
    ::LAB_00f1d2d7::
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId)
        return
    end
    ::LAB_00f1d2e6::
    if not quest:IsLevelLoaded("Arena") then goto LAB_00f1e350 end
    if quest:IsActiveThreadTerminating() then
        DeregisterTimers(); return
    end
    quest:UpdateQuestInfoBar(i_stk_fc_3, quest:GetStateInt("NewCrowdPoints"), -1.0, -1.0)
    if quest:GetTimer(timerId) < 1 then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId3)
            quest:DeregisterTimer(timerId)
            return
        end
        quest:SetTimer(timerId, 5)
        scratchValue6 = 2
        repeat
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId)
                return
            end
            if not ((scratchValue[scratchValue6] <= quest:GetStateInt("NewCrowdPoints")) or (scratchValue6 == 0)) then scratchValue6 = scratchValue6 - 1; scratchValue10 = -1; goto continue_2 end
            scratchValue10 = scratchValue6
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId)
                return
            end
            break
            scratchValue6 = scratchValue6 - 1
            scratchValue10 = -1
            ::continue_2::
        until not (-1 < scratchValue6)
        scratchValue13 = scratchValue10
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId3)
            quest:DeregisterTimer(timerId)
            return
        end
        local getStateInt = quest:GetStateInt("NewCrowdPoints") - 4
        quest:SetStateInt("NewCrowdPoints", getStateInt)
        if 400 < getStateInt then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId)
                return
            end
            quest:SetStateInt("NewCrowdPoints", 400)
        end
        if scratchValue13 ~= quest:GetStateInt("NewCrowdHappiness") or (-1 ~= quest:GetStateInt("NewCrowdBaseLevel")) then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId)
                return
            end
            local newCrowdBaseLevel2 = quest:GetStateInt("NewCrowdBaseLevel")
            quest:SetStateInt("NewCrowdHappiness", scratchValue13)
            scratchValue22 = 0
            newCrowdBaseLevel = newCrowdBaseLevel2
            if #crowdSoundSource ~= 0 then
                scratchValue11 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId3)
                        quest:DeregisterTimer(timerId)
                        return
                    end
                    -- TODO(native): quest:StopSound(*(xStack_12c + uVar9 * 4))
                    quest:Pause(0.2)
                    -- TODO(native): puStack_10 = (uint *)((int)xStack_12c + uVar9 * 4);
                    quest:PlayCriteriaSoundOnThing(crowdSoundSource[scratchValue11 + 1], quest:GetStateString("CrowdLoopTags_" .. newCrowdBaseLevel2 .. "_" .. scratchValue13))
                    -- TODO(native): *puStack_10 = uVar5;
                    c_stk_151_1 = 0
                    quest:Pause(0.3)
                    scratchValue22 = scratchValue22 + 1
                    scratchValue11 = scratchValue11 + 1
                until scratchValue22 >= #crowdSoundSource
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId)
                return
            end
        end
        scratchValue23 = 0
        if #arenaAudience01 ~= 0 then
            scratchValue7 = 0
            repeat
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId3)
                    quest:DeregisterTimer(timerId)
                    return
                end
                if not quest:IsCameraPosOnScreen(arenaAudience01[scratchValue7 + 1]:GetPos()) then scratchValue23 = scratchValue23 + 1; scratchValue7 = scratchValue7 + 1; goto continue_4 end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId3)
                    quest:DeregisterTimer(timerId)
                    return
                end
                if scratchValue13 == 0 then
                    local scratchValue19 = math.random(0, 32767) & 0x80000001
                    scratchValue2 = scratchValue19 == 0
                    if scratchValue19 < 0 then
                        scratchValue2 = (scratchValue19 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if scratchValue2 then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId3)
                            quest:DeregisterTimer(timerId)
                            return
                        end
                        quest:EntityPlayObjectAnimation(arenaAudience01[scratchValue7 + 1], "CLAP_LOOP_02", true)
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId3)
                            quest:DeregisterTimer(timerId)
                            return
                        end
                        quest:EntityPlayObjectAnimation(arenaAudience01[scratchValue7 + 1], "IDLE_BREATHE_01", true)
                    end
                elseif scratchValue13 == 1 then
                    local scratchValue21 = math.random(0, 32767) & 0x80000001
                    scratchValue3 = scratchValue21 == 0
                    if scratchValue21 < 0 then
                        scratchValue3 = (scratchValue21 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if scratchValue3 then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId3)
                            quest:DeregisterTimer(timerId)
                            return
                        end
                        quest:EntityPlayObjectAnimation(arenaAudience01[scratchValue7 + 1], "CLAP_LOOP_02", true)
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId3)
                            quest:DeregisterTimer(timerId)
                            return
                        end
                        quest:EntityPlayObjectAnimation(arenaAudience01[scratchValue7 + 1], "CHEER_LOOP_01", true)
                    end
                elseif scratchValue13 == 2 then
                    quest:EntityPlayObjectAnimation(arenaAudience01[scratchValue7 + 1], "CHEER_LOOP_01", true)
                end
                scratchValue23 = scratchValue23 + 1
                scratchValue7 = scratchValue7 + 1
                ::continue_4::
            until scratchValue23 >= #arenaAudience01
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId3)
            quest:DeregisterTimer(timerId)
            return
        end
    end
    if quest:GetTimer(timerId3) < 1 then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId3)
            quest:DeregisterTimer(timerId)
            return
        end
        local sequence = c_stk_151_1 ~= 0 and quest:IsSoundPlaying(0)
        if sequence then goto LAB_00f1e2fb end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId3)
            quest:DeregisterTimer(timerId)
            return
        end
        local singleShouter = quest:GetThingWithScriptName("SingleShouter")
        quest:PlayCriteriaSoundOnThing(singleShouter, quest:GetStateString("CrowdLoopTags_" .. newCrowdBaseLevel .. "_" .. scratchValue13))
        quest:SetTimer(timerId3, math.random(0, 32767) % quest:ReadGlobalGameData(2664) + quest:ReadGlobalGameData(2660))
        local conversationId = quest:AddNewConversation(singleShouter, false, false)
        quest:AddPersonToConversation(conversationId, hero)
        if quest:GetStateInt("NewCrowdHappiness") < 1 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId)
                return
            end
            local switch = quest:GetStateInt("ArenaRound")
            repeat
                if switch == 0 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_WASPS", singleShouter, hero, false)
                    break
                elseif switch == 1 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_HOBBES", singleShouter, hero, false)
                    break
                elseif switch == 2 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_BALVERINES", singleShouter, hero, false)
                    break
                elseif switch == 3 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_UNDEAD", singleShouter, hero, false)
                    break
                elseif switch == 4 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_BANDITS", singleShouter, hero, false)
                    break
                elseif switch == 5 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_EARTH_TROLL", singleShouter, hero, false)
                    break
                elseif switch == 6 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_ROCK_TROLL", singleShouter, hero, false)
                    break
                elseif switch == 7 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_SCORPION", singleShouter, hero, false)
                    break
                elseif switch == 8 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHEER_WASPS", singleShouter, hero, false)
                    break
                else
                    goto FLOW_native_label_1
                end
            until true
            goto LAB_00f1e2e9
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId3)
            quest:DeregisterTimer(timerId)
            return
        end
        scratchValue25 = math.random(0, 32767) & 0x80000001
        scratchValue4 = scratchValue25 == 0
        if scratchValue25 < 0 then
            scratchValue4 = (scratchValue25 - 1 | 0xfffffffe) == 0xffffffff
        end
        if scratchValue4 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId)
                return
            end
            local switch3 = quest:GetStateInt("ArenaRound")
            repeat
                if switch3 == 0 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_WASPS", singleShouter, hero, false)
                    break
                elseif switch3 == 1 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_HOBBES", singleShouter, hero, false)
                    break
                elseif switch3 == 2 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_BALVERINES", singleShouter, hero, false)
                    break
                elseif switch3 == 3 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_UNDEAD", singleShouter, hero, false)
                    break
                elseif switch3 == 4 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_BANDITS", singleShouter, hero, false)
                    break
                elseif switch3 == 5 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_EARTH_TROLL", singleShouter, hero, false)
                    break
                elseif switch3 == 6 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_ROCK_TROLL", singleShouter, hero, false)
                    break
                elseif switch3 == 7 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_BOO_SCORPION", singleShouter, hero, false)
                    break
                else
                    goto FLOW_native_label_1
                end
            until true
            goto LAB_00f1e2e9
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId3)
            quest:DeregisterTimer(timerId)
            return
        end
        switch4 = quest:GetHeroTitle()
        repeat
            if switch4 == 1 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_17", singleShouter, hero, false)
                break
            elseif switch4 == 2 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_20", singleShouter, hero, false)
                break
            elseif switch4 == 3 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_11", singleShouter, hero, false)
                break
            elseif switch4 == 4 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_06", singleShouter, hero, false)
                break
            elseif switch4 == 5 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_03", singleShouter, hero, false)
                break
            elseif switch4 == 6 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_12", singleShouter, hero, false)
                break
            elseif switch4 == 7 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_04", singleShouter, hero, false)
                break
            elseif switch4 == 8 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_15", singleShouter, hero, false)
                break
            elseif switch4 == 9 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_10", singleShouter, hero, false)
                break
            elseif switch4 == 10 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_13", singleShouter, hero, false)
                break
            elseif switch4 == 11 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_07", singleShouter, hero, false)
                break
            elseif switch4 == 12 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_16", singleShouter, hero, false)
                break
            elseif switch4 == 13 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_18", singleShouter, hero, false)
                break
            elseif switch4 == 14 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_09", singleShouter, hero, false)
                break
            elseif switch4 == 15 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_08", singleShouter, hero, false)
                break
            elseif switch4 == 16 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_19", singleShouter, hero, false)
                break
            elseif switch4 == 17 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_01", singleShouter, hero, false)
                break
            elseif switch4 == 18 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_14", singleShouter, hero, false)
                break
            elseif switch4 == 19 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_05", singleShouter, hero, false)
                break
            elseif switch4 == 20 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_02", singleShouter, hero, false)
                break
            else
                goto FLOW_native_label_1
            end
        until true
        ::LAB_00f1e2e9::
        ::FLOW_native_label_1::
    end
    ::LAB_00f1e2fb::
    hero39 = hero
    if hero ~= nil and hero:MsgIsHitBy("") then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId3)
            quest:DeregisterTimer(timerId)
            return
        end
        quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") - 5)
    end
    ::LAB_00f1e350::
    if not quest:IsLevelLoaded("Arena") then
        if quest:IsActiveThreadTerminating() then goto LAB_00f1eba3 end
        quest:RemoveQuestInfoElement(i_stk_fc_3)
        quest:DisplayQuestInfo(false)
        quest:NewScriptFrame()
        goto FLOW_after_lab_00f1e382
    end
    -- TODO(native): goto LAB_00f1d1e0
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
    local i_stk_1c_1
    local scratchValue = #quest:GetAllThingsWithScriptName("AudienceMember")
    if 0 >= scratchValue then return end
    i_stk_1c_1 = scratchValue
    repeat
        -- TODO(native): xStack_18._4_4_ = *(undefined4 *)(iVar5 + 4 + (int)xStack_c);
        local switch1 = math.random(0, 32767) % 6
        repeat
            if switch1 == 0 then
                quest:EntityPlayObjectAnimation(nil, "CHEER_LOOP_01", true)
                break
            elseif switch1 == 1 then
                quest:EntityPlayObjectAnimation(nil, "CHEER_LOOP_02", true)
                break
            elseif switch1 == 2 then
                quest:EntityPlayObjectAnimation(nil, "CLAP_LOOP_02", true)
                break
            elseif switch1 == 3 then
                quest:EntityPlayObjectAnimation(nil, "SHOUT_LOOP_01", true)
                break
            elseif switch1 == 4 then
                quest:EntityPlayObjectAnimation(nil, "THUMBS_DOWN_LOOP_01", true)
                break
            else
                quest:EntityPlayObjectAnimation(nil, "WAVE_LOOP_01", true)
            end
        until true
        quest:EntitySetCutsceneBehaviour(nil, CUTSCENE_BEHAVIOUR_NOT_PAUSED)
        i_stk_1c_1 = i_stk_1c_1 - 1
    until i_stk_1c_1 == 0
end

-- Q_Arena.GetFanfareMusic (retail 0x00f14270)
function GetFanfareMusic(quest, fanfare)
    repeat
        if fanfare == 1 then
            return 36
        elseif fanfare == 2 then
            return 37
        elseif fanfare == 3 then
            return 38
        elseif fanfare == 4 then
            return 39
        elseif fanfare == 5 then
            return 40
        elseif fanfare == 6 then
            return 41
        elseif fanfare == 7 then
            return 42
        elseif fanfare == 8 then
            return 43
        elseif fanfare == 9 then
            return 44
        else
            return 35
        end
    until true
end

-- Q_Arena.PlayWave (retail 0x00f1eed0)
function PlayWave(quest)
    local hero = quest:GetHero()
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
    local scratchValue, scratchValue2, predicateResult37, scratchValue4, creatureCounterIds
    local creatureGroupIndex, creatureType, ctr_10c, ctr_fc, timerId, createCreatureNearby
    local scratchValue12, scratchValue13, scratchValue14, count, scratchValue20, scratchValue21
    local scratchValue22, scratchValue23, scratchValue24, initialCreatureCounts, sequence1, getPos
    local whisperAlly, hero10, scratchValue28, whisperAlly3, hero11, scratchValue32, arenaSpawn
    local savedCreatureGroupIndex, totalCreaturesIndex, totalCreaturesIndex2, scratchValue33
    local multipleBigCreatureSpawnPoint, arenaEnemy, arenaSpawn2, movie, dataString, resource
    local scratchValue36, createCreatureNearby2, scratchValue38
    local bigCreatureSpawnPoint = quest:GetThingWithScriptName("BigCreatureSpawnPoint")
    timerId = quest:RegisterTimer()
    quest:SetStateBool("PauseCrowdChecker", false)
    local timerId2 = timerId
    if not quest:GetStateBool("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_ShortWave") and (quest:GetStateInt("ArenaRound") ~= 0 or (quest:GetStateInt("ArenaRoundWave") ~= 0)) then
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); goto LAB_00f2119d end
        quest:AddLineToConversation(quest:AddNewConversation(hero, false, false), "TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_THREE", hero, hero, false)
        timerId = timerId2
        quest:SetTimer(timerId2, 2)
        while 0 < quest:GetTimer(timerId2) do
            if not quest:NewScriptFrame() then goto LAB_00f21195 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f21195 end
        quest:AddLineToConversation(quest:AddNewConversation(hero, false, false), "TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_TWO", hero, hero, false)
        timerId = timerId2
        quest:SetTimer(timerId2, 2)
        while 0 < quest:GetTimer(timerId2) do
            if not quest:NewScriptFrame() then goto LAB_00f20cc5 end
        end
        if not quest:IsActiveThreadTerminating() then
            quest:AddLineToConversation(quest:AddNewConversation(hero, false, false), "TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_ONE", hero, hero, false)
            timerId = timerId2
            quest:SetTimer(timerId2, 2)
            while 0 < quest:GetTimer(timerId2) do
                if not quest:NewScriptFrame() then goto LAB_00f20cc5 end
            end
            if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(quest:AddNewConversation(hero, false, false), "TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_GO", hero, hero, false); goto LAB_00f1f251 end
        end
        quest:DeregisterTimer(timerId2)
        goto LAB_00f2119d
    end
    ::LAB_00f1f251::
    multipleBigCreatureSpawnPoint = quest:GetAllThingsWithScriptName("MultipleBigCreatureSpawnPoint")
    count = #multipleBigCreatureSpawnPoint
    arenaSpawn2 = quest:GetAllThingsWithScriptName("ArenaSpawn")
    createCreatureNearby = #arenaSpawn2
    scratchValue22 = createCreatureNearby
    arenaSpawn = quest:GetFurthestWithScriptName(hero, "ArenaSpawn")
    if arenaSpawn ~= nil then
        dataString = arenaSpawn:GetDataString()
    end
    timerId = parseGameInteger(dataString)
    scratchValue20 = timerId
    scratchValue38 = timerId - 1
    scratchValue4 = 1
    if scratchValue38 == 0xffffffff then
        if quest:IsActiveThreadTerminating() then goto LAB_00f2116f end
        scratchValue38 = createCreatureNearby - 1
    end
    arenaEnemy = quest:GetAllThingsWithScriptName("ArenaEnemy")
    sequence1 = quest:GetStateInt("ArenaRound") == 0 and quest:GetStateInt("ArenaRoundWave") == 0 and #arenaEnemy ~= 0
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
                scratchValue21 = 0
                if 0 < quest:GetStateInt("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_NumCreatures") then
                    repeat
                        if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
                        if quest:GetStateInt("ArenaRound") < 5 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
                            if scratchValue4 == 0 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
                                scratchValue24 = 0
                                if 0 < createCreatureNearby then
                                    scratchValue23 = 0
                                    repeat
                                        timerId = scratchValue23
                                        scratchValue32 = arenaSpawn2[scratchValue23 / 12 + 1]:GetDataString()
                                        if parseGameInteger(scratchValue32) == scratchValue38 then
                                            if not quest:IsActiveThreadTerminating() then
                                                if quest:GetStateInt("ArenaRound") == 3 then
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:SetCreatureCreationDelayFrames(1)
                                                        scratchValue36 = quest:CreateCreature(quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_CreatureType"), arenaSpawn2[timerId / 12 + 1]:GetPos(), "ArenaEnemy")
                                                        goto LAB_00f1fcce
                                                    end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    quest:SetCreatureCreationDelayFrames(1)
                                                    scratchValue36 = quest:CreateCreatureNearby(quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_CreatureType"), arenaSpawn2[timerId / 12 + 1]:GetPos(), 1.0, "ArenaEnemy")
                                                    goto LAB_00f1fcce
                                                end
                                                goto FLOW_past_lab_00f1fcce
                                                ::LAB_00f1fcce::
                                                quest:ResetCreatureCreationDelayFrames()
                                                quest:EntitySetCutsceneBehaviour(scratchValue36, CUTSCENE_BEHAVIOUR_PAUSED)
                                                whisperAlly = quest:GetThingWithScriptName("WhisperAlly")
                                                if not (whisperAlly ~= nil and whisperAlly:IsAlive()) then
                                                    if not quest:IsActiveThreadTerminating() then hero10 = hero; goto LAB_00f1fe19 end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    scratchValue33 = math.random(0, 32767) & 0x80000001
                                                    scratchValue = scratchValue33 == 0
                                                    if scratchValue33 < 0 then
                                                        scratchValue = (scratchValue33 - 1 | 0xfffffffe) == 0xffffffff
                                                    end
                                                    if scratchValue then
                                                        if not quest:IsActiveThreadTerminating() then quest:GiveThingBestEnemyTarget(scratchValue36, quest:GetThingWithScriptName("WhisperAlly")); goto LAB_00f1fe22 end
                                                    elseif not quest:IsActiveThreadTerminating() then
                                                        hero10 = hero
                                                        goto LAB_00f1fe19
                                                    end
                                                end
                                                goto FLOW_past_lab_00f1fe19
                                                ::LAB_00f1fe19::
                                                quest:GiveThingBestEnemyTarget(scratchValue36, hero10)
                                                goto LAB_00f1fe22
                                                ::FLOW_past_lab_00f1fe19::
                                                goto FLOW_past_lab_00f1fe22
                                                ::LAB_00f1fe22::
                                                quest:SetStateBool("ArenaSpawnNeeded_" .. scratchValue38, true)
                                                timerId = scratchValue23
                                                createCreatureNearby = scratchValue22
                                                goto LAB_00f1fe48
                                                ::FLOW_past_lab_00f1fe22::
                                                ::FLOW_past_lab_00f1fcce::
                                            end
                                            goto LAB_00f20cc5
                                        end
                                        ::LAB_00f1fe48::
                                        scratchValue24 = scratchValue24 + 1
                                        scratchValue23 = timerId + 12
                                    until scratchValue24 >= createCreatureNearby
                                end
                                scratchValue38 = scratchValue38 - 1
                                if scratchValue38 == 0xffffffff then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
                                    scratchValue38 = createCreatureNearby - 1
                                end
                                scratchValue4 = 1
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
                                scratchValue23 = 0
                                if 0 < createCreatureNearby then
                                    scratchValue24 = 0
                                    repeat
                                        timerId = scratchValue24
                                        scratchValue32 = arenaSpawn2[scratchValue24 / 12 + 1]:GetDataString()
                                        if parseGameInteger(scratchValue32) == scratchValue20 then
                                            if not quest:IsActiveThreadTerminating() then
                                                if quest:GetStateInt("ArenaRound") == 3 then
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:SetCreatureCreationDelayFrames(1)
                                                        scratchValue28 = quest:CreateCreature(quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_CreatureType"), arenaSpawn2[timerId / 12 + 1]:GetPos(), "ArenaEnemy")
                                                        goto LAB_00f1f934
                                                    end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    quest:SetCreatureCreationDelayFrames(1)
                                                    scratchValue28 = quest:CreateCreatureNearby(quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_CreatureType"), arenaSpawn2[timerId / 12 + 1]:GetPos(), 1.0, "ArenaEnemy")
                                                    goto LAB_00f1f934
                                                end
                                                goto FLOW_past_lab_00f1f934
                                                ::LAB_00f1f934::
                                                quest:ResetCreatureCreationDelayFrames()
                                                quest:EntitySetCutsceneBehaviour(scratchValue28, CUTSCENE_BEHAVIOUR_PAUSED)
                                                whisperAlly3 = quest:GetThingWithScriptName("WhisperAlly")
                                                if not (whisperAlly3 ~= nil and whisperAlly3:IsAlive()) then
                                                    if not quest:IsActiveThreadTerminating() then hero11 = hero; goto LAB_00f1fa88 end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    scratchValue33 = math.random(0, 32767) & 0x80000001
                                                    scratchValue2 = scratchValue33 == 0
                                                    if scratchValue33 < 0 then
                                                        scratchValue2 = (scratchValue33 - 1 | 0xfffffffe) == 0xffffffff
                                                    end
                                                    if scratchValue2 then
                                                        if not quest:IsActiveThreadTerminating() then quest:GiveThingBestEnemyTarget(quest:GetThingWithScriptName("WhisperAlly"), arenaSpawn); goto LAB_00f1fa91 end
                                                    elseif not quest:IsActiveThreadTerminating() then
                                                        hero11 = hero
                                                        goto LAB_00f1fa88
                                                    end
                                                end
                                                goto FLOW_past_lab_00f1fa88
                                                ::LAB_00f1fa88::
                                                quest:GiveThingBestEnemyTarget(hero11, bigCreatureSpawnPoint)
                                                goto LAB_00f1fa91
                                                ::FLOW_past_lab_00f1fa88::
                                                goto FLOW_past_lab_00f1fa91
                                                ::LAB_00f1fa91::
                                                quest:SetStateBool("ArenaSpawnNeeded_" .. scratchValue20, true)
                                                timerId = scratchValue24
                                                createCreatureNearby = scratchValue22
                                                goto LAB_00f1faba
                                                ::FLOW_past_lab_00f1fa91::
                                                ::FLOW_past_lab_00f1f934::
                                            end
                                            goto LAB_00f20cc5
                                        end
                                        ::LAB_00f1faba::
                                        scratchValue23 = scratchValue23 + 1
                                        scratchValue24 = timerId + 12
                                    until scratchValue23 >= createCreatureNearby
                                end
                                scratchValue20 = (scratchValue20 + 1) % createCreatureNearby
                                scratchValue4 = 0
                            end
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
                            createCreatureNearby2 = nil
                            local predicateResult = quest:IsActiveThreadTerminating()
                            if createCreatureNearby == 1 then
                                if predicateResult then
                                    goto LAB_00f20cc5
                                end
                                quest:SetCreatureCreationDelayFrames(1)
                                if not (bigCreatureSpawnPoint ~= nil and not bigCreatureSpawnPoint:IsNull()) then
                                    getPos = {x = 0, y = 0, z = 0}
                                else
                                    getPos = bigCreatureSpawnPoint:GetPos()
                                end
                                createCreatureNearby2 = quest:CreateCreatureNearby(quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_CreatureType"), getPos, 1.0, "ArenaEnemy")
                                quest:ResetCreatureCreationDelayFrames()
                            else
                                if predicateResult then
                                    goto LAB_00f20cc5
                                end
                                ctr_fc = 0
                                if 0 < count then
                                    timerId = 0
                                    repeat
                                        scratchValue32 = multipleBigCreatureSpawnPoint[timerId / 12 + 1]:GetDataString()
                                        if parseGameInteger(scratchValue32) ~= scratchValue21 then ctr_fc = ctr_fc + 1; timerId = timerId + 12; goto continue_1 end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
                                        quest:SetCreatureCreationDelayFrames(1)
                                        creatureType = quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_CreatureType")
                                        savedCreatureGroupIndex = creatureGroupIndex
                                        createCreatureNearby = quest:CreateCreatureNearby(creatureType, multipleBigCreatureSpawnPoint[timerId / 12 + 1]:GetPos(), timerId, "ArenaEnemy")
                                        -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_f4,iVar6);
                                        quest:ResetCreatureCreationDelayFrames()
                                        ctr_fc = ctr_fc + 1
                                        timerId = timerId + 12
                                        ::continue_1::
                                    until ctr_fc >= count
                                end
                            end
                            quest:EntitySetCutsceneBehaviour(createCreatureNearby2, CUTSCENE_BEHAVIOUR_PAUSED)
                            quest:GiveThingBestEnemyTarget(createCreatureNearby2, hero)
                        end
                        quest:SetStateInt("TotalCreatures_" .. totalCreaturesIndex, quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex) + 1)
                        scratchValue21 = scratchValue21 + 1
                    until scratchValue21 >= quest:GetStateInt("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. savedCreatureGroupIndex .. "_NumCreatures")
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
                    return
                end
                ctr_10c = ctr_10c + 1
                totalCreaturesIndex = totalCreaturesIndex + 1
                creatureGroupIndex = savedCreatureGroupIndex + 1
            until ctr_10c >= quest:GetStateInt("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_NumWaveCreatures")
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
    end
    creatureGroupIndex = 0
    initialCreatureCounts = {}
    creatureCounterIds = {}
    totalCreaturesIndex2 = 0
    timerId = 0
    repeat
        savedCreatureGroupIndex = creatureGroupIndex
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId2)
            -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
            return
        end
        local getStateInt = quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex2)
        initialCreatureCounts[(timerId / 4) + 1] = getStateInt
        creatureCounterIds[(timerId / 4) + 1] = -1
        if 0 < getStateInt then
            savedCreatureGroupIndex = creatureGroupIndex
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            creatureCounterIds[(timerId / 4) + 1] = quest:AddQuestInfoCounterList(quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. creatureGroupIndex .. "_HUDType"), quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex2), 1.0)
        end
        if 0 < initialCreatureCounts[(timerId / 4) + 1] then
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            quest:UpdateQuestInfoCounterList(creatureCounterIds[(timerId / 4) + 1], quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex2), -1)
        end
        creatureGroupIndex = savedCreatureGroupIndex + 1
        timerId = timerId + 4
        totalCreaturesIndex2 = totalCreaturesIndex2 + 1
    until creatureGroupIndex >= 3
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId2)
        -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
        return
    end
    quest:DisplayQuestInfo(true)
    timerId = 0
    scratchValue12 = 0
    totalCreaturesIndex2 = 0
    repeat
        if quest:IsActiveThreadTerminating() then goto LAB_00f2049f end
        timerId = timerId + quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex2)
        scratchValue12 = scratchValue12 + 1
        totalCreaturesIndex2 = totalCreaturesIndex2 + 1
    until scratchValue12 >= 3
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId2)
        -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
        return
    end
    while 0 < timerId do
        quest:NewScriptFrame()
        timerId = 0
        if quest:IsActiveThreadTerminating() then goto LAB_00f207f4 end
        totalCreaturesIndex2 = 0
        scratchValue13 = 0
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00f208af end
            timerId = timerId + quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex2)
            if 0 < initialCreatureCounts[(scratchValue13 / 4) + 1] then
                if quest:IsActiveThreadTerminating() then goto LAB_00f2096f end
                quest:UpdateQuestInfoCounterList(creatureCounterIds[(scratchValue13 / 4) + 1], quest:GetStateInt("TotalCreatures_" .. totalCreaturesIndex2), -1)
            end
            scratchValue13 = scratchValue13 + 4
            totalCreaturesIndex2 = totalCreaturesIndex2 + 1
        until scratchValue13 >= 12
        if quest:IsActiveThreadTerminating() then goto LAB_00f20a2f end
        if quest:GetStateInt("ExtraCreatures") ~= 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            quest:RemoveQuestInfoElement(creatureCounterIds[0 + 1])
            initialCreatureCounts[0 + 1] = initialCreatureCounts[0 + 1] + quest:GetStateInt("ExtraCreatures")
            creatureCounterIds[0 + 1] = quest:AddQuestInfoCounterList(quest:GetStateString("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. 0 .. "_HUDType"), initialCreatureCounts[0 + 1], 1.0)
            quest:SetStateInt("ExtraCreatures", 0)
        end
        if not quest:IsLevelLoaded("Arena") then
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            quest:SetStateBool("PlayerLeaving", true)
            scratchValue14 = 0
            totalCreaturesIndex2 = 0
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
                quest:SetStateInt("TotalCreatures_" .. totalCreaturesIndex2, 0)
                scratchValue14 = scratchValue14 + 1
                totalCreaturesIndex2 = totalCreaturesIndex2 + 1
            until scratchValue14 >= 3
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
        end
    end
    if not quest:IsActiveThreadTerminating() then timerId = 0; goto LAB_00f20700 end
    ::LAB_00f20ca1::
    ::LAB_00f20cc5::
    quest:DeregisterTimer(timerId2)
    goto LAB_00f2119d
    ::LAB_00f2049f::
    quest:DeregisterTimer(timerId2)
    -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
    do return end
    ::LAB_00f207f4::
    goto LAB_00f20ae2
    ::LAB_00f208af::
    goto LAB_00f20ae2
    ::LAB_00f2096f::
    goto LAB_00f20ae2
    ::LAB_00f20b8b::
    goto LAB_00f21195
    ::LAB_00f20700::
    while true do
        if quest:IsActiveThreadTerminating() then goto LAB_00f20b8b end
        if 0 < initialCreatureCounts[(timerId / 4) + 1] then
            if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
            quest:RemoveQuestInfoElement(creatureCounterIds[(timerId / 4) + 1])
        end
        timerId = timerId + 4
        if 11 < timerId then break end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
    quest:DisplayQuestInfo(false)
    timerId = quest:GetStateInt("ArenaRoundWave")
    quest:SetStateInt("ArenaRoundWave", timerId + 1)
    quest:SetStateBool("PauseCrowdChecker", true)
    if timerId + 1 == quest:GetStateInt("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_NumWaves") then
        if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
        quest:SetStateInt("ArenaRound", quest:GetStateInt("ArenaRound") + 1)
        quest:SetStateInt("ArenaRoundWave", 0)
        if quest:GetStateInt("NewCrowdPoints") / 100 < 6 then
            if 0 < quest:GetStateInt("NewCrowdPoints") / 100 then goto LAB_00f20ce5 end
            goto LAB_00f20e68
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            goto LAB_00f20ce5
        end
        goto FLOW_past_lab_00f20e68
        ::LAB_00f20e68::
        if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
        quest:Pause(1.0)
        quest:OverrideMusic(GetFanfareMusic(quest, math.random(0, 32767) % 10), false, false)
        quest:Pause(5.0)
        quest:StopOverrideMusic(false)
        GivePrizeFund(quest)
        while not quest:IsHeroControlledByPlayer() do
            if not quest:NewScriptFrame() then goto LAB_00f212cc end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f212cc end
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        resource = resources:NewResource()
        resources:PrepareResource(resource)
        while not resources:TryAcquire(resource, hero, 4) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource)
                quest:PauseAllNonScriptedEntities(false)
                goto FLOW_after_lab_00f211ba
            end
        end
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            quest:PauseAllNonScriptedEntities(false)
            goto FLOW_after_lab_00f211ba
        end
        if 7 < quest:GetStateInt("ArenaRound") then
            quest:FadeScreenOut(0.5, 0.5)
            quest:Pause(0.5)
            resources:ReleaseResource(resource)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            goto LAB_00f21166
        end
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            quest:PauseAllNonScriptedEntities(false)
        else
            quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_YES", "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_NO", "", scratchValue32, GetEndRoundQuestion(quest) ~= 0)
            timerId = quest:MsgIsQuestionAnsweredYesOrNo()
            while timerId < 0 do
                if not quest:NewScriptFrame() then goto LAB_00f212a6 end
                timerId = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource)
                quest:PauseAllNonScriptedEntities(false)
                goto FLOW_after_lab_00f211ba
            end
            predicateResult37 = quest:IsActiveThreadTerminating()
            if timerId == 1 then
                if not predicateResult37 then
                    quest:SetStateInt("GoldMultiplier", quest:GetStateInt("GoldMultiplier") + 1)
                    quest:FadeScreenOut(0.5, 0.5)
                    quest:Pause(0.5)
                    resources:ReleaseResource(resource)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00f21166
                end
            else
                if predicateResult37 then
                    resources:ReleaseResource(resource)
                    quest:PauseAllNonScriptedEntities(false)
                    goto FLOW_after_lab_00f211ba
                end
                quest:SetStateBool("PlayerLeaving", true)
                quest:GiveHeroGold(quest:GetStateInt("GoldTotal"))
                quest:SetStateInt("GoldTotal", 0)
                local conversationId = quest:AddNewConversation(hero, false, false)
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_V2_ARENA_KEEPER_RETURN", hero, hero, false)
                while quest:IsConversationActive(conversationId) do
                    quest:NewScriptFrame()
                    if quest:IsActiveThreadTerminating() then
                        resources:ReleaseResource(resource)
                        quest:PauseAllNonScriptedEntities(false)
                        goto FLOW_after_lab_00f211ba
                    end
                end
                if not quest:IsActiveThreadTerminating() then
                    quest:FadeScreenOut(0.5, 0.5)
                    quest:Pause(0.5)
                    resources:ReleaseResource(resource)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00f21166
                end
            end
            ::LAB_00f212a6::
            resources:ReleaseResource(resource)
            quest:PauseAllNonScriptedEntities(false)
        end
        ::FLOW_after_lab_00f211ba::
        resources:DestroyMovie(movie)
        ::FLOW_past_lab_00f20e68::
        goto FLOW_past_lab_00f20ce5
        ::LAB_00f20ce5::
        if not quest:IsActiveThreadTerminating() then
            quest:ReadGlobalGameData(2672)
            quest:ReadGlobalGameData(2668)
            math.random(0, 32767)
            math.random(0, 32767)
            -- TODO(native): fStack_d8 = (float)(iVar12 % 0x168) * 0.0027777778450399637;
            -- TODO(native): Math_CartesianToSpherical(xStack_f4,(int)fStack_d8);
            getPos = quest:GetThingWithScriptName("ARENA_CentrePoint"):GetPos()
            -- TODO(native): xStack_e4._4_4_ = f_stk_f0 + pCVar13.y;
            -- TODO(native): xStack_e4._0_4_ = (float)xStack_f4 + pCVar13.x;
            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(quest:ReadGlobalGameData(0xa6c) + (uVar11 % (uint)(iVar4 - iVar6 >> 2)) * 4),(int)&xStack_108);
            quest:CreateObject("", getPos, "ArenaEnemy")
            goto LAB_00f20e68
        end
        ::FLOW_past_lab_00f20ce5::
        ::LAB_00f212cc::
        goto LAB_00f20cc5
    end
    ::LAB_00f21166::
    ::LAB_00f2116f::
    timerId = timerId2
    ::LAB_00f21195::
    quest:DeregisterTimer(timerId)
    ::LAB_00f2119d::
    do return end
    ::LAB_00f20a2f::
    ::LAB_00f20ae2::
    quest:DeregisterTimer(timerId2)
    -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
end

-- Q_Arena.GivePrizeFund (retail 0x00f21300)
function GivePrizeFund(quest)
    local pOther
    local hero = quest:GetHero()
    local conversationID = quest:AddNewConversation(hero, true, false)
    quest:AddPersonToConversation(conversationID, hero)
    local switch = quest:GetStateInt("GoldMultiplier")
    repeat
        if switch == 0 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 100)
            -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND1_PRIZE");
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND1_PRIZE_LATER"
            break
        elseif switch == 1 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 500)
            -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND2_PRIZE");
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND2_PRIZE_LATER"
            break
        elseif switch == 2 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 1000)
            -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND3_PRIZE");
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND3_PRIZE_LATER"
            break
        elseif switch == 3 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 2000)
            -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND4_PRIZE");
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND4_PRIZE_LATER"
            break
        elseif switch == 4 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 3000)
            -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND5_PRIZE");
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND5_PRIZE_LATER"
            break
        elseif switch == 5 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 4000)
            -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND6_PRIZE");
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND6_PRIZE_LATER"
            break
        elseif switch == 6 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 8000)
            -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND7_PRIZE");
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND7_PRIZE_LATER"
            break
        elseif switch == 7 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 10000)
            -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND8_PRIZE");
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND8_PRIZE_LATER"
            break
        else
            break
        end
    until true
    quest:AddLineToConversation(conversationID, "", hero, hero, false)
    quest:AddLineToConversation(conversationID, pOther, hero, hero, false)
    while quest:IsConversationActive(conversationID) do
        if not quest:NewScriptFrame() then return end
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

