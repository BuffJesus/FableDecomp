-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

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
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isActiveThreadTerminating, taskRunning, taskRunning4, c_stk_22d_1, c_stk_22d_2, ctr_22c
    local tutorialState, scratchValue6, questionAnswer, questionAnswer2, questionAnswer3
    local questionAnswer4, questionAnswer5, scratchValue9, index, willApprentice, willApprentice2
    local willApprentice3, resource, actorMap2, movie2, resource2, resource3, actorMap3, timerId7
    local timerId8, timerId9
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
    quest:EntitySetAlwaysBlockAttacksFromThing(me, hero, true)
    quest:SetThingHasInformation(me, false, false, false)
    quest:SetPlayerUsingWillDummies(true)
    quest:SetIsPushableByHero(me, false)
    local timerId = quest:RegisterTimer()
    local timerId6 = quest:RegisterTimer()
    quest:SetTimer(timerId6, 0)
    ctr_22c = 0
    quest:EntitySetTargetingType(me, 26)
    if not quest:GetStateBool("TestFinished") then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId6)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource4)
            return
        end
        me:MoveToPosition(quest:GetThingWithScriptName("M_WillTeacherStand"):GetPos(), 3.0, ENTITY_MOVE_WALK, false, true)
        while quest:GetStateInt("TutorialState") == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
            if me:IsTalkedToByHero() then
                if not quest:GetMasterGameState("HeroTakingGuildTest") then
                    quest:SetStateInt("TutorialState", 2)
                else
                    resource2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    me:ClearCommands()
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_WILL_NOT_START", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(resource2)
                                quest:DeregisterTimer(timerId6)
                                quest:DeregisterTimer(timerId)
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
            if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId6) < 1) and not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                local conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, hero)
                quest:SetTimer(timerId6, 10)
                if ctr_22c == 0 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_FIRST", me, hero, false)
                elseif ctr_22c == 1 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_SECOND", me, hero, false)
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
                while not resources:TryAcquire(resource5, hero, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00d60aee end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d60aee end
                goto FLOW_past_lab_00d60aee
                ::LAB_00d60aee::
                resources:ReleaseResource(resource5)
                goto LAB_00d61b69
                ::FLOW_past_lab_00d60aee::
                local actorMap4 = resources:NewActorMap()
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
                    if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                    while c_stk_22d_1 == 0 and quest:GetMasterGameState("WillScore") == 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        if me:IsTalkedToByHero() then
                            c_stk_22d_1 = 1
                        end
                        if quest:GetTimer(timerId9) < 1 then
                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                            quest:SetTimer(timerId9, 2)
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    if quest:GetMasterGameState("WillScore") ~= 0 then
                        quest:SetStateInt("TutorialState", 3)
                        local resource6 = resources:NewResource()
                        resources:PrepareResource(resource6)
                        while not resources:TryAcquire(resource6, hero, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00d60aff end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d60aff end
                        goto FLOW_past_lab_00d60aff
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
                        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                        if isActiveThreadTerminating then goto LAB_00d6138b end
                        c_stk_22d_1 = isActiveThreadTerminating
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
                    if me:IsTalkedToByHero() then
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
                    end
                end
                timerId8 = quest:RegisterTimer()
                quest:SetTimer(timerId8, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_WillTimer))))
                quest:SetStateInt("TutorialState", 3)
                quest:SetMasterGameState("WillScore", 0)
                timerId7 = quest:RegisterTimer()
                quest:SetTimer(ctr_22c, 0)
                local addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                local infoElement = quest:AddQuestInfoTimer(timerId8, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                c_stk_22d_2 = 0
                quest:SetTimer(timerId9, 0)
                while 0 < quest:GetTimer(timerId8) and c_stk_22d_2 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                    if quest:GetTimer(timerId9) < 1 then
                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
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
                        quest:AddPersonToConversation(conversationId2, hero)
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_GUILDMASTER_WILL_NO_WILL", me, hero, false)
                        quest:SetTimer(ctr_22c, 8)
                    end
                    if not me:IsTalkedToByHero() then
                        quest:UpdateQuestInfoCounter(addQuestInfoCounter, quest:GetMasterGameState("WillScore"), -1)
                    else
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
                    end
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
                resources:PrepareResource(resource3)
                while not resources:TryAcquire(resource3, hero, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61370 end
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
                    until scratchValue6 >= 7
                    actorMap2 = resources:NewStringMap()
                    repeat
                        if scratchValue9 == 0 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_APLUS")
                            break
                        elseif scratchValue9 == 1 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_A")
                            break
                        elseif scratchValue9 == 2 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_B")
                            break
                        elseif scratchValue9 == 3 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_C")
                            break
                        elseif scratchValue9 == 4 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_D")
                            break
                        elseif scratchValue9 == 5 then
                            resources:SetString(actorMap2, "$GRADE", "TEXT_QST_028_GUILDMASTER_WILL_GRADE_E")
                            break
                        elseif scratchValue9 == 6 then
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
                        local meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
                        resource2 = resources:NewResource()
                        resources:PrepareResource(resource2)
                        while not resources:TryAcquire(resource2, meleeApprentice, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00d6132d end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61563 end
                        resources:SetActor(actorMap3, "HERO", resource3)
                        resources:SetActor(actorMap3, "TEACHER", resource4)
                        resources:SetActor(actorMap3, "WHISPER", resource2)
                        resources:RunMacro("CS_GUILD_WILL_CONTINUE", actorMap3, false, true)
                        isActiveThreadTerminating = false
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true)
                        questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer2 < 0 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d6132d end
                            questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61563 end
                        if questionAnswer2 == 1 then
                            if quest:GetHealth(resources:ScriptThing(resource4)) <= 0.0 then goto LAB_00d610d3 end
                            me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false)
                            taskRunning = me:IsPerformingScriptTask()
                            goto LAB_00d61098
                        end
                        if quest:GetHealth(resources:ScriptThing(resource4)) > 0.0 then
                            if not me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d61563 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6132d end
                        end
                        resources:PrepareResource(resource2)
                        resources:ReleaseResource(resource2)
                    else
                        quest:SetHeroWillEnergyLevel(1.0)
                        resources:SetActor(actorMap3, "TEACHER", resource4)
                        resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap3, false, false)
                        isActiveThreadTerminating = true
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
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61004 end
                        resources:SetActor(actorMap3, "HERO", resource3)
                        resources:SetActor(actorMap3, "TEACHER", resource4)
                        resources:SetActor(actorMap3, "WHISPER", resource)
                        resources:RunMacro("CS_GUILD_WILL_CONTINUE", actorMap3, false, true)
                        isActiveThreadTerminating = false
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true)
                        questionAnswer4 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer4 < 0 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d60b19 end
                            questionAnswer4 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61004 end
                        if questionAnswer4 == 1 then
                            if quest:GetHealth(resources:ScriptThing(resource4)) <= 0.0 then goto LAB_00d60be3 end
                            me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false)
                            taskRunning4 = me:IsPerformingScriptTask()
                            goto LAB_00d60bad
                        end
                        if quest:GetHealth(resources:ScriptThing(resource4)) > 0.0 then
                            if not me:Speak(me, "TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d61004 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d60b19 end
                        end
                        resources:PrepareResource(resource)
                        resources:ReleaseResource(resource)
                        quest:PauseAllNonScriptedEntities(false)
                    else
                        quest:SetHeroWillEnergyLevel(1.0)
                        resources:SetActor(actorMap3, "TEACHER", resource4)
                        resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap3, false, false)
                        isActiveThreadTerminating = true
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
            until not isActiveThreadTerminating
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
    me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 1.0, ENTITY_MOVE_WALK, false, true)
    willApprentice = quest:GetThingWithScriptName("WillApprentice")
    if not (willApprentice ~= nil and willApprentice:IsAlive()) then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId6)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource4)
            return
        end
        local guildEvilApprenticeMale3 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")
        if guildEvilApprenticeMale3 ~= nil then
            guildEvilApprenticeMale3:SetToKillOnLevelUnload(false)
        end
    end
    if quest:GetStateBool("BanditsDefeated") then
        goto LAB_00d609de
    end
    goto FLOW_past_lab_00d609de
    ::LAB_00d609de::
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_HERO_ATTRIBUTES")
    quest:SetPlayerUsingWillDummies(false)
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    ::LAB_00d60a99::
    if not isActiveThreadTerminating then
        if not (quest:GetStateBool("BanditsDefeated") or quest:IsQuestActive("Q_GuildTrainingWoodsWill")) then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId6)
                quest:DeregisterTimer(timerId)
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
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if questionAnswer5 == 1 then
                    if not isActiveThreadTerminating then
                        quest:FadeScreenOut(0.5, 0.5)
                        quest:Pause(1.0)
                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
                        quest:ConfiscateAllHeroWeapons()
                        if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d61b44 end
                            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
                        end
                        quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                        quest:SetTimeOfDay(11.0)
                        quest:ResetPlayerCreatureCombatMultiplier()
                        quest:SetHeroAsTeenager(false)
                        quest:ChangeHeroHealthBy(1000.0, true, false)
                        quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
                        quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos(), "MeleeApprentice")
                        goto LAB_00d61ad8
                    end
                elseif not isActiveThreadTerminating then
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                        if not me:Speak(hero, "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d61b44 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61b53 end
                    end
                    local guildmasterMarker = quest:GetThingWithScriptName("M_GuildmasterMarker")
                    me:MoveToPosition(guildmasterMarker:GetPos(), 1.0, ENTITY_MOVE_WALK, false, true)
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
    quest:DeregisterTimer(timerId)
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
    if not quest:IsActiveThreadTerminating() then goto LAB_00d610d3 end
    goto LAB_00d61563
    goto FLOW_past_lab_00d610d3
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
    quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
    quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos(), "MeleeApprentice")
    willApprentice2 = quest:GetThingWithScriptName("WillApprentice")
    if willApprentice2 ~= nil and willApprentice2:IsAlive() then
        goto LAB_00d6144d
    end
    goto FLOW_past_lab_00d6144d
    ::LAB_00d6144d::
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_HERO_ATTRIBUTES")
    quest:SetPlayerUsingWillDummies(false)
    quest:SetMasterGameState("HeroTakingGuildTest", false)
    quest:SetStateInt("TutorialState", 4)
    quest:SetStateBool("TestFinished", true)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    goto LAB_00d61563
    ::FLOW_past_lab_00d6144d::
    if not quest:IsActiveThreadTerminating() then
        local guildEvilApprenticeMale = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")
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
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    goto LAB_00d60a99
    ::LAB_00d60bd4::
    if not quest:IsActiveThreadTerminating() then goto LAB_00d60be3 end
    goto FLOW_past_lab_00d60be3
    ::LAB_00d60be3::
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
    willApprentice3 = quest:GetThingWithScriptName("WillApprentice")
    if not (willApprentice3 ~= nil and willApprentice3:IsAlive()) then
        if not quest:IsActiveThreadTerminating() then
            local guildEvilApprenticeMale2 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")
            if guildEvilApprenticeMale2 ~= nil then
                guildEvilApprenticeMale2:SetToKillOnLevelUnload(false)
            end
            goto LAB_00d60ef5
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
    ::LAB_00d60ef5::
    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
    quest:AddLogbookTutorialEntry("TEXT_QST_LOG_HERO_ATTRIBUTES")
    quest:SetPlayerUsingWillDummies(false)
    quest:SetMasterGameState("HeroTakingGuildTest", false)
    quest:SetStateInt("TutorialState", 4)
    quest:SetStateBool("TestFinished", true)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    ::FLOW_past_lab_00d60b19::
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
    quest:DeregisterTimer(timerId7)
    quest:DeregisterTimer(timerId8)
    ::LAB_00d6138b::
    quest:DeregisterTimer(timerId9)
    ::LAB_00d61b69::
    quest:DeregisterTimer(timerId6)
    quest:DeregisterTimer(timerId)
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

