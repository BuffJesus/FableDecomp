-- Readable native conversion: CombatApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

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

-- CombatApprentice.Main (retail 0x00d4a270)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue2, scratchValue3, scratchValue4, scratchValue5, scratchValue6, dist
    local scratchValue8, scratchValue9, fret_10, scratchValue, scratchValue14, scratchValue15
    local sequence1, switch, p0, combatApprenticeTargetMarker, meleeApprentice, resource, resource3
    local movie, actorMap, resource4, timerId, scratchValue19, movie2, movie3, movie4, movie5
    local movie6, movie7, addQuestInfoBarHealth
    local function __cleanup_LAB_00d4c4a5()
        resources:ReleaseResource(resource4)
    end
    resource4 = resources:NewResource()
    scratchValue2 = resources:TryAcquire(resource4, me, 4)
    while not scratchValue2 do
        if not quest:NewScriptFrame(me) then __cleanup_LAB_00d4c4a5(); return end
        scratchValue2 = resources:TryAcquire(resource4, me, 4)
    end
    if quest:IsActiveThreadTerminating() then resources:DestroyMovie(resource4); return end
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(me)
    scratchValue = quest:RegisterTimer()
    scratchValue19 = scratchValue
    quest:SetTimer(scratchValue, 10)
    combatApprenticeTargetMarker = quest:GetThingWithScriptName("CombatApprenticeTargetMarker")
    scratchValue5 = 0
    scratchValue2 = quest:IsActiveThreadTerminating()
    repeat
        if scratchValue2 then
            quest:DeregisterTimer(scratchValue)
            __cleanup_LAB_00d4c4a5()
            return
        end
        if not quest:IsQuestActive("Q_GuildTrainingDeparture") then goto LAB_00d4a512 end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue)
            resources:DestroyMovie(resource4)
            return
        end
        if ((quest:GetMasterGameState("GlobalMeleeGrade") < 4) and (quest:GetMasterGameState("GlobalSkillGrade") < 4)) and quest:GetMasterGameState("GlobalWillGrade") < 4 then
            if not quest:IsActiveThreadTerminating() then
                if scratchValue5 == 0 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(scratchValue19)
                        resources:DestroyMovie(resource4)
                        return
                    end
                    quest:SetThingHasInformation(me, false, true, false)
                    scratchValue5 = 1
                end
                goto LAB_00d4a512
            end
            quest:DeregisterTimer(scratchValue19)
            resources:DestroyMovie(resource4)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue)
            resources:DestroyMovie(resource4)
            return
        end
        if scratchValue5 ~= 0 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue19)
                resources:DestroyMovie(resource4)
                return
            end
            quest:ClearThingHasInformation(me)
            scratchValue5 = 0
        end
        ::LAB_00d4a512::
        if not quest:IsDistanceBetweenThingsOver(me, combatApprenticeTargetMarker, 4.0) or me:IsPerformingScriptTask() then
            scratchValue = me:IsPerformingScriptTask()
            if scratchValue then goto LAB_00d4a71c end
            dist = 10.0
            sequence1 = not quest:IsDistanceBetweenThingsUnder(me, hero, 10.0)
            if not sequence1 then
                scratchValue = quest:GetTimer(scratchValue19)
                sequence1 = 0 < scratchValue
            end
            if sequence1 then goto LAB_00d4a71c end
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                quest:SetTimer(scratchValue19, 20)
                scratchValue14 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue14, hero)
                scratchValue = quest:GetMasterGameState("GlobalMeleeGrade")
                if scratchValue == 0 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_COMMENT", me, hero, false); goto LAB_00d4a71c end
                elseif scratchValue == 7 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_APPRENTICE_MELEE_APLUS_COMMENT", me, hero, false); goto LAB_00d4a71c end
                elseif not quest:IsActiveThreadTerminating() then
                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_APPRENTICE_MELEE_NOT_APLUS_COMMENT", me, hero, false)
                    goto LAB_00d4a71c
                end
            end
            quest:DeregisterTimer(scratchValue19)
            resources:DestroyMovie(resource4)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(dist)
            resources:DestroyMovie(resource4)
            return
        end
        if combatApprenticeTargetMarker ~= nil and not combatApprenticeTargetMarker:IsNull() then
            p0 = combatApprenticeTargetMarker:GetPos()
        end
        me:MoveToPosition(p0, 3.0, 1, false, true)
        ::LAB_00d4a71c::
        if state:GetBool("WaitingForFight") and me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue19)
                resources:DestroyMovie(resource4)
                return
            end
            scratchValue = me:IsPerformingScriptTask()
            if scratchValue then
                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6da end
                me:ClearCommands()
                movie6 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                    me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_EARLY", 0, false, true, false)
                    scratchValue = me:IsPerformingScriptTask()
                    scratchValue3 = scratchValue
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie6)
                            goto LAB_00d4c6da
                        end
                        scratchValue = me:IsPerformingScriptTask()
                        scratchValue3 = scratchValue
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie6)
                        goto LAB_00d4c6da
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie6)
                goto LAB_00d4c416
            end
            if not quest:IsActiveThreadTerminating() then
                if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                    if not quest:IsActiveThreadTerminating() then
                        movie5 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                            me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_OTHER_MELEE_GRADE", 0, false, true, false)
                            scratchValue = me:IsPerformingScriptTask()
                            scratchValue3 = scratchValue
                            while scratchValue3 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:ReleaseResource(movie5)
                                    goto LAB_00d4c6da
                                end
                                scratchValue = me:IsPerformingScriptTask()
                                scratchValue3 = scratchValue
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(movie5)
                                goto LAB_00d4c6da
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(movie5)
                        goto LAB_00d4c416
                    end
                    goto LAB_00d4c6da
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6da end
                meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
                scratchValue = meleeApprentice ~= nil and meleeApprentice:IsAlive()
                if not scratchValue then
                    if not quest:IsActiveThreadTerminating() then
                        movie4 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                            me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_NO_WHISPER", 0, false, true, false)
                            scratchValue = me:IsPerformingScriptTask()
                            scratchValue3 = scratchValue
                            while scratchValue3 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:ReleaseResource(movie4)
                                    goto LAB_00d4c6d1
                                end
                                scratchValue = me:IsPerformingScriptTask()
                                scratchValue3 = scratchValue
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(movie4)
                                goto LAB_00d4c6d1
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        goto LAB_00d4c416
                    end
                else
                    movie = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                        me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_HELLO", 0, false, true, false)
                        scratchValue3 = me:IsPerformingScriptTask()
                        while scratchValue3 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d4c5ff end
                            scratchValue3 = me:IsPerformingScriptTask()
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00d4ac95 end
                        ::LAB_00d4c5ff::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d4c6d1
                    end
                    ::LAB_00d4ac95::
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                    while scratchValue < 0 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00d4c6d1
                        end
                        scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d4c6d1
                    end
                    if scratchValue == 0 then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00d4c6d1
                        end
                        scratchValue6 = 1
                        if quest:GetHealth(resources:ScriptThing(resource4)) <= 0.0 then
                            scratchValue6 = scratchValue
                        end
                        if scratchValue6 ~= 0 then
                            me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_RETURN", 0, false, true, false)
                            scratchValue3 = me:IsPerformingScriptTask()
                            while scratchValue3 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie)
                                    goto LAB_00d4c6d1
                                end
                                scratchValue3 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00d4c6d1
                            end
                        end
                    else
                        if scratchValue ~= 1 then goto LAB_00d4b11f end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00d4c6d1
                        end
                        scratchValue6 = scratchValue
                        if quest:GetHealth(resources:ScriptThing(resource4)) <= 0.0 then
                            scratchValue6 = 0
                        end
                        if scratchValue6 ~= 0 then
                            me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_START", 0, false, true, false)
                            scratchValue3 = me:IsPerformingScriptTask()
                            while scratchValue3 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie)
                                    goto LAB_00d4c6d1
                                end
                                scratchValue3 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00d4c6d1
                            end
                        end
                        if quest:GetMasterGameState("GlobalMeleeGrade") ~= 7 then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00d4c6d1
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                                me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_START_APLUS", 0, false, true, false)
                                scratchValue3 = me:IsPerformingScriptTask()
                                while scratchValue3 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie)
                                        goto LAB_00d4c6d1
                                    end
                                    scratchValue3 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie)
                                    goto LAB_00d4c6d1
                                end
                            end
                        end
                        quest:FadeScreenOut(0.5, 0.5)
                        quest:Pause(1.0)
                        quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("HeroMeleeStart"), false)
                        quest:EntityTeleportToThing(meleeApprentice, quest:GetThingWithScriptName("WhisperMeleeStart"), false)
                        quest:EntityUnsheatheMeleeWeapon(hero, false)
                        quest:EntityUnsheatheWeapons(meleeApprentice, false)
                    end
                    ::LAB_00d4b11f::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    if scratchValue ~= 1 then
                        goto LAB_00d4c416
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4c6d1 end
                    quest:FadeScreenIn()
                    quest:SetPlayerCreatureOnlyTarget(meleeApprentice)
                    quest:SetMasterGameState("HeroTakingGuildTest", true)
                    quest:SetStateBool("StartedMeleeTesting", true)
                    state:SetBool("WaitingForFight", false)
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                    quest:ModifyThingHealth(meleeApprentice, 1000.0, false)
                    quest:EntitySetAsKillable(meleeApprentice, false, true)
                    quest:EntitySetCombatType(meleeApprentice, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE")
                    quest:EntitySetInFaction(meleeApprentice, "FACTION_BANDITS")
                    if meleeApprentice ~= nil and not meleeApprentice:IsNull() then
                        meleeApprentice:SetFriendsWithEverythingFlag(0)
                    end
                    quest:GiveThingBestEnemyTarget(meleeApprentice, hero)
                    quest:SetStateBool("FightFinished", false)
                    timerId = quest:RegisterTimer()
                    scratchValue14 = timerId
                    quest:SetTimer(timerId, 15)
                    quest:DisplayQuestInfo(true)
                    addQuestInfoBarHealth = quest:AddQuestInfoBarHealth(meleeApprentice, {R = 255, G = 255, B = 255, A = 255}, "HUD_WHISPER_ICON", 1.0)
                    scratchValue9 = quest:GetHealth(hero)
                    scratchValue8 = quest:GetHealth(meleeApprentice)
                    scratchValue3 = quest:GetStateBool("FightFinished")
                    scratchValue6 = 0
                    scratchValue4 = 0
                    while not scratchValue3 do
                        if not quest:NewScriptFrame(me) then goto LAB_00d4c6c8 end
                        if quest:GetMasterGameState("GuildWarningOccuring") == 1 then
                            scratchValue4 = 1
                            scratchValue6 = 1
                            quest:SetStateBool("FightFinished", true)
                        end
                        if not (meleeApprentice ~= nil and not meleeApprentice:IsNull()) then
                            scratchValue3 = 0
                        else
                            scratchValue3 = meleeApprentice:MsgIsHitByHeroWithProjectileWeapon()
                        end
                        if not scratchValue3 then
                            if not (meleeApprentice ~= nil and not meleeApprentice:IsNull()) then
                                scratchValue3 = 0
                            else
                                scratchValue3 = meleeApprentice:MsgIsHitByHeroSpecialAbility(11)
                            end
                            if scratchValue3 then
                                quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                                if meleeApprentice ~= nil and not meleeApprentice:IsNull() then
                                    meleeApprentice:SetFriendsWithEverythingFlag(1)
                                end
                                movie3 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                                    me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_NO_LIGHTNING", 0, false, true, false)
                                    scratchValue = me:IsPerformingScriptTask()
                                    scratchValue3 = scratchValue
                                    while scratchValue3 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(movie3)
                                            goto LAB_00d4c6c8
                                        end
                                        scratchValue = me:IsPerformingScriptTask()
                                        scratchValue3 = scratchValue
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie3)
                                        goto LAB_00d4c6c8
                                    end
                                end
                                quest:SetStateBool("FightFinished", true)
                                scratchValue6 = 1
                                quest:PauseAllNonScriptedEntities(false)
                                -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_bc;
                                resources:DestroyMovie(this_00)
                                goto FLOW_after_lab_00d4b6f6
                            end
                        else
                            quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                            if meleeApprentice ~= nil and not meleeApprentice:IsNull() then
                                meleeApprentice:SetFriendsWithEverythingFlag(1)
                            end
                            movie2 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                                me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_NO_BOW", 0, false, true, false)
                                scratchValue = me:IsPerformingScriptTask()
                                scratchValue3 = scratchValue
                                while scratchValue3 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie2)
                                        goto LAB_00d4c6c8
                                    end
                                    scratchValue = me:IsPerformingScriptTask()
                                    scratchValue3 = scratchValue
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    goto LAB_00d4c6c8
                                end
                            end
                            quest:SetStateBool("FightFinished", true)
                            scratchValue6 = 1
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_ac;
                            resources:DestroyMovie(this_00)
                        end
                        ::FLOW_after_lab_00d4b6f6::
                        fret_10 = quest:GetHealth(hero)
                        if quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) <= fret_10 then
                            if quest:GetHealth(meleeApprentice) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                                quest:SetStateBool("FightFinished", true)
                            end
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                            quest:SetStateBool("FightFinished", true)
                        end
                        if not (meleeApprentice ~= nil and not meleeApprentice:IsNull()) then
                            scratchValue3 = 0
                        else
                            scratchValue3 = meleeApprentice:MsgIsHitByHero()
                        end
                        if not scratchValue3 then
                            if quest:IsPlayerCreatureBlocking() then
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                if not hero:MsgIsHitBy("MeleeOpponent") then
                                    scratchValue2 = false
                                    goto FLOW_after_lab_00d4b8c5
                                end
                                scratchValue2 = true
                            else
                                scratchValue2 = false
                            end
                            ::FLOW_after_lab_00d4b8c5::
                            if scratchValue2 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                                scratchValue = quest:GetTimer(scratchValue14)
                                if scratchValue < 1 then
                                    if not quest:IsActiveThreadTerminating() then
                                        scratchValue14 = quest:AddNewConversation(meleeApprentice, false, false)
                                        quest:AddPersonToConversation(scratchValue14, hero)
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", meleeApprentice, hero, false)
                                        quest:SetTimer(timerId, 15)
                                        goto FLOW_after_lab_00d4b857
                                    end
                                    goto LAB_00d4c6c8
                                end
                            else
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                if hero:MsgIsHitBy("MeleeOpponent") then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                                    scratchValue = quest:GetTimer(timerId)
                                    if scratchValue < 1 then
                                        scratchValue14 = quest:AddNewConversation(meleeApprentice, false, false)
                                        quest:AddPersonToConversation(scratchValue14, hero)
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", meleeApprentice, hero, false)
                                        quest:SetTimer(timerId, 15)
                                    end
                                end
                            end
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                            scratchValue = quest:GetTimer(scratchValue14)
                            if scratchValue < 9 then
                                scratchValue14 = quest:AddNewConversation(meleeApprentice, false, false)
                                quest:AddPersonToConversation(scratchValue14, hero)
                                quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", meleeApprentice, hero, false)
                                quest:SetTimer(timerId, 15)
                            end
                        end
                        ::FLOW_after_lab_00d4b857::
                        scratchValue3 = quest:GetStateBool("FightFinished")
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                    quest:ResetPlayerCreatureOnlyTarget()
                    quest:RemoveQuestInfoElement(addQuestInfoBarHealth)
                    quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                    if meleeApprentice ~= nil and not meleeApprentice:IsNull() then
                        meleeApprentice:SetFriendsWithEverythingFlag(1)
                    end
                    quest:DisplayQuestInfo(false)
                    if scratchValue6 == 1 then
                        if not quest:IsActiveThreadTerminating() then
                            if scratchValue4 ~= 1 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                                quest:FadeScreenOut(0.5, 0.5)
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                                quest:Pause(1.0)
                                quest:FadeScreenOut(0.5, 0.5)
                                quest:SheatheHeroWeapons()
                                quest:EntitySheatheWeapons(meleeApprentice, false)
                                quest:Pause(1.0)
                            end
                            quest:ChangeHeroHealthBy(1000.0, true, false)
                            quest:ModifyThingHealth(meleeApprentice, 1000.0, false)
                            quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName("M_MeleeHeroStand"), false)
                            quest:EntityTeleportToThing(meleeApprentice, quest:GetThingWithScriptName("M_MeleeOpponentStand"), false)
                            quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                            quest:FadeScreenIn()
                            state:SetBool("WaitingForFight", true)
                            quest:SetStateBool("StartedMeleeTesting", false)
                            quest:SetMasterGameState("HeroTakingGuildTest", false)
                            quest:DeregisterTimer(timerId)
                            goto LAB_00d4c416
                        end
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                        scratchValue15 = 0
                        scratchValue9 = (scratchValue8 - quest:GetHealth(meleeApprentice)) - (scratchValue9 - quest:GetHealth(hero))
                        scratchValue = 0
                        repeat
                            scratchValue14 = scratchValue
                            if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_MeleeGrades, scratchValue15) <= scratchValue9 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                                break
                            end
                            scratchValue15 = scratchValue15 + 1
                            scratchValue = scratchValue14 + 1
                        until not (scratchValue14 + 1 < 7)
                        resource3 = resources:NewResource()
                        scratchValue2 = resources:TryAcquire(resource3, meleeApprentice, 4)
                        while not scratchValue2 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d4c6bf end
                            scratchValue2 = resources:TryAcquire(resource3, meleeApprentice, 4)
                        end
                        if not quest:IsActiveThreadTerminating() then
                            resource = resources:NewResource()
                            scratchValue2 = resources:TryAcquire(resource, hero, 4)
                            while not scratchValue2 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d4c6b3 end
                                scratchValue2 = resources:TryAcquire(resource, hero, 4)
                            end
                            if not quest:IsActiveThreadTerminating() then
                                actorMap = resources:NewActorMap()
                                resources:SetActor(actorMap, "ME", resource4)
                                resources:SetActor(actorMap, "HERO", resource)
                                resources:SetActor(actorMap, "WHISPER", resource3)
                                movie7 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_END", actorMap, false, true)
                                resources:SetActor(actorMap, "ME", resource4)
                                switch = scratchValue14
                                repeat
                                    if switch == 0 then
                                        if quest:GetMasterGameState("GlobalMeleeGrade") ~= 7 then
                                            if not quest:IsActiveThreadTerminating() then
                                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_APLUS_PRIZE", actorMap, false, true)
                                                quest:ClearThingHasInformation(me)
                                                goto FLOW_native_label_1
                                            end
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(movie7)
                                            resources:DestroyActorMap(actorMap)
                                            goto LAB_00d4c6b3
                                        end
                                        if not quest:IsActiveThreadTerminating() then resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_APLUS", actorMap, false, true); break end
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie7)
                                        resources:DestroyActorMap(actorMap)
                                        goto LAB_00d4c6b3
                                    elseif switch == 1 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_A", actorMap, false, true)
                                        break
                                    elseif switch == 2 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_B", actorMap, false, true)
                                        break
                                    elseif switch == 3 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_C", actorMap, false, true)
                                        break
                                    elseif switch == 4 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_D", actorMap, false, true)
                                        break
                                    elseif switch == 5 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_E", actorMap, false, true)
                                        break
                                    elseif switch == 6 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_F", actorMap, false, true)
                                        break
                                    else
                                        goto FLOW_native_label_1
                                    end
                                until true
                                ::FLOW_native_label_1::
                                if quest:GetMasterGameState("GlobalMeleeGrade") < 7 - scratchValue14 then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie7)
                                        resources:DestroyActorMap(actorMap)
                                        goto LAB_00d4c6b3
                                    end
                                    quest:SetMasterGameState("GlobalMeleeGrade", 7 - scratchValue14)
                                end
                                resources:SetActor(actorMap, "ME", resource4)
                                resources:SetActor(actorMap, "HERO", resource)
                                resources:SetActor(actorMap, "WHISPER", resource3)
                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_OVER", actorMap, false, true)
                                quest:FixMovieSequenceCamera(false)
                                state:SetBool("WaitingForFight", true)
                                quest:ChangeHeroHealthBy(1000.0, true, false)
                                quest:ModifyThingHealth(meleeApprentice, 1000.0, false)
                                quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie7)
                                resources:DestroyActorMap(actorMap)
                                resources:DestroyMovie(resource)
                                resources:ReleaseResource(resource3)
                                quest:SetStateBool("StartedMeleeTesting", false)
                                quest:SetMasterGameState("HeroTakingGuildTest", false)
                                quest:DeregisterTimer(timerId)
                                goto LAB_00d4c416
                            end
                            ::LAB_00d4c6b3::
                            resources:ReleaseResource(resource)
                        end
                        ::LAB_00d4c6bf::
                        resources:ReleaseResource(resource3)
                    end
                    ::LAB_00d4c6c8::
                    quest:DeregisterTimer(timerId)
                end
                ::LAB_00d4c6d1::
            end
            ::LAB_00d4c6da::
            quest:DeregisterTimer(scratchValue19)
            resources:DestroyMovie(resource4)
            return
        end
        ::LAB_00d4c416::
        quest:NewScriptFrame(me)
        scratchValue2 = quest:IsActiveThreadTerminating()
    until false
end

-- CombatApprentice.Init (retail 0x00d41ae0)
function Init(quest, me)
    state:SetBool("WaitingForFight", true)
end

-- CombatApprentice.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- CombatApprentice.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

