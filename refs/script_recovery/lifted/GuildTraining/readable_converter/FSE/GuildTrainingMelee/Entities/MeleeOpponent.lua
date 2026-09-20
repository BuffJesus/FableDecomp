-- Readable native conversion: MeleeOpponent. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_MinHealth = 3800,  -- 6.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local repeatMelee, badHit

-- MeleeOpponent.Main (retail 0x00d56790)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local timerId, predicateResult, predicateResult47, ctr_c, tutorialState, addNewConversation
    local scratchValue8, movie
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d5685e end
    quest:EntitySetAsKillable(me, false, true)
    while not quest:GetStateBool("TalkedToWhisper") do
        if not quest:NewScriptFrame(me) then goto LAB_00d5685e end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d5685e end
    me:MoveToPosition(quest:GetThingWithScriptName("M_MeleeOpponentStand"):GetPos(), 3.0, ENTITY_MOVE_RUN, false, true)
    while me:IsPerformingScriptTask() and not quest:GetStateBool("WhisperStopWalking") do
        if not quest:NewScriptFrame(me) then goto LAB_00d5685e end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d5685e end
    me:ClearCommands()
    resources:PrepareResource(resource)
    quest:SetStateBool("WhisperArrived", true)
    quest:EntitySetAsKillable(me, false, true)
    while quest:GetStateInt("TutorialState") == 1 do
        if not quest:NewScriptFrame(me) then goto LAB_00d57f5d end
        if me:IsTalkedToByHero() then
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:ReleaseResource(resource)
                return
            end
            local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
            if 0.0 >= fret_0 then resources:PrepareResource(resource); quest:PauseAllNonScriptedEntities(false); resources:DestroyMovie(movie); goto continue_3 end
            me:Speak(hero, "TEXT_QST_028_WHISPER_MEET", GROUP_SELECT_FIRST, false, true, false)
            while me:IsPerformingScriptTask() do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:ReleaseResource(resource)
                return
            end
            resources:PrepareResource(resource)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        elseif me:MsgIsHitByHero() then
            quest:SetStateBool("EarlyHitWhisper", true)
        end
        ::continue_3::
    end
    if not quest:IsActiveThreadTerminating() then
        while repeatMelee do
            if not quest:NewScriptFrame(me) then goto LAB_00d57f5d end
            while quest:GetStateInt("TutorialState") ~= 3 do
                if not quest:NewScriptFrame(me) then goto LAB_00d57f5d end
            end
            quest:SetStateBool("MeleeOpponentReset", false)
            quest:EntitySetCombatType(me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE_DONT_ATTACK")
            quest:GiveThingBestEnemyTarget(me, hero)
            quest:EntitySetInFaction(me, "FACTION_BANDITS")
            me:SetFriendsWithEverythingFlag(false)
            local timerId2 = quest:RegisterTimer()
            quest:SetTimer(timerId2, 15)
            tutorialState = quest:GetStateInt("TutorialState")
            ctr_c = 0
            while tutorialState == 3 and quest:GetStateInt("GenericTutorialCounter") < 7 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(resource)
                    return
                end
                if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    quest:SetStateInt("GenericTutorialCounter", quest:GetStateInt("GenericTutorialCounter") + 1)
                    if quest:GetTimer(timerId2) < 9 then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId2)
                            resources:ReleaseResource(resource)
                            return
                        end
                        addNewConversation = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(addNewConversation, hero)
                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", me, hero, false)
                        quest:SetTimer(timerId2, 15)
                    end
                elseif me:MsgIsHitByHero() then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    quest:ModifyThingHealth(me, 1000.0, false)
                    local scratchValue = ctr_c
                    if ctr_c == 0 then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId2)
                            resources:ReleaseResource(resource)
                            return
                        end
                        if quest:IsXbox() then
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId2)
                                resources:ReleaseResource(resource)
                                return
                            end
                            quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP")
                            while not quest:MsgIsGameInfoClickedPast() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId2)
                                    resources:ReleaseResource(resource)
                                    do return end
                                end
                            end
                        else
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId2)
                                resources:ReleaseResource(resource)
                                return
                            end
                            quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP_PC")
                            while not quest:MsgIsGameInfoClickedPast() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId2)
                                    resources:ReleaseResource(resource)
                                    do return end
                                end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId2)
                            resources:ReleaseResource(resource)
                            return
                        end
                    end
                    ctr_c = (scratchValue + 1) % 5
                end
                if quest:GetTimer(timerId2) >= 1 then
                    tutorialState = quest:GetStateInt("TutorialState")
                else
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_MELEE_WAIT_INSULT", me, hero, false)
                    quest:SetTimer(timerId2, 15)
                    tutorialState = quest:GetStateInt("TutorialState")
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource)
                return
            end
            while quest:GetStateInt("TutorialState") ~= 4 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource)
                return
            end
            quest:EntitySetCombatType(me, "HERO_WHISPER_TUTORIAL_VS_BLOCK_ATTACK_STYLE")
            quest:GiveThingBestEnemyTarget(me, hero)
            quest:EntitySetInFaction(me, "FACTION_BANDITS")
            me:SetFriendsWithEverythingFlag(false)
            ctr_c = 0
            quest:SetTimer(timerId2, 15)
            while quest:GetStateInt("TutorialState") == 4 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(resource)
                    return
                end
                if not quest:IsPlayerCreatureBlocking() then goto LAB_00d57276 end
                -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc | 1);
                if not hero:MsgIsHitBy("MeleeOpponent") then goto LAB_00d57276 end
                predicateResult = true
                goto FLOW_past_lab_00d57276
                ::LAB_00d57276::
                predicateResult = false
                ::FLOW_past_lab_00d57276::
                if movie & 1 ~= 0 then
                    -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc & 0xfffffffe);
                end
                if predicateResult then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    quest:SetStateInt("GenericTutorialCounter", quest:GetStateInt("GenericTutorialCounter") + 1)
                    if quest:GetTimer(timerId2) < 9 then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId2)
                            resources:ReleaseResource(resource)
                            return
                        end
                        addNewConversation = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(addNewConversation, hero)
                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", me, hero, false)
                        goto LAB_00d57742
                    end
                elseif hero:MsgIsHitBy("MeleeOpponent") then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    if ctr_c == 0 then
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                        local theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
                        addNewConversation = quest:AddNewConversation(theRealGuildmaster, false, false)
                        quest:AddPersonToConversation(addNewConversation, hero)
                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_BLOCK", theRealGuildmaster, hero, false)
                        if quest:IsXbox() then
                            if not quest:IsActiveThreadTerminating() then
                                quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP")
                                while not quest:MsgIsGameInfoClickedPast() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d57f71 end
                                end
                                goto LAB_00d5754c
                            end
                        elseif not quest:IsActiveThreadTerminating() then
                            quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP_PC")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d57f71 end
                            end
                            goto LAB_00d5754c
                        end
                        goto FLOW_past_lab_00d5754c
                        ::LAB_00d5754c::
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00d5754c end
                        quest:SetTimer(timerId2, 15)
                        ctr_c = (ctr_c + 1) % 5
                        goto LAB_00d5775c
                        ::FLOW_past_lab_00d5754c::
                        ::LAB_00d57f71::
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    scratchValue8 = math.random(0, 32767) & 0x80000001
                    if scratchValue8 < 0 then
                        scratchValue8 = (scratchValue8 - 1 | 0xfffffffe) + 1
                    end
                    if scratchValue8 == 1 then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId2)
                            resources:ReleaseResource(resource)
                            return
                        end
                        if quest:GetTimer(timerId2) < 9 then
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId2)
                                resources:ReleaseResource(resource)
                                return
                            end
                            addNewConversation = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(addNewConversation, hero)
                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", me, hero, false)
                            quest:SetTimer(timerId2, 15)
                        end
                    end
                    ctr_c = (ctr_c + 1) % 5
                elseif me:MsgIsHitByHero() then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    if quest:GetTimer(timerId2) < 9 then
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                        addNewConversation = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(addNewConversation, hero)
                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_MELEE_NOT_BLOCK", me, hero, false)
                        goto LAB_00d57742
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                goto FLOW_past_lab_00d57742
                ::LAB_00d57742::
                quest:SetTimer(timerId2, 15)
                ::FLOW_past_lab_00d57742::
                ::LAB_00d5775c::
                if quest:GetTimer(timerId2) < 1 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_BLOCK_WAIT_INSULT", me, hero, false)
                    quest:SetTimer(timerId2, 15)
                end
                if quest:GetHealth(hero) < quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_MinHealth) then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    quest:ClearThingBestEnemyTarget(me)
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource)
                return
            end
            while quest:GetStateInt("TutorialState") ~= 6 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource)
                return
            end
            quest:EntitySetCombatType(me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE")
            timerId = timerId2
            quest:SetTimer(timerId2, 15)
            while quest:GetStateInt("TutorialState") == 6 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(resource)
                    return
                end
                if me:MsgIsHitByHero() then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    if quest:GetTimer(timerId) < 9 then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId2)
                            resources:ReleaseResource(resource)
                            return
                        end
                        scratchValue8 = math.random(0, 32767) & 0x80000001
                        if scratchValue8 < 0 then
                            scratchValue8 = (scratchValue8 - 1 | 0xfffffffe) + 1
                        end
                        if scratchValue8 == 1 then
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId2)
                                resources:ReleaseResource(resource)
                                return
                            end
                            addNewConversation = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(addNewConversation, hero)
                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", me, hero, false)
                        else
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId2)
                                resources:ReleaseResource(resource)
                                return
                            end
                            local meleeThunder = quest:GetThingWithScriptName("MeleeThunder")
                            addNewConversation = quest:AddNewConversation(meleeThunder, false, false)
                            quest:AddPersonToConversation(addNewConversation, hero)
                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_THUNDER_MELEE_DEFEND", meleeThunder, hero, false)
                        end
                        goto FLOW_hoist_lab_00d57e71_1
                    end
                else
                    if not quest:IsPlayerCreatureBlocking() then goto LAB_00d57af8 end
                    -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc | 2);
                    if not hero:MsgIsHitBy("MeleeOpponent") then goto LAB_00d57af8 end
                    predicateResult47 = true
                    goto FLOW_past_lab_00d57af8
                    ::LAB_00d57af8::
                    predicateResult47 = false
                    ::FLOW_past_lab_00d57af8::
                    if movie & 2 ~= 0 then
                        -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc & 0xfffffffd);
                    end
                    if predicateResult47 then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId2)
                            resources:ReleaseResource(resource)
                            return
                        end
                        if quest:GetTimer(timerId) < 9 then
                            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                            scratchValue8 = math.random(0, 32767) & 0x80000001
                            if scratchValue8 < 0 then
                                scratchValue8 = (scratchValue8 - 1 | 0xfffffffe) + 1
                            end
                            if scratchValue8 == 1 then
                                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                                addNewConversation = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(addNewConversation, hero)
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", me, hero, false)
                                goto LAB_00d57e76
                            else
                                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                                local meleeThunder2 = quest:GetThingWithScriptName("MeleeThunder")
                                addNewConversation = quest:AddNewConversation(meleeThunder2, false, false)
                                quest:AddPersonToConversation(addNewConversation, hero)
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_THUNDER_MELEE_ATTACK", meleeThunder2, hero, false)
                                goto LAB_00d57e71
                            end
                            quest:DeregisterTimer(timerId2)
                            resources:ReleaseResource(resource)
                            return
                        end
                    elseif hero:MsgIsHitBy("MeleeOpponent") then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId2)
                            resources:ReleaseResource(resource)
                            return
                        end
                        if quest:GetTimer(timerId) < 9 then
                            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                            scratchValue8 = math.random(0, 32767) & 0x80000001
                            if scratchValue8 < 0 then
                                scratchValue8 = (scratchValue8 - 1 | 0xfffffffe) + 1
                            end
                            if scratchValue8 == 1 then
                                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                                addNewConversation = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(addNewConversation, hero)
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", me, hero, false)
                                goto LAB_00d57e76
                            else
                                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); resources:ReleaseResource(resource); return end
                                local meleeThunder3 = quest:GetThingWithScriptName("MeleeThunder")
                                addNewConversation = quest:AddNewConversation(meleeThunder3, false, false)
                                quest:AddPersonToConversation(addNewConversation, hero)
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_THUNDER_MELEE_FINISH", meleeThunder3, hero, false)
                                goto LAB_00d57e71
                            end
                            quest:DeregisterTimer(timerId2)
                            resources:ReleaseResource(resource)
                            return
                        end
                    end
                end
                goto FLOW_past_lab_00d57e71
                ::LAB_00d57e71::
                ::FLOW_hoist_lab_00d57e71_1::
                goto LAB_00d57e76
                ::FLOW_past_lab_00d57e71::
                goto FLOW_past_lab_00d57e76
                ::LAB_00d57e76::
                quest:SetTimer(timerId2, 15)
                ::FLOW_past_lab_00d57e76::
                timerId = timerId2
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource)
                return
            end
            while quest:GetStateInt("TutorialState") ~= 7 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource)
                return
            end
            while not quest:GetStateBool("MeleeRepeatKnown") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource)
                return
            end
            if not quest:GetStateBool("MeleeRepeating") then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId2)
                    resources:ReleaseResource(resource)
                    return
                end
                repeatMelee = false
            elseif quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource)
                return
            end
            quest:SetStateBool("MeleeOpponentReset", true)
            quest:DeregisterTimer(timerId2)
        end
    end
    ::LAB_00d57f5d::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d5685e::
    resources:ReleaseResource(resource)
end

-- MeleeOpponent.Init (retail 0x00d56750)
function Init(quest, me)
    quest:SetStateInt("TutorialState", 1)
    badHit = 0
    repeatMelee = true
end

-- MeleeOpponent.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- MeleeOpponent.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

