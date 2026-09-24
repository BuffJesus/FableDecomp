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
    local CStack_110, CVar19, CVar24, CVar24_b3, __push1, __push10, __push11, __push12, __push13, __push2, __push3, __push4, __push5, __push6, __push7, __push8, __push9, b, bVar2, cVar3, c_stk_169, c_stk_171, fVar18, fVar22, iVar15, iVar23, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, native_arg_sequence_4, native_arg_switch_1, p0, p3, p4, p5, pCVar10, pCVar11, pCVar12, pCVar4, pcVar20, piVar14, piVar9, puVar1, puVar5, pvVar6, r1, r10, r11, r2, r3, r4, r5, r6, r7, r8, r9, this_00, uVar13, uVar16, uVar7, xStack_114, xStack_118, xStack_120, xStack_128, xStack_134, xStack_14c, xStack_158, xStack_168, xStack_20, xStack_c0, xStack_e4, xStack_e8, xStack_f4, x_stk_f8
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    xStack_168 = resources:NewResource()
    cVar3 = quest:GetStateBool("IntroFinished")
    while not cVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e08601 end
        -- TODO(native): c_stk_171 = (**(*me + 0x6c))("SCRIPT_NAME_HERO")
        c_stk_171 = nil --[[unresolved native value]]
        native_arg_sequence_1 = false
        if c_stk_171 ~= 0 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if native_arg_sequence_1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then goto LAB_00e08601 end
        cVar3 = quest:GetStateBool("IntroFinished")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e08601 end
    if __native_entity_state:GetStateInt("BrainState") ~= 1 then
        goto LAB_00e083cd
    end
    goto FLOW_past_lab_00e083cd
    ::LAB_00e083cd::
    -- TODO(native): CCreatureAction_TrollWhackGroundBase__HandleTrader(this,2);
    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") + 1)
    quest:SetTimer(__native_entity_state:GetStateInt("RegulateBanterComment"), 0x1e)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        repeat
            if not __native_entity_state:GetStateBool("LeadToCamp") then
                -- TODO(native): xStack_168 = xStack_168 | 2;
                cVar3 = quest:IsRegionLoaded("BarrowFields")
                c_stk_171 = 1
                if not cVar3 then goto LAB_00e0843a end
            else
                goto LAB_00e0843a
            end
            goto FLOW_past_lab_00e0843a
            ::LAB_00e0843a::
            c_stk_171 = 0
            ::FLOW_past_lab_00e0843a::
            if (xStack_168 & 2) ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xfffffffd;
            end
            if c_stk_171 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                quest:EntityStopFollowing(me)
                __push1 = quest:GetHero()
                quest:SetEntityAsRegionFollowing(__push1, me, false)
                quest:EntitySetAsScared(me, false)
                quest:SetTimer(quest:GetStateInt("CommentTimer"), 0)
                require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "IN_BARROW_FIELD", me, 0)
                resources:PrepareResource(xStack_168)
                cVar3 = me:AcquireControl(4)
                while not cVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e08601 end
                    cVar3 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                r1 = quest:GetThingWithScriptName("M_TradersStopHere")
                -- TODO(native): unaff_EBP_b3 = 0;
                iVar23 = 0
                me:ClearCommands()
                if xStack_120 == nil then
                    puVar5 = {x = 0, y = 0, z = 0}
                else
                    -- TODO(native): puVar5 = (**(*xStack_120 + 0x18))()
                    puVar5 = nil --[[unresolved native value]]
                end
                me:MoveToPosition(puVar5, 1.0, 1, false, true)
                bVar2 = quest:IsDistanceBetweenThingsUnder(me, r1, 2.0)
                while not bVar2 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e08601 end
                    -- TODO(native): xStack_20[0] = (quest:GetDistanceBetweenThings(p0, xStack_12c) ^ 2);
                    pvVar6 = quest:GetHero()
                    fVar22 = (quest:GetDistanceBetweenThings(pvVar6, r1) ^ 2)
                    c_stk_171 = xStack_20[0] < fVar22
                    CVar24 = 1091567616
                    pCVar10 = quest:GetHero()
                    bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar10, CVar24)
                    if (bVar2) or (not c_stk_171) then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e08601 end
                        CVar24 = 1084227584
                        pCVar10 = quest:GetHero()
                        bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar10, CVar24)
                        if (bVar2) or (not c_stk_171) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e08601 end
                            if iVar23 ~= 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00e08601 end
                                me:ClearCommands()
                                if xStack_120 == nil then
                                    puVar5 = {x = 0, y = 0, z = 0}
                                else
                                    -- TODO(native): puVar5 = (**(*xStack_120 + 0x18))()
                                    puVar5 = nil --[[unresolved native value]]
                                end
                                me:MoveToPosition(puVar5, 1.0, 1, false, true)
                                iVar23 = 0
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e08601 end
                            if iVar23 ~= 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00e08601 end
                                me:ClearCommands()
                                if xStack_120 == nil then
                                    puVar5 = {x = 0, y = 0, z = 0}
                                else
                                    -- TODO(native): puVar5 = (**(*xStack_120 + 0x18))()
                                    puVar5 = nil --[[unresolved native value]]
                                end
                                me:MoveToPosition(puVar5, 1.0, 0, false, true)
                                iVar23 = 1
                            end
                        end
                        -- TODO(native): unaff_EBP_b3 = 0;
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e08601 end
                        me:ClearCommands()
                        me:ClearAllActions()
                        if not unaff_EBP_b3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e08601 end
                            __push2 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(me, __push2, false)
                            uVar7 = quest:AddNewConversation(me, false, false)
                            __push3 = quest:GetHero()
                            quest:AddPersonToConversation(uVar7, __push3)
                            __push4 = quest:GetHero()
                            -- TODO(native): pCVar11 = (**(*me + 0xc))(&xStack_14)
                            pCVar11 = nil --[[unresolved native value]]
                            pCVar11 = ("TEXT_QST_067_" .. pCVar11)
                            pCVar12 = (pCVar11 .. "_FOLLOW_ME")
                            quest:AddLineToConversation(uVar7, pCVar12, me, __push4, false)
                            quest:Pause(1.0)
                            -- TODO(native): unaff_EBP = (int *)CONCAT13(1,(int3)unaff_EBP);
                            iVar23 = 2
                        end
                    end
                    bVar2 = quest:IsDistanceBetweenThingsUnder(me, xStack_128, 2.0)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    -- LAB_00e09ee8: (native jump target)
                    resources:ReleaseResource(xStack_168)
                    return
                end
                __push5 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, __push5, false)
                require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "THANKS_10", me, 0)
                __native_entity_state:SetStateBool("LeadToCamp", true)
            end
            if quest:GetStateBool("EndStarted") then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                -- TODO(native): quest:RemoveQuestInfoElement(*(*(this + 4) + 0x30))
                quest:MiniMapRemoveMarker(me)
                r2 = quest:GetThingWithScriptName("TraderEndPos")
                quest:EntityStopFollowing(me)
                __push6 = quest:GetHero()
                quest:SetEntityAsRegionFollowing(__push6, me, false)
                quest:EntitySetAsScared(me, false)
                r3 = quest:GetHealth(me)
                quest:ModifyThingHealth(me, __unknown_push, false)
                resources:PrepareResource(xStack_168)
                cVar3 = resources:TryAcquire(xStack_168, me, 4)
                goto LAB_00e09fd9
            end
            if quest:GetStateBool("TradersShouldBeScared") then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                r4 = quest:GetThingWithScriptName("DTE_RunOffPos")
                quest:EntityStopFollowing(me)
                __push7 = quest:GetHero()
                quest:SetEntityAsRegionFollowing(__push7, me, false)
                quest:EntitySetAsScared(me, true)
                resources:PrepareResource(xStack_168)
                cVar3 = me:AcquireControl(4)
                while not cVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e08601 end
                    cVar3 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    -- LAB_00e0a363: (native jump target)
                    resources:ReleaseResource(xStack_168)
                    return
                end
                if xStack_158 == nil then
                    puVar5 = {x = 0, y = 0, z = 0}
                else
                    -- TODO(native): puVar5 = (**(*xStack_158 + 0x18))()
                    puVar5 = nil --[[unresolved native value]]
                end
                -- TODO(native): xStack_e8 = (int *)puVar5.x;
                -- TODO(native): CStack_110 = *(CCharString *)(puVar5 + 1);
                iVar23 = 3.0
                -- TODO(native): xStack_cc = puVar5.z;
                x_stk_f8 = resources:ScriptThing(xStack_168)
                pvVar6 = x_stk_f8
                -- TODO(native): iVar23 = IsDistanceFromThingToPositionOver(pvVar6,&xStack_e8,iVar23);
                iVar23 = nil --[[unresolved native result]]
                cVar3 = iVar23
                while cVar3 ~= 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e08601 end
                    me:MoveToPosition(xStack_e8, 1.0, 1, false, true)
                    iVar23 = me:IsPerformingScriptTask()
                    cVar3 = iVar23
                    while cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e08601 end
                        iVar23 = me:IsPerformingScriptTask()
                        cVar3 = iVar23
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e08601 end
                    iVar23 = 3.0
                    x_stk_f8 = resources:ScriptThing(xStack_168)
                    pvVar6 = x_stk_f8
                    -- TODO(native): iVar23 = IsDistanceFromThingToPositionOver(pvVar6,&xStack_e8,iVar23);
                    iVar23 = nil --[[unresolved native result]]
                    cVar3 = iVar23
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e08601 end
                cVar3 = quest:GetStateBool("TradersShouldBeScared")
                while cVar3 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e08601 end
                    cVar3 = quest:GetStateBool("TradersShouldBeScared")
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e08601 end
                resources:PrepareResource(xStack_168)
                fVar22 = 1.4013e-45
                __push8 = quest:GetHero()
                quest:EntityFollowThing(me, __push8, nil --[[missing]], nil --[[missing]])
                uVar7 = quest:GetHero()
                quest:SetEntityAsRegionFollowing(uVar7, me, true)
                pCVar4 = quest:GetHero()
                bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar4, fVar22)
                while not bVar2 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e08601 end
                    fVar22 = 20.0
                    pCVar4 = quest:GetHero()
                    bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar4, fVar22)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e08601 end
                uVar7 = quest:AddNewConversation(me, false, false)
                uVar13 = quest:GetHero()
                quest:AddPersonToConversation(uVar7, uVar13)
                uVar13 = quest:GetHero()
                -- TODO(native): pCVar11 = (**(*me + 0xc))(pCVar12,"_KILLED_EARTH_TROLL",0,me,uVar13)
                pCVar11 = nil --[[unresolved native value]]
                pCVar11 = ("TEXT_QST_067_" .. pCVar11)
                pCVar12 = (pCVar11 .. "_KILLED_EARTH_TROLL")
                quest:AddLineToConversation(uVar7, pCVar12, me, uVar13, false)
            end
            if not __native_entity_state:GetStateBool("InSafeZone") then
                -- TODO(native): xStack_168 = xStack_168 | 4;
                cVar3 = quest:IsRegionLoaded("Darkwood4")
                c_stk_171 = 1
                if not cVar3 then goto LAB_00e08d39 end
            else
                goto LAB_00e08d39
            end
            goto FLOW_past_lab_00e08d39
            ::LAB_00e08d39::
            c_stk_171 = 0
            ::FLOW_past_lab_00e08d39::
            if c_stk_171 == 0 then
                if not __native_entity_state:GetStateBool("InSafeZone") then
                    goto LAB_00e08dc8
                else
                    -- TODO(native): xStack_168 = xStack_168 | 8;
                    cVar3 = quest:IsRegionLoaded("Darkwood4")
                    c_stk_171 = 1
                    if cVar3 then goto LAB_00e08dc8 end
                end
                goto FLOW_past_lab_00e08dc8
                ::LAB_00e08dc8::
                c_stk_171 = 0
                ::FLOW_past_lab_00e08dc8::
                if c_stk_171 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    quest:EntitySetAsScared(me, true)
                    __native_entity_state:SetStateBool("InSafeZone", false)
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                quest:EntitySetAsScared(me, false)
                __native_entity_state:SetStateBool("InSafeZone", true)
            end
            if not __native_entity_state:GetStateBool("GreetedBuddy") then
                -- TODO(native): xStack_168 = xStack_168 | 0x10;
                cVar3 = quest:IsRegionLoaded("Darkwood4")
                c_stk_171 = 1
                if not cVar3 then goto LAB_00e08e52 end
            else
                goto LAB_00e08e52
            end
            goto FLOW_past_lab_00e08e52
            ::LAB_00e08e52::
            c_stk_171 = 0
            ::FLOW_past_lab_00e08e52::
            if (xStack_168 & 0x10) ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xffffffef;
            end
            if c_stk_171 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                __push9 = quest:GetHero()
                r5 = quest:GetNearestWithScriptName(__push9, "DarkwoodTrader")
                pCVar12 = xStack_14c:GetDataString()
                quest:SetStateString("TraderToTalk", pCVar12)
                r6 = quest:GetThingWithScriptName("TE_CampTrader_A")
                iVar23 = (r6 ~= nil and r6:IsAlive())
                if iVar23 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e08601 end
                    quest:EntityStopFollowing(me)
                    __push10 = quest:GetHero()
                    quest:SetEntityAsRegionFollowing(__push10, me, false)
                    resources:PrepareResource(xStack_168)
                    cVar3 = me:AcquireControl(4)
                    while not cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e08601 end
                        cVar3 = me:AcquireControl(4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e08601 end
                    if xStack_14c == nil then
                        puVar5 = {x = 0, y = 0, z = 0}
                    else
                        -- TODO(native): puVar5 = (**(*xStack_14c + 0x18))()
                        puVar5 = nil --[[unresolved native value]]
                    end
                    -- TODO(native): xStack_118 = puVar5.x;
                    -- TODO(native): xStack_e8 = (int *)puVar5.y;
                    iVar23 = 5.0
                    -- TODO(native): CStack_110 = *(CCharString *)(puVar5 + 2);
                    xStack_c0 = resources:ScriptThing(xStack_168)
                    pvVar6 = xStack_c0
                    -- TODO(native): iVar23 = IsDistanceFromThingToPositionOver(pvVar6,&xStack_118,iVar23);
                    iVar23 = nil --[[unresolved native result]]
                    c_stk_171 = iVar23
                    while c_stk_171 ~= 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e08601 end
                        me:MoveToPosition(xStack_118, 3.0, 0, false, true)
                        iVar23 = me:IsPerformingScriptTask()
                        cVar3 = iVar23
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e08601 end
                            iVar23 = me:IsPerformingScriptTask()
                            cVar3 = iVar23
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e08601 end
                        iVar23 = 5.0
                        xStack_c0 = resources:ScriptThing(xStack_168)
                        pvVar6 = xStack_c0
                        -- TODO(native): iVar23 = IsDistanceFromThingToPositionOver(pvVar6,&xStack_118,iVar23);
                        iVar23 = nil --[[unresolved native result]]
                        c_stk_171 = iVar23
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e08601 end
                    -- TODO(native): (**(code **)(*(int *)p0 + 0x18))();
                    -- TODO(native): CreateEffect is not a ForgeFSE binding
                    quest:CreateEffect()
                    quest:ModifyThingHealth(me, 1000.0, false)
                    -- TODO(native): piVar14 = (**(*me + 0xc))(&xStack_34)
                    piVar14 = nil --[[unresolved native value]]
                    -- TODO(native): puVar5 = *piVar14
                    puVar5 = nil --[[unresolved native value]]
                    -- TODO(native): puVar1 = *(__native_entity_state:GetStateInt("self_0x14") + 0x78)
                    puVar1 = nil --[[unresolved native value]]
                    if puVar1 == puVar5 then
                        c_stk_171 = 1
                    else
                        if (puVar1 == nil) or (puVar5 == nil) then
                            c_stk_171 = 0
                        else
                            if puVar1[1] == puVar5.y then
                                -- TODO(native): iVar23 = CBasicString<char>::Compare((void *)*puVar1,(void *)puVar5.x);
                                c_stk_171 = (not (iVar23 ~= 0)) and 1 or 0
                            else
                                c_stk_171 = 0
                            end
                        end
                    end
                    if c_stk_171 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            me:PlayAnimation("STANDARD_WAVE", false, false, false, true, true, false, false)
                            iVar23 = me:IsPerformingScriptTask()
                            cVar3 = iVar23
                            while cVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00e0a380 end
                                iVar23 = me:IsPerformingScriptTask()
                                cVar3 = iVar23
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if not bVar2 then
                                quest:Pause(2.0)
                                goto LAB_00e09493
                            end
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            uVar7 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(uVar7, r6)
                            -- TODO(native): pCVar11 = (**(*me + 0xc))(&xStack_1c)
                            pCVar11 = nil --[[unresolved native value]]
                            pCVar11 = ("TEXT_QST_067_" .. pCVar11)
                            __push11 = (pCVar11 .. "_GREET_ALLY_10")
                            quest:AddLineToConversation(uVar7, __push11, me, r5, false)
                            -- TODO(native): pCVar12 = (**(*me + 0xc))(piVar14,"_GREET_ALLY_RESPONSE")
                            pCVar12 = nil --[[unresolved native value]]
                            pCVar12 = ("TEXT_QST_067_" .. pCVar12)
                            pCVar12 = (pCVar12 .. "_GREET_ALLY_RESPONSE")
                            quest:AddLineToConversation(uVar7, pCVar12, me, pCVar4, false)
                            -- TODO(native): pCVar12 = (**(*me + 0xc))(b,"_GREET_ALLY_20",0,me,xStack_168)
                            pCVar12 = nil --[[unresolved native value]]
                            pCVar12 = ("TEXT_QST_067_" .. pCVar12)
                            pCVar12 = (pCVar12 .. b)
                            quest:AddLineToConversation(uVar7, pCVar12, me, r4, false)
                            quest:Pause(2.0)
                            quest:RemoveThing(r2, false, true)
                            cVar3 = quest:IsConversationActive(uVar7)
                            while cVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00e0a380 end
                                cVar3 = quest:IsConversationActive(uVar7)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if not bVar2 then
                                goto LAB_00e09493
                            end
                        end
                    end
                    goto FLOW_past_lab_00e09493
                    ::LAB_00e09493::
                    __native_entity_state:SetStateBool("GreetedBuddy", true)
                    iVar23 = xStack_108:IsAlive()
                    if iVar23 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e0a380 end
                        quest:RemoveThing(pCVar10, false, true)
                    end
                    resources:PrepareResource(xStack_168)
                    __push12 = quest:GetHero()
                    quest:EntityFollowThing(me, __push12, 3.0, true)
                    __push13 = quest:GetHero()
                    quest:SetEntityAsRegionFollowing(__push13, me, true)
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
                bVar2 = not alive
                if bVar2 then break end
                quest:EntitySetAsScared(me, true)
            end
            if (__native_entity_state:GetStateInt("Infected") == 0) or (not quest:GetStateBool("SavedInMiddle")) then
                goto LAB_00e095cf
            else
                cVar3 = quest:IsRegionLoaded("Darkwood4")
                if cVar3 then goto LAB_00e095cf end
                -- TODO(native): xStack_168 = xStack_168 | 0x40;
                cVar3 = quest:IsRegionLoaded("BarrowFields")
                c_stk_171 = 0
                if cVar3 then goto LAB_00e095cf end
            end
            goto FLOW_past_lab_00e095cf
            ::LAB_00e095cf::
            c_stk_171 = 1
            ::FLOW_past_lab_00e095cf::
            if (xStack_168 & 0x40) ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xffffffbf;
            end
            if (xStack_168 & 0x20) ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xffffffdf;
            end
            if c_stk_171 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                quest:SetTimer(__native_entity_state:GetStateInt("IncubationTime"), quest:ReadGlobalGameData(0xdf4))
            end
            iVar23 = quest:GetTimer(__native_entity_state:GetStateInt("IncubationTime"))
            if iVar23 == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    quest:SetStateInt("TradersStillAliveCounter", quest:GetStateInt("TradersStillAliveCounter") + -1)
                    resources:PrepareResource(xStack_168)
                    if this_00 == nil then
                        this_00 = 0x0
                    else
                        -- TODO(native): xStack_168 = xStack_168 | 0x380;
                        pCVar12 = extraout_EAX
                        pCVar12 = (a .. pCVar12)
                        -- TODO(native): CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript>(this_00,pCVar12,0);
                        -- TODO(native): *(code **)(this_00 + 0x34) = Script_Darkwood_Balverine_Trader;
                        -- TODO(native): *(undefined4 *)(this_00 + 0x38) = uVar7;
                    end
                    -- TODO(native): CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_00,sectionName);
                    if (xStack_168 & 0x200) ~= 0 then
                        uVar16 = xStack_168 & 0xfffffdff
                    end
                    if (uVar16 & 0x100) ~= 0 then
                        uVar16 = uVar16 & 0xfffffeff
                    end
                    if uVar16 < 0 then
                    end
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                    until not (not bVar2)
                end
                break
            end
            iVar23 = quest:GetStateInt("IncubationTimeHigh")
            iVar15 = quest:GetTimer(__native_entity_state:GetStateInt("IncubationTime"))
            native_arg_sequence_2 = false
            if iVar15 < iVar23 then
                native_arg_sequence_2 = true
            else
                native_arg_sequence_2 = false
            end
            if native_arg_sequence_2 then
                uVar7 = 4
                if __native_entity_state:GetStateInt("Infected") < 4 then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
            end
            if native_arg_sequence_2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                c_stk_171 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "INFECTED_HIGH", me, 0)
                goto LAB_00e096c7
            else
                iVar23 = quest:GetStateInt("IncubationTimeMedium")
                iVar15 = quest:GetTimer(__native_entity_state:GetStateInt("IncubationTime"))
                native_arg_sequence_3 = false
                if iVar15 < iVar23 then
                    native_arg_sequence_3 = true
                else
                    native_arg_sequence_3 = false
                end
                if native_arg_sequence_3 then
                    uVar7 = 3
                    if __native_entity_state:GetStateInt("Infected") < 3 then
                        native_arg_sequence_3 = true
                    else
                        native_arg_sequence_3 = false
                    end
                end
                if native_arg_sequence_3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        c_stk_171 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "INFECTED_MEDIUM", me, 0)
                        goto LAB_00e096c7
                    end
                    break
                end
                iVar23 = quest:GetStateInt("IncubationTimeLow")
                iVar15 = quest:GetTimer(__native_entity_state:GetStateInt("IncubationTime"))
                if (iVar15 < iVar23) and (__native_entity_state:GetStateInt("Infected") < 2) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    c_stk_171 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "INFECTED_LOW", me, 0)
                    if c_stk_171 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then break end
                        __native_entity_state:SetStateInt("Infected", 2)
                    end
                end
            end
            goto FLOW_past_lab_00e096c7
            ::LAB_00e096c7::
            if c_stk_171 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                __native_entity_state:SetStateInt("Infected", uVar7)
            end
            ::FLOW_past_lab_00e096c7::
            -- TODO(native): cVar3 = (**(*me + 0x54))("")
            cVar3 = nil --[[unresolved native value]]
            if cVar3 == 0 then
                -- TODO(native): cVar3 = (**(*me + 0xa8))("")
                cVar3 = nil --[[unresolved native value]]
                if cVar3 ~= 0 then goto LAB_00e09848 end
                goto LAB_00e09886
            else
                goto LAB_00e09848
            end
            goto FLOW_past_lab_00e09886
            ::LAB_00e09886::
            c_stk_171 = 0
            ::FLOW_past_lab_00e09886::
            goto FLOW_past_lab_00e09848
            ::LAB_00e09848::
            -- TODO(native): xStack_168 = xStack_168 | 0x1000;
            -- TODO(native): cVar3 = (**(*me + 0xa4))(0xe,"SCRIPT_NAME_HERO")
            cVar3 = nil --[[unresolved native value]]
            c_stk_171 = 1
            if cVar3 ~= 0 then goto LAB_00e09886 end
            ::FLOW_past_lab_00e09848::
            if (xStack_168 & 0x1000) ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xffffefff;
            end
            if (xStack_168 & 0x800) ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xfffff7ff;
            end
            if (xStack_168 & 0x400) ~= 0 then
                -- TODO(native): xStack_168 = xStack_168 & 0xfffffbff;
            end
            if c_stk_171 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                quest:SetTimer(__native_entity_state:GetStateInt("RegulateFollowStateComment"), 5)
                fVar18 = quest:GetHealth(me)
                if fVar18 <= 5.0 then
                    goto LAB_00e099dc
                else
                    uVar16 = xStack_168
                    -- TODO(native): cVar3 = (**(*me + 0x54))("SCRIPT_NAME_HERO")
                    cVar3 = nil --[[unresolved native value]]
                    if cVar3 ~= 0 then goto LAB_00e099dc end
                    -- TODO(native): xStack_168 = uVar16 | 0x6000;
                    -- TODO(native): cVar3 = (**(*me + 0xa8))("SCRIPT_NAME_HERO")
                    cVar3 = nil --[[unresolved native value]]
                    if cVar3 ~= 0 then
                        -- TODO(native): xStack_168 = xStack_168 | 0x8000;
                        -- TODO(native): cVar3 = (**(*me + 0xa4))(0xe,"SCRIPT_NAME_HERO")
                        cVar3 = nil --[[unresolved native value]]
                        if cVar3 == 0 then goto LAB_00e099dc end
                    end
                    c_stk_171 = 1
                end
                goto FLOW_past_lab_00e099dc
                ::LAB_00e099dc::
                c_stk_171 = 0
                ::FLOW_past_lab_00e099dc::
                if (xStack_168 >> 8) < 0 then
                    -- TODO(native): xStack_168 = xStack_168 & 0xffff7fff;
                end
                if (xStack_168 & 0x4000) ~= 0 then
                    -- TODO(native): xStack_168 = xStack_168 & 0xffffbfff;
                end
                if (xStack_168 & 0x2000) ~= 0 then
                    -- TODO(native): xStack_168 = xStack_168 & 0xffffdfff;
                end
                if c_stk_171 == 0 then
                    fVar18 = quest:GetHealth(me)
                    if fVar18 <= 5.0 then
                        goto LAB_00e09b4f
                    else
                        uVar16 = xStack_168
                        -- TODO(native): cVar3 = (**(*me + 0x54))("SCRIPT_NAME_HERO")
                        cVar3 = nil --[[unresolved native value]]
                        if cVar3 == 0 then
                            -- TODO(native): xStack_168 = uVar16 | 0x30000;
                            -- TODO(native): cVar3 = (**(*me + 0xa8))("SCRIPT_NAME_HERO")
                            cVar3 = nil --[[unresolved native value]]
                            if cVar3 ~= 0 then
                                -- TODO(native): xStack_168 = xStack_168 | 0x40000;
                                -- TODO(native): cVar3 = (**(*me + 0xa4))(0xe,"SCRIPT_NAME_HERO")
                                cVar3 = nil --[[unresolved native value]]
                                if cVar3 == 0 then goto LAB_00e09b48 end
                            end
                            goto LAB_00e09b4f
                        end
                        ::LAB_00e09b48::
                        c_stk_171 = 1
                    end
                    goto FLOW_past_lab_00e09b4f
                    ::LAB_00e09b4f::
                    c_stk_171 = 0
                    ::FLOW_past_lab_00e09b4f::
                    if (xStack_168 & 0x40000) ~= 0 then
                        -- TODO(native): xStack_168 = xStack_168 & 0xfffbffff;
                    end
                    if (xStack_168 & 0x20000) ~= 0 then
                        -- TODO(native): xStack_168 = xStack_168 & 0xfffdffff;
                    end
                    if (xStack_168 & 0x10000) ~= 0 then
                        -- TODO(native): xStack_168 = xStack_168 & 0xfffeffff;
                    end
                    if c_stk_171 == 0 then goto LAB_00e09c13 end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "HERO_HIT_ME", me, 0)
                    iVar23 = 2
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "UNDER_ATTACK", me, 0)
                    iVar23 = 3
                end
                quest:SetTimer(quest:GetStateInt("CommentTimer"), iVar23)
            end
            ::LAB_00e09c13::
            if __native_entity_state:GetStateInt("CurrentAIState") == __native_entity_state:GetStateInt("PreviousAIState") then
                iVar23 = quest:GetTimer(__native_entity_state:GetStateInt("RegulateBanterComment"))
                if (iVar23 == 0) and (not __native_entity_state:GetStateBool("LeadToCamp")) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    if quest:GetStateInt("TradersStillAliveCounter") == 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then break end
                        c_stk_169 = require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "BANTER_SOLO", me, 0)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then break end
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
                        bVar2 = not alive
                        if bVar2 then break end
                        iVar23 = math.random(0, 32767)
                        quest:SetTimer(__native_entity_state:GetStateInt("RegulateBanterComment"), iVar23 % 0xf + 0x19)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then break end
                        iVar23 = math.random(0, 32767)
                        quest:SetTimer(__native_entity_state:GetStateInt("RegulateBanterComment"), iVar23 % 0x14 + 0x1e)
                        quest:SetStateInt("TimesBantered", quest:GetStateInt("TimesBantered") + 1)
                    end
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                iVar23 = quest:GetTimer(__native_entity_state:GetStateInt("RegulateFollowStateComment"))
                if (iVar23 == 0) and (__native_entity_state:GetStateInt("CurrentAIState") == 2) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    require("TraderEscort.native_quest_helpers").MakeTraderComment(quest, me, "FOLLOWING", me, 0)
                    quest:SetTimer(__native_entity_state:GetStateInt("RegulateFollowStateComment"), 10)
                end
            end
            __native_entity_state:SetStateInt("PreviousAIState", __native_entity_state:GetStateInt("CurrentAIState"))
            -- TODO(native): uVar7 = (**(*me + 0x34))()
            uVar7 = nil --[[unresolved native value]]
            __native_entity_state:SetStateInt("CurrentAIState", uVar7)
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
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
    bVar2 = not alive
    if bVar2 then goto LAB_00e08601 end
    resources:PrepareResource(xStack_168)
    cVar3 = me:AcquireControl(4)
    while not cVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e08601 end
        cVar3 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e08601 end
    quest:SetThingAsConscious(me, false, "")
    quest:EntitySetTargetable(me, false)
    quest:EntitySetAsDamageable(me, false)
    cVar3 = quest:GetStateBool("InfectedTraderCanGetUp")
    while not cVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e08601 end
        cVar3 = quest:GetStateBool("InfectedTraderCanGetUp")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e08601 end
    fVar22 = 1.4013e-45
    quest:EntitySetAsDamageable(me, true)
    quest:EntitySetTargetable(me, true)
    quest:SetThingAsConscious(me, true, "")
    r7 = quest:GetThingWithScriptName("MK_DTBE_CUTSCENETRIGGER")
    pCVar4 = quest:GetHero()
    bVar2 = quest:IsDistanceBetweenThingsOver(xStack_14c, pCVar4, fVar22)
    if bVar2 then
        repeat
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e085f8 end
            if quest:GetStateBool("MissionFailed") then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e085f8 end
                r8 = quest:GetThingWithScriptName("MK_DTEB_INFEXIT")
                if CStack_110 == nil then
                    puVar5 = {x = 0, y = 0, z = 0}
                else
                    -- TODO(native): puVar5 = (**(*CStack_110 + 0x18))()
                    puVar5 = nil --[[unresolved native value]]
                end
                -- TODO(native): xStack_14c = puVar5.x;
                -- TODO(native): xStack_158 = *(CCharString *)(puVar5 + 1);
                iVar23 = 2.0
                -- TODO(native): xStack_114 = *(CCharString *)(puVar5 + 2);
                xStack_114 = resources:ScriptThing(xStack_168)
                pvVar6 = xStack_114
                -- TODO(native): iVar23 = IsDistanceFromThingToPositionOver(pvVar6,xStack_14c,iVar23);
                iVar23 = nil --[[unresolved native result]]
                -- TODO(native): unaff_EBX = CONCAT13((char)iVar23,(int3)unaff_EBX);
                cVar3 = (unaff_EBX >> 0x18)
                while cVar3 ~= 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        goto LAB_00e085f8
                    end
                    me:MoveToPosition(xStack_14c, 0, 1, false, true)
                    iVar23 = me:IsPerformingScriptTask()
                    cVar3 = iVar23
                    while cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            goto LAB_00e085f8
                        end
                        iVar23 = me:IsPerformingScriptTask()
                        cVar3 = iVar23
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        goto LAB_00e085f8
                    end
                    iVar23 = 2.0
                    xStack_114 = resources:ScriptThing(xStack_168)
                    pvVar6 = xStack_114
                    -- TODO(native): iVar23 = IsDistanceFromThingToPositionOver(pvVar6,xStack_14c,iVar23);
                    iVar23 = nil --[[unresolved native result]]
                    -- TODO(native): unaff_EBX = CONCAT13((char)iVar23,(int3)unaff_EBX);
                    cVar3 = (unaff_EBX >> 0x18)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    -- LAB_00e07e06: (native jump target)
                    goto LAB_00e085f8
                end
                quest:FadeOutAndKillEntity(me, true, 2.0, true)
            end
            fVar22 = 5.0
            pCVar4 = quest:GetHero()
            bVar2 = quest:IsDistanceBetweenThingsOver(xStack_134, pCVar4, fVar22)
        until not (bVar2)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        -- TODO(native): unaff_EBX = CONCAT13(bVar2,(int3)unaff_EBX);
        -- TODO(native): cVar3 = (**(*me + 0x130))()
        cVar3 = nil --[[unresolved native value]]
        native_arg_sequence_4 = false
        if cVar3 == 0 then
            native_arg_sequence_4 = true
        else
            native_arg_sequence_4 = false
        end
        if native_arg_sequence_4 then
            fVar18 = quest:GetHealth(me)
            if 0.0 < fVar18 then
                native_arg_sequence_4 = true
            else
                native_arg_sequence_4 = false
            end
        end
        if native_arg_sequence_4 then
            r8 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            r9 = quest:GetNearestWithScriptName(me, "DarkwoodTrader")
            r10 = quest:GetFurthestWithScriptName(me, "DarkwoodTrader")
            xStack_e4 = resources:NewResource()
            xStack_114 = resources:NewResource()
            xStack_f4 = resources:NewResource()
            resources:PrepareResource(xStack_e4)
            pCVar4 = quest:GetHero()
            cVar3 = me:AcquireControl(4)
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    -- LAB_00e07e39_c4: (native jump target)
                    resources:ReleaseResource(xStack_f4)
                    resources:ReleaseResource(xStack_114)
                    resources:ReleaseResource(xStack_e4)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(r8)
                    goto LAB_00e085f8
                end
                uVar7 = quest:GetHero()
                cVar3 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                -- TODO(native): cVar3 = (**(*unaff_EBP + 0x138))(xStack_124)
                cVar3 = nil --[[unresolved native value]]
                if cVar3 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        -- TODO(native): iVar23 = CCarriedReadableDef::CCarriedReadableDef__at6e7b40(xStack_bc_2);
                        -- TODO(native): CScriptThing::operator=((CScriptThing *)&xStack_128,iVar23);
                        goto LAB_00e07c7c
                    end
                    -- LAB_00e07e39: (native jump target)
                    resources:ReleaseResource(xStack_f4)
                    resources:ReleaseResource(xStack_114)
                    resources:ReleaseResource(uVar7)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(r8)
                    goto LAB_00e085f8
                end
                ::LAB_00e07c7c::
                pcVar20 = "SCARED"
                pvVar6 = r9:GetDataString()
                iVar23 = ((pvVar6 ~= pcVar20) and 1 or 0)
                if iVar23 == 0 then
                    goto LAB_00e07cac
                else
                    iVar23 = (r10 ~= nil and r10:IsAlive())
                    cVar3 = 1
                    if not iVar23 then goto LAB_00e07cac end
                end
                goto FLOW_past_lab_00e07cac
                ::LAB_00e07cac::
                cVar3 = 0
                ::FLOW_past_lab_00e07cac::
                if cVar3 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e07db9 end
                    -- TODO(native): CScriptThing::operator=((CScriptThing *)&xStack_140,(int)&xStack_128);
                    -- TODO(native): xStack_128 = xStack_bc_2;
                end
                quest:FixMovieSequenceCamera(true)
                xStack_14c = resources:NewActorMap()
                resources:SetActor(xStack_14c, "HERO", uVar7)
                resources:SetActor(xStack_14c, "TRADERI", r6)
                iVar23 = (r9 ~= nil and r9:IsAlive())
                if not iVar23 then
                    goto LAB_00e081e9
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        iVar23 = (r10 ~= nil and r10:IsAlive())
                        if iVar23 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if not bVar2 then
                                resources:PrepareResource(xStack_114)
                                cVar3 = resources:TryAcquire(xStack_114, r9, 4)
                                while cVar3 == 0 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00e07e30 end
                                    cVar3 = resources:TryAcquire(xStack_114, r9, 4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00e07db0 end
                                resources:PrepareResource(xStack_f4)
                                cVar3 = resources:TryAcquire(xStack_f4, r10, 4)
                                while cVar3 == 0 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00e07db0 end
                                    cVar3 = resources:TryAcquire(xStack_f4, r10, 4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if not bVar2 then
                                    resources:SetActor(xStack_14c, "TRADERS", xStack_114)
                                    resources:SetActor(xStack_14c, "TRADERN", xStack_f4)
                                    pcVar20 = "CS_DARKWOOD_TRADER_INFECTED_BOTH"
                                    goto LAB_00e081c2
                                end
                            end
                            ::LAB_00e07e30::
                            resources:DestroyActorMap(xStack_14c)
                            resources:ReleaseResource(xStack_f4)
                            resources:ReleaseResource(xStack_114)
                            resources:ReleaseResource(uVar7)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(r8)
                            goto LAB_00e085f8
                        end
                        piVar9 = r9:GetDataString()
                        if piVar9 == nil then
                            piVar14 = r8
                            cVar3 = false
                        else
                            iVar23 = ((piVar9 == "SCARED") and 0 or 1)
                            cVar3 = not (iVar23 ~= 0)
                        end
                        if not cVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                resources:DestroyActorMap(xStack_14c)
                                resources:ReleaseResource(xStack_f4)
                                resources:ReleaseResource(xStack_114)
                                resources:ReleaseResource(uVar7)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(r8)
                                goto LAB_00e085f8
                            end
                            resources:PrepareResource(xStack_f4)
                            cVar3 = resources:TryAcquire(xStack_f4, r9, 4)
                            while cVar3 == 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    resources:DestroyActorMap(xStack_14c)
                                    resources:ReleaseResource(xStack_f4)
                                    resources:ReleaseResource(xStack_114)
                                    resources:ReleaseResource(uVar7)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(r8)
                                    goto LAB_00e085f8
                                end
                                cVar3 = resources:TryAcquire(xStack_f4, r9, 4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if not bVar2 then
                                resources:SetActor(xStack_14c, "TRADERN", xStack_f4)
                                pcVar20 = "CS_DARKWOOD_TRADER_INFECTED_NORMAL"
                                goto LAB_00e081c2
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if not bVar2 then
                                resources:PrepareResource(xStack_114)
                                cVar3 = resources:TryAcquire(xStack_114, r9, 4)
                                while cVar3 == 0 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00e07db0 end
                                    cVar3 = resources:TryAcquire(xStack_114, r9, 4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if not bVar2 then
                                    resources:SetActor(xStack_14c, "TRADERS", xStack_114)
                                    pcVar20 = "CS_DARKWOOD_TRADER_INFECTED_SCARED"
                                    goto LAB_00e081c2
                                end
                                goto FLOW_hoist_lab_00e081c2_1
                            end
                        end
                        goto FLOW_past_lab_00e081c2
                        ::LAB_00e081c2::
                        resources:RunMacro(pcVar20, xStack_14c, false, true)
                        goto LAB_00e081e9
                        ::FLOW_hoist_lab_00e081c2_1::
                        resources:DestroyActorMap(xStack_14c)
                        resources:ReleaseResource(xStack_f4)
                        resources:ReleaseResource(xStack_114)
                        resources:ReleaseResource(uVar7)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(r8)
                        goto LAB_00e085f8
                        ::FLOW_past_lab_00e081c2::
                    end
                end
                goto FLOW_past_lab_00e081e9
                ::LAB_00e081e9::
                quest:GiveHeroYesNoQuestion("TEXT_QST_067_INFECTED_ASK_TO_FOLLOW", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                iVar23 = quest:MsgIsQuestionAnsweredYesOrNo()
                while iVar23 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        -- LAB_00e07e30_c9: (native jump target)
                        resources:DestroyActorMap(xStack_14c)
                        resources:ReleaseResource(xStack_f4)
                        resources:ReleaseResource(xStack_114)
                        resources:ReleaseResource(uVar7)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(r8)
                        goto LAB_00e085f8
                    end
                    iVar23 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if iVar23 == 1 then
                        if bVar2 then
                            -- LAB_00e07e30_c10: (native jump target)
                            resources:DestroyActorMap(xStack_14c)
                            resources:ReleaseResource(xStack_f4)
                            resources:ReleaseResource(xStack_114)
                            resources:ReleaseResource(uVar7)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(r8)
                            goto LAB_00e085f8
                        end
                        pcVar20 = "CS_DARKWOOD_TRADER_INFECTED_JOINS"
                    else
                        if bVar2 then goto LAB_00e07db0 end
                        pcVar20 = "CS_DARKWOOD_TRADER_INFECTED_LEAVES"
                    end
                    resources:RunMacro(pcVar20, xStack_14c, false, true)
                    pCVar4 = 0x0
                    quest:FixMovieSequenceCamera(false)
                    resources:DestroyActorMap(xStack_14c)
                    resources:ReleaseResource(xStack_f4)
                    resources:ReleaseResource(xStack_114)
                    resources:ReleaseResource(uVar7)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(r8)
                    if not unaff_EBX_b3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            quest:GiveHeroMorality(__unknown_push)
                            quest:RemoveThing(me, false, true)
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            resources:PrepareResource(xStack_168)
                            quest:ClearThingHasInformation(me)
                            goto LAB_00e083cd
                        end
                    end
                    goto LAB_00e085f8
                end
                ::FLOW_past_lab_00e081e9::
                ::LAB_00e07db0::
                resources:DestroyActorMap(xStack_14c)
            end
            ::LAB_00e07db9::
            resources:ReleaseResource(xStack_f4)
            resources:ReleaseResource(xStack_114)
            resources:ReleaseResource(uVar7)
            -- TODO(native): (**(code **)(*piVar14 + 0x5ec))(0);
            resources:DestroyMovie(r8)
        else
            while true do
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not (not bVar2) then break end
                alive = quest:NewScriptFrame(me)
            end
        end
    end
    ::LAB_00e085f8::
    ::LAB_00e08601::
    resources:ReleaseResource(xStack_168)
    do return end
    ::LAB_00e09fd9::
    if cVar3 ~= 0 then goto LAB_00e0a00b end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e0a346 end
    cVar3 = resources:TryAcquire(xStack_168, me, 4)
    goto LAB_00e09fd9
    ::LAB_00e0a00b::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        bVar2 = quest:IsDistanceBetweenThingsOver(me, r6, 7.0)
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e0a346 end
                iVar23 = me:IsPerformingScriptTask()
                if not iVar23 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e0a346 end
                    me:MoveToThing(uVar7, 4.0, 0)
                end
                bVar2 = quest:IsDistanceBetweenThingsOver(me, r6, 7.0)
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            resources:PrepareResource(xStack_168)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                repeat
                    -- TODO(native): cVar3 = (**(*me + 0x6c))("SCRIPT_NAME_HERO")
                    cVar3 = nil --[[unresolved native value]]
                    if cVar3 ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then break end
                        resources:PrepareResource(xStack_168)
                        cVar3 = resources:TryAcquire(xStack_168, me, 4)
                        while cVar3 == 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e0a346 end
                            cVar3 = resources:TryAcquire(xStack_168, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then break end
                        r8 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        xStack_114 = resources:ScriptThing(stack0xfffffe48)
                        pCVar4 = xStack_114
                        fVar18 = quest:GetHealth(pCVar4)
                        -- TODO(native): CVar19._0_3_ = CVar24._0_3_;
                        -- TODO(native): CVar19._3_1_ = 1;
                        if fVar18 <= 0.0 then
                            CVar19 = me & 0xffffff
                        end
                        CVar24 = CVar19
                        if CVar24_b3 ~= 0 then
                            p5 = 0
                            p4 = 1
                            p3 = 0
                            iVar15 = 0
                            pcVar20 = "_THANKS"
                            -- TODO(native): pCVar11 = (**(*me + 0xc))(&xStack_c8)
                            pCVar11 = nil --[[unresolved native value]]
                            pCVar11 = ("TEXT_QST_067_" .. pCVar11)
                            pCVar12 = (pCVar11 .. "_THANKS")
                            pvVar6 = pCVar12
                            iVar23 = quest:GetHero()
                            r11 = me:Speak(iVar23, pcVar20, iVar15, (p3 ~= 0), (p4 ~= 0), (p5 ~= 0))
                            iVar23 = me:IsPerformingScriptTask()
                            cVar3 = iVar23
                            while cVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00e0a33a
                                end
                                iVar23 = me:IsPerformingScriptTask()
                                cVar3 = iVar23
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00e0a33a
                            end
                            goto FLOW_past_lab_00e0a33a
                            ::LAB_00e0a33a::
                            resources:ReleaseResource(uVar7)
                            break
                            ::FLOW_past_lab_00e0a33a::
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(uVar7)
                        resources:PrepareResource(r6)
                    end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        resources:ReleaseResource(r6)
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
    local bVar2, bVar3, bVar4, cVar9, iVar7, local_20, p1, pCVar6, pcVar10, piVar5, this_00, this_01, uVar1
    bVar4 = false
    bVar3 = false
    bVar2 = false
    __native_entity_state:SetStateInt("BrainState", 0)
    __native_entity_state:SetStateInt("CurrentAIState", 0)
    __native_entity_state:SetStateInt("PreviousAIState", 0)
    piVar5 = me:GetDataString()
    iVar7 = ((piVar5 == "INFECTED") and 0 or 1)
    cVar9 = not (iVar7 ~= 0)
    if cVar9 then
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
    pCVar6 = me:GetDataString()
    iVar7 = ((pCVar6 == "INFECTED") and 0 or 1)
    cVar9 = not (iVar7 ~= 0)
    if not cVar9 then
        pCVar6 = me:GetDataString()
        iVar7 = ((pCVar6 == "SCARED") and 0 or 1)
        cVar9 = not (iVar7 ~= 0)
        if not cVar9 then
            pCVar6 = me:GetDataString()
            iVar7 = ((pCVar6 == "FRIENDLY") and 0 or 1)
            cVar9 = not (iVar7 ~= 0)
            if not cVar9 then goto LAB_00e04e13 end
            pcVar10 = "FRIENDLY_TRADER"
        else
            pcVar10 = "SCARED_TRADER"
        end
    else
        pcVar10 = "INFECTED_TRADER"
    end
    quest:EntitySetWillBeUsingNarrator(me, pcVar10)
    ::LAB_00e04e13::
    if this_01 == nil then
        this_01 = 0x0
    else
        -- TODO(native): xStack_10 = *(CCharString (*) [4])(this + 0xc);
        -- TODO(native): local_20 = *(int **)(this + 0x10);
        bVar4 = true
        bVar3 = true
        bVar2 = true
        if local_20 ~= nil then
            -- TODO(native): *local_20 = *local_20 + 1;
        end
        pCVar6 = extraout_EAX
        pCVar6 = (a .. pCVar6)
        -- TODO(native): CSpawnedFunc<NScript::CExpression_FollowScript>::CSpawnedFunc<NScript::CExpression_FollowScript> (this_01,pCVar6,0);
        -- TODO(native): *(code **)(this_01 + 0x34) = NScript::CQ_TraderEscortScript::WatchForPickpocketing;
        -- TODO(native): *(undefined4 *)(this_01 + 0x38) = uVar1;
    end
    -- TODO(native): CGuiVarTransferStruct::Add(*(CGuiVarTransferStruct **)(this + 0x14),this_01,sectionName);
    if bVar2 then
    end
    if bVar3 then
    end
    if bVar4 then
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
    local CStack_24, bVar1, iVar4, pCVar2, pcVar5, pcVar6, this_00, uStack_1c, uStack_1c_b0, uStack_20, uStack_30, uVar3
    local alive = true
    __native_entity_state:SetStateInt("BrainState", native_arg_brain_state)
    if native_arg_brain_state == 2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        native_arg_brain_state = bVar1
        if not bVar1 then
            uStack_20 = quest:GetHero()
            CStack_24 = this_00
            quest:EntityFollowThing(me, uStack_20, 3.0, true)
            uStack_30 = quest:GetHero()
            quest:SetEntityAsRegionFollowing(uStack_30, me, true)
            quest:DisplayQuestInfo(true)
            -- TODO(native): pCVar2 = (**(*me + 0xc))(me,&CStack_24)
            pCVar2 = nil --[[unresolved native value]]
            if pCVar2 == nil then
                bVar1 = false
                uStack_1c_b0 = bVar1
            else
                iVar4 = ((pCVar2 == "INFECTED") and 0 or 1)
                -- TODO(native): uStack_1c_b0 = !(iVar4 != 0);
            end
            if 3.0 == 0 then
                -- TODO(native): pCVar2 = (**(*me + 0xc))(me,&CStack_24)
                pCVar2 = nil --[[unresolved native value]]
                if pCVar2 == nil then
                    iVar4 = 7
                    repeat
                        if iVar4 == 0 then break end
                        iVar4 = iVar4 + -1
                        -- TODO(native): uStack_1c_b0 = *pcVar5 == *pcVar6;
                        pcVar5 = "SCARED" + 1
                        pcVar6 = "" + 1
                    until not (3.0)
                else
                    iVar4 = ((pCVar2 == "SCARED") and 0 or 1)
                    -- TODO(native): uStack_1c_b0 = !(iVar4 != 0);
                end
                if 3.0 == 0 then
                    -- TODO(native): pCVar2 = (**(*me + 0xc))(me,&CStack_24)
                    pCVar2 = nil --[[unresolved native value]]
                    if pCVar2 == nil then
                        bVar1 = false
                        -- TODO(native): uStack_1c = CONCAT31(uStack_1c._1_3_,bVar1);
                    else
                        iVar4 = ((pCVar2 == "FRIENDLY") and 0 or 1)
                        -- TODO(native): uStack_1c = CONCAT31(uStack_1c._1_3_,!(iVar4 != 0));
                    end
                    native_arg_brain_state = CONCAT31((int3)(extraout_EAX >> 8),uStack_1c)
                    if uStack_1c ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        native_arg_brain_state = bVar1
                        if not bVar1 then
                            uVar3 = quest:AddQuestInfoBarHealth(me, 0xffff0000, "HUD_QUEST_ICON_TRADER_HAT_02", 1.0)
                            __native_entity_state:SetStateInt("TraderHealthID", uVar3)
                            native_arg_brain_state = extraout_EAX_00
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    native_arg_brain_state = bVar1
                    if not bVar1 then
                        uVar3 = quest:AddQuestInfoBarHealth(me, 0xffff0000, "HUD_QUEST_ICON_TRADER_HAT_01", 1.0)
                        __native_entity_state:SetStateInt("TraderHealthID", uVar3)
                        iVar4 = quest:EntitySetAsScared(me, true)
                        return iVar4
                    end
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                native_arg_brain_state = bVar1
                if not bVar1 then
                    uVar3 = quest:AddQuestInfoBarHealth(me, 0xffff0000, "HUD_QUEST_ICON_TRADER", 1.0)
                    __native_entity_state:SetStateInt("TraderHealthID", uVar3)
                    __native_entity_state:SetStateInt("Infected", 1)
                    if not quest:GetStateBool("SavedNearEnd") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        native_arg_brain_state = bVar1
                        if not bVar1 then
                            iVar4 = quest:SetTimer(__native_entity_state:GetStateInt("IncubationTime"), quest:ReadGlobalGameData(0xdf4))
                            return iVar4
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        native_arg_brain_state = bVar1
                        if not bVar1 then
                            iVar4 = quest:SetTimer(__native_entity_state:GetStateInt("IncubationTime"), quest:ReadGlobalGameData(0xdf8))
                            return iVar4
                        end
                    end
                end
            end
        end
    end
    return native_arg_brain_state
end

