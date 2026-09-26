-- Generated native draft: ArenaCellDoorGuard. Review coverage report before use.
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
    local bVar3, cVar4, c_stk_1f5, dist, fVar1, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, fret_08, fret_09, fret_10, fret_11, fret_12, fret_13, fret_14, fret_15, fret_16, fret_17, fret_18, fret_19, fret_20, fret_21, fret_22, fret_23, fret_24, fret_25, fret_26, fret_27, iVar5, native_arg_switch_1, p0, p2, p3, p4, pCVar10, pCVar11, pCVar12, pCVar6, pCVar7, pcVar9, ppuVar13, r1, r2, this_01, uVar2, uVar8, u_stk_1f4, xStack_184, xStack_1ec, xStack_214, xStack_224, xStack_238
    local alive = true
    u_stk_1f4 = 0
    ppuVar13 = resources:NewResource()
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    resources:PrepareResource(xStack_238)
    bVar3 = resources:TryAcquire(xStack_238, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then return end
        bVar3 = resources:TryAcquire(xStack_238, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        c_stk_1f5 = bVar3
        quest:SetThingHasInformation(me, false, true, false)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        repeat
            if bVar3 then
                return
            end
            cVar4 = me:IsTalkedToByHero()
            if cVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                iVar5 = me:IsPerformingScriptTask()
                if iVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                end
                xStack_214 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                iVar5 = quest:GetStateInt("ArenaState")
                if iVar5 ~= 3 then
                    if (iVar5 ~= 5) or (__native_entity_state:GetStateBool("ChamTalk")) then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00f19a17 end
                        pCVar6 = resources:ScriptThing(xStack_238)
                        fret_05 = quest:GetHealth(pCVar6)
                        fVar1 = 0.0
                        if fVar1 < fret_05 then
                            bVar3 = false
                            pCVar12 = 0x1
                            pCVar11 = 0x0
                            pCVar10 = 0x0
                            pcVar9 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_SECOND_QUESTION"
                            pCVar6 = quest:GetHero()
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00f19a17 end
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                this_01 = xStack_214
                                goto LAB_00f19a72
                            end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_ENTER_ARENA_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar5 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00f19a3c end
                            iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00f19a17 end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if iVar5 ~= 1 then
                            if not bVar3 then
                                pCVar6 = resources:ScriptThing(xStack_238)
                                fret_25 = quest:GetHealth(pCVar6)
                                fVar1 = 0.0
                                if fVar1 < fret_25 then
                                    bVar3 = false
                                    pCVar12 = 0x1
                                    pCVar11 = 0x0
                                    pCVar10 = 0x0
                                    pcVar9 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_ANOTHER_REFUSAL"
                                    pCVar6 = quest:GetHero()
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00f19a3c end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00f19a17 end
                                end
                                goto LAB_00f194b7
                            end
                            goto LAB_00f19a17
                        end
                        if bVar3 then goto LAB_00f19a3c end
                        quest:SetStateBool("NeedBertForSpeech", true)
                        r1 = quest:GetThingWithScriptName("ArenaCellDoorGuard2")
                        xStack_224 = resources:NewResource()
                        resources:PrepareResource(xStack_224)
                        bVar3 = resources:TryAcquire(xStack_224, r1, 4)
                        while not bVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00f19a09 end
                            bVar3 = resources:TryAcquire(xStack_224, r1, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            native_arg_switch_1 = quest:GetStateInt("ArenaRound")
                            repeat
                                if native_arg_switch_1 == 0 then
                                    pCVar6 = resources:ScriptThing(xStack_238)
                                    fret_06 = quest:GetHealth(pCVar6)
                                    fVar1 = 0.0
                                    if fVar1 < fret_06 then
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                        while cVar4 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00f19a09 end
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar4 = iVar5
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00f19a2a end
                                    end
                                    pCVar6 = resources:ScriptThing(xStack_224)
                                    fret_07 = quest:GetHealth(pCVar6)
                                    fVar1 = 0.0
                                    if fVar1 < fret_07 then
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                        while cVar4 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00f19a09 end
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar4 = iVar5
                                        end
                                        goto LAB_00f19379
                                    end
                                    goto FLOW_hoist_lab_00f19379_1
                                else
                                    if native_arg_switch_1 == 1 then
                                        pCVar6 = resources:ScriptThing(xStack_238)
                                        fret_08 = quest:GetHealth(pCVar6)
                                        fVar1 = 0.0
                                        if fVar1 < fret_08 then
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar4 = iVar5
                                            while cVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then goto LAB_00f19a09 end
                                                iVar5 = me:IsPerformingScriptTask()
                                                cVar4 = iVar5
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00f19a2a end
                                        end
                                        pCVar6 = resources:ScriptThing(xStack_224)
                                        fret_09 = quest:GetHealth(pCVar6)
                                        fVar1 = 0.0
                                        if fVar1 < fret_09 then
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar4 = iVar5
                                            while cVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then goto LAB_00f19a09 end
                                                iVar5 = me:IsPerformingScriptTask()
                                                cVar4 = iVar5
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00f19a2a end
                                        end
                                        pCVar6 = resources:ScriptThing(xStack_238)
                                        fret_10 = quest:GetHealth(pCVar6)
                                        fVar1 = 0.0
                                        if fVar1 < fret_10 then
                                            bVar3 = false
                                            pCVar12 = 0x1
                                            pCVar11 = 0x0
                                            pCVar10 = 0x0
                                            pcVar9 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND1_SECOND"
                                            pCVar6 = quest:GetHero()
                                            iVar5 = me:IsPerformingScriptTask()
                                            cVar4 = iVar5
                                            while cVar4 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then goto LAB_00f19a09 end
                                                iVar5 = me:IsPerformingScriptTask()
                                                cVar4 = iVar5
                                            end
                                            goto LAB_00f19379
                                        end
                                        break
                                    else
                                        if native_arg_switch_1 == 2 then
                                            pCVar6 = resources:ScriptThing(xStack_238)
                                            fret_11 = quest:GetHealth(pCVar6)
                                            fVar1 = 0.0
                                            if fVar1 < fret_11 then
                                                iVar5 = me:IsPerformingScriptTask()
                                                cVar4 = iVar5
                                                while cVar4 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00f19a09 end
                                                    iVar5 = me:IsPerformingScriptTask()
                                                    cVar4 = iVar5
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then goto LAB_00f19a2a end
                                            end
                                            pCVar6 = resources:ScriptThing(xStack_224)
                                            fret_12 = quest:GetHealth(pCVar6)
                                            fVar1 = 0.0
                                            if fVar1 < fret_12 then
                                                iVar5 = me:IsPerformingScriptTask()
                                                cVar4 = iVar5
                                                while cVar4 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00f19a09 end
                                                    iVar5 = me:IsPerformingScriptTask()
                                                    cVar4 = iVar5
                                                end
                                                goto LAB_00f19379
                                            end
                                            break
                                        else
                                            if native_arg_switch_1 == 3 then
                                                pCVar6 = resources:ScriptThing(xStack_238)
                                                fret_13 = quest:GetHealth(pCVar6)
                                                fVar1 = 0.0
                                                if fVar1 < fret_13 then
                                                    iVar5 = me:IsPerformingScriptTask()
                                                    cVar4 = iVar5
                                                    while cVar4 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then goto LAB_00f19a09 end
                                                        iVar5 = me:IsPerformingScriptTask()
                                                        cVar4 = iVar5
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then goto LAB_00f19a2a end
                                                end
                                                pCVar6 = resources:ScriptThing(xStack_224)
                                                fret_14 = quest:GetHealth(pCVar6)
                                                fVar1 = 0.0
                                                if fVar1 < fret_14 then
                                                    iVar5 = me:IsPerformingScriptTask()
                                                    cVar4 = iVar5
                                                    while cVar4 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then goto LAB_00f19a09 end
                                                        iVar5 = me:IsPerformingScriptTask()
                                                        cVar4 = iVar5
                                                    end
                                                    goto LAB_00f19379
                                                end
                                                break
                                            else
                                                if native_arg_switch_1 == 4 then
                                                    pCVar6 = resources:ScriptThing(xStack_238)
                                                    fret_15 = quest:GetHealth(pCVar6)
                                                    fVar1 = 0.0
                                                    if fVar1 < fret_15 then
                                                        iVar5 = me:IsPerformingScriptTask()
                                                        cVar4 = iVar5
                                                        while cVar4 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then goto LAB_00f19a09 end
                                                            iVar5 = me:IsPerformingScriptTask()
                                                            cVar4 = iVar5
                                                        end
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then goto LAB_00f19a2a end
                                                    end
                                                    pCVar6 = resources:ScriptThing(xStack_224)
                                                    fret_16 = quest:GetHealth(pCVar6)
                                                    fVar1 = 0.0
                                                    if fVar1 < fret_16 then
                                                        iVar5 = me:IsPerformingScriptTask()
                                                        cVar4 = iVar5
                                                        while cVar4 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then goto LAB_00f19a09 end
                                                            iVar5 = me:IsPerformingScriptTask()
                                                            cVar4 = iVar5
                                                        end
                                                        goto LAB_00f19379
                                                    end
                                                    break
                                                else
                                                    if native_arg_switch_1 == 5 then
                                                        pCVar6 = resources:ScriptThing(xStack_238)
                                                        fret_17 = quest:GetHealth(pCVar6)
                                                        fVar1 = 0.0
                                                        if fVar1 < fret_17 then
                                                            iVar5 = me:IsPerformingScriptTask()
                                                            cVar4 = iVar5
                                                            while cVar4 do
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if bVar3 then goto LAB_00f19a09 end
                                                                iVar5 = me:IsPerformingScriptTask()
                                                                cVar4 = iVar5
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then goto LAB_00f19a2a end
                                                        end
                                                        pCVar6 = resources:ScriptThing(xStack_224)
                                                        fret_18 = quest:GetHealth(pCVar6)
                                                        fVar1 = 0.0
                                                        if fVar1 < fret_18 then
                                                            iVar5 = me:IsPerformingScriptTask()
                                                            cVar4 = iVar5
                                                            while cVar4 do
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if bVar3 then goto LAB_00f19a09 end
                                                                iVar5 = me:IsPerformingScriptTask()
                                                                cVar4 = iVar5
                                                            end
                                                            goto LAB_00f19379
                                                        end
                                                        break
                                                    else
                                                        if native_arg_switch_1 == 6 then
                                                            pCVar6 = resources:ScriptThing(xStack_238)
                                                            fret_19 = quest:GetHealth(pCVar6)
                                                            fVar1 = 0.0
                                                            if fVar1 < fret_19 then
                                                                iVar5 = me:IsPerformingScriptTask()
                                                                cVar4 = iVar5
                                                                while cVar4 do
                                                                    alive = quest:NewScriptFrame(me)
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00f19a09 end
                                                                    iVar5 = me:IsPerformingScriptTask()
                                                                    cVar4 = iVar5
                                                                end
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                bVar3 = not alive
                                                                if bVar3 then goto LAB_00f19a2a end
                                                            end
                                                            pCVar6 = resources:ScriptThing(xStack_224)
                                                            fret_20 = quest:GetHealth(pCVar6)
                                                            fVar1 = 0.0
                                                            if fVar1 < fret_20 then
                                                                iVar5 = me:IsPerformingScriptTask()
                                                                cVar4 = iVar5
                                                                while cVar4 do
                                                                    alive = quest:NewScriptFrame(me)
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00f19a09 end
                                                                    iVar5 = me:IsPerformingScriptTask()
                                                                    cVar4 = iVar5
                                                                end
                                                                goto LAB_00f19379
                                                            end
                                                            break
                                                        else
                                                            if native_arg_switch_1 == 7 then
                                                                pCVar6 = resources:ScriptThing(xStack_238)
                                                                fret_21 = quest:GetHealth(pCVar6)
                                                                fVar1 = 0.0
                                                                if fVar1 < fret_21 then
                                                                    iVar5 = me:IsPerformingScriptTask()
                                                                    cVar4 = iVar5
                                                                    while cVar4 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then goto LAB_00f19a09 end
                                                                        iVar5 = me:IsPerformingScriptTask()
                                                                        cVar4 = iVar5
                                                                    end
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00f19a2a end
                                                                end
                                                                pCVar6 = resources:ScriptThing(xStack_224)
                                                                fret_22 = quest:GetHealth(pCVar6)
                                                                fVar1 = 0.0
                                                                if fVar1 < fret_22 then
                                                                    iVar5 = me:IsPerformingScriptTask()
                                                                    cVar4 = iVar5
                                                                    while cVar4 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then goto LAB_00f19a09 end
                                                                        iVar5 = me:IsPerformingScriptTask()
                                                                        cVar4 = iVar5
                                                                    end
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00f19a2a end
                                                                end
                                                                pCVar6 = resources:ScriptThing(xStack_238)
                                                                fret_23 = quest:GetHealth(pCVar6)
                                                                fVar1 = 0.0
                                                                if fVar1 < fret_23 then
                                                                    iVar5 = me:IsPerformingScriptTask()
                                                                    cVar4 = iVar5
                                                                    while cVar4 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then goto LAB_00f19a09 end
                                                                        iVar5 = me:IsPerformingScriptTask()
                                                                        cVar4 = iVar5
                                                                    end
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    bVar3 = not alive
                                                                    if bVar3 then goto LAB_00f19a2a end
                                                                end
                                                                pCVar6 = resources:ScriptThing(xStack_224)
                                                                fret_24 = quest:GetHealth(pCVar6)
                                                                fVar1 = 0.0
                                                                if fVar1 < fret_24 then
                                                                    bVar3 = false
                                                                    pCVar12 = 0x1
                                                                    pCVar11 = 0x0
                                                                    pCVar10 = 0x0
                                                                    pcVar9 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND7_SECOND"
                                                                    pCVar6 = quest:GetHero()
                                                                    iVar5 = me:IsPerformingScriptTask()
                                                                    cVar4 = iVar5
                                                                    while cVar4 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        bVar3 = not alive
                                                                        if bVar3 then goto LAB_00f19a09 end
                                                                        iVar5 = me:IsPerformingScriptTask()
                                                                        cVar4 = iVar5
                                                                    end
                                                                    goto LAB_00f19379
                                                                end
                                                            end
                                                        end
                                                    end
                                                end
                                            end
                                        end
                                    end
                                end
                                goto FLOW_past_lab_00f19379
                                ::LAB_00f19379::
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00f19a2a end
                                ::FLOW_hoist_lab_00f19379_1::
                                break
                                ::FLOW_past_lab_00f19379::
                            until not (false)
                            quest:FadeScreenOut(0.5, 0.5)
                            quest:Pause(0.5)
                            quest:SetStateInt("ArenaState", 6)
                            quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
                            resources:PrepareResource(xStack_224)
                            quest:SetStateBool("NeedBertForSpeech", false)
                            resources:ReleaseResource(xStack_224)
                            goto LAB_00f194b7
                        end
                        ::LAB_00f19a2a::
                        resources:ReleaseResource(xStack_224)
                        goto LAB_00f19a37
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        -- LAB_00f199e1: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        this_01 = xStack_214
                        goto LAB_00f19a72
                    end
                    pCVar6 = resources:ScriptThing(xStack_238)
                    fret_01 = quest:GetHealth(pCVar6)
                    fVar1 = 0.0
                    if fVar1 < fret_01 then
                        bVar3 = false
                        pCVar12 = 0x1
                        pCVar11 = 0x0
                        pCVar10 = 0x0
                        pcVar9 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_LATE_QUESTION"
                        pCVar6 = quest:GetHero()
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00f19a17 end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00f19a17 end
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_ENTER_ARENA_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar5 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00f19a17 end
                        iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if iVar5 == 1 then
                            if bVar3 then goto LAB_00f19a17 end
                            quest:SetStateBool("NeedBertForSpeech", true)
                            r2 = quest:GetThingWithScriptName("ArenaCellDoorGuard2")
                            xStack_1ec = resources:NewResource()
                            resources:PrepareResource(xStack_1ec)
                            bVar3 = resources:TryAcquire(xStack_1ec, r2, 4)
                            while not bVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00f199ae end
                                bVar3 = resources:TryAcquire(xStack_1ec, r2, 4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                goto LAB_00f199b9
                            end
                            goto FLOW_past_lab_00f199b9
                            ::LAB_00f199b9::
                            resources:ReleaseResource(xStack_1ec)
                            goto LAB_00f19a17
                            ::FLOW_past_lab_00f199b9::
                            pCVar6 = resources:ScriptThing(xStack_238)
                            fret_02 = quest:GetHealth(pCVar6)
                            fVar1 = 0.0
                            if fVar1 < fret_02 then
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00f199ae end
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00f199b9 end
                            end
                            pCVar6 = resources:ScriptThing(xStack_1ec)
                            fret_03 = quest:GetHealth(pCVar6)
                            fVar1 = 0.0
                            if fVar1 < fret_03 then
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00f199ae end
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00f199b9 end
                            end
                            quest:FadeScreenOut(0.5, 0.5)
                            quest:Pause(0.5)
                            quest:SetStateInt("ArenaState", 6)
                            quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
                            resources:PrepareResource(xStack_1ec)
                            quest:SetStateBool("NeedBertForSpeech", false)
                            resources:ReleaseResource(xStack_1ec)
                            __native_entity_state:SetStateBool("ChamTalk", true)
                            goto LAB_00f194b7
                        end
                        if not bVar3 then
                            pCVar6 = resources:ScriptThing(xStack_238)
                            fret_04 = quest:GetHealth(pCVar6)
                            fVar1 = 0.0
                            if fVar1 < fret_04 then
                                bVar3 = false
                                pCVar12 = 0x1
                                pCVar11 = 0x0
                                pCVar10 = 0x0
                                pcVar9 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_NO"
                                pCVar6 = quest:GetHero()
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00f19a17 end
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00f19a3c end
                            end
                            __native_entity_state:SetStateBool("ChamTalk", true)
                            goto LAB_00f194b7
                        end
                    end
                    goto LAB_00f19a3c
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    if not __native_entity_state:GetStateBool("EarlyTalk") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00f19a17 end
                        pCVar6 = resources:ScriptThing(xStack_238)
                        fret_0 = quest:GetHealth(pCVar6)
                        fVar1 = 0.0
                        if fVar1 < fret_0 then
                            bVar3 = false
                            pCVar12 = 0x1
                            pCVar11 = 0x0
                            pCVar10 = 0x0
                            pcVar9 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_EARLY_FIRST"
                            pCVar6 = quest:GetHero()
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00f19a17 end
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00f19a17 end
                        end
                        __native_entity_state:SetStateBool("EarlyTalk", true)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00f19a17 end
                        pCVar6 = resources:ScriptThing(xStack_238)
                        fret_00 = quest:GetHealth(pCVar6)
                        fVar1 = 0.0
                        if fVar1 < fret_00 then
                            bVar3 = false
                            pCVar12 = 0x1
                            pCVar11 = 0x0
                            pCVar10 = 0x0
                            pcVar9 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_EARLY_SECOND"
                            pCVar6 = quest:GetHero()
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00f19a3c end
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                -- LAB_00f199cb: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                this_01 = xStack_214
                                goto LAB_00f19a72
                            end
                        end
                    end
                    goto LAB_00f194b7
                end
                goto FLOW_past_lab_00f194b7
                ::LAB_00f194b7::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_214)
                goto LAB_00f19935
                ::FLOW_past_lab_00f194b7::
                -- LAB_00f19995: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                this_01 = xStack_214
                goto LAB_00f19a72
            end
            uVar2 = u_stk_1f4
            u_stk_1f4 = u_stk_1f4 | 1
            bVar3 = pCVar6:MsgIsHitByHero()
            if bVar3 then
                goto LAB_00f19571
            else
                uVar8 = uVar2 | 3
                bVar3 = pCVar6:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar3 then
                    uVar8 = uVar2 | 7
                    bVar3 = pCVar6:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar3 then goto LAB_00f19571 end
                end
                bVar3 = false
            end
            goto FLOW_past_lab_00f19571
            ::LAB_00f19571::
            bVar3 = true
            ::FLOW_past_lab_00f19571::
            if (uVar8 & 4) ~= 0 then
                uVar8 = uVar8 & 0xfffffffb
            end
            if (uVar8 & 2) ~= 0 then
                uVar8 = uVar8 & 0xfffffffd
            end
            if (uVar8 & 1) ~= 0 then
                uVar8 = uVar8 & 0xfffffffe
            end
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                iVar5 = me:IsPerformingScriptTask()
                if iVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                end
                if not quest:GetStateBool("InHitCutsceneAlready") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                    quest:SetStateBool("InHitCutsceneAlready", true)
                    xStack_184 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    pCVar6 = resources:ScriptThing(xStack_238)
                    fret_26 = quest:GetHealth(pCVar6)
                    fVar1 = 0.0
                    if fVar1 < fret_26 then
                        bVar3 = false
                        pCVar12 = 0x1
                        pCVar11 = 0x0
                        pCVar10 = 0x0
                        pcVar9 = "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_ATTACKED"
                        pCVar6 = quest:GetHero()
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00f19a6b
                            end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00f19a6b
                        end
                        goto FLOW_past_lab_00f19a6b
                        ::LAB_00f19a6b::
                        this_01 = xStack_184
                        goto LAB_00f19a72
                        ::FLOW_past_lab_00f19a6b::
                    end
                    quest:ModifyThingHealth(me, 10000.0, false)
                    pCVar7 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, pCVar7)
                    pCVar7 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(pCVar7, me)
                    quest:SetStateBool("InHitCutsceneAlready", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_184)
                end
            else
                dist = 1.0
                uVar8 = uVar8 | 0x18
                pCVar7 = quest:GetThingWithScriptName("GuardingDoorMarkerRight")
                bVar3 = quest:IsDistanceBetweenThingsOver(me, pCVar7, dist)
                if bVar3 then
                    iVar5 = me:IsPerformingScriptTask()
                    bVar3 = true
                    if iVar5 then goto LAB_00f197e9 end
                else
                    goto LAB_00f197e9
                end
                goto FLOW_past_lab_00f197e9
                ::LAB_00f197e9::
                bVar3 = false
                ::FLOW_past_lab_00f197e9::
                if (uVar8 & 0x10) ~= 0 then
                    uVar8 = uVar8 & 0xffffffef
                end
                if (uVar8 & 8) ~= 0 then
                    u_stk_1f4 = uVar8 & 0xfffffff7
                end
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                    pCVar6 = quest:GetThingWithScriptName("GuardingDoorMarkerRight")
                    p4 = 1
                    p3 = 0
                    p2 = 0
                    iVar5 = 0
                    p0 = pCVar6:GetPos()
                    me:MoveToPosition(p0, iVar5, p2, (p3 ~= 0), (p4 ~= 0))
                    c_stk_1f5 = 0
                else
                    if c_stk_1f5 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then break end
                        iVar5 = me:IsPerformingScriptTask()
                        if not iVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then break end
                            c_stk_1f5 = 1
                            pCVar7 = quest:GetThingWithScriptName("GuardingDoorMarkerRight")
                            bVar3 = false
                            fret_27 = pCVar7:GetAngleXY()
                            quest:EntitySetFacingAngle(me, fret_27, bVar3)
                        end
                    end
                end
            end
            ::LAB_00f19935::
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
        until false
    end
    goto LAB_00f19a7b
    ::LAB_00f19a09::
    resources:ReleaseResource(xStack_224)
    ::LAB_00f19a17::
    quest:PauseAllNonScriptedEntities(false)
    this_01 = xStack_214
    goto LAB_00f19a72
    ::LAB_00f199ae::
    resources:ReleaseResource(xStack_1ec)
    ::LAB_00f19a37::
    ::LAB_00f19a3c::
    quest:PauseAllNonScriptedEntities(false)
    this_01 = xStack_214
    ::LAB_00f19a72::
    resources:DestroyMovie(this_01)
    ::LAB_00f19a7b::
    resources:ReleaseResource(xStack_238)
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

