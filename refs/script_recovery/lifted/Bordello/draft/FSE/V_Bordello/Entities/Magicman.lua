-- Generated native draft: Magicman. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, bVar5, bVar7, cVar16, cVar6, fVar13, fVar23, iVar15, iVar18, iVar19, iVar20, iVar21, iVar22, i_stk_350, i_stk_378, native_arg_switch_1, native_arg_switch_2, p0, p0_00, pCVar10, pCVar11, pcVar14, ppuStack_348, ppuVar4, pvVar8, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r3, r30, r31, r32, r33, r34, r35, r36, r37, r38, r39, r4, r40, r41, r42, r43, r5, r6, r7, r8, r9, this_00, this_01, timerId, uVar3, u_stk_374, xStack_160, xStack_16c, xStack_180, xStack_214, xStack_254, xStack_2d8, xStack_2f8, xStack_2f8_2, xStack_310, xStack_330, xStack_340, xStack_344, xStack_350, xStack_354, xStack_364, x_stk_20c, x_stk_224, x_stk_230, x_stk_23c, x_stk_248, x_stk_260, x_stk_268
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    xStack_364 = resources:NewResource()
    resources:PrepareResource(0)
    cVar6 = me:AcquireControl(4)
    while not cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00e44528 end
        cVar6 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then goto LAB_00e44528 end
    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)(*(int *)(this + 0x14) + 0x74),xStack_364);
    i_stk_378 = quest:RegisterTimer()
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0x28)
    i_stk_350 = 1
    cVar6 = quest:GetStateBool("PlayerOwned")
    while not cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00e4450d end
        cVar6 = quest:GetStateBool("HeroTricking")
        while (not cVar6 and (not quest:GetStateBool("PlayerOwned"))) do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00e448a1 end
            iVar21 = 2.0
            pvVar8 = me:GetHomePos()
            iVar21 = (me ~= nil and me:IsDistanceFromPositionOver(pvVar8, iVar21))
            __native_condition_1 = iVar21
            if __native_condition_1 then
                iVar21 = me:IsPerformingScriptTask()
                __native_condition_1 = not iVar21
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e448a1 end
                iVar22 = 1
                iVar20 = 0
                iVar19 = 0
                iVar18 = 1.0
                iVar21 = me:GetHomePos()
                me:MoveToPosition(iVar21, iVar18, iVar19, (iVar20 ~= 0), (iVar22 ~= 0))
            end
            if not quest:GetStateBool("BeerSetToDPad") then
                u_stk_374 = u_stk_374 | 1
                iVar21 = quest:GetHeroTargetedThing()
                bVar7 = (iVar21 ~= nil and iVar21:IsEqualTo(me))
                bVar5 = true
                if not bVar7 then goto LAB_00e410ba end
            else
                goto LAB_00e410ba
            end
            goto FLOW_past_lab_00e410ba
            ::LAB_00e410ba::
            bVar5 = false
            ::FLOW_past_lab_00e410ba::
            if (u_stk_374 & 1) ~= 0 then
                u_stk_374 = u_stk_374 & 0xfffffffe
                x_stk_268 = nil
            end
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e448a1 end
                quest:SetStateBool("BeerSetToDPad", true)
                quest:SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", iVar18, iVar19)
                quest:CreateThread("WatchForHeroLeavingRegionWithBeer")  -- native thread body 0x00E44980: lift it as function WatchForHeroLeavingRegionWithBeer(quest)
                if (u_stk_374 & 8) ~= 0 then
                    u_stk_374 = u_stk_374 & 0xfffffff7
                end
                if (u_stk_374 & 4) ~= 0 then
                    u_stk_374 = u_stk_374 & 0xfffffffb
                end
                if (u_stk_374 & 2) ~= 0 then
                    u_stk_374 = u_stk_374 & 0xfffffffd
                end
            else
                if not quest:GetStateBool("BeerSetToDPad") then
                    goto LAB_00e412bc
                else
                    u_stk_374 = u_stk_374 | 0x10
                    iVar21 = quest:GetHeroTargetedThing()
                    bVar7 = (iVar21 ~= nil and iVar21:IsEqualTo(me))
                    bVar5 = true
                    if bVar7 then goto LAB_00e412bc end
                end
                goto FLOW_past_lab_00e412bc
                ::LAB_00e412bc::
                bVar5 = false
                ::FLOW_past_lab_00e412bc::
                if (u_stk_374 & 0x10) ~= 0 then
                    u_stk_374 = u_stk_374 & 0xffffffef
                    xStack_214 = nil
                end
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e448a1 end
                    quest:SetStateBool("BeerSetToDPad", false)
                end
            end
            iVar21 = i_stk_378
            iVar18 = quest:GetTimer(i_stk_378)
            if iVar18 == 0 then
                fVar23 = 8.0
                pCVar11 = quest:GetHero()
                bVar5 = quest:IsDistanceBetweenThingsUnder(me, pCVar11, fVar23)
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(iVar21)
                        goto LAB_00e4451f
                    end
                    quest:SetTimer(iVar21, 4)
                    r1 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(r1, pCVar11)
                end
            end
            iVar18 = quest:GetTimer(timerId)
            if iVar18 == 0 then
                fVar23 = 15.0
                pCVar11 = quest:GetHero()
                bVar5 = quest:IsDistanceBetweenThingsUnder(me, pCVar11, fVar23)
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(iVar21)
                        goto LAB_00e4451f
                    end
                    pCVar10 = tostring(i_stk_350)
                    ("TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. pCVar10)
                    cVar6 = quest:TextEntryExists()
                    if not cVar6 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:DeregisterTimer(timerId)
                            quest:DeregisterTimer(iVar21)
                            goto LAB_00e4451f
                        end
                        i_stk_350 = 1
                    end
                    r2 = quest:AddNewConversation(pCVar11, (i_stk_350 ~= 0), (fVar23 ~= 0))
                    r3 = quest:GetHero()
                    quest:AddPersonToConversation(iVar20, r3)
                    -- TODO(native): xStack_308 = (undefined **)**(int **)(this + 4);
                    -- TODO(native): xStack_344 = (int *)(*(code *)xStack_308[0x46])();
                    ppuVar4 = 0
                    xStack_350 = p0
                    pCVar10 = tostring(0)
                    ("TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. pCVar10)
                    -- TODO(native): (*(code *)xStack_308[0x16e])();
                    i_stk_350 = ppuVar4 + 1
                    quest:SetTimer(timerId, 0x1e)
                end
            end
            cVar6 = me:IsTalkedToByHero()
            if cVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e448a1 end
                me:ClearCommands()
                xStack_340 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities((iVar22 ~= 0))
                quest:FixMovieSequenceCamera(false)
                xStack_344 = quest:GetHero()
                xStack_350 = p0
                quest:EntitySetFacingAngleTowardsThing(xStack_344, nil --[[missing]])
                quest:Pause(0)
                iVar18 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(iVar18, nil --[[missing]])
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], 0x0, nil --[[missing]], nil --[[missing]])
                -- TODO(native): bVar5 = NScript::CV_BordelloScript::IsHeroWearingBeard__ate3e320(*(CV_BordelloScript **)(this + 0x14));
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        x_stk_268 = resources:ScriptThing(xStack_350)
                        fVar13 = quest:GetHealth(nil --[[missing]])
                        cVar6 = 0.0 < fVar13
                        x_stk_260 = nil
                        if cVar6 then
                            iVar15 = 0
                            iVar22 = 1
                            iVar20 = 0
                            iVar19 = 0
                            pcVar14 = "TEXT_QST_B13_MAGICMAN_PIMPING"
                            iVar21 = quest:GetHero()
                            r4 = me:Speak(iVar21, pcVar14, iVar19, (iVar20 ~= 0), (iVar22 ~= 0), (iVar15 ~= 0))
                            iVar21 = me:IsPerformingScriptTask()
                            cVar6 = iVar21
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    quest:DeregisterTimer(me)
                                    quest:DeregisterTimer(iVar18)
                                    return
                                end
                                iVar21 = me:IsPerformingScriptTask()
                                cVar6 = iVar21
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                this_01 = xStack_340
                                resources:DestroyMovie(this_01)
                                goto LAB_00e448a1
                            end
                        end
                        goto LAB_00e41b75
                    end
                    goto LAB_00e445a0
                end
                if not __native_entity_state:GetStateBool("DoneToutIntro") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e445a0 end
                    __native_entity_state:SetStateBool("DoneToutIntro", true)
                    xStack_254 = resources:ScriptThing(xStack_350)
                    fVar13 = quest:GetHealth(nil --[[missing]])
                    cVar6 = 0.0 < fVar13
                    xStack_254 = nil
                    if cVar6 then
                        iVar22 = 0
                        iVar20 = 1
                        iVar19 = 0
                        iVar18 = 0
                        pcVar14 = "TEXT_QST_B13_MAGICMAN_TOUTING"
                        iVar21 = quest:GetHero()
                        r5 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                        iVar21 = me:IsPerformingScriptTask()
                        cVar6 = iVar21
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e44615 end
                            iVar21 = me:IsPerformingScriptTask()
                            cVar6 = iVar21
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e445a0 end
                    end
                    goto LAB_00e41b75
                end
                goto FLOW_past_lab_00e41b75
                ::LAB_00e41b75::
                quest:FixMovieSequenceCamera(nil --[[missing]])
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                resources:DestroyMovie(xStack_340)
                goto LAB_00e41ba8
                ::FLOW_past_lab_00e41b75::
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    -- LAB_00e44677: (native jump target)
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    this_01 = xStack_340
                    -- LAB_00e4489c_c2: (native jump target)
                    resources:DestroyMovie(this_01)
                    goto LAB_00e448a1
                end
                if not __native_entity_state:GetStateBool("MentionedPimpHat") then
                    -- TODO(native): xStack_350 = (CScriptThing *)((uint)xStack_350 | 0x20);
                    r6 = quest:GetHero()
                    cVar6 = quest:IsWearingClothingItem(r6, "OBJECT_HERO_HAT_PIMP")
                    cVar16 = true
                    if not cVar6 then goto LAB_00e418e3 end
                else
                    goto LAB_00e418e3
                end
                goto FLOW_past_lab_00e418e3
                ::LAB_00e418e3::
                cVar16 = false
                ::FLOW_past_lab_00e418e3::
                if (xStack_350 & 0x20) ~= 0 then
                    -- TODO(native): xStack_350 = (CScriptThing *)((uint)xStack_350 & 0xffffffdf);
                end
                if not cVar16 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        if 4 < xStack_364 then
                        end
                        pCVar10 = tostring(1)
                        ("TEXT_QST_B13_MAGICMAN_CHATTER_0" .. pCVar10)
                        x_stk_23c = resources:ScriptThing(1)
                        fVar13 = quest:GetHealth(nil --[[missing]])
                        cVar6 = 0.0 < fVar13
                        x_stk_23c = nil
                        if cVar6 then
                            iVar22 = 0
                            iVar20 = 1
                            iVar19 = 0
                            iVar18 = 0
                            iVar21 = quest:GetHero()
                            r7 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                            iVar21 = me:IsPerformingScriptTask()
                            cVar6 = iVar21
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(true)
                                    this_00 = xStack_340
                                    goto LAB_00e44862
                                end
                                iVar21 = me:IsPerformingScriptTask()
                                cVar6 = iVar21
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(cVar16)
                                this_01 = xStack_340
                                resources:DestroyMovie(this_01)
                                goto LAB_00e448a1
                            end
                        end
                        -- TODO(native): xStack_364 = xStack_364 + 1;
                        goto LAB_00e41b75
                    end
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    this_01 = xStack_340
                    resources:DestroyMovie(this_01)
                    goto LAB_00e448a1
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    x_stk_224 = resources:ScriptThing(xStack_364)
                    fVar13 = quest:GetHealth(nil --[[missing]])
                    cVar6 = 0.0 < fVar13
                    x_stk_224 = nil
                    if cVar6 then
                        iVar22 = 0
                        iVar20 = 1
                        iVar19 = 0
                        iVar18 = 0
                        pcVar14 = "TEXT_QST_B13_MAGICMAN_PIMP_HAT"
                        iVar21 = quest:GetHero()
                        r8 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                        iVar21 = me:IsPerformingScriptTask()
                        cVar6 = iVar21
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e44615 end
                            iVar21 = me:IsPerformingScriptTask()
                            cVar6 = iVar21
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e445a0 end
                    end
                    __native_entity_state:SetStateBool("MentionedPimpHat", true)
                    goto LAB_00e41b75
                end
                ::LAB_00e445a0::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_340)
                goto LAB_00e44862
            end
            ::LAB_00e41ba8::
            uVar3 = u_stk_374
            u_stk_374 = u_stk_374 | 0x40
            cVar6 = me:MsgIsHitByHero()
            if not cVar6 then
                u_stk_374 = uVar3 | 0xc0
                cVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if cVar6 then
                    u_stk_374 = uVar3 | 0x1c0
                    cVar6 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                    if not cVar6 then goto LAB_00e41c4b end
                end
                bVar5 = false
            else
                goto LAB_00e41c4b
            end
            goto FLOW_past_lab_00e41c4b
            ::LAB_00e41c4b::
            bVar5 = true
            ::FLOW_past_lab_00e41c4b::
            if (u_stk_374 & 0x100) ~= 0 then
                u_stk_374 = u_stk_374 & 0xfffffeff
            end
            if u_stk_374 < 0 then
                u_stk_374 = u_stk_374 & 0xffffff7f
            end
            if (u_stk_374 & 0x40) ~= 0 then
                u_stk_374 = u_stk_374 & 0xffffffbf
            end
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e448a1 end
                me:ClearCommands()
                xStack_2f8 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                quest:FixMovieSequenceCamera(nil --[[missing]])
                xStack_344 = quest:GetHero()
                xStack_364 = p0
                quest:EntitySetFacingAngleTowardsThing(xStack_344, nil --[[missing]])
                quest:Pause(nil --[[missing]])
                iVar18 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(iVar18, nil --[[missing]])
                alive = quest:NewScriptFrame(me)
                iVar21 = -1
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], iVar21, nil --[[missing]], nil --[[missing]])
                -- TODO(native): bVar5 = NScript::CV_BordelloScript::IsHeroWearingBeard__ate3e320(*(CV_BordelloScript **)(this + 0x14));
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        x_stk_260 = resources:ScriptThing(xStack_364)
                        fVar13 = quest:GetHealth(nil --[[missing]])
                        cVar6 = 0.0 < fVar13
                        x_stk_20c = nil
                        x_stk_260 = 0
                        if not cVar6 then
                            goto LAB_00e42003
                        end
                        goto FLOW_past_lab_00e42003
                        ::LAB_00e42003::
                        quest:FixMovieSequenceCamera((x_stk_260 ~= 0))
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        resources:DestroyMovie(xStack_2f8)
                        goto LAB_00e4203f
                        ::FLOW_past_lab_00e42003::
                        iVar15 = 0
                        iVar22 = 1
                        iVar20 = 0
                        iVar19 = 0
                        pcVar14 = "TEXT_QST_B13_MAGICMAN_ATTACKED_FEMALE"
                        iVar21 = quest:GetHero()
                        r9 = me:Speak(iVar21, pcVar14, iVar19, (iVar20 ~= 0), (iVar22 ~= 0), (iVar15 ~= 0))
                        iVar21 = me:IsPerformingScriptTask()
                        cVar6 = iVar21
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                resources:DestroyMovie(ppuStack_348)
                                quest:DeregisterTimer(me)
                                quest:DeregisterTimer(iVar18)
                                return
                            end
                            iVar21 = me:IsPerformingScriptTask()
                            cVar6 = iVar21
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then goto LAB_00e42003 end
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        this_01 = xStack_2f8
                        -- LAB_00e4489c: (native jump target)
                        resources:DestroyMovie(this_01)
                        goto LAB_00e448a1
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        xStack_2d8 = resources:ScriptThing(xStack_364)
                        fVar13 = quest:GetHealth(nil --[[missing]])
                        cVar6 = 0.0 < fVar13
                        x_stk_230 = nil
                        if cVar6 then
                            iVar15 = 0
                            iVar22 = 1
                            iVar20 = 0
                            iVar19 = 0
                            pcVar14 = "TEXT_QST_B13_MAGICMAN_ATTACKED_MALE"
                            iVar18 = quest:GetHero()
                            r10 = me:Speak(iVar18, pcVar14, iVar19, (iVar20 ~= 0), (iVar22 ~= 0), (iVar15 ~= 0))
                            iVar18 = me:IsPerformingScriptTask()
                            cVar6 = iVar18
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_340)
                                    quest:DeregisterTimer(iVar21)
                                    quest:DeregisterTimer(me)
                                    return
                                end
                                iVar18 = me:IsPerformingScriptTask()
                                cVar6 = iVar18
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(this_01)
                                goto LAB_00e448a1
                            end
                        end
                        -- LAB_00e42003_c7: (native jump target)
                        quest:FixMovieSequenceCamera(nil --[[missing]])
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        resources:DestroyMovie(xStack_2f8)
                        goto LAB_00e4203f
                    end
                end
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                resources:DestroyMovie(xStack_16c)
                goto LAB_00e44862
            end
            ::LAB_00e4203f::
            cVar6 = me:MsgIsPresentedWithItem()
            if cVar6 then
                if unaff_EBP == nil then
                    bVar5 = false
                    if bVar5 then
                        goto LAB_00e42094
                    end
                else
                    -- TODO(native): iVar21 = CBasicString<char>::Compare((void *)*unaff_EBP,"OBJECT_BEER_TANKARD");
                    if iVar21 == 0 then goto LAB_00e42094 end
                end
                goto FLOW_past_lab_00e42094
                ::LAB_00e42094::
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e448a1 end
                me:ClearCommands()
                xStack_330 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                quest:FixMovieSequenceCamera(nil --[[missing]])
                xStack_344 = quest:GetHero()
                xStack_364 = p0
                quest:EntitySetFacingAngleTowardsThing(xStack_344, nil --[[missing]])
                quest:Pause(nil --[[missing]])
                r11 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(r11, nil --[[missing]])
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]])
                native_arg_switch_1 = quest:GetStateInt("BeersDrunk")
                repeat
                    if native_arg_switch_1 == 0 then
                        fVar13 = quest:GetHealth(nil --[[missing]])
                        cVar6 = 0.0 < fVar13
                        if cVar6 then
                            iVar22 = 0
                            iVar20 = 1
                            iVar19 = 0
                            iVar18 = 0
                            pCVar10 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                            pCVar10 = ("TEXT_QST_B13_MAGICMAN_BEER1" .. pCVar10)
                            pvVar8 = pCVar10
                            iVar21 = quest:GetHero()
                            r12 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                            iVar21 = me:IsPerformingScriptTask()
                            cVar6 = iVar21
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e44811 end
                                iVar21 = me:IsPerformingScriptTask()
                                cVar6 = iVar21
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- LAB_00e4488e: (native jump target)
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                this_01 = xStack_330
                                -- LAB_00e4489c_c8: (native jump target)
                                resources:DestroyMovie(this_01)
                                goto LAB_00e448a1
                            end
                        end
                        break
                    else
                        if native_arg_switch_1 == 1 then
                            xStack_180 = resources:ScriptThing(xStack_364)
                            fVar13 = quest:GetHealth(nil --[[missing]])
                            cVar6 = 0.0 < fVar13
                            if cVar6 then
                                iVar22 = 0
                                iVar20 = 1
                                iVar19 = 0
                                iVar18 = 0
                                pCVar10 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                                pCVar10 = ("TEXT_QST_B13_MAGICMAN_BEER2" .. pCVar10)
                                pvVar8 = pCVar10
                                iVar21 = quest:GetHero()
                                r13 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                iVar21 = me:IsPerformingScriptTask()
                                cVar6 = iVar21
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e44811 end
                                    iVar21 = me:IsPerformingScriptTask()
                                    cVar6 = iVar21
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                    this_01 = xStack_330
                                    resources:DestroyMovie(this_01)
                                    goto LAB_00e448a1
                                end
                            end
                            break
                        else
                            if native_arg_switch_1 == 2 then
                                fVar13 = quest:GetHealth(nil --[[missing]])
                                cVar6 = 0.0 < fVar13
                                if cVar6 then
                                    iVar22 = 0
                                    iVar20 = 1
                                    iVar19 = 0
                                    iVar18 = 0
                                    pCVar10 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                                    pCVar10 = ("TEXT_QST_B13_MAGICMAN_BEER3" .. pCVar10)
                                    pvVar8 = pCVar10
                                    iVar21 = quest:GetHero()
                                    r14 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                    iVar21 = me:IsPerformingScriptTask()
                                    cVar6 = iVar21
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e44811 end
                                        iVar21 = me:IsPerformingScriptTask()
                                        cVar6 = iVar21
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                        this_01 = xStack_330
                                        resources:DestroyMovie(this_01)
                                        goto LAB_00e448a1
                                    end
                                end
                                break
                            else
                                if native_arg_switch_1 == 3 then
                                    fVar13 = quest:GetHealth(nil --[[missing]])
                                    cVar6 = 0.0 < fVar13
                                    if cVar6 then
                                        iVar22 = 0
                                        iVar20 = 1
                                        iVar19 = 0
                                        iVar18 = 0
                                        pCVar10 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                                        pCVar10 = ("TEXT_QST_B13_MAGICMAN_BEER4" .. pCVar10)
                                        pvVar8 = pCVar10
                                        iVar21 = quest:GetHero()
                                        r15 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                        iVar21 = me:IsPerformingScriptTask()
                                        cVar6 = iVar21
                                        while cVar6 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e44811 end
                                            iVar21 = me:IsPerformingScriptTask()
                                            cVar6 = iVar21
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then
                                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                            this_01 = xStack_330
                                            resources:DestroyMovie(this_01)
                                            goto LAB_00e448a1
                                        end
                                    end
                                    break
                                else
                                    if native_arg_switch_1 == 4 then
                                        fVar13 = quest:GetHealth(nil --[[missing]])
                                        cVar6 = 0.0 < fVar13
                                        if cVar6 then
                                            iVar22 = 0
                                            iVar20 = 1
                                            iVar19 = 0
                                            iVar18 = 0
                                            pCVar10 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                                            pCVar10 = ("TEXT_QST_B13_MAGICMAN_BEER5" .. pCVar10)
                                            pvVar8 = pCVar10
                                            iVar21 = quest:GetHero()
                                            r16 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                            iVar21 = me:IsPerformingScriptTask()
                                            cVar6 = iVar21
                                            while cVar6 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e44811 end
                                                iVar21 = me:IsPerformingScriptTask()
                                                cVar6 = iVar21
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then
                                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                this_01 = xStack_330
                                                resources:DestroyMovie(this_01)
                                                goto LAB_00e448a1
                                            end
                                        end
                                        break
                                    else
                                        if native_arg_switch_1 == 5 then
                                            fVar13 = quest:GetHealth(nil --[[missing]])
                                            cVar6 = 0.0 < fVar13
                                            if cVar6 then
                                                iVar22 = 0
                                                iVar20 = 1
                                                iVar19 = 0
                                                iVar18 = 0
                                                pCVar10 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                                                pCVar10 = ("TEXT_QST_B13_MAGICMAN_BEER6" .. pCVar10)
                                                pvVar8 = pCVar10
                                                iVar21 = quest:GetHero()
                                                r17 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                                iVar21 = me:IsPerformingScriptTask()
                                                cVar6 = iVar21
                                                while cVar6 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then goto LAB_00e44811 end
                                                    iVar21 = me:IsPerformingScriptTask()
                                                    cVar6 = iVar21
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then
                                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                    this_01 = xStack_330
                                                    resources:DestroyMovie(this_01)
                                                    goto LAB_00e448a1
                                                end
                                            end
                                            if not quest:GetStateBool("HeroFoundDeedsLocation") then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then
                                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                    this_01 = xStack_330
                                                    resources:DestroyMovie(this_01)
                                                    goto LAB_00e448a1
                                                end
                                                -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                                                quest:SetStateBool("HeroFoundDeedsLocation", true)
                                                quest:SetStateBool("HeroPartying", true)
                                            else
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then
                                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                    resources:DestroyMovie(xStack_330)
                                                    goto LAB_00e44862
                                                end
                                                -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                                                quest:SetStateBool("HeroPartying", true)
                                            end
                                            break
                                        else
                                            fVar13 = quest:GetHealth(nil --[[missing]])
                                            cVar6 = 0.0 < fVar13
                                            x_stk_248 = nil
                                            if cVar6 then
                                                iVar22 = 0
                                                iVar20 = 1
                                                iVar19 = 0
                                                iVar18 = 0
                                                pcVar14 = "TEXT_QST_B13_MAGICMAN_BEERTOOMUCH"
                                                iVar21 = quest:GetHero()
                                                r18 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                                iVar21 = me:IsPerformingScriptTask()
                                                if iVar21 then
                                                    -- LAB_00e428b0: (native jump target)
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if not bVar5 then goto LAB_00e428c7 end
                                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                    this_00 = xStack_330
                                                    -- LAB_00e4485d: (native jump target)
                                                    goto LAB_00e44862
                                                end
                                                -- LAB_00e428d4: (native jump target)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then
                                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                    this_01 = xStack_330
                                                    resources:DestroyMovie(this_01)
                                                    goto LAB_00e448a1
                                                end
                                            end
                                            quest:GiveHeroObject("OBJECT_BEER_TANKARD", nil --[[missing]])
                                        end
                                    end
                                end
                            end
                        end
                    end
                until not (false)
                quest:SetStateInt("BeersDrunk", quest:GetStateInt("BeersDrunk") + 1)
                quest:FixMovieSequenceCamera(nil --[[missing]])
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                resources:DestroyMovie(xStack_330)
                ::FLOW_past_lab_00e42094::
            end
            cVar6 = quest:GetStateBool("HeroTricking")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00e4450d end
        cVar6 = quest:GetStateBool("PlayerOwned")
        while not cVar6 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00e4450d end
            iVar21 = 2.0
            pvVar8 = me:GetHomePos()
            iVar21 = (me ~= nil and me:IsDistanceFromPositionOver(pvVar8, iVar21))
            __native_condition_2 = iVar21
            if __native_condition_2 then
                iVar21 = me:IsPerformingScriptTask()
                __native_condition_2 = not iVar21
            end
            if __native_condition_2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e4450d end
                iVar22 = 1
                iVar20 = 0
                iVar19 = 0
                iVar18 = 1.0
                iVar21 = me:GetHomePos()
                me:MoveToPosition(iVar21, iVar18, iVar19, (iVar20 ~= 0), (iVar22 ~= 0))
            end
            if not quest:GetStateBool("BeerSetToDPad") then
                u_stk_374 = u_stk_374 | 0x200
                iVar21 = quest:GetHeroTargetedThing()
                bVar7 = (iVar21 ~= nil and iVar21:IsEqualTo(me))
                bVar5 = true
                if not bVar7 then goto LAB_00e42a3a end
            else
                goto LAB_00e42a3a
            end
            goto FLOW_past_lab_00e42a3a
            ::LAB_00e42a3a::
            bVar5 = false
            ::FLOW_past_lab_00e42a3a::
            if (u_stk_374 & 0x200) ~= 0 then
                u_stk_374 = u_stk_374 & 0xfffffdff
            end
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e4450d end
                quest:SetStateBool("BeerSetToDPad", true)
                quest:SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", iVar18, iVar19)
                quest:CreateThread("WatchForHeroLeavingRegionWithBeer")  -- native thread body 0x00E44980: lift it as function WatchForHeroLeavingRegionWithBeer(quest)
                if (u_stk_374 & 0x1000) ~= 0 then
                    u_stk_374 = u_stk_374 & 0xffffefff
                end
                if (u_stk_374 & 0x800) ~= 0 then
                    u_stk_374 = u_stk_374 & 0xfffff7ff
                end
                if (u_stk_374 & 0x400) ~= 0 then
                    u_stk_374 = u_stk_374 & 0xfffffbff
                end
            else
                if not quest:GetStateBool("BeerSetToDPad") then
                    goto LAB_00e42bf9
                else
                    u_stk_374 = u_stk_374 | 0x2000
                    iVar21 = quest:GetHeroTargetedThing()
                    bVar7 = (iVar21 ~= nil and iVar21:IsEqualTo(me))
                    bVar5 = true
                    if bVar7 then goto LAB_00e42bf9 end
                end
                goto FLOW_past_lab_00e42bf9
                ::LAB_00e42bf9::
                bVar5 = false
                ::FLOW_past_lab_00e42bf9::
                if (u_stk_374 & 0x2000) ~= 0 then
                    u_stk_374 = u_stk_374 & 0xffffdfff
                end
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e4450d end
                    quest:SetStateBool("BeerSetToDPad", false)
                end
            end
            iVar21 = i_stk_378
            iVar18 = quest:GetTimer(i_stk_378)
            if iVar18 == 0 then
                fVar23 = 8.0
                pCVar11 = quest:GetHero()
                bVar5 = quest:IsDistanceBetweenThingsUnder(me, pCVar11, fVar23)
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e4450d end
                    quest:SetTimer(iVar21, 4)
                    r19 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(r19, pCVar11)
                end
            end
            iVar21 = quest:GetTimer(timerId)
            if iVar21 == 0 then
                fVar23 = 15.0
                pCVar11 = quest:GetHero()
                bVar5 = quest:IsDistanceBetweenThingsUnder(me, pCVar11, fVar23)
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then goto LAB_00e4450d end
                    pCVar10 = tostring(i_stk_350)
                    ("TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. pCVar10)
                    cVar6 = quest:TextEntryExists()
                    if not cVar6 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e4450d end
                        i_stk_350 = 1
                    end
                    r20 = quest:AddNewConversation(pCVar11, (i_stk_350 ~= 0), (fVar23 ~= 0))
                    r21 = quest:GetHero()
                    quest:AddPersonToConversation(iVar20, r21)
                    -- TODO(native): xStack_194 = (undefined **)**(int **)(this + 4);
                    -- TODO(native): xStack_344 = (int *)(*(code *)xStack_194[0x46])();
                    ppuVar4 = xStack_364
                    xStack_364 = p0
                    pCVar10 = tostring(xStack_364)
                    ("TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. pCVar10)
                    -- TODO(native): (*(code *)xStack_194[0x16e])();
                    i_stk_350 = ppuVar4 + 1
                    quest:SetTimer(timerId, 0x1e)
                end
            end
            cVar6 = me:IsTalkedToByHero()
            if cVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e4450d end
                me:ClearCommands()
                xStack_344 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(false)
                quest:FixMovieSequenceCamera((iVar22 ~= 0))
                xStack_344 = quest:GetHero()
                xStack_364 = p0
                quest:EntitySetFacingAngleTowardsThing(xStack_344, nil --[[missing]])
                quest:Pause(nil --[[missing]])
                r22 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(r22, nil --[[missing]])
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]])
                -- TODO(native): bVar5 = NScript::CV_BordelloScript::IsHeroWearingBeard__ate3e320(*(CV_BordelloScript **)(this + 0x14));
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        -- TODO(native): bVar5 = helper_E44A40(quest, me, *(this + 0x14))
                        bVar5 = nil --[[unresolved native value]]
                        if bVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                fVar13 = quest:GetHealth(nil --[[missing]])
                                cVar6 = 0.0 < fVar13
                                if cVar6 then
                                    iVar22 = 0
                                    iVar20 = 1
                                    iVar19 = 0
                                    iVar18 = 0
                                    pcVar14 = "TEXT_QST_B13_MAGICMAN_BEARD_LADY"
                                    iVar21 = quest:GetHero()
                                    r23 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                    iVar21 = me:IsPerformingScriptTask()
                                    cVar6 = iVar21
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e448cc end
                                        iVar21 = me:IsPerformingScriptTask()
                                        cVar6 = iVar21
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then
                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                        resources:DestroyMovie(xStack_310)
                                        goto LAB_00e4450d
                                    end
                                end
                                goto LAB_00e437b3
                            end
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(xStack_310)
                            goto LAB_00e4450d
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            -- TODO(native): bVar5 = helper_E44CC0(quest, me, *(this + 0x14))
                            bVar5 = nil --[[unresolved native value]]
                            if bVar5 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if not bVar5 then
                                    fVar13 = quest:GetHealth(nil --[[missing]])
                                    cVar6 = 0.0 < fVar13
                                    if cVar6 then
                                        iVar22 = 0
                                        iVar20 = 1
                                        iVar19 = 0
                                        iVar18 = 0
                                        pcVar14 = "TEXT_QST_B13_MAGICMAN_BEARD_LADY_MOUSTACHE"
                                        iVar21 = quest:GetHero()
                                        r24 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                        iVar21 = me:IsPerformingScriptTask()
                                        cVar6 = iVar21
                                        while cVar6 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e448cc end
                                            iVar21 = me:IsPerformingScriptTask()
                                            cVar6 = iVar21
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then
                                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                            resources:DestroyMovie(xStack_310)
                                            goto LAB_00e4450d
                                        end
                                    end
                                    goto LAB_00e437b3
                                end
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                resources:DestroyMovie(xStack_310)
                                goto LAB_00e4450d
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                fVar13 = quest:GetHealth(nil --[[missing]])
                                cVar6 = 0.0 < fVar13
                                if cVar6 then
                                    r25 = me:Speak(me, "TEXT_QST_B13_MAGICMAN_LOVELY_SKIN", 0, false, true, false)
                                    iVar21 = me:IsPerformingScriptTask()
                                    cVar6 = iVar21
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then
                                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                            resources:DestroyMovie(xStack_310)
                                            goto LAB_00e4450d
                                        end
                                        iVar21 = me:IsPerformingScriptTask()
                                        cVar6 = iVar21
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e448cc end
                                end
                                r26 = quest:GetHero()
                                cVar6 = quest:IsWearingClothingItem(r26, "OBJECT_HERO_HAT_WHOREWIG")
                                if cVar6 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if not bVar5 then
                                        fVar13 = quest:GetHealth(nil --[[missing]])
                                        cVar6 = 0.0 < fVar13
                                        if cVar6 then
                                            iVar22 = 0
                                            iVar20 = 1
                                            iVar19 = 0
                                            iVar18 = 0
                                            pcVar14 = "TEXT_QST_B13_MAGICMAN_LOVELY_HAIR"
                                            iVar21 = quest:GetHero()
                                            r27 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                            iVar21 = me:IsPerformingScriptTask()
                                            cVar6 = iVar21
                                            while cVar6 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e448cc end
                                                iVar21 = me:IsPerformingScriptTask()
                                                cVar6 = iVar21
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then
                                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                resources:DestroyMovie(xStack_310)
                                                goto LAB_00e4450d
                                            end
                                        end
                                        quest:GiveHeroYesNoQuestion("TEXT_QST_B13_MAGICMAN_PARTY", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                                        iVar21 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        while iVar21 < 0 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e448cc end
                                            iVar21 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if not bVar5 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if iVar21 == 1 then
                                                if bVar5 then goto LAB_00e448cc end
                                                fVar13 = quest:GetHealth(nil --[[missing]])
                                                cVar6 = 0.0 < fVar13
                                                if cVar6 then
                                                    iVar22 = 0
                                                    iVar20 = 1
                                                    iVar19 = 0
                                                    iVar18 = 0
                                                    pcVar14 = "TEXT_QST_B13_MAGICMAN_HORNY"
                                                    iVar21 = quest:GetHero()
                                                    r28 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                                    iVar21 = me:IsPerformingScriptTask()
                                                    cVar6 = iVar21
                                                    while cVar6 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar5 = not alive
                                                        if bVar5 then
                                                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                            resources:DestroyMovie(xStack_310)
                                                            goto LAB_00e4450d
                                                        end
                                                        iVar21 = me:IsPerformingScriptTask()
                                                        cVar6 = iVar21
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then goto LAB_00e448cc end
                                                end
                                                quest:GiveHeroGold(nil --[[missing]])
                                                r29 = quest:GetNumberOfTimesHeroHasHadSex()
                                                quest:SetNumberOfTimesHeroHasHadSex(nil --[[missing]])
                                                quest:SetCutsceneSkippable(nil --[[missing]])
                                                if not quest:GetStateBool("HeroFoundDeedsLocation") then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then goto LAB_00e448cc end
                                                    -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                                                    quest:SetStateBool("HeroFoundDeedsLocation", true)
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then
                                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                        resources:DestroyMovie(xStack_310)
                                                        goto LAB_00e4450d
                                                    end
                                                    -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                                                end
                                                quest:SetCutsceneSkippable(nil --[[missing]])
                                                quest:SetStateBool("HeroPartying", true)
                                                quest:SetStateBool("HeroFoundDeedsLocation", true)
                                                quest:SetHeroAsHavingHadGaySex(nil --[[missing]])
                                            else
                                                if bVar5 then
                                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                    resources:DestroyMovie(xStack_310)
                                                    goto LAB_00e4450d
                                                end
                                                fVar13 = quest:GetHealth(nil --[[missing]])
                                                cVar6 = 0.0 < fVar13
                                                if cVar6 then
                                                    iVar22 = 0
                                                    iVar20 = 1
                                                    iVar19 = 0
                                                    iVar18 = 0
                                                    pcVar14 = "TEXT_QST_B13_MAGICMAN_REJECTED"
                                                    iVar21 = quest:GetHero()
                                                    r30 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                                    iVar21 = me:IsPerformingScriptTask()
                                                    cVar6 = iVar21
                                                    while cVar6 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar5 = not alive
                                                        if bVar5 then goto LAB_00e448cc end
                                                        iVar21 = me:IsPerformingScriptTask()
                                                        cVar6 = iVar21
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then
                                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                        resources:DestroyMovie(xStack_310)
                                                        goto LAB_00e4450d
                                                    end
                                                end
                                            end
                                            goto LAB_00e437b3
                                        end
                                    end
                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                    resources:DestroyMovie(xStack_310)
                                    goto LAB_00e4450d
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if not bVar5 then
                                    fVar13 = quest:GetHealth(nil --[[missing]])
                                    cVar6 = 0.0 < fVar13
                                    if cVar6 then
                                        iVar22 = 0
                                        iVar20 = 1
                                        iVar19 = 0
                                        iVar18 = 0
                                        pcVar14 = "TEXT_QST_B13_MAGICMAN_BAD_HAIR"
                                        iVar21 = quest:GetHero()
                                        r31 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                        iVar21 = me:IsPerformingScriptTask()
                                        cVar6 = iVar21
                                        while cVar6 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then
                                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                resources:DestroyMovie(xStack_310)
                                                goto LAB_00e4450d
                                            end
                                            iVar21 = me:IsPerformingScriptTask()
                                            cVar6 = iVar21
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e448cc end
                                    end
                                    goto LAB_00e437b3
                                end
                            end
                        end
                    end
                    ::LAB_00e448cc::
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    resources:DestroyMovie(xStack_310)
                    goto LAB_00e4450d
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    -- LAB_00e43023: (native jump target)
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    resources:DestroyMovie(xStack_310)
                    goto LAB_00e4450d
                end
                fVar13 = quest:GetHealth(nil --[[missing]])
                cVar6 = 0.0 < fVar13
                if cVar6 then
                    iVar22 = 0
                    iVar20 = 1
                    iVar19 = 0
                    iVar18 = 0
                    pcVar14 = "TEXT_QST_B13_MAGICMAN_TOUTING"
                    iVar21 = quest:GetHero()
                    r32 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                    iVar21 = me:IsPerformingScriptTask()
                    cVar6 = iVar21
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(xStack_310)
                            goto LAB_00e4450d
                        end
                        iVar21 = me:IsPerformingScriptTask()
                        cVar6 = iVar21
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        resources:DestroyMovie(xStack_310)
                        goto LAB_00e4450d
                    end
                end
                ::LAB_00e437b3::
                quest:FixMovieSequenceCamera(nil --[[missing]])
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                resources:DestroyMovie(xStack_310)
            end
            uVar3 = u_stk_374
            u_stk_374 = u_stk_374 | 0x4000
            cVar6 = me:MsgIsHitByHero()
            if not cVar6 then
                u_stk_374 = uVar3 | 0xc000
                cVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if cVar6 then
                    u_stk_374 = u_stk_374 | 0x10000
                    cVar6 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                    if not cVar6 then goto LAB_00e43893 end
                end
                bVar5 = false
            else
                goto LAB_00e43893
            end
            goto FLOW_past_lab_00e43893
            ::LAB_00e43893::
            bVar5 = true
            ::FLOW_past_lab_00e43893::
            if (u_stk_374 & 0x10000) ~= 0 then
                u_stk_374 = u_stk_374 & 0xfffeffff
            end
            if (u_stk_374 >> 8) < 0 then
                u_stk_374 = u_stk_374 & 0xffff7fff
            end
            if (u_stk_374 & 0x4000) ~= 0 then
                u_stk_374 = u_stk_374 & 0xffffbfff
            end
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e4450d end
                me:ClearCommands()
                xStack_2f8_2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                quest:FixMovieSequenceCamera(nil --[[missing]])
                xStack_344 = quest:GetHero()
                xStack_344 = p0
                quest:EntitySetFacingAngleTowardsThing(nil --[[missing]], nil --[[missing]])
                quest:Pause(nil --[[missing]])
                r33 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(r33, nil --[[missing]])
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]])
                -- TODO(native): bVar5 = NScript::CV_BordelloScript::IsHeroWearingBeard__ate3e320(*(CV_BordelloScript **)(this + 0x14));
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        -- LAB_00e43ae7: (native jump target)
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        resources:DestroyMovie(xStack_2f8_2)
                        goto LAB_00e4450d
                    end
                    fVar13 = quest:GetHealth(nil --[[missing]])
                    cVar6 = 0.0 < fVar13
                    if cVar6 then
                        iVar22 = 0
                        iVar20 = 1
                        iVar19 = 0
                        iVar18 = 0
                        pcVar14 = "TEXT_QST_B13_MAGICMAN_ATTACKED_FEMALE"
                        iVar21 = quest:GetHero()
                        r34 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                        iVar21 = me:IsPerformingScriptTask()
                        cVar6 = iVar21
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- LAB_00e448e6_c30: (native jump target)
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                resources:DestroyMovie(xStack_2f8_2)
                                goto LAB_00e4450d
                            end
                            iVar21 = me:IsPerformingScriptTask()
                            cVar6 = iVar21
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(xStack_2f8_2)
                            goto LAB_00e4450d
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        -- LAB_00e448e6: (native jump target)
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        resources:DestroyMovie(xStack_2f8_2)
                        goto LAB_00e4450d
                    end
                    fVar13 = quest:GetHealth(nil --[[missing]])
                    cVar6 = 0.0 < fVar13
                    if cVar6 then
                        iVar22 = 0
                        iVar20 = 1
                        iVar19 = 0
                        iVar18 = 0
                        pcVar14 = "TEXT_QST_B13_MAGICMAN_ATTACKED_MALE"
                        iVar21 = quest:GetHero()
                        r35 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                        iVar21 = me:IsPerformingScriptTask()
                        cVar6 = iVar21
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- LAB_00e43ae7_c32: (native jump target)
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                resources:DestroyMovie(xStack_2f8_2)
                                goto LAB_00e4450d
                            end
                            iVar21 = me:IsPerformingScriptTask()
                            cVar6 = iVar21
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(xStack_2f8_2)
                            goto LAB_00e4450d
                        end
                    end
                end
                quest:FixMovieSequenceCamera(nil --[[missing]])
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                resources:DestroyMovie(xStack_2f8_2)
            end
            cVar6 = me:MsgIsPresentedWithItem()
            if cVar6 then
                if unaff_EBP == nil then
                    bVar5 = false
                    if bVar5 then
                        goto LAB_00e43c54
                    end
                else
                    -- TODO(native): iVar21 = CBasicString<char>::Compare((void *)*unaff_EBP,"OBJECT_BEER_TANKARD");
                    if iVar21 == 0 then goto LAB_00e43c54 end
                end
                goto FLOW_past_lab_00e43c54
                ::LAB_00e43c54::
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then goto LAB_00e4450d end
                me:ClearCommands()
                quest:FixMovieSequenceCamera(nil --[[missing]])
                r36 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(r36, nil --[[missing]])
                quest:Pause(nil --[[missing]])
                xStack_344 = quest:GetHero()
                xStack_344 = p0
                quest:EntitySetFacingAngleTowardsThing(nil --[[missing]], nil --[[missing]])
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]], nil --[[missing]])
                xStack_310 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                native_arg_switch_2 = quest:GetStateInt("BeersDrunk")
                repeat
                    if native_arg_switch_2 == 0 then
                        fVar13 = quest:GetHealth(nil --[[missing]])
                        cVar6 = 0.0 < fVar13
                        if cVar6 then
                            iVar22 = 0
                            iVar20 = 1
                            iVar19 = 0
                            iVar18 = 0
                            pCVar10 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                            pCVar10 = ("TEXT_QST_B13_MAGICMAN_BEER1" .. pCVar10)
                            pvVar8 = pCVar10
                            iVar21 = quest:GetHero()
                            r37 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                            iVar21 = me:IsPerformingScriptTask()
                            cVar6 = iVar21
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e44903 end
                                iVar21 = me:IsPerformingScriptTask()
                                cVar6 = iVar21
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- LAB_00e43e65: (native jump target)
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                resources:DestroyMovie(xStack_330)
                                goto LAB_00e4450d
                            end
                        end
                        break
                    else
                        if native_arg_switch_2 == 1 then
                            fVar13 = quest:GetHealth(nil --[[missing]])
                            cVar6 = 0.0 < fVar13
                            if cVar6 then
                                iVar22 = 0
                                iVar20 = 1
                                iVar19 = 0
                                iVar18 = 0
                                pCVar10 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                                pCVar10 = ("TEXT_QST_B13_MAGICMAN_BEER2" .. pCVar10)
                                pvVar8 = pCVar10
                                iVar21 = quest:GetHero()
                                r38 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                iVar21 = me:IsPerformingScriptTask()
                                cVar6 = iVar21
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e44903 end
                                    iVar21 = me:IsPerformingScriptTask()
                                    cVar6 = iVar21
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e44903 end
                            end
                            break
                        else
                            if native_arg_switch_2 == 2 then
                                fVar13 = quest:GetHealth(nil --[[missing]])
                                cVar6 = 0.0 < fVar13
                                if cVar6 then
                                    iVar22 = 0
                                    iVar20 = 1
                                    iVar19 = 0
                                    iVar18 = 0
                                    pCVar10 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                                    pCVar10 = ("TEXT_QST_B13_MAGICMAN_BEER3" .. pCVar10)
                                    pvVar8 = pCVar10
                                    iVar21 = quest:GetHero()
                                    r39 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                    iVar21 = me:IsPerformingScriptTask()
                                    cVar6 = iVar21
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e44903 end
                                        iVar21 = me:IsPerformingScriptTask()
                                        cVar6 = iVar21
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e44903 end
                                end
                                break
                            else
                                if native_arg_switch_2 == 3 then
                                    xStack_160 = resources:ScriptThing(xStack_354)
                                    fVar13 = quest:GetHealth(nil --[[missing]])
                                    cVar6 = 0.0 < fVar13
                                    if cVar6 then
                                        iVar22 = 0
                                        iVar20 = 1
                                        iVar19 = 0
                                        iVar18 = 0
                                        pCVar10 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                                        pCVar10 = ("TEXT_QST_B13_MAGICMAN_BEER4" .. pCVar10)
                                        pvVar8 = pCVar10
                                        iVar21 = quest:GetHero()
                                        r40 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                        iVar21 = me:IsPerformingScriptTask()
                                        cVar6 = iVar21
                                        while cVar6 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e44903 end
                                            iVar21 = me:IsPerformingScriptTask()
                                            cVar6 = iVar21
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e44903 end
                                    end
                                    break
                                else
                                    if native_arg_switch_2 == 4 then
                                        fVar13 = quest:GetHealth(nil --[[missing]])
                                        cVar6 = 0.0 < fVar13
                                        if cVar6 then
                                            iVar22 = 0
                                            iVar20 = 1
                                            iVar19 = 0
                                            iVar18 = 0
                                            pCVar10 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                                            pCVar10 = ("TEXT_QST_B13_MAGICMAN_BEER5" .. pCVar10)
                                            pvVar8 = pCVar10
                                            iVar21 = quest:GetHero()
                                            r41 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                            iVar21 = me:IsPerformingScriptTask()
                                            cVar6 = iVar21
                                            while cVar6 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e44903 end
                                                iVar21 = me:IsPerformingScriptTask()
                                                cVar6 = iVar21
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then goto LAB_00e44903 end
                                        end
                                        break
                                    else
                                        if native_arg_switch_2 == 5 then
                                            fVar13 = quest:GetHealth(nil --[[missing]])
                                            cVar6 = 0.0 < fVar13
                                            if cVar6 then
                                                iVar22 = 0
                                                iVar20 = 1
                                                iVar19 = 0
                                                iVar18 = 0
                                                pCVar10 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                                                pCVar10 = ("TEXT_QST_B13_MAGICMAN_BEER6" .. pCVar10)
                                                pvVar8 = pCVar10
                                                iVar21 = quest:GetHero()
                                                r42 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                                iVar21 = me:IsPerformingScriptTask()
                                                cVar6 = iVar21
                                                while cVar6 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then goto LAB_00e44903 end
                                                    iVar21 = me:IsPerformingScriptTask()
                                                    cVar6 = iVar21
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e44903 end
                                            end
                                            if not quest:GetStateBool("HeroFoundDeedsLocation") then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e44903 end
                                                -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                                                quest:SetStateBool("HeroFoundDeedsLocation", true)
                                            else
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then
                                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                    resources:DestroyMovie(xStack_330)
                                                    goto LAB_00e4450d
                                                end
                                                -- TODO(native): NScript::CV_BordelloScript::PlayCutscene(*(CV_BordelloScript **)(this + 0x14));
                                            end
                                            break
                                        else
                                            fVar13 = quest:GetHealth(nil --[[missing]])
                                            cVar6 = 0.0 < fVar13
                                            if cVar6 then
                                                iVar22 = 0
                                                iVar20 = 1
                                                iVar19 = 0
                                                iVar18 = 0
                                                pcVar14 = "TEXT_QST_B13_MAGICMAN_BEERTOOMUCH"
                                                iVar21 = quest:GetHero()
                                                r43 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                                iVar21 = me:IsPerformingScriptTask()
                                                cVar6 = iVar21
                                                while cVar6 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then
                                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                        resources:DestroyMovie(xStack_330)
                                                        goto LAB_00e4450d
                                                    end
                                                    iVar21 = me:IsPerformingScriptTask()
                                                    cVar6 = iVar21
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e44903 end
                                            end
                                            quest:GiveHeroObject("OBJECT_BEER_TANKARD", nil --[[missing]])
                                        end
                                    end
                                end
                            end
                        end
                    end
                until not (false)
                quest:SetStateInt("BeersDrunk", quest:GetStateInt("BeersDrunk") + 1)
                quest:FixMovieSequenceCamera(nil --[[missing]])
                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                resources:DestroyMovie(xStack_330)
                ::FLOW_past_lab_00e43c54::
            end
            cVar6 = quest:GetStateBool("PlayerOwned")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00e4450d end
        cVar6 = quest:GetStateBool("PlayerOwned")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        quest:RemoveThing(nil --[[missing]])
    end
    ::LAB_00e4450d::
    quest:DeregisterTimer(i_stk_378)
    quest:DeregisterTimer(i_stk_378)
    ::LAB_00e4451f::
    ::LAB_00e44528::
    resources:DestroyMovie(xStack_354)
    do return end
    ::LAB_00e44615::
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    resources:DestroyMovie(xStack_344)
    ::LAB_00e448a1::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(i_stk_378)
    goto LAB_00e4451f
    ::LAB_00e428c7::
    iVar21 = me:IsPerformingScriptTask()
    if not iVar21 then return end  -- TODO(native): goto LAB_00e428d4
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then goto LAB_00e428c7 end
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    this_00 = xStack_330
    -- LAB_00e4485d_c34: (native jump target)
    goto LAB_00e44862
    ::LAB_00e44811::
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    resources:DestroyMovie(xStack_330)
    ::LAB_00e44862::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(i_stk_378)
    goto LAB_00e4451f
    ::LAB_00e44903::
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    resources:DestroyMovie(xStack_330)
    goto LAB_00e4450d
end

function Init(quest, me)
    quest:SetThingHasInformation(me, true, false, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:SetIsThingForcePushable(me, false)
    __native_entity_state:SetStateBool("DoneToutIntro", false)
    __native_entity_state:SetStateBool("MentionedPimpHat", false)
end

function OnPersist(quest, me, context)
    local doneToutIntro = quest:GetStateBool("DoneToutIntro") or false
    doneToutIntro = quest:PersistTransferBool(context, "DoneToutIntro", doneToutIntro)
    quest:SetStateBool("DoneToutIntro", doneToutIntro)
end

function OnPredicateFail(quest, me)
end

function helper_E44A40(quest, me)
    local bVar3, bVar5, pCVar4
    bVar5 = 1
    pCVar4 = quest:GetHero()
    bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_01")
    if not bVar3 then
        bVar5 = 3
        pCVar4 = quest:GetHero()
        bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_02")
        if not bVar3 then
            bVar5 = 7
            pCVar4 = quest:GetHero()
            bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_03")
            if not bVar3 then
                bVar5 = 0xf
                pCVar4 = quest:GetHero()
                bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_MUTTON_01")
                if not bVar3 then
                    bVar5 = 0x1f
                    pCVar4 = quest:GetHero()
                    bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_LONG_01")
                    if not bVar3 then
                        bVar5 = 0x3f
                        pCVar4 = quest:GetHero()
                        bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_CHIN_01")
                        if not bVar3 then
                            bVar5 = 0x7f
                            pCVar4 = quest:GetHero()
                            bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_TRAMP_01")
                            if not bVar3 then
                                bVar5 = 0xff
                                pCVar4 = quest:GetHero()
                                bVar3 = quest:IsWearingHairstyle(pCVar4, "OBJECT_HERO_BEARD_WATSON_01")
                                if not bVar3 then goto LAB_00e44c26 end
                            end
                        end
                    end
                end
            end
        end
    end
    ::LAB_00e44c26::
    if bVar5 < 0 then
        bVar5 = bVar5 & 0x7f
    end
    if (bVar5 & 0x40) ~= 0 then
        bVar5 = bVar5 & 0xbf
    end
    if (bVar5 & 0x20) ~= 0 then
        bVar5 = bVar5 & 0xdf
    end
    if (bVar5 & 0x10) ~= 0 then
        bVar5 = bVar5 & 0xef
    end
    if (bVar5 & 8) ~= 0 then
        bVar5 = bVar5 & 0xf7
    end
    if (bVar5 & 4) ~= 0 then
        bVar5 = bVar5 & 0xfb
    end
    if (bVar5 & 2) ~= 0 then
        bVar5 = bVar5 & 0xfd
    end
    return
end

function helper_E44CC0(quest, me)
    local bVar3, bVar4, bVar5, bVar6, bVar7, bVar8, pCVar9
    bVar7 = false
    bVar6 = false
    bVar5 = false
    bVar4 = false
    bVar3 = false
    pCVar9 = quest:GetHero()
    bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHSMITH_01")
    if not bVar8 then
        bVar7 = true
        bVar6 = false
        bVar5 = false
        bVar4 = false
        bVar3 = false
        pCVar9 = quest:GetHero()
        bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHTRADER_01")
        if not bVar8 then
            bVar7 = true
            bVar6 = true
            bVar5 = false
            bVar4 = false
            bVar3 = false
            pCVar9 = quest:GetHero()
            bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHKHG_01")
            if not bVar8 then
                bVar7 = true
                bVar6 = true
                bVar5 = true
                bVar4 = false
                bVar3 = false
                pCVar9 = quest:GetHero()
                bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHSHERIFF_01")
                if not bVar8 then
                    bVar7 = true
                    bVar6 = true
                    bVar5 = true
                    bVar4 = true
                    bVar3 = false
                    pCVar9 = quest:GetHero()
                    bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHCHINESE_01")
                    if not bVar8 then
                        bVar7 = true
                        bVar6 = true
                        bVar5 = true
                        bVar4 = true
                        bVar3 = true
                        pCVar9 = quest:GetHero()
                        bVar8 = quest:IsWearingHairstyle(pCVar9, "OBJECT_HERO_TASHSMALL_01")
                        if not bVar8 then goto LAB_00e44e30 end
                    end
                end
            end
        end
    end
    ::LAB_00e44e30::
    if bVar3 then
    end
    if bVar4 then
    end
    if bVar5 then
    end
    if bVar6 then
    end
    if bVar7 then
    end
    return
end

