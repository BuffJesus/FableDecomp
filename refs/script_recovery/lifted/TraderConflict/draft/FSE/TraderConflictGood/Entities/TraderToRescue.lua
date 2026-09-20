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
    local CVar13, __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, __native_condition_5, __push1, __push10, __push11, __push12, __push13, __push14, __push15, __push16, __push17, __push18, __push19, __push2, __push20, __push21, __push22, __push23, __push24, __push25, __push3, __push4, __push5, __push6, __push7, __push8, __push9, au_stk_24, bVar2, cVar3, c_stk_161, dist, fVar16, iVar11, iVar18, iVar19, iVar20, iVar21, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, native_arg_sequence_4, p0, p0_00, p0_00_b3, p1, pCVar10, pCVar4, pCVar5, pCVar7, pcVar17, piVar9, pvVar8, r1, r2, r3, r4, r5, r6, r7, r8, uVar12, uVar14, uVar6, xStack_110, xStack_114, xStack_138, xStack_148, xStack_14c_2, xStack_150, xStack_154, xStack_158, xStack_15c, xStack_164, xStack_168, xStack_18, xStack_28, xStack_b8, x_stk_16c, x_stk_170
    local alive = true
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
    resources:PrepareResource(xStack_148)
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
        xStack_164 = quest:RegisterTimer()
        r1 = quest:GetNearestWithScriptName(me, "TC_BanditHostageKeeper")
        c_stk_161 = 0
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        repeat
            if bVar2 then
                quest:DeregisterTimer(xStack_164)
                quest:DeregisterTimer(unaff_EBX)
                resources:ReleaseResource(xStack_148)
                return
            end
            uVar12 = unaff_EBP | 1
            bVar2 = me:IsTalkedToByHero()
            if bVar2 then goto LAB_00dfe32c end
            uVar12 = unaff_EBP | 3
            pCVar4 = me:GetDataString()
            if pCVar4 == nil then
                bVar2 = false
                if bVar2 then
                    goto LAB_00dfe2e9
                end
            else
                iVar11 = ((pCVar4 == "TRADERB") and 0 or 1)
                if iVar11 == 0 then goto LAB_00dfe2e9 end
            end
            goto FLOW_past_lab_00dfe2e9
            ::LAB_00dfe2e9::
            if quest:GetStateBool("OpenedCage") then goto LAB_00dfe32c end
            ::FLOW_past_lab_00dfe2e9::
            xStack_168 = me:MsgExpressionPerformedTo()
            bVar2 = xStack_168 ~= nil
            if bVar2 then
                if xStack_168 == nil then
                    bVar2 = false
                    if not bVar2 then goto LAB_00dfe4df end
                else
                    iVar11 = ((xStack_168 == "EXPRESSION_FOLLOW") and 0 or 1)
                    if iVar11 ~= 0 then goto LAB_00dfe4df end
                end
                goto LAB_00dfe32c
            else
                goto LAB_00dfe4df
            end
            goto FLOW_past_lab_00dfe4df
            ::LAB_00dfe4df::
            bVar2 = false
            ::FLOW_past_lab_00dfe4df::
            goto FLOW_past_lab_00dfe32c
            ::LAB_00dfe32c::
            bVar2 = true
            ::FLOW_past_lab_00dfe32c::
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
                quest:PauseAllNonScriptedEntities(true)
                xStack_148 = resources:ScriptThing(xStack_14c_2)
                pCVar7 = xStack_148
                fVar16 = quest:GetHealth(pCVar7)
                p0_00_b3 = not (fVar16 <= 0.0)
                if not p0_00_b3 then goto LAB_00dfebc5 end
                iVar19 = 0
                iVar18 = 1
                iVar11 = 0
                pcVar17 = "_INTRO"
                pCVar4 = me:GetDataString()
                pCVar4 = ("TEXT_QST_B11_" .. pCVar4)
                pCVar4 = (pCVar4 .. pcVar17)
                pvVar8 = pCVar4
                iVar21 = quest:GetHero()
                r2 = me:Speak(iVar21, pvVar8, iVar11, false, (iVar18 ~= 0), (iVar19 ~= 0))
                iVar21 = me:IsPerformingScriptTask()
                cVar3 = iVar21
                goto LAB_00dfeb57
            end
            iVar11 = quest:GetTimer(xStack_164)
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
                    -- TODO(native): cVar3 = (**(xStack_124 + 0x12c))()
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
                    __push1 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, __push1, false)
                    native_arg_sequence_2 = false
                    if c_stk_161 == 0 then
                        native_arg_sequence_2 = true
                    else
                        native_arg_sequence_2 = false
                    end
                    if native_arg_sequence_2 then
                        -- TODO(native): cVar3 = (**(xStack_124 + 0x12c))()
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
                        uVar6 = quest:AddNewConversation(me, false, false)
                        __push2 = quest:GetHero()
                        quest:AddPersonToConversation(uVar6, __push2)
                        __push3 = quest:GetHero()
                        pcVar17 = "_KEEPERISDEAD"
                        pCVar4 = me:GetDataString()
                        pCVar4 = ("TEXT_QST_B11_" .. pCVar4)
                        pCVar4 = (pCVar4 .. pcVar17)
                        quest:AddLineToConversation(uVar6, pCVar4, me, __push3, false)
                        c_stk_161 = 1
                    else
                        iVar11 = math.random(0, 32767)
                        if iVar11 % 5 == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005d5 end
                            uVar6 = quest:AddNewConversation(me, false, false)
                            __push4 = quest:GetHero()
                            quest:AddPersonToConversation(uVar6, __push4)
                            __push5 = quest:GetHero()
                            pcVar17 = "_OVERHERE"
                            pCVar4 = me:GetDataString()
                            pCVar4 = ("TEXT_QST_B11_" .. pCVar4)
                            pCVar4 = (pCVar4 .. pcVar17)
                            quest:AddLineToConversation(uVar6, pCVar4, me, __push5, false)
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
                if not bVar2 then goto LAB_00dfe63d end
                cVar3 = quest:IsPlayerHoldingLockTargetButton()
                bVar2 = true
                if not cVar3 then goto LAB_00dfe63d end
            else
                goto LAB_00dfe63d
            end
            goto FLOW_past_lab_00dfe63d
            ::LAB_00dfe63d::
            bVar2 = false
            ::FLOW_past_lab_00dfe63d::
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
                    iVar11 = math.random(0, 32767)
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
                    uVar6 = quest:AddNewConversation(me, false, false)
                    __push6 = quest:GetHero()
                    quest:AddPersonToConversation(uVar6, __push6)
                    __push7 = quest:GetHero()
                    pcVar17 = "_THREATEN"
                    pCVar4 = me:GetDataString()
                    pCVar4 = ("TEXT_QST_B11_" .. pCVar4)
                    pCVar4 = (pCVar4 .. pcVar17)
                    quest:AddLineToConversation(uVar6, pCVar4, me, __push7, false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then goto LAB_00e005d5 end
                    uVar6 = quest:AddNewConversation(me, false, false)
                    __push8 = quest:GetHero()
                    quest:AddPersonToConversation(uVar6, __push8)
                    __push9 = quest:GetHero()
                    pcVar17 = "_THREATENWEAPON"
                    pCVar4 = me:GetDataString()
                    pCVar4 = ("TEXT_QST_B11_" .. pCVar4)
                    pCVar4 = (pCVar4 .. pcVar17)
                    quest:AddLineToConversation(uVar6, pCVar4, me, __push9, false)
                end
                quest:SetTimer(unaff_EBX, 0x14)
                CVar13 = x_stk_170
                iVar11 = quest:GetTimer(x_stk_170)
                quest:SetTimer(iVar11 + 5, dist)
            end
            -- TODO(native): unaff_EBP = uVar12 | 8;
            bVar2 = me:MsgIsHitByHero()
            if bVar2 then
                goto LAB_00dfe8b5
            else
                -- TODO(native): unaff_EBP = uVar12 | 0x18;
                bVar2 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar2 then
                    -- TODO(native): unaff_EBP = uVar12 | 0x38;
                    bVar2 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar2 then goto LAB_00dfe8b5 end
                end
                bVar2 = false
            end
            goto FLOW_past_lab_00dfe8b5
            ::LAB_00dfe8b5::
            bVar2 = true
            ::FLOW_past_lab_00dfe8b5::
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
                uVar6 = quest:AddNewConversation(me, false, false)
                __push10 = quest:GetHero()
                quest:AddPersonToConversation(uVar6, __push10)
                __push11 = quest:GetHero()
                pcVar17 = "_ONHIT"
                pCVar4 = me:GetDataString()
                pCVar4 = ("TEXT_QST_B11_" .. pCVar4)
                pCVar4 = (pCVar4 .. pcVar17)
                quest:AddLineToConversation(uVar6, pCVar4, me, __push11, false)
                quest:SetTimer(unaff_EBX, 10)
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
        until false
    end
    goto LAB_00e005f9
    ::LAB_00dfeb57::
    if not cVar3 then goto LAB_00dfeb80 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        quest:PauseAllNonScriptedEntities(false)
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
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_b8)
    else
        goto LAB_00dfebc5
    end
    goto FLOW_past_lab_00dfebc5
    ::LAB_00dfebc5::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_b8)
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:DeactivateQuest("Q_TraderConflictGood_Extras", 0)
        quest:ActivateQuest("Q_TraderConflictGood_Extras")
        quest:SetIsPushableByHero(pCVar5, __unknown_push)
        uVar6 = quest:GetHero()
        quest:EntityFollowThing(me, uVar6, nil --[[missing]], nil --[[missing]])
        uVar6 = quest:GetHero()
        quest:SetEntityAsRegionFollowing(uVar6, me, true)
        quest:EntitySetOpinionReactionsEnabled(me, false)
        quest:EntitySetDeedReactionsEnabled(me, false)
        quest:EntitySetCombatEnabled(me, false)
        quest:EntitySetInFaction(me, "FACTION_TRADERS")
        quest:DisplayQuestInfo(true)
        pCVar4 = me:GetDataString()
        iVar21 = ((pCVar4 == "TRADERA") and 0 or 1)
        cVar3 = not (iVar21 ~= 0)
        if not cVar3 then
            pCVar4 = me:GetDataString()
            iVar21 = ((pCVar4 == "TRADERB") and 0 or 1)
            cVar3 = not (iVar21 ~= 0)
            if cVar3 then
                goto LAB_00dfee05
            end
            goto FLOW_past_lab_00dfee05
            ::LAB_00dfee05::
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e005d5 end
            pcVar17 = "HUD_QUEST_ICON_TRADER_HAT_02"
            goto LAB_00dfee1b
            ::FLOW_past_lab_00dfee05::
            pCVar4 = me:GetDataString()
            iVar21 = ((pCVar4 == "TRADERC") and 0 or 1)
            cVar3 = not (iVar21 ~= 0)
            if cVar3 then goto LAB_00dfee05 end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e005d5 end
            pcVar17 = "HUD_QUEST_ICON_TRADER"
            goto LAB_00dfee1b
        end
        goto FLOW_past_lab_00dfee1b
        ::LAB_00dfee1b::
        uVar6 = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, pcVar17, 1.0)
        __native_entity_state:SetStateInt("BarIndex", uVar6)
        ::FLOW_past_lab_00dfee1b::
        resources:PrepareResource(xStack_148)
        cVar3 = quest:IsEntityFollowingHero(me)
        while not cVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then goto LAB_00e005d5 end
            cVar3 = quest:IsEntityFollowingHero(me)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if not bVar2 then
            quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") + 1)
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
                        pCVar4 = ("TEXT_QST_B11_" .. pCVar4)
                        pCVar4 = (pCVar4 .. "_ONTALK_")
                        xStack_158 = pCVar4
                        pCVar4 = tostring(CVar13)
                        pCVar4 = (xStack_158 .. pCVar4)
                        xStack_15c = pCVar4
                        cVar3 = quest:TextEntryExists()
                        if not cVar3 then
                            CVar13 = 0xa
                            pCVar4 = tostring(10)
                            pCVar4 = (xStack_158 .. pCVar4)
                            xStack_15c = pCVar4
                        end
                        -- TODO(native): xStack_10c = (CCharString)((int)CVar13 + 0xa);
                        resources:PrepareResource(xStack_148)
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
                        quest:PauseAllNonScriptedEntities(true)
                        au_stk_24 = resources:ScriptThing(xStack_154)
                        pCVar7 = au_stk_24
                        fVar16 = quest:GetHealth(pCVar7)
                        p0_00_b3 = not (fVar16 <= 0.0)
                        if p0_00_b3 then
                            iVar20 = 0
                            iVar19 = 1
                            iVar18 = 0
                            iVar11 = 0
                            iVar21 = quest:GetHero()
                            r3 = me:Speak(iVar21, pvVar8, iVar11, (iVar18 ~= 0), (iVar19 ~= 0), (iVar20 ~= 0))
                            iVar21 = me:IsPerformingScriptTask()
                            cVar3 = iVar21
                            while cVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_18)
                                    goto LAB_00e005b1
                                end
                                iVar21 = me:IsPerformingScriptTask()
                                cVar3 = iVar21
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_18)
                                goto LAB_00e005b1
                            end
                        end
                        resources:PrepareResource(xStack_148)
                        quest:PauseAllNonScriptedEntities(false)
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
                        cVar3 = quest:IsPlayerHoldingLockTargetButton()
                        __native_condition_1 = cVar3
                    end
                    if __native_condition_1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e005b1 end
                        uVar6 = quest:AddNewConversation(me, false, false)
                        __push12 = quest:GetHero()
                        quest:AddPersonToConversation(uVar6, __push12)
                        __push13 = quest:GetHero()
                        pCVar4 = me:GetDataString()
                        pCVar4 = ("TEXT_QST_B11_" .. pCVar4)
                        pCVar4 = (pCVar4 .. "_THREATEN")
                        quest:AddLineToConversation(uVar6, pCVar4, me, __push13, false)
                        quest:SetTimer(unaff_EBX, 0x14)
                    end
                    uVar14 = uVar12 | 0x40
                    cVar3 = me:MsgIsHitBy("")
                    if not cVar3 then
                        uVar14 = uVar12 | 0xc0
                        cVar3 = me:MsgIsHitByAnySpecialAbilityFrom("")
                        if cVar3 then goto LAB_00dff3b5 end
                        goto LAB_00dff3f2
                    else
                        goto LAB_00dff3b5
                    end
                    goto FLOW_past_lab_00dff3f2
                    ::LAB_00dff3f2::
                    bVar2 = false
                    ::FLOW_past_lab_00dff3f2::
                    goto FLOW_past_lab_00dff3b5
                    ::LAB_00dff3b5::
                    uVar14 = uVar14 | 0x100
                    cVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    bVar2 = true
                    if cVar3 then goto LAB_00dff3f2 end
                    ::FLOW_past_lab_00dff3b5::
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
                        fVar16 = quest:GetHealth(me)
                        uVar12 = uVar14
                        __native_condition_3 = fVar16 <= 5.0
                        if not __native_condition_3 then
                            iVar21 = quest:GetTimer(xStack_148)
                            __native_condition_3 = iVar21 ~= 0
                        end
                        if __native_condition_3 then
                            goto LAB_00dff54a
                        else
                            uVar12 = uVar14 | 0x200
                            cVar3 = me:MsgIsHitByHero()
                            if cVar3 then goto LAB_00dff54a end
                            uVar12 = uVar14 | 0x600
                            cVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                            if cVar3 then
                                uVar12 = uVar14 | 0xe00
                                cVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                if not cVar3 then goto LAB_00dff54a end
                            end
                            bVar2 = true
                        end
                        goto FLOW_past_lab_00dff54a
                        ::LAB_00dff54a::
                        bVar2 = false
                        ::FLOW_past_lab_00dff54a::
                        uVar14 = uVar12
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
                                    pCVar4 = tostring(xStack_148)
                                    __push14 = ("SND_MM_TRADER_C_SCREAM_0" .. pCVar4)
                                    r4 = quest:PlaySoundOnThing(me, __push14)
                                    bVar2 = CVar13 == 0x4
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar2 = not alive
                                    CVar13 = xStack_148
                                    if bVar2 then goto LAB_00e005b1 end
                                    pCVar4 = tostring(xStack_148)
                                    __push15 = ("SND_MM_TRADER_B_SCREAM_0" .. pCVar4)
                                    r5 = quest:PlaySoundOnThing(me, __push15)
                                    bVar2 = CVar13 == 0x3
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                CVar13 = xStack_148
                                if bVar2 then goto LAB_00e005b1 end
                                pCVar4 = tostring(xStack_148)
                                __push16 = ("SND_MM_TRADER_A_SCREAM_0" .. pCVar4)
                                r6 = quest:PlaySoundOnThing(me, __push16)
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
                            fVar16 = quest:GetHealth(me)
                            uVar12 = uVar14
                            if fVar16 <= 5.0 then
                                goto LAB_00dff887
                            else
                                uVar12 = uVar14 | 0x1000
                                cVar3 = me:MsgIsHitByHero()
                                if not cVar3 then
                                    uVar12 = uVar14 | 0x3000
                                    cVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                                    if cVar3 then
                                        uVar12 = uVar14 | 0x7000
                                        cVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                        if not cVar3 then goto LAB_00dff880 end
                                    end
                                    goto LAB_00dff887
                                end
                                ::LAB_00dff880::
                                bVar2 = true
                            end
                            goto FLOW_past_lab_00dff887
                            ::LAB_00dff887::
                            bVar2 = false
                            ::FLOW_past_lab_00dff887::
                            uVar14 = uVar12
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
                                uVar6 = quest:AddNewConversation(me, false, false)
                                __push17 = quest:GetHero()
                                quest:AddPersonToConversation(uVar6, __push17)
                                __push18 = quest:GetHero()
                                pCVar4 = me:GetDataString()
                                pCVar4 = ("TEXT_QST_B11_" .. pCVar4)
                                pCVar4 = (pCVar4 .. "_ONHIT")
                                quest:AddLineToConversation(uVar6, pCVar4, me, __push18, false)
                            end
                        end
                    end
                    uVar12 = uVar14 | 0x8000
                    cVar3 = me:MsgIsHitByHero()
                    if not cVar3 then
                        uVar12 = uVar14 | 0x18000
                        cVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if cVar3 then
                            uVar12 = uVar14 | 0x38000
                            cVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not cVar3 then goto LAB_00dffa46 end
                        end
                        bVar2 = false
                    else
                        goto LAB_00dffa46
                    end
                    goto FLOW_past_lab_00dffa46
                    ::LAB_00dffa46::
                    bVar2 = true
                    ::FLOW_past_lab_00dffa46::
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
                                goto LAB_00dffb06
                            end
                        else
                            iVar21 = ((xStack_15c == "EXPRESSION_WAIT") and 0 or 1)
                            if iVar21 == 0 then goto LAB_00dffb06 end
                        end
                        goto FLOW_past_lab_00dffb06
                        ::LAB_00dffb06::
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e005b1 end
                        quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") + -1)
                        ::FLOW_past_lab_00dffb06::
                    end
                    cVar3 = quest:IsRegionLoaded("BanditCampEntrance")
                    c_stk_161 = not (cVar3)
                    CVar13 = 0xa
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then break end
                r7 = quest:GetThingWithScriptName("TeleporterMarker")
                c_stk_161 = quest:IsRegionLoaded("BanditCampEntrance")
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
                        pCVar4 = ("TEXT_QST_B11_" .. pCVar4)
                        pCVar4 = (pCVar4 .. "_ONTALK_")
                        xStack_158 = pCVar4
                        pCVar4 = tostring(CVar13)
                        pCVar4 = (xStack_158 .. pCVar4)
                        xStack_15c = pCVar4
                        cVar3 = quest:TextEntryExists()
                        if not cVar3 then
                            CVar13 = 0xa
                            pCVar4 = tostring(10)
                            pCVar4 = (xStack_158 .. pCVar4)
                            xStack_15c = pCVar4
                        end
                        -- TODO(native): xStack_10c = (CCharString)((int)CVar13 + 0xa);
                        resources:PrepareResource(xStack_148)
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
                        quest:PauseAllNonScriptedEntities(true)
                        xStack_b8 = resources:ScriptThing(xStack_148)
                        pCVar7 = xStack_b8
                        fVar16 = quest:GetHealth(pCVar7)
                        p0_00_b3 = not (fVar16 <= 0.0)
                        if p0_00_b3 then
                            iVar20 = 0
                            iVar19 = 1
                            iVar18 = 0
                            iVar11 = 0
                            iVar21 = quest:GetHero()
                            r8 = me:Speak(iVar21, pvVar8, iVar11, (iVar18 ~= 0), (iVar19 ~= 0), (iVar20 ~= 0))
                            iVar21 = me:IsPerformingScriptTask()
                            cVar3 = iVar21
                            while cVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar2 = not alive
                                if bVar2 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_28)
                                    goto LAB_00e005ac
                                end
                                iVar21 = me:IsPerformingScriptTask()
                                cVar3 = iVar21
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_28)
                                goto LAB_00e005ac
                            end
                        end
                        resources:PrepareResource(xStack_148)
                        quest:PauseAllNonScriptedEntities(false)
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
                        cVar3 = quest:IsPlayerHoldingLockTargetButton()
                        __native_condition_4 = cVar3
                    end
                    if __native_condition_4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e005ac end
                        uVar6 = quest:AddNewConversation(me, false, false)
                        __push19 = quest:GetHero()
                        quest:AddPersonToConversation(uVar6, __push19)
                        __push20 = quest:GetHero()
                        pCVar10 = me:GetDataString()
                        pCVar10 = ("TEXT_QST_B11_" .. pCVar10)
                        pCVar4 = (pCVar10 .. "_THREATEN")
                        quest:AddLineToConversation(uVar6, pCVar4, me, __push20, false)
                        quest:SetTimer(unaff_EBX, 0x14)
                    end
                    uVar14 = uVar12 | 0x40000
                    cVar3 = me:MsgIsHitByHero()
                    if not cVar3 then
                        uVar14 = uVar12 | 0xc0000
                        cVar3 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if cVar3 then
                            uVar14 = uVar12 | 0x1c0000
                            cVar3 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not cVar3 then goto LAB_00e0006b end
                        end
                        bVar2 = false
                    else
                        goto LAB_00e0006b
                    end
                    goto FLOW_past_lab_00e0006b
                    ::LAB_00e0006b::
                    bVar2 = true
                    ::FLOW_past_lab_00e0006b::
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
                        uVar6 = quest:AddNewConversation(me, false, false)
                        __push21 = quest:GetHero()
                        quest:AddPersonToConversation(uVar6, __push21)
                        __push22 = quest:GetHero()
                        pCVar10 = me:GetDataString()
                        pCVar10 = ("TEXT_QST_B11_" .. pCVar10)
                        pCVar4 = (pCVar10 .. "_ONHIT")
                        quest:AddLineToConversation(uVar6, pCVar4, me, __push22, false)
                    end
                    cVar3 = me:MsgExpressionPerformedTo()
                    if cVar3 then
                        if xStack_164 == nil then
                            bVar2 = false
                            if bVar2 then
                                goto LAB_00e001c0
                            end
                        else
                            iVar21 = ((xStack_164 == "EXPRESSION_WAIT") and 0 or 1)
                            if iVar21 == 0 then goto LAB_00e001c0 end
                        end
                        goto FLOW_past_lab_00e001c0
                        ::LAB_00e001c0::
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e005ac end
                        quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") + -1)
                        ::FLOW_past_lab_00e001c0::
                    end
                    bVar2 = quest:IsDistanceBetweenThingsUnder(me, xStack_114, 20.0)
                    if bVar2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e005ac end
                        quest:SetStateInt("TradersReachedTeleporter", quest:GetStateInt("TradersReachedTeleporter") + 1)
                        quest:EntityStopFollowing(me)
                        __push23 = quest:GetHero()
                        quest:SetEntityAsRegionFollowing(__push23, me, false)
                        quest:EntitySetAsScared(me, false)
                        if 2 < quest:GetStateInt("TradersReachedTeleporter") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar2 = not alive
                            if bVar2 then goto LAB_00e005ac end
                            quest:SetStateBool("OutroStart", true)
                            quest:SetStateBool("MissionSucceeded", true)
                            alive = quest:NewScriptFrame(me)
                            -- TODO(native): Main_InitializeFourierAnalysis_4(*(undefined4 *)(this + 0x14));
                            quest:SetStateBool("OutroDone", true)
                            goto LAB_00e00595
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then goto LAB_00e005ac end
                        resources:PrepareResource(xStack_138)
                        cVar3 = me:AcquireControl(4)
                        goto LAB_00e0035a
                    end
                    c_stk_161 = quest:IsRegionLoaded("BanditCampEntrance")
                    CVar13 = 0xa
                    uVar12 = uVar14
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
    ::FLOW_past_lab_00dfebc5::
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
        uVar6 = quest:AddNewConversation(me, false, false)
        __push24 = quest:GetHero()
        quest:AddPersonToConversation(uVar6, __push24)
        __push25 = quest:GetHero()
        pCVar4 = xStack_b8
        pCVar10 = me:GetDataString()
        pCVar10 = ("TEXT_QST_B11_" .. pCVar10)
        pCVar4 = (pCVar10 .. "_FREED_10")
        quest:AddLineToConversation(uVar6, pCVar4, me, __push25, false)
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
                    me:MoveToThing(r7, 1.0, 1)
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
                    quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("BarIndex"))
                    quest:FadeOutAndKillEntity(me, true, 1.0, true)
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if not bVar2 then
                    me:ClearCommands()
                    resources:PrepareResource(r1)
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
                        goto LAB_00e00595
                    end
                end
            end
        end
    end
    goto FLOW_past_lab_00e00595
    ::LAB_00e00595::
    quest:RemoveThing(me, false, true)
    ::FLOW_past_lab_00e00595::
    ::LAB_00e005ac::
    ::LAB_00e005b1::
    quest:DeregisterTimer(xStack_154)
    quest:DeregisterTimer(xStack_150)
    ::LAB_00e005d5::
    quest:DeregisterTimer(0x1)
    quest:DeregisterTimer(0xa)
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

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    local pQuestName
    local cVar2 = me:MsgIsKilledBy("")
    if cVar2 then
        pQuestName = quest:GetActiveQuestName()
        quest:SetQuestAsFailed(pQuestName, true, "TEXT_QST_B11_QUEST_FAILED_TRADERS_DIED", true)
    end
end

