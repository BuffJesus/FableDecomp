-- Readable native conversion: WillApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_WillGrades = 3788,  -- '070000000000304100002041000000410000c040000040400000803f00000000'
    GUI_WillTimer = 3848,  -- 30.0
}

-- WillApprentice.Main (retail 0x00d4efe0)
function Main(quest, me)
    local willHelpTimer = quest:GetStateInt("WillHelpTimer")
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, scratchValue3, scratchValue4, scratchValue5, scratchValue, questionAnswer
    local scratchValue30, index, p0, timerId, resource, timerId4, infoElement
    local function ReleaseEverything()
        quest:DeregisterTimer(timerId4)
        resources:ReleaseResource(resource)
    end
    resource = resources:NewResource()
    resources:PrepareResource(resource)
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
    me:SetFriendsWithEverythingFlag(true)
    scratchValue5 = 0
    local willApprenticeTargetMarker = quest:GetThingWithScriptName("WillApprenticeTargetMarker")
    timerId4 = quest:RegisterTimer()
    quest:SetTimer(timerId4, 10)
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            quest:DeregisterTimer(timerId4)
            resources:ReleaseResource(resource)
            return
        end
        if not quest:IsQuestActive("Q_GuildTrainingDeparture") then goto LAB_00d4f285 end
        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
        if ((3 < quest:GetMasterGameState("GlobalMeleeGrade")) or (3 < quest:GetMasterGameState("GlobalSkillGrade"))) or 3 < quest:GetMasterGameState("GlobalWillGrade") then
            if not quest:IsActiveThreadTerminating() then
                if scratchValue5 ~= 0 then
                    quest:ClearThingHasInformation(me)
                    scratchValue5 = 0
                end
                goto LAB_00d4f285
            end
            ReleaseEverything(); return
        end
        if scratchValue5 == 0 then
            quest:SetThingHasInformation(me, false, true, false)
            scratchValue5 = 1
        end
        ::LAB_00d4f285::
        if not quest:IsDistanceBetweenThingsOver(me, willApprenticeTargetMarker, 4.0) or me:IsPerformingScriptTask() then
            if me:IsPerformingScriptTask() then goto LAB_00d4f49e end
            if not quest:IsDistanceBetweenThingsUnder(me, hero, 10.0) or 0 < quest:GetTimer(timerId4) then goto LAB_00d4f49e end
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:SetTimer(timerId4, 20)
                local conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, hero)
                local getMasterGameState = quest:GetMasterGameState("GlobalWillGrade")
                if getMasterGameState == 0 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_WILL_EARLY_COMMENT", me, hero, false); goto LAB_00d4f499 end
                elseif getMasterGameState == 7 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_WILL_APLUS_COMMENT", me, hero, false); goto LAB_00d4f499 end
                elseif not quest:IsActiveThreadTerminating() then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_WILL_NOT_APLUS_COMMENT", me, hero, false)
                    goto LAB_00d4f499
                end
                goto FLOW_past_lab_00d4f499
                ::LAB_00d4f499::
                goto LAB_00d4f49e
                ::FLOW_past_lab_00d4f499::
            end
            ReleaseEverything(); return
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
        if not (willApprenticeTargetMarker ~= nil and not willApprenticeTargetMarker:IsNull()) then
            p0 = {x = 0, y = 0, z = 0}
        else
            p0 = willApprenticeTargetMarker:GetPos()
        end
        me:MoveToPosition(p0, 3.0, ENTITY_MOVE_WALK, false, true)
        ::LAB_00d4f49e::
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            if not me:IsPerformingScriptTask() then
                if quest:GetMasterGameState("HeroTakingGuildTest") then
                    if not quest:IsActiveThreadTerminating() then
                        local movie3 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_028_APPRENTICE_WILL_MULTI_GRADE", GROUP_SELECT_FIRST, false, true, false)
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
                        goto LAB_00d503cb
                    end
                    ReleaseEverything(); return
                end
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_WILL_HELLO", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
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
                if questionAnswer ~= 1 then
                    if not isActiveThreadTerminating then
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_028_APPRENTICE_WILL_RETURN", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d50542 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d50542 end
                        end
                        goto LAB_00d4fb0a
                    end
                    ::LAB_00d50542::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    ReleaseEverything(); return
                end
                if isActiveThreadTerminating then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    ReleaseEverything(); return
                end
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, "TEXT_QST_028_APPRENTICE_WILL_INTRO", GROUP_SELECT_FIRST, false, true, false)
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
                if quest:GetMasterGameState("GlobalWillGrade") ~= 7 then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        ReleaseEverything(); return
                    end
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_028_APPRENTICE_WILL_INTRO_APLUS", GROUP_SELECT_FIRST, false, true, false)
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
                end
                ::LAB_00d4fb0a::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                if questionAnswer ~= 1 then goto LAB_00d503cb end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                quest:SetPlayerUsingWillDummies(true)
                quest:SetMasterGameState("HeroTakingGuildTest", true)
                while not quest:MsgOnHeroCastSpell() do
                    if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                local timerId2 = quest:RegisterTimer()
                quest:SetTimer(timerId2, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_WillTimer))))
                quest:SetMasterGameState("WillScore", 0)
                quest:SetTimer(willHelpTimer, 0)
                local addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                infoElement = quest:AddQuestInfoTimer(timerId2, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                scratchValue3 = 0
                scratchValue4 = 0
                local timerId3 = quest:RegisterTimer()
                timerId = timerId3
                quest:SetTimer(timerId3, 0)
                while 0 < quest:GetTimer(timerId2) and scratchValue3 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d50594 end
                    if quest:GetTimer(timerId) < 1 then
                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                        quest:SetTimer(timerId, 2)
                    end
                    if quest:GetMasterGameState("GuildWarningOccuring") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d50594 end
                        scratchValue3 = 1
                        scratchValue4 = 1
                    end
                    if quest:GetHeroWillEnergy() == 0 and quest:GetTimer(willHelpTimer) < 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d50594 end
                        local conversationId2 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId2, hero)
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_APPRENTICE_WILL_NO_WILL", me, hero, false)
                        quest:SetTimer(willHelpTimer, 8)
                        timerId = timerId3
                        scratchValue3 = scratchValue4
                    end
                    if not quest:IsDistanceBetweenThingsOver(hero, me, 30.0) then
                        quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetMasterGameState("WillScore"), -1)
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00d50594 end
                        scratchValue3 = 1
                        scratchValue4 = 1
                        quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetMasterGameState("WillScore"), -1)
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d50594 end
                while not quest:IsHeroControlledByPlayer() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d50594 end
                end
                quest:DisplayQuestInfo(false)
                quest:RemoveQuestInfoElement(addQuestInfoCounter)
                quest:RemoveQuestInfoElement(infoElement)
                if scratchValue3 == 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d50594 end
                    infoElement = quest:GetMasterGameState("WillScore")
                    index = 0
                    scratchValue30 = 0
                    repeat
                        scratchValue = scratchValue30
                        if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_WillGrades, index) <= infoElement then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d50594 end
                            break
                        end
                        index = index + 1
                        scratchValue30 = scratchValue + 1
                    until not (scratchValue + 1 < 7)
                    local resource3 = resources:NewResource()
                    resources:PrepareResource(resource3)
                    while not resources:TryAcquire(resource3, hero, 4) do
                        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource3); goto LAB_00d50594 end
                    end
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource3); goto LAB_00d50594 end
                    local actorMap = resources:NewActorMap()
                    resources:SetActor(actorMap, "ME", resource)
                    resources:SetActor(actorMap, "HERO", resource3)
                    local movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    quest:FixMovieSequenceCamera(true)
                    resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_END", actorMap, false, true)
                    resources:SetActor(actorMap, "ME", resource)
                    repeat
                        if scratchValue == 0 then
                            if quest:GetMasterGameState("GlobalWillGrade") ~= 7 then
                                if not quest:IsActiveThreadTerminating() then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_APLUS_PRIZE", actorMap, false, true)
                                    quest:ClearThingHasInformation(me)
                                    goto FLOW_native_label_1
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                resources:DestroyActorMap(actorMap)
                                resources:ReleaseResource(resource3)
                                goto LAB_00d50594
                            end
                            if not quest:IsActiveThreadTerminating() then resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_APLUS", actorMap, false, true); break end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            resources:DestroyActorMap(actorMap)
                            resources:ReleaseResource(resource3)
                            goto LAB_00d50594
                        elseif scratchValue == 1 then
                            resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_A", actorMap, false, true)
                            break
                        elseif scratchValue == 2 then
                            resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_B", actorMap, false, true)
                            break
                        elseif scratchValue == 3 then
                            resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_C", actorMap, false, true)
                            break
                        elseif scratchValue == 4 then
                            resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_D", actorMap, false, true)
                            break
                        elseif scratchValue == 5 then
                            resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_E", actorMap, false, true)
                            break
                        elseif scratchValue == 6 then
                            resources:RunMacro("CS_GUILD_DEPARTURE_WILL_TEST_F", actorMap, false, true)
                            break
                        else
                            goto FLOW_native_label_1
                        end
                    until true
                    ::FLOW_native_label_1::
                    if quest:GetMasterGameState("GlobalWillGrade") < 7 - scratchValue then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            resources:DestroyActorMap(actorMap)
                            resources:ReleaseResource(resource3)
                            goto LAB_00d50594
                        end
                        quest:SetMasterGameState("GlobalWillGrade", 7 - scratchValue)
                    end
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    resources:DestroyActorMap(actorMap)
                    resources:ReleaseResource(resource3)
                end
                quest:SetMasterGameState("HeroTakingGuildTest", false)
                quest:SetPlayerUsingWillDummies(false)
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId2)
                goto LAB_00d503cb
                ::LAB_00d50594::
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId2)
            else
                me:ClearCommands()
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                    me:Speak(hero, "TEXT_QST_028_APPRENTICE_WILL_EARLY", GROUP_SELECT_FIRST, false, true, false)
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
                goto LAB_00d503cb
            end
            ReleaseEverything()
            return
        end
        ::LAB_00d503cb::
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
    until false
end

-- WillApprentice.Init (retail 0x00d43330)
function Init(quest, me)
end

-- WillApprentice.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- WillApprentice.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

