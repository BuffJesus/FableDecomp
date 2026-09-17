-- Readable native conversion: WillApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- WillApprentice.Main (retail 0x00d4efe0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue3, scratchValue4, scratchValue5, scratchValue6, scratchValue9, scratchValue13
    local scratchValue14, switch2, p0, scratchValue19, timerId, scratchValue20, scratchValue21
    local timerId2, scratchValue22, timerId3, timerId4, scratchValue23, scratchValue24
    local scratchValue25, scratchValue26, scratchValue27, scratchValue28
    local willHelpTimer = quest:GetStateInt("WillHelpTimer")
    local function __cleanup_LAB_00d50495()
        resources:ReleaseResource(scratchValue22)
    end
    local function __cleanup_LAB_00d504b9()
        quest:DeregisterTimer(timerId4)
        resources:DestroyMovie(scratchValue22)
    end
    local function __cleanup_LAB_00d505a6()
        quest:DeregisterTimer(timerId4)
        resources:DestroyMovie(scratchValue22)
    end
    scratchValue22 = resources:NewResource()
    scratchValue3 = resources:TryAcquire(scratchValue22, me, 4)
    while not scratchValue3 do
        if not quest:NewScriptFrame(me) then __cleanup_LAB_00d50495(); return end
        scratchValue3 = resources:TryAcquire(scratchValue22, me, 4)
    end
    if quest:IsActiveThreadTerminating() then resources:DestroyMovie(scratchValue22); return end
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntityAttachToVillage(me, quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE"))
    me:SetFriendsWithEverythingFlag(me)
    scratchValue6 = 0
    scratchValue19 = quest:GetThingWithScriptName("WillApprenticeTargetMarker")
    timerId4 = quest:RegisterTimer()
    quest:SetTimer(timerId4, 10)
    scratchValue3 = quest:IsActiveThreadTerminating()
    repeat
        if scratchValue3 then
            quest:DeregisterTimer(timerId4)
            __cleanup_LAB_00d50495()
            return
        end
        if not quest:IsQuestActive("Q_GuildTrainingDeparture") then goto LAB_00d4f285 end
        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d504b9(); return end
        if ((3 < quest:GetMasterGameState("GlobalMeleeGrade")) or (3 < quest:GetMasterGameState("GlobalSkillGrade"))) or 3 < quest:GetMasterGameState("GlobalWillGrade") then
            if not quest:IsActiveThreadTerminating() then
                if scratchValue6 ~= 0 then
                    quest:ClearThingHasInformation(me)
                    scratchValue6 = 0
                end
                goto LAB_00d4f285
            end
            __cleanup_LAB_00d504b9(); return
        end
        if scratchValue6 == 0 then
            quest:SetThingHasInformation(me, false, true, false)
            scratchValue6 = 1
        end
        ::LAB_00d4f285::
        if not quest:IsDistanceBetweenThingsOver(me, scratchValue19, 4.0) or me:IsPerformingScriptTask() then
            if me:IsPerformingScriptTask() then goto LAB_00d4f49e end
            if not quest:IsDistanceBetweenThingsUnder(me, quest:GetHero(), 10.0) or 0 < quest:GetTimer(timerId4) then goto LAB_00d4f49e end
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                quest:SetTimer(timerId4, 20)
                scratchValue9 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue9, quest:GetHero())
                scratchValue13 = quest:GetMasterGameState("GlobalWillGrade")
                if scratchValue13 == 0 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue9, "TEXT_QST_028_APPRENTICE_WILL_EARLY_COMMENT", me, quest:GetHero(), false); goto LAB_00d4f49e end
                elseif scratchValue13 == 7 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue9, "TEXT_QST_028_APPRENTICE_WILL_APLUS_COMMENT", me, quest:GetHero(), false); goto LAB_00d4f49e end
                elseif not quest:IsActiveThreadTerminating() then
                    quest:AddLineToConversation(scratchValue9, "TEXT_QST_028_APPRENTICE_WILL_NOT_APLUS_COMMENT", me, quest:GetHero(), false)
                    goto LAB_00d4f49e
                end
            end
            __cleanup_LAB_00d505a6(); return
        end
        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d504b9(); return end
        if scratchValue19 ~= nil and not scratchValue19:IsNull() then
            p0 = scratchValue19:GetPos()
        end
        me:MoveToPosition(p0, 3.0, 0, false, true)
        ::LAB_00d4f49e::
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d504b9(); return end
            if not me:IsPerformingScriptTask() then
                if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue24 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue22)) then
                            me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_WILL_MULTI_GRADE", 0, false, true, false)
                            scratchValue4 = me:IsPerformingScriptTask()
                            while scratchValue4 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue24)
                                    __cleanup_LAB_00d505a6(); return
                                end
                                scratchValue4 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue24)
                                __cleanup_LAB_00d505a6(); return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue24)
                        goto LAB_00d503cb
                    end
                    __cleanup_LAB_00d505a6(); return
                end
                scratchValue20 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_WILL_HELLO", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                while scratchValue13 < 0 do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue20)
                        __cleanup_LAB_00d505a6(); return
                    end
                    scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:ReleaseResource(scratchValue20)
                    __cleanup_LAB_00d505a6(); return
                end
                scratchValue3 = quest:IsActiveThreadTerminating()
                if scratchValue13 ~= 1 then
                    if not scratchValue3 then
                        if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue22)) then
                            me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_WILL_RETURN", 0, false, true, false)
                            scratchValue4 = me:IsPerformingScriptTask()
                            while scratchValue4 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:ReleaseResource(scratchValue20)
                                    __cleanup_LAB_00d505a6(); return
                                end
                                scratchValue4 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d50542 end
                        end
                        goto LAB_00d4fb0a
                    end
                    ::LAB_00d50542::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue20)
                    __cleanup_LAB_00d505a6(); return
                end
                if scratchValue3 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue20)
                    __cleanup_LAB_00d505a6(); return
                end
                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue22)) then
                    me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_WILL_INTRO", 0, false, true, false)
                    scratchValue4 = me:IsPerformingScriptTask()
                    while scratchValue4 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(scratchValue20)
                            __cleanup_LAB_00d505a6(); return
                        end
                        scratchValue4 = me:IsPerformingScriptTask()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue20)
                        __cleanup_LAB_00d505a6(); return
                    end
                end
                if quest:GetMasterGameState("GlobalWillGrade") ~= 7 then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(scratchValue20)
                        __cleanup_LAB_00d505a6(); return
                    end
                    if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue22)) then
                        me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_WILL_INTRO_APLUS", 0, false, true, false)
                        scratchValue4 = me:IsPerformingScriptTask()
                        while scratchValue4 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue20)
                                __cleanup_LAB_00d505a6(); return
                            end
                            scratchValue4 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(scratchValue20)
                            __cleanup_LAB_00d505a6(); return
                        end
                    end
                end
                ::LAB_00d4fb0a::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue20)
                if scratchValue13 ~= 1 then goto LAB_00d503cb end
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d505a6(); return end
                quest:SetPlayerUsingWillDummies(true)
                quest:SetMasterGameState("HeroTakingGuildTest", true)
                scratchValue3 = quest:MsgOnHeroCastSpell()
                while not scratchValue3 do
                    if not quest:NewScriptFrame(me) then __cleanup_LAB_00d505a6(); return end
                    scratchValue3 = quest:MsgOnHeroCastSpell()
                end
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d505a6(); return end
                timerId2 = quest:RegisterTimer()
                quest:SetTimer(timerId2, math.modf(quest:ReadGlobalGameDataFloat(3848)))
                quest:SetMasterGameState("WillScore", 0)
                quest:SetTimer(willHelpTimer, 0)
                scratchValue13 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                scratchValue26 = scratchValue13
                scratchValue28 = quest:AddQuestInfoTimer(timerId2, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                scratchValue4 = 0
                scratchValue5 = 0
                timerId3 = quest:RegisterTimer()
                timerId = timerId3
                quest:SetTimer(timerId3, 0)
                scratchValue9 = quest:GetTimer(timerId2)
                while 0 < scratchValue9 and scratchValue4 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d50594 end
                    if quest:GetTimer(timerId) < 1 then
                        quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                        quest:SetTimer(timerId, 2)
                    end
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d50594 end
                        scratchValue4 = 1
                        scratchValue5 = 1
                    end
                    if quest:GetHeroWillEnergy() == 0 and quest:GetTimer(willHelpTimer) < 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d50594 end
                        scratchValue9 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue9, quest:GetHero())
                        quest:AddLineToConversation(scratchValue9, "TEXT_QST_028_APPRENTICE_WILL_NO_WILL", me, quest:GetHero(), false)
                        quest:SetTimer(willHelpTimer, 8)
                        timerId = timerId3
                        scratchValue4 = scratchValue5
                    end
                    if quest:IsDistanceBetweenThingsOver(quest:GetHero(), me, 30.0) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d50594 end
                        scratchValue4 = 1
                        scratchValue5 = 1
                    end
                    scratchValue13 = scratchValue26
                    quest:UpdateQuestInfoCounter(scratchValue26, quest:GetMasterGameState("WillScore"), -1)
                    scratchValue9 = quest:GetTimer(timerId2)
                end
                if not quest:IsActiveThreadTerminating() then
                    scratchValue3 = quest:IsHeroControlledByPlayer()
                    while not scratchValue3 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d50594 end
                        scratchValue3 = quest:IsHeroControlledByPlayer()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d50594 end
                    quest:DisplayQuestInfo(false)
                    quest:RemoveQuestInfoElement(scratchValue13)
                    quest:RemoveQuestInfoElement(scratchValue28)
                    if scratchValue4 == 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d50594 end
                        scratchValue28 = quest:GetMasterGameState("WillScore")
                        scratchValue14 = 0
                        scratchValue13 = 0
                        repeat
                            scratchValue9 = scratchValue13
                            if quest:ReadGlobalGameDataFloatAt(3788, scratchValue14) <= scratchValue28 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d50594 end
                                break
                            end
                            scratchValue14 = scratchValue14 + 1
                            scratchValue13 = scratchValue9 + 1
                        until not (scratchValue9 + 1 < 7)
                        scratchValue27 = resources:StartMovie("")
                        scratchValue3 = resources:TryAcquire(scratchValue27, quest:GetHero(), 4)
                        while not scratchValue3 do
                            if not quest:NewScriptFrame(me) then resources:DestroyMovie(scratchValue27); goto LAB_00d50594 end
                            scratchValue3 = resources:TryAcquire(scratchValue27, quest:GetHero(), 4)
                        end
                        if quest:IsActiveThreadTerminating() then resources:DestroyMovie(scratchValue27); goto LAB_00d50594 end
                        scratchValue21 = resources:NewActorMap()
                        resources:SetActor(scratchValue21, "ME", scratchValue22)
                        resources:SetActor(scratchValue21, "HERO", scratchValue27)
                        scratchValue25 = resources:NewResource()
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_END", scratchValue21, false, true)
                        resources:SetActor(scratchValue21, "ME", scratchValue22)
                        switch2 = scratchValue9
                        repeat
                            if switch2 == 0 then
                                if quest:GetMasterGameState("GlobalWillGrade") ~= 7 then
                                    if not quest:IsActiveThreadTerminating() then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_APLUS_PRIZE", scratchValue21, false, true)
                                        quest:ClearThingHasInformation(me)
                                        goto FLOW_native_label_1
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue25)
                                    resources:DestroyActorMap(scratchValue21)
                                    resources:DestroyMovie(scratchValue27)
                                    goto LAB_00d50594
                                end
                                if not quest:IsActiveThreadTerminating() then resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_APLUS", scratchValue21, false, true); break end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue25)
                                resources:DestroyActorMap(scratchValue21)
                                resources:DestroyMovie(scratchValue27)
                                goto LAB_00d50594
                            elseif switch2 == 1 then
                                resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_A", scratchValue21, false, true)
                                break
                            elseif switch2 == 2 then
                                resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_B", scratchValue21, false, true)
                                break
                            elseif switch2 == 3 then
                                resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_C", scratchValue21, false, true)
                                break
                            elseif switch2 == 4 then
                                resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_D", scratchValue21, false, true)
                                break
                            elseif switch2 == 5 then
                                resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_E", scratchValue21, false, true)
                                break
                            elseif switch2 == 6 then
                                resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_F", scratchValue21, false, true)
                                break
                            else
                                goto FLOW_native_label_1
                            end
                        until true
                        ::FLOW_native_label_1::
                        if quest:GetMasterGameState("GlobalWillGrade") < 7 - scratchValue9 then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue25)
                                resources:DestroyActorMap(scratchValue21)
                                resources:DestroyMovie(scratchValue27)
                                goto LAB_00d50594
                            end
                            quest:SetMasterGameState("GlobalWillGrade", 7 - scratchValue9)
                        end
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue25)
                        resources:DestroyActorMap(scratchValue21)
                        resources:DestroyMovie(scratchValue27)
                    end
                    quest:SetMasterGameState("HeroTakingGuildTest", false)
                    quest:SetPlayerUsingWillDummies(false)
                    quest:DeregisterTimer(timerId3)
                    quest:DeregisterTimer(timerId2)
                    goto LAB_00d503cb
                end
                ::LAB_00d50594::
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId2)
            elseif not quest:IsActiveThreadTerminating() then
                me:ClearCommands()
                scratchValue23 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue22)) then
                    me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_WILL_EARLY", 0, false, true, false)
                    scratchValue4 = me:IsPerformingScriptTask()
                    while scratchValue4 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue23)
                            __cleanup_LAB_00d505a6(); return
                        end
                        scratchValue4 = me:IsPerformingScriptTask()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(scratchValue23)
                        __cleanup_LAB_00d505a6(); return
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:ReleaseResource(scratchValue23)
                goto LAB_00d503cb
            end
            __cleanup_LAB_00d505a6()
            return
        end
        ::LAB_00d503cb::
        quest:NewScriptFrame(me)
        scratchValue3 = quest:IsActiveThreadTerminating()
    until false
end

-- WillApprentice.Init (retail 0x00d43330)
function Init(quest, me)
end

-- WillApprentice.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- WillApprentice.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

