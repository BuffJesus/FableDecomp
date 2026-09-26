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
    local resource7, actorMap3, actorMap4, scratchValue12, resource8, resource9, actorMap5
    local actorMap6, movie, movie2
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
        if quest:IsActiveThreadTerminating() then goto LAB_00f141c7 end
        quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_02", "ArenaCells", "KnotholeGlade")
        quest:SetTimer(timerId, 60)
        while 0 < quest:GetTimer(timerId) do
            if not quest:NewScriptFrame() then goto LAB_00f141c7 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f141c7 end
        while not quest:IsHeroControlledByPlayer() do
            if not quest:NewScriptFrame() then goto LAB_00f141c7 end
        end
        quest:SetStateInt("ArenaState", 4)
    end
    if quest:GetStateInt("ArenaState") == 4 then
        if quest:IsActiveThreadTerminating() then goto LAB_00f141c7 end
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
        if quest:IsActiveThreadTerminating() then goto LAB_00f141c7 end
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
            if not quest:NewScriptFrame() then goto LAB_00f141c7 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f141c7 end
        quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("ArenaInsideHSP"), false)
        while not quest:IsLevelLoaded("Arena") do
            if not quest:NewScriptFrame() then goto LAB_00f141c7 end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f141c7 end
        quest:MiniMapSetAsEnabled(false)
        quest:SetStateBool("PlayerLeaving", false)
        local arenaHeroGate = quest:GetThingWithScriptName("ArenaHeroGate")
        quest:OpenDoor(arenaHeroGate)
        quest:SetThingPersistent(arenaHeroGate, true)
    end
    actorMap4 = resources:NewActorMap()
    if quest:GetStateInt("ArenaState") == 7 then
        if not quest:IsActiveThreadTerminating() then
            if quest:GetStateInt("ArenaRound") < 3 then
                local cellWhisper4 = quest:GetThingWithScriptName("CellWhisper")
                if cellWhisper4 ~= nil and cellWhisper4:IsAlive() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00f141be end
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
                if not quest:NewScriptFrame() then goto LAB_00f141be end
            end
            if not quest:IsActiveThreadTerminating() then
                quest:FadeScreenOut(0.5, 0.5)
                quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("ArenaInsideHSP"), false)
                while not quest:IsLevelLoaded("Arena") do
                    if not quest:NewScriptFrame() then goto LAB_00f141be end
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
    goto FLOW_past_lab_00f11eec
    ::LAB_00f11eec::
    AnimateCrowd(quest)
    while not quest:GetStateBool("PlayerWon") do
        if not quest:NewScriptFrame() then goto LAB_00f141be end
        while not quest:GetStateBool("PlayerLeaving") and not quest:GetStateBool("PlayerWon") do
            if not quest:NewScriptFrame() then goto LAB_00f141be end
            if quest:GetStateInt("ArenaRoundWave") ~= 0 then goto LAB_00f13472 end
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
                            goto FLOW_native_label_1
                        end
                    until true
                    ::FLOW_native_label_1::
                    scratchValue12 = "CS_ARENA_ROUND_1"
                    quest:OverrideMusic(GetFanfareMusic(quest, math.random(0, 32767) % 10), false, false)
                    goto FLOW_native_label_2
                elseif switch == 1 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_02", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 0)
                    resources:SetString(actorMap6, "$SAY1", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND2_10")
                    resources:SetString(actorMap6, "$SAY2", "TEXT_QST_005_V2_ARENA_KEEPER_ROUND2_20")
                    scratchValue12 = "CS_ARENA_ROUND_BEGIN_GENERIC"
                    goto FLOW_native_label_2
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
                    goto FLOW_native_label_2
                elseif switch == 8 then
                    quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_09", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 3)
                    quest:SetStateInt("ArenaState", 8)
                    scratchValue12 = "CS_ARENA_ROUND_BOTH_WINNERS"
                    resources:SetString(actorMap6, "$THEME", "RESET")
                    break
                else
                    goto FLOW_native_label_2
                end
            until true
            ::FLOW_native_label_2::
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
            goto LAB_00f141be
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
            goto LAB_00f141be
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
                if quest:IsActiveThreadTerminating() then goto LAB_00f141be end
                if quest:GetStateInt("ArenaState") == 8 then
                    while not quest:IsLevelLoaded("ArenaCells") do
                        if not quest:NewScriptFrame() then goto LAB_00f141be end
                    end
                    local cellWhisper7 = quest:GetThingWithScriptName("CellWhisper")
                    if cellWhisper7 ~= nil and cellWhisper7:IsAlive() then
                        quest:RemoveThing(quest:GetThingWithScriptName("CellWhisper"), false, true)
                    end
                    quest:SetStateBool("PlayerWon", true)
                else
                    PlayWave(quest)
                end
            end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f141be end
        if not quest:GetStateBool("PlayerLeaving") then goto continue_2 end
        movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(0.5)
        quest:ResetToDefaultTheme(0.0)
        quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("ArenaCellEntrance2"), false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        while not quest:IsLevelLoaded("ArenaCells") do
            if not quest:NewScriptFrame() then goto LAB_00f141be end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f141be end
        quest:FadeScreenIn()
        quest:MiniMapSetAsEnabled(true)
        quest:SetStateInt("ArenaState", 7)
        if quest:GetStateInt("ArenaRound") < 3 then
            local cellWhisper9 = quest:GetThingWithScriptName("CellWhisper")
            if cellWhisper9 ~= nil and cellWhisper9:IsAlive() then
                if not quest:IsActiveThreadTerminating() then quest:RemoveThing(quest:GetThingWithScriptName("CellWhisper"), false, true); goto LAB_00f13a09 end
                goto LAB_00f141be
            end
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00f141be end
            local cellWhisper11 = quest:GetThingWithScriptName("CellWhisper")
            if not (cellWhisper11 ~= nil and cellWhisper11:IsAlive()) then
                quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", quest:GetThingWithScriptName("FlickPoint"):GetPos(), "CellWhisper")
            end
        end
        ::LAB_00f13a09::
        quest:AutoSaveCheckPoint()
        while quest:GetStateInt("ArenaState") ~= 6 do
            if not quest:NewScriptFrame() then goto LAB_00f141be end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f141be end
        quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("ArenaInsideHSP"), false)
        quest:FadeScreenOut(0.5, 0.5)
        while not quest:IsLevelLoaded("Arena") do
            if not quest:NewScriptFrame() then goto LAB_00f141be end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f141be end
        AnimateCrowd(quest)
        quest:MiniMapSetAsEnabled(false)
        quest:SetStateInt("GoldMultiplier", 0)
        ::continue_2::
    end
    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00f11eec end
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
    goto LAB_00f141be
    ::LAB_00f13f09::
    resources:ReleaseResource(resource9)
    resources:ReleaseResource(resource8)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(resource7)
    ::FLOW_past_lab_00f11eec::
    ::LAB_00f141be::
    resources:DestroyActorMap(actorMap4)
    ::LAB_00f141c7::
    quest:DeregisterTimer(timerId)
end

-- Q_Arena.Init (retail 0x00cfa700)
function Init(quest)
    quest:SetStateBool("ArenaSpawnNeeded_10", true)  -- native constructor: initial value
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
    -- TODO(native): *(undefined4 *)__element("TotalCreatures", 0) = 0;
    -- TODO(native): *(undefined4 *)__element("TotalCreatures", 1) = 0;
    -- TODO(native): *(undefined4 *)__element("TotalCreatures", 2) = 0;
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
    local scratchValue4, c_stk_151_1, arenaAudience01, scratchValue6, newCrowdBaseLevel
    local scratchValue7, scratchValue10, scratchValue11, timerId, timerId3, i_stk_fc_3, switch4
    local hero39, playCriteriaSoundOnThing, scratchValue21, scratchValue22, scratchValue24
    local crowdSoundSource
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
        local i_stk_114_2 = scratchValue10
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
        if i_stk_114_2 ~= quest:GetStateInt("NewCrowdHappiness") or (-1 ~= quest:GetStateInt("NewCrowdBaseLevel")) then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId)
                return
            end
            newCrowdBaseLevel = quest:GetStateInt("NewCrowdBaseLevel")
            quest:SetStateInt("NewCrowdHappiness", i_stk_114_2)
            scratchValue21 = 0
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
                    quest:PlayCriteriaSoundOnThing(crowdSoundSource[scratchValue11 + 1], arenaAudience01)
                    -- TODO(native): *puStack_10 = uVar5;
                    c_stk_151_1 = 0
                    quest:Pause(0.3)
                    scratchValue21 = scratchValue21 + 1
                    scratchValue11 = scratchValue11 + 1
                until scratchValue21 >= #crowdSoundSource
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId)
                return
            end
        end
        scratchValue22 = 0
        if #arenaAudience01 ~= 0 then
            scratchValue7 = 0
            repeat
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId3)
                    quest:DeregisterTimer(timerId)
                    return
                end
                if not quest:IsCameraPosOnScreen(arenaAudience01[scratchValue7 + 1]:GetPos()) then scratchValue22 = scratchValue22 + 1; scratchValue7 = scratchValue7 + 1; goto continue_4 end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId3)
                    quest:DeregisterTimer(timerId)
                    return
                end
                if i_stk_114_2 == 0 then
                    local scratchValue18 = math.random(0, 32767) & 0x80000001
                    scratchValue2 = scratchValue18 == 0
                    if scratchValue18 < 0 then
                        scratchValue2 = (scratchValue18 - 1 | 0xfffffffe) == 0xffffffff
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
                elseif i_stk_114_2 == 1 then
                    local scratchValue20 = math.random(0, 32767) & 0x80000001
                    scratchValue3 = scratchValue20 == 0
                    if scratchValue20 < 0 then
                        scratchValue3 = (scratchValue20 - 1 | 0xfffffffe) == 0xffffffff
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
                elseif i_stk_114_2 == 2 then
                    quest:EntityPlayObjectAnimation(arenaAudience01[scratchValue7 + 1], "CHEER_LOOP_01", true)
                end
                scratchValue22 = scratchValue22 + 1
                scratchValue7 = scratchValue7 + 1
                ::continue_4::
            until scratchValue22 >= #arenaAudience01
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
        quest:PlayCriteriaSoundOnThing(singleShouter, crowdSoundSource)
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
        scratchValue24 = math.random(0, 32767) & 0x80000001
        scratchValue4 = scratchValue24 == 0
        if scratchValue24 < 0 then
            scratchValue4 = (scratchValue24 - 1 | 0xfffffffe) == 0xffffffff
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
    helper_F25980(quest, this + 152, DAT_0143e90c + 0x1044)
    -- TODO(native): name field 0x48 (CCharString)
    quest:SetStateString("self_0x48", "ARENA_HECKLE_SMALL_LOOP")
    -- TODO(native): name field 0x4c (CCharString)
    quest:SetStateString("self_0x4c", "ARENA_CHANT_LOOP")
    -- TODO(native): name field 0x50 (CCharString)
    quest:SetStateString("self_0x50", "ARENA_APPLAUSE_SMALL_LOOP")
    -- TODO(native): name field 0x54 (CCharString)
    quest:SetStateString("self_0x54", "ARENA_CHEER_CROWD")
    -- TODO(native): name field 0x58 (CCharString)
    quest:SetStateString("self_0x58", "ARENA_AWWW")
    -- TODO(native): name field 0x5c (CCharString)
    quest:SetStateString("self_0x5c", "ARENA_HECKLE_SMALL_LOOP")
    -- TODO(native): name field 0x60 (CCharString)
    quest:SetStateString("self_0x60", "ARENA_CHANT_LOOP")
    -- TODO(native): name field 0x64 (CCharString)
    quest:SetStateString("self_0x64", "ARENA_APPLAUSE_MEDIUM_LOOP")
    -- TODO(native): name field 0x68 (CCharString)
    quest:SetStateString("self_0x68", "ARENA_CHEER_CROWD")
    -- TODO(native): name field 0x6c (CCharString)
    quest:SetStateString("self_0x6c", "ARENA_AWWW")
    -- TODO(native): name field 0x70 (CCharString)
    quest:SetStateString("self_0x70", "ARENA_HECKLE_MEDIUM_LOOP")
    -- TODO(native): name field 0x74 (CCharString)
    quest:SetStateString("self_0x74", "ARENA_CHANT_LOOP")
    -- TODO(native): name field 0x78 (CCharString)
    quest:SetStateString("self_0x78", "ARENA_APPLAUSE_BIG_LOOP")
    -- TODO(native): name field 0x7c (CCharString)
    quest:SetStateString("self_0x7c", "ARENA_CHEER_CROWD")
    -- TODO(native): name field 0x80 (CCharString)
    quest:SetStateString("self_0x80", "ARENA_AWWW")
    -- TODO(native): name field 0x84 (CCharString)
    quest:SetStateString("self_0x84", "ARENA_HECKLE_BIG_LOOP")
    -- TODO(native): name field 0x88 (CCharString)
    quest:SetStateString("self_0x88", "ARENA_CHANT_LOOP")
    -- TODO(native): name field 0x8c (CCharString)
    quest:SetStateString("self_0x8c", "ARENA_APPLAUSE_BIG_LOOP")
    -- TODO(native): name field 0x90 (CCharString)
    quest:SetStateString("self_0x90", "ARENA_CHEER_CROWD")
    -- TODO(native): pCVar1 = CCharString::operator=((CCharString *)(this + 0x94),"ARENA_AWWW");
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
    local self_0x = quest:GetStateInt("self_0x98")
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue, scratchValue2, scratchValue3, predicateResult32, scratchValue5, ctr_10c
    local ctr_fc, timerId, createCreatureNearby, readGlobalGameData, scratchValue7, scratchValue8
    local scratchValue9, scratchValue12, scratchValue13, scratchValue14, scratchValue15, sequence1
    local whisperAlly, hero10, scratchValue21, whisperAlly3, hero11, scratchValue24, scratchValue25
    local scratchValue27, arenaEnemy, movie, getDataString, resource, scratchValue29, infoElement
    local scratchValue30
    local bigCreatureSpawnPoint = quest:GetThingWithScriptName("BigCreatureSpawnPoint")
    timerId = quest:RegisterTimer()
    quest:SetStateBool("PauseCrowdChecker", false)
    local timerId2 = timerId
    -- TODO(native): if (*(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x38 + quest:GetStateInt("ArenaRoundWave") * 0x3c) == 0) and ((quest:GetStateInt("ArenaRound") ~= 0 or (quest:GetStateInt("ArenaRoundWave") ~= 0))) then
    ::LAB_00f1f251::
    local multipleBigCreatureSpawnPoint = quest:GetAllThingsWithScriptName("MultipleBigCreatureSpawnPoint")
    local count = #multipleBigCreatureSpawnPoint
    local arenaSpawn2 = quest:GetAllThingsWithScriptName("ArenaSpawn")
    createCreatureNearby = #arenaSpawn2
    local arenaSpawn = quest:GetFurthestWithScriptName(hero, "ArenaSpawn")
    if arenaSpawn ~= nil then
        getDataString = arenaSpawn:GetDataString()
    end
    timerId = tonumber(getDataString)
    scratchValue12 = timerId
    scratchValue30 = timerId - 1
    scratchValue5 = 1
    if scratchValue30 == 0xffffffff then
        if quest:IsActiveThreadTerminating() then goto LAB_00f2116f end
        scratchValue30 = createCreatureNearby - 1
    end
    arenaEnemy = quest:GetAllThingsWithScriptName("ArenaEnemy")
    sequence1 = quest:GetStateInt("ArenaRound") == 0 and quest:GetStateInt("ArenaRoundWave") == 0 and #arenaEnemy ~= 0
    if sequence1 then
        if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
        -- TODO(native): *(int *)__element("TotalCreatures", 0) = #xStack_124;
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
        -- TODO(native): if 0 < *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x28 + quest:GetStateInt("ArenaRoundWave") * 0x3c) then
        if false then
            repeat
                scratchValue = 0
                if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
                -- TODO(native): *(int *)__element("TotalCreatures", 0) = 0;
                scratchValue13 = 0
                -- TODO(native): if 0 < *(*(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c) + 0x2c + CVar14) then
                if false then
                    repeat
                        if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
                        if quest:GetStateInt("ArenaRound") < 5 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
                            if scratchValue5 == 0 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
                                scratchValue15 = 0
                                if 0 < createCreatureNearby then
                                    scratchValue14 = 0
                                    repeat
                                        timerId = scratchValue14
                                        scratchValue24 = arenaSpawn2[scratchValue14 / 12 + 1]:GetDataString()
                                        if tonumber(scratchValue24) == scratchValue30 then
                                            if not quest:IsActiveThreadTerminating() then
                                                if quest:GetStateInt("ArenaRound") == 3 then
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:SetCreatureCreationDelayFrames(1)
                                                        -- TODO(native): iVar6 = *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c)
                                                        createCreatureNearby = nil --[[unresolved native value]]
                                                        scratchValue29 = quest:CreateCreature("ArenaEnemy", arenaSpawn2[timerId / 12 + 1]:GetPos(), scratchValue24)
                                                        goto LAB_00f1fcce
                                                    end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    quest:SetCreatureCreationDelayFrames(1)
                                                    -- TODO(native): iVar6 = *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c)
                                                    createCreatureNearby = nil --[[unresolved native value]]
                                                    scratchValue29 = quest:CreateCreatureNearby("ArenaEnemy", arenaSpawn2[timerId / 12 + 1]:GetPos(), createCreatureNearby + 40 + scratchValue, arenaEnemy)
                                                    goto LAB_00f1fcce
                                                end
                                                goto FLOW_past_lab_00f1fcce
                                                ::LAB_00f1fcce::
                                                quest:ResetCreatureCreationDelayFrames()
                                                quest:EntitySetCutsceneBehaviour(scratchValue29, CUTSCENE_BEHAVIOUR_PAUSED)
                                                whisperAlly = quest:GetThingWithScriptName("WhisperAlly")
                                                if not (whisperAlly ~= nil and whisperAlly:IsAlive()) then
                                                    if not quest:IsActiveThreadTerminating() then hero10 = hero; goto LAB_00f1fe19 end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    scratchValue25 = math.random(0, 32767) & 0x80000001
                                                    scratchValue2 = scratchValue25 == 0
                                                    if scratchValue25 < 0 then
                                                        scratchValue2 = (scratchValue25 - 1 | 0xfffffffe) == 0xffffffff
                                                    end
                                                    if scratchValue2 then
                                                        if not quest:IsActiveThreadTerminating() then quest:GiveThingBestEnemyTarget(scratchValue29, quest:GetThingWithScriptName("WhisperAlly")); goto LAB_00f1fe22 end
                                                    elseif not quest:IsActiveThreadTerminating() then
                                                        hero10 = hero
                                                        goto LAB_00f1fe19
                                                    end
                                                end
                                                goto FLOW_past_lab_00f1fe19
                                                ::LAB_00f1fe19::
                                                quest:GiveThingBestEnemyTarget(scratchValue29, hero10)
                                                goto LAB_00f1fe22
                                                ::FLOW_past_lab_00f1fe19::
                                                goto FLOW_past_lab_00f1fe22
                                                ::LAB_00f1fe22::
                                                -- TODO(native): this[(int)x_stk_104 + 0xe2] = (CQ_ArenaScript)0x1;
                                                timerId = scratchValue14
                                                goto LAB_00f1fe48
                                                ::FLOW_past_lab_00f1fe22::
                                                ::FLOW_past_lab_00f1fcce::
                                            end
                                            goto LAB_00f20cc5
                                        end
                                        ::LAB_00f1fe48::
                                        scratchValue15 = scratchValue15 + 1
                                        scratchValue14 = timerId + 12
                                    until scratchValue15 >= createCreatureNearby
                                end
                                scratchValue30 = scratchValue30 - 1
                                if scratchValue30 == 0xffffffff then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
                                    scratchValue30 = createCreatureNearby - 1
                                end
                                scratchValue5 = 1
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
                                scratchValue14 = 0
                                if 0 < createCreatureNearby then
                                    scratchValue15 = 0
                                    repeat
                                        timerId = scratchValue15
                                        scratchValue24 = arenaSpawn2[scratchValue15 / 12 + 1]:GetDataString()
                                        if tonumber(scratchValue24) == scratchValue12 then
                                            if not quest:IsActiveThreadTerminating() then
                                                if quest:GetStateInt("ArenaRound") == 3 then
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:SetCreatureCreationDelayFrames(1)
                                                        -- TODO(native): iVar6 = *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c)
                                                        createCreatureNearby = nil --[[unresolved native value]]
                                                        scratchValue21 = quest:CreateCreature("ArenaEnemy", arenaSpawn2[timerId / 12 + 1]:GetPos(), scratchValue24)
                                                        scratchValue14 = scratchValue21
                                                        goto LAB_00f1f934
                                                    end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    quest:SetCreatureCreationDelayFrames(1)
                                                    -- TODO(native): iVar6 = *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c)
                                                    createCreatureNearby = nil --[[unresolved native value]]
                                                    scratchValue21 = quest:CreateCreatureNearby("ArenaEnemy", arenaSpawn2[timerId / 12 + 1]:GetPos(), createCreatureNearby + 40 + scratchValue, getDataString)
                                                    goto LAB_00f1f934
                                                end
                                                goto FLOW_past_lab_00f1f934
                                                ::LAB_00f1f934::
                                                quest:ResetCreatureCreationDelayFrames()
                                                quest:EntitySetCutsceneBehaviour(scratchValue21, CUTSCENE_BEHAVIOUR_PAUSED)
                                                whisperAlly3 = quest:GetThingWithScriptName("WhisperAlly")
                                                if not (whisperAlly3 ~= nil and whisperAlly3:IsAlive()) then
                                                    if not quest:IsActiveThreadTerminating() then hero11 = hero; goto LAB_00f1fa88 end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    scratchValue25 = math.random(0, 32767) & 0x80000001
                                                    scratchValue3 = scratchValue25 == 0
                                                    if scratchValue25 < 0 then
                                                        scratchValue3 = (scratchValue25 - 1 | 0xfffffffe) == 0xffffffff
                                                    end
                                                    if scratchValue3 then
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
                                                -- TODO(native): this[i_stk_b4 + 0xe2] = (CQ_ArenaScript)0x1;
                                                timerId = scratchValue15
                                                goto LAB_00f1faba
                                                ::FLOW_past_lab_00f1fa91::
                                                ::FLOW_past_lab_00f1f934::
                                            end
                                            goto LAB_00f20cc5
                                        end
                                        ::LAB_00f1faba::
                                        scratchValue14 = scratchValue14 + 1
                                        scratchValue15 = timerId + 12
                                    until scratchValue14 >= createCreatureNearby
                                end
                                scratchValue12 = (scratchValue12 + 1) % createCreatureNearby
                                scratchValue5 = 0
                            end
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
                            infoElement = nil
                            local predicateResult = quest:IsActiveThreadTerminating()
                            if createCreatureNearby == 1 then
                                if predicateResult then
                                    goto LAB_00f20cc5
                                end
                                quest:SetCreatureCreationDelayFrames(1)
                                -- TODO(native): pCVar5 = quest:CreateCreatureNearby("ArenaEnemy", pCVar13, (*(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c) + 0x28 + CVar14), xStack_130)
    --[[unresolved native value]]
                                infoElement = nil
                                quest:ResetCreatureCreationDelayFrames()
                            else
                                if predicateResult then
                                    goto LAB_00f20cc5
                                end
                                ctr_fc = 0
                                if 0 < count then
                                    timerId = 0
                                    repeat
                                        scratchValue24 = multipleBigCreatureSpawnPoint[timerId / 12 + 1]:GetDataString()
                                        if tonumber(scratchValue24) ~= scratchValue13 then ctr_fc = ctr_fc + 1; timerId = timerId + 12; goto continue_1 end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
                                        quest:SetCreatureCreationDelayFrames(1)
                                        -- TODO(native): iVar6 = *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c)
    --[[unresolved native value]]
                                        scratchValue = 0
                                        createCreatureNearby = quest:CreateCreatureNearby("ArenaEnemy", multipleBigCreatureSpawnPoint[timerId / 12 + 1]:GetPos(), nil + 40 + 0, scratchValue24)
                                        -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_f4,iVar6);
                                        quest:ResetCreatureCreationDelayFrames()
                                        ctr_fc = ctr_fc + 1
                                        timerId = timerId + 12
                                        ::continue_1::
                                    until ctr_fc >= count
                                end
                            end
                            quest:EntitySetCutsceneBehaviour(infoElement, CUTSCENE_BEHAVIOUR_PAUSED)
                            quest:GiveThingBestEnemyTarget(infoElement, hero)
                        end
                        -- TODO(native): *(int *)__element("TotalCreatures", 0) = *(int *)__element("TotalCreatures", 0) + 1;
                        scratchValue13 = scratchValue13 + 1
                    -- TODO(native): until not (i_stk_b8 < *(*(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c) + 0x2c + CVar14))
                    until true
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
                    return
                end
                -- TODO(native): xStack_108 = (CCharString)((int)CVar14 + 0x38);
            -- TODO(native): until not (ctr_10c < *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x28 + quest:GetStateInt("ArenaRoundWave") * 0x3c))
            until true
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
    end
    timerId = 0
    repeat
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId2)
            -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
            return
        end
        -- TODO(native): iVar6 = *__element("TotalCreatures", 0)
    --[[unresolved native value]]
        -- TODO(native): *(int *)(xStack_e4 + iVar4) = iVar6;
        -- TODO(native): *(undefined4 *)(xStack_f4 + iVar4) = 0xffffffff;
        if 0 < nil then
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            -- TODO(native): iVar6 = quest:AddQuestInfoCounterList("ArenaEnemy", (0x0 + 0x30 + *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c)), *__element("TotalCreatures", 0))
            readGlobalGameData = nil --[[unresolved native value]]
            -- TODO(native): *(int *)(xStack_f4 + iVar4) = iVar6;
        end
        if 0 < *(scratchValue29 + timerId) then
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            -- TODO(native): quest:UpdateQuestInfoCounterList(*(xStack_f4 + iVar4), *__element("TotalCreatures", 0), -1)
        end
        -- TODO(native): xStack_108 = (CCharString)((int)CVar14 + 0x38);
        timerId = timerId + 4
    until scratchValue27 >= 168
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId2)
        -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
        return
    end
    quest:DisplayQuestInfo(true)
    timerId = 0
    scratchValue7 = 0
    repeat
        if quest:IsActiveThreadTerminating() then goto LAB_00f2049f end
        timerId = timerId + *__element("TotalCreatures", 0)
        scratchValue7 = scratchValue7 + 1
    until scratchValue7 >= 3
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId2)
        -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
        return
    end
    while 0 < timerId do
        quest:NewScriptFrame()
        timerId = 0
        if quest:IsActiveThreadTerminating() then goto LAB_00f207f4 end
        scratchValue8 = 0
        repeat
            if quest:IsActiveThreadTerminating() then goto LAB_00f208af end
            timerId = timerId + *__element("TotalCreatures", 0)
            if 0 < *(scratchValue29 + scratchValue8) then
                if quest:IsActiveThreadTerminating() then goto LAB_00f2096f end
                -- TODO(native): quest:UpdateQuestInfoCounterList(*(xStack_f4 + iVar6), *__element("TotalCreatures", 0), -1)
            end
            scratchValue8 = scratchValue8 + 4
        until scratchValue8 >= 12
        if quest:IsActiveThreadTerminating() then goto LAB_00f20a2f end
        if quest:GetStateInt("ExtraCreatures") ~= 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            quest:RemoveQuestInfoElement(infoElement)
            -- TODO(native): xStack_e4._0_4_ = xStack_e4._0_4_ + *(int *)(this + 0xdc);
            -- TODO(native): xStack_f4 = quest:AddQuestInfoCounterList("ArenaEnemy", (*(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + self_0x98) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c) + 0x30), xStack_e4._0_4_)
            infoElement = nil --[[unresolved native value]]
            quest:SetStateInt("ExtraCreatures", 0)
        end
        if not quest:IsLevelLoaded("Arena") then
            if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
            quest:SetStateBool("PlayerLeaving", true)
            scratchValue9 = 0
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00f20ca1 end
                -- TODO(native): *(undefined4 *)__element("TotalCreatures", 0) = 0;
                scratchValue9 = scratchValue9 + 1
            until scratchValue9 >= 3
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
        if 0 < *(scratchValue29 + timerId) then
            if quest:IsActiveThreadTerminating() then goto LAB_00f21166 end
            -- TODO(native): quest:RemoveQuestInfoElement(*(xStack_f4 + iVar4))
        end
        timerId = timerId + 4
        if 11 < timerId then break end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00f20cc5 end
    quest:DisplayQuestInfo(false)
    quest:SetStateInt("ArenaRoundWave", quest:GetStateInt("ArenaRoundWave") + 1)
    quest:SetStateBool("PauseCrowdChecker", true)
    -- TODO(native): if iVar4 + 1 == *(quest:GetStateInt("ArenaRound") * 0x38 + 0x28 + self_0x98) then
    if false then
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
            quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_YES", "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_NO", "", "ArenaEnemy", GetEndRoundQuestion(quest) ~= 0)
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
            predicateResult32 = quest:IsActiveThreadTerminating()
            if timerId == 1 then
                if not predicateResult32 then
                    quest:SetStateInt("GoldMultiplier", quest:GetStateInt("GoldMultiplier") + 1)
                    quest:FadeScreenOut(0.5, 0.5)
                    quest:Pause(0.5)
                    resources:ReleaseResource(resource)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00f21166
                end
            else
                if predicateResult32 then
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
            local position = quest:GetThingWithScriptName("ARENA_CentrePoint"):GetPos()
            -- TODO(native): xStack_e4._4_4_ = f_stk_f0 + pCVar13.y;
            -- TODO(native): xStack_e4._0_4_ = (float)xStack_f4 + pCVar13.x;
            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(quest:ReadGlobalGameData(0xa6c) + (uVar11 % (uint)(iVar4 - iVar6 >> 2)) * 4),(int)&xStack_108);
            quest:CreateObject("", position, multipleBigCreatureSpawnPoint)
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
            goto FLOW_native_label_1
        end
    until true
    ::FLOW_native_label_1::
    quest:AddLineToConversation(conversationID, "", hero, hero, false)
    quest:AddLineToConversation(conversationID, pOther, hero, hero, false)
    while quest:IsConversationActive(conversationID) do
        if not quest:NewScriptFrame() then goto LAB_00f2154a end
    end
    ::LAB_00f2154a::
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

-- Q_Arena.helper_F25980 (retail 0x00f25980)
function helper_F25980(quest, param2)
    local scratchValue, scratchValue3, scratchValue5
    if param2 == this then return end
    -- TODO(native): iVar3 = *native_arg_param_2
    --[[unresolved native value]]
    -- TODO(native): iVar1 = *this
--[[unresolved native value]]
    -- TODO(native): xStack_4 = native_arg_param_2[1];
    local scratchValue4 = (scratchValue5 - nil) / 56
    -- TODO(native): if ((*(this + 8) - iVar1) / 0x38) < uVar2 then
    if false then
        helper_F261A0(quest, scratchValue4, nil, scratchValue5)
        -- TODO(native): *(int *)this = iVar3;
        -- TODO(native): *(uint *)(this + 8) = uVar2 * 0x38 + iVar3;
    -- TODO(native): elseif ((*(this + 4) - iVar1) / 0x38) < uVar2 then
    elseif false then
        -- TODO(native): helper_F26B30(quest, iVar3, iVar1, &0)
        -- TODO(native): xStack_4 = native_arg_param_2[1];
        -- TODO(native): pvVar4 = *(this + 4)
        scratchValue3 = nil --[[unresolved native value]]
        scratchValue = ((scratchValue3 - *this) / 56) * 56 + *param2
        while scratchValue ~= scratchValue5 do
            scratchValue = scratchValue + 56
            scratchValue3 = scratchValue3 + 56
        end
    else
        -- TODO(native): pvVar4 = helper_F26B30(quest, iVar3, iVar1, &native_arg_param_2)
--[[unresolved native value]]
        -- TODO(native): std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>,std::allocator<CIntelligentPointer<NParticleEngine::CParticleEmitter>_>_> ::_Destroy(pvVar4,*(void **)(this + 4));
    end
    -- TODO(native): *(uint *)(this + 4) = uVar2 * 0x38 + *(int *)this;
end

-- Q_Arena.helper_F261A0 (retail 0x00f261a0)
function helper_F261A0(quest, param2, param3, param4)
    local scratchValue, scratchValue2
    if param2 == 0 then
        scratchValue2 = 0
    else
        scratchValue2 = malloc(param2 * 56)
    end
    if param3 ~= param4 then
        scratchValue = param3
        repeat
            scratchValue = scratchValue + 56
        until scratchValue == param4
    end
    return scratchValue2
end

-- Q_Arena.helper_F25AC0 (retail 0x00f25ac0)
function helper_F25AC0(quest, this)
    local scratchValue, scratchValue2
    -- TODO(native): local puVar1 = *(native_arg_this + 4)
    -- TODO(native): puVar2 = *native_arg_this
    scratchValue2 = nil --[[unresolved native value]]
    while scratchValue2 ~= scratchValue do
        -- TODO(native): (**(code **)*puVar2)(0);
        scratchValue2 = scratchValue2 + 14
    end
    -- TODO(native): if *native_arg_this ~= nil then
end

-- Q_Arena.helper_F26B30 (retail 0x00f26b30)
function helper_F26B30(quest, param2)
    local scratchValue
    local scratchValue2 = (in_EDX - this) / 56
    local this_00 = param2
    if 0 < scratchValue2 then
        scratchValue = this + 44
        repeat
            -- TODO(native): CThingBuildingDef::operator=(this_00,iVar2 + -0x2c);
            -- TODO(native): *(undefined4 *)(this_00 + 0x28) = *(undefined4 *)(iVar2 - 4);
            helper_F26920(quest, param2 + (scratchValue - this), scratchValue)
            this_00 = this_00 + 56
            scratchValue = scratchValue + 56
            scratchValue2 = scratchValue2 - 1
        until scratchValue2 == 0
    end
    return this_00
end

-- Q_Arena.helper_F25B00 (retail 0x00f25b00)
function helper_F25B00(quest, param1, param2, param3)
    -- TODO(native): /* [bsim sim=0.8438385635009688 <- ego_r]
    -- TODO(native): void __fastcall
    if this ~= nil then
        -- TODO(native): CDefClassBase::CDefClassBase(this,in_EDX);
        -- TODO(native): name field 0x28 (undefined4)
        -- TODO(native): quest:SetStateInt("self_0x28", *(in_EDX + 0x28))
        helper_F25BB0(quest, this + 44, in_EDX + 44)
    end
    return
    end
end

-- Q_Arena.helper_F26920 (retail 0x00f26920)
function helper_F26920(quest, param2)
    local scratchValue3, scratchValue4, scratchValue5, scratchValue7, scratchValue8, scratchValue9
    if param2 == this then return end
    local scratchValue = param2[1]
    -- TODO(native): pCVar4 = *this
    scratchValue5 = nil --[[unresolved native value]]
    -- TODO(native): pCVar5 = *native_arg_param_2
    scratchValue7 = nil --[[unresolved native value]]
    local scratchValue10 = (scratchValue - scratchValue7) / 60
    -- TODO(native): if ((*(this + 8) - pCVar4) / 0x3c) < uVar1 then
    if false then
        helper_F26AE0(quest, scratchValue10, scratchValue7, scratchValue)
        -- TODO(native): *(int *)this = iVar3;
        -- TODO(native): *(uint *)(this + 8) = uVar1 * 0x3c + iVar3;
    else
        -- TODO(native): uVar2 = (*(this + 4) - pCVar4) / 0x3c
    --[[unresolved native value]]
        if nil < scratchValue10 then
            scratchValue3 = (nil * 60) / 60
            if 0 < scratchValue3 then
                repeat
                    -- TODO(native): CParentDefClassBase::operator=(pCVar4,pCVar5);
                    scratchValue7 = scratchValue7 + 60
                    scratchValue5 = scratchValue5 + 60
                    scratchValue3 = scratchValue3 - 1
                until scratchValue3 == 0
            end
            local scratchValue6 = param2[1]
            -- TODO(native): pCVar5 = *(this + 4)
            scratchValue8 = nil --[[unresolved native value]]
            scratchValue9 = ((scratchValue8 - *this) / 60) * 60 + *param2
            while scratchValue9 ~= scratchValue6 do
                if scratchValue8 ~= nil then
                    CParentDefClassBase(quest, scratchValue8, scratchValue9)
                end
                scratchValue8 = scratchValue8 + 60
                scratchValue9 = scratchValue9 + 60
            end
        else
            scratchValue4 = (scratchValue - scratchValue7) / 60
            if 0 < scratchValue4 then
                repeat
                    -- TODO(native): CParentDefClassBase::operator=(pCVar4,pCVar5);
                    scratchValue7 = scratchValue7 + 60
                    scratchValue5 = scratchValue5 + 60
                    scratchValue4 = scratchValue4 - 1
                until scratchValue4 == 0
            end
            -- TODO(native): pCVar5 = *(this + 4)
    --[[unresolved native value]]
            while scratchValue5 ~= nil do
                -- TODO(native): (*(code *)**(undefined4 **)pCVar4)(0);
                scratchValue5 = scratchValue5 + 60
            end
        end
    end
    -- TODO(native): *(uint *)(this + 4) = uVar1 * 0x3c + *(int *)this;
end

-- Q_Arena.helper_F25BB0 (retail 0x00f25bb0)
function helper_F25BB0(quest, param2)
    local scratchValue, this_00
    -- TODO(native): helper_F25C20(quest, (native_arg_param_2[1] - *native_arg_param_2) / 0x3c, &native_arg_param_2)
    local scratchValue3 = param2[1]
    -- TODO(native): local this_00 = *this
    -- TODO(native): pCVar3 = *piVar2
    scratchValue = nil --[[unresolved native value]]
    while scratchValue ~= scratchValue3 do
        if this_00 ~= nil then
            CParentDefClassBase(quest, this_00, scratchValue)
        end
        this_00 = this_00 + 60
        scratchValue = scratchValue + 60
    end
    -- TODO(native): *(CParentDefClassBase **)(this + 4) = this_00;
end

-- Q_Arena.helper_F26AE0 (retail 0x00f26ae0)
function helper_F26AE0(quest, param2, param, param4)
    local scratchValue2
    if param2 == 0 then
        scratchValue2 = 0
    else
        scratchValue2 = malloc(param2 * 60)
    end
    if param ~= param4 then
        local scratchValue = scratchValue2 - param
        repeat
            if param + scratchValue ~= nil then
                CParentDefClassBase(quest, param + scratchValue, param)
            end
            param = param + 60
        until param == param4
    end
    return scratchValue2
end

-- Q_Arena.helper_F26AA0 (retail 0x00f26aa0)
function helper_F26AA0(quest, this)
    local scratchValue, scratchValue2
    -- TODO(native): local puVar1 = *(native_arg_this + 4)
    -- TODO(native): puVar2 = *native_arg_this
    scratchValue2 = nil --[[unresolved native value]]
    while scratchValue2 ~= scratchValue do
        -- TODO(native): (**(code **)*puVar2)(0);
        scratchValue2 = scratchValue2 + 15
    end
    -- TODO(native): if *native_arg_this ~= nil then
end

-- Q_Arena.helper_F25F60 (retail 0x00f25f60)
function helper_F25F60(quest, param1)
    -- TODO(native): CThingBuildingDef::operator=((CThingBuildingDef *)this,(int)native_arg_param_1);
    -- TODO(native): name field 0x28 (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x28", *(native_arg_param_1 + 0x28))
    helper_F25F90(quest, this + 44, param1 + 44)
    -- TODO(native): name field 0x38 (undefined1)
    quest:SetStateBool("self_0x38", native_arg_quest:GetStateBool("self_0x38"))
end

-- Q_Arena.CParentDefClassBase (retail 0x00f25c60)
-- F25C60: bsim names this body CParentDefClassBase::CParentDefClassBase (a homologous script member); no PDB name
function CParentDefClassBase(quest, param1)
    -- TODO(native): CDefClassBase::CDefClassBase((CDefClassBase *)this,(int)native_arg_param_1);
    -- TODO(native): name field 0x28 (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x28", *(native_arg_param_1 + 0x28))
    helper_F25D20(quest, this + 44, param1 + 44)
    -- TODO(native): name field 0x38 (undefined1)
    quest:SetStateBool("self_0x38", native_arg_quest:GetStateBool("self_0x38"))
end

-- Q_Arena.helper_F25C20 (retail 0x00f25c20)
function helper_F25C20(quest, param2)
    -- TODO(native): *(undefined4 *)this = 0;
    -- TODO(native): *(undefined4 *)(this + 4) = 0;
    -- TODO(native): *(undefined4 *)(this + 8) = 0;
    if param2 ~= 0 then
        malloc(param2 * 60)
    end
    -- TODO(native): *(void **)(this + 8) = (void *)(native_arg_param_2 * 0x3c + (int)pvVar1);
    -- TODO(native): *(void **)this = pvVar1;
    -- TODO(native): *(void **)(this + 4) = pvVar1;
end

-- Q_Arena.helper_F25F90 (retail 0x00f25f90)
function helper_F25F90(quest, param2)
    local scratchValue3, scratchValue4, scratchValue6, scratchValue7, this_00, this_01
    if param2 == this then return end
    local scratchValue = param2[1]
    -- TODO(native): this_00 = *this
    this_00 = nil --[[unresolved native value]]
    -- TODO(native): pCVar6 = *native_arg_param_2
    scratchValue6 = nil --[[unresolved native value]]
    local scratchValue8 = (scratchValue - scratchValue6) / 56
    -- TODO(native): if ((*(this + 8) - this_00) / 0x38) < uVar3 then
    if false then
        helper_F26150(quest, scratchValue8, scratchValue6, scratchValue)
        -- TODO(native): *(int *)this = iVar5;
        -- TODO(native): *(uint *)(this + 8) = uVar3 * 0x38 + iVar5;
    else
        -- TODO(native): uVar4 = (*(this + 4) - this_00) / 0x38
    --[[unresolved native value]]
        if nil < scratchValue8 then
            scratchValue3 = (nil * 56) / 56
            if 0 < scratchValue3 then
                repeat
                    Copy(quest, this_00, scratchValue6)
                    scratchValue6 = scratchValue6 + 56
                    this_00 = this_00 + 56
                    scratchValue3 = scratchValue3 - 1
                until scratchValue3 == 0
            end
            local scratchValue5 = param2[1]
            -- TODO(native): this_01 = *(this + 4)
            this_01 = nil --[[unresolved native value]]
            scratchValue7 = ((this_01 - *this) / 56) * 56 + *param2
            while scratchValue7 ~= scratchValue5 do
                if this_01 ~= nil then
                    COpinionDeedReactionDef(quest, this_01, scratchValue7)
                end
                this_01 = this_01 + 56
                scratchValue7 = scratchValue7 + 56
            end
        else
            scratchValue4 = (scratchValue - scratchValue6) / 56
            if 0 < scratchValue4 then
                repeat
                    Copy(quest, this_00, scratchValue6)
                    scratchValue6 = scratchValue6 + 56
                    this_00 = this_00 + 56
                    scratchValue4 = scratchValue4 - 1
                until scratchValue4 == 0
            end
            -- TODO(native): pCVar1 = *(this + 4)
    --[[unresolved native value]]
            while this_00 ~= nil do
                -- TODO(native): (*(code *)**(undefined4 **)this_00)(0);
                this_00 = this_00 + 56
            end
        end
    end
    -- TODO(native): *(uint *)(this + 4) = uVar3 * 0x38 + *(int *)this;
end

-- Q_Arena.helper_F25D20 (retail 0x00f25d20)
function helper_F25D20(quest, param2)
    local scratchValue, this_00
    -- TODO(native): helper_F25D90(quest, (native_arg_param_2[1] - *native_arg_param_2) / 0x38, &native_arg_param_2)
    local scratchValue3 = param2[1]
    -- TODO(native): local this_00 = *this
    -- TODO(native): pCVar3 = *piVar2
    scratchValue = nil --[[unresolved native value]]
    while scratchValue ~= scratchValue3 do
        if this_00 ~= nil then
            COpinionDeedReactionDef(quest, this_00, scratchValue)
        end
        this_00 = this_00 + 56
        scratchValue = scratchValue + 56
    end
    -- TODO(native): *(COpinionDeedReactionDef **)(this + 4) = this_00;
end

-- Q_Arena.helper_F26150 (retail 0x00f26150)
function helper_F26150(quest, param2, param, param4)
    local scratchValue2
    if param2 == 0 then
        scratchValue2 = 0
    else
        scratchValue2 = malloc(param2 * 56)
    end
    if param ~= param4 then
        local scratchValue = scratchValue2 - param
        repeat
            if param + scratchValue ~= nil then
                COpinionDeedReactionDef(quest, param + scratchValue, param)
            end
            param = param + 56
        until param == param4
    end
    return scratchValue2
end

-- Q_Arena.helper_F26110 (retail 0x00f26110)
function helper_F26110(quest, this)
    local scratchValue, scratchValue2
    -- TODO(native): local puVar1 = *(native_arg_this + 4)
    -- TODO(native): puVar2 = *native_arg_this
    scratchValue2 = nil --[[unresolved native value]]
    while scratchValue2 ~= scratchValue do
        -- TODO(native): (**(code **)*puVar2)(0);
        scratchValue2 = scratchValue2 + 14
    end
    -- TODO(native): if *native_arg_this ~= nil then
end

-- Q_Arena.Copy (retail 0x00f25f10)
-- F25F10: bsim names this body CArenaCreatureDef::Copy (a homologous script member); no PDB name
function Copy(quest, param1)
    -- TODO(native): CThingBuildingDef::operator=((CThingBuildingDef *)this,(int)native_arg_param_1);
    -- TODO(native): CCharString::operator=((CCharString *)(this + 0x28),(CCharString *)(native_arg_param_1 + 0x28));
    -- TODO(native): name field 0x2c (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x2c", *(native_arg_param_1 + 0x2c))
    -- TODO(native): CCharString::operator=((CCharString *)(this + 0x30),(CCharString *)(native_arg_param_1 + 0x30));
    -- TODO(native): name field 0x34 (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x34", *(native_arg_param_1 + 0x34))
end

-- Q_Arena.COpinionDeedReactionDef (retail 0x00f25dd0)
-- F25DD0: bsim names this body COpinionDeedReactionDef::COpinionDeedReactionDef (a homologous script member); no PDB name
function COpinionDeedReactionDef(quest, param1)
    -- TODO(native): CDefClassBase::CDefClassBase((CDefClassBase *)this,(int)native_arg_param_1);
    -- TODO(native): CCharString::CCharString((CCharString *)(this + 0x28),(CCharString *)(native_arg_param_1 + 0x28));
    -- TODO(native): name field 0x2c (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x2c", *(native_arg_param_1 + 0x2c))
    -- TODO(native): CCharString::CCharString((CCharString *)(this + 0x30),(CCharString *)(native_arg_param_1 + 0x30));
    -- TODO(native): name field 0x34 (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x34", *(native_arg_param_1 + 0x34))
end

-- Q_Arena.helper_F25D90 (retail 0x00f25d90)
function helper_F25D90(quest, param2)
    -- TODO(native): *(undefined4 *)this = 0;
    -- TODO(native): *(undefined4 *)(this + 4) = 0;
    -- TODO(native): *(undefined4 *)(this + 8) = 0;
    if param2 ~= 0 then
        malloc(param2 * 56)
    end
    -- TODO(native): *(void **)(this + 8) = (void *)(native_arg_param_2 * 0x38 + (int)pvVar1);
    -- TODO(native): *(void **)this = pvVar1;
    -- TODO(native): *(void **)(this + 4) = pvVar1;
end

