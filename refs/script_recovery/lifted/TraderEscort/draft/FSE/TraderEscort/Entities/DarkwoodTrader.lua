-- Generated native draft: DarkwoodTrader. Review coverage report before use.
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
    local CVar14, __native_condition_1, bVar3, bVar5, cVar4, c_stk_169, c_stk_171, fStack_20, fVar19, fVar2, fret_0, fret_00, fret_01, fret_02, fret_03, iVar20, iVar21, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, native_arg_switch_1, p0, p4, p5, pCVar10, pCVar11, pCVar13, pCVar18, pCVar6, pPosition, pSpeaker, pcVar22, piVar12, puVar1, puVar7, pvVar8, r1, r10, r2, r3, r4, r5, r6, r7, r8, r9, this_01, uVar15, uVar17, xStack_118, xStack_158, xStack_168, xStack_170, xStack_c0, xStack_e8, xStack_f8
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    xStack_168 = resources:NewResource()
    cVar4 = quest:GetStateBool("IntroFinished")
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e08601 end
        cVar4 = me:IsTalkedToByHero()
        native_arg_sequence_1 = false
        if cVar4 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if native_arg_sequence_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then goto LAB_00e08601 end
        cVar4 = quest:GetStateBool("IntroFinished")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00e08601 end
    if __native_entity_state:GetStateInt("BrainState") ~= 1 then
        goto LAB_00e083cd
    end
    goto FLOW_past_lab_00e083cd
    ::LAB_00e083cd::
    -- TODO(native): CCreatureAction_TrollWhackGroundBase__HandleTrader(this,2);
    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") + 1)
    quest:SetTimer(__native_entity_state:GetStateInt("RegulateBanterComment"), 0x1e)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        repeat
            if not __native_entity_state:GetStateBool("LeadToCamp") then
                bVar5 = quest:IsRegionLoaded("BarrowFields")
                bVar3 = true
                if not bVar5 then goto LAB_00e0843a end
            else
                goto LAB_00e0843a
            end
            goto FLOW_past_lab_00e0843a
            ::LAB_00e0843a::
            bVar3 = false
            ::FLOW_past_lab_00e0843a::
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                quest:EntityStopFollowing(me)
                bVar3 = false
                pCVar10 = quest:GetHero()
                quest:SetEntityAsRegionFollowing(pCVar10, me, bVar3)
                quest:EntitySetAsScared(me, false)
                quest:SetTimer(quest:GetStateInt("CommentTimer"), 0)
                require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "IN_BARROW_FIELD", me, 0)
                resources:PrepareResource(xStack_168)
                bVar3 = resources:TryAcquire(xStack_168, me, 4)
                while not bVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e08601 end
                    bVar3 = resources:TryAcquire(xStack_168, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                r1 = quest:GetThingWithScriptName("M_TradersStopHere")
                bVar3 = false
                iVar20 = 0
                me:ClearCommands()
                if not (r1 ~= nil and not r1:IsNull()) then
                    puVar7 = {x = 0, y = 0, z = 0}
                else
                    puVar7 = r1:GetPos()
                end
                me:MoveToPosition(puVar7, 1.0, 1, false, true)
                bVar5 = quest:IsDistanceBetweenThingsUnder(me, r1, 2.0)
                while not bVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e08601 end
                    fStack_20 = (quest:GetDistanceBetweenThings(me, r1) ^ 2)
                    pCVar6 = quest:GetHero()
                    fVar19 = (quest:GetDistanceBetweenThings(pCVar6, r1) ^ 2)
                    c_stk_169 = fStack_20 < fVar19
                    fVar19 = 9.0
                    pCVar6 = quest:GetHero()
                    bVar5 = quest:IsDistanceBetweenThingsUnder(me, pCVar6, fVar19)
                    if (bVar5) or (c_stk_169 == 0) then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e08601 end
                        fVar19 = 5.0
                        pCVar6 = quest:GetHero()
                        bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar6, fVar19)
                        if (bVar3) or (c_stk_169 == 0) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e08601 end
                            if iVar20 ~= 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00e08601 end
                                me:ClearCommands()
                                if not (r1 ~= nil and not r1:IsNull()) then
                                    puVar7 = {x = 0, y = 0, z = 0}
                                else
                                    puVar7 = r1:GetPos()
                                end
                                me:MoveToPosition(puVar7, 1.0, 1, false, true)
                                iVar20 = 0
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e08601 end
                            if iVar20 ~= 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00e08601 end
                                me:ClearCommands()
                                if not (r1 ~= nil and not r1:IsNull()) then
                                    puVar7 = {x = 0, y = 0, z = 0}
                                else
                                    puVar7 = r1:GetPos()
                                end
                                me:MoveToPosition(puVar7, 1.0, 0, false, true)
                                iVar20 = 1
                            end
                        end
                        bVar3 = false
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e08601 end
                        me:ClearCommands()
                        me:ClearAllActions()
                        if not bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e08601 end
                            bVar3 = false
                            pCVar6 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                            iVar21 = quest:AddNewConversation(me, false, false)
                            pCVar6 = quest:GetHero()
                            quest:AddPersonToConversation(iVar21, pCVar6)
                            pCVar10 = quest:GetHero()
                            uVar17 = 0
                            pcVar22 = "_FOLLOW_ME"
                            pCVar11 = me:GetDataString()
                            pCVar11 = ("TEXT_QST_067_" .. pCVar11)
                            pCVar11 = (pCVar11 .. pcVar22)
                            quest:AddLineToConversation(iVar21, pCVar11, me, pCVar10, (uVar17 ~= 0))
                            quest:Pause(1.0)
                            bVar3 = true
                            iVar20 = 2
                        end
                    end
                    bVar5 = quest:IsDistanceBetweenThingsUnder(me, r1, 2.0)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    -- LAB_00e09ee8: (native jump target)
                    resources:ReleaseResource(xStack_168)
                    return
                end
                bVar3 = false
                pCVar6 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, pCVar6, bVar3)
                require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "THANKS_10", me, 0)
                __native_entity_state:SetStateBool("LeadToCamp", true)
            end
            if quest:GetStateBool("EndStarted") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("TraderHealthID"))
                quest:MiniMapRemoveMarker(me)
                r2 = quest:GetThingWithScriptName("TraderEndPos")
                quest:EntityStopFollowing(me)
                bVar3 = false
                pCVar10 = quest:GetHero()
                quest:SetEntityAsRegionFollowing(pCVar10, me, bVar3)
                quest:EntitySetAsScared(me, false)
                bVar3 = false
                fret_02 = quest:GetHealth(me)
                quest:ModifyThingHealth(me, (30.0 - fret_02), bVar3)
                resources:PrepareResource(xStack_168)
                bVar3 = resources:TryAcquire(xStack_168, me, 4)
                goto LAB_00e09fd9
            end
            if quest:GetStateBool("TradersShouldBeScared") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                r3 = quest:GetThingWithScriptName("DTE_RunOffPos")
                quest:EntityStopFollowing(me)
                bVar3 = false
                pCVar10 = quest:GetHero()
                quest:SetEntityAsRegionFollowing(pCVar10, me, bVar3)
                quest:EntitySetAsScared(me, true)
                resources:PrepareResource(xStack_168)
                bVar3 = resources:TryAcquire(xStack_168, me, 4)
                while not bVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e08601 end
                    bVar3 = resources:TryAcquire(xStack_168, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    -- LAB_00e0a363: (native jump target)
                    resources:ReleaseResource(xStack_168)
                    return
                end
                if not (r3 ~= nil and not r3:IsNull()) then
                    puVar7 = {x = 0, y = 0, z = 0}
                else
                    puVar7 = r3:GetPos()
                end
                -- TODO(native): xStack_e8 = puVar7.x;
                iVar20 = 3.0
                xStack_f8 = resources:ScriptThing(xStack_168)
                pvVar8 = xStack_f8
                -- TODO(native): iVar20 = IsDistanceFromThingToPositionOver(pvVar8,xStack_e8,iVar20);
                iVar20 = nil --[[unresolved native result]]
                cVar4 = iVar20
                while cVar4 ~= 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e08601 end
                    me:MoveToPosition(xStack_e8, 1.0, 1, false, true)
                    iVar20 = me:IsPerformingScriptTask()
                    cVar4 = iVar20
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e08601 end
                        iVar20 = me:IsPerformingScriptTask()
                        cVar4 = iVar20
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e08601 end
                    iVar20 = 3.0
                    xStack_f8 = resources:ScriptThing(xStack_168)
                    pvVar8 = xStack_f8
                    -- TODO(native): iVar20 = IsDistanceFromThingToPositionOver(pvVar8,xStack_e8,iVar20);
                    iVar20 = nil --[[unresolved native result]]
                    cVar4 = iVar20
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00e08601 end
                cVar4 = quest:GetStateBool("TradersShouldBeScared")
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e08601 end
                    cVar4 = quest:GetStateBool("TradersShouldBeScared")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00e08601 end
                resources:PrepareResource(xStack_168)
                bVar3 = true
                fVar19 = 3.0
                pCVar6 = quest:GetHero()
                quest:EntityFollowThing(me, pCVar6, fVar19, bVar3)
                bVar3 = true
                pCVar10 = quest:GetHero()
                quest:SetEntityAsRegionFollowing(pCVar10, me, bVar3)
                fVar19 = 20.0
                pCVar6 = quest:GetHero()
                bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar6, fVar19)
                while not bVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e08601 end
                    fVar19 = 20.0
                    pCVar6 = quest:GetHero()
                    bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar6, fVar19)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00e08601 end
                iVar21 = quest:AddNewConversation(me, false, false)
                pCVar6 = quest:GetHero()
                quest:AddPersonToConversation(iVar21, pCVar6)
                pCVar10 = quest:GetHero()
                uVar17 = 0
                pcVar22 = "_KILLED_EARTH_TROLL"
                pCVar11 = me:GetDataString()
                pCVar11 = ("TEXT_QST_067_" .. pCVar11)
                pCVar11 = (pCVar11 .. pcVar22)
                quest:AddLineToConversation(iVar21, pCVar11, me, pCVar10, (uVar17 ~= 0))
            end
            if not __native_entity_state:GetStateBool("InSafeZone") then
                bVar5 = quest:IsRegionLoaded("Darkwood4")
                bVar3 = true
                if not bVar5 then goto LAB_00e08d39 end
            else
                goto LAB_00e08d39
            end
            goto FLOW_past_lab_00e08d39
            ::LAB_00e08d39::
            bVar3 = false
            ::FLOW_past_lab_00e08d39::
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                quest:EntitySetAsScared(me, false)
                __native_entity_state:SetStateBool("InSafeZone", true)
            else
                if not __native_entity_state:GetStateBool("InSafeZone") then
                    goto LAB_00e08dc8
                else
                    bVar5 = quest:IsRegionLoaded("Darkwood4")
                    bVar3 = true
                    if bVar5 then goto LAB_00e08dc8 end
                end
                goto FLOW_past_lab_00e08dc8
                ::LAB_00e08dc8::
                bVar3 = false
                ::FLOW_past_lab_00e08dc8::
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                    quest:EntitySetAsScared(me, true)
                    __native_entity_state:SetStateBool("InSafeZone", false)
                end
            end
            if not __native_entity_state:GetStateBool("GreetedBuddy") then
                bVar5 = quest:IsRegionLoaded("Darkwood4")
                bVar3 = true
                if not bVar5 then goto LAB_00e08e52 end
            else
                goto LAB_00e08e52
            end
            goto FLOW_past_lab_00e08e52
            ::LAB_00e08e52::
            bVar3 = false
            ::FLOW_past_lab_00e08e52::
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                pCVar6 = quest:GetHero()
                r4 = quest:GetNearestWithScriptName(pCVar6, "DarkwoodTrader")
                pCVar11 = r4:GetDataString()
                quest:SetStateString("TraderToTalk", pCVar11)
                r5 = quest:GetThingWithScriptName("TE_CampTrader_A")
                iVar20 = (r5 ~= nil and r5:IsAlive())
                if iVar20 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e08601 end
                    quest:EntityStopFollowing(me)
                    bVar3 = false
                    pCVar10 = quest:GetHero()
                    quest:SetEntityAsRegionFollowing(pCVar10, me, bVar3)
                    resources:PrepareResource(xStack_168)
                    bVar3 = resources:TryAcquire(xStack_168, me, 4)
                    while not bVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e08601 end
                        bVar3 = resources:TryAcquire(xStack_168, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e08601 end
                    if not (r5 ~= nil and not r5:IsNull()) then
                        puVar7 = {x = 0, y = 0, z = 0}
                    else
                        puVar7 = r5:GetPos()
                    end
                    -- TODO(native): xStack_118 = puVar7.x;
                    iVar20 = 5.0
                    xStack_c0 = resources:ScriptThing(xStack_168)
                    pvVar8 = xStack_c0
                    -- TODO(native): iVar20 = IsDistanceFromThingToPositionOver(pvVar8,xStack_118,iVar20);
                    iVar20 = nil --[[unresolved native result]]
                    cVar4 = iVar20
                    while cVar4 ~= 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e08601 end
                        me:MoveToPosition(xStack_118, 3.0, 0, false, true)
                        iVar20 = me:IsPerformingScriptTask()
                        cVar4 = iVar20
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e08601 end
                            iVar20 = me:IsPerformingScriptTask()
                            cVar4 = iVar20
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e08601 end
                        iVar20 = 5.0
                        xStack_c0 = resources:ScriptThing(xStack_168)
                        pvVar8 = xStack_c0
                        -- TODO(native): iVar20 = IsDistanceFromThingToPositionOver(pvVar8,xStack_118,iVar20);
                        iVar20 = nil --[[unresolved native result]]
                        cVar4 = iVar20
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e08601 end
                    bVar5 = false
                    bVar3 = false
                    fVar19 = 0.0
                    pPosition = me:GetPos()
                    -- TODO(native): CreateEffect is not a ForgeFSE binding
                    quest:CreateEffect(xStack_f8, pPosition, "", fVar19, bVar3, bVar5)
                    quest:ModifyThingHealth(me, 1000.0, false)
                    piVar12 = me:GetDataString()
                    -- TODO(native): puVar7 = *piVar12
                    puVar7 = nil --[[unresolved native value]]
                    -- TODO(native): puVar1 = *(__native_entity_state:GetStateInt("self_0x14") + 0x78)
                    puVar1 = nil --[[unresolved native value]]
                    if puVar1 == puVar7 then
                        c_stk_171 = 1
                    else
                        if (puVar1 == nil) or (puVar7 == nil) then
                            c_stk_171 = 0
                        else
                            if puVar1[1] == puVar7.y then
                                -- TODO(native): iVar20 = CBasicString<char>::Compare((void *)*puVar1,(void *)puVar7.x);
                                c_stk_171 = (not (iVar20 ~= 0)) and 1 or 0
                            else
                                c_stk_171 = 0
                            end
                        end
                    end
                    if c_stk_171 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            me:PlayAnimation("STANDARD_WAVE", false, false, false, true, true, false, false)
                            iVar20 = me:IsPerformingScriptTask()
                            cVar4 = iVar20
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00e0a380 end
                                iVar20 = me:IsPerformingScriptTask()
                                cVar4 = iVar20
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                quest:Pause(2.0)
                                goto LAB_00e09493
                            end
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            iVar21 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(iVar21, r5)
                            pCVar6 = r5
                            pCVar18 = 0x0
                            uVar17 = 8
                            pCVar13 = me:GetDataString()
                            pCVar13 = ("TEXT_QST_067_" .. pCVar13)
                            pCVar11 = (pCVar13 .. "_GREET_ALLY_10")
                            quest:AddLineToConversation(iVar21, pCVar11, me, r5, (uVar17 ~= 0))
                            pCVar10 = "THANKS_10"
                            pSpeaker = 0x0
                            uVar17 = 0xf0
                            pCVar11 = me:GetDataString()
                            pCVar11 = ("TEXT_QST_067_" .. pCVar11)
                            pCVar11 = (pCVar11 .. "_GREET_ALLY_RESPONSE")
                            quest:AddLineToConversation(iVar21, pCVar11, r4, r3, (uVar17 ~= 0))
                            -- TODO(native): uVar17 = SUB41(&xStack_158,0)
                            uVar17 = nil --[[unresolved native value]]
                            pCVar11 = me:GetDataString()
                            pCVar11 = ("TEXT_QST_067_" .. pCVar11)
                            pCVar11 = (pCVar11 .. "_GREET_ALLY_20")
                            quest:AddLineToConversation(iVar21, pCVar11, me, pCVar6)
                            quest:Pause(2.0)
                            quest:RemoveThing(xStack_f8, false, true)
                            bVar3 = quest:IsConversationActive(iVar21)
                            if bVar3 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e0a380 end
                                    bVar3 = quest:IsConversationActive(iVar21)
                                until not (bVar3)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                goto LAB_00e09493
                            end
                        end
                    end
                    goto FLOW_past_lab_00e09493
                    ::LAB_00e09493::
                    __native_entity_state:SetStateBool("GreetedBuddy", true)
                    iVar20 = (xStack_f8 ~= nil and xStack_f8:IsAlive())
                    if iVar20 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00e0a380 end
                        quest:RemoveThing(xStack_f8, false, true)
                    end
                    resources:PrepareResource(xStack_158)
                    bVar3 = true
                    fVar19 = 3.0
                    pCVar6 = quest:GetHero()
                    quest:EntityFollowThing(me, pCVar6, fVar19, bVar3)
                    bVar3 = true
                    pCVar10 = quest:GetHero()
                    quest:SetEntityAsRegionFollowing(pCVar10, me, bVar3)
                    goto LAB_00e09511
                    ::FLOW_past_lab_00e09493::
                    ::LAB_00e0a380::
                    -- LAB_00e0a389: (native jump target)
                    resources:ReleaseResource(xStack_168)
                    return
                end
                ::LAB_00e09511::
            end
            if (0 < quest:GetStateInt("BalverinesToSurpriseHeroNeeded")) and (not __native_entity_state:GetStateBool("InSafeZone")) then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                quest:EntitySetAsScared(me, true)
            end
            if (__native_entity_state:GetStateInt("Infected") == 0) or (not quest:GetStateBool("SavedInMiddle")) then
                goto LAB_00e095cf
            else
                bVar3 = quest:IsRegionLoaded("Darkwood4")
                if bVar3 then goto LAB_00e095cf end
                bVar5 = quest:IsRegionLoaded("BarrowFields")
                bVar3 = false
                if bVar5 then goto LAB_00e095cf end
            end
            goto FLOW_past_lab_00e095cf
            ::LAB_00e095cf::
            bVar3 = true
            ::FLOW_past_lab_00e095cf::
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                quest:SetTimer(__native_entity_state:GetStateInt("IncubationTime"), quest:ReadGlobalGameData(0xdf4))
            end
            iVar20 = quest:GetTimer(__native_entity_state:GetStateInt("IncubationTime"))
            if iVar20 == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") + -1)
                    resources:PrepareResource(xStack_168)
                    if this_01 == nil then
                        this_01 = 0x0
                    else
                        pCVar11 = extraout_EAX
                        pCVar11 = (a .. pCVar11)
                        -- TODO(native): CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript>(this_01,pCVar11,0);
                        -- TODO(native): *(code **)(this_01 + 0x34) = Script_Darkwood_Balverine_Trader;
                        -- TODO(native): *(undefined4 *)(this_01 + 0x38) = uVar15;
                    end
                    -- TODO(native): CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_01,sectionName);
                    if (xStack_170 & 0x200) ~= 0 then
                        CVar14 = xStack_170 & 0xfffffdff
                    end
                    if (CVar14 & 0x100) ~= 0 then
                        CVar14 = CVar14 & 0xfffffeff
                    end
                    if ((CVar14 & 0x80) ~= 0) then
                    end
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                    until not (not bVar3)
                end
                break
            end
            iVar20 = quest:GetStateInt("IncubationTimeHigh")
            iVar21 = quest:GetTimer(__native_entity_state:GetStateInt("IncubationTime"))
            native_arg_sequence_2 = false
            if iVar21 < iVar20 then
                native_arg_sequence_2 = true
            else
                native_arg_sequence_2 = false
            end
            if native_arg_sequence_2 then
                uVar15 = 4
                if __native_entity_state:GetStateInt("Infected") < 4 then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
            end
            if native_arg_sequence_2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                bVar3 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "INFECTED_HIGH", me, 0)
                goto LAB_00e096c7
            else
                iVar20 = quest:GetStateInt("IncubationTimeMedium")
                iVar21 = quest:GetTimer(__native_entity_state:GetStateInt("IncubationTime"))
                native_arg_sequence_3 = false
                if iVar21 < iVar20 then
                    native_arg_sequence_3 = true
                else
                    native_arg_sequence_3 = false
                end
                if native_arg_sequence_3 then
                    uVar15 = 3
                    if __native_entity_state:GetStateInt("Infected") < 3 then
                        native_arg_sequence_3 = true
                    else
                        native_arg_sequence_3 = false
                    end
                end
                if native_arg_sequence_3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        bVar3 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "INFECTED_MEDIUM", me, 0)
                        goto LAB_00e096c7
                    end
                    break
                end
                iVar20 = quest:GetStateInt("IncubationTimeLow")
                iVar21 = quest:GetTimer(__native_entity_state:GetStateInt("IncubationTime"))
                if (iVar21 < iVar20) and (__native_entity_state:GetStateInt("Infected") < 2) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                    bVar3 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "INFECTED_LOW", me, 0)
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then break end
                        __native_entity_state:SetStateInt("Infected", 2)
                    end
                end
            end
            goto FLOW_past_lab_00e096c7
            ::LAB_00e096c7::
            if bVar3 ~= false then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                __native_entity_state:SetStateInt("Infected", uVar15)
            end
            ::FLOW_past_lab_00e096c7::
            -- TODO(native): xStack_170 = xStack_170 | 0x400;
            cVar4 = me:MsgIsHitBy("")
            if not cVar4 then
                -- TODO(native): xStack_170 = xStack_170 | 0x800;
                cVar4 = me:MsgIsHitByAnySpecialAbilityFrom("")
                if cVar4 then goto LAB_00e09848 end
                goto LAB_00e09886
            else
                goto LAB_00e09848
            end
            goto FLOW_past_lab_00e09886
            ::LAB_00e09886::
            bVar3 = false
            ::FLOW_past_lab_00e09886::
            goto FLOW_past_lab_00e09848
            ::LAB_00e09848::
            -- TODO(native): xStack_170 = xStack_170 | 0x1000;
            cVar4 = me:MsgIsHitByHeroSpecialAbility(0xe)
            bVar3 = true
            if cVar4 then goto LAB_00e09886 end
            ::FLOW_past_lab_00e09848::
            if (xStack_170 & 0x1000) ~= 0 then
                -- TODO(native): xStack_170 = xStack_170 & 0xffffefff;
            end
            if (xStack_170 & 0x800) ~= 0 then
                -- TODO(native): xStack_170 = xStack_170 & 0xfffff7ff;
            end
            if (xStack_170 & 0x400) ~= 0 then
                -- TODO(native): xStack_170 = xStack_170 & 0xfffffbff;
            end
            if bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                quest:SetTimer(__native_entity_state:GetStateInt("RegulateFollowStateComment"), 5)
                fret_00 = quest:GetHealth(me)
                if fret_00 <= 5.0 then
                    goto LAB_00e099dc
                else
                    -- TODO(native): xStack_170 = xStack_170 | 0x2000;
                    cVar4 = me:MsgIsHitByHero()
                    if cVar4 then goto LAB_00e099dc end
                    -- TODO(native): xStack_170 = CVar14 | 0x6000;
                    cVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if cVar4 then
                        -- TODO(native): xStack_170 = xStack_170 | 0x8000;
                        cVar4 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not cVar4 then goto LAB_00e099dc end
                    end
                    bVar3 = true
                end
                goto FLOW_past_lab_00e099dc
                ::LAB_00e099dc::
                bVar3 = false
                ::FLOW_past_lab_00e099dc::
                if ((xStack_170 & 0x8000) ~= 0) then
                    -- TODO(native): xStack_170 = xStack_170 & 0xffff7fff;
                end
                if (xStack_170 & 0x4000) ~= 0 then
                    -- TODO(native): xStack_170 = xStack_170 & 0xffffbfff;
                end
                if (xStack_170 & 0x2000) ~= 0 then
                    -- TODO(native): xStack_170 = xStack_170 & 0xffffdfff;
                end
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                    require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "UNDER_ATTACK", me, 0)
                    iVar20 = 3
                else
                    fret_01 = quest:GetHealth(me)
                    if fret_01 <= 5.0 then
                        goto LAB_00e09b4f
                    else
                        -- TODO(native): xStack_170 = xStack_170 | 0x10000;
                        cVar4 = me:MsgIsHitByHero()
                        if not cVar4 then
                            -- TODO(native): xStack_170 = CVar14 | 0x30000;
                            cVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
                            if cVar4 then
                                -- TODO(native): xStack_170 = xStack_170 | 0x40000;
                                cVar4 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                if not cVar4 then goto LAB_00e09b48 end
                            end
                            goto LAB_00e09b4f
                        end
                        ::LAB_00e09b48::
                        bVar3 = true
                    end
                    goto FLOW_past_lab_00e09b4f
                    ::LAB_00e09b4f::
                    bVar3 = false
                    ::FLOW_past_lab_00e09b4f::
                    if (xStack_170 & 0x40000) ~= 0 then
                        -- TODO(native): xStack_170 = xStack_170 & 0xfffbffff;
                    end
                    if (xStack_170 & 0x20000) ~= 0 then
                        -- TODO(native): xStack_170 = xStack_170 & 0xfffdffff;
                    end
                    if (xStack_170 & 0x10000) ~= 0 then
                        -- TODO(native): xStack_170 = xStack_170 & 0xfffeffff;
                    end
                    if not bVar3 then goto LAB_00e09c13 end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                    require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "HERO_HIT_ME", me, 0)
                    iVar20 = 2
                end
                quest:SetTimer(quest:GetStateInt("CommentTimer"), iVar20)
            end
            ::LAB_00e09c13::
            if __native_entity_state:GetStateInt("CurrentAIState") == __native_entity_state:GetStateInt("PreviousAIState") then
                iVar20 = quest:GetTimer(__native_entity_state:GetStateInt("RegulateBanterComment"))
                if (iVar20 == 0) and (not __native_entity_state:GetStateBool("LeadToCamp")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                    if quest:GetStateInt("TradersStillAliveCounter") == 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then break end
                        c_stk_169 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "BANTER_SOLO", me, 0)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then break end
                        native_arg_switch_1 = quest:GetStateInt("TimesBantered")
                        repeat
                            if native_arg_switch_1 == 0 then
                                c_stk_169 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "BANTER_FIRST", me, 1)
                                break
                            else
                                if native_arg_switch_1 == 1 then
                                    c_stk_169 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "BANTER_SECOND", me, 1)
                                    break
                                else
                                    if native_arg_switch_1 == 2 then
                                        c_stk_169 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "BANTER_THIRD", me, 1)
                                        break
                                    else
                                        if native_arg_switch_1 == 3 then
                                            c_stk_169 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "BANTER_FOURTH", me, 1)
                                            break
                                        else
                                            c_stk_169 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "BANTER", me, 0)
                                        end
                                    end
                                end
                            end
                        until not (false)
                    end
                    if not c_stk_169 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then break end
                        iVar20 = math.random(0, 32767)
                        quest:SetTimer(__native_entity_state:GetStateInt("RegulateBanterComment"), iVar20 % 0xf + 0x19)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then break end
                        iVar20 = math.random(0, 32767)
                        quest:SetTimer(__native_entity_state:GetStateInt("RegulateBanterComment"), iVar20 % 0x14 + 0x1e)
                        quest:SetStateInt("TimesBantered", quest:GetStateInt("TimesBantered") + 1)
                    end
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then break end
                iVar20 = quest:GetTimer(__native_entity_state:GetStateInt("RegulateFollowStateComment"))
                if (iVar20 == 0) and (__native_entity_state:GetStateInt("CurrentAIState") == 2) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then break end
                    require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "FOLLOWING", me, 0)
                    quest:SetTimer(__native_entity_state:GetStateInt("RegulateFollowStateComment"), 10)
                end
            end
            __native_entity_state:SetStateInt("PreviousAIState", __native_entity_state:GetStateInt("CurrentAIState"))
            uVar15 = me:GetCurrentStateGroupType()
            __native_entity_state:SetStateInt("CurrentAIState", uVar15)
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:ReleaseResource(xStack_168)
                return
            end
        until false
    end
    -- LAB_00e0a4ef: (native jump target)
    resources:ReleaseResource(xStack_168)
    do return end
    ::FLOW_past_lab_00e083cd::
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00e08601 end
    resources:PrepareResource(xStack_168)
    bVar3 = resources:TryAcquire(xStack_168, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e08601 end
        bVar3 = resources:TryAcquire(xStack_168, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00e08601 end
    quest:SetThingAsConscious(me, false, "")
    quest:EntitySetTargetable(me, false)
    quest:EntitySetAsDamageable(me, false)
    cVar4 = quest:GetStateBool("InfectedTraderCanGetUp")
    while not cVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00e08601 end
        cVar4 = quest:GetStateBool("InfectedTraderCanGetUp")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00e08601 end
    quest:EntitySetAsDamageable(me, true)
    quest:EntitySetTargetable(me, true)
    quest:SetThingAsConscious(me, true, "")
    r6 = quest:GetThingWithScriptName("MK_DTBE_CUTSCENETRIGGER")
    fVar19 = 5.0
    pCVar6 = quest:GetHero()
    bVar3 = quest:IsDistanceBetweenThingsOver(r6, pCVar6, fVar19)
    if bVar3 then
        repeat
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e085f8 end
            if quest:GetStateBool("MissionFailed") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00e085f8 end
                r7 = quest:GetThingWithScriptName("MK_DTEB_INFEXIT")
                if not (r7 ~= nil and not r7:IsNull()) then
                    puVar7 = {x = 0, y = 0, z = 0}
                else
                    puVar7 = r7:GetPos()
                end
                -- TODO(native): xStack_150._0_4_ = puVar7.x;
                -- TODO(native): xStack_150._4_4_ = puVar7.y;
                iVar20 = 2.0
                -- TODO(native): xStack_150._8_4_ = puVar7.z;
                xStack_118 = resources:ScriptThing(xStack_168)
                pvVar8 = xStack_118
                -- TODO(native): iVar20 = IsDistanceFromThingToPositionOver(pvVar8,xStack_150,iVar20);
                iVar20 = nil --[[unresolved native result]]
                c_stk_169 = iVar20
                while c_stk_169 ~= 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        goto LAB_00e085f8
                    end
                    me:MoveToPosition(r5, 0, 1, false, true)
                    iVar20 = me:IsPerformingScriptTask()
                    cVar4 = iVar20
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            goto LAB_00e085f8
                        end
                        iVar20 = me:IsPerformingScriptTask()
                        cVar4 = iVar20
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        goto LAB_00e085f8
                    end
                    iVar20 = 2.0
                    xStack_118 = resources:ScriptThing(xStack_168)
                    pvVar8 = xStack_118
                    -- TODO(native): iVar20 = IsDistanceFromThingToPositionOver(pvVar8,xStack_150,iVar20);
                    iVar20 = nil --[[unresolved native result]]
                    c_stk_169 = iVar20
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    -- LAB_00e07e06: (native jump target)
                    goto LAB_00e085f8
                end
                fret_0 = quest:FadeOutAndKillEntity(me, true, 2.0, true)
            end
            fVar19 = 5.0
            pCVar6 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsOver(r6, pCVar6, fVar19)
        until not (bVar3)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        c_stk_169 = bVar3
        cVar4 = me:IsDead()
        __native_condition_1 = not cVar4
        if __native_condition_1 then
            fret_0 = quest:GetHealth(me)
            __native_condition_1 = 0.0 < fret_0
        end
        if __native_condition_1 then
            r7 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            r8 = quest:GetNearestWithScriptName(me, "DarkwoodTrader")
            r9 = quest:GetFurthestWithScriptName(me, "DarkwoodTrader")
            xStack_e8 = resources:NewResource()
            xStack_118 = resources:NewResource()
            xStack_f8 = resources:NewResource()
            resources:PrepareResource(xStack_e8)
            iVar21 = 4
            puVar7 = xStack_e8
            pCVar6 = quest:GetHero()
            bVar3 = resources:TryAcquire(puVar7, pCVar6, iVar21)
            while not bVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    -- LAB_00e07e39_c4: (native jump target)
                    resources:ReleaseResource(xStack_f8)
                    resources:ReleaseResource(xStack_118)
                    resources:ReleaseResource(xStack_e8)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(r7)
                    goto LAB_00e085f8
                end
                iVar21 = 4
                puVar7 = xStack_e8
                pCVar6 = quest:GetHero()
                bVar3 = resources:TryAcquire(puVar7, pCVar6, iVar21)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                cVar4 = (r8 ~= nil and r8:IsEqualTo(r9._4_4_))
                if cVar4 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        -- TODO(native): iVar20 = CCarriedReadableDef::CCarriedReadableDef__at6e7b40((CCarriedReadableDef *)xStack_c0);
                        -- TODO(native): CScriptThing::operator=((CScriptThing *)xStack_12c,iVar20);
                        goto LAB_00e07c7c
                    end
                    -- LAB_00e07e39: (native jump target)
                    resources:ReleaseResource(xStack_f8)
                    resources:ReleaseResource(xStack_118)
                    resources:ReleaseResource(xStack_e8)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(r7)
                    goto LAB_00e085f8
                end
                ::LAB_00e07c7c::
                pcVar22 = "SCARED"
                pvVar8 = r8:GetDataString()
                iVar20 = ((pvVar8 ~= pcVar22) and 1 or 0)
                if iVar20 == 0 then
                    goto LAB_00e07cac
                else
                    iVar20 = (r9 ~= nil and r9:IsAlive())
                    bVar3 = true
                    if not iVar20 then goto LAB_00e07cac end
                end
                goto FLOW_past_lab_00e07cac
                ::LAB_00e07cac::
                bVar3 = false
                ::FLOW_past_lab_00e07cac::
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e07db9 end
                    -- TODO(native): xStack_144 = xStack_12c;
                    r9 = xStack_c0
                end
                quest:FixMovieSequenceCamera(true)
                r5 = resources:NewActorMap()
                resources:SetActor(r5, "HERO", xStack_e8)
                resources:SetActor(r5, "TRADERI", xStack_168)
                iVar20 = (r8 ~= nil and r8:IsAlive())
                if not iVar20 then
                    goto LAB_00e081e9
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        iVar20 = (r9 ~= nil and r9:IsAlive())
                        if iVar20 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                resources:PrepareResource(xStack_118)
                                bVar3 = resources:TryAcquire(xStack_118, r8, 4)
                                while not bVar3 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e07e30 end
                                    bVar3 = resources:TryAcquire(xStack_118, r8, 4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00e07db0 end
                                resources:PrepareResource(xStack_f8)
                                bVar3 = resources:TryAcquire(xStack_f8, r9, 4)
                                while not bVar3 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e07db0 end
                                    bVar3 = resources:TryAcquire(xStack_f8, r9, 4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if not bVar3 then
                                    resources:SetActor(r5, "TRADERS", xStack_118)
                                    resources:SetActor(r5, "TRADERN", xStack_f8)
                                    pcVar22 = "CS_DARKWOOD_TRADER_INFECTED_BOTH"
                                    goto LAB_00e081c2
                                end
                            end
                            ::LAB_00e07e30::
                            resources:DestroyActorMap(r5)
                            resources:ReleaseResource(xStack_f8)
                            resources:ReleaseResource(xStack_118)
                            resources:ReleaseResource(xStack_e8)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(r7)
                            goto LAB_00e085f8
                        end
                        piVar12 = r8:GetDataString()
                        if piVar12 == nil then
                            CVar14 = "BANTER_FOURTH"
                            c_stk_171 = false
                        else
                            iVar20 = ((piVar12 == "SCARED") and 0 or 1)
                            c_stk_171 = not (iVar20 ~= 0)
                        end
                        if not c_stk_171 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                resources:DestroyActorMap(r5)
                                resources:ReleaseResource(xStack_f8)
                                resources:ReleaseResource(xStack_118)
                                resources:ReleaseResource(xStack_e8)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(r7)
                                goto LAB_00e085f8
                            end
                            resources:PrepareResource(xStack_f8)
                            bVar3 = resources:TryAcquire(xStack_f8, r8, 4)
                            while not bVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    resources:DestroyActorMap(r5)
                                    resources:ReleaseResource(xStack_f8)
                                    resources:ReleaseResource(xStack_118)
                                    resources:ReleaseResource(xStack_e8)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(r7)
                                    goto LAB_00e085f8
                                end
                                bVar3 = resources:TryAcquire(xStack_f8, r8, 4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                resources:SetActor(r5, "TRADERN", xStack_f8)
                                pcVar22 = "CS_DARKWOOD_TRADER_INFECTED_NORMAL"
                                goto LAB_00e081c2
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                resources:PrepareResource(xStack_118)
                                bVar3 = resources:TryAcquire(xStack_118, r8, 4)
                                while not bVar3 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00e07db0 end
                                    bVar3 = resources:TryAcquire(xStack_118, r8, 4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if not bVar3 then
                                    resources:SetActor(r5, "TRADERS", xStack_118)
                                    pcVar22 = "CS_DARKWOOD_TRADER_INFECTED_SCARED"
                                    goto LAB_00e081c2
                                end
                                goto FLOW_hoist_lab_00e081c2_1
                            end
                        end
                        goto FLOW_past_lab_00e081c2
                        ::LAB_00e081c2::
                        resources:RunMacro(pcVar22, r5, false, true)
                        goto LAB_00e081e9
                        ::FLOW_hoist_lab_00e081c2_1::
                        resources:DestroyActorMap(r5)
                        resources:ReleaseResource(xStack_f8)
                        resources:ReleaseResource(xStack_118)
                        resources:ReleaseResource(xStack_e8)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(r7)
                        goto LAB_00e085f8
                        ::FLOW_past_lab_00e081c2::
                    end
                end
                goto FLOW_past_lab_00e081e9
                ::LAB_00e081e9::
                quest:GiveHeroYesNoQuestion("TEXT_QST_067_INFECTED_ASK_TO_FOLLOW", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                iVar20 = quest:MsgIsQuestionAnsweredYesOrNo()
                while iVar20 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        -- LAB_00e07e30_c9: (native jump target)
                        resources:DestroyActorMap(r5)
                        resources:ReleaseResource(xStack_f8)
                        resources:ReleaseResource(xStack_118)
                        resources:ReleaseResource(xStack_e8)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(r7)
                        goto LAB_00e085f8
                    end
                    iVar20 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if iVar20 == 1 then
                        if bVar3 then
                            -- LAB_00e07e30_c10: (native jump target)
                            resources:DestroyActorMap(r5)
                            resources:ReleaseResource(xStack_f8)
                            resources:ReleaseResource(xStack_118)
                            resources:ReleaseResource(xStack_e8)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(r7)
                            goto LAB_00e085f8
                        end
                        c_stk_169 = 1
                        pcVar22 = "CS_DARKWOOD_TRADER_INFECTED_JOINS"
                    else
                        if bVar3 then goto LAB_00e07db0 end
                        pcVar22 = "CS_DARKWOOD_TRADER_INFECTED_LEAVES"
                    end
                    resources:RunMacro(pcVar22, r5, false, true)
                    quest:FixMovieSequenceCamera(false)
                    resources:DestroyActorMap(r5)
                    resources:ReleaseResource(xStack_f8)
                    resources:ReleaseResource(xStack_118)
                    resources:ReleaseResource(xStack_e8)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(r7)
                    if c_stk_169 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(0xe2c))
                            quest:RemoveThing(me, false, true)
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            resources:PrepareResource(xStack_168)
                            quest:ClearThingHasInformation(me)
                            goto LAB_00e083cd
                        end
                    end
                    goto LAB_00e085f8
                end
                ::FLOW_past_lab_00e081e9::
                ::LAB_00e07db0::
                resources:DestroyActorMap(r5)
            end
            ::LAB_00e07db9::
            resources:ReleaseResource(xStack_f8)
            resources:ReleaseResource(xStack_118)
            resources:ReleaseResource(xStack_e8)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(r7)
        else
            while true do
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if not (not bVar3) then break end
                alive = quest:NewScriptFrame(me)
            end
        end
    end
    ::LAB_00e085f8::
    ::LAB_00e08601::
    resources:ReleaseResource(xStack_168)
    do return end
    ::LAB_00e09fd9::
    if bVar3 then goto LAB_00e0a00b end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00e0a346 end
    bVar3 = resources:TryAcquire(xStack_168, me, 4)
    goto LAB_00e09fd9
    ::LAB_00e0a00b::
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        bVar3 = quest:IsDistanceBetweenThingsOver(me, r5, 7.0)
        if bVar3 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00e0a346 end
                iVar20 = me:IsPerformingScriptTask()
                if not iVar20 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00e0a346 end
                    me:MoveToThing(pCVar6, 4.0, 0)
                end
                bVar3 = quest:IsDistanceBetweenThingsOver(me, r5, 7.0)
            until not (bVar3)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            resources:PrepareResource(xStack_168)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                repeat
                    cVar4 = me:IsTalkedToByHero()
                    if cVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then break end
                        resources:PrepareResource(xStack_168)
                        bVar3 = resources:TryAcquire(xStack_168, me, 4)
                        while not bVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00e0a346 end
                            bVar3 = resources:TryAcquire(xStack_168, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then break end
                        xStack_f8 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        xStack_c0 = resources:ScriptThing(xStack_168)
                        pCVar6 = xStack_c0
                        fret_03 = quest:GetHealth(pCVar6)
                        fVar2 = 0.0
                        if fVar2 < fret_03 then
                            p5 = 0
                            p4 = 1
                            iVar21 = 0
                            iVar20 = 0
                            pcVar22 = "_THANKS"
                            pCVar11 = me:GetDataString()
                            pCVar11 = ("TEXT_QST_067_" .. pCVar11)
                            pCVar11 = (pCVar11 .. pcVar22)
                            pvVar8 = pCVar11
                            pCVar6 = quest:GetHero()
                            r10 = me:Speak(pCVar6, pvVar8, iVar20, (iVar21 ~= 0), (p4 ~= 0), (p5 ~= 0))
                            iVar20 = me:IsPerformingScriptTask()
                            cVar4 = iVar20
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00e0a33a
                                end
                                iVar20 = me:IsPerformingScriptTask()
                                cVar4 = iVar20
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00e0a33a
                            end
                            goto FLOW_past_lab_00e0a33a
                            ::LAB_00e0a33a::
                            resources:ReleaseResource(xStack_e8)
                            break
                            ::FLOW_past_lab_00e0a33a::
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(xStack_e8)
                        resources:PrepareResource("TEXT_QST_067_INFECTED_ASK_TO_FOLLOW")
                    end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        resources:ReleaseResource("TEXT_QST_067_INFECTED_ASK_TO_FOLLOW")
                        return
                    end
                until false
            end
        end
    end
    ::LAB_00e0a346::
    resources:ReleaseResource(xStack_168)
end

function Init(quest, me)
    __native_entity_state:SetStateInt("IncubationTime", quest:RegisterTimer())  -- native constructor: CTimer member
    __native_entity_state:SetStateInt("RegulateFollowStateComment", quest:RegisterTimer())  -- native constructor: CTimer member
    __native_entity_state:SetStateInt("RegulateBanterComment", quest:RegisterTimer())  -- native constructor: CTimer member
    local auVar5, cVar7, iVar4, p1, pCVar3, pThing, pcVar8, piVar2, this_00, uVar1, v_stk_20, xStack_c
    p1 = v_stk_20
    __native_entity_state:SetStateInt("BrainState", 0)
    __native_entity_state:SetStateInt("CurrentAIState", 0)
    __native_entity_state:SetStateInt("PreviousAIState", 0)
    piVar2 = me:GetDataString()
    iVar4 = ((piVar2 == "INFECTED") and 0 or 1)
    cVar7 = not (iVar4 ~= 0)
    if cVar7 then
        __native_entity_state:SetStateInt("BrainState", 1)
    end
    __native_entity_state:SetStateInt("Infected", 0)
    quest:SetTimer(__native_entity_state:GetStateInt("IncubationTime"), 0)
    __native_entity_state:SetStateBool("GreetedBuddy", false)
    __native_entity_state:SetStateBool("LeadToCamp", false)
    __native_entity_state:SetStateBool("InSafeZone", false)
    quest:SetTimer(__native_entity_state:GetStateInt("RegulateFollowStateComment"), 0)
    quest:SetTimer(__native_entity_state:GetStateInt("RegulateBanterComment"), 0x3c)
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    quest:SetThingPersistent(me, true)
    quest:SetEntityAsFollowingHeroThroughTeleporters(me, false)
    pCVar3 = me:GetDataString()
    iVar4 = ((pCVar3 == "INFECTED") and 0 or 1)
    cVar7 = not (iVar4 ~= 0)
    if not cVar7 then
        pCVar3 = me:GetDataString()
        iVar4 = ((pCVar3 == "SCARED") and 0 or 1)
        cVar7 = not (iVar4 ~= 0)
        if not cVar7 then
            pCVar3 = me:GetDataString()
            iVar4 = ((pCVar3 == "FRIENDLY") and 0 or 1)
            cVar7 = not (iVar4 ~= 0)
            if not cVar7 then goto LAB_00e04e13 end
            pcVar8 = "FRIENDLY_TRADER"
        else
            pcVar8 = "SCARED_TRADER"
        end
    else
        pcVar8 = "INFECTED_TRADER"
    end
    quest:EntitySetWillBeUsingNarrator(me, pcVar8)
    ::LAB_00e04e13::
    if this_00 == nil then
        this_00 = 0x0
        auVar5 = v_stk_20
    else
        -- TODO(native): xStack_c._4_4_ = *(undefined4 *)(this + 0xc);
        -- TODO(native): xStack_c._8_4_ = *(undefined4 *)(this + 0x10);
        xStack_c = nil
        if xStack_c._8_4_ ~= nil then
            -- TODO(native): *xStack_c._8_4_ = *xStack_c._8_4_ + 1;
        end
        pCVar3 = extraout_EAX
        pCVar3 = (a .. pCVar3)
        -- TODO(native): CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript> (this_00,pCVar3,0);
        -- TODO(native): *(code **)(this_00 + 0x34) = NScript::CQ_TraderEscortScript::WatchForPickpocketing;
        -- TODO(native): *(undefined4 *)(this_00 + 0x38) = uVar1;
        auVar5 = 7
    end
    -- TODO(native): CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_00,sectionName);
    if (auVar5 & 4) ~= 0 then
        auVar5 = (auVar5 & 0xfffffffb)
    end
    if (auVar5 & 2) ~= 0 then
        auVar5 = (auVar5 & 0xfffffffd)
    end
end

function OnPersist(quest, me, context)
    local brainState = quest:GetStateInt("BrainState") or 0
    brainState = quest:PersistTransferInt(context, "BrainState", brainState)
    quest:SetStateInt("BrainState", brainState)
end

function OnPredicateFail(quest, me)
    local cVar1, iVar2, p0, pCVar3
    iVar2 = quest:GetTimer(__native_entity_state:GetStateInt("IncubationTime"))
    if iVar2 < 1 then
        return
    end
    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") + -1)
    quest:SetTimer(quest:GetStateInt("CommentTimer"), 0)
    p0 = "SCRIPT_NAME_HERO"
    cVar1 = me:MsgIsKilledBy("SCRIPT_NAME_HERO")
    if not cVar1 then
        if quest:GetStateInt("TradersStillAliveCounter") == 2 then
            iVar2 = 1
            pCVar3 = quest:GetNearestWithScriptName(me, "DarkwoodTrader")
        else
            if quest:GetStateInt("TradersStillAliveCounter") ~= 1 then goto LAB_00e018e4 end
            iVar2 = 0
            pCVar3 = quest:GetNearestWithScriptName(me, "DarkwoodTrader")
        end
        require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "LAST_TRADER", me, iVar2)
    else
        iVar2 = 0
        pCVar3 = quest:GetNearestWithScriptName(me, "DarkwoodTrader")
        require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "HERO_KILLED_TRADER", pCVar3, iVar2)
    end
    ::LAB_00e018e4::
    quest:SetMasterGameState("DarkwoodAllTradersAlive", false)
end

function SetBrainState(quest, me, native_arg_brain_state)
    local bVar1, pCVar2, pCVar3, pFollower, pFollower_00
    local alive = true
    __native_entity_state:SetStateInt("BrainState", native_arg_brain_state)
    local iVar4 = native_arg_brain_state
    if native_arg_brain_state == 2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        iVar4 = bVar1
        if not bVar1 then
            bVar1 = true
            pCVar2 = quest:GetHero()
            quest:EntityFollowThing(me, pCVar2, 3.0, bVar1)
            bVar1 = true
            pFollower_00 = quest:GetHero()
            quest:SetEntityAsRegionFollowing(pFollower_00, me, bVar1)
            quest:DisplayQuestInfo(true)
            pCVar3 = me:GetDataString()
            if pCVar3 == nil then
                bVar1 = false
                native_arg_brain_state = CONCAT31(native_arg_brain_state._1_3_,bVar1)
            else
                iVar4 = ((pCVar3 == "INFECTED") and 0 or 1)
                native_arg_brain_state = CONCAT31(native_arg_brain_state._1_3_,not (iVar4 ~= 0))
            end
            if native_arg_brain_state == 0 then
                pCVar3 = me:GetDataString()
                if pCVar3 == nil then
                    bVar1 = false
                    native_arg_brain_state = CONCAT31(native_arg_brain_state._1_3_,bVar1)
                else
                    iVar4 = ((pCVar3 == "SCARED") and 0 or 1)
                    native_arg_brain_state = CONCAT31(native_arg_brain_state._1_3_,not (iVar4 ~= 0))
                end
                if native_arg_brain_state == 0 then
                    pCVar3 = me:GetDataString()
                    if pCVar3 == nil then
                        bVar1 = false
                        native_arg_brain_state = CONCAT31(native_arg_brain_state._1_3_,bVar1)
                    else
                        iVar4 = ((pCVar3 == "FRIENDLY") and 0 or 1)
                        native_arg_brain_state = CONCAT31(native_arg_brain_state._1_3_,not (iVar4 ~= 0))
                    end
                    iVar4 = CONCAT31((int3)(extraout_EAX >> 8),native_arg_brain_state)
                    if native_arg_brain_state ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        iVar4 = bVar1
                        if not bVar1 then
                            native_arg_brain_state = -0x10000
                            -- TODO(native): iVar4 = quest:AddQuestInfoBarHealth(me, &native_arg_brain_state, "HUD_QUEST_ICON_TRADER_HAT_02", 1.0)
                            iVar4 = nil --[[unresolved native value]]
                            __native_entity_state:SetStateInt("TraderHealthID", iVar4)
                            iVar4 = extraout_EAX_00
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    iVar4 = bVar1
                    if not bVar1 then
                        native_arg_brain_state = -0x10000
                        -- TODO(native): iVar4 = quest:AddQuestInfoBarHealth(me, &native_arg_brain_state, "HUD_QUEST_ICON_TRADER_HAT_01", 1.0)
                        iVar4 = nil --[[unresolved native value]]
                        __native_entity_state:SetStateInt("TraderHealthID", iVar4)
                        iVar4 = quest:EntitySetAsScared(me, true)
                        return iVar4
                    end
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                iVar4 = bVar1
                if not bVar1 then
                    native_arg_brain_state = -0x10000
                    -- TODO(native): iVar4 = quest:AddQuestInfoBarHealth(me, &native_arg_brain_state, "HUD_QUEST_ICON_TRADER", 1.0)
                    iVar4 = nil --[[unresolved native value]]
                    __native_entity_state:SetStateInt("TraderHealthID", iVar4)
                    __native_entity_state:SetStateInt("Infected", 1)
                    if not quest:GetStateBool("SavedNearEnd") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        iVar4 = bVar1
                        if not bVar1 then
                            iVar4 = quest:SetTimer(__native_entity_state:GetStateInt("IncubationTime"), quest:ReadGlobalGameData(0xdf4))
                            return iVar4
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        iVar4 = bVar1
                        if not bVar1 then
                            iVar4 = quest:SetTimer(__native_entity_state:GetStateInt("IncubationTime"), quest:ReadGlobalGameData(0xdf8))
                            return iVar4
                        end
                    end
                end
            end
        end
    end
    return iVar4
end

