-- Generated native draft: Gate2Guard1. Review coverage report before use.
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
    local bVar3, bVar4, cVar5, c_stk_ed, dist, fVar2, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, iVar14, iVar15, iVar6, iVar8, i_stk_108, native_arg_switch_1, p0, pCVar7, pcVar13, r1, r2, r3, r4, r5, r6, r7, r8, uVar10, uVar11, u_stk_c8, u_stk_f4, xStack_104, xStack_b4, xStack_c4, xStack_dc, xStack_ec, x_stk_18, x_stk_24, x_stk_30, x_stk_3c, x_stk_48, x_stk_54, x_stk_c
    local alive = true
    local function __cleanup_LAB_00d0e96f()
        quest:DeregisterTimer(i_stk_108)
        resources:ReleaseResource(xStack_104)
    end
    u_stk_f4 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        return
    end
    xStack_104 = resources:NewResource()
    resources:PrepareResource(xStack_104)
    bVar3 = resources:TryAcquire(xStack_104, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d0eb7c end
        bVar3 = resources:TryAcquire(xStack_104, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        quest:EntitySetAsKillable(me, false, true)
        iVar6 = quest:RegisterTimer()
        i_stk_108 = iVar6
        quest:SetTimer(i_stk_108, 0)
        c_stk_ed = 0
        u_stk_c8 = 0
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        uVar11 = 0
        repeat
            if bVar3 then
                quest:DeregisterTimer(i_stk_108)
                resources:ReleaseResource(xStack_104)
                return
            end
            if not quest:GetStateBool("Gate2Open") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    __cleanup_LAB_00d0e96f()
                    return
                end
                uVar10 = uVar11 | 1
                u_stk_f4 = uVar10
                bVar3 = me:MsgIsHitByHero()
                if bVar3 then
                    goto LAB_00d0daf6
                else
                    uVar10 = uVar11 | 3
                    u_stk_f4 = uVar10
                    bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar3 then
                        uVar10 = uVar11 | 7
                        u_stk_f4 = uVar10
                        bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar3 then goto LAB_00d0daf6 end
                    end
                    bVar3 = false
                end
                goto FLOW_past_lab_00d0daf6
                ::LAB_00d0daf6::
                bVar3 = true
                ::FLOW_past_lab_00d0daf6::
                if (uVar10 & 4) ~= 0 then
                    uVar10 = uVar10 & 0xfffffffb
                    u_stk_f4 = uVar10
                end
                if (uVar10 & 2) ~= 0 then
                    uVar10 = uVar10 & 0xfffffffd
                    u_stk_f4 = uVar10
                end
                if (uVar10 & 1) ~= 0 then
                    u_stk_f4 = uVar10 & 0xfffffffe
                end
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d0eb73 end
                    xStack_dc = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    native_arg_switch_1 = u_stk_c8
                    repeat
                        if native_arg_switch_1 == 0 then
                            x_stk_24 = resources:ScriptThing(xStack_104)
                            pCVar7 = x_stk_24
                            fret_0 = quest:GetHealth(pCVar7)
                            fVar2 = 0.0
                            if fret_0 <= fVar2 then
                                goto LAB_00d0dc78
                            end
                            goto FLOW_past_lab_00d0dc78
                            ::LAB_00d0dc78::
                            me:SetFriendsWithEverythingFlag(true)
                            u_stk_c8 = 1
                            break
                            ::FLOW_past_lab_00d0dc78::
                            iVar15 = 0
                            iVar14 = 1
                            iVar8 = 0
                            iVar6 = 0
                            pcVar13 = "TEXT_QST_009_BANDIT2_HIT_FIRST"
                            pCVar7 = quest:GetHero()
                            r1 = me:Speak(pCVar7, pcVar13, iVar6, (iVar8 ~= 0), (iVar14 ~= 0), (iVar15 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar5 = iVar6
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d0e8c4 end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar5 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then goto LAB_00d0dc78 end
                            -- LAB_00d0e8ef: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d0e8d0
                        else
                            if native_arg_switch_1 == 1 then
                                x_stk_c = resources:ScriptThing(xStack_104)
                                pCVar7 = x_stk_c
                                fret_00 = quest:GetHealth(pCVar7)
                                fVar2 = 0.0
                                if fVar2 < fret_00 then
                                    iVar15 = 0
                                    iVar14 = 1
                                    iVar8 = 0
                                    iVar6 = 0
                                    pcVar13 = "TEXT_QST_009_BANDIT2_HIT_SECOND"
                                    pCVar7 = quest:GetHero()
                                    r2 = me:Speak(pCVar7, pcVar13, iVar6, (iVar8 ~= 0), (iVar14 ~= 0), (iVar15 ~= 0))
                                    iVar6 = me:IsPerformingScriptTask()
                                    cVar5 = iVar6
                                    while cVar5 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d0e8c4 end
                                        iVar6 = me:IsPerformingScriptTask()
                                        cVar5 = iVar6
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d0e8c4 end
                                end
                                me:SetFriendsWithEverythingFlag(true)
                                break
                            else
                                if native_arg_switch_1 == 2 then
                                    x_stk_18 = resources:ScriptThing(xStack_104)
                                    pCVar7 = x_stk_18
                                    fret_01 = quest:GetHealth(pCVar7)
                                    fVar2 = 0.0
                                    if fVar2 < fret_01 then
                                        iVar15 = 0
                                        iVar14 = 1
                                        iVar8 = 0
                                        iVar6 = 0
                                        pcVar13 = "TEXT_QST_009_BANDIT2_HIT_THIRD"
                                        pCVar7 = quest:GetHero()
                                        r3 = me:Speak(pCVar7, pcVar13, iVar6, (iVar8 ~= 0), (iVar14 ~= 0), (iVar15 ~= 0))
                                        iVar6 = me:IsPerformingScriptTask()
                                        cVar5 = iVar6
                                        while cVar5 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00d0e8c4 end
                                            iVar6 = me:IsPerformingScriptTask()
                                            cVar5 = iVar6
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d0e8c4 end
                                    end
                                    me:SetFriendsWithEverythingFlag(true)
                                    break
                                else
                                    if native_arg_switch_1 == 3 then
                                        x_stk_30 = resources:ScriptThing(xStack_104)
                                        pCVar7 = x_stk_30
                                        fret_02 = quest:GetHealth(pCVar7)
                                        fVar2 = 0.0
                                        if fVar2 < fret_02 then
                                            iVar15 = 0
                                            iVar14 = 1
                                            iVar8 = 0
                                            iVar6 = 0
                                            pcVar13 = "TEXT_QST_009_BANDIT2_HIT_FOURTH"
                                            pCVar7 = quest:GetHero()
                                            r4 = me:Speak(pCVar7, pcVar13, iVar6, (iVar8 ~= 0), (iVar14 ~= 0), (iVar15 ~= 0))
                                            iVar6 = me:IsPerformingScriptTask()
                                            cVar5 = iVar6
                                            while cVar5 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if bVar3 then goto LAB_00d0e8c4 end
                                                iVar6 = me:IsPerformingScriptTask()
                                                cVar5 = iVar6
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then goto LAB_00d0e8c4 end
                                        end
                                        me:SetFriendsWithEverythingFlag(false)
                                        quest:ModifyThingHealth(me, 10000.0, false)
                                        quest:EntitySetAsKillable(me, true, true)
                                        pCVar7 = quest:GetHero()
                                        quest:GiveThingBestEnemyTarget(me, pCVar7)
                                        quest:ClearThingHasInformation(me)
                                    end
                                end
                            end
                        end
                    until not (false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_dc)
                end
                iVar6 = quest:GetTimer(i_stk_108)
                if iVar6 < 1 then
                    dist = 8.0
                    pCVar7 = quest:GetHero()
                    bVar3 = quest:IsDistanceBetweenThingsUnder(pCVar7, me, dist)
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d0eb73 end
                        iVar8 = quest:AddNewConversation(me, false, false)
                        pCVar7 = quest:GetHero()
                        quest:AddPersonToConversation(iVar8, pCVar7)
                        if c_stk_ed == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d0eb73 end
                            c_stk_ed = 1
                            pCVar7 = quest:GetHero()
                            quest:AddLineToConversation(iVar8, "TEXT_QST_009_BANDIT2_COMMENT_FIRST", me, pCVar7, false)
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d0eb73 end
                            pCVar7 = quest:GetHero()
                            quest:AddLineToConversation(iVar8, "TEXT_QST_009_BANDIT2_COMMENT_SECOND", me, pCVar7, false)
                        end
                        quest:SetTimer(i_stk_108, 10)
                    end
                end
                bVar3 = me:IsTalkedToByHero()
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d0eb73 end
                    quest:SetStateBool("SpokenToSecondGuard", true)
                    resources:PrepareResource(xStack_104)
                    bVar3 = resources:TryAcquire(xStack_104, me, 4)
                    while not bVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d0eb73 end
                        bVar3 = resources:TryAcquire(xStack_104, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d0eb73 end
                    pCVar7 = quest:GetHero()
                    bVar3 = quest:IsObjectInThingsPossession("OBJECT_RESIDENTIAL_BANDIT_CAMP_PASS", pCVar7)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar3 then
                        if bVar4 then goto LAB_00d0eb73 end
                        quest:GiveHeroExperience(quest:ReadGlobalGameData(0x40))
                        xStack_b4 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_3c = resources:ScriptThing(xStack_104)
                        pCVar7 = x_stk_3c
                        fret_04 = quest:GetHealth(pCVar7)
                        fVar2 = 0.0
                        if fVar2 < fret_04 then
                            iVar15 = 0
                            iVar14 = 1
                            iVar8 = 0
                            iVar6 = 0
                            pcVar13 = "TEXT_QST_009_BANDIT2_GIVEN_CAMP_PASS"
                            pCVar7 = quest:GetHero()
                            r5 = me:Speak(pCVar7, pcVar13, iVar6, (iVar8 ~= 0), (iVar14 ~= 0), (iVar15 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar5 = iVar6
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00d0e942
                                end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar5 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d0e942
                            end
                            goto FLOW_past_lab_00d0e942
                            ::LAB_00d0e942::
                            resources:DestroyMovie(xStack_b4)
                            goto LAB_00d0eb73
                            ::FLOW_past_lab_00d0e942::
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_b4)
                        quest:SetStateBool("Gate2Open", true)
                        quest:CreateThread("OpenGate", {args = {2.0, "Gate2Outer"}})  -- native parent-quest worker OpenGate, bound values
                        if (u_stk_f4 & 0x20) ~= 0 then
                            uVar11 = u_stk_f4 & 0xffffffdf
                        end
                        if (uVar11 & 0x10) ~= 0 then
                            uVar11 = uVar11 & 0xffffffef
                        end
                        if (uVar11 & 8) ~= 0 then
                            u_stk_f4 = uVar11 & 0xfffffff7
                        end
                        iVar8 = quest:AddNewConversation(me, false, false)
                        pCVar7 = quest:GetHero()
                        quest:AddPersonToConversation(iVar8, pCVar7)
                        pCVar7 = quest:GetHero()
                        quest:AddLineToConversation(iVar8, "TEXT_QST_009_BANDIT2_PASS", me, pCVar7, false)
                        quest:ClearThingHasInformation(me)
                    else
                        if bVar4 then goto LAB_00d0eb73 end
                        xStack_c4 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_48 = resources:ScriptThing(xStack_104)
                        pCVar7 = x_stk_48
                        fret_03 = quest:GetHealth(pCVar7)
                        fVar2 = 0.0
                        if fVar2 < fret_03 then
                            iVar15 = 0
                            iVar14 = 1
                            iVar8 = 0
                            iVar6 = 0
                            pcVar13 = "TEXT_QST_009_BANDIT2_GATE_BLOCKED"
                            pCVar7 = quest:GetHero()
                            r6 = me:Speak(pCVar7, pcVar13, iVar6, (iVar8 ~= 0), (iVar14 ~= 0), (iVar15 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar5 = iVar6
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00d0e909
                                end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar5 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d0e909
                            end
                            goto FLOW_past_lab_00d0e909
                            ::LAB_00d0e909::
                            resources:DestroyMovie(xStack_c4)
                            goto LAB_00d0eb73
                            ::FLOW_past_lab_00d0e909::
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_c4)
                    end
                    resources:PrepareResource(xStack_104)
                    bVar3 = resources:TryAcquire(xStack_104, me, 4)
                    while not bVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d0eb73 end
                        bVar3 = resources:TryAcquire(xStack_104, me, 4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d0eb73 end
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then __cleanup_LAB_00d0e96f(); return end
                bVar3 = me:IsTalkedToByHero()
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d0eb73 end
                    xStack_ec = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_54 = resources:ScriptThing(xStack_104)
                    pCVar7 = x_stk_54
                    fret_05 = quest:GetHealth(pCVar7)
                    fVar2 = 0.0
                    if fVar2 < fret_05 then
                        iVar15 = 0
                        iVar14 = 1
                        iVar8 = 0
                        iVar6 = 0
                        pcVar13 = "TEXT_QST_009_BANDIT2_ASIDE"
                        pCVar7 = quest:GetHero()
                        r7 = me:Speak(pCVar7, pcVar13, iVar6, (iVar8 ~= 0), (iVar14 ~= 0), (iVar15 ~= 0))
                        iVar6 = me:IsPerformingScriptTask()
                        cVar5 = iVar6
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d0e996 end
                            iVar6 = me:IsPerformingScriptTask()
                            cVar5 = iVar6
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d0e996 end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_ec)
                end
                uVar11 = u_stk_f4
                u_stk_f4 = u_stk_f4 | 0x40
                bVar3 = me:MsgIsHitByHero()
                if bVar3 then
                    goto LAB_00d0e82e
                else
                    uVar10 = uVar11 | 0xc0
                    u_stk_f4 = uVar10
                    bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar3 then
                        uVar10 = uVar11 | 0x1c0
                        u_stk_f4 = uVar10
                        bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar3 then goto LAB_00d0e82e end
                    end
                    bVar3 = false
                end
                goto FLOW_past_lab_00d0e82e
                ::LAB_00d0e82e::
                bVar3 = true
                ::FLOW_past_lab_00d0e82e::
                if (uVar10 & 0x100) ~= 0 then
                    uVar10 = uVar10 & 0xfffffeff
                    u_stk_f4 = uVar10
                end
                if uVar10 < 0 then
                    uVar10 = uVar10 & 0xffffff7f
                    u_stk_f4 = uVar10
                end
                if (uVar10 & 0x40) ~= 0 then
                    u_stk_f4 = uVar10 & 0xffffffbf
                end
                if bVar3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d0eb73 end
                    xStack_ec = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_54 = resources:ScriptThing(xStack_104)
                    pCVar7 = x_stk_54
                    fret_06 = quest:GetHealth(pCVar7)
                    fVar2 = 0.0
                    if fret_06 <= fVar2 then goto LAB_00d0eade end
                    iVar15 = 0
                    iVar14 = 1
                    iVar8 = 0
                    iVar6 = 0
                    pcVar13 = "TEXT_QST_009_BANDIT2_ATTACKED_NEW"
                    pCVar7 = quest:GetHero()
                    r8 = me:Speak(pCVar7, pcVar13, iVar6, (iVar8 ~= 0), (iVar14 ~= 0), (iVar15 ~= 0))
                    iVar6 = me:IsPerformingScriptTask()
                    cVar5 = iVar6
                    goto LAB_00d0ea82
                end
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            uVar11 = u_stk_f4
        until false
    end
    goto LAB_00d0eb7c
    ::LAB_00d0ea82::
    if not cVar5 then goto LAB_00d0eaa8 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then goto LAB_00d0e996 end
    iVar6 = me:IsPerformingScriptTask()
    cVar5 = iVar6
    goto LAB_00d0ea82
    ::LAB_00d0eaa8::
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        -- LAB_00d0eab3: (native jump target)
        quest:PauseAllNonScriptedEntities(false)
        goto LAB_00d0eabf
    end
    ::LAB_00d0eade::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_ec)
    me:SetFriendsWithEverythingFlag(false)
    quest:EntitySetAsKillable(me, true, true)
    pCVar7 = quest:GetHero()
    quest:GiveThingBestEnemyTarget(me, pCVar7)
    quest:ClearThingHasInformation(me)
    resources:PrepareResource(xStack_104)
    repeat
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    until not (not bVar3)
    goto LAB_00d0eb73
    ::LAB_00d0e8c4::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d0e8d0::
    resources:DestroyMovie(xStack_dc)
    goto LAB_00d0eb73
    ::LAB_00d0e996::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d0eabf::
    resources:DestroyMovie(xStack_ec)
    ::LAB_00d0eb73::
    quest:DeregisterTimer(i_stk_108)
    ::LAB_00d0eb7c::
    resources:ReleaseResource(xStack_104)
end

function Init(quest, me)
    __native_entity_state:SetStateInt("AIState", 0)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:SetIsPushableByHero(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

