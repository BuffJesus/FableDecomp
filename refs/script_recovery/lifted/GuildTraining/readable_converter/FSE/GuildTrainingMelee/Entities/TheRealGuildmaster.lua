-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_MeleeGrades = 3764,  -- '070000000000904100007041000020410000a040000000000000a0c0000030c1'
    GUI_MinHealth = 3800,  -- 6.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local heroStanding, whisperStanding, whisperEarly, whisperLate

-- TheRealGuildmaster.Main (retail 0x00d58490)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, fret_04, fret_06, fret_07, conversationId, scratchValue, scratchValue13
    local switch, getActiveQuestName, meleeOpponent, rivalHeroThunder, resource2, resource3
    local actorMap, resource4, resource5, actorMap2, infoCounter, resource6, timerId, timerId2
    local movie, movie2, movie3, actorMap3, actorMap4, resource7, resource8, resource9, actorMap5
    local movie4, resource, resource11, scratchValue16
    local function __cleanup_LAB_00d5a922()
        resources:ReleaseResource(resource)
        quest:DeregisterTimer(timerId)
        quest:DeregisterTimer(timerId2)
        resources:ReleaseResource(resource6)
    end
    local function __cleanup_LAB_00d5a9b5()
        quest:DeregisterTimer(timerId)
        quest:DeregisterTimer(timerId2)
        resources:ReleaseResource(resource6)
    end
    resource6 = resources:NewResource()
    while not resources:TryAcquire(resource6, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource6)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d5a8e2 end
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetAlwaysBlockAttacksFromThing(me, hero, true)
    quest:SetIsPushableByHero(me, false)
    meleeOpponent = quest:GetThingWithScriptName("MeleeOpponent")
    quest:SetThingHasInformation(me, false, false, false)
    timerId2 = quest:RegisterTimer()
    quest:SetTimer(timerId2, 0)
    scratchValue = quest:GetStateInt("TutorialState")
    scratchValue16 = 0
    while scratchValue == 1 do
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
                resource4 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource6)) then
                    me:Speak(hero, "TEXT_QST_028_GUILDMASTER_MELEE_HIT_WHISPER", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(resource4)
                            goto LAB_00d5933c
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(resource4)
                        goto LAB_00d5933c
                    end
                end
                quest:SetStateInt("TutorialState", 2)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(resource4)
            end
        else
            quest:SetStateInt("TutorialState", 2)
        end
        if not ((quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId2) < 1) and not me:IsPerformingScriptTask()) then scratchValue = quest:GetStateInt("TutorialState"); goto continue_3 end
        if quest:IsActiveThreadTerminating() then goto LAB_00d5933c end
        conversationId = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId, hero)
        quest:SetTimer(timerId2, 10)
        if scratchValue16 == nil then
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_FIRST", me, hero, false)
            scratchValue16 = 1
        elseif scratchValue16 == 1 then
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_SECOND", me, hero, false)
        end
        scratchValue = quest:GetStateInt("TutorialState")
        ::continue_3::
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId2)
    else
        quest:SetMasterGameState("HeroTakingGuildTest", true)
        quest:SetStateBool("WhisperStopWalking", true)
        quest:SetPlayerCreatureOnlyTarget(meleeOpponent)
        while quest:GetStateBool("MeleeRepeating") do
            if not quest:NewScriptFrame(me) then goto LAB_00d5933c end
            quest:SetStateInt("TutorialState", 2)
            quest:SetStateBool("MeleeRepeatKnown", false)
            resource4 = resources:NewResource()
            while not resources:TryAcquire(resource4, hero, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource4)
                    quest:DeregisterTimer(timerId2)
                    goto LAB_00d5a8d9
                end
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource4)
                quest:DeregisterTimer(timerId2)
                goto LAB_00d5a8d9
            end
            resource8 = resources:NewResource()
            while not resources:TryAcquire(resource8, meleeOpponent, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource8)
                    resources:ReleaseResource(resource4)
                    quest:DeregisterTimer(timerId2)
                    goto LAB_00d5a8d9
                end
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource8)
                resources:ReleaseResource(resource4)
                quest:DeregisterTimer(timerId2)
                goto LAB_00d5a8d9
            end
            actorMap3 = resources:NewActorMap()
            resources:SetActor(actorMap3, "HERO", resource4)
            resources:SetActor(actorMap3, "TEACHER", resource6)
            resources:SetActor(actorMap3, "WHISPER", resource8)
            movie2 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_ATTACK", actorMap3, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            resources:DestroyActorMap(actorMap3)
            resources:ReleaseResource(resource8)
            resources:ReleaseResource(resource4)
            quest:SetStateInt("GenericTutorialCounter", 0)
            quest:DisplayQuestInfo(true)
            infoCounter = quest:AddQuestInfoCounter("HUD_WHISPER_ICON", 7, 1.0)
            quest:SetStateInt("TutorialState", 3)
            timerId = quest:RegisterTimer()
            quest:SetTimer(timerId, 15)
            while quest:GetStateInt("GenericTutorialCounter") < 7 do
                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5a9b5(); return end
                if quest:GetTimer(timerId) < 1 then
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_HELP_ATTACK", me, hero, false)
                    quest:SetTimer(timerId, 15)
                end
                quest:UpdateQuestInfoCounter(infoCounter, quest:GetStateInt("GenericTutorialCounter"), -1)
                if quest:GetHealth(hero) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5a9b5(); return end
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_HEAL_HERO", me, hero, false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                end
            end
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5a9b5(); return end
            quest:ClearAllRumbles()
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(infoCounter)
            resource = resources:NewResource()
            while not resources:TryAcquire(resource, hero, 4) do
                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5a922(); return end
            end
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5a922(); return end
            resource7 = resources:NewResource()
            while not resources:TryAcquire(resource7, meleeOpponent, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a916
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a916: (native jump target)
                resources:ReleaseResource(resource7)
                __cleanup_LAB_00d5a922(); return
            end
            actorMap4 = resources:NewActorMap()
            resources:SetActor(actorMap4, "HERO", resource)
            resources:SetActor(actorMap4, "TEACHER", resource6)
            resources:SetActor(actorMap4, "WHISPER", resource7)
            movie = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_BLOCK", actorMap4, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap4)
            resources:ReleaseResource(resource7)
            resources:ReleaseResource(resource)
            quest:SetStateInt("GenericTutorialCounter", 0)
            if not quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5a9b5(); return end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        -- TODO(native): CSubtitleRenderer::SetText__atcbea81((CSubtitleRenderer *)xStack_f4);
                        goto LAB_00d5941c
                    end
                end
                __cleanup_LAB_00d5a9b5(); return
            end
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5a9b5(); return end
            quest:DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5a9b5(); return end
            end
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5a9b5(); return end
            ::LAB_00d5941c::
            quest:SetStateInt("TutorialState", 4)
            infoCounter = quest:AddQuestInfoCounter("HUD_WHISPER_ICON", 5, 1.0)
            quest:DisplayQuestInfo(true)
            while quest:GetStateInt("GenericTutorialCounter") < 5 do
                if not quest:NewScriptFrame(me) then __cleanup_LAB_00d5a9b5(); return end
                quest:UpdateQuestInfoCounter(infoCounter, quest:GetStateInt("GenericTutorialCounter"), -1)
                if quest:GetHealth(hero) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                    if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5a9b5(); return end
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_HEAL_HERO", me, hero, false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                end
            end
            if quest:IsActiveThreadTerminating() then __cleanup_LAB_00d5a9b5(); return end
            quest:ClearAllRumbles()
            quest:SetStateInt("TutorialState", 5)
            rivalHeroThunder = quest:CreateCreature("CREATURE_RIVAL_HERO_THUNDER", quest:GetThingWithScriptName("SkillApprenticeMarker"):GetPos(), "MeleeThunder")
            quest:EntitySetAppearanceMorphSeed(rivalHeroThunder, 1)
            resource5 = resources:NewResource()
            while not resources:TryAcquire(resource5, rivalHeroThunder, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a9a7: (native jump target)
                resources:ReleaseResource(resource5)
                __cleanup_LAB_00d5a9b5(); return
            end
            resource11 = resources:NewResource()
            while not resources:TryAcquire(resource11, hero, 4) do
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
            resource9 = resources:NewResource()
            while not resources:TryAcquire(resource9, meleeOpponent, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a933
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a933: (native jump target)
                resources:ReleaseResource(resource9)
                -- TODO(native): goto LAB_00d5a93f
            end
            actorMap5 = resources:NewActorMap()
            resources:SetActor(actorMap5, "HERO", resource11)
            resources:SetActor(actorMap5, "TEACHER", resource6)
            resources:SetActor(actorMap5, "THUNDER", resource5)
            resources:SetActor(actorMap5, "WHISPER", resource9)
            movie3 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_BATTLE", actorMap5, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:ChangeHeroHealthBy(1000.0, true, false)
            quest:ModifyThingHealth(meleeOpponent, 1000.0, false)
            quest:SetStateInt("TutorialState", 6)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            resources:DestroyActorMap(actorMap5)
            resources:ReleaseResource(resource9)
            resources:ReleaseResource(resource11)
            heroStanding = true
            whisperStanding = true
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(infoCounter)
            quest:DisplayQuestInfo(true)
            quest:AddQuestInfoBarHealth(quest:GetThingWithScriptName("MeleeOpponent"), getActiveQuestName, "HUD_WHISPER_ICON", 1.0)
            while heroStanding and whisperStanding do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
                fret_04 = quest:GetHealth(hero)
                if quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) <= fret_04 then
                    if quest:GetHealth(quest:GetThingWithScriptName("MeleeOpponent")) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                        if not quest:IsActiveThreadTerminating() then
                            whisperStanding = false
                            conversationId = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(conversationId, hero)
                            quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_FIGHT_OVER", me, hero, false)
                            goto FLOW_after_lab_00d59cc6
                        end
                        -- TODO(native): goto LAB_00d5a9a7
                    end
                else
                    heroStanding = false
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_MAZE_FIGHT_OVER", me, hero, false)
                end
                ::FLOW_after_lab_00d59cc6::
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
            quest:ClearAllRumbles()
            -- TODO(native): xStack_1d4 = (CCharString)(float)fret_06;
            scratchValue13 = 0
            -- TODO(native): xStack_1d4 = (CCharString)(float)(((float10)f_stk_74 - fret_07) - ((float10)f_stk_70 - (float10)(float)xStack_1d4));
            scratchValue = 0
            repeat
                conversationId = scratchValue
                if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_MeleeGrades, scratchValue13) < xStack_1d4 ~= (quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_MeleeGrades, scratchValue13) == xStack_1d4) then
                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
                    break
                end
                scratchValue = conversationId + 1
                scratchValue13 = scratchValue13 + 1
            until scratchValue >= 7
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
            actorMap2 = resources:NewActorMap()
            resources:SetActor(actorMap2, "HERO", resource3)
            resources:SetActor(actorMap2, "TEACHER", resource6)
            resources:SetActor(actorMap2, "THUNDER", resource5)
            resources:SetActor(actorMap2, "WHISPER", resource2)
            actorMap = resources:NewStringMap()
            switch = conversationId
            repeat
                if switch == 0 then
                    resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_APLUS")
                    break
                elseif switch == 1 then
                    resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_A")
                    break
                elseif switch == 2 then
                    resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_B")
                    break
                elseif switch == 3 then
                    resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_C")
                    break
                elseif switch == 4 then
                    resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_D")
                    break
                elseif switch == 5 then
                    resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_E")
                    break
                elseif switch == 6 then
                    resources:SetString(actorMap, "$GRADE", "TEXT_QST_028_GUILDMASTER_MELEE_TEST_GRADE_F")
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
            if not whisperStanding then
                if not quest:IsActiveThreadTerminating() then resources:RunMacroWithStrings("CS_GUILD_MELEE_BATTLE_WON", actorMap2, actorMap, false, true); goto LAB_00d5a28a end
                -- LAB_00d5a948: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00d5a96a: (native jump target)
                resources:DestroyMovie(movie4)
                resources:DestroyStringMap(actorMap)
                resources:DestroyActorMap(actorMap2)
                -- TODO(native): goto LAB_00d5a98b
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d5a956: (native jump target)
                -- LAB_00d5a962: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
                -- TODO(native): goto LAB_00d5a96a
            end
            resources:RunMacroWithStrings("CS_GUILD_MELEE_BATTLE_LOST", actorMap2, actorMap, false, true)
            ::LAB_00d5a28a::
            quest:ChangeHeroHealthBy(1000.0, true, false)
            quest:EntitySetInFaction(meleeOpponent, "FACTION_HERO")
            me:SetFriendsWithEverythingFlag(1)
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeThunder"), false, true)
            quest:Pause(2.0)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_MELEE_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
            scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
            while scratchValue < 0 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a956
                scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a948
            if scratchValue == 1 then
                if false then return end  -- TODO(native): goto LAB_00d5a956
                resources:RunMacro("CS_GUILD_MELEE_CONTINUE", actorMap2, false, true)
                quest:SetStateBool("MeleeRepeating", false)
                quest:SetStateBool("MeleeRepeatKnown", true)
            else
                if false then return end  -- TODO(native): goto LAB_00d5a948
                resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap2, false, false)
                quest:SetStateBool("MeleeRepeating", true)
                quest:SetStateBool("MeleeRepeatKnown", true)
                if quest:IsObjectInThingsPossession("OBJECT_IRON_LONGSWORD", hero) then
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): iVar7 = *xStack_23c
                        scratchValue = nil --[[unresolved native value]]
                        -- TODO(native): goto LAB_00d5a962
                    end
                    quest:TakeObjectFromHero("OBJECT_IRON_LONGSWORD")
                end
            end
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie4)
            resources:DestroyStringMap(actorMap)
            resources:DestroyActorMap(actorMap2)
            resources:ReleaseResource(resource2)
            resources:ReleaseResource(resource3)
            while not quest:GetStateBool("MeleeOpponentReset") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
            end
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d5a9a7
            resources:ReleaseResource(resource5)
            quest:DeregisterTimer(timerId)
        end
        if not quest:IsActiveThreadTerminating() then
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            quest:RemoveThing(quest:GetThingWithScriptName("MeleeOpponent"), false, true)
            me:MoveToPosition(quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos(), 3.0, 0, false, true)
            quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos(), "MeleeApprentice")
            quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", quest:GetThingWithScriptName("CombatApprenticeMarker"):GetPos(), "CombatApprentice")
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_04", "", "")
        end
        quest:DeregisterTimer(timerId2)
    end
    ::FLOW_after_lab_00d5a9be::
    ::LAB_00d5a8d9::
    ::LAB_00d5a8e2::
    resources:ReleaseResource(resource6)
    do return end
    ::LAB_00d5933c::
    quest:DeregisterTimer(timerId2)
    goto LAB_00d5a8d9
end

-- TheRealGuildmaster.Init (retail 0x00d56670)
function Init(quest, me)
    whisperEarly = false
    whisperLate = false
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

