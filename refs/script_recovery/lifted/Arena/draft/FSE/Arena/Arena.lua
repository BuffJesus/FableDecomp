-- Generated native draft: Q_Arena. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local angle, b2, bVar16, bVar21, cVar2, c_stk_23d, iVar22, iVar6, i_stk_2b8, native_arg_switch_3, native_arg_switch_4, pCVar10, pCVar13, pCVar3, pCVar7, pCVar8, pOther, pcVar23, pppuVar19, r1, r10, r11, r12, r13, r14, r15, r16, r2, r3, r4, r5, r6, r7, r8, r9, this_00, uVar11, uVar20, xStack_1d8, xStack_1e8, xStack_1f8, xStack_220, xStack_22c, xStack_250, xStack_25c, xStack_268, xStack_26c, xStack_27c, xStack_28c, xStack_298, xStack_2ac, xStack_40, xStack_50
    local alive = true
    pCVar3 = quest:GetActiveQuestName()
    quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_ARENA_OBJECTIVE_01", "Arena", "KnotholeGlade")
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
    bVar16 = quest:IsQuestActive("Q_ArenaHoldingScript")
    c_stk_23d = (1 - (bVar16 and 1 or 0))
    if c_stk_23d ~= 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then
            return
        end
        quest:ActivateQuest("Q_ArenaHoldingScript")
    end
    quest:CreateThread("WatchForTermination")  -- native thread body Script_Arena_Teleport_Thread: lift it as function WatchForTermination(quest)
    if not bVar16 then
    end
    quest:CreateThread("CrowdChecker")  -- native thread body CQ_ArenaScript__CrowdChecker: lift it as function CrowdChecker(quest)
    iVar6 = quest:GetHeroTitle()
    if iVar6 == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then
            return
        end
        quest:GiveHeroTitle("OBJECT_HERO_TITLE_CHICKEN_CHASER")
    end
    if quest:GetStateInt("ArenaState") == 0 then
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then
            return
        end
        bVar16 = quest:IsLevelLoaded("ArenaHallOfHeroes")
        while not bVar16 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then
                return
            end
            bVar16 = quest:IsLevelLoaded("ArenaHallOfHeroes")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then
            return
        end
        quest:SetStateInt("ArenaState", 1)
    end
    quest:SetTimeOfDay(12.0)
    quest:SetTimeAsStopped(true)
    quest:SetTeleportingAsActive(false)
    if quest:GetStateInt("ArenaState") == 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then
            return
        end
        iVar22 = 4
        xStack_27c = resources:NewResource()
        pppuVar19 = xStack_27c
        pCVar7 = quest:GetHero()
        resources:TryAcquire(pppuVar19, pCVar7, iVar22)
        uVar20 = false
        pCVar7 = quest:GetThingWithScriptName("Q_Nav1")
        pCVar8 = quest:GetHero()
        quest:EntityTeleportToThing(pCVar8, pCVar7, uVar20)
        xStack_298 = resources:NewActorMap()
        resources:SetActor(xStack_298, "Hero", xStack_27c)
        xStack_28c = resources:StartMovie("")
        resources:RunMacro("CS_ARENA_HOH_INTRO", xStack_298, false, true)
        resources:DestroyMovie(xStack_28c)
        resources:DestroyActorMap(xStack_298)
        resources:ReleaseResource(xStack_27c)
        uVar20 = false
        pCVar7 = quest:GetThingWithScriptName("ArenaCellEntrance")
        pCVar8 = quest:GetHero()
        quest:EntityTeleportToThing(pCVar8, pCVar7, uVar20)
        bVar16 = quest:IsLevelLoaded("ArenaCells")
        while not bVar16 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then
                return
            end
            bVar16 = quest:IsLevelLoaded("ArenaCells")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then
            return
        end
        bVar16 = false
        pCVar7 = quest:GetThingWithScriptName("CellsExitToArena")
        quest:SetThingAsUsable(pCVar7, bVar16)
        bVar16 = false
        pCVar7 = quest:GetThingWithScriptName("CellsExitToHOH")
        quest:SetThingAsUsable(pCVar7, bVar16)
        bVar16 = true
        pCVar7 = quest:GetThingWithScriptName("CellsExitToArena")
        quest:SetThingPersistent(pCVar7, bVar16)
        bVar16 = true
        pCVar7 = quest:GetThingWithScriptName("CellsExitToHOH")
        quest:SetThingPersistent(pCVar7, bVar16)
        quest:FadeScreenIn()
        quest:SetStateInt("ArenaState", 2)
    end
    if quest:GetStateInt("ArenaState") == 2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then
            return
        end
        r1 = quest:GetThingWithScriptName("Roth")
        xStack_220 = resources:NewResource()
        resources:PrepareResource(xStack_220)
        bVar16 = resources:TryAcquire(xStack_220, r1, 4)
        while not bVar16 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f10d3a end
            bVar16 = resources:TryAcquire(xStack_220, r1, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then goto LAB_00f10d3a end
        r2 = quest:GetThingWithScriptName("Flick")
        xStack_1f8 = resources:NewResource()
        resources:PrepareResource(xStack_1f8)
        bVar16 = resources:TryAcquire(xStack_1f8, r2, 4)
        while not bVar16 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f10d22 end
            bVar16 = resources:TryAcquire(xStack_1f8, r2, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if not bVar16 then
            r3 = quest:GetThingWithScriptName("Needle")
            xStack_28c = resources:NewResource()
            resources:PrepareResource(xStack_28c)
            bVar16 = resources:TryAcquire(xStack_28c, r3, 4)
            while not bVar16 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then goto LAB_00f10d0d end
                bVar16 = resources:TryAcquire(xStack_28c, r3, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if not bVar16 then
                r4 = quest:GetThingWithScriptName("Cham")
                xStack_27c = resources:NewResource()
                resources:PrepareResource(xStack_27c)
                bVar16 = resources:TryAcquire(xStack_27c, r4, 4)
                while not bVar16 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if bVar16 then goto LAB_00f10cf8 end
                    bVar16 = resources:TryAcquire(xStack_27c, r4, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if not bVar16 then
                    r5 = quest:GetThingWithScriptName("Shadow")
                    xStack_250 = resources:NewResource()
                    resources:PrepareResource(xStack_250)
                    bVar16 = resources:TryAcquire(xStack_250, r5, 4)
                    while not bVar16 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar16 = not alive
                        if bVar16 then goto LAB_00f10ce3 end
                        bVar16 = resources:TryAcquire(xStack_250, r5, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if not bVar16 then
                        r6 = quest:GetThingWithScriptName("ArenaCellDoorGuard2")
                        xStack_1e8 = resources:NewResource()
                        resources:PrepareResource(xStack_1e8)
                        bVar16 = resources:TryAcquire(xStack_1e8, r6, 4)
                        while not bVar16 do
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar16 = not alive
                            if bVar16 then goto LAB_00f10cce end
                            bVar16 = resources:TryAcquire(xStack_1e8, r6, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar16 = not alive
                        if not bVar16 then
                            xStack_1d8 = resources:NewResource()
                            resources:PrepareResource(xStack_1d8)
                            iVar22 = 4
                            pCVar7 = xStack_1d8
                            pCVar8 = quest:GetHero()
                            bVar16 = resources:TryAcquire(pCVar7, pCVar8, iVar22)
                            while not bVar16 do
                                alive = quest:NewScriptFrame()
                                alive = not quest:IsActiveThreadTerminating()
                                bVar16 = not alive
                                if bVar16 then goto LAB_00f10cc2 end
                                iVar22 = 4
                                pCVar7 = xStack_1d8
                                pCVar8 = quest:GetHero()
                                bVar16 = resources:TryAcquire(pCVar7, pCVar8, iVar22)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar16 = not alive
                            if not bVar16 then
                                xStack_25c = resources:NewActorMap()
                                resources:SetActor(xStack_25c, "Hero", xStack_1d8)
                                resources:SetActor(xStack_25c, "Roth", xStack_220)
                                resources:SetActor(xStack_25c, "Flick", xStack_1f8)
                                resources:SetActor(xStack_25c, "Cham", xStack_27c)
                                resources:SetActor(xStack_25c, "Guard", xStack_1e8)
                                xStack_40 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                resources:RunMacro("CS_ARENA_CELLS_INTRO", xStack_25c, false, true)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_40)
                                resources:DestroyActorMap(xStack_25c)
                                resources:ReleaseResource(xStack_1d8)
                                resources:ReleaseResource(xStack_1e8)
                                resources:ReleaseResource(xStack_250)
                                resources:ReleaseResource(xStack_27c)
                                resources:ReleaseResource(xStack_28c)
                                resources:ReleaseResource(xStack_1f8)
                                resources:ReleaseResource(xStack_220)
                                quest:SetStateBool("ChamLeaving", true)
                                quest:SetStateInt("ArenaState", 3)
                                goto LAB_00f10fa6
                            end
                            ::LAB_00f10cc2::
                            resources:ReleaseResource(xStack_1d8)
                        end
                        ::LAB_00f10cce::
                        resources:ReleaseResource(xStack_1e8)
                    end
                    ::LAB_00f10ce3::
                    resources:ReleaseResource(xStack_250)
                end
                ::LAB_00f10cf8::
                resources:ReleaseResource(xStack_27c)
            end
            ::LAB_00f10d0d::
            resources:ReleaseResource(xStack_28c)
        end
        ::LAB_00f10d22::
        resources:ReleaseResource(xStack_1f8)
        ::LAB_00f10d3a::
        resources:ReleaseResource(xStack_220)
        return
    end
    ::LAB_00f10fa6::
    iVar6 = quest:RegisterTimer()
    i_stk_2b8 = iVar6
    if quest:GetStateInt("ArenaState") == 3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then goto LAB_00f141c7 end
        pCVar3 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_ARENA_OBJECTIVE_02", "ArenaCells", "KnotholeGlade")
        quest:SetTimer(i_stk_2b8, 0x3c)
        iVar22 = quest:GetTimer(i_stk_2b8)
        while 0 < iVar22 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f141c7 end
            iVar22 = quest:GetTimer(i_stk_2b8)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then goto LAB_00f141c7 end
        bVar16 = quest:IsHeroControlledByPlayer()
        while not bVar16 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f141c7 end
            bVar16 = quest:IsHeroControlledByPlayer()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then goto LAB_00f141c7 end
        quest:SetStateInt("ArenaState", 4)
    end
    if quest:GetStateInt("ArenaState") == 4 then
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then goto LAB_00f141c7 end
        pCVar3 = quest:GetActiveQuestName()
        quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_ARENA_OBJECTIVE_03", "Arena", "KnotholeGlade")
        r7 = quest:GetThingWithScriptName("Roth")
        xStack_220 = resources:NewResource()
        resources:PrepareResource(xStack_220)
        bVar16 = resources:TryAcquire(xStack_220, r7, 4)
        while not bVar16 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f11602 end
            bVar16 = resources:TryAcquire(xStack_220, r7, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then goto LAB_00f11602 end
        r8 = quest:GetThingWithScriptName("Flick")
        xStack_27c = resources:NewResource()
        resources:PrepareResource(xStack_27c)
        bVar16 = resources:TryAcquire(xStack_27c, r8, 4)
        while not bVar16 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f115ed end
            bVar16 = resources:TryAcquire(xStack_27c, r8, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if not bVar16 then
            r9 = quest:GetThingWithScriptName("Needle")
            xStack_28c = resources:NewResource()
            resources:PrepareResource(xStack_28c)
            bVar16 = resources:TryAcquire(xStack_28c, r9, 4)
            while not bVar16 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then goto LAB_00f115d8 end
                bVar16 = resources:TryAcquire(xStack_28c, r9, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if not bVar16 then
                r10 = quest:GetThingWithScriptName("Shadow")
                xStack_250 = resources:NewResource()
                resources:PrepareResource(xStack_250)
                bVar16 = resources:TryAcquire(xStack_250, r10, 4)
                while not bVar16 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if bVar16 then goto LAB_00f115c3 end
                    bVar16 = resources:TryAcquire(xStack_250, r10, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if not bVar16 then
                    r11 = quest:GetThingWithScriptName("ArenaCellDoorGuard2")
                    xStack_1f8 = resources:NewResource()
                    resources:PrepareResource(xStack_1f8)
                    bVar16 = resources:TryAcquire(xStack_1f8, r11, 4)
                    while not bVar16 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar16 = not alive
                        if bVar16 then goto LAB_00f115ae end
                        bVar16 = resources:TryAcquire(xStack_1f8, r11, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if not bVar16 then
                        xStack_1e8 = resources:NewResource()
                        resources:PrepareResource(xStack_1e8)
                        iVar22 = 4
                        pCVar7 = xStack_1e8
                        pCVar8 = quest:GetHero()
                        bVar16 = resources:TryAcquire(pCVar7, pCVar8, iVar22)
                        while not bVar16 do
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar16 = not alive
                            if bVar16 then goto LAB_00f115a2 end
                            iVar22 = 4
                            pCVar7 = xStack_1e8
                            pCVar8 = quest:GetHero()
                            bVar16 = resources:TryAcquire(pCVar7, pCVar8, iVar22)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar16 = not alive
                        if not bVar16 then
                            xStack_25c = resources:NewActorMap()
                            resources:SetActor(xStack_25c, "Hero", xStack_1e8)
                            resources:SetActor(xStack_25c, "Roth", xStack_220)
                            resources:SetActor(xStack_25c, "Flick", xStack_27c)
                            resources:SetActor(xStack_25c, "Guard", xStack_1f8)
                            xStack_1d8 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            resources:RunMacro("CS_ARENA_CELLS_INTRO_CHAMDEAD", xStack_25c, false, true)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_1d8)
                            resources:DestroyActorMap(xStack_25c)
                            resources:ReleaseResource(xStack_1e8)
                            resources:ReleaseResource(xStack_1f8)
                            resources:ReleaseResource(xStack_250)
                            resources:ReleaseResource(xStack_28c)
                            resources:ReleaseResource(xStack_27c)
                            resources:ReleaseResource(xStack_220)
                            quest:SetStateInt("ArenaState", 5)
                            goto LAB_00f11828
                        end
                        ::LAB_00f115a2::
                        resources:ReleaseResource(xStack_1e8)
                    end
                    ::LAB_00f115ae::
                    resources:ReleaseResource(xStack_1f8)
                end
                ::LAB_00f115c3::
                resources:ReleaseResource(xStack_250)
            end
            ::LAB_00f115d8::
            resources:ReleaseResource(xStack_28c)
        end
        ::LAB_00f115ed::
        resources:ReleaseResource(xStack_27c)
        ::LAB_00f11602::
        resources:ReleaseResource(xStack_220)
        quest:DeregisterTimer(i_stk_2b8)
        return
    end
    ::LAB_00f11828::
    if quest:GetStateInt("ArenaState") == 5 then
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then goto LAB_00f141c7 end
        if quest:GetStateInt("ArenaRound") < 3 then
            pCVar7 = quest:GetThingWithScriptName("CellWhisper")
            bVar16 = (pCVar7 ~= nil and pCVar7:IsAlive())
            if bVar16 then
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then goto LAB_00f141c7 end
                bVar21 = true
                bVar16 = false
                pCVar7 = quest:GetThingWithScriptName("CellWhisper")
                quest:RemoveThing(pCVar7, bVar16, bVar21)
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f141c7 end
            pCVar7 = quest:GetThingWithScriptName("CellWhisper")
            bVar16 = (pCVar7 ~= nil and pCVar7:IsAlive())
            if not bVar16 then
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then goto LAB_00f141c7 end
                pCVar7 = quest:GetThingWithScriptName("FlickPoint")
                bVar16 = false
                pCVar10 = pCVar7:GetPos()
                r12 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", pCVar10, "CellWhisper")
                goto LAB_00f119fc
            end
        end
        ::LAB_00f119fc::
        iVar6 = quest:GetStateInt("ArenaState")
        while iVar6 ~= 6 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f141c7 end
            iVar6 = quest:GetStateInt("ArenaState")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then goto LAB_00f141c7 end
        uVar20 = false
        pCVar7 = quest:GetThingWithScriptName("ArenaInsideHSP")
        pCVar8 = quest:GetHero()
        quest:EntityTeleportToThing(pCVar8, pCVar7, uVar20)
        bVar16 = quest:IsLevelLoaded("Arena")
        while not bVar16 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f141c7 end
            bVar16 = quest:IsLevelLoaded("Arena")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then goto LAB_00f141c7 end
        quest:MiniMapSetAsEnabled(false)
        quest:SetStateBool("PlayerLeaving", false)
        r13 = quest:GetThingWithScriptName("ArenaHeroGate")
        quest:OpenDoor(r13)
        quest:SetThingPersistent(r13, true)
    end
    xStack_268 = resources:NewActorMap()
    if quest:GetStateInt("ArenaState") == 7 then
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if not bVar16 then
            if quest:GetStateInt("ArenaRound") < 3 then
                pCVar7 = quest:GetThingWithScriptName("CellWhisper")
                bVar16 = (pCVar7 ~= nil and pCVar7:IsAlive())
                if bVar16 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if bVar16 then goto LAB_00f141be end
                    bVar21 = true
                    bVar16 = false
                    pCVar7 = quest:GetThingWithScriptName("CellWhisper")
                    quest:RemoveThing(pCVar7, bVar16, bVar21)
                end
                goto FLOW_hoist_lab_00f11d57_1
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if not bVar16 then
                    pCVar7 = quest:GetThingWithScriptName("CellWhisper")
                    bVar16 = (pCVar7 ~= nil and pCVar7:IsAlive())
                    if bVar16 then goto LAB_00f11d5c end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if not bVar16 then
                        pCVar7 = quest:GetThingWithScriptName("FlickPoint")
                        bVar16 = false
                        pCVar10 = pCVar7:GetPos()
                        r14 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", pCVar10, "CellWhisper")
                        goto LAB_00f11d57
                    end
                end
            end
            goto FLOW_past_lab_00f11d57
            ::LAB_00f11d57::
            ::FLOW_hoist_lab_00f11d57_1::
            goto LAB_00f11d5c
            ::FLOW_past_lab_00f11d57::
            goto FLOW_past_lab_00f11d5c
            ::LAB_00f11d5c::
            iVar6 = quest:GetStateInt("ArenaState")
            while iVar6 ~= 6 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then goto LAB_00f141be end
                iVar6 = quest:GetStateInt("ArenaState")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if not bVar16 then
                quest:FadeScreenOut(0.5, 0.5)
                uVar20 = false
                pCVar7 = quest:GetThingWithScriptName("ArenaInsideHSP")
                pCVar8 = quest:GetHero()
                quest:EntityTeleportToThing(pCVar8, pCVar7, uVar20)
                bVar16 = quest:IsLevelLoaded("Arena")
                while not bVar16 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if bVar16 then goto LAB_00f141be end
                    bVar16 = quest:IsLevelLoaded("Arena")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if not bVar16 then
                    quest:MiniMapSetAsEnabled(false)
                    quest:SetStateInt("GoldMultiplier", 0)
                    quest:SetStateBool("PlayerLeaving", false)
                    goto LAB_00f11eec
                end
            end
            ::FLOW_past_lab_00f11d5c::
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if not bVar16 then
            quest:FadeScreenOut(0.5, 0.5)
            goto LAB_00f11eec
        end
    end
    goto FLOW_past_lab_00f11eec
    ::LAB_00f11eec::
    AnimateCrowd(quest)
    cVar2 = quest:GetStateBool("PlayerWon")
    while not cVar2 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then goto LAB_00f141be end
        cVar2 = quest:GetStateBool("PlayerLeaving")
        while (not cVar2 and (not quest:GetStateBool("PlayerWon"))) do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f141be end
            if quest:GetStateInt("ArenaRoundWave") ~= 0 then goto LAB_00f13472 end
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f141be end
            quest:AutoSaveCheckPoint()
            quest:SetStateBool("WhisperNeededForCutscene", true)
            bVar16 = false
            pCVar7 = quest:GetThingWithScriptName("ArenaMainExit")
            quest:SetThingAsUsable(pCVar7, bVar16)
            uVar20 = false
            pCVar7 = quest:GetThingWithScriptName("ARENA_CentrePoint")
            pCVar8 = quest:GetHero()
            quest:EntityTeleportToThing(pCVar8, pCVar7, uVar20)
            xStack_28c = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            xStack_27c = resources:NewResource()
            xStack_250 = resources:NewResource()
            resources:PrepareResource(xStack_27c)
            iVar22 = 4
            pppuVar19 = xStack_27c
            pCVar7 = quest:GetHero()
            bVar16 = resources:TryAcquire(pppuVar19, pCVar7, iVar22)
            while not bVar16 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then
                    resources:ReleaseResource(xStack_250)
                    resources:ReleaseResource(xStack_27c)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_28c)
                    resources:DestroyActorMap(xStack_268)
                    quest:DeregisterTimer(i_stk_2b8)
                    return
                end
                iVar22 = 4
                pppuVar19 = xStack_27c
                pCVar7 = quest:GetHero()
                bVar16 = resources:TryAcquire(pppuVar19, pCVar7, iVar22)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f13e6c end
            uVar11 = math.random(0, 32767)
            uVar11 = uVar11 & 0x80000003
            if uVar11 < 0 then
                uVar11 = (uVar11 - 1 | 0xfffffffc) + 1
            end
            if uVar11 ~= 1 then
                if uVar11 ~= 2 then goto LAB_00f121a5 end
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if not bVar16 then
                    pcVar23 = "CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC3"
                    goto LAB_00f121a0
                end
                goto LAB_00f13e5e
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f13e6c end
            pcVar23 = "CS_ARENA_ROUND_BEGIN_WHISPER_GENERIC2"
            ::LAB_00f121a0::
            xStack_26c = pcVar23
            ::LAB_00f121a5::
            xStack_22c = resources:NewActorMap()
            resources:SetActor(xStack_22c, "Hero", xStack_27c)
            xStack_2ac = resources:NewStringMap()
            pOther = "NULL"
            resources:SetString(xStack_2ac, "$THEME", pOther)
            iVar6 = quest:GetHeroTitle()
            if quest:GetStateInt("ArenaRound") < 3 then goto LAB_00f123aa end
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f13e9c end
            this_00 = quest:GetStateThing("Whisper")
            pCVar7 = quest:GetThingWithScriptName("WhisperAlly")
            -- TODO(native): CScriptThing::operator=(this_00,(int)pCVar7);
            cVar2 = (this_00 ~= nil and this_00:IsAlive())
            if not cVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if not bVar16 then
                    pCVar7 = quest:GetThingWithScriptName("ARENA_WhisperStartPoint")
                    bVar16 = false
                    pCVar10 = pCVar7:GetPos()
                    pCVar7 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", pCVar10, "WhisperAlly")
                    -- TODO(native): CScriptThing::operator=(this_00,(int)pCVar7);
                    goto LAB_00f123aa
                end
                goto LAB_00f13e45
            end
            ::LAB_00f123aa::
            native_arg_switch_3 = quest:GetStateInt("ArenaRound")
            repeat
                if native_arg_switch_3 == 0 then
                    pCVar3 = quest:GetActiveQuestName()
                    quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_01", "Arena", "KnotholeGlade")
                    quest:SetStateInt("NewCrowdBaseLevel", 0)
                    native_arg_switch_4 = iVar6
                    repeat
                        if native_arg_switch_4 == 1 then
                            pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_17"
                            resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                            break
                        else
                            if native_arg_switch_4 == 2 then
                                pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_20"
                                resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                break
                            else
                                if native_arg_switch_4 == 3 then
                                    pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_11"
                                    resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                    break
                                else
                                    if native_arg_switch_4 == 4 then
                                        pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_06"
                                        resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                        break
                                    else
                                        if native_arg_switch_4 == 5 then
                                            pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_03"
                                            resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                            break
                                        else
                                            if native_arg_switch_4 == 6 then
                                                pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_12"
                                                resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                break
                                            else
                                                if native_arg_switch_4 == 7 then
                                                    pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_04"
                                                    resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                    break
                                                else
                                                    if native_arg_switch_4 == 8 then
                                                        pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_15"
                                                        resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                        break
                                                    else
                                                        if native_arg_switch_4 == 9 then
                                                            pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_10"
                                                            resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                            break
                                                        else
                                                            if native_arg_switch_4 == 10 then
                                                                pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_13"
                                                                resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                                break
                                                            else
                                                                if native_arg_switch_4 == 0xb then
                                                                    pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_07"
                                                                    resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                                    break
                                                                else
                                                                    if native_arg_switch_4 == 0xc then
                                                                        pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_16"
                                                                        resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                                        break
                                                                    else
                                                                        if native_arg_switch_4 == 0xd then
                                                                            pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_18"
                                                                            resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                                            break
                                                                        else
                                                                            if native_arg_switch_4 == 0xe then
                                                                                pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_09"
                                                                                resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                                                break
                                                                            else
                                                                                if native_arg_switch_4 == 0xf then
                                                                                    pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_08"
                                                                                    resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                                                    break
                                                                                else
                                                                                    if native_arg_switch_4 == 0x10 then
                                                                                        pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_19"
                                                                                        resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                                                        break
                                                                                    else
                                                                                        if native_arg_switch_4 == 0x11 then
                                                                                            pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_01"
                                                                                            resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                                                            break
                                                                                        else
                                                                                            if native_arg_switch_4 == 0x12 then
                                                                                                pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_14"
                                                                                                resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                                                                break
                                                                                            else
                                                                                                if native_arg_switch_4 == 0x13 then
                                                                                                    pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_05"
                                                                                                    resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                                                                    break
                                                                                                else
                                                                                                    if native_arg_switch_4 == 0x14 then
                                                                                                        pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ANNOUNCEMENT_NAME_02"
                                                                                                        resources:SetString(xStack_2ac, "$HEROTITLE", pcVar23)
                                                                                                        break
                                                                                                    else
                                                                                                        goto FLOW_native_label_1
                                                                                                    end
                                                                                                end
                                                                                            end
                                                                                        end
                                                                                    end
                                                                                end
                                                                            end
                                                                        end
                                                                    end
                                                                end
                                                            end
                                                        end
                                                    end
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    until not (false)
                    ::FLOW_native_label_1::
                    xStack_26c = "CS_ARENA_ROUND_1"
                    b2 = false
                    uVar20 = false
                    iVar22 = math.random(0, 32767)
                    iVar22 = GetFanfareMusic(quest, iVar22 % 10)
                    quest:OverrideMusic(iVar22, uVar20, b2)
                    goto FLOW_native_label_2
                else
                    if native_arg_switch_3 == 1 then
                        pCVar3 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_02", "Arena", "KnotholeGlade")
                        quest:SetStateInt("NewCrowdBaseLevel", 0)
                        pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ROUND2_10"
                        resources:SetString(xStack_2ac, "$SAY1", pcVar23)
                        pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ROUND2_20"
                        resources:SetString(xStack_2ac, "$SAY2", pcVar23)
                        xStack_26c = "CS_ARENA_ROUND_BEGIN_GENERIC"
                        goto FLOW_native_label_2
                    else
                        if native_arg_switch_3 == 2 then
                            pCVar3 = quest:GetActiveQuestName()
                            quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_03", "Arena", "KnotholeGlade")
                            quest:SetStateInt("NewCrowdBaseLevel", 1)
                            xStack_26c = "CS_ARENA_ROUND_WHISPER_INTRO"
                            pcVar23 = "ENVIRONMENT_WITCHWOOD"
                            resources:SetString(xStack_2ac, "$THEME", pcVar23)
                            break
                        else
                            if native_arg_switch_3 == 3 then
                                pCVar3 = quest:GetActiveQuestName()
                                quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_04", "Arena", "KnotholeGlade")
                                quest:SetStateInt("NewCrowdBaseLevel", 1)
                                pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ROUND3_10"
                                resources:SetString(xStack_2ac, "$SAY1", pcVar23)
                                pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ROUND3_20"
                                resources:SetString(xStack_2ac, "$SAY2", pcVar23)
                                pcVar23 = "ENVIRONMENT_HAUNTED"
                                resources:SetString(xStack_2ac, "$THEME", pcVar23)
                                break
                            else
                                if native_arg_switch_3 == 4 then
                                    pCVar3 = quest:GetActiveQuestName()
                                    quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_05", "Arena", "KnotholeGlade")
                                    quest:SetStateInt("NewCrowdBaseLevel", 2)
                                    pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ROUND4_10"
                                    resources:SetString(xStack_2ac, "$SAY1", pcVar23)
                                    pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ROUND4_20"
                                    resources:SetString(xStack_2ac, "$SAY2", pcVar23)
                                    pcVar23 = "RESET"
                                    resources:SetString(xStack_2ac, "$THEME", pcVar23)
                                    break
                                else
                                    if native_arg_switch_3 == 5 then
                                        pCVar3 = quest:GetActiveQuestName()
                                        quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_06", "Arena", "KnotholeGlade")
                                        quest:SetStateInt("NewCrowdBaseLevel", 2)
                                        pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ROUND5_10"
                                        resources:SetString(xStack_2ac, "$SAY1", pcVar23)
                                        pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ROUND5_20"
                                        resources:SetString(xStack_2ac, "$SAY2", pcVar23)
                                        break
                                    else
                                        if native_arg_switch_3 == 6 then
                                            pCVar3 = quest:GetActiveQuestName()
                                            quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_07", "Arena", "KnotholeGlade")
                                            quest:SetStateInt("NewCrowdBaseLevel", 3)
                                            pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ROUND6_10"
                                            resources:SetString(xStack_2ac, "$SAY1", pcVar23)
                                            pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ROUND6_20"
                                            resources:SetString(xStack_2ac, "$SAY2", pcVar23)
                                            break
                                        else
                                            if native_arg_switch_3 == 7 then
                                                pCVar3 = quest:GetActiveQuestName()
                                                quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_08", "Arena", "KnotholeGlade")
                                                quest:SetStateInt("NewCrowdBaseLevel", 3)
                                                pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ROUND7_10"
                                                resources:SetString(xStack_2ac, "$SAY1", pcVar23)
                                                pcVar23 = "TEXT_QST_005_V2_ARENA_KEEPER_ROUND7_20"
                                                resources:SetString(xStack_2ac, "$SAY2", pcVar23)
                                                pcVar23 = "ENVIRONMENT_EOW_04"
                                                resources:SetString(xStack_2ac, "$THEME", pcVar23)
                                                quest:OverrideMusic(0x17, false, false)
                                                goto FLOW_native_label_2
                                            else
                                                if native_arg_switch_3 == 8 then
                                                    pCVar3 = quest:GetActiveQuestName()
                                                    quest:SetQuestCardObjective(pCVar3, "TEXT_QUEST_ARENA_OBJECTIVE_04_ROUND_09", "Arena", "KnotholeGlade")
                                                    quest:SetStateInt("NewCrowdBaseLevel", 3)
                                                    quest:SetStateInt("ArenaState", 8)
                                                    xStack_26c = "CS_ARENA_ROUND_BOTH_WINNERS"
                                                    pcVar23 = "RESET"
                                                    resources:SetString(xStack_2ac, "$THEME", pcVar23)
                                                    break
                                                else
                                                    goto FLOW_native_label_2
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            until not (false)
            ::FLOW_native_label_2::
            cVar2 = quest:GetStateThing("Whisper"):IsAlive()
            if cVar2 then
                resources:PrepareResource(xStack_250)
                bVar16 = resources:TryAcquire(xStack_250, quest:GetStateThing("Whisper"), 4)
                while not bVar16 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if bVar16 then goto LAB_00f13e9c end
                    bVar16 = resources:TryAcquire(xStack_250, quest:GetStateThing("Whisper"), 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then
                    goto LAB_00f13e45
                end
                goto FLOW_hoist_lab_00f13e45_1
            end
            goto FLOW_past_lab_00f13e45
            ::LAB_00f13e45::
            resources:DestroyStringMap(xStack_2ac)
            resources:DestroyActorMap(xStack_22c)
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
            resources:ReleaseResource(xStack_250)
            resources:ReleaseResource(xStack_27c)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_28c)
            goto LAB_00f141be
            ::FLOW_hoist_lab_00f13e6c_1::
            resources:SetActor(xStack_22c, "Whisper", xStack_250)
            ::FLOW_past_lab_00f13e6c::
            resources:RunMacroWithStrings(xStack_26c, xStack_22c, xStack_2ac, false, true)
            quest:StopOverrideMusic(false)
            if xStack_26c == nil then
                bVar16 = false
                if bVar16 then
                    goto LAB_00f13403
                end
            else
                iVar6 = ((xStack_26c == "CS_ARENA_ROUND_BOTH_WINNERS") and 0 or 1)
                c_stk_23d = (not (iVar6 ~= 0)) and 1 or 0
                if c_stk_23d ~= 0 then goto LAB_00f13403 end
            end
            goto FLOW_past_lab_00f13403
            ::LAB_00f13403::
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then
                goto LAB_00f13e9c
            end
            goto FLOW_hoist_lab_00f13e9c_1
            ::FLOW_past_lab_00f13403::
            goto FLOW_past_lab_00f13e9c
            ::LAB_00f13e9c::
            resources:DestroyStringMap(xStack_2ac)
            resources:DestroyActorMap(xStack_22c)
            -- LAB_00f13eb5: (native jump target)
            resources:ReleaseResource(xStack_250)
            resources:ReleaseResource(xStack_27c)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_28c)
            goto LAB_00f141be
            ::FLOW_hoist_lab_00f13e9c_1::
            quest:SetStateBool("FinalBattleCS", true)
            ::FLOW_past_lab_00f13e9c::
            quest:SetStateBool("WhisperNeededForCutscene", false)
            resources:DestroyStringMap(xStack_2ac)
            resources:DestroyActorMap(xStack_22c)
            resources:ReleaseResource(xStack_250)
            resources:ReleaseResource(xStack_27c)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_28c)
            ::LAB_00f13472::
            if not quest:GetStateBool("PlayerWon") then
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then goto LAB_00f141be end
                if quest:GetStateInt("ArenaState") == 8 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if bVar16 then goto LAB_00f141be end
                    bVar16 = quest:IsLevelLoaded("ArenaCells")
                    while not bVar16 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar16 = not alive
                        if bVar16 then goto LAB_00f141be end
                        bVar16 = quest:IsLevelLoaded("ArenaCells")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if bVar16 then goto LAB_00f141be end
                    pCVar7 = quest:GetThingWithScriptName("CellWhisper")
                    bVar16 = (pCVar7 ~= nil and pCVar7:IsAlive())
                    if bVar16 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar16 = not alive
                        if bVar16 then goto LAB_00f141be end
                        bVar21 = true
                        bVar16 = false
                        pCVar7 = quest:GetThingWithScriptName("CellWhisper")
                        quest:RemoveThing(pCVar7, bVar16, bVar21)
                    end
                    quest:SetStateBool("PlayerWon", true)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if bVar16 then goto LAB_00f141be end
                    PlayWave(quest)
                end
            end
            cVar2 = quest:GetStateBool("PlayerLeaving")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if bVar16 then goto LAB_00f141be end
        if quest:GetStateBool("PlayerLeaving") then
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f141be end
            xStack_50 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FadeScreenOut(0.5, 0.5)
            quest:Pause(0.5)
            quest:ResetToDefaultTheme(0.0)
            uVar20 = false
            pCVar7 = quest:GetThingWithScriptName("ArenaCellEntrance2")
            pCVar8 = quest:GetHero()
            quest:EntityTeleportToThing(pCVar8, pCVar7, uVar20)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_50)
            bVar16 = quest:IsLevelLoaded("ArenaCells")
            while not bVar16 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then goto LAB_00f141be end
                bVar16 = quest:IsLevelLoaded("ArenaCells")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f141be end
            quest:FadeScreenIn()
            quest:MiniMapSetAsEnabled(true)
            quest:SetStateInt("ArenaState", 7)
            if quest:GetStateInt("ArenaRound") < 3 then
                pCVar7 = quest:GetThingWithScriptName("CellWhisper")
                bVar16 = (pCVar7 ~= nil and pCVar7:IsAlive())
                if bVar16 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if not bVar16 then
                        bVar21 = true
                        bVar16 = false
                        pCVar7 = quest:GetThingWithScriptName("CellWhisper")
                        quest:RemoveThing(pCVar7, bVar16, bVar21)
                        goto LAB_00f13a09
                    end
                    goto LAB_00f141be
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then goto LAB_00f141be end
                pCVar7 = quest:GetThingWithScriptName("CellWhisper")
                bVar16 = (pCVar7 ~= nil and pCVar7:IsAlive())
                if not bVar16 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if bVar16 then goto LAB_00f141be end
                    pCVar7 = quest:GetThingWithScriptName("FlickPoint")
                    bVar16 = false
                    pCVar10 = pCVar7:GetPos()
                    r15 = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_ARENA", pCVar10, "CellWhisper")
                end
            end
            ::LAB_00f13a09::
            quest:AutoSaveCheckPoint()
            iVar6 = quest:GetStateInt("ArenaState")
            while iVar6 ~= 6 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then goto LAB_00f141be end
                iVar6 = quest:GetStateInt("ArenaState")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f141be end
            uVar20 = false
            pCVar7 = quest:GetThingWithScriptName("ArenaInsideHSP")
            pCVar8 = quest:GetHero()
            quest:EntityTeleportToThing(pCVar8, pCVar7, uVar20)
            quest:FadeScreenOut(0.5, 0.5)
            bVar16 = quest:IsLevelLoaded("Arena")
            while not bVar16 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then goto LAB_00f141be end
                bVar16 = quest:IsLevelLoaded("Arena")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f141be end
            AnimateCrowd(quest)
            quest:MiniMapSetAsEnabled(false)
            quest:SetStateInt("GoldMultiplier", 0)
        end
        cVar2 = quest:GetStateBool("PlayerWon")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar16 = not alive
    if not bVar16 then
        xStack_250 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        xStack_27c = resources:NewResource()
        xStack_28c = resources:NewResource()
        resources:PrepareResource(xStack_27c)
        iVar22 = 4
        pppuVar19 = xStack_27c
        pCVar7 = quest:GetHero()
        bVar16 = resources:TryAcquire(pppuVar19, pCVar7, iVar22)
        while not bVar16 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if bVar16 then goto LAB_00f13f09 end
            iVar22 = 4
            pppuVar19 = xStack_27c
            pCVar7 = quest:GetHero()
            bVar16 = resources:TryAcquire(pppuVar19, pCVar7, iVar22)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar16 = not alive
        if not bVar16 then
            resources:PrepareResource(xStack_28c)
            iVar22 = 4
            pppuVar19 = xStack_28c
            pCVar7 = quest:GetThingWithScriptName("Roth")
            bVar16 = resources:TryAcquire(pppuVar19, pCVar7, iVar22)
            while not bVar16 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if bVar16 then goto LAB_00f13f09 end
                iVar22 = 4
                pppuVar19 = xStack_28c
                pCVar7 = quest:GetThingWithScriptName("Roth")
                bVar16 = resources:TryAcquire(pppuVar19, pCVar7, iVar22)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar16 = not alive
            if not bVar16 then
                xStack_298 = resources:NewActorMap()
                resources:SetActor(xStack_298, "Hero", xStack_27c)
                resources:SetActor(xStack_298, "Roth", xStack_28c)
                pCVar13 = helper_F14250(quest)
                if pCVar13[0x74] == 0x1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if bVar16 then
                        goto LAB_00f13efc
                    end
                    goto FLOW_hoist_lab_00f13efc_1
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if bVar16 then goto LAB_00f13efc end
                    pcVar23 = "CS_ARENA_CELLS_LADYGREY_ONE"
                end
                goto FLOW_past_lab_00f13efc
                ::LAB_00f13efc::
                resources:DestroyActorMap(xStack_298)
                goto LAB_00f13f09
                ::FLOW_hoist_lab_00f13efc_1::
                pcVar23 = "CS_ARENA_CELLS_LADYGREY_TWO"
                ::FLOW_past_lab_00f13efc::
                resources:RunMacro(pcVar23, xStack_298, false, true)
                resources:DestroyActorMap(xStack_298)
                resources:ReleaseResource(xStack_28c)
                resources:ReleaseResource(xStack_27c)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_250)
                r16 = quest:GetThingWithScriptName("CellsToHallOfHeroesEntrance")
                bVar16 = false
                pCVar7 = r16
                pCVar8 = quest:GetHero()
                quest:EntityTeleportToThing(pCVar8, pCVar7, bVar16)
                uVar20 = true
                angle = r16:GetAngleXY()
                pCVar7 = quest:GetHero()
                quest:EntitySetFacingAngle(pCVar7, angle, uVar20)
                bVar16 = quest:IsLevelLoaded("ArenaHallOfHeroes")
                while not bVar16 do
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar16 = not alive
                    if bVar16 then goto LAB_00f141b5 end
                    bVar16 = quest:IsLevelLoaded("ArenaHallOfHeroes")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar16 = not alive
                if not bVar16 then
                    xStack_28c = resources:NewResource()
                    iVar22 = 4
                    pppuVar19 = xStack_28c
                    pCVar7 = quest:GetHero()
                    resources:TryAcquire(pppuVar19, pCVar7, iVar22)
                    xStack_25c = resources:NewActorMap()
                    resources:SetActor(xStack_25c, "Hero", xStack_28c)
                    xStack_50 = resources:StartMovie("")
                    resources:RunMacro("CS_ARENA_HOH_OUTRO", xStack_25c, false, true)
                    resources:DestroyMovie(xStack_50)
                    resources:DestroyActorMap(xStack_25c)
                    quest:FadeScreenOut(0.5, 0.0)
                    quest:ReturnAllConfiscatedItemsToHero()
                    quest:MiniMapSetAsEnabled(true)
                    quest:SetStateBool("MissionSucceeded", true)
                    resources:ReleaseResource(xStack_28c)
                end
                ::LAB_00f141b5::
                goto LAB_00f141be
            end
        end
        ::LAB_00f13f09::
        resources:ReleaseResource(xStack_28c)
        resources:ReleaseResource(xStack_27c)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_250)
    end
    ::FLOW_past_lab_00f11eec::
    ::LAB_00f141be::
    resources:DestroyActorMap(xStack_268)
    ::LAB_00f141c7::
    quest:DeregisterTimer(i_stk_2b8)
end

function Init(quest)
    quest:SetStateBool("ArenaSpawnNeeded_10", true)  -- native constructor: initial value
    quest:SetStateInt("GlobalCrowdTimer", quest:RegisterTimer())  -- native constructor: CTimer member
    local bVar1, iVar2
    local alive = true
    quest:AddQuestRegion("Q_Arena", "ArenaExterior")
    quest:AddQuestRegion("Q_Arena", "ArenaHallOfHeroes")
    quest:AddQuestRegion("Q_Arena", "ArenaCells")
    quest:AddQuestRegion("Q_Arena", "Arena")
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, quest:ReadGlobalGameData(0x70), quest:ReadGlobalGameData(0x74), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, quest:ReadGlobalGameData(0x78), quest:ReadGlobalGameData(0x7c), false, "", 0)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_KILLARENAOPPONENTS", 0x1e, quest:ReadGlobalGameData(0xadc), quest:ReadGlobalGameData(0xae4), true, "", 1)
    quest:AddBoast("TEXT_QST_BOAST_DESCRIPTION_SPAREARENAOPPONENTS", 0x1f, quest:ReadGlobalGameData(0xae0), quest:ReadGlobalGameData(0xae8), false, "", 1)
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
    iVar2 = 0
    repeat
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        -- TODO(native): this[iVar2 + 0xe2] = (CQ_ArenaScript)0x0;
        iVar2 = iVar2 + 1
    until not (iVar2 < 0x10)
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        quest:SetStateBool("FinalBattleCS", false)
        quest:SetStateInt("ArenaState", 0)
        quest:SetStateBool("WhisperNeededForCutscene", true)
    end
end

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

function WatchForTermination(quest)
    local bVar4, pCVar5, pTargetThing, pThingToMove, r1
    local alive = true
    local cVar1 = quest:GetStateBool("MissionFailed")
    while (not cVar1 and (not quest:GetStateBool("MissionSucceeded"))) do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        cVar1 = quest:GetStateBool("MissionFailed")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        quest:SetTimeAsStopped(false)
        if not quest:GetStateBool("MissionFailed") then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
            quest:GiveHeroGold(quest:GetStateInt("GoldTotal"))
            if not quest:GetMasterGameState("WhisperKilledByHero") then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
                quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xaec))
            end
            r1 = quest:GetThingWithScriptName("ArenaExtEntrance")
            quest:SetAllowScreenFadingOnNextRegionChange(false)
            bVar4 = false
            pTargetThing = r1
            pThingToMove = quest:GetHero()
            quest:EntityTeleportToThing(pThingToMove, pTargetThing, bVar4)
            bVar4 = quest:IsRegionLoaded("ArenaExterior")
            while not bVar4 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    return
                end
                bVar4 = quest:IsRegionLoaded("ArenaExterior")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                -- LAB_00f144b0: (native jump target)
                return
            end
            bVar4 = true
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar5, bVar4, false, false)
            quest:FadeScreenIn()
            quest:SetAllowScreenFadingOnNextRegionChange(true)
            quest:SetMasterGameState("ArenaFinished", true)
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                return
            end
        end
        quest:SetTeleportingAsActive(true)
        pCVar5 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pCVar5, 0)
    end
end

function CrowdChecker(quest)
    local aiStack_c, bVar2, c_stk_151, elem_1, iStack_138, iVar3, iVar4, i_stk_114, i_stk_158, i_stk_15c, i_stk_f4, i_stk_fc, native_arg_sequence_1, native_arg_switch_2, native_arg_switch_3, native_arg_switch_4, pCVar6, pPos, puVar1, puVar8, pu_stk_14c, r1, r2, uVar5, uVar9, xStack_150, xStack_c0
    local alive = true
    -- TODO(native): aiStack_c[0] = 0;
    -- TODO(native): aiStack_c[1] = 200;
    -- TODO(native): aiStack_c[2] = 400;
    local function __cleanup_LAB_00f1e552()
        quest:DeregisterTimer(i_stk_15c)
        quest:DeregisterTimer(i_stk_158)
    end
    iVar3 = quest:RegisterTimer()
    i_stk_158 = iVar3
    quest:SetTimer(i_stk_158, 0)
    i_stk_15c = quest:RegisterTimer()
    quest:SetTimer(i_stk_15c, 0xf)
    i_stk_114 = -1
    i_stk_f4 = -1
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    while true do
        pu_stk_14c = puVar8
        if not (not bVar2) then break end
        bVar2 = quest:IsLevelLoaded("ArenaCells")
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                puVar8 = 0
                if bVar2 then goto LAB_00f1e423 end
                uVar9 = 0
                r1 = quest:GetThingWithScriptName("CellsExitToArena")
                bVar2 = quest:IsLevelLoaded("ArenaCells")
                if bVar2 then
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            r1 = nil
                            goto LAB_00f1eba3
                        end
                        iVar4 = math.random(0, 32767)
                        iVar4 = iVar4 % 3
                        if iVar4 == 0 then
                            uVar9 = quest:PlayCriteriaSoundOnThing(r1, "ARENA_HECKLE_BIG_MUFFLED_LOOP")
                        else
                            if iVar4 == 1 then
                                uVar9 = quest:PlayCriteriaSoundOnThing(r1, "ARENA_APPLAUSE_MEDIUM_MUFFLED_LOOP")
                                goto LAB_00f1cf00
                            end
                            if iVar4 == 2 then
                                uVar9 = quest:PlayCriteriaSoundOnThing(r1, "ARENA_CHANT_MUFFLED_LOOP")
                                goto LAB_00f1cf00
                            end
                        end
                        ::LAB_00f1cf00::
                        i_stk_fc = math.random(0, 32767)
                        i_stk_fc = i_stk_fc % 10
                        quest:Pause(i_stk_fc + 2.0)
                        quest:StopSound(uVar9)
                        bVar2 = quest:IsLevelLoaded("ArenaCells")
                    until not (bVar2)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                r1 = nil
                if bVar2 then
                    goto LAB_00f1eba3
                end
                bVar2 = quest:IsLevelLoaded("ArenaCells")
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            __cleanup_LAB_00f1e552()
            return
        end
        bVar2 = quest:IsLevelLoaded("Arena")
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                i_stk_fc = quest:AddQuestInfoBar(400.0, 0.0, {R = 255, G = 255, B = 0, A = 255}, {R = 255, G = 255, B = 0, A = 255}, "HUD_EXPR_CLAP", "", 1.0)
                quest:DisplayQuestInfo(true)
                xStack_150 = quest:GetAllThingsWithScriptName("CrowdSoundSource")
                -- TODO(native): Vector_ZeroInit(#xStack_150);
                iStack_138 = quest:GetAllThingsWithDefName("OBJECT_ARENA_AUDIENCE_01")
                c_stk_151 = 0
                bVar2 = quest:IsLevelLoaded("Arena")
                if bVar2 then
                    -- LAB_00f1d1e0: (native jump target)
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        quest:DeregisterTimer(i_stk_15c)
                        quest:DeregisterTimer(i_stk_158)
                        return
                    end
                    if (not quest:GetStateBool("PauseCrowdChecker")) or (c_stk_151 == 0) then goto LAB_00f1d2e6 end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        repeat
                            if not quest:GetStateBool("PauseCrowdChecker") then
                                goto LAB_00f1d260
                            else
                                bVar2 = quest:IsLevelLoaded("Arena")
                                if not bVar2 then goto LAB_00f1d260 end
                                bVar2 = true
                            end
                            goto FLOW_past_lab_00f1d260
                            ::LAB_00f1d260::
                            bVar2 = false
                            ::FLOW_past_lab_00f1d260::
                            if not bVar2 then goto LAB_00f1d2d7 end
                            alive = quest:NewScriptFrame()
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:DeregisterTimer(i_stk_15c)
                                quest:DeregisterTimer(i_stk_158)
                                return
                            end
                        until false
                    end
                    goto LAB_00f1eb80
                end
                -- LAB_00f1e382: (native jump target)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    quest:RemoveQuestInfoElement(i_stk_fc)
                    quest:DisplayQuestInfo(false)
                    goto LAB_00f1e3c7
                end
                ::LAB_00f1eb80::
            end
            goto LAB_00f1eba3
        end
        ::LAB_00f1e3c7::
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    end
    ::FLOW_after_lab_00f1e382::
    ::LAB_00f1eba3::
    quest:DeregisterTimer(i_stk_15c)
    quest:DeregisterTimer(i_stk_158)
    do return end
    ::LAB_00f1e423::
    goto LAB_00f1eba3
    ::LAB_00f1d2d7::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        quest:DeregisterTimer(i_stk_15c)
        quest:DeregisterTimer(i_stk_158)
        return
    end
    ::LAB_00f1d2e6::
    bVar2 = quest:IsLevelLoaded("Arena")
    if not bVar2 then goto LAB_00f1e350 end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        -- LAB_00f1e547: (native jump target)
        __cleanup_LAB_00f1e552(); return
    end
    quest:UpdateQuestInfoBar(i_stk_fc, quest:GetStateInt("NewCrowdPoints"), -1.0, -1.0)
    iVar4 = quest:GetTimer(i_stk_158)
    if iVar4 < 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            quest:DeregisterTimer(i_stk_15c)
            quest:DeregisterTimer(i_stk_158)
            return
        end
        quest:SetTimer(i_stk_158, 5)
        iVar3 = 2
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                quest:DeregisterTimer(i_stk_15c)
                quest:DeregisterTimer(i_stk_158)
                return
            end
            if (aiStack_c[iVar3] <= quest:GetStateInt("NewCrowdPoints")) or (iVar3 == 0) then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                iVar4 = iVar3
                if bVar2 then
                    quest:DeregisterTimer(i_stk_15c)
                    quest:DeregisterTimer(i_stk_158)
                    return
                end
                break
            end
            iVar3 = iVar3 + -1
            iVar4 = i_stk_114
        until not (-1 < iVar3)
        i_stk_114 = iVar4
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            quest:DeregisterTimer(i_stk_15c)
            quest:DeregisterTimer(i_stk_158)
            return
        end
        iVar3 = quest:GetStateInt("NewCrowdPoints") + -4
        quest:SetStateInt("NewCrowdPoints", iVar3)
        if 400 < iVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                quest:DeregisterTimer(i_stk_15c)
                quest:DeregisterTimer(i_stk_158)
                return
            end
            quest:SetStateInt("NewCrowdPoints", 400)
        end
        if (i_stk_114 ~= quest:GetStateInt("NewCrowdHappiness")) or (i_stk_f4 ~= quest:GetStateInt("NewCrowdBaseLevel")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                quest:DeregisterTimer(i_stk_15c)
                quest:DeregisterTimer(i_stk_158)
                return
            end
            iVar3 = quest:GetStateInt("NewCrowdBaseLevel")
            quest:SetStateInt("NewCrowdHappiness", i_stk_114)
            uVar9 = 0
            i_stk_f4 = iVar3
            if #xStack_150 ~= 0 then
                iVar4 = 0
                repeat
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        quest:DeregisterTimer(i_stk_15c)
                        quest:DeregisterTimer(i_stk_158)
                        return
                    end
                    -- TODO(native): quest:StopSound(*(xStack_12c + uVar9 * 4))
                    quest:Pause(0.2)
                    -- TODO(native): puStack_10 = (uint *)((int)xStack_12c + uVar9 * 4);
                    uVar5 = quest:PlayCriteriaSoundOnThing(xStack_150[(iVar4) / 0xc + 1], iStack_138)
                    -- TODO(native): *puStack_10 = uVar5;
                    c_stk_151 = 0
                    quest:Pause(0.3)
                    uVar9 = uVar9 + 1
                    iVar4 = iVar4 + 0xc
                until not (uVar9 < (#xStack_150))
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                quest:DeregisterTimer(i_stk_15c)
                quest:DeregisterTimer(i_stk_158)
                return
            end
        end
        uVar9 = 0
        if #iStack_138 ~= 0 then
            iVar3 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    quest:DeregisterTimer(i_stk_15c)
                    quest:DeregisterTimer(i_stk_158)
                    return
                end
                elem_1 = iStack_138[(iVar3) / 0xc + 1]
                pPos = elem_1:GetPos()
                bVar2 = quest:IsCameraPosOnScreen(pPos)
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        quest:DeregisterTimer(i_stk_15c)
                        quest:DeregisterTimer(i_stk_158)
                        return
                    end
                    if i_stk_114 == 0 then
                        uVar5 = math.random(0, 32767)
                        uVar5 = uVar5 & 0x80000001
                        bVar2 = uVar5 == 0
                        if uVar5 < 0 then
                            bVar2 = (uVar5 - 1 | 0xfffffffe) == 0xffffffff
                        end
                        if bVar2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:DeregisterTimer(i_stk_15c)
                                quest:DeregisterTimer(i_stk_158)
                                return
                            end
                            quest:EntityPlayObjectAnimation(iStack_138[(iVar3) / 0xc + 1], "CLAP_LOOP_02", true)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:DeregisterTimer(i_stk_15c)
                                quest:DeregisterTimer(i_stk_158)
                                return
                            end
                            quest:EntityPlayObjectAnimation(iStack_138[(iVar3) / 0xc + 1], "IDLE_BREATHE_01", true)
                        end
                    else
                        if i_stk_114 == 1 then
                            uVar5 = math.random(0, 32767)
                            uVar5 = uVar5 & 0x80000001
                            bVar2 = uVar5 == 0
                            if uVar5 < 0 then
                                bVar2 = (uVar5 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:DeregisterTimer(i_stk_15c)
                                    quest:DeregisterTimer(i_stk_158)
                                    return
                                end
                                quest:EntityPlayObjectAnimation(iStack_138[(iVar3) / 0xc + 1], "CLAP_LOOP_02", true)
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:DeregisterTimer(i_stk_15c)
                                    quest:DeregisterTimer(i_stk_158)
                                    return
                                end
                                quest:EntityPlayObjectAnimation(iStack_138[(iVar3) / 0xc + 1], "CHEER_LOOP_01", true)
                            end
                        else
                            if i_stk_114 ~= 2 then goto LAB_00f1d721 end
                            quest:EntityPlayObjectAnimation(iStack_138[(iVar3) / 0xc + 1], "CHEER_LOOP_01", true)
                        end
                    end
                end
                ::LAB_00f1d721::
                uVar9 = uVar9 + 1
                iVar3 = iVar3 + 0xc
            until not (uVar9 < (#iStack_138))
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            quest:DeregisterTimer(i_stk_15c)
            quest:DeregisterTimer(i_stk_158)
            return
        end
    end
    iVar4 = quest:GetTimer(i_stk_15c)
    if iVar4 < 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            quest:DeregisterTimer(i_stk_15c)
            quest:DeregisterTimer(i_stk_158)
            return
        end
        native_arg_sequence_1 = false
        if c_stk_151 ~= 0 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if native_arg_sequence_1 then
            bVar2 = quest:IsSoundPlaying(0)
            if bVar2 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then goto LAB_00f1e2fb end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            quest:DeregisterTimer(i_stk_15c)
            quest:DeregisterTimer(i_stk_158)
            return
        end
        c_stk_151 = 1
        r2 = quest:GetThingWithScriptName("SingleShouter")
        xStack_c0 = quest:PlayCriteriaSoundOnThing(r2, xStack_150)
        iVar4 = math.random(0, 32767)
        quest:SetTimer(i_stk_15c, iVar4 % quest:ReadGlobalGameData(0xa68) + quest:ReadGlobalGameData(0xa64))
        iVar4 = quest:AddNewConversation(r2, false, false)
        pCVar6 = quest:GetHero()
        quest:AddPersonToConversation(iVar4, pCVar6)
        if quest:GetStateInt("NewCrowdHappiness") < 1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                quest:DeregisterTimer(i_stk_15c)
                quest:DeregisterTimer(i_stk_158)
                return
            end
            native_arg_switch_2 = quest:GetStateInt("ArenaRound")
            repeat
                if native_arg_switch_2 == 0 then
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHEER_WASPS", r2, pCVar6, false)
                    break
                else
                    if native_arg_switch_2 == 1 then
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHEER_HOBBES", r2, pCVar6, false)
                        break
                    else
                        if native_arg_switch_2 == 2 then
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHEER_BALVERINES", r2, pCVar6, false)
                            break
                        else
                            if native_arg_switch_2 == 3 then
                                pCVar6 = quest:GetHero()
                                quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHEER_UNDEAD", r2, pCVar6, false)
                                break
                            else
                                if native_arg_switch_2 == 4 then
                                    pCVar6 = quest:GetHero()
                                    quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHEER_BANDITS", r2, pCVar6, false)
                                    break
                                else
                                    if native_arg_switch_2 == 5 then
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHEER_EARTH_TROLL", r2, pCVar6, false)
                                        break
                                    else
                                        if native_arg_switch_2 == 6 then
                                            pCVar6 = quest:GetHero()
                                            quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHEER_ROCK_TROLL", r2, pCVar6, false)
                                            break
                                        else
                                            if native_arg_switch_2 == 7 then
                                                pCVar6 = quest:GetHero()
                                                quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHEER_SCORPION", r2, pCVar6, false)
                                                break
                                            else
                                                if native_arg_switch_2 == 8 then
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHEER_WASPS", r2, pCVar6, false)
                                                    break
                                                else
                                                    goto FLOW_native_label_1
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            until not (false)
            goto LAB_00f1e2e9
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            quest:DeregisterTimer(i_stk_15c)
            quest:DeregisterTimer(i_stk_158)
            return
        end
        uVar9 = math.random(0, 32767)
        uVar9 = uVar9 & 0x80000001
        bVar2 = uVar9 == 0
        if uVar9 < 0 then
            bVar2 = (uVar9 - 1 | 0xfffffffe) == 0xffffffff
        end
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                quest:DeregisterTimer(i_stk_15c)
                quest:DeregisterTimer(i_stk_158)
                return
            end
            native_arg_switch_3 = quest:GetStateInt("ArenaRound")
            repeat
                if native_arg_switch_3 == 0 then
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_BOO_WASPS", r2, pCVar6, false)
                    break
                else
                    if native_arg_switch_3 == 1 then
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_BOO_HOBBES", r2, pCVar6, false)
                        break
                    else
                        if native_arg_switch_3 == 2 then
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_BOO_BALVERINES", r2, pCVar6, false)
                            break
                        else
                            if native_arg_switch_3 == 3 then
                                pCVar6 = quest:GetHero()
                                quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_BOO_UNDEAD", r2, pCVar6, false)
                                break
                            else
                                if native_arg_switch_3 == 4 then
                                    pCVar6 = quest:GetHero()
                                    quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_BOO_BANDITS", r2, pCVar6, false)
                                    break
                                else
                                    if native_arg_switch_3 == 5 then
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_BOO_EARTH_TROLL", r2, pCVar6, false)
                                        break
                                    else
                                        if native_arg_switch_3 == 6 then
                                            pCVar6 = quest:GetHero()
                                            quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_BOO_ROCK_TROLL", r2, pCVar6, false)
                                            break
                                        else
                                            if native_arg_switch_3 == 7 then
                                                pCVar6 = quest:GetHero()
                                                quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_BOO_SCORPION", r2, pCVar6, false)
                                                break
                                            else
                                                goto FLOW_native_label_1
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            until not (false)
            goto LAB_00f1e2e9
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            quest:DeregisterTimer(i_stk_15c)
            quest:DeregisterTimer(i_stk_158)
            return
        end
        iVar3 = quest:GetHeroTitle()
        native_arg_switch_4 = iVar3
        repeat
            if native_arg_switch_4 == 1 then
                pCVar6 = quest:GetHero()
                quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_17", r2, pCVar6, false)
                break
            else
                if native_arg_switch_4 == 2 then
                    pCVar6 = quest:GetHero()
                    quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_20", r2, pCVar6, false)
                    break
                else
                    if native_arg_switch_4 == 3 then
                        pCVar6 = quest:GetHero()
                        quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_11", r2, pCVar6, false)
                        break
                    else
                        if native_arg_switch_4 == 4 then
                            pCVar6 = quest:GetHero()
                            quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_06", r2, pCVar6, false)
                            break
                        else
                            if native_arg_switch_4 == 5 then
                                pCVar6 = quest:GetHero()
                                quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_03", r2, pCVar6, false)
                                break
                            else
                                if native_arg_switch_4 == 6 then
                                    pCVar6 = quest:GetHero()
                                    quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_12", r2, pCVar6, false)
                                    break
                                else
                                    if native_arg_switch_4 == 7 then
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_04", r2, pCVar6, false)
                                        break
                                    else
                                        if native_arg_switch_4 == 8 then
                                            pCVar6 = quest:GetHero()
                                            quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_15", r2, pCVar6, false)
                                            break
                                        else
                                            if native_arg_switch_4 == 9 then
                                                pCVar6 = quest:GetHero()
                                                quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_10", r2, pCVar6, false)
                                                break
                                            else
                                                if native_arg_switch_4 == 10 then
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_13", r2, pCVar6, false)
                                                    break
                                                else
                                                    if native_arg_switch_4 == 0xb then
                                                        pCVar6 = quest:GetHero()
                                                        quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_07", r2, pCVar6, false)
                                                        break
                                                    else
                                                        if native_arg_switch_4 == 0xc then
                                                            pCVar6 = quest:GetHero()
                                                            quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_16", r2, pCVar6, false)
                                                            break
                                                        else
                                                            if native_arg_switch_4 == 0xd then
                                                                pCVar6 = quest:GetHero()
                                                                quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_18", r2, pCVar6, false)
                                                                break
                                                            else
                                                                if native_arg_switch_4 == 0xe then
                                                                    pCVar6 = quest:GetHero()
                                                                    quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_09", r2, pCVar6, false)
                                                                    break
                                                                else
                                                                    if native_arg_switch_4 == 0xf then
                                                                        pCVar6 = quest:GetHero()
                                                                        quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_08", r2, pCVar6, false)
                                                                        break
                                                                    else
                                                                        if native_arg_switch_4 == 0x10 then
                                                                            pCVar6 = quest:GetHero()
                                                                            quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_19", r2, pCVar6, false)
                                                                            break
                                                                        else
                                                                            if native_arg_switch_4 == 0x11 then
                                                                                pCVar6 = quest:GetHero()
                                                                                quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_01", r2, pCVar6, false)
                                                                                break
                                                                            else
                                                                                if native_arg_switch_4 == 0x12 then
                                                                                    pCVar6 = quest:GetHero()
                                                                                    quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_14", r2, pCVar6, false)
                                                                                    break
                                                                                else
                                                                                    if native_arg_switch_4 == 0x13 then
                                                                                        pCVar6 = quest:GetHero()
                                                                                        quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_05", r2, pCVar6, false)
                                                                                        break
                                                                                    else
                                                                                        if native_arg_switch_4 == 0x14 then
                                                                                            pCVar6 = quest:GetHero()
                                                                                            quest:AddLineToConversation(iVar4, "TEXT_QST_005_ARENA_CROWD_CHANT_NAME_02", r2, pCVar6, false)
                                                                                            break
                                                                                        else
                                                                                            goto FLOW_native_label_1
                                                                                        end
                                                                                    end
                                                                                end
                                                                            end
                                                                        end
                                                                    end
                                                                end
                                                            end
                                                        end
                                                    end
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            end
        until not (false)
        ::LAB_00f1e2e9::
        ::FLOW_native_label_1::
    end
    ::LAB_00f1e2fb::
    pCVar6 = quest:GetHero()
    bVar2 = (pCVar6 ~= nil and pCVar6:MsgIsHitBy(""))
    if bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            quest:DeregisterTimer(i_stk_15c)
            quest:DeregisterTimer(i_stk_158)
            return
        end
        quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + -5)
    end
    ::LAB_00f1e350::
    bVar2 = quest:IsLevelLoaded("Arena")
    if not bVar2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:RemoveQuestInfoElement(i_stk_fc)
            quest:DisplayQuestInfo(false)
            goto LAB_00f1e3c7_c2
        end
        -- LAB_00f1eb80_c2: (native jump target)
        goto LAB_00f1eba3
        ::LAB_00f1e3c7_c2::
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        goto FLOW_after_lab_00f1e382
    end
    -- TODO(native): goto LAB_00f1d1e0
end

function InitialiseVariables(quest)
    helper_F25980(quest, this + 0x98, DAT_0143e90c + 0x1044)
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

function AnimateCrowd(quest)
    local iVar5, i_stk_1c, native_arg_switch_1, xStack_18
    local xStack_c = quest:GetAllThingsWithScriptName("AudienceMember")
    local iVar4 = #xStack_c
    iVar5 = extraout_EAX
    if 0 < iVar4 then
        iVar5 = 0
        i_stk_1c = iVar4
        repeat
            xStack_18 = nil
            -- TODO(native): xStack_18._4_4_ = *(undefined4 *)(iVar5 + 4 + (int)xStack_c);
            iVar4 = math.random(0, 32767)
            native_arg_switch_1 = iVar4 % 6
            repeat
                if native_arg_switch_1 == 0 then
                    quest:EntityPlayObjectAnimation(xStack_18, "CHEER_LOOP_01", true)
                    break
                else
                    if native_arg_switch_1 == 1 then
                        quest:EntityPlayObjectAnimation(xStack_18, "CHEER_LOOP_02", true)
                        break
                    else
                        if native_arg_switch_1 == 2 then
                            quest:EntityPlayObjectAnimation(xStack_18, "CLAP_LOOP_02", true)
                            break
                        else
                            if native_arg_switch_1 == 3 then
                                quest:EntityPlayObjectAnimation(xStack_18, "SHOUT_LOOP_01", true)
                                break
                            else
                                if native_arg_switch_1 == 4 then
                                    quest:EntityPlayObjectAnimation(xStack_18, "THUMBS_DOWN_LOOP_01", true)
                                    break
                                else
                                    quest:EntityPlayObjectAnimation(xStack_18, "WAVE_LOOP_01", true)
                                end
                            end
                        end
                    end
                end
            until not (false)
            quest:EntitySetCutsceneBehaviour(xStack_18, 2)
            xStack_18 = nil
            iVar5 = iVar5 + 0xc
            i_stk_1c = i_stk_1c + -1
        until not (i_stk_1c ~= 0)
        iVar5 = 0
        i_stk_1c = 0
    end
end

function GetFanfareMusic(quest, native_arg_Fanfare)
    local native_arg_switch_1 = native_arg_Fanfare
    repeat
        if native_arg_switch_1 == 1 then
            return 0x24
        else
            if native_arg_switch_1 == 2 then
                return 0x25
            else
                if native_arg_switch_1 == 3 then
                    return 0x26
                else
                    if native_arg_switch_1 == 4 then
                        return 0x27
                    else
                        if native_arg_switch_1 == 5 then
                            return 0x28
                        else
                            if native_arg_switch_1 == 6 then
                                return 0x29
                            else
                                if native_arg_switch_1 == 7 then
                                    return 0x2a
                                else
                                    if native_arg_switch_1 == 8 then
                                        return 0x2b
                                    else
                                        if native_arg_switch_1 == 9 then
                                            return 0x2c
                                        else
                                            return 0x23
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            end
        end
    until not (false)
end

function PlayWave(quest)
    local resources = quest:RetailResources()
    local CVar14, b1, b2, bVar20, bVar3, c_stk_13d, c_stk_fd, ctr_10c, ctr_fc, fVar18, f_stk_ec, f_stk_f0, iVar12, iVar4, iVar6, i_stk_15c, i_stk_84, i_stk_b4, i_stk_b8, i_stk_d0, i_stk_e8, native_arg_sequence_1, pCVar13, pCVar19, pCVar5, pCVar7, pQuestionText, piVar10, pvVar8, r1, r2, r3, uVar11, uVar9, xStack_108, xStack_118, xStack_124, xStack_130, xStack_9c, xStack_a0, xStack_b0, xStack_c0, xStack_e4, xStack_f4, x_stk_104, x_stk_cc
    local alive = true
    r1 = quest:GetThingWithScriptName("ARENA_SpawnPoint")
    r2 = quest:GetThingWithScriptName("BigCreatureSpawnPoint")
    iVar4 = quest:RegisterTimer()
    quest:SetStateBool("PauseCrowdChecker", false)
    i_stk_15c = iVar4
    -- TODO(native): if (*(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x38 + quest:GetStateInt("ArenaRoundWave") * 0x3c) == 0) and ((quest:GetStateInt("ArenaRound") ~= 0 or (quest:GetStateInt("ArenaRoundWave") ~= 0))) then
    if false then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00f1f059: (native jump target)
            quest:DeregisterTimer(iVar4)
            goto LAB_00f2119d
        end
        bVar20 = false
        bVar3 = false
        pCVar5 = quest:GetHero()
        iVar6 = quest:AddNewConversation(pCVar5, bVar3, bVar20)
        pCVar5 = quest:GetHero()
        pCVar7 = quest:GetHero()
        quest:AddLineToConversation(iVar6, "TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_THREE", pCVar7, pCVar5, false)
        iVar4 = i_stk_15c
        quest:SetTimer(i_stk_15c, 2)
        iVar6 = quest:GetTimer(iVar4)
        while 0 < iVar6 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f21195 end
            iVar6 = quest:GetTimer(iVar4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00f21195 end
        bVar20 = false
        bVar3 = false
        pCVar5 = quest:GetHero()
        iVar6 = quest:AddNewConversation(pCVar5, bVar3, bVar20)
        pCVar5 = quest:GetHero()
        pCVar7 = quest:GetHero()
        quest:AddLineToConversation(iVar6, "TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_TWO", pCVar7, pCVar5, false)
        iVar4 = i_stk_15c
        quest:SetTimer(i_stk_15c, 2)
        iVar6 = quest:GetTimer(iVar4)
        while 0 < iVar6 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f20cc5 end
            iVar6 = quest:GetTimer(iVar4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            bVar20 = false
            bVar3 = false
            pCVar5 = quest:GetHero()
            iVar6 = quest:AddNewConversation(pCVar5, bVar3, bVar20)
            pCVar5 = quest:GetHero()
            pCVar7 = quest:GetHero()
            quest:AddLineToConversation(iVar6, "TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_ONE", pCVar7, pCVar5, false)
            iVar4 = i_stk_15c
            quest:SetTimer(i_stk_15c, 2)
            iVar6 = quest:GetTimer(iVar4)
            while 0 < iVar6 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f20cc5 end
                iVar6 = quest:GetTimer(iVar4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                bVar20 = false
                bVar3 = false
                pCVar5 = quest:GetHero()
                iVar6 = quest:AddNewConversation(pCVar5, bVar3, bVar20)
                pCVar5 = quest:GetHero()
                pCVar7 = quest:GetHero()
                quest:AddLineToConversation(iVar6, "TEXT_QST_005_V2_ARENA_KEEPER_COUNTDOWN_GO", pCVar7, pCVar5, false)
                goto LAB_00f1f251
            end
        end
        -- LAB_00f212f0: (native jump target)
        quest:DeregisterTimer(i_stk_15c)
        goto LAB_00f2119d
    end
    ::LAB_00f1f251::
    xStack_118 = quest:GetAllThingsWithScriptName("MultipleBigCreatureSpawnPoint")
    i_stk_84 = #xStack_118
    xStack_130 = quest:GetAllThingsWithScriptName("ArenaSpawn")
    iVar6 = #xStack_130
    pCVar5 = quest:GetHero()
    r3 = quest:GetFurthestWithScriptName(pCVar5, "ArenaSpawn")
    if not (r3 ~= nil and not r3:IsNull()) then
        -- TODO(native): CCharString::CCharString(xStack_a0,(CCharString *)&DAT_0143e8ec);
    else
        xStack_a0 = r3:GetDataString()
    end
    iVar4 = tonumber(xStack_a0)
    i_stk_b4 = iVar4
    x_stk_104 = (iVar4 + -1)
    c_stk_fd = 1
    if x_stk_104 == 0xffffffff then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            x_stk_104 = (iVar6 + -1)
            goto LAB_00f1f37d
        end
        goto LAB_00f2116f
    end
    ::LAB_00f1f37d::
    xStack_124 = quest:GetAllThingsWithScriptName("ArenaEnemy")
    native_arg_sequence_1 = false
    if quest:GetStateInt("ArenaRound") == 0 then
        native_arg_sequence_1 = true
    else
        native_arg_sequence_1 = false
    end
    if native_arg_sequence_1 then
        if quest:GetStateInt("ArenaRoundWave") == 0 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
    end
    if native_arg_sequence_1 then
        iVar4 = #xStack_124 * 0xc >> 0x1f
        if #xStack_124 ~= 0 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
    end
    if native_arg_sequence_1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            goto LAB_00f20cc5
        end
        -- TODO(native): *(int *)__element("TotalCreatures", 0) = #xStack_124;
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00f21166 end
        ctr_10c = 0
        -- TODO(native): if 0 < *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x28 + quest:GetStateInt("ArenaRoundWave") * 0x3c) then
        if false then
            repeat
                CVar14 = 0x0
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f21166 end
                -- TODO(native): *(int *)__element("TotalCreatures", 0) = 0;
                i_stk_b8 = 0
                -- TODO(native): if 0 < *(*(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c) + 0x2c + CVar14) then
                if false then
                    repeat
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00f21166 end
                        if quest:GetStateInt("ArenaRound") < 5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00f21166 end
                            if c_stk_fd == 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    goto LAB_00f20cc5
                                end
                                i_stk_e8 = 0
                                if 0 < iVar6 then
                                    i_stk_d0 = 0
                                    repeat
                                        iVar4 = i_stk_d0
                                        pvVar8 = xStack_130[(i_stk_d0) / 0xc + 1]:GetDataString()
                                        piVar10 = tonumber(pvVar8)
                                        c_stk_13d = piVar10 == x_stk_104
                                        if c_stk_13d then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if not bVar3 then
                                                xStack_e4 = nil
                                                if quest:GetStateInt("ArenaRound") == 3 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if not bVar3 then
                                                        quest:SetCreatureCreationDelayFrames(1)
                                                        -- TODO(native): iVar6 = *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c)
                                                        iVar6 = nil --[[unresolved native value]]
                                                        bVar3 = true
                                                        pCVar13 = xStack_130[(iVar4) / 0xc + 1]:GetPos()
                                                        pCVar5 = quest:CreateCreature("ArenaEnemy", pCVar13, pvVar8)
                                                        xStack_e4 = pCVar5
                                                        goto LAB_00f1fcce
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if not bVar3 then
                                                        quest:SetCreatureCreationDelayFrames(1)
                                                        -- TODO(native): iVar6 = *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c)
                                                        iVar6 = nil --[[unresolved native value]]
                                                        bVar3 = false
                                                        fVar18 = 1.0
                                                        pCVar13 = xStack_130[(iVar4) / 0xc + 1]:GetPos()
                                                        pCVar5 = quest:CreateCreatureNearby("ArenaEnemy", pCVar13, (iVar6 + 0x28 + CVar14), xStack_124)
                                                        xStack_e4 = pCVar5
                                                        goto LAB_00f1fcce
                                                    end
                                                end
                                                goto FLOW_past_lab_00f1fcce
                                                ::LAB_00f1fcce::
                                                quest:ResetCreatureCreationDelayFrames()
                                                quest:EntitySetCutsceneBehaviour(xStack_e4, 1)
                                                pCVar5 = quest:GetThingWithScriptName("WhisperAlly")
                                                c_stk_13d = (pCVar5 ~= nil and pCVar5:IsAlive())
                                                if not c_stk_13d then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if not bVar3 then
                                                        pCVar5 = quest:GetHero()
                                                        goto LAB_00f1fe19
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if not bVar3 then
                                                        uVar11 = math.random(0, 32767)
                                                        uVar11 = uVar11 & 0x80000001
                                                        bVar3 = uVar11 == 0
                                                        if uVar11 < 0 then
                                                            bVar3 = (uVar11 - 1 | 0xfffffffe) == 0xffffffff
                                                        end
                                                        if bVar3 then
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                pCVar5 = quest:GetThingWithScriptName("WhisperAlly")
                                                                quest:GiveThingBestEnemyTarget(xStack_e4, pCVar5)
                                                                goto LAB_00f1fe22
                                                            end
                                                        else
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                pCVar5 = quest:GetHero()
                                                                goto LAB_00f1fe19
                                                            end
                                                        end
                                                    end
                                                end
                                                goto FLOW_past_lab_00f1fe19
                                                ::LAB_00f1fe19::
                                                quest:GiveThingBestEnemyTarget(xStack_e4, pCVar5)
                                                goto LAB_00f1fe22
                                                ::FLOW_past_lab_00f1fe19::
                                                goto FLOW_past_lab_00f1fe22
                                                ::LAB_00f1fe22::
                                                -- TODO(native): this[(int)x_stk_104 + 0xe2] = (CQ_ArenaScript)0x1;
                                                iVar4 = i_stk_d0
                                                goto LAB_00f1fe48
                                                ::FLOW_past_lab_00f1fe22::
                                                ::FLOW_past_lab_00f1fcce::
                                            end
                                            goto LAB_00f20cc5
                                        end
                                        ::LAB_00f1fe48::
                                        i_stk_e8 = i_stk_e8 + 1
                                        i_stk_d0 = iVar4 + 0xc
                                    until not (i_stk_e8 < iVar6)
                                end
                                x_stk_104 = (x_stk_104 + -1)
                                if x_stk_104 == 0xffffffff then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        goto LAB_00f20cc5
                                    end
                                    x_stk_104 = (iVar6 + -1)
                                end
                                c_stk_fd = 1
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    goto LAB_00f20cc5
                                end
                                i_stk_d0 = 0
                                if 0 < iVar6 then
                                    i_stk_e8 = 0
                                    repeat
                                        iVar4 = i_stk_e8
                                        pvVar8 = xStack_130[(i_stk_e8) / 0xc + 1]:GetDataString()
                                        iVar12 = tonumber(pvVar8)
                                        c_stk_13d = iVar12 == i_stk_b4
                                        if c_stk_13d then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if not bVar3 then
                                                x_stk_cc = nil
                                                if quest:GetStateInt("ArenaRound") == 3 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if not bVar3 then
                                                        quest:SetCreatureCreationDelayFrames(1)
                                                        -- TODO(native): iVar6 = *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c)
                                                        iVar6 = nil --[[unresolved native value]]
                                                        pCVar19 = "ArenaEnemy"
                                                        pCVar13 = xStack_130[(iVar4) / 0xc + 1]:GetPos()
                                                        pCVar5 = quest:CreateCreature("ArenaEnemy", pCVar13, pvVar8)
                                                        i_stk_d0 = pCVar5
                                                        goto LAB_00f1f934
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if not bVar3 then
                                                        quest:SetCreatureCreationDelayFrames(1)
                                                        -- TODO(native): iVar6 = *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c)
                                                        iVar6 = nil --[[unresolved native value]]
                                                        bVar3 = false
                                                        fVar18 = 1.0
                                                        pCVar13 = xStack_130[(iVar4) / 0xc + 1]:GetPos()
                                                        pCVar5 = quest:CreateCreatureNearby("ArenaEnemy", pCVar13, (iVar6 + 0x28 + CVar14), xStack_a0)
                                                        xStack_c0 = pCVar5
                                                        goto LAB_00f1f934
                                                    end
                                                end
                                                goto FLOW_past_lab_00f1f934
                                                ::LAB_00f1f934::
                                                quest:ResetCreatureCreationDelayFrames()
                                                quest:EntitySetCutsceneBehaviour(pCVar5, 1)
                                                pCVar5 = quest:GetThingWithScriptName("WhisperAlly")
                                                c_stk_13d = (pCVar5 ~= nil and pCVar5:IsAlive())
                                                if not c_stk_13d then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if not bVar3 then
                                                        pCVar5 = quest:GetHero()
                                                        goto LAB_00f1fa88
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if not bVar3 then
                                                        uVar11 = math.random(0, 32767)
                                                        uVar11 = uVar11 & 0x80000001
                                                        bVar3 = uVar11 == 0
                                                        if uVar11 < 0 then
                                                            bVar3 = (uVar11 - 1 | 0xfffffffe) == 0xffffffff
                                                        end
                                                        if bVar3 then
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                pCVar5 = quest:GetThingWithScriptName("WhisperAlly")
                                                                quest:GiveThingBestEnemyTarget(pCVar5, r3)
                                                                goto LAB_00f1fa91
                                                            end
                                                        else
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if not bVar3 then
                                                                pCVar5 = quest:GetHero()
                                                                goto LAB_00f1fa88
                                                            end
                                                        end
                                                    end
                                                end
                                                goto FLOW_past_lab_00f1fa88
                                                ::LAB_00f1fa88::
                                                quest:GiveThingBestEnemyTarget(pCVar5, r2)
                                                goto LAB_00f1fa91
                                                ::FLOW_past_lab_00f1fa88::
                                                goto FLOW_past_lab_00f1fa91
                                                ::LAB_00f1fa91::
                                                -- TODO(native): this[i_stk_b4 + 0xe2] = (CQ_ArenaScript)0x1;
                                                iVar4 = i_stk_e8
                                                goto LAB_00f1faba
                                                ::FLOW_past_lab_00f1fa91::
                                                ::FLOW_past_lab_00f1f934::
                                            end
                                            goto LAB_00f20cc5
                                        end
                                        ::LAB_00f1faba::
                                        i_stk_d0 = i_stk_d0 + 1
                                        i_stk_e8 = iVar4 + 0xc
                                    until not (i_stk_d0 < iVar6)
                                end
                                i_stk_b4 = (i_stk_b4 + 1) % iVar6
                                c_stk_fd = 0
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00f21166 end
                            xStack_f4 = nil
                            f_stk_f0 = 0.0
                            f_stk_ec = 0.0
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if iVar6 == 1 then
                                if bVar3 then
                                    -- LAB_00f20181: (native jump target)
                                    goto LAB_00f20cc5
                                end
                                quest:SetCreatureCreationDelayFrames(1)
                                if not (r2 ~= nil and not r2:IsNull()) then
                                    pCVar13 = {x = 0, y = 0, z = 0}
                                else
                                    pCVar13 = r2:GetPos()
                                end
                                -- TODO(native): pCVar5 = quest:CreateCreatureNearby("ArenaEnemy", pCVar13, (*(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c) + 0x28 + CVar14), xStack_130)
                                pCVar5 = nil --[[unresolved native value]]
                                xStack_f4 = pCVar5
                                quest:ResetCreatureCreationDelayFrames()
                            else
                                if bVar3 then
                                    goto LAB_00f20cc5
                                end
                                ctr_fc = 0
                                if 0 < i_stk_84 then
                                    iVar4 = 0
                                    repeat
                                        pvVar8 = xStack_118[(iVar4) / 0xc + 1]:GetDataString()
                                        iVar12 = tonumber(pvVar8)
                                        c_stk_13d = iVar12 == i_stk_b8
                                        if c_stk_13d then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then
                                                goto LAB_00f20cc5
                                            end
                                            quest:SetCreatureCreationDelayFrames(1)
                                            -- TODO(native): iVar6 = *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c)
                                            iVar6 = nil --[[unresolved native value]]
                                            uVar9 = xStack_118[(iVar4) / 0xc + 1]:GetPos()
                                            CVar14 = 0x0
                                            iVar6 = quest:CreateCreatureNearby("ArenaEnemy", uVar9, iVar6 + 0x28 + 0x0, pvVar8)
                                            -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_f4,iVar6);
                                            quest:ResetCreatureCreationDelayFrames()
                                        end
                                        ctr_fc = ctr_fc + 1
                                        iVar4 = iVar4 + 0xc
                                    until not (ctr_fc < i_stk_84)
                                end
                            end
                            quest:EntitySetCutsceneBehaviour(xStack_f4, 1)
                            pCVar5 = quest:GetHero()
                            quest:GiveThingBestEnemyTarget(xStack_f4, pCVar5)
                        end
                        -- TODO(native): *(int *)__element("TotalCreatures", 0) = *(int *)__element("TotalCreatures", 0) + 1;
                        i_stk_b8 = i_stk_b8 + 1
                    -- TODO(native): until not (i_stk_b8 < *(*(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c) + 0x2c + CVar14))
                    until true
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    r3 = nil
                    quest:DeregisterTimer(i_stk_15c)
                    r2 = nil
                    r1 = nil
                    -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
                    return
                end
                ctr_10c = ctr_10c + 1
                -- TODO(native): xStack_108 = (CCharString)((int)CVar14 + 0x38);
            -- TODO(native): until not (ctr_10c < *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x28 + quest:GetStateInt("ArenaRoundWave") * 0x3c))
            until true
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00f20ca1 end
    end
    iVar4 = 0
    repeat
        CVar14 = 0x0
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            r3 = nil
            quest:DeregisterTimer(i_stk_15c)
            r2 = nil
            r1 = nil
            -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
            return
        end
        -- TODO(native): iVar6 = *__element("TotalCreatures", 0)
        iVar6 = nil --[[unresolved native value]]
        -- TODO(native): *(int *)(xStack_e4 + iVar4) = iVar6;
        -- TODO(native): *(undefined4 *)(xStack_f4 + iVar4) = 0xffffffff;
        if 0 < iVar6 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            CVar14 = 0x0
            if bVar3 then goto LAB_00f20ca1 end
            -- TODO(native): iVar6 = quest:AddQuestInfoCounterList("ArenaEnemy", (0x0 + 0x30 + *(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c)), *__element("TotalCreatures", 0))
            iVar6 = nil --[[unresolved native value]]
            -- TODO(native): *(int *)(xStack_f4 + iVar4) = iVar6;
        end
        if 0 < *(xStack_e4 + iVar4) then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f20ca1 end
            -- TODO(native): quest:UpdateQuestInfoCounterList(*(xStack_f4 + iVar4), *__element("TotalCreatures", 0), -1)
        end
        -- TODO(native): xStack_108 = (CCharString)((int)CVar14 + 0x38);
        iVar4 = iVar4 + 4
    until not (xStack_108 < 0xa8)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        r3 = nil
        -- LAB_00f200e2: (native jump target)
        quest:DeregisterTimer(i_stk_15c)
        r2 = nil
        r1 = nil
        -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
        return
    end
    quest:DisplayQuestInfo(true)
    iVar4 = 0
    iVar6 = 0
    repeat
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00f2049f end
        iVar4 = iVar4 + *__element("TotalCreatures", 0)
        iVar6 = iVar6 + 1
    until not (iVar6 < 3)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        r3 = nil
        quest:DeregisterTimer(i_stk_15c)
        r2 = nil
        r1 = nil
        -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
        return
    end
    while 0 < iVar4 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        iVar4 = 0
        if bVar3 then goto LAB_00f207f4 end
        iVar6 = 0
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f208af end
            iVar4 = iVar4 + *__element("TotalCreatures", 0)
            if 0 < *(xStack_e4 + iVar6) then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f2096f end
                -- TODO(native): quest:UpdateQuestInfoCounterList(*(xStack_f4 + iVar6), *__element("TotalCreatures", 0), -1)
            end
            iVar6 = iVar6 + 4
        until not (iVar6 < 0xc)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00f20a2f end
        if quest:GetStateInt("ExtraCreatures") ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f20ca1 end
            quest:RemoveQuestInfoElement(xStack_f4)
            -- TODO(native): xStack_e4._0_4_ = xStack_e4._0_4_ + *(int *)(this + 0xdc);
            -- TODO(native): xStack_f4 = quest:AddQuestInfoCounterList("ArenaEnemy", (*(*(quest:GetStateInt("ArenaRound") * 0x38 + 0x2c + quest:GetStateInt("self_0x98")) + 0x2c + quest:GetStateInt("ArenaRoundWave") * 0x3c) + 0x30), xStack_e4._0_4_)
            xStack_f4 = nil --[[unresolved native value]]
            quest:SetStateInt("ExtraCreatures", 0)
        end
        bVar3 = quest:IsLevelLoaded("Arena")
        if not bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f20ca1 end
            quest:SetStateBool("PlayerLeaving", true)
            iVar6 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f20ca1 end
                -- TODO(native): *(undefined4 *)__element("TotalCreatures", 0) = 0;
                iVar6 = iVar6 + 1
            until not (iVar6 < 3)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f20ca1 end
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        iVar4 = 0
        goto LAB_00f20700
    end
    ::LAB_00f20ca1::
    ::LAB_00f20cc5::
    quest:DeregisterTimer(i_stk_15c)
    goto LAB_00f2119d
    ::LAB_00f2049f::
    r3 = nil
    quest:DeregisterTimer(i_stk_15c)
    r2 = nil
    r1 = nil
    -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
    do return end
    ::LAB_00f207f4::
    r3 = nil
    goto LAB_00f20ae2
    ::LAB_00f208af::
    r3 = nil
    goto LAB_00f20ae2
    ::LAB_00f2096f::
    r3 = nil
    goto LAB_00f20ae2
    ::LAB_00f20b8b::
    goto LAB_00f21195
    ::LAB_00f20700::
    while true do
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00f20b8b end
        if 0 < *(xStack_e4 + iVar4) then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f21166 end
            -- TODO(native): quest:RemoveQuestInfoElement(*(xStack_f4 + iVar4))
        end
        iVar4 = iVar4 + 4
        if 0xb < iVar4 then break end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        goto LAB_00f20cc5
    end
    quest:DisplayQuestInfo(false)
    iVar4 = quest:GetStateInt("ArenaRoundWave")
    quest:SetStateInt("ArenaRoundWave", iVar4 + 1)
    quest:SetStateBool("PauseCrowdChecker", true)
    -- TODO(native): if iVar4 + 1 == *(quest:GetStateInt("ArenaRound") * 0x38 + 0x28 + quest:GetStateInt("self_0x98")) then
    if false then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00f20ca1 end
        quest:SetStateInt("ArenaRound", quest:GetStateInt("ArenaRound") + 1)
        quest:SetStateInt("ArenaRoundWave", 0)
        if quest:GetStateInt("NewCrowdPoints") / 100 < 6 then
            if 0 < quest:GetStateInt("NewCrowdPoints") / 100 then goto LAB_00f20ce5 end
            goto LAB_00f20e68
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f20ca1 end
            goto LAB_00f20ce5
        end
        goto FLOW_past_lab_00f20e68
        ::LAB_00f20e68::
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00f20ca1 end
        quest:Pause(1.0)
        b2 = false
        b1 = false
        iVar6 = math.random(0, 32767)
        iVar6 = GetFanfareMusic(quest, iVar6 % 10)
        quest:OverrideMusic(iVar6, b1, b2)
        quest:Pause(5.0)
        quest:StopOverrideMusic(false)
        GivePrizeFund(quest)
        bVar3 = quest:IsHeroControlledByPlayer()
        while not bVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f212cc end
            bVar3 = quest:IsHeroControlledByPlayer()
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00f212cc end
        xStack_9c = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        xStack_b0 = resources:NewResource()
        resources:PrepareResource(xStack_b0)
        iVar6 = 4
        pCVar5 = xStack_b0
        pCVar7 = quest:GetHero()
        bVar3 = resources:TryAcquire(pCVar5, pCVar7, iVar6)
        while not bVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:ReleaseResource(xStack_b0)
                quest:PauseAllNonScriptedEntities(false)
                goto FLOW_after_lab_00f211ba
            end
            iVar6 = 4
            pCVar5 = xStack_b0
            pCVar7 = quest:GetHero()
            bVar3 = resources:TryAcquire(pCVar5, pCVar7, iVar6)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            resources:ReleaseResource(xStack_b0)
            quest:PauseAllNonScriptedEntities(false)
            goto FLOW_after_lab_00f211ba
        end
        if 7 < quest:GetStateInt("ArenaRound") then
            -- LAB_00f21107_c14: (native jump target)
            quest:FadeScreenOut(0.5, 0.5)
            quest:Pause(0.5)
            resources:ReleaseResource(xStack_b0)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_9c)
            goto LAB_00f21166
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00f211ba: (native jump target)
            resources:ReleaseResource(xStack_b0)
            quest:PauseAllNonScriptedEntities(false)
        else
            bVar3 = true
            pQuestionText = GetEndRoundQuestion(quest)
            quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_YES", "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_NO", "", "ArenaEnemy", (pQuestionText ~= 0))
            iVar4 = quest:MsgIsQuestionAnsweredYesOrNo()
            while iVar4 < 0 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00f212a6 end
                iVar4 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:ReleaseResource(xStack_b0)
                quest:PauseAllNonScriptedEntities(false)
                goto FLOW_after_lab_00f211ba
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if iVar4 == 1 then
                if not bVar3 then
                    quest:SetStateInt("GoldMultiplier", quest:GetStateInt("GoldMultiplier") + 1)
                    -- LAB_00f21107: (native jump target)
                    quest:FadeScreenOut(0.5, 0.5)
                    quest:Pause(0.5)
                    resources:ReleaseResource(xStack_b0)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_9c)
                    goto LAB_00f21166
                end
            else
                if bVar3 then
                    resources:ReleaseResource(xStack_b0)
                    quest:PauseAllNonScriptedEntities(false)
                    goto FLOW_after_lab_00f211ba
                end
                quest:SetStateBool("PlayerLeaving", true)
                quest:GiveHeroGold(quest:GetStateInt("GoldTotal"))
                bVar20 = false
                quest:SetStateInt("GoldTotal", 0)
                bVar3 = false
                pCVar5 = quest:GetHero()
                iVar6 = quest:AddNewConversation(pCVar5, bVar3, bVar20)
                pCVar5 = quest:GetHero()
                pCVar7 = quest:GetHero()
                quest:AddLineToConversation(iVar6, "TEXT_QST_005_V2_ARENA_KEEPER_RETURN", pCVar7, pCVar5, false)
                bVar3 = quest:IsConversationActive(iVar6)
                if bVar3 then
                    repeat
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            resources:ReleaseResource(xStack_b0)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00f211ba
                        end
                        bVar3 = quest:IsConversationActive(iVar6)
                    until not (bVar3)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    quest:FadeScreenOut(0.5, 0.5)
                    quest:Pause(0.5)
                    resources:ReleaseResource(xStack_b0)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_9c)
                    goto LAB_00f21166
                end
            end
            ::LAB_00f212a6::
            resources:ReleaseResource(xStack_b0)
            quest:PauseAllNonScriptedEntities(false)
        end
        ::FLOW_after_lab_00f211ba::
        resources:DestroyMovie(xStack_9c)
        ::FLOW_past_lab_00f20e68::
        goto FLOW_past_lab_00f20ce5
        ::LAB_00f20ce5::
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            iVar4 = quest:ReadGlobalGameData(0xa70)
            iVar6 = quest:ReadGlobalGameData(0xa6c)
            uVar11 = math.random(0, 32767)
            x_stk_cc = nil
            f_stk_f0 = 0.0
            f_stk_ec = 5.0
            iVar12 = math.random(0, 32767)
            -- TODO(native): fStack_d8 = (float)(iVar12 % 0x168) * 0.0027777778450399637;
            -- TODO(native): Math_CartesianToSpherical(xStack_f4,(int)fStack_d8);
            pCVar5 = quest:GetThingWithScriptName("ARENA_CentrePoint")
            pCVar13 = pCVar5:GetPos()
            -- TODO(native): xStack_e4._4_4_ = f_stk_f0 + pCVar13.y;
            -- TODO(native): xStack_e4._0_4_ = (float)xStack_f4 + pCVar13.x;
            -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(quest:ReadGlobalGameData(0xa6c) + (uVar11 % (uint)(iVar4 - iVar6 >> 2)) * 4),(int)&xStack_108);
            pCVar5 = quest:CreateObject("", pCVar13, xStack_118)
            x_stk_cc = pCVar5
            goto LAB_00f20e68
        end
        ::FLOW_past_lab_00f20ce5::
        ::LAB_00f212cc::
        goto LAB_00f20cc5
    end
    ::LAB_00f21166::
    ::LAB_00f2116f::
    iVar4 = i_stk_15c
    ::LAB_00f21195::
    quest:DeregisterTimer(iVar4)
    ::LAB_00f2119d::
    do return end
    ::LAB_00f20a2f::
    r3 = nil
    ::LAB_00f20ae2::
    quest:DeregisterTimer(i_stk_15c)
    r2 = nil
    r1 = nil
    -- TODO(native): iVar4 = NHeroInformationScreens::CBase::CBase__at99a2e0((CBase *)xStack_14c);
end

function GivePrizeFund(quest)
    local b2, bVar3, conversationID, native_arg_switch_2, pCVar4, pCVar5, pOther
    local alive = true
    b2 = false
    bVar3 = true
    pCVar4 = quest:GetHero()
    conversationID = quest:AddNewConversation(pCVar4, bVar3, b2)
    pCVar4 = quest:GetHero()
    quest:AddPersonToConversation(conversationID, pCVar4)
    native_arg_switch_2 = quest:GetStateInt("GoldMultiplier")
    repeat
        if native_arg_switch_2 == 0 then
            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 100)
            -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND1_PRIZE");
            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND1_PRIZE_LATER"
            break
        else
            if native_arg_switch_2 == 1 then
                quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 500)
                -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND2_PRIZE");
                pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND2_PRIZE_LATER"
                break
            else
                if native_arg_switch_2 == 2 then
                    quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 1000)
                    -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND3_PRIZE");
                    pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND3_PRIZE_LATER"
                    break
                else
                    if native_arg_switch_2 == 3 then
                        quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 2000)
                        -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND4_PRIZE");
                        pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND4_PRIZE_LATER"
                        break
                    else
                        if native_arg_switch_2 == 4 then
                            quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 3000)
                            -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND5_PRIZE");
                            pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND5_PRIZE_LATER"
                            break
                        else
                            if native_arg_switch_2 == 5 then
                                quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 4000)
                                -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND6_PRIZE");
                                pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND6_PRIZE_LATER"
                                break
                            else
                                if native_arg_switch_2 == 6 then
                                    quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 8000)
                                    -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND7_PRIZE");
                                    pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND7_PRIZE_LATER"
                                    break
                                else
                                    if native_arg_switch_2 == 7 then
                                        quest:SetStateInt("GoldTotal", quest:GetStateInt("GoldTotal") + 10000)
                                        -- TODO(native): CCharString::operator= (&xStack_4,"TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND8_PRIZE");
                                        pOther = "TEXT_QST_005_V2_ARENA_KEEPER_POST_ROUND8_PRIZE_LATER"
                                        break
                                    else
                                        goto FLOW_native_label_1
                                    end
                                end
                            end
                        end
                    end
                end
            end
        end
    until not (false)
    ::FLOW_native_label_1::
    pCVar4 = quest:GetHero()
    pCVar5 = quest:GetHero()
    quest:AddLineToConversation(conversationID, "", pCVar5, pCVar4, false)
    pCVar4 = quest:GetHero()
    pCVar5 = quest:GetHero()
    quest:AddLineToConversation(conversationID, pOther, pCVar5, pCVar4, false)
    bVar3 = quest:IsConversationActive(conversationID)
    if bVar3 then
        repeat
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00f2154a end
            bVar3 = quest:IsConversationActive(conversationID)
        until not (bVar3)
    end
    alive = not quest:IsActiveThreadTerminating()
    ::LAB_00f2154a::
end

function GetEndRoundQuestion(quest)
    local string
    local native_arg_switch_1 = quest:GetStateInt("GoldMultiplier")
    repeat
        if native_arg_switch_1 == 0 then
            string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_1"
            break
        else
            if native_arg_switch_1 == 1 then
                string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_2"
                break
            else
                if native_arg_switch_1 == 2 then
                    string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_3"
                    break
                else
                    if native_arg_switch_1 == 3 then
                        string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_4"
                        break
                    else
                        if native_arg_switch_1 == 4 then
                            string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_5"
                            break
                        else
                            if native_arg_switch_1 == 5 then
                                string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_6"
                                break
                            else
                                if native_arg_switch_1 == 6 then
                                    string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_7"
                                    break
                                else
                                    if native_arg_switch_1 == 7 then
                                        string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_8"
                                        break
                                    else
                                        string = "TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION"
                                    end
                                end
                            end
                        end
                    end
                end
            end
        end
    until not (false)
    return string
end

function helper_F14250(quest)
    -- TODO(native): return *(this + 0x44)
end

function helper_F25980(quest, native_arg_param_2)
    local iVar1, iVar3, pvVar4, uVar2, xStack_4
    if native_arg_param_2 ~= this then
        -- TODO(native): iVar3 = *native_arg_param_2
        iVar3 = nil --[[unresolved native value]]
        -- TODO(native): iVar1 = *this
        iVar1 = nil --[[unresolved native value]]
        -- TODO(native): xStack_4 = native_arg_param_2[1];
        uVar2 = (xStack_4 - iVar3) / 0x38
        -- TODO(native): if ((*(this + 8) - iVar1) / 0x38) < uVar2 then
        if false then
            iVar3 = helper_F261A0(quest, uVar2, iVar3, xStack_4)
            -- TODO(native): *(int *)this = iVar3;
            -- TODO(native): *(uint *)(this + 8) = uVar2 * 0x38 + iVar3;
        -- TODO(native): elseif ((*(this + 4) - iVar1) / 0x38) < uVar2 then
        elseif false then
            -- TODO(native): helper_F26B30(quest, iVar3, iVar1, &0)
            -- TODO(native): xStack_4 = native_arg_param_2[1];
            -- TODO(native): pvVar4 = *(this + 4)
            pvVar4 = nil --[[unresolved native value]]
            iVar3 = ((pvVar4 - *this) / 0x38) * 0x38 + *native_arg_param_2
            if iVar3 ~= xStack_4 then
                repeat
                    iVar3 = iVar3 + 0x38
                    pvVar4 = (pvVar4 + 0x38)
                until not (iVar3 ~= xStack_4)
            end
        else
            -- TODO(native): pvVar4 = helper_F26B30(quest, iVar3, iVar1, &native_arg_param_2)
            pvVar4 = nil --[[unresolved native value]]
            -- TODO(native): std::vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>,std::allocator<CIntelligentPointer<NParticleEngine::CParticleEmitter>_>_> ::_Destroy(pvVar4,*(void **)(this + 4));
        end
        -- TODO(native): *(uint *)(this + 4) = uVar2 * 0x38 + *(int *)this;
    end
    return
end

function helper_F261A0(quest, native_arg_param_2, native_arg_param_3, native_arg_param_4)
    local iVar2, pvVar1
    if native_arg_param_2 == 0 then
        pvVar1 = 0x0
    else
        pvVar1 = malloc(native_arg_param_2 * 0x38)
    end
    if native_arg_param_3 ~= native_arg_param_4 then
        iVar2 = native_arg_param_3
        repeat
            iVar2 = iVar2 + 0x38
        until not (iVar2 ~= native_arg_param_4)
    end
    return pvVar1
end

function helper_F25AC0(quest, native_arg_this)
    local puVar1, puVar2
    -- TODO(native): local puVar1 = *(native_arg_this + 4)
    -- TODO(native): puVar2 = *native_arg_this
    puVar2 = nil --[[unresolved native value]]
    while puVar2 ~= puVar1 do
        -- TODO(native): (**(code **)*puVar2)(0);
        puVar2 = puVar2 + 0xe
    end
    -- TODO(native): if *native_arg_this ~= nil then
    if false then
        -- TODO(native): free(*(void **)native_arg_this);
    end
end

function helper_F26B30(quest, native_arg_param_2)
    local iVar2
    local iVar1 = (in_EDX - this) / 0x38
    local this_00 = native_arg_param_2
    if 0 < iVar1 then
        iVar2 = this + 0x2c
        repeat
            -- TODO(native): CThingBuildingDef::operator=(this_00,iVar2 + -0x2c);
            -- TODO(native): *(undefined4 *)(this_00 + 0x28) = *(undefined4 *)(iVar2 + -4);
            helper_F26920(quest, native_arg_param_2 + (iVar2 - this), iVar2)
            this_00 = this_00 + 0x38
            iVar2 = iVar2 + 0x38
            iVar1 = iVar1 + -1
        until not (iVar1 ~= 0)
    end
    return this_00
end

function helper_F25B00(quest, native_arg_param_1, native_arg_param_2, native_arg_param_3)
    -- TODO(native): /* [bsim sim=0.8438385635009688 <- ego_r]
    -- TODO(native): void __fastcall
    if this ~= nil then
        -- TODO(native): CDefClassBase::CDefClassBase(this,in_EDX);
        -- TODO(native): name field 0x28 (undefined4)
        -- TODO(native): quest:SetStateInt("self_0x28", *(in_EDX + 0x28))
        helper_F25BB0(quest, this + 0x2c, in_EDX + 0x2c)
    end
    return
    end
end

function helper_F26920(quest, native_arg_param_2)
    local iVar3, pCVar4, pCVar5, pCVar6, uVar1, uVar2
    if native_arg_param_2 ~= this then
        iVar3 = native_arg_param_2[1]
        -- TODO(native): pCVar4 = *this
        pCVar4 = nil --[[unresolved native value]]
        -- TODO(native): pCVar5 = *native_arg_param_2
        pCVar5 = nil --[[unresolved native value]]
        uVar1 = (iVar3 - pCVar5) / 0x3c
        -- TODO(native): if ((*(this + 8) - pCVar4) / 0x3c) < uVar1 then
        if false then
            iVar3 = helper_F26AE0(quest, uVar1, pCVar5, iVar3)
            -- TODO(native): *(int *)this = iVar3;
            -- TODO(native): *(uint *)(this + 8) = uVar1 * 0x3c + iVar3;
        else
            -- TODO(native): uVar2 = (*(this + 4) - pCVar4) / 0x3c
            uVar2 = nil --[[unresolved native value]]
            if uVar2 < uVar1 then
                iVar3 = (uVar2 * 0x3c) / 0x3c
                if 0 < iVar3 then
                    repeat
                        -- TODO(native): CParentDefClassBase::operator=(pCVar4,pCVar5);
                        pCVar5 = pCVar5 + 0x3c
                        pCVar4 = pCVar4 + 0x3c
                        iVar3 = iVar3 + -1
                    until not (iVar3 ~= 0)
                end
                pCVar4 = native_arg_param_2[1]
                -- TODO(native): pCVar5 = *(this + 4)
                pCVar5 = nil --[[unresolved native value]]
                pCVar6 = (((pCVar5 - *this) / 0x3c) * 0x3c + *native_arg_param_2)
                while pCVar6 ~= pCVar4 do
                    if pCVar5 ~= nil then
                        helper_F25C60(quest, pCVar5, pCVar6)
                    end
                    pCVar5 = pCVar5 + 0x3c
                    pCVar6 = pCVar6 + 0x3c
                end
            else
                iVar3 = (iVar3 - pCVar5) / 0x3c
                if 0 < iVar3 then
                    repeat
                        -- TODO(native): CParentDefClassBase::operator=(pCVar4,pCVar5);
                        pCVar5 = pCVar5 + 0x3c
                        pCVar4 = pCVar4 + 0x3c
                        iVar3 = iVar3 + -1
                    until not (iVar3 ~= 0)
                end
                -- TODO(native): pCVar5 = *(this + 4)
                pCVar5 = nil --[[unresolved native value]]
                while pCVar4 ~= pCVar5 do
                    -- TODO(native): (*(code *)**(undefined4 **)pCVar4)(0);
                    pCVar4 = pCVar4 + 0x3c
                end
            end
        end
        -- TODO(native): *(uint *)(this + 4) = uVar1 * 0x3c + *(int *)this;
    end
    return
end

function helper_F25BB0(quest, native_arg_param_2)
    local pCVar3, this_00
    local piVar2 = native_arg_param_2
    -- TODO(native): helper_F25C20(quest, (native_arg_param_2[1] - *native_arg_param_2) / 0x3c, &native_arg_param_2)
    local pCVar1 = piVar2[1]
    -- TODO(native): local this_00 = *this
    -- TODO(native): pCVar3 = *piVar2
    pCVar3 = nil --[[unresolved native value]]
    while pCVar3 ~= pCVar1 do
        if this_00 ~= nil then
            helper_F25C60(quest, this_00, pCVar3)
        end
        this_00 = this_00 + 0x3c
        pCVar3 = pCVar3 + 0x3c
    end
    -- TODO(native): *(CParentDefClassBase **)(this + 4) = this_00;
    return
end

function helper_F26AE0(quest, native_arg_param_2, native_arg_param_3, native_arg_param_4)
    local iVar2, pvVar1
    if native_arg_param_2 == 0 then
        pvVar1 = 0x0
    else
        pvVar1 = malloc(native_arg_param_2 * 0x3c)
    end
    if native_arg_param_3 ~= native_arg_param_4 then
        iVar2 = pvVar1 - native_arg_param_3
        repeat
            if native_arg_param_3 + iVar2 ~= nil then
                helper_F25C60(quest, native_arg_param_3 + iVar2, native_arg_param_3)
            end
            native_arg_param_3 = native_arg_param_3 + 0x3c
        until not (native_arg_param_3 ~= native_arg_param_4)
    end
    return pvVar1
end

function helper_F26AA0(quest, native_arg_this)
    local puVar1, puVar2
    -- TODO(native): local puVar1 = *(native_arg_this + 4)
    -- TODO(native): puVar2 = *native_arg_this
    puVar2 = nil --[[unresolved native value]]
    while puVar2 ~= puVar1 do
        -- TODO(native): (**(code **)*puVar2)(0);
        puVar2 = puVar2 + 0xf
    end
    -- TODO(native): if *native_arg_this ~= nil then
    if false then
        -- TODO(native): free(*(void **)native_arg_this);
    end
end

function helper_F25F60(quest, native_arg_param_1)
    -- TODO(native): CThingBuildingDef::operator=((CThingBuildingDef *)this,(int)native_arg_param_1);
    -- TODO(native): name field 0x28 (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x28", *(native_arg_param_1 + 0x28))
    helper_F25F90(quest, this + 0x2c, native_arg_param_1 + 0x2c)
    -- TODO(native): name field 0x38 (undefined1)
    quest:SetStateBool("self_0x38", native_arg_quest:GetStateBool("self_0x38"))
    return
end

function helper_F25C60(quest, native_arg_param_1)
    -- TODO(native): CDefClassBase::CDefClassBase((CDefClassBase *)this,(int)native_arg_param_1);
    -- TODO(native): name field 0x28 (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x28", *(native_arg_param_1 + 0x28))
    helper_F25D20(quest, this + 0x2c, native_arg_param_1 + 0x2c)
    -- TODO(native): name field 0x38 (undefined1)
    quest:SetStateBool("self_0x38", native_arg_quest:GetStateBool("self_0x38"))
    return
end

function helper_F25C20(quest, native_arg_param_2)
    local pvVar1
    pvVar1 = 0x0
    -- TODO(native): *(undefined4 *)this = 0;
    -- TODO(native): *(undefined4 *)(this + 4) = 0;
    -- TODO(native): *(undefined4 *)(this + 8) = 0;
    if native_arg_param_2 ~= 0 then
        pvVar1 = malloc(native_arg_param_2 * 0x3c)
    end
    -- TODO(native): *(void **)(this + 8) = (void *)(native_arg_param_2 * 0x3c + (int)pvVar1);
    -- TODO(native): *(void **)this = pvVar1;
    -- TODO(native): *(void **)(this + 4) = pvVar1;
    return
end

function helper_F25F90(quest, native_arg_param_2)
    local iVar5, pCVar1, pCVar2, pCVar6, pCVar7, this_00, this_01, uVar3, uVar4
    if native_arg_param_2 ~= this then
        iVar5 = native_arg_param_2[1]
        -- TODO(native): this_00 = *this
        this_00 = nil --[[unresolved native value]]
        -- TODO(native): pCVar6 = *native_arg_param_2
        pCVar6 = nil --[[unresolved native value]]
        uVar3 = (iVar5 - pCVar6) / 0x38
        -- TODO(native): if ((*(this + 8) - this_00) / 0x38) < uVar3 then
        if false then
            iVar5 = helper_F26150(quest, uVar3, pCVar6, iVar5)
            -- TODO(native): *(int *)this = iVar5;
            -- TODO(native): *(uint *)(this + 8) = uVar3 * 0x38 + iVar5;
        else
            -- TODO(native): uVar4 = (*(this + 4) - this_00) / 0x38
            uVar4 = nil --[[unresolved native value]]
            if uVar4 < uVar3 then
                iVar5 = (uVar4 * 0x38) / 0x38
                if 0 < iVar5 then
                    repeat
                        helper_F25F10(quest, this_00, pCVar6)
                        pCVar6 = pCVar6 + 0x38
                        this_00 = this_00 + 0x38
                        iVar5 = iVar5 + -1
                    until not (iVar5 ~= 0)
                end
                pCVar2 = native_arg_param_2[1]
                -- TODO(native): this_01 = *(this + 4)
                this_01 = nil --[[unresolved native value]]
                pCVar7 = (((this_01 - *this) / 0x38) * 0x38 + *native_arg_param_2)
                while pCVar7 ~= pCVar2 do
                    if this_01 ~= nil then
                        helper_F25DD0(quest, this_01, pCVar7)
                    end
                    this_01 = this_01 + 0x38
                    pCVar7 = pCVar7 + 0x38
                end
            else
                iVar5 = (iVar5 - pCVar6) / 0x38
                if 0 < iVar5 then
                    repeat
                        helper_F25F10(quest, this_00, pCVar6)
                        pCVar6 = pCVar6 + 0x38
                        this_00 = this_00 + 0x38
                        iVar5 = iVar5 + -1
                    until not (iVar5 ~= 0)
                end
                -- TODO(native): pCVar1 = *(this + 4)
                pCVar1 = nil --[[unresolved native value]]
                while this_00 ~= pCVar1 do
                    -- TODO(native): (*(code *)**(undefined4 **)this_00)(0);
                    this_00 = this_00 + 0x38
                end
            end
        end
        -- TODO(native): *(uint *)(this + 4) = uVar3 * 0x38 + *(int *)this;
    end
    return
end

function helper_F25D20(quest, native_arg_param_2)
    local pCVar3, this_00
    local piVar2 = native_arg_param_2
    -- TODO(native): helper_F25D90(quest, (native_arg_param_2[1] - *native_arg_param_2) / 0x38, &native_arg_param_2)
    local pCVar1 = piVar2[1]
    -- TODO(native): local this_00 = *this
    -- TODO(native): pCVar3 = *piVar2
    pCVar3 = nil --[[unresolved native value]]
    while pCVar3 ~= pCVar1 do
        if this_00 ~= nil then
            helper_F25DD0(quest, this_00, pCVar3)
        end
        this_00 = this_00 + 0x38
        pCVar3 = pCVar3 + 0x38
    end
    -- TODO(native): *(COpinionDeedReactionDef **)(this + 4) = this_00;
    return
end

function helper_F26150(quest, native_arg_param_2, native_arg_param_3, native_arg_param_4)
    local iVar2, pvVar1
    if native_arg_param_2 == 0 then
        pvVar1 = 0x0
    else
        pvVar1 = malloc(native_arg_param_2 * 0x38)
    end
    if native_arg_param_3 ~= native_arg_param_4 then
        iVar2 = pvVar1 - native_arg_param_3
        repeat
            if native_arg_param_3 + iVar2 ~= nil then
                helper_F25DD0(quest, native_arg_param_3 + iVar2, native_arg_param_3)
            end
            native_arg_param_3 = native_arg_param_3 + 0x38
        until not (native_arg_param_3 ~= native_arg_param_4)
    end
    return pvVar1
end

function helper_F26110(quest, native_arg_this)
    local puVar1, puVar2
    -- TODO(native): local puVar1 = *(native_arg_this + 4)
    -- TODO(native): puVar2 = *native_arg_this
    puVar2 = nil --[[unresolved native value]]
    while puVar2 ~= puVar1 do
        -- TODO(native): (**(code **)*puVar2)(0);
        puVar2 = puVar2 + 0xe
    end
    -- TODO(native): if *native_arg_this ~= nil then
    if false then
        -- TODO(native): free(*(void **)native_arg_this);
    end
end

function helper_F25F10(quest, native_arg_param_1)
    -- TODO(native): CThingBuildingDef::operator=((CThingBuildingDef *)this,(int)native_arg_param_1);
    -- TODO(native): CCharString::operator=((CCharString *)(this + 0x28),(CCharString *)(native_arg_param_1 + 0x28));
    -- TODO(native): name field 0x2c (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x2c", *(native_arg_param_1 + 0x2c))
    -- TODO(native): CCharString::operator=((CCharString *)(this + 0x30),(CCharString *)(native_arg_param_1 + 0x30));
    -- TODO(native): name field 0x34 (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x34", *(native_arg_param_1 + 0x34))
end

function helper_F25DD0(quest, native_arg_param_1)
    -- TODO(native): CDefClassBase::CDefClassBase((CDefClassBase *)this,(int)native_arg_param_1);
    -- TODO(native): CCharString::CCharString((CCharString *)(this + 0x28),(CCharString *)(native_arg_param_1 + 0x28));
    -- TODO(native): name field 0x2c (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x2c", *(native_arg_param_1 + 0x2c))
    -- TODO(native): CCharString::CCharString((CCharString *)(this + 0x30),(CCharString *)(native_arg_param_1 + 0x30));
    -- TODO(native): name field 0x34 (undefined4)
    -- TODO(native): quest:SetStateInt("self_0x34", *(native_arg_param_1 + 0x34))
    return
end

function helper_F25D90(quest, native_arg_param_2)
    local pvVar1
    pvVar1 = 0x0
    -- TODO(native): *(undefined4 *)this = 0;
    -- TODO(native): *(undefined4 *)(this + 4) = 0;
    -- TODO(native): *(undefined4 *)(this + 8) = 0;
    if native_arg_param_2 ~= 0 then
        pvVar1 = malloc(native_arg_param_2 * 0x38)
    end
    -- TODO(native): *(void **)(this + 8) = (void *)(native_arg_param_2 * 0x38 + (int)pvVar1);
    -- TODO(native): *(void **)this = pvVar1;
    -- TODO(native): *(void **)(this + 4) = pvVar1;
    return
end

