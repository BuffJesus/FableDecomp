-- Readable native conversion: CombatApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_LIGHTNING_SPELL = 11  -- EHeroAbility (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_MeleeGrades = 3764,  -- '070000000000904100007041000020410000a040000000000000a0c0000030c1'
    GUI_MinHealth = 3800,  -- 6.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local waitingForFight

-- CombatApprentice.Main (retail 0x00d4a270)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult3, predicateResult, predicateResult41, fightFinished
    local msgIsHitByHeroSpecialAbility, msgIsHitByHero, scratchValue, scratchValue15, scratchValue16
    local getHealth, scratchValue22, addNewConversation, index, p0, thing, hero, line, healthBar
    local combatApprenticeTargetMarker, meleeApprentice, this_00, addQuestInfoBarHealth
    local resource4 = resources:NewResource()
    resources:PrepareResource(resource4)
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
    me:SetFriendsWithEverythingFlag(true)
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 10)
    combatApprenticeTargetMarker = quest:GetThingWithScriptName("CombatApprenticeTargetMarker")
    scratchValue15 = 0
    predicateResult3 = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult3 then
            combatApprenticeTargetMarker = nil
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource4)
            return
        end
        if not (not quest:IsQuestActive("Q_GuildTrainingDeparture")) then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource4)
                return
            end
            if ((quest:GetMasterGameState("GlobalMeleeGrade") < 4) and (quest:GetMasterGameState("GlobalSkillGrade") < 4)) and quest:GetMasterGameState("GlobalWillGrade") < 4 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource4); return end
                if scratchValue15 == 0 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource4)
                        return
                    end
                    quest:SetThingHasInformation(me, false, true, false)
                    scratchValue15 = 1
                end
                goto LAB_00d4a512
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource4)
                return
            end
            if scratchValue15 ~= 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource4)
                    return
                end
                quest:ClearThingHasInformation(me)
                scratchValue15 = 0
            end
        end
        ::LAB_00d4a512::
        if not quest:IsDistanceBetweenThingsOver(me, combatApprenticeTargetMarker, 4.0) or me:IsPerformingScriptTask() then
            if me:IsPerformingScriptTask() then goto LAB_00d4a71c end
            hero = quest:GetHero()
            if not quest:IsDistanceBetweenThingsUnder(me, hero, 10.0) or 0 < quest:GetTimer(timerId) then goto LAB_00d4a71c end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource4); return end
            hero = quest:GetHero()
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            quest:SetTimer(timerId, 20)
            addNewConversation = quest:AddNewConversation(me, false, false)
            hero = quest:GetHero()
            quest:AddPersonToConversation(addNewConversation, hero)
            scratchValue22 = quest:GetMasterGameState("GlobalMeleeGrade")
            if scratchValue22 == 0 then
                if not quest:IsActiveThreadTerminating() then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_COMMENT", me, hero, false)
                    goto LAB_00d4a717
                end
            elseif scratchValue22 == 7 then
                if not quest:IsActiveThreadTerminating() then
                    hero = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_APPRENTICE_MELEE_APLUS_COMMENT", me, hero, false)
                    goto LAB_00d4a717
                end
            elseif not quest:IsActiveThreadTerminating() then
                hero = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_APPRENTICE_MELEE_NOT_APLUS_COMMENT", me, hero, false)
                goto LAB_00d4a717
            end
            goto FLOW_past_lab_00d4a717
            ::LAB_00d4a717::
            goto LAB_00d4a71c
            ::FLOW_past_lab_00d4a717::
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource4)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource4)
            return
        end
        if not (combatApprenticeTargetMarker ~= nil and not combatApprenticeTargetMarker:IsNull()) then
            p0 = {x = 0, y = 0, z = 0}
        else
            p0 = combatApprenticeTargetMarker:GetPos()
        end
        me:MoveToPosition(p0, 3.0, ENTITY_MOVE_RUN, false, true)
        ::LAB_00d4a71c::
        if not waitingForFight then
            goto LAB_00d4a758
        else
            if not me:IsTalkedToByHero() then goto LAB_00d4a758 end
            predicateResult = true
        end
        goto FLOW_past_lab_00d4a758
        ::LAB_00d4a758::
        predicateResult = false
        ::FLOW_past_lab_00d4a758::
        if predicateResult then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource4)
                return
            end
            if me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c40d end
                me:ClearCommands()
                local movie6 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue28 = resources:ScriptThing(resource4)
                if 0.0 < quest:GetHealth(scratchValue28) then
                    line = "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_EARLY"
                    thing = quest:GetHero()
                    if not me:Speak(thing, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie6)
                        goto FLOW_past_lab_00d4c40d
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie6)
                            goto FLOW_past_lab_00d4c40d
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie6)
                        goto FLOW_past_lab_00d4c40d
                    end
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie6)
                goto LAB_00d4c416
            end
            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c40d end
            if quest:GetMasterGameState("HeroTakingGuildTest") then
                if not quest:IsActiveThreadTerminating() then
                    local movie5 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue27 = resources:ScriptThing(resource4)
                    if 0.0 < quest:GetHealth(scratchValue27) then
                        line = "TEXT_QST_028_APPRENTICE_MELEE_OTHER_MELEE_GRADE"
                        thing = quest:GetHero()
                        if not me:Speak(thing, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie5)
                            goto FLOW_past_lab_00d4c40d
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie5)
                                goto FLOW_past_lab_00d4c40d
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie5)
                            goto FLOW_past_lab_00d4c40d
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie5)
                    goto LAB_00d4c416
                end
                goto FLOW_past_lab_00d4c40d
            end
            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c40d end
            meleeApprentice = quest:GetThingWithScriptName("MeleeApprentice")
            if not (meleeApprentice ~= nil and meleeApprentice:IsAlive()) then
                if not quest:IsActiveThreadTerminating() then
                    local movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue32 = resources:ScriptThing(resource4)
                    if 0.0 < quest:GetHealth(scratchValue32) then
                        line = "TEXT_QST_028_APPRENTICE_MELEE_NO_WHISPER"
                        thing = quest:GetHero()
                        if not me:Speak(thing, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            goto FLOW_past_lab_00d4c40d
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto FLOW_past_lab_00d4c40d
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            goto FLOW_past_lab_00d4c40d
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00d4c40d
                end
            else
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c40d end
                local movie = resources:StartMovie("")
                addQuestInfoBarHealth = healthBar
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue30 = resources:ScriptThing(resource4)
                if 0.0 < quest:GetHealth(scratchValue30) then
                    line = "TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_HELLO"
                    thing = quest:GetHero()
                    if me:Speak(thing, line, 0, false, true, false) then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d4c5ff end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00d4ac95 end
                    end
                    ::LAB_00d4c5ff::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto FLOW_past_lab_00d4c40d
                end
                ::LAB_00d4ac95::
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                scratchValue22 = quest:MsgIsQuestionAnsweredYesOrNo()
                while scratchValue22 < 0 do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue22 = quest:MsgIsQuestionAnsweredYesOrNo()
                    else
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto FLOW_past_lab_00d4c40d
                        scratchValue22 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto FLOW_past_lab_00d4c40d
                end
                if scratchValue22 == 0 then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto FLOW_past_lab_00d4c40d
                    end
                    thing = resources:ScriptThing(resource4)
                    scratchValue16 = 1
                    if quest:GetHealth(thing) <= 0.0 then
                        scratchValue16 = scratchValue22
                    end
                    if scratchValue16 ~= 0 then
                        line = "TEXT_QST_028_APPRENTICE_MELEE_RETURN"
                        thing = quest:GetHero()
                        if not me:Speak(thing, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto FLOW_past_lab_00d4c40d
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto FLOW_past_lab_00d4c40d
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto FLOW_past_lab_00d4c40d
                        end
                    end
                elseif scratchValue22 == 1 then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto FLOW_past_lab_00d4c40d
                    end
                    thing = resources:ScriptThing(resource4)
                    if scratchValue22 ~= 0 and not (quest:GetHealth(thing) <= 0.0) then
                        line = "TEXT_QST_028_APPRENTICE_MELEE_START"
                        thing = quest:GetHero()
                        if not me:Speak(thing, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto FLOW_past_lab_00d4c40d
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto FLOW_past_lab_00d4c40d
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto FLOW_past_lab_00d4c40d
                        end
                    end
                    if quest:GetMasterGameState("GlobalMeleeGrade") ~= 7 then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto FLOW_past_lab_00d4c40d
                        end
                        local scratchValue29 = resources:ScriptThing(resource4)
                        if not not (0.0 < quest:GetHealth(scratchValue29)) then
                            line = "TEXT_QST_028_APPRENTICE_MELEE_START_APLUS"
                            thing = quest:GetHero()
                            if not me:Speak(thing, line, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto FLOW_past_lab_00d4c40d
                            end
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie)
                                    goto FLOW_past_lab_00d4c40d
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto FLOW_past_lab_00d4c40d
                            end
                        end
                    end
                    quest:FadeScreenOut(0.5, 0.5)
                    quest:Pause(1.0)
                    thing = quest:GetThingWithScriptName("HeroMeleeStart")
                    hero = quest:GetHero()
                    quest:EntityTeleportToThing(hero, thing, false)
                    thing = quest:GetThingWithScriptName("WhisperMeleeStart")
                    quest:EntityTeleportToThing(meleeApprentice, thing, false)
                    thing = quest:GetHero()
                    quest:EntityUnsheatheMeleeWeapon(thing, false)
                    quest:EntityUnsheatheWeapons(meleeApprentice, false)
                    healthBar = addQuestInfoBarHealth
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                if scratchValue22 ~= 1 then goto LAB_00d4c40d end
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c40d end
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
                    meleeApprentice:SetFriendsWithEverythingFlag(false)
                end
                thing = quest:GetHero()
                quest:GiveThingBestEnemyTarget(meleeApprentice, thing)
                quest:SetStateBool("FightFinished", false)
                local timerId2 = quest:RegisterTimer()
                addNewConversation = timerId2
                quest:SetTimer(timerId2, 15)
                quest:DisplayQuestInfo(true)
                addQuestInfoBarHealth = quest:AddQuestInfoBarHealth(meleeApprentice, {R = 255, G = 255, B = 255, A = 255}, "HUD_WHISPER_ICON", 1.0)
                thing = quest:GetHero()
                getHealth = quest:GetHealth(thing)
                local health = quest:GetHealth(meleeApprentice)
                fightFinished = quest:GetStateBool("FightFinished")
                scratchValue16 = 0
                scratchValue = 0
                while not fightFinished do
                    if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00d4c3f3 end
                    if quest:GetMasterGameState("GuildWarningOccuring") then
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                        scratchValue = 1
                        scratchValue16 = 1
                        quest:SetStateBool("FightFinished", true)
                    end
                    if not ((meleeApprentice ~= nil and not meleeApprentice:IsNull()) and meleeApprentice:MsgIsHitByHeroWithProjectileWeapon() ~= nil) then
                        if not (meleeApprentice ~= nil and not meleeApprentice:IsNull()) then
                            msgIsHitByHeroSpecialAbility = false
                        else
                            msgIsHitByHeroSpecialAbility = meleeApprentice:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_LIGHTNING_SPELL)
                        end
                        if msgIsHitByHeroSpecialAbility then
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                            quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                            if meleeApprentice ~= nil and not meleeApprentice:IsNull() then
                                meleeApprentice:SetFriendsWithEverythingFlag(true)
                            end
                            local movie3 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            local scratchValue33 = resources:ScriptThing(resource4)
                            if 0.0 < quest:GetHealth(scratchValue33) then
                                line = "TEXT_QST_028_APPRENTICE_MELEE_NO_LIGHTNING"
                                thing = quest:GetHero()
                                if not me:Speak(thing, line, 0, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto FLOW_past_lab_00d4c3f3
                                end
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie3)
                                        goto FLOW_past_lab_00d4c3f3
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto FLOW_past_lab_00d4c3f3
                                end
                            end
                            quest:SetStateBool("FightFinished", true)
                            scratchValue16 = 1
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie3
                            goto LAB_00d4b6f6
                        end
                    else
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                        quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                        if meleeApprentice ~= nil and not meleeApprentice:IsNull() then
                            meleeApprentice:SetFriendsWithEverythingFlag(true)
                        end
                        local movie2 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        local scratchValue26 = resources:ScriptThing(resource4)
                        if 0.0 < quest:GetHealth(scratchValue26) then
                            line = "TEXT_QST_028_APPRENTICE_MELEE_NO_BOW"
                            thing = quest:GetHero()
                            if not me:Speak(thing, line, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                goto FLOW_past_lab_00d4c3f3
                            end
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    goto FLOW_past_lab_00d4c3f3
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                goto FLOW_past_lab_00d4c3f3
                            end
                        end
                        quest:SetStateBool("FightFinished", true)
                        scratchValue16 = 1
                        quest:PauseAllNonScriptedEntities(false)
                        this_00 = movie2
                        goto LAB_00d4b6f6
                    end
                    goto FLOW_past_lab_00d4b6f6
                    ::LAB_00d4b6f6::
                    resources:DestroyMovie(this_00)
                    ::FLOW_past_lab_00d4b6f6::
                    thing = quest:GetHero()
                    local fret_10 = quest:GetHealth(thing)
                    if quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) <= fret_10 then
                        if quest:GetHealth(meleeApprentice) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                            quest:SetStateBool("FightFinished", true)
                        end
                    else
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                        quest:SetStateBool("FightFinished", true)
                    end
                    if not (meleeApprentice ~= nil and not meleeApprentice:IsNull()) then
                        msgIsHitByHero = false
                    else
                        msgIsHitByHero = meleeApprentice:MsgIsHitByHero()
                    end
                    if not msgIsHitByHero then
                        if not (not quest:IsPlayerCreatureBlocking()) then
                            thing = quest:GetHero()
                            if not (thing ~= nil and thing:MsgIsHitBy("MeleeOpponent")) then goto LAB_00d4b8c5 end
                            predicateResult41 = true
                            goto FLOW_past_lab_00d4b8c5
                        end
                        ::LAB_00d4b8c5::
                        predicateResult41 = false
                        ::FLOW_past_lab_00d4b8c5::
                        if predicateResult41 then
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                            if quest:GetTimer(addNewConversation) < 1 then
                                if not quest:IsActiveThreadTerminating() then
                                    addNewConversation = quest:AddNewConversation(meleeApprentice, false, false)
                                    thing = quest:GetHero()
                                    quest:AddPersonToConversation(addNewConversation, thing)
                                    thing = quest:GetHero()
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", meleeApprentice, thing, false)
                                    goto LAB_00d4b857
                                end
                                goto FLOW_past_lab_00d4c3f3
                            end
                        else
                            thing = quest:GetHero()
                            if thing ~= nil and thing:MsgIsHitBy("MeleeOpponent") then
                                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                                if quest:GetTimer(timerId2) < 1 then
                                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                                    addNewConversation = quest:AddNewConversation(meleeApprentice, false, false)
                                    thing = quest:GetHero()
                                    quest:AddPersonToConversation(addNewConversation, thing)
                                    thing = quest:GetHero()
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", meleeApprentice, thing, false)
                                    quest:SetTimer(timerId2, 15)
                                end
                            end
                        end
                    else
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                        if quest:GetTimer(addNewConversation) < 9 then
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                            addNewConversation = quest:AddNewConversation(meleeApprentice, false, false)
                            thing = quest:GetHero()
                            quest:AddPersonToConversation(addNewConversation, thing)
                            thing = quest:GetHero()
                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", meleeApprentice, thing, false)
                            goto LAB_00d4b857
                        end
                    end
                    goto FLOW_past_lab_00d4b857
                    ::LAB_00d4b857::
                    quest:SetTimer(timerId2, 15)
                    ::FLOW_past_lab_00d4b857::
                    addNewConversation = timerId2
                    fightFinished = quest:GetStateBool("FightFinished")
                end
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                quest:ResetPlayerCreatureOnlyTarget()
                quest:RemoveQuestInfoElement(addQuestInfoBarHealth)
                quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                if meleeApprentice ~= nil and not meleeApprentice:IsNull() then
                    meleeApprentice:SetFriendsWithEverythingFlag(true)
                end
                quest:DisplayQuestInfo(false)
                if scratchValue16 == 1 then
                    if not quest:IsActiveThreadTerminating() then
                        if scratchValue ~= 1 then
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                            quest:FadeScreenOut(0.5, 0.5)
                        else
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                            quest:Pause(1.0)
                            quest:FadeScreenOut(0.5, 0.5)
                            quest:SheatheHeroWeapons()
                            quest:EntitySheatheWeapons(meleeApprentice, false)
                            quest:Pause(1.0)
                        end
                        quest:ChangeHeroHealthBy(1000.0, true, false)
                        quest:ModifyThingHealth(meleeApprentice, 1000.0, false)
                        thing = quest:GetThingWithScriptName("M_MeleeHeroStand")
                        hero = quest:GetHero()
                        quest:EntityTeleportToThing(hero, thing, false)
                        thing = quest:GetThingWithScriptName("M_MeleeOpponentStand")
                        quest:EntityTeleportToThing(meleeApprentice, thing, false)
                        quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                        quest:FadeScreenIn()
                        waitingForFight = true
                        goto LAB_00d4c3f3
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                    thing = quest:GetHero()
                    addQuestInfoBarHealth = quest:GetHealth(thing)
                    local fret_13 = quest:GetHealth(meleeApprentice)
                    index = 0
                    getHealth = string.unpack("<f", string.pack("<f", (health - fret_13) - (getHealth - addQuestInfoBarHealth)))
                    scratchValue22 = 0
                    repeat
                        addNewConversation = scratchValue22
                        if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_MeleeGrades, index) <= getHealth then
                            if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d4c3f3 end
                            break
                        end
                        index = index + 1
                        scratchValue22 = addNewConversation + 1
                    until not (addNewConversation + 1 < 7)
                    local resource3 = resources:NewResource()
                    resources:PrepareResource(resource3)
                    while not resources:TryAcquire(resource3, meleeApprentice, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00d4c6bf end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        local resource = resources:NewResource()
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, quest:GetHero(), 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00d4c6b3 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            local actorMap = resources:NewActorMap()
                            resources:SetActor(actorMap, "ME", resource4)
                            resources:SetActor(actorMap, "HERO", resource)
                            resources:SetActor(actorMap, "WHISPER", resource3)
                            local movie7 = resources:StartMovie("")
                            quest:PauseAllNonScriptedEntities(true)
                            quest:FixMovieSequenceCamera(true)
                            resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_END", actorMap, false, true)
                            resources:SetActor(actorMap, "ME", resource4)
                            local switch = addNewConversation
                            repeat
                                if switch == 0 then
                                    if quest:GetMasterGameState("GlobalMeleeGrade") ~= 7 then
                                        if not quest:IsActiveThreadTerminating() then
                                            resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_APLUS_PRIZE", actorMap, false, true)
                                            quest:ClearThingHasInformation(me)
                                            break
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
                                    break
                                end
                            until true
                            if quest:GetMasterGameState("GlobalMeleeGrade") < 7 - addNewConversation then
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie7)
                                    resources:DestroyActorMap(actorMap)
                                    goto LAB_00d4c6b3
                                end
                                quest:SetMasterGameState("GlobalMeleeGrade", 7 - addNewConversation)
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
                            goto LAB_00d4c3f3
                        end
                        ::LAB_00d4c6b3::
                        resources:ReleaseResource(resource)
                    end
                    ::LAB_00d4c6bf::
                    resources:ReleaseResource(resource3)
                end
                goto FLOW_past_lab_00d4c3f3
                ::LAB_00d4c3f3::
                quest:SetStateBool("StartedMeleeTesting", false)
                quest:SetMasterGameState("HeroTakingGuildTest", false)
                quest:DeregisterTimer(timerId2)
                goto LAB_00d4c40d
                ::FLOW_past_lab_00d4c3f3::
                quest:DeregisterTimer(timerId2)
            end
            goto FLOW_past_lab_00d4c40d
            ::LAB_00d4c40d::
            goto LAB_00d4c416
            ::FLOW_past_lab_00d4c40d::
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource4)
            return
        end
        ::LAB_00d4c416::
        quest:NewScriptFrame(me)
        predicateResult3 = quest:IsActiveThreadTerminating()
    until false
end

-- CombatApprentice.Init (retail 0x00d41ae0)
function Init(quest, me)
    waitingForFight = true
end

-- CombatApprentice.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- CombatApprentice.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

