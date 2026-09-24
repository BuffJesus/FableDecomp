-- Generated native draft: WillApprentice. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, bVar4, cVar5, c_stk_115, c_stk_131, fVar18, fVar3, f_stk_108, fret_0, fret_00, fret_01, fret_02, fret_03, iVar13, iVar14, iVar16, iVar17, iVar6, i_stk_150, i_stk_70, ixVar11, native_arg_sequence_1, native_arg_switch_2, p0, pCVar15, pCVar7, pCVar8, pcVar12, r1, r2, r3, r4, r5, r6, r7, timerId, xStack_104, xStack_114, xStack_138, xStack_148, xStack_14c, xStack_4c, xStack_5c, xStack_6c, xStack_d8, x_stk_18, x_stk_24, x_stk_30, x_stk_3c, x_stk_c
    local alive = true
    local function __cleanup_LAB_00d504b9()
        quest:DeregisterTimer(i_stk_150)
        resources:ReleaseResource(xStack_148)
    end
    local function __cleanup_LAB_00d505a6()
        quest:DeregisterTimer(i_stk_150)
        resources:ReleaseResource(xStack_148)
    end
    xStack_148 = resources:NewResource()
    resources:PrepareResource(xStack_148)
    bVar4 = resources:TryAcquire(xStack_148, me, 4)
    while not bVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            resources:ReleaseResource(xStack_148)
            return
        end
        bVar4 = resources:TryAcquire(xStack_148, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        -- LAB_00d505c1: (native jump target)
        resources:ReleaseResource(xStack_148)
        return
    end
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, true)
    r1 = quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:EntityAttachToVillage(me, r1)
    me:SetFriendsWithEverythingFlag(true)
    c_stk_131 = 0
    r2 = quest:GetThingWithScriptName("WillApprenticeTargetMarker")
    i_stk_150 = quest:RegisterTimer()
    quest:SetTimer(i_stk_150, 10)
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    repeat
        if bVar4 then
            quest:DeregisterTimer(i_stk_150)
            r2 = nil
            r1 = nil
            -- LAB_00d50495: (native jump target)
            resources:ReleaseResource(xStack_148)
            return
        end
        bVar4 = quest:IsQuestActive("Q_GuildTrainingDeparture")
        if not bVar4 then goto LAB_00d4f285 end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            __cleanup_LAB_00d504b9()
            return
        end
        if ((3 < quest:GetMasterGameState("GlobalMeleeGrade")) or (3 < quest:GetMasterGameState("GlobalSkillGrade"))) or (3 < quest:GetMasterGameState("GlobalWillGrade")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                if c_stk_131 ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d505a6(); return end
                    quest:ClearThingHasInformation(me)
                    c_stk_131 = 0
                end
                goto LAB_00d4f285
            end
            __cleanup_LAB_00d504b9(); return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then __cleanup_LAB_00d505a6(); return end
        if c_stk_131 == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then __cleanup_LAB_00d505a6(); return end
            quest:SetThingHasInformation(me, false, true, false)
            c_stk_131 = 1
        end
        ::LAB_00d4f285::
        bVar4 = quest:IsDistanceBetweenThingsOver(me, r2, 4.0)
        __native_condition_1 = not bVar4
        if not __native_condition_1 then
            iVar6 = me:IsPerformingScriptTask()
            __native_condition_1 = iVar6
        end
        if __native_condition_1 then
            iVar6 = me:IsPerformingScriptTask()
            if iVar6 then goto LAB_00d4f49e end
            fVar18 = 10.0
            pCVar7 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar7, fVar18)
            native_arg_sequence_1 = false
            if not bVar4 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                iVar6 = quest:GetTimer(i_stk_150)
                if 0 < iVar6 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then goto LAB_00d4f49e end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                bVar4 = false
                pCVar7 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, pCVar7, bVar4)
                quest:SetTimer(i_stk_150, 0x14)
                iVar13 = quest:AddNewConversation(me, false, false)
                pCVar7 = quest:GetHero()
                quest:AddPersonToConversation(iVar13, pCVar7)
                iVar6 = quest:GetMasterGameState("GlobalWillGrade")
                if iVar6 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        pCVar7 = quest:GetHero()
                        quest:AddLineToConversation(iVar13, "TEXT_QST_028_APPRENTICE_WILL_EARLY_COMMENT", me, pCVar7, false)
                        goto LAB_00d4f499
                    end
                else
                    if iVar6 == 7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            pCVar7 = quest:GetHero()
                            quest:AddLineToConversation(iVar13, "TEXT_QST_028_APPRENTICE_WILL_APLUS_COMMENT", me, pCVar7, false)
                            goto LAB_00d4f499
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            pCVar7 = quest:GetHero()
                            quest:AddLineToConversation(iVar13, "TEXT_QST_028_APPRENTICE_WILL_NOT_APLUS_COMMENT", me, pCVar7, false)
                            goto LAB_00d4f499
                        end
                    end
                end
                goto FLOW_past_lab_00d4f499
                ::LAB_00d4f499::
                goto LAB_00d4f49e
                ::FLOW_past_lab_00d4f499::
            end
            __cleanup_LAB_00d505a6(); return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then __cleanup_LAB_00d504b9(); return end
        if not (r2 ~= nil and not r2:IsNull()) then
            p0 = {x = 0, y = 0, z = 0}
        else
            p0 = r2:GetPos()
        end
        me:MoveToPosition(p0, 3.0, 0, false, true)
        ::LAB_00d4f49e::
        bVar4 = me:IsTalkedToByHero()
        if bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then __cleanup_LAB_00d504b9(); return end
            iVar6 = me:IsPerformingScriptTask()
            if not iVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d505a6(); return end
                if quest:GetMasterGameState("HeroTakingGuildTest") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        xStack_5c = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_18 = resources:ScriptThing(xStack_148)
                        pCVar8 = x_stk_18
                        fret_00 = quest:GetHealth(pCVar8)
                        fVar3 = 0.0
                        if fVar3 < fret_00 then
                            iVar16 = 0
                            iVar14 = 1
                            iVar13 = 0
                            iVar6 = 0
                            pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_MULTI_GRADE"
                            pCVar8 = quest:GetHero()
                            r3 = me:Speak(pCVar8, pcVar12, iVar6, (iVar13 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar5 = iVar6
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_5c)
                                    __cleanup_LAB_00d505a6(); return
                                end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar5 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_5c)
                                __cleanup_LAB_00d505a6(); return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_5c)
                        goto LAB_00d503cb
                    end
                    __cleanup_LAB_00d505a6(); return
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d505a6(); return end
                xStack_104 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_WILL_HELLO", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                while iVar6 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_104)
                        __cleanup_LAB_00d505a6(); return
                    end
                    iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_104)
                    __cleanup_LAB_00d505a6(); return
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if iVar6 ~= 1 then
                    if not bVar4 then
                        x_stk_30 = resources:ScriptThing(xStack_148)
                        pCVar8 = x_stk_30
                        fret_03 = quest:GetHealth(pCVar8)
                        fVar3 = 0.0
                        if fVar3 < fret_03 then
                            iVar17 = 0
                            iVar16 = 1
                            iVar14 = 0
                            iVar13 = 0
                            pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_RETURN"
                            pCVar8 = quest:GetHero()
                            r4 = me:Speak(pCVar8, pcVar12, iVar13, (iVar14 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                            iVar13 = me:IsPerformingScriptTask()
                            cVar5 = iVar13
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then goto LAB_00d50542 end
                                iVar13 = me:IsPerformingScriptTask()
                                cVar5 = iVar13
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d50542 end
                        end
                        goto LAB_00d4fb0a
                    end
                    ::LAB_00d50542::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_104)
                    __cleanup_LAB_00d505a6(); return
                end
                if bVar4 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_104)
                    __cleanup_LAB_00d505a6(); return
                end
                x_stk_c = resources:ScriptThing(xStack_148)
                pCVar8 = x_stk_c
                fret_01 = quest:GetHealth(pCVar8)
                fVar3 = 0.0
                if fVar3 < fret_01 then
                    iVar17 = 0
                    iVar16 = 1
                    iVar14 = 0
                    iVar13 = 0
                    pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_INTRO"
                    pCVar8 = quest:GetHero()
                    r5 = me:Speak(pCVar8, pcVar12, iVar13, (iVar14 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                    iVar13 = me:IsPerformingScriptTask()
                    cVar5 = iVar13
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_104)
                            __cleanup_LAB_00d505a6(); return
                        end
                        iVar13 = me:IsPerformingScriptTask()
                        cVar5 = iVar13
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_104)
                        __cleanup_LAB_00d505a6(); return
                    end
                end
                if quest:GetMasterGameState("GlobalWillGrade") ~= 7 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        -- LAB_00d4fa43: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_104)
                        __cleanup_LAB_00d505a6(); return
                    end
                    x_stk_24 = resources:ScriptThing(xStack_148)
                    pCVar8 = x_stk_24
                    fret_02 = quest:GetHealth(pCVar8)
                    fVar3 = 0.0
                    if fVar3 < fret_02 then
                        iVar17 = 0
                        iVar16 = 1
                        iVar14 = 0
                        iVar13 = 0
                        pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_INTRO_APLUS"
                        pCVar8 = quest:GetHero()
                        r6 = me:Speak(pCVar8, pcVar12, iVar13, (iVar14 ~= 0), (iVar16 ~= 0), (iVar17 ~= 0))
                        iVar13 = me:IsPerformingScriptTask()
                        cVar5 = iVar13
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_104)
                                __cleanup_LAB_00d505a6(); return
                            end
                            iVar13 = me:IsPerformingScriptTask()
                            cVar5 = iVar13
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_104)
                            __cleanup_LAB_00d505a6(); return
                        end
                    end
                end
                ::LAB_00d4fb0a::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_104)
                if iVar6 ~= 1 then goto LAB_00d503cb end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d505a6(); return end
                quest:SetPlayerUsingWillDummies(true)
                quest:SetMasterGameState("HeroTakingGuildTest", true)
                bVar4 = quest:MsgOnHeroCastSpell()
                while not bVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d505a6(); return end
                    bVar4 = quest:MsgOnHeroCastSpell()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d505a6(); return end
                xStack_138 = quest:RegisterTimer()
                iVar13 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xf08)))
                quest:SetTimer(xStack_138, iVar13)
                quest:SetMasterGameState("WillScore", 0)
                quest:SetTimer(quest:GetStateInt("WillHelpTimer"), 0)
                iVar6 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                i_stk_70 = iVar6
                f_stk_108 = quest:AddQuestInfoTimer(xStack_138, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                cVar5 = false
                c_stk_115 = 0
                xStack_14c = quest:RegisterTimer()
                timerId = xStack_14c
                quest:SetTimer(xStack_14c, 0)
                iVar13 = quest:GetTimer(xStack_138)
                while (0 < iVar13 and (not cVar5)) do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d50594 end
                    iVar6 = quest:GetTimer(timerId)
                    if iVar6 < 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594 end
                        bVar4 = false
                        pCVar8 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, pCVar8, bVar4)
                        quest:SetTimer(timerId, 2)
                    end
                    if quest:GetMasterGameState("GuildWarningOccuring") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594 end
                        cVar5 = true
                        c_stk_115 = 1
                    end
                    iVar6 = quest:GetHeroWillEnergy()
                    __native_condition_2 = iVar6 == 0
                    if __native_condition_2 then
                        iVar6 = quest:GetTimer(quest:GetStateInt("WillHelpTimer"))
                        __native_condition_2 = iVar6 < 1
                    end
                    if __native_condition_2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594 end
                        iVar13 = quest:AddNewConversation(me, false, false)
                        pCVar8 = quest:GetHero()
                        quest:AddPersonToConversation(iVar13, pCVar8)
                        pCVar8 = quest:GetHero()
                        quest:AddLineToConversation(iVar13, "TEXT_QST_028_APPRENTICE_WILL_NO_WILL", me, pCVar8, false)
                        quest:SetTimer(quest:GetStateInt("WillHelpTimer"), 8)
                        timerId = xStack_14c
                        cVar5 = c_stk_115
                    end
                    fVar18 = 30.0
                    pCVar8 = quest:GetHero()
                    bVar4 = quest:IsDistanceBetweenThingsOver(pCVar8, me, fVar18)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594 end
                        cVar5 = true
                        c_stk_115 = 1
                    end
                    iVar6 = i_stk_70
                    quest:UpdateQuestInfoCounter(i_stk_70, quest:GetMasterGameState("WillScore"), -1)
                    iVar13 = quest:GetTimer(xStack_138)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    bVar4 = quest:IsHeroControlledByPlayer()
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594 end
                        bVar4 = quest:IsHeroControlledByPlayer()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d50594 end
                    quest:DisplayQuestInfo(false)
                    quest:RemoveQuestInfoElement(iVar6)
                    quest:RemoveQuestInfoElement(f_stk_108)
                    if not cVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d50594 end
                        f_stk_108 = quest:GetMasterGameState("WillScore")
                        ixVar11 = 0
                        iVar6 = 0
                        repeat
                            iVar13 = iVar6
                            if quest:ReadGlobalGameDataFloatAt(0xecc, ixVar11) <= f_stk_108 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then goto LAB_00d50594 end
                                break
                            end
                            ixVar11 = ixVar11 + 1
                            iVar6 = iVar13 + 1
                        until not (iVar13 + 1 < 7)
                        xStack_d8 = resources:NewResource()
                        resources:PrepareResource(xStack_d8)
                        iVar14 = 4
                        pCVar15 = xStack_d8
                        pCVar8 = quest:GetHero()
                        bVar4 = resources:TryAcquire(pCVar15, pCVar8, iVar14)
                        while not bVar4 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                resources:ReleaseResource(xStack_d8)
                                goto LAB_00d50594
                            end
                            iVar14 = 4
                            pCVar15 = xStack_d8
                            pCVar8 = quest:GetHero()
                            bVar4 = resources:TryAcquire(pCVar15, pCVar8, iVar14)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            -- LAB_00d50588: (native jump target)
                            resources:ReleaseResource(xStack_d8)
                            goto LAB_00d50594
                        end
                        xStack_114 = resources:NewActorMap()
                        resources:SetActor(xStack_114, "ME", xStack_148)
                        resources:SetActor(xStack_114, "HERO", xStack_d8)
                        xStack_6c = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_END", xStack_114, false, true)
                        resources:SetActor(xStack_114, "ME", xStack_148)
                        native_arg_switch_2 = iVar13
                        repeat
                            if native_arg_switch_2 == 0 then
                                if quest:GetMasterGameState("GlobalWillGrade") ~= 7 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if not bVar4 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_APLUS_PRIZE", xStack_114, false, true)
                                        quest:ClearThingHasInformation(me)
                                        goto FLOW_native_label_1
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- LAB_00d50573_c11: (native jump target)
                                    resources:DestroyMovie(xStack_6c)
                                    resources:DestroyActorMap(xStack_114)
                                    resources:ReleaseResource(xStack_d8)
                                    goto LAB_00d50594
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if not bVar4 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_APLUS", xStack_114, false, true)
                                    break
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_6c)
                                resources:DestroyActorMap(xStack_114)
                                resources:ReleaseResource(xStack_d8)
                                goto LAB_00d50594
                            else
                                if native_arg_switch_2 == 1 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_A", xStack_114, false, true)
                                    break
                                else
                                    if native_arg_switch_2 == 2 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_B", xStack_114, false, true)
                                        break
                                    else
                                        if native_arg_switch_2 == 3 then
                                            resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_C", xStack_114, false, true)
                                            break
                                        else
                                            if native_arg_switch_2 == 4 then
                                                resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_D", xStack_114, false, true)
                                                break
                                            else
                                                if native_arg_switch_2 == 5 then
                                                    resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_E", xStack_114, false, true)
                                                    break
                                                else
                                                    if native_arg_switch_2 == 6 then
                                                        resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_F", xStack_114, false, true)
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
                        if quest:GetMasterGameState("GlobalWillGrade") < 7 - iVar13 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                -- LAB_00d50567: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                -- LAB_00d50573: (native jump target)
                                resources:DestroyMovie(xStack_6c)
                                resources:DestroyActorMap(xStack_114)
                                resources:ReleaseResource(xStack_d8)
                                goto LAB_00d50594
                            end
                            quest:SetMasterGameState("GlobalWillGrade", 7 - iVar13)
                        end
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_6c)
                        resources:DestroyActorMap(xStack_114)
                        resources:ReleaseResource(xStack_d8)
                    end
                    quest:SetMasterGameState("HeroTakingGuildTest", false)
                    quest:SetPlayerUsingWillDummies(false)
                    quest:DeregisterTimer(xStack_14c)
                    quest:DeregisterTimer(xStack_138)
                    goto LAB_00d503cb
                end
                ::LAB_00d50594::
                quest:DeregisterTimer(xStack_14c)
                quest:DeregisterTimer(xStack_138)
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    me:ClearCommands()
                    xStack_4c = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    x_stk_3c = resources:ScriptThing(xStack_148)
                    pCVar8 = x_stk_3c
                    fret_0 = quest:GetHealth(pCVar8)
                    fVar3 = 0.0
                    if fVar3 < fret_0 then
                        iVar16 = 0
                        iVar14 = 1
                        iVar13 = 0
                        iVar6 = 0
                        pcVar12 = "TEXT_QST_028_APPRENTICE_WILL_EARLY"
                        pCVar8 = quest:GetHero()
                        r7 = me:Speak(pCVar8, pcVar12, iVar6, (iVar13 ~= 0), (iVar14 ~= 0), (iVar16 ~= 0))
                        iVar6 = me:IsPerformingScriptTask()
                        cVar5 = iVar6
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_4c)
                                __cleanup_LAB_00d505a6(); return
                            end
                            iVar6 = me:IsPerformingScriptTask()
                            cVar5 = iVar6
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_4c)
                            __cleanup_LAB_00d505a6(); return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_4c)
                    goto LAB_00d503cb
                end
            end
            __cleanup_LAB_00d505a6()
            return
        end
        ::LAB_00d503cb::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
    until false
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

