-- Generated native draft: CampHostageGuard. Review coverage report before use.
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
    local bVar2, cVar3, c_stk_d5, c_stk_d6, fVar1, fVar15, fret_0, fret_00, fret_01, fret_02, fret_03, iVar13, iVar14, iVar16, iVar4, iVar6, i_stk_dc, i_stk_e0, native_arg_sequence_1, native_arg_switch_1, native_arg_switch_2, p0, pCVar5, pCVar7, pCVar8, pcVar12, r1, r2, r3, r4, r5, r6, uVar10, uVar9, u_stk_b4, u_stk_d4, xStack_30, xStack_48, xStack_98, xStack_b0, xStack_d0
    local alive = true
    uVar9 = 0
    u_stk_d4 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    xStack_d0 = resources:NewResource()
    resources:PrepareResource(xStack_d0)
    bVar2 = resources:TryAcquire(xStack_d0, me, 4)
    while not bVar2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d0a6ae end
        bVar2 = resources:TryAcquire(xStack_d0, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00d0a6ae end
    i_stk_e0 = quest:RegisterTimer()
    quest:SetTimer(i_stk_e0, 0)
    u_stk_b4 = 0
    c_stk_d5 = 0
    r1 = quest:GetNearestWithScriptName(me, "CampHostage")
    quest:EntitySetFacingAngleTowardsThing(me, r1, false)
    c_stk_d6 = 0
    iVar4 = quest:RegisterTimer()
    __native_entity_state:SetStateBool("DropPass", true)
    cVar3 = quest:GetStateBool("HostagesRescued")
    i_stk_dc = iVar4
    while not cVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d0a693 end
        bVar2 = quest:MsgOnHeroPickedPocket()
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d0a693 end
            __native_entity_state:SetStateBool("DropPass", false)
        end
        fVar15 = 7.0
        pCVar5 = quest:GetHero()
        bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar5, fVar15)
        native_arg_sequence_1 = false
        if not bVar2 then
            native_arg_sequence_1 = true
        else
            native_arg_sequence_1 = false
        end
        if not native_arg_sequence_1 then
            bVar2 = quest:IsDistanceBetweenThingsUnder(me, r1, 12.0)
            if not bVar2 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if not native_arg_sequence_1 then
            iVar6 = quest:GetTimer(i_stk_e0)
            if 0 < iVar6 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
        end
        if native_arg_sequence_1 then goto LAB_00d09458 end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d0a693 end
        iVar4 = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(iVar4, r1)
        native_arg_switch_1 = u_stk_b4
        repeat
            if native_arg_switch_1 == 0 then
                quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_FIRST", me, r1, false)
                u_stk_b4 = 1
                break
            else
                if native_arg_switch_1 == 1 then
                    quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_SECOND", me, r1, false)
                    u_stk_b4 = 2
                    break
                else
                    if native_arg_switch_1 == 2 then
                        quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_THIRD", me, r1, false)
                        u_stk_b4 = 3
                        break
                    else
                        if native_arg_switch_1 == 3 then
                            quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_FOURTH", me, r1, false)
                            u_stk_b4 = 4
                            break
                        else
                            if native_arg_switch_1 == 4 then
                                quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_FIFTH", me, r1, false)
                                goto LAB_00d09432
                            else
                                if native_arg_switch_1 == 5 then
                                    quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_SIXTH", me, r1, false)
                                    u_stk_b4 = 6
                                    break
                                else
                                    if native_arg_switch_1 == 6 then
                                        quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_SEVENTH", me, r1, false)
                                        goto LAB_00d09432
                                    end
                                end
                            end
                            goto FLOW_past_lab_00d09432
                            ::LAB_00d09432::
                            u_stk_b4 = 5
                            ::FLOW_past_lab_00d09432::
                        end
                    end
                end
            end
        until not (false)
        quest:SetTimer(i_stk_e0, 10)
        iVar4 = i_stk_dc
        ::LAB_00d09458::
        if 0x1 == 0x1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d0a693 end
            fVar15 = 2.0
            uVar10 = uVar9 | 3
            u_stk_d4 = uVar10
            pCVar5 = quest:GetThingWithScriptName("GuardFirstMarker")
            bVar2 = quest:IsDistanceBetweenThingsOver(me, pCVar5, fVar15)
            if bVar2 then
                iVar6 = me:IsPerformingScriptTask()
                bVar2 = true
                if iVar6 then goto LAB_00d094cb end
            else
                goto LAB_00d094cb
            end
            goto FLOW_past_lab_00d094cb
            ::LAB_00d094cb::
            bVar2 = false
            ::FLOW_past_lab_00d094cb::
            if (uVar10 & 2) ~= 0 then
                uVar10 = uVar10 & 0xfffffffd
                u_stk_d4 = uVar10
            end
            if (uVar10 & 1) ~= 0 then
                uVar10 = uVar10 & 0xfffffffe
                u_stk_d4 = uVar10
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d0a693 end
                pCVar5 = quest:GetThingWithScriptName("GuardFirstMarker")
                iVar16 = 1
                iVar14 = 0
                iVar13 = 0
                iVar6 = 1.0
                pCVar7 = pCVar5:GetPos()
                me:MoveToPosition(pCVar7, iVar6, iVar13, (iVar14 ~= 0), (iVar16 ~= 0))
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d0a693 end
                iVar6 = quest:GetTimer(iVar4)
                if iVar6 < 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d0a693 end
                    if c_stk_d6 ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if not bVar2 then
                            goto LAB_00d09756
                        end
                        goto LAB_00d0a693
                    end
                    goto LAB_00d095b5
                end
            end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d0a693 end
            fVar15 = 2.0
            uVar10 = uVar9 | 0xc
            u_stk_d4 = uVar10
            pCVar5 = quest:GetThingWithScriptName("GuardSecondMarker")
            bVar2 = quest:IsDistanceBetweenThingsOver(me, pCVar5, fVar15)
            if bVar2 then
                iVar6 = me:IsPerformingScriptTask()
                bVar2 = true
                if iVar6 then goto LAB_00d09657 end
            else
                goto LAB_00d09657
            end
            goto FLOW_past_lab_00d09657
            ::LAB_00d09657::
            bVar2 = false
            ::FLOW_past_lab_00d09657::
            if (uVar10 & 8) ~= 0 then
                uVar10 = uVar10 & 0xfffffff7
                u_stk_d4 = uVar10
            end
            if (uVar10 & 4) ~= 0 then
                uVar10 = uVar10 & 0xfffffffb
                u_stk_d4 = uVar10
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d0a693 end
                pCVar5 = quest:GetThingWithScriptName("GuardSecondMarker")
                iVar16 = 1
                iVar14 = 0
                iVar13 = 0
                iVar6 = 1.0
                pCVar7 = pCVar5:GetPos()
                me:MoveToPosition(pCVar7, iVar6, iVar13, (iVar14 ~= 0), (iVar16 ~= 0))
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d0a693 end
                iVar6 = quest:GetTimer(iVar4)
                if iVar6 < 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d0a693 end
                    if c_stk_d6 == 0 then goto LAB_00d095b5 end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d0a693 end
                    goto LAB_00d09756
                end
            end
        end
        goto FLOW_past_lab_00d095b5
        ::LAB_00d095b5::
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00d0a693 end
        c_stk_d6 = 1
        quest:SetTimer(iVar4, 10)
        ::FLOW_past_lab_00d095b5::
        goto FLOW_past_lab_00d09756
        ::LAB_00d09756::
        c_stk_d6 = 0
        ::FLOW_past_lab_00d09756::
        bVar2 = me:IsTalkedToByHero()
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00d0a693 end
            me:ClearCommands()
            if c_stk_d5 == 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d0a693 end
                xStack_98 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                xStack_30 = resources:ScriptThing(xStack_d0)
                pCVar5 = xStack_30
                fret_0 = quest:GetHealth(pCVar5)
                fVar1 = 0.0
                if fVar1 < fret_0 then
                    iVar14 = 0
                    iVar13 = 1
                    iVar6 = 0
                    iVar4 = 0
                    pcVar12 = "TEXT_QST_009_HOSTAGE_GUARD_FIRST_CHAT"
                    pCVar5 = quest:GetHero()
                    r2 = me:Speak(pCVar5, pcVar12, iVar4, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                    iVar4 = me:IsPerformingScriptTask()
                    cVar3 = iVar4
                    while cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar8 = xStack_98
                            -- LAB_00d09e08: (native jump target)
                            quest:DeregisterTimer(i_stk_dc)
                            quest:DeregisterTimer(i_stk_e0)
                            resources:ReleaseResource(xStack_d0)
                            return
                        end
                        iVar4 = me:IsPerformingScriptTask()
                        cVar3 = iVar4
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d09dba end
                end
                quest:EntitySetFacingAngleTowardsThing(me, r1, false)
                c_stk_d5 = 1
                quest:PauseAllNonScriptedEntities(false)
                pCVar8 = xStack_98
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00d0a693 end
                xStack_b0 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                xStack_48 = resources:ScriptThing(xStack_d0)
                pCVar5 = xStack_48
                fret_00 = quest:GetHealth(pCVar5)
                fVar1 = 0.0
                if fVar1 < fret_00 then
                    iVar14 = 0
                    iVar13 = 1
                    iVar6 = 0
                    iVar4 = 0
                    pcVar12 = "TEXT_QST_009_HOSTAGE_GUARD_SECOND_CHAT"
                    pCVar5 = quest:GetHero()
                    r3 = me:Speak(pCVar5, pcVar12, iVar4, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                    iVar4 = me:IsPerformingScriptTask()
                    cVar3 = iVar4
                    while cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar8 = xStack_b0
                            quest:DeregisterTimer(i_stk_dc)
                            quest:DeregisterTimer(i_stk_e0)
                            resources:ReleaseResource(xStack_d0)
                            return
                        end
                        iVar4 = me:IsPerformingScriptTask()
                        cVar3 = iVar4
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00d0a4c8 end
                end
                quest:EntitySetFacingAngleTowardsThing(me, r1, false)
                quest:PauseAllNonScriptedEntities(false)
                pCVar8 = xStack_b0
            end
            uVar10 = u_stk_d4
            iVar4 = i_stk_dc
        end
        uVar9 = uVar10 | 0x10
        bVar2 = me:MsgIsHitByHero()
        if bVar2 then
            goto LAB_00d09aee
        else
            uVar9 = uVar10 | 0x30
            bVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar2 then
                uVar9 = uVar10 | 0x70
                bVar2 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar2 then goto LAB_00d09aee end
            end
            bVar2 = false
            if quest:GetStateBool("HostageKilled") then goto LAB_00d09aee end
        end
        goto FLOW_past_lab_00d09aee
        ::LAB_00d09aee::
        bVar2 = true
        ::FLOW_past_lab_00d09aee::
        if (uVar9 & 0x40) ~= 0 then
            uVar9 = uVar9 & 0xffffffbf
        end
        if (uVar9 & 0x20) ~= 0 then
            uVar9 = uVar9 & 0xffffffdf
        end
        if (uVar9 & 0x10) ~= 0 then
            uVar9 = uVar9 & 0xffffffef
        end
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                quest:ClearThingHasInformation(me)
                pCVar5 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(me, pCVar5)
                resources:PrepareResource(xStack_d0)
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                until not (not bVar2)
                quest:DeregisterTimer(iVar4)
                quest:DeregisterTimer(i_stk_e0)
                resources:ReleaseResource(xStack_d0)
                return
            end
            goto LAB_00d0a693
        end
        cVar3 = quest:GetStateBool("HostagesRescued")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        c_stk_d5 = bVar2
        quest:ClearThingHasInformation(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        while not bVar2 do
            fVar15 = 2.0
            uVar10 = uVar9 | 0x180
            u_stk_d4 = uVar10
            pCVar5 = quest:GetThingWithScriptName("GuardFirstMarker")
            bVar2 = quest:IsDistanceBetweenThingsOver(me, pCVar5, fVar15)
            if bVar2 then
                iVar4 = me:IsPerformingScriptTask()
                bVar2 = true
                if iVar4 then goto LAB_00d09bc7 end
            else
                goto LAB_00d09bc7
            end
            goto FLOW_past_lab_00d09bc7
            ::LAB_00d09bc7::
            bVar2 = false
            ::FLOW_past_lab_00d09bc7::
            if (uVar10 & 0x100) ~= 0 then
                uVar10 = uVar10 & 0xfffffeff
                u_stk_d4 = uVar10
            end
            if uVar10 < 0 then
                uVar10 = uVar10 & 0xffffff7f
                u_stk_d4 = uVar10
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                pCVar5 = quest:GetThingWithScriptName("GuardFirstMarker")
                iVar14 = 1
                iVar13 = 0
                iVar6 = 0
                iVar4 = 1.0
                pCVar7 = pCVar5:GetPos()
                me:MoveToPosition(pCVar7, iVar4, iVar6, (iVar13 ~= 0), (iVar14 ~= 0))
            end
            fVar15 = 7.0
            pCVar5 = quest:GetHero()
            bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar5, fVar15)
            if bVar2 then
                fVar15 = 12.0
                uVar10 = uVar10 | 0x600
                u_stk_d4 = uVar10
                pCVar5 = quest:GetThingWithScriptName("GuardFirstMarker")
                bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar5, fVar15)
                if not bVar2 then goto LAB_00d09cf7 end
                iVar4 = quest:GetTimer(i_stk_e0)
                bVar2 = true
                if 0 < iVar4 then goto LAB_00d09cf7 end
            else
                goto LAB_00d09cf7
            end
            goto FLOW_past_lab_00d09cf7
            ::LAB_00d09cf7::
            bVar2 = false
            ::FLOW_past_lab_00d09cf7::
            if (uVar10 & 0x400) ~= 0 then
                uVar10 = uVar10 & 0xfffffbff
                u_stk_d4 = uVar10
            end
            if (uVar10 & 0x200) ~= 0 then
                uVar10 = uVar10 & 0xfffffdff
                u_stk_d4 = uVar10
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                iVar4 = quest:AddNewConversation(me, false, false)
                pCVar5 = quest:GetHero()
                quest:AddPersonToConversation(iVar4, pCVar5)
                native_arg_switch_2 = u_stk_b4
                repeat
                    if native_arg_switch_2 == 0 then
                        pCVar5 = quest:GetHero()
                        quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIRST", me, pCVar5, false)
                        u_stk_b4 = 1
                        uVar10 = u_stk_d4
                        break
                    else
                        if native_arg_switch_2 == 1 then
                            pCVar5 = quest:GetHero()
                            quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SECOND", me, pCVar5, false)
                            u_stk_b4 = 2
                            uVar10 = u_stk_d4
                            break
                        else
                            if native_arg_switch_2 == 2 then
                                pCVar5 = quest:GetHero()
                                quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_THIRD", me, pCVar5, false)
                                u_stk_b4 = 3
                                uVar10 = u_stk_d4
                                break
                            else
                                if native_arg_switch_2 == 3 then
                                    pCVar5 = quest:GetHero()
                                    quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FOURTH", me, pCVar5, false)
                                    u_stk_b4 = 4
                                    uVar10 = u_stk_d4
                                    break
                                else
                                    if native_arg_switch_2 == 4 then
                                        pCVar5 = quest:GetHero()
                                        quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIFTH", me, pCVar5, false)
                                        u_stk_b4 = 5
                                        uVar10 = u_stk_d4
                                        break
                                    else
                                        if native_arg_switch_2 == 5 then
                                            pCVar5 = quest:GetHero()
                                            quest:AddLineToConversation(iVar4, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SIXTH", me, pCVar5, false)
                                            pCVar5 = quest:GetHero()
                                            quest:GiveThingBestEnemyTarget(me, pCVar5)
                                            resources:PrepareResource(xStack_d0)
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar2 = not alive
                                            until not (not bVar2)
                                            goto LAB_00d0a693
                                        end
                                    end
                                end
                            end
                        end
                    end
                until not (false)
                quest:SetTimer(i_stk_e0, 10)
            end
            bVar2 = me:IsTalkedToByHero()
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                me:ClearCommands()
                if not c_stk_d5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    xStack_b0 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    pCVar5 = resources:ScriptThing(xStack_d0)
                    pCVar5 = pCVar5
                    fret_01 = quest:GetHealth(pCVar5)
                    fVar1 = 0.0
                    if fVar1 < fret_01 then
                        iVar14 = 0
                        iVar13 = 1
                        iVar6 = 0
                        iVar4 = 0
                        pcVar12 = "TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_FIRST"
                        pCVar5 = quest:GetHero()
                        r4 = me:Speak(pCVar5, pcVar12, iVar4, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                        iVar4 = me:IsPerformingScriptTask()
                        cVar3 = iVar4
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00d0a4c8 end
                            iVar4 = me:IsPerformingScriptTask()
                            cVar3 = iVar4
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00d0a4c8 end
                    end
                    c_stk_d5 = 1
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar8 = xStack_b0
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    xStack_98 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    pCVar5 = resources:ScriptThing(xStack_d0)
                    pCVar5 = pCVar5
                    fret_02 = quest:GetHealth(pCVar5)
                    fVar1 = 0.0
                    if fVar1 < fret_02 then
                        iVar14 = 0
                        iVar13 = 1
                        iVar6 = 0
                        iVar4 = 0
                        pcVar12 = "TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_SECOND"
                        pCVar5 = quest:GetHero()
                        r5 = me:Speak(pCVar5, pcVar12, iVar4, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                        iVar4 = me:IsPerformingScriptTask()
                        cVar3 = iVar4
                        while cVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d09dc7
                            end
                            iVar4 = me:IsPerformingScriptTask()
                            cVar3 = iVar4
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00d09dba end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar8 = xStack_98
                end
                uVar10 = u_stk_d4
            end
            uVar9 = uVar10 | 0x800
            bVar2 = me:MsgIsHitByHero()
            if bVar2 then
                goto LAB_00d0a3d3
            else
                uVar9 = uVar10 | 0x1800
                bVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar2 then
                    uVar9 = uVar10 | 0x3800
                    bVar2 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar2 then goto LAB_00d0a3d3 end
                end
                bVar2 = false
            end
            goto FLOW_past_lab_00d0a3d3
            ::LAB_00d0a3d3::
            bVar2 = true
            ::FLOW_past_lab_00d0a3d3::
            if (uVar9 & 0x2000) ~= 0 then
                uVar9 = uVar9 & 0xffffdfff
            end
            if (uVar9 & 0x1000) ~= 0 then
                uVar9 = uVar9 & 0xffffefff
            end
            if (uVar9 & 0x800) ~= 0 then
                uVar9 = uVar9 & 0xfffff7ff
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    me:ClearCommands()
                    xStack_b0 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    pCVar5 = resources:ScriptThing(xStack_d0)
                    pCVar5 = pCVar5
                    fret_03 = quest:GetHealth(pCVar5)
                    fVar1 = 0.0
                    if fret_03 <= fVar1 then goto LAB_00d0a62b end
                    iVar14 = 0
                    iVar13 = 1
                    iVar6 = 0
                    iVar4 = 0
                    pcVar12 = "TEXT_QST_009_HOSTAGE_GUARD_LATE_HIT"
                    pCVar5 = quest:GetHero()
                    r6 = me:Speak(pCVar5, pcVar12, iVar4, (iVar6 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                    iVar4 = me:IsPerformingScriptTask()
                    cVar3 = iVar4
                    goto LAB_00d0a5d1
                end
                break
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
        end
    end
    ::LAB_00d0a693::
    quest:DeregisterTimer(i_stk_dc)
    quest:DeregisterTimer(i_stk_e0)
    ::LAB_00d0a6ae::
    resources:ReleaseResource(xStack_d0)
    do return end
    ::LAB_00d09dba::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d09dc7::
    resources:DestroyMovie(xStack_98)
    goto LAB_00d0a693
    ::LAB_00d0a5d1::
    if not cVar3 then goto LAB_00d0a5f7 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00d0a4c8 end
    iVar4 = me:IsPerformingScriptTask()
    cVar3 = iVar4
    goto LAB_00d0a5d1
    ::LAB_00d0a4c8::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d0a60f::
    resources:DestroyMovie(xStack_b0)
    goto LAB_00d0a693
    ::LAB_00d0a5f7::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        -- LAB_00d0a602: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
        goto LAB_00d0a60f
    end
    ::LAB_00d0a62b::
    pCVar5 = quest:GetHero()
    quest:GiveThingBestEnemyTarget(me, pCVar5)
    resources:PrepareResource(xStack_d0)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_b0)
    repeat
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    until not (not bVar2)
    goto LAB_00d0a693
end

function Init(quest, me)
    quest:AddItemToContainer(me, "OBJECT_BANDIT_CAMP_HOSTAGE_KEY")
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:EntitySetAsOpinionSource(me, "OPINION_SOURCE_BANDIT_HOSTAGE_GUARD")
    quest:EntitySetDeathContainerAsEnabled(me, false)
    quest:SetThingHasInformation(me, false, true, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    if __native_entity_state:GetStateBool("DropPass") then
        quest:GiveHeroObject("OBJECT_BANDIT_CAMP_HOSTAGE_KEY", -1, false)
    end
end

