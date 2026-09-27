-- Generated native draft: ArenaCellDoorGuard2. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local resources = quest:RetailResources()
    local bVar4, b_stk_b5, cVar5, dist, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, iVar11, iVar13, iVar15, iVar17, native_arg_sequence_1, pCVar12, pCVar14, pCVar16, pCVar6, pCVar7, pRelativeTo, pThing1, pcVar10, ppuVar18, this_00, uVar8, uVar9, u_stk_b0, xStack_64, xStack_74, xStack_c8
    local alive = true
    u_stk_b0 = 0
    ppuVar18 = resources:NewResource()
    pCVar6 = quest:GetNearestWithDefName(me, "VILLAGE_ARENA_CELLS")
    quest:SetStateThing("CellsVillage", pCVar6)
    pCVar6 = nil
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    iVar11 = quest:GetStateInt("ArenaState")
    while iVar11 == 2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            return
        end
        iVar11 = quest:GetStateInt("ArenaState")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        resources:PrepareResource(xStack_c8)
        bVar4 = resources:TryAcquire(xStack_c8, me, 4)
        while not bVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00f1a7f6 end
            bVar4 = resources:TryAcquire(xStack_c8, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        b_stk_b5 = not alive
        native_arg_sequence_1 = false
        if not b_stk_b5 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if native_arg_sequence_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then
            repeat
                if quest:GetStateInt("ArenaState") == 4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then break end
                    quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
                    resources:PrepareResource(xStack_c8)
                    iVar11 = quest:GetStateInt("ArenaState")
                    while iVar11 == 4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00f1a7f6 end
                        iVar11 = quest:GetStateInt("ArenaState")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then break end
                    quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
                    resources:PrepareResource(xStack_c8)
                    bVar4 = resources:TryAcquire(xStack_c8, me, 4)
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00f1a7f6 end
                        bVar4 = resources:TryAcquire(xStack_c8, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then break end
                    pCVar6 = quest:GetThingWithScriptName("GuardingDoorMarkerLeft")
                    iVar17 = 1
                    iVar15 = 0
                    iVar13 = 1
                    iVar11 = 0
                    pCVar7 = pCVar6:GetPos()
                    me:MoveToPosition(pCVar7, iVar11, iVar13, (iVar15 ~= 0), (iVar17 ~= 0))
                    b_stk_b5 = false
                end
                if quest:GetStateBool("NeedBertForSpeech") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then break end
                    quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
                    resources:PrepareResource(xStack_c8)
                    cVar5 = quest:GetStateBool("NeedBertForSpeech")
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00f1a7f6 end
                        cVar5 = quest:GetStateBool("NeedBertForSpeech")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then break end
                    resources:PrepareResource(xStack_c8)
                    bVar4 = resources:TryAcquire(xStack_c8, me, 4)
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00f1a7f6 end
                        bVar4 = resources:TryAcquire(xStack_c8, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then break end
                end
                bVar4 = me:IsTalkedToByHero()
                if bVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then break end
                    iVar11 = me:IsPerformingScriptTask()
                    if iVar11 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then break end
                    end
                    xStack_74 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    iVar11 = quest:GetStateInt("ArenaState")
                    if (iVar11 == 3) and (not __native_entity_state:GetStateBool("EarlyTalk")) then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            goto LAB_00f1a7b7
                        end
                        goto FLOW_past_lab_00f1a7b7
                        ::LAB_00f1a7b7::
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = xStack_74
                        -- LAB_00f1a7ed: (native jump target)
                        resources:DestroyMovie(this_00)
                        break
                        ::FLOW_past_lab_00f1a7b7::
                        pCVar6 = resources:ScriptThing(xStack_c8)
                        fret_0 = quest:GetHealth(pCVar6)
                        fVar3 = 0.0
                        if fVar3 < fret_0 then
                            bVar4 = false
                            pCVar16 = 0x1
                            pCVar14 = 0x0
                            pCVar12 = 0x0
                            pcVar10 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_EARLY_FIRST"
                            pCVar6 = quest:GetHero()
                            iVar11 = me:IsPerformingScriptTask()
                            cVar5 = iVar11
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then goto LAB_00f1a7a3 end
                                iVar11 = me:IsPerformingScriptTask()
                                cVar5 = iVar11
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then return end  -- TODO(native): goto LAB_00f1a7b7
                        end
                        __native_entity_state:SetStateBool("EarlyTalk", true)
                    else
                        if (iVar11 == 5) and (not __native_entity_state:GetStateBool("ChamTalk")) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                pCVar6 = resources:ScriptThing(xStack_c8)
                                fret_00 = quest:GetHealth(pCVar6)
                                fVar3 = 0.0
                                if fVar3 < fret_00 then
                                    bVar4 = false
                                    pCVar16 = 0x1
                                    pCVar14 = 0x0
                                    pCVar12 = 0x0
                                    pcVar10 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_LATE"
                                    pCVar6 = quest:GetHero()
                                    iVar11 = me:IsPerformingScriptTask()
                                    cVar5 = iVar11
                                    while cVar5 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then return end  -- TODO(native): goto LAB_00f1a7b7
                                        iVar11 = me:IsPerformingScriptTask()
                                        cVar5 = iVar11
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then goto LAB_00f1a7a3 end
                                end
                                __native_entity_state:SetStateBool("ChamTalk", true)
                                goto LAB_00f1a343
                            end
                            goto LAB_00f1a7a3
                        end
                        goto FLOW_hoist_lab_00f1a7a3_1
                    end
                    goto FLOW_past_lab_00f1a7a3
                    ::LAB_00f1a7a3::
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = xStack_74
                    -- TODO(native): goto LAB_00f1a7ed
                    ::FLOW_hoist_lab_00f1a7a3_1::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then return end  -- TODO(native): goto LAB_00f1a7b7
                    pCVar6 = resources:ScriptThing(xStack_c8)
                    fret_01 = quest:GetHealth(pCVar6)
                    fVar3 = 0.0
                    if fVar3 < fret_01 then
                        bVar4 = false
                        pCVar16 = 0x1
                        pCVar14 = 0x0
                        pCVar12 = 0x0
                        pcVar10 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_VERY_LATE"
                        pCVar6 = quest:GetHero()
                        iVar11 = me:IsPerformingScriptTask()
                        cVar5 = iVar11
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00f1a7a3 end
                            iVar11 = me:IsPerformingScriptTask()
                            cVar5 = iVar11
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then return end  -- TODO(native): goto LAB_00f1a7b7
                    end
                    ::FLOW_past_lab_00f1a7a3::
                    ::LAB_00f1a343::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_74)
                else
                    dist = 1.0
                    uVar8 = u_stk_b0 | 3
                    pCVar6 = quest:GetThingWithScriptName("GuardingDoorMarkerLeft")
                    bVar4 = quest:IsDistanceBetweenThingsOver(me, pCVar6, dist)
                    if bVar4 then
                        iVar11 = me:IsPerformingScriptTask()
                        if iVar11 then goto LAB_00f1a3b6 end
                    else
                    end
                    ::LAB_00f1a3b6::
                    if (uVar8 & 2) ~= 0 then
                        uVar8 = uVar8 & 0xfffffffd
                    end
                    if (uVar8 & 1) ~= 0 then
                        uVar8 = uVar8 & 0xfffffffe
                    end
                    if (in_stack_ffffff34 & 0xffffff >> 0x18) == 0 then
                        uVar9 = uVar8 | 4
                        u_stk_b0 = uVar9
                        bVar4 = me:MsgIsHitByHero()
                        if bVar4 then
                        else
                            uVar9 = uVar8 | 0xc
                            u_stk_b0 = uVar9
                            bVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
                            if bVar4 then
                                uVar9 = uVar8 | 0x1c
                                u_stk_b0 = uVar9
                                bVar4 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                if not bVar4 then goto LAB_00f1a4e2 end
                            end
                        end
                        ::LAB_00f1a4e2::
                        if (uVar9 & 0x10) ~= 0 then
                            uVar9 = uVar9 & 0xffffffef
                            u_stk_b0 = uVar9
                        end
                        if (uVar9 & 8) ~= 0 then
                            uVar9 = uVar9 & 0xfffffff7
                            u_stk_b0 = uVar9
                        end
                        if (uVar9 & 4) ~= 0 then
                            u_stk_b0 = uVar9 & 0xfffffffb
                        end
                        if (in_stack_ffffff34 & 0xffffff >> 0x18) == 0 then
                            if b_stk_b5 == false then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then break end
                                iVar11 = me:IsPerformingScriptTask()
                                if not iVar11 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then break end
                                    b_stk_b5 = true
                                    pCVar6 = quest:GetThingWithScriptName("GuardingDoorMarkerLeft")
                                    bVar4 = false
                                    fret_03 = pCVar6:GetAngleXY()
                                    quest:EntitySetFacingAngle(me, fret_03, bVar4)
                                end
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then break end
                            iVar11 = me:IsPerformingScriptTask()
                            if iVar11 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then break end
                            end
                            if not quest:GetStateBool("InHitCutsceneAlready") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then break end
                                quest:SetStateBool("InHitCutsceneAlready", true)
                                xStack_64 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                pCVar6 = resources:ScriptThing(xStack_c8)
                                fret_02 = quest:GetHealth(pCVar6)
                                fVar3 = 0.0
                                if fVar3 < fret_02 then
                                    bVar4 = false
                                    pCVar16 = 0x1
                                    pCVar14 = 0x0
                                    pCVar12 = 0x0
                                    pcVar10 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_ATTACKED"
                                    pCVar6 = quest:GetHero()
                                    iVar11 = me:IsPerformingScriptTask()
                                    cVar5 = iVar11
                                    while cVar5 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            goto LAB_00f1a7e9
                                        end
                                        iVar11 = me:IsPerformingScriptTask()
                                        cVar5 = iVar11
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        goto LAB_00f1a7e9
                                    end
                                    goto FLOW_past_lab_00f1a7e9
                                    ::LAB_00f1a7e9::
                                    this_00 = xStack_64
                                    -- TODO(native): goto LAB_00f1a7ed
                                    ::FLOW_past_lab_00f1a7e9::
                                end
                                quest:ModifyThingHealth(me, 10000.0, false)
                                pCVar6 = quest:GetHero()
                                quest:EntitySetThingAsAllyOfThing(me, pCVar6)
                                pThing1 = quest:GetHero()
                                quest:EntitySetThingAsAllyOfThing(pThing1, me)
                                quest:SetStateBool("InHitCutsceneAlready", false)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_64)
                            end
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then break end
                        pCVar6 = quest:GetThingWithScriptName("GuardingDoorMarkerLeft")
                        iVar17 = 1
                        iVar15 = 0
                        iVar13 = 0
                        iVar11 = 0
                        pCVar7 = pCVar6:GetPos()
                        me:MoveToPosition(pCVar7, iVar11, iVar13, (iVar15 ~= 0), (iVar17 ~= 0))
                        b_stk_b5 = false
                    end
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    resources:DestroyMovie(xStack_c8)
                    return
                end
            until false
        end
    end
    ::LAB_00f1a7f6::
    resources:ReleaseResource(ppuVar18)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("EarlyTalk", false)
    __native_entity_state:SetStateBool("ChamTalk", false)
    quest:EntitySetAllStategroupsEnabled(me, false)
    quest:EntitySetCombatEnabled(me, false)
end

function OnPersist(quest, me, context)
    local earlyTalk = quest:GetStateBool("EarlyTalk") or false
    earlyTalk = quest:PersistTransferBool(context, "EarlyTalk", earlyTalk)
    quest:SetStateBool("EarlyTalk", earlyTalk)
    local chamTalk = quest:GetStateBool("ChamTalk") or false
    chamTalk = quest:PersistTransferBool(context, "ChamTalk", chamTalk)
    quest:SetStateBool("ChamTalk", chamTalk)
end

function OnPredicateFail(quest, me)
end

