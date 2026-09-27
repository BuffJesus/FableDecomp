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
    local __native_condition_1, __native_condition_2, __push1, __push10, __push11, __push12, __push13, __push14, __push15, __push16, __push17, __push18, __push19, __push2, __push20, __push21, __push22, __push23, __push24, __push25, __push26, __push27, __push28, __push29, __push3, __push30, __push31, __push32, __push33, __push34, __push4, __push5, __push6, __push7, __push8, __push9, au_stk_100, au_stk_10c, au_stk_178, au_stk_190, au_stk_a0, au_stk_ac, au_stk_b8, au_stk_c4, au_stk_d0, au_stk_e8, bVar5, bVar7, cVar16, cVar6, fVar13, fVar23, iVar15, iVar18, iVar19, iVar20, iVar21, iVar22, i_stk_350, i_stk_378, native_arg_switch_1, native_arg_switch_2, p0, p0_00, pCVar10, pCVar11, pcVar14, ppuVar4, pvVar8, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r3, r30, r4, r5, r6, r7, r8, r9, this_00, this_01, timerId, uVar3, u_stk_294, u_stk_374, xStack_160, xStack_180, xStack_214, xStack_254, xStack_270, xStack_2e4, xStack_2f8, xStack_2f8_2, xStack_2fc, xStack_310, xStack_31c, xStack_32c, xStack_330, xStack_340, xStack_344, xStack_350, xStack_354, xStack_364, x_stk_104, x_stk_110, x_stk_128, x_stk_20c, x_stk_224, x_stk_230, x_stk_23c, x_stk_248, x_stk_260, x_stk_268, x_stk_38, x_stk_80, x_stk_98, x_stk_bc, x_stk_c0, x_stk_c8, x_stk_e4, x_stk_ec
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    xStack_364 = resources:NewResource()
    resources:PrepareResource(xStack_364)
    xStack_32c = xStack_364
    cVar6 = resources:TryAcquire(xStack_364, me, 4)
    while not cVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then goto LAB_00e44528 end
        cVar6 = resources:TryAcquire(xStack_364, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then goto LAB_00e44528 end
    resources:AssignResource(resources:MemberResource("seh_Boss"), xStack_364)
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
                quest:SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", 2, 0xf4240)
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
                    __push1 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, __push1, false)
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
                    __push2 = ("TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. pCVar10)
                    cVar6 = quest:TextEntryExists(__push2)
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
                    __push3 = quest:AddNewConversation(me, false, false)
                    __push4 = quest:GetHero()
                    quest:AddPersonToConversation(__push3, __push4)
                    -- TODO(native): xStack_308 = (undefined **)**(int **)(this + 4);
                    -- TODO(native): xStack_344 = (int *)(*(code *)xStack_308[0x46])();
                    ppuVar4 = xStack_364
                    xStack_350 = p0
                    pCVar10 = tostring(xStack_364)
                    __push5 = ("TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. pCVar10)
                    -- TODO(native): (*(code *)xStack_308[0x16e])(__push3,__push5,0,(this + 8),__unknown_push);
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
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                xStack_344 = quest:GetHero()
                xStack_350 = p0
                quest:EntitySetFacingAngleTowardsThing(me, xStack_344, false)
                quest:Pause(1.0)
                iVar18 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, iVar18, true)
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(me, pCVar11, -1.0, 0, -1)
                bVar5 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        x_stk_268 = resources:ScriptThing(xStack_350)
                        __push6 = x_stk_268
                        fVar13 = quest:GetHealth(__push6)
                        cVar6 = 0.0 < fVar13
                        x_stk_260 = nil
                        if cVar6 then
                            iVar15 = 0
                            iVar22 = 1
                            iVar20 = 0
                            iVar19 = 0
                            pcVar14 = "TEXT_QST_B13_MAGICMAN_PIMPING"
                            iVar21 = quest:GetHero()
                            r1 = me:Speak(iVar21, pcVar14, iVar19, (iVar20 ~= 0), (iVar22 ~= 0), (iVar15 ~= 0))
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
                                quest:PauseAllNonScriptedEntities(false)
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
                    __push7 = xStack_254
                    fVar13 = quest:GetHealth(__push7)
                    cVar6 = 0.0 < fVar13
                    xStack_254 = nil
                    if cVar6 then
                        iVar22 = 0
                        iVar20 = 1
                        iVar19 = 0
                        iVar18 = 0
                        pcVar14 = "TEXT_QST_B13_MAGICMAN_TOUTING"
                        iVar21 = quest:GetHero()
                        r2 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_340)
                goto LAB_00e41ba8
                ::FLOW_past_lab_00e41b75::
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    -- LAB_00e44677: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    this_01 = xStack_340
                    -- LAB_00e4489c_c2: (native jump target)
                    resources:DestroyMovie(this_01)
                    goto LAB_00e448a1
                end
                if not __native_entity_state:GetStateBool("MentionedPimpHat") then
                    -- TODO(native): xStack_350 = (CScriptThing *)((uint)xStack_350 | 0x20);
                    __push8 = quest:GetHero()
                    cVar6 = quest:IsWearingClothingItem(__push8, "OBJECT_HERO_HAT_PIMP")
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
                        xStack_310 = ("TEXT_QST_B13_MAGICMAN_CHATTER_0" .. pCVar10)
                        x_stk_224 = resources:ScriptThing(1)
                        __push9 = x_stk_224
                        fVar13 = quest:GetHealth(__push9)
                        cVar6 = 0.0 < fVar13
                        x_stk_224 = nil
                        if cVar6 then
                            iVar22 = 0
                            iVar20 = 1
                            iVar19 = 0
                            iVar18 = 0
                            pvVar8 = xStack_310
                            iVar21 = quest:GetHero()
                            r3 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                            iVar21 = me:IsPerformingScriptTask()
                            cVar6 = iVar21
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = xStack_340
                                    goto LAB_00e44862
                                end
                                iVar21 = me:IsPerformingScriptTask()
                                cVar6 = iVar21
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                this_01 = xStack_340
                                resources:DestroyMovie(this_01)
                                goto LAB_00e448a1
                            end
                        end
                        -- TODO(native): xStack_364 = xStack_364 + 1;
                        goto LAB_00e41b75
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    this_01 = xStack_340
                    resources:DestroyMovie(this_01)
                    goto LAB_00e448a1
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    x_stk_23c = resources:ScriptThing(xStack_364)
                    __push10 = x_stk_23c
                    fVar13 = quest:GetHealth(__push10)
                    cVar6 = 0.0 < fVar13
                    x_stk_23c = nil
                    if cVar6 then
                        iVar22 = 0
                        iVar20 = 1
                        iVar19 = 0
                        iVar18 = 0
                        pcVar14 = "TEXT_QST_B13_MAGICMAN_PIMP_HAT"
                        iVar21 = quest:GetHero()
                        r4 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                    cVar6 = me:MsgIsHitByHeroSpecialAbility(0xe)
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
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                xStack_344 = quest:GetHero()
                xStack_350 = p0
                quest:EntitySetFacingAngleTowardsThing(me, xStack_344, false)
                quest:Pause(1.0)
                iVar18 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, iVar18, true)
                alive = quest:NewScriptFrame(me)
                iVar21 = -1
                quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0, -1)
                bVar5 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        x_stk_260 = resources:ScriptThing(xStack_350)
                        __push11 = x_stk_260
                        fVar13 = quest:GetHealth(__push11)
                        cVar6 = 0.0 < fVar13
                        x_stk_20c = nil
                        x_stk_260 = 0
                        if not cVar6 then
                            goto LAB_00e42003
                        end
                        goto FLOW_past_lab_00e42003
                        ::LAB_00e42003::
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_2e4)
                        goto LAB_00e4203f
                        ::FLOW_past_lab_00e42003::
                        iVar15 = 0
                        iVar22 = 1
                        iVar20 = 0
                        iVar19 = 0
                        pcVar14 = "TEXT_QST_B13_MAGICMAN_ATTACKED_FEMALE"
                        iVar21 = quest:GetHero()
                        r5 = me:Speak(iVar21, pcVar14, iVar19, (iVar20 ~= 0), (iVar22 ~= 0), (iVar15 ~= 0))
                        iVar21 = me:IsPerformingScriptTask()
                        cVar6 = iVar21
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_2e4)
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
                        quest:PauseAllNonScriptedEntities(false)
                        -- LAB_00e4489c: (native jump target)
                        resources:DestroyMovie(this_01)
                        goto LAB_00e448a1
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        xStack_270 = resources:ScriptThing(xStack_350)
                        __push12 = xStack_270
                        fVar13 = quest:GetHealth(__push12)
                        cVar6 = 0.0 < fVar13
                        x_stk_230 = nil
                        if cVar6 then
                            iVar15 = 0
                            iVar22 = 1
                            iVar20 = 0
                            iVar19 = 0
                            pcVar14 = "TEXT_QST_B13_MAGICMAN_ATTACKED_MALE"
                            iVar18 = quest:GetHero()
                            r6 = me:Speak(iVar18, pcVar14, iVar19, (iVar20 ~= 0), (iVar22 ~= 0), (iVar15 ~= 0))
                            iVar18 = me:IsPerformingScriptTask()
                            cVar6 = iVar18
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_2e4)
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
                                this_01 = xStack_2f8
                                resources:DestroyMovie(this_01)
                                goto LAB_00e448a1
                            end
                        end
                        -- LAB_00e42003_c7: (native jump target)
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_2e4)
                        goto LAB_00e4203f
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_2f8)
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
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                xStack_344 = quest:GetHero()
                xStack_350 = p0
                quest:EntitySetFacingAngleTowardsThing(me, xStack_344, false)
                quest:Pause(1.0)
                __push13 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, __push13, true)
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(me, nil --[[missing]], -1.0, 0, -1)
                native_arg_switch_1 = quest:GetStateInt("BeersDrunk")
                repeat
                    if native_arg_switch_1 == 0 then
                        au_stk_100 = resources:ScriptThing(xStack_350)
                        pCVar11 = au_stk_100
                        fVar13 = quest:GetHealth(pCVar11)
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
                            r7 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                quest:PauseAllNonScriptedEntities(0x126008c)
                                this_01 = xStack_330
                                -- LAB_00e4489c_c8: (native jump target)
                                resources:DestroyMovie(this_01)
                                goto LAB_00e448a1
                            end
                        end
                        break
                    else
                        if native_arg_switch_1 == 1 then
                            xStack_180 = resources:ScriptThing(xStack_350)
                            pCVar11 = xStack_180
                            fVar13 = quest:GetHealth(pCVar11)
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
                                r8 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                    quest:PauseAllNonScriptedEntities(0x126008c)
                                    this_01 = xStack_330
                                    resources:DestroyMovie(this_01)
                                    goto LAB_00e448a1
                                end
                            end
                            break
                        else
                            if native_arg_switch_1 == 2 then
                                au_stk_e8 = resources:ScriptThing(xStack_350)
                                pCVar11 = au_stk_e8
                                fVar13 = quest:GetHealth(pCVar11)
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
                                    r9 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                        quest:PauseAllNonScriptedEntities(0x126008c)
                                        this_01 = xStack_330
                                        resources:DestroyMovie(this_01)
                                        goto LAB_00e448a1
                                    end
                                end
                                break
                            else
                                if native_arg_switch_1 == 3 then
                                    au_stk_d0 = resources:ScriptThing(xStack_350)
                                    pCVar11 = au_stk_d0
                                    fVar13 = quest:GetHealth(pCVar11)
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
                                        r10 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                            quest:PauseAllNonScriptedEntities(0x126008c)
                                            this_01 = xStack_330
                                            resources:DestroyMovie(this_01)
                                            goto LAB_00e448a1
                                        end
                                    end
                                    break
                                else
                                    if native_arg_switch_1 == 4 then
                                        au_stk_b8 = resources:ScriptThing(xStack_354)
                                        pCVar11 = au_stk_b8
                                        fVar13 = quest:GetHealth(pCVar11)
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
                                            r11 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                                quest:PauseAllNonScriptedEntities(0x126008c)
                                                this_01 = xStack_330
                                                resources:DestroyMovie(this_01)
                                                goto LAB_00e448a1
                                            end
                                        end
                                        break
                                    else
                                        if native_arg_switch_1 == 5 then
                                            au_stk_a0 = resources:ScriptThing(xStack_354)
                                            pCVar11 = au_stk_a0
                                            fVar13 = quest:GetHealth(pCVar11)
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
                                                    quest:PauseAllNonScriptedEntities(0x126008c)
                                                    this_01 = xStack_330
                                                    resources:DestroyMovie(this_01)
                                                    goto LAB_00e448a1
                                                end
                                            end
                                            if not quest:GetStateBool("HeroFoundDeedsLocation") then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then
                                                    quest:PauseAllNonScriptedEntities(0x126008c)
                                                    this_01 = xStack_330
                                                    resources:DestroyMovie(this_01)
                                                    goto LAB_00e448a1
                                                end
                                                require("V_Bordello.native_quest_helpers").helper_E3E720(quest, me, "CS_BORDELLO_MAGICIANDRUNK", false)
                                                quest:SetStateBool("HeroFoundDeedsLocation", true)
                                                quest:SetStateBool("HeroPartying", true)
                                            else
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    resources:DestroyMovie(xStack_330)
                                                    goto LAB_00e44862
                                                end
                                                require("V_Bordello.native_quest_helpers").helper_E3E720(quest, me, "CS_BORDELLO_MAGICIANDRUNK_QUICKIE", false)
                                                quest:SetStateBool("HeroPartying", true)
                                            end
                                            break
                                        else
                                            u_stk_294 = resources:ScriptThing(xStack_354)
                                            __push14 = u_stk_294
                                            fVar13 = quest:GetHealth(__push14)
                                            cVar6 = 0.0 < fVar13
                                            x_stk_248 = nil
                                            u_stk_294 = 0
                                            if cVar6 then
                                                iVar22 = 0
                                                iVar20 = 1
                                                iVar19 = 0
                                                iVar18 = 0
                                                pcVar14 = "TEXT_QST_B13_MAGICMAN_BEERTOOMUCH"
                                                iVar21 = quest:GetHero()
                                                r13 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                                iVar21 = me:IsPerformingScriptTask()
                                                if iVar21 then
                                                    -- LAB_00e428b0: (native jump target)
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if not bVar5 then goto LAB_00e428c7 end
                                                    quest:PauseAllNonScriptedEntities(0x126008c)
                                                    -- LAB_00e4485d: (native jump target)
                                                    goto LAB_00e44862
                                                end
                                                -- LAB_00e428d4: (native jump target)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then
                                                    quest:PauseAllNonScriptedEntities(0x126008c)
                                                    this_01 = xStack_330
                                                    resources:DestroyMovie(this_01)
                                                    goto LAB_00e448a1
                                                end
                                            end
                                            quest:GiveHeroObject("OBJECT_BEER_TANKARD", u_stk_294)
                                        end
                                    end
                                end
                            end
                        end
                    end
                until not (false)
                quest:SetStateInt("BeersDrunk", quest:GetStateInt("BeersDrunk") + 1)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_31c)
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
                quest:SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", 2, 0xf4240)
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
                    __push15 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, __push15, false)
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
                    __push16 = ("TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. pCVar10)
                    cVar6 = quest:TextEntryExists(__push16)
                    if not cVar6 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e4450d end
                        i_stk_350 = 1
                    end
                    __push17 = quest:AddNewConversation(me, false, false)
                    __push18 = quest:GetHero()
                    quest:AddPersonToConversation(__push17, __push18)
                    -- TODO(native): xStack_2bc = (undefined **)**(int **)(this + 4);
                    -- TODO(native): xStack_344 = (int *)(*(code *)xStack_2bc[0x46])();
                    ppuVar4 = xStack_364
                    xStack_354 = p0
                    pCVar10 = tostring(xStack_364)
                    __push19 = ("TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. pCVar10)
                    -- TODO(native): (*(code *)xStack_2bc[0x16e])(__push17,__push19,0,(this + 8),__unknown_push);
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
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                xStack_344 = quest:GetHero()
                xStack_354 = p0
                quest:EntitySetFacingAngleTowardsThing(me, xStack_344, false)
                quest:Pause(1.0)
                __push20 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, __push20, true)
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(me, pCVar11, -1.0, 0, -1)
                bVar5 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        bVar5 = require("V_Bordello.native_quest_helpers").helper_E44A40(quest, me)
                        if bVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                x_stk_128 = resources:ScriptThing(xStack_354)
                                __push21 = x_stk_128
                                fVar13 = quest:GetHealth(__push21)
                                cVar6 = 0.0 < fVar13
                                if cVar6 then
                                    iVar22 = 0
                                    iVar20 = 1
                                    iVar19 = 0
                                    iVar18 = 0
                                    pcVar14 = "TEXT_QST_B13_MAGICMAN_BEARD_LADY"
                                    iVar21 = quest:GetHero()
                                    r14 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_330)
                                        goto LAB_00e4450d
                                    end
                                end
                                goto LAB_00e437b3
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_330)
                            goto LAB_00e4450d
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if not bVar5 then
                            bVar5 = require("V_Bordello.native_quest_helpers").helper_E44CC0(quest, me)
                            if bVar5 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if not bVar5 then
                                    x_stk_38 = resources:ScriptThing(xStack_344)
                                    __push22 = x_stk_38
                                    fVar13 = quest:GetHealth(__push22)
                                    cVar6 = 0.0 < fVar13
                                    if cVar6 then
                                        iVar22 = 0
                                        iVar20 = 1
                                        iVar19 = 0
                                        iVar18 = 0
                                        pcVar14 = "TEXT_QST_B13_MAGICMAN_BEARD_LADY_MOUSTACHE"
                                        iVar21 = quest:GetHero()
                                        r15 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(xStack_330)
                                            goto LAB_00e4450d
                                        end
                                    end
                                    goto LAB_00e437b3
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_330)
                                goto LAB_00e4450d
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                x_stk_110 = resources:ScriptThing(xStack_354)
                                __push23 = x_stk_110
                                fVar13 = quest:GetHealth(__push23)
                                cVar6 = 0.0 < fVar13
                                if cVar6 then
                                    r16 = me:Speak(me, "TEXT_QST_B13_MAGICMAN_LOVELY_SKIN", 0, false, true, false)
                                    iVar21 = me:IsPerformingScriptTask()
                                    cVar6 = iVar21
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(xStack_330)
                                            goto LAB_00e4450d
                                        end
                                        iVar21 = me:IsPerformingScriptTask()
                                        cVar6 = iVar21
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e448cc end
                                end
                                __push24 = quest:GetHero()
                                cVar6 = quest:IsWearingClothingItem(__push24, "OBJECT_HERO_HAT_WHOREWIG")
                                if cVar6 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if not bVar5 then
                                        x_stk_ec = resources:ScriptThing(xStack_344)
                                        __push25 = x_stk_ec
                                        fVar13 = quest:GetHealth(__push25)
                                        cVar6 = 0.0 < fVar13
                                        if cVar6 then
                                            iVar22 = 0
                                            iVar20 = 1
                                            iVar19 = 0
                                            iVar18 = 0
                                            pcVar14 = "TEXT_QST_B13_MAGICMAN_LOVELY_HAIR"
                                            iVar21 = quest:GetHero()
                                            r17 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyMovie(xStack_330)
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
                                                x_stk_e4 = resources:ScriptThing(xStack_330)
                                                __push26 = x_stk_e4
                                                fVar13 = quest:GetHealth(__push26)
                                                cVar6 = 0.0 < fVar13
                                                if cVar6 then
                                                    iVar22 = 0
                                                    iVar20 = 1
                                                    iVar19 = 0
                                                    iVar18 = 0
                                                    pcVar14 = "TEXT_QST_B13_MAGICMAN_HORNY"
                                                    iVar21 = quest:GetHero()
                                                    r18 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                                    iVar21 = me:IsPerformingScriptTask()
                                                    cVar6 = iVar21
                                                    while cVar6 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar5 = not alive
                                                        if bVar5 then
                                                            quest:PauseAllNonScriptedEntities(false)
                                                            resources:DestroyMovie(xStack_330)
                                                            goto LAB_00e4450d
                                                        end
                                                        iVar21 = me:IsPerformingScriptTask()
                                                        cVar6 = iVar21
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then goto LAB_00e448cc end
                                                end
                                                quest:GiveHeroGold(0x3e8)
                                                __push27 = quest:GetNumberOfTimesHeroHasHadSex()
                                                quest:SetNumberOfTimesHeroHasHadSex(__push27)
                                                quest:SetCutsceneSkippable(false)
                                                if not quest:GetStateBool("HeroFoundDeedsLocation") then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then goto LAB_00e448cc end
                                                    require("V_Bordello.native_quest_helpers").helper_E3E720(quest, me, "CS_BORDELLO_MAGICIANSEX", true)
                                                    quest:SetStateBool("HeroFoundDeedsLocation", true)
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        resources:DestroyMovie(xStack_330)
                                                        goto LAB_00e4450d
                                                    end
                                                    require("V_Bordello.native_quest_helpers").helper_E3E720(quest, me, "CS_BORDELLO_MAGICIANSEX_QUICKIE", true)
                                                end
                                                quest:SetCutsceneSkippable(true)
                                                quest:SetStateBool("HeroPartying", true)
                                                quest:SetStateBool("HeroFoundDeedsLocation", true)
                                                quest:SetHeroAsHavingHadGaySex(true)
                                            else
                                                if bVar5 then
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    resources:DestroyMovie(xStack_330)
                                                    goto LAB_00e4450d
                                                end
                                                x_stk_c0 = resources:ScriptThing(xStack_344)
                                                __push28 = x_stk_c0
                                                fVar13 = quest:GetHealth(__push28)
                                                cVar6 = 0.0 < fVar13
                                                if cVar6 then
                                                    iVar22 = 0
                                                    iVar20 = 1
                                                    iVar19 = 0
                                                    iVar18 = 0
                                                    pcVar14 = "TEXT_QST_B13_MAGICMAN_REJECTED"
                                                    iVar21 = quest:GetHero()
                                                    r19 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        resources:DestroyMovie(xStack_330)
                                                        goto LAB_00e4450d
                                                    end
                                                end
                                            end
                                            goto LAB_00e437b3
                                        end
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_330)
                                    goto LAB_00e4450d
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if not bVar5 then
                                    x_stk_bc = resources:ScriptThing(xStack_330)
                                    __push29 = x_stk_bc
                                    fVar13 = quest:GetHealth(__push29)
                                    cVar6 = 0.0 < fVar13
                                    if cVar6 then
                                        iVar22 = 0
                                        iVar20 = 1
                                        iVar19 = 0
                                        iVar18 = 0
                                        pcVar14 = "TEXT_QST_B13_MAGICMAN_BAD_HAIR"
                                        iVar21 = quest:GetHero()
                                        r20 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                        iVar21 = me:IsPerformingScriptTask()
                                        cVar6 = iVar21
                                        while cVar6 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar5 = not alive
                                            if bVar5 then
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyMovie(xStack_330)
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
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_330)
                    goto LAB_00e4450d
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    -- LAB_00e43023: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_330)
                    goto LAB_00e4450d
                end
                x_stk_98 = resources:ScriptThing(xStack_344)
                __push30 = x_stk_98
                fVar13 = quest:GetHealth(__push30)
                cVar6 = 0.0 < fVar13
                if cVar6 then
                    iVar22 = 0
                    iVar20 = 1
                    iVar19 = 0
                    iVar18 = 0
                    pcVar14 = "TEXT_QST_B13_MAGICMAN_TOUTING"
                    iVar21 = quest:GetHero()
                    r21 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                    iVar21 = me:IsPerformingScriptTask()
                    cVar6 = iVar21
                    while cVar6 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_330)
                            goto LAB_00e4450d
                        end
                        iVar21 = me:IsPerformingScriptTask()
                        cVar6 = iVar21
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_330)
                        goto LAB_00e4450d
                    end
                end
                ::LAB_00e437b3::
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_344)
            end
            uVar3 = u_stk_374
            u_stk_374 = u_stk_374 | 0x4000
            cVar6 = me:MsgIsHitByHero()
            if not cVar6 then
                u_stk_374 = uVar3 | 0xc000
                cVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if cVar6 then
                    u_stk_374 = u_stk_374 | 0x10000
                    cVar6 = me:MsgIsHitByHeroSpecialAbility(0xe)
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
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                xStack_344 = quest:GetHero()
                xStack_344 = p0
                quest:EntitySetFacingAngleTowardsThing(xStack_344, nil --[[missing]], __unknown_push)
                quest:Pause(1.0)
                __push31 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(__push31, nil --[[missing]], __unknown_push)
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], __unknown_push, -1.0, 0)
                bVar5 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        -- LAB_00e43ae7: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_2e4)
                        goto LAB_00e4450d
                    end
                    x_stk_80 = resources:ScriptThing(xStack_344)
                    __push32 = x_stk_80
                    fVar13 = quest:GetHealth(__push32)
                    cVar6 = 0.0 < fVar13
                    if cVar6 then
                        iVar22 = 0
                        iVar20 = 1
                        iVar19 = 0
                        iVar18 = 0
                        pcVar14 = "TEXT_QST_B13_MAGICMAN_ATTACKED_FEMALE"
                        iVar21 = quest:GetHero()
                        r22 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                        iVar21 = me:IsPerformingScriptTask()
                        cVar6 = iVar21
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- LAB_00e448e6_c30: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_2e4)
                                goto LAB_00e4450d
                            end
                            iVar21 = me:IsPerformingScriptTask()
                            cVar6 = iVar21
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_2e4)
                            goto LAB_00e4450d
                        end
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        -- LAB_00e448e6: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_2e4)
                        goto LAB_00e4450d
                    end
                    x_stk_c8 = resources:ScriptThing(xStack_354)
                    __push33 = x_stk_c8
                    fVar13 = quest:GetHealth(__push33)
                    cVar6 = 0.0 < fVar13
                    if cVar6 then
                        iVar22 = 0
                        iVar20 = 1
                        iVar19 = 0
                        iVar18 = 0
                        pcVar14 = "TEXT_QST_B13_MAGICMAN_ATTACKED_MALE"
                        iVar21 = quest:GetHero()
                        r23 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                        iVar21 = me:IsPerformingScriptTask()
                        cVar6 = iVar21
                        while cVar6 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                -- LAB_00e43ae7_c32: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_2e4)
                                goto LAB_00e4450d
                            end
                            iVar21 = me:IsPerformingScriptTask()
                            cVar6 = iVar21
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_2e4)
                            goto LAB_00e4450d
                        end
                    end
                end
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
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
                quest:FixMovieSequenceCamera(true)
                __push34 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(__push34, nil --[[missing]], __unknown_push)
                quest:Pause(1.0)
                xStack_344 = quest:GetHero()
                xStack_350 = p0
                quest:EntitySetFacingAngleTowardsThing(xStack_344, nil --[[missing]], __unknown_push)
                alive = quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], __unknown_push, -1.0, 0)
                xStack_310 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                native_arg_switch_2 = quest:GetStateInt("BeersDrunk")
                repeat
                    if native_arg_switch_2 == 0 then
                        au_stk_ac = resources:ScriptThing(xStack_344)
                        pCVar11 = au_stk_ac
                        fVar13 = quest:GetHealth(pCVar11)
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
                            r24 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_310)
                                goto LAB_00e4450d
                            end
                        end
                        break
                    else
                        if native_arg_switch_2 == 1 then
                            au_stk_10c = resources:ScriptThing(xStack_344)
                            pCVar11 = au_stk_10c
                            fVar13 = quest:GetHealth(pCVar11)
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
                                r25 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                au_stk_c4 = resources:ScriptThing(xStack_344)
                                pCVar11 = au_stk_c4
                                fVar13 = quest:GetHealth(pCVar11)
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
                                    r26 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                    pCVar11 = xStack_160
                                    fVar13 = quest:GetHealth(pCVar11)
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
                                        r27 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                        au_stk_190 = resources:ScriptThing(xStack_354)
                                        pCVar11 = au_stk_190
                                        fVar13 = quest:GetHealth(pCVar11)
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
                                            r28 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                            au_stk_178 = resources:ScriptThing(xStack_354)
                                            pCVar11 = au_stk_178
                                            fVar13 = quest:GetHealth(pCVar11)
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
                                                r29 = me:Speak(iVar21, pvVar8, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
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
                                                require("V_Bordello.native_quest_helpers").helper_E3E720(quest, me, "CS_BORDELLO_MAGICIANDRUNK", false)
                                                quest:SetStateBool("HeroFoundDeedsLocation", true)
                                            else
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then
                                                    quest:PauseAllNonScriptedEntities((fVar23 ~= 0))
                                                    resources:DestroyMovie(xStack_310)
                                                    goto LAB_00e4450d
                                                end
                                                require("V_Bordello.native_quest_helpers").helper_E3E720(quest, me, "CS_BORDELLO_MAGICIANDRUNK_QUICKIE", false)
                                            end
                                            break
                                        else
                                            x_stk_104 = resources:ScriptThing(xStack_354)
                                            pCVar11 = x_stk_104
                                            fVar13 = quest:GetHealth(pCVar11)
                                            cVar6 = 0.0 < fVar13
                                            if cVar6 then
                                                iVar22 = 0
                                                iVar20 = 1
                                                iVar19 = 0
                                                iVar18 = 0
                                                pcVar14 = "TEXT_QST_B13_MAGICMAN_BEERTOOMUCH"
                                                iVar21 = quest:GetHero()
                                                r30 = me:Speak(iVar21, pcVar14, iVar18, (iVar19 ~= 0), (iVar20 ~= 0), (iVar22 ~= 0))
                                                iVar21 = me:IsPerformingScriptTask()
                                                cVar6 = iVar21
                                                while cVar6 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar5 = not alive
                                                    if bVar5 then
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        resources:DestroyMovie(xStack_2fc)
                                                        goto LAB_00e4450d
                                                    end
                                                    iVar21 = me:IsPerformingScriptTask()
                                                    cVar6 = iVar21
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar5 = not alive
                                                if bVar5 then goto LAB_00e44903 end
                                            end
                                            quest:GiveHeroObject("OBJECT_BEER_TANKARD", x_stk_260)
                                        end
                                    end
                                end
                            end
                        end
                    end
                until not (false)
                quest:SetStateInt("BeersDrunk", quest:GetStateInt("BeersDrunk") + 1)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_2fc)
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
        quest:RemoveThing(nil --[[missing]], __unknown_push, false)
    end
    ::LAB_00e4450d::
    quest:DeregisterTimer(i_stk_378)
    quest:DeregisterTimer(i_stk_378)
    ::LAB_00e4451f::
    ::LAB_00e44528::
    resources:ReleaseResource(xStack_364)
    do return end
    ::LAB_00e44615::
    quest:PauseAllNonScriptedEntities(0x126008c)
    resources:DestroyMovie(xStack_330)
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
    quest:PauseAllNonScriptedEntities(0x126008c)
    -- LAB_00e4485d_c34: (native jump target)
    goto LAB_00e44862
    ::LAB_00e44811::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_330)
    ::LAB_00e44862::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(i_stk_378)
    goto LAB_00e4451f
    ::LAB_00e44903::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_310)
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

