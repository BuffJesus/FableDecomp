-- Generated native draft: BordelloGuard. Review coverage report before use.
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
    local bVar5, cVar6, c_stk_bd, dist, fVar3, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, iVar11, iVar12, iVar13, iVar14, native_arg_sequence_1, native_arg_sequence_2, p1, pCVar15, pCVar2, pCVar7, pCVar8, pcVar10, r1, r2, r3, r4, r5, r6, r7, r8, this_02, uVar4, uVar9, u_stk_bc, xStack_64, xStack_88, xStack_b8, xStack_d0, x_stk_18, x_stk_24, x_stk_30, x_stk_3c, x_stk_48, x_stk_54, x_stk_c
    local alive = true
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        xStack_d0 = resources:NewResource()
        resources:PrepareResource(xStack_d0)
        bVar5 = resources:TryAcquire(xStack_d0, me, 4)
        while not bVar5 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then goto LAB_00e40e63 end
            bVar5 = resources:TryAcquire(xStack_d0, me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            resources:AssignResource(resources:MemberResource("seh_Guard"), xStack_d0)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar5 then
                while true do
                    cVar6 = me:IsTalkedToByHero()
                    if cVar6 then break end
                    -- LAB_00e40a6c: (native jump target)
                    cVar6 = me:MsgIsHitByHero()
                    if not cVar6 then
                        cVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if cVar6 then
                            cVar6 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not cVar6 then goto LAB_00e40afa end
                        end
                        c_stk_bd = 0
                    else
                        goto LAB_00e40afa
                    end
                    goto FLOW_past_lab_00e40afa
                    ::LAB_00e40afa::
                    c_stk_bd = 1
                    ::FLOW_past_lab_00e40afa::
                    if c_stk_bd ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e40e63 end
                        xStack_88 = resources:StartMovie("")
                        pCVar7 = 0x1
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_30 = resources:ScriptThing(xStack_d0)
                        pCVar15 = x_stk_30
                        fret_05 = quest:GetHealth(pCVar15)
                        fVar3 = 0.0
                        if fVar3 < fret_05 then
                            iVar14 = 0
                            iVar13 = 1
                            iVar12 = 0
                            iVar11 = 2
                            pCVar8 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                            pCVar8 = (a .. pCVar8)
                            p1 = pCVar8
                            pCVar15 = quest:GetHero()
                            r1 = me:Speak(pCVar15, p1, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                            iVar11 = me:IsPerformingScriptTask()
                            cVar6 = iVar11
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00e40e5a
                                end
                                iVar11 = me:IsPerformingScriptTask()
                                cVar6 = iVar11
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00e40e5a
                            end
                            goto FLOW_past_lab_00e40e5a
                            ::LAB_00e40e5a::
                            this_02 = xStack_88
                            goto LAB_00e40e5e
                            ::FLOW_past_lab_00e40e5a::
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_88)
                    end
                    native_arg_sequence_1 = false
                    if not quest:GetStateBool("HeroPartying") then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if native_arg_sequence_1 then
                        if not quest:GetStateBool("HeroTricking") then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                        if not native_arg_sequence_1 then
                            bVar5 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                            if not bVar5 then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                        end
                        if native_arg_sequence_1 then
                            if not quest:GetStateBool("PlayerOwned") then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                        end
                    end
                    if native_arg_sequence_1 then
                        dist = 3.0
                        pCVar15 = quest:GetHero()
                        bVar5 = quest:IsDistanceBetweenThingsUnder(me, pCVar15, dist)
                        if ((bVar5) and (not quest:GetStateBool("CutscenePlaying"))) and (not quest:GetStateBool("MagicianSleeping")) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e40e63 end
                            pCVar8 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                            pCVar8 = ("TEXT_QST_B13_GUARD_ENTRY_REFUSED" .. pCVar8)
                            resources:SetString(resources:MemberStringMap("csargs"), "$DIALOGUE", pCVar8)
                            xStack_64 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            require("V_Bordello.native_quest_helpers").helper_E3E720(quest, me, "CS_BORDELLO_KICKEDOUT", false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_64)
                        end
                    end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        resources:ReleaseResource(xStack_d0)
                        return
                    end
                end
                ::FLOW_after_lab_00e40a6c::
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if not bVar5 then
                    xStack_b8 = resources:StartMovie("")
                    pCVar15 = 0x1
                    quest:PauseAllNonScriptedEntities(true)
                    if not quest:GetStateBool("BecomeNunnery") then
                        native_arg_sequence_2 = false
                        if quest:GetStateBool("HeroTricking") then
                            native_arg_sequence_2 = true
                        else
                            native_arg_sequence_2 = false
                        end
                        if native_arg_sequence_2 then
                            bVar5 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                            if bVar5 then
                                native_arg_sequence_2 = true
                            else
                                native_arg_sequence_2 = false
                            end
                        end
                        if native_arg_sequence_2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e40673 end
                            x_stk_48 = resources:ScriptThing(xStack_d0)
                            pCVar7 = x_stk_48
                            fret_00 = quest:GetHealth(pCVar7)
                            fVar3 = 0.0
                            if fVar3 < fret_00 then
                                iVar14 = 0
                                iVar13 = 1
                                iVar12 = 0
                                iVar11 = 0
                                pcVar10 = "TEXT_QST_B13_GUARD_STRANGE_LADY"
                                pCVar15 = quest:GetHero()
                                r2 = me:Speak(pCVar15, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                iVar11 = me:IsPerformingScriptTask()
                                cVar6 = iVar11
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e40673 end
                                    iVar11 = me:IsPerformingScriptTask()
                                    cVar6 = iVar11
                                end
                                goto LAB_00e40a48
                            end
                            goto LAB_00e40a57
                        end
                        if not quest:GetStateBool("HeroPartying") then
                            bVar5 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                            if bVar5 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    -- LAB_00e40e2c: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_02 = xStack_b8
                                    goto LAB_00e40e5e
                                end
                                x_stk_24 = resources:ScriptThing(xStack_d0)
                                pCVar7 = x_stk_24
                                fret_02 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_02 then
                                    iVar14 = 0
                                    iVar13 = 1
                                    iVar12 = 0
                                    iVar11 = 0
                                    pcVar10 = "TEXT_QST_B13_GUARD_FEMALE_NO_ENTRY"
                                    pCVar15 = quest:GetHero()
                                    r3 = me:Speak(pCVar15, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                    iVar11 = me:IsPerformingScriptTask()
                                    cVar6 = iVar11
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e40673 end
                                        iVar11 = me:IsPerformingScriptTask()
                                        cVar6 = iVar11
                                    end
                                    goto LAB_00e40a48
                                end
                            elseif not quest:GetStateBool("PlayerOwned") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e40673 end
                                x_stk_18 = resources:ScriptThing(xStack_d0)
                                pCVar7 = x_stk_18
                                fret_04 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_04 then
                                    iVar14 = 0
                                    iVar13 = 1
                                    iVar12 = 0
                                    iVar11 = 0
                                    pcVar10 = "TEXT_QST_B13_GUARD_MALE_NO_ENTRY"
                                    pCVar15 = quest:GetHero()
                                    r4 = me:Speak(pCVar15, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                    iVar11 = me:IsPerformingScriptTask()
                                    cVar6 = iVar11
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e40673 end
                                        iVar11 = me:IsPerformingScriptTask()
                                        cVar6 = iVar11
                                    end
                                    goto LAB_00e40a48
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e40673 end
                                x_stk_54 = resources:ScriptThing(xStack_d0)
                                pCVar7 = x_stk_54
                                fret_03 = quest:GetHealth(pCVar7)
                                fVar3 = 0.0
                                if fVar3 < fret_03 then
                                    iVar14 = 0
                                    iVar13 = 1
                                    iVar12 = 0
                                    iVar11 = 0
                                    pcVar10 = "TEXT_QST_B13_GUARD_GREETING"
                                    pCVar15 = quest:GetHero()
                                    r5 = me:Speak(pCVar15, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                    iVar11 = me:IsPerformingScriptTask()
                                    cVar6 = iVar11
                                    while cVar6 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar5 = not alive
                                        if bVar5 then goto LAB_00e40673 end
                                        iVar11 = me:IsPerformingScriptTask()
                                        cVar6 = iVar11
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e40673 end
                                end
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e40673 end
                            x_stk_3c = resources:ScriptThing(xStack_d0)
                            pCVar7 = x_stk_3c
                            fret_01 = quest:GetHealth(pCVar7)
                            fVar3 = 0.0
                            if fVar3 < fret_01 then
                                iVar14 = 0
                                iVar13 = 1
                                iVar12 = 0
                                iVar11 = 0
                                pcVar10 = "TEXT_QST_B13_GUARD_HERO_EXHAUSTED"
                                pCVar15 = quest:GetHero()
                                r6 = me:Speak(pCVar15, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                                iVar11 = me:IsPerformingScriptTask()
                                cVar6 = iVar11
                                while cVar6 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar5 = not alive
                                    if bVar5 then goto LAB_00e40673 end
                                    iVar11 = me:IsPerformingScriptTask()
                                    cVar6 = iVar11
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e40673 end
                            end
                        end
                        goto FLOW_past_lab_00e40a48
                        ::LAB_00e40a48::
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e40673 end
                        ::FLOW_past_lab_00e40a48::
                        goto LAB_00e40a57
                    end
                    goto FLOW_past_lab_00e40a57
                    ::LAB_00e40a57::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_b8)
                    cVar6 = me:MsgIsHitByHero()
                    if not cVar6 then
                        cVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                        if cVar6 then
                            cVar6 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not cVar6 then goto LAB_00e40afa_c1 end
                        end
                        c_stk_bd = 0
                    end
                    goto FLOW_past_lab_00e40afa_c1
                    ::LAB_00e40afa_c1::
                    c_stk_bd = 1
                    ::FLOW_past_lab_00e40afa_c1::
                    if c_stk_bd ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then goto LAB_00e40e63 end
                        xStack_88 = resources:StartMovie("")
                        pCVar7 = 0x1
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_30 = resources:ScriptThing(xStack_d0)
                        pCVar15 = x_stk_30
                        fret_05 = quest:GetHealth(pCVar15)
                        fVar3 = 0.0
                        if fVar3 < fret_05 then
                            iVar14 = 0
                            iVar13 = 1
                            iVar12 = 0
                            iVar11 = 2
                            pCVar8 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                            pCVar8 = (a .. pCVar8)
                            p1 = pCVar8
                            pCVar15 = quest:GetHero()
                            r7 = me:Speak(pCVar15, p1, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                            iVar11 = me:IsPerformingScriptTask()
                            cVar6 = iVar11
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00e40e5a_c1
                                end
                                iVar11 = me:IsPerformingScriptTask()
                                cVar6 = iVar11
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00e40e5a_c1
                            end
                            goto FLOW_past_lab_00e40e5a_c1
                            ::LAB_00e40e5a_c1::
                            this_02 = xStack_88
                            goto LAB_00e40e5e
                            ::FLOW_past_lab_00e40e5a_c1::
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_88)
                    end
                    native_arg_sequence_1 = false
                    if not quest:GetStateBool("HeroPartying") then
                        native_arg_sequence_1 = true
                    end
                    if native_arg_sequence_1 then
                        if not quest:GetStateBool("HeroTricking") then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                        if not native_arg_sequence_1 then
                            bVar5 = require("V_Bordello.native_quest_helpers").helper_E3E320(quest, me)
                            if not bVar5 then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                        end
                        if native_arg_sequence_1 then
                            if not quest:GetStateBool("PlayerOwned") then
                                native_arg_sequence_1 = true
                            else
                                native_arg_sequence_1 = false
                            end
                        end
                    end
                    if native_arg_sequence_1 then
                        dist = 3.0
                        pCVar15 = quest:GetHero()
                        bVar5 = quest:IsDistanceBetweenThingsUnder(me, pCVar15, dist)
                        if ((bVar5) and (not quest:GetStateBool("CutscenePlaying"))) and (not quest:GetStateBool("MagicianSleeping")) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e40e63 end
                            pCVar8 = require("V_Bordello.native_quest_helpers").helper_E3E6B0(quest, me)
                            pCVar8 = ("TEXT_QST_B13_GUARD_ENTRY_REFUSED" .. pCVar8)
                            resources:SetString(resources:MemberStringMap("csargs"), "$DIALOGUE", pCVar8)
                            xStack_64 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            require("V_Bordello.native_quest_helpers").helper_E3E720(quest, me, "CS_BORDELLO_KICKEDOUT", false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_64)
                        end
                    end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        resources:ReleaseResource(xStack_d0)
                        return
                    end
                    goto FLOW_after_lab_00e40a6c
                    ::FLOW_past_lab_00e40a57::
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if not bVar5 then
                        x_stk_c = resources:ScriptThing(xStack_d0)
                        pCVar7 = x_stk_c
                        fret_0 = quest:GetHealth(pCVar7)
                        fVar3 = 0.0
                        if fVar3 < fret_0 then
                            iVar14 = 0
                            iVar13 = 1
                            iVar12 = 0
                            iVar11 = 0
                            pcVar10 = "TEXT_QST_B13_GUARD_EMPLOYED_NUNNERY"
                            pCVar15 = quest:GetHero()
                            r8 = me:Speak(pCVar15, pcVar10, iVar11, (iVar12 ~= 0), (iVar13 ~= 0), (iVar14 ~= 0))
                            iVar11 = me:IsPerformingScriptTask()
                            cVar6 = iVar11
                            while cVar6 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar5 = not alive
                                if bVar5 then goto LAB_00e40673 end
                                iVar11 = me:IsPerformingScriptTask()
                                cVar6 = iVar11
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00e40673 end
                        end
                        goto LAB_00e40a57
                    end
                    ::LAB_00e40673::
                    quest:PauseAllNonScriptedEntities(false)
                    this_02 = xStack_b8
                    goto LAB_00e40e5e
                end
                goto FLOW_past_lab_00e40e5e
                ::LAB_00e40e5e::
                resources:DestroyMovie(this_02)
                ::FLOW_past_lab_00e40e5e::
            end
        end
        ::LAB_00e40e63::
        resources:ReleaseResource(xStack_d0)
    end
end

function Init(quest, me)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

