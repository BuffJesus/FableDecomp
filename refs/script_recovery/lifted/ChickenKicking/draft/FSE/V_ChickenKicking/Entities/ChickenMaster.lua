-- Generated native draft: ChickenMaster. Review coverage report before use.
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
    local CVar17, CVar27, C_stk_370, __native_condition_1, __native_condition_2, __native_condition_3, __native_condition_4, bVar6, bVar8, cVar7, c_stk_381, c_stk_399, fVar28, fVar4, f_stk_374, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, iVar10, iVar11, iVar22, i_stk_190, i_stk_1d0, i_stk_210, i_stk_3a0, i_stk_3a4, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, native_arg_sequence_4, pCVar12, pCVar13, pCVar14, pCVar15, pCVar16, pCVar9, pcVar20, piVar2, puVar1, puVar29, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r3, r4, r5, r6, r7, r8, r9, thing_b10, thing_b11, thing_b8, thing_b9, this_00, uVar18, vec_188, xStack_154, xStack_164, xStack_32c, xStack_33c, xStack_34c, xStack_394, xStack_398, x_stk_124, x_stk_170, x_stk_17c, x_stk_30, x_stk_90, x_stk_c0
    local alive = true
    C_stk_370 = 0
    xStack_394 = resources:NewResource()
    resources:PrepareResource(xStack_394)
    bVar6 = resources:TryAcquire(xStack_394, me, 4)
    while not bVar6 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            resources:ReleaseResource(xStack_394)
            return
        end
        bVar6 = resources:TryAcquire(xStack_394, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar6 = not alive
    if not bVar6 then
        resources:AssignResource(resources:MemberResource("seh_ChickenMaster"), xStack_394)
        quest:EntitySetAppearanceMorphSeed(me, 4)
        quest:SetThingHasInformation(me, false, false, false)
        thing_b8 = piVar2
        thing_b9 = (piVar2 >> 8)
        thing_b10 = (piVar2 >> 0x10)
        thing_b11 = (piVar2 >> 0x18)
        quest:SetIsPushableByHero(me, false)
        quest:EntitySetAsKillable(me, false, true)
        pCVar9 = me:GetPos()
        vec_188 = {x = pCVar9.x, y = pCVar9.y, z = pCVar9.z}
        iVar10 = quest:RegisterTimer()
        i_stk_3a4 = iVar10
        quest:SetTimer(i_stk_3a4, 0)
        iVar11 = quest:RegisterTimer()
        i_stk_3a0 = iVar11
        quest:SetTimer(i_stk_3a0, 0)
        c_stk_381 = 0
        if quest:GetStateBool("KnowGhostHasGone") then
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then
                quest:DeregisterTimer(i_stk_3a0)
                quest:DeregisterTimer(i_stk_3a4)
                resources:ReleaseResource(xStack_394)
                return
            end
            CVar27 = 0x0
            pCVar12 = quest:GetThingWithScriptName("MK_CK_ORG")
            quest:EntityTeleportToThing(me, pCVar12, (CVar27 ~= 0))
        end
        cVar7 = quest:GetStateBool("KnowGhostHasGone")
        while not cVar7 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if bVar6 then goto LAB_00e68af4 end
            fVar28 = 5.5
            pCVar12 = quest:GetHero()
            bVar6 = quest:IsDistanceBetweenThingsUnder(pCVar12, me, fVar28)
            __native_condition_2 = bVar6
            if __native_condition_2 then
                iVar10 = (me ~= nil and me:IsDistanceFromPositionUnder(vec_188, 6.0))
                __native_condition_2 = iVar10
            end
            __native_condition_1 = __native_condition_2
            if __native_condition_1 then
                iVar10 = quest:GetTimer(i_stk_3a4)
                __native_condition_1 = iVar10 < 1
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00e68af4 end
                CVar27 = 0x0
                pCVar12 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, pCVar12, (CVar27 ~= 0))
                iVar11 = quest:AddNewConversation(me, false, false)
                pCVar12 = quest:GetHero()
                quest:AddPersonToConversation(iVar11, pCVar12)
                if not __native_entity_state:GetStateBool("GhostChat") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e68af4 end
                    pCVar12 = quest:GetHero()
                    quest:AddLineToConversation(iVar11, "TEXT_QST_B17_MASTER_EARLY_ASIDE_FIRST_PRECHAT_10", me, pCVar12, false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e68af4 end
                    pCVar12 = quest:GetHero()
                    quest:AddLineToConversation(iVar11, "TEXT_QST_B17_MASTER_EARLY_ASIDE_FIRST_POSTCHAT", me, pCVar12, false)
                end
                iVar10 = i_stk_3a4
                quest:SetTimer(i_stk_3a4, 8)
                iVar11 = quest:GetTimer(i_stk_3a4)
                while ((0 < iVar11 and (c_stk_381 == 0)) and (not quest:GetStateBool("TalkedTo"))) do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then
                        quest:DeregisterTimer(i_stk_3a0)
                        quest:DeregisterTimer(i_stk_3a4)
                        resources:ReleaseResource(xStack_394)
                        return
                    end
                    cVar7 = me:IsTalkedToByHero()
                    if cVar7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            quest:DeregisterTimer(i_stk_3a4)
                            quest:DeregisterTimer(i_stk_3a4)
                            resources:ReleaseResource(xStack_394)
                            return
                        end
                        quest:SetStateBool("TalkedTo", true)
                    end
                    -- TODO(native): ctr_CVar19 = xStack_374;
                    -- TODO(native): xStack_374 = xStack_374 | 1;
                    cVar7 = me:MsgIsHitByHero()
                    if not cVar7 then
                        CVar17 = ctr_CVar19 | 3
                        C_stk_370 = CVar17
                        cVar7 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if cVar7 then
                            CVar17 = ctr_CVar19 | 7
                            C_stk_370 = CVar17
                            cVar7 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not cVar7 then goto LAB_00e654e0 end
                        end
                        c_stk_399 = 0
                    else
                        goto LAB_00e654e0
                    end
                    goto FLOW_past_lab_00e654e0
                    ::LAB_00e654e0::
                    c_stk_399 = 1
                    ::FLOW_past_lab_00e654e0::
                    if (CVar17 & 4) ~= 0 then
                        CVar17 = CVar17 & 0xfffffffb
                        C_stk_370 = CVar17
                    end
                    if (CVar17 & 2) ~= 0 then
                        CVar17 = CVar17 & 0xfffffffd
                        C_stk_370 = CVar17
                    end
                    if (CVar17 & 1) ~= 0 then
                        C_stk_370 = CVar17 & 0xfffffffe
                    end
                    if c_stk_399 ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then
                            -- LAB_00e660d3: (native jump target)
                            quest:DeregisterTimer(i_stk_3a0)
                            quest:DeregisterTimer(i_stk_3a4)
                            resources:ReleaseResource(xStack_394)
                            return
                        end
                        c_stk_381 = 1
                    end
                    iVar11 = quest:GetTimer(i_stk_3a4)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00e68af4 end
                quest:SetTimer(i_stk_3a4, 0xf)
            end
            -- TODO(native): ctr_CVar19 = C_stk_370;
            uVar18 = C_stk_370 | 8
            cVar7 = me:MsgIsHitByHero()
            if not cVar7 then
                uVar18 = ctr_CVar19 | 0x18
                cVar7 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if cVar7 then
                    uVar18 = ctr_CVar19 | 0x38
                    cVar7 = me:MsgIsHitByHeroSpecialAbility(0xe)
                    if not cVar7 then goto LAB_00e65617 end
                end
                c_stk_399 = 0
                if c_stk_381 ~= 0 then goto LAB_00e65617 end
            else
                goto LAB_00e65617
            end
            goto FLOW_past_lab_00e65617
            ::LAB_00e65617::
            c_stk_399 = 1
            ::FLOW_past_lab_00e65617::
            if (uVar18 & 0x20) ~= 0 then
                uVar18 = uVar18 & 0xffffffdf
            end
            if (uVar18 & 0x10) ~= 0 then
                uVar18 = uVar18 & 0xffffffef
            end
            if (uVar18 & 8) ~= 0 then
                uVar18 = uVar18 & 0xfffffff7
            end
            if c_stk_399 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00e68af4 end
                c_stk_381 = bVar6
                xStack_33c = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                require("V_ChickenKicking.native_quest_helpers").helper_E68B20(quest, me, "CS_CHICKING_HITGUYTOP")
                quest:SetStateBool("RanOff", true)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_33c)
            end
            C_stk_370 = (uVar18 | 0x40)
            cVar7 = me:IsTalkedToByHero()
            native_arg_sequence_1 = false
            if cVar7 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                c_stk_399 = 0
                if quest:GetStateBool("TalkedTo") then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then
                c_stk_399 = 1
            end
            C_stk_370 = (uVar18 & 0xffffffbf)
            if c_stk_399 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                if bVar6 then goto LAB_00e68af4 end
                quest:SetStateBool("TalkedTo", false)
                CVar27 = 0x0
                pCVar13 = quest:GetActiveQuestName()
                quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_CHICKEN_KICKING", pCVar13, (CVar27 ~= 0))
                pCVar14 = quest:GetActiveQuestName()
                quest:SetQuestCardObjective(pCVar14, "TEXT_QUEST_CHICKEN_KICKING_OBJECTIVE_01", "", "")
                if not __native_entity_state:GetStateBool("GhostChat") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e68af4 end
                    __native_entity_state:SetStateBool("GhostChat", true)
                    xStack_32c = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    pCVar15 = quest:GetThingWithScriptName("GhostFisherman")
                    bVar6 = (pCVar15 ~= nil and pCVar15:IsAlive())
                    pCVar15 = nil
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                    if bVar6 then
                        if bVar8 then
                            goto LAB_00e66150
                        end
                        goto FLOW_past_lab_00e66150
                        ::LAB_00e66150::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_32c)
                        goto LAB_00e68af4
                        ::FLOW_past_lab_00e66150::
                        x_stk_17c = resources:ScriptThing(xStack_394)
                        pCVar15 = x_stk_17c
                        fret_0 = quest:GetHealth(pCVar15)
                        fVar4 = 0.0
                        if fVar4 < fret_0 then
                            iVar22 = 0
                            iVar11 = 0
                            iVar10 = 0
                            pcVar20 = "TEXT_QST_B17_MASTER_INITIAL_MEETING"
                            pCVar15 = quest:GetHero()
                            r1 = me:Speak(pCVar15, pcVar20, iVar10, (iVar11 ~= 0), true, (iVar22 ~= 0))
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar16 = xStack_32c
                                    -- LAB_00e661a5: (native jump target)
                                    quest:DeregisterTimer(i_stk_3a0)
                                    quest:DeregisterTimer(i_stk_3a4)
                                    resources:ReleaseResource(xStack_394)
                                    return
                                end
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e66150 end
                        end
                    else
                        if bVar8 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_32c)
                            goto LAB_00e68af4
                        end
                        require("V_ChickenKicking.native_quest_helpers").helper_E68B20(quest, me, "CS_CHICKING_INITIALWALK1")
                        quest:SetStateBool("KnowGhostHasGone", true)
                        pCVar14 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(pCVar14, "TEXT_QUEST_CHICKEN_KICKING_OBJECTIVE_02", "", "")
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar16 = xStack_32c
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                    if bVar6 then goto LAB_00e68af4 end
                    xStack_34c = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    pCVar15 = quest:GetThingWithScriptName("GhostFisherman")
                    bVar6 = (pCVar15 ~= nil and pCVar15:IsAlive())
                    pCVar15 = nil
                    alive = not quest:IsActiveThreadTerminating()
                    bVar8 = not alive
                    if bVar6 then
                        if bVar8 then
                            goto LAB_00e661e4
                        end
                        goto FLOW_past_lab_00e661e4
                        ::LAB_00e661e4::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_34c)
                        goto LAB_00e68af4
                        ::FLOW_past_lab_00e661e4::
                        x_stk_170 = resources:ScriptThing(xStack_394)
                        pCVar15 = x_stk_170
                        fret_00 = quest:GetHealth(pCVar15)
                        fVar4 = 0.0
                        if fVar4 < fret_00 then
                            iVar22 = 0
                            iVar11 = 0
                            iVar10 = 0
                            pcVar20 = "TEXT_QST_B17_MASTER_RETURN_MEETING"
                            pCVar15 = quest:GetHero()
                            r2 = me:Speak(pCVar15, pcVar20, iVar10, (iVar11 ~= 0), true, (iVar22 ~= 0))
                            iVar10 = me:IsPerformingScriptTask()
                            cVar7 = iVar10
                            while cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    pCVar16 = xStack_34c
                                    quest:DeregisterTimer(i_stk_3a0)
                                    quest:DeregisterTimer(i_stk_3a4)
                                    resources:ReleaseResource(xStack_394)
                                    return
                                end
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e661e4 end
                        end
                    else
                        if bVar8 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_34c)
                            goto LAB_00e68af4
                        end
                        require("V_ChickenKicking.native_quest_helpers").helper_E68B20(quest, me, "CS_CHICKING_INITIALWALK2")
                        quest:SetStateBool("KnowGhostHasGone", true)
                        pCVar14 = quest:GetActiveQuestName()
                        quest:SetQuestCardObjective(pCVar14, "TEXT_QUEST_CHICKEN_KICKING_OBJECTIVE_02", "", "")
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    pCVar16 = xStack_34c
                end
            end
            cVar7 = quest:GetStateBool("KnowGhostHasGone")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if not bVar6 then
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar6 = not alive
            if not bVar6 then
                r3 = quest:GetThingWithScriptName("MK_CK_ORG")
                alive = not quest:IsActiveThreadTerminating()
                bVar6 = not alive
                while not bVar6 do
                    fVar28 = 5.5
                    pCVar12 = quest:GetHero()
                    bVar6 = quest:IsDistanceBetweenThingsUnder(pCVar12, me, fVar28)
                    __native_condition_4 = bVar6
                    if __native_condition_4 then
                        bVar6 = quest:IsDistanceBetweenThingsUnder(r3, me, 6.0)
                        __native_condition_4 = bVar6
                    end
                    __native_condition_3 = __native_condition_4
                    if __native_condition_3 then
                        iVar10 = quest:GetTimer(i_stk_3a4)
                        __native_condition_3 = iVar10 < 1
                    end
                    if __native_condition_3 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then break end
                        CVar27 = 0x0
                        pCVar12 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar12, (CVar27 ~= 0))
                        iVar11 = quest:AddNewConversation(me, false, false)
                        pCVar12 = quest:GetHero()
                        quest:AddPersonToConversation(iVar11, pCVar12)
                        pCVar12 = quest:GetHero()
                        quest:AddLineToConversation(iVar11, "TEXT_QST_B17_MASTER_GAME_ASIDE", me, pCVar12, false)
                        iVar10 = i_stk_3a4
                        quest:SetTimer(i_stk_3a4, 8)
                        iVar11 = quest:GetTimer(i_stk_3a4)
                        while ((0 < iVar11 and (not c_stk_381)) and (not quest:GetStateBool("TalkedTo"))) do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e68aeb end
                            cVar7 = me:IsTalkedToByHero()
                            if cVar7 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e68aeb end
                                quest:SetStateBool("TalkedTo", true)
                            end
                            -- TODO(native): ctr_CVar19 = C_stk_370;
                            C_stk_370 = C_stk_370 | 0x80
                            cVar7 = me:MsgIsHitByHero()
                            if not cVar7 then
                                CVar17 = ctr_CVar19 | 0x180
                                C_stk_370 = CVar17
                                cVar7 = me:MsgIsHitByAnySpecialAbilityFromHero()
                                if cVar7 then
                                    CVar17 = ctr_CVar19 | 0x380
                                    C_stk_370 = CVar17
                                    cVar7 = me:MsgIsHitByHeroSpecialAbility(0xe)
                                    if not cVar7 then goto LAB_00e661fd end
                                end
                                c_stk_399 = 0
                            else
                                goto LAB_00e661fd
                            end
                            goto FLOW_past_lab_00e661fd
                            ::LAB_00e661fd::
                            c_stk_399 = 1
                            ::FLOW_past_lab_00e661fd::
                            if (CVar17 & 0x200) ~= 0 then
                                CVar17 = CVar17 & 0xfffffdff
                                C_stk_370 = CVar17
                            end
                            if (CVar17 & 0x100) ~= 0 then
                                CVar17 = CVar17 & 0xfffffeff
                                C_stk_370 = CVar17
                            end
                            if (CVar17 & 0x80) ~= 0 then
                                C_stk_370 = CVar17 & 0xffffff7f
                            end
                            if c_stk_399 ~= 0 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e68aeb end
                                c_stk_381 = 1
                            end
                            iVar11 = quest:GetTimer(i_stk_3a4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then break end
                        quest:SetTimer(i_stk_3a4, 0xf)
                    end
                    -- TODO(native): ctr_CVar19 = C_stk_370;
                    uVar18 = C_stk_370 | 0x400
                    cVar7 = me:MsgIsHitByHero()
                    if not cVar7 then
                        uVar18 = ctr_CVar19 | 0xc00
                        cVar7 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if cVar7 then
                            uVar18 = ctr_CVar19 | 0x1c00
                            cVar7 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not cVar7 then goto LAB_00e6634f end
                        end
                        native_arg_sequence_2 = false
                        if c_stk_381 ~= 0 then
                            native_arg_sequence_2 = true
                        else
                            native_arg_sequence_2 = false
                        end
                        if not native_arg_sequence_2 then
                            c_stk_399 = 0
                            if quest:GetStateBool("SpectatorsUnderAttack") then
                                native_arg_sequence_2 = true
                            else
                                native_arg_sequence_2 = false
                            end
                        end
                        if native_arg_sequence_2 then goto LAB_00e6634f end
                    else
                        goto LAB_00e6634f
                    end
                    goto FLOW_past_lab_00e6634f
                    ::LAB_00e6634f::
                    c_stk_399 = 1
                    ::FLOW_past_lab_00e6634f::
                    if (uVar18 & 0x1000) ~= 0 then
                        uVar18 = uVar18 & 0xffffefff
                    end
                    if (uVar18 & 0x800) ~= 0 then
                        uVar18 = uVar18 & 0xfffff7ff
                    end
                    if (uVar18 & 0x400) ~= 0 then
                        uVar18 = uVar18 & 0xfffffbff
                    end
                    if c_stk_399 ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then break end
                        quest:SetStateBool("SpectatorsUnderAttack", false)
                        c_stk_381 = bVar6
                        xStack_154 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        require("V_ChickenKicking.native_quest_helpers").helper_E68B20(quest, me, "CS_CHICKING_HITGUYBOTTOM")
                        quest:SetStateBool("RanOff", true)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_154)
                    end
                    C_stk_370 = (uVar18 | 0x2000)
                    cVar7 = me:IsTalkedToByHero()
                    native_arg_sequence_3 = false
                    if cVar7 then
                        native_arg_sequence_3 = true
                    else
                        native_arg_sequence_3 = false
                    end
                    if not native_arg_sequence_3 then
                        c_stk_399 = 0
                        if quest:GetStateBool("TalkedTo") then
                            native_arg_sequence_3 = true
                        else
                            native_arg_sequence_3 = false
                        end
                    end
                    if native_arg_sequence_3 then
                        c_stk_399 = 1
                    end
                    C_stk_370 = (uVar18 & 0xffffdfff)
                    if c_stk_399 ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then break end
                        quest:SetStateBool("TalkedTo", false)
                        if __native_entity_state:GetStateBool("HaveTalked") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then break end
                            xStack_33c = resources:StartMovie("")
                            pCVar15 = 0x1
                            quest:PauseAllNonScriptedEntities(true)
                            x_stk_170 = resources:ScriptThing(xStack_394)
                            pCVar12 = x_stk_170
                            fret_01 = quest:GetHealth(pCVar12)
                            fVar4 = 0.0
                            if fret_01 <= fVar4 then
                                goto LAB_00e665da
                            else
                                iVar22 = 1
                                iVar11 = 0
                                iVar10 = 0
                                pcVar20 = "TEXT_QST_B17_MASTER_GREETING_RETURN"
                                pCVar12 = quest:GetHero()
                                r4 = me:Speak(pCVar12, pcVar20, iVar10, (iVar11 ~= 0), (iVar22 ~= 0), false)
                                iVar10 = me:IsPerformingScriptTask()
                                cVar7 = iVar10
                                while cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e667c9 end
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar7 = iVar10
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if not bVar6 then goto LAB_00e665da end
                            end
                            goto FLOW_past_lab_00e665da
                            ::LAB_00e665da::
                            quest:GiveHeroYesNoQuestion("TEXT_QST_B17_GREETING_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while iVar10 < 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e667c9 end
                                iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if not bVar6 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if iVar10 == 1 then
                                    if not bVar6 then
                                        iVar10 = quest:GetHeroGold()
                                        if iVar10 < 0x32 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if not bVar6 then
                                                x_stk_30 = resources:ScriptThing(xStack_394)
                                                pCVar12 = x_stk_30
                                                fret_04 = quest:GetHealth(pCVar12)
                                                fVar4 = 0.0
                                                if fVar4 < fret_04 then
                                                    iVar22 = 1
                                                    iVar11 = 0
                                                    iVar10 = 0
                                                    pcVar20 = "TEXT_QST_B17_MASTER_NO_MONEY"
                                                    pCVar15 = quest:GetHero()
                                                    r5 = me:Speak(pCVar15, pcVar20, iVar10, (iVar11 ~= 0), (iVar22 ~= 0), false)
                                                    iVar10 = me:IsPerformingScriptTask()
                                                    cVar7 = iVar10
                                                    while cVar7 do
                                                        alive = quest:NewScriptFrame(me)
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar6 = not alive
                                                        if bVar6 then goto LAB_00e667c9 end
                                                        iVar10 = me:IsPerformingScriptTask()
                                                        cVar7 = iVar10
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if bVar6 then goto LAB_00e66c23 end
                                                end
                                                goto LAB_00e66ceb
                                            end
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if not bVar6 then
                                                if not __native_entity_state:GetStateBool("HeroHasPlayed") then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if bVar6 then goto LAB_00e66c23 end
                                                    x_stk_c0 = resources:ScriptThing(xStack_394)
                                                    pCVar12 = x_stk_c0
                                                    fret_03 = quest:GetHealth(pCVar12)
                                                    fVar4 = 0.0
                                                    if fVar4 < fret_03 then
                                                        iVar22 = 1
                                                        iVar11 = 0
                                                        iVar10 = 0
                                                        pcVar20 = "TEXT_QST_B17_MASTER_START_KICKING"
                                                        pCVar12 = quest:GetHero()
                                                        r6 = me:Speak(pCVar12, pcVar20, iVar10, (iVar11 ~= 0), (iVar22 ~= 0), false)
                                                        iVar10 = me:IsPerformingScriptTask()
                                                        cVar7 = iVar10
                                                        while cVar7 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar6 = not alive
                                                            if bVar6 then goto LAB_00e667c9 end
                                                            iVar10 = me:IsPerformingScriptTask()
                                                            cVar7 = iVar10
                                                        end
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar6 = not alive
                                                        if bVar6 then goto LAB_00e66c23 end
                                                    end
                                                    __native_entity_state:SetStateBool("HeroHasPlayed", true)
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if bVar6 then goto LAB_00e667c9 end
                                                    x_stk_17c = resources:ScriptThing(xStack_394)
                                                    pCVar12 = x_stk_17c
                                                    fret_02 = quest:GetHealth(pCVar12)
                                                    fVar4 = 0.0
                                                    if fVar4 < fret_02 then
                                                        iVar22 = 1
                                                        iVar11 = 0
                                                        iVar10 = 0
                                                        pcVar20 = "TEXT_QST_B17_MASTER_START_KICKING_RETURN"
                                                        pCVar12 = quest:GetHero()
                                                        r7 = me:Speak(pCVar12, pcVar20, iVar10, (iVar11 ~= 0), (iVar22 ~= 0), false)
                                                        iVar10 = me:IsPerformingScriptTask()
                                                        cVar7 = iVar10
                                                        while cVar7 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar6 = not alive
                                                            if bVar6 then goto LAB_00e66c23 end
                                                            iVar10 = me:IsPerformingScriptTask()
                                                            cVar7 = iVar10
                                                        end
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar6 = not alive
                                                        if bVar6 then goto LAB_00e667c9 end
                                                    end
                                                end
                                                quest:FadeScreenOut(0.5, 0.5)
                                                if (quest:GetStateBool("KnowGhostHasGone")) and (not quest:GetStateBool("SpectatorsCreated")) then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if bVar6 then goto LAB_00e667c9 end
                                                    pCVar12 = quest:GetThingWithScriptName("Spectator1")
                                                    bVar6 = false
                                                    pCVar9 = pCVar12:GetPos()
                                                    r8 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", pCVar9, "Spectator")
                                                    pCVar12 = quest:GetThingWithScriptName("Spectator2")
                                                    bVar6 = false
                                                    pCVar9 = pCVar12:GetPos()
                                                    r9 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", pCVar9, "Spectator")
                                                    pCVar12 = quest:GetThingWithScriptName("Spectator3")
                                                    bVar6 = false
                                                    pCVar13 = "Spectator"
                                                    pCVar9 = pCVar12:GetPos()
                                                    r10 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", pCVar9, pCVar13)
                                                    quest:SetStateBool("SpectatorsCreated", true)
                                                end
                                                quest:ConfiscateAllHeroWeapons()
                                                quest:SetHeroWillAsUsable(false)
                                                quest:SetTeleportingAsActive(false)
                                                quest:GiveHeroGold(-0x32)
                                                __native_entity_state:SetStateBool("KickingChickens", true)
                                                goto LAB_00e66cef
                                            end
                                        end
                                        goto LAB_00e66c23
                                    end
                                elseif not bVar6 then
                                    x_stk_124 = resources:ScriptThing(xStack_394)
                                    pCVar12 = x_stk_124
                                    fret_05 = quest:GetHealth(pCVar12)
                                    fVar4 = 0.0
                                    if fVar4 < fret_05 then
                                        iVar22 = 1
                                        iVar11 = 0
                                        iVar10 = 0
                                        pcVar20 = "TEXT_QST_B17_MASTER_DONT_KICK"
                                        pCVar15 = quest:GetHero()
                                        r11 = me:Speak(pCVar15, pcVar20, iVar10, (iVar11 ~= 0), (iVar22 ~= 0), false)
                                        iVar10 = me:IsPerformingScriptTask()
                                        cVar7 = iVar10
                                        while cVar7 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if bVar6 then goto LAB_00e66c23 end
                                            iVar10 = me:IsPerformingScriptTask()
                                            cVar7 = iVar10
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e667c9 end
                                    end
                                    goto LAB_00e66ceb
                                end
                                goto FLOW_past_lab_00e66ceb
                                ::LAB_00e66ceb::
                                __native_entity_state:SetStateBool("KickingChickens", false)
                                ::LAB_00e66cef::
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = xStack_33c
                                goto LAB_00e6762e
                                ::FLOW_past_lab_00e66ceb::
                                goto LAB_00e667c9
                            end
                            goto FLOW_past_lab_00e667c9
                            ::LAB_00e667c9::
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_33c)
                            break
                            ::FLOW_past_lab_00e667c9::
                            ::FLOW_past_lab_00e665da::
                            ::LAB_00e66c23::
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_33c)
                            break
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then break end
                        xStack_32c = resources:StartMovie("")
                        -- TODO(native): ctr_CVar19 = *(int **)(this + 4);
                        -- TODO(native): (**(code **)(*CVar19 + 0x5ec))(ctr_CVar19,true);
                        __native_entity_state:SetStateBool("HaveTalked", true)
                        iVar10 = me:IsPerformingScriptTask()
                        native_arg_sequence_4 = false
                        if not iVar10 then
                            native_arg_sequence_4 = true
                        else
                            native_arg_sequence_4 = false
                        end
                        if not native_arg_sequence_4 then
                            bVar6 = quest:IsDistanceBetweenThingsOver(r3, me, 6.0)
                            if not bVar6 then
                                native_arg_sequence_4 = true
                            else
                                native_arg_sequence_4 = false
                            end
                        end
                        if native_arg_sequence_4 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if not bVar6 then
                                quest:FadeScreenOut(0.5, 0.5)
                                if (quest:GetStateBool("KnowGhostHasGone")) and (not quest:GetStateBool("SpectatorsCreated")) then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then
                                        goto LAB_00e68a59
                                    end
                                    goto FLOW_hoist_lab_00e68a59_1
                                end
                                goto FLOW_hoist_lab_00e68a59_2
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if not bVar6 then
                                me:ClearCommands()
                                x_stk_90 = resources:ScriptThing(xStack_394)
                                pCVar15 = x_stk_90
                                fret_06 = quest:GetHealth(pCVar15)
                                fVar4 = 0.0
                                if fVar4 < fret_06 then
                                    iVar22 = 0
                                    iVar11 = 0
                                    iVar10 = 0
                                    pcVar20 = "TEXT_QST_B17_MASTER_QUICK_WALK"
                                    pCVar15 = quest:GetHero()
                                    r12 = me:Speak(pCVar15, pcVar20, iVar10, (iVar11 ~= 0), true, (iVar22 ~= 0))
                                    iVar10 = me:IsPerformingScriptTask()
                                    cVar7 = iVar10
                                    while cVar7 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e68a59 end
                                        iVar10 = me:IsPerformingScriptTask()
                                        cVar7 = iVar10
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e68a3c end
                                end
                                quest:FadeScreenOut(0.5, 0.5)
                                CVar27 = 0x0
                                pCVar15 = quest:GetThingWithScriptName("MK_CK_ORG")
                                quest:EntityTeleportToThing(me, pCVar15, (CVar27 ~= 0))
                                if (not quest:GetStateBool("KnowGhostHasGone")) or (quest:GetStateBool("SpectatorsCreated")) then
                                    goto LAB_00e6743e
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if not bVar6 then
                                        pCVar15 = quest:GetThingWithScriptName("Spectator1")
                                        CVar27 = 0x0
                                        pCVar9 = pCVar15:GetPos()
                                        r13 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", pCVar9, "Spectator")
                                        pCVar15 = quest:GetThingWithScriptName("Spectator2")
                                        CVar27 = 0x0
                                        pCVar9 = pCVar15:GetPos()
                                        r14 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", pCVar9, "Spectator")
                                        pCVar15 = quest:GetThingWithScriptName("Spectator3")
                                        CVar27 = 0x0
                                        pCVar9 = pCVar15:GetPos()
                                        r15 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", pCVar9, "Spectator")
                                        quest:SetStateBool("SpectatorsCreated", true)
                                        goto LAB_00e6743e
                                    end
                                    goto LAB_00e68a8f
                                end
                                goto FLOW_hoist_lab_00e68a8f_1
                            end
                        end
                        goto FLOW_past_lab_00e68a8f
                        ::LAB_00e68a8f::
                        -- TODO(native): (**(code **)(*(int *)xStack_374 + 0x5ec))((void *)xStack_374,false);
                        resources:DestroyMovie(xStack_32c)
                        ::FLOW_hoist_lab_00e68a8f_1::
                        goto FLOW_hoist_lab_00e6743e_1
                        ::FLOW_past_lab_00e68a8f::
                        goto FLOW_past_lab_00e68a59
                        ::LAB_00e68a59::
                        -- TODO(native): (**(code **)(*CVar19 + 0x5ec))(ctr_CVar19,false);
                        resources:DestroyMovie(xStack_32c)
                        break
                        ::FLOW_hoist_lab_00e68a59_1::
                        pCVar12 = quest:GetThingWithScriptName("Spectator1")
                        CVar27 = 0x0
                        pCVar9 = pCVar12:GetPos()
                        r16 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", pCVar9, "Spectator")
                        pCVar12 = quest:GetThingWithScriptName("Spectator2")
                        CVar27 = 0x0
                        pCVar9 = pCVar12:GetPos()
                        r17 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", pCVar9, "Spectator")
                        pCVar12 = quest:GetThingWithScriptName("Spectator3")
                        CVar27 = 0x0
                        pCVar9 = pCVar12:GetPos()
                        r18 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", pCVar9, "Spectator")
                        quest:SetStateBool("SpectatorsCreated", true)
                        ::FLOW_hoist_lab_00e68a59_2::
                        CVar27 = 0x0
                        pCVar12 = quest:GetThingWithScriptName("MK_CK_ORG")
                        quest:EntityTeleportToThing(me, pCVar12, (CVar27 ~= 0))
                        goto LAB_00e6743e
                        ::FLOW_past_lab_00e68a59::
                        goto FLOW_past_lab_00e6743e
                        ::LAB_00e6743e::
                        require("V_ChickenKicking.native_quest_helpers").helper_E68B20(quest, me, "CS_CHICKING_INTRO")
                        quest:FadeScreenIn()
                        quest:GiveHeroYesNoQuestion("TEXT_QST_B17_GREETING_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar10 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if bVar6 then goto LAB_00e68a73 end
                            iVar10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if bVar6 then goto LAB_00e68a8f end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if iVar10 == 1 then
                            if not bVar6 then
                                iVar10 = quest:GetHeroGold()
                                if 0x31 < iVar10 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e68a8f end
                                    require("V_ChickenKicking.native_quest_helpers").helper_E68B20(quest, me, "CS_CHICKING_START")
                                    __native_entity_state:SetStateBool("HeroHasPlayed", true)
                                    quest:ConfiscateAllHeroWeapons()
                                    quest:SetHeroWillAsUsable(false)
                                    quest:SetTeleportingAsActive(false)
                                    quest:GiveHeroGold(-0x32)
                                    __native_entity_state:SetStateBool("KickingChickens", true)
                                    goto LAB_00e67619
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if not bVar6 then
                                    pcVar20 = "CS_CHICKING_NOCASH"
                                    goto LAB_00e6760c
                                end
                            end
                            goto LAB_00e68a73
                        else
                            if bVar6 then goto LAB_00e68a8f end
                            pcVar20 = "CS_CHICKING_NOKICK"
                            goto LAB_00e6760c
                        end
                        goto FLOW_past_lab_00e6760c
                        ::LAB_00e6760c::
                        require("V_ChickenKicking.native_quest_helpers").helper_E68B20(quest, me, pcVar20)
                        goto LAB_00e67619
                        ::FLOW_past_lab_00e6760c::
                        goto FLOW_past_lab_00e67619
                        ::LAB_00e67619::
                        -- TODO(native): (**(code **)(*(int *)xStack_374 + 0x5ec))((void *)xStack_374,false);
                        this_00 = xStack_32c
                        goto LAB_00e6762e
                        ::FLOW_past_lab_00e67619::
                        goto FLOW_past_lab_00e68a73
                        ::LAB_00e68a73::
                        -- TODO(native): (**(code **)(*(int *)xStack_374 + 0x5ec))((void *)xStack_374,false);
                        resources:DestroyMovie(xStack_32c)
                        ::FLOW_past_lab_00e68a73::
                        ::FLOW_hoist_lab_00e6743e_1::
                        goto FLOW_hoist_lab_00e6762e_2
                        ::FLOW_past_lab_00e6743e::
                        goto FLOW_past_lab_00e6762e
                        ::LAB_00e6762e::
                        resources:DestroyMovie(this_00)
                        if not __native_entity_state:GetStateBool("KickingChickens") then goto LAB_00e68a20 end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar6 = not alive
                        if not bVar6 then
                            i_stk_1d0 = quest:AddQuestInfoCounter("HUD_ICON_CHICKEN_BROWN", 5, 1.0)
                            iVar10 = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
                            i_stk_190 = iVar10
                            i_stk_210 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                            quest:UpdateQuestInfoCounter(iVar10, quest:GetMasterGameState("MaxChickenKickingScore"), -1)
                            quest:DisplayQuestInfo(true)
                            quest:Pause(1.0)
                            CVar27 = 0x0
                            pCVar15 = quest:GetThingWithScriptName("CK_HeroStart")
                            pCVar15 = quest:GetHero()
                            quest:EntityTeleportToThing(pCVar15, pCVar15, (CVar27 ~= 0))
                            CVar27 = 0x0
                            pCVar15 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(me, pCVar15, (CVar27 ~= 0))
                            iVar10 = 0
                            -- TODO(native): ctr_CVar19 = 0;
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e68ae2 end
                                pCVar15 = quest:GetThingWithScriptName("CK_ChickenStart")
                                CVar27 = 0x0
                                pCVar9 = pCVar15:GetPos()
                                pCVar15 = "CREATURE_KICKING_CHICKEN_01"
                                r19 = quest:CreateCreature(pCVar15, pCVar9, "KickedChicken")
                                quest:SetPlayerCreatureOnlyTarget(r19)
                                CVar27 = 0x0
                                quest:SetIsPushableByHero(r19, (CVar27 ~= 0))
                                quest:UpdateQuestInfoCounter(i_stk_210, ctr_CVar19, -1)
                                quest:UpdateQuestInfoCounter(i_stk_1d0, iVar10, -1)
                                quest:FadeScreenIn()
                                if not quest:GetStateBool("InfoDisplayed") then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if not bVar6 then
                                        quest:DisplayGameInfo("TEXT_QST_B17_TUTORIAL_10")
                                        bVar6 = quest:MsgIsGameInfoClickedPast()
                                        while not bVar6 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if bVar6 then goto LAB_00e68ace end
                                            bVar6 = quest:MsgIsGameInfoClickedPast()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if not bVar6 then
                                            quest:DisplayGameInfo("TEXT_QST_B17_TUTORIAL_20")
                                            bVar6 = quest:MsgIsGameInfoClickedPast()
                                            while not bVar6 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e68ace end
                                                bVar6 = quest:MsgIsGameInfoClickedPast()
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if not bVar6 then
                                                quest:DisplayGameInfo("TEXT_QST_B17_TUTORIAL_30")
                                                bVar6 = quest:MsgIsGameInfoClickedPast()
                                                while not bVar6 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if bVar6 then goto LAB_00e68ace end
                                                    bVar6 = quest:MsgIsGameInfoClickedPast()
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if not bVar6 then
                                                    quest:SetStateBool("InfoDisplayed", true)
                                                    goto LAB_00e67a8c
                                                end
                                            end
                                        end
                                    end
                                    goto LAB_00e68ace
                                end
                                goto FLOW_past_lab_00e68ace
                                ::LAB_00e68ace::
                                goto LAB_00e68ae2
                                ::FLOW_past_lab_00e68ace::
                                ::LAB_00e67a8c::
                                quest:SetStateBool("ChickenLanded", false)
                                cVar7 = quest:GetStateBool("ChickenLanded")
                                while not cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e68ace end
                                    cVar7 = quest:GetStateBool("ChickenLanded")
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e68ace end
                                xStack_34c = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                -- TODO(native): CCharString::operator=(&xStack_398,"CS_CHICKING_LANDED");
                                iVar11 = quest:GetStateInt("DistanceBand")
                                if iVar11 == 4 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if not bVar6 then
                                        pcVar20 = "TEXT_QST_B17_MASTER_TOO_FAR_NEW"
                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1c4);
                                        goto LAB_00e6804a
                                    end
                                    goto LAB_00e68aab
                                end
                                if iVar11 == 0 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if not bVar6 then
                                        pcVar20 = "TEXT_QST_B17_MASTER_TOO_WEAK_NEW"
                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1c0);
                                        goto LAB_00e6805d
                                    end
                                    goto LAB_00e68ab9
                                end
                                goto FLOW_past_lab_00e68ab9
                                ::LAB_00e68ab9::
                                quest:PauseAllNonScriptedEntities(false)
                                ::LAB_00e68ac5::
                                resources:DestroyMovie(xStack_34c)
                                goto LAB_00e68ace
                                ::FLOW_past_lab_00e68ab9::
                                if iVar11 ~= -1 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e68ab9 end
                                    iVar11 = quest:GetStateInt("FinalSector")
                                    if iVar11 == 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if not bVar6 then
                                            iVar11 = quest:GetStateInt("DistanceBand")
                                            if iVar11 == 3 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e68ab9 end
                                                pcVar20 = "TEXT_QST_B17_MASTER_FAR_LEFT_NEW"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_240);
                                                -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x64;
                                            elseif iVar11 == 2 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e68aab end
                                                pcVar20 = "TEXT_QST_B17_MASTER_CENTRE_LEFT_NEW"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1e8);
                                                -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x32;
                                            elseif iVar11 == 1 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e68ab9 end
                                                pcVar20 = "TEXT_QST_B17_MASTER_FRONT_LEFT_NEW"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_238);
                                                -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x19;
                                            end
                                            goto LAB_00e6805d
                                        end
                                    elseif iVar11 == 2 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if not bVar6 then
                                            iVar11 = quest:GetStateInt("DistanceBand")
                                            if iVar11 == 3 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e68ab9 end
                                                pcVar20 = "TEXT_QST_B17_MASTER_FAR_CENTRE_NEW"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1a8);
                                                -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0xa;
                                            elseif iVar11 == 2 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e68aab end
                                                pcVar20 = "TEXT_QST_B17_MASTER_CENTRE_CENTRE_NEW"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2b8);
                                                -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x19;
                                            elseif iVar11 == 1 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e68ab9 end
                                                pcVar20 = "TEXT_QST_B17_MASTER_NEAR_CENTRE_NEW"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),(CCharString *)xStack_1e0);
                                                -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0xa;
                                            end
                                            goto LAB_00e6805d
                                        end
                                    else
                                        if iVar11 ~= 3 then
                                            if iVar11 == 0 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e68aab end
                                                pcVar20 = "TEXT_QST_B17_MASTER_OFF_COURSE"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1d8);
                                            else
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if bVar6 then goto LAB_00e68ab9 end
                                                pcVar20 = "TEXT_QST_B17_MASTER_NOWHERE"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_218);
                                            end
                                            goto LAB_00e6804a
                                        end
                                        goto FLOW_hoist_lab_00e6804a_1
                                    end
                                    goto FLOW_hoist_lab_00e6804a_2
                                end
                                goto FLOW_past_lab_00e6804a
                                ::LAB_00e6804a::
                                pcVar20 = "CS_CHICKING_MISSED"
                                goto LAB_00e68054
                                ::FLOW_hoist_lab_00e6804a_1::
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if not bVar6 then
                                    iVar11 = quest:GetStateInt("DistanceBand")
                                    if iVar11 == 3 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e68ab9 end
                                        pcVar20 = "TEXT_QST_B17_MASTER_FAR_RIGHT_NEW"
                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_228);
                                        -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x64;
                                    elseif iVar11 == 2 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e68aab end
                                        pcVar20 = "TEXT_QST_B17_MASTER_CENTRE_RIGHT_NEW"
                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1b8);
                                        -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x32;
                                    elseif iVar11 == 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if bVar6 then goto LAB_00e68ab9 end
                                        pcVar20 = "TEXT_QST_B17_MASTER_NEAR_RIGHT_NEW"
                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_220);
                                        -- TODO(native): ctr_CVar19 = ctr_CVar19 + 0x19;
                                    end
                                    goto LAB_00e6805d
                                end
                                ::FLOW_hoist_lab_00e6804a_2::
                                ::LAB_00e68aab::
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00e68ac5
                                ::FLOW_past_lab_00e6804a::
                                alive = not quest:IsActiveThreadTerminating()
                                bVar6 = not alive
                                if bVar6 then goto LAB_00e68aab end
                                pcVar20 = "CS_CHICKING_FOWL"
                                ::LAB_00e68054::
                                xStack_398 = pcVar20
                                ::LAB_00e6805d::
                                -- TODO(native): CCharString::CCharString(&xStack_398_2,&xStack_398);
                                -- TODO(native): Game_InitializeArena(*(undefined4 *)(this + 0x14));
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_34c)
                                CVar27 = 0x1
                                bVar6 = false
                                pCVar15 = quest:GetThingWithScriptName("KickedChicken")
                                quest:RemoveThing(pCVar15, bVar6, (CVar27 ~= 0))
                                iVar10 = iVar10 + 1
                                quest:ResetPlayerCreatureOnlyTarget()
                            until not (iVar10 < 5)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar6 = not alive
                            if not bVar6 then
                                quest:RemoveQuestInfoElement(i_stk_210)
                                quest:RemoveQuestInfoElement(i_stk_190)
                                quest:RemoveQuestInfoElement(i_stk_1d0)
                                quest:ReturnAllConfiscatedItemsToHero()
                                quest:SetHeroWillAsUsable(true)
                                quest:SetTeleportingAsActive(true)
                                -- TODO(native): CCharString__SetFromFormatV();
                                -- TODO(native): CWideString::CWideString(xStack_208,(int)&xStack_354);
                                -- TODO(native): Vector_PushBack(xStack_360,(int)xStack_208);
                                -- TODO(native): GetFormattedString is not a ForgeFSE binding
                                quest:GetFormattedString("TEXT_QST_B17_SCORE", pCVar15)
                                -- TODO(native): CWideString::operator=((CWideString *)&DAT_0143e908,(int)p0);
                                xStack_398 = "CS_CHICKING_END"
                                if f_stk_374 < quest:ReadGlobalGameDataFloat(0xfb0) then
                                    if f_stk_374 < quest:ReadGlobalGameDataFloat(0xfac) then
                                        if f_stk_374 < quest:ReadGlobalGameDataFloat(0xfa8) then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if not bVar6 then
                                                pcVar20 = "CHICK_KICK_LOW"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_274);
                                                pcVar20 = "CHICK_KICK_LOW_LOOP"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2c8);
                                                if quest:GetMasterGameState("MaxChickenKickingScore") < ctr_CVar19 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if not bVar6 then
                                                        -- TODO(native): *(CCharString *)(*(int *)(this + 0x18) + 0xf8) = ctr_CVar19;
                                                        pcVar20 = "TEXT_QST_B17_MASTER_NO_PRIZE_AGAIN_HIGH"
                                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2c4);
                                                        goto LAB_00e6894f
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if not bVar6 then
                                                        pcVar20 = "TEXT_QST_B17_MASTER_NO_PRIZE_AGAIN"
                                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2c0);
                                                        goto LAB_00e6894f
                                                    end
                                                end
                                                goto FLOW_past_lab_00e6894f
                                                ::LAB_00e6894f::
                                                goto LAB_00e68954
                                                ::FLOW_past_lab_00e6894f::
                                            end
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if not bVar6 then
                                                pcVar20 = "CHICK_KICK_MID"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2a4);
                                                pcVar20 = "CHICK_KICK_MID_LOOP"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_29c);
                                                if (quest:GetStateInt("PrizesWon") & 1) == 0 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if not bVar6 then
                                                        pcVar20 = "TEXT_QST_B17_MASTER_LOW_PRIZE_FIRST"
                                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_284);
                                                        puVar1 = (__native_entity_state:GetStateInt("self_0x14") + 0x5c)
                                                        -- TODO(native): *puVar1 = *puVar1 | 1;
                                                        xStack_398 = "CS_CHICKING_LOWPRIZE"
                                                        goto LAB_00e687e4
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if not bVar6 then
                                                        if quest:GetMasterGameState("MaxChickenKickingScore") < ctr_CVar19 then
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar6 = not alive
                                                            if not bVar6 then
                                                                -- TODO(native): *(CCharString *)(*(int *)(this + 0x18) + 0xf8) = ctr_CVar19;
                                                                pcVar20 = "TEXT_QST_B17_MASTER_LOW_PRIZE_AGAIN_HIGH"
                                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_28c);
                                                                goto LAB_00e687e4
                                                            end
                                                        else
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar6 = not alive
                                                            if not bVar6 then
                                                                pcVar20 = "TEXT_QST_B17_MASTER_LOW_PRIZE_AGAIN"
                                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_294);
                                                                goto LAB_00e687e4
                                                            end
                                                        end
                                                    end
                                                end
                                                goto FLOW_past_lab_00e687e4
                                                ::LAB_00e687e4::
                                                puVar29 = "50"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_27c);
                                                goto LAB_00e68954
                                                ::FLOW_past_lab_00e687e4::
                                            end
                                        end
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar6 = not alive
                                        if not bVar6 then
                                            pcVar20 = "CHICK_KICK_HIGH"
                                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_24c);
                                            pcVar20 = "CHICK_KICK_HIGH_LOOP"
                                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_244);
                                            if (quest:GetStateInt("PrizesWon") & 2) == 0 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if not bVar6 then
                                                    pcVar20 = "TEXT_QST_B17_MASTER_MIDDLE_PRIZE_FIRST"
                                                    -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2b4);
                                                    puVar1 = (__native_entity_state:GetStateInt("self_0x14") + 0x5c)
                                                    -- TODO(native): *puVar1 = *puVar1 | 2;
                                                    xStack_398 = "CS_CHICKING_MIDPRIZE"
                                                    goto LAB_00e685d7
                                                end
                                            else
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar6 = not alive
                                                if not bVar6 then
                                                    if quest:GetMasterGameState("MaxChickenKickingScore") < ctr_CVar19 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar6 = not alive
                                                        if not bVar6 then
                                                            -- TODO(native): *(CCharString *)(*(int *)(this + 0x18) + 0xf8) = ctr_CVar19;
                                                            pcVar20 = "TEXT_QST_B17_MASTER_MIDDLE_PRIZE_AGAIN_HIGH"
                                                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),xStack_234);
                                                            goto LAB_00e685d7
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar6 = not alive
                                                        if not bVar6 then
                                                            pcVar20 = "TEXT_QST_B17_MASTER_MIDDLE_PRIZE_AGAIN"
                                                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_23c);
                                                            goto LAB_00e685d7
                                                        end
                                                    end
                                                end
                                            end
                                            goto FLOW_past_lab_00e685d7
                                            ::LAB_00e685d7::
                                            puVar29 = "100"
                                            -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_2ac);
                                            -- LAB_00e6894f_c4: (native jump target)
                                            goto LAB_00e68954
                                            ::FLOW_past_lab_00e685d7::
                                        end
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if not bVar6 then
                                        pcVar20 = "CHICK_KICK_HIGH"
                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_200);
                                        pcVar20 = "CHICK_KICK_HIGH_LOOP"
                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1c8);
                                        if (quest:GetStateInt("PrizesWon") & 4) == 0 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if not bVar6 then
                                                -- TODO(native): *(CCharString *)(*(int *)(this + 0x18) + 0xf8) = ctr_CVar19;
                                                puVar1 = (__native_entity_state:GetStateInt("self_0x14") + 0x5c)
                                                -- TODO(native): *puVar1 = *puVar1 | 4;
                                                xStack_398 = "CS_CHICKING_TOPPRIZE"
                                                goto LAB_00e68954
                                            end
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar6 = not alive
                                            if not bVar6 then
                                                if quest:GetMasterGameState("MaxChickenKickingScore") < ctr_CVar19 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if not bVar6 then
                                                        -- TODO(native): *(CCharString *)(*(int *)(this + 0x18) + 0xf8) = ctr_CVar19;
                                                        pcVar20 = "TEXT_QST_B17_MASTER_TOP_PRIZE_AGAIN_HIGH"
                                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1a0);
                                                        goto LAB_00e68395
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar6 = not alive
                                                    if not bVar6 then
                                                        pcVar20 = "TEXT_QST_B17_MASTER_TOP_PRIZE_AGAIN"
                                                        -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1f8);
                                                        goto LAB_00e68395
                                                    end
                                                end
                                                goto FLOW_past_lab_00e68395
                                                ::LAB_00e68395::
                                                puVar29 = "200"
                                                -- TODO(native): pCVar13 = std::map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> ::operator[](( map<CCharString,CCharString,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCharString>_>_> *)(*(int *)(this + 0x14) + 100),&xStack_1f0);
                                                -- LAB_00e6894f_c5: (native jump target)
                                                goto LAB_00e68954
                                                ::FLOW_past_lab_00e68395::
                                            end
                                        end
                                    end
                                end
                                goto FLOW_past_lab_00e68954
                                ::LAB_00e68954::
                                -- TODO(native): if ctr_CVar19 == *(__native_entity_state:GetStateInt("self_0x18") + 0xf8) then
                                if false then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar6 = not alive
                                    if bVar6 then goto LAB_00e68ad9 end
                                    -- TODO(native): UpdateOnlineScore_ChickenKick is not a ForgeFSE binding
                                    quest:UpdateOnlineScore_ChickenKick(f_stk_374)
                                end
                                xStack_164 = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                -- TODO(native): CCharString::CCharString(&xStack_398_2,&xStack_398);
                                -- TODO(native): Game_InitializeArena(*(undefined4 *)(this + 0x14));
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_164)
                                -- TODO(native): CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)xStack_360);
                                goto LAB_00e68a20
                                ::FLOW_past_lab_00e68954::
                                ::LAB_00e68ad9::
                                -- TODO(native): CDefendingCombatantInfo::CCombatWheel::ResetRings((CCombatWheel *)xStack_360);
                            end
                            ::LAB_00e68ae2::
                        end
                        ::FLOW_hoist_lab_00e6762e_2::
                        break
                        ::FLOW_past_lab_00e6762e::
                        ::LAB_00e68a3c::
                        -- TODO(native): (**(code **)(*CVar19 + 0x5ec))(ctr_CVar19,false);
                        resources:DestroyMovie(xStack_32c)
                        break
                    end
                    ::LAB_00e68a20::
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar6 = not alive
                end
                ::LAB_00e68aeb::
            end
        end
        ::LAB_00e68af4::
        quest:DeregisterTimer(i_stk_3a0)
        quest:DeregisterTimer(i_stk_3a4)
    end
    resources:ReleaseResource(xStack_394)
end

function Init(quest, me)
    __native_entity_state:SetStateBool("HeroHasPlayed", false)
    __native_entity_state:SetStateBool("HaveTalked", false)
    __native_entity_state:SetStateBool("KickingChickens", false)
    __native_entity_state:SetStateBool("GhostChat", false)
end

function OnPersist(quest, me, context)
    local heroHasPlayed = quest:GetStateBool("HeroHasPlayed") or false
    heroHasPlayed = quest:PersistTransferBool(context, "HeroHasPlayed", heroHasPlayed)
    quest:SetStateBool("HeroHasPlayed", heroHasPlayed)
    local haveTalked = quest:GetStateBool("HaveTalked") or false
    haveTalked = quest:PersistTransferBool(context, "HaveTalked", haveTalked)
    quest:SetStateBool("HaveTalked", haveTalked)
    local ghostChat = quest:GetStateBool("GhostChat") or false
    ghostChat = quest:PersistTransferBool(context, "GhostChat", ghostChat)
    quest:SetStateBool("GhostChat", ghostChat)
end

function OnPredicateFail(quest, me)
end

