-- Generated native draft: SickChild. Review coverage report before use.
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
    local CVar10, bVar4, cVar5, c_stk_81, dist, fStack_24, fVar12, f_stk_20, f_stk_8c, fret_0, fret_00, fret_01, iVar14, iVar15, iVar6, iVar8, i_stk_7c, p0, pCVar16, pCVar7, pCVar9, pcVar13, r1, r2, r3, r4, this_00, uVar11, uVar3, u_stk_88, xStack_34, xStack_4c, xStack_78, xStack_8c, x_stk_18
    local alive = true
    u_stk_88 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if not bVar4 then
        xStack_78 = resources:NewResource()
        resources:PrepareResource(xStack_78)
        bVar4 = resources:TryAcquire(xStack_78, me, 4)
        while not bVar4 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                resources:ReleaseResource(xStack_78)
                return
            end
            bVar4 = resources:TryAcquire(xStack_78, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            -- LAB_00ec5ea7: (native jump target)
            resources:ReleaseResource(xStack_78)
            return
        end
        iVar6 = quest:RegisterTimer()
        i_stk_7c = iVar6
        pCVar7 = quest:GetThingWithScriptName("SickChildBed")
        r1 = quest:GetSleepingPositionAndOrientationFromBed(me, pCVar7)
        fVar12 = math.atan(fStack_24,f_stk_20)
        f_stk_8c = (fVar12 * 0.15915493667125702)
        if (fVar12 * 0.15915493667125702 < 0.0) or (1.0 <= f_stk_8c) then
            f_stk_8c = fret_0
            if fret_0 < 0.0 then
                -- TODO(native): xStack_8c = (CCharString)(f_stk_8c + 1.0);
            end
        end
        quest:EntityTeleportToPosition(me, nil --[[missing]], false, false)
        xStack_8c = quest:ReadGlobalGameDataString(0x748)
        me:PlayLoopingAnimation(r1, -1, false, false, false)
        bVar4 = false
        pCVar7 = quest:GetThingWithScriptName("SickChildBed")
        quest:SetBedAvailability(pCVar7, bVar4)
        bVar4 = false
        pCVar7 = quest:GetThingWithScriptName("SickChildBed")
        quest:SetThingAsUsable(pCVar7, bVar4)
        quest:EntitySetTargetable(me, false)
        quest:EntitySetAsRespondToHit(me, false)
        quest:EntitySetAsKillable(me, false, false)
        quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
        cVar5 = quest:GetStateBool("FinishedQuest")
        while not cVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                quest:DeregisterTimer(i_stk_7c)
                resources:ReleaseResource(xStack_78)
                return
            end
            iVar8 = quest:GetTimer(i_stk_7c)
            if iVar8 == 0 then
                dist = 14.0
                pCVar7 = quest:GetHero()
                bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, dist)
                if not bVar4 then goto LAB_00ec6108 end
                goto LAB_00ec6137
            else
                goto LAB_00ec6108
            end
            goto FLOW_past_lab_00ec6137
            ::LAB_00ec6137::
            bVar4 = true
            ::FLOW_past_lab_00ec6137::
            goto FLOW_past_lab_00ec6108
            ::LAB_00ec6108::
            u_stk_88 = u_stk_88 | 1
            bVar4 = me:IsTalkedToByHero()
            if bVar4 then goto LAB_00ec6137 end
            bVar4 = false
            ::FLOW_past_lab_00ec6108::
            if (u_stk_88 & 1) ~= 0 then
                u_stk_88 = u_stk_88 & 0xfffffffe
            end
            if bVar4 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    quest:DeregisterTimer(i_stk_7c)
                    resources:ReleaseResource(xStack_78)
                    return
                end
                iVar6 = quest:AddNewConversation(me, false, false)
                pCVar7 = quest:GetHero()
                quest:AddPersonToConversation(iVar6, pCVar7)
                CVar10 = 0xa
                pCVar9 = tostring(0xa)
                pCVar9 = ("TEXT_QST_B10_BOY_JABBERS_" .. pCVar9)
                xStack_8c = pCVar9
                bVar4 = quest:TextEntryExists(xStack_8c)
                if not bVar4 then
                    CVar10 = 0xa
                    pCVar9 = tostring(10)
                    pCVar9 = ("TEXT_QST_B10_BOY_JABBERS_" .. pCVar9)
                    xStack_8c = pCVar9
                end
                -- TODO(native): xStack_80 = (CCharString)((int)CVar10 + 0xa);
                pCVar7 = quest:GetHero()
                quest:AddLineToConversation(iVar6, xStack_8c, me, pCVar7, false)
                iVar6 = i_stk_7c
                quest:SetTimer(i_stk_7c, 0x14)
            end
            cVar5 = quest:GetStateBool("FinishedQuest")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if not bVar4 then
            bVar4 = true
            pCVar7 = quest:GetThingWithScriptName("SickChildBed")
            quest:SetBedAvailability(pCVar7, bVar4)
            bVar4 = false
            pCVar7 = quest:GetThingWithScriptName("MK_SC_BOY")
            quest:EntityTeleportToThing(me, pCVar7, bVar4)
            me:ClearAllActionsIncludingLoopingAnimations()
            quest:EntitySetAsRespondToHit(me, true)
            quest:EntitySetTargetable(me, true)
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            while true do
                if bVar4 then
                    quest:DeregisterTimer(i_stk_7c)
                    resources:ReleaseResource(xStack_78)
                    return
                end
                bVar4 = me:IsTalkedToByHero()
                if bVar4 then break end
                -- LAB_00ec64df: (native jump target)
                uVar3 = u_stk_88
                u_stk_88 = u_stk_88 | 2
                bVar4 = me:MsgIsHitByHero()
                if bVar4 then
                    goto LAB_00ec6567
                else
                    uVar11 = uVar3 | 6
                    u_stk_88 = uVar11
                    bVar4 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar4 then
                        uVar11 = uVar3 | 0xe
                        u_stk_88 = uVar11
                        bVar4 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar4 then goto LAB_00ec6567 end
                    end
                    c_stk_81 = 0
                end
                goto FLOW_past_lab_00ec6567
                ::LAB_00ec6567::
                c_stk_81 = 1
                ::FLOW_past_lab_00ec6567::
                if (uVar11 & 8) ~= 0 then
                    uVar11 = uVar11 & 0xfffffff7
                    u_stk_88 = uVar11
                end
                if (uVar11 & 4) ~= 0 then
                    uVar11 = uVar11 & 0xfffffffb
                    u_stk_88 = uVar11
                end
                if (uVar11 & 2) ~= 0 then
                    u_stk_88 = uVar11 & 0xfffffffd
                end
                if c_stk_81 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00ec6786 end
                    xStack_4c = resources:StartMovie("")
                    pCVar16 = 0x1
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_18 = resources:ScriptThing(xStack_78)
                    pCVar7 = x_stk_18
                    fret_01 = quest:GetHealth(pCVar7)
                    fVar12 = 0.0
                    if fVar12 < fret_01 then
                        iVar15 = 0
                        iVar14 = 1
                        iVar6 = 0
                        iVar8 = 0
                        pcVar13 = "TEXT_QST_B10_MOTHER_HIT"
                        pCVar7 = quest:GetHero()
                        r2 = me:Speak(pCVar7, pcVar13, iVar8, (iVar6 ~= 0), (iVar14 ~= 0), (iVar15 ~= 0))
                        iVar8 = me:IsPerformingScriptTask()
                        cVar5 = iVar8
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00ec677d
                            end
                            iVar8 = me:IsPerformingScriptTask()
                            cVar5 = iVar8
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00ec677d
                        end
                        goto FLOW_past_lab_00ec677d
                        ::LAB_00ec677d::
                        this_00 = xStack_4c
                        goto LAB_00ec6781
                        ::FLOW_past_lab_00ec677d::
                    end
                    pCVar7 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, pCVar7)
                    pCVar16 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(pCVar16, me)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_4c)
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
            end
            ::FLOW_after_lab_00ec64df::
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                xStack_34 = resources:StartMovie("")
                pCVar16 = 0x1
                quest:PauseAllNonScriptedEntities(true)
                pCVar7 = resources:ScriptThing(xStack_78)
                pCVar7 = pCVar7
                fret_00 = quest:GetHealth(pCVar7)
                fVar12 = 0.0
                if fret_00 <= fVar12 then
                    goto LAB_00ec64c5
                end
                goto FLOW_past_lab_00ec64c5
                ::LAB_00ec64c5::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_34)
                uVar3 = u_stk_88
                u_stk_88 = u_stk_88 | 2
                bVar4 = me:MsgIsHitByHero()
                if bVar4 then
                    goto LAB_00ec6567_c2
                end
                goto FLOW_past_lab_00ec6567_c2
                ::LAB_00ec6567_c2::
                c_stk_81 = 1
                ::FLOW_past_lab_00ec6567_c2::
                if (uVar11 & 8) ~= 0 then
                    uVar11 = uVar11 & 0xfffffff7
                    u_stk_88 = uVar11
                end
                if (uVar11 & 4) ~= 0 then
                    uVar11 = uVar11 & 0xfffffffb
                    u_stk_88 = uVar11
                end
                if (uVar11 & 2) ~= 0 then
                    u_stk_88 = uVar11 & 0xfffffffd
                end
                if c_stk_81 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00ec6786 end
                    xStack_4c = resources:StartMovie("")
                    pCVar16 = 0x1
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_18 = resources:ScriptThing(xStack_78)
                    pCVar7 = x_stk_18
                    fret_01 = quest:GetHealth(pCVar7)
                    fVar12 = 0.0
                    if fVar12 < fret_01 then
                        iVar15 = 0
                        iVar14 = 1
                        iVar6 = 0
                        iVar8 = 0
                        pcVar13 = "TEXT_QST_B10_MOTHER_HIT"
                        pCVar7 = quest:GetHero()
                        r3 = me:Speak(pCVar7, pcVar13, iVar8, (iVar6 ~= 0), (iVar14 ~= 0), (iVar15 ~= 0))
                        iVar8 = me:IsPerformingScriptTask()
                        cVar5 = iVar8
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00ec677d_c2
                            end
                            iVar8 = me:IsPerformingScriptTask()
                            cVar5 = iVar8
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00ec677d_c2
                        end
                        goto FLOW_past_lab_00ec677d_c2
                        ::LAB_00ec677d_c2::
                        this_00 = xStack_4c
                        goto LAB_00ec6781
                        ::FLOW_past_lab_00ec677d_c2::
                    end
                    pCVar7 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, pCVar7)
                    pCVar16 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(pCVar16, me)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_4c)
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                goto FLOW_after_lab_00ec64df
                ::FLOW_past_lab_00ec64c5::
                iVar15 = 0
                iVar14 = 1
                iVar6 = 0
                iVar8 = 0
                pcVar13 = "TEXT_QST_B10_BOY_CURED_10"
                pCVar7 = quest:GetHero()
                r4 = me:Speak(pCVar7, pcVar13, iVar8, (iVar6 ~= 0), (iVar14 ~= 0), (iVar15 ~= 0))
                iVar8 = me:IsPerformingScriptTask()
                cVar5 = iVar8
                while cVar5 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = xStack_34
                        goto LAB_00ec6781
                    end
                    iVar8 = me:IsPerformingScriptTask()
                    cVar5 = iVar8
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then goto LAB_00ec64c5 end
                quest:PauseAllNonScriptedEntities(false)
                this_00 = xStack_34
                goto LAB_00ec6781
            end
            goto FLOW_past_lab_00ec6781
            ::LAB_00ec6781::
            resources:DestroyMovie(this_00)
            ::FLOW_past_lab_00ec6781::
        end
        ::LAB_00ec6786::
        quest:DeregisterTimer(i_stk_7c)
        resources:ReleaseResource(xStack_78)
    end
end

function Init(quest, me)
    quest:SetIsPushableByHero(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

