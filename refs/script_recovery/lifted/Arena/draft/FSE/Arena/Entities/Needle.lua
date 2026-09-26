-- Generated native draft: Needle. Review coverage report before use.
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
    local bVar5, cVar6, c_stk_91, center, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, iVar9, native_arg_switch_2, pCVar12, pCVar13, pCVar14, pCVar7, pCVar8, pThing, pThing1, pcVar11, ppuVar15, this_00, uVar10, uVar4, u_stk_90, xStack_58, xStack_8c, xStack_ac
    local alive = true
    local function __cleanup_LAB_00f1612b()
        this_00 = xStack_8c
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(xStack_ac)
    end
    local function __cleanup_LAB_00f1613e()
        this_00 = xStack_8c
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(xStack_ac)
    end
    local function __cleanup_LAB_00f1616d()
        this_00 = xStack_58
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(xStack_ac)
    end
    local function __cleanup_LAB_00f16171()
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(xStack_ac)
    end
    u_stk_90 = 0
    ppuVar15 = resources:NewResource()
    quest:EntityUnsetAsOpinionSource(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    iVar9 = quest:GetStateInt("ArenaState")
    while iVar9 == 2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then return end
        iVar9 = quest:GetStateInt("ArenaState")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        -- LAB_00f1617a: (native jump target)
        resources:DestroyMovie(xStack_ac)
        return
    end
    quest:SetCreatureBrain(me, "BRAIN_ARENA_CELLS")
    pCVar7 = quest:GetThingWithScriptName("NeedlePoint")
    pCVar8 = pCVar7:GetPos()
    center = pCVar8.x
    quest:SetWanderCentrePoint(me, pCVar8)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 5.0)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    repeat
        if bVar5 then
            return
        end
        bVar5 = me:IsTalkedToByHero()
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:DestroyMovie(xStack_ac)
                return
            end
            resources:PrepareResource(xStack_ac)
            bVar5 = resources:TryAcquire(xStack_ac, me, 4)
            while not bVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:DestroyMovie(xStack_ac)
                    return
                end
                bVar5 = resources:TryAcquire(xStack_ac, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:DestroyMovie(xStack_ac)
                return
            end
            iVar9 = me:IsPerformingScriptTask()
            if iVar9 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:DestroyMovie(xStack_ac)
                    return
                end
            end
            xStack_8c = resources:StartMovie("")
            -- TODO(native): CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities((CWideScreenMagicPauseEntities *)&xStack_9c,*(int *)(this + 4));
            iVar9 = quest:GetStateInt("ArenaState")
            if (iVar9 == 3) and (not __native_entity_state:GetStateBool("EarlyTalk")) then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_ac);
                    fret_0 = quest:GetHealth(nil --[[missing]])
                    fVar3 = 0.0
                    if fVar3 < fret_0 then
                        bVar5 = false
                        pCVar14 = 0x1
                        pCVar13 = 0x0
                        pCVar12 = 0x0
                        pcVar11 = "TEXT_QST_005_V2_ARENA_NEEDLE_FIRST_CHAT_PRE_CHAM"
                        pCVar7 = quest:GetHero()
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- TODO(native): (**(code **)(*xStack_9c + 0x5ec))(0);
                                this_00 = xStack_8c
                                __cleanup_LAB_00f16171(); return
                            end
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then __cleanup_LAB_00f1613e(); return end
                    end
                    __native_entity_state:SetStateBool("EarlyTalk", true)
                    goto FLOW_native_label_1
                end
                __cleanup_LAB_00f1613e()
                return
            end
            if (iVar9 == 5) and (not __native_entity_state:GetStateBool("ChamTalk")) then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_ac);
                    fret_00 = quest:GetHealth(nil --[[missing]])
                    fVar3 = 0.0
                    if fVar3 < fret_00 then
                        bVar5 = false
                        pCVar14 = 0x1
                        pCVar13 = 0x0
                        pCVar12 = 0x0
                        pcVar11 = "TEXT_QST_005_V2_ARENA_NEEDLE_FIRST_CHAT_POST_CHAM"
                        pCVar7 = quest:GetHero()
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00f1613e(); return end
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then __cleanup_LAB_00f1612b(); return end
                    end
                    __native_entity_state:SetStateBool("ChamTalk", true)
                    goto FLOW_native_label_1
                end
                __cleanup_LAB_00f1612b()
                return
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then __cleanup_LAB_00f1613e(); return end
            native_arg_switch_2 = __native_entity_state:GetStateInt("HintNumber")
            if not (native_arg_switch_2 == 0 or native_arg_switch_2 == 1 or native_arg_switch_2 == 2 or native_arg_switch_2 == 3) then
                native_arg_switch_2 = 0x7ffffffe
            end
            repeat
                if native_arg_switch_2 == 0 then
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_ac);
                    fret_01 = quest:GetHealth(nil --[[missing]])
                    fVar3 = 0.0
                    if fVar3 < fret_01 then
                        bVar5 = false
                        pCVar14 = 0x1
                        pCVar13 = 0x0
                        pCVar12 = 0x0
                        pcVar11 = "TEXT_QST_005_V2_ARENA_NEEDLE_FIRST_HINT"
                        pCVar7 = quest:GetHero()
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00f1612b(); return end
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                        end
                        goto LAB_00f15cec
                    end
                    goto FLOW_hoist_lab_00f15cec_1
                end
                goto FLOW_past_lab_00f15cec
                ::LAB_00f15cec::
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then __cleanup_LAB_00f1613e(); return end
                ::FLOW_hoist_lab_00f15cec_1::
                break
                ::FLOW_past_lab_00f15cec::
                if native_arg_switch_2 == 1 then
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_ac);
                    fret_02 = quest:GetHealth(nil --[[missing]])
                    fVar3 = 0.0
                    if fVar3 < fret_02 then
                        bVar5 = false
                        pCVar14 = 0x1
                        pCVar13 = 0x0
                        pCVar12 = 0x0
                        pcVar11 = "TEXT_QST_005_V2_ARENA_NEEDLE_SECOND_HINT"
                        pCVar7 = quest:GetHero()
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00f1612b(); return end
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                        end
                        goto LAB_00f15cec
                    end
                    break
                end
                if native_arg_switch_2 == 2 then
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_ac);
                    fret_03 = quest:GetHealth(nil --[[missing]])
                    fVar3 = 0.0
                    if fVar3 < fret_03 then
                        bVar5 = false
                        pCVar14 = 0x1
                        pCVar13 = 0x0
                        pCVar12 = 0x0
                        pcVar11 = "TEXT_QST_005_V2_ARENA_NEEDLE_THIRD_HINT"
                        pCVar7 = quest:GetHero()
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00f1612b(); return end
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                        end
                        goto LAB_00f15cec
                    end
                    break
                end
                if native_arg_switch_2 == 3 then
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_ac);
                    fret_04 = quest:GetHealth(nil --[[missing]])
                    fVar3 = 0.0
                    if fVar3 < fret_04 then
                        bVar5 = false
                        pCVar14 = 0x1
                        pCVar13 = 0x0
                        pCVar12 = 0x0
                        pcVar11 = "TEXT_QST_005_V2_ARENA_NEEDLE_FOURTH_HINT"
                        pCVar7 = quest:GetHero()
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00f1612b(); return end
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then __cleanup_LAB_00f1613e(); return end
                    end
                    __native_entity_state:SetStateInt("HintNumber", 2)
                    native_arg_switch_2 = 0x7ffffffe
                end
                if native_arg_switch_2 == 0x7ffffffe then goto FLOW_native_label_1 end
            until not (false)
            __native_entity_state:SetStateInt("HintNumber", __native_entity_state:GetStateInt("HintNumber") + 1)
            ::FLOW_native_label_1::
            -- TODO(native): (**(code **)(*xStack_9c + 0x5ec))(0);
            resources:DestroyMovie(xStack_8c)
        else
            uVar4 = u_stk_90
            u_stk_90 = u_stk_90 | 1
            bVar5 = me:MsgIsHitByHero()
            if bVar5 then
                goto LAB_00f15e52
            else
                uVar10 = uVar4 | 3
                u_stk_90 = uVar10
                bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar5 then
                    uVar10 = uVar4 | 7
                    u_stk_90 = uVar10
                    bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar5 then goto LAB_00f15e52 end
                end
                c_stk_91 = 0
            end
            goto FLOW_past_lab_00f15e52
            ::LAB_00f15e52::
            c_stk_91 = 1
            ::FLOW_past_lab_00f15e52::
            if (uVar10 & 4) ~= 0 then
                uVar10 = uVar10 & 0xfffffffb
                u_stk_90 = uVar10
            end
            if (uVar10 & 2) ~= 0 then
                uVar10 = uVar10 & 0xfffffffd
                u_stk_90 = uVar10
            end
            if (uVar10 & 1) ~= 0 then
                u_stk_90 = uVar10 & 0xfffffffe
            end
            if c_stk_91 == 0 then goto LAB_00f160cb end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:DestroyMovie(xStack_ac)
                return
            end
            resources:PrepareResource(xStack_ac)
            bVar5 = resources:TryAcquire(xStack_ac, me, 4)
            while not bVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:DestroyMovie(xStack_ac)
                    return
                end
                bVar5 = resources:TryAcquire(xStack_ac, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:DestroyMovie(xStack_ac)
                return
            end
            iVar9 = me:IsPerformingScriptTask()
            if iVar9 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    resources:DestroyMovie(xStack_ac)
                    return
                end
            end
            if not quest:GetStateBool("InHitCutsceneAlready") then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    quest:SetStateBool("InHitCutsceneAlready", true)
                    xStack_58 = resources:StartMovie("")
                    -- TODO(native): CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities((CWideScreenMagicPauseEntities *)&xStack_98,*(int *)(this + 4));
                    -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_ac);
                    fret_05 = quest:GetHealth(nil --[[missing]])
                    fVar3 = 0.0
                    if fVar3 < fret_05 then
                        bVar5 = false
                        pCVar14 = 0x1
                        pCVar13 = 0x0
                        pCVar12 = 0x0
                        pcVar11 = "TEXT_QST_005_V2_ARENA_NEEDLE_ATTACKED"
                        pCVar7 = quest:GetHero()
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- TODO(native): (**(code **)(*(int *)xStack_98 + 0x5ec))(0);
                                __cleanup_LAB_00f1616d(); return
                            end
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            -- TODO(native): (**(code **)(*(int *)xStack_98 + 0x5ec))(0);
                            __cleanup_LAB_00f1616d()
                            return
                        end
                    end
                    quest:ModifyThingHealth(me, 10000.0, false)
                    pCVar7 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, pCVar7)
                    pThing1 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(pThing1, me)
                    quest:SetStateBool("InHitCutsceneAlready", false)
                    -- TODO(native): (**(code **)(*(int *)xStack_98 + 0x5ec))(0);
                    resources:DestroyMovie(xStack_58)
                    goto LAB_00f160b5
                end
                resources:DestroyMovie(xStack_ac)
                return
            end
        end
        ::LAB_00f160b5::
        resources:PrepareResource(xStack_ac)
        ::LAB_00f160cb::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
    until false
end

function Init(quest, me)
    __native_entity_state:SetStateBool("EarlyTalk", false)
    __native_entity_state:SetStateBool("ChamTalk", false)
    __native_entity_state:SetStateInt("HintNumber", 0)
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

function OnPersist(quest, me, context)
    local earlyTalk = quest:GetStateBool("EarlyTalk") or false
    earlyTalk = quest:PersistTransferBool(context, "EarlyTalk", earlyTalk)
    quest:SetStateBool("EarlyTalk", earlyTalk)
    local chamTalk = quest:GetStateBool("ChamTalk") or false
    chamTalk = quest:PersistTransferBool(context, "ChamTalk", chamTalk)
    quest:SetStateBool("ChamTalk", chamTalk)
    local hintNumber = quest:GetStateInt("HintNumber") or 0
    hintNumber = quest:PersistTransferInt(context, "HintNumber", hintNumber)
    quest:SetStateInt("HintNumber", hintNumber)
end

function OnPredicateFail(quest, me)
end

