-- Generated native draft: ManWithDoorName. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, bVar5, cVar6, center, distance, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, iVar11, iVar12, iVar13, iVar14, p0, p0_00, pCVar7, pCVar8, pcVar10, r1, r2, r3, r4, r5, r6, r7, this_00, uVar4, u_stk_ec, xStack_58, xStack_68, xStack_78, xStack_88, xStack_98, xStack_cc, xStack_e8, xStack_fc, x_stk_18, x_stk_24, x_stk_30, x_stk_3c, x_stk_48, x_stk_c
    local alive = true
    u_stk_ec = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    xStack_fc = resources:NewResource()
    quest:SetThingHasInformation(me, false, true, false)
    pCVar7 = me:GetPos()
    center = pCVar7.x
    quest:SetWanderCentrePoint(me, pCVar7)
    quest:SetWanderMinDistance(me, 0.0)
    distance = quest:ReadGlobalGameDataFloat(0x778)
    quest:SetWanderMaxDistance(me, distance)
    quest:SetScriptingStateGroup(me, 4)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    repeat
        if bVar5 then
            resources:ReleaseResource(xStack_fc)
            return
        end
        uVar4 = u_stk_ec
        u_stk_ec = u_stk_ec | 1
        bVar5 = me:MsgIsHitByHero()
        if bVar5 then
            goto LAB_00ed19df
        else
            u_stk_ec = uVar4 | 3
            bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if bVar5 then
                u_stk_ec = uVar4 | 7
                bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                if not bVar5 then goto LAB_00ed19df end
            end
            bVar5 = false
        end
        goto FLOW_past_lab_00ed19df
        ::LAB_00ed19df::
        bVar5 = true
        ::FLOW_past_lab_00ed19df::
        if (u_stk_ec & 4) ~= 0 then
            u_stk_ec = u_stk_ec & 0xfffffffb
        end
        if (u_stk_ec & 2) ~= 0 then
            u_stk_ec = u_stk_ec & 0xfffffffd
        end
        if (u_stk_ec & 1) ~= 0 then
            u_stk_ec = u_stk_ec & 0xfffffffe
        end
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00ed286a end
            fret_0 = quest:GetHealth(me)
            if 9.999999747378752e-05 < fret_0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00ed286a end
                resources:PrepareResource(xStack_fc)
                bVar5 = resources:TryAcquire(xStack_fc, me, 4)
                while not bVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00ed286a end
                    bVar5 = resources:TryAcquire(xStack_fc, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00ed286a end
                xStack_98 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                x_stk_24 = resources:ScriptThing(xStack_fc)
                pCVar8 = x_stk_24
                fret_00 = quest:GetHealth(pCVar8)
                fVar3 = 0.0
                if fVar3 < fret_00 then
                    iVar14 = 0
                    iVar13 = 1
                    iVar12 = 0
                    iVar11 = 0
                    pcVar10 = "TEXT_QST_060_MAN_WITH_DOOR_NAME_HIT"
                    pCVar8 = quest:GetHero()
                    r1 = me:Speak(pCVar8, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                    iVar11 = me:IsPerformingScriptTask()
                    cVar6 = iVar11
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_98)
                            goto LAB_00ed286a
                        end
                        iVar11 = me:IsPerformingScriptTask()
                        cVar6 = iVar11
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_98)
                        goto LAB_00ed286a
                    end
                end
                resources:PrepareResource(xStack_fc)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_98)
            end
            quest:SetStateBool("DoorManAttackedByHero", true)
        end
        if (quest:GetStateBool("DoorManHasBribe")) and (not quest:GetStateBool("DoorManComplete")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00ed286a end
            xStack_cc = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:PrepareResource(xStack_fc)
            bVar5 = resources:TryAcquire(xStack_fc, me, 4)
            while not bVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_cc)
                    goto LAB_00ed286a
                end
                bVar5 = resources:TryAcquire(xStack_fc, me, 4)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                goto LAB_00ed276d
            end
            goto FLOW_past_lab_00ed276d
            ::LAB_00ed276d::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_cc)
            goto LAB_00ed286a
            ::FLOW_past_lab_00ed276d::
            x_stk_48 = resources:ScriptThing(xStack_fc)
            pCVar8 = x_stk_48
            fret_01 = quest:GetHealth(pCVar8)
            fVar3 = 0.0
            if fVar3 < fret_01 then
                iVar14 = 0
                iVar13 = 1
                iVar12 = 0
                iVar11 = 0
                pcVar10 = "TEXT_QST_060_MAN_WITH_DOOR_NAME_SPEAKS"
                pCVar8 = quest:GetHero()
                r2 = me:Speak(pCVar8, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                iVar11 = me:IsPerformingScriptTask()
                cVar6 = iVar11
                while cVar6 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_cc)
                        goto LAB_00ed286a
                    end
                    iVar11 = me:IsPerformingScriptTask()
                    cVar6 = iVar11
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00ed276d end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(xStack_cc)
            quest:SetStateBool("DoorManComplete", true)
            r3 = quest:GetNearestWithDefName(me, "REGION_EXIT_POINT")
            iVar11 = (r3 ~= nil and r3:IsAlive())
            if iVar11 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    bVar5 = (not resources:ScriptThing(xStack_fc):IsNull())
                    if bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00ed2861 end
                        if not (r3 ~= nil and not r3:IsNull()) then
                            p0_00 = {x = 0, y = 0, z = 0}
                        else
                            p0_00 = r3:GetPos()
                        end
                        me:MoveToPosition(p0_00, 1.0, 1, false, true)
                    end
                    bVar5 = quest:IsDistanceBetweenThingsUnder(me, r3, 2.0)
                    while true do
                        __native_condition_1 = not bVar5
                        if __native_condition_1 then
                            bVar5 = quest:MsgOnRegionLoaded()
                            __native_condition_1 = not bVar5
                        end
                        if not __native_condition_1 then break end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00ed2861 end
                        bVar5 = quest:IsDistanceBetweenThingsUnder(me, r3, 2.0)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        quest:FadeOutAndKillEntity(me, true, 1.0, true)
                        goto LAB_00ed1ecb
                    end
                end
                -- LAB_00ed2785: (native jump target)
                goto LAB_00ed286a
            end
            ::LAB_00ed1ecb::
        end
        bVar5 = me:IsTalkedToByHero()
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00ed286a end
            if not quest:GetStateBool("DoorManIntroComplete") then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00ed286a end
                xStack_68 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(xStack_fc)
                bVar5 = resources:TryAcquire(xStack_fc, me, 4)
                while not bVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00ed2793 end
                    bVar5 = resources:TryAcquire(xStack_fc, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    -- LAB_00ed27b0: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_68)
                    goto LAB_00ed286a
                end
                x_stk_c = resources:ScriptThing(xStack_fc)
                pCVar8 = x_stk_c
                fret_02 = quest:GetHealth(pCVar8)
                fVar3 = 0.0
                if fVar3 < fret_02 then
                    iVar14 = 0
                    iVar13 = 1
                    iVar12 = 0
                    iVar11 = 0
                    pcVar10 = "TEXT_QST_060_MAN_WITH_DOOR_NAME_INTRO"
                    pCVar8 = quest:GetHero()
                    r4 = me:Speak(pCVar8, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                    iVar11 = me:IsPerformingScriptTask()
                    cVar6 = iVar11
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00ed2793 end
                        iVar11 = me:IsPerformingScriptTask()
                        cVar6 = iVar11
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00ed2793 end
                end
                resources:PrepareResource(xStack_fc)
                quest:SetStateBool("DoorManIntroComplete", true)
                quest:PauseAllNonScriptedEntities(false)
                this_00 = xStack_68
            elseif not quest:GetStateBool("DoorManAttackedByHero") then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00ed286a end
                xStack_88 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(xStack_fc)
                bVar5 = resources:TryAcquire(xStack_fc, me, 4)
                while not bVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00ed27cd end
                    bVar5 = resources:TryAcquire(xStack_fc, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    -- LAB_00ed27ea: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_88)
                    goto LAB_00ed286a
                end
                x_stk_30 = resources:ScriptThing(xStack_fc)
                pCVar8 = x_stk_30
                fret_03 = quest:GetHealth(pCVar8)
                fVar3 = 0.0
                if fVar3 < fret_03 then
                    iVar14 = 0
                    iVar13 = 1
                    iVar12 = 0
                    iVar11 = 2
                    pcVar10 = "TEXT_QST_060_MAN_WITH_DOOR_NAME_REMINDER"
                    pCVar8 = quest:GetHero()
                    r5 = me:Speak(pCVar8, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                    iVar11 = me:IsPerformingScriptTask()
                    cVar6 = iVar11
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00ed27cd end
                        iVar11 = me:IsPerformingScriptTask()
                        cVar6 = iVar11
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00ed27cd end
                end
                resources:PrepareResource(xStack_fc)
                quest:SetStateBool("DoorManIntroComplete", true)
                quest:PauseAllNonScriptedEntities(false)
                this_00 = xStack_88
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00ed286a end
                xStack_78 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(xStack_fc)
                bVar5 = resources:TryAcquire(xStack_fc, me, 4)
                while not bVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00ed2807 end
                    bVar5 = resources:TryAcquire(xStack_fc, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    -- LAB_00ed2821: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_78)
                    goto LAB_00ed286a
                end
                x_stk_18 = resources:ScriptThing(xStack_fc)
                pCVar8 = x_stk_18
                fret_04 = quest:GetHealth(pCVar8)
                fVar3 = 0.0
                if fVar3 < fret_04 then
                    iVar14 = 0
                    iVar13 = 1
                    iVar12 = 0
                    iVar11 = 2
                    pcVar10 = "TEXT_QST_060_MAN_WITH_DOOR_NAME_ATTACKED"
                    pCVar8 = quest:GetHero()
                    r6 = me:Speak(pCVar8, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                    iVar11 = me:IsPerformingScriptTask()
                    cVar6 = iVar11
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00ed2807 end
                        iVar11 = me:IsPerformingScriptTask()
                        cVar6 = iVar11
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00ed2807 end
                end
                resources:PrepareResource(xStack_fc)
                quest:PauseAllNonScriptedEntities(false)
                this_00 = xStack_78
            end
            resources:DestroyMovie(this_00)
        end
        __native_condition_2 = not quest:GetStateBool("DoorManHasBribe")
        if __native_condition_2 then
            xStack_e8 = ""
            bVar5 = me:MsgIsPresentedWithItem()
            if bVar5 then xStack_e8 = _G.g_PresentedItemName end
            __native_condition_2 = bVar5
        end
        if __native_condition_2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar5 then
                if xStack_e8 == nil then
                    bVar5 = false
                    if bVar5 then
                        goto LAB_00ed26a8
                    end
                else
                    iVar11 = ((xStack_e8 == "OBJECT_GEMSTONE_RUBY") and 0 or 1)
                    if iVar11 == 0 then goto LAB_00ed26a8 end
                end
                goto FLOW_past_lab_00ed26a8
                ::LAB_00ed26a8::
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    quest:AddItemToContainer(me, "OBJECT_GEMSTONE_RUBY")
                    quest:SetStateBool("DoorManHasBribe", true)
                    goto LAB_00ed2618
                end
                goto LAB_00ed2861
                ::FLOW_past_lab_00ed26a8::
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00ed2861 end
                xStack_58 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(xStack_fc)
                bVar5 = resources:TryAcquire(xStack_fc, me, 4)
                while not bVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00ed283b end
                    bVar5 = resources:TryAcquire(xStack_fc, me, 4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    x_stk_3c = resources:ScriptThing(xStack_fc)
                    pCVar8 = x_stk_3c
                    fret_05 = quest:GetHealth(pCVar8)
                    fVar3 = 0.0
                    if fVar3 < fret_05 then
                        iVar14 = 0
                        iVar13 = 1
                        iVar12 = 0
                        iVar11 = 2
                        pcVar10 = "TEXT_QST_060_MAN_WITH_DOOR_WRONG_ITEM"
                        pCVar8 = quest:GetHero()
                        r7 = me:Speak(pCVar8, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                        iVar11 = me:IsPerformingScriptTask()
                        cVar6 = iVar11
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00ed283b end
                            iVar11 = me:IsPerformingScriptTask()
                            cVar6 = iVar11
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00ed283b end
                    end
                    resources:PrepareResource(xStack_fc)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_58)
                    goto LAB_00ed2618
                end
                -- LAB_00ed2849: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                goto LAB_00ed2855
            end
            goto LAB_00ed2861
        end
        ::LAB_00ed2618::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
    until false
    ::LAB_00ed2807::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_78)
    goto LAB_00ed286a
    ::LAB_00ed27cd::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_88)
    goto LAB_00ed286a
    ::LAB_00ed2793::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_68)
    goto LAB_00ed286a
    ::LAB_00ed283b::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00ed2855::
    resources:DestroyMovie(xStack_58)
    ::LAB_00ed2861::
    ::LAB_00ed286a::
    resources:ReleaseResource(xStack_fc)
end

function Init(quest, me)
    quest:SetThingPersistent(me, true)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

