-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_LIGHTNING_SPELL = 11  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_WillGrades = 3788,  -- '070000000000304100002041000000410000c040000040400000803f00000000'
    GUI_WillTimer = 3848,  -- 30.0
}

-- TheRealGuildmaster.Main (retail 0x00d5e0c0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local timerId3, isTalkedToByHero, scratchValue4, ctr_22c, dist, tutorialState, scratchValue8
    local questionAnswer, questionAnswer2, questionAnswer3, questionAnswer4, questionAnswer5
    local questionAnswer6, conversationId, scratchValue, index, willApprentice5, grade, resource
    local actorMap2, movie2, infoElement, resource2, resource3, actorMap3, resource4, timerId6
    local timerId8, timerId9, timerId, resource6
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(resource2)
    end
    local function ReleaseEverything2()
        quest:DeregisterTimer(timerId)
        quest:DeregisterTimer(timerId9)
        resources:ReleaseResource(resource4)
    end
    resource4 = resources:NewResource()
    while not resources:TryAcquire(resource4, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource4)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d61b7b end
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAlwaysBlockAttacksFromThing(me, hero, true)
    quest:SetThingHasInformation(me, false, false, false)
    quest:SetPlayerUsingWillDummies(true)
    quest:SetIsPushableByHero(me, false)
    timerId9 = quest:RegisterTimer()
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    ctr_22c = 0
    quest:EntitySetTargetingType(me, 26)
    if not quest:GetStateBool("TestFinished") then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            quest:DeregisterTimer(timerId9)
            resources:ReleaseResource(resource4)
            return
        end
        me:MoveToPosition(quest:GetThingWithScriptName("M_WillTeacherStand"):GetPos(), 3.0, ENTITY_MOVE_WALK, false, true)
        while quest:GetStateInt("TutorialState") == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
            if me:IsTalkedToByHero() then
                if quest:GetMasterGameState("HeroTakingGuildTest") == 0 then
                    quest:SetStateInt("TutorialState", 2)
                else
                    resource2 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    me:ClearCommands()
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                        grade = "TEXT_QST_028_GUILDMASTER_WILL_NOT_START"
                        me:Speak(hero, grade, 0, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(resource2)
                                ReleaseEverything2(); do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(resource2)
                            goto LAB_00d61b69
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(resource2)
                end
            end
            dist = 5.5
            if not ((quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1) and not me:IsPerformingScriptTask()) then goto continue_3 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
            conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:SetTimer(timerId, 10)
            if ctr_22c == 0 then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_FIRST", me, hero, false)
            elseif ctr_22c == 1 then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_SECOND", me, hero, false)
            end
            ctr_22c = 1 - ctr_22c
            ::continue_3::
        end
        if not quest:IsActiveThreadTerminating() then
            quest:SetMasterGameState("HeroTakingGuildTest", true)
            repeat
                if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
                quest:SetStateInt("TutorialState", 2)
                local resource5 = resources:NewResource()
                while not resources:TryAcquire(resource5, hero, 4) do
                    if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource5); goto LAB_00d61b69 end
                end
                local actorMap4 = resources:NewActorMap()
                resources:SetActor(actorMap4, "HERO", resource5)
                resources:SetActor(actorMap4, "TEACHER", resource4)
                quest:GiveHeroAbility(HERO_ABILITY_LIGHTNING_SPELL, true)
                quest:SetMasterGameState("WillTrainingStarted", true)
                local movie3 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_GUILD_WILL_LIGHTNING", actorMap4, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                resources:DestroyActorMap(actorMap4)
                resources:ReleaseResource(resource5)
                if quest:IsXbox() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)xStack_134);
                end
                quest:SetMasterGameState("WillScore", 0)
                timerId8 = quest:RegisterTimer()
                quest:SetTimer(timerId8, 0)
                tutorialState = quest:GetStateInt("TutorialState")
                scratchValue4 = 0
                while tutorialState == 2 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                    while scratchValue4 == 0 and quest:GetMasterGameState("WillScore") == 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        if me:IsTalkedToByHero() then
                            scratchValue4 = 1
                        end
                        if quest:GetTimer(timerId8) < 1 then
                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                            quest:SetTimer(timerId8, 2)
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    if quest:GetMasterGameState("WillScore") ~= 0 then
                        quest:SetStateInt("TutorialState", 3)
                        resource6 = resources:NewResource()
                        while not resources:TryAcquire(resource6, hero, 4) do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(resource6)
                                quest:DeregisterTimer(timerId8)
                                goto LAB_00d61b69
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(resource6)
                            quest:DeregisterTimer(timerId8)
                            goto LAB_00d61b69
                        end
                        local actorMap = resources:NewActorMap()
                        resources:SetActor(actorMap, "HERO", resource6)
                        resources:SetActor(actorMap, "TEACHER", resource4)
                        local movie = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_WILL_TEST", actorMap, false, true)
                        quest:FixMovieSequenceCamera(false)
                        resources:DestroyMovie(movie)
                        resources:DestroyActorMap(actorMap)
                        resources:ReleaseResource(resource6)
                    end
                    if scratchValue4 == 0 then tutorialState = quest:GetStateInt("TutorialState"); goto continue_7 end
                    timerId3 = quest:IsActiveThreadTerminating()
                    if timerId3 then goto LAB_00d6138b end
                    scratchValue4 = timerId3
                    if quest:IsXbox() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                        quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        end
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                        quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    tutorialState = quest:GetStateInt("TutorialState")
                    ::continue_7::
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                if quest:IsXbox() then
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                    end
                else
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                while not quest:MsgOnHeroCastSpell() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                    if not me:IsTalkedToByHero() then goto continue_8 end
                    if quest:IsXbox() then
                        quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                        quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        end
                    else
                        quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                        quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    ::continue_8::
                end
                local timerId7 = quest:RegisterTimer()
                quest:SetTimer(timerId7, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_WillTimer))))
                quest:SetStateInt("TutorialState", 3)
                quest:SetMasterGameState("WillScore", 0)
                timerId6 = quest:RegisterTimer()
                quest:SetTimer(ctr_22c, 0)
                local addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                infoElement = quest:AddQuestInfoTimer(timerId6, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                scratchValue4 = 0
                quest:SetTimer(timerId8, 0)
                while 0 < quest:GetTimer(timerId7) and scratchValue4 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                    if quest:GetTimer(timerId8) < 1 then
                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                        quest:SetTimer(timerId8, 2)
                    end
                    quest:SetMasterGameState("WillTestOccuring", true)
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        scratchValue4 = 1
                    end
                    if quest:GetHeroWillEnergy() == 0 and quest:GetTimer(timerId) < 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        local conversationId2 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId2, hero)
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_GUILDMASTER_WILL_NO_WILL", me, hero, false)
                        quest:SetTimer(ctr_22c, 8)
                    end
                    if not me:IsTalkedToByHero() then quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetMasterGameState("WillScore"), -1); goto continue_9 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                    if quest:IsXbox() then
                        quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                        quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                        end
                    else
                        quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                        quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                    quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetMasterGameState("WillScore"), -1)
                    ::continue_9::
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                quest:SetMasterGameState("WillTestOccuring", false)
                while not quest:IsHeroControlledByPlayer() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                end
                quest:DisplayQuestInfo(false)
                quest:RemoveQuestInfoElement(addQuestInfoCounter)
                quest:RemoveQuestInfoElement(infoElement)
                quest:SetStateInt("TutorialState", 0)
                resource3 = resources:NewResource()
                while not resources:TryAcquire(resource6, hero, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61370 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d61370 end
                actorMap3 = resources:NewActorMap()
                movie2 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:FixMovieSequenceCamera(true)
                if scratchValue4 == 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                    infoElement = quest:GetMasterGameState("WillScore")
                    index = 0
                    scratchValue8 = 0
                    repeat
                        scratchValue = scratchValue8
                        if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_WillGrades, index) < infoElement ~= (quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_WillGrades, index) == infoElement) then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                            break
                        end
                        scratchValue8 = scratchValue + 1
                        index = index + 1
                    until scratchValue8 >= 7
                    actorMap2 = resources:NewStringMap()
                    repeat
                        if scratchValue == 0 then
                            grade = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_APLUS"
                            resources:SetString(actorMap2, "$GRADE", grade)
                            break
                        elseif scratchValue == 1 then
                            grade = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_A"
                            resources:SetString(actorMap2, "$GRADE", grade)
                            break
                        elseif scratchValue == 2 then
                            grade = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_B"
                            resources:SetString(actorMap2, "$GRADE", grade)
                            break
                        elseif scratchValue == 3 then
                            grade = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_C"
                            resources:SetString(actorMap2, "$GRADE", grade)
                            break
                        elseif scratchValue == 4 then
                            grade = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_D"
                            resources:SetString(actorMap2, "$GRADE", grade)
                            break
                        elseif scratchValue == 5 then
                            grade = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_E"
                            resources:SetString(actorMap2, "$GRADE", grade)
                            break
                        elseif scratchValue == 6 then
                            grade = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_F"
                            resources:SetString(actorMap2, "$GRADE", grade)
                            break
                        else
                            goto FLOW_native_label_1
                        end
                    until true
                    ::FLOW_native_label_1::
                    resources:SetActor(actorMap3, "HERO", resource3)
                    resources:SetActor(actorMap3, "TEACHER", resource4)
                    resources:RunMacro("CS_GUILD_WILL_WON_START", actorMap3, false, false)
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacroWithStrings("CS_GUILD_WILL_WON", actorMap3, actorMap2, false, false)
                    quest:Pause(2.0)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        quest:NewScriptFrame(me)
                        if not quest:IsActiveThreadTerminating() then
                            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        else
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                    end
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d61578 end
                    if questionAnswer == 1 then
                        quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", true)
                        local meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
                        resource2 = resources:NewResource()
                        while not resources:TryAcquire(resource2, meleeApprentice, 4) do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(resource2)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(resource2)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        -- TODO(native): resources:SetActor(xStack_210, "HERO", &xStack_204_2)
                        resources:SetActor(actorMap3, "TEACHER", resource4)
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,(CCharString *)xStack_1a0);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,xStack_1f0);
                        resources:RunMacro("CS_GUILD_WILL_CONTINUE", actorMap3, false, true)
                        timerId3 = false
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true)
                        questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer2 < 0 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                            else
                                resources:ReleaseResource(resource2)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(resource2)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                        if questionAnswer2 == 1 then
                            if isActiveThreadTerminating then
                                resources:ReleaseResource(resource2)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            if quest:GetHealth(resources:ScriptThing(resource4)) <= 0.0 then return end  -- TODO(native): goto LAB_00d610d3
                            me:Speak(me, "WHISPER", 0x12d1148, false, false, true)
                            isTalkedToByHero = me:IsPerformingScriptTask()
                            goto LAB_00d61098
                        end
                        if isActiveThreadTerminating then
                            resources:ReleaseResource(resource2)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        if quest:GetHealth(resources:ScriptThing(resource4)) <= 0.0 then
                            -- TODO(native): xStack_234 = (CCharString)((uint)CVar5 & 0xffffff);
                        end
                        me:Speak(me, "CS_GUILD_WILL_WON", 0x12d1368, false, false, true)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(resource2)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(resource2)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        resources:ReleaseResource(resource2)
                    else
                        quest:SetHeroWillEnergyLevel(1.0)
                        resources:SetActor(actorMap3, "TEACHER", resource4)
                        resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap3, false, false)
                        timerId3 = true
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyStringMap(actorMap2)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                    quest:PauseAllNonScriptedEntities(true)
                    -- TODO(native): resources:SetActor(xStack_210, "HERO", &xStack_204_2)
                    resources:SetActor(actorMap3, "TEACHER", resource4)
                    resources:RunMacro("CS_GUILD_WILL_DISQUALIFIED", actorMap3, false, true)
                    quest:Pause(2.0)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
                    questionAnswer3 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer3 < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6101c end
                        questionAnswer3 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6101c end
                    if questionAnswer3 == 1 then
                        quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", true)
                        local meleeApprentice7 = quest:GetThingWithScriptName("MeleeApprentice")
                        resource = resources:NewResource()
                        while not resources:TryAcquire(resource, meleeApprentice7, 4) do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(resource)
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d6135b
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61004 end
                        -- TODO(native): resources:SetActor(xStack_210, "HERO", &xStack_204_2)
                        resources:SetActor(actorMap3, "TEACHER", resource4)
                        resources:SetActor(actorMap3, "WHISPER", resource)
                        resources:RunMacro("CS_GUILD_WILL_CONTINUE", actorMap3, false, true)
                        timerId3 = false
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true)
                        questionAnswer4 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer4 < 0 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                questionAnswer4 = quest:MsgIsQuestionAnsweredYesOrNo()
                            else
                                resources:ReleaseResource(resource)
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d6135b
                                questionAnswer4 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61004 end
                        if questionAnswer4 == 1 then
                            if quest:GetHealth(resources:ScriptThing(resource4)) <= 0.0 then
                                quest:FadeScreenOut(0.5, 0.5)
                                quest:Pause(1.0)
                                quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
                                quest:ConfiscateAllHeroWeapons()
                                if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d61004 end
                                    quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
                                end
                                quest:SetTimeOfDay(11.0)
                                quest:ResetPlayerCreatureCombatMultiplier()
                                quest:SetHeroAsTeenager(false)
                                quest:ChangeHeroHealthBy(1000.0, true, false)
                                quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
                                quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos(), "MeleeApprentice")
                                local willApprentice3 = quest:GetThingWithScriptName("WillApprentice")
                                timerId3 = willApprentice3 ~= nil and willApprentice3:IsAlive()
                                if not timerId3 then
                                    if not quest:IsActiveThreadTerminating() then
                                        timerId3 = false
                                        if quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")._0_4_ ~= nil then
                                            -- TODO(native): (**(code **)(*xStack_1b4._0_4_ + 0x118))(0);
                                        end
                                    else
                                        resources:ReleaseResource(resource)
                                        quest:PauseAllNonScriptedEntities(false)
                                        goto LAB_00d6135b
                                    end
                                end
                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
                                -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_238,(CCharString *)pCVar12);
                                quest:SetPlayerUsingWillDummies(false)
                                quest:SetMasterGameState("HeroTakingGuildTest", false)
                                quest:SetStateInt("TutorialState", 4)
                                quest:SetStateBool("TestFinished", true)
                                quest:DeactivateQuestLater(quest:GetActiveQuestName(), timerId3)
                                repeat
                                    quest:NewScriptFrame(me)
                                until quest:IsActiveThreadTerminating()
                                goto FLOW_after_lab_00d60be3
                            end
                            me:Speak(me, "TEXT_QST_LOG_HERO_ATTRIBUTES", 0x12d1148, false, false, true)
                            isTalkedToByHero = me:IsPerformingScriptTask()
                            goto LAB_00d60bad
                        end
                        if quest:GetHealth(resources:ScriptThing(resource4)) <= 0.0 then
                            -- TODO(native): xStack_234 = (CCharString)((uint)CVar5 & 0xffffff);
                        end
                        if not me:Speak(me, grade, 0x12d1368, false, false, true) then goto LAB_00d61004 end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(resource)
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d6135b
                        end
                        resources:ReleaseResource(resource)
                        quest:PauseAllNonScriptedEntities(false)
                    else
                        quest:SetHeroWillEnergyLevel(1.0)
                        resources:SetActor(actorMap3, "TEACHER", resource4)
                        resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap3, false, false)
                        timerId3 = true
                        quest:PauseAllNonScriptedEntities(false)
                    end
                end
                quest:FixMovieSequenceCamera(false)
                quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", false)
                resources:DestroyMovie(movie2)
                resources:DestroyActorMap(actorMap3)
                resources:ReleaseResource(resource3)
                quest:DeregisterTimer(timerId6)
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId8)
            until not timerId3
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetTargetingType(me, 58)
                quest:SetMasterGameState("HeroTakingGuildTest", false)
                quest:SetStateInt("TutorialState", 4)
                quest:SetStateBool("TestFinished", true)
                me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 1.0, ENTITY_MOVE_WALK, resource4 ~= 0, true)
                local willApprentice = quest:GetThingWithScriptName("WillApprentice")
                if not (willApprentice ~= nil and willApprentice:IsAlive()) then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                    local guildEvilApprenticeMale = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")
                    if guildEvilApprenticeMale ~= nil then
                        guildEvilApprenticeMale:SetToKillOnLevelUnload(0)
                    end
                end
                if quest:GetStateBool("BanditsDefeated") then
                    -- LAB_00d609de_c22: (native jump target)
                    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
                    quest:SetPlayerUsingWillDummies(false)
                    if not quest:IsActiveThreadTerminating() then
                        if not quest:GetStateBool("BanditsDefeated") then
                            if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then return end  -- TODO(native): goto LAB_00d6158a_c22
                            timerId3 = true
                        else
                            -- LAB_00d6158a_c22: (native jump target)
                            timerId3 = false
                        end
                        if timerId3 then
                            if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                            quest:SetStateBool("BanditsDefeated", true)
                        end
                        if not me:IsTalkedToByHero() then goto LAB_00d61af3 end
                        if not quest:IsActiveThreadTerminating() then
                            me:ClearCommands()
                            resource2 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            questionAnswer5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while questionAnswer5 < 0 do
                                if not quest:NewScriptFrame(me) then ReleaseEverything(); goto LAB_00d61b69 end
                                questionAnswer5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                            else
                                timerId3 = quest:IsActiveThreadTerminating()
                                if questionAnswer5 == 1 then
                                    if not timerId3 then
                                        quest:FadeScreenOut(0.5, 0.5)
                                        quest:Pause(1.0)
                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
                                        quest:ConfiscateAllHeroWeapons()
                                        if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); goto LAB_00d61b69 end
                                            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
                                        end
                                        quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                                        quest:SetTimeOfDay(11.0)
                                        quest:ResetPlayerCreatureCombatMultiplier()
                                        quest:SetHeroAsTeenager(false)
                                        quest:ChangeHeroHealthBy(1000.0, true, false)
                                        quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
                                        quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos(), "MeleeApprentice")
                                        ReleaseEverything()
                                        goto LAB_00d61af3
                                    end
                                elseif not timerId3 then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO", GROUP_SELECT_FIRST, false, false, 0.0 ~= 0)
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then ReleaseEverything(); goto LAB_00d61b69 end
                                        end
                                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); goto LAB_00d61b69 end
                                    end
                                    me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 1.0, ENTITY_MOVE_WALK, true)
                                    ReleaseEverything(); goto LAB_00d61af3
                                end
                                quest:PauseAllNonScriptedEntities(false)
                            end
                            resources:DestroyMovie(resource2)
                            goto LAB_00d61b69
                        end
                    end
                    ReleaseEverything2(); return
                end
                if not quest:IsActiveThreadTerminating() then
                    quest:ActivateQuest("Q_GuildTrainingWoodsWill")
                    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false)
                    quest:SetQuestAsPersistent("Q_GuildTrainingWoodsWill", false)
                    quest:SetQuestCardObjective("Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_01", "GuildWoods", "")
                    -- TODO(native): goto LAB_00d609de_c22
                end
                goto FLOW_after_lab_00d6070e
            end
        end
    else
        quest:EntitySetTargetingType(me, 58)
        quest:SetMasterGameState("HeroTakingGuildTest", false)
        quest:SetStateInt("TutorialState", 4)
        quest:SetStateBool("TestFinished", true)
        me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 1.0, ENTITY_MOVE_WALK, resource4 ~= 0, true)
        local willApprentice2 = quest:GetThingWithScriptName("WillApprentice")
        if not (willApprentice2 ~= nil and willApprentice2:IsAlive()) then
            if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
            local guildEvilApprenticeMale2 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")
            if guildEvilApprenticeMale2 ~= nil then
                guildEvilApprenticeMale2:SetToKillOnLevelUnload(0)
            end
        end
        if quest:GetStateBool("BanditsDefeated") then
            -- LAB_00d609de: (native jump target)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
            quest:SetPlayerUsingWillDummies(false)
            timerId3 = quest:IsActiveThreadTerminating()
            -- LAB_00d60a99: (native jump target)
            if timerId3 then quest:DeregisterTimer(timerId); quest:DeregisterTimer(timerId9); resources:ReleaseResource(resource4); return end
            if not quest:GetStateBool("BanditsDefeated") then
                if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                    timerId3 = false
                    goto FLOW_after_lab_00d6158a
                end
                timerId3 = true
            else
                -- LAB_00d6158a: (native jump target)
                timerId3 = false
            end
            ::FLOW_after_lab_00d6158a::
            if timerId3 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    quest:DeregisterTimer(timerId9)
                    resources:ReleaseResource(resource4)
                    return
                end
                quest:SetStateBool("BanditsDefeated", true)
            end
            if not me:IsTalkedToByHero() then goto LAB_00d61af3 end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); quest:DeregisterTimer(timerId9); resources:ReleaseResource(resource4); return end
            me:ClearCommands()
            resource2 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            questionAnswer6 = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer6 < 0 do
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then
                    questionAnswer6 = quest:MsgIsQuestionAnsweredYesOrNo()
                else
                    quest:PauseAllNonScriptedEntities(false)
                    goto FLOW_after_lab_00d61b53
                    questionAnswer6 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
            else
                timerId3 = quest:IsActiveThreadTerminating()
                if questionAnswer6 == 1 then
                    if not timerId3 then
                        quest:FadeScreenOut(0.5, 0.5)
                        quest:Pause(1.0)
                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
                        quest:ConfiscateAllHeroWeapons()
                        if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d61b53 end
                            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
                        end
                        quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                        quest:SetTimeOfDay(11.0)
                        quest:ResetPlayerCreatureCombatMultiplier()
                        quest:SetHeroAsTeenager(false)
                        quest:ChangeHeroHealthBy(1000.0, true, false)
                        quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
                        quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos(), "MeleeApprentice")
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(resource2)
                        goto LAB_00d61af3
                    end
                elseif not timerId3 then
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO", GROUP_SELECT_FIRST, false, false, 0.0 ~= 0)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d61b53 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61b53 end
                    end
                    me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 1.0, ENTITY_MOVE_WALK, true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(resource2)
                    goto LAB_00d61af3
                end
                ::LAB_00d61b53::
                quest:PauseAllNonScriptedEntities(false)
            end
            ::FLOW_after_lab_00d61b53::
            resources:DestroyMovie(resource2)
            goto LAB_00d61b69
            quest:DeregisterTimer(timerId)
            quest:DeregisterTimer(timerId9)
            resources:ReleaseResource(resource4)
            return
        end
        if not quest:IsActiveThreadTerminating() then
            quest:ActivateQuest("Q_GuildTrainingWoodsWill")
            quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false)
            quest:SetQuestAsPersistent("Q_GuildTrainingWoodsWill", false)
            quest:SetQuestCardObjective("Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_01", "GuildWoods", "")
            -- TODO(native): goto LAB_00d609de
        end
    end
    ::FLOW_after_lab_00d6070e::
    goto LAB_00d61b69
    ::LAB_00d61098::
    if not isTalkedToByHero then goto LAB_00d610c4 end
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource2)
        quest:PauseAllNonScriptedEntities(false)
        goto FLOW_after_lab_00d61578
    end
    isTalkedToByHero = me:IsPerformingScriptTask()
    goto LAB_00d61098
    ::LAB_00d610c4::
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource2)
        quest:PauseAllNonScriptedEntities(false)
    else
        -- LAB_00d610d3: (native jump target)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
        quest:ConfiscateAllHeroWeapons()
        if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource2)
                quest:PauseAllNonScriptedEntities(false)
                goto FLOW_after_lab_00d61578
            end
            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
        end
        quest:SetTimeOfDay(11.0)
        quest:ResetPlayerCreatureCombatMultiplier()
        quest:SetHeroAsTeenager(false)
        quest:ChangeHeroHealthBy(1000.0, true, false)
        quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
        quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos(), "MeleeApprentice")
        local willApprentice4 = quest:GetThingWithScriptName("WillApprentice")
        if willApprentice4 ~= nil and willApprentice4:IsAlive() then
            -- LAB_00d6144d: (native jump target)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
            -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_238,(CCharString *)pCVar12);
            quest:SetPlayerUsingWillDummies(false)
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            quest:SetStateInt("TutorialState", 4)
            quest:SetStateBool("TestFinished", true)
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 1)
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
            resources:ReleaseResource(resource2)
            quest:PauseAllNonScriptedEntities(false)
            goto FLOW_after_lab_00d61578
        end
        if not quest:IsActiveThreadTerminating() then
            if quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")._0_4_ ~= nil then
                -- TODO(native): (**(code **)(*xStack_1b4._0_4_ + 0x118))(0);
            end
            -- TODO(native): goto LAB_00d6144d
        end
        resources:ReleaseResource(resource2)
        quest:PauseAllNonScriptedEntities(false)
    end
    ::FLOW_after_lab_00d61578::
    resources:DestroyStringMap(actorMap2)
    goto LAB_00d6135b
    ::LAB_00d60bad::
    if not isTalkedToByHero then goto LAB_00d60bd4 end
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource)
        quest:PauseAllNonScriptedEntities(false)
        goto LAB_00d6135b
    end
    isTalkedToByHero = me:IsPerformingScriptTask()
    goto LAB_00d60bad
    ::LAB_00d61af3::
    quest:NewScriptFrame(me)
    -- TODO(native): goto LAB_00d60a99
    ::LAB_00d60bd4::
    if quest:IsActiveThreadTerminating() then goto FLOW_after_lab_00d60be3 end
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(1.0)
    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
    quest:ConfiscateAllHeroWeapons()
    if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
        if quest:IsActiveThreadTerminating() then goto LAB_00d61004 end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
    end
    quest:SetTimeOfDay(11.0)
    quest:ResetPlayerCreatureCombatMultiplier()
    quest:SetHeroAsTeenager(false)
    quest:ChangeHeroHealthBy(1000.0, true, false)
    quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
    quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos(), "MeleeApprentice")
    willApprentice5 = quest:GetThingWithScriptName("WillApprentice")
    timerId3 = willApprentice5 ~= nil and willApprentice5:IsAlive()
    if not timerId3 then
        if not quest:IsActiveThreadTerminating() then
            timerId3 = false
            if quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")._0_4_ ~= nil then
                -- TODO(native): (**(code **)(*xStack_1b4._0_4_ + 0x118))(0);
            end
        else
            resources:ReleaseResource(resource)
            quest:PauseAllNonScriptedEntities(false)
            goto LAB_00d6135b
        end
    end
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
    -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_238,(CCharString *)pCVar12);
    quest:SetPlayerUsingWillDummies(false)
    quest:SetMasterGameState("HeroTakingGuildTest", false)
    quest:SetStateInt("TutorialState", 4)
    quest:SetStateBool("TestFinished", true)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), timerId3)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    ::FLOW_after_lab_00d60be3::
    ::LAB_00d61004::
    resources:ReleaseResource(resource)
    ::LAB_00d6101c::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d6135b::
    resources:DestroyMovie(movie2)
    resources:DestroyActorMap(actorMap3)
    ::LAB_00d61370::
    resources:ReleaseResource(resource3)
    ::LAB_00d61379::
    quest:DeregisterTimer(timerId6)
    quest:DeregisterTimer(dist)
    ::LAB_00d6138b::
    quest:DeregisterTimer(timerId8)
    ::LAB_00d61b69::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId9)
    ::LAB_00d61b7b::
    resources:ReleaseResource(resource4)
end

-- TheRealGuildmaster.Init (retail 0x00d5de90)
function Init(quest, me)
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

