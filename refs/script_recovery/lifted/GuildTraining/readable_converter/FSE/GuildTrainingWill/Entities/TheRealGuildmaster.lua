-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_WillGrades = 3788,  -- '070000000000304100002041000000410000c040000040400000803f00000000'
    GUI_WillTimer = 3848,  -- 30.0
}

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- TheRealGuildmaster.Main (retail 0x00d5e0c0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue4, predicateResult, taskRunning, taskRunning4, c_stk_22d_1, c_stk_22d_2
    local ctr_22c, dist, tutorialState, addQuestInfoCounter, scratchValue, questionAnswer
    local questionAnswer2, questionAnswer3, questionAnswer4, questionAnswer5, questionAnswer6
    local conversationId, conversationId2, scratchValue29, scratchValue44, scratchValue45
    local scratchValue46, scratchValue47, guildEvilApprenticeMale, guildEvilApprenticeMale2
    local meleeApprentice, meleeApprentice2, movie, resource, scratchValue50, movie2, scratchValue51
    local getMasterGameState, scratchValue55, movie3, resource2, actorMap, resource3, timerId
    local timerId6, timerId7, timerId8, timerId9, resource4, resource5, actorMap2
    local function __region_LAB_00d61ad8_c22()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(scratchValue55)
    end
    local function __cleanup_LAB_00d61b0a()
        quest:DeregisterTimer(timerId9)
        quest:DeregisterTimer(timerId8)
        resources:ReleaseResource(resource3)
    end
    local function __cleanup_LAB_00d61b0a_c22()
        quest:DeregisterTimer(timerId9)
        quest:DeregisterTimer(timerId8)
        resources:ReleaseResource(resource3)
    end
    resource3 = resources:NewResource()
    while not resources:TryAcquire(resource3, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource3)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d61b7b end
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAlwaysBlockAttacksFromThing(me, hero, true)
    quest:SetThingHasInformation(me, false, false, false)
    quest:SetPlayerUsingWillDummies(true)
    quest:SetIsPushableByHero(me, false)
    timerId8 = quest:RegisterTimer()
    timerId9 = quest:RegisterTimer()
    quest:SetTimer(timerId9, 0)
    ctr_22c = 0
    quest:EntitySetTargetingType(me, 26)
    if not quest:GetStateBool("TestFinished") then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId9)
            quest:DeregisterTimer(timerId8)
            resources:ReleaseResource(resource3)
            return
        end
        me:MoveToPosition(quest:GetThingWithScriptName("M_WillTeacherStand"):GetPos(), 3.0, 0, false, true)
        while quest:GetStateInt("TutorialState") == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
            if me:IsTalkedToByHero() then
                if quest:GetMasterGameState("HeroTakingGuildTest") == 0 then
                    quest:SetStateInt("TutorialState", 2)
                else
                    scratchValue55 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    me:ClearCommands()
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                        scratchValue47 = "TEXT_QST_028_GUILDMASTER_WILL_NOT_START"
                        me:Speak(hero, scratchValue47, 0, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue55)
                                __cleanup_LAB_00d61b0a(); do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue55)
                            goto LAB_00d61b69
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue55)
                end
            end
            dist = 5.5
            if not ((quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId9) < 1) and not me:IsPerformingScriptTask()) then goto continue_3 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
            conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:SetTimer(timerId9, 10)
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
                resource4 = resources:NewResource()
                while not resources:TryAcquire(resource4, hero, 4) do
                    if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource4); goto LAB_00d61b69 end
                end
                actorMap2 = resources:NewActorMap()
                resources:SetActor(actorMap2, "HERO", resource4)
                resources:SetActor(actorMap2, "TEACHER", resource3)
                quest:GiveHeroAbility(11, true)
                quest:SetMasterGameState("WillTrainingStarted", true)
                movie3 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_GUILD_WILL_LIGHTNING", actorMap2, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                resources:DestroyActorMap(actorMap2)
                resources:ReleaseResource(resource4)
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
                timerId7 = quest:RegisterTimer()
                quest:SetTimer(timerId7, 0)
                tutorialState = quest:GetStateInt("TutorialState")
                c_stk_22d_1 = 0
                while tutorialState == 2 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                    while c_stk_22d_1 == 0 and quest:GetMasterGameState("WillScore") == 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        if me:IsTalkedToByHero() then
                            c_stk_22d_1 = 1
                        end
                        if quest:GetTimer(timerId7) < 1 then
                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                            quest:SetTimer(timerId7, 2)
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    if quest:GetMasterGameState("WillScore") ~= 0 then
                        quest:SetStateInt("TutorialState", 3)
                        resource5 = resources:NewResource()
                        while not resources:TryAcquire(resource5, hero, 4) do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(resource5)
                                quest:DeregisterTimer(timerId7)
                                goto LAB_00d61b69
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(resource5)
                            quest:DeregisterTimer(timerId7)
                            goto LAB_00d61b69
                        end
                        scratchValue45 = resources:NewActorMap()
                        resources:SetActor(scratchValue45, "HERO", resource5)
                        resources:SetActor(scratchValue45, "TEACHER", resource3)
                        movie = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_WILL_TEST", scratchValue45, false, true)
                        quest:FixMovieSequenceCamera(false)
                        resources:DestroyMovie(movie)
                        resources:DestroyActorMap(scratchValue45)
                        resources:ReleaseResource(resource5)
                    end
                    if c_stk_22d_1 == 0 then tutorialState = quest:GetStateInt("TutorialState"); goto continue_7 end
                    scratchValue4 = quest:IsActiveThreadTerminating()
                    if scratchValue4 then goto LAB_00d6138b end
                    c_stk_22d_1 = scratchValue4
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
                timerId6 = quest:RegisterTimer()
                quest:SetTimer(timerId6, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_WillTimer))))
                quest:SetStateInt("TutorialState", 3)
                quest:SetMasterGameState("WillScore", 0)
                timerId = quest:RegisterTimer()
                quest:SetTimer(ctr_22c, 0)
                addQuestInfoCounter = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                scratchValue51 = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                c_stk_22d_2 = 0
                quest:SetTimer(timerId7, 0)
                while 0 < quest:GetTimer(timerId6) and c_stk_22d_2 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                    if quest:GetTimer(timerId7) < 1 then
                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                        quest:SetTimer(timerId7, 2)
                    end
                    quest:SetMasterGameState("WillTestOccuring", true)
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        c_stk_22d_2 = 1
                    end
                    if quest:GetHeroWillEnergy() == 0 and quest:GetTimer(timerId9) < 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        conversationId2 = quest:AddNewConversation(me, false, false)
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
                quest:RemoveQuestInfoElement(scratchValue51)
                quest:SetStateInt("TutorialState", 0)
                resource2 = resources:NewResource()
                while not resources:TryAcquire(resource5, hero, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61370 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d61370 end
                actorMap = resources:NewActorMap()
                movie2 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:FixMovieSequenceCamera(true)
                if c_stk_22d_2 == 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                    getMasterGameState = quest:GetMasterGameState("WillScore")
                    scratchValue44 = 0
                    scratchValue = 0
                    repeat
                        scratchValue29 = scratchValue
                        if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_WillGrades, scratchValue44) < getMasterGameState ~= (quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_WillGrades, scratchValue44) == getMasterGameState) then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                            break
                        end
                        scratchValue = scratchValue29 + 1
                        scratchValue44 = scratchValue44 + 1
                    until scratchValue >= 7
                    scratchValue50 = resources:NewStringMap()
                    repeat
                        if scratchValue29 == 0 then
                            scratchValue47 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_APLUS"
                            resources:SetString(scratchValue50, "$GRADE", scratchValue47)
                            break
                        elseif scratchValue29 == 1 then
                            scratchValue47 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_A"
                            resources:SetString(scratchValue50, "$GRADE", scratchValue47)
                            break
                        elseif scratchValue29 == 2 then
                            scratchValue47 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_B"
                            resources:SetString(scratchValue50, "$GRADE", scratchValue47)
                            break
                        elseif scratchValue29 == 3 then
                            scratchValue47 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_C"
                            resources:SetString(scratchValue50, "$GRADE", scratchValue47)
                            break
                        elseif scratchValue29 == 4 then
                            scratchValue47 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_D"
                            resources:SetString(scratchValue50, "$GRADE", scratchValue47)
                            break
                        elseif scratchValue29 == 5 then
                            scratchValue47 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_E"
                            resources:SetString(scratchValue50, "$GRADE", scratchValue47)
                            break
                        elseif scratchValue29 == 6 then
                            scratchValue47 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_F"
                            resources:SetString(scratchValue50, "$GRADE", scratchValue47)
                            break
                        else
                            goto FLOW_native_label_1
                        end
                    until true
                    ::FLOW_native_label_1::
                    resources:SetActor(actorMap, "HERO", resource2)
                    resources:SetActor(actorMap, "TEACHER", resource3)
                    resources:RunMacro("CS_GUILD_WILL_WON_START", actorMap, false, false)
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacroWithStrings("CS_GUILD_WILL_WON", actorMap, scratchValue50, false, false)
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
                        meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
                        scratchValue55 = resources:NewResource()
                        while not resources:TryAcquire(scratchValue55, meleeApprentice, 4) do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue55)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(scratchValue55)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        -- TODO(native): resources:SetActor(xStack_210, "HERO", &xStack_204_2)
                        resources:SetActor(actorMap, "TEACHER", resource3)
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,(CCharString *)xStack_1a0);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,xStack_1f0);
                        resources:RunMacro("CS_GUILD_WILL_CONTINUE", actorMap, false, true)
                        scratchValue4 = false
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true)
                        questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer2 < 0 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                            else
                                resources:ReleaseResource(scratchValue55)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(scratchValue55)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        predicateResult = quest:IsActiveThreadTerminating()
                        if questionAnswer2 == 1 then
                            if predicateResult then
                                resources:ReleaseResource(scratchValue55)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            if quest:GetHealth(resources:ScriptThing(resource3)) <= 0.0 then return end  -- TODO(native): goto LAB_00d610d3
                            me:Speak(me, "WHISPER", 0x12d1148, false, false, true)
                            taskRunning = me:IsPerformingScriptTask()
                            goto LAB_00d61098
                        end
                        if predicateResult then
                            resources:ReleaseResource(scratchValue55)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        if quest:GetHealth(resources:ScriptThing(resource3)) <= 0.0 then
                            -- TODO(native): xStack_234 = (CCharString)((uint)CVar5 & 0xffffff);
                        end
                        me:Speak(me, "CS_GUILD_WILL_WON", 0x12d1368, false, false, true)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue55)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(scratchValue55)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        resources:ReleaseResource(scratchValue55)
                    else
                        quest:SetHeroWillEnergyLevel(1.0)
                        resources:SetActor(actorMap, "TEACHER", resource3)
                        resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap, false, false)
                        scratchValue4 = true
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyStringMap(scratchValue50)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                    quest:PauseAllNonScriptedEntities(true)
                    -- TODO(native): resources:SetActor(xStack_210, "HERO", &xStack_204_2)
                    resources:SetActor(actorMap, "TEACHER", resource3)
                    resources:RunMacro("CS_GUILD_WILL_DISQUALIFIED", actorMap, false, true)
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
                        meleeApprentice2 = quest:GetThingWithScriptName("MeleeApprentice")
                        resource = resources:NewResource()
                        while not resources:TryAcquire(resource, meleeApprentice2, 4) do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(resource)
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d6135b
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61004 end
                        -- TODO(native): resources:SetActor(xStack_210, "HERO", &xStack_204_2)
                        resources:SetActor(actorMap, "TEACHER", resource3)
                        resources:SetActor(actorMap, "WHISPER", resource)
                        resources:RunMacro("CS_GUILD_WILL_CONTINUE", actorMap, false, true)
                        scratchValue4 = false
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
                            if quest:GetHealth(resources:ScriptThing(resource3)) <= 0.0 then
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
                                scratchValue46 = quest:GetThingWithScriptName("WillApprentice")
                                scratchValue4 = scratchValue46 ~= nil and scratchValue46:IsAlive()
                                if not scratchValue4 then
                                    if not quest:IsActiveThreadTerminating() then
                                        scratchValue4 = false
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
                                quest:DeactivateQuestLater(quest:GetActiveQuestName(), scratchValue4)
                                repeat
                                    quest:NewScriptFrame(me)
                                until quest:IsActiveThreadTerminating()
                                goto FLOW_after_lab_00d60be3
                            end
                            me:Speak(me, "TEXT_QST_LOG_HERO_ATTRIBUTES", 0x12d1148, false, false, true)
                            taskRunning4 = me:IsPerformingScriptTask()
                            goto LAB_00d60bad
                        end
                        if quest:GetHealth(resources:ScriptThing(resource3)) <= 0.0 then
                            -- TODO(native): xStack_234 = (CCharString)((uint)CVar5 & 0xffffff);
                        end
                        if not me:Speak(me, scratchValue47, 0x12d1368, false, false, true) then goto LAB_00d61004 end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(resource)
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d6135b
                        end
                        resources:ReleaseResource(resource)
                        quest:PauseAllNonScriptedEntities(false)
                    else
                        quest:SetHeroWillEnergyLevel(1.0)
                        resources:SetActor(actorMap, "TEACHER", resource3)
                        resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap, false, false)
                        scratchValue4 = true
                        quest:PauseAllNonScriptedEntities(false)
                    end
                end
                quest:FixMovieSequenceCamera(false)
                quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", false)
                resources:DestroyMovie(movie2)
                resources:DestroyActorMap(actorMap)
                resources:ReleaseResource(resource2)
                quest:DeregisterTimer(timerId)
                quest:DeregisterTimer(scratchValue4)
                quest:DeregisterTimer(timerId7)
            until not scratchValue4
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetTargetingType(me, 58)
                quest:SetMasterGameState("HeroTakingGuildTest", false)
                quest:SetStateInt("TutorialState", 4)
                quest:SetStateBool("TestFinished", true)
                me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 1.0, 0, resource3 ~= 0, true)
                scratchValue45 = quest:GetThingWithScriptName("WillApprentice")
                if not (scratchValue45 ~= nil and scratchValue45:IsAlive()) then
                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d61b0a_c22(); return end
                    guildEvilApprenticeMale = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")
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
                            scratchValue4 = true
                        else
                            -- LAB_00d6158a_c22: (native jump target)
                            scratchValue4 = false
                        end
                        if scratchValue4 then
                            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d61b0a_c22(); return end
                            quest:SetStateBool("BanditsDefeated", true)
                        end
                        if not me:IsTalkedToByHero() then goto LAB_00d61af3 end
                        if not quest:IsActiveThreadTerminating() then
                            me:ClearCommands()
                            scratchValue55 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            questionAnswer5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while questionAnswer5 < 0 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d61b53_c22
                                questionAnswer5 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if quest:IsActiveThreadTerminating() then
                                -- LAB_00d61b44_c22: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                            else
                                scratchValue4 = quest:IsActiveThreadTerminating()
                                if questionAnswer5 == 1 then
                                    if not scratchValue4 then
                                        quest:FadeScreenOut(0.5, 0.5)
                                        quest:Pause(1.0)
                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
                                        quest:ConfiscateAllHeroWeapons()
                                        if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d61b44_c22
                                            quest:DeactivateQuestLater("Q_GuildTrainingWoodsWill", 0)
                                        end
                                        quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                                        quest:SetTimeOfDay(11.0)
                                        quest:ResetPlayerCreatureCombatMultiplier()
                                        quest:SetHeroAsTeenager(false)
                                        quest:ChangeHeroHealthBy(1000.0, true, false)
                                        quest:RemoveThing(quest:GetThingWithScriptName("MeleeApprentice"), false, true)
                                        quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_APPRENTICE", quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos(), "MeleeApprentice")
                                        __region_LAB_00d61ad8_c22()
                                        goto LAB_00d61af3
                                    end
                                elseif not scratchValue4 then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO", GROUP_SELECT_FIRST, false, false, 0.0 ~= 0)
                                        while me:IsPerformingScriptTask() do
                                            quest:NewScriptFrame(me)
                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d61b44_c22
                                        end
                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d61b53_c22
                                    end
                                    me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 1.0, 0, true)
                                    __region_LAB_00d61ad8_c22(); goto LAB_00d61af3
                                end
                                -- LAB_00d61b53_c22: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                            end
                            resources:DestroyMovie(scratchValue55)
                            goto LAB_00d61b69
                        end
                    end
                    __cleanup_LAB_00d61b0a_c22(); return
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
        me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 1.0, 0, resource3 ~= 0, true)
        scratchValue45 = quest:GetThingWithScriptName("WillApprentice")
        if not (scratchValue45 ~= nil and scratchValue45:IsAlive()) then
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d61b0a(); return end
            guildEvilApprenticeMale2 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")
            if guildEvilApprenticeMale2 ~= nil then
                guildEvilApprenticeMale2:SetToKillOnLevelUnload(0)
            end
        end
        if quest:GetStateBool("BanditsDefeated") then
            -- LAB_00d609de: (native jump target)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
            quest:SetPlayerUsingWillDummies(false)
            scratchValue4 = quest:IsActiveThreadTerminating()
            -- LAB_00d60a99: (native jump target)
            if scratchValue4 then quest:DeregisterTimer(timerId9); quest:DeregisterTimer(timerId8); resources:ReleaseResource(resource3); return end
            if not quest:GetStateBool("BanditsDefeated") then
                if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                    scratchValue4 = false
                    goto FLOW_after_lab_00d6158a
                end
                scratchValue4 = true
            else
                -- LAB_00d6158a: (native jump target)
                scratchValue4 = false
            end
            ::FLOW_after_lab_00d6158a::
            if scratchValue4 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId9)
                    quest:DeregisterTimer(timerId8)
                    resources:ReleaseResource(resource3)
                    return
                end
                quest:SetStateBool("BanditsDefeated", true)
            end
            if not me:IsTalkedToByHero() then goto LAB_00d61af3 end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId9); quest:DeregisterTimer(timerId8); resources:ReleaseResource(resource3); return end
            me:ClearCommands()
            scratchValue55 = resources:StartMovie("")
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
                -- LAB_00d61b44: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
            else
                scratchValue4 = quest:IsActiveThreadTerminating()
                if questionAnswer6 == 1 then
                    if not scratchValue4 then
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
                        resources:DestroyMovie(scratchValue55)
                        goto LAB_00d61af3
                    end
                elseif not scratchValue4 then
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO", GROUP_SELECT_FIRST, false, false, 0.0 ~= 0)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d61b53 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61b53 end
                    end
                    me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 1.0, 0, true)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue55)
                    goto LAB_00d61af3
                end
                ::LAB_00d61b53::
                quest:PauseAllNonScriptedEntities(false)
            end
            ::FLOW_after_lab_00d61b53::
            resources:DestroyMovie(scratchValue55)
            goto LAB_00d61b69
            quest:DeregisterTimer(timerId9)
            quest:DeregisterTimer(timerId8)
            resources:ReleaseResource(resource3)
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
    if not taskRunning then goto LAB_00d610c4 end
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(scratchValue55)
        quest:PauseAllNonScriptedEntities(false)
        goto FLOW_after_lab_00d61578
    end
    taskRunning = me:IsPerformingScriptTask()
    goto LAB_00d61098
    ::LAB_00d610c4::
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(scratchValue55)
        quest:PauseAllNonScriptedEntities(false)
    else
        -- LAB_00d610d3: (native jump target)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
        quest:ConfiscateAllHeroWeapons()
        if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(scratchValue55)
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
        scratchValue46 = quest:GetThingWithScriptName("WillApprentice")
        if scratchValue46 ~= nil and scratchValue46:IsAlive() then
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
            resources:ReleaseResource(scratchValue55)
            quest:PauseAllNonScriptedEntities(false)
            goto FLOW_after_lab_00d61578
        end
        if not quest:IsActiveThreadTerminating() then
            if quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")._0_4_ ~= nil then
                -- TODO(native): (**(code **)(*xStack_1b4._0_4_ + 0x118))(0);
            end
            -- TODO(native): goto LAB_00d6144d
        end
        resources:ReleaseResource(scratchValue55)
        quest:PauseAllNonScriptedEntities(false)
    end
    ::FLOW_after_lab_00d61578::
    resources:DestroyStringMap(scratchValue50)
    goto LAB_00d6135b
    ::LAB_00d60bad::
    if not taskRunning4 then goto LAB_00d60bd4 end
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource)
        quest:PauseAllNonScriptedEntities(false)
        goto LAB_00d6135b
    end
    taskRunning4 = me:IsPerformingScriptTask()
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
    scratchValue46 = quest:GetThingWithScriptName("WillApprentice")
    scratchValue4 = scratchValue46 ~= nil and scratchValue46:IsAlive()
    if not scratchValue4 then
        if not quest:IsActiveThreadTerminating() then
            scratchValue4 = false
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
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), scratchValue4)
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
    resources:DestroyActorMap(actorMap)
    ::LAB_00d61370::
    resources:ReleaseResource(resource2)
    ::LAB_00d61379::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(dist)
    ::LAB_00d6138b::
    quest:DeregisterTimer(timerId7)
    ::LAB_00d61b69::
    quest:DeregisterTimer(timerId9)
    quest:DeregisterTimer(timerId8)
    ::LAB_00d61b7b::
    resources:ReleaseResource(resource3)
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

