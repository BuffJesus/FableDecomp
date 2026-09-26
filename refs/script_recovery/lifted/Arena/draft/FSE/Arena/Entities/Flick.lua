-- Generated native draft: Flick. Review coverage report before use.
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
    local bVar5, cVar6, c_stk_85, center, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, iVar9, pCVar12, pCVar13, pCVar14, pCVar7, pCVar8, pThing, pThing1, pcVar11, ppuVar15, this_00, uVar10, uVar4, u_stk_84, xStack_4c, xStack_80, xStack_a0
    local alive = true
    local function __cleanup_LAB_00f154eb()
        this_00 = xStack_80
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(xStack_a0)
    end
    local function __cleanup_LAB_00f154fe()
        this_00 = xStack_80
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(xStack_a0)
    end
    local function __cleanup_LAB_00f1552d()
        this_00 = xStack_4c
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(xStack_a0)
    end
    local function __cleanup_LAB_00f15531()
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(xStack_a0)
    end
    u_stk_84 = 0
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
        if bVar5 then
            return
        end
        iVar9 = quest:GetStateInt("ArenaState")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        -- LAB_00f1553a: (native jump target)
        resources:DestroyMovie(xStack_a0)
        return
    end
    quest:SetCreatureBrain(me, "BRAIN_ARENA_CELLS")
    pCVar7 = quest:GetThingWithScriptName("FlickPoint")
    pCVar8 = pCVar7:GetPos()
    center = pCVar8.x
    quest:SetWanderCentrePoint(me, pCVar8)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 5.0)
    quest:SetScriptingStateGroup(me, 4)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        -- LAB_00f154a2: (native jump target)
        return
    end
    ::LAB_00f14ca0::
    bVar5 = me:IsTalkedToByHero()
    if bVar5 then
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            resources:DestroyMovie(xStack_a0)
            return
        end
        resources:PrepareResource(xStack_a0)
        bVar5 = resources:TryAcquire(xStack_a0, me, 4)
        while not bVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:DestroyMovie(xStack_a0)
                return
            end
            bVar5 = resources:TryAcquire(xStack_a0, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            resources:DestroyMovie(xStack_a0)
            return
        end
        iVar9 = me:IsPerformingScriptTask()
        if iVar9 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:DestroyMovie(xStack_a0)
                return
            end
        end
        xStack_80 = resources:StartMovie("")
        -- TODO(native): CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities((CWideScreenMagicPauseEntities *)&xStack_90,*(int *)(this + 4));
        iVar9 = quest:GetStateInt("ArenaState")
        if (iVar9 == 3) and (not __native_entity_state:GetStateBool("EarlyTalk")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                __cleanup_LAB_00f154fe()
                return
            end
            -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_a0);
            fret_0 = quest:GetHealth(nil --[[missing]])
            fVar3 = 0.0
            if fVar3 < fret_0 then
                bVar5 = false
                pCVar14 = 0x1
                pCVar13 = 0x0
                pCVar12 = 0x0
                pcVar11 = "TEXT_QST_005_V2_ARENA_FLICK_FIRST_CHAT_PRE_CHAM"
                pCVar7 = quest:GetHero()
                iVar9 = me:IsPerformingScriptTask()
                cVar6 = iVar9
                while cVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then __cleanup_LAB_00f154eb(); return end
                    iVar9 = me:IsPerformingScriptTask()
                    cVar6 = iVar9
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then __cleanup_LAB_00f154fe(); return end
            end
            __native_entity_state:SetStateBool("EarlyTalk", true)
        elseif (iVar9 == 5) and (not __native_entity_state:GetStateBool("ChamTalk")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                __cleanup_LAB_00f154eb()
                return
            end
            -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_a0);
            fret_00 = quest:GetHealth(nil --[[missing]])
            fVar3 = 0.0
            if fVar3 < fret_00 then
                bVar5 = false
                pCVar14 = 0x1
                pCVar13 = 0x0
                pCVar12 = 0x0
                pcVar11 = "TEXT_QST_005_V2_ARENA_FLICK_FIRST_CHAT_POST_CHAM"
                pCVar7 = quest:GetHero()
                iVar9 = me:IsPerformingScriptTask()
                cVar6 = iVar9
                while cVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then __cleanup_LAB_00f154fe(); return end
                    iVar9 = me:IsPerformingScriptTask()
                    cVar6 = iVar9
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then __cleanup_LAB_00f154eb(); return end
            end
            __native_entity_state:SetStateBool("ChamTalk", true)
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then __cleanup_LAB_00f154fe(); return end
            iVar9 = __native_entity_state:GetStateInt("HintNumber")
            if iVar9 == 0 then
                -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_a0);
                fret_03 = quest:GetHealth(nil --[[missing]])
                fVar3 = 0.0
                if fVar3 < fret_03 then
                    bVar5 = false
                    pCVar14 = 0x1
                    pCVar13 = 0x0
                    pCVar12 = 0x0
                    pcVar11 = "TEXT_QST_005_V2_ARENA_FLICK_FIRST_HINT"
                    pCVar7 = quest:GetHero()
                    iVar9 = me:IsPerformingScriptTask()
                    cVar6 = iVar9
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then __cleanup_LAB_00f154eb(); return end
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                    end
                    goto LAB_00f1515c
                end
            else
                if not iVar9 then
                    if iVar9 == 2 then
                        -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_a0);
                        fret_01 = quest:GetHealth(nil --[[missing]])
                        fVar3 = 0.0
                        if fVar3 < fret_01 then
                            bVar5 = false
                            pCVar14 = 0x1
                            pCVar13 = 0x0
                            pCVar12 = 0x0
                            pcVar11 = "TEXT_QST_005_V2_ARENA_FLICK_THIRD_HINT"
                            pCVar7 = quest:GetHero()
                            iVar9 = me:IsPerformingScriptTask()
                            cVar6 = iVar9
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then __cleanup_LAB_00f154eb(); return end
                                iVar9 = me:IsPerformingScriptTask()
                                cVar6 = iVar9
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then __cleanup_LAB_00f154fe(); return end
                        end
                        __native_entity_state:SetStateInt("HintNumber", 1)
                    end
                    goto LAB_00f1516e
                end
                -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_a0);
                fret_02 = quest:GetHealth(nil --[[missing]])
                fVar3 = 0.0
                if fVar3 < fret_02 then
                    bVar5 = false
                    pCVar14 = 0x1
                    pCVar13 = 0x0
                    pCVar12 = 0x0
                    pcVar11 = "TEXT_QST_005_V2_ARENA_FLICK_SECOND_HINT"
                    pCVar7 = quest:GetHero()
                    iVar9 = me:IsPerformingScriptTask()
                    cVar6 = iVar9
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then __cleanup_LAB_00f154eb(); return end
                        iVar9 = me:IsPerformingScriptTask()
                        cVar6 = iVar9
                    end
                    goto LAB_00f1515c
                end
            end
            goto FLOW_past_lab_00f1515c
            ::LAB_00f1515c::
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then __cleanup_LAB_00f154fe(); return end
            ::FLOW_past_lab_00f1515c::
            __native_entity_state:SetStateInt("HintNumber", __native_entity_state:GetStateInt("HintNumber") + 1)
        end
        ::LAB_00f1516e::
        -- TODO(native): (**(code **)(*xStack_90 + 0x5ec))(0);
        resources:DestroyMovie(xStack_80)
    else
        uVar4 = u_stk_84
        u_stk_84 = u_stk_84 | 1
        bVar5 = me:MsgIsHitByHero()
        if bVar5 then
            goto LAB_00f15214
        else
            uVar10 = uVar4 | 3
            u_stk_84 = uVar10
            bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar5 then
                uVar10 = uVar4 | 7
                u_stk_84 = uVar10
                bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar5 then goto LAB_00f15214 end
            end
            c_stk_85 = 0
        end
        goto FLOW_past_lab_00f15214
        ::LAB_00f15214::
        c_stk_85 = 1
        ::FLOW_past_lab_00f15214::
        if (uVar10 & 4) ~= 0 then
            uVar10 = uVar10 & 0xfffffffb
            u_stk_84 = uVar10
        end
        if (uVar10 & 2) ~= 0 then
            uVar10 = uVar10 & 0xfffffffd
            u_stk_84 = uVar10
        end
        if (uVar10 & 1) ~= 0 then
            u_stk_84 = uVar10 & 0xfffffffe
        end
        if c_stk_85 == 0 then goto LAB_00f1548b end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            resources:DestroyMovie(xStack_a0)
            return
        end
        resources:PrepareResource(xStack_a0)
        bVar5 = resources:TryAcquire(xStack_a0, me, 4)
        while not bVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:DestroyMovie(xStack_a0)
                return
            end
            bVar5 = resources:TryAcquire(xStack_a0, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            resources:DestroyMovie(xStack_a0)
            return
        end
        iVar9 = me:IsPerformingScriptTask()
        if iVar9 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                resources:DestroyMovie(xStack_a0)
                return
            end
        end
        if quest:GetStateBool("InHitCutsceneAlready") then goto LAB_00f15475 end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            resources:DestroyMovie(xStack_a0)
            return
        end
        quest:SetStateBool("InHitCutsceneAlready", true)
        xStack_4c = resources:StartMovie("")
        -- TODO(native): CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities((CWideScreenMagicPauseEntities *)&xStack_8c,*(int *)(this + 4));
        -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_a0);
        fret_04 = quest:GetHealth(nil --[[missing]])
        fVar3 = 0.0
        if fVar3 < fret_04 then
            bVar5 = false
            pCVar14 = 0x1
            pCVar13 = 0x0
            pCVar12 = 0x0
            pcVar11 = "TEXT_QST_005_V2_ARENA_FLICK_ATTACKED"
            pCVar7 = quest:GetHero()
            iVar9 = me:IsPerformingScriptTask()
            cVar6 = iVar9
            while cVar6 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    -- TODO(native): (**(code **)(*(int *)xStack_8c + 0x5ec))(0);
                    __cleanup_LAB_00f1552d(); return
                end
                iVar9 = me:IsPerformingScriptTask()
                cVar6 = iVar9
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                -- TODO(native): (**(code **)(*(int *)xStack_8c + 0x5ec))(0);
                __cleanup_LAB_00f1552d()
                return
            end
        end
        quest:ModifyThingHealth(me, 10000.0, false)
        pCVar7 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(me, pCVar7)
        pThing1 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(pThing1, me)
        quest:SetStateBool("InHitCutsceneAlready", false)
        -- TODO(native): (**(code **)(*(int *)xStack_8c + 0x5ec))(0);
        resources:DestroyMovie(xStack_4c)
    end
    ::LAB_00f15475::
    resources:PrepareResource(xStack_a0)
    ::LAB_00f1548b::
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    goto LAB_00f14ca0
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

