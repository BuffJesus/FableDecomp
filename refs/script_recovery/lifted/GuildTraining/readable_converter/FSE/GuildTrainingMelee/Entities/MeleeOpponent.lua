-- Readable native conversion: MeleeOpponent. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
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

-- MeleeOpponent.Main (retail 0x00d56790)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local timerId, predicateResult, predicateResult48, conversationId, conversationId2
    local conversationId3, conversationId4, conversationId5, conversationId6, conversationId7
    local conversationId8, conversationId9, conversationId10, conversationId11, conversationId12
    local conversationId13, theRealGuildmaster, meleeThunder, meleeThunder2, meleeThunder3
    local scratchValue, movie, resource, timerId2
    resource = resources:NewResource()
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:EntitySetAsKillable(me, false, true)
        while not quest:GetStateBool("TalkedToWhisper") do
            if not quest:NewScriptFrame(me) then goto LAB_00d5685e end
        end
        if not quest:IsActiveThreadTerminating() then
            me:MoveToPosition(quest:GetThingWithScriptName("M_MeleeOpponentStand"):GetPos(), 0x40400000, 1, false, true)
            while me:IsPerformingScriptTask() and not quest:GetStateBool("WhisperStopWalking") do
                if not quest:NewScriptFrame(me) then goto LAB_00d5685e end
            end
            if not quest:IsActiveThreadTerminating() then
                me:ClearCommands()
                quest:SetStateBool("WhisperArrived", true)
                quest:EntitySetAsKillable(me, false, true)
                while quest:GetStateInt("TutorialState") == 1 do
                    if not quest:NewScriptFrame(me) then goto LAB_00d57f5d end
                    if me:IsTalkedToByHero() then
                        movie = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        while not resources:TryAcquire(resource, me, 4) do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                resources:ReleaseResource(resource)
                                return
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            resources:ReleaseResource("")
                            return
                        end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_028_WHISPER_MEET", GROUP_SELECT_FIRST, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                resources:ReleaseResource(resource)
                                return
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                resources:ReleaseResource("")
                                return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                    elseif me:MsgIsHitByHero() then
                        quest:SetStateBool("EarlyHitWhisper", true)
                    end
                end
                if not quest:IsActiveThreadTerminating() then
                    while state:GetBool("RepeatMelee") do
                        if not quest:NewScriptFrame(me) then goto LAB_00d57f5d end
                        while quest:GetStateInt("TutorialState") ~= 3 do
                            if not quest:NewScriptFrame(me) then goto LAB_00d57f5d end
                        end
                        quest:SetStateBool("MeleeOpponentReset", false)
                        quest:EntitySetCombatType(me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE_DONT_ATTACK")
                        quest:GiveThingBestEnemyTarget(me, hero)
                        quest:EntitySetInFaction(me, "FACTION_BANDITS")
                        me:SetFriendsWithEverythingFlag(me)
                        timerId2 = quest:RegisterTimer()
                        quest:SetTimer(timerId2, 15)
                        while quest:GetStateInt("TutorialState") == 3 and quest:GetStateInt("GenericTutorialCounter") < 7 do
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
                                    conversationId = quest:AddNewConversation(me, false, false)
                                    quest:AddPersonToConversation(conversationId, hero)
                                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", me, hero, false)
                                    quest:SetTimer(timerId2, 15)
                                end
                            elseif me:MsgIsHitByHero() then
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId2)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                quest:ModifyThingHealth(me, 1000.0, false)
                                if nil == nil then
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
                                                return
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
                                                return
                                            end
                                        end
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId2)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                end
                                -- TODO(native): xStack_c8 = (CCharString)(((int)CVar3 + 1) % 5);
                            end
                            if quest:GetTimer(timerId2) < 1 then
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId2)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                conversationId2 = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(conversationId2, hero)
                                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_WHISPER_MELEE_WAIT_INSULT", me, hero, false)
                                quest:SetTimer(timerId2, 15)
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
                                return
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
                        me:SetFriendsWithEverythingFlag(me)
                        quest:SetTimer(timerId2, 15)
                        while quest:GetStateInt("TutorialState") == 4 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId2)
                                resources:ReleaseResource(resource)
                                return
                            end
                            if quest:IsPlayerCreatureBlocking() then
                                -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc | 1);
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                if not hero:MsgIsHitBy("MeleeOpponent") then
                                    predicateResult = false
                                    goto FLOW_after_lab_00d57276
                                end
                                predicateResult = true
                            else
                                predicateResult = false
                            end
                            ::FLOW_after_lab_00d57276::
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
                                    conversationId3 = quest:AddNewConversation(me, false, false)
                                    quest:AddPersonToConversation(conversationId3, hero)
                                    quest:AddLineToConversation(conversationId3, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", me, hero, false)
                                    quest:SetTimer(timerId2, 15)
                                end
                            else
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                if hero:MsgIsHitBy("MeleeOpponent") then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId2)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                    if nil == nil then
                                        if not quest:IsActiveThreadTerminating() then
                                            theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
                                            conversationId4 = quest:AddNewConversation(theRealGuildmaster, false, false)
                                            quest:AddPersonToConversation(conversationId4, hero)
                                            quest:AddLineToConversation(conversationId4, "TEXT_QST_028_MAZE_BLOCK", theRealGuildmaster, hero, false)
                                            if quest:IsXbox() then
                                                if not quest:IsActiveThreadTerminating() then
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP")
                                                    while not quest:MsgIsGameInfoClickedPast() do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d57f71 end
                                                    end
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:SetTimer(timerId2, 15)
                                                        -- TODO(native): xStack_c8 = (CCharString)(((int)xStack_c8 + 1) % 5);
                                                        goto LAB_00d5775c
                                                    end
                                                end
                                            elseif not quest:IsActiveThreadTerminating() then
                                                quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP_PC")
                                                while not quest:MsgIsGameInfoClickedPast() do
                                                    if not quest:NewScriptFrame(me) then goto LAB_00d57f71 end
                                                end
                                                if not quest:IsActiveThreadTerminating() then
                                                    quest:SetTimer(timerId2, 15)
                                                    -- TODO(native): xStack_c8 = (CCharString)(((int)xStack_c8 + 1) % 5);
                                                    goto LAB_00d5775c
                                                end
                                            end
                                            ::LAB_00d57f71::
                                        end
                                        quest:DeregisterTimer(timerId2)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                    scratchValue = scratchValue & 0x80000001
                                    if scratchValue < 0 then
                                        scratchValue = (scratchValue - 1 | 0xfffffffe) + 1
                                    end
                                    if scratchValue == 1 then
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
                                            conversationId5 = quest:AddNewConversation(me, false, false)
                                            quest:AddPersonToConversation(conversationId5, hero)
                                            quest:AddLineToConversation(conversationId5, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", me, hero, false)
                                            quest:SetTimer(timerId2, 15)
                                        end
                                    end
                                    -- TODO(native): xStack_c8 = (CCharString)(((int)xStack_c8 + 1) % 5);
                                elseif me:MsgIsHitByHero() then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId2)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                    if quest:GetTimer(timerId2) < 9 then
                                        if not quest:IsActiveThreadTerminating() then
                                            conversationId6 = quest:AddNewConversation(me, false, false)
                                            quest:AddPersonToConversation(conversationId6, hero)
                                            quest:AddLineToConversation(conversationId6, "TEXT_QST_028_WHISPER_MELEE_NOT_BLOCK", me, hero, false)
                                            quest:SetTimer(timerId2, 15)
                                        else
                                            quest:DeregisterTimer(timerId2)
                                            resources:ReleaseResource(resource)
                                            return
                                        end
                                    end
                                end
                            end
                            ::LAB_00d5775c::
                            if quest:GetTimer(timerId2) < 1 then
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId2)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                conversationId7 = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(conversationId7, hero)
                                quest:AddLineToConversation(conversationId7, "TEXT_QST_028_WHISPER_BLOCK_WAIT_INSULT", me, hero, false)
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
                                return
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
                                    scratchValue = scratchValue & 0x80000001
                                    if scratchValue < 0 then
                                        scratchValue = (scratchValue - 1 | 0xfffffffe) + 1
                                    end
                                    if scratchValue == 1 then
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId2)
                                            resources:ReleaseResource(resource)
                                            return
                                        end
                                        conversationId8 = quest:AddNewConversation(me, false, false)
                                        quest:AddPersonToConversation(conversationId8, hero)
                                        quest:AddLineToConversation(conversationId8, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", me, hero, false)
                                    else
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId2)
                                            resources:ReleaseResource(resource)
                                            return
                                        end
                                        meleeThunder = quest:GetThingWithScriptName("MeleeThunder")
                                        conversationId9 = quest:AddNewConversation(meleeThunder, false, false)
                                        quest:AddPersonToConversation(conversationId9, hero)
                                        quest:AddLineToConversation(conversationId9, "TEXT_QST_028_THUNDER_MELEE_DEFEND", meleeThunder, hero, false)
                                    end
                                    quest:SetTimer(timerId2, 15)
                                end
                            else
                                if quest:IsPlayerCreatureBlocking() then
                                    -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc | 2);
                                    -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                    if not hero:MsgIsHitBy("MeleeOpponent") then
                                        predicateResult48 = false
                                        goto FLOW_after_lab_00d57af8
                                    end
                                    predicateResult48 = true
                                else
                                    predicateResult48 = false
                                end
                                ::FLOW_after_lab_00d57af8::
                                if movie & 2 ~= 0 then
                                    -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc & 0xfffffffd);
                                end
                                if predicateResult48 then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId2)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                    if quest:GetTimer(timerId) < 9 then
                                        if not quest:IsActiveThreadTerminating() then
                                            scratchValue = scratchValue & 0x80000001
                                            if scratchValue < 0 then
                                                scratchValue = (scratchValue - 1 | 0xfffffffe) + 1
                                            end
                                            if scratchValue == 1 then
                                                if not quest:IsActiveThreadTerminating() then
                                                    conversationId10 = quest:AddNewConversation(me, false, false)
                                                    quest:AddPersonToConversation(conversationId10, hero)
                                                    quest:AddLineToConversation(conversationId10, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", me, hero, false)
                                                    quest:SetTimer(timerId2, 15)
                                                    goto FLOW_after_lab_00d57e76
                                                end
                                            elseif not quest:IsActiveThreadTerminating() then
                                                meleeThunder2 = quest:GetThingWithScriptName("MeleeThunder")
                                                conversationId11 = quest:AddNewConversation(meleeThunder2, false, false)
                                                quest:AddPersonToConversation(conversationId11, hero)
                                                quest:AddLineToConversation(conversationId11, "TEXT_QST_028_THUNDER_MELEE_ATTACK", meleeThunder2, hero, false)
                                                quest:SetTimer(timerId2, 15)
                                                goto FLOW_after_lab_00d57e76
                                            end
                                        end
                                        quest:DeregisterTimer(timerId2)
                                        resources:ReleaseResource(resource)
                                        return
                                    end
                                else
                                    -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                    if hero:MsgIsHitBy("MeleeOpponent") then
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId2)
                                            resources:ReleaseResource(resource)
                                            return
                                        end
                                        if quest:GetTimer(timerId) < 9 then
                                            if not quest:IsActiveThreadTerminating() then
                                                scratchValue = scratchValue & 0x80000001
                                                if scratchValue < 0 then
                                                    scratchValue = (scratchValue - 1 | 0xfffffffe) + 1
                                                end
                                                if scratchValue == 1 then
                                                    if not quest:IsActiveThreadTerminating() then
                                                        conversationId12 = quest:AddNewConversation(me, false, false)
                                                        quest:AddPersonToConversation(conversationId12, hero)
                                                        quest:AddLineToConversation(conversationId12, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", me, hero, false)
                                                        quest:SetTimer(timerId2, 15)
                                                        goto FLOW_after_lab_00d57e76
                                                    end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    meleeThunder3 = quest:GetThingWithScriptName("MeleeThunder")
                                                    conversationId13 = quest:AddNewConversation(meleeThunder3, false, false)
                                                    quest:AddPersonToConversation(conversationId13, hero)
                                                    quest:AddLineToConversation(conversationId13, "TEXT_QST_028_THUNDER_MELEE_FINISH", meleeThunder3, hero, false)
                                                    quest:SetTimer(timerId2, 15)
                                                    goto FLOW_after_lab_00d57e76
                                                end
                                            end
                                            quest:DeregisterTimer(timerId2)
                                            resources:ReleaseResource(resource)
                                            return
                                        end
                                    end
                                end
                            end
                            ::FLOW_after_lab_00d57e76::
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
                                return
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
                                return
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
                            state:SetBool("RepeatMelee", false)
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
                return
            end
        end
    end
    ::LAB_00d5685e::
    resources:ReleaseResource(resource)
end

-- MeleeOpponent.Init (retail 0x00d56750)
function Init(quest, me)
    quest:SetStateInt("TutorialState", 1)
    state:SetInt("BadHit", 0)
    state:SetBool("RepeatMelee", true)
end

-- MeleeOpponent.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- MeleeOpponent.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

