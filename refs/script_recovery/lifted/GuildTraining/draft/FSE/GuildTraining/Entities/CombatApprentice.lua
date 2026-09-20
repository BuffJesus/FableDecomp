-- Generated native draft: CombatApprentice. Review coverage report before use.
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
    local __native_condition_1, bVar3, cVar4, c_stk_229, c_stk_22a, c_stk_259, dist, fVar2, f_stk_100, f_stk_210, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04, fret_05, fret_06, fret_07, fret_08, fret_09, fret_10, fret_11, fret_12, fret_13, iVar14, iVar17, iVar18, iVar5, iVar6, ixVar11, native_arg_sequence_1, native_arg_switch_2, p0, pCVar15, pCVar7, pCVar8, pcVar13, piVar12, r1, r10, r11, r2, r3, r4, r5, r6, r7, r8, r9, this_00, uVar16, xStack_1ec, xStack_1fc, xStack_20c, xStack_220, xStack_254, xStack_258, xStack_260, xStack_ac, xStack_bc, xStack_cc, xStack_dc, xStack_ec, xStack_fc, x_stk_18, x_stk_214, x_stk_24, x_stk_30, x_stk_60, x_stk_6c, x_stk_78, x_stk_84, x_stk_90, x_stk_c
    local alive = true
    xStack_254 = resources:NewResource()
    resources:PrepareResource(xStack_254)
    bVar3 = resources:TryAcquire(xStack_254, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            resources:ReleaseResource(xStack_254)
            return
        end
        bVar3 = resources:TryAcquire(xStack_254, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        -- LAB_00d4c6ec: (native jump target)
        resources:ReleaseResource(xStack_254)
        return
    end
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(me)
    iVar5 = quest:RegisterTimer()
    xStack_260 = iVar5
    quest:SetTimer(xStack_260, 10)
    r1 = quest:GetThingWithScriptName("CombatApprenticeTargetMarker")
    c_stk_22a = 0
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    repeat
        if bVar3 then
            r1 = nil
            quest:DeregisterTimer(xStack_260)
            -- LAB_00d4c4a5: (native jump target)
            resources:ReleaseResource(xStack_254)
            return
        end
        bVar3 = quest:IsQuestActive("Q_GuildTrainingDeparture")
        if not bVar3 then goto LAB_00d4a512 end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            quest:DeregisterTimer(xStack_260)
            resources:ReleaseResource(xStack_254)
            return
        end
        if ((quest:GetMasterGameState("GlobalMeleeGrade") < 4) and (quest:GetMasterGameState("GlobalSkillGrade") < 4)) and (quest:GetMasterGameState("GlobalWillGrade") < 4) then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                if c_stk_22a == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:DeregisterTimer(xStack_260)
                        resources:ReleaseResource(xStack_254)
                        return
                    end
                    quest:SetThingHasInformation(me, false, true, false)
                    c_stk_22a = 1
                end
                goto LAB_00d4a512
            end
            quest:DeregisterTimer(xStack_260)
            resources:ReleaseResource(xStack_254)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            quest:DeregisterTimer(xStack_260)
            resources:ReleaseResource(xStack_254)
            return
        end
        if c_stk_22a ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(xStack_260)
                resources:ReleaseResource(xStack_254)
                return
            end
            quest:ClearThingHasInformation(me)
            c_stk_22a = 0
        end
        ::LAB_00d4a512::
        bVar3 = quest:IsDistanceBetweenThingsOver(me, r1, 4.0)
        __native_condition_1 = not bVar3
        if not __native_condition_1 then
            iVar6 = me:IsPerformingScriptTask()
            __native_condition_1 = iVar6
        end
        if __native_condition_1 then
            iVar5 = me:IsPerformingScriptTask()
            if iVar5 then goto LAB_00d4a71c end
            dist = 10.0
            pCVar8 = quest:GetHero()
            bVar3 = quest:IsDistanceBetweenThingsUnder(me, pCVar8, dist)
            native_arg_sequence_1 = false
            if not bVar3 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                iVar5 = quest:GetTimer(xStack_260)
                if 0 < iVar5 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then goto LAB_00d4a71c end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                bVar3 = false
                pCVar8 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, pCVar8, bVar3)
                quest:SetTimer(xStack_260, 0x14)
                iVar6 = quest:AddNewConversation(me, false, false)
                pCVar8 = quest:GetHero()
                quest:AddPersonToConversation(iVar6, pCVar8)
                iVar5 = quest:GetMasterGameState("GlobalMeleeGrade")
                if iVar5 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        pCVar8 = quest:GetHero()
                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_COMMENT", me, pCVar8, false)
                        goto LAB_00d4a717
                    end
                else
                    if iVar5 == 7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            pCVar8 = quest:GetHero()
                            quest:AddLineToConversation(iVar6, "TEXT_QST_028_APPRENTICE_MELEE_APLUS_COMMENT", me, pCVar8, false)
                            goto LAB_00d4a717
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            pCVar8 = quest:GetHero()
                            quest:AddLineToConversation(iVar6, "TEXT_QST_028_APPRENTICE_MELEE_NOT_APLUS_COMMENT", me, pCVar8, false)
                            goto LAB_00d4a717
                        end
                    end
                end
                goto FLOW_past_lab_00d4a717
                ::LAB_00d4a717::
                goto LAB_00d4a71c
                ::FLOW_past_lab_00d4a717::
            end
            quest:DeregisterTimer(xStack_260)
            resources:ReleaseResource(xStack_254)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00d4c4f5: (native jump target)
            quest:DeregisterTimer(xStack_260)
            resources:ReleaseResource(xStack_254)
            return
        end
        if not (r1 ~= nil and not r1:IsNull()) then
            p0 = {x = 0, y = 0, z = 0}
        else
            p0 = r1:GetPos()
        end
        me:MoveToPosition(p0, 3.0, 1, false, true)
        ::LAB_00d4a71c::
        if not __native_entity_state:GetStateBool("WaitingForFight") then
            goto LAB_00d4a758
        else
            bVar3 = me:IsTalkedToByHero()
            if not bVar3 then goto LAB_00d4a758 end
            bVar3 = true
        end
        goto FLOW_past_lab_00d4a758
        ::LAB_00d4a758::
        bVar3 = false
        ::FLOW_past_lab_00d4a758::
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(xStack_260)
                resources:ReleaseResource(xStack_254)
                return
            end
            iVar5 = me:IsPerformingScriptTask()
            if iVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d4c6da end
                me:ClearCommands()
                xStack_ec = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                x_stk_60 = resources:ScriptThing(xStack_254)
                pCVar7 = x_stk_60
                fret_0 = quest:GetHealth(pCVar7)
                fVar2 = 0.0
                if fVar2 < fret_0 then
                    iVar17 = 0
                    iVar14 = 1
                    iVar6 = 0
                    iVar5 = 0
                    pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_EARLY"
                    pCVar7 = quest:GetHero()
                    r2 = me:Speak(pCVar7, pcVar13, iVar5, (iVar6 ~= 0), (iVar14 ~= 0), (iVar17 ~= 0))
                    iVar5 = me:IsPerformingScriptTask()
                    cVar4 = iVar5
                    while cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_ec)
                            goto LAB_00d4c6da
                        end
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_ec)
                        goto LAB_00d4c6da
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_ec)
                goto LAB_00d4c416
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                if quest:GetMasterGameState("HeroTakingGuildTest") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        xStack_dc = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_30 = resources:ScriptThing(xStack_254)
                        pCVar7 = x_stk_30
                        fret_00 = quest:GetHealth(pCVar7)
                        fVar2 = 0.0
                        if fVar2 < fret_00 then
                            iVar17 = 0
                            iVar14 = 1
                            iVar6 = 0
                            iVar5 = 0
                            pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_OTHER_MELEE_GRADE"
                            pCVar7 = quest:GetHero()
                            r3 = me:Speak(pCVar7, pcVar13, iVar5, (iVar6 ~= 0), (iVar14 ~= 0), (iVar17 ~= 0))
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_dc)
                                    goto LAB_00d4c6da
                                end
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_dc)
                                goto LAB_00d4c6da
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_dc)
                        goto LAB_00d4c416
                    end
                    goto LAB_00d4c6da
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d4c6da end
                r4 = quest:GetThingWithScriptName("MeleeApprentice")
                iVar5 = (r4 ~= nil and r4:IsAlive())
                if not iVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        xStack_cc = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_90 = resources:ScriptThing(xStack_254)
                        pCVar7 = x_stk_90
                        fret_01 = quest:GetHealth(pCVar7)
                        fVar2 = 0.0
                        if fVar2 < fret_01 then
                            iVar17 = 0
                            iVar14 = 1
                            iVar6 = 0
                            iVar5 = 0
                            pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_NO_WHISPER"
                            pCVar7 = quest:GetHero()
                            r5 = me:Speak(pCVar7, pcVar13, iVar5, (iVar6 ~= 0), (iVar14 ~= 0), (iVar17 ~= 0))
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_cc)
                                    goto LAB_00d4c6d1
                                end
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_cc)
                                goto LAB_00d4c6d1
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_cc)
                        goto LAB_00d4c40d
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4c6d1 end
                    xStack_20c = resources:StartMovie("")
                    x_stk_214 = piVar12
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_78 = resources:ScriptThing(xStack_254)
                    pCVar7 = x_stk_78
                    fret_02 = quest:GetHealth(pCVar7)
                    fVar2 = 0.0
                    if fVar2 < fret_02 then
                        iVar17 = 0
                        iVar14 = 1
                        iVar6 = 0
                        iVar5 = 0
                        pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_HELLO"
                        pCVar7 = quest:GetHero()
                        r6 = me:Speak(pCVar7, pcVar13, iVar5, (iVar6 ~= 0), (iVar14 ~= 0), (iVar17 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d4c5ff end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then goto LAB_00d4ac95 end
                        ::LAB_00d4c5ff::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_20c)
                        goto LAB_00d4c6d1
                    end
                    ::LAB_00d4ac95::
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while iVar5 < 0 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_20c)
                            goto LAB_00d4c6d1
                        end
                        iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_20c)
                        goto LAB_00d4c6d1
                    end
                    if iVar5 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            -- LAB_00d4ae53: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_20c)
                            goto LAB_00d4c6d1
                        end
                        x_stk_18 = resources:ScriptThing(xStack_254)
                        pCVar7 = x_stk_18
                        fret_03 = quest:GetHealth(pCVar7)
                        c_stk_259 = 1
                        if fret_03 <= 0.0 then
                            c_stk_259 = iVar5
                        end
                        if c_stk_259 ~= 0 then
                            iVar18 = 0
                            iVar17 = 1
                            iVar14 = 0
                            iVar6 = 0
                            pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_RETURN"
                            pCVar7 = quest:GetHero()
                            r7 = me:Speak(pCVar7, pcVar13, iVar6, (iVar14 ~= 0), (iVar17 ~= 0), (iVar18 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar4 = iVar6
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_20c)
                                    goto LAB_00d4c6d1
                                end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar4 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_20c)
                                goto LAB_00d4c6d1
                            end
                        end
                    else
                        if iVar5 ~= 1 then goto LAB_00d4b11f end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_20c)
                            goto LAB_00d4c6d1
                        end
                        x_stk_84 = resources:ScriptThing(xStack_254)
                        pCVar7 = x_stk_84
                        fret_04 = quest:GetHealth(pCVar7)
                        c_stk_259 = iVar5
                        if fret_04 <= 0.0 then
                            c_stk_259 = 0
                        end
                        if c_stk_259 ~= 0 then
                            iVar18 = 0
                            iVar17 = 1
                            iVar14 = 0
                            iVar6 = 0
                            pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_START"
                            pCVar7 = quest:GetHero()
                            r8 = me:Speak(pCVar7, pcVar13, iVar6, (iVar14 ~= 0), (iVar17 ~= 0), (iVar18 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar4 = iVar6
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_20c)
                                    goto LAB_00d4c6d1
                                end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar4 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_20c)
                                goto LAB_00d4c6d1
                            end
                        end
                        if quest:GetMasterGameState("GlobalMeleeGrade") ~= 7 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_20c)
                                goto LAB_00d4c6d1
                            end
                            x_stk_6c = resources:ScriptThing(xStack_254)
                            pCVar7 = x_stk_6c
                            fret_05 = quest:GetHealth(pCVar7)
                            c_stk_259 = 0.0 < fret_05
                            if c_stk_259 then
                                iVar18 = 0
                                iVar17 = 1
                                iVar14 = 0
                                iVar6 = 0
                                pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_START_APLUS"
                                pCVar7 = quest:GetHero()
                                r9 = me:Speak(pCVar7, pcVar13, iVar6, (iVar14 ~= 0), (iVar17 ~= 0), (iVar18 ~= 0))
                                iVar6 = me:IsPerformingScriptTask()
                                cVar4 = iVar6
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_20c)
                                        goto LAB_00d4c6d1
                                    end
                                    iVar6 = me:IsPerformingScriptTask()
                                    cVar4 = iVar6
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_20c)
                                    goto LAB_00d4c6d1
                                end
                            end
                        end
                        quest:FadeScreenOut(0.5, 0.5)
                        quest:Pause(1.0)
                        uVar16 = 0
                        pCVar7 = quest:GetThingWithScriptName("HeroMeleeStart")
                        pCVar8 = quest:GetHero()
                        quest:EntityTeleportToThing(pCVar8, pCVar7, (uVar16 ~= 0))
                        bVar3 = false
                        pCVar7 = quest:GetThingWithScriptName("WhisperMeleeStart")
                        quest:EntityTeleportToThing(r4, pCVar7, bVar3)
                        bVar3 = false
                        pCVar7 = quest:GetHero()
                        quest:EntityUnsheatheMeleeWeapon(pCVar7, bVar3)
                        quest:EntityUnsheatheWeapons(r4, false)
                        piVar12 = x_stk_214
                    end
                    ::LAB_00d4b11f::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_20c)
                    if iVar5 ~= 1 then goto LAB_00d4c40d end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4c6d1 end
                    quest:FadeScreenIn()
                    quest:SetPlayerCreatureOnlyTarget(r4)
                    quest:SetMasterGameState("HeroTakingGuildTest", true)
                    quest:SetStateBool("StartedMeleeTesting", true)
                    __native_entity_state:SetStateBool("WaitingForFight", false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                    quest:ModifyThingHealth(r4, 1000.0, false)
                    quest:EntitySetAsKillable(r4, false, true)
                    quest:EntitySetCombatType(r4, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE")
                    quest:EntitySetInFaction(r4, "FACTION_BANDITS")
                    if (r4 ~= nil and not r4:IsNull()) then
                        r4:SetFriendsWithEverythingFlag(0)
                    end
                    pCVar7 = quest:GetHero()
                    quest:GiveThingBestEnemyTarget(r4, pCVar7)
                    quest:SetStateBool("FightFinished", false)
                    xStack_258 = quest:RegisterTimer()
                    iVar6 = xStack_258
                    quest:SetTimer(xStack_258, 0xf)
                    quest:DisplayQuestInfo(true)
                    x_stk_214 = quest:AddQuestInfoBarHealth(r4, {R = 255, G = 255, B = 255, A = 255}, "HUD_WHISPER_ICON", 1.0)
                    pCVar7 = quest:GetHero()
                    fret_06 = quest:GetHealth(pCVar7)
                    f_stk_210 = fret_06
                    fret_07 = quest:GetHealth(r4)
                    f_stk_100 = fret_07
                    cVar4 = quest:GetStateBool("FightFinished")
                    c_stk_259 = 0
                    c_stk_229 = 0
                    while not cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d4c6c8 end
                        if quest:GetMasterGameState("GuildWarningOccuring") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d4c6c8 end
                            c_stk_229 = 1
                            c_stk_259 = 1
                            quest:SetStateBool("FightFinished", true)
                        end
                        if not (r4 ~= nil and not r4:IsNull()) then
                            cVar4 = 0
                        else
                            cVar4 = r4:MsgIsHitByHeroWithProjectileWeapon()
                        end
                        if not cVar4 then
                            if not (r4 ~= nil and not r4:IsNull()) then
                                cVar4 = 0
                            else
                                cVar4 = r4:MsgIsHitByHeroSpecialAbility(0xb)
                            end
                            if cVar4 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d4c6c8 end
                                quest:EntitySetInFaction(r4, "FACTION_HERO")
                                if (r4 ~= nil and not r4:IsNull()) then
                                    r4:SetFriendsWithEverythingFlag(1)
                                end
                                xStack_bc = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                x_stk_c = resources:ScriptThing(xStack_254)
                                pCVar7 = x_stk_c
                                fret_09 = quest:GetHealth(pCVar7)
                                fVar2 = 0.0
                                if fVar2 < fret_09 then
                                    iVar18 = 0
                                    iVar17 = 1
                                    iVar14 = 0
                                    iVar5 = 0
                                    pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_NO_LIGHTNING"
                                    pCVar7 = quest:GetHero()
                                    r10 = me:Speak(pCVar7, pcVar13, iVar5, (iVar14 ~= 0), (iVar17 ~= 0), (iVar18 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(xStack_bc)
                                            goto LAB_00d4c6c8
                                        end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_bc)
                                        goto LAB_00d4c6c8
                                    end
                                end
                                quest:SetStateBool("FightFinished", true)
                                c_stk_259 = 1
                                quest:PauseAllNonScriptedEntities(false)
                                -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_bc;
                                goto LAB_00d4b6f6
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d4c6c8 end
                            quest:EntitySetInFaction(r4, "FACTION_HERO")
                            if (r4 ~= nil and not r4:IsNull()) then
                                r4:SetFriendsWithEverythingFlag(1)
                            end
                            xStack_ac = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            x_stk_24 = resources:ScriptThing(xStack_254)
                            pCVar7 = x_stk_24
                            fret_08 = quest:GetHealth(pCVar7)
                            fVar2 = 0.0
                            if fVar2 < fret_08 then
                                iVar18 = 0
                                iVar17 = 1
                                iVar14 = 0
                                iVar5 = 0
                                pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_NO_BOW"
                                pCVar7 = quest:GetHero()
                                r11 = me:Speak(pCVar7, pcVar13, iVar5, (iVar14 ~= 0), (iVar17 ~= 0), (iVar18 ~= 0))
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_ac)
                                        goto LAB_00d4c6c8
                                    end
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_ac)
                                    goto LAB_00d4c6c8
                                end
                            end
                            quest:SetStateBool("FightFinished", true)
                            c_stk_259 = 1
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_ac;
                            goto LAB_00d4b6f6
                        end
                        goto FLOW_past_lab_00d4b6f6
                        ::LAB_00d4b6f6::
                        resources:DestroyMovie(this_00)
                        ::FLOW_past_lab_00d4b6f6::
                        pCVar7 = quest:GetHero()
                        fret_10 = quest:GetHealth(pCVar7)
                        if quest:ReadGlobalGameDataFloat(0xed8) <= fret_10 then
                            fret_11 = quest:GetHealth(r4)
                            if fret_11 < quest:ReadGlobalGameDataFloat(0xed8) then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d4c6c8 end
                                quest:SetStateBool("FightFinished", true)
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d4c6c8 end
                            quest:SetStateBool("FightFinished", true)
                        end
                        if not (r4 ~= nil and not r4:IsNull()) then
                            cVar4 = 0
                        else
                            cVar4 = r4:MsgIsHitByHero()
                        end
                        if not cVar4 then
                            bVar3 = quest:IsPlayerCreatureBlocking()
                            if bVar3 then
                                pCVar7 = quest:GetHero()
                                bVar3 = pCVar7:MsgIsHitBy("MeleeOpponent")
                                if not bVar3 then goto LAB_00d4b8c5 end
                                bVar3 = true
                            else
                                goto LAB_00d4b8c5
                            end
                            goto FLOW_past_lab_00d4b8c5
                            ::LAB_00d4b8c5::
                            bVar3 = false
                            ::FLOW_past_lab_00d4b8c5::
                            if bVar3 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d4c6c8 end
                                iVar5 = quest:GetTimer(iVar6)
                                if iVar5 < 1 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if not bVar3 then
                                        iVar6 = quest:AddNewConversation(r4, false, false)
                                        pCVar7 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar6, pCVar7)
                                        pCVar7 = quest:GetHero()
                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", r4, pCVar7, false)
                                        goto LAB_00d4b857
                                    end
                                    goto LAB_00d4c6c8
                                end
                            else
                                pCVar7 = quest:GetHero()
                                bVar3 = pCVar7:MsgIsHitBy("MeleeOpponent")
                                if bVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d4c6c8 end
                                    iVar5 = quest:GetTimer(xStack_258)
                                    if iVar5 < 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d4c6c8 end
                                        iVar6 = quest:AddNewConversation(r4, false, false)
                                        pCVar7 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar6, pCVar7)
                                        pCVar7 = quest:GetHero()
                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", r4, pCVar7, false)
                                        quest:SetTimer(xStack_258, 0xf)
                                    end
                                end
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d4c6c8 end
                            iVar5 = quest:GetTimer(iVar6)
                            if iVar5 < 9 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d4c6c8 end
                                iVar6 = quest:AddNewConversation(r4, false, false)
                                pCVar7 = quest:GetHero()
                                quest:AddPersonToConversation(iVar6, pCVar7)
                                pCVar7 = quest:GetHero()
                                quest:AddLineToConversation(iVar6, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", r4, pCVar7, false)
                                goto LAB_00d4b857
                            end
                        end
                        goto FLOW_past_lab_00d4b857
                        ::LAB_00d4b857::
                        quest:SetTimer(xStack_258, 0xf)
                        ::FLOW_past_lab_00d4b857::
                        cVar4 = quest:GetStateBool("FightFinished")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4c6c8 end
                    quest:ResetPlayerCreatureOnlyTarget()
                    quest:RemoveQuestInfoElement(x_stk_214)
                    quest:EntitySetInFaction(r4, "FACTION_HERO")
                    if (r4 ~= nil and not r4:IsNull()) then
                        r4:SetFriendsWithEverythingFlag(1)
                    end
                    quest:DisplayQuestInfo(false)
                    if c_stk_259 == 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            if c_stk_229 ~= 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if not bVar3 then
                                    quest:FadeScreenOut(0.5, 0.5)
                                    goto LAB_00d4bbeb
                                end
                                goto LAB_00d4c6c8
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d4c6c8 end
                            quest:Pause(1.0)
                            quest:FadeScreenOut(0.5, 0.5)
                            quest:SheatheHeroWeapons()
                            quest:EntitySheatheWeapons(r4, false)
                            quest:Pause(1.0)
                            ::LAB_00d4bbeb::
                            quest:ChangeHeroHealthBy(1000.0, true, false)
                            quest:ModifyThingHealth(r4, 1000.0, false)
                            uVar16 = 0
                            pCVar7 = quest:GetThingWithScriptName("M_MeleeHeroStand")
                            pCVar8 = quest:GetHero()
                            quest:EntityTeleportToThing(pCVar8, pCVar7, (uVar16 ~= 0))
                            bVar3 = false
                            pCVar7 = quest:GetThingWithScriptName("M_MeleeOpponentStand")
                            quest:EntityTeleportToThing(r4, pCVar7, bVar3)
                            quest:EntitySetInFaction(r4, "FACTION_HERO")
                            quest:FadeScreenIn()
                            __native_entity_state:SetStateBool("WaitingForFight", true)
                            goto LAB_00d4c3f3
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d4c6c8 end
                        pCVar7 = quest:GetHero()
                        fret_12 = quest:GetHealth(pCVar7)
                        x_stk_214 = fret_12
                        fret_13 = quest:GetHealth(r4)
                        ixVar11 = 0
                        f_stk_210 = ((f_stk_100 - fret_13) - (f_stk_210 - x_stk_214))
                        iVar5 = 0
                        repeat
                            iVar6 = iVar5
                            if quest:ReadGlobalGameDataFloatAt(0xeb4, ixVar11) <= f_stk_210 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d4c6c8 end
                                break
                            end
                            ixVar11 = ixVar11 + 1
                            iVar5 = iVar6 + 1
                        until not (iVar6 + 1 < 7)
                        xStack_1fc = resources:NewResource()
                        resources:PrepareResource(xStack_1fc)
                        bVar3 = resources:TryAcquire(xStack_1fc, r4, 4)
                        while not bVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d4c6bf end
                            bVar3 = resources:TryAcquire(xStack_1fc, r4, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            xStack_1ec = resources:NewResource()
                            resources:PrepareResource(xStack_1ec)
                            iVar14 = 4
                            pCVar15 = xStack_1ec
                            pCVar7 = quest:GetHero()
                            bVar3 = resources:TryAcquire(pCVar15, pCVar7, iVar14)
                            while not bVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d4c6b3 end
                                iVar14 = 4
                                pCVar15 = xStack_1ec
                                pCVar7 = quest:GetHero()
                                bVar3 = resources:TryAcquire(pCVar15, pCVar7, iVar14)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if not bVar3 then
                                xStack_220 = resources:NewActorMap()
                                resources:SetActor(xStack_220, "ME", xStack_254)
                                resources:SetActor(xStack_220, "HERO", xStack_1ec)
                                resources:SetActor(xStack_220, "WHISPER", xStack_1fc)
                                xStack_fc = resources:StartMovie("")
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_END", xStack_220, false, true)
                                resources:SetActor(xStack_220, "ME", xStack_254)
                                native_arg_switch_2 = iVar6
                                repeat
                                    if native_arg_switch_2 == 0 then
                                        if quest:GetMasterGameState("GlobalMeleeGrade") ~= 7 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if not bVar3 then
                                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_APLUS_PRIZE", xStack_220, false, true)
                                                quest:ClearThingHasInformation(me)
                                                goto FLOW_native_label_1
                                            end
                                            quest:PauseAllNonScriptedEntities(false)
                                            -- LAB_00d4c69e_c17: (native jump target)
                                            resources:DestroyMovie(xStack_fc)
                                            resources:DestroyActorMap(xStack_220)
                                            goto LAB_00d4c6b3
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if not bVar3 then
                                            resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_APLUS", xStack_220, false, true)
                                            break
                                        end
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_fc)
                                        resources:DestroyActorMap(xStack_220)
                                        goto LAB_00d4c6b3
                                    else
                                        if native_arg_switch_2 == 1 then
                                            resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_A", xStack_220, false, true)
                                            break
                                        else
                                            if native_arg_switch_2 == 2 then
                                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_B", xStack_220, false, true)
                                                break
                                            else
                                                if native_arg_switch_2 == 3 then
                                                    resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_C", xStack_220, false, true)
                                                    break
                                                else
                                                    if native_arg_switch_2 == 4 then
                                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_D", xStack_220, false, true)
                                                        break
                                                    else
                                                        if native_arg_switch_2 == 5 then
                                                            resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_E", xStack_220, false, true)
                                                            break
                                                        else
                                                            if native_arg_switch_2 == 6 then
                                                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_F", xStack_220, false, true)
                                                                break
                                                            else
                                                                goto FLOW_native_label_1
                                                            end
                                                        end
                                                    end
                                                end
                                            end
                                        end
                                    end
                                until not (false)
                                ::FLOW_native_label_1::
                                if quest:GetMasterGameState("GlobalMeleeGrade") < 7 - iVar6 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        -- LAB_00d4c692: (native jump target)
                                        quest:PauseAllNonScriptedEntities(false)
                                        -- LAB_00d4c69e: (native jump target)
                                        resources:DestroyMovie(xStack_fc)
                                        resources:DestroyActorMap(xStack_220)
                                        goto LAB_00d4c6b3
                                    end
                                    quest:SetMasterGameState("GlobalMeleeGrade", 7 - iVar6)
                                end
                                resources:SetActor(xStack_220, "ME", xStack_254)
                                resources:SetActor(xStack_220, "HERO", xStack_1ec)
                                resources:SetActor(xStack_220, "WHISPER", xStack_1fc)
                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_OVER", xStack_220, false, true)
                                quest:FixMovieSequenceCamera(false)
                                __native_entity_state:SetStateBool("WaitingForFight", true)
                                quest:ChangeHeroHealthBy(1000.0, true, false)
                                quest:ModifyThingHealth(r4, 1000.0, false)
                                quest:EntitySetInFaction(r4, "FACTION_HERO")
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_fc)
                                resources:DestroyActorMap(xStack_220)
                                resources:ReleaseResource(xStack_1ec)
                                resources:ReleaseResource(xStack_1fc)
                                goto LAB_00d4c3f3
                            end
                            ::LAB_00d4c6b3::
                            resources:ReleaseResource(xStack_1ec)
                        end
                        ::LAB_00d4c6bf::
                        resources:ReleaseResource(xStack_1fc)
                    end
                    goto FLOW_past_lab_00d4c3f3
                    ::LAB_00d4c3f3::
                    quest:SetStateBool("StartedMeleeTesting", false)
                    quest:SetMasterGameState("HeroTakingGuildTest", false)
                    quest:DeregisterTimer(xStack_258)
                    goto LAB_00d4c40d
                    ::FLOW_past_lab_00d4c3f3::
                    ::LAB_00d4c6c8::
                    quest:DeregisterTimer(xStack_258)
                end
                goto FLOW_past_lab_00d4c40d
                ::LAB_00d4c40d::
                goto LAB_00d4c416
                ::FLOW_past_lab_00d4c40d::
                ::LAB_00d4c6d1::
            end
            ::LAB_00d4c6da::
            quest:DeregisterTimer(xStack_260)
            resources:ReleaseResource(xStack_254)
            return
        end
        ::LAB_00d4c416::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
    until false
end

function Init(quest, me)
    __native_entity_state:SetStateBool("WaitingForFight", true)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

