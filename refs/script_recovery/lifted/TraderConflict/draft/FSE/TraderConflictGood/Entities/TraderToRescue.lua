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
    local CVar18, CVar25, __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, __native_condition_5, __native_condition_6, bVar10, bVar11, bVar4, bVar6, bVar8, bVar9, cVar7, c_stk_155, c_stk_171, fVar26, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, iVar14, iVar17, iVar27, iVar28, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, p0, pCVar12, pCVar13, pCVar15, pCVar20, pcVar21, pvVar16, r1, r2, r3, r4, r5, r6, r7, r8, uVar22, uVar5, u_stk_170, xStack_124, xStack_148, xStack_150, xStack_154, xStack_15c, xStack_160, xStack_164, xStack_168, xStack_16c, xStack_1c, xStack_2c, xStack_b8, x_stk_c
    local alive = true
    bVar4 = false
    bVar9 = false
    bVar11 = false
    bVar10 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then
        return
    end
    cVar7 = quest:GetStateBool("IntroDone")
    while not cVar7 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            return
        end
        cVar7 = quest:GetStateBool("IntroDone")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if bVar6 then
        return
    end
    xStack_148 = resources:NewResource()
    resources:PrepareResource(xStack_148)
    bVar6 = resources:TryAcquire(xStack_148, me, 4)
    while not bVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then goto LAB_00e005f9 end
        bVar6 = resources:TryAcquire(xStack_148, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if not bVar6 then
        xStack_16c = quest:RegisterTimer()
        xStack_164 = quest:RegisterTimer()
        r1 = quest:GetNearestWithScriptName(me, "TC_BanditHostageKeeper")
        c_stk_155 = 0
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        repeat
            if bVar6 then
                quest:DeregisterTimer(xStack_164)
                quest:DeregisterTimer(xStack_16c)
                resources:ReleaseResource(xStack_148)
                return
            end
            bVar6 = me:IsTalkedToByHero()
            if bVar6 then goto LAB_00dfe32c end
            bVar4 = true
            pCVar12 = me:GetDataString()
            if pCVar12 == nil then
                bVar6 = false
                if bVar6 then
                    goto LAB_00dfe2e9
                end
            else
                iVar17 = ((pCVar12 == "TRADERB") and 0 or 1)
                if iVar17 == 0 then goto LAB_00dfe2e9 end
            end
            goto FLOW_past_lab_00dfe2e9
            ::LAB_00dfe2e9::
            if quest:GetStateBool("OpenedCage") then goto LAB_00dfe32c end
            ::FLOW_past_lab_00dfe2e9::
            xStack_168 = me:MsgExpressionPerformedTo()
            bVar6 = xStack_168 ~= nil
            if bVar6 then
                if xStack_168 == nil then
                    bVar6 = false
                    if not bVar6 then goto LAB_00dfe4df end
                else
                    iVar17 = ((xStack_168 == "EXPRESSION_FOLLOW") and 0 or 1)
                    if iVar17 ~= 0 then goto LAB_00dfe4df end
                end
                goto LAB_00dfe32c
            else
                goto LAB_00dfe4df
            end
            goto FLOW_past_lab_00dfe4df
            ::LAB_00dfe4df::
            bVar6 = false
            ::FLOW_past_lab_00dfe4df::
            goto FLOW_past_lab_00dfe32c
            ::LAB_00dfe32c::
            bVar6 = true
            ::FLOW_past_lab_00dfe32c::
            if bVar4 then
                bVar4 = false
            end
            u_stk_170 = 0
            if bVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if bVar10 then goto LAB_00e005d5 end
                xStack_b8 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                xStack_124 = resources:ScriptThing(xStack_148)
                pCVar13 = xStack_124
                fret_0 = quest:GetHealth(pCVar13)
                fVar3 = 0.0
                if fret_0 <= fVar3 then goto LAB_00dfebc5 end
                iVar27 = 0
                iVar14 = 1
                iVar17 = 0
                pCVar20 = 0x0
                pcVar21 = "_INTRO"
                pCVar12 = me:GetDataString()
                pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
                pCVar12 = (pCVar12 .. pcVar21)
                pvVar16 = pCVar12
                pCVar13 = quest:GetHero()
                r2 = me:Speak(pCVar13, pvVar16, pCVar20, (iVar17 ~= 0), (iVar14 ~= 0), (iVar27 ~= 0))
                iVar17 = me:IsPerformingScriptTask()
                cVar7 = iVar17
                goto LAB_00dfeb57
            end
            iVar17 = quest:GetTimer(xStack_164)
            native_arg_sequence_1 = false
            if iVar17 == 0 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                iVar17 = xStack_16c
                if c_stk_155 == 0 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
                if native_arg_sequence_1 then
                    cVar7 = (r1 ~= nil and r1:IsAlive())
                    iVar17 = xStack_16c
                    if not cVar7 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                end
            end
            if native_arg_sequence_1 then
                fVar26 = 15.0
                pCVar13 = quest:GetHero()
                bVar6 = quest:IsDistanceBetweenThingsUnder(me, pCVar13, fVar26)
                if bVar6 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e005d5 end
                    CVar25 = 0x0
                    pCVar13 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, pCVar13, (CVar25 ~= 0))
                    __native_condition_1 = c_stk_155 == 0
                    if __native_condition_1 then
                        cVar7 = (r1 ~= nil and r1:IsAlive())
                        __native_condition_1 = not cVar7
                    end
                    if __native_condition_1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00e005d5 end
                        iVar14 = quest:AddNewConversation(me, false, false)
                        pCVar20 = quest:GetHero()
                        quest:AddPersonToConversation(iVar14, pCVar20)
                        pCVar15 = quest:GetHero()
                        uVar22 = false
                        pcVar21 = "_KEEPERISDEAD"
                        pCVar12 = me:GetDataString()
                        pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
                        pCVar20 = (pCVar12 .. pcVar21)
                        quest:AddLineToConversation(iVar14, pCVar20, me, pCVar15, uVar22)
                        c_stk_155 = 1
                    else
                        iVar17 = math.random(0, 32767)
                        if iVar17 % 5 == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e005d5 end
                            iVar14 = quest:AddNewConversation(me, false, false)
                            pCVar20 = quest:GetHero()
                            quest:AddPersonToConversation(iVar14, pCVar20)
                            pCVar15 = quest:GetHero()
                            uVar22 = false
                            pcVar21 = "_OVERHERE"
                            pCVar12 = me:GetDataString()
                            pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
                            pCVar20 = (pCVar12 .. pcVar21)
                            quest:AddLineToConversation(iVar14, pCVar20, me, pCVar15, uVar22)
                        end
                    end
                    quest:SetTimer(xStack_164, 10)
                    iVar17 = xStack_16c
                    iVar14 = quest:GetTimer(xStack_16c)
                    quest:SetTimer(iVar17, iVar14 + 5)
                end
            end
            iVar17 = quest:GetTimer(iVar17)
            if iVar17 == 0 then
                bVar10 = true
                pCVar13 = quest:GetHeroTargetedThing()
                cVar7 = (pCVar13 ~= nil and pCVar13:IsEqualTo(me))
                if not cVar7 then goto LAB_00dfe63d end
                bVar8 = quest:IsPlayerHoldingLockTargetButton()
                bVar6 = true
                if not bVar8 then goto LAB_00dfe63d end
            else
                goto LAB_00dfe63d
            end
            goto FLOW_past_lab_00dfe63d
            ::LAB_00dfe63d::
            bVar6 = false
            ::FLOW_past_lab_00dfe63d::
            if bVar10 then
                bVar10 = false
            end
            if bVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00e005d5 end
                iVar17 = IsPlayerThreateningEntity(me)
                native_arg_sequence_2 = false
                if iVar17 == 0 then
                    native_arg_sequence_2 = true
                else
                    native_arg_sequence_2 = false
                end
                if not native_arg_sequence_2 then
                    iVar17 = math.random(0, 32767)
                    if iVar17 % 3 ~= 0 then
                        native_arg_sequence_2 = true
                    else
                        native_arg_sequence_2 = false
                    end
                end
                if native_arg_sequence_2 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e005d5 end
                    iVar14 = quest:AddNewConversation(me, false, false)
                    pCVar20 = quest:GetHero()
                    quest:AddPersonToConversation(iVar14, pCVar20)
                    pCVar15 = quest:GetHero()
                    uVar22 = false
                    pcVar21 = "_THREATEN"
                    pCVar12 = me:GetDataString()
                    pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
                    pCVar20 = (pCVar12 .. pcVar21)
                    quest:AddLineToConversation(iVar14, pCVar20, me, pCVar15, uVar22)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e005d5 end
                    iVar14 = quest:AddNewConversation(me, false, false)
                    pCVar20 = quest:GetHero()
                    quest:AddPersonToConversation(iVar14, pCVar20)
                    pCVar15 = quest:GetHero()
                    uVar22 = false
                    pcVar21 = "_THREATENWEAPON"
                    pCVar12 = me:GetDataString()
                    pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
                    pCVar20 = (pCVar12 .. pcVar21)
                    quest:AddLineToConversation(iVar14, pCVar20, me, pCVar15, uVar22)
                end
                quest:SetTimer(xStack_16c, 0x14)
                iVar17 = xStack_164
                iVar14 = quest:GetTimer(xStack_164)
                quest:SetTimer(iVar17, iVar14 + 5)
            end
            bVar6 = me:MsgIsHitByHero()
            if bVar6 then
                goto LAB_00dfe8b5
            else
                bVar9 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if bVar9 then
                    bVar9 = true
                    bVar11 = true
                    bVar6 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not bVar6 then goto LAB_00dfe8b5 end
                end
                bVar9 = true
                bVar6 = false
            end
            goto FLOW_past_lab_00dfe8b5
            ::LAB_00dfe8b5::
            bVar6 = true
            ::FLOW_past_lab_00dfe8b5::
            if bVar11 then
                bVar11 = false
            end
            if bVar9 then
                bVar9 = false
            end
            if bVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00e005d5 end
                iVar14 = quest:AddNewConversation(me, false, false)
                pCVar20 = quest:GetHero()
                quest:AddPersonToConversation(iVar14, pCVar20)
                pCVar15 = quest:GetHero()
                uVar22 = false
                pcVar21 = "_ONHIT"
                pCVar12 = me:GetDataString()
                pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
                pCVar20 = (pCVar12 .. pcVar21)
                quest:AddLineToConversation(iVar14, pCVar20, me, pCVar15, uVar22)
                quest:SetTimer(xStack_16c, 10)
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
        until false
    end
    goto LAB_00e005f9
    ::LAB_00dfeb57::
    if not cVar7 then goto LAB_00dfeb80 end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar10 = not alive
    if bVar10 then
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(xStack_b8)
        goto LAB_00e005d5
    end
    iVar17 = me:IsPerformingScriptTask()
    cVar7 = iVar17
    goto LAB_00dfeb57
    ::LAB_00dfeb80::
    alive = not quest:IsActiveThreadTerminating()
    bVar10 = not alive
    if bVar10 then
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
    bVar10 = not alive
    if not bVar10 then
        quest:DeactivateQuest("Q_TraderConflictGood_Extras", 0)
        quest:ActivateQuest("Q_TraderConflictGood_Extras")
        CVar25 = 0x1
        quest:SetIsPushableByHero(me, (CVar25 ~= 0))
        CVar25 = 0x1
        fVar26 = 3.0
        pCVar20 = quest:GetHero()
        quest:EntityFollowThing(me, pCVar20, fVar26, (CVar25 ~= 0))
        CVar25 = 0x1
        pCVar13 = quest:GetHero()
        quest:SetEntityAsRegionFollowing(pCVar13, me, (CVar25 ~= 0))
        quest:EntitySetOpinionReactionsEnabled(me, false)
        quest:EntitySetDeedReactionsEnabled(me, false)
        quest:EntitySetCombatEnabled(me, false)
        quest:EntitySetInFaction(me, "FACTION_TRADERS")
        quest:DisplayQuestInfo(true)
        pCVar12 = me:GetDataString()
        iVar17 = ((pCVar12 == "TRADERA") and 0 or 1)
        c_stk_171 = not (iVar17 ~= 0)
        if not c_stk_171 then
            pCVar12 = me:GetDataString()
            iVar17 = ((pCVar12 == "TRADERB") and 0 or 1)
            c_stk_171 = not (iVar17 ~= 0)
            if c_stk_171 then
                goto LAB_00dfee05
            end
            goto FLOW_past_lab_00dfee05
            ::LAB_00dfee05::
            alive = not quest:IsActiveThreadTerminating()
            bVar10 = not alive
            if bVar10 then goto LAB_00e005d5 end
            pcVar21 = "HUD_QUEST_ICON_TRADER_HAT_02"
            goto LAB_00dfee1b
            ::FLOW_past_lab_00dfee05::
            pCVar12 = me:GetDataString()
            iVar17 = ((pCVar12 == "TRADERC") and 0 or 1)
            c_stk_171 = not (iVar17 ~= 0)
            if c_stk_171 then goto LAB_00dfee05 end
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar10 = not alive
            if bVar10 then goto LAB_00e005d5 end
            pcVar21 = "HUD_QUEST_ICON_TRADER"
            goto LAB_00dfee1b
        end
        goto FLOW_past_lab_00dfee1b
        ::LAB_00dfee1b::
        iVar17 = quest:AddQuestInfoBarHealth(me, {R = 255, G = 0, B = 0, A = 255}, pcVar21, 1.0)
        __native_entity_state:SetStateInt("BarIndex", iVar17)
        ::FLOW_past_lab_00dfee1b::
        resources:PrepareResource(xStack_148)
        bVar10 = quest:IsEntityFollowingHero(me)
        while not bVar10 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar10 = not alive
            if bVar10 then goto LAB_00e005d5 end
            bVar10 = quest:IsEntityFollowingHero(me)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar10 = not alive
        if not bVar10 then
            quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") + 1)
            CVar18 = 0xa
            xStack_150 = quest:RegisterTimer()
            quest:SetTimer(xStack_150, 0x14)
            xStack_154 = quest:RegisterTimer()
            alive = not quest:IsActiveThreadTerminating()
            bVar10 = not alive
            uVar5 = u_stk_170
            while not bVar10 do
                bVar10 = quest:IsRegionLoaded("BanditCampEntrance")
                c_stk_155 = (1 - (bVar10 and 1 or 0))
                while c_stk_155 ~= 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar10 = not alive
                    if bVar10 then goto LAB_00e005b1 end
                    bVar10 = me:IsTalkedToByHero()
                    if bVar10 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005b1 end
                        pcVar21 = "_ONTALK_"
                        pCVar12 = me:GetDataString()
                        pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
                        pCVar12 = (pCVar12 .. pcVar21)
                        xStack_15c = pCVar12
                        pCVar12 = tostring(CVar18)
                        pCVar12 = (xStack_15c .. pCVar12)
                        xStack_160 = pCVar12
                        bVar10 = quest:TextEntryExists(xStack_160)
                        if not bVar10 then
                            CVar18 = 0xa
                            pCVar12 = tostring(10)
                            pCVar12 = (xStack_15c .. pCVar12)
                            xStack_160 = pCVar12
                        end
                        -- TODO(native): xStack_118 = (CCharString)((int)CVar18 + 0xa);
                        resources:PrepareResource(xStack_148)
                        bVar10 = resources:TryAcquire(xStack_148, me, 4)
                        while not bVar10 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar10 = not alive
                            if bVar10 then goto LAB_00e005b1 end
                            bVar10 = resources:TryAcquire(xStack_148, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005b1 end
                        xStack_1c = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_c = resources:ScriptThing(xStack_148)
                        pCVar20 = x_stk_c
                        fret_00 = quest:GetHealth(pCVar20)
                        fVar3 = 0.0
                        if fVar3 < fret_00 then
                            iVar28 = 0
                            iVar27 = 1
                            iVar14 = 0
                            iVar17 = 0
                            pvVar16 = xStack_160
                            pCVar20 = quest:GetHero()
                            r3 = me:Speak(pCVar20, pvVar16, iVar17, (iVar14 ~= 0), (iVar27 ~= 0), (iVar28 ~= 0))
                            iVar17 = me:IsPerformingScriptTask()
                            cVar7 = iVar17
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar10 = not alive
                                if bVar10 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_1c)
                                    goto LAB_00e005b1
                                end
                                iVar17 = me:IsPerformingScriptTask()
                                cVar7 = iVar17
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar10 = not alive
                            if bVar10 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_1c)
                                goto LAB_00e005b1
                            end
                        end
                        resources:PrepareResource(xStack_148)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_1c)
                    end
                    iVar17 = quest:GetTimer(xStack_16c)
                    __native_condition_3 = iVar17 == 0
                    if __native_condition_3 then
                        iVar17 = IsPlayerThreateningEntity(me)
                        __native_condition_3 = iVar17 ~= 0
                    end
                    __native_condition_2 = __native_condition_3
                    if __native_condition_2 then
                        bVar10 = quest:IsPlayerHoldingLockTargetButton()
                        __native_condition_2 = bVar10
                    end
                    if __native_condition_2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005b1 end
                        iVar14 = quest:AddNewConversation(me, false, false)
                        pCVar20 = quest:GetHero()
                        quest:AddPersonToConversation(iVar14, pCVar20)
                        pCVar13 = quest:GetHero()
                        uVar22 = false
                        pcVar21 = "_THREATEN"
                        pCVar12 = me:GetDataString()
                        pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
                        pCVar12 = (pCVar12 .. pcVar21)
                        quest:AddLineToConversation(iVar14, pCVar12, me, pCVar13, uVar22)
                        quest:SetTimer(xStack_16c, 0x14)
                    end
                    u_stk_170 = uVar5 | 0x40
                    bVar10 = me:MsgIsHitBy("")
                    if bVar10 then
                        goto LAB_00dff3b5
                    else
                        u_stk_170 = uVar5 | 0xc0
                        bVar10 = me:MsgIsHitByAnySpecialAbilityFrom("")
                        if bVar10 then goto LAB_00dff3b5 end
                        goto LAB_00dff3f2
                    end
                    goto FLOW_past_lab_00dff3b5
                    ::LAB_00dff3b5::
                    u_stk_170 = u_stk_170 | 0x100
                    bVar11 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    bVar10 = true
                    if bVar11 then goto LAB_00dff3f2 end
                    ::FLOW_past_lab_00dff3b5::
                    goto FLOW_past_lab_00dff3f2
                    ::LAB_00dff3f2::
                    bVar10 = false
                    ::FLOW_past_lab_00dff3f2::
                    if (u_stk_170 & 0x100) ~= 0 then
                        u_stk_170 = u_stk_170 & 0xfffffeff
                    end
                    if (u_stk_170 & 0x40) ~= 0 then
                        u_stk_170 = u_stk_170 & 0xffffffbf
                    end
                    if bVar10 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005b1 end
                        fret_01 = quest:GetHealth(me)
                        uVar5 = u_stk_170
                        __native_condition_4 = fret_01 <= 5.0
                        if not __native_condition_4 then
                            iVar17 = quest:GetTimer(xStack_154)
                            __native_condition_4 = iVar17 ~= 0
                        end
                        if __native_condition_4 then
                            goto LAB_00dff54a
                        else
                            bVar10 = me:MsgIsHitByHero()
                            uVar5 = u_stk_170 | 0x200
                            if bVar10 then goto LAB_00dff54a end
                            bVar10 = me:MsgIsHitByAnySpecialAbilityFromHero()
                            uVar5 = u_stk_170 | 0x600
                            if bVar10 then
                                bVar10 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                uVar5 = u_stk_170 | 0xe00
                                if not bVar10 then goto LAB_00dff54a end
                            end
                            u_stk_170 = uVar5
                            bVar10 = true
                        end
                        goto FLOW_past_lab_00dff54a
                        ::LAB_00dff54a::
                        u_stk_170 = uVar5
                        bVar10 = false
                        ::FLOW_past_lab_00dff54a::
                        if (u_stk_170 & 0x800) ~= 0 then
                            u_stk_170 = u_stk_170 & 0xfffff7ff
                        end
                        if (u_stk_170 & 0x400) ~= 0 then
                            u_stk_170 = u_stk_170 & 0xfffffbff
                        end
                        if (u_stk_170 & 0x200) ~= 0 then
                            u_stk_170 = u_stk_170 & 0xfffffdff
                        end
                        if bVar10 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar10 = not alive
                            if bVar10 then goto LAB_00e005b1 end
                            pCVar12 = me:GetDataString()
                            iVar17 = ((pCVar12 == "TRADERA") and 0 or 1)
                            c_stk_171 = not (iVar17 ~= 0)
                            if not c_stk_171 then
                                pCVar12 = me:GetDataString()
                                iVar17 = ((pCVar12 == "TRADERB") and 0 or 1)
                                c_stk_171 = not (iVar17 ~= 0)
                                if not c_stk_171 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar10 = not alive
                                    CVar18 = 0x1
                                    if bVar10 then goto LAB_00e005b1 end
                                    pCVar12 = tostring(0x1)
                                    pCVar12 = ("SND_MM_TRADER_C_SCREAM_0" .. pCVar12)
                                    r4 = quest:PlaySoundOnThing(me, pCVar12)
                                    bVar10 = CVar18 == 0x4
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar10 = not alive
                                    CVar18 = 0x1
                                    if bVar10 then goto LAB_00e005b1 end
                                    pCVar12 = tostring(0x1)
                                    pCVar12 = ("SND_MM_TRADER_B_SCREAM_0" .. pCVar12)
                                    r5 = quest:PlaySoundOnThing(me, pCVar12)
                                    bVar10 = CVar18 == 0x3
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar10 = not alive
                                CVar18 = 0x1
                                if bVar10 then goto LAB_00e005b1 end
                                pCVar12 = tostring(0x1)
                                pCVar12 = ("SND_MM_TRADER_A_SCREAM_0" .. pCVar12)
                                r6 = quest:PlaySoundOnThing(me, pCVar12)
                                bVar10 = CVar18 == 0x3
                            end
                            if bVar10 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar10 = not alive
                                if bVar10 then goto LAB_00e005b1 end
                                CVar18 = 0x0
                            end
                            -- TODO(native): xStack_14c = (CCharString)((int)CVar18 + 1);
                            quest:SetTimer(xStack_154, 4)
                        else
                            fret_02 = quest:GetHealth(me)
                            uVar5 = u_stk_170
                            if fret_02 <= 5.0 then
                                goto LAB_00dff887
                            else
                                bVar10 = me:MsgIsHitByHero()
                                uVar5 = u_stk_170 | 0x1000
                                if not bVar10 then
                                    bVar10 = me:MsgIsHitByAnySpecialAbilityFromHero()
                                    uVar5 = u_stk_170 | 0x3000
                                    if bVar10 then
                                        bVar10 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                        uVar5 = u_stk_170 | 0x7000
                                        if not bVar10 then goto LAB_00dff880 end
                                    end
                                    goto LAB_00dff887
                                end
                                ::LAB_00dff880::
                                u_stk_170 = uVar5
                                bVar10 = true
                            end
                            goto FLOW_past_lab_00dff887
                            ::LAB_00dff887::
                            u_stk_170 = uVar5
                            bVar10 = false
                            ::FLOW_past_lab_00dff887::
                            if (u_stk_170 & 0x4000) ~= 0 then
                                u_stk_170 = u_stk_170 & 0xffffbfff
                            end
                            if (u_stk_170 & 0x2000) ~= 0 then
                                u_stk_170 = u_stk_170 & 0xffffdfff
                            end
                            if (u_stk_170 & 0x1000) ~= 0 then
                                u_stk_170 = u_stk_170 & 0xffffefff
                            end
                            if bVar10 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar10 = not alive
                                if bVar10 then goto LAB_00e005b1 end
                                iVar14 = quest:AddNewConversation(me, false, false)
                                pCVar20 = quest:GetHero()
                                quest:AddPersonToConversation(iVar14, pCVar20)
                                pCVar13 = quest:GetHero()
                                uVar22 = false
                                pcVar21 = "_ONHIT"
                                pCVar12 = me:GetDataString()
                                pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
                                pCVar12 = (pCVar12 .. pcVar21)
                                quest:AddLineToConversation(iVar14, pCVar12, me, pCVar13, uVar22)
                            end
                        end
                    end
                    bVar10 = me:MsgIsHitByHero()
                    uVar5 = u_stk_170 | 0x8000
                    if bVar10 then
                        goto LAB_00dffa46
                    else
                        bVar10 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        uVar5 = u_stk_170 | 0x18000
                        if bVar10 then
                            bVar10 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            uVar5 = u_stk_170 | 0x38000
                            if not bVar10 then goto LAB_00dffa46 end
                        end
                        u_stk_170 = uVar5
                        bVar10 = false
                    end
                    goto FLOW_past_lab_00dffa46
                    ::LAB_00dffa46::
                    u_stk_170 = uVar5
                    bVar10 = true
                    ::FLOW_past_lab_00dffa46::
                    if (u_stk_170 & 0x20000) ~= 0 then
                        u_stk_170 = u_stk_170 & 0xfffdffff
                    end
                    if (u_stk_170 & 0x10000) ~= 0 then
                        u_stk_170 = u_stk_170 & 0xfffeffff
                    end
                    if (u_stk_170 & 0x8000) ~= 0 then
                        u_stk_170 = u_stk_170 & 0xffff7fff
                    end
                    native_arg_sequence_3 = false
                    if bVar10 then
                        native_arg_sequence_3 = true
                    else
                        native_arg_sequence_3 = false
                    end
                    if native_arg_sequence_3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then
                            native_arg_sequence_3 = true
                        else
                            native_arg_sequence_3 = false
                        end
                    end
                    if native_arg_sequence_3 then goto LAB_00e005b1 end
                    xStack_168 = me:MsgExpressionPerformedTo()
                    bVar10 = xStack_168 ~= nil
                    if bVar10 then
                        if xStack_168 == nil then
                            bVar10 = false
                            if bVar10 then
                                goto LAB_00dffb06
                            end
                        else
                            iVar17 = ((xStack_168 == "EXPRESSION_WAIT") and 0 or 1)
                            if iVar17 == 0 then goto LAB_00dffb06 end
                        end
                        goto FLOW_past_lab_00dffb06
                        ::LAB_00dffb06::
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005b1 end
                        quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") + -1)
                        ::FLOW_past_lab_00dffb06::
                    end
                    bVar10 = quest:IsRegionLoaded("BanditCampEntrance")
                    c_stk_155 = (1 - (bVar10 and 1 or 0))
                    uVar5 = u_stk_170
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if bVar10 then break end
                r7 = quest:GetThingWithScriptName("TeleporterMarker")
                c_stk_155 = quest:IsRegionLoaded("BanditCampEntrance")
                while c_stk_155 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar10 = not alive
                    if bVar10 then goto LAB_00e005ac end
                    bVar10 = me:IsTalkedToByHero()
                    if bVar10 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005ac end
                        pcVar21 = "_ONTALK_"
                        pCVar12 = me:GetDataString()
                        pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
                        pCVar12 = (pCVar12 .. pcVar21)
                        xStack_15c = pCVar12
                        pCVar12 = tostring(CVar18)
                        pCVar12 = (xStack_15c .. pCVar12)
                        xStack_160 = pCVar12
                        bVar10 = quest:TextEntryExists(xStack_160)
                        if not bVar10 then
                            CVar18 = 0xa
                            pCVar12 = tostring(10)
                            pCVar12 = (xStack_15c .. pCVar12)
                            xStack_160 = pCVar12
                        end
                        -- TODO(native): xStack_118 = (CCharString)((int)CVar18 + 0xa);
                        resources:PrepareResource(xStack_148)
                        bVar10 = resources:TryAcquire(xStack_148, me, 4)
                        while not bVar10 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar10 = not alive
                            if bVar10 then goto LAB_00e005ac end
                            bVar10 = resources:TryAcquire(xStack_148, me, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005ac end
                        xStack_2c = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        xStack_b8 = resources:ScriptThing(xStack_148)
                        pCVar20 = xStack_b8
                        fret_03 = quest:GetHealth(pCVar20)
                        fVar3 = 0.0
                        if fVar3 < fret_03 then
                            iVar28 = 0
                            iVar27 = 1
                            iVar14 = 0
                            iVar17 = 0
                            pvVar16 = xStack_160
                            pCVar20 = quest:GetHero()
                            r8 = me:Speak(pCVar20, pvVar16, iVar17, (iVar14 ~= 0), (iVar27 ~= 0), (iVar28 ~= 0))
                            iVar17 = me:IsPerformingScriptTask()
                            cVar7 = iVar17
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar10 = not alive
                                if bVar10 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_2c)
                                    goto LAB_00e005ac
                                end
                                iVar17 = me:IsPerformingScriptTask()
                                cVar7 = iVar17
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar10 = not alive
                            if bVar10 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_2c)
                                goto LAB_00e005ac
                            end
                        end
                        resources:PrepareResource(xStack_148)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_2c)
                    end
                    iVar17 = quest:GetTimer(xStack_16c)
                    __native_condition_6 = iVar17 == 0
                    if __native_condition_6 then
                        iVar17 = IsPlayerThreateningEntity(me)
                        __native_condition_6 = iVar17 ~= 0
                    end
                    __native_condition_5 = __native_condition_6
                    if __native_condition_5 then
                        bVar10 = quest:IsPlayerHoldingLockTargetButton()
                        __native_condition_5 = bVar10
                    end
                    if __native_condition_5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005ac end
                        iVar14 = quest:AddNewConversation(me, false, false)
                        pCVar20 = quest:GetHero()
                        quest:AddPersonToConversation(iVar14, pCVar20)
                        pCVar13 = quest:GetHero()
                        uVar22 = false
                        pcVar21 = "_THREATEN"
                        pCVar12 = me:GetDataString()
                        pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
                        pCVar12 = (pCVar12 .. pcVar21)
                        quest:AddLineToConversation(iVar14, pCVar12, me, pCVar13, uVar22)
                        quest:SetTimer(xStack_16c, 0x14)
                    end
                    u_stk_170 = uVar5 | 0x40000
                    bVar10 = me:MsgIsHitByHero()
                    if bVar10 then
                        goto LAB_00e0006b
                    else
                        u_stk_170 = uVar5 | 0xc0000
                        bVar10 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if bVar10 then
                            u_stk_170 = uVar5 | 0x1c0000
                            bVar10 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar10 then goto LAB_00e0006b end
                        end
                        bVar10 = false
                    end
                    goto FLOW_past_lab_00e0006b
                    ::LAB_00e0006b::
                    bVar10 = true
                    ::FLOW_past_lab_00e0006b::
                    if (u_stk_170 & 0x100000) ~= 0 then
                        u_stk_170 = u_stk_170 & 0xffefffff
                    end
                    if (u_stk_170 & 0x80000) ~= 0 then
                        u_stk_170 = u_stk_170 & 0xfff7ffff
                    end
                    if (u_stk_170 & 0x40000) ~= 0 then
                        u_stk_170 = u_stk_170 & 0xfffbffff
                    end
                    if bVar10 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005ac end
                        iVar14 = quest:AddNewConversation(me, false, false)
                        pCVar20 = quest:GetHero()
                        quest:AddPersonToConversation(iVar14, pCVar20)
                        pCVar13 = quest:GetHero()
                        uVar22 = false
                        pcVar21 = "_ONHIT"
                        pCVar12 = me:GetDataString()
                        pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
                        pCVar12 = (pCVar12 .. pcVar21)
                        quest:AddLineToConversation(iVar14, pCVar12, me, pCVar13, uVar22)
                    end
                    xStack_168 = me:MsgExpressionPerformedTo()
                    bVar10 = xStack_168 ~= nil
                    if bVar10 then
                        if xStack_168 == nil then
                            bVar10 = false
                            if bVar10 then
                                goto LAB_00e001c0
                            end
                        else
                            iVar17 = ((xStack_168 == "EXPRESSION_WAIT") and 0 or 1)
                            if iVar17 == 0 then goto LAB_00e001c0 end
                        end
                        goto FLOW_past_lab_00e001c0
                        ::LAB_00e001c0::
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005ac end
                        quest:SetStateInt("TradersFollowing", quest:GetStateInt("TradersFollowing") + -1)
                        ::FLOW_past_lab_00e001c0::
                    end
                    bVar10 = quest:IsDistanceBetweenThingsUnder(me, r7, 20.0)
                    if bVar10 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005ac end
                        quest:SetStateInt("TradersReachedTeleporter", quest:GetStateInt("TradersReachedTeleporter") + 1)
                        quest:EntityStopFollowing(me)
                        CVar25 = 0x0
                        pCVar13 = quest:GetHero()
                        quest:SetEntityAsRegionFollowing(pCVar13, me, (CVar25 ~= 0))
                        quest:EntitySetAsScared(me, false)
                        if 2 < quest:GetStateInt("TradersReachedTeleporter") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar10 = not alive
                            if bVar10 then goto LAB_00e005ac end
                            quest:SetStateBool("OutroStart", true)
                            quest:SetStateBool("MissionSucceeded", true)
                            alive = quest:NewScriptFrame(me)
                            require("TraderConflictGood.native_quest_helpers").helper_DFDED0(quest, me, "CS_TRADERCON_GOOD_OUTRO")
                            quest:SetStateBool("OutroDone", true)
                            goto LAB_00e00595
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005ac end
                        resources:PrepareResource(xStack_148)
                        bVar10 = resources:TryAcquire(xStack_148, me, 4)
                        goto LAB_00e0035a
                    end
                    c_stk_155 = quest:IsRegionLoaded("BanditCampEntrance")
                    uVar5 = u_stk_170
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if bVar10 then goto LAB_00e005ac end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
            end
            goto LAB_00e005b1
        end
    end
    ::FLOW_past_lab_00dfebc5::
    goto LAB_00e005d5
    ::LAB_00e0035a::
    if bVar10 then goto LAB_00e0038b end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar10 = not alive
    if bVar10 then goto LAB_00e005ac end
    bVar10 = resources:TryAcquire(xStack_148, me, 4)
    goto LAB_00e0035a
    ::LAB_00e0038b::
    alive = not quest:IsActiveThreadTerminating()
    bVar10 = not alive
    if not bVar10 then
        iVar14 = quest:AddNewConversation(me, false, false)
        pCVar20 = quest:GetHero()
        quest:AddPersonToConversation(iVar14, pCVar20)
        pCVar13 = quest:GetHero()
        uVar22 = false
        pcVar21 = "_FREED_10"
        pCVar12 = me:GetDataString()
        pCVar12 = ("TEXT_QST_B11_" .. pCVar12)
        pCVar12 = (pCVar12 .. pcVar21)
        quest:AddLineToConversation(iVar14, pCVar12, me, pCVar13, uVar22)
        bVar10 = quest:IsDistanceBetweenThingsOver(me, r7, 2.0)
        if bVar10 then
            repeat
                if quest:GetStateBool("OutroStart") then break end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if bVar10 then goto LAB_00e005ac end
                iVar17 = me:IsPerformingScriptTask()
                if not iVar17 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar10 = not alive
                    if bVar10 then goto LAB_00e005ac end
                    me:MoveToThing(r7, 1.0, 1)
                end
                bVar10 = quest:IsDistanceBetweenThingsOver(me, r7, 2.0)
            until not (bVar10)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar10 = not alive
        if not bVar10 then
            if not quest:GetStateBool("OutroStart") then
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if not bVar10 then
                    quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("BarIndex"))
                    quest:FadeOutAndKillEntity(me, true, 1.0, true)
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar10 = not alive
                if not bVar10 then
                    me:ClearCommands()
                    resources:PrepareResource(xStack_148)
                    cVar7 = quest:GetStateBool("OutroDone")
                    while not cVar7 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar10 = not alive
                        if bVar10 then goto LAB_00e005ac end
                        cVar7 = quest:GetStateBool("OutroDone")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar10 = not alive
                    if not bVar10 then
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
    quest:DeregisterTimer(xStack_164)
    quest:DeregisterTimer(xStack_16c)
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

