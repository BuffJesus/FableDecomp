-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_LIGHTNING_SPELL = 11  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_WillGrades = 3788,  -- '070000000000304100002041000000410000c040000040400000803f00000000'
    GUI_WillTimer = 3848,  -- 30.0
}

-- TheRealGuildmaster.Main (retail 0x00d5e0c0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue4, taskRunning, taskRunning4, c_stk_22d_1, c_stk_22d_2, ctr_22c, tutorialState
    local addQuestInfoCounter, scratchValue6, questionAnswer, questionAnswer2, questionAnswer3
    local questionAnswer4, questionAnswer5, scratchValue9, index, willApprentice, position
    local getActiveQuestName, meleeApprentice2, willApprentice2, meleeApprentice3, willApprentice3
    local grade, speechResult, speechResult5, resource, actorMap2, movie2, scriptThing, resource2
    local resource3, actorMap3, timerId7, timerId8, timerId9, actorMap4
    local resource4 = resources:NewResource()
    resources:PrepareResource(resource4)
    while not resources:TryAcquire(resource4, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource4)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource4); return end
    quest:EntitySetAsKillable(me, false, true)
    local hero13 = quest:GetHero()
    quest:EntitySetAlwaysBlockAttacksFromThing(me, hero13, true)
    quest:SetThingHasInformation(me, false, false, false)
    quest:SetPlayerUsingWillDummies(true)
    quest:SetIsPushableByHero(me, false)
    local timerId5 = quest:RegisterTimer()
    local timerId6 = quest:RegisterTimer()
    quest:SetTimer(timerId6, 0)
    ctr_22c = 0
    quest:EntitySetTargetingType(me, 26)
    if not quest:GetStateBool("TestFinished") then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId6)
            quest:DeregisterTimer(timerId5)
            resources:ReleaseResource(resource4)
            return
        end
        position = quest:GetThingWithScriptName("M_WillTeacherStand"):GetPos()
        me:MoveToPosition(position, 3.0, ENTITY_MOVE_WALK, false, true)
        while quest:GetStateInt("TutorialState") == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                if not quest:GetMasterGameState("HeroTakingGuildTest") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    quest:SetStateInt("TutorialState", 2)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    resource2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    me:ClearCommands()
                    local scratchValue = resources:ScriptThing(resource4)
                    if 0.0 < quest:GetHealth(scratchValue) then
                        grade = "TEXT_QST_028_GUILDMASTER_WILL_NOT_START"
                        local hero = quest:GetHero()
                        if not me:Speak(hero, grade, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(resource2)
                            quest:DeregisterTimer(timerId6)
                            quest:DeregisterTimer(timerId5)
                            resources:ReleaseResource(resource4)
                            return
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(resource2)
                                quest:DeregisterTimer(timerId6)
                                quest:DeregisterTimer(timerId5)
                                resources:ReleaseResource(resource4)
                                do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61b5a end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(resource2)
                end
            end
            local hero14 = quest:GetHero()
            if (quest:IsDistanceBetweenThingsUnder(hero14, me, 5.5) and quest:GetTimer(timerId6) < 1) and not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                local conversationId = quest:AddNewConversation(me, false, false)
                local hero15 = quest:GetHero()
                quest:AddPersonToConversation(conversationId, hero15)
                quest:SetTimer(timerId6, 10)
                if ctr_22c == 0 then
                    local hero16 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, hero16, false)
                    local hero17 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_FIRST", me, hero17, false)
                elseif ctr_22c == 1 then
                    local hero18 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, hero18, false)
                    local hero19 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_SECOND", me, hero19, false)
                end
                ctr_22c = 1 - ctr_22c
            end
        end
        if not quest:IsActiveThreadTerminating() then
            quest:SetMasterGameState("HeroTakingGuildTest", true)
            repeat
                if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
                quest:SetStateInt("TutorialState", 2)
                local resource5 = resources:NewResource()
                resources:PrepareResource(resource5)
                local hero2 = quest:GetHero()
                scratchValue4 = resources:TryAcquire(resource5, hero2, 4)
                while not scratchValue4 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d60aee end
                    local hero3 = quest:GetHero()
                    scratchValue4 = resources:TryAcquire(resource5, hero3, 4)
                end
                if not (quest:IsActiveThreadTerminating()) then
                    goto FLOW_past_lab_00d60aee
                end
                ::LAB_00d60aee::
                resources:ReleaseResource(resource5)
                goto LAB_00d61b69
                ::FLOW_past_lab_00d60aee::
                actorMap4 = resources:NewActorMap()
                resources:SetActor(actorMap4, "HERO", resource5)
                resources:SetActor(actorMap4, "TEACHER", resource4)
                quest:GiveHeroAbility(HERO_ABILITY_LIGHTNING_SPELL, true)
                quest:SetMasterGameState("WillTrainingStarted", true)
                local movie3 = resources:StartMovie("")
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
                    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_USINGSPELLS")
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    quest:AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_USINGSPELLS")
                end
                quest:SetMasterGameState("WillScore", 0)
                timerId9 = quest:RegisterTimer()
                quest:SetTimer(timerId9, 0)
                tutorialState = quest:GetStateInt("TutorialState")
                c_stk_22d_1 = 0
                while tutorialState == 2 do
                    quest:NewScriptFrame(me)
                    local timerId = timerId9
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    while c_stk_22d_1 == 0 and quest:GetMasterGameState("WillScore") == 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        if me:IsTalkedToByHero() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                            c_stk_22d_1 = 1
                        end
                        if quest:GetTimer(timerId) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                            local hero4 = quest:GetHero()
                            quest:EntitySetFacingAngleTowardsThing(me, hero4, false)
                            quest:SetTimer(timerId, 2)
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    if quest:GetMasterGameState("WillScore") ~= 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                        quest:SetStateInt("TutorialState", 3)
                        local resource6 = resources:NewResource()
                        resources:PrepareResource(resource6)
                        local hero5 = quest:GetHero()
                        scratchValue4 = resources:TryAcquire(resource6, hero5, 4)
                        while not scratchValue4 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d60aff end
                            local hero6 = quest:GetHero()
                            scratchValue4 = resources:TryAcquire(resource6, hero6, 4)
                        end
                        if not (quest:IsActiveThreadTerminating()) then
                            goto FLOW_past_lab_00d60aff
                        end
                        ::LAB_00d60aff::
                        resources:ReleaseResource(resource6)
                        quest:DeregisterTimer(timerId9)
                        goto LAB_00d61b69
                        ::FLOW_past_lab_00d60aff::
                        local actorMap = resources:NewActorMap()
                        resources:SetActor(actorMap, "HERO", resource6)
                        resources:SetActor(actorMap, "TEACHER", resource4)
                        local movie = resources:StartMovie("")
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_WILL_TEST", actorMap, false, true)
                        quest:FixMovieSequenceCamera(false)
                        resources:DestroyMovie(movie)
                        resources:DestroyActorMap(actorMap)
                        resources:ReleaseResource(resource6)
                    end
                    if c_stk_22d_1 == 0 then
                        tutorialState = quest:GetStateInt("TutorialState")
                    else
                        scratchValue4 = quest:IsActiveThreadTerminating()
                        if scratchValue4 then goto LAB_00d6138b end
                        c_stk_22d_1 = scratchValue4 and 1 or 0
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
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                if quest:IsXbox() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                while not quest:MsgOnHeroCastSpell() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                        if quest:IsXbox() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                            end
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                timerId8 = quest:RegisterTimer()
                local scratchValue7 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_WillTimer)))
                quest:SetTimer(timerId8, scratchValue7)
                quest:SetStateInt("TutorialState", 3)
                quest:SetMasterGameState("WillScore", 0)
                timerId7 = quest:RegisterTimer()
                quest:SetTimer(ctr_22c, 0)
                addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                local infoCounter = addQuestInfoCounter
                local infoElement = quest:AddQuestInfoTimer(timerId8, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                c_stk_22d_2 = 0
                quest:SetTimer(timerId9, 0)
                while 0 < quest:GetTimer(timerId8) and c_stk_22d_2 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                    if quest:GetTimer(timerId9) < 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        local hero7 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, hero7, false)
                        quest:SetTimer(timerId9, 2)
                    end
                    quest:SetMasterGameState("WillTestOccuring", true)
                    if quest:GetMasterGameState("GuildWarningOccuring") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        c_stk_22d_2 = 1
                    end
                    if quest:GetHeroWillEnergy() == 0 and quest:GetTimer(timerId6) < 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        local conversationId2 = quest:AddNewConversation(me, false, false)
                        local hero8 = quest:GetHero()
                        quest:AddPersonToConversation(conversationId2, hero8)
                        local hero9 = quest:GetHero()
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_GUILDMASTER_WILL_NO_WILL", me, hero9, false)
                        quest:SetTimer(ctr_22c, 8)
                        addQuestInfoCounter = infoCounter
                    end
                    if not me:IsTalkedToByHero() then
                        quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetMasterGameState("WillScore"), -1)
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        if quest:IsXbox() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                            end
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetMasterGameState("WillScore"), -1)
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                quest:SetMasterGameState("WillTestOccuring", false)
                while not quest:IsHeroControlledByPlayer() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                quest:DisplayQuestInfo(false)
                quest:RemoveQuestInfoElement(addQuestInfoCounter)
                quest:RemoveQuestInfoElement(infoElement)
                quest:SetStateInt("TutorialState", 0)
                resource3 = resources:NewResource()
                resources:PrepareResource(resource3)
                local hero10 = quest:GetHero()
                scratchValue4 = resources:TryAcquire(resource3, hero10, 4)
                while not scratchValue4 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61370 end
                    local hero11 = quest:GetHero()
                    scratchValue4 = resources:TryAcquire(resource3, hero11, 4)
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d61370 end
                actorMap3 = resources:NewActorMap()
                movie2 = resources:StartMovie("")
                quest:FixMovieSequenceCamera(true)
                if c_stk_22d_2 == 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                    local getMasterGameState = quest:GetMasterGameState("WillScore")
                    index = 0
                    scratchValue6 = 0
                    repeat
                        scratchValue9 = scratchValue6
                        if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_WillGrades, index) < getMasterGameState ~= (quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_WillGrades, index) == getMasterGameState) then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                            break
                        end
                        scratchValue6 = scratchValue9 + 1
                        index = index + 1
                    until not (scratchValue6 < 7)
                    actorMap2 = resources:NewStringMap()
                    local switch = scratchValue9
                    repeat
                        if switch == 0 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_APLUS")
                            break
                        elseif switch == 1 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_A")
                            break
                        elseif switch == 2 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_B")
                            break
                        elseif switch == 3 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_C")
                            break
                        elseif switch == 4 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_D")
                            break
                        elseif switch == 5 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_E")
                            break
                        elseif switch == 6 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_F")
                            break
                        else
                            break
                        end
                    until true
                    resources:SetActor(actorMap3, "HERO", resource3)
                    resources:SetActor(actorMap3, "TEACHER", resource4)
                    resources:RunMacro("CS_GUILD_WILL_WON_START", actorMap3, false, false)
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacroWithStrings("CS_GUILD_WILL_WON", actorMap3, actorMap2, false, false)
                    quest:Pause(2.0)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d61578 end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61342 end
                    if questionAnswer == 1 then
                        quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", true)
                        local meleeApprentice4 = quest:GetThingWithScriptName("MeleeApprentice")
                        resource2 = resources:NewResource()
                        resources:PrepareResource(resource2)
                        while not resources:TryAcquire(resource2, meleeApprentice4, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00d6132d end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61563 end
                        resources:SetActor(actorMap3, "HERO", resource3)
                        resources:SetActor(actorMap3, "TEACHER", resource4)
                        resources:SetActor(actorMap3, "WHISPER", resource2)
                        resources:RunMacro("CS_GUILD_WILL_CONTINUE", actorMap3, false, true)
                        scratchValue4 = false
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true)
                        questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer2 < 0 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d6132d end
                            questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61563 end
                        if questionAnswer2 == 1 then
                            scriptThing = resources:ScriptThing(resource4)
                            if quest:GetHealth(scriptThing) <= 0.0 then goto LAB_00d610d3 end
                            speechResult = me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false)
                            taskRunning = me:IsPerformingScriptTask()
                            goto LAB_00d61098
                        end
                        local scratchValue15 = resources:ScriptThing(resource4)
                        if 1 ~= 0 and not (quest:GetHealth(scratchValue15) <= 0.0) then
                            if not me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false) then
                                goto LAB_00d61563
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d61563 end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6132d end
                        end
                        resources:PrepareResource(resource2)
                        resources:ReleaseResource(resource2)
                    else
                        quest:SetHeroWillEnergyLevel(1.0)
                        resources:SetActor(actorMap3, "TEACHER", resource4)
                        resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap3, false, false)
                        scratchValue4 = true
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyStringMap(actorMap2)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                    quest:PauseAllNonScriptedEntities(true)
                    resources:SetActor(actorMap3, "HERO", resource3)
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
                        local meleeApprentice5 = quest:GetThingWithScriptName("MeleeApprentice")
                        resource = resources:NewResource()
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, meleeApprentice5, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00d60b19 end
                        end
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d60b19 end
                        resources:SetActor(actorMap3, "HERO", resource3)
                        resources:SetActor(actorMap3, "TEACHER", resource4)
                        resources:SetActor(actorMap3, "WHISPER", resource)
                        resources:RunMacro("CS_GUILD_WILL_CONTINUE", actorMap3, false, true)
                        scratchValue4 = false
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true)
                        questionAnswer4 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer4 < 0 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d60b19 end
                            questionAnswer4 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d60b19 end
                        if questionAnswer4 == 1 then
                            scriptThing = resources:ScriptThing(resource4)
                            if quest:GetHealth(scriptThing) <= 0.0 then goto LAB_00d60be3 end
                            speechResult5 = me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false)
                            taskRunning4 = me:IsPerformingScriptTask()
                            goto LAB_00d60bad
                        end
                        local scratchValue17 = resources:ScriptThing(resource4)
                        if 1 ~= 0 and not (quest:GetHealth(scratchValue17) <= 0.0) then
                            if not me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false) then
                                goto FLOW_past_lab_00d60b19
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00d60b19 end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d60b19 end
                        end
                        resources:PrepareResource(resource)
                        resources:ReleaseResource(resource)
                        quest:PauseAllNonScriptedEntities(false)
                    else
                        quest:SetHeroWillEnergyLevel(1.0)
                        resources:SetActor(actorMap3, "TEACHER", resource4)
                        resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap3, false, false)
                        scratchValue4 = true
                        quest:PauseAllNonScriptedEntities(false)
                    end
                end
                quest:FixMovieSequenceCamera(false)
                quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", false)
                resources:DestroyMovie(movie2)
                resources:DestroyActorMap(actorMap3)
                resources:ReleaseResource(resource3)
                quest:DeregisterTimer(timerId7)
                quest:DeregisterTimer(timerId8)
                quest:DeregisterTimer(timerId9)
            until not scratchValue4
            if not quest:IsActiveThreadTerminating() then goto LAB_00d6070e end
        end
    else
        goto LAB_00d6070e
    end
    goto FLOW_past_lab_00d6070e
    ::LAB_00d6070e::
    quest:EntitySetTargetingType(me, 58)
    quest:SetMasterGameState("HeroTakingGuildTest", false)
    quest:SetStateInt("TutorialState", 4)
    quest:SetStateBool("TestFinished", true)
    position = quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos()
    me:MoveToPosition(position, 1.0, ENTITY_MOVE_WALK, false, true)
    willApprentice = quest:GetThingWithScriptName("WillApprentice")
    if not (willApprentice ~= nil and willApprentice:IsAlive()) then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId6)
            quest:DeregisterTimer(timerId5)
            resources:ReleaseResource(resource4)
            return
        end
        position = quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos()
        local guildEvilApprenticeMale3 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", position, "WillApprentice", false)
        if guildEvilApprenticeMale3 ~= nil then
            guildEvilApprenticeMale3:SetToKillOnLevelUnload(false)
        end
    end
    if not (quest:GetStateBool("BanditsDefeated")) then
        goto FLOW_past_lab_00d609de
    end
    ::LAB_00d609de::
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_HERO_ATTRIBUTES")
    quest:SetPlayerUsingWillDummies(false)
    scratchValue4 = quest:IsActiveThreadTerminating()
    ::LAB_00d60a99::
    if not scratchValue4 then
        if not (quest:GetStateBool("BanditsDefeated") or quest:IsQuestActive("Q_GuildTrainingWoodsWill")) then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId6)
                quest:DeregisterTimer(timerId5)
                resources:ReleaseResource(resource4)
                return
            end
            quest:SetStateBool("BanditsDefeated", true)
        end
        if not me:IsTalkedToByHero() then goto LAB_00d61af3 end
        if not quest:IsActiveThreadTerminating() then
            me:ClearCommands()
            resource2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            questionAnswer5 = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer5 < 0 do
                if not quest:NewScriptFrame(me) then goto LAB_00d61b53 end
                questionAnswer5 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then
                goto LAB_00d61b44
            else
                scratchValue4 = quest:IsActiveThreadTerminating()
                if questionAnswer5 == 1 then
                    if not scratchValue4 then
                        quest:FadeScreenOut(0.5, 0.5)
                        quest:Pause(1.0)
                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
                        quest:ConfiscateAllHeroWeapons()
                        if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d61b44 end
                            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
                        end
                        getActiveQuestName = quest:GetActiveQuestName()
                        quest:DeactivateQuestLater(getActiveQuestName, 0)
                        quest:SetTimeOfDay(11.0)
                        quest:ResetPlayerCreatureCombatMultiplier()
                        quest:SetHeroAsTeenager(false)
                        quest:ChangeHeroHealthBy(1000.0, true, false)
                        local meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
                        quest:RemoveThing(meleeApprentice, false, true)
                        position = quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos()
                        quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", position, "MeleeApprentice", false)
                        goto LAB_00d61ad8
                    end
                elseif not scratchValue4 then
                    actorMap4 = resources:ScriptThing(resource4)
                    if 0.0 < quest:GetHealth(actorMap4) then
                        grade = "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO"
                        local hero12 = quest:GetHero()
                        if not me:Speak(hero12, grade, 0, false, true, false) then
                            goto LAB_00d61b44
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d61b44 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61b53 end
                    end
                    local guildmasterMarker = quest:GetThingWithScriptName("M_GuildmasterMarker")
                    position = guildmasterMarker:GetPos()
                    me:MoveToPosition(position, 1.0, ENTITY_MOVE_WALK, false, true)
                    goto LAB_00d61ad8
                end
                goto FLOW_past_lab_00d61ad8
                ::LAB_00d61ad8::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(resource2)
                goto LAB_00d61af3
                ::FLOW_past_lab_00d61ad8::
                goto LAB_00d61b53
            end
            goto FLOW_past_lab_00d61b44
            ::LAB_00d61b44::
            quest:PauseAllNonScriptedEntities(false)
            ::FLOW_past_lab_00d61b44::
            goto FLOW_past_lab_00d61b53
            ::LAB_00d61b53::
            goto LAB_00d61b5a
            ::FLOW_past_lab_00d61b53::
            goto FLOW_hoist_lab_00d61b5a_1
        end
    end
    goto FLOW_hoist_lab_00d61b5a_3
    ::FLOW_past_lab_00d609de::
    goto FLOW_hoist_lab_00d61b5a_4
    ::FLOW_past_lab_00d6070e::
    goto FLOW_past_lab_00d61b5a
    ::LAB_00d61b5a::
    quest:PauseAllNonScriptedEntities(false)
    ::FLOW_hoist_lab_00d61b5a_1::
    resources:DestroyMovie(resource2)
    goto LAB_00d61b69
    ::FLOW_hoist_lab_00d61b5a_3::
    quest:DeregisterTimer(timerId6)
    quest:DeregisterTimer(timerId5)
    resources:ReleaseResource(resource4)
    do return end
    ::FLOW_hoist_lab_00d61b5a_4::
    if not quest:IsActiveThreadTerminating() then
        quest:ActivateQuest("Q_GuildTrainingWoodsWill")
        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BANDITS", "Q_GuildTrainingWoodsWill", false)
        quest:SetQuestAsPersistent("Q_GuildTrainingWoodsWill", false)
        quest:SetQuestCardObjective("Q_GuildTrainingWoodsWill", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06_OPTION_01", "GuildWoods", "")
        goto LAB_00d609de
    end
    ::FLOW_past_lab_00d61b5a::
    goto LAB_00d61b69
    ::LAB_00d61098::
    if taskRunning then
        if not quest:NewScriptFrame(me) then goto LAB_00d6132d end
        taskRunning = me:IsPerformingScriptTask()
        goto LAB_00d61098
    end
    if not (not quest:IsActiveThreadTerminating()) then
        goto LAB_00d61563
        goto FLOW_past_lab_00d610d3
    end
    ::LAB_00d610d3::
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(1.0)
    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
    quest:ConfiscateAllHeroWeapons()
    if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
        if quest:IsActiveThreadTerminating() then goto LAB_00d61563 end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
    end
    quest:SetTimeOfDay(11.0)
    quest:ResetPlayerCreatureCombatMultiplier()
    quest:SetHeroAsTeenager(false)
    quest:ChangeHeroHealthBy(1000.0, true, false)
    meleeApprentice2 = quest:GetThingWithScriptName("MeleeApprentice")
    quest:RemoveThing(meleeApprentice2, false, true)
    position = quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos()
    quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", position, "MeleeApprentice", false)
    willApprentice2 = quest:GetThingWithScriptName("WillApprentice")
    if not (willApprentice2 ~= nil and willApprentice2:IsAlive()) then
        goto FLOW_past_lab_00d6144d
    end
    ::LAB_00d6144d::
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_HERO_ATTRIBUTES")
    quest:SetPlayerUsingWillDummies(false)
    quest:SetMasterGameState("HeroTakingGuildTest", false)
    quest:SetStateInt("TutorialState", 4)
    quest:SetStateBool("TestFinished", true)
    getActiveQuestName = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName, 0)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    goto LAB_00d61563
    ::FLOW_past_lab_00d6144d::
    if not quest:IsActiveThreadTerminating() then
        position = quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos()
        local guildEvilApprenticeMale = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", position, "WillApprentice", false)
        if guildEvilApprenticeMale ~= nil then
            guildEvilApprenticeMale:SetToKillOnLevelUnload(false)
        end
        goto LAB_00d6144d
    end
    goto LAB_00d6132d
    ::FLOW_past_lab_00d610d3::
    goto FLOW_past_lab_00d61563
    ::LAB_00d61563::
    resources:ReleaseResource(resource2)
    goto LAB_00d61578
    ::FLOW_past_lab_00d61563::
    goto FLOW_past_lab_00d6132d
    ::LAB_00d6132d::
    resources:ReleaseResource(resource2)
    goto LAB_00d61342
    ::FLOW_past_lab_00d6132d::
    goto FLOW_past_lab_00d61342
    ::LAB_00d61342::
    quest:PauseAllNonScriptedEntities(false)
    ::FLOW_past_lab_00d61342::
    goto FLOW_past_lab_00d61578
    ::LAB_00d61578::
    quest:PauseAllNonScriptedEntities(false)
    ::FLOW_past_lab_00d61578::
    resources:DestroyStringMap(actorMap2)
    goto LAB_00d6135b
    ::LAB_00d60bad::
    if not taskRunning4 then goto LAB_00d60bd4 end
    if not quest:NewScriptFrame(me) then goto LAB_00d60b19 end
    taskRunning4 = me:IsPerformingScriptTask()
    goto LAB_00d60bad
    ::LAB_00d61af3::
    quest:NewScriptFrame(me)
    scratchValue4 = quest:IsActiveThreadTerminating()
    goto LAB_00d60a99
    ::LAB_00d60bd4::
    if not (not quest:IsActiveThreadTerminating()) then
        goto FLOW_past_lab_00d60be3
    end
    ::LAB_00d60be3::
    quest:FadeScreenOut(0.5, 0.5)
    quest:Pause(1.0)
    quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
    quest:ConfiscateAllHeroWeapons()
    if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d60b19 end
        quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
    end
    quest:SetTimeOfDay(11.0)
    quest:ResetPlayerCreatureCombatMultiplier()
    quest:SetHeroAsTeenager(false)
    quest:ChangeHeroHealthBy(1000.0, true, false)
    meleeApprentice3 = quest:GetThingWithScriptName("MeleeApprentice")
    quest:RemoveThing(meleeApprentice3, false, true)
    position = quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos()
    quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", position, "MeleeApprentice", false)
    willApprentice3 = quest:GetThingWithScriptName("WillApprentice")
    if not (willApprentice3 ~= nil and willApprentice3:IsAlive()) then
        if not quest:IsActiveThreadTerminating() then
            position = quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos()
            local guildEvilApprenticeMale2 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", position, "WillApprentice", false)
            if guildEvilApprenticeMale2 ~= nil then
                guildEvilApprenticeMale2:SetToKillOnLevelUnload(false)
            end
            goto FLOW_hoist_lab_00d60b19_1
        end
        goto LAB_00d60b19
    end
    goto FLOW_hoist_lab_00d60b19_1
    ::FLOW_past_lab_00d60be3::
    goto FLOW_past_lab_00d60b19
    ::LAB_00d60b19::
    resources:ReleaseResource(resource)
    quest:PauseAllNonScriptedEntities(false)
    goto LAB_00d6135b
    ::FLOW_hoist_lab_00d60b19_1::
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_HERO_ATTRIBUTES")
    quest:SetPlayerUsingWillDummies(false)
    quest:SetMasterGameState("HeroTakingGuildTest", false)
    quest:SetStateInt("TutorialState", 4)
    quest:SetStateBool("TestFinished", true)
    getActiveQuestName = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(getActiveQuestName, 0)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    ::FLOW_past_lab_00d60b19::
    resources:ReleaseResource(resource)
    ::LAB_00d6101c::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d6135b::
    resources:DestroyMovie(movie2)
    resources:DestroyActorMap(actorMap3)
    ::LAB_00d61370::
    resources:ReleaseResource(resource3)
    ::LAB_00d61379::
    quest:DeregisterTimer(timerId7)
    quest:DeregisterTimer(timerId8)
    ::LAB_00d6138b::
    quest:DeregisterTimer(timerId9)
    ::LAB_00d61b69::
    quest:DeregisterTimer(timerId6)
    quest:DeregisterTimer(timerId5)
    resources:ReleaseResource(resource4)
end

-- TheRealGuildmaster.Init (retail 0x00d5de90)
function Init(quest, me)
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

