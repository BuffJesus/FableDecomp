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
    local predicateResult, isActiveThreadTerminating, scratchValue2, scratchValue3, scratchValue4
    local getMasterGameState, questionAnswer, infoCounter, scratchValue, conversationId
    local conversationId2, scratchValue23, scratchValue25, p0, skillApprenticeTargetMarker, movie
    local infoCounter2, timerId, resource, timerId3, movie2, movie3, infoElement
    local function __cleanup_LAB_00d4dd41()
        quest:DeregisterTimer(timerId3)
        resources:ReleaseResource(resource)
    end
    local function __cleanup_LAB_00d4de46()
        quest:DeregisterTimer(timerId3)
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
    scratchValue4 = 0
    timerId3 = quest:RegisterTimer()
    quest:SetTimer(timerId3, 10)
    skillApprenticeTargetMarker = quest:GetThingWithScriptName("SkillApprenticeTargetMarker")
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            quest:DeregisterTimer(timerId3)
            resources:ReleaseResource(resource)
            return
        end
        if not quest:IsQuestActive("Q_GuildTrainingDeparture") then goto LAB_00d4c9a2 end
        if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4dd41(); return end
        if ((3 < quest:GetMasterGameState("GlobalMeleeGrade")) or (3 < quest:GetMasterGameState("GlobalSkillGrade"))) or 3 < quest:GetMasterGameState("GlobalWillGrade") then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId3); resources:ReleaseResource(resource); return end
            if scratchValue4 ~= 0 then
                if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d4de46(); return end
                quest:ClearThingHasInformation(me)
                scratchValue4 = 0
            end
            goto LAB_00d4c9a2
            quest:DeregisterTimer(timerId3)
            resources:ReleaseResource(resource)
            return
        end
        if scratchValue4 == 0 then
            quest:SetThingHasInformation(me, false, true, false)
            scratchValue4 = 1
        end
        ::LAB_00d4c9a2::
        if not quest:IsDistanceBetweenThingsOver(me, skillApprenticeTargetMarker, 4.0) or me:IsPerformingScriptTask() then
            if me:IsPerformingScriptTask() then goto LAB_00d4cbac end
            if not quest:IsDistanceBetweenThingsUnder(me, hero, 10.0) or 0 < quest:GetTimer(timerId3) then goto LAB_00d4cbac end
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:SetTimer(timerId3, 20)
                conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, hero)
                getMasterGameState = quest:GetMasterGameState("GlobalSkillGrade")
                if getMasterGameState == 0 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_SKILL_EARLY_COMMENT", me, hero, false); goto LAB_00d4cbac end
                elseif getMasterGameState == 7 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_SKILL_APLUS_COMMENT", me, hero, false); goto LAB_00d4cbac end
                elseif not quest:IsActiveThreadTerminating() then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_SKILL_NOT_APLUS_COMMENT", me, hero, false)
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
                quest:DeregisterTimer(timerId3)
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
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                while questionAnswer < 0 do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    else
                        quest:EndCutscene()
                        resources:DestroyMovie(movie)
                        __cleanup_LAB_00d4de46(); do return end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:EndCutscene()
                    resources:DestroyMovie(movie)
                    __cleanup_LAB_00d4de46(); return
                end
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if questionAnswer == 1 then
                    if not isActiveThreadTerminating then
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
                if isActiveThreadTerminating then
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
                if questionAnswer ~= 1 then goto LAB_00d4dc36 end
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
                scratchValue3 = 0
                infoCounter = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
                infoCounter2 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                infoElement = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                quest:UpdateQuestInfoCounter(infoCounter, quest:GetMasterGameState("HighestSkillScore"), -1)
                scratchValue2 = 0
                while 0 < quest:GetTimer(timerId) and scratchValue3 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d4de3d end
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        scratchValue3 = 1
                    end
                    if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                        quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                        scratchValue2 = 1
                        quest:UpdateQuestInfoCounter(infoCounter2, quest:GetMasterGameState("HighestSkillScore"), -1)
                    end
                    quest:UpdateQuestInfoCounter(infoCounter2, quest:GetMasterGameState("SkillScore"), -1)
                    if not quest:IsDistanceBetweenThingsOver(hero, quest:GetThingWithScriptName("ArcheryRing"), 6.0) then goto continue_6 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                    if playerNotWarned then
                        playerNotWarned = false
                        conversationId2 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId2, hero)
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_APPRENTICE_SKILL_RING_OUT", me, hero, false)
                    end
                    scratchValue3 = 1
                    ::continue_6::
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                while not quest:IsHeroControlledByPlayer() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d4de3d end
                end
                quest:DisplayQuestInfo(false)
                quest:RemoveQuestInfoElement(infoCounter)
                quest:RemoveQuestInfoElement(infoCounter2)
                quest:RemoveQuestInfoElement(infoElement)
                if scratchValue3 ~= 0 then
                    quest:SetMasterGameState("HeroTakingGuildTest", false)
                    quest:SetPlayerUsingRangedDummies(false)
                    quest:DeregisterTimer(timerId)
                    goto LAB_00d4dc36
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                infoElement = quest:GetMasterGameState("SkillScore")
                scratchValue25 = 0
                scratchValue = 0
                repeat
                    scratchValue23 = scratchValue
                    if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_SkillGrades, scratchValue25) <= infoElement then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                        break
                    end
                    scratchValue25 = scratchValue25 + 1
                    scratchValue = scratchValue23 + 1
                until not (scratchValue23 + 1 < 7)
                if not hero:AcquireControl(4) then goto LAB_00d4de34 end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4de34 end
                quest:StartCutscene({ME = me, HERO = hero, ME = me, ME = me, ME = me}, {}, true)
                quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_END", true, false)
                if scratchValue2 == 0 then
                    repeat
                        if scratchValue23 == 0 then
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
                        elseif scratchValue23 == 1 then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_A", true, false)
                            break
                        elseif scratchValue23 == 2 then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_B", true, false)
                            break
                        elseif scratchValue23 == 3 then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_C", true, false)
                            break
                        elseif scratchValue23 == 4 then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_D", true, false)
                            break
                        elseif scratchValue23 == 5 then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_E", true, false)
                            break
                        elseif scratchValue23 == 6 then
                            quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_F", true, false)
                            break
                        else
                            goto FLOW_native_label_1
                        end
                    until true
                    ::FLOW_native_label_1::
                    if quest:GetMasterGameState("GlobalSkillGrade") < 7 - scratchValue23 then
                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d4de1f end
                        quest:SetMasterGameState("GlobalSkillGrade", 7 - scratchValue23)
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
                repeat
                    if scratchValue23 == 0 then
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
                    elseif scratchValue23 == 1 then
                        quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_A", true, false)
                        break
                    elseif scratchValue23 == 2 then
                        quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_B", true, false)
                        break
                    elseif scratchValue23 == 3 then
                        quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_C", true, false)
                        break
                    elseif scratchValue23 == 4 then
                        quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_D", true, false)
                        break
                    elseif scratchValue23 == 5 then
                        quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_E", true, false)
                        break
                    elseif scratchValue23 == 6 then
                        quest:RunCutscene("CS_GUILD_DEPARTURE_SKILL_TEST_F", true, false)
                        break
                    else
                        goto FLOW_native_label_1_c17
                    end
                until true
                ::FLOW_native_label_1_c17::
                if quest:GetMasterGameState("GlobalSkillGrade") < 7 - scratchValue23 then
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d4de1f end
                    quest:SetMasterGameState("GlobalSkillGrade", 7 - scratchValue23)
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
        predicateResult = quest:IsActiveThreadTerminating()
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

