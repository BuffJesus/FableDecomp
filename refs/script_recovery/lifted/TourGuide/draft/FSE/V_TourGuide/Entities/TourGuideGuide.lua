-- Generated native draft: TourGuideGuide. Review coverage report before use.
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
    local __native_condition_1, bVar4, cVar5, fVar13, iVar6, iVar7, i_stk_a0, i_stk_a4, native_arg_sequence_1, pCVar8, pThing, puVar10, pvVar11, r1, r2, r3, uVar12, u_stk_7c, xStack_80, xStack_8c, xStack_9c
    local alive = true
    u_stk_7c = 0
    xStack_9c = resources:NewResource()
    resources:PrepareResource(xStack_9c)
    bVar4 = resources:TryAcquire(xStack_9c, me, 4)
    while not bVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            resources:ReleaseResource(xStack_9c)
            return
        end
        bVar4 = resources:TryAcquire(xStack_9c, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        resources:ReleaseResource(xStack_9c)
        return
    end
    iVar6 = quest:RegisterTimer()
    i_stk_a4 = iVar6
    iVar7 = quest:RegisterTimer()
    i_stk_a0 = iVar7
    quest:SetTimer(i_stk_a0, 0)
    xStack_8c = nil
    quest:SetStateBool("Initialise", true)
    quest:SetStateBool("TourGuideKilled", false)
    quest:SetStateInt("WaypointCounter", 0)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    while true do
        if bVar4 then
            xStack_8c = nil
            quest:DeregisterTimer(i_stk_a0)
            quest:DeregisterTimer(i_stk_a4)
            resources:ReleaseResource(xStack_9c)
            return
        end
        if (quest:GetStateBool("AllFollowersDead")) or (quest:GetStateBool("TourFinished")) then break end
        if quest:GetStateBool("Initialise") then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                quest:DeregisterTimer(i_stk_a0)
                quest:DeregisterTimer(i_stk_a4)
                resources:ReleaseResource(xStack_9c)
                return
            end
            resources:PrepareResource(xStack_9c)
            bVar4 = resources:TryAcquire(xStack_9c, me, 4)
            while not bVar4 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00ee6817 end
                bVar4 = resources:TryAcquire(xStack_9c, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00ee6817 end
            if __native_entity_state:GetStateInt("saveWaypointIdx") == -1 then
                cVar5 = quest:GetStateThing("NextTourWaypoint"):IsAlive()
                if (not cVar5) or (0x11 < quest:GetStateInt("WaypointCounter")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00ee6817 end
                    pCVar8 = quest:GetThingWithScriptName("M_TG_LocationStart")
                    quest:SetStateThing("NextTourWaypoint", pCVar8)
                    quest:SetStateInt("WaypointCounter", 0)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00ee6817 end
                    pCVar8 = quest:GetThingWithScriptName(nil --[[missing]])
                    quest:SetStateThing("NextTourWaypoint", pCVar8)
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00ee6817 end
                quest:SetStateInt("WaypointCounter", __native_entity_state:GetStateInt("saveWaypointIdx") + 1)
                if 0x11 < quest:GetStateInt("WaypointCounter") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00ee6817 end
                    quest:SetStateInt("WaypointCounter", 0)
                end
                pCVar8 = quest:GetThingWithScriptName(nil --[[missing]])
                quest:SetStateThing("NextTourWaypoint", pCVar8)
                __native_entity_state:SetStateInt("saveWaypointIdx", 0xffffffff)
            end
            -- TODO(native): helper_EE6850(quest, me, *(this + 0x14), (*(this + 0x14) + 0x168), xStack_9c)
            if not quest:GetStateBool("SpawnedQuestFinishThread") then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00ee6817 end
                quest:CreateThread("WatchForNoFollowers")  -- native thread body NScript::CV_TourGuideScript::WatchForNoFollowers: lift it as function WatchForNoFollowers(quest)
                if (u_stk_7c & 4) ~= 0 then
                    uVar12 = u_stk_7c & 0xfffffffb
                    u_stk_7c = uVar12
                end
                if (uVar12 & 2) ~= 0 then
                    uVar12 = uVar12 & 0xfffffffd
                    u_stk_7c = uVar12
                end
                if (uVar12 & 1) ~= 0 then
                    u_stk_7c = uVar12 & 0xfffffffe
                end
                quest:SetStateBool("SpawnedQuestFinishThread", true)
            end
            quest:SetStateBool("Initialise", false)
            quest:SetStateBool("GuideSpokenToHeroThisWaypoint", false)
            quest:SetStateBool("OverheardTourGuideThisWaypoint", false)
        end
        iVar6 = math.random(0, 32767)
        if iVar6 % 0x32 == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00ee6817 end
            r1 = quest:GetRandomThingWithScriptName("TourGuideFollower")
            iVar6 = (r1 ~= nil and r1:IsAlive())
            if iVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    goto LAB_00ee6817
                end
                quest:EntityForceToLookAtThing(me, r1)
            end
        end
        fVar13 = quest:ReadGlobalGameDataFloat(0x8d4)
        pCVar8 = quest:GetHero()
        bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar13)
        if (bVar4) and (not quest:GetStateBool("OverheardTourGuideThisWaypoint")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00ee6817 end
            -- TODO(native): CCharString::CCharString(&xStack_68,(CCharString *)(*(int *)(this + 0x14) + 0x4c + *(int *)(*(int *)(this + 0x14) + 0x164) * 0xc));
            if not quest:GetStateBool("TourGuideKilled") then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    -- LAB_00ee6533: (native jump target)
                    goto LAB_00ee6817
                end
                r2 = quest:GetRandomThingWithScriptName("TourGuideFollower")
                iVar6 = (r2 ~= nil and r2:IsAlive())
                if iVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        goto LAB_00ee6558
                    end
                    iVar6 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(iVar6, r2)
                    quest:AddLineToConversation(iVar6, "", me, r2, false)
                end
            end
            quest:SetStateBool("OverheardTourGuideThisWaypoint", true)
        end
        fVar13 = quest:ReadGlobalGameDataFloat(0x8d8)
        pCVar8 = quest:GetHero()
        bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, fVar13)
        if bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then goto LAB_00ee6817 end
            bVar4 = me:IsTalkedToByHero()
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    resources:PrepareResource(xStack_9c)
                    bVar4 = resources:TryAcquire(xStack_9c, me, 4)
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00ee6817 end
                        bVar4 = resources:TryAcquire(xStack_9c, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        me:ClearCommands()
                        bVar4 = false
                        pCVar8 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar8, bVar4)
                        if not quest:GetStateBool("GuideSpokenToHeroThisWaypoint") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                -- TODO(native): CCharString::operator= (&xStack_80,(CCharString *)(*(int *)(this + 0x14) + 0x50 + *(int *)(*(int *)(this + 0x14) + 0x164) * 0xc));
                                quest:SetStateBool("GuideSpokenToHeroThisWaypoint", true)
                                goto LAB_00ee5fd7
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                while true do
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then goto LAB_00ee6558 end
                                    iVar6 = math.random(0, 32767)
                                    iVar6 = iVar6 % 5
                                    if iVar6 ~= __native_entity_state:GetStateInt("lastRandomSpeechIdx") then break end
                                    alive = quest:NewScriptFrame(me)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if not bVar4 then
                                    __native_entity_state:SetStateInt("lastRandomSpeechIdx", iVar6)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if not bVar4 then
                                        xStack_80 = quest:GetStateString(("RandomGuideResponse_" .. iVar6))
                                        goto LAB_00ee5fd7
                                    end
                                end
                            end
                        end
                        goto FLOW_past_lab_00ee5fd7
                        ::LAB_00ee5fd7::
                        if not quest:GetStateBool("TourGuideKilled") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00ee6558 end
                            iVar7 = quest:AddNewConversation(me, false, false)
                            pCVar8 = quest:GetHero()
                            quest:AddPersonToConversation(iVar7, pCVar8)
                            pCVar8 = quest:GetHero()
                            quest:AddLineToConversation(iVar7, xStack_80, me, pCVar8, false)
                            quest:SetTimer(i_stk_a0, quest:ReadGlobalGameData(0x8e0))
                        end
                        resources:PrepareResource(xStack_9c)
                        bVar4 = resources:TryAcquire(xStack_9c, me, 4)
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00ee6558 end
                            bVar4 = resources:TryAcquire(xStack_9c, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            -- TODO(native): helper_EE6850(quest, me, *(this + 0x14), (*(this + 0x14) + 0x168), xStack_9c)
                            goto LAB_00ee60d9
                        end
                        ::FLOW_past_lab_00ee5fd7::
                        -- LAB_00ee6541: (native jump target)
                    end
                end
                goto LAB_00ee6817
            end
        end
        ::LAB_00ee60d9::
        bVar4 = quest:IsDistanceBetweenThingsUnder(me, (__native_entity_state:GetStateInt("self_0x14") + 0x168), 2.0)
        if bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                r3 = quest:GetNearestWithScriptName(me, "TourGuideFollower")
                iVar6 = (r3 ~= nil and r3:IsAlive())
                if not iVar6 then
                    goto LAB_00ee6232
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        resources:PrepareResource(xStack_9c)
                        bVar4 = resources:TryAcquire(xStack_9c, me, 4)
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00ee6558 end
                            bVar4 = resources:TryAcquire(xStack_9c, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            me:ClearCommands()
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if not bVar4 then
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if not bVar4 then
                                    quest:EntitySetFacingAngleTowardsThing(me, r3, false)
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if not bVar4 then
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if not bVar4 then goto LAB_00ee6232 end
                                    end
                                end
                            end
                        end
                    end
                end
                goto FLOW_past_lab_00ee6232
                ::LAB_00ee6232::
                quest:SetTimer(i_stk_a4, quest:ReadGlobalGameData(0x8dc))
                while true do
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00ee6558 end
                    iVar6 = quest:GetTimer(i_stk_a4)
                    if iVar6 == 0 then break end
                    bVar4 = me:IsTalkedToByHero()
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00ee6558 end
                        resources:PrepareResource(xStack_9c)
                        bVar4 = resources:TryAcquire(xStack_9c, me, 4)
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00ee6558 end
                            bVar4 = resources:TryAcquire(xStack_9c, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00ee6558 end
                        me:ClearCommands()
                        bVar4 = false
                        pCVar8 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar8, bVar4)
                        while true do
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00ee6558 end
                            iVar6 = math.random(0, 32767)
                            iVar6 = iVar6 % 5
                            if iVar6 ~= __native_entity_state:GetStateInt("lastRandomSpeechIdx") then break end
                            alive = quest:NewScriptFrame(me)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00ee6558 end
                        __native_entity_state:SetStateInt("lastRandomSpeechIdx", iVar6)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00ee6558 end
                        -- TODO(native): CCharString::CCharString(&xStack_58,quest:GetStateString(("RandomGuideResponse_" .. iVar6)));
                        if not quest:GetStateBool("TourGuideKilled") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                goto LAB_00ee6558
                            end
                            iVar7 = quest:AddNewConversation(me, false, false)
                            pCVar8 = quest:GetHero()
                            quest:AddPersonToConversation(iVar7, pCVar8)
                            pCVar8 = quest:GetHero()
                            quest:AddLineToConversation(iVar7, nil --[[missing]], me, pCVar8, false)
                        end
                    end
                    alive = quest:NewScriptFrame(me)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                native_arg_sequence_1 = false
                if not bVar4 then
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
                    quest:SetStateInt("WaypointCounter", quest:GetStateInt("WaypointCounter") + 1)
                    quest:SetStateBool("Initialise", true)
                    goto LAB_00ee6456
                end
                ::FLOW_past_lab_00ee6232::
            end
            goto FLOW_hoist_lab_00ee6558_1
        end
        goto FLOW_past_lab_00ee6558
        ::LAB_00ee6558::
        ::FLOW_hoist_lab_00ee6558_1::
        goto LAB_00ee6817
        ::FLOW_past_lab_00ee6558::
        ::LAB_00ee6456::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        iVar6 = i_stk_a4
        iVar7 = i_stk_a0
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        quest:DeregisterTimer(i_stk_a0)
        quest:DeregisterTimer(i_stk_a4)
        resources:ReleaseResource(xStack_9c)
        return
    end
    quest:ClearThingHasInformation(me)
    resources:PrepareResource(xStack_9c)
    bVar4 = resources:TryAcquire(xStack_9c, me, 4)
    while not bVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            quest:DeregisterTimer(i_stk_a0)
            quest:DeregisterTimer(i_stk_a4)
            resources:ReleaseResource(xStack_9c)
            return
        end
        bVar4 = resources:TryAcquire(xStack_9c, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        -- LAB_00ee65d9: (native jump target)
        quest:DeregisterTimer(i_stk_a0)
        quest:DeregisterTimer(i_stk_a4)
        resources:ReleaseResource(xStack_9c)
        return
    end
    iVar6 = me:IsPerformingScriptTask()
    cVar5 = iVar6
    while cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then goto LAB_00ee6817 end
        iVar6 = me:IsPerformingScriptTask()
        cVar5 = iVar6
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        pCVar8 = quest:GetThingWithScriptName("M_TG_ClosingTimeExit")
        xStack_8c = pCVar8
        bVar4 = (not resources:ScriptThing(xStack_9c):IsNull())
        __native_condition_1 = bVar4
        if __native_condition_1 then
            iVar6 = (xStack_8c ~= nil and xStack_8c:IsAlive())
            __native_condition_1 = iVar6
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                if not (xStack_8c ~= nil and not xStack_8c:IsNull()) then
                    puVar10 = {x = 0, y = 0, z = 0}
                else
                    puVar10 = xStack_8c:GetPos()
                end
                r2 = {x = puVar10.x, y = puVar10.y, z = puVar10.z}
                iVar6 = 3.0
                r1 = resources:ScriptThing(xStack_9c)
                pvVar11 = r1
                iVar6 = (pvVar11 ~= nil and pvVar11:IsDistanceFromPositionOver(r2, iVar6))
                cVar5 = iVar6
                while cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00ee6817 end
                    me:MoveToPosition(r2, 1.0, 0, false, true)
                    iVar6 = me:IsPerformingScriptTask()
                    cVar5 = iVar6
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00ee6817 end
                        iVar6 = me:IsPerformingScriptTask()
                        cVar5 = iVar6
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00ee6817 end
                    iVar6 = 3.0
                    r1 = resources:ScriptThing(xStack_9c)
                    pvVar11 = r1
                    iVar6 = (pvVar11 ~= nil and pvVar11:IsDistanceFromPositionOver(r2, iVar6))
                    cVar5 = iVar6
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    quest:FadeOutAndKillEntity(me, true, 1.0, true)
                end
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                resources:PrepareResource(xStack_9c)
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                until not (not bVar4)
            end
        end
    end
    ::LAB_00ee6817::
    quest:DeregisterTimer(i_stk_a0)
    quest:DeregisterTimer(i_stk_a4)
    resources:ReleaseResource(xStack_9c)
end

function Init(quest, me)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetPersonalityOverrideByString(me, "OPINION_PERSONALITY_PERMANENT_FRIEND")
    __native_entity_state:SetStateInt("saveWaypointIdx", 0xffffffff)
    __native_entity_state:SetStateInt("lastRandomSpeechIdx", 0xffffffff)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

function helper_EE6850(quest, me, native_arg_param_1, native_arg_param_2)
    local __native_condition_1, bVar4, cVar5, iVar1, iVar9, native_arg_sequence_1, pCVar6, pCVar7, puVar8, uVar10, uVar11, uVar12, uVar13, xStack_18
    local alive = true
    bVar4 = (native_arg_param_1 ~= nil and native_arg_param_1:IsAlive())
    if bVar4 then
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            -- TODO(native): iVar1 = *native_arg_param_2
            iVar1 = nil --[[unresolved native value]]
            uVar13 = 1
            uVar12 = 0
            uVar11 = 0
            uVar10 = 1.0
            pCVar6 = native_arg_param_1:GetPos()
            -- TODO(native): (**(code **)(iVar1 + 0x10))(pCVar6,uVar10,uVar11,uVar12,uVar13);
            return
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            -- TODO(native): xStack_18 = *(int **)(native_arg_param_1 + 4);
            -- TODO(native): xStack_18 = *(int **)(native_arg_param_1 + 8);
            xStack_18 = nil
            if xStack_18 ~= nil then
                -- TODO(native): *xStack_18 = *xStack_18 + 1;
            end
            iVar1 = __native_entity_state:GetStateInt("self_0x164")
            while true do
                __native_condition_1 = xStack_18 == nil
                if not __native_condition_1 then
                    cVar5 = (xStack_18 ~= nil and xStack_18:IsAlive())
                    __native_condition_1 = not cVar5
                end
                if not __native_condition_1 then break end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then goto LAB_00ee6a24 end
                iVar9 = __native_entity_state:GetStateInt("self_0x164") + 1
                -- TODO(native): name field 0x164 (int)
                __native_entity_state:SetStateInt("self_0x164", iVar9)
                if iVar9 < 0x12 then
                    native_arg_sequence_1 = false
                    if iVar9 == iVar1 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if native_arg_sequence_1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then goto LAB_00ee6a24 end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00ee6a24 end
                    -- TODO(native): name field 0x164 (undefined4)
                    __native_entity_state:SetStateInt("self_0x164", 0)
                end
                pCVar7 = quest:GetThingWithScriptName(nil --[[missing]])
                xStack_18 = pCVar7
                pCVar7 = nil
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                if xStack_18 == nil then
                    puVar8 = {x = 0, y = 0, z = 0}
                else
                    puVar8 = xStack_18:GetPos()
                end
                native_arg_param_2:SetDataString(puVar8)
            end
            ::LAB_00ee6a24::
            return
        end
    end
end

