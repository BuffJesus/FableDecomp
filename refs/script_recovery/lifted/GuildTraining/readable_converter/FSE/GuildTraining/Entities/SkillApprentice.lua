-- Readable native conversion: SkillApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- SkillApprentice.Main (retail 0x00d4c720)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue2, scratchValue3, scratchValue4, scratchValue5, scratchValue6, scratchValue12
    local scratchValue13, scratchValue14, sequence1, switch2, p0, scratchValue20, scratchValue21
    local scratchValue22, scratchValue23, scratchValue24, timerId, scratchValue25, scratchValue26
    local scratchValue27, scratchValue28, scratchValue29, scratchValue30, scratchValue31
    local function __cleanup_LAB_00d4dcdb()
        resources:ReleaseResource(scratchValue25)
    end
    local function __cleanup_LAB_00d4dd41()
        quest:DeregisterTimer(scratchValue12)
        resources:DestroyMovie(scratchValue25)
    end
    local function __cleanup_LAB_00d4de46()
        quest:DeregisterTimer(scratchValue26)
        resources:DestroyMovie(scratchValue25)
    end
    scratchValue25 = resources:NewResource()
    scratchValue2 = resources:TryAcquire(scratchValue25, me, 4)
    while not scratchValue2 do
        if not quest:NewScriptFrame(me) then __cleanup_LAB_00d4dcdb(); return end
        scratchValue2 = resources:TryAcquire(scratchValue25, me, 4)
    end
    if quest:IsActiveThreadTerminating() then resources:DestroyMovie(scratchValue25); return end
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntityAttachToVillage(me, quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE"))
    me:SetFriendsWithEverythingFlag(me)
    state:SetBool("PlayerNotWarned", true)
    scratchValue6 = 0
    scratchValue12 = quest:RegisterTimer()
    scratchValue26 = scratchValue12
    quest:SetTimer(scratchValue12, 10)
    scratchValue20 = quest:GetThingWithScriptName("SkillApprenticeTargetMarker")
    scratchValue2 = quest:IsActiveThreadTerminating()
    repeat
        if scratchValue2 then
            quest:DeregisterTimer(scratchValue12)
            __cleanup_LAB_00d4dcdb()
            return
        end
        if not quest:IsQuestActive("Q_GuildTrainingDeparture") then goto LAB_00d4c9a2 end
        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4dd41(); return end
        if ((3 < quest:GetMasterGameState("GlobalMeleeGrade")) or (3 < quest:GetMasterGameState("GlobalSkillGrade"))) or 3 < quest:GetMasterGameState("GlobalWillGrade") then
            if not quest:IsActiveThreadTerminating() then
                if scratchValue6 ~= 0 then
                    quest:ClearThingHasInformation(me)
                    scratchValue6 = 0
                end
                goto LAB_00d4c9a2
            end
            quest:DeregisterTimer(scratchValue12)
            resources:DestroyMovie(scratchValue25)
            return
        end
        if scratchValue6 == 0 then
            quest:SetThingHasInformation(me, false, true, false)
            scratchValue6 = 1
        end
        ::LAB_00d4c9a2::
        if not quest:IsDistanceBetweenThingsOver(me, scratchValue20, 4.0) or me:IsPerformingScriptTask() then
            scratchValue12 = me:IsPerformingScriptTask()
            if scratchValue12 then goto LAB_00d4cbac end
            sequence1 = not quest:IsDistanceBetweenThingsUnder(me, quest:GetHero(), 10.0)
            if not sequence1 then
                scratchValue12 = quest:GetTimer(scratchValue26)
                sequence1 = 0 < scratchValue12
            end
            if sequence1 then goto LAB_00d4cbac end
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                quest:SetTimer(scratchValue26, 20)
                scratchValue13 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue13, quest:GetHero())
                scratchValue12 = quest:GetMasterGameState("GlobalSkillGrade")
                if scratchValue12 == 0 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_APPRENTICE_SKILL_EARLY_COMMENT", me, quest:GetHero(), false); goto LAB_00d4cbac end
                elseif scratchValue12 == 7 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_APPRENTICE_SKILL_APLUS_COMMENT", me, quest:GetHero(), false); goto LAB_00d4cbac end
                elseif not quest:IsActiveThreadTerminating() then
                    quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_APPRENTICE_SKILL_NOT_APLUS_COMMENT", me, quest:GetHero(), false)
                    goto LAB_00d4cbac
                end
            end
            __cleanup_LAB_00d4de46(); return
        end
        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4dd41(); return end
        if scratchValue20 ~= nil and not scratchValue20:IsNull() then
            p0 = scratchValue20:GetPos()
        end
        me:MoveToPosition(p0, 3.0, 0, false, true)
        ::LAB_00d4cbac::
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue26)
                resources:DestroyMovie(scratchValue25)
                return
            end
            scratchValue12 = me:IsPerformingScriptTask()
            if not scratchValue12 then
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
                if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue28 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue25)) then
                            scratchValue12 = 0
                            me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_SKILL_TEST_ALREADY", scratchValue12, false, true, false)
                            scratchValue12 = me:IsPerformingScriptTask()
                            scratchValue3 = scratchValue12
                            while scratchValue3 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue28)
                                    __cleanup_LAB_00d4de46(); return
                                end
                                scratchValue12 = me:IsPerformingScriptTask()
                                scratchValue3 = scratchValue12
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue28)
                                __cleanup_LAB_00d4de46(); return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue28)
                        goto LAB_00d4dc36
                    end
                    __cleanup_LAB_00d4de46(); return
                end
                scratchValue22 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue25)) then
                    scratchValue12 = 0
                    me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_SKILL_HELLO", scratchValue12, false, true, false)
                    scratchValue12 = me:IsPerformingScriptTask()
                    scratchValue3 = scratchValue12
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(scratchValue22)
                            __cleanup_LAB_00d4de46(); return
                        end
                        scratchValue12 = me:IsPerformingScriptTask()
                        scratchValue3 = scratchValue12
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue22)
                        __cleanup_LAB_00d4de46(); return
                    end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_SKILL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                scratchValue12 = quest:MsgIsQuestionAnsweredYesOrNo()
                while scratchValue12 < 0 do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(scratchValue22)
                        __cleanup_LAB_00d4de46(); return
                    end
                    scratchValue12 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue22)
                    __cleanup_LAB_00d4de46(); return
                end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue12 == 1 then
                    if not scratchValue2 then
                        if quest:GetMasterGameState("GlobalSkillGrade") ~= 7 then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue22)
                                __cleanup_LAB_00d4de46(); return
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue25)) then
                                me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_SKILL_TIME_LIMIT_APLUS", 0, false, true, false)
                                scratchValue3 = me:IsPerformingScriptTask()
                                while scratchValue3 do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d4dded end
                                    scratchValue3 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue22)
                                    __cleanup_LAB_00d4de46(); return
                                end
                            end
                        end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue25)) then
                            me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_SKILL_TIME_LIMIT", 0, false, true, false)
                            scratchValue3 = me:IsPerformingScriptTask()
                            while scratchValue3 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d4dded end
                                scratchValue3 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue22)
                                __cleanup_LAB_00d4de46(); return
                            end
                        end
                        goto LAB_00d4d2bb
                    end
                    ::LAB_00d4dded::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(scratchValue22)
                    __cleanup_LAB_00d4de46(); return
                end
                if scratchValue2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(scratchValue22)
                    __cleanup_LAB_00d4de46(); return
                end
                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue25)) then
                    me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_SKILL_RETURN", 0, false, true, false)
                    scratchValue3 = me:IsPerformingScriptTask()
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue22)
                            __cleanup_LAB_00d4de46(); return
                        end
                        scratchValue3 = me:IsPerformingScriptTask()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(scratchValue22)
                        __cleanup_LAB_00d4de46(); return
                    end
                end
                ::LAB_00d4d2bb::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue22)
                if scratchValue12 ~= 1 then goto LAB_00d4dc36 end
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
                quest:SetPlayerUsingRangedDummies(true)
                quest:SetMasterGameState("HeroTakingGuildTest", true)
                scratchValue2 = quest:MsgOnHeroFiredRangedWeapon()
                while not scratchValue2 do
                    if not quest:NewScriptFrame(me) then __cleanup_LAB_00d4de46(); return end
                    scratchValue2 = quest:MsgOnHeroFiredRangedWeapon()
                end
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
                timerId = quest:RegisterTimer()
                quest:SetTimer(timerId, math.modf(quest:ReadGlobalGameDataFloat(3844)))
                quest:SetMasterGameState("SkillScore", 0)
                scratchValue5 = 0
                scratchValue12 = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
                scratchValue30 = scratchValue12
                scratchValue24 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                scratchValue31 = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                quest:UpdateQuestInfoCounter(scratchValue12, quest:GetMasterGameState("HighestSkillScore"), -1)
                scratchValue4 = 0
                scratchValue12 = quest:GetTimer(timerId)
                while 0 < scratchValue12 and scratchValue5 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d4de3d end
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        scratchValue5 = 1
                    end
                    if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                        quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                        scratchValue4 = 1
                        quest:UpdateQuestInfoCounter(scratchValue24, quest:GetMasterGameState("HighestSkillScore"), -1)
                    end
                    quest:UpdateQuestInfoCounter(scratchValue24, quest:GetMasterGameState("SkillScore"), -1)
                    if quest:IsDistanceBetweenThingsOver(quest:GetHero(), quest:GetThingWithScriptName("ArcheryRing"), 6.0) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                        if state:GetBool("PlayerNotWarned") then
                            state:SetBool("PlayerNotWarned", false)
                            scratchValue13 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue13, quest:GetHero())
                            quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_APPRENTICE_SKILL_RING_OUT", me, quest:GetHero(), false)
                        end
                        scratchValue5 = 1
                    end
                    scratchValue12 = quest:GetTimer(timerId)
                end
                if not quest:IsActiveThreadTerminating() then
                    scratchValue2 = quest:IsHeroControlledByPlayer()
                    while not scratchValue2 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d4de3d end
                        scratchValue2 = quest:IsHeroControlledByPlayer()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                    quest:DisplayQuestInfo(false)
                    quest:RemoveQuestInfoElement(scratchValue30)
                    quest:RemoveQuestInfoElement(scratchValue24)
                    quest:RemoveQuestInfoElement(scratchValue31)
                    if scratchValue5 ~= 0 then
                        quest:SetMasterGameState("HeroTakingGuildTest", false)
                        quest:SetPlayerUsingRangedDummies(false)
                        quest:DeregisterTimer(timerId)
                        goto LAB_00d4dc36
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                    scratchValue31 = quest:GetMasterGameState("SkillScore")
                    scratchValue14 = 0
                    scratchValue12 = 0
                    repeat
                        scratchValue13 = scratchValue12
                        if quest:ReadGlobalGameDataFloatAt(3776, scratchValue14) <= scratchValue31 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                            break
                        end
                        scratchValue14 = scratchValue14 + 1
                        scratchValue12 = scratchValue13 + 1
                    until not (scratchValue13 + 1 < 7)
                    scratchValue21 = resources:StartMovie("")
                    scratchValue2 = resources:TryAcquire(scratchValue21, quest:GetHero(), 4)
                    while not scratchValue2 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d4de34 end
                        scratchValue2 = resources:TryAcquire(scratchValue21, quest:GetHero(), 4)
                    end
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue23 = resources:NewActorMap()
                        resources:SetActor(scratchValue23, "ME", scratchValue25)
                        resources:SetActor(scratchValue23, "HERO", scratchValue21)
                        scratchValue29 = resources:NewResource()
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_END", scratchValue23, false, true)
                        if scratchValue4 == 0 then
                            resources:SetActor(scratchValue23, "ME", scratchValue25)
                            switch2 = scratchValue13
                            repeat
                                if switch2 == 0 then
                                    if quest:GetMasterGameState("GlobalSkillGrade") == 7 then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de05 end
                                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS", scratchValue23, false, true)
                                        break
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS_PRIZE", scratchValue23, false, true)
                                        quest:ClearThingHasInformation(me)
                                        goto FLOW_native_label_1
                                    end
                                    goto LAB_00d4de05
                                elseif switch2 == 1 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_A", scratchValue23, false, true)
                                    break
                                elseif switch2 == 2 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_B", scratchValue23, false, true)
                                    break
                                elseif switch2 == 3 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_C", scratchValue23, false, true)
                                    break
                                elseif switch2 == 4 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_D", scratchValue23, false, true)
                                    break
                                elseif switch2 == 5 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_E", scratchValue23, false, true)
                                    break
                                elseif switch2 == 6 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_F", scratchValue23, false, true)
                                    break
                                else
                                    goto FLOW_native_label_1
                                end
                            until true
                            ::FLOW_native_label_1::
                            if quest:GetMasterGameState("GlobalSkillGrade") < 7 - scratchValue13 then
                                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d4de1f end
                                quest:SetMasterGameState("GlobalSkillGrade", 7 - scratchValue13)
                            end
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue29)
                            resources:DestroyActorMap(scratchValue23)
                            resources:DestroyMovie(scratchValue21)
                            quest:SetMasterGameState("HeroTakingGuildTest", false)
                            quest:SetPlayerUsingRangedDummies(false)
                            quest:DeregisterTimer(timerId)
                            goto LAB_00d4dc36
                        end
                        if not quest:IsActiveThreadTerminating() then
                            resources:SetActor(scratchValue23, "ME", scratchValue25)
                            resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_HIGH", scratchValue23, false, true)
                            resources:SetActor(scratchValue23, "ME", scratchValue25)
                            switch2 = scratchValue13
                            repeat
                                if switch2 == 0 then
                                    if quest:GetMasterGameState("GlobalSkillGrade") == 7 then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de05 end
                                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS", scratchValue23, false, true)
                                        break
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS_PRIZE", scratchValue23, false, true)
                                        quest:ClearThingHasInformation(me)
                                        goto FLOW_native_label_1_c16
                                    end
                                    goto LAB_00d4de05
                                elseif switch2 == 1 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_A", scratchValue23, false, true)
                                    break
                                elseif switch2 == 2 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_B", scratchValue23, false, true)
                                    break
                                elseif switch2 == 3 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_C", scratchValue23, false, true)
                                    break
                                elseif switch2 == 4 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_D", scratchValue23, false, true)
                                    break
                                elseif switch2 == 5 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_E", scratchValue23, false, true)
                                    break
                                elseif switch2 == 6 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_F", scratchValue23, false, true)
                                    break
                                else
                                    goto FLOW_native_label_1_c16
                                end
                            until true
                            ::FLOW_native_label_1_c16::
                            if quest:GetMasterGameState("GlobalSkillGrade") < 7 - scratchValue13 then
                                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d4de1f end
                                quest:SetMasterGameState("GlobalSkillGrade", 7 - scratchValue13)
                            end
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue29)
                            resources:DestroyActorMap(scratchValue23)
                            resources:DestroyMovie(scratchValue21)
                            quest:SetMasterGameState("HeroTakingGuildTest", false)
                            quest:SetPlayerUsingRangedDummies(false)
                            quest:DeregisterTimer(timerId)
                            goto LAB_00d4dc36
                        end
                        ::LAB_00d4de05::
                        quest:PauseAllNonScriptedEntities(false)
                        ::LAB_00d4de1f::
                        resources:DestroyMovie(scratchValue29)
                        resources:DestroyActorMap(scratchValue23)
                    end
                    ::LAB_00d4de34::
                    resources:DestroyMovie(scratchValue21)
                end
                ::LAB_00d4de3d::
                quest:DeregisterTimer(timerId)
            elseif not quest:IsActiveThreadTerminating() then
                me:ClearCommands()
                scratchValue27 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue25)) then
                    scratchValue12 = 0
                    me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_SKILL_EARLY", scratchValue12, false, true, false)
                    scratchValue12 = me:IsPerformingScriptTask()
                    scratchValue3 = scratchValue12
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue27)
                            __cleanup_LAB_00d4de46(); return
                        end
                        scratchValue12 = me:IsPerformingScriptTask()
                        scratchValue3 = scratchValue12
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(scratchValue27)
                        __cleanup_LAB_00d4de46(); return
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:ReleaseResource(scratchValue27)
                goto LAB_00d4dc36
            end
            __cleanup_LAB_00d4de46()
            return
        end
        ::LAB_00d4dc36::
        quest:NewScriptFrame(me)
        scratchValue2 = quest:IsActiveThreadTerminating()
    until false
end

-- SkillApprentice.Init (retail 0x00d41bc0)
function Init(quest, me)
end

-- SkillApprentice.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- SkillApprentice.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

