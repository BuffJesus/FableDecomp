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
    local bVar12, bVar14, bVar3, bVar5, bVar6, cVar4, c_stk_a5, fVar15, fVar2, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, iVar17, iVar18, iVar19, iVar20, native_arg_sequence_1, nextPostIntroReminderNumber, p0, p1, p5, pCVar10, pCVar11, pCVar7, pcVar16, postIntroReminderNumber, puVar9, r1, r2, r3, r4, r5, r6, r7, r8, r9, uVar8, xStack_10, xStack_20, xStack_54, xStack_64, xStack_68, xStack_78, xStack_94, xStack_a0, xStack_b8, xStack_bc, xStack_c0, x_stk_2c, x_stk_44
    local alive = true
    local function __cleanup_LAB_00ec782a()
        quest:DeregisterTimer(xStack_bc)
        quest:DeregisterTimer(xStack_c0)
        resources:ReleaseResource(xStack_b8)
    end
    local function __cleanup_LAB_00ec7838()
        quest:DeregisterTimer(xStack_c0)
        resources:ReleaseResource(xStack_b8)
    end
    local function __cleanup_LAB_00ec7fd1()
        resources:DestroyMovie(pCVar11)
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
                xStack_bc = "SCRIPT_NAME_HERO"
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
                        uVar8 = quest:RetailRandModulo(32768)
                        xStack_94 = quest:ReadGlobalGameDataDefStringAt(0x74c, uVar8 % ((iVar17 - iVar19) >> 2))
                        resources:PlayAnimationWithNativeArgument5(xStack_b8, xStack_94, false, false, false, true, false, false)
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
                    bVar3 = resources:IsHitByHeroExceptAbility(me, 14)
                    bVar6 = 0
                    if bVar3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then __cleanup_LAB_00ec7838(); return end
                        pCVar7 = quest:GetHero()
                        me:StopFollowingThing(pCVar7)
                        xStack_78 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        xStack_a0, pCVar7 = nil, nil
                        if resources:ResourceThingHealthAbove(xStack_b8, 0.0) then
                            iVar20 = 0
                            iVar18 = 1
                            iVar19 = 0
                            iVar17 = 2
                            pcVar16 = "TEXT_QST_B10_SISTER_ON_HIT"
                            pCVar7 = quest:GetHero()
                            r1 = me:Speak(pCVar7, pcVar16, iVar17, (iVar19 ~= 0), (iVar18 ~= 0), (iVar20 ~= 0))
                            if not r1 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_78)
                                quest:DeregisterTimer(xStack_c0)
                                resources:ReleaseResource(xStack_b8)
                                return
                            end
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
                quest:CameraUseCameraPoint("CAM_SC_GIRL", me, -1.0, 0, -1)
                xStack_78 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                x_stk_44, pCVar7 = nil, nil
                if not resources:ResourceThingHealthAbove(xStack_b8, 0.0) then goto LAB_00ec700a end
                iVar20 = 0
                iVar18 = 1
                iVar19 = 0
                iVar17 = 0
                pcVar16 = "TEXT_QST_B10_SISTER_TOLD_TO_FOLLOW_20"
                pCVar7 = quest:GetHero()
                r2 = me:Speak(pCVar7, pcVar16, iVar17, (iVar19 ~= 0), (iVar18 ~= 0), (iVar20 ~= 0))
                if not r2 then
                    -- LAB_00ec6e32: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_78)
                    quest:DeregisterTimer(xStack_c0)
                    resources:ReleaseResource(xStack_b8)
                    return
                end
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
                    xStack_68 = (("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(me, r3))))
                    pCVar7 = quest:GetHero()
                    fVar15 = resources:DistanceBetweenThings(pCVar7, r3)
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
                                x_stk_44, pCVar7 = nil, nil
                                if not resources:ResourceThingHealthAbove(xStack_b8, 0.0) then
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
                                if not r4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_78
                                    goto LAB_00ec7821
                                end
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
                                __cleanup_LAB_00ec782a(); return
                            else
                                goto LAB_00ec75cd
                            end
                            goto FLOW_past_lab_00ec75cd
                            ::LAB_00ec75cd::
                            bVar3 = resources:IsHitByHeroExceptAbility(me, 14)
                            bVar6 = 0
                            if not bVar3 then goto LAB_00ec730e end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                me:ClearCommands()
                                xStack_64 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                x_stk_2c, pCVar7 = nil, nil
                                if resources:ResourceThingHealthAbove(xStack_b8, 0.0) then
                                    iVar20 = 0
                                    iVar18 = 1
                                    iVar19 = 0
                                    iVar17 = 2
                                    pcVar16 = "TEXT_QST_B10_SISTER_ON_HIT"
                                    pCVar7 = quest:GetHero()
                                    r5 = me:Speak(pCVar7, pcVar16, iVar17, (iVar19 ~= 0), (iVar18 ~= 0), (iVar20 ~= 0))
                                    if not r5 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        pCVar11 = xStack_64
                                        resources:DestroyMovie(pCVar11)
                                        __cleanup_LAB_00ec782a(); return
                                    end
                                    iVar17 = me:IsPerformingScriptTask()
                                    cVar4 = iVar17
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            pCVar11 = xStack_64
                                            resources:DestroyMovie(pCVar11)
                                            __cleanup_LAB_00ec782a(); return
                                        end
                                        iVar17 = me:IsPerformingScriptTask()
                                        cVar4 = iVar17
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        -- LAB_00ec781d: (native jump target)
                                        pCVar11 = xStack_64
                                        resources:DestroyMovie(pCVar11)
                                        __cleanup_LAB_00ec782a(); return
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
                                resources:PlayAnimationWithNativeArgument5(xStack_b8, "STANDARD_WAVE", false, false, false, true, false, false)
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
        nextPostIntroReminderNumber = 10
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        postIntroReminderNumber = 10
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
                            x_stk_2c, pCVar7 = nil, nil
                            if not resources:ResourceThingHealthAbove(xStack_b8, 0.0) then
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
                            if not r6 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar11 = xStack_64
                                __cleanup_LAB_00ec7fd1(); return
                            end
                            iVar17 = me:IsPerformingScriptTask()
                            cVar4 = iVar17
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar11 = xStack_64
                                    __cleanup_LAB_00ec7fd1(); return
                                end
                                iVar17 = me:IsPerformingScriptTask()
                                cVar4 = iVar17
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then goto LAB_00ec79b4 end
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar11 = xStack_64
                            __cleanup_LAB_00ec7fd1()
                            return
                        end
                        break
                    end
                    if quest:GetStateBool("MotherIntroDone") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            pCVar10 = tostring(postIntroReminderNumber)
                            pCVar10 = ("TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_" .. pCVar10)
                            xStack_bc = pCVar10
                            bVar5 = quest:TextEntryExists(xStack_bc)
                            if not bVar5 then
                                postIntroReminderNumber = 10
                                pCVar10 = tostring(10)
                                pCVar10 = ("TEXT_QST_B10_SISTER_REMINDER_POSTINTRO_" .. pCVar10)
                                xStack_bc = pCVar10
                            end
                            nextPostIntroReminderNumber = ((postIntroReminderNumber + 0xa + 0x80000000) & 0xffffffff) - 0x80000000
                            xStack_20 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            r3, pCVar7 = nil, nil
                            if resources:ResourceThingHealthAbove(xStack_b8, 0.0) then
                                p5 = 0
                                iVar20 = 1
                                iVar18 = 0
                                iVar19 = 0
                                p1 = xStack_bc
                                iVar17 = quest:GetHero()
                                r7 = me:Speak(iVar17, p1, iVar19, (iVar18 ~= 0), (iVar20 ~= 0), (p5 ~= 0))
                                if not r7 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_20)
                                    resources:ReleaseResource(xStack_b8)
                                    return
                                end
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
                    x_stk_44, pCVar7 = nil, nil
                    if resources:ResourceThingHealthAbove(xStack_b8, 0.0) then
                        iVar20 = 0
                        iVar18 = 1
                        iVar19 = 0
                        iVar17 = 0
                        pcVar16 = "TEXT_QST_B10_SISTER_REMINDER"
                        pCVar7 = quest:GetHero()
                        r8 = me:Speak(pCVar7, pcVar16, iVar17, (iVar19 ~= 0), (iVar18 ~= 0), (iVar20 ~= 0))
                        if not r8 then
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar11 = xStack_10
                            resources:DestroyMovie(pCVar11)
                            resources:ReleaseResource(xStack_b8)
                            return
                        end
                        iVar17 = me:IsPerformingScriptTask()
                        cVar4 = iVar17
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar11 = xStack_10
                                resources:DestroyMovie(pCVar11)
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
                            pCVar11 = xStack_10
                            resources:DestroyMovie(pCVar11)
                            resources:ReleaseResource(xStack_b8)
                            return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_10)
                end
                ::LAB_00ec7cd4::
                bVar5 = resources:IsHitByHeroExceptAbility(me, 14)
                bVar12 = 0
                bVar3 = false
                bVar14 = false
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    xStack_54 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    xStack_78, pCVar7 = nil, nil
                    if resources:ResourceThingHealthAbove(xStack_b8, 0.0) then
                        iVar20 = 0
                        iVar18 = 1
                        iVar19 = 0
                        iVar17 = 0
                        pcVar16 = "TEXT_QST_B10_SISTER_ON_HIT"
                        pCVar7 = quest:GetHero()
                        r9 = me:Speak(pCVar7, pcVar16, iVar17, (iVar19 ~= 0), (iVar18 ~= 0), (iVar20 ~= 0))
                        if not r9 then
                            quest:PauseAllNonScriptedEntities(false)
                            pCVar11 = xStack_54
                            resources:DestroyMovie(pCVar11)
                            resources:ReleaseResource(xStack_b8)
                            return
                        end
                        iVar17 = me:IsPerformingScriptTask()
                        cVar4 = iVar17
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                pCVar11 = xStack_54
                                resources:DestroyMovie(pCVar11)
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
                            -- LAB_00ec7fca: (native jump target)
                            pCVar11 = xStack_54
                            __cleanup_LAB_00ec7fd1(); return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_54)
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                postIntroReminderNumber = nextPostIntroReminderNumber
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
    local leadHeroToSickChild = __native_entity_state:GetStateBool("LeadHeroToSickChild") or false
    leadHeroToSickChild = quest:PersistTransferBool(context, "LeadHeroToSickChild", leadHeroToSickChild)
    __native_entity_state:SetStateBool("LeadHeroToSickChild", leadHeroToSickChild)
end

function OnPredicateFail(quest, me)
end

