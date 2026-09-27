-- Readable native conversion: SkillApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

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
    local predicateResult, scratchValue2, scratchValue3, scratchValue4, infoElement, questionAnswer
    local scratchValue, scratchValue27, timerId, index, p0, resource, actorMap, resource3, movie4
    local function ReleaseEverything()
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource3)
    end
    resource3 = resources:NewResource()
    resources:PrepareResource(resource3)
    while not resources:TryAcquire(resource3, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource3)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); return end
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntityAttachToVillage(me, quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE"))
    me:SetFriendsWithEverythingFlag(true)
    playerNotWarned = true
    scratchValue4 = 0
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 10)
    local skillApprenticeTargetMarker = quest:GetThingWithScriptName("SkillApprenticeTargetMarker")
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource3)
            return
        end
        if not quest:IsQuestActive("Q_GuildTrainingDeparture") then goto LAB_00d4c9a2 end
        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
        if ((3 < quest:GetMasterGameState("GlobalMeleeGrade")) or (3 < quest:GetMasterGameState("GlobalSkillGrade"))) or 3 < quest:GetMasterGameState("GlobalWillGrade") then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource3); return end
            if scratchValue4 ~= 0 then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                quest:ClearThingHasInformation(me)
                scratchValue4 = 0
            end
            goto LAB_00d4c9a2
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource3)
            return
        end
        if scratchValue4 == 0 then
            quest:SetThingHasInformation(me, false, true, false)
            scratchValue4 = 1
        end
        ::LAB_00d4c9a2::
        if not quest:IsDistanceBetweenThingsOver(me, skillApprenticeTargetMarker, 4.0) or me:IsPerformingScriptTask() then
            if me:IsPerformingScriptTask() then goto LAB_00d4cbac end
            if not quest:IsDistanceBetweenThingsUnder(me, hero, 10.0) or 0 < quest:GetTimer(timerId) then goto LAB_00d4cbac end
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:SetTimer(timerId, 20)
                local conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, hero)
                local getMasterGameState = quest:GetMasterGameState("GlobalSkillGrade")
                if getMasterGameState == 0 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_SKILL_EARLY_COMMENT", me, hero, false); goto LAB_00d4cba7 end
                elseif getMasterGameState == 7 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_SKILL_APLUS_COMMENT", me, hero, false); goto LAB_00d4cba7 end
                elseif not quest:IsActiveThreadTerminating() then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_SKILL_NOT_APLUS_COMMENT", me, hero, false)
                    goto LAB_00d4cba7
                end
                goto FLOW_past_lab_00d4cba7
                ::LAB_00d4cba7::
                goto LAB_00d4cbac
                ::FLOW_past_lab_00d4cba7::
            end
            ReleaseEverything(); return
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
        if not (skillApprenticeTargetMarker ~= nil and not skillApprenticeTargetMarker:IsNull()) then
            p0 = {x = 0, y = 0, z = 0}
        else
            p0 = skillApprenticeTargetMarker:GetPos()
        end
        me:MoveToPosition(p0, 3.0, ENTITY_MOVE_WALK, false, true)
        ::LAB_00d4cbac::
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource3)
                return
            end
            if not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                if quest:GetMasterGameState("HeroTakingGuildTest") then
                    if not quest:IsActiveThreadTerminating() then
                        local movie3 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                            me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_TEST_ALREADY", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    ReleaseEverything(); do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                ReleaseEverything(); return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        goto LAB_00d4dc36
                    end
                    ReleaseEverything(); return
                end
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                    me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_HELLO", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            ReleaseEverything(); do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        ReleaseEverything(); return
                    end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_SKILL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                while questionAnswer < 0 do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    else
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        ReleaseEverything(); do return end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    ReleaseEverything(); return
                end
                local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if questionAnswer == 1 then
                    if not isActiveThreadTerminating then
                        if quest:GetMasterGameState("GlobalSkillGrade") ~= 7 then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                ReleaseEverything(); return
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                                if not me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_TIME_LIMIT_APLUS", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d4dded end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie)
                                    ReleaseEverything(); return
                                end
                            end
                        end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                            if not me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_TIME_LIMIT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d4dded end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                ReleaseEverything(); return
                            end
                        end
                        goto LAB_00d4d2bb
                    end
                    ::LAB_00d4dded::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    ReleaseEverything(); return
                end
                if isActiveThreadTerminating then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    ReleaseEverything(); return
                end
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                    me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_RETURN", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            ReleaseEverything(); do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        ReleaseEverything(); return
                    end
                end
                ::LAB_00d4d2bb::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                if questionAnswer ~= 1 then goto LAB_00d4dc36 end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                quest:SetPlayerUsingRangedDummies(true)
                quest:SetMasterGameState("HeroTakingGuildTest", true)
                while not quest:MsgOnHeroFiredRangedWeapon() do
                    if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                local timerId3 = quest:RegisterTimer()
                quest:SetTimer(timerId3, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_SkillTimer))))
                quest:SetMasterGameState("SkillScore", 0)
                scratchValue3 = 0
                local infoCounter = quest:AddQuestInfoCounter("HUD_ICON_MULTI_ARROW", 0, 1.0)
                local infoCounter3 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                infoElement = quest:AddQuestInfoTimer(timerId3, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                quest:UpdateQuestInfoCounter(infoCounter, quest:GetMasterGameState("HighestSkillScore"), -1)
                scratchValue2 = 0
                while 0 < quest:GetTimer(timerId3) and scratchValue3 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d4de3d end
                    if quest:GetMasterGameState("GuildWarningOccuring") then
                        scratchValue3 = 1
                    end
                    if quest:GetMasterGameState("HighestSkillScore") < quest:GetMasterGameState("SkillScore") then
                        quest:SetMasterGameState("HighestSkillScore", quest:GetMasterGameState("SkillScore"))
                        scratchValue2 = 1
                        quest:UpdateQuestInfoCounter(infoCounter3, quest:GetMasterGameState("HighestSkillScore"), -1)
                    end
                    quest:UpdateQuestInfoCounter(infoCounter3, quest:GetMasterGameState("SkillScore"), -1)
                    if quest:IsDistanceBetweenThingsOver(hero, quest:GetThingWithScriptName("ArcheryRing"), 6.0) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                        if playerNotWarned then
                            playerNotWarned = false
                            local conversationId2 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(conversationId2, hero)
                            quest:AddLineToConversation(conversationId2, "TEXT_QST_028_APPRENTICE_SKILL_RING_OUT", me, hero, false)
                        end
                        scratchValue3 = 1
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                while not quest:IsHeroControlledByPlayer() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d4de3d end
                end
                quest:DisplayQuestInfo(false)
                quest:RemoveQuestInfoElement(infoCounter)
                quest:RemoveQuestInfoElement(infoCounter3)
                quest:RemoveQuestInfoElement(infoElement)
                if scratchValue3 ~= 0 then
                    goto LAB_00d4dc16
                end
                goto FLOW_past_lab_00d4dc16
                ::LAB_00d4dc16::
                quest:SetMasterGameState("HeroTakingGuildTest", false)
                quest:SetPlayerUsingRangedDummies(false)
                quest:DeregisterTimer(timerId3)
                goto LAB_00d4dc36
                ::FLOW_past_lab_00d4dc16::
                if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                infoElement = quest:GetMasterGameState("SkillScore")
                index = 0
                scratchValue = 0
                repeat
                    scratchValue27 = scratchValue
                    if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_SkillGrades, index) <= infoElement then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4de3d end
                        break
                    end
                    index = index + 1
                    scratchValue = scratchValue27 + 1
                until not (scratchValue27 + 1 < 7)
                resource = resources:NewResource()
                resources:PrepareResource(resource)
                while not resources:TryAcquire(resource, hero, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00d4de34 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4de34 end
                actorMap = resources:NewActorMap()
                resources:SetActor(actorMap, "ME", resource3)
                resources:SetActor(actorMap, "HERO", resource)
                movie4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_END", actorMap, false, true)
                if scratchValue2 == 0 then
                    goto LAB_00d4d979
                end
                goto FLOW_past_lab_00d4d979
                ::LAB_00d4d979::
                resources:SetActor(actorMap, "ME", resource3)
                repeat
                    if scratchValue27 == 0 then
                        if quest:GetMasterGameState("GlobalSkillGrade") == 7 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4de05 end
                            resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS", actorMap, false, true)
                            break
                        end
                        if not quest:IsActiveThreadTerminating() then
                            resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_APLUS_PRIZE", actorMap, false, true)
                            quest:ClearThingHasInformation(me)
                            break
                        end
                        goto LAB_00d4de05
                    elseif scratchValue27 == 1 then
                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_A", actorMap, false, true)
                        break
                    elseif scratchValue27 == 2 then
                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_B", actorMap, false, true)
                        break
                    elseif scratchValue27 == 3 then
                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_C", actorMap, false, true)
                        break
                    elseif scratchValue27 == 4 then
                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_D", actorMap, false, true)
                        break
                    elseif scratchValue27 == 5 then
                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_E", actorMap, false, true)
                        break
                    elseif scratchValue27 == 6 then
                        resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_F", actorMap, false, true)
                        break
                    else
                        break
                    end
                until true
                if quest:GetMasterGameState("GlobalSkillGrade") < 7 - scratchValue27 then
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d4de1f end
                    quest:SetMasterGameState("GlobalSkillGrade", 7 - scratchValue27)
                end
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                resources:DestroyActorMap(actorMap)
                resources:ReleaseResource(resource)
                goto LAB_00d4dc16
                ::FLOW_past_lab_00d4d979::
                if quest:IsActiveThreadTerminating() then goto LAB_00d4de05 end
                resources:SetActor(actorMap, "ME", resource3)
                resources:RunMacro("CS_GUILD_DEPARTURE_SKILL_TEST_HIGH", actorMap, false, true)
                goto LAB_00d4d979
                ::LAB_00d4de05::
                quest:PauseAllNonScriptedEntities(false)
                ::LAB_00d4de1f::
                resources:DestroyMovie(movie4)
                resources:DestroyActorMap(actorMap)
                ::LAB_00d4de34::
                resources:ReleaseResource(resource)
                ::LAB_00d4de3d::
                quest:DeregisterTimer(timerId3)
            else
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                me:ClearCommands()
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                    me:Speak(hero, "TEXT_QST_028_APPRENTICE_SKILL_EARLY", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            ReleaseEverything(); do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        ReleaseEverything(); return
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                goto LAB_00d4dc36
            end
            ReleaseEverything()
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
function OnPersist(quest, me, context)
end

-- SkillApprentice.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

