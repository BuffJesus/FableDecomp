-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

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
    local scratchValue4, isActiveThreadTerminating, isTalkedToByHero, scratchValue5, scratchValue6
    local scratchValue7, scratchValue8, scratchValue, scratchValue11, switch, scratchValue12
    local scratchValue13, scratchValue14, guildEvilApprenticeMale, guildEvilApprenticeMale2
    local meleeApprentice, meleeApprentice2, movie, resource, scratchValue15, movie2, scratchValue16
    local scratchValue17, movie3, resource2, actorMap, resource3, timerId, timerId3, timerId4
    local scratchValue18, scratchValue19, resource4, resource5, scratchValue20
    local function __region_LAB_00d61ad8_c22()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(scratchValue17)
    end
    local function __cleanup_LAB_00d61b0a()
        quest:DeregisterTimer(scratchValue19)
        quest:DeregisterTimer(scratchValue18)
        resources:ReleaseResource(resource3)
    end
    local function __cleanup_LAB_00d61b0a_c22()
        quest:DeregisterTimer(scratchValue19)
        quest:DeregisterTimer(scratchValue18)
        resources:ReleaseResource(resource3)
    end
    resource3 = resources:NewResource()
    scratchValue4 = resources:TryAcquire(resource3, me, 4)
    while not scratchValue4 do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource3); return end
        scratchValue4 = resources:TryAcquire(resource3, me, 4)
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d61b7b end
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAlwaysBlockAttacksFromThing(me, hero, true)
    quest:SetThingHasInformation(me, false, false, false)
    quest:SetPlayerUsingWillDummies(true)
    quest:SetIsPushableByHero(me, false)
    scratchValue7 = quest:RegisterTimer()
    scratchValue18 = scratchValue7
    scratchValue8 = quest:RegisterTimer()
    scratchValue19 = scratchValue8
    quest:SetTimer(scratchValue8, 0)
    quest:EntitySetTargetingType(me, 26)
    if not quest:GetStateBool("TestFinished") then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue8)
            quest:DeregisterTimer(scratchValue7)
            resources:ReleaseResource(resource3)
            return
        end
        me:MoveToPosition(quest:GetThingWithScriptName("M_WillTeacherStand"):GetPos(), 0x40400000, 0, false, true)
        scratchValue7 = quest:GetStateInt("TutorialState")
        while scratchValue7 == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
            if me:IsTalkedToByHero() then
                if quest:GetMasterGameState("HeroTakingGuildTest") == 0 then
                    quest:SetStateInt("TutorialState", 2)
                else
                    scratchValue17 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    me:ClearCommands()
                    scratchValue6 = 0.0
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                        scratchValue14 = "TEXT_QST_028_GUILDMASTER_WILL_NOT_START"
                        me:Speak(hero, scratchValue14, 0, false, true, false)
                        isTalkedToByHero = me:IsPerformingScriptTask()
                        while isTalkedToByHero do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue17)
                                __cleanup_LAB_00d61b0a(); return
                            end
                            isTalkedToByHero = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue17)
                            goto LAB_00d61b69
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue17)
                end
            end
            if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(scratchValue19) < 1) and not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                scratchValue8 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue8, hero)
                quest:SetTimer(scratchValue19, 10)
                if true then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(scratchValue8, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_FIRST", me, hero, false)
                elseif 0 == 1 then
                    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                    quest:AddLineToConversation(scratchValue8, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_SECOND", me, hero, false)
                end
                -- TODO(native): xStack_22c = 1 - xStack_22c;
            end
            scratchValue7 = quest:GetStateInt("TutorialState")
        end
        if not quest:IsActiveThreadTerminating() then
            quest:SetMasterGameState("HeroTakingGuildTest", true)
            repeat
                if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
                quest:SetStateInt("TutorialState", 2)
                resource4 = resources:NewResource()
                scratchValue4 = resources:TryAcquire(resource4, hero, 4)
                while not scratchValue4 do
                    if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource4); goto LAB_00d61b69 end
                    scratchValue4 = resources:TryAcquire(resource4, hero, 4)
                end
                scratchValue20 = resources:NewActorMap()
                resources:SetActor(scratchValue20, "HERO", resource4)
                resources:SetActor(scratchValue20, "TEACHER", resource3)
                quest:GiveHeroAbility(11, true)
                quest:SetMasterGameState("WillTrainingStarted", true)
                movie3 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_GUILD_WILL_LIGHTNING", scratchValue20, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                resources:DestroyActorMap(scratchValue20)
                resources:ReleaseResource(resource4)
                if quest:IsXbox() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                    scratchValue4 = quest:MsgIsGameInfoClickedPast()
                    while not scratchValue4 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
                        scratchValue4 = quest:MsgIsGameInfoClickedPast()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                    scratchValue4 = quest:MsgIsGameInfoClickedPast()
                    while not scratchValue4 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
                        scratchValue4 = quest:MsgIsGameInfoClickedPast()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)xStack_134);
                end
                quest:SetMasterGameState("WillScore", 0)
                timerId4 = quest:RegisterTimer()
                quest:SetTimer(timerId4, 0)
                scratchValue7 = quest:GetStateInt("TutorialState")
                scratchValue5 = 0
                while scratchValue7 == 2 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                    while scratchValue5 == 0 and quest:GetMasterGameState("WillScore") == 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        if me:IsTalkedToByHero() then
                            scratchValue5 = 1
                        end
                        if quest:GetTimer(timerId4) < 1 then
                            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                            quest:SetTimer(timerId4, 2)
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    if quest:GetMasterGameState("WillScore") ~= 0 then
                        quest:SetStateInt("TutorialState", 3)
                        resource5 = resources:NewResource()
                        scratchValue4 = resources:TryAcquire(resource5, hero, 4)
                        while not scratchValue4 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(resource5)
                                quest:DeregisterTimer(timerId4)
                                goto LAB_00d61b69
                            end
                            scratchValue4 = resources:TryAcquire(resource5, hero, 4)
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(resource5)
                            quest:DeregisterTimer(timerId4)
                            goto LAB_00d61b69
                        end
                        scratchValue12 = resources:NewActorMap()
                        resources:SetActor(scratchValue12, "HERO", resource5)
                        resources:SetActor(scratchValue12, "TEACHER", resource3)
                        movie = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_WILL_TEST", scratchValue12, false, true)
                        quest:FixMovieSequenceCamera(false)
                        resources:DestroyMovie(movie)
                        resources:DestroyActorMap(scratchValue12)
                        resources:ReleaseResource(resource5)
                    end
                    if scratchValue5 ~= 0 then
                        scratchValue4 = quest:IsActiveThreadTerminating()
                        if scratchValue4 then goto LAB_00d6138b end
                        scratchValue5 = scratchValue4
                        if quest:IsXbox() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            scratchValue4 = quest:MsgIsGameInfoClickedPast()
                            while not scratchValue4 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                                scratchValue4 = quest:MsgIsGameInfoClickedPast()
                            end
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            scratchValue4 = quest:MsgIsGameInfoClickedPast()
                            while not scratchValue4 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                                scratchValue4 = quest:MsgIsGameInfoClickedPast()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    end
                    scratchValue7 = quest:GetStateInt("TutorialState")
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                if quest:IsXbox() then
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                    scratchValue4 = quest:MsgIsGameInfoClickedPast()
                    while not scratchValue4 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        scratchValue4 = quest:MsgIsGameInfoClickedPast()
                    end
                else
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                    scratchValue4 = quest:MsgIsGameInfoClickedPast()
                    while not scratchValue4 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        scratchValue4 = quest:MsgIsGameInfoClickedPast()
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                scratchValue4 = quest:MsgOnHeroCastSpell()
                while not scratchValue4 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                    if me:IsTalkedToByHero() then
                        if quest:IsXbox() then
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                            scratchValue4 = quest:MsgIsGameInfoClickedPast()
                            while not scratchValue4 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                                scratchValue4 = quest:MsgIsGameInfoClickedPast()
                            end
                        else
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                            scratchValue4 = quest:MsgIsGameInfoClickedPast()
                            while not scratchValue4 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                                scratchValue4 = quest:MsgIsGameInfoClickedPast()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    end
                    scratchValue4 = quest:MsgOnHeroCastSpell()
                end
                timerId3 = quest:RegisterTimer()
                quest:SetTimer(timerId3, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_WillTimer))))
                quest:SetStateInt("TutorialState", 3)
                quest:SetMasterGameState("WillScore", 0)
                timerId = quest:RegisterTimer()
                scratchValue8 = timerId
                quest:SetTimer(timerId, 0)
                scratchValue7 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                scratchValue = scratchValue7
                scratchValue16 = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                scratchValue5 = 0
                quest:SetTimer(timerId4, 0)
                while 0 < quest:GetTimer(timerId3) and scratchValue5 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                    if quest:GetTimer(timerId4) < 1 then
                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                        quest:SetTimer(timerId4, 2)
                    end
                    quest:SetMasterGameState("WillTestOccuring", true)
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        scratchValue5 = 1
                    end
                    if quest:GetHeroWillEnergy() == 0 and quest:GetTimer(scratchValue8) < 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        scratchValue8 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue8, hero)
                        quest:AddLineToConversation(scratchValue8, "TEXT_QST_028_GUILDMASTER_WILL_NO_WILL", me, hero, false)
                        quest:SetTimer(timerId, 8)
                        scratchValue7 = scratchValue
                    end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        if quest:IsXbox() then
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                            scratchValue4 = quest:MsgIsGameInfoClickedPast()
                            while not scratchValue4 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                                scratchValue4 = quest:MsgIsGameInfoClickedPast()
                            end
                        else
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                            scratchValue4 = quest:MsgIsGameInfoClickedPast()
                            while not scratchValue4 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                                scratchValue4 = quest:MsgIsGameInfoClickedPast()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                    end
                    quest:UpdateQuestInfoCounter(scratchValue7, quest:GetMasterGameState("WillScore"), -1)
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                quest:SetMasterGameState("WillTestOccuring", false)
                scratchValue4 = quest:IsHeroControlledByPlayer()
                while not scratchValue4 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                    scratchValue4 = quest:IsHeroControlledByPlayer()
                end
                quest:DisplayQuestInfo(false)
                quest:RemoveQuestInfoElement(scratchValue7)
                quest:RemoveQuestInfoElement(scratchValue16)
                quest:SetStateInt("TutorialState", 0)
                resource2 = resources:NewResource()
                scratchValue4 = hero:AcquireControl(4)
                while not scratchValue4 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61370 end
                    scratchValue4 = hero:AcquireControl(4)
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d61370 end
                actorMap = resources:NewActorMap()
                movie2 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:FixMovieSequenceCamera(true)
                if scratchValue5 == 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                    scratchValue16 = quest:GetMasterGameState("WillScore")
                    scratchValue11 = 0
                    scratchValue7 = 0
                    repeat
                        scratchValue8 = scratchValue7
                        if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_WillGrades, scratchValue11) < scratchValue16 ~= (quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_WillGrades, scratchValue11) == scratchValue16) then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                            break
                        end
                        scratchValue7 = scratchValue8 + 1
                        scratchValue11 = scratchValue11 + 1
                    until scratchValue7 >= 7
                    scratchValue15 = resources:NewStringMap()
                    switch = scratchValue8
                    repeat
                        if switch == 0 then
                            scratchValue14 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_APLUS"
                            resources:SetString(scratchValue15, "$GRADE", scratchValue14)
                            break
                        elseif switch == 1 then
                            scratchValue14 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_A"
                            resources:SetString(scratchValue15, "$GRADE", scratchValue14)
                            break
                        elseif switch == 2 then
                            scratchValue14 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_B"
                            resources:SetString(scratchValue15, "$GRADE", scratchValue14)
                            break
                        elseif switch == 3 then
                            scratchValue14 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_C"
                            resources:SetString(scratchValue15, "$GRADE", scratchValue14)
                            break
                        elseif switch == 4 then
                            scratchValue14 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_D"
                            resources:SetString(scratchValue15, "$GRADE", scratchValue14)
                            break
                        elseif switch == 5 then
                            scratchValue14 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_E"
                            resources:SetString(scratchValue15, "$GRADE", scratchValue14)
                            break
                        elseif switch == 6 then
                            scratchValue14 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_F"
                            resources:SetString(scratchValue15, "$GRADE", scratchValue14)
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
                    resources:RunMacroWithStrings("CS_GUILD_WILL_WON", actorMap, scratchValue15, false, false)
                    quest:Pause(2.0)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
                    scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while scratchValue7 < 0 do
                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d61578 end
                        scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d61578 end
                    if scratchValue7 == 1 then
                        quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", true)
                        meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
                        scratchValue17 = resources:NewResource()
                        scratchValue4 = resources:TryAcquire(scratchValue17, meleeApprentice, 4)
                        while not scratchValue4 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue17)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            scratchValue4 = resources:TryAcquire(scratchValue17, meleeApprentice, 4)
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(scratchValue17)
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
                        scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue7 < 0 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue17)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(scratchValue17)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                        if scratchValue7 == 1 then
                            if isActiveThreadTerminating then
                                resources:ReleaseResource(scratchValue17)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            scratchValue6 = 0.0
                            if quest:GetHealth(resources:ScriptThing(resource3)) <= 0.0 then return end  -- TODO(native): goto LAB_00d610d3
                            me:Speak(me, "WHISPER", 0x12d1148, false, false, true)
                            isTalkedToByHero = me:IsPerformingScriptTask()
                            goto LAB_00d61098
                        end
                        if isActiveThreadTerminating then
                            resources:ReleaseResource(scratchValue17)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        if quest:GetHealth(resources:ScriptThing(resource3)) <= 0.0 then
                            -- TODO(native): xStack_234 = (CCharString)((uint)CVar5 & 0xffffff);
                        end
                        me:Speak(me, "CS_GUILD_WILL_WON", 0x12d1368, false, false, true)
                        isTalkedToByHero = me:IsPerformingScriptTask()
                        while isTalkedToByHero do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue17)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            isTalkedToByHero = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(scratchValue17)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        resources:ReleaseResource(scratchValue17)
                    else
                        quest:SetHeroWillEnergyLevel(1.0)
                        resources:SetActor(actorMap, "TEACHER", resource3)
                        resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap, false, false)
                        scratchValue4 = true
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyStringMap(scratchValue15)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                    quest:PauseAllNonScriptedEntities(true)
                    -- TODO(native): resources:SetActor(xStack_210, "HERO", &xStack_204_2)
                    resources:SetActor(actorMap, "TEACHER", resource3)
                    resources:RunMacro("CS_GUILD_WILL_DISQUALIFIED", actorMap, false, true)
                    quest:Pause(2.0)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
                    scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while scratchValue7 < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6101c end
                        scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6101c end
                    if scratchValue7 == 1 then
                        quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", true)
                        meleeApprentice2 = quest:GetThingWithScriptName("MeleeApprentice")
                        resource = resources:NewResource()
                        scratchValue4 = resources:TryAcquire(resource, meleeApprentice2, 4)
                        while not scratchValue4 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(resource)
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d6135b
                            end
                            scratchValue4 = resources:TryAcquire(resource, meleeApprentice2, 4)
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61004 end
                        -- TODO(native): resources:SetActor(xStack_210, "HERO", &xStack_204_2)
                        resources:SetActor(actorMap, "TEACHER", resource3)
                        resources:SetActor(actorMap, "WHISPER", resource)
                        resources:RunMacro("CS_GUILD_WILL_CONTINUE", actorMap, false, true)
                        scratchValue4 = false
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true)
                        scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue7 < 0 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(resource)
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d6135b
                            end
                            scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61004 end
                        if scratchValue7 == 1 then
                            scratchValue6 = 0.0
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
                                scratchValue13 = quest:GetThingWithScriptName("WillApprentice")
                                if not (scratchValue13 ~= nil and scratchValue13:IsAlive()) then
                                    if not quest:IsActiveThreadTerminating() then
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
                                quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
                                repeat
                                    quest:NewScriptFrame(me)
                                until quest:IsActiveThreadTerminating()
                                goto FLOW_after_lab_00d60be3
                            end
                            me:Speak(me, "TEXT_QST_LOG_HERO_ATTRIBUTES", 0x12d1148, false, false, true)
                            isTalkedToByHero = me:IsPerformingScriptTask()
                            goto LAB_00d60bad
                        end
                        if quest:GetHealth(resources:ScriptThing(resource3)) <= 0.0 then
                            -- TODO(native): xStack_234 = (CCharString)((uint)CVar5 & 0xffffff);
                        end
                        me:Speak(me, scratchValue14, 0x12d1368, false, false, true)
                        isTalkedToByHero = me:IsPerformingScriptTask()
                        while isTalkedToByHero do
                            if not quest:NewScriptFrame(me) then goto LAB_00d61004 end
                            isTalkedToByHero = me:IsPerformingScriptTask()
                        end
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
                quest:DeregisterTimer(timerId4)
            until not scratchValue4
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetTargetingType(me, 58)
                quest:SetMasterGameState("HeroTakingGuildTest", false)
                quest:SetStateInt("TutorialState", 4)
                quest:SetStateBool("TestFinished", true)
                me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 0x3f800000, 0, false, true)
                scratchValue12 = quest:GetThingWithScriptName("WillApprentice")
                if not (scratchValue12 ~= nil and scratchValue12:IsAlive()) then
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
                            scratchValue17 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "", "TEXT_OBJECT_HERO_ANSWER_NO", "TEXT_OBJECT_HERO_ANSWER_NO")
                            scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while scratchValue7 < 0 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d61b53_c22
                                scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if quest:IsActiveThreadTerminating() then
                                -- LAB_00d61b44_c22: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                            else
                                scratchValue4 = quest:IsActiveThreadTerminating()
                                if scratchValue7 == 1 then
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
                                    scratchValue6 = 0.0
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                                        me:Speak(hero, "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO", 0, false, true, false)
                                        isTalkedToByHero = me:IsPerformingScriptTask()
                                        while isTalkedToByHero do
                                            quest:NewScriptFrame(me)
                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d61b44_c22
                                            isTalkedToByHero = me:IsPerformingScriptTask()
                                        end
                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d61b53_c22
                                    end
                                    me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 0x3f800000, 0, false, true)
                                    __region_LAB_00d61ad8_c22(); goto LAB_00d61af3
                                end
                                -- LAB_00d61b53_c22: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                            end
                            resources:DestroyMovie(scratchValue17)
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
        me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 0x3f800000, 0, false, true)
        scratchValue12 = quest:GetThingWithScriptName("WillApprentice")
        if not (scratchValue12 ~= nil and scratchValue12:IsAlive()) then
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
            if not scratchValue4 then
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
                        quest:DeregisterTimer(scratchValue19)
                        quest:DeregisterTimer(scratchValue18)
                        resources:ReleaseResource(resource3)
                        return
                    end
                    quest:SetStateBool("BanditsDefeated", true)
                end
                if not me:IsTalkedToByHero() then goto LAB_00d61af3 end
                if not quest:IsActiveThreadTerminating() then
                    me:ClearCommands()
                    scratchValue17 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "", "TEXT_OBJECT_HERO_ANSWER_NO", "TEXT_OBJECT_HERO_ANSWER_NO")
                    scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while scratchValue7 < 0 do
                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d61b53 end
                        scratchValue7 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then
                        -- LAB_00d61b44: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                    else
                        scratchValue4 = quest:IsActiveThreadTerminating()
                        if scratchValue7 == 1 then
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
                                resources:DestroyMovie(scratchValue17)
                                goto LAB_00d61af3
                            end
                        elseif not scratchValue4 then
                            scratchValue6 = 0.0
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                                me:Speak(hero, "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO", 0, false, true, false)
                                isTalkedToByHero = me:IsPerformingScriptTask()
                                while isTalkedToByHero do
                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d61b53 end
                                    isTalkedToByHero = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d61b53 end
                            end
                            me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 0x3f800000, 0, false, true)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue17)
                            goto LAB_00d61af3
                        end
                        ::LAB_00d61b53::
                        quest:PauseAllNonScriptedEntities(false)
                    end
                    ::FLOW_after_lab_00d61b53::
                    resources:DestroyMovie(scratchValue17)
                    goto LAB_00d61b69
                end
            end
            quest:DeregisterTimer(scratchValue19)
            quest:DeregisterTimer(scratchValue18)
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
    if not isTalkedToByHero then goto LAB_00d610c4 end
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(scratchValue17)
        quest:PauseAllNonScriptedEntities(false)
        goto FLOW_after_lab_00d61578
    end
    isTalkedToByHero = me:IsPerformingScriptTask()
    goto LAB_00d61098
    ::LAB_00d610c4::
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(scratchValue17)
        quest:PauseAllNonScriptedEntities(false)
    else
        -- LAB_00d610d3: (native jump target)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
        quest:ConfiscateAllHeroWeapons()
        if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(scratchValue17)
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
        scratchValue13 = quest:GetThingWithScriptName("WillApprentice")
        if scratchValue13 ~= nil and scratchValue13:IsAlive() then
            -- LAB_00d6144d: (native jump target)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
            -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)&xStack_238,(CCharString *)pCVar12);
            quest:SetPlayerUsingWillDummies(false)
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            quest:SetStateInt("TutorialState", 4)
            quest:SetStateBool("TestFinished", true)
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
            resources:ReleaseResource(scratchValue17)
            quest:PauseAllNonScriptedEntities(false)
            goto FLOW_after_lab_00d61578
        end
        if not quest:IsActiveThreadTerminating() then
            if quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")._0_4_ ~= nil then
                -- TODO(native): (**(code **)(*xStack_1b4._0_4_ + 0x118))(0);
            end
            -- TODO(native): goto LAB_00d6144d
        end
        resources:ReleaseResource(scratchValue17)
        quest:PauseAllNonScriptedEntities(false)
    end
    ::FLOW_after_lab_00d61578::
    resources:DestroyStringMap(scratchValue15)
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
    if not quest:IsActiveThreadTerminating() then
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
        scratchValue13 = quest:GetThingWithScriptName("WillApprentice")
        if not (scratchValue13 ~= nil and scratchValue13:IsAlive()) then
            if not quest:IsActiveThreadTerminating() then
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
        quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
        repeat
            quest:NewScriptFrame(me)
        until quest:IsActiveThreadTerminating()
    end
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
    quest:DeregisterTimer(scratchValue6)
    ::LAB_00d6138b::
    quest:DeregisterTimer(timerId4)
    ::LAB_00d61b69::
    quest:DeregisterTimer(scratchValue19)
    quest:DeregisterTimer(scratchValue18)
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

