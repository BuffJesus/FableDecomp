-- Generated native draft: SkillApprentice. Review coverage report before use.
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
    local __native_condition_1, bVar4, cVar5, c_stk_13d, c_stk_14d, c_stk_14e, fVar18, fVar3, iVar14, iVar15, iVar17, iVar6, iVar7, native_arg_sequence_1, native_arg_switch_2, p0, pCVar16, pCVar8, pCVar9, pcVar13, pfVar12, r1, r10, r11, r12, r13, r14, r2, r3, r4, r5, r6, r7, r8, r9, thing, uVar1, xStack_118, xStack_128, xStack_138, xStack_13c, xStack_170, xStack_24, xStack_64, xStack_74, xStack_84, xStack_c, x_stk_12c, x_stk_18, x_stk_30, x_stk_54
    local alive = true
    local function __cleanup_LAB_00d4dd41()
        quest:DeregisterTimer(iVar6)
        resources:DestroyMovie(xStack_64)
    end
    local function __cleanup_LAB_00d4de46()
        resources:DestroyMovie(xStack_64)
    end
    xStack_170 = resources:NewResource()
    bVar4 = false
    if bVar4 ~= 0 then
    end
    bVar4 = resources:TryAcquire(xStack_170, me, 4)
    while not bVar4 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then return end
        bVar4 = resources:TryAcquire(xStack_170, me, 4)
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    if bVar4 then
        -- LAB_00d4de61: (native jump target)
        resources:DestroyMovie(xStack_64)
        return
    end
    uVar1 = __native_entity_state:GetStateInt("self_0xc")
    -- TODO(native): thing._4_4_ = uVar1;
    thing = nil
    -- TODO(native): thing._8_4_ = piVar2;
    quest:SetIsPushableByHero(thing, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, true)
    r1 = quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:EntityAttachToVillage(me, r1)
    me:SetFriendsWithEverythingFlag(me)
    __native_entity_state:SetStateBool("PlayerNotWarned", true)
    c_stk_14e = 0
    iVar6 = quest:RegisterTimer()
    quest:SetTimer(iVar6, 10)
    r2 = quest:GetThingWithScriptName("SkillApprenticeTargetMarker")
    alive = not quest:IsActiveThreadTerminating()
    bVar4 = not alive
    repeat
        if bVar4 then
            r2 = nil
            quest:DeregisterTimer(iVar6)
            r1 = nil
            return
        end
        bVar4 = quest:IsQuestActive("Q_GuildTrainingDeparture")
        if not bVar4 then goto LAB_00d4c9a2 end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then
            __cleanup_LAB_00d4dd41()
            return
        end
        if ((3 < quest:GetMasterGameState("GlobalMeleeGrade")) or (3 < quest:GetMasterGameState("GlobalSkillGrade"))) or (3 < quest:GetMasterGameState("GlobalWillGrade")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                if c_stk_14e ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d4de46(); return end
                    quest:ClearThingHasInformation(me)
                    c_stk_14e = 0
                end
                goto LAB_00d4c9a2
            end
            quest:DeregisterTimer(iVar6)
            resources:DestroyMovie(xStack_64)
            return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then __cleanup_LAB_00d4de46(); return end
        if c_stk_14e == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then __cleanup_LAB_00d4de46(); return end
            quest:SetThingHasInformation(me, false, true, false)
            c_stk_14e = '\x01'
        end
        ::LAB_00d4c9a2::
        bVar4 = quest:IsDistanceBetweenThingsOver(me, xStack_48, 4.0)
        __native_condition_1 = not bVar4
        if not __native_condition_1 then
            iVar7 = me:IsPerformingScriptTask()
            __native_condition_1 = iVar7
        end
        if __native_condition_1 then
            iVar6 = me:IsPerformingScriptTask()
            if iVar6 then goto LAB_00d4cbac end
            fVar18 = 10.0
            pCVar9 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(me, pCVar9, fVar18)
            native_arg_sequence_1 = false
            if not bVar4 then
                native_arg_sequence_1 = true
            else
                native_arg_sequence_1 = false
            end
            if not native_arg_sequence_1 then
                iVar6 = quest:GetTimer(iVar6)
                if 0 < iVar6 then
                    native_arg_sequence_1 = true
                else
                    native_arg_sequence_1 = false
                end
            end
            if native_arg_sequence_1 then goto LAB_00d4cbac end
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if not bVar4 then
                bVar4 = false
                pCVar9 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, pCVar9, bVar4)
                quest:SetTimer(iVar6, xStack_174)
                iVar7 = quest:AddNewConversation(me, false, false)
                pCVar9 = quest:GetHero()
                quest:AddPersonToConversation(iVar7, pCVar9)
                iVar6 = quest:GetMasterGameState("GlobalSkillGrade")
                if iVar6 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        pCVar9 = quest:GetHero()
                        quest:AddLineToConversation(iVar7, "TEXT_QST_028_APPRENTICE_SKILL_EARLY_COMMENT", me, pCVar9, false)
                        goto LAB_00d4cbac
                    end
                else
                    if iVar6 == 7 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            pCVar9 = quest:GetHero()
                            quest:AddLineToConversation(iVar7, "TEXT_QST_028_APPRENTICE_SKILL_APLUS_COMMENT", me, pCVar9, false)
                            goto LAB_00d4cbac
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            pCVar9 = quest:GetHero()
                            quest:AddLineToConversation(iVar7, "TEXT_QST_028_APPRENTICE_SKILL_NOT_APLUS_COMMENT", me, pCVar9, false)
                            -- LAB_00d4cba7: (native jump target)
                            goto LAB_00d4cbac
                        end
                    end
                end
            end
            __cleanup_LAB_00d4de46(); return
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
        if bVar4 then __cleanup_LAB_00d4dd41(); return end
        if not (r2 ~= nil and not r2:IsNull()) then
        else
            p0 = r2:GetPos()
        end
        me:MoveToPosition(p0, 0x40400000, 0, false, true)
        ::LAB_00d4cbac::
        bVar4 = me:IsTalkedToByHero()
        if bVar4 then
            alive = not quest:IsActiveThreadTerminating()
            bVar4 = not alive
            if bVar4 then
                quest:DeregisterTimer(iVar6)
                resources:DestroyMovie(xStack_64)
                return
            end
            iVar6 = me:IsPerformingScriptTask()
            if not iVar6 then
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d4de46(); return end
                if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        xStack_64 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        x_stk_54 = resources:ScriptThing(0)
                        pCVar8 = x_stk_54
                        r3 = quest:GetHealth(pCVar8)
                        fVar3 = 0.0
                        if fVar3 < fret_00 then
                            iVar17 = 0
                            iVar14 = 1
                            iVar7 = 0
                            iVar6 = 0
                            pcVar13 = "TEXT_QST_028_APPRENTICE_SKILL_TEST_ALREADY"
                            pCVar8 = quest:GetHero()
                            r4 = me:Speak(pCVar8, pcVar13, iVar6, (iVar7 ~= 0), (iVar14 ~= 0), (iVar17 ~= 0))
                            iVar6 = me:IsPerformingScriptTask()
                            cVar5 = iVar6
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_74)
                                    __cleanup_LAB_00d4de46(); return
                                end
                                iVar6 = me:IsPerformingScriptTask()
                                cVar5 = iVar6
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_128)
                                __cleanup_LAB_00d4de46(); return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(xStack_128)
                        goto LAB_00d4dc36
                    end
                    __cleanup_LAB_00d4de46(); return
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d4de46(); return end
                xStack_74 = resources:StartMovie("")
                quest:StartMovieSequence()
                pCVar8 = 0x1
                quest:PauseAllNonScriptedEntities((pCVar8 ~= 0))
                x_stk_30 = resources:ScriptThing(0)
                pCVar9 = x_stk_30
                r5 = quest:GetHealth(pCVar9)
                fVar3 = 0.0
                if fVar3 < fret_01 then
                    iVar17 = 0
                    iVar14 = 1
                    iVar7 = 0
                    iVar6 = 0
                    pcVar13 = "TEXT_QST_028_APPRENTICE_SKILL_HELLO"
                    pCVar9 = quest:GetHero()
                    r6 = me:Speak(pCVar9, pcVar13, iVar6, (iVar7 ~= 0), (iVar14 ~= 0), (iVar17 ~= 0))
                    iVar6 = me:IsPerformingScriptTask()
                    cVar5 = iVar6
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(xStack_118)
                            __cleanup_LAB_00d4de46(); return
                        end
                        iVar6 = me:IsPerformingScriptTask()
                        cVar5 = iVar6
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then goto LAB_00d4cf79 end
                    -- LAB_00d4d1f3: (native jump target)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_84)
                    __cleanup_LAB_00d4de46(); return
                end
                ::LAB_00d4cf79::
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_SKILL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                while iVar6 < 0 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(xStack_118)
                        __cleanup_LAB_00d4de46(); return
                    end
                    iVar6 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(xStack_84)
                    __cleanup_LAB_00d4de46(); return
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if iVar6 == 1 then
                    if not bVar4 then
                        if quest:GetMasterGameState("GlobalSkillGrade") ~= 7 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_84)
                                __cleanup_LAB_00d4de46(); return
                            end
                            xStack_c = resources:ScriptThing(0)
                            pCVar9 = xStack_c
                            r7 = quest:GetHealth(pCVar9)
                            fVar3 = 0.0
                            if fVar3 < fret_02 then
                                iVar15 = 0
                                iVar17 = 1
                                iVar14 = 0
                                iVar7 = 0
                                pcVar13 = "TEXT_QST_028_APPRENTICE_SKILL_TIME_LIMIT_APLUS"
                                pCVar9 = quest:GetHero()
                                r8 = me:Speak(pCVar9, pcVar13, iVar7, (iVar14 ~= 0), (iVar17 ~= 0), (iVar15 ~= 0))
                                iVar7 = me:IsPerformingScriptTask()
                                cVar5 = iVar7
                                while cVar5 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar4 then goto LAB_00d4dded end
                                    iVar7 = me:IsPerformingScriptTask()
                                    cVar5 = iVar7
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(xStack_84)
                                    __cleanup_LAB_00d4de46(); return
                                end
                            end
                        end
                        x_stk_18 = resources:ScriptThing(0)
                        pCVar9 = x_stk_18
                        r9 = quest:GetHealth(pCVar9)
                        fVar3 = 0.0
                        if fVar3 < fret_03 then
                            iVar15 = 0
                            iVar17 = 1
                            iVar14 = 0
                            iVar7 = 0
                            pcVar13 = "TEXT_QST_028_APPRENTICE_SKILL_TIME_LIMIT"
                            pCVar8 = quest:GetHero()
                            r10 = me:Speak(pCVar8, pcVar13, iVar7, (iVar14 ~= 0), (iVar17 ~= 0), (iVar15 ~= 0))
                            iVar7 = me:IsPerformingScriptTask()
                            cVar5 = iVar7
                            while cVar5 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then goto LAB_00d4dded end
                                iVar7 = me:IsPerformingScriptTask()
                                cVar5 = iVar7
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_84)
                                __cleanup_LAB_00d4de46(); return
                            end
                        end
                        goto LAB_00d4d2bb
                    end
                    ::LAB_00d4dded::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(xStack_118)
                    __cleanup_LAB_00d4de46(); return
                end
                if bVar4 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(xStack_118)
                    __cleanup_LAB_00d4de46(); return
                end
                r2 = resources:ScriptThing(0)
                pCVar9 = r2
                r11 = quest:GetHealth(pCVar9)
                fVar3 = 0.0
                if fVar3 < fret_04 then
                    iVar15 = 0
                    iVar17 = 1
                    iVar14 = 0
                    iVar7 = 0
                    pcVar13 = "TEXT_QST_028_APPRENTICE_SKILL_RETURN"
                    pCVar8 = quest:GetHero()
                    r12 = me:Speak(pCVar8, pcVar13, iVar7, (iVar14 ~= 0), (iVar17 ~= 0), (iVar15 ~= 0))
                    iVar7 = me:IsPerformingScriptTask()
                    cVar5 = iVar7
                    while cVar5 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_84)
                            __cleanup_LAB_00d4de46(); return
                        end
                        iVar7 = me:IsPerformingScriptTask()
                        cVar5 = iVar7
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(xStack_118)
                        __cleanup_LAB_00d4de46(); return
                    end
                end
                ::LAB_00d4d2bb::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(xStack_64)
                if iVar6 ~= 1 then goto LAB_00d4dc36 end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d4de46(); return end
                quest:SetPlayerUsingRangedDummies(true)
                quest:SetMasterGameState("HeroTakingGuildTest", true)
                bVar4 = quest:MsgOnHeroFiredRangedWeapon()
                while not bVar4 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then __cleanup_LAB_00d4de46(); return end
                    bVar4 = quest:MsgOnHeroFiredRangedWeapon()
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if bVar4 then __cleanup_LAB_00d4de46(); return end
                -- TODO(native): CTimer::CTimer((CTimer *)&xStack_160);
                -- TODO(native): piVar2 = DAT_0143e8f8;
                iVar7 = __ftol2()
                quest:SetTimer(iVar6, xStack_160)
                quest:SetMasterGameState("SkillScore", 0)
                c_stk_14d = 0
                iVar6 = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
                xStack_13c = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                x_stk_12c = quest:AddQuestInfoTimer(iVar6, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                quest:UpdateQuestInfoCounter(iVar6, quest:GetMasterGameState("HighestSkillScore"), -1)
                c_stk_13d = 0
                iVar6 = quest:GetTimer(iVar6)
                while (0 < iVar6 and (c_stk_14d == 0)) do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d4de3d end
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d4de3d end
                        c_stk_14d = '\x01'
                    end
                    if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d4de3d end
                        quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                        c_stk_13d = '\x01'
                        quest:UpdateQuestInfoCounter(xStack_13c, quest:GetMasterGameState("HighestSkillScore"), -1)
                    end
                    quest:UpdateQuestInfoCounter(xStack_13c, quest:GetMasterGameState("SkillScore"), -1)
                    fVar18 = 6.0
                    pCVar8 = quest:GetThingWithScriptName("ArcheryRing")
                    pCVar9 = quest:GetHero()
                    bVar4 = quest:IsDistanceBetweenThingsOver(pCVar9, pCVar8, fVar18)
                    if bVar4 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d4de3d end
                        if __native_entity_state:GetStateBool("PlayerNotWarned") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d4de3d end
                            __native_entity_state:SetStateBool("PlayerNotWarned", false)
                            iVar7 = quest:AddNewConversation(me, false, false)
                            pCVar8 = quest:GetHero()
                            quest:AddPersonToConversation(iVar7, pCVar8)
                            pCVar8 = quest:GetHero()
                            quest:AddLineToConversation(iVar7, "TEXT_QST_028_APPRENTICE_SKILL_RING_OUT", me, pCVar8, xStack_e0)
                        end
                        c_stk_14d = '\x01'
                    end
                    iVar6 = quest:GetTimer(iVar6)
                end
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    bVar4 = quest:IsHeroControlledByPlayer()
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d4de3d end
                        bVar4 = quest:IsHeroControlledByPlayer()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d4de3d end
                    quest:DisplayQuestInfo(false)
                    quest:RemoveQuestInfoElement(iVar6)
                    quest:RemoveQuestInfoElement(xStack_13c)
                    quest:RemoveQuestInfoElement(x_stk_12c)
                    if c_stk_14d ~= 0 then
                        -- LAB_00d4dc16: (native jump target)
                        quest:SetMasterGameState("HeroTakingGuildTest", false)
                        quest:SetPlayerUsingRangedDummies(false)
                        goto LAB_00d4dc36
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if bVar4 then goto LAB_00d4de3d end
                    x_stk_12c = quest:GetMasterGameState("SkillScore")
                    -- TODO(native): pfVar12 = *(float **)(DAT_0143e90c + 0xec0);
                    iVar6 = 0
                    repeat
                        iVar7 = iVar6
                        if *pfVar12 < x_stk_12c ~= (*pfVar12 == x_stk_12c) then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then goto LAB_00d4de3d end
                            break
                        end
                        pfVar12 = pfVar12 + 1
                        iVar6 = iVar7 + 1
                    until not (iVar7 + 1 < 7)
                    xStack_128 = resources:StartMovie("")
                    bVar4 = false
                    if bVar4 ~= 0 then
                    end
                    iVar14 = 4
                    pCVar16 = xStack_128
                    pCVar8 = quest:GetHero()
                    bVar4 = resources:TryAcquire(pCVar16, pCVar8, iVar14)
                    while not bVar4 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then goto LAB_00d4de34 end
                        iVar14 = 4
                        pCVar8 = quest:GetHero()
                        bVar4 = resources:TryAcquire(pCVar16, pCVar8, iVar14)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar4 = not alive
                    if not bVar4 then
                        xStack_138 = resources:NewActorMap()
                        resources:SetActor(xStack_138, "ME", 0)
                        resources:SetActor(xStack_138, "HERO", xStack_118)
                        xStack_118 = resources:NewResource()
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro(xStack_d0, xStack_138, false, true)
                        if c_stk_13d == 0 then
                            -- LAB_00d4d979: (native jump target)
                            resources:SetActor(xStack_138, "ME", 0)
                            native_arg_switch_2 = iVar7
                            repeat
                                if native_arg_switch_2 == 0 then
                                    if quest:GetMasterGameState("GlobalSkillGrade") == 7 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then goto LAB_00d4de05 end
                                        resources:RunMacro(xStack_c8, xStack_138, false, true)
                                        break
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if not bVar4 then
                                        resources:RunMacro(xStack_c0, xStack_138, false, true)
                                        quest:ClearThingHasInformation(me)
                                        goto FLOW_native_label_1
                                    end
                                    goto LAB_00d4de05
                                else
                                    if native_arg_switch_2 == 1 then
                                        resources:RunMacro(xStack_98, xStack_138, false, true)
                                        break
                                    else
                                        if native_arg_switch_2 == 2 then
                                            resources:RunMacro(xStack_e8, xStack_138, false, true)
                                            break
                                        else
                                            if native_arg_switch_2 == 3 then
                                                resources:RunMacro(xStack_e4, xStack_138, false, true)
                                                break
                                            else
                                                if native_arg_switch_2 == 4 then
                                                    resources:RunMacro(xStack_104, xStack_138, false, true)
                                                    break
                                                else
                                                    if native_arg_switch_2 == 5 then
                                                        resources:RunMacro(xStack_100, xStack_138, false, true)
                                                        break
                                                    else
                                                        if native_arg_switch_2 == 6 then
                                                            resources:RunMacro(xStack_f8, xStack_138, false, true)
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
                            if quest:GetMasterGameState("GlobalSkillGrade") < 7 - iVar7 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    -- LAB_00d4de13: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00d4de1f
                                end
                                quest:SetMasterGameState("GlobalSkillGrade", 7 - iVar7)
                            end
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_64)
                            resources:DestroyActorMap(xStack_138)
                            resources:DestroyMovie(xStack_74)
                            quest:SetMasterGameState("HeroTakingGuildTest", false)
                            quest:SetPlayerUsingRangedDummies(false)
                            goto LAB_00d4dc36
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if not bVar4 then
                            resources:SetActor(xStack_138, "ME", 0)
                            resources:RunMacro(xStack_f0, xStack_138, false, true)
                            resources:SetActor(xStack_138, "ME", 0)
                            native_arg_switch_2 = iVar7
                            repeat
                                if native_arg_switch_2 == 0 then
                                    if quest:GetMasterGameState("GlobalSkillGrade") == 7 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar4 = not alive
                                        if bVar4 then goto LAB_00d4de05 end
                                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS_PRIZE", xStack_138, false, true)
                                        break
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if not bVar4 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_B", xStack_138, false, true)
                                        quest:ClearThingHasInformation(me)
                                        goto FLOW_native_label_1_c16
                                    end
                                    goto LAB_00d4de05
                                else
                                    if native_arg_switch_2 == 1 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_C", xStack_138, false, true)
                                        break
                                    else
                                        if native_arg_switch_2 == 2 then
                                            resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_D", xStack_138, false, true)
                                            break
                                        else
                                            if native_arg_switch_2 == 3 then
                                                resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_E", xStack_138, false, true)
                                                break
                                            else
                                                if native_arg_switch_2 == 4 then
                                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_F", xStack_138, false, true)
                                                    break
                                                else
                                                    if native_arg_switch_2 == 5 then
                                                        resources:RunMacro(xStack_100, xStack_138, false, true)
                                                        break
                                                    else
                                                        if native_arg_switch_2 == 6 then
                                                            resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_HIGH", xStack_138, false, true)
                                                            break
                                                        else
                                                            goto FLOW_native_label_1_c16
                                                        end
                                                    end
                                                end
                                            end
                                        end
                                    end
                                end
                            until not (false)
                            ::FLOW_native_label_1_c16::
                            if quest:GetMasterGameState("GlobalSkillGrade") < 7 - iVar7 then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar4 then
                                    -- LAB_00d4de13_c16: (native jump target)
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto LAB_00d4de1f
                                end
                                quest:SetMasterGameState("GlobalSkillGrade", 7 - iVar7)
                            end
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(xStack_64)
                            resources:DestroyActorMap(xStack_138)
                            resources:DestroyMovie(xStack_74)
                            quest:SetMasterGameState("HeroTakingGuildTest", false)
                            quest:SetPlayerUsingRangedDummies(false)
                            goto LAB_00d4dc36
                        end
                        ::LAB_00d4de05::
                        quest:PauseAllNonScriptedEntities(false)
                        ::LAB_00d4de1f::
                        resources:DestroyMovie(xStack_74)
                        resources:DestroyActorMap(xStack_138)
                    end
                    ::LAB_00d4de34::
                    resources:DestroyMovie(xStack_128)
                end
                ::LAB_00d4de3d::
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar4 = not alive
                if not bVar4 then
                    me:ClearCommands()
                    xStack_84 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    xStack_24 = resources:ScriptThing(0)
                    pCVar8 = xStack_24
                    r13 = quest:GetHealth(pCVar8)
                    fVar3 = 0.0
                    if fVar3 < fret_0 then
                        iVar17 = 0
                        iVar14 = 1
                        iVar7 = 0
                        iVar6 = 0
                        pcVar13 = "TEXT_QST_028_APPRENTICE_SKILL_EARLY"
                        pCVar8 = quest:GetHero()
                        r14 = me:Speak(pCVar8, pcVar13, iVar6, (iVar7 ~= 0), (iVar14 ~= 0), (iVar17 ~= 0))
                        iVar6 = me:IsPerformingScriptTask()
                        cVar5 = iVar6
                        while cVar5 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar4 = not alive
                            if bVar4 then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(xStack_84)
                                __cleanup_LAB_00d4de46(); return
                            end
                            iVar6 = me:IsPerformingScriptTask()
                            cVar5 = iVar6
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar4 = not alive
                        if bVar4 then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(xStack_118)
                            __cleanup_LAB_00d4de46(); return
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(0)
                    goto LAB_00d4dc36
                end
            end
            __cleanup_LAB_00d4de46()
            return
        end
        ::LAB_00d4dc36::
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar4 = not alive
    until false
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

