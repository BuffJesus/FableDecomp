-- Generated native draft: SickChildsSister. Review coverage report before use.
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
    local bVar12, bVar14, bVar3, bVar5, bVar6, cVar4, c_stk_a5, fVar15, fVar2, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, iVar17, iVar18, iVar19, iVar20, native_arg_sequence_1, p0, p1, p5, pCVar10, pCVar11, pCVar7, pcVar16, puVar9, r1, r2, r3, r4, r5, r6, r7, r8, r9, uVar8, value, xStack_10, xStack_20, xStack_54, xStack_64, xStack_68, xStack_78, xStack_a0, xStack_b8, xStack_bc, xStack_c0, x_stk_2c, x_stk_44
    local alive = true
    local function __region_LAB_00ec781d()
        pCVar11 = xStack_64
        resources:DestroyMovie(pCVar11)
    end
    local function __cleanup_LAB_00ec782a()
        quest:DeregisterTimer(xStack_bc)
        quest:DeregisterTimer(xStack_c0)
        resources:ReleaseResource(xStack_b8)
    end
    local function __cleanup_LAB_00ec7838()
        quest:DeregisterTimer(xStack_c0)
        resources:ReleaseResource(xStack_b8)
    end
    bVar6 = 0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        xStack_b8 = resources:NewResource()
        resources:PrepareResource(xStack_b8)
        bVar3 = resources:TryAcquire(xStack_b8, me, 4)
        while not bVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:ReleaseResource(xStack_b8)
                return
            end
            bVar3 = resources:TryAcquire(xStack_b8, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00ec6973: (native jump target)
            resources:ReleaseResource(xStack_b8)
            return
        end
        if (not __native_entity_state:GetStateBool("LeadHeroToSickChild")) and (not quest:GetStateBool("MotherIntroDone")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                iVar19 = 1
                iVar17 = 1.0
                pCVar7 = quest:GetHero()
                me:FollowThing(pCVar7, iVar17, (iVar19 ~= 0))
                xStack_c0 = quest:RegisterTimer()
                ::LAB_00ec69d4::
                bVar3 = me:IsTalkedToByHero()
                native_arg_sequence_1 = false
                if bVar3 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if not native_arg_sequence_1 then
                    bVar3 = true
                    if quest:GetStateBool("MotherIntroDone") then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
                if native_arg_sequence_1 then
                    bVar3 = false
                end
                if bVar3 then
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then __cleanup_LAB_00ec7838(); return end
                    iVar17 = quest:GetTimer(xStack_c0)
                    if iVar17 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00ec7838(); return end
                        pCVar7 = quest:GetHero()
                        me:StopFollowingThing(pCVar7)
                        iVar19 = quest:AddNewConversation(me, false, false)
                        pCVar7 = quest:GetHero()
                        quest:AddPersonToConversation(iVar19, pCVar7)
                        pCVar7 = quest:GetHero()
                        quest:AddLineToConversation(iVar19, "TEXT_QST_B10_SISTER_CALLS_OVER_10", me, pCVar7, false)
                        iVar17 = quest:ReadGlobalGameData(0x750)
                        iVar19 = quest:ReadGlobalGameData(0x74c)
                        uVar8 = math.random(0, 32767)
                        -- TODO(native): CDefString::operator_class_CCharString((CDefString *)(quest:ReadGlobalGameData(0x74c) + (uVar8 % (uint)(iVar17 - iVar19 >> 2)) * 4),(int)&xStack_94);
                        me:PlayAnimation(nil --[[missing]], false, false, false, true, true, false, false)
                        iVar17 = me:IsPerformingScriptTask()
                        cVar4 = iVar17
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:DeregisterTimer(xStack_c0)
                                resources:ReleaseResource(xStack_b8)
                                return
                            end
                            iVar17 = me:IsPerformingScriptTask()
                            cVar4 = iVar17
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00ec7838(); return end
                        quest:SetTimer(xStack_c0, 10)
                        iVar19 = 1
                        iVar17 = 1.0
                        pCVar7 = quest:GetHero()
                        me:FollowThing(pCVar7, iVar17, (iVar19 ~= 0))
                    end
                    bVar12 = bVar6 | 2
                    bVar3 = me:MsgIsHitByHero()
                    if bVar3 then
                        goto LAB_00ec6c50
                    else
                        bVar12 = bVar6 | 6
                        bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar3 then
                            bVar12 = 0xe
                            bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar3 then goto LAB_00ec6c50 end
                        end
                        bVar3 = false
                    end
                    goto FLOW_past_lab_00ec6c50
                    ::LAB_00ec6c50::
                    bVar3 = true
                    ::FLOW_past_lab_00ec6c50::
                    if (bVar12 & 8) ~= 0 then
                        bVar12 = bVar12 & 0xf7
                    end
                    if (bVar12 & 4) ~= 0 then
                        bVar12 = bVar12 & 0xfb
                    end
                    bVar6 = bVar12
                    if (bVar12 & 2) ~= 0 then
                        bVar6 = bVar12 & 0xfd
                    end
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00ec7838(); return end
                        pCVar7 = quest:GetHero()
                        me:StopFollowingThing(pCVar7)
                        xStack_78 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        xStack_a0 = resources:ScriptThing(xStack_b8)
                        pCVar7 = xStack_a0
                        fret_0 = quest:GetHealth(pCVar7)
                        fVar2 = 0.0
                        if fVar2 < fret_0 then
                            iVar20 = 0
                            iVar18 = 1
                            iVar19 = 0
                            iVar17 = 2
                            pcVar16 = "TEXT_QST_B10_SISTER_ON_HIT"
                            pCVar7 = quest:GetHero()
                            r1 = me:Speak(pCVar7, pcVar16, iVar17, (iVar19 ~= 0), (iVar18 ~= 0), (iVar20 ~= 0))
                            iVar17 = me:IsPerformingScriptTask()
                            cVar4 = iVar17
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_78)
                                    quest:DeregisterTimer(xStack_c0)
                                    resources:ReleaseResource(xStack_b8)
                                    return
                                end
                                iVar17 = me:IsPerformingScriptTask()
                                cVar4 = iVar17
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_78)
                                quest:DeregisterTimer(xStack_c0)
                                resources:ReleaseResource(xStack_b8)
                                return
                            end
                        end
                        iVar19 = 1
                        iVar17 = 1.0
                        pCVar7 = quest:GetHero()
                        me:FollowThing(pCVar7, iVar17, (iVar19 ~= 0))
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_78)
                    end
                    goto LAB_00ec69d4
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then __cleanup_LAB_00ec7838(); return end
                pCVar7 = quest:GetHero()
                me:StopFollowingThing(pCVar7)
                if quest:GetStateBool("MotherIntroDone") then goto LAB_00ec734b end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then __cleanup_LAB_00ec7838(); return end
                quest:FixMovieSequenceCamera(true)
                quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0, -1)
                xStack_78 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                x_stk_44 = resources:ScriptThing(xStack_b8)
                pCVar7 = x_stk_44
                fret_00 = quest:GetHealth(pCVar7)
                fVar2 = 0.0
                if fret_00 <= fVar2 then goto LAB_00ec700a end
                iVar20 = 0
                iVar18 = 1
                iVar19 = 0
                iVar17 = 0
                pcVar16 = "TEXT_QST_B10_SISTER_TOLD_TO_FOLLOW_20"
                pCVar7 = quest:GetHero()
                r2 = me:Speak(pCVar7, pcVar16, iVar17, (iVar19 ~= 0), (iVar18 ~= 0), (iVar20 ~= 0))
                iVar17 = me:IsPerformingScriptTask()
                cVar4 = iVar17
                while cVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        -- LAB_00ec6e32: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_78)
                        quest:DeregisterTimer(xStack_c0)
                        resources:ReleaseResource(xStack_b8)
                        return
                    end
                    iVar17 = me:IsPerformingScriptTask()
                    cVar4 = iVar17
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    -- LAB_00ec6e64: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_78)
                    quest:DeregisterTimer(xStack_c0)
                    resources:ReleaseResource(xStack_b8)
                    return
                end
                ::LAB_00ec700a::
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_78)
                r3 = quest:GetThingWithScriptName("SickChildsMother")
                quest:SetThingHasInformation(me, false, false, false)
                c_stk_a5 = 0
                iVar17 = 0
                xStack_bc = quest:RegisterTimer()
                me:ClearCommands()
                if not (r3 ~= nil and not r3:IsNull()) then
                    puVar9 = {x = 0, y = 0, z = 0}
                else
                    puVar9 = r3:GetPos()
                end
                me:MoveToPosition(puVar9, 1.0, 1, false, true)
                bVar3 = quest:IsDistanceBetweenThingsUnder(me, r3, 2.0)
                if bVar3 then
                    goto LAB_00ec7326
                end
                goto FLOW_past_lab_00ec7326
                ::LAB_00ec7326::
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then __cleanup_LAB_00ec782a(); return end
                quest:DeregisterTimer(xStack_bc)
                __native_entity_state:SetStateBool("LeadHeroToSickChild", true)
                goto LAB_00ec734b
                ::FLOW_past_lab_00ec7326::
                goto FLOW_past_lab_00ec734b
                ::LAB_00ec734b::
                quest:DeregisterTimer(xStack_c0)
                goto LAB_00ec7354
                ::FLOW_past_lab_00ec734b::
                repeat
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then __cleanup_LAB_00ec782a(); return end
                    xStack_68 = (quest:GetDistanceBetweenThings(me, r3) ^ 2)
                    pCVar7 = quest:GetHero()
                    fVar15 = (quest:GetDistanceBetweenThings(pCVar7, r3) ^ 2)
                    bVar14 = fVar15 <= xStack_68
                    fVar15 = 15.0
                    pCVar7 = quest:GetHero()
                    bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, fVar15)
                    if (bVar3) or (bVar14) then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            fVar15 = 11.0
                            pCVar7 = quest:GetHero()
                            bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, fVar15)
                            if (bVar3) or (bVar14) then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if not bVar3 then
                                    if iVar17 ~= 0 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then __cleanup_LAB_00ec782a(); return end
                                        me:ClearCommands()
                                        if not (r3 ~= nil and not r3:IsNull()) then
                                            puVar9 = {x = 0, y = 0, z = 0}
                                        else
                                            puVar9 = r3:GetPos()
                                        end
                                        me:MoveToPosition(puVar9, 1.0, 1, false, true)
                                        iVar17 = 0
                                    end
                                    goto LAB_00ec7467
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if not bVar3 then
                                    if iVar17 ~= 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then __cleanup_LAB_00ec782a(); return end
                                        me:ClearCommands()
                                        if not (r3 ~= nil and not r3:IsNull()) then
                                            puVar9 = {x = 0, y = 0, z = 0}
                                        else
                                            puVar9 = r3:GetPos()
                                        end
                                        me:MoveToPosition(puVar9, 1.0, 0, false, true)
                                        iVar17 = 1
                                    end
                                    goto LAB_00ec7467
                                end
                            end
                            goto FLOW_past_lab_00ec7467
                            ::LAB_00ec7467::
                            c_stk_a5 = 0
                            bVar3 = me:IsTalkedToByHero()
                            if bVar3 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then __cleanup_LAB_00ec782a(); return end
                                me:ClearCommands()
                                xStack_78 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                x_stk_44 = resources:ScriptThing(xStack_b8)
                                pCVar7 = x_stk_44
                                fret_01 = quest:GetHealth(pCVar7)
                                fVar2 = 0.0
                                if fret_01 <= fVar2 then
                                    goto LAB_00ec75b3
                                end
                                goto FLOW_past_lab_00ec75b3
                                ::LAB_00ec75b3::
                                iVar17 = 2
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_78)
                                goto LAB_00ec75cd
                                ::FLOW_past_lab_00ec75b3::
                                iVar20 = 0
                                iVar18 = 1
                                iVar19 = 0
                                iVar17 = 0
                                pcVar16 = "TEXT_QST_B10_SISTER_THIS_WAY"
                                pCVar7 = quest:GetHero()
                                r4 = me:Speak(pCVar7, pcVar16, iVar17, (iVar19 ~= 0), (iVar18 ~= 0), (iVar20 ~= 0))
                                iVar17 = me:IsPerformingScriptTask()
                                cVar4 = iVar17
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_78
                                        goto LAB_00ec7821
                                    end
                                    iVar17 = me:IsPerformingScriptTask()
                                    cVar4 = iVar17
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if not bVar3 then goto LAB_00ec75b3 end
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar11 = xStack_78
                                ::LAB_00ec7821::
                                resources:DestroyMovie(pCVar11)
                            else
                                goto LAB_00ec75cd
                            end
                            ::FLOW_after_lab_00ec7821::
                            goto FLOW_past_lab_00ec75cd
                            ::LAB_00ec75cd::
                            bVar3 = me:MsgIsHitByHero()
                            bVar12 = bVar6 | 0x10
                            if bVar3 then
                                goto LAB_00ec765b
                            else
                                bVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                                bVar12 = bVar6 | 0x30
                                if bVar3 then
                                    bVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                    bVar12 = bVar6 | 0x70
                                    if not bVar3 then goto LAB_00ec765b end
                                end
                                bVar6 = bVar12
                                bVar3 = false
                            end
                            goto FLOW_past_lab_00ec765b
                            ::LAB_00ec765b::
                            bVar6 = bVar12
                            bVar3 = true
                            ::FLOW_past_lab_00ec765b::
                            if (bVar6 & 0x40) ~= 0 then
                                bVar6 = bVar6 & 0xbf
                            end
                            if (bVar6 & 0x20) ~= 0 then
                                bVar6 = bVar6 & 0xdf
                            end
                            if (bVar6 & 0x10) ~= 0 then
                                bVar6 = bVar6 & 0xef
                            end
                            if not bVar3 then goto LAB_00ec730e end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                me:ClearCommands()
                                xStack_64 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                x_stk_2c = resources:ScriptThing(xStack_b8)
                                pCVar7 = x_stk_2c
                                fret_02 = quest:GetHealth(pCVar7)
                                fVar2 = 0.0
                                if fVar2 < fret_02 then
                                    iVar20 = 0
                                    iVar18 = 1
                                    iVar19 = 0
                                    iVar17 = 2
                                    pcVar16 = "TEXT_QST_B10_SISTER_ON_HIT"
                                    pCVar7 = quest:GetHero()
                                    r5 = me:Speak(pCVar7, pcVar16, iVar17, (iVar19 ~= 0), (iVar18 ~= 0), (iVar20 ~= 0))
                                    iVar17 = me:IsPerformingScriptTask()
                                    cVar4 = iVar17
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            __region_LAB_00ec781d(); return  -- TODO(native): goto FLOW_after_lab_00ec7821
                                        end
                                        iVar17 = me:IsPerformingScriptTask()
                                        cVar4 = iVar17
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        __region_LAB_00ec781d()
                                        goto FLOW_after_lab_00ec7821
                                    end
                                end
                                iVar17 = 2
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_64)
                                goto LAB_00ec730e
                            end
                            ::FLOW_past_lab_00ec75cd::
                            ::FLOW_past_lab_00ec7467::
                        end
                        __cleanup_LAB_00ec782a()
                        return
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then __cleanup_LAB_00ec782a(); return end
                    me:ClearCommands()
                    me:ClearAllActions()
                    if c_stk_a5 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            bVar3 = false
                            pCVar7 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(me, pCVar7, bVar3)
                            iVar19 = quest:AddNewConversation(me, false, false)
                            pCVar7 = quest:GetHero()
                            quest:AddPersonToConversation(iVar19, pCVar7)
                            pCVar7 = quest:GetHero()
                            quest:AddLineToConversation(iVar19, "TEXT_QST_B10_SISTER_THIS_WAY", me, pCVar7, false)
                            iVar17 = me:IsPerformingScriptTask()
                            cVar4 = iVar17
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then __cleanup_LAB_00ec782a(); return end
                                iVar17 = me:IsPerformingScriptTask()
                                cVar4 = iVar17
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                quest:Pause(0.8)
                                me:PlayAnimation("STANDARD_WAVE", false, false, false, true, true, false, false)
                                iVar17 = me:IsPerformingScriptTask()
                                cVar4 = iVar17
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then __cleanup_LAB_00ec782a(); return end
                                    iVar17 = me:IsPerformingScriptTask()
                                    cVar4 = iVar17
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if not bVar3 then
                                    c_stk_a5 = 1
                                    iVar17 = 2
                                    quest:SetTimer(xStack_bc, 5)
                                    goto LAB_00ec72e4
                                end
                            end
                        end
                        __cleanup_LAB_00ec782a(); return
                    end
                    ::LAB_00ec72e4::
                    iVar19 = quest:GetTimer(xStack_bc)
                    if iVar19 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00ec782a(); return end
                        c_stk_a5 = 0
                    end
                    ::LAB_00ec730e::
                    bVar3 = quest:IsDistanceBetweenThingsUnder(me, r3, 2.0)
                    if bVar3 then goto LAB_00ec7326 end
                until false
            end
        else
            goto LAB_00ec7354
        end
        goto FLOW_past_lab_00ec7354
        ::LAB_00ec7354::
        bVar14 = false
        bVar3 = false
        quest:ClearThingHasInformation(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        value = 0xa
        if not bVar5 then
            repeat
                bVar5 = me:IsTalkedToByHero()
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    if quest:GetStateBool("FinishedQuest") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            xStack_64 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            x_stk_2c = resources:ScriptThing(xStack_b8)
                            pCVar7 = x_stk_2c
                            fret_03 = quest:GetHealth(pCVar7)
                            fVar2 = 0.0
                            if fret_03 <= fVar2 then
                                goto LAB_00ec79b4
                            end
                            goto FLOW_past_lab_00ec79b4
                            ::LAB_00ec79b4::
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_64)
                            goto LAB_00ec7cd4
                            ::FLOW_past_lab_00ec79b4::
                            iVar20 = 0
                            iVar18 = 1
                            iVar19 = 0
                            iVar17 = 0
                            pcVar16 = "TEXT_QST_B10_SISTER_THANKS"
                            pCVar7 = quest:GetHero()
                            r6 = me:Speak(pCVar7, pcVar16, iVar17, (iVar19 ~= 0), (iVar18 ~= 0), (iVar20 ~= 0))
                            iVar17 = me:IsPerformingScriptTask()
                            cVar4 = iVar17
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_64
                                    -- TODO(native): goto LAB_00ec7fd1
                                end
                                iVar17 = me:IsPerformingScriptTask()
                                cVar4 = iVar17
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then goto LAB_00ec79b4 end
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar11 = xStack_64
                            -- TODO(native): goto LAB_00ec7fd1
                        end
                        goto FLOW_hoist_lab_00ec7fd1_1
                    end
                    goto FLOW_past_lab_00ec7fd1
                    -- LAB_00ec7fd1: (native jump target)
                    resources:DestroyMovie(pCVar11)
                    ::FLOW_hoist_lab_00ec7fd1_1::
                    break
                    ::FLOW_past_lab_00ec7fd1::
                    if quest:GetStateBool("MotherIntroDone") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            pCVar10 = tostring(value)
                            pCVar10 = ("TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_" .. pCVar10)
                            xStack_bc = pCVar10
                            bVar5 = quest:TextEntryExists(xStack_bc)
                            if not bVar5 then
                                value = 0xa
                                pCVar10 = tostring(10)
                                pCVar10 = ("TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_" .. pCVar10)
                                xStack_bc = pCVar10
                            end
                            -- TODO(native): xStack_88 = (CCharString)((int)value + 0xa);
                            xStack_20 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            r3 = resources:ScriptThing(xStack_b8)
                            pCVar7 = r3
                            fret_05 = quest:GetHealth(pCVar7)
                            fVar2 = 0.0
                            if fVar2 < fret_05 then
                                p5 = 0
                                iVar20 = 1
                                iVar18 = 0
                                iVar19 = 0
                                p1 = xStack_bc
                                iVar17 = quest:GetHero()
                                r7 = me:Speak(iVar17, p1, iVar19, (iVar18 ~= 0), (iVar20 ~= 0), (p5 ~= 0))
                                iVar17 = me:IsPerformingScriptTask()
                                cVar4 = iVar17
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_20)
                                        resources:ReleaseResource(xStack_b8)
                                        return
                                    end
                                    iVar17 = me:IsPerformingScriptTask()
                                    cVar4 = iVar17
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- LAB_00ec7f87: (native jump target)
                                    resources:DestroyMovie(xStack_20)
                                    resources:ReleaseResource(xStack_b8)
                                    return
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_20)
                            goto LAB_00ec7cd4
                        end
                        break
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    xStack_10 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_44 = resources:ScriptThing(xStack_b8)
                    pCVar7 = x_stk_44
                    fret_04 = quest:GetHealth(pCVar7)
                    fVar2 = 0.0
                    if fVar2 < fret_04 then
                        iVar20 = 0
                        iVar18 = 1
                        iVar19 = 0
                        iVar17 = 0
                        pcVar16 = "TEXT_QST_B10_SISTER_REMINDER"
                        pCVar7 = quest:GetHero()
                        r8 = me:Speak(pCVar7, pcVar16, iVar17, (iVar19 ~= 0), (iVar18 ~= 0), (iVar20 ~= 0))
                        iVar17 = me:IsPerformingScriptTask()
                        cVar4 = iVar17
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar11 = xStack_10
                                -- TODO(native): goto LAB_00ec7fd1
                            end
                            iVar17 = me:IsPerformingScriptTask()
                            cVar4 = iVar17
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar11 = xStack_10
                            -- TODO(native): goto LAB_00ec7fd1
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_10)
                end
                ::LAB_00ec7cd4::
                bVar5 = me:MsgIsHitByHero()
                if bVar5 then
                    goto LAB_00ec7d68
                else
                    bVar14 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar14 then
                        bVar14 = true
                        bVar3 = true
                        bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar5 then goto LAB_00ec7d68 end
                    end
                    bVar14 = true
                    bVar5 = false
                end
                goto FLOW_past_lab_00ec7d68
                ::LAB_00ec7d68::
                bVar5 = true
                ::FLOW_past_lab_00ec7d68::
                bVar12 = bVar6 | 0x80
                if bVar3 then
                    bVar3 = false
                end
                if bVar14 then
                    bVar14 = false
                end
                if bVar12 < 0 then
                    bVar12 = bVar6 & 0x7f
                end
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    xStack_54 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    xStack_78 = resources:ScriptThing(xStack_b8)
                    pCVar7 = xStack_78
                    fret_06 = quest:GetHealth(pCVar7)
                    fVar2 = 0.0
                    if fVar2 < fret_06 then
                        iVar20 = 0
                        iVar18 = 1
                        iVar19 = 0
                        iVar17 = 0
                        pcVar16 = "TEXT_QST_B10_SISTER_ON_HIT"
                        pCVar7 = quest:GetHero()
                        r9 = me:Speak(pCVar7, pcVar16, iVar17, (iVar19 ~= 0), (iVar18 ~= 0), (iVar20 ~= 0))
                        iVar17 = me:IsPerformingScriptTask()
                        cVar4 = iVar17
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00ec7fca
                            end
                            iVar17 = me:IsPerformingScriptTask()
                            cVar4 = iVar17
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00ec7fca
                        end
                        goto FLOW_past_lab_00ec7fca
                        ::LAB_00ec7fca::
                        pCVar11 = xStack_54
                        -- TODO(native): goto LAB_00ec7fd1
                        ::FLOW_past_lab_00ec7fca::
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_54)
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                bVar6 = bVar12
                if bVar5 then
                    resources:ReleaseResource(xStack_b8)
                    return
                end
            until false
        end
        ::FLOW_past_lab_00ec7354::
        resources:ReleaseResource(xStack_b8)
    end
end

function Init(quest, me)
    __native_entity_state:SetStateBool("LeadHeroToSickChild", false)
    quest:SetThingHasInformation(me, false, true, false)
end

function OnPersist(quest, me, context)
    local leadHeroToSickChild = quest:GetStateBool("LeadHeroToSickChild") or false
    leadHeroToSickChild = quest:PersistTransferBool(context, "LeadHeroToSickChild", leadHeroToSickChild)
    quest:SetStateBool("LeadHeroToSickChild", leadHeroToSickChild)
end

function OnPredicateFail(quest, me)
end

