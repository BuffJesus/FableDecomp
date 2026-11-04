-- Readable native conversion: TheRealGuildmaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_MeleeGrades = 3764,  -- '070000000000904100007041000020410000a040000000000000a0c0000030c1'
    GUI_MinHealth = 3800,  -- 6.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local heroStanding, whisperStanding, whisperEarly, whisperLate

-- TheRealGuildmaster.Main (retail 0x00d58490)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult6, predicateResult11, delay, readGlobalGameDataFloat, getHealth
    local scratchValue10, conversationId, scratchValue18, tutorialState, scratchValue20
    local questionAnswer, timerId, index, p4, resource, thing2, position, pppuVar, meleeOpponent
    local healthBar, rivalHeroWhisperTeenApprentice, guildEvilApprenticeMale, this_01, resource2
    local resource3, actorMap, resource4, resource5, actorMap2, infoCounter, resource6, timerId3
    local resource7, resource9, movie4, resource10, resource11, scratchValue22
    local function ReleaseEverything()
        resources:ReleaseResource(resource7)
        resources:ReleaseResource(resource10)
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource6)
    end
    local function ReleaseEverything2()
        resources:ReleaseResource(resource10)
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource6)
    end
    local function ReleaseEverything3()
        resources:ReleaseResource(resource9)
        local this_01 = resource11
        resources:ReleaseResource(this_01)
        resources:ReleaseResource(resource5)
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource6)
    end
    local function ReleaseEverything4()
        local this_01 = resource11
        resources:ReleaseResource(this_01)
        resources:ReleaseResource(resource5)
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource6)
    end
    local function ReleaseEverything5()
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie4)
        resources:DestroyStringMap(actorMap)
        resources:DestroyActorMap(actorMap2)
        resources:ReleaseResource(resource2)
        local this_01 = resource3
        resources:ReleaseResource(this_01)
        resources:ReleaseResource(resource5)
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource6)
    end
    local function ReleaseEverything6()
        resources:ReleaseResource(resource2)
        local this_01 = resource3
        resources:ReleaseResource(this_01)
        resources:ReleaseResource(resource5)
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource6)
    end
    local function ReleaseEverything7()
        local this_01 = resource3
        resources:ReleaseResource(this_01)
        resources:ReleaseResource(resource5)
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource6)
    end
    local function ReleaseEverything8()
        resources:ReleaseResource(resource5)
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource6)
    end
    local function ReleaseEverything9()
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource6)
    end
    resource6 = resources:NewResource()
    resources:PrepareResource(resource6)
    while not resources:TryAcquire(resource6, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource6)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d5a8e2 end
    quest:EntitySetAsKillable(me, false, true)
    thing2 = quest:GetHero()
    quest:EntitySetAlwaysBlockAttacksFromThing(me, thing2, true)
    quest:SetIsPushableByHero(me, false)
    meleeOpponent = quest:GetThingWithScriptName("MeleeOpponent")
    quest:SetThingHasInformation(me, false, false, false)
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    tutorialState = quest:GetStateInt("TutorialState")
    scratchValue22 = 0
    while tutorialState == 1 do
        quest:NewScriptFrame(me)
        local predicateResult5 = quest:IsActiveThreadTerminating()
        scratchValue10 = timerId
        if predicateResult5 then goto LAB_00d5933c end
        if not me:IsTalkedToByHero() then
            if not ((not quest:GetStateBool("EarlyHitWhisper")) or (not quest:GetStateBool("WhisperArrived"))) then goto LAB_00d586db end
            if me:MsgIsHitByHero() then goto LAB_00d586db end
            predicateResult6 = false
            goto FLOW_past_lab_00d586db
            ::LAB_00d586db::
            predicateResult6 = true
            ::FLOW_past_lab_00d586db::
            if predicateResult6 then
                local predicateResult7 = quest:IsActiveThreadTerminating()
                scratchValue10 = timerId
                if predicateResult7 then goto LAB_00d5933c end
                resource4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                actorMap2 = resources:ScriptThing(resource6)
                thing2 = actorMap2
                local fret_0 = quest:GetHealth(thing2)
                actorMap2 = nil
                if 0.0 < fret_0 then
                    local line = "TEXT_QST_028_GUILDMASTER_MELEE_HIT_WHISPER"
                    thing2 = quest:GetHero()
                    if not me:Speak(thing2, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(resource4)
                        goto LAB_00d5933c
                    end
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
                        scratchValue10 = timerId
                        goto LAB_00d5933c
                    end
                end
                quest:SetStateInt("TutorialState", 2)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(resource4)
            end
        else
            local predicateResult = quest:IsActiveThreadTerminating()
            scratchValue10 = timerId
            if predicateResult then goto LAB_00d5933c end
            quest:SetStateInt("TutorialState", 2)
        end
        readGlobalGameDataFloat = 5.5
        thing2 = quest:GetHero()
        local isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(thing2, me, 5.5) and quest:GetTimer(timerId) < 1
        local scratchValue = isDistanceBetweenThingsUnder and not me:IsPerformingScriptTask()
        if not scratchValue then scratchValue10 = timerId; tutorialState = quest:GetStateInt("TutorialState"); goto continue_3 end
        predicateResult11 = quest:IsActiveThreadTerminating()
        scratchValue10 = timerId
        if predicateResult11 then goto LAB_00d5933c end
        conversationId = quest:AddNewConversation(me, false, false)
        thing2 = quest:GetHero()
        quest:AddPersonToConversation(conversationId, thing2)
        quest:SetTimer(timerId, 10)
        if scratchValue22 == nil then
            thing2 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, thing2, false)
            thing2 = quest:GetHero()
            quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_FIRST", me, thing2, false)
            scratchValue22 = 1
        elseif scratchValue22 == 1 then
            thing2 = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, thing2, false)
            thing2 = quest:GetHero()
            quest:AddLineToConversation(conversationId, "TEXT_QST_028_GUILDMASTER_MELEE_COMMENT_SECOND", me, thing2, false)
        end
        scratchValue10 = timerId
        tutorialState = quest:GetStateInt("TutorialState")
        ::continue_3::
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
            resource4 = resources:NewResource()
            resources:PrepareResource(resource4)
            pppuVar = resource4
            thing2 = quest:GetHero()
            while not resources:TryAcquire(pppuVar, thing2, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource4)
                    quest:DeregisterTimer(timerId)
                    goto LAB_00d5a8d9
                end
                pppuVar = resource4
                thing2 = quest:GetHero()
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource4)
                quest:DeregisterTimer(timerId)
                goto LAB_00d5a8d9
            end
            local resource8 = resources:NewResource()
            resources:PrepareResource(resource8)
            while not resources:TryAcquire(resource8, meleeOpponent, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource8)
                    resources:ReleaseResource(resource4)
                    quest:DeregisterTimer(timerId)
                    goto LAB_00d5a8d9
                end
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource8)
                resources:ReleaseResource(resource4)
                quest:DeregisterTimer(timerId)
                goto LAB_00d5a8d9
            end
            local actorMap3 = resources:NewActorMap()
            resources:SetActor(actorMap3, "HERO", resource4)
            resources:SetActor(actorMap3, "TEACHER", resource6)
            resources:SetActor(actorMap3, "WHISPER", resource8)
            local movie2 = resources:StartMovie("")
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
            timerId3 = quest:RegisterTimer()
            quest:SetTimer(timerId3, 15)
            while quest:GetStateInt("GenericTutorialCounter") < 7 do
                if not quest:NewScriptFrame(me) then ReleaseEverything9(); return end
                if quest:GetTimer(timerId3) < 1 then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything9(); return end
                    local conversationId2 = quest:AddNewConversation(me, false, false)
                    thing2 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId2, thing2)
                    thing2 = quest:GetHero()
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_MAZE_HELP_ATTACK", me, thing2, false)
                    quest:SetTimer(timerId3, 15)
                end
                quest:UpdateQuestInfoCounter(infoCounter, quest:GetStateInt("GenericTutorialCounter"), -1)
                thing2 = quest:GetHero()
                if quest:GetHealth(thing2) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything9(); return end
                    local conversationId3 = quest:AddNewConversation(me, false, false)
                    thing2 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId3, thing2)
                    thing2 = quest:GetHero()
                    quest:AddLineToConversation(conversationId3, "TEXT_QST_028_MAZE_HEAL_HERO", me, thing2, false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                end
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything9(); return end
            quest:ClearAllRumbles()
            quest:DisplayQuestInfo(false)
            quest:RemoveQuestInfoElement(infoCounter)
            resource10 = resources:NewResource()
            resources:PrepareResource(resource10)
            resource = resource10
            thing2 = quest:GetHero()
            while not resources:TryAcquire(resource, thing2, 4) do
                if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                resource = resource10
                thing2 = quest:GetHero()
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
            resource7 = resources:NewResource()
            resources:PrepareResource(resource7)
            while not resources:TryAcquire(resource7, meleeOpponent, 4) do
                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            local actorMap4 = resources:NewActorMap()
            resources:SetActor(actorMap4, "HERO", resource10)
            resources:SetActor(actorMap4, "TEACHER", resource6)
            resources:SetActor(actorMap4, "WHISPER", resource7)
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_GUILD_MELEE_BLOCK", actorMap4, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:DestroyActorMap(actorMap4)
            resources:ReleaseResource(resource7)
            resources:ReleaseResource(resource10)
            quest:SetStateInt("GenericTutorialCounter", 0)
            if not quest:IsXbox() then
                if not quest:IsActiveThreadTerminating() then
                    quest:DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK_PC")
                    while not quest:MsgIsGameInfoClickedPast() do
                        if not quest:NewScriptFrame(me) then ReleaseEverything9(); return end
                    end
                    if not quest:IsActiveThreadTerminating() then quest:AddLogbookTutorialEntryPC("TEXT_QST_LOG_COMBAT_BLOCKING"); goto LAB_00d5941c end
                end
                ReleaseEverything9(); return
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything9(); return end
            quest:DisplayGameInfo("TEXT_QST_028_MELEE_INSTRUCTIONS_BLOCK")
            while not quest:MsgIsGameInfoClickedPast() do
                if not quest:NewScriptFrame(me) then ReleaseEverything9(); return end
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything9(); return end
            quest:AddLogbookTutorialEntry("TEXT_QST_LOG_COMBAT_BLOCKING")
            ::LAB_00d5941c::
            quest:SetStateInt("TutorialState", 4)
            infoCounter = quest:AddQuestInfoCounter("HUD_WHISPER_ICON", 5, 1.0)
            quest:DisplayQuestInfo(true)
            while quest:GetStateInt("GenericTutorialCounter") < 5 do
                if not quest:NewScriptFrame(me) then ReleaseEverything9(); return end
                quest:UpdateQuestInfoCounter(infoCounter, quest:GetStateInt("GenericTutorialCounter"), -1)
                thing2 = quest:GetHero()
                if quest:GetHealth(thing2) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything9(); return end
                    local conversationId4 = quest:AddNewConversation(me, false, false)
                    thing2 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId4, thing2)
                    thing2 = quest:GetHero()
                    quest:AddLineToConversation(conversationId4, "TEXT_QST_028_MAZE_HEAL_HERO", me, thing2, false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                end
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything9(); return end
            quest:ClearAllRumbles()
            quest:SetStateInt("TutorialState", 5)
            position = quest:GetThingWithScriptName("SkillApprenticeMarker"):GetPos()
            local rivalHeroThunder = quest:CreateCreature("CREATURE_RIVAL_HERO_THUNDER", position, "MeleeThunder", false)
            quest:EntitySetAppearanceMorphSeed(rivalHeroThunder, 1)
            resource5 = resources:NewResource()
            resources:PrepareResource(resource5)
            while not resources:TryAcquire(resource5, rivalHeroThunder, 4) do
                if not quest:NewScriptFrame(me) then ReleaseEverything8(); return end
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
            resource11 = resources:NewResource()
            resources:PrepareResource(resource11)
            resource = resource11
            thing2 = quest:GetHero()
            while not resources:TryAcquire(resource, thing2, 4) do
                if not quest:NewScriptFrame(me) then ReleaseEverything4(); return end
                resource = resource11
                thing2 = quest:GetHero()
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything4(); return end
            resource9 = resources:NewResource()
            resources:PrepareResource(resource9)
            while not resources:TryAcquire(resource9, meleeOpponent, 4) do
                if not quest:NewScriptFrame(me) then ReleaseEverything3(); return end
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything3(); return end
            local actorMap5 = resources:NewActorMap()
            resources:SetActor(actorMap5, "HERO", resource11)
            resources:SetActor(actorMap5, "TEACHER", resource6)
            resources:SetActor(actorMap5, "THUNDER", resource5)
            resources:SetActor(actorMap5, "WHISPER", resource9)
            local movie3 = resources:StartMovie("")
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
            readGlobalGameDataFloat = 1.0
            thing2 = quest:GetThingWithScriptName("MeleeOpponent")
            healthBar = quest:AddQuestInfoBarHealth(thing2, {R = 255, G = 255, B = 255, A = 255}, "HUD_WHISPER_ICON", 1.0)
            thing2 = quest:GetHero()
            local health = quest:GetHealth(thing2)
            local health2 = quest:GetHealth(meleeOpponent)
            while heroStanding and whisperStanding do
                if not quest:NewScriptFrame(me) then ReleaseEverything8(); return end
                thing2 = quest:GetHero()
                local fret_04 = quest:GetHealth(thing2)
                if quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) <= fret_04 then
                    thing2 = quest:GetThingWithScriptName("MeleeOpponent")
                    if quest:GetHealth(thing2) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                        if not quest:IsActiveThreadTerminating() then
                            whisperStanding = false
                            local conversationId5 = quest:AddNewConversation(me, false, false)
                            thing2 = quest:GetHero()
                            quest:AddPersonToConversation(conversationId5, thing2)
                            thing2 = quest:GetHero()
                            quest:AddLineToConversation(conversationId5, "TEXT_QST_028_MAZE_FIGHT_OVER", me, thing2, false)
                            goto LAB_00d59cc6
                        end
                        ReleaseEverything8(); return
                    end
                else
                    if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
                    heroStanding = false
                    local conversationId6 = quest:AddNewConversation(me, false, false)
                    thing2 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId6, thing2)
                    thing2 = quest:GetHero()
                    quest:AddLineToConversation(conversationId6, "TEXT_QST_028_MAZE_FIGHT_OVER", me, thing2, false)
                end
                ::LAB_00d59cc6::
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
            quest:ClearAllRumbles()
            thing2 = quest:GetHero()
            getHealth = quest:GetHealth(thing2)
            local fret_07 = quest:GetHealth(meleeOpponent)
            index = 0
            getHealth = string.unpack("<f", string.pack("<f", (health2 - fret_07) - (health - getHealth)))
            scratchValue20 = 0
            repeat
                scratchValue18 = scratchValue20
                if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_MeleeGrades, index) < getHealth ~= (quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_MeleeGrades, index) == getHealth) then
                    if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
                    break
                end
                scratchValue20 = scratchValue18 + 1
                index = index + 1
            until not (scratchValue20 < 7)
            quest:ResetPlayerCreatureOnlyTarget()
            quest:SetStateInt("TutorialState", 7)
            quest:DisplayQuestInfo(false)
            resource3 = resources:NewResource()
            resources:PrepareResource(resource3)
            while not resources:TryAcquire(resource3, quest:GetHero(), 4) do
                if not quest:NewScriptFrame(me) then ReleaseEverything7(); return end
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything7(); return end
            resource2 = resources:NewResource()
            resources:PrepareResource(resource2)
            while not resources:TryAcquire(resource2, meleeOpponent, 4) do
                if not quest:NewScriptFrame(me) then ReleaseEverything6(); return end
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything6(); return end
            quest:RemoveQuestInfoElement(infoCounter)
            actorMap2 = resources:NewActorMap()
            resources:SetActor(actorMap2, "HERO", resource3)
            resources:SetActor(actorMap2, "TEACHER", resource6)
            resources:SetActor(actorMap2, "THUNDER", resource5)
            resources:SetActor(actorMap2, "WHISPER", resource2)
            actorMap = resources:NewStringMap()
            local switch = scratchValue18
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
                    break
                end
            until true
            movie4 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            if not whisperStanding then
                if not quest:IsActiveThreadTerminating() then
                    resources:RunMacroWithStrings("CS_GUILD_MELEE_BATTLE_WON", actorMap2, actorMap, false, true)
                else
                    ReleaseEverything5()
                    return
                end
            else
                if quest:IsActiveThreadTerminating() then ReleaseEverything5(); return end
                resources:RunMacroWithStrings("CS_GUILD_MELEE_BATTLE_LOST", actorMap2, actorMap, false, true)
            end
            quest:ChangeHeroHealthBy(1000.0, true, false)
            quest:EntitySetInFaction(meleeOpponent, "FACTION_HERO")
            me:SetFriendsWithEverythingFlag(true)
            thing2 = quest:GetThingWithScriptName("MeleeThunder")
            quest:RemoveThing(thing2, false, true)
            quest:Pause(2.0)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_MELEE_REPEAT_QUESTION", "TEXT_OBJECT_HERO_ANSWER_CONTINUE", "TEXT_OBJECT_HERO_ANSWER_RETAKE", "", true)
            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer < 0 do
                if not quest:NewScriptFrame(me) then ReleaseEverything5(); return end
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything5(); return end
            local predicateResult33 = quest:IsActiveThreadTerminating()
            if questionAnswer == 1 then
                if predicateResult33 then ReleaseEverything5(); return end
                resources:RunMacro("CS_GUILD_MELEE_CONTINUE", actorMap2, false, true)
                quest:SetStateBool("MeleeRepeating", false)
                quest:SetStateBool("MeleeRepeatKnown", true)
            else
                if predicateResult33 then ReleaseEverything5(); return end
                resources:RunMacro("CS_GUILD_MELEE_REPEAT", actorMap2, false, false)
                quest:SetStateBool("MeleeRepeating", true)
                quest:SetStateBool("MeleeRepeatKnown", true)
                thing2 = quest:GetHero()
                if quest:IsObjectInThingsPossession("OBJECT_IRON_LONGSWORD", thing2) then
                    if quest:IsActiveThreadTerminating() then
                        -- reviewed native saved-table staging: iVar7 = *xStack_23c
                        questionAnswer = nil --[[reviewed native interface-table staging]]
                        ReleaseEverything5(); return
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
                if not quest:NewScriptFrame(me) then ReleaseEverything8(); return end
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything8(); return end
            resources:ReleaseResource(resource5)
            quest:DeregisterTimer(timerId3)
            scratchValue10 = timerId
        end
        if not quest:IsActiveThreadTerminating() then
            quest:SetMasterGameState("HeroTakingGuildTest", false)
            local thing = quest:GetThingWithScriptName("MeleeOpponent")
            quest:RemoveThing(thing, false, true)
            p4 = 1
            position = quest:GetThingWithScriptName("M_GuildmasterMarker"):GetPos()
            me:MoveToPosition(position, 3.0, ENTITY_MOVE_WALK, false, true)
            position = quest:GetThingWithScriptName("MeleeApprenticeMarker"):GetPos()
            rivalHeroWhisperTeenApprentice = quest:CreateCreature("CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE", position, "MeleeApprentice", false)
            position = quest:GetThingWithScriptName("CombatApprenticeMarker"):GetPos()
            guildEvilApprenticeMale = quest:CreateCreature("CREATURE_GUILD_EVIL_APPRENTICE_MALE", position, "CombatApprentice", false)
            delay = 0
            local getActiveQuestName = quest:GetActiveQuestName()
            quest:DeactivateQuestLater(getActiveQuestName, 0)
            quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_04", "", "")
        end
        quest:DeregisterTimer(timerId)
    end
    ::LAB_00d5a8d9::
    ::LAB_00d5a8e2::
    resources:ReleaseResource(resource6)
    do return end
    ::LAB_00d5933c::
    quest:DeregisterTimer(timerId)
    goto LAB_00d5a8d9
end

-- TheRealGuildmaster.Init (retail 0x00d56670)
function Init(quest, me)
    whisperEarly = false
    whisperLate = false
end

-- TheRealGuildmaster.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TheRealGuildmaster.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

