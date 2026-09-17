-- Readable native conversion: CombatApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

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
    local resources = quest:RetailResources()
    local scratchValue2, scratchValue3, scratchValue4, scratchValue5, scratchValue6, dist
    local scratchValue8, scratchValue9, fret_10, scratchValue13, scratchValue14, scratchValue15
    local sequence1, switch2, p0, scratchValue17, scratchValue19, scratchValue20, meleeApprentice
    local scratchValue22, scratchValue23, scratchValue24, scratchValue25, scratchValue26
    local scratchValue27, timerId, scratchValue28, scratchValue29, scratchValue30, scratchValue31
    local scratchValue32, scratchValue33, scratchValue34, scratchValue36
    local function __cleanup_LAB_00d4c4a5()
        resources:ReleaseResource(scratchValue27)
    end
    scratchValue22 = 0
    scratchValue27 = resources:NewResource()
    scratchValue2 = resources:TryAcquire(scratchValue27, me, 4)
    while not scratchValue2 do
        if not quest:NewScriptFrame(me) then __cleanup_LAB_00d4c4a5(); return end
        scratchValue2 = resources:TryAcquire(scratchValue27, me, 4)
    end
    if quest:IsActiveThreadTerminating() then resources:DestroyMovie(scratchValue27); return end
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(me)
    scratchValue13 = quest:RegisterTimer()
    scratchValue28 = scratchValue13
    quest:SetTimer(scratchValue13, 10)
    scratchValue20 = quest:GetThingWithScriptName("CombatApprenticeTargetMarker")
    scratchValue5 = 0
    scratchValue2 = quest:IsActiveThreadTerminating()
    repeat
        if scratchValue2 then
            quest:DeregisterTimer(scratchValue13)
            __cleanup_LAB_00d4c4a5()
            return
        end
        if not quest:IsQuestActive("Q_GuildTrainingDeparture") then goto LAB_00d4a512 end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue13)
            resources:DestroyMovie(scratchValue27)
            return
        end
        if ((quest:GetMasterGameState("GlobalMeleeGrade") < 4) and (quest:GetMasterGameState("GlobalSkillGrade") < 4)) and quest:GetMasterGameState("GlobalWillGrade") < 4 then
            if not quest:IsActiveThreadTerminating() then
                if scratchValue5 == 0 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(scratchValue28)
                        resources:DestroyMovie(scratchValue27)
                        return
                    end
                    quest:SetThingHasInformation(me, false, true, false)
                    scratchValue5 = 1
                end
                goto LAB_00d4a512
            end
            quest:DeregisterTimer(scratchValue28)
            resources:DestroyMovie(scratchValue27)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue13)
            resources:DestroyMovie(scratchValue27)
            return
        end
        if scratchValue5 ~= 0 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue28)
                resources:DestroyMovie(scratchValue27)
                return
            end
            quest:ClearThingHasInformation(me)
            scratchValue5 = 0
        end
        ::LAB_00d4a512::
        if not quest:IsDistanceBetweenThingsOver(me, scratchValue20, 4.0) or me:IsPerformingScriptTask() then
            scratchValue13 = me:IsPerformingScriptTask()
            if scratchValue13 then goto LAB_00d4a71c end
            dist = 10.0
            sequence1 = not quest:IsDistanceBetweenThingsUnder(me, quest:GetHero(), 10.0)
            if not sequence1 then
                scratchValue13 = quest:GetTimer(scratchValue28)
                sequence1 = 0 < scratchValue13
            end
            if sequence1 then goto LAB_00d4a71c end
            if not quest:IsActiveThreadTerminating() then
                quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), false)
                quest:SetTimer(scratchValue28, 20)
                scratchValue14 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue14, quest:GetHero())
                scratchValue13 = quest:GetMasterGameState("GlobalMeleeGrade")
                if scratchValue13 == 0 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_COMMENT", me, quest:GetHero(), false); goto LAB_00d4a71c end
                elseif scratchValue13 == 7 then
                    if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_APPRENTICE_MELEE_APLUS_COMMENT", me, quest:GetHero(), false); goto LAB_00d4a71c end
                elseif not quest:IsActiveThreadTerminating() then
                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_APPRENTICE_MELEE_NOT_APLUS_COMMENT", me, quest:GetHero(), false)
                    goto LAB_00d4a71c
                end
            end
            quest:DeregisterTimer(scratchValue28)
            resources:DestroyMovie(scratchValue27)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(dist)
            resources:DestroyMovie(scratchValue27)
            return
        end
        if scratchValue20 ~= nil and not scratchValue20:IsNull() then
            p0 = scratchValue20:GetPos()
        end
        me:MoveToPosition(p0, 3.0, 1, false, true)
        ::LAB_00d4a71c::
        if not state:GetBool("WaitingForFight") then
            scratchValue2 = false
        else
            scratchValue22 = scratchValue22 | 1
            scratchValue2 = me:IsTalkedToByHero()
        end
        if scratchValue22 & 1 ~= 0 then
            scratchValue22 = scratchValue22 & 0xfffffffe
        end
        if scratchValue2 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue28)
                resources:DestroyMovie(scratchValue27)
                return
            end
            scratchValue13 = me:IsPerformingScriptTask()
            if scratchValue13 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6da end
                me:ClearCommands()
                scratchValue33 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue27)) then
                    me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_EARLY", 0, false, true, false)
                    scratchValue13 = me:IsPerformingScriptTask()
                    scratchValue3 = scratchValue13
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue33)
                            goto LAB_00d4c6da
                        end
                        scratchValue13 = me:IsPerformingScriptTask()
                        scratchValue3 = scratchValue13
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue33)
                        goto LAB_00d4c6da
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(scratchValue33)
                goto LAB_00d4c416
            end
            if not quest:IsActiveThreadTerminating() then
                if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue32 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue27)) then
                            me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_MELEE_OTHER_MELEE_GRADE", 0, false, true, false)
                            scratchValue13 = me:IsPerformingScriptTask()
                            scratchValue3 = scratchValue13
                            while scratchValue3 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:ReleaseResource(scratchValue32)
                                    goto LAB_00d4c6da
                                end
                                scratchValue13 = me:IsPerformingScriptTask()
                                scratchValue3 = scratchValue13
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(scratchValue32)
                                goto LAB_00d4c6da
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:ReleaseResource(scratchValue32)
                        goto LAB_00d4c416
                    end
                    goto LAB_00d4c6da
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6da end
                meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
                scratchValue13 = meleeApprentice ~= nil and meleeApprentice:IsAlive()
                if not scratchValue13 then
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue31 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue27)) then
                            me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_MELEE_NO_WHISPER", 0, false, true, false)
                            scratchValue13 = me:IsPerformingScriptTask()
                            scratchValue3 = scratchValue13
                            while scratchValue3 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:ReleaseResource(scratchValue31)
                                    goto LAB_00d4c6d1
                                end
                                scratchValue13 = me:IsPerformingScriptTask()
                                scratchValue3 = scratchValue13
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:ReleaseResource(scratchValue31)
                                goto LAB_00d4c6d1
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue31)
                        goto LAB_00d4c416
                    end
                else
                    scratchValue25 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    scratchValue36 = scratchValue19
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue27)) then
                        me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_HELLO", 0, false, true, false)
                        scratchValue3 = me:IsPerformingScriptTask()
                        while scratchValue3 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d4c5ff end
                            scratchValue3 = me:IsPerformingScriptTask()
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00d4ac95 end
                        ::LAB_00d4c5ff::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue25)
                        goto LAB_00d4c6d1
                    end
                    ::LAB_00d4ac95::
                    quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while scratchValue13 < 0 do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue25)
                            goto LAB_00d4c6d1
                        end
                        scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue25)
                        goto LAB_00d4c6d1
                    end
                    if scratchValue13 == 0 then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue25)
                            goto LAB_00d4c6d1
                        end
                        scratchValue6 = 1
                        if quest:GetHealth(resources:ScriptThing(scratchValue27)) <= 0.0 then
                            scratchValue6 = scratchValue13
                        end
                        if scratchValue6 ~= 0 then
                            me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_MELEE_RETURN", 0, false, true, false)
                            scratchValue3 = me:IsPerformingScriptTask()
                            while scratchValue3 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue25)
                                    goto LAB_00d4c6d1
                                end
                                scratchValue3 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue25)
                                goto LAB_00d4c6d1
                            end
                        end
                    else
                        if scratchValue13 ~= 1 then goto LAB_00d4b11f end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue25)
                            goto LAB_00d4c6d1
                        end
                        scratchValue6 = scratchValue13
                        if quest:GetHealth(resources:ScriptThing(scratchValue27)) <= 0.0 then
                            scratchValue6 = 0
                        end
                        if scratchValue6 ~= 0 then
                            me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_MELEE_START", 0, false, true, false)
                            scratchValue3 = me:IsPerformingScriptTask()
                            while scratchValue3 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue25)
                                    goto LAB_00d4c6d1
                                end
                                scratchValue3 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue25)
                                goto LAB_00d4c6d1
                            end
                        end
                        if quest:GetMasterGameState("GlobalMeleeGrade") ~= 7 then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue25)
                                goto LAB_00d4c6d1
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue27)) then
                                me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_MELEE_START_APLUS", 0, false, true, false)
                                scratchValue3 = me:IsPerformingScriptTask()
                                while scratchValue3 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue25)
                                        goto LAB_00d4c6d1
                                    end
                                    scratchValue3 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue25)
                                    goto LAB_00d4c6d1
                                end
                            end
                        end
                        quest:FadeScreenOut(0.5, 0.5)
                        quest:Pause(1.0)
                        quest:EntityTeleportToThing(quest:GetHero(), quest:GetThingWithScriptName("HeroMeleeStart"), false)
                        quest:EntityTeleportToThing(meleeApprentice, quest:GetThingWithScriptName("WhisperMeleeStart"), false)
                        quest:EntityUnsheatheMeleeWeapon(quest:GetHero(), false)
                        quest:EntityUnsheatheWeapons(meleeApprentice, false)
                        scratchValue19 = scratchValue36
                    end
                    ::LAB_00d4b11f::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue25)
                    if scratchValue13 ~= 1 then
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
                    quest:GiveThingBestEnemyTarget(meleeApprentice, quest:GetHero())
                    quest:SetStateBool("FightFinished", false)
                    timerId = quest:RegisterTimer()
                    scratchValue14 = timerId
                    quest:SetTimer(timerId, 15)
                    quest:DisplayQuestInfo(true)
                    scratchValue36 = quest:AddQuestInfoBarHealth(meleeApprentice, {R = 255, G = 255, B = 255, A = 255}, "HUD_WHISPER_ICON", 1.0)
                    scratchValue9 = quest:GetHealth(quest:GetHero())
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
                                scratchValue30 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue27)) then
                                    me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_MELEE_NO_LIGHTNING", 0, false, true, false)
                                    scratchValue13 = me:IsPerformingScriptTask()
                                    scratchValue3 = scratchValue13
                                    while scratchValue3 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(scratchValue30)
                                            goto LAB_00d4c6c8
                                        end
                                        scratchValue13 = me:IsPerformingScriptTask()
                                        scratchValue3 = scratchValue13
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue30)
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
                            scratchValue29 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue27)) then
                                me:Speak(quest:GetHero(), "TEXT_QST_028_APPRENTICE_MELEE_NO_BOW", 0, false, true, false)
                                scratchValue13 = me:IsPerformingScriptTask()
                                scratchValue3 = scratchValue13
                                while scratchValue3 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue29)
                                        goto LAB_00d4c6c8
                                    end
                                    scratchValue13 = me:IsPerformingScriptTask()
                                    scratchValue3 = scratchValue13
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue29)
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
                        fret_10 = quest:GetHealth(quest:GetHero())
                        if quest:ReadGlobalGameDataFloat(3800) <= fret_10 then
                            if quest:GetHealth(meleeApprentice) < quest:ReadGlobalGameDataFloat(3800) then
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
                                scratchValue22 = scratchValue22 | 2
                                scratchValue17 = quest:GetHero()
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                if not scratchValue17:MsgIsHitBy("MeleeOpponent") then
                                    scratchValue2 = false
                                    goto FLOW_after_lab_00d4b8c5
                                end
                                scratchValue2 = true
                            else
                                scratchValue2 = false
                            end
                            ::FLOW_after_lab_00d4b8c5::
                            if scratchValue22 & 2 ~= 0 then
                                scratchValue22 = scratchValue22 & 0xfffffffd
                            end
                            if scratchValue2 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                                scratchValue13 = quest:GetTimer(scratchValue14)
                                if scratchValue13 < 1 then
                                    if not quest:IsActiveThreadTerminating() then
                                        scratchValue14 = quest:AddNewConversation(meleeApprentice, false, false)
                                        quest:AddPersonToConversation(scratchValue14, quest:GetHero())
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", meleeApprentice, quest:GetHero(), false)
                                        quest:SetTimer(timerId, 15)
                                        goto FLOW_after_lab_00d4b857
                                    end
                                    goto LAB_00d4c6c8
                                end
                            else
                                scratchValue17 = quest:GetHero()
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                if scratchValue17:MsgIsHitBy("MeleeOpponent") then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                                    scratchValue13 = quest:GetTimer(timerId)
                                    if scratchValue13 < 1 then
                                        scratchValue14 = quest:AddNewConversation(meleeApprentice, false, false)
                                        quest:AddPersonToConversation(scratchValue14, quest:GetHero())
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", meleeApprentice, quest:GetHero(), false)
                                        quest:SetTimer(timerId, 15)
                                    end
                                end
                            end
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                            scratchValue13 = quest:GetTimer(scratchValue14)
                            if scratchValue13 < 9 then
                                scratchValue14 = quest:AddNewConversation(meleeApprentice, false, false)
                                quest:AddPersonToConversation(scratchValue14, quest:GetHero())
                                quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", meleeApprentice, quest:GetHero(), false)
                                quest:SetTimer(timerId, 15)
                            end
                        end
                        ::FLOW_after_lab_00d4b857::
                        scratchValue3 = quest:GetStateBool("FightFinished")
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                    quest:ResetPlayerCreatureOnlyTarget()
                    quest:RemoveQuestInfoElement(scratchValue36)
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
                            quest:EntityTeleportToThing(quest:GetHero(), quest:GetThingWithScriptName("M_MeleeHeroStand"), false)
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
                        scratchValue9 = (scratchValue8 - quest:GetHealth(meleeApprentice)) - (scratchValue9 - quest:GetHealth(quest:GetHero()))
                        scratchValue13 = 0
                        repeat
                            scratchValue14 = scratchValue13
                            if quest:ReadGlobalGameDataFloatAt(3764, scratchValue15) <= scratchValue9 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                                break
                            end
                            scratchValue15 = scratchValue15 + 1
                            scratchValue13 = scratchValue14 + 1
                        until not (scratchValue14 + 1 < 7)
                        scratchValue24 = resources:NewResource()
                        scratchValue2 = resources:TryAcquire(scratchValue24, meleeApprentice, 4)
                        while not scratchValue2 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d4c6bf end
                            scratchValue2 = resources:TryAcquire(scratchValue24, meleeApprentice, 4)
                        end
                        if not quest:IsActiveThreadTerminating() then
                            scratchValue23 = resources:NewResource()
                            scratchValue2 = resources:TryAcquire(scratchValue23, quest:GetHero(), 4)
                            while not scratchValue2 do
                                if not quest:NewScriptFrame(me) then goto LAB_00d4c6b3 end
                                scratchValue2 = resources:TryAcquire(scratchValue23, quest:GetHero(), 4)
                            end
                            if not quest:IsActiveThreadTerminating() then
                                scratchValue26 = resources:NewActorMap()
                                resources:SetActor(scratchValue26, "ME", scratchValue27)
                                resources:SetActor(scratchValue26, "HERO", scratchValue23)
                                resources:SetActor(scratchValue26, "WHISPER", scratchValue24)
                                scratchValue34 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_END", scratchValue26, false, true)
                                resources:SetActor(scratchValue26, "ME", scratchValue27)
                                switch2 = scratchValue14
                                repeat
                                    if switch2 == 0 then
                                        if quest:GetMasterGameState("GlobalMeleeGrade") ~= 7 then
                                            if not quest:IsActiveThreadTerminating() then
                                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_APLUS_PRIZE", scratchValue26, false, true)
                                                quest:ClearThingHasInformation(me)
                                                goto FLOW_native_label_1
                                            end
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(scratchValue34)
                                            resources:DestroyActorMap(scratchValue26)
                                            goto LAB_00d4c6b3
                                        end
                                        if not quest:IsActiveThreadTerminating() then resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_APLUS", scratchValue26, false, true); break end
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue34)
                                        resources:DestroyActorMap(scratchValue26)
                                        goto LAB_00d4c6b3
                                    elseif switch2 == 1 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_A", scratchValue26, false, true)
                                        break
                                    elseif switch2 == 2 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_B", scratchValue26, false, true)
                                        break
                                    elseif switch2 == 3 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_C", scratchValue26, false, true)
                                        break
                                    elseif switch2 == 4 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_D", scratchValue26, false, true)
                                        break
                                    elseif switch2 == 5 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_E", scratchValue26, false, true)
                                        break
                                    elseif switch2 == 6 then
                                        resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_F", scratchValue26, false, true)
                                        break
                                    else
                                        goto FLOW_native_label_1
                                    end
                                until true
                                ::FLOW_native_label_1::
                                if quest:GetMasterGameState("GlobalMeleeGrade") < 7 - scratchValue14 then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue34)
                                        resources:DestroyActorMap(scratchValue26)
                                        goto LAB_00d4c6b3
                                    end
                                    quest:SetMasterGameState("GlobalMeleeGrade", 7 - scratchValue14)
                                end
                                resources:SetActor(scratchValue26, "ME", scratchValue27)
                                resources:SetActor(scratchValue26, "HERO", scratchValue23)
                                resources:SetActor(scratchValue26, "WHISPER", scratchValue24)
                                resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_OVER", scratchValue26, false, true)
                                quest:FixMovieSequenceCamera(false)
                                state:SetBool("WaitingForFight", true)
                                quest:ChangeHeroHealthBy(1000.0, true, false)
                                quest:ModifyThingHealth(meleeApprentice, 1000.0, false)
                                quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue34)
                                resources:DestroyActorMap(scratchValue26)
                                resources:DestroyMovie(scratchValue23)
                                resources:ReleaseResource(scratchValue24)
                                quest:SetStateBool("StartedMeleeTesting", false)
                                quest:SetMasterGameState("HeroTakingGuildTest", false)
                                quest:DeregisterTimer(timerId)
                                goto LAB_00d4c416
                            end
                            ::LAB_00d4c6b3::
                            resources:ReleaseResource(scratchValue23)
                        end
                        ::LAB_00d4c6bf::
                        resources:ReleaseResource(scratchValue24)
                    end
                    ::LAB_00d4c6c8::
                    quest:DeregisterTimer(timerId)
                end
                ::LAB_00d4c6d1::
            end
            ::LAB_00d4c6da::
            quest:DeregisterTimer(scratchValue28)
            resources:DestroyMovie(scratchValue27)
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

