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
    local __native_condition_1, bVar3, cVar4, c_stk_229, c_stk_22a, c_stk_259, dist, fVar2, f_stk_100, f_stk_210, fret_03, fret_04, fret_05, fret_06, fret_07, fret_10, fret_11, fret_12, fret_13, iVar14, iVar17, iVar18, iVar5, iVar6, native_arg_sequence_1, native_arg_switch_2, p0, pCVar15, pCVar7, pCVar8, pcVar13, pfVar11, piVar12, r1, r10, r11, r12, r13, r14, r15, r16, r17, r2, r3, r4, r5, r6, r7, r8, r9, thing, uVar1, uVar16, u_stk_228, xStack_18, xStack_1ec, xStack_1fc, xStack_20c, xStack_220, xStack_254, xStack_78, xStack_84, xStack_ac, xStack_bc, xStack_cc, xStack_dc, xStack_ec, xStack_fc, x_stk_214, x_stk_24, x_stk_30, x_stk_60, x_stk_c
    local alive = true
    u_stk_228 = 0
    xStack_254 = resources:NewResource()
    bVar3 = false
    if bVar3 ~= 0 then
    end
    bVar3 = resources:TryAcquire(xStack_254, me, 4)
    while not bVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then return end
        bVar3 = resources:TryAcquire(xStack_254, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if bVar3 then
        -- LAB_00d4c6ec: (native jump target)
        resources:DestroyMovie(xStack_ec)
        return
    end
    uVar1 = __native_entity_state:GetStateInt("self_0xc")
    -- TODO(native): thing._4_4_ = uVar1;
    thing = nil
    -- TODO(native): thing._8_4_ = piVar12;
    quest:SetIsPushableByHero(thing, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(me)
    iVar5 = quest:RegisterTimer()
    quest:SetTimer(iVar5, 10)
    r1 = quest:GetThingWithScriptName("CombatApprenticeTargetMarker")
    c_stk_22a = 0
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    repeat
        if bVar3 then
            r1 = nil
            quest:DeregisterTimer(iVar5)
            return
        end
        bVar3 = quest:IsQuestActive("Q_GuildTrainingDeparture")
        if not bVar3 then goto LAB_00d4a512 end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            quest:DeregisterTimer(iVar5)
            resources:DestroyMovie(xStack_cc)
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
                        resources:DestroyMovie(xStack_ec)
                        return
                    end
                    quest:SetThingHasInformation(me, false, true, false)
                    c_stk_22a = '\x01'
                end
                goto LAB_00d4a512
            end
            resources:DestroyMovie(xStack_ec)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            quest:DeregisterTimer(iVar5)
            resources:DestroyMovie(xStack_dc)
            return
        end
        if c_stk_22a ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                resources:DestroyMovie(xStack_ec)
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
                iVar5 = quest:GetTimer(iVar5)
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
                quest:SetTimer(iVar5, xStack_260)
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
                        goto LAB_00d4a71c
                    end
                else
                    if iVar5 == 7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            pCVar8 = quest:GetHero()
                            quest:AddLineToConversation(iVar6, "TEXT_QST_028_APPRENTICE_MELEE_APLUS_COMMENT", me, pCVar8, false)
                            goto LAB_00d4a71c
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            pCVar8 = quest:GetHero()
                            quest:AddLineToConversation(iVar6, "TEXT_QST_028_APPRENTICE_MELEE_NOT_APLUS_COMMENT", me, pCVar8, false)
                            -- LAB_00d4a717: (native jump target)
                            goto LAB_00d4a71c
                        end
                    end
                end
            end
            resources:DestroyMovie(xStack_ec)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00d4c4f5: (native jump target)
            quest:DeregisterTimer(iVar5)
            resources:DestroyMovie(xStack_cc)
            return
        end
        if not (r1 ~= nil and not r1:IsNull()) then
        else
            p0 = r1:GetPos()
        end
        me:MoveToPosition(p0, 0x40400000, 1, false, true)
        ::LAB_00d4a71c::
        if not __native_entity_state:GetStateBool("WaitingForFight") then
            -- LAB_00d4a758: (native jump target)
            bVar3 = false
        else
            u_stk_228 = u_stk_228 | 1
            bVar3 = me:IsTalkedToByHero()
            if not bVar3 then
                bVar3 = false
                goto FLOW_after_lab_00d4a758
            end
            bVar3 = true
        end
        ::FLOW_after_lab_00d4a758::
        if (u_stk_228 & 1) ~= 0 then
            u_stk_228 = u_stk_228 & 0xfffffffe
        end
        if bVar3 then
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                quest:DeregisterTimer(iVar5)
                resources:DestroyMovie(xStack_20c)
                return
            end
            iVar5 = me:IsPerformingScriptTask()
            if iVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d4c6da end
                me:ClearCommands()
                xStack_ec = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                x_stk_60 = resources:ScriptThing(0)
                pCVar7 = x_stk_60
                r2 = quest:GetHealth(pCVar7)
                fVar2 = 0.0
                if fVar2 < fret_0 then
                    iVar17 = 0
                    iVar14 = 1
                    iVar6 = 0
                    iVar5 = 0
                    pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_EARLY"
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
                            resources:DestroyMovie(xStack_20c)
                            goto LAB_00d4c6da
                        end
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_bc)
                        goto LAB_00d4c6da
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_fc)
                goto LAB_00d4c416
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        xStack_dc = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_30 = resources:ScriptThing(0)
                        pCVar7 = x_stk_30
                        r4 = quest:GetHealth(pCVar7)
                        fVar2 = 0.0
                        if fVar2 < fret_00 then
                            iVar17 = 0
                            iVar14 = 1
                            iVar6 = 0
                            iVar5 = 0
                            pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_OTHER_MELEE_GRADE"
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
                                    resources:ReleaseResource(xStack_1ec)
                                    goto LAB_00d4c6da
                                end
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(xStack_1fc)
                                goto LAB_00d4c6da
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(0)
                        goto LAB_00d4c416
                    end
                    goto LAB_00d4c6da
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d4c6da end
                r6 = quest:GetThingWithScriptName("MeleeApprentice")
                iVar5 = (r6 ~= nil and r6:IsAlive())
                if not iVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if not bVar3 then
                        xStack_cc = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        xStack_84 = resources:ScriptThing(0)
                        pCVar7 = xStack_84
                        r7 = quest:GetHealth(pCVar7)
                        fVar2 = 0.0
                        if fVar2 < fret_01 then
                            iVar17 = 0
                            iVar14 = 1
                            iVar6 = 0
                            iVar5 = 0
                            pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_NO_WHISPER"
                            pCVar7 = quest:GetHero()
                            r8 = me:Speak(pCVar7, pcVar13, iVar5, (iVar6 ~= 0), (iVar14 ~= 0), (iVar17 ~= 0))
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:ReleaseResource(0)
                                    goto LAB_00d4c6d1
                                end
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(0)
                                goto LAB_00d4c6d1
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_ec)
                        -- LAB_00d4c40d: (native jump target)
                        goto LAB_00d4c416
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4c6d1 end
                    xStack_20c = resources:StartMovie("")
                    quest:StartMovieSequence()
                    x_stk_214 = piVar12
                    quest:PauseAllNonScriptedEntities(true)
                    xStack_78 = resources:ScriptThing(0)
                    pCVar7 = xStack_78
                    r9 = quest:GetHealth(pCVar7)
                    fVar2 = 0.0
                    if fVar2 < fret_02 then
                        iVar17 = 0
                        iVar14 = 1
                        iVar6 = 0
                        iVar5 = 0
                        pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_HELLO"
                        pCVar7 = quest:GetHero()
                        r10 = me:Speak(pCVar7, pcVar13, iVar5, (iVar6 ~= 0), (iVar14 ~= 0), (iVar17 ~= 0))
                        iVar5 = me:IsPerformingScriptTask()
                        cVar4 = iVar5
                        while cVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                -- LAB_00d4ae53_c9: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_dc)
                                goto LAB_00d4c6d1
                            end
                            iVar5 = me:IsPerformingScriptTask()
                            cVar4 = iVar5
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then goto LAB_00d4ac95 end
                        -- LAB_00d4c5ff: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_ec)
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
                            -- LAB_00d4ae53_c10: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_dc)
                            goto LAB_00d4c6d1
                        end
                        iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_ec)
                        goto LAB_00d4c6d1
                    end
                    if iVar5 == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            -- LAB_00d4ae53: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_dc)
                            goto LAB_00d4c6d1
                        end
                        xStack_18 = resources:ScriptThing(0)
                        pCVar7 = xStack_18
                        fret_03 = quest:GetHealth(pCVar7)
                        c_stk_259 = '\x01'
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
                            r11 = me:Speak(pCVar7, pcVar13, iVar6, (iVar14 ~= 0), (iVar17 ~= 0), (iVar18 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar4 = iVar6
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_ec)
                                    goto LAB_00d4c6d1
                                end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar4 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_dc)
                                goto LAB_00d4c6d1
                            end
                        end
                    else
                        if iVar5 ~= 1 then goto LAB_00d4b11f end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_ec)
                            goto LAB_00d4c6d1
                        end
                        x_stk_24 = resources:ScriptThing(0)
                        pCVar7 = x_stk_24
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
                            r12 = me:Speak(pCVar7, pcVar13, iVar6, (iVar14 ~= 0), (iVar17 ~= 0), (iVar18 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar4 = iVar6
                            while cVar4 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    -- LAB_00d4ae53_c15: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_dc)
                                    goto LAB_00d4c6d1
                                end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar4 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_ec)
                                goto LAB_00d4c6d1
                            end
                        end
                        if quest:GetMasterGameState("GlobalMeleeGrade") ~= 7 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                -- LAB_00d4ae53_c17: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_dc)
                                goto LAB_00d4c6d1
                            end
                            x_stk_c = resources:ScriptThing(0)
                            pCVar7 = x_stk_c
                            fret_05 = quest:GetHealth(pCVar7)
                            c_stk_259 = 0.0 < fret_05
                            if c_stk_259 ~= 0 then
                                iVar18 = 0
                                iVar17 = 1
                                iVar14 = 0
                                iVar6 = 0
                                pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_START_APLUS"
                                pCVar7 = quest:GetHero()
                                r13 = me:Speak(pCVar7, pcVar13, iVar6, (iVar14 ~= 0), (iVar17 ~= 0), (iVar18 ~= 0))
                                iVar6 = me:IsPerformingScriptTask()
                                cVar4 = iVar6
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_ec)
                                        goto LAB_00d4c6d1
                                    end
                                    iVar6 = me:IsPerformingScriptTask()
                                    cVar4 = iVar6
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    -- LAB_00d4ae53_c19: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_dc)
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
                        quest:EntityTeleportToThing(r6, pCVar7, bVar3)
                        bVar3 = false
                        pCVar7 = quest:GetHero()
                        quest:EntityUnsheatheMeleeWeapon(pCVar7, bVar3)
                        quest:EntityUnsheatheWeapons(r6, false)
                        piVar12 = x_stk_214
                    end
                    ::LAB_00d4b11f::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_dc)
                    if iVar5 ~= 1 then
                        goto LAB_00d4c416
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4c6d1 end
                    quest:FadeScreenIn()
                    quest:SetPlayerCreatureOnlyTarget(r6)
                    quest:SetMasterGameState("HeroTakingGuildTest", true)
                    quest:SetStateBool("StartedMeleeTesting", true)
                    __native_entity_state:SetStateBool("WaitingForFight", false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                    quest:ModifyThingHealth(r6, 1000.0, false)
                    quest:EntitySetAsKillable(r6, false, true)
                    quest:EntitySetCombatType(r6, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE")
                    quest:EntitySetInFaction(r6, "FACTION_BANDITS")
                    if (r6 ~= nil and not r6:IsNull()) then
                        r6:SetFriendsWithEverythingFlag(0)
                    end
                    pCVar7 = quest:GetHero()
                    quest:GiveThingBestEnemyTarget(r6, pCVar7)
                    quest:SetStateBool("FightFinished", false)
                    -- TODO(native): CTimer::CTimer((CTimer *)&xStack_258);
                    quest:SetTimer(iVar5, xStack_258)
                    quest:DisplayQuestInfo(true)
                    x_stk_214 = quest:AddQuestInfoBarHealth(r6, {R = 255, G = 255, B = 255, A = 255}, "HUD_WHISPER_ICON", 1.0)
                    pCVar7 = quest:GetHero()
                    fret_06 = quest:GetHealth(pCVar7)
                    f_stk_210 = fret_06
                    fret_07 = quest:GetHealth(r6)
                    f_stk_100 = fret_07
                    cVar4 = quest:GetStateBool("FightFinished")
                    c_stk_259 = 0
                    c_stk_229 = 0
                    while not cVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d4c6c8 end
                        if quest:GetMasterGameState("GuildWarningOccuring") == '\x01' then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d4c6c8 end
                            c_stk_229 = '\x01'
                            c_stk_259 = '\x01'
                            quest:SetStateBool("FightFinished", true)
                        end
                        if not (r6 ~= nil and not r6:IsNull()) then
                            cVar4 = 0
                        else
                            cVar4 = r6:MsgIsHitByHeroWithProjectileWeapon()
                        end
                        if not cVar4 then
                            if not (r6 ~= nil and not r6:IsNull()) then
                                cVar4 = 0
                            else
                                cVar4 = r6:MsgIsHitByHeroSpecialAbility(0xb)
                            end
                            if cVar4 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d4c6c8 end
                                quest:EntitySetInFaction(r6, "FACTION_HERO")
                                if (r6 ~= nil and not r6:IsNull()) then
                                    r6:SetFriendsWithEverythingFlag(1)
                                end
                                xStack_ac = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                r6 = resources:ScriptThing(0)
                                pCVar7 = r6
                                r14 = quest:GetHealth(pCVar7)
                                fVar2 = 0.0
                                if fVar2 < fret_09 then
                                    iVar18 = 0
                                    iVar17 = 1
                                    iVar14 = 0
                                    iVar5 = 0
                                    pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_NO_LIGHTNING"
                                    pCVar7 = quest:GetHero()
                                    r15 = me:Speak(pCVar7, pcVar13, iVar5, (iVar14 ~= 0), (iVar17 ~= 0), (iVar18 ~= 0))
                                    iVar5 = me:IsPerformingScriptTask()
                                    cVar4 = iVar5
                                    while cVar4 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(xStack_cc)
                                            goto LAB_00d4c6c8
                                        end
                                        iVar5 = me:IsPerformingScriptTask()
                                        cVar4 = iVar5
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_cc)
                                        goto LAB_00d4c6c8
                                    end
                                end
                                quest:SetStateBool("FightFinished", true)
                                c_stk_259 = '\x01'
                                quest:PauseAllNonScriptedEntities(false)
                                -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_cc;
                                resources:DestroyMovie(this_00)
                                goto FLOW_after_lab_00d4b6f6
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d4c6c8 end
                            quest:EntitySetInFaction(r6, "FACTION_HERO")
                            if (r6 ~= nil and not r6:IsNull()) then
                                r6:SetFriendsWithEverythingFlag(1)
                            end
                            xStack_bc = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            x_stk_c = resources:ScriptThing(0)
                            pCVar7 = x_stk_c
                            r16 = quest:GetHealth(pCVar7)
                            fVar2 = 0.0
                            if fVar2 < fret_08 then
                                iVar18 = 0
                                iVar17 = 1
                                iVar14 = 0
                                iVar5 = 0
                                pcVar13 = "TEXT_QST_028_APPRENTICE_MELEE_NO_BOW"
                                pCVar7 = quest:GetHero()
                                r17 = me:Speak(pCVar7, pcVar13, iVar5, (iVar14 ~= 0), (iVar17 ~= 0), (iVar18 ~= 0))
                                iVar5 = me:IsPerformingScriptTask()
                                cVar4 = iVar5
                                while cVar4 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(xStack_20c)
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
                            c_stk_259 = '\x01'
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_20c;
                            -- LAB_00d4b6f6: (native jump target)
                            resources:DestroyMovie(this_00)
                        end
                        ::FLOW_after_lab_00d4b6f6::
                        pCVar7 = quest:GetHero()
                        fret_10 = quest:GetHealth(pCVar7)
                        if quest:ReadGlobalGameData(0xed8) <= fret_10 then
                            fret_11 = quest:GetHealth(r6)
                            if fret_11 < quest:ReadGlobalGameData(0xed8) then
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
                        if not (r6 ~= nil and not r6:IsNull()) then
                            cVar4 = 0
                        else
                            cVar4 = r6:MsgIsHitByHero()
                        end
                        if not cVar4 then
                            bVar3 = quest:IsPlayerCreatureBlocking()
                            if bVar3 then
                                u_stk_228 = u_stk_228 | 2
                                pCVar7 = quest:GetHero()
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                bVar3 = pCVar7:MsgIsHitBy("MeleeOpponent")
                                if not bVar3 then
                                    bVar3 = false
                                    goto FLOW_after_lab_00d4b8c5
                                end
                                bVar3 = true
                            else
                                -- LAB_00d4b8c5: (native jump target)
                                bVar3 = false
                            end
                            ::FLOW_after_lab_00d4b8c5::
                            if (u_stk_228 & 2) ~= 0 then
                                u_stk_228 = u_stk_228 & 0xfffffffd
                            end
                            if bVar3 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d4c6c8 end
                                iVar5 = quest:GetTimer(iVar5)
                                if iVar5 < 1 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if not bVar3 then
                                        iVar6 = quest:AddNewConversation(r6, false, false)
                                        pCVar7 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar6, pCVar7)
                                        pCVar7 = quest:GetHero()
                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", r6, pCVar7, false)
                                        quest:SetTimer(iVar5, xStack_258)
                                        goto FLOW_after_lab_00d4b857
                                    end
                                    goto LAB_00d4c6c8
                                end
                            else
                                pCVar7 = quest:GetHero()
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                bVar3 = pCVar7:MsgIsHitBy("MeleeOpponent")
                                if bVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then goto LAB_00d4c6c8 end
                                    iVar5 = quest:GetTimer(iVar5)
                                    if iVar5 < 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then goto LAB_00d4c6c8 end
                                        iVar6 = quest:AddNewConversation(r6, false, false)
                                        pCVar7 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar6, pCVar7)
                                        pCVar7 = quest:GetHero()
                                        quest:AddLineToConversation(iVar6, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", r6, pCVar7, false)
                                        quest:SetTimer(iVar5, xStack_258)
                                    end
                                end
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d4c6c8 end
                            iVar5 = quest:GetTimer(iVar5)
                            if iVar5 < 9 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d4c6c8 end
                                iVar6 = quest:AddNewConversation(r6, false, false)
                                pCVar7 = quest:GetHero()
                                quest:AddPersonToConversation(iVar6, pCVar7)
                                pCVar7 = quest:GetHero()
                                quest:AddLineToConversation(iVar6, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", r6, pCVar7, false)
                                -- LAB_00d4b857: (native jump target)
                                quest:SetTimer(iVar5, xStack_258)
                            end
                        end
                        ::FLOW_after_lab_00d4b857::
                        cVar4 = quest:GetStateBool("FightFinished")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then goto LAB_00d4c6c8 end
                    quest:ResetPlayerCreatureOnlyTarget()
                    quest:RemoveQuestInfoElement(x_stk_214)
                    quest:EntitySetInFaction(r6, "FACTION_HERO")
                    if (r6 ~= nil and not r6:IsNull()) then
                        r6:SetFriendsWithEverythingFlag(1)
                    end
                    quest:DisplayQuestInfo(false)
                    if c_stk_259 == '\x01' then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            if c_stk_229 ~= '\x01' then
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
                            quest:EntitySheatheWeapons(r6, false)
                            quest:Pause(1.0)
                            ::LAB_00d4bbeb::
                            quest:ChangeHeroHealthBy(1000.0, true, false)
                            quest:ModifyThingHealth(r6, 1000.0, false)
                            uVar16 = 0
                            pCVar7 = quest:GetThingWithScriptName("M_MeleeHeroStand")
                            pCVar8 = quest:GetHero()
                            quest:EntityTeleportToThing(pCVar8, pCVar7, (uVar16 ~= 0))
                            bVar3 = false
                            pCVar7 = quest:GetThingWithScriptName("M_MeleeOpponentStand")
                            quest:EntityTeleportToThing(r6, pCVar7, bVar3)
                            quest:EntitySetInFaction(r6, "FACTION_HERO")
                            quest:FadeScreenIn()
                            __native_entity_state:SetStateBool("WaitingForFight", true)
                            -- LAB_00d4c3f3: (native jump target)
                            quest:SetStateBool("StartedMeleeTesting", false)
                            quest:SetMasterGameState("HeroTakingGuildTest", false)
                            goto LAB_00d4c416
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then goto LAB_00d4c6c8 end
                        pCVar7 = quest:GetHero()
                        fret_12 = quest:GetHealth(pCVar7)
                        x_stk_214 = fret_12
                        fret_13 = quest:GetHealth(r6)
                        -- TODO(native): pfVar11 = *(float **)(DAT_0143e90c + 0xeb4);
                        f_stk_210 = ((f_stk_100 - fret_13) - (f_stk_210 - x_stk_214))
                        iVar5 = 0
                        repeat
                            iVar6 = iVar5
                            if *pfVar11 < f_stk_210 ~= (*pfVar11 == f_stk_210) then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then goto LAB_00d4c6c8 end
                                break
                            end
                            pfVar11 = pfVar11 + 1
                            iVar5 = iVar6 + 1
                        until not (iVar6 + 1 < 7)
                        xStack_1fc = resources:NewResource()
                        bVar3 = false
                        if bVar3 ~= 0 then
                        end
                        bVar3 = resources:TryAcquire(xStack_1fc, r6, 4)
                        while not bVar3 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then goto LAB_00d4c6bf end
                            bVar3 = resources:TryAcquire(xStack_1fc, r6, 4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if not bVar3 then
                            xStack_1ec = resources:NewResource()
                            bVar3 = false
                            if bVar3 ~= 0 then
                            end
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
                                resources:SetActor(xStack_220, "ME", 0)
                                resources:SetActor(xStack_220, "HERO", xStack_1ec)
                                resources:SetActor(xStack_220, "WHISPER", xStack_1fc)
                                xStack_fc = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_END", xStack_220, false, true)
                                resources:SetActor(xStack_220, "ME", 0)
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
                                            -- LAB_00d4c69e_c25: (native jump target)
                                            resources:DestroyMovie(xStack_bc)
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
                                        resources:DestroyMovie(xStack_bc)
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
                                        resources:DestroyMovie(xStack_bc)
                                        resources:DestroyActorMap(xStack_220)
                                        goto LAB_00d4c6b3
                                    end
                                    quest:SetMasterGameState("GlobalMeleeGrade", 7 - iVar6)
                                end
                                resources:SetActor(xStack_220, "ME", 0)
                                resources:SetActor(xStack_220, "HERO", xStack_fc)
                                resources:SetActor(xStack_220, "WHISPER", xStack_1ec)
                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_OVER", xStack_220, false, true)
                                quest:FixMovieSequenceCamera(false)
                                __native_entity_state:SetStateBool("WaitingForFight", true)
                                quest:ChangeHeroHealthBy(1000.0, true, false)
                                quest:ModifyThingHealth(r6, 1000.0, false)
                                quest:EntitySetInFaction(r6, "FACTION_HERO")
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_bc)
                                resources:DestroyActorMap(xStack_220)
                                resources:DestroyMovie(xStack_fc)
                                resources:ReleaseResource(xStack_1ec)
                                quest:SetStateBool("StartedMeleeTesting", false)
                                quest:SetMasterGameState("HeroTakingGuildTest", false)
                                goto LAB_00d4c416
                            end
                            ::LAB_00d4c6b3::
                            resources:ReleaseResource(xStack_1fc)
                        end
                        ::LAB_00d4c6bf::
                        resources:ReleaseResource(0)
                    end
                    ::LAB_00d4c6c8::
                end
                ::LAB_00d4c6d1::
            end
            ::LAB_00d4c6da::
            resources:DestroyMovie(xStack_ec)
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

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

