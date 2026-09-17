-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

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
    local resources = quest:RetailResources()
    local scratchValue, scratchValue5, scratchValue6, scratchValue7, scratchValue8, scratchValue9
    local scratchValue10, scratchValue11, scratchValue13, scratchValue14, switch2, scratchValue15
    local scratchValue16, scratchValue17, scratchValue18, scratchValue20, meleeApprentice
    local meleeApprentice2, scratchValue28, scratchValue29, scratchValue30, scratchValue31
    local scratchValue32, scratchValue34, scratchValue35, scratchValue36, scratchValue37
    local scratchValue38, timerId, timerId2, timerId3, scratchValue39, scratchValue40
    local scratchValue41, scratchValue42, scratchValue43
    local function __region_LAB_00d61ad8_c22()
        quest:PauseAllNonScriptedEntities(false)
        resources:ReleaseResource(scratchValue34)
    end
    local function __cleanup_LAB_00d61b0a()
        quest:DeregisterTimer(scratchValue40)
        quest:DeregisterTimer(scratchValue39)
        resources:ReleaseResource(scratchValue38)
    end
    local function __cleanup_LAB_00d61b0a_c22()
        quest:DeregisterTimer(scratchValue40)
        quest:DeregisterTimer(scratchValue39)
        resources:ReleaseResource(scratchValue38)
    end
    scratchValue38 = resources:NewResource()
    scratchValue5 = resources:TryAcquire(scratchValue38, me, 4)
    while not scratchValue5 do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(scratchValue38); return end
        scratchValue5 = resources:TryAcquire(scratchValue38, me, 4)
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d61b7b end
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAlwaysBlockAttacksFromThing(me, quest:GetHero(), true)
    quest:SetThingHasInformation(me, false, false, false)
    quest:SetPlayerUsingWillDummies(true)
    quest:SetIsPushableByHero(me, false)
    scratchValue10 = quest:RegisterTimer()
    scratchValue39 = scratchValue10
    scratchValue11 = quest:RegisterTimer()
    scratchValue40 = scratchValue11
    quest:SetTimer(scratchValue11, 0)
    quest:EntitySetTargetingType(me, 26)
    if not quest:GetStateBool("TestFinished") then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue11)
            quest:DeregisterTimer(scratchValue10)
            resources:ReleaseResource(scratchValue38)
            return
        end
        me:MoveToPosition(quest:GetThingWithScriptName("M_WillTeacherStand"):GetPos(), 0x40400000, 0, false, true)
        scratchValue10 = quest:GetStateInt("TutorialState")
        while scratchValue10 == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
            if me:IsTalkedToByHero() then
                if quest:GetMasterGameState("HeroTakingGuildTest") == 0 then
                    quest:SetStateInt("TutorialState", 2)
                else
                    scratchValue34 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    me:ClearCommands()
                    scratchValue9 = 0.0
                    if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue38)) then
                        scratchValue17 = "TEXT_QST_028_GUILDMASTER_WILL_NOT_START"
                        me:Speak(quest:GetHero(), scratchValue17, 0, false, true, false)
                        scratchValue7 = me:IsPerformingScriptTask()
                        while scratchValue7 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue34)
                                __cleanup_LAB_00d61b0a(); return
                            end
                            scratchValue7 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(scratchValue34)
                            goto LAB_00d61b69
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue34)
                end
            end
            if (quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(scratchValue40) < 1) and not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                scratchValue11 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue11, quest:GetHero())
                quest:SetTimer(scratchValue40, 10)
                if true then
                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                    quest:AddLineToConversation(scratchValue11, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_FIRST", me, quest:GetHero(), false)
                elseif 0 == 1 then
                    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                    quest:AddLineToConversation(scratchValue11, "TEXT_QST_028_GUILDMASTER_WILL_COMMENT_SECOND", me, quest:GetHero(), false)
                end
                -- TODO(native): xStack_22c = 1 - xStack_22c;
            end
            scratchValue10 = quest:GetStateInt("TutorialState")
        end
        if not quest:IsActiveThreadTerminating() then
            quest:SetMasterGameState("HeroTakingGuildTest", true)
            repeat
                if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
                quest:SetStateInt("TutorialState", 2)
                scratchValue41 = resources:NewResource()
                scratchValue5 = resources:TryAcquire(scratchValue41, quest:GetHero(), 4)
                while not scratchValue5 do
                    if not quest:NewScriptFrame(me) then resources:ReleaseResource(scratchValue41); goto LAB_00d61b69 end
                    scratchValue5 = resources:TryAcquire(scratchValue41, quest:GetHero(), 4)
                end
                scratchValue43 = resources:NewActorMap()
                resources:SetActor(scratchValue43, "HERO", scratchValue41)
                resources:SetActor(scratchValue43, "TEACHER", scratchValue38)
                quest:GiveHeroAbility(11, true)
                quest:SetMasterGameState("WillTrainingStarted", true)
                scratchValue35 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                resources:RunMacro("CS_GUILD_WILL_LIGHTNING", scratchValue43, false, true)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue35)
                resources:DestroyActorMap(scratchValue43)
                resources:DestroyMovie(scratchValue41)
                if quest:IsXbox() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                    scratchValue5 = quest:MsgIsGameInfoClickedPast()
                    while not scratchValue5 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
                        scratchValue5 = quest:MsgIsGameInfoClickedPast()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                    scratchValue5 = quest:MsgIsGameInfoClickedPast()
                    while not scratchValue5 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d61b69 end
                        scratchValue5 = quest:MsgIsGameInfoClickedPast()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d61b69 end
                    -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)xStack_134);
                end
                quest:SetMasterGameState("WillScore", 0)
                timerId3 = quest:RegisterTimer()
                quest:SetTimer(timerId3, 0)
                scratchValue10 = quest:GetStateInt("TutorialState")
                scratchValue8 = 0
                while scratchValue10 == 2 do
                    quest:NewScriptFrame(me)
                    scratchValue = timerId3
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    while scratchValue8 == 0 and quest:GetMasterGameState("WillScore") == 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        if me:IsTalkedToByHero() then
                            scratchValue8 = 1
                        end
                        if quest:GetTimer(scratchValue) < 1 then
                            quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                            quest:SetTimer(scratchValue, 2)
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    if quest:GetMasterGameState("WillScore") ~= 0 then
                        quest:SetStateInt("TutorialState", 3)
                        scratchValue42 = resources:NewResource()
                        scratchValue5 = resources:TryAcquire(scratchValue42, quest:GetHero(), 4)
                        while not scratchValue5 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue42)
                                quest:DeregisterTimer(timerId3)
                                goto LAB_00d61b69
                            end
                            scratchValue5 = resources:TryAcquire(scratchValue42, quest:GetHero(), 4)
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(scratchValue42)
                            quest:DeregisterTimer(timerId3)
                            goto LAB_00d61b69
                        end
                        scratchValue15 = resources:NewActorMap()
                        resources:SetActor(scratchValue15, "HERO", scratchValue42)
                        resources:SetActor(scratchValue15, "TEACHER", scratchValue38)
                        scratchValue28 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:FixMovieSequenceCamera(true)
                        resources:RunMacro("CS_GUILD_WILL_TEST", scratchValue15, false, true)
                        quest:FixMovieSequenceCamera(false)
                        resources:ReleaseResource(scratchValue28)
                        resources:DestroyActorMap(scratchValue15)
                        resources:ReleaseResource(scratchValue42)
                    end
                    if scratchValue8 ~= 0 then
                        scratchValue5 = quest:IsActiveThreadTerminating()
                        if scratchValue5 then goto LAB_00d6138b end
                        scratchValue8 = scratchValue5
                        if quest:IsXbox() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            scratchValue5 = quest:MsgIsGameInfoClickedPast()
                            while not scratchValue5 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                                scratchValue5 = quest:MsgIsGameInfoClickedPast()
                            end
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            scratchValue5 = quest:MsgIsGameInfoClickedPast()
                            while not scratchValue5 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                                scratchValue5 = quest:MsgIsGameInfoClickedPast()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    end
                    scratchValue10 = quest:GetStateInt("TutorialState")
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                if quest:IsXbox() then
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                    scratchValue5 = quest:MsgIsGameInfoClickedPast()
                    while not scratchValue5 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        scratchValue5 = quest:MsgIsGameInfoClickedPast()
                    end
                else
                    quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                    scratchValue5 = quest:MsgIsGameInfoClickedPast()
                    while not scratchValue5 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                        scratchValue5 = quest:MsgIsGameInfoClickedPast()
                    end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                scratchValue5 = quest:MsgOnHeroCastSpell()
                while not scratchValue5 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                    if me:IsTalkedToByHero() then
                        if quest:IsXbox() then
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                            scratchValue5 = quest:MsgIsGameInfoClickedPast()
                            while not scratchValue5 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                                scratchValue5 = quest:MsgIsGameInfoClickedPast()
                            end
                        else
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                            scratchValue5 = quest:MsgIsGameInfoClickedPast()
                            while not scratchValue5 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d6138b end
                                scratchValue5 = quest:MsgIsGameInfoClickedPast()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d6138b end
                    end
                    scratchValue5 = quest:MsgOnHeroCastSpell()
                end
                timerId2 = quest:RegisterTimer()
                quest:SetTimer(timerId2, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(3848))))
                quest:SetStateInt("TutorialState", 3)
                quest:SetMasterGameState("WillScore", 0)
                timerId = quest:RegisterTimer()
                scratchValue11 = timerId
                quest:SetTimer(timerId, 0)
                scratchValue10 = quest:AddQuestInfoCounter("HUD_ICON_ARROW", 0, 1.0)
                scratchValue13 = scratchValue10
                scratchValue32 = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                quest:DisplayQuestInfo(true)
                scratchValue8 = 0
                quest:SetTimer(timerId3, 0)
                while 0 < quest:GetTimer(timerId2) and scratchValue8 == 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                    if quest:GetTimer(timerId3) < 1 then
                        quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                        quest:SetTimer(timerId3, 2)
                    end
                    quest:SetMasterGameState("WillTestOccuring", true)
                    if quest:GetMasterGameState("GuildWarningOccuring") ~= 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        scratchValue8 = 1
                    end
                    if quest:GetHeroWillEnergy() == 0 and quest:GetTimer(scratchValue11) < 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        scratchValue11 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue11, quest:GetHero())
                        quest:AddLineToConversation(scratchValue11, "TEXT_QST_028_GUILDMASTER_WILL_NO_WILL", me, quest:GetHero(), false)
                        quest:SetTimer(timerId, 8)
                        scratchValue10 = scratchValue13
                    end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                        if quest:IsXbox() then
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST")
                            scratchValue5 = quest:MsgIsGameInfoClickedPast()
                            while not scratchValue5 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                                scratchValue5 = quest:MsgIsGameInfoClickedPast()
                            end
                        else
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAP_PC")
                            quest:DisplayGameInfo("TEXT_QST_028_WILL_INSTRUCTIONS_ZAPTEST_PC")
                            scratchValue5 = quest:MsgIsGameInfoClickedPast()
                            while not scratchValue5 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                                scratchValue5 = quest:MsgIsGameInfoClickedPast()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                    end
                    quest:UpdateQuestInfoCounter(scratchValue10, quest:GetMasterGameState("WillScore"), -1)
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d61379 end
                quest:SetMasterGameState("WillTestOccuring", false)
                scratchValue5 = quest:IsHeroControlledByPlayer()
                while not scratchValue5 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61379 end
                    scratchValue5 = quest:IsHeroControlledByPlayer()
                end
                quest:DisplayQuestInfo(false)
                quest:RemoveQuestInfoElement(scratchValue10)
                quest:RemoveQuestInfoElement(scratchValue32)
                quest:SetStateInt("TutorialState", 0)
                scratchValue36 = resources:NewResource()
                scratchValue5 = resources:TryAcquire(0, quest:GetHero(), 4)
                while not scratchValue5 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d61370 end
                    scratchValue5 = resources:TryAcquire(0, quest:GetHero(), 4)
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d61370 end
                scratchValue37 = resources:NewActorMap()
                scratchValue31 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:FixMovieSequenceCamera(true)
                if scratchValue8 == 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                    scratchValue32 = quest:GetMasterGameState("WillScore")
                    scratchValue14 = 0
                    scratchValue10 = 0
                    repeat
                        scratchValue11 = scratchValue10
                        if quest:ReadGlobalGameDataFloatAt(3788, scratchValue14) < scratchValue32 ~= (quest:ReadGlobalGameDataFloatAt(3788, scratchValue14) == scratchValue32) then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                            break
                        end
                        scratchValue10 = scratchValue11 + 1
                        scratchValue14 = scratchValue14 + 1
                    until scratchValue10 >= 7
                    scratchValue30 = resources:NewStringMap()
                    switch2 = scratchValue11
                    repeat
                        if switch2 == 0 then
                            scratchValue17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_APLUS"
                            resources:SetString(scratchValue30, "$GRADE", scratchValue17)
                            break
                        elseif switch2 == 1 then
                            scratchValue17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_A"
                            resources:SetString(scratchValue30, "$GRADE", scratchValue17)
                            break
                        elseif switch2 == 2 then
                            scratchValue17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_B"
                            resources:SetString(scratchValue30, "$GRADE", scratchValue17)
                            break
                        elseif switch2 == 3 then
                            scratchValue17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_C"
                            resources:SetString(scratchValue30, "$GRADE", scratchValue17)
                            break
                        elseif switch2 == 4 then
                            scratchValue17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_D"
                            resources:SetString(scratchValue30, "$GRADE", scratchValue17)
                            break
                        elseif switch2 == 5 then
                            scratchValue17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_E"
                            resources:SetString(scratchValue30, "$GRADE", scratchValue17)
                            break
                        elseif switch2 == 6 then
                            scratchValue17 = "TEXT_QST_028_GUILDMASTER_WILL_GRADE_F"
                            resources:SetString(scratchValue30, "$GRADE", scratchValue17)
                            break
                        else
                            goto FLOW_native_label_1
                        end
                    until true
                    ::FLOW_native_label_1::
                    resources:SetActor(scratchValue37, "HERO", scratchValue36)
                    resources:SetActor(scratchValue37, "TEACHER", scratchValue38)
                    resources:RunMacro("CS_GUILD_WILL_WON_START", scratchValue37, false, false)
                    quest:PauseAllNonScriptedEntities(true)
                    resources:RunMacroWithStrings("CS_GUILD_WILL_WON", scratchValue37, scratchValue30, false, false)
                    quest:Pause(2.0)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
                    scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while scratchValue10 < 0 do
                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d61578 end
                        scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d61578 end
                    if scratchValue10 == 1 then
                        quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", true)
                        meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
                        scratchValue34 = resources:NewResource()
                        scratchValue5 = resources:TryAcquire(scratchValue34, meleeApprentice, 4)
                        while not scratchValue5 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue34)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            scratchValue5 = resources:TryAcquire(scratchValue34, meleeApprentice, 4)
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:DestroyMovie(scratchValue34)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        -- TODO(native): resources:SetActor(xStack_210, "HERO", &xStack_204_2)
                        resources:SetActor(scratchValue37, "TEACHER", scratchValue38)
                        -- TODO(native): pCVar14 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_210,(CCharString *)xStack_1a0);
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= (pCVar14,xStack_1f0);
                        resources:RunMacro("CS_GUILD_WILL_CONTINUE", scratchValue37, false, true)
                        scratchValue5 = false
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true)
                        scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue10 < 0 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue34)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:DestroyMovie(scratchValue34)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        scratchValue6 = quest:IsActiveThreadTerminating()
                        if scratchValue10 == 1 then
                            if scratchValue6 then
                                resources:DestroyMovie(scratchValue34)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            scratchValue9 = 0.0
                            if quest:GetHealth(resources:ScriptThing(scratchValue38)) <= 0.0 then return end  -- TODO(native): goto LAB_00d610d3
                            me:Speak(me, "WHISPER", 0x12d1148, false, false, true)
                            scratchValue7 = me:IsPerformingScriptTask()
                            goto LAB_00d61098
                        end
                        if scratchValue6 then
                            resources:ReleaseResource(scratchValue34)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        if quest:GetHealth(resources:ScriptThing(scratchValue38)) <= 0.0 then
                            -- TODO(native): xStack_234 = (CCharString)((uint)CVar5 & 0xffffff);
                        end
                        me:Speak(me, "CS_GUILD_WILL_WON", 0x12d1368, false, false, true)
                        scratchValue7 = me:IsPerformingScriptTask()
                        while scratchValue7 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:DestroyMovie(scratchValue34)
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d61578
                            end
                            scratchValue7 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(scratchValue34)
                            quest:PauseAllNonScriptedEntities(false)
                            goto FLOW_after_lab_00d61578
                        end
                        resources:DestroyMovie(scratchValue34)
                    else
                        quest:SetHeroWillEnergyLevel(1.0)
                        resources:SetActor(scratchValue37, "TEACHER", scratchValue38)
                        resources:RunMacro("CS_GUILD_MELEE_REPEAT", scratchValue37, false, false)
                        scratchValue5 = true
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyStringMap(scratchValue30)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6135b end
                    quest:PauseAllNonScriptedEntities(true)
                    -- TODO(native): resources:SetActor(xStack_210, "HERO", &xStack_204_2)
                    resources:SetActor(scratchValue37, "TEACHER", scratchValue38)
                    resources:RunMacro("CS_GUILD_WILL_DISQUALIFIED", scratchValue37, false, true)
                    quest:Pause(2.0)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
                    scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while scratchValue10 < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d6101c end
                        scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d6101c end
                    if scratchValue10 == 1 then
                        quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", true)
                        meleeApprentice2 = quest:GetThingWithScriptName("MeleeApprentice")
                        scratchValue29 = resources:NewResource()
                        scratchValue5 = resources:TryAcquire(scratchValue29, meleeApprentice2, 4)
                        while not scratchValue5 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue29)
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d6135b
                            end
                            scratchValue5 = resources:TryAcquire(scratchValue29, meleeApprentice2, 4)
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61004 end
                        -- TODO(native): resources:SetActor(xStack_210, "HERO", &xStack_204_2)
                        resources:SetActor(scratchValue37, "TEACHER", scratchValue38)
                        resources:SetActor(scratchValue37, "WHISPER", scratchValue29)
                        resources:RunMacro("CS_GUILD_WILL_CONTINUE", scratchValue37, false, true)
                        scratchValue5 = false
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PLAY_WHISPER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_PLAY", "", true)
                        scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue10 < 0 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue29)
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d6135b
                            end
                            scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d61004 end
                        if scratchValue10 == 1 then
                            scratchValue9 = 0.0
                            if quest:GetHealth(resources:ScriptThing(scratchValue38)) <= 0.0 then
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
                                scratchValue16 = quest:GetThingWithScriptName("WillApprentice")
                                if not (scratchValue16 ~= nil and scratchValue16:IsAlive()) then
                                    if not quest:IsActiveThreadTerminating() then
                                        if quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")._0_4_ ~= nil then
                                            -- TODO(native): (**(code **)(*xStack_1b4._0_4_ + 0x118))(0);
                                        end
                                    else
                                        resources:ReleaseResource(scratchValue29)
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
                            scratchValue7 = me:IsPerformingScriptTask()
                            goto LAB_00d60bad
                        end
                        if quest:GetHealth(resources:ScriptThing(scratchValue38)) <= 0.0 then
                            -- TODO(native): xStack_234 = (CCharString)((uint)CVar5 & 0xffffff);
                        end
                        me:Speak(me, scratchValue17, 0x12d1368, false, false, true)
                        scratchValue7 = me:IsPerformingScriptTask()
                        while scratchValue7 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d61004 end
                            scratchValue7 = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            resources:ReleaseResource(scratchValue29)
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d6135b
                        end
                        resources:ReleaseResource(scratchValue29)
                        quest:PauseAllNonScriptedEntities(false)
                    else
                        quest:SetHeroWillEnergyLevel(1.0)
                        resources:SetActor(scratchValue37, "TEACHER", scratchValue38)
                        resources:RunMacro("CS_GUILD_MELEE_REPEAT", scratchValue37, false, false)
                        scratchValue5 = true
                        quest:PauseAllNonScriptedEntities(false)
                    end
                end
                quest:FixMovieSequenceCamera(false)
                quest:SetMasterGameState("MeleeApprenticeNeededForCutscene", false)
                resources:ReleaseResource(scratchValue31)
                resources:DestroyActorMap(scratchValue37)
                resources:ReleaseResource(scratchValue36)
                quest:DeregisterTimer(timerId)
                quest:DeregisterTimer(scratchValue5)
                quest:DeregisterTimer(timerId3)
            until not scratchValue5
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetTargetingType(me, 58)
                quest:SetMasterGameState("HeroTakingGuildTest", false)
                quest:SetStateInt("TutorialState", 4)
                quest:SetStateBool("TestFinished", true)
                me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 0x3f800000, 0, false, true)
                scratchValue15 = quest:GetThingWithScriptName("WillApprentice")
                if not (scratchValue15 ~= nil and scratchValue15:IsAlive()) then
                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d61b0a_c22(); return end
                    scratchValue18 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")
                    if scratchValue18 ~= nil and not scratchValue18:IsNull() then
                        scratchValue18:SetToKillOnLevelUnload(0)
                    end
                end
                if quest:GetStateBool("BanditsDefeated") then
                    -- LAB_00d609de_c22: (native jump target)
                    quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
                    quest:SetPlayerUsingWillDummies(false)
                    if not quest:IsActiveThreadTerminating() then
                        if not quest:GetStateBool("BanditsDefeated") then
                            if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then return end  -- TODO(native): goto LAB_00d6158a_c22
                            scratchValue5 = true
                        else
                            -- LAB_00d6158a_c22: (native jump target)
                            scratchValue5 = false
                        end
                        if scratchValue5 then
                            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d61b0a_c22(); return end
                            quest:SetStateBool("BanditsDefeated", true)
                        end
                        if not me:IsTalkedToByHero() then goto LAB_00d61af3 end
                        if not quest:IsActiveThreadTerminating() then
                            me:ClearCommands()
                            scratchValue34 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "", "TEXT_OBJECT_HERO_ANSWER_NO", "TEXT_OBJECT_HERO_ANSWER_NO")
                            scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while scratchValue10 < 0 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d61b53_c22
                                scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if quest:IsActiveThreadTerminating() then
                                -- LAB_00d61b44_c22: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                            else
                                scratchValue5 = quest:IsActiveThreadTerminating()
                                if scratchValue10 == 1 then
                                    if not scratchValue5 then
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
                                elseif not scratchValue5 then
                                    scratchValue9 = 0.0
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue38)) then
                                        me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO", 0, false, true, false)
                                        scratchValue7 = me:IsPerformingScriptTask()
                                        while scratchValue7 do
                                            quest:NewScriptFrame(me)
                                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d61b44_c22
                                            scratchValue7 = me:IsPerformingScriptTask()
                                        end
                                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d61b53_c22
                                    end
                                    me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 0x3f800000, 0, false, true)
                                    __region_LAB_00d61ad8_c22(); goto LAB_00d61af3
                                end
                                -- LAB_00d61b53_c22: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                            end
                            resources:ReleaseResource(scratchValue34)
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
        scratchValue15 = quest:GetThingWithScriptName("WillApprentice")
        if not (scratchValue15 ~= nil and scratchValue15:IsAlive()) then
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d61b0a(); return end
            scratchValue20 = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")
            if scratchValue20 ~= nil and not scratchValue20:IsNull() then
                scratchValue20:SetToKillOnLevelUnload(0)
            end
        end
        if quest:GetStateBool("BanditsDefeated") then
            -- LAB_00d609de: (native jump target)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_06", "", "")
            quest:SetPlayerUsingWillDummies(false)
            scratchValue5 = quest:IsActiveThreadTerminating()
            -- LAB_00d60a99: (native jump target)
            if not scratchValue5 then
                if not quest:GetStateBool("BanditsDefeated") then
                    if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
                        scratchValue5 = false
                        goto FLOW_after_lab_00d6158a
                    end
                    scratchValue5 = true
                else
                    -- LAB_00d6158a: (native jump target)
                    scratchValue5 = false
                end
                ::FLOW_after_lab_00d6158a::
                if scratchValue5 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(scratchValue40)
                        quest:DeregisterTimer(scratchValue39)
                        resources:ReleaseResource(scratchValue38)
                        return
                    end
                    quest:SetStateBool("BanditsDefeated", true)
                end
                if not me:IsTalkedToByHero() then goto LAB_00d61af3 end
                if not quest:IsActiveThreadTerminating() then
                    me:ClearCommands()
                    scratchValue34 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "", "TEXT_OBJECT_HERO_ANSWER_NO", "TEXT_OBJECT_HERO_ANSWER_NO")
                    scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while scratchValue10 < 0 do
                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d61b53 end
                        scratchValue10 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then
                        -- LAB_00d61b44: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                    else
                        scratchValue5 = quest:IsActiveThreadTerminating()
                        if scratchValue10 == 1 then
                            if not scratchValue5 then
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
                                resources:ReleaseResource(scratchValue34)
                                goto LAB_00d61af3
                            end
                        elseif not scratchValue5 then
                            scratchValue9 = 0.0
                            if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue38)) then
                                me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_WILL_END_QUESTION_NO", 0, false, true, false)
                                scratchValue7 = me:IsPerformingScriptTask()
                                while scratchValue7 do
                                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d61b53 end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00d61b53 end
                            end
                            me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 0x3f800000, 0, false, true)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:ReleaseResource(scratchValue34)
                            goto LAB_00d61af3
                        end
                        ::LAB_00d61b53::
                        quest:PauseAllNonScriptedEntities(false)
                    end
                    ::FLOW_after_lab_00d61b53::
                    resources:ReleaseResource(scratchValue34)
                    goto LAB_00d61b69
                end
            end
            quest:DeregisterTimer(scratchValue40)
            quest:DeregisterTimer(scratchValue39)
            resources:ReleaseResource(scratchValue38)
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
    if not scratchValue7 then goto LAB_00d610c4 end
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(scratchValue34)
        quest:PauseAllNonScriptedEntities(false)
        goto FLOW_after_lab_00d61578
    end
    scratchValue7 = me:IsPerformingScriptTask()
    goto LAB_00d61098
    ::LAB_00d610c4::
    if quest:IsActiveThreadTerminating() then
        resources:DestroyMovie(scratchValue34)
        quest:PauseAllNonScriptedEntities(false)
    else
        -- LAB_00d610d3: (native jump target)
        quest:FadeScreenOut(0.5, 0.5)
        quest:Pause(1.0)
        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_2_comp.xmv")
        quest:ConfiscateAllHeroWeapons()
        if quest:IsQuestActive("Q_GuildTrainingWoodsWill") then
            if quest:IsActiveThreadTerminating() then
                resources:DestroyMovie(scratchValue34)
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
        scratchValue16 = quest:GetThingWithScriptName("WillApprentice")
        if scratchValue16 ~= nil and scratchValue16:IsAlive() then
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
            resources:DestroyMovie(scratchValue34)
            quest:PauseAllNonScriptedEntities(false)
            goto FLOW_after_lab_00d61578
        end
        if not quest:IsActiveThreadTerminating() then
            if quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")._0_4_ ~= nil then
                -- TODO(native): (**(code **)(*xStack_1b4._0_4_ + 0x118))(0);
            end
            -- TODO(native): goto LAB_00d6144d
        end
        resources:ReleaseResource(scratchValue34)
        quest:PauseAllNonScriptedEntities(false)
    end
    ::FLOW_after_lab_00d61578::
    resources:DestroyStringMap(scratchValue30)
    goto LAB_00d6135b
    ::LAB_00d60bad::
    if not scratchValue7 then goto LAB_00d60bd4 end
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(scratchValue29)
        quest:PauseAllNonScriptedEntities(false)
        goto LAB_00d6135b
    end
    scratchValue7 = me:IsPerformingScriptTask()
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
        scratchValue16 = quest:GetThingWithScriptName("WillApprentice")
        if not (scratchValue16 ~= nil and scratchValue16:IsAlive()) then
            if not quest:IsActiveThreadTerminating() then
                if quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("WillApprenticeMarker"):GetPos(), "WillApprentice")._0_4_ ~= nil then
                    -- TODO(native): (**(code **)(*xStack_1b4._0_4_ + 0x118))(0);
                end
            else
                resources:ReleaseResource(scratchValue29)
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
    resources:DestroyMovie(scratchValue29)
    ::LAB_00d6101c::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d6135b::
    resources:ReleaseResource(scratchValue31)
    resources:DestroyActorMap(scratchValue37)
    ::LAB_00d61370::
    resources:DestroyMovie(scratchValue36)
    ::LAB_00d61379::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(scratchValue9)
    ::LAB_00d6138b::
    quest:DeregisterTimer(timerId3)
    ::LAB_00d61b69::
    quest:DeregisterTimer(scratchValue40)
    quest:DeregisterTimer(scratchValue39)
    ::LAB_00d61b7b::
    resources:ReleaseResource(scratchValue38)
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

