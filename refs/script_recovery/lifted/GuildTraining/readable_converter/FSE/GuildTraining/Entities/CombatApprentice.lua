-- Readable native conversion: CombatApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

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
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult3, predicateResult, predicateResult41, scratchValue4, scratchValue5
    local scratchValue6, scratchValue7, getHealth, scratchValue14, addNewConversation
    local scratchValue15, index, p0, meleeApprentice, this_00
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
    local timerId2 = quest:RegisterTimer()
    quest:SetTimer(timerId2, 10)
    local combatApprenticeTargetMarker = quest:GetThingWithScriptName("CombatApprenticeTargetMarker")
    scratchValue6 = 0
    predicateResult3 = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult3 then
            quest:DeregisterTimer(timerId2)
            resources:ReleaseResource(resource4)
            return
        end
        if not quest:IsQuestActive("Q_GuildTrainingDeparture") then goto LAB_00d4a512 end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId2)
            resources:ReleaseResource(resource4)
            return
        end
        if ((quest:GetMasterGameState("GlobalMeleeGrade") < 4) and (quest:GetMasterGameState("GlobalSkillGrade") < 4)) and quest:GetMasterGameState("GlobalWillGrade") < 4 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource4); return end
            if scratchValue6 == 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(resource4)
                    return
                end
                quest:SetThingHasInformation(me, false, true, false)
                scratchValue6 = 1
            end
            goto LAB_00d4a512
            quest:DeregisterTimer(timerId2)
            resources:ReleaseResource(resource4)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId2)
            resources:ReleaseResource(resource4)
            return
        end
        if scratchValue6 ~= 0 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource4)
                return
            end
            quest:ClearThingHasInformation(me)
            scratchValue6 = 0
        end
        ::LAB_00d4a512::
        if not quest:IsDistanceBetweenThingsOver(me, combatApprenticeTargetMarker, 4.0) or me:IsPerformingScriptTask() then
            if me:IsPerformingScriptTask() then goto LAB_00d4a71c end
            if not quest:IsDistanceBetweenThingsUnder(me, hero, 10.0) or 0 < quest:GetTimer(timerId2) then goto LAB_00d4a71c end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource4); return end
            quest:EntitySetFacingAngleTowardsThing(me, hero, false)
            quest:SetTimer(timerId2, 20)
            local conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            scratchValue14 = quest:GetMasterGameState("GlobalMeleeGrade")
            if scratchValue14 == 0 then
                if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_MELEE_APPRENTICE_COMMENT", me, hero, false); goto LAB_00d4a717 end
            elseif scratchValue14 == 7 then
                if not quest:IsActiveThreadTerminating() then quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_MELEE_APLUS_COMMENT", me, hero, false); goto LAB_00d4a717 end
            elseif not quest:IsActiveThreadTerminating() then
                quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPRENTICE_MELEE_NOT_APLUS_COMMENT", me, hero, false)
                goto LAB_00d4a717
            end
            goto FLOW_past_lab_00d4a717
            ::LAB_00d4a717::
            goto LAB_00d4a71c
            ::FLOW_past_lab_00d4a717::
            quest:DeregisterTimer(timerId2)
            resources:ReleaseResource(resource4)
            return
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId2)
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
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource4)
                return
            end
            if me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6da end
                me:ClearCommands()
                local movie6 = resources:StartMovie("")
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
            if quest:GetMasterGameState("HeroTakingGuildTest") then
                if not quest:IsActiveThreadTerminating() then
                    local movie5 = resources:StartMovie("")
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
                if not quest:IsActiveThreadTerminating() then
                    local movie4 = resources:StartMovie("")
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
                    goto LAB_00d4c40d
                end
            else
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(resource4)) then
                    if not (me:Speak(hero, "TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_HELLO", GROUP_SELECT_FIRST, false, true, false) and not quest:IsActiveThreadTerminating()) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d4c6d1
                    end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPRENTICE_MELEE_DEPARTURE_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                scratchValue14 = quest:MsgIsQuestionAnsweredYesOrNo()
                while scratchValue14 < 0 do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue14 = quest:MsgIsQuestionAnsweredYesOrNo()
                    else
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d4c6d1
                        scratchValue14 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00d4c6d1
                end
                if scratchValue14 == 0 then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d4c6d1
                    end
                    scratchValue7 = 1
                    if quest:GetHealth(resources:ScriptThing(resource4)) <= 0.0 then
                        scratchValue7 = scratchValue14
                    end
                    if scratchValue7 ~= 0 then
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
                elseif scratchValue14 == 1 then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00d4c6d1
                    end
                    if scratchValue14 ~= 0 and quest:GetHealth(resources:ScriptThing(resource4)) > 0.0 then
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
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                if scratchValue14 ~= 1 then goto LAB_00d4c40d end
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
                local timerId = quest:RegisterTimer()
                addNewConversation = timerId
                quest:SetTimer(timerId, 15)
                quest:DisplayQuestInfo(true)
                local addQuestInfoBarHealth = quest:AddQuestInfoBarHealth(meleeApprentice, {R = 255, G = 255, B = 255, A = 255}, "HUD_WHISPER_ICON", 1.0)
                getHealth = quest:GetHealth(hero)
                local health = quest:GetHealth(meleeApprentice)
                scratchValue4 = quest:GetStateBool("FightFinished")
                scratchValue7 = 0
                scratchValue5 = 0
                while not scratchValue4 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d4c6c8 end
                    if quest:GetMasterGameState("GuildWarningOccuring") then
                        scratchValue5 = 1
                        scratchValue7 = 1
                        quest:SetStateBool("FightFinished", true)
                    end
                    if not (meleeApprentice ~= nil and not meleeApprentice:IsNull()) then
                        scratchValue4 = 0
                    else
                        scratchValue4 = meleeApprentice:MsgIsHitByHeroWithProjectileWeapon() ~= nil
                    end
                    if not scratchValue4 then
                        if not (meleeApprentice ~= nil and not meleeApprentice:IsNull()) then
                            scratchValue4 = 0
                        else
                            scratchValue4 = meleeApprentice:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_LIGHTNING_SPELL)
                        end
                        if scratchValue4 then
                            quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                            if meleeApprentice ~= nil and not meleeApprentice:IsNull() then
                                meleeApprentice:SetFriendsWithEverythingFlag(1)
                            end
                            local movie3 = resources:StartMovie("")
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
                            scratchValue7 = 1
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_bc;
                            goto LAB_00d4b6f6
                        end
                    else
                        quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                        if meleeApprentice ~= nil and not meleeApprentice:IsNull() then
                            meleeApprentice:SetFriendsWithEverythingFlag(1)
                        end
                        local movie2 = resources:StartMovie("")
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
                        scratchValue7 = 1
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): this_00 = (CScriptGameResourceObjectMovieBase *)xStack_ac;
                        goto LAB_00d4b6f6
                    end
                    goto FLOW_past_lab_00d4b6f6
                    ::LAB_00d4b6f6::
                    resources:DestroyMovie(this_00)
                    ::FLOW_past_lab_00d4b6f6::
                    local fret_10 = quest:GetHealth(hero)
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
                        scratchValue4 = 0
                    else
                        scratchValue4 = meleeApprentice:MsgIsHitByHero()
                    end
                    if not scratchValue4 then
                        if not quest:IsPlayerCreatureBlocking() then goto LAB_00d4b8c5 end
                        if not hero:MsgIsHitBy("MeleeOpponent") then goto LAB_00d4b8c5 end
                        predicateResult41 = true
                        goto FLOW_past_lab_00d4b8c5
                        ::LAB_00d4b8c5::
                        predicateResult41 = false
                        ::FLOW_past_lab_00d4b8c5::
                        if predicateResult41 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                            if quest:GetTimer(addNewConversation) < 1 then
                                if not quest:IsActiveThreadTerminating() then
                                    addNewConversation = quest:AddNewConversation(meleeApprentice, false, false)
                                    quest:AddPersonToConversation(addNewConversation, hero)
                                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", meleeApprentice, hero, false)
                                    goto LAB_00d4b857
                                end
                                goto LAB_00d4c6c8
                            end
                        elseif hero:MsgIsHitBy("MeleeOpponent") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                            if quest:GetTimer(timerId) < 1 then
                                addNewConversation = quest:AddNewConversation(meleeApprentice, false, false)
                                quest:AddPersonToConversation(addNewConversation, hero)
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", meleeApprentice, hero, false)
                                quest:SetTimer(timerId, 15)
                            end
                        end
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                        if quest:GetTimer(addNewConversation) < 9 then
                            addNewConversation = quest:AddNewConversation(meleeApprentice, false, false)
                            quest:AddPersonToConversation(addNewConversation, hero)
                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", meleeApprentice, hero, false)
                            goto LAB_00d4b857
                        end
                    end
                    goto FLOW_past_lab_00d4b857
                    ::LAB_00d4b857::
                    quest:SetTimer(timerId, 15)
                    ::FLOW_past_lab_00d4b857::
                    scratchValue4 = quest:GetStateBool("FightFinished")
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                quest:ResetPlayerCreatureOnlyTarget()
                quest:RemoveQuestInfoElement(addQuestInfoBarHealth)
                quest:EntitySetInFaction(meleeApprentice, "FACTION_HERO")
                if meleeApprentice ~= nil and not meleeApprentice:IsNull() then
                    meleeApprentice:SetFriendsWithEverythingFlag(1)
                end
                quest:DisplayQuestInfo(false)
                if scratchValue7 == 1 then
                    if not quest:IsActiveThreadTerminating() then
                        if scratchValue5 ~= 1 then
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
                        waitingForFight = true
                        goto LAB_00d4c3f3
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                    index = 0
                    getHealth = (health - quest:GetHealth(meleeApprentice)) - (getHealth - quest:GetHealth(hero))
                    scratchValue14 = 0
                    repeat
                        scratchValue15 = scratchValue14
                        if quest:ReadGlobalGameDataFloatAt(SCRIPT_DEF.GUI_MeleeGrades, index) <= getHealth then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d4c6c8 end
                            break
                        end
                        index = index + 1
                        scratchValue14 = scratchValue15 + 1
                    until not (scratchValue15 + 1 < 7)
                    local resource3 = resources:NewResource()
                    resources:PrepareResource(resource3)
                    while not resources:TryAcquire(resource3, meleeApprentice, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00d4c6bf end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        local resource = resources:NewResource()
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, hero, 4) do
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
                            repeat
                                if scratchValue15 == 0 then
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
                                elseif scratchValue15 == 1 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_A", actorMap, false, true)
                                    break
                                elseif scratchValue15 == 2 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_B", actorMap, false, true)
                                    break
                                elseif scratchValue15 == 3 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_C", actorMap, false, true)
                                    break
                                elseif scratchValue15 == 4 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_D", actorMap, false, true)
                                    break
                                elseif scratchValue15 == 5 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_E", actorMap, false, true)
                                    break
                                elseif scratchValue15 == 6 then
                                    resources:RunMacro("CS_GUILD_DEPARTURE_MELEE_TEST_F", actorMap, false, true)
                                    break
                                else
                                    goto FLOW_native_label_1
                                end
                            until true
                            ::FLOW_native_label_1::
                            if quest:GetMasterGameState("GlobalMeleeGrade") < 7 - scratchValue15 then
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie7)
                                    resources:DestroyActorMap(actorMap)
                                    goto LAB_00d4c6b3
                                end
                                quest:SetMasterGameState("GlobalMeleeGrade", 7 - scratchValue15)
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
                quest:DeregisterTimer(timerId)
                goto LAB_00d4c40d
                ::FLOW_past_lab_00d4c3f3::
                ::LAB_00d4c6c8::
                quest:DeregisterTimer(timerId)
            end
            goto FLOW_past_lab_00d4c40d
            ::LAB_00d4c40d::
            goto LAB_00d4c416
            ::FLOW_past_lab_00d4c40d::
            ::LAB_00d4c6d1::
            ::LAB_00d4c6da::
            quest:DeregisterTimer(timerId2)
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

