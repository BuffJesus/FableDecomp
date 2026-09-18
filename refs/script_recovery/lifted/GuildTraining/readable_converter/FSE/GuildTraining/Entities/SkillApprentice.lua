-- Readable native conversion: SkillApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_SkillGrades = 3776,  -- '07000000000016430000f0420000a042000048420000c8410000204100000000'
    GUI_SkillTimer = 3844,  -- 60.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local playerNotWarned

-- SkillApprentice.Main (retail 0x00d4c720)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue2, scratchValue4, scratchValue5, scratchValue6, scratchValue, scratchValue13
    local scratchValue14, switch, p0, skillApprenticeTargetMarker, resource, movie, actorMap
    local infoCounter, timerId, resource3, scratchValue17, movie2, movie3, movie4, scratchValue18
    local scratchValue19
    local function __cleanup_LAB_00d4dd41()
        quest:DeregisterTimer(scratchValue17)
        resources:ReleaseResource(resource3)
    end
    local function __cleanup_LAB_00d4de46()
        quest:DeregisterTimer(scratchValue17)
        resources:ReleaseResource(resource3)
    end
    resource3 = resources:NewResource()
    while not resources:TryAcquire(resource3, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource3); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); return end
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntityAttachToVillage(me, quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE"))
    me:SetFriendsWithEverythingFlag(me)
    playerNotWarned = true
    scratchValue6 = 0
    scratchValue17 = quest:RegisterTimer()
    quest:SetTimer(scratchValue17, 10)
    skillApprenticeTargetMarker = quest:GetThingWithScriptName("SkillApprenticeTargetMarker")
    scratchValue2 = quest:IsActiveThreadTerminating()
    repeat
        if scratchValue2 then
            quest:DeregisterTimer(scratchValue17)
            resources:ReleaseResource(resource3)
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
            quest:DeregisterTimer(scratchValue17)
            resources:ReleaseResource(resource3)
            return
        end
        if scratchValue6 == 0 then
            quest:SetThingHasInformation(me, false, true, false)
            scratchValue6 = 1
        end
        ::LAB_00d4c9a2::
        if not quest:IsDistanceBetweenThingsOver(me, skillApprenticeTargetMarker, 4.0) or me:IsPerformingScriptTask() then
            if me:IsPerformingScriptTask() then goto LAB_00d4cbac end
            if not quest:IsDistanceBetweenThingsUnder(me, hero, 10.0) or 0 < quest:GetTimer(scratchValue17) then goto LAB_00d4cbac end
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:SetTimer(scratchValue17, 20)
                scratchValue13 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue13, hero)
                scratchValue = quest:GetMasterGameState("GlobalSkillGrade")
                if scratchValue == 0 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_APPRENTICE_SKILL_EARLY_COMMENT", me, hero, false); goto LAB_00d4cbac end
                elseif scratchValue == 7 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_APPRENTICE_SKILL_APLUS_COMMENT", me, hero, false); goto LAB_00d4cbac end
                elseif not quest:IsActiveThreadTerminating() then
                    quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_APPRENTICE_SKILL_NOT_APLUS_COMMENT", me, hero, false)
                    goto LAB_00d4cbac
                end
            end
            __cleanup_LAB_00d4de46(); return
        end
        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4dd41(); return end
        if skillApprenticeTargetMarker ~= nil and not skillApprenticeTargetMarker:IsNull() then
            p0 = skillApprenticeTargetMarker:GetPos()
        end
        me:MoveToPosition(p0, 3.0, 0, false, true)
        ::LAB_00d4cbac::
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue17)
                resources:ReleaseResource(resource3)
                return
            end
            if not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
                if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                    if not quest:IsActiveThreadTerminating() then
                        movie3 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                            if not me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_TEST_ALREADY", GROUP_SELECT_FIRST, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                __cleanup_LAB_00d4de46(); return
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                __cleanup_LAB_00d4de46(); return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        goto LAB_00d4dc36
                    end
                    __cleanup_LAB_00d4de46(); return
                end
                movie = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                    if not me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_HELLO", GROUP_SELECT_FIRST, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        __cleanup_LAB_00d4de46(); return
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        __cleanup_LAB_00d4de46(); return
                    end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_SKILL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                while scratchValue < 0 do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        __cleanup_LAB_00d4de46(); return
                    end
                    scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    __cleanup_LAB_00d4de46(); return
                end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue == 1 then
                    if not scratchValue2 then
                        if quest:GetMasterGameState("GlobalSkillGrade") ~= 7 then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                __cleanup_LAB_00d4de46(); return
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                                if not me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_TIME_LIMIT_APLUS", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d4dded end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie)
                                    __cleanup_LAB_00d4de46(); return
                                end
                            end
                        end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                            if not me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_TIME_LIMIT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d4dded end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                __cleanup_LAB_00d4de46(); return
                            end
                        end
                        goto LAB_00d4d2bb
                    end
                    ::LAB_00d4dded::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    __cleanup_LAB_00d4de46(); return
                end
                if scratchValue2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    __cleanup_LAB_00d4de46(); return
                end
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                    if not me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_RETURN", GROUP_SELECT_FIRST, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        __cleanup_LAB_00d4de46(); return
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        __cleanup_LAB_00d4de46(); return
                    end
                end
                ::LAB_00d4d2bb::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                if scratchValue ~= 1 then goto LAB_00d4dc36 end
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
                quest:SetPlayerUsingRangedDummies(true)
                quest:SetMasterGameState("HeroTakingGuildTest", true)
                while not quest:MsgOnHeroFiredRangedWeapon() do
                    if not quest:NewScriptFrame(me) then __cleanup_LAB_00d4de46(); return end
                end
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
                timerId = quest:RegisterTimer()
                quest:SetTimer(timerId, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_SkillTimer))))
                quest:SetMasterGameState("SkillScore", 0)
                scratchValue5 = 0
                scratchValue = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
                scratchValue18 = scratchValue
                infoCounter = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                scratchValue19 = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                quest:UpdateQuestInfoCounter(scratchValue, quest:GetMasterGameState("HighestSkillScore"), -1)
                scratchValue4 = 0
                while 0 < quest:GetTimer(timerId) and scratchValue5 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d4de3d end
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        scratchValue5 = 1
                    end
                    if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                        quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                        scratchValue4 = 1
                        quest:UpdateQuestInfoCounter(infoCounter, quest:GetMasterGameState("HighestSkillScore"), -1)
                    end
                    quest:UpdateQuestInfoCounter(infoCounter, quest:GetMasterGameState("SkillScore"), -1)
                    if quest:IsDistanceBetweenThingsOver(hero, quest:GetThingWithScriptName("ArcheryRing"), 6.0) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                        if playerNotWarned then
                            playerNotWarned = false
                            scratchValue13 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue13, hero)
                            quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_APPRENTICE_SKILL_RING_OUT", me, hero, false)
                        end
                        scratchValue5 = 1
                    end
                end
                if not quest:IsActiveThreadTerminating() then
                    while not quest:IsHeroControlledByPlayer() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d4de3d end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                    quest:DisplayQuestInfo(false)
                    quest:RemoveQuestInfoElement(scratchValue18)
                    quest:RemoveQuestInfoElement(infoCounter)
                    quest:RemoveQuestInfoElement(scratchValue19)
                    if scratchValue5 ~= 0 then
                        quest:SetMasterGameState("HeroTakingGuildTest", false)
                        quest:SetPlayerUsingRangedDummies(false)
                        quest:DeregisterTimer(timerId)
                        goto LAB_00d4dc36
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                    scratchValue19 = quest:GetMasterGameState("SkillScore")
                    scratchValue14 = 0
                    scratchValue = 0
                    repeat
                        scratchValue13 = scratchValue
                        if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_SkillGrades, scratchValue14) <= scratchValue19 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                            break
                        end
                        scratchValue14 = scratchValue14 + 1
                        scratchValue = scratchValue13 + 1
                    until not (scratchValue13 + 1 < 7)
                    resource = resources:NewResource()
                    while not resources:TryAcquire(resource, hero, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00d4de34 end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        actorMap = resources:NewActorMap()
                        resources:SetActor(actorMap, "ME", resource3)
                        resources:SetActor(actorMap, "HERO", resource)
                        movie4 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_END", actorMap, false, true)
                        if scratchValue4 == 0 then
                            resources:SetActor(actorMap, "ME", resource3)
                            switch = scratchValue13
                            repeat
                                if switch == 0 then
                                    if quest:GetMasterGameState("GlobalSkillGrade") == 7 then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de05 end
                                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS", actorMap, false, true)
                                        break
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS_PRIZE", actorMap, false, true)
                                        quest:ClearThingHasInformation(me)
                                        goto FLOW_native_label_1
                                    end
                                    goto LAB_00d4de05
                                elseif switch == 1 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_A", actorMap, false, true)
                                    break
                                elseif switch == 2 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_B", actorMap, false, true)
                                    break
                                elseif switch == 3 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_C", actorMap, false, true)
                                    break
                                elseif switch == 4 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_D", actorMap, false, true)
                                    break
                                elseif switch == 5 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_E", actorMap, false, true)
                                    break
                                elseif switch == 6 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_F", actorMap, false, true)
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
                            resources:DestroyMovie(movie4)
                            resources:DestroyActorMap(actorMap)
                            resources:ReleaseResource(resource)
                            quest:SetMasterGameState("HeroTakingGuildTest", false)
                            quest:SetPlayerUsingRangedDummies(false)
                            quest:DeregisterTimer(timerId)
                            goto LAB_00d4dc36
                        end
                        if not quest:IsActiveThreadTerminating() then
                            resources:SetActor(actorMap, "ME", resource3)
                            resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_HIGH", actorMap, false, true)
                            resources:SetActor(actorMap, "ME", resource3)
                            switch = scratchValue13
                            repeat
                                if switch == 0 then
                                    if quest:GetMasterGameState("GlobalSkillGrade") == 7 then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de05 end
                                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS", actorMap, false, true)
                                        break
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS_PRIZE", actorMap, false, true)
                                        quest:ClearThingHasInformation(me)
                                        goto FLOW_native_label_1_c17
                                    end
                                    goto LAB_00d4de05
                                elseif switch == 1 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_A", actorMap, false, true)
                                    break
                                elseif switch == 2 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_B", actorMap, false, true)
                                    break
                                elseif switch == 3 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_C", actorMap, false, true)
                                    break
                                elseif switch == 4 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_D", actorMap, false, true)
                                    break
                                elseif switch == 5 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_E", actorMap, false, true)
                                    break
                                elseif switch == 6 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_F", actorMap, false, true)
                                    break
                                else
                                    goto FLOW_native_label_1_c17
                                end
                            until true
                            ::FLOW_native_label_1_c17::
                            if quest:GetMasterGameState("GlobalSkillGrade") < 7 - scratchValue13 then
                                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d4de1f end
                                quest:SetMasterGameState("GlobalSkillGrade", 7 - scratchValue13)
                            end
                            quest:FixMovieSequenceCamera(false)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            resources:DestroyActorMap(actorMap)
                            resources:ReleaseResource(resource)
                            quest:SetMasterGameState("HeroTakingGuildTest", false)
                            quest:SetPlayerUsingRangedDummies(false)
                            quest:DeregisterTimer(timerId)
                            goto LAB_00d4dc36
                        end
                        ::LAB_00d4de05::
                        quest:PauseAllNonScriptedEntities(false)
                        ::LAB_00d4de1f::
                        resources:DestroyMovie(movie4)
                        resources:DestroyActorMap(actorMap)
                    end
                    ::LAB_00d4de34::
                    resources:ReleaseResource(resource)
                end
                ::LAB_00d4de3d::
                quest:DeregisterTimer(timerId)
            elseif not quest:IsActiveThreadTerminating() then
                me:ClearCommands()
                movie2 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                    if not me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_EARLY", GROUP_SELECT_FIRST, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        __cleanup_LAB_00d4de46(); return
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        __cleanup_LAB_00d4de46(); return
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
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

