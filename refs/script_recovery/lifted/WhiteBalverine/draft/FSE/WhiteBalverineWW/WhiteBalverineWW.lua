-- Generated native draft: Q_WhiteBalverineWW. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local CVar1, bVar13, bVar14, bVar15, bVar16, cVar7, delay, dist, pCVar11, pCVar12, pCVar9, pQuestName, r1, xStack_10, xStack_20, xStack_30
    local alive = true
    quest:AddEntityBinding("KG_Chief", "WhiteBalverineWW/Entities/KG_Chief", 1)
    bVar14 = not bVar16 and not bVar15
    quest:AddEntityBinding("WBWW_WhiteBalverine", "WhiteBalverineWW/Entities/WBWW_WhiteBalverine", 1)
    quest:AddEntityBinding("WBWW_SoldierBalverine", "WhiteBalverineWW/Entities/WBWW_SoldierBalverine", 1)
    quest:FinalizeEntityBindings()
    quest:SetCreatureGeneratorsEnabled("Witchwood4", false)
    bVar15 = quest:IsLevelLoaded("WitchWood_7")
    while not bVar15 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar15 = not alive
        if bVar15 then
            return
        end
        bVar15 = quest:IsLevelLoaded("WitchWood_7")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar15 = not alive
    if bVar15 then
        return
    end
    quest:SetQuestCardObjective("Q_WhiteBalverineKnotholeGlade", "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_07", "Witchwood4", "KnotholeGlade")
    r1 = quest:GetThingWithScriptName("MK_WBW_FIRSTSPAWNB")
    pCVar9 = quest:GetThingWithScriptName("WBWW_WhiteBalverine")
    quest:SetStateThing("WhiteBalverine", pCVar9)
    pCVar9 = quest:GetStateThing("WhiteBalverine")
    quest:SetThingPersistent(pCVar9, true)
    quest:CreateThread("MonitorBalverine")  -- native thread body MonitorBalverine: lift it as function MonitorBalverine(quest)
    if (bVar14 & 8) ~= 0 then
        bVar14 = bVar14 & 0xf7
    end
    quest:CreateThread("SpawnBalverines")  -- native thread body 0x00E19560: lift it as function SpawnBalverines(quest)
    if (bVar14 & 0x10) ~= 0 then
        bVar14 = bVar14 & 0xef
    end
    xStack_20 = resources:NewResource()
    resources:TryAcquire(xStack_20, pCVar9, 4)
    -- TODO(native): xStack_48._0_4_ = (undefined1 *)0x0;
    -- TODO(native): xStack_48._0_4_ = malloc(0x24);
    -- TODO(native): *(undefined1 *)xStack_48._0_4_ = 0;
    -- TODO(native): *(undefined4 *)(xStack_48._0_4_ + 4) = 0;
    -- TODO(native): *(undefined4 *)(xStack_48._0_4_ + 8) = xStack_48._0_4_;
    -- TODO(native): *(undefined4 *)(xStack_48._0_4_ + 0xc) = xStack_48._0_4_;
    resources:SetActor(pCVar9, "BALV", xStack_20)
    xStack_10 = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    resources:RunMacro("CS_WBW_DRINK", pCVar9, false, true)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    resources:DestroyActorMap(pCVar9)
    resources:ReleaseResource(xStack_20)
    xStack_30 = resources:NewResource()
    resources:TryAcquire(xStack_30, pCVar9, 4)
    repeat
        dist = 8.0
        pCVar11 = quest:GetHero()
        bVar15 = quest:IsDistanceBetweenThingsUnder(pCVar11, pCVar9, dist)
        bVar13 = bVar14
        if bVar15 then
            goto LAB_00e18ce8
        else
            bVar13 = bVar14 | 0x20
            cVar7 = pCVar9:MsgIsHitByHero()
            if cVar7 then goto LAB_00e18ce8 end
            bVar13 = bVar14 | 0x60
            cVar7 = pCVar9:MsgIsHitByAnySpecialAbilityFromHero()
            if cVar7 then
                bVar13 = bVar14 | 0xe0
                cVar7 = pCVar9:MsgIsHitByHeroSpecialAbility(0xe)
                if not cVar7 then goto LAB_00e18ce8 end
            end
            bVar15 = true
        end
        goto FLOW_past_lab_00e18ce8
        ::LAB_00e18ce8::
        bVar15 = false
        ::FLOW_past_lab_00e18ce8::
        if bVar13 < 0 then
            bVar13 = bVar13 & 0x7f
        end
        if (bVar13 & 0x40) ~= 0 then
            bVar13 = bVar13 & 0xbf
        end
        if (bVar13 & 0x20) ~= 0 then
            bVar14 = bVar13 & 0xdf
        end
        if not bVar15 then
            alive = not quest:IsActiveThreadTerminating()
            bVar15 = not alive
            if not bVar15 then
                resources:PrepareResource(xStack_30)
                quest:OverrideMusic(0x17, false, false)
                pCVar11 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(pCVar9, pCVar11)
                CVar1 = quest:GetStateBool("MissionFailed")
                goto LAB_00e18d99
            end
            break
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar15 = not alive
    until not (not bVar15)
    ::LAB_00e18e80::
    resources:ReleaseResource(xStack_30)
    do return end
    ::LAB_00e18d99::
    if (CVar1) or (quest:GetStateBool("MissionSucceeded")) then goto LAB_00e18dc5 end
    alive = quest:NewScriptFrame()
    alive = not quest:IsActiveThreadTerminating()
    bVar15 = not alive
    if bVar15 then goto LAB_00e18e80 end
    CVar1 = quest:GetStateBool("MissionFailed")
    goto LAB_00e18d99
    ::LAB_00e18dc5::
    alive = not quest:IsActiveThreadTerminating()
    bVar15 = not alive
    if not bVar15 then
        if not quest:GetStateBool("MissionSucceeded") then
            alive = not quest:IsActiveThreadTerminating()
            bVar15 = not alive
            if bVar15 then goto LAB_00e18e80 end
            bVar16 = true
            bVar15 = true
            pQuestName = quest:GetActiveQuestName()
            quest:SetQuestAsFailed(pQuestName, bVar15, "", bVar16)
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar15 = not alive
            if bVar15 then goto LAB_00e18e80 end
            quest:SetMasterGameState("WhiteBalverineFinished", true)
        end
        delay = 0
        pCVar12 = quest:GetActiveQuestName()
        quest:DeactivateQuestLater(pCVar12, delay)
        quest:DisplayQuestInfo(false)
    end
    goto LAB_00e18e80
end

function Init(quest)
    quest:AddQuestRegion("Q_WhiteBalverineWW", "KnotholeGlade")
    quest:AddQuestRegion("Q_WhiteBalverineWW", "Witchwood4")
    quest:AddQuestRegion("Q_WhiteBalverineWW", "DemonDoor_KnotholeGlade")
    quest:SetStateBool("MissionFailed", false)
    quest:SetStateBool("MissionSucceeded", false)
    quest:SetStateBool("WhiteBalverineAlive", true)
end

function OnPersist(quest, context)
end

function MonitorBalverine(quest)
    local bVar2
    local alive = true
    local pThing = quest:GetStateThing("WhiteBalverine")
    local r1 = quest:AddQuestInfoBarHealth(pThing, {R = 255, G = 255, B = 255, A = 255}, "HUD_QUEST_ICON_WHITE_BALVERINE", 1.0)
    quest:DisplayQuestInfo(true)
    local cVar1 = (pThing ~= nil and pThing:MsgIsKilledBy(""))
    while true do
        if cVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:SetQuestCardObjective("Q_WhiteBalverineKnotholeGlade", "TEXT_QUEST_WHITE_BALVERINE_KNOTHOLE_GLADE_OBJECTIVE_08", "KnotholeGlade", "KnotholeGlade")
                quest:SetStateBool("WhiteBalverineAlive", false)
                quest:Pause(0.5)
                quest:StopOverrideMusic(false)
                quest:GiveHeroObject("OBJECT_TROPHY_BALVERINE_FM_HEAD_01", -1, false)
                quest:SetCreatureGeneratorsEnabled("Witchwood4", true)
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then break end
        cVar1 = (pThing ~= nil and pThing:MsgIsKilledBy(""))
    end
end

function SpawnBalverines(quest)
    local resources = quest:RetailResources()
    local bVar2, bVar3, bVar5, cVar4, fret_01, iVar8, i_stk_a4, i_stk_d0, pCVar6, pPosition, r1, r2, r3, r4, this_01, this_02, xStack_1c, xStack_2c, xStack_38, xStack_44, xStack_60, xStack_bc, xStack_cc
    local alive = true
    local pThing = quest:GetStateThing("WhiteBalverine")
    local function __cleanup_LAB_00e19d25()
        resources:DestroyMovie(xStack_cc)
        this_02 = xStack_bc
        resources:ReleaseResource(this_02)
    end
    local function __cleanup_LAB_00e19d43()
        resources:ReleaseResource(this_02)
    end
    local function __cleanup_LAB_00e19d69()
        this_02 = xStack_60
        resources:ReleaseResource(this_02)
    end
    bVar3 = false
    bVar5 = false
    quest:ModifyThingHealth(pThing, 250.0, false)
    local fret_0 = quest:GetHealth(pThing)
    local i_stk_a8 = math.tointeger(math.modf(fret_0 * 0.25))
    local fret_00 = quest:GetHealth(pThing)
    local i_stk_d4 = math.tointeger(math.modf(fret_00 - i_stk_a8))
    iVar8 = 1
    i_stk_d0 = 1
    repeat
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        fret_01 = quest:GetHealth(pThing)
        if fret_01 < i_stk_d4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            r1 = quest:GetThingWithScriptName("MK_WBW_FIRSTSPAWN")
            pCVar6 = quest:GetHero()
            bVar2 = quest:IsDistanceBetweenThingsUnder(r1, pCVar6, 10.0)
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                pCVar6 = quest:GetThingWithScriptName("MK_WBW_FIRSTSPAWNB")
                r1 = pCVar6
                bVar3 = true
            end
            i_stk_a4 = 0
            if 0 < iVar8 then
                repeat
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        r1 = nil
                        return
                    end
                    if not (r1 ~= nil and not r1:IsNull()) then
                        pPosition = {x = 0, y = 0, z = 0}
                    else
                        pPosition = r1:GetPos()
                    end
                    r2 = quest:CreateCreature("CREATURE_BALVERINE_01", pPosition, "WBWW_SoldierBalverine")
                    pCVar6 = quest:GetHero()
                    quest:GiveThingBestEnemyTarget(r2, pCVar6)
                    quest:Pause(0.2)
                    r2 = nil
                    i_stk_a4 = i_stk_a4 + 1
                until not (i_stk_a4 < i_stk_d0)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                r1 = nil
                -- LAB_00e19cd7: (native jump target)
                return
            end
            quest:EntitySetAsDamageable(pThing, false)
            xStack_bc = resources:NewResource()
            resources:TryAcquire(xStack_bc, pThing, 4)
            xStack_cc = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:Pause(0.2)
            bVar2 = true
            pCVar6 = quest:GetHero()
            quest:EntitySetAttackThingImmediately(pThing, pCVar6, bVar2, true)
            quest:Pause(0.2)
            r3 = quest:PlaySoundOnThing(pThing, "SND_LONGWOLFHOWL_01")
            -- TODO(native): PlayAnimation: unresolved entity receiver/resource in quest context; arguments: 0,0,0,1,true,0,0
            -- TODO(native): IsPerformingScriptTask: unresolved entity receiver/resource in quest context; arguments: 
            iVar8 = nil --[[unresolved native result]]
            cVar4 = iVar8
            while cVar4 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    quest:PauseAllNonScriptedEntities(false)
                    __cleanup_LAB_00e19d25(); return
                end
                -- TODO(native): IsPerformingScriptTask: unresolved entity receiver/resource in quest context; arguments: 
                iVar8 = nil --[[unresolved native result]]
                cVar4 = iVar8
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                quest:PauseAllNonScriptedEntities(false)
                __cleanup_LAB_00e19d25()
                return
            end
            quest:CameraDefault()
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_cc)
            resources:PrepareResource(xStack_bc)
            resources:ReleaseResource(xStack_bc)
            quest:EntitySetAsDamageable(pThing, true)
            if not bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                xStack_60 = resources:NewResource()
                r4 = quest:GetRandomThingWithScriptName("WBWW_SoldierBalverine")
                resources:TryAcquire(xStack_60, r4, 4)
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        __cleanup_LAB_00e19d69()
                        return
                    end
                    xStack_38 = resources:NewActorMap()
                    resources:SetActor(xStack_38, "BALV", xStack_60)
                    xStack_1c = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacro("CS_WBW_FIRSTSPAWNB", xStack_38, false, true)
                    quest:Pause(0.1)
                    quest:CameraDefault()
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_1c)
                    this_01 = xStack_38
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then __cleanup_LAB_00e19d69(); return end
                    xStack_44 = resources:NewActorMap()
                    resources:SetActor(xStack_44, "BALV", xStack_60)
                    xStack_2c = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacro("CS_WBW_FIRSTSPAWN", xStack_44, false, true)
                    quest:Pause(0.1)
                    quest:CameraDefault()
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_2c)
                    this_01 = xStack_44
                end
                resources:DestroyActorMap(this_01)
                bVar5 = true
                resources:ReleaseResource(xStack_60)
            end
            iVar8 = i_stk_d0 + 1
            i_stk_d0 = iVar8
        end
        if 3 < iVar8 then
            alive = not quest:IsActiveThreadTerminating()
            return
        end
    until false
end

