-- Readable native conversion: CombatApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_MeleeGrades = 3764,  -- '070000000000904100007041000020410000a040000000000000a0c0000030c1'
    GUI_MinHealth = 3800,  -- 6.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local waitingForFight

-- CombatApprentice.Main (retail 0x00d4a270)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, predicateResult41, fightFinished, msgIsHitByHeroWithProjectileWeapon
    local msgIsHitByHeroSpecialAbility, msgIsHitByHero, scratchValue, scratchValue14, c_stk_259_1
    local c_stk_259_4, scratchValue16, f_stk_210_1, f_stk_210_2, fret_10, scratchValue20
    local scratchValue21, scratchValue22, switch, p0, combatApprenticeTargetMarker, meleeApprentice
    local resource, resource3, movie, actorMap, resource4, timerId, scratchValue26, movie2, movie3
    local movie4, movie5, movie6, movie7, addQuestInfoBarHealth
    resource4 = resources:NewResource()
    while not resources:TryAcquire(resource4, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource4)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource4); return end
    quest:SetIsPushableByHero(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(me)
    scratchValue26 = quest:RegisterTimer()
    quest:SetTimer(scratchValue26, 10)
    combatApprenticeTargetMarker = quest:GetThingWithScriptName("CombatApprenticeTargetMarker")
    scratchValue14 = 0
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            quest:DeregisterTimer(scratchValue26)
            resources:ReleaseResource(resource4)
            return
        end
        if not quest:IsQuestActive("Q_GuildTrainingDeparture") then goto LAB_00d4a512 end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue26)
            resources:ReleaseResource(resource4)
            return
        end
        if ((quest:GetMasterGameState("GlobalMeleeGrade") < 4) and (quest:GetMasterGameState("GlobalSkillGrade") < 4)) and quest:GetMasterGameState("GlobalWillGrade") < 4 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue26); resources:ReleaseResource(resource4); return end
            if scratchValue14 == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(scratchValue26)
                    resources:ReleaseResource(resource4)
                    return
                end
                quest:SetThingHasInformation(me, false, true, false)
                scratchValue14 = 1
            end
            goto LAB_00d4a512
            quest:DeregisterTimer(scratchValue26)
            resources:ReleaseResource(resource4)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue26)
            resources:ReleaseResource(resource4)
            return
        end
        if scratchValue14 ~= 0 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue26)
                resources:ReleaseResource(resource4)
                return
            end
            quest:ClearThingHasInformation(me)
            scratchValue14 = 0
        end
        ::LAB_00d4a512::
        if not quest:IsDistanceBetweenThingsOver(me, combatApprenticeTargetMarker, 4.0) or me:IsPerformingScriptTask() then
            if me:IsPerformingScriptTask() then goto LAB_00d4a71c end
            if not quest:IsDistanceBetweenThingsUnder(me, hero, 10.0) or 0 < quest:GetTimer(scratchValue26) then goto LAB_00d4a71c end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(scratchValue26); resources:ReleaseResource(resource4); return end
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            quest:SetTimer(scratchValue26, 20)
            scratchValue21 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(scratchValue21, hero)
            scratchValue20 = quest:GetMasterGameState("GlobalMeleeGrade")
            if scratchValue20 == 0 then
                if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_COMMENT", me, hero, false); goto LAB_00d4a71c end
            elseif scratchValue20 == 7 then
                if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_APPRENTICE_MELEE_APLUS_COMMENT", me, hero, false); goto LAB_00d4a71c end
            elseif not quest:IsActiveThreadTerminating() then
                quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_APPRENTICE_MELEE_NOT_APLUS_COMMENT", me, hero, false)
                goto LAB_00d4a71c
            end
            quest:DeregisterTimer(scratchValue26)
            resources:ReleaseResource(resource4)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(scratchValue26)
            resources:ReleaseResource(resource4)
            return
        end
        if combatApprenticeTargetMarker ~= nil and not combatApprenticeTargetMarker:IsNull() then
            p0 = combatApprenticeTargetMarker:GetPos()
        end
        me:MoveToPosition(p0, 3.0, 1, false, true)
        ::LAB_00d4a71c::
        if waitingForFight and me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(scratchValue26)
                resources:ReleaseResource(resource4)
                return
            end
            if me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6da end
                me:ClearCommands()
                movie6 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                    me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_EARLY", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie6)
                            goto LAB_00d4c6da
                        end
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
            if quest:IsActiveThreadTerminating() then goto LAB_00d4c6da end
            if quest:GetMasterGameState("HeroTakingGuildTest") ~= 0 then
                if not quest:IsActiveThreadTerminating() then
                    movie5 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                        me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_OTHER_MELEE_GRADE", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie5)
                                goto LAB_00d4c6da
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie5)
                            goto LAB_00d4c6da
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    goto LAB_00d4c416
                end
                goto LAB_00d4c6da
            end
            meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
            if not (meleeApprentice ~= nil and meleeApprentice:IsAlive()) then
                movie4 = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                    me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_NO_WHISPER", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            goto LAB_00d4c6d1
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        goto LAB_00d4c6d1
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                goto LAB_00d4c416
            else
                movie = resources:StartMovie("")
                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                    if not (me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_HELLO", GROUP_SELECT_FIRST, false, true, false) and not quest:IsActiveThreadTerminating()) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d4c6d1
                    end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                while scratchValue20 < 0 do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                    else
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d4c6d1
                        scratchValue20 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00d4c6d1
                end
                if scratchValue20 == 0 then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d4c6d1
                    end
                    c_stk_259_1 = 1
                    if quest:GetHealth(resources:ScriptThing(resource4)) <= 0.0 then
                        c_stk_259_1 = scratchValue20
                    end
                    if c_stk_259_1 ~= 0 then
                        me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_RETURN", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00d4c6d1
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00d4c6d1
                        end
                    end
                else
                    if scratchValue20 ~= 1 then goto LAB_00d4b11f end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d4c6d1
                    end
                    if scratchValue20 ~= 0 and quest:GetHealth(resources:ScriptThing(resource4)) > 0.0 then
                        me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_START", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00d4c6d1
                            end
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
                            me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_START_APLUS", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie)
                                    goto LAB_00d4c6d1
                                end
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
                if scratchValue20 ~= 1 then
                    goto LAB_00d4c416
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6d1 end
                quest:FadeScreenIn()
                quest:SetPlayerCreatureOnlyTarget(meleeApprentice)
                quest:SetMasterGameState("HeroTakingGuildTest", true)
                quest:SetStateBool("StartedMeleeTesting", true)
                waitingForFight = false
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
                scratchValue21 = timerId
                quest:SetTimer(timerId, 15)
                quest:DisplayQuestInfo(true)
                addQuestInfoBarHealth = quest:AddQuestInfoBarHealth(meleeApprentice, {R = 255, G = 255, B = 255, A = 255}, "HUD_WHISPER_ICON", 1.0)
                f_stk_210_1 = quest:GetHealth(hero)
                scratchValue16 = quest:GetHealth(meleeApprentice)
                fightFinished = quest:GetStateBool("FightFinished")
                c_stk_259_4 = 0
                scratchValue = 0
                while not fightFinished do
                    if not quest:NewScriptFrame(me) then goto LAB_00d4c6c8 end
                    if quest:GetMasterGameState("GuildWarningOccuring") == 1 then
                        scratchValue = 1
                        c_stk_259_4 = 1
                        quest:SetStateBool("FightFinished", true)
                    end
                    if not (meleeApprentice ~= nil and not meleeApprentice:IsNull()) then
                        msgIsHitByHeroWithProjectileWeapon = 0
                    else
                        msgIsHitByHeroWithProjectileWeapon = meleeApprentice:MsgIsHitByHeroWithProjectileWeapon()
                    end
                    if not msgIsHitByHeroWithProjectileWeapon then
                        if not (meleeApprentice ~= nil and not meleeApprentice:IsNull()) then
                            msgIsHitByHeroSpecialAbility = 0
                        else
                            msgIsHitByHeroSpecialAbility = meleeApprentice:MsgIsHitByHeroSpecialAbility(11)
                        end
                        if msgIsHitByHeroSpecialAbility then
                            quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                            if meleeApprentice ~= nil and not meleeApprentice:IsNull() then
                                meleeApprentice:SetFriendsWithEverythingFlag(1)
                            end
                            movie3 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                                me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_NO_LIGHTNING", GROUP_SELECT_FIRST, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie3)
                                        goto LAB_00d4c6c8
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d4c6c8
                                end
                            end
                            quest:SetStateBool("FightFinished", true)
                            c_stk_259_4 = 1
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
                            me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_NO_BOW", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    goto LAB_00d4c6c8
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                goto LAB_00d4c6c8
                            end
                        end
                        quest:SetStateBool("FightFinished", true)
                        c_stk_259_4 = 1
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
                        msgIsHitByHero = 0
                    else
                        msgIsHitByHero = meleeApprentice:MsgIsHitByHero()
                    end
                    if not msgIsHitByHero then
                        if quest:IsPlayerCreatureBlocking() then
                            -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                            if not hero:MsgIsHitBy("MeleeOpponent") then
                                predicateResult41 = false
                                goto FLOW_after_lab_00d4b8c5
                            end
                            predicateResult41 = true
                        else
                            predicateResult41 = false
                        end
                        ::FLOW_after_lab_00d4b8c5::
                        if predicateResult41 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                            if quest:GetTimer(scratchValue21) < 1 then
                                if not quest:IsActiveThreadTerminating() then
                                    scratchValue21 = quest:AddNewConversation(meleeApprentice, false, false)
                                    quest:AddPersonToConversation(scratchValue21, hero)
                                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", meleeApprentice, hero, false)
                                    quest:SetTimer(timerId, 15)
                                    goto FLOW_after_lab_00d4b857
                                end
                                goto LAB_00d4c6c8
                            end
                        else
                            -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                            if hero:MsgIsHitBy("MeleeOpponent") then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                                if quest:GetTimer(timerId) < 1 then
                                    scratchValue21 = quest:AddNewConversation(meleeApprentice, false, false)
                                    quest:AddPersonToConversation(scratchValue21, hero)
                                    quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", meleeApprentice, hero, false)
                                    quest:SetTimer(timerId, 15)
                                end
                            end
                        end
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                        if quest:GetTimer(scratchValue21) < 9 then
                            scratchValue21 = quest:AddNewConversation(meleeApprentice, false, false)
                            quest:AddPersonToConversation(scratchValue21, hero)
                            quest:AddLineToConversation(scratchValue21, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", meleeApprentice, hero, false)
                            quest:SetTimer(timerId, 15)
                        end
                    end
                    ::FLOW_after_lab_00d4b857::
                    fightFinished = quest:GetStateBool("FightFinished")
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                quest:ResetPlayerCreatureOnlyTarget()
                quest:RemoveQuestInfoElement(addQuestInfoBarHealth)
                quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                if meleeApprentice ~= nil and not meleeApprentice:IsNull() then
                    meleeApprentice:SetFriendsWithEverythingFlag(1)
                end
                quest:DisplayQuestInfo(false)
                if c_stk_259_4 == 1 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                    if scratchValue ~= 1 then
                        quest:FadeScreenOut(0.5, 0.5)
                    else
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
                    waitingForFight = true
                    quest:SetStateBool("StartedMeleeTesting", false)
                    quest:SetMasterGameState("HeroTakingGuildTest", false)
                    quest:DeregisterTimer(timerId)
                    goto LAB_00d4c416
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                    scratchValue22 = 0
                    f_stk_210_2 = (scratchValue16 - quest:GetHealth(meleeApprentice)) - (f_stk_210_1 - quest:GetHealth(hero))
                    scratchValue20 = 0
                    repeat
                        scratchValue21 = scratchValue20
                        if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_MeleeGrades, scratchValue22) <= f_stk_210_2 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                            break
                        end
                        scratchValue22 = scratchValue22 + 1
                        scratchValue20 = scratchValue21 + 1
                    until not (scratchValue21 + 1 < 7)
                    resource3 = resources:NewResource()
                    while not resources:TryAcquire(resource3, meleeApprentice, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00d4c6bf end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4c6bf end
                    resource = resources:NewResource()
                    while not resources:TryAcquire(resource, hero, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00d4c6b3 end
                    end
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
                    switch = scratchValue21
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
                    if quest:GetMasterGameState("GlobalMeleeGrade") < 7 - scratchValue21 then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie7)
                            resources:DestroyActorMap(actorMap)
                            goto LAB_00d4c6b3
                        end
                        quest:SetMasterGameState("GlobalMeleeGrade", 7 - scratchValue21)
                    end
                    resources:SetActor(actorMap, "ME", resource4)
                    resources:SetActor(actorMap, "HERO", resource)
                    resources:SetActor(actorMap, "WHISPER", resource3)
                    resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_OVER", actorMap, false, true)
                    quest:FixMovieSequenceCamera(false)
                    waitingForFight = true
                    quest:ChangeHeroHealthBy(1000.0, true, false)
                    quest:ModifyThingHealth(meleeApprentice, 1000.0, false)
                    quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie7)
                    resources:DestroyActorMap(actorMap)
                    resources:ReleaseResource(resource)
                    resources:ReleaseResource(resource3)
                    quest:SetStateBool("StartedMeleeTesting", false)
                    quest:SetMasterGameState("HeroTakingGuildTest", false)
                    quest:DeregisterTimer(timerId)
                    goto LAB_00d4c416
                    ::LAB_00d4c6b3::
                    resources:ReleaseResource(resource)
                    ::LAB_00d4c6bf::
                    resources:ReleaseResource(resource3)
                end
                ::LAB_00d4c6c8::
                quest:DeregisterTimer(timerId)
            end
            ::LAB_00d4c6d1::
            ::LAB_00d4c6da::
            quest:DeregisterTimer(scratchValue26)
            resources:ReleaseResource(resource4)
            return
        end
        ::LAB_00d4c416::
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
    until false
end

-- CombatApprentice.Init (retail 0x00d41ae0)
function Init(quest, me)
    waitingForFight = true
end

-- CombatApprentice.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- CombatApprentice.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

