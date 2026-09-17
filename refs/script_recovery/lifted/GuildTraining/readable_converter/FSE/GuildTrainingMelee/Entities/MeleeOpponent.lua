-- Readable native conversion: MeleeOpponent. Review coverage report before use.
-- Registration remains disabled until the package is verified.

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
    local resources = quest:RetailResources()
    local scratchValue, predicateResult25, predicateResult48, conversationId, conversationId2
    local conversationId3, conversationId4, conversationId5, conversationId6, conversationId7
    local conversationId8, conversationId9, conversationId10, conversationId11, conversationId12
    local conversationId13, scratchValue9, theRealGuildmaster, meleeThunder, meleeThunder2
    local meleeThunder3, scratchValue11, scratchValue12, scratchValue13, timerId
    scratchValue13 = resources:NewResource()
    while not resources:TryAcquire(scratchValue13, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(scratchValue13); return end
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
                        scratchValue12 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        while not resources:TryAcquire(scratchValue13, me, 4) do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue12)
                                resources:ReleaseResource(scratchValue13)
                                return
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue12)
                            resources:ReleaseResource("")
                            return
                        end
                        if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue13)) then
                            me:Speak(quest:GetHero(), "TEXT_QST_028_WHISPER_MEET", 0, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue12)
                                    resources:ReleaseResource(scratchValue13)
                                    return
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue12)
                                resources:ReleaseResource("")
                                return
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue12)
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
                        quest:GiveThingBestEnemyTarget(me, quest:GetHero())
                        quest:EntitySetInFaction(me, "FACTION_BANDITS")
                        me:SetFriendsWithEverythingFlag(me)
                        timerId = quest:RegisterTimer()
                        quest:SetTimer(timerId, 15)
                        while quest:GetStateInt("TutorialState") == 3 and quest:GetStateInt("GenericTutorialCounter") < 7 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(scratchValue13)
                                return
                            end
                            if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(scratchValue13)
                                    return
                                end
                                quest:SetStateInt("GenericTutorialCounter", quest:GetStateInt("GenericTutorialCounter") + 1)
                                if quest:GetTimer(timerId) < 9 then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(scratchValue13)
                                        return
                                    end
                                    conversationId = quest:AddNewConversation(me, false, false)
                                    quest:AddPersonToConversation(conversationId, quest:GetHero())
                                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", me, quest:GetHero(), false)
                                    quest:SetTimer(timerId, 15)
                                end
                            elseif me:MsgIsHitByHero() then
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(scratchValue13)
                                    return
                                end
                                quest:ModifyThingHealth(me, 1000.0, false)
                                if nil == nil then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(scratchValue13)
                                        return
                                    end
                                    if quest:IsXbox() then
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(scratchValue13)
                                            return
                                        end
                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP")
                                        while not quest:MsgIsGameInfoClickedPast() do
                                            quest:NewScriptFrame(me)
                                            if quest:IsActiveThreadTerminating() then
                                                quest:DeregisterTimer(timerId)
                                                resources:ReleaseResource(scratchValue13)
                                                return
                                            end
                                        end
                                    else
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(scratchValue13)
                                            return
                                        end
                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_SWORD_WIELD_HELP_PC")
                                        while not quest:MsgIsGameInfoClickedPast() do
                                            quest:NewScriptFrame(me)
                                            if quest:IsActiveThreadTerminating() then
                                                quest:DeregisterTimer(timerId)
                                                resources:ReleaseResource(scratchValue13)
                                                return
                                            end
                                        end
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(scratchValue13)
                                        return
                                    end
                                end
                                -- TODO(native): xStack_c8 = (CCharString)(((int)CVar3 + 1) % 5);
                            end
                            if quest:GetTimer(timerId) < 1 then
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(scratchValue13)
                                    return
                                end
                                conversationId2 = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(conversationId2, quest:GetHero())
                                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_WHISPER_MELEE_WAIT_INSULT", me, quest:GetHero(), false)
                                quest:SetTimer(timerId, 15)
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(scratchValue13)
                            return
                        end
                        while quest:GetStateInt("TutorialState") ~= 4 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(scratchValue13)
                                return
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(scratchValue13)
                            return
                        end
                        quest:EntitySetCombatType(me, "HERO_WHISPER_TUTORIAL_VS_BLOCK_ATTACK_STYLE")
                        quest:GiveThingBestEnemyTarget(me, quest:GetHero())
                        quest:EntitySetInFaction(me, "FACTION_BANDITS")
                        me:SetFriendsWithEverythingFlag(me)
                        quest:SetTimer(timerId, 15)
                        while quest:GetStateInt("TutorialState") == 4 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(scratchValue13)
                                return
                            end
                            if quest:IsPlayerCreatureBlocking() then
                                -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc | 1);
                                scratchValue9 = quest:GetHero()
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                if not scratchValue9:MsgIsHitBy("MeleeOpponent") then
                                    predicateResult25 = false
                                    goto FLOW_after_lab_00d57276
                                end
                                predicateResult25 = true
                            else
                                predicateResult25 = false
                            end
                            ::FLOW_after_lab_00d57276::
                            if scratchValue12 & 1 ~= 0 then
                                -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc & 0xfffffffe);
                            end
                            if predicateResult25 then
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(scratchValue13)
                                    return
                                end
                                -- TODO(native): *piVar1 = *piVar1 + 1;
                                if quest:GetTimer(timerId) < 9 then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(scratchValue13)
                                        return
                                    end
                                    conversationId3 = quest:AddNewConversation(me, false, false)
                                    quest:AddPersonToConversation(conversationId3, quest:GetHero())
                                    quest:AddLineToConversation(conversationId3, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", me, quest:GetHero(), false)
                                    quest:SetTimer(timerId, 15)
                                end
                            else
                                scratchValue9 = quest:GetHero()
                                -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                if scratchValue9:MsgIsHitBy("MeleeOpponent") then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(scratchValue13)
                                        return
                                    end
                                    if nil == nil then
                                        if not quest:IsActiveThreadTerminating() then
                                            theRealGuildmaster = quest:GetThingWithScriptName("TheRealGuildmaster")
                                            conversationId4 = quest:AddNewConversation(theRealGuildmaster, false, false)
                                            quest:AddPersonToConversation(conversationId4, quest:GetHero())
                                            quest:AddLineToConversation(conversationId4, "TEXT_QST_028_MAZE_BLOCK", theRealGuildmaster, quest:GetHero(), false)
                                            if quest:IsXbox() then
                                                if not quest:IsActiveThreadTerminating() then
                                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_BLOCK_HELP")
                                                    while not quest:MsgIsGameInfoClickedPast() do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00d57f71 end
                                                    end
                                                    if not quest:IsActiveThreadTerminating() then
                                                        quest:SetTimer(timerId, 15)
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
                                                    quest:SetTimer(timerId, 15)
                                                    -- TODO(native): xStack_c8 = (CCharString)(((int)xStack_c8 + 1) % 5);
                                                    goto LAB_00d5775c
                                                end
                                            end
                                            ::LAB_00d57f71::
                                        end
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(scratchValue13)
                                        return
                                    end
                                    scratchValue11 = scratchValue11 & 0x80000001
                                    if scratchValue11 < 0 then
                                        scratchValue11 = (scratchValue11 - 1 | 0xfffffffe) + 1
                                    end
                                    if scratchValue11 == 1 then
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(scratchValue13)
                                            return
                                        end
                                        if quest:GetTimer(timerId) < 9 then
                                            if quest:IsActiveThreadTerminating() then
                                                quest:DeregisterTimer(timerId)
                                                resources:ReleaseResource(scratchValue13)
                                                return
                                            end
                                            conversationId5 = quest:AddNewConversation(me, false, false)
                                            quest:AddPersonToConversation(conversationId5, quest:GetHero())
                                            quest:AddLineToConversation(conversationId5, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", me, quest:GetHero(), false)
                                            quest:SetTimer(timerId, 15)
                                        end
                                    end
                                    -- TODO(native): xStack_c8 = (CCharString)(((int)xStack_c8 + 1) % 5);
                                elseif me:MsgIsHitByHero() then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(scratchValue13)
                                        return
                                    end
                                    if quest:GetTimer(timerId) < 9 then
                                        if not quest:IsActiveThreadTerminating() then
                                            conversationId6 = quest:AddNewConversation(me, false, false)
                                            quest:AddPersonToConversation(conversationId6, quest:GetHero())
                                            quest:AddLineToConversation(conversationId6, "TEXT_QST_028_WHISPER_MELEE_NOT_BLOCK", me, quest:GetHero(), false)
                                            quest:SetTimer(timerId, 15)
                                        else
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(scratchValue13)
                                            return
                                        end
                                    end
                                end
                            end
                            ::LAB_00d5775c::
                            if quest:GetTimer(timerId) < 1 then
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(scratchValue13)
                                    return
                                end
                                conversationId7 = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(conversationId7, quest:GetHero())
                                quest:AddLineToConversation(conversationId7, "TEXT_QST_028_WHISPER_BLOCK_WAIT_INSULT", me, quest:GetHero(), false)
                                quest:SetTimer(timerId, 15)
                            end
                            if quest:GetHealth(quest:GetHero()) < quest:ReadGlobalGameDataFloat(3800) then
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(scratchValue13)
                                    return
                                end
                                quest:ClearThingBestEnemyTarget(me)
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(scratchValue13)
                            return
                        end
                        while quest:GetStateInt("TutorialState") ~= 6 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(scratchValue13)
                                return
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(scratchValue13)
                            return
                        end
                        quest:EntitySetCombatType(me, "HERO_WHISPER_TUTORIAL_ATTACK_STYLE")
                        scratchValue = timerId
                        quest:SetTimer(timerId, 15)
                        while quest:GetStateInt("TutorialState") == 6 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(scratchValue13)
                                return
                            end
                            if me:MsgIsHitByHero() then
                                if quest:IsActiveThreadTerminating() then
                                    quest:DeregisterTimer(timerId)
                                    resources:ReleaseResource(scratchValue13)
                                    return
                                end
                                if quest:GetTimer(scratchValue) < 9 then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(scratchValue13)
                                        return
                                    end
                                    scratchValue11 = scratchValue11 & 0x80000001
                                    if scratchValue11 < 0 then
                                        scratchValue11 = (scratchValue11 - 1 | 0xfffffffe) + 1
                                    end
                                    if scratchValue11 == 1 then
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(scratchValue13)
                                            return
                                        end
                                        conversationId8 = quest:AddNewConversation(me, false, false)
                                        quest:AddPersonToConversation(conversationId8, quest:GetHero())
                                        quest:AddLineToConversation(conversationId8, "TEXT_QST_028_WHISPER_MELEE_HIT_INSULT", me, quest:GetHero(), false)
                                    else
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(scratchValue13)
                                            return
                                        end
                                        meleeThunder = quest:GetThingWithScriptName("MeleeThunder")
                                        conversationId9 = quest:AddNewConversation(meleeThunder, false, false)
                                        quest:AddPersonToConversation(conversationId9, quest:GetHero())
                                        quest:AddLineToConversation(conversationId9, "TEXT_QST_028_THUNDER_MELEE_DEFEND", meleeThunder, quest:GetHero(), false)
                                    end
                                    quest:SetTimer(timerId, 15)
                                end
                            else
                                if quest:IsPlayerCreatureBlocking() then
                                    -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc | 2);
                                    scratchValue9 = quest:GetHero()
                                    -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                    if not scratchValue9:MsgIsHitBy("MeleeOpponent") then
                                        predicateResult48 = false
                                        goto FLOW_after_lab_00d57af8
                                    end
                                    predicateResult48 = true
                                else
                                    predicateResult48 = false
                                end
                                ::FLOW_after_lab_00d57af8::
                                if scratchValue12 & 2 ~= 0 then
                                    -- TODO(native): xStack_bc = (undefined **)((uint)xStack_bc & 0xfffffffd);
                                end
                                if predicateResult48 then
                                    if quest:IsActiveThreadTerminating() then
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(scratchValue13)
                                        return
                                    end
                                    if quest:GetTimer(scratchValue) < 9 then
                                        if not quest:IsActiveThreadTerminating() then
                                            scratchValue11 = scratchValue11 & 0x80000001
                                            if scratchValue11 < 0 then
                                                scratchValue11 = (scratchValue11 - 1 | 0xfffffffe) + 1
                                            end
                                            if scratchValue11 == 1 then
                                                if not quest:IsActiveThreadTerminating() then
                                                    conversationId10 = quest:AddNewConversation(me, false, false)
                                                    quest:AddPersonToConversation(conversationId10, quest:GetHero())
                                                    quest:AddLineToConversation(conversationId10, "TEXT_QST_028_WHISPER_BLOCK_HIT_INSULT", me, quest:GetHero(), false)
                                                    quest:SetTimer(timerId, 15)
                                                    goto FLOW_after_lab_00d57e76
                                                end
                                            elseif not quest:IsActiveThreadTerminating() then
                                                meleeThunder2 = quest:GetThingWithScriptName("MeleeThunder")
                                                conversationId11 = quest:AddNewConversation(meleeThunder2, false, false)
                                                quest:AddPersonToConversation(conversationId11, quest:GetHero())
                                                quest:AddLineToConversation(conversationId11, "TEXT_QST_028_THUNDER_MELEE_ATTACK", meleeThunder2, quest:GetHero(), false)
                                                quest:SetTimer(timerId, 15)
                                                goto FLOW_after_lab_00d57e76
                                            end
                                        end
                                        quest:DeregisterTimer(timerId)
                                        resources:ReleaseResource(scratchValue13)
                                        return
                                    end
                                else
                                    scratchValue9 = quest:GetHero()
                                    -- TODO(native): MsgIsHitBy is not a ForgeFSE binding
                                    if scratchValue9:MsgIsHitBy("MeleeOpponent") then
                                        if quest:IsActiveThreadTerminating() then
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(scratchValue13)
                                            return
                                        end
                                        if quest:GetTimer(scratchValue) < 9 then
                                            if not quest:IsActiveThreadTerminating() then
                                                scratchValue11 = scratchValue11 & 0x80000001
                                                if scratchValue11 < 0 then
                                                    scratchValue11 = (scratchValue11 - 1 | 0xfffffffe) + 1
                                                end
                                                if scratchValue11 == 1 then
                                                    if not quest:IsActiveThreadTerminating() then
                                                        conversationId12 = quest:AddNewConversation(me, false, false)
                                                        quest:AddPersonToConversation(conversationId12, quest:GetHero())
                                                        quest:AddLineToConversation(conversationId12, "TEXT_QST_028_WHISPER_BATTLE_HIT_INSULT", me, quest:GetHero(), false)
                                                        quest:SetTimer(timerId, 15)
                                                        goto FLOW_after_lab_00d57e76
                                                    end
                                                elseif not quest:IsActiveThreadTerminating() then
                                                    meleeThunder3 = quest:GetThingWithScriptName("MeleeThunder")
                                                    conversationId13 = quest:AddNewConversation(meleeThunder3, false, false)
                                                    quest:AddPersonToConversation(conversationId13, quest:GetHero())
                                                    quest:AddLineToConversation(conversationId13, "TEXT_QST_028_THUNDER_MELEE_FINISH", meleeThunder3, quest:GetHero(), false)
                                                    quest:SetTimer(timerId, 15)
                                                    goto FLOW_after_lab_00d57e76
                                                end
                                            end
                                            quest:DeregisterTimer(timerId)
                                            resources:ReleaseResource(scratchValue13)
                                            return
                                        end
                                    end
                                end
                            end
                            ::FLOW_after_lab_00d57e76::
                            scratchValue = timerId
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(scratchValue13)
                            return
                        end
                        while quest:GetStateInt("TutorialState") ~= 7 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(scratchValue13)
                                return
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(scratchValue13)
                            return
                        end
                        while not quest:GetStateBool("MeleeRepeatKnown") do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(scratchValue13)
                                return
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(scratchValue13)
                            return
                        end
                        if not quest:GetStateBool("MeleeRepeating") then
                            if quest:IsActiveThreadTerminating() then
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(scratchValue13)
                                return
                            end
                            state:SetBool("RepeatMelee", false)
                        elseif quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(scratchValue13)
                            return
                        end
                        quest:SetStateBool("MeleeOpponentReset", true)
                        quest:DeregisterTimer(timerId)
                    end
                end
                ::LAB_00d57f5d::
                resources:ReleaseResource(scratchValue13)
                return
            end
        end
    end
    ::LAB_00d5685e::
    resources:ReleaseResource(scratchValue13)
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

