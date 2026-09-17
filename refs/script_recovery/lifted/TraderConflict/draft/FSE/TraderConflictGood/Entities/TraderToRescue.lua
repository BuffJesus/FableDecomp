-- Generated native draft: TraderToRescue. Review coverage report before use.
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
    local CVar13, __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, __native_condition_5, bVar2, cVar3, c_stk_161, dist, fVar16, iVar11, iVar18, iVar19, iVar20, iVar21, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, native_arg_sequence_4, p0, p0_00, p0_00_b3, p1, pCVar10, pCVar4, pCVar5, pcVar17, piVar9, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r3, r30, r4, r5, r6, r7, r8, r9, uVar12, uVar14, uVar6, xStack_148, xStack_150, xStack_154, xStack_18, xStack_28, xStack_b8, x_stk_164, x_stk_16c
    local alive = true
    x_stk_164 = 0x0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    cVar3 = quest:GetStateBool("IntroDone")
    while not cVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        cVar3 = quest:GetStateBool("IntroDone")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    xStack_148 = resources:NewResource()
    -- TODO(native): xStack_148[0] = 0;
    bVar2 = false
    if bVar2 ~= 0 then
    end
    iVar21 = 4
    p1 = xStack_148
    cVar3 = me:AcquireControl(4)
    while not cVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then goto LAB_00e005f9 end
        cVar3 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        x_stk_16c = quest:RegisterTimer()
        x_stk_164 = quest:RegisterTimer()
        r1 = quest:GetNearestWithScriptName(nil --[[missing]], "TC_BanditHostageKeeper")
        c_stk_161 = 0
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        repeat
            if bVar2 then
                quest:DeregisterTimer(x_stk_164)
                quest:DeregisterTimer(unaff_EBX)
                resources:ReleaseResource(xStack_148)
                return
            end
            bVar2 = me:IsTalkedToByHero()
            if bVar2 then
                bVar2 = true
                goto FLOW_after_lab_00dfe32c
            end
            uVar12 = unaff_EBP | 3
            pCVar4 = me:GetDataString()
            -- TODO(native): if *pCVar4 == nil then
            if false then
                bVar2 = false
                if bVar2 then
                    -- LAB_00dfe2e9: (native jump target)
                    if quest:GetStateBool("OpenedCage") then
                        bVar2 = true
                        goto FLOW_after_lab_00dfe32c
                    end
                end
            else
                iVar11 = ((pCVar4 == "TRADERB") and 0 or 1)
                if iVar11 == 0 then return end  -- TODO(native): goto LAB_00dfe2e9
            end
            bVar2 = me:MsgExpressionPerformedTo()
            if bVar2 then
                if "" == nil then
                    bVar2 = false
                    if not bVar2 then
                        bVar2 = false
                        goto FLOW_after_lab_00dfe32c
                    end
                else
                    -- TODO(native): iVar11 = CBasicString<char>::Compare(*(void **)xStack_168,"EXPRESSION_FOLLOW");
                    if iVar11 ~= 0 then
                        bVar2 = false
                        goto FLOW_after_lab_00dfe32c
                    end
                end
                -- LAB_00dfe32c: (native jump target)
                bVar2 = true
            else
                -- LAB_00dfe4df: (native jump target)
                bVar2 = false
            end
            ::FLOW_after_lab_00dfe32c::
            if (uVar12 & 2) ~= 0 then
                uVar12 = uVar12 & 0xfffffffd
            end
            if (uVar12 & 1) ~= 0 then
                uVar12 = uVar12 & 0xfffffffe
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e005d5 end
                xStack_b8 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities((iVar21 ~= 0))
                xStack_148 = resources:ScriptThing(xStack_14c_3)
                fVar16 = quest:GetHealth(r1)
                p0_00_b3 = not (fVar16 <= 0.0)
                if not p0_00_b3 then return end  -- TODO(native): goto LAB_00dfebc5
                iVar19 = 0
                iVar18 = 1
                iVar11 = 0
                pcVar17 = "_INTRO"
                pCVar4 = me:GetDataString()
                -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_10c,"TEXT_QST_B11_",pCVar4)
                pCVar4 = nil --[[unresolved native value]]
                pCVar4 = (pCVar4 .. pcVar17)
                -- TODO(native): pvVar8 = CCharString::operator_char_const_((CCharString *)pCVar4);
                iVar21 = quest:GetHero()
                r2 = me:Speak(iVar21, pcVar17, pvVar8, (iVar11 ~= 0), false, (iVar18 ~= 0))
                iVar21 = me:IsPerformingScriptTask()
                cVar3 = iVar21
                goto LAB_00dfeb57
            end
            iVar11 = quest:GetTimer(x_stk_164)
            native_arg_sequence_1 = false
            if iVar11 == 0 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                if c_stk_161 == 0 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if native_arg_sequence_1 then
                    -- TODO(native): cVar3 = (**(xStack_134 + 0x12c))()
                    cVar3 = nil --[[unresolved native value]]
                    if cVar3 == 0 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
            end
            if native_arg_sequence_1 then
                dist = 15.0
                pCVar5 = quest:GetHero()
                bVar2 = quest:IsDistanceBetweenThingsUnder(me, pCVar5, dist)
                if bVar2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e005d5 end
                    r3 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(r3, pCVar5)
                    native_arg_sequence_2 = false
                    if c_stk_161 == 0 then
                        native_arg_sequence_2 = true
                    else
                        native_arg_sequence_2 = false
                    end
                    if native_arg_sequence_2 then
                        -- TODO(native): cVar3 = (**(xStack_134 + 0x12c))()
                        cVar3 = nil --[[unresolved native value]]
                        if cVar3 == 0 then
                            native_arg_sequence_2 = true
                        else
                            native_arg_sequence_2 = false
                        end
                    end
                    if native_arg_sequence_2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e005d5 end
                        uVar6 = quest:AddNewConversation(nil --[[missing]], (dist ~= 0), nil --[[missing]])
                        r4 = quest:GetHero()
                        quest:AddPersonToConversation(nil --[[missing]], r4)
                        r5 = quest:GetHero()
                        pcVar17 = "_KEEPERISDEAD"
                        pCVar4 = me:GetDataString()
                        pCVar4 = CCharString_OperatorPlus_API(xStack_fc,"TEXT_QST_B11_",pCVar4)
                        pCVar4 = (pCVar4 .. pcVar17)
                        quest:AddLineToConversation(uVar6, pCVar4, r5, nil --[[missing]])
                        c_stk_161 = 1
                    else
                        if iVar11 % 5 == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005d5 end
                            uVar6 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                            r6 = quest:GetHero()
                            quest:AddPersonToConversation(nil --[[missing]], r6)
                            r7 = quest:GetHero()
                            pcVar17 = "_OVERHERE"
                            pCVar4 = me:GetDataString()
                            -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_f0,"TEXT_QST_B11_",pCVar4)
                            pCVar4 = nil --[[unresolved native value]]
                            pCVar4 = (pCVar4 .. pcVar17)
                            quest:AddLineToConversation(uVar6, pCVar4, r7, nil --[[missing]])
                        end
                    end
                    quest:SetTimer(x_stk_16c, 10)
                    iVar11 = quest:GetTimer(unaff_EBX)
                    quest:SetTimer(unaff_EBX, iVar11 + 5)
                end
            end
            iVar11 = quest:GetTimer(unaff_EBX)
            if iVar11 == 0 then
                uVar12 = uVar12 | 4
                iVar11 = quest:GetHeroTargetedThing()
                -- TODO(native): bVar2 = (**(*(iVar11 + 0x0) + 0x138))((me))
                bVar2 = nil --[[unresolved native value]]
                if not bVar2 then
                    bVar2 = false
                    goto FLOW_after_lab_00dfe63d
                end
                -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                quest:IsPlayerHoldingLockTargetButton()
                bVar2 = true
                if cVar3 == 0 then
                    bVar2 = false
                    goto FLOW_after_lab_00dfe63d
                end
            else
                -- LAB_00dfe63d: (native jump target)
                bVar2 = false
            end
            ::FLOW_after_lab_00dfe63d::
            if (uVar12 & 4) ~= 0 then
                uVar12 = uVar12 & 0xfffffffb
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e005d5 end
                iVar11 = IsPlayerThreateningEntity(me)
                native_arg_sequence_3 = false
                if iVar11 == 0 then
                    native_arg_sequence_3 = true
                else
                    native_arg_sequence_3 = false
                end
                if not native_arg_sequence_3 then
                    if iVar11 % 3 ~= 0 then
                        native_arg_sequence_3 = true
                    else
                        native_arg_sequence_3 = false
                    end
                end
                if native_arg_sequence_3 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e005d5 end
                    uVar6 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                    r8 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r8)
                    r9 = quest:GetHero()
                    pcVar17 = "_THREATEN"
                    pCVar4 = me:GetDataString()
                    -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_ec,"TEXT_QST_B11_",pCVar4)
                    pCVar4 = nil --[[unresolved native value]]
                    pCVar4 = (pCVar4 .. pcVar17)
                    quest:AddLineToConversation(uVar6, pCVar4, r9, nil --[[missing]])
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e005d5 end
                    uVar6 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                    r10 = quest:GetHero()
                    quest:AddPersonToConversation(nil --[[missing]], r10)
                    r11 = quest:GetHero()
                    pcVar17 = "_THREATENWEAPON"
                    pCVar4 = me:GetDataString()
                    -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_dc,"TEXT_QST_B11_",pCVar4)
                    pCVar4 = nil --[[unresolved native value]]
                    pCVar4 = (pCVar4 .. pcVar17)
                    quest:AddLineToConversation(uVar6, pCVar4, r11, nil --[[missing]])
                end
                quest:SetTimer(unaff_EBX, 0x14)
                CVar13 = x_stk_170
                iVar11 = quest:GetTimer(x_stk_170)
                quest:SetTimer(iVar11 + 5, nil --[[missing]])
            end
            -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
            bVar2 = me:MsgIsHitBy("")
            if bVar2 then
                -- LAB_00dfe8b5: (native jump target)
                bVar2 = true
            else
                bVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar2 then
                    -- TODO(native): unaff_EBP = uVar12 | 0x38;
                    bVar2 = me:MsgIsHitByHeroSpecialAbility(me)
                    if not bVar2 then
                        bVar2 = true
                        goto FLOW_after_lab_00dfe8b5
                    end
                end
                bVar2 = false
            end
            ::FLOW_after_lab_00dfe8b5::
            if (unaff_EBP & 0x20) ~= 0 then
                -- TODO(native): unaff_EBP = unaff_EBP & 0xffffffdf;
            end
            if (unaff_EBP & 0x10) ~= 0 then
                -- TODO(native): unaff_EBP = unaff_EBP & 0xffffffef;
            end
            if (unaff_EBP & 8) ~= 0 then
                -- TODO(native): unaff_EBP = unaff_EBP & 0xfffffff7;
            end
            if bVar2 then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e005d5 end
                uVar6 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                r12 = quest:GetHero()
                quest:AddPersonToConversation(nil --[[missing]], r12)
                r13 = quest:GetHero()
                pcVar17 = "_ONHIT"
                pCVar4 = me:GetDataString()
                -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_10c,"TEXT_QST_B11_",pCVar4)
                pCVar4 = nil --[[unresolved native value]]
                pCVar4 = (pCVar4 .. pcVar17)
                quest:AddLineToConversation(uVar6, pCVar4, r13, nil --[[missing]])
                quest:SetTimer(unaff_EBX, 10)
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
        until false
    end
    goto LAB_00e005f9
    ::LAB_00dfeb57::
    if cVar3 == 0 then goto LAB_00dfeb80 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        quest:PauseAllNonScriptedEntities(nil --[[missing]])
        resources:DestroyMovie(xStack_b8)
        goto LAB_00e005d5
    end
    iVar21 = me:IsPerformingScriptTask()
    cVar3 = iVar21
    goto LAB_00dfeb57
    ::LAB_00dfeb80::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        quest:PauseAllNonScriptedEntities(nil --[[missing]])
        resources:DestroyMovie(xStack_b8)
    else
        -- LAB_00dfebc5: (native jump target)
        quest:PauseAllNonScriptedEntities(nil --[[missing]])
        resources:DestroyMovie(xStack_b8)
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:DeactivateQuest("Q_TraderConflictGood_Extras", nil --[[missing]])
            quest:ActivateQuest("Q_TraderConflictGood_Extras")
            quest:SetIsPushableByHero(nil --[[missing]], nil --[[missing]])
            uVar6 = quest:GetHero()
            quest:EntityFollowThing(me, uVar6, nil --[[missing]], nil --[[missing]])
            uVar6 = quest:GetHero()
            quest:SetEntityAsRegionFollowing(uVar6, nil --[[missing]], nil --[[missing]])
            quest:EntitySetOpinionReactionsEnabled(me, false)
            quest:EntitySetDeedReactionsEnabled(me, false)
            quest:EntitySetCombatEnabled(me, false)
            quest:EntitySetInFaction(me, "FACTION_TRADERS")
            quest:DisplayQuestInfo(true)
            pCVar4 = me:GetDataString()
            -- TODO(native): if *pCVar4 == nil then
            if false then
                cVar3 = false
            else
                iVar21 = ((pCVar4 == "TRADERA") and 0 or 1)
                cVar3 = not (iVar21 ~= 0)
            end
            if not cVar3 then
                pCVar4 = me:GetDataString()
                -- TODO(native): if *pCVar4 == nil then
                if false then
                    cVar3 = false
                else
                    iVar21 = ((pCVar4 == "TRADERB") and 0 or 1)
                    cVar3 = not (iVar21 ~= 0)
                end
                if cVar3 then
                    -- LAB_00dfee05: (native jump target)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e005d5 end
                    pcVar17 = "HUD_QUEST_ICON_TRADER_HAT_02"
                    uVar6 = quest:AddQuestInfoBarHealth(nil --[[missing]], nil --[[missing]], pcVar17, nil --[[missing]])
                    __native_entity_state:SetStateInt("BarIndex", uVar6)
                    goto FLOW_after_lab_00dfee1b
                end
                pCVar4 = me:GetDataString()
                -- TODO(native): if *pCVar4 == nil then
                if false then
                    cVar3 = false
                else
                    iVar21 = ((pCVar4 == "TRADERC") and 0 or 1)
                    cVar3 = not (iVar21 ~= 0)
                end
                if cVar3 then return end  -- TODO(native): goto LAB_00dfee05
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e005d5 end
                pcVar17 = "HUD_QUEST_ICON_TRADER"
                -- LAB_00dfee1b: (native jump target)
                uVar6 = quest:AddQuestInfoBarHealth(nil --[[missing]], nil --[[missing]], pcVar17, nil --[[missing]])
                __native_entity_state:SetStateInt("BarIndex", uVar6)
            end
            ::FLOW_after_lab_00dfee1b::
            bVar2 = false
            if bVar2 ~= 0 then
            end
            cVar3 = quest:IsEntityFollowingHero(nil --[[missing]])
            while not cVar3 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e005d5 end
                cVar3 = quest:IsEntityFollowingHero(nil --[[missing]])
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                piVar9 = (__native_entity_state:GetStateInt("self_0x14") + 0x54)
                -- TODO(native): *piVar9 = *piVar9 + 1;
                CVar13 = 0xa
                xStack_150 = quest:RegisterTimer()
                quest:SetTimer(xStack_150, 0x14)
                xStack_154 = quest:RegisterTimer()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                while not bVar2 do
                    cVar3 = quest:IsRegionLoaded("BanditCampEntrance")
                    c_stk_161 = not (cVar3)
                    while c_stk_161 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e005b1 end
                        cVar3 = me:IsTalkedToByHero()
                        if cVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005b1 end
                            pCVar4 = me:GetDataString()
                            -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_68,"TEXT_QST_B11_",pCVar4)
                            pCVar4 = nil --[[unresolved native value]]
                            pCVar4 = (pCVar4 .. piVar9)
                            -- TODO(native): CCharString::operator=((CCharString *)&xStack_158,pCVar4);
                            -- TODO(native): pCVar4 = GFIntToCharString_API(&xStack_50,CVar13)
                            pCVar4 = nil --[[unresolved native value]]
                            pCVar4 = (xStack_158 .. pCVar4)
                            -- TODO(native): CCharString::operator=((CCharString *)&xStack_15c,pCVar4);
                            -- TODO(native): TextEntryExists is not a ForgeFSE binding
                            quest:TextEntryExists()
                            if not cVar3 then
                                CVar13 = 0xa
                                -- TODO(native): pCVar4 = GFIntToCharString_API(&xStack_34,10)
                                pCVar4 = nil --[[unresolved native value]]
                                pCVar4 = ("" .. pCVar4)
                                -- TODO(native): CCharString::operator=((CCharString *)&xStack_15c,pCVar4);
                            end
                            -- TODO(native): CStack_12c = (CCharString)((int)CVar13 + 0xa);
                            bVar2 = false
                            if bVar2 ~= 0 then
                            end
                            cVar3 = me:AcquireControl(4)
                            while not cVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00e005b1 end
                                cVar3 = me:AcquireControl(4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005b1 end
                            xStack_18 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities((CVar13 ~= 0))
                            xStack_28 = resources:ScriptThing(xStack_154)
                            fVar16 = quest:GetHealth(nil --[[missing]])
                            p0_00_b3 = not (fVar16 <= 0.0)
                            if p0_00_b3 then
                                iVar20 = 0
                                iVar19 = 1
                                iVar18 = 0
                                iVar11 = 0
                                -- TODO(native): pvVar8 = CCharString::operator_char_const_((CCharString *)&stack0xfffffe84);
                                iVar21 = quest:GetHero()
                                r14 = me:Speak(iVar21, "", pvVar8, (iVar11 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                iVar21 = me:IsPerformingScriptTask()
                                cVar3 = iVar21
                                while cVar3 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then
                                        quest:PauseAllNonScriptedEntities(true)
                                        resources:DestroyMovie(xStack_18)
                                        goto LAB_00e005b1
                                    end
                                    iVar21 = me:IsPerformingScriptTask()
                                    cVar3 = iVar21
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(0xa)
                                    resources:DestroyMovie(xStack_28)
                                    goto LAB_00e005b1
                                end
                            end
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(xStack_18)
                        end
                        iVar21 = quest:GetTimer(unaff_EBX)
                        __native_condition_2 = iVar21 == 0
                        if __native_condition_2 then
                            iVar21 = IsPlayerThreateningEntity(me)
                            __native_condition_2 = iVar21 ~= 0
                        end
                        __native_condition_1 = __native_condition_2
                        if __native_condition_1 then
                            -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                            quest:IsPlayerHoldingLockTargetButton()
                            __native_condition_1 = cVar3
                        end
                        if __native_condition_1 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005b1 end
                            uVar6 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                            r15 = quest:GetHero()
                            quest:AddPersonToConversation(nil --[[missing]], r15)
                            r16 = quest:GetHero()
                            pCVar4 = me:GetDataString()
                            pCVar4 = CCharString_OperatorPlus_API(xStack_88,"TEXT_QST_B11_",pCVar4)
                            pCVar4 = (pCVar4 .. piVar9)
                            quest:AddLineToConversation(uVar6, pCVar4, r16, nil --[[missing]])
                            quest:SetTimer(unaff_EBX, 0x14)
                        end
                        -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                        cVar3 = me:MsgIsHitBy("")
                        if not cVar3 then
                            -- TODO(native): MsgIsHitByAnySpecialAbilityFrom is not a ForgeFSE binding
                            cVar3 = me:MsgIsHitByAnySpecialAbilityFrom("")
                            if cVar3 then
                                uVar14 = uVar14 | 0x100
                                cVar3 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                bVar2 = true
                                if cVar3 then goto LAB_00dff3f2 end
                                goto FLOW_after_lab_00dff3b5
                            end
                            ::LAB_00dff3f2::
                            bVar2 = false
                        else
                            -- LAB_00dff3b5: (native jump target)
                            uVar14 = uVar14 | 0x100
                            cVar3 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                            bVar2 = true
                            if cVar3 then
                                bVar2 = false
                                goto FLOW_after_lab_00dff3b5
                            end
                        end
                        ::FLOW_after_lab_00dff3b5::
                        if (uVar14 & 0x100) ~= 0 then
                            uVar14 = uVar14 & 0xfffffeff
                        end
                        if SUB41(uVar14,0) < 0 then
                            uVar14 = uVar14 & 0xffffff7f
                        end
                        if (uVar14 & 0x40) ~= 0 then
                            uVar14 = uVar14 & 0xffffffbf
                        end
                        if bVar2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005b1 end
                            fVar16 = quest:GetHealth(nil --[[missing]])
                            __native_condition_3 = fVar16 <= 5.0
                            if not __native_condition_3 then
                                iVar21 = quest:GetTimer(xStack_148)
                                __native_condition_3 = iVar21 ~= 0
                            end
                            if __native_condition_3 then
                                -- LAB_00dff54a: (native jump target)
                                bVar2 = false
                            else
                                cVar3 = me:MsgIsHitByHero()
                                if cVar3 then
                                    bVar2 = false
                                    goto FLOW_after_lab_00dff54a
                                end
                                cVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                                if cVar3 then
                                    uVar12 = uVar14 | 0xe00
                                    cVar3 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                    if not cVar3 then
                                        bVar2 = false
                                        goto FLOW_after_lab_00dff54a
                                    end
                                end
                                bVar2 = true
                            end
                            ::FLOW_after_lab_00dff54a::
                            if (uVar12 & 0x800) ~= 0 then
                                uVar14 = uVar12 & 0xfffff7ff
                            end
                            if (uVar14 & 0x400) ~= 0 then
                                uVar14 = uVar14 & 0xfffffbff
                            end
                            if (uVar14 & 0x200) ~= 0 then
                                uVar14 = uVar14 & 0xfffffdff
                            end
                            if bVar2 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00e005b1 end
                                piVar9 = me:GetDataString()
                                iVar21 = ((piVar9 == "TRADERA") and 0 or 1)
                                cVar3 = not (iVar21 ~= 0)
                                if not cVar3 then
                                    piVar9 = me:GetDataString()
                                    iVar21 = ((piVar9 == "TRADERB") and 0 or 1)
                                    cVar3 = not (iVar21 ~= 0)
                                    if not cVar3 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        CVar13 = xStack_148
                                        if bVar2 then goto LAB_00e005b1 end
                                        -- TODO(native): pCVar4 = GFIntToCharString_API(&xStack_84,xStack_148)
                                        pCVar4 = nil --[[unresolved native value]]
                                        -- TODO(native): CCharString_OperatorPlus_API(&xStack_34,"SND_MM_TRADER_C_SCREAM_0",pCVar4);
                                        r17 = quest:PlaySoundOnThing(nil --[[missing]], piVar9)
                                        bVar2 = CVar13 == 0x4
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar2 = not alive
                                        CVar13 = xStack_148
                                        if bVar2 then goto LAB_00e005b1 end
                                        pCVar4 = GFIntToCharString_API(xStack_30,xStack_148)
                                        -- TODO(native): CCharString_OperatorPlus_API(&xStack_7c,"SND_MM_TRADER_B_SCREAM_0",pCVar4);
                                        r18 = quest:PlaySoundOnThing(nil --[[missing]], "")
                                        bVar2 = CVar13 == 0x3
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    CVar13 = xStack_148
                                    if bVar2 then goto LAB_00e005b1 end
                                    pCVar4 = GFIntToCharString_API(xStack_44,xStack_148)
                                    -- TODO(native): CCharString_OperatorPlus_API(&xStack_74,"SND_MM_TRADER_A_SCREAM_0",pCVar4);
                                    r19 = quest:PlaySoundOnThing(nil --[[missing]], "")
                                    bVar2 = CVar13 == 0x3
                                end
                                if bVar2 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00e005b1 end
                                    CVar13 = 0x0
                                end
                                -- TODO(native): xStack_148 = (CCharString)((int)CVar13 + 1);
                                quest:SetTimer(4, CVar13)
                            else
                                fVar16 = quest:GetHealth(nil --[[missing]])
                                if fVar16 <= 5.0 then
                                    -- LAB_00dff887: (native jump target)
                                    bVar2 = false
                                else
                                    cVar3 = me:MsgIsHitByHero()
                                    if not cVar3 then
                                        cVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                                        if cVar3 then
                                            uVar12 = uVar14 | 0x7000
                                            cVar3 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                            if not cVar3 then goto LAB_00dff880 end
                                        end
                                        bVar2 = false
                                        goto FLOW_after_lab_00dff887
                                    end
                                    ::LAB_00dff880::
                                    bVar2 = true
                                end
                                ::FLOW_after_lab_00dff887::
                                if (uVar12 & 0x4000) ~= 0 then
                                    uVar14 = uVar12 & 0xffffbfff
                                end
                                if (uVar14 & 0x2000) ~= 0 then
                                    uVar14 = uVar14 & 0xffffdfff
                                end
                                if (uVar14 & 0x1000) ~= 0 then
                                    uVar14 = uVar14 & 0xffffefff
                                end
                                if bVar2 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00e005b1 end
                                    uVar6 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                    r20 = quest:GetHero()
                                    quest:AddPersonToConversation(nil --[[missing]], r20)
                                    r21 = quest:GetHero()
                                    pCVar4 = me:GetDataString()
                                    pCVar4 = CCharString_OperatorPlus_API(xStack_18,"TEXT_QST_B11_",pCVar4)
                                    pCVar4 = (pCVar4 .. piVar9)
                                    quest:AddLineToConversation(uVar6, pCVar4, r21, nil --[[missing]])
                                end
                            end
                        end
                        cVar3 = me:MsgIsHitByHero()
                        if not cVar3 then
                            cVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                            if cVar3 then
                                uVar12 = uVar14 | 0x38000
                                cVar3 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                if not cVar3 then
                                    bVar2 = true
                                    goto FLOW_after_lab_00dffa46
                                end
                            end
                            bVar2 = false
                        else
                            -- LAB_00dffa46: (native jump target)
                            bVar2 = true
                        end
                        ::FLOW_after_lab_00dffa46::
                        if (uVar12 & 0x20000) ~= 0 then
                            uVar12 = uVar12 & 0xfffdffff
                        end
                        if (uVar12 & 0x10000) ~= 0 then
                            uVar12 = uVar12 & 0xfffeffff
                        end
                        if (uVar12 & 0x8000) ~= 0 then
                            uVar12 = uVar12 & 0xffff7fff
                        end
                        native_arg_sequence_4 = false
                        if bVar2 then
                            native_arg_sequence_4 = true
                        else
                            native_arg_sequence_4 = false
                        end
                        if native_arg_sequence_4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                native_arg_sequence_4 = true
                            else
                                native_arg_sequence_4 = false
                            end
                        end
                        if native_arg_sequence_4 then goto LAB_00e005b1 end
                        cVar3 = me:MsgExpressionPerformedTo()
                        if cVar3 then
                            if xStack_15c == nil then
                                bVar2 = false
                                if bVar2 then
                                    -- LAB_00dffb06: (native jump target)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00e005b1 end
                                    piVar9 = (__native_entity_state:GetStateInt("self_0x14") + 0x54)
                                    -- TODO(native): *piVar9 = *piVar9 + -1;
                                end
                            else
                                -- TODO(native): iVar21 = CBasicString<char>::Compare(*(void **)xStack_15c,"EXPRESSION_WAIT");
                                if iVar21 == 0 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00e005b1 end
                                    piVar9 = (__native_entity_state:GetStateInt("self_0x14") + 0x54)
                                    -- TODO(native): *piVar9 = *piVar9 + -1;
                                    goto FLOW_after_lab_00dffb06
                                end
                            end
                            ::FLOW_after_lab_00dffb06::
                        end
                        cVar3 = quest:IsRegionLoaded("BanditCampEntrance")
                        c_stk_161 = not (cVar3)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then break end
                    r22 = quest:GetThingWithScriptName("TeleporterMarker")
                    -- TODO(native): CCharString::CCharString({R = 255, G = 0, B = 0, A = 255},"BanditCampEntrance",-1);
                    c_stk_161 = quest:IsRegionLoaded(pcVar17)
                    while c_stk_161 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e005ac end
                        cVar3 = me:IsTalkedToByHero()
                        if cVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005ac end
                            pCVar4 = me:GetDataString()
                            -- TODO(native): pCVar4 = CCharString_OperatorPlus_API(&xStack_64,"TEXT_QST_B11_",pCVar4)
                            pCVar4 = nil --[[unresolved native value]]
                            pCVar4 = (pCVar4 .. piVar9)
                            -- TODO(native): CCharString::operator=((CCharString *)&xStack_158,pCVar4);
                            -- TODO(native): pCVar4 = GFIntToCharString_API(&xStack_98,CVar13)
                            pCVar4 = nil --[[unresolved native value]]
                            pCVar4 = (xStack_158 .. pCVar4)
                            -- TODO(native): CCharString::operator=((CCharString *)&xStack_15c,pCVar4);
                            -- TODO(native): TextEntryExists is not a ForgeFSE binding
                            quest:TextEntryExists()
                            if not cVar3 then
                                CVar13 = 0xa
                                -- TODO(native): pCVar4 = GFIntToCharString_API(&xStack_88,10)
                                pCVar4 = nil --[[unresolved native value]]
                                pCVar4 = (xStack_15c .. pCVar4)
                                -- TODO(native): CCharString::operator=((CCharString *)&xStack_15c,pCVar4);
                            end
                            -- TODO(native): CStack_12c = (CCharString)((int)CVar13 + 0xa);
                            bVar2 = false
                            if bVar2 ~= 0 then
                            end
                            cVar3 = me:AcquireControl(4)
                            while not cVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00e005ac end
                                cVar3 = me:AcquireControl(4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005ac end
                            xStack_28 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities((CVar13 ~= 0))
                            xStack_b8 = resources:ScriptThing(xStack_148)
                            fVar16 = quest:GetHealth(r22)
                            p0_00_b3 = not (fVar16 <= 0.0)
                            if p0_00_b3 then
                                iVar20 = 0
                                iVar19 = 1
                                iVar18 = 0
                                iVar11 = 0
                                -- TODO(native): pvVar8 = CCharString::operator_char_const_((CCharString *)&stack0xfffffe84);
                                iVar21 = quest:GetHero()
                                r23 = me:Speak(iVar21, "", pvVar8, (iVar11 ~= 0), (iVar18 ~= 0), (iVar19 ~= 0))
                                iVar21 = me:IsPerformingScriptTask()
                                cVar3 = iVar21
                                while cVar3 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then
                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                        resources:DestroyMovie(xStack_18)
                                        goto LAB_00e005ac
                                    end
                                    iVar21 = me:IsPerformingScriptTask()
                                    cVar3 = iVar21
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                    resources:DestroyMovie(xStack_28)
                                    goto LAB_00e005ac
                                end
                            end
                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                            resources:DestroyMovie(xStack_28)
                        end
                        iVar21 = quest:GetTimer(unaff_EBX)
                        __native_condition_5 = iVar21 == 0
                        if __native_condition_5 then
                            iVar21 = IsPlayerThreateningEntity(me)
                            __native_condition_5 = iVar21 ~= 0
                        end
                        __native_condition_4 = __native_condition_5
                        if __native_condition_4 then
                            -- TODO(native): IsPlayerHoldingLockTargetButton is not a ForgeFSE binding
                            quest:IsPlayerHoldingLockTargetButton()
                            __native_condition_4 = cVar3
                        end
                        if __native_condition_4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005ac end
                            uVar6 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                            r24 = quest:GetHero()
                            quest:AddPersonToConversation(nil --[[missing]], r24)
                            r25 = quest:GetHero()
                            pCVar10 = me:GetDataString()
                            -- TODO(native): pCVar10 = CCharString_OperatorPlus_API(&xStack_64,"TEXT_QST_B11_",pCVar10)
                            pCVar10 = nil --[[unresolved native value]]
                            pCVar4 = (pCVar10 .. pCVar4)
                            quest:AddLineToConversation(uVar6, pCVar4, r25, nil --[[missing]])
                            quest:SetTimer(unaff_EBX, 0x14)
                        end
                        cVar3 = me:MsgIsHitByHero()
                        if not cVar3 then
                            cVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                            if cVar3 then
                                uVar14 = uVar12 | 0x1c0000
                                cVar3 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                                if not cVar3 then
                                    bVar2 = true
                                    goto FLOW_after_lab_00e0006b
                                end
                            end
                            bVar2 = false
                        else
                            -- LAB_00e0006b: (native jump target)
                            bVar2 = true
                        end
                        ::FLOW_after_lab_00e0006b::
                        if (uVar14 & 0x100000) ~= 0 then
                            uVar14 = uVar14 & 0xffefffff
                        end
                        if (uVar14 & 0x80000) ~= 0 then
                            uVar14 = uVar14 & 0xfff7ffff
                        end
                        if (uVar14 & 0x40000) ~= 0 then
                            uVar14 = uVar14 & 0xfffbffff
                        end
                        if bVar2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005ac end
                            uVar6 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                            r26 = quest:GetHero()
                            quest:AddPersonToConversation(nil --[[missing]], r26)
                            r27 = quest:GetHero()
                            pCVar10 = me:GetDataString()
                            pCVar10 = CCharString_OperatorPlus_API(xStack_b8,"TEXT_QST_B11_",pCVar10)
                            pCVar4 = (pCVar10 .. pCVar4)
                            quest:AddLineToConversation(uVar6, pCVar4, r27, nil --[[missing]])
                        end
                        cVar3 = me:MsgExpressionPerformedTo()
                        if cVar3 then
                            if xStack_15c == nil then
                                bVar2 = false
                                if bVar2 then
                                    -- LAB_00e001c0: (native jump target)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00e005ac end
                                    piVar9 = (__native_entity_state:GetStateInt("self_0x14") + 0x54)
                                    -- TODO(native): *piVar9 = *piVar9 + -1;
                                end
                            else
                                -- TODO(native): iVar21 = CBasicString<char>::Compare(*(void **)xStack_15c,"EXPRESSION_WAIT");
                                if iVar21 == 0 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    if bVar2 then goto LAB_00e005ac end
                                    piVar9 = (__native_entity_state:GetStateInt("self_0x14") + 0x54)
                                    -- TODO(native): *piVar9 = *piVar9 + -1;
                                    goto FLOW_after_lab_00e001c0
                                end
                            end
                            ::FLOW_after_lab_00e001c0::
                        end
                        bVar2 = quest:IsDistanceBetweenThingsUnder(me, xStack_114_2, 20.0)
                        if bVar2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005ac end
                            piVar9 = (__native_entity_state:GetStateInt("self_0x14") + 0x58)
                            -- TODO(native): *piVar9 = *piVar9 + 1;
                            quest:EntityStopFollowing(nil --[[missing]])
                            r28 = quest:GetHero()
                            quest:SetEntityAsRegionFollowing(r28, nil --[[missing]], nil --[[missing]])
                            quest:EntitySetAsScared(nil --[[missing]], nil --[[missing]])
                            if 2 < quest:GetStateInt("TradersReachedTeleporter") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then goto LAB_00e005ac end
                                quest:SetStateBool("OutroStart", true)
                                quest:SetStateBool("MissionSucceeded", true)
                                alive = quest:NewScriptFrame(me)
                                -- TODO(native): Main_InitializeFourierAnalysis_4(*(undefined4 *)(this + 0x14));
                                quest:SetStateBool("OutroDone", true)
                                quest:RemoveThing(nil --[[missing]])
                                goto FLOW_after_lab_00e00595
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005ac end
                            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh({R = 255, G = 0, B = 0, A = 255});
                            if bVar2 then
                            end
                            cVar3 = me:AcquireControl(4)
                            goto LAB_00e0035a
                        end
                        c_stk_161 = quest:IsRegionLoaded("BanditCampEntrance")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e005ac end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                end
                goto LAB_00e005b1
            end
        end
    end
    goto LAB_00e005d5
    ::LAB_00e0035a::
    if cVar3 then goto LAB_00e0038b end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then goto LAB_00e005ac end
    cVar3 = me:AcquireControl(4)
    goto LAB_00e0035a
    ::LAB_00e0038b::
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        uVar6 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
        r29 = quest:GetHero()
        quest:AddPersonToConversation(nil --[[missing]], r29)
        r30 = quest:GetHero()
        pCVar4 = xStack_b8
        pCVar10 = me:GetDataString()
        pCVar10 = CCharString_OperatorPlus_API(xStack_b8,"TEXT_QST_B11_",pCVar10)
        pCVar4 = (pCVar10 .. pCVar4)
        quest:AddLineToConversation(uVar6, pCVar4, r30, nil --[[missing]])
        bVar2 = quest:IsDistanceBetweenThingsOver(me, xStack_110, 2.0)
        if bVar2 then
            repeat
                if quest:GetStateBool("OutroStart") then break end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then goto LAB_00e005ac end
                iVar21 = me:IsPerformingScriptTask()
                if not iVar21 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e005ac end
                    me:MoveToThing(nil --[[missing]], 1.0, 1)
                end
                bVar2 = quest:IsDistanceBetweenThingsOver(me, xStack_110, 2.0)
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            if not quest:GetStateBool("OutroStart") then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    quest:RemoveQuestInfoElement(nil --[[missing]])
                    quest:FadeOutAndKillEntity(me, true, 1.0, true)
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    me:ClearCommands()
                    cVar3 = quest:GetStateBool("OutroDone")
                    while not cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e005ac end
                        cVar3 = quest:GetStateBool("OutroDone")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if not bVar2 then
                        -- LAB_00e00595: (native jump target)
                        quest:RemoveThing(nil --[[missing]])
                    end
                end
            end
        end
    end
    ::FLOW_after_lab_00e00595::
    ::LAB_00e005ac::
    ::LAB_00e005b1::
    quest:DeregisterTimer(xStack_154)
    quest:DeregisterTimer(xStack_150)
    ::LAB_00e005d5::
    quest:DeregisterTimer(xStack_160)
    quest:DeregisterTimer(nil --[[missing]])
    ::LAB_00e005f9::
    resources:ReleaseResource(xStack_148)
end

function Init(quest, me)
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetAsScared(me, true)
    quest:EntitySetInFaction(me, "FACTION_NEUTRAL")
    quest:EntitySetAsAllowedToFollowHero(me, true)
    __native_entity_state:SetStateInt("BarIndex", 0)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
    local pQuestName
    local cVar2 = me:MsgIsKilledBy("")
    if cVar2 then
        pQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsFailed(pQuestName, true, "TEXT_QST_B11_QUEST_FAILED_TRADERS_DIED", true)
    end
end

