-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_MeleeGrades = 3764,  -- '070000000000904100007041000020410000a040000000000000a0c0000030c1'
    GUI_MinHealth = 3800,  -- 6.0
}

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
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, fret_04, fret_06, fret_07, conversationId, conversationId2
    local conversationId3, conversationId4, conversationId5, conversationId6, scratchValue6
    local tutorialState, scratchValue7, questionAnswer, scratchValue8, getActiveQuestName
    local meleeOpponent, rivalHeroThunder, resource2, resource3, scratchValue, scratchValue12
    local resource4, scratchValue13, infoCounter, resource5, timerId, timerId3, movie, movie2
    local movie3, actorMap, actorMap2, resource6, resource7, resource8, actorMap3, movie4, resource9
    local resource, scratchValue14
    resource5 = resources:NewResource()
    while not resources:TryAcquire(resource5, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource5); return end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d5a8e2 end
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAlwaysBlockAttacksFromThing(me, hero, true)
    quest:SetIsPushableByHero(me, false)
    meleeOpponent = quest:GetThingWithScriptName("MeleeOpponent")
    quest:SetThingHasInformation(me, false, false, false)
    timerId3 = quest:RegisterTimer()
    quest:SetTimer(timerId3, 0)
    tutorialState = quest:GetStateInt("TutorialState")
    scratchValue14 = 0
    while tutorialState == 1 do
        if not quest:NewScriptFrame(me) then goto LAB_00d5933c end
        if not me:IsTalkedToByHero() then
            if not quest:GetStateBool("EarlyHitWhisper") or not quest:GetStateBool("WhisperArrived") then
                -- TODO(native): xStack_23c = (int *)((uint)xStack_23c | 1);
                if me:MsgIsHitByHero() then
                    predicateResult = true
                    goto FLOW_after_lab_00d586db
                end
                predicateResult = false
            else
                predicateResult = true
            end
            ::FLOW_after_lab_00d586db::
            if infoCounter & 1 ~= 0 then
                -- TODO(native): xStack_23c = (int *)((uint)xStack_23c & 0xfffffffe);
            end
            if predicateResult then
                if quest:IsActiveThreadTerminating() then goto LAB_00d5933c end
                scratchValue12 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource5)) then
                    if not me:Speak(hero, "TEXT_QST_028_GUILDMASTER_MELEE_HIT_WHISPER", GROUP_SELECT_FIRST, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue12)
                        goto LAB_00d5933c
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue12)
                        goto LAB_00d5933c
                    end
                end
                quest:SetStateInt("TutorialState", 2)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue12)
            end
        else
            quest:SetStateInt("TutorialState", 2)
        end
        if (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId3) < 1) and not me:IsPerformingScriptTask() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d5933c end
            conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:SetTimer(timerId3, 10)
            if scratchValue14 == nil then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_FIRST", me, hero, false)
                scratchValue14 = 1
            elseif scratchValue14 == 1 then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_SECOND", me, hero, false)
            end
        end
        tutorialState = quest:GetStateInt("TutorialState")
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId3)
    else
        quest:SetMasterGameState("HeroTakingGuildTest", true)
        quest:SetStateBool("WhisperStopWalking", true)
        quest:SetPlayerCreatureOnlyTarget(meleeOpponent)
        while quest:GetStateBool("MeleeRepeating") do
            if not quest:NewScriptFrame(me) then goto LAB_00d5933c end
            quest:SetStateInt("TutorialState", 2)
            quest:SetStateBool("MeleeRepeatKnown", false)
            scratchValue12 = resources:NewResource()
            while not resources:TryAcquire(scratchValue12, hero, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(scratchValue12)
                    quest:DeregisterTimer(timerId3)
                    goto LAB_00d5a8d9
                end
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(scratchValue12)
                quest:DeregisterTimer(timerId3)
                goto LAB_00d5a8d9
            end
            resource7 = resources:NewResource()
            while not resources:TryAcquire(resource7, meleeOpponent, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource7)
                    resources:ReleaseResource(scratchValue12)
                    quest:DeregisterTimer(timerId3)
                    goto LAB_00d5a8d9
                end
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource7)
                resources:ReleaseResource(scratchValue12)
                quest:DeregisterTimer(timerId3)
                goto LAB_00d5a8d9
            end
            actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "HERO", scratchValue12)
            resources:SetActor(actorMap, "TEACHER", resource5)
            resources:SetActor(actorMap, "WHISPER", resource7)
            movie2 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_ATTACK", actorMap, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource7)
            resources:ReleaseResource(scratchValue12)
            quest:SetStateInt("GenericTutorialCounter", 0)
            quest:DisplayQuestInfo(true)
            infoCounter = quest:AddQuestInfoCounter("HUD_WHISPER_ICON", 7, 1.0)
            quest:SetStateInt("TutorialState", 3)
            timerId = quest:RegisterTimer()
            quest:SetTimer(timerId, 15)
            while quest:GetStateInt("GenericTutorialCounter") < 7 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
                if quest:GetTimer(timerId) < 1 then
                    conversationId2 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId2, hero)
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_MAZE_HELP_ATTACK", me, hero, false)
                    quest:SetTimer(timerId, 15)
                end
                quest:UpdateQuestInfoCounter(infoCounter, quest:GetStateInt("GenericTutorialCounter"), -1)
                if quest:GetHealth(hero) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
                    conversationId3 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId3, hero)
                    quest:AddLineToConversation(conversationId3, "TEXT_QST_028_MAZE_HEAL_HERO", me, hero, false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                end
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
            quest:ClearAllRumbles()
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(infoCounter)
            resource9 = resources:NewResource()
            while not resources:TryAcquire(resource9, hero, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a922
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a922: (native jump target)
                resources:DestroyMovie(resource9)
                -- LAB_00d5a9b5: (native jump target)
                quest:DeregisterTimer(timerId)
                quest:DeregisterTimer(timerId3)
                goto FLOW_after_lab_00d5a9be
            end
            resource6 = resources:NewResource()
            while not resources:TryAcquire(resource6, meleeOpponent, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a916
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a916: (native jump target)
                resources:DestroyMovie(resource6)
                -- TODO(native): goto LAB_00d5a922
            end
            actorMap2 = resources:NewActorMap()
            resources:SetActor(actorMap2, "HERO", resource9)
            resources:SetActor(actorMap2, "TEACHER", resource5)
            resources:SetActor(actorMap2, "WHISPER", resource6)
            movie = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_BLOCK", actorMap2, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(movie)
            resources:DestroyActorMap(actorMap2)
            resources:ReleaseResource(resource6)
            resources:DestroyMovie(resource9)
            quest:SetStateInt("GenericTutorialCounter", 0)
            if not quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
                    end
                    if not quest:IsActiveThreadTerminating() then
                        -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)xStack_f4);
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
            infoCounter = quest:AddQuestInfoCounter("HUD_WHISPER_ICON", 5, 1.0)
            quest:DisplayQuestInfo(true)
            while quest:GetStateInt("GenericTutorialCounter") < 5 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
                quest:UpdateQuestInfoCounter(infoCounter, quest:GetStateInt("GenericTutorialCounter"), -1)
                if quest:GetHealth(hero) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
                    conversationId4 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId4, hero)
                    quest:AddLineToConversation(conversationId4, "TEXT_QST_028_MAZE_HEAL_HERO", me, hero, false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                end
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9b5
            quest:ClearAllRumbles()
            quest:SetStateInt("TutorialState", 5)
            rivalHeroThunder = quest:CreateCreature("CREATURE_RIVAL_HERO_THUNDER", quest:GetThingWithScriptName("SkillApprenticeMarker"):GetPos(), "MeleeThunder")
            quest:EntitySetAppearanceMorphSeed(rivalHeroThunder, 1)
            resource4 = resources:NewResource()
            while not resources:TryAcquire(resource4, rivalHeroThunder, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a9a7: (native jump target)
                resources:ReleaseResource(resource4)
                -- TODO(native): goto LAB_00d5a9b5
            end
            resource = resources:NewResource()
            while not resources:TryAcquire(resource, hero, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a93f
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a93f: (native jump target)
                -- TODO(native): this_01 = (CScriptGameResourceObjectMovieBase *)xStack_e0;
                -- LAB_00d5a99e: (native jump target)
                resources:ReleaseResource(this_01)
                -- TODO(native): goto LAB_00d5a9a7
            end
            resource8 = resources:NewResource()
            while not resources:TryAcquire(resource8, meleeOpponent, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a933
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a933: (native jump target)
                resources:ReleaseResource(resource8)
                -- TODO(native): goto LAB_00d5a93f
            end
            actorMap3 = resources:NewActorMap()
            resources:SetActor(actorMap3, "HERO", resource)
            resources:SetActor(actorMap3, "TEACHER", resource5)
            resources:SetActor(actorMap3, "THUNDER", resource4)
            resources:SetActor(actorMap3, "WHISPER", resource8)
            movie3 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_BATTLE", actorMap3, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:ChangeHeroHealthBy(1000.0, true, false)
            quest:ModifyThingHealth(meleeOpponent, 1000.0, false)
            quest:SetStateInt("TutorialState", 6)
            quest:PauseAllNonScriptedEntities(false)
            resources:ReleaseResource(movie3)
            resources:DestroyActorMap(actorMap3)
            resources:ReleaseResource(resource8)
            resources:ReleaseResource(resource)
            state:SetBool("HeroStanding", true)
            state:SetBool("WhisperStanding", true)
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(infoCounter)
            quest:DisplayQuestInfo(true)
            quest:AddQuestInfoBarHealth(quest:GetThingWithScriptName("MeleeOpponent"), getActiveQuestName, "HUD_WHISPER_ICON", 1.0)
            while state:GetBool("HeroStanding") and state:GetBool("WhisperStanding") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
                fret_04 = quest:GetHealth(hero)
                if quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) <= fret_04 then
                    if quest:GetHealth(quest:GetThingWithScriptName("MeleeOpponent")) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                        if not quest:IsActiveThreadTerminating() then
                            state:SetBool("WhisperStanding", false)
                            conversationId5 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(conversationId5, hero)
                            quest:AddLineToConversation(conversationId5, "TEXT_QST_028_MAZE_FIGHT_OVER", me, hero, false)
                            goto FLOW_after_lab_00d59cc6
                        end
                        -- TODO(native): goto LAB_00d5a9a7
                    end
                else
                    state:SetBool("HeroStanding", false)
                    conversationId6 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId6, hero)
                    quest:AddLineToConversation(conversationId6, "TEXT_QST_028_MAZE_FIGHT_OVER", me, hero, false)
                end
                ::FLOW_after_lab_00d59cc6::
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
            quest:ClearAllRumbles()
            -- TODO(native): xStack_1d4 = (CCharString)(float)fret_06;
            scratchValue8 = 0
            -- TODO(native): xStack_1d4 = (CCharString)(float)(((float10)f_stk_74 - fret_07) - ((float10)f_stk_70 - (float10)(float)xStack_1d4));
            scratchValue7 = 0
            repeat
                scratchValue6 = scratchValue7
                if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_MeleeGrades, scratchValue8) < xStack_1d4 ~= (quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_MeleeGrades, scratchValue8) == xStack_1d4) then
                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
                    break
                end
                scratchValue7 = scratchValue6 + 1
                scratchValue8 = scratchValue8 + 1
            until scratchValue7 >= 7
            quest:ResetPlayerCreatureOnlyTarget()
            quest:SetStateInt("TutorialState", 7)
            quest:DisplayQuestInfo(false)
            resource3 = resources:NewResource()
            while not resources:TryAcquire(resource3, hero, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a997
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a997: (native jump target)
                -- TODO(native): this_01 = (CScriptGameResourceObjectMovieBase *)xStack_1d0;
                -- TODO(native): goto LAB_00d5a99e
            end
            resource2 = resources:NewResource()
            while not resources:TryAcquire(resource2, meleeOpponent, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a98b
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a98b: (native jump target)
                resources:ReleaseResource(resource2)
                -- TODO(native): goto LAB_00d5a997
            end
            quest:RemoveQuestInfoElement(infoCounter)
            scratchValue13 = resources:NewActorMap()
            resources:SetActor(scratchValue13, "HERO", resource3)
            resources:SetActor(scratchValue13, "TEACHER", resource5)
            resources:SetActor(scratchValue13, "THUNDER", resource4)
            resources:SetActor(scratchValue13, "WHISPER", resource2)
            scratchValue = resources:NewStringMap()
            repeat
                if scratchValue6 == 0 then
                    resources:SetString(scratchValue, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_APLUS")
                    break
                elseif scratchValue6 == 1 then
                    resources:SetString(scratchValue, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_A")
                    break
                elseif scratchValue6 == 2 then
                    resources:SetString(scratchValue, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_B")
                    break
                elseif scratchValue6 == 3 then
                    resources:SetString(scratchValue, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_C")
                    break
                elseif scratchValue6 == 4 then
                    resources:SetString(scratchValue, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_D")
                    break
                elseif scratchValue6 == 5 then
                    resources:SetString(scratchValue, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_E")
                    break
                elseif scratchValue6 == 6 then
                    resources:SetString(scratchValue, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_F")
                    break
                else
                    goto FLOW_native_label_1
                end
            until true
            ::FLOW_native_label_1::
            movie4 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            if not state:GetBool("WhisperStanding") then
                if not quest:IsActiveThreadTerminating() then resources:RunMacroWithStrings("CS_GUILD_MELEE_BATTLE_WON", scratchValue13, scratchValue, false, true); goto LAB_00d5a28a end
                -- LAB_00d5a948: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00d5a96a: (native jump target)
                resources:ReleaseResource(movie4)
                resources:DestroyStringMap(scratchValue)
                resources:DestroyActorMap(scratchValue13)
                -- TODO(native): goto LAB_00d5a98b
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a956: (native jump target)
                -- LAB_00d5a962: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): goto LAB_00d5a96a
            end
            resources:RunMacroWithStrings("CS_GUILD_MELEE_BATTLE_LOST", scratchValue13, scratchValue, false, true)
            ::LAB_00d5a28a::
            quest:ChangeHeroHealthBy(1000.0, true, false)
            quest:EntitySetInFaction(meleeOpponent, "FACTION_HERO")
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
                resources:RunMacro("CS_GUILD_MELEE_CONTINUE", scratchValue13, false, true)
                quest:SetStateBool("MeleeRepeating", false)
                quest:SetStateBool("MeleeRepeatKnown", true)
            else
                if false then return end  -- TODO(native): goto LAB_00d5a948
                resources:RunMacro("CS_GUILD_MELEE_REPEAT", scratchValue13, false, false)
                quest:SetStateBool("MeleeRepeating", true)
                quest:SetStateBool("MeleeRepeatKnown", true)
                if quest:IsObjectInThingsPossession("OBJECT_IRON_LONGSWORD", hero) then
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
            resources:ReleaseResource(movie4)
            resources:DestroyStringMap(scratchValue)
            resources:DestroyActorMap(scratchValue13)
            resources:DestroyMovie(resource2)
            resources:ReleaseResource(resource3)
            while not quest:GetStateBool("MeleeOpponentReset") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
            resources:ReleaseResource(resource4)
            quest:DeregisterTimer(timerId)
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
        quest:DeregisterTimer(timerId3)
    end
    ::FLOW_after_lab_00d5a9be::
    ::LAB_00d5a8d9::
    ::LAB_00d5a8e2::
    resources:ReleaseResource(resource5)
    do return end
    ::LAB_00d5933c::
    quest:DeregisterTimer(timerId3)
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

