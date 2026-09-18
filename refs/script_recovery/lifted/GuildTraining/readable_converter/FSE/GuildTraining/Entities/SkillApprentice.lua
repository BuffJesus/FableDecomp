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
    local scratchValue, scratchValue2, scratchValue4, scratchValue5, scratchValue6, scratchValue12
    local scratchValue13, scratchValue14, sequence1, switch, p0, skillApprenticeTargetMarker, movie
    local infoCounter, timerId, resource, scratchValue17, movie2, movie3, scratchValue18
    local scratchValue19
    local function __cleanup_LAB_00d4dd41()
        quest:DeregisterTimer(scratchValue17)
        resources:ReleaseResource(resource)
    end
    local function __cleanup_LAB_00d4de46()
        quest:DeregisterTimer(scratchValue17)
        resources:ReleaseResource(resource)
    end
    resource = resources:NewResource()
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
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
            resources:ReleaseResource(resource)
            return
        end
        if not quest:IsQuestActive("Q_GuildTrainingDeparture") then goto LAB_00d4c9a2 end
        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4dd41(); return end
        if ((3 < quest:GetMasterGameState("GlobalMeleeGrade")) or (3 < quest:GetMasterGameState("GlobalSkillGrade"))) or 3 < quest:GetMasterGameState("GlobalWillGrade") then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue17); resources:ReleaseResource(resource); return end
            if scratchValue6 ~= 0 then
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
                quest:ClearThingHasInformation(me)
                scratchValue6 = 0
            end
            goto LAB_00d4c9a2
            quest:DeregisterTimer(scratchValue17)
            resources:ReleaseResource(resource)
            return
        end
        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
        if scratchValue6 == 0 then
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
            quest:SetThingHasInformation(me, false, true, false)
            scratchValue6 = 1
        end
        ::LAB_00d4c9a2::
        scratchValue = not quest:IsDistanceBetweenThingsOver(me, skillApprenticeTargetMarker, 4.0) or me:IsPerformingScriptTask()
        if scratchValue then
            if me:IsPerformingScriptTask() then goto LAB_00d4cbac end
            sequence1 = not quest:IsDistanceBetweenThingsUnder(me, hero, 10.0) or 0 < quest:GetTimer(scratchValue17)
            if sequence1 then goto LAB_00d4cbac end
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:SetTimer(scratchValue17, 20)
                scratchValue13 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue13, hero)
                scratchValue12 = quest:GetMasterGameState("GlobalSkillGrade")
                if scratchValue12 == 0 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_APPRENTICE_SKILL_EARLY_COMMENT", me, hero, false); goto LAB_00d4cbac end
                elseif scratchValue12 == 7 then
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
                resources:ReleaseResource(resource)
                return
            end
            if not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
                if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                    if not quest:IsActiveThreadTerminating() then
                        movie3 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_TEST_ALREADY", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:EndCutscene()
                                    resources:DestroyMovie(movie3)
                                    __cleanup_LAB_00d4de46(); do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:EndCutscene()
                                resources:DestroyMovie(movie3)
                                __cleanup_LAB_00d4de46(); return
                            end
                        end
                        quest:EndCutscene()
                        resources:DestroyMovie(movie3)
                        goto LAB_00d4dc36
                    end
                    __cleanup_LAB_00d4de46(); return
                end
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
                movie = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_HELLO", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:EndCutscene()
                            resources:DestroyMovie(movie)
                            __cleanup_LAB_00d4de46(); do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:EndCutscene()
                        resources:DestroyMovie(movie)
                        __cleanup_LAB_00d4de46(); return
                    end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_SKILL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                scratchValue12 = quest:MsgIsQuestionAnsweredYesOrNo()
                while scratchValue12 < 0 do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue12 = quest:MsgIsQuestionAnsweredYesOrNo()
                    else
                        quest:EndCutscene()
                        resources:DestroyMovie(movie)
                        __cleanup_LAB_00d4de46(); do return end
                        scratchValue12 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:EndCutscene()
                    resources:DestroyMovie(movie)
                    __cleanup_LAB_00d4de46(); return
                end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue12 == 1 then
                    if not scratchValue2 then
                        if quest:GetMasterGameState("GlobalSkillGrade") ~= 7 then
                            if quest:IsActiveThreadTerminating() then
                                quest:EndCutscene()
                                resources:DestroyMovie(movie)
                                __cleanup_LAB_00d4de46(); return
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                if not me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_TIME_LIMIT_APLUS", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d4dded end
                                if quest:IsActiveThreadTerminating() then
                                    quest:EndCutscene()
                                    resources:DestroyMovie(movie)
                                    __cleanup_LAB_00d4de46(); return
                                end
                            end
                        end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_TIME_LIMIT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d4dded end
                            if quest:IsActiveThreadTerminating() then
                                quest:EndCutscene()
                                resources:DestroyMovie(movie)
                                __cleanup_LAB_00d4de46(); return
                            end
                        end
                        goto LAB_00d4d2bb
                    end
                    ::LAB_00d4dded::
                    quest:EndCutscene()
                    resources:DestroyMovie(movie)
                    __cleanup_LAB_00d4de46(); return
                end
                if scratchValue2 then
                    quest:EndCutscene()
                    resources:DestroyMovie(movie)
                    __cleanup_LAB_00d4de46(); return
                end
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_RETURN", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:EndCutscene()
                            resources:DestroyMovie(movie)
                            __cleanup_LAB_00d4de46(); do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:EndCutscene()
                        resources:DestroyMovie(movie)
                        __cleanup_LAB_00d4de46(); return
                    end
                end
                ::LAB_00d4d2bb::
                quest:EndCutscene()
                resources:DestroyMovie(movie)
                if scratchValue12 ~= 1 then goto LAB_00d4dc36 end
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
                scratchValue12 = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
                scratchValue18 = scratchValue12
                infoCounter = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                scratchValue19 = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                quest:UpdateQuestInfoCounter(scratchValue12, quest:GetMasterGameState("HighestSkillScore"), -1)
                scratchValue4 = 0
                while 0 < quest:GetTimer(timerId) and scratchValue5 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d4de3d end
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                        scratchValue5 = 1
                    end
                    if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                        quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                        scratchValue4 = 1
                        quest:UpdateQuestInfoCounter(infoCounter, quest:GetMasterGameState("HighestSkillScore"), -1)
                    end
                    quest:UpdateQuestInfoCounter(infoCounter, quest:GetMasterGameState("SkillScore"), -1)
                    if not quest:IsDistanceBetweenThingsOver(hero, quest:GetThingWithScriptName("ArcheryRing"), 6.0) then goto continue_6 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                    if playerNotWarned then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                        playerNotWarned = false
                        scratchValue13 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue13, hero)
                        quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_APPRENTICE_SKILL_RING_OUT", me, hero, false)
                    end
                    scratchValue5 = 1
                    ::continue_6::
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
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
                scratchValue12 = 0
                repeat
                    scratchValue13 = scratchValue12
                    if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_SkillGrades, scratchValue14) <= scratchValue19 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                        break
                    end
                    scratchValue14 = scratchValue14 + 1
                    scratchValue12 = scratchValue13 + 1
                until not (scratchValue13 + 1 < 7)
                if not hero:AcquireControl(4) then goto LAB_00d4de34 end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4de34 end
                quest:StartCutscene({ME = me, HERO = hero, ME = me, ME = me, ME = me}, {}, true)
                quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_END", true, false)
                if scratchValue4 == 0 then
                    switch = scratchValue13
                    repeat
                        if switch == 0 then
                            if quest:GetMasterGameState("GlobalSkillGrade") == 7 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d4de05 end
                                quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS", true, false)
                                break
                            end
                            if not quest:IsActiveThreadTerminating() then
                                quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS_PRIZE", true, false)
                                quest:ClearThingHasInformation(me)
                                goto FLOW_native_label_1
                            end
                            goto LAB_00d4de05
                        elseif switch == 1 then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_A", true, false)
                            break
                        elseif switch == 2 then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_B", true, false)
                            break
                        elseif switch == 3 then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_C", true, false)
                            break
                        elseif switch == 4 then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_D", true, false)
                            break
                        elseif switch == 5 then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_E", true, false)
                            break
                        elseif switch == 6 then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_F", true, false)
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
                    quest:EndCutscene()
                    hero:ReleaseControl()
                    quest:SetMasterGameState("HeroTakingGuildTest", false)
                    quest:SetPlayerUsingRangedDummies(false)
                    quest:DeregisterTimer(timerId)
                    goto LAB_00d4dc36
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4de05 end
                quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_HIGH", true, false)
                switch = scratchValue13
                repeat
                    if switch == 0 then
                        if quest:GetMasterGameState("GlobalSkillGrade") == 7 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4de05 end
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS", true, false)
                            break
                        end
                        if not quest:IsActiveThreadTerminating() then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS_PRIZE", true, false)
                            quest:ClearThingHasInformation(me)
                            goto FLOW_native_label_1_c17
                        end
                        goto LAB_00d4de05
                    elseif switch == 1 then
                        quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_A", true, false)
                        break
                    elseif switch == 2 then
                        quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_B", true, false)
                        break
                    elseif switch == 3 then
                        quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_C", true, false)
                        break
                    elseif switch == 4 then
                        quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_D", true, false)
                        break
                    elseif switch == 5 then
                        quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_E", true, false)
                        break
                    elseif switch == 6 then
                        quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_F", true, false)
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
                quest:EndCutscene()
                hero:ReleaseControl()
                quest:SetMasterGameState("HeroTakingGuildTest", false)
                quest:SetPlayerUsingRangedDummies(false)
                quest:DeregisterTimer(timerId)
                goto LAB_00d4dc36
                ::LAB_00d4de05::
                ::LAB_00d4de1f::
                quest:EndCutscene()
                ::LAB_00d4de34::
                hero:ReleaseControl()
                ::LAB_00d4de3d::
                quest:DeregisterTimer(timerId)
            else
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
                me:ClearCommands()
                movie2 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_EARLY", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:EndCutscene()
                            resources:DestroyMovie(movie2)
                            __cleanup_LAB_00d4de46(); do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:EndCutscene()
                        resources:DestroyMovie(movie2)
                        __cleanup_LAB_00d4de46(); return
                    end
                end
                quest:EndCutscene()
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

