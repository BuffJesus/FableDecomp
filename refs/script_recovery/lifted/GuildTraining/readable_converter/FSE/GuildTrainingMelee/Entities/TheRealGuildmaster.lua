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

-- TheRealGuildmaster.Main (retail 0x00d58490)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult3, fret_03, fret_04, fret_06, fret_07, scratchValue7, conversationId
    local conversationId2, conversationId3, conversationId4, conversationId5, scratchValue8
    local tutorialState, scratchValue9, questionAnswer, scratchValue10, switch2, getActiveQuestName
    local scratchValue13, meleeOpponent, scratchValue15, scratchValue19, scratchValue20
    local scratchValue21, scratchValue22, scratchValue23, scratchValue24, scratchValue25
    local scratchValue26, scratchValue27, timerId, scratchValue28, scratchValue29, scratchValue30
    local scratchValue31, scratchValue32, scratchValue33, scratchValue34, scratchValue35
    local scratchValue36, scratchValue37, scratchValue38, scratchValue39, scratchValue40
    local scratchValue41
    scratchValue27 = resources:NewResource()
    while not resources:TryAcquire(scratchValue27, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(scratchValue27); return end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d5a8e2 end
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAlwaysBlockAttacksFromThing(me, quest:GetHero(), true)
    quest:SetIsPushableByHero(me, false)
    meleeOpponent = quest:GetThingWithScriptName("MeleeOpponent")
    quest:SetThingHasInformation(me, false, false, false)
    scratchValue7 = quest:RegisterTimer()
    scratchValue28 = scratchValue7
    quest:SetTimer(scratchValue7, 0)
    tutorialState = quest:GetStateInt("TutorialState")
    scratchValue41 = 0
    while tutorialState == 1 do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(scratchValue7); goto FLOW_after_lab_00d5a8cb end
        if not me:IsTalkedToByHero() then
            if not quest:GetStateBool("EarlyHitWhisper") or not quest:GetStateBool("WhisperArrived") then
                -- TODO(native): xStack_23c = (int *)((uint)xStack_23c | 1);
                if me:MsgIsHitByHero() then
                    predicateResult3 = true
                    goto FLOW_after_lab_00d586db
                end
                predicateResult3 = false
            else
                predicateResult3 = true
            end
            ::FLOW_after_lab_00d586db::
            if scratchValue26 & 1 ~= 0 then
                -- TODO(native): xStack_23c = (int *)((uint)xStack_23c & 0xfffffffe);
            end
            if predicateResult3 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue7); goto FLOW_after_lab_00d5a8cb end
                scratchValue22 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue27)) then
                    scratchValue7 = 0
                    me:Speak(quest:GetHero(), "TEXT_QST_028_GUILDMASTER_MELEE_HIT_WHISPER", 0, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue22)
                            goto LAB_00d5933c
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue22)
                        quest:DeregisterTimer(0)
                        goto FLOW_after_lab_00d5a8cb
                    end
                end
                quest:SetStateInt("TutorialState", 2)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue22)
            end
        else
            quest:SetStateInt("TutorialState", 2)
        end
        if (quest:IsDistanceBetweenThingsUnder(quest:GetHero(), me, 5.5) and quest:GetTimer(scratchValue28) < 1) and not me:IsPerformingScriptTask() then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue7); goto FLOW_after_lab_00d5a8cb end
            scratchValue7 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(scratchValue7, quest:GetHero())
            quest:SetTimer(scratchValue28, 10)
            if scratchValue41 == nil then
                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                quest:AddLineToConversation(scratchValue7, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_FIRST", me, quest:GetHero(), false)
                scratchValue41 = 1
            elseif scratchValue41 == 1 then
                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                quest:AddLineToConversation(scratchValue7, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_SECOND", me, quest:GetHero(), false)
            end
        end
        tutorialState = quest:GetStateInt("TutorialState")
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId)
    else
        quest:SetMasterGameState("HeroTakingGuildTest", true)
        quest:SetStateBool("WhisperStopWalking", true)
        quest:SetPlayerCreatureOnlyTarget(meleeOpponent)
        while quest:GetStateBool("MeleeRepeating") do
            if not quest:NewScriptFrame(me) then goto LAB_00d5933c end
            quest:SetStateInt("TutorialState", 2)
            quest:SetStateBool("MeleeRepeatKnown", false)
            scratchValue23 = resources:NewResource()
            while not resources:TryAcquire(scratchValue23, quest:GetHero(), 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(scratchValue23)
                    quest:DeregisterTimer(scratchValue28)
                    goto LAB_00d5a8d9
                end
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(scratchValue23)
                quest:DeregisterTimer(scratchValue28)
                goto LAB_00d5a8d9
            end
            scratchValue35 = resources:NewResource()
            while not resources:TryAcquire(scratchValue35, meleeOpponent, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(scratchValue35)
                    resources:ReleaseResource(scratchValue23)
                    quest:DeregisterTimer(scratchValue28)
                    goto LAB_00d5a8d9
                end
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(scratchValue35)
                resources:ReleaseResource(scratchValue23)
                quest:DeregisterTimer(scratchValue28)
                goto LAB_00d5a8d9
            end
            scratchValue32 = resources:NewActorMap()
            resources:SetActor(scratchValue32, "HERO", scratchValue23)
            resources:SetActor(scratchValue32, "TEACHER", scratchValue27)
            resources:SetActor(scratchValue32, "WHISPER", scratchValue35)
            scratchValue30 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_ATTACK", scratchValue32, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue30)
            resources:DestroyActorMap(scratchValue32)
            resources:ReleaseResource(scratchValue35)
            resources:ReleaseResource(scratchValue23)
            quest:SetStateInt("GenericTutorialCounter", 0)
            quest:DisplayQuestInfo(true)
            scratchValue26 = quest:AddQuestInfoCounter("HUD_WHISPER_ICON", 7, 1.0)
            quest:SetStateInt("TutorialState", 3)
            timerId = quest:RegisterTimer()
            quest:SetTimer(timerId, 15)
            while quest:GetStateInt("GenericTutorialCounter") < 7 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
                if quest:GetTimer(timerId) < 1 then
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, quest:GetHero())
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_HELP_ATTACK", me, quest:GetHero(), false)
                    quest:SetTimer(timerId, 15)
                end
                quest:UpdateQuestInfoCounter(scratchValue26, quest:GetStateInt("GenericTutorialCounter"), -1)
                if quest:GetHealth(quest:GetHero()) < quest:ReadGlobalGameDataFloat(3800) then
                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
                    conversationId2 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId2, quest:GetHero())
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_MAZE_HEAL_HERO", me, quest:GetHero(), false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                end
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
            quest:ClearAllRumbles()
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(scratchValue26)
            scratchValue39 = resources:NewResource()
            while not resources:TryAcquire(scratchValue39, quest:GetHero(), 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a922
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a922: (native jump target)
                resources:DestroyMovie(scratchValue39)
                -- LAB_00d5a9b5: (native jump target)
                quest:DeregisterTimer(timerId)
                quest:DeregisterTimer(timerId)
                goto FLOW_after_lab_00d5a8cb
            end
            scratchValue34 = resources:NewResource()
            while not resources:TryAcquire(scratchValue34, meleeOpponent, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a916
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a916: (native jump target)
                resources:DestroyMovie(scratchValue34)
                -- TODO(native): goto LAB_00d5a922
            end
            scratchValue33 = resources:NewActorMap()
            resources:SetActor(scratchValue33, "HERO", scratchValue39)
            resources:SetActor(scratchValue33, "TEACHER", scratchValue27)
            resources:SetActor(scratchValue33, "WHISPER", scratchValue34)
            scratchValue29 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_BLOCK", scratchValue33, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(scratchValue29)
            resources:DestroyActorMap(scratchValue33)
            resources:ReleaseResource(scratchValue34)
            resources:DestroyMovie(scratchValue39)
            quest:SetStateInt("GenericTutorialCounter", 0)
            if not quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
                    end
                    if not quest:IsActiveThreadTerminating() then
                        -- TODO(native): CSubtitleRenderer::SetText__atcbe9ee((CSubtitleRenderer *)xStack_10c);
                        goto LAB_00d5941c
                    end
                end
                -- TODO(native): goto LAB_00d5a9b5
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
            quest:DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK")
            while not quest:MsgIsGameInfoClickedPast() do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
            ::LAB_00d5941c::
            quest:SetStateInt("TutorialState", 4)
            scratchValue26 = quest:AddQuestInfoCounter("HUD_WHISPER_ICON", 5, 1.0)
            quest:DisplayQuestInfo(true)
            while quest:GetStateInt("GenericTutorialCounter") < 5 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
                quest:UpdateQuestInfoCounter(scratchValue26, quest:GetStateInt("GenericTutorialCounter"), -1)
                if quest:GetHealth(quest:GetHero()) < quest:ReadGlobalGameDataFloat(3800) then
                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
                    conversationId3 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId3, quest:GetHero())
                    quest:AddLineToConversation(conversationId3, "TEXT_QST_028_MAZE_HEAL_HERO", me, quest:GetHero(), false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                end
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
            quest:ClearAllRumbles()
            quest:SetStateInt("TutorialState", 5)
            scratchValue15 = quest:CreateCreature("CREATURE_RIVAL_HERO_THUNDER", quest:GetThingWithScriptName("SkillApprenticeMarker"):GetPos(), "MeleeThunder")
            quest:EntitySetAppearanceMorphSeed(scratchValue15, 1)
            scratchValue24 = resources:NewResource()
            while not resources:TryAcquire(scratchValue24, scratchValue15, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a9a7: (native jump target)
                resources:ReleaseResource(scratchValue24)
                -- TODO(native): goto LAB_00d5a9b5
            end
            scratchValue40 = resources:NewResource()
            scratchValue13 = quest:GetHero()
            while not resources:TryAcquire(scratchValue40, scratchValue13, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a93f
                scratchValue13 = quest:GetHero()
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a93f: (native jump target)
                -- TODO(native): this_01 = (CScriptGameResourceObjectMovieBase *)xStack_e0;
                -- LAB_00d5a99e: (native jump target)
                resources:ReleaseResource(this_01)
                -- TODO(native): goto LAB_00d5a9a7
            end
            scratchValue36 = resources:NewResource()
            while not resources:TryAcquire(scratchValue36, meleeOpponent, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a933
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a933: (native jump target)
                resources:ReleaseResource(scratchValue36)
                -- TODO(native): goto LAB_00d5a93f
            end
            scratchValue37 = resources:NewActorMap()
            resources:SetActor(scratchValue37, "HERO", scratchValue40)
            resources:SetActor(scratchValue37, "TEACHER", scratchValue27)
            resources:SetActor(scratchValue37, "THUNDER", scratchValue24)
            resources:SetActor(scratchValue37, "WHISPER", scratchValue36)
            scratchValue31 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_BATTLE", scratchValue37, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:ChangeHeroHealthBy(1000.0, true, false)
            quest:ModifyThingHealth(scratchValue13, 1000.0, false)
            quest:SetStateInt("TutorialState", 6)
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(scratchValue31)
            resources:DestroyActorMap(scratchValue37)
            resources:ReleaseResource(scratchValue36)
            resources:ReleaseResource(scratchValue40)
            state:SetBool("HeroStanding", true)
            state:SetBool("WhisperStanding", true)
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(scratchValue26)
            quest:DisplayQuestInfo(true)
            quest:AddQuestInfoBarHealth(quest:GetThingWithScriptName("MeleeOpponent"), getActiveQuestName, "HUD_WHISPER_ICON", 1.0)
            fret_03 = quest:GetHealth(nil --[[missing]])
            while state:GetBool("HeroStanding") and state:GetBool("WhisperStanding") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
                fret_04 = quest:GetHealth(quest:GetHero())
                if quest:ReadGlobalGameDataFloat(3800) <= fret_04 then
                    if quest:GetHealth(quest:GetThingWithScriptName("MeleeOpponent")) < quest:ReadGlobalGameDataFloat(3800) then
                        if not quest:IsActiveThreadTerminating() then
                            state:SetBool("WhisperStanding", false)
                            conversationId4 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(conversationId4, quest:GetHero())
                            quest:AddLineToConversation(conversationId4, "TEXT_QST_028_MAZE_FIGHT_OVER", me, quest:GetHero(), false)
                            goto FLOW_after_lab_00d59cc6
                        end
                        -- TODO(native): goto LAB_00d5a9a7
                    end
                else
                    state:SetBool("HeroStanding", false)
                    conversationId5 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId5, quest:GetHero())
                    quest:AddLineToConversation(conversationId5, "TEXT_QST_028_MAZE_FIGHT_OVER", me, quest:GetHero(), false)
                end
                ::FLOW_after_lab_00d59cc6::
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
            quest:ClearAllRumbles()
            -- TODO(native): CStack_1d4 = (CCharString)(float)fret_06;
            fret_07 = quest:GetHealth(nil --[[missing]])
            scratchValue10 = 0
            -- TODO(native): CStack_1d4 = (CCharString) (float)(((float10)f_stk_74 - fret_07) - ((float10)f_stk_70 - (float10)(float)CStack_1d4));
            scratchValue9 = 0
            repeat
                scratchValue8 = scratchValue9
                if quest:ReadGlobalGameDataFloatAt(3764, scratchValue10) < CStack_1d4 ~= (quest:ReadGlobalGameDataFloatAt(3764, scratchValue10) == CStack_1d4) then
                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
                    break
                end
                scratchValue9 = scratchValue8 + 1
                scratchValue10 = scratchValue10 + 1
            until scratchValue9 >= 7
            quest:ResetPlayerCreatureOnlyTarget()
            quest:SetStateInt("TutorialState", 7)
            quest:DisplayQuestInfo(false)
            scratchValue20 = resources:NewResource()
            scratchValue13 = quest:GetHero()
            while not resources:TryAcquire(scratchValue20, scratchValue13, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a997
                scratchValue13 = quest:GetHero()
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a997: (native jump target)
                -- TODO(native): this_01 = (CScriptGameResourceObjectMovieBase *)xStack_1d0;
                -- TODO(native): goto LAB_00d5a99e
            end
            scratchValue19 = resources:NewResource()
            while not resources:TryAcquire(scratchValue19, meleeOpponent, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a98b
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a98b: (native jump target)
                resources:ReleaseResource(scratchValue19)
                -- TODO(native): goto LAB_00d5a997
            end
            quest:RemoveQuestInfoElement(scratchValue26)
            scratchValue25 = resources:NewActorMap()
            resources:SetActor(scratchValue25, "HERO", scratchValue20)
            resources:SetActor(scratchValue25, "TEACHER", scratchValue27)
            resources:SetActor(scratchValue25, "THUNDER", scratchValue24)
            resources:SetActor(scratchValue25, "WHISPER", scratchValue19)
            scratchValue21 = resources:NewStringMap()
            switch2 = scratchValue8
            repeat
                if switch2 == 0 then
                    resources:SetString(scratchValue21, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_APLUS")
                    break
                elseif switch2 == 1 then
                    resources:SetString(scratchValue21, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_A")
                    break
                elseif switch2 == 2 then
                    resources:SetString(scratchValue21, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_B")
                    break
                elseif switch2 == 3 then
                    resources:SetString(scratchValue21, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_C")
                    break
                elseif switch2 == 4 then
                    resources:SetString(scratchValue21, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_D")
                    break
                elseif switch2 == 5 then
                    resources:SetString(scratchValue21, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_E")
                    break
                elseif switch2 == 6 then
                    resources:SetString(scratchValue21, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_F")
                    break
                else
                    goto FLOW_native_label_1
                end
            until true
            ::FLOW_native_label_1::
            scratchValue38 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            if not state:GetBool("WhisperStanding") then
                if not quest:IsActiveThreadTerminating() then resources:RunMacroWithStrings("CS_GUILD_MELEE_BATTLE_WON", scratchValue25, scratchValue21, false, true); goto LAB_00d5a28a end
                -- LAB_00d5a948: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00d5a96a: (native jump target)
                resources:ReleaseResource(scratchValue38)
                resources:DestroyStringMap(scratchValue21)
                resources:DestroyActorMap(scratchValue25)
                -- TODO(native): goto LAB_00d5a98b
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a956: (native jump target)
                -- LAB_00d5a962: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): goto LAB_00d5a96a
            end
            resources:RunMacroWithStrings("CS_GUILD_MELEE_BATTLE_LOST", scratchValue25, scratchValue21, false, true)
            ::LAB_00d5a28a::
            quest:ChangeHeroHealthBy(1000.0, true, false)
            quest:EntitySetInFaction(scratchValue13, "FACTION_HERO")
            me:SetFriendsWithEverythingFlag(1)
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeThunder"), false, true)
            quest:Pause(2.0)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_MELEE_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer < 0 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a956
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a948
            if questionAnswer == 1 then
                if false then return end  -- TODO(native): goto LAB_00d5a956
                resources:RunMacro("CS_GUILD_MELEE_CONTINUE", scratchValue25, false, true)
                quest:SetStateBool("MeleeRepeating", false)
                quest:SetStateBool("MeleeRepeatKnown", true)
            else
                if false then return end  -- TODO(native): goto LAB_00d5a948
                resources:RunMacro("CS_GUILD_MELEE_REPEAT", scratchValue25, false, false)
                quest:SetStateBool("MeleeRepeating", true)
                quest:SetStateBool("MeleeRepeatKnown", true)
                if quest:IsObjectInThingsPossession("OBJECT_IRON_LONGSWORD", quest:GetHero()) then
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): iVar7 = *xStack_23c
--[[unresolved native value]]
                        -- TODO(native): goto LAB_00d5a962
                    end
                    quest:TakeObjectFromHero("OBJECT_IRON_LONGSWORD")
                end
            end
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(scratchValue38)
            resources:DestroyStringMap(scratchValue21)
            resources:DestroyActorMap(scratchValue25)
            resources:DestroyMovie(scratchValue19)
            resources:ReleaseResource(scratchValue20)
            while not quest:GetStateBool("MeleeOpponentReset") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
            resources:ReleaseResource(scratchValue24)
            quest:DeregisterTimer(scratchValue28)
            scratchValue7 = scratchValue28
        end
        if not quest:IsActiveThreadTerminating() then
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeOpponent"), false, true)
            me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 0x40400000, 0, false, true)
            quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos(), "MeleeApprentice")
            quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("CombatApprenticeMarker"):GetPos(), "CombatApprentice")
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_04", "", "")
        end
        quest:DeregisterTimer(scratchValue7)
    end
    ::FLOW_after_lab_00d5a8cb::
    ::LAB_00d5a8d9::
    ::LAB_00d5a8e2::
    resources:ReleaseResource(scratchValue27)
    do return end
    ::LAB_00d5933c::
    quest:DeregisterTimer(scratchValue28)
    goto LAB_00d5a8d9
end

-- TheRealGuildmaster.Init (retail 0x00d56670)
function Init(quest, me)
    state:SetBool("WhisperEarly", false)
    state:SetBool("WhisperLate", false)
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

