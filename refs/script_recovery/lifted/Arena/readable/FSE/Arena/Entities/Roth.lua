-- Readable native conversion: Roth. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local earlyTalk_, chamTalk_, hintNumber_

-- Roth.Main (retail 0x00f21880)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue, predicateResult, controlAcquired, predicateResult9, predicateResult30
    local scratchValue13, timerId, registerTimer, switch, switch3, p1, movie, resource3, movie2
    local resource4, movie3, movie4, actorMap, actorMap2
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        local movie = movie2
        resources:DestroyMovie(movie)
        quest:DeregisterTimer(registerTimer)
        resources:ReleaseResource(resource4)
    end
    local function ReleaseEverything2()
        local movie = movie4
        resources:DestroyMovie(movie)
        quest:DeregisterTimer(registerTimer)
        resources:ReleaseResource(resource4)
    end
    local function ReleaseEverything3()
        resources:DestroyMovie(movie)
        quest:DeregisterTimer(registerTimer)
        resources:ReleaseResource(resource4)
    end
    resource4 = resources:NewResource()
    resource3 = resource4
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    while quest:GetStateInt("ArenaState") == 2 do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource4)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource4); return end
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    local position = quest:GetThingWithScriptName("RothPoint"):GetPos()
    scratchValue = position.x
    p1 = nil
    quest:SetWanderCentrePoint(me, position)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 4.0)
    quest:SetScriptingStateGroup(me, 4)
    quest:SetThingHasInformation(me, false, true, false)
    registerTimer = quest:RegisterTimer()
    scratchValue13 = 0
    timerId = registerTimer
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            quest:DeregisterTimer(registerTimer)
            resources:ReleaseResource(resource4)
            return
        end
        if quest:GetStateBool("PlayerLeaving") then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(registerTimer)
                resources:ReleaseResource(resource4)
                return
            end
            local pQuestName = quest:GetActiveQuestName()
            quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_ARENA_OBJECTIVE_05", "Arena", "KnotholeGlade")
            resources:PrepareResource(resource4)
            while not resources:TryAcquire(resource4, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(registerTimer)
                    resources:ReleaseResource(resource4)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(registerTimer)
                resources:ReleaseResource(resource4)
                return
            end
            local resource5 = resources:NewResource()
            resources:PrepareResource(resource5)
            local resource = resource5
            local hero6 = quest:GetHero()
            controlAcquired = resources:TryAcquire(resource, hero6, 4)
            while not controlAcquired do
                if not quest:NewScriptFrame(me) then goto LAB_00f22c81 end
                local resource2 = resource5
                local hero7 = quest:GetHero()
                controlAcquired = resources:TryAcquire(resource2, hero7, 4)
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00f22c81 end
            actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "Hero", resource5)
            resources:SetActor(actorMap, "Roth", resource4)
            actorMap2 = resources:NewStringMap()
            switch = quest:GetStateInt("GoldMultiplier")
            repeat
                if switch == 0 or switch == 1 then
                    resources:SetString(actorMap2, "$SAY", "TEXT_QST_005_V2_ARENA_ROTH_SHORT_ARENA")
                    break
                elseif switch == 2 or switch == 3 then
                    resources:SetString(actorMap2, "$SAY", "TEXT_QST_005_V2_ARENA_ROTH_MIDDLE_ARENA")
                    break
                elseif switch == 4 then
                    resources:SetString(actorMap2, "$SAY", "TEXT_QST_005_V2_ARENA_ROTH_GOOD_ARENA")
                    break
                else
                    resources:SetString(actorMap2, "$SAY", "TEXT_QST_005_V2_ARENA_ROTH_EXCELLENT_ARENA")
                end
            until true
            movie3 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:FixMovieSequenceCamera(true)
            resources:RunMacroWithStrings("CS_ARENA_ROTHWIMP", actorMap, actorMap2, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetStateBool("PlayerLeaving", false)
            earlyTalk_ = true
            chamTalk_ = true
            resources:PrepareResource(resource4)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            resources:DestroyStringMap(actorMap2)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource5)
            goto LAB_00f21f12
            ::LAB_00f22c81::
            resources:ReleaseResource(resource5)
            quest:DeregisterTimer(registerTimer)
            resources:ReleaseResource(resource4)
            return
        end
        ::LAB_00f21f12::
        if quest:GetStateInt("ArenaState") ~= 5 then goto LAB_00f221ae end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(registerTimer)
            resources:ReleaseResource(resource4)
            return
        end
        predicateResult9 = quest:IsActiveThreadTerminating()
        if scratchValue13 == 2 then
            if predicateResult9 then quest:DeregisterTimer(registerTimer); resources:ReleaseResource(resource4); return end
            local hero = quest:GetHero()
            if quest:IsDistanceBetweenThingsUnder(me, hero, 5.0) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registerTimer); resources:ReleaseResource(resource4); return end
                local conversationId = quest:AddNewConversation(me, false, false)
                local hero8 = quest:GetHero()
                quest:AddPersonToConversation(conversationId, hero8)
                local hero2 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_005_V2_ARENA_ROTH_COME_ON", me, hero2, false)
                scratchValue13 = 1
                quest:SetTimer(timerId, 10)
                local position2 = quest:GetThingWithScriptName("RothPoint"):GetPos()
                local center = {x = position2.x, y = position2.y, z = position2.z}
                quest:SetWanderCentrePoint(me, center)
                registerTimer = timerId
                timerId = registerTimer
                goto LAB_00f221ae
            else
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registerTimer); resources:ReleaseResource(resource4); return end
                scratchValue13 = quest:GetTimer(registerTimer)
                if 0 < scratchValue13 then goto LAB_00f221ae end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registerTimer); resources:ReleaseResource(resource4); return end
                local position3 = quest:GetHero():GetPos()
                local scratchValue2 = {x = position3.x, y = position3.y, z = position3.z}
                quest:SetWanderCentrePoint(me, scratchValue2)
                goto LAB_00f220f0
            end
            quest:DeregisterTimer(registerTimer)
            resources:ReleaseResource(resource4)
            return
        end
        if predicateResult9 then
            quest:DeregisterTimer(registerTimer)
            resources:ReleaseResource(resource4)
            return
        end
        if quest:GetTimer(registerTimer) < 1 and scratchValue13 == 1 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(registerTimer)
                resources:ReleaseResource(resource4)
                return
            end
            local position4 = quest:GetHero():GetPos()
            local center_00 = {x = position4.x, y = position4.y, z = position4.z}
            quest:SetWanderCentrePoint(me, center_00)
            scratchValue13 = 2
            goto LAB_00f220f0
        elseif scratchValue13 == 0 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(registerTimer)
                resources:ReleaseResource(resource4)
                return
            end
            quest:SetTimer(registerTimer, 10)
            scratchValue13 = 1
        end
        goto FLOW_past_lab_00f220f0
        ::LAB_00f220f0::
        quest:SetTimer(registerTimer, 3)
        ::FLOW_past_lab_00f220f0::
        ::LAB_00f221ae::
        if not me:IsTalkedToByHero() then
            if me:MsgIsHitByHero() then
                goto LAB_00f22979
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f22979 end
                predicateResult30 = false
            end
            goto FLOW_past_lab_00f22979
            ::LAB_00f22979::
            predicateResult30 = true
            ::FLOW_past_lab_00f22979::
            if predicateResult30 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(registerTimer)
                    resources:ReleaseResource(resource4)
                    return
                end
                resources:PrepareResource(resource4)
                while not resources:TryAcquire(resource4, me, 4) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(registerTimer)
                        resources:ReleaseResource(resource4)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(registerTimer)
                    resources:ReleaseResource(resource4)
                    return
                end
                scratchValue13 = resources:IsPerformingScriptTask(resource4)
                if scratchValue13 then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(registerTimer)
                        resources:ReleaseResource(resource4)
                        return
                    end
                    resources:ClearCommands(resource4)
                end
                if not quest:GetStateBool("InHitCutsceneAlready") then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(registerTimer)
                        resources:ReleaseResource(resource4)
                        return
                    end
                    quest:SetStateBool("InHitCutsceneAlready", true)
                    movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue26 = resources:ScriptThing(resource4)
                    if quest:GetHealth(scratchValue26) > 0.0 then
                        p1 = quest:GetHero()
                        resources:Speak(resource4, p1, "TEXT_QST_005_V2_ARENA_ROTH_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
                        scratchValue13 = resources:IsPerformingScriptTask(resource4)
                        while scratchValue13 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                scratchValue13 = resources:IsPerformingScriptTask(resource4)
                            else
                                quest:PauseAllNonScriptedEntities(false)
                                ReleaseEverything2(); do return end
                                scratchValue13 = resources:IsPerformingScriptTask(resource4)
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            ReleaseEverything2()
                            return
                        end
                    end
                    quest:ModifyThingHealth(me, 10000.0, false)
                    local hero5 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, hero5)
                    local pThing1 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(pThing1, me)
                    quest:SetStateBool("InHitCutsceneAlready", false)
                    quest:PauseAllNonScriptedEntities(false)
                    movie = movie4
                    goto LAB_00f22bf5
                end
                goto LAB_00f22bfa
            end
            goto LAB_00f22c10
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(registerTimer)
            resources:ReleaseResource(resource4)
            return
        end
        resources:PrepareResource(resource4)
        while not resources:TryAcquire(resource4, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(registerTimer)
                resources:ReleaseResource(resource4)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(registerTimer)
            resources:ReleaseResource(resource4)
            return
        end
        if resources:IsPerformingScriptTask(resource4) then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(registerTimer)
                resources:ReleaseResource(resource4)
                return
            end
            resources:ClearCommands(resource4)
        end
        movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        scratchValue13 = quest:GetStateInt("ArenaState")
        if scratchValue13 == 3 and not earlyTalk_ then
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            local scratchValue16 = resources:ScriptThing(resource4)
            if quest:GetHealth(scratchValue16) > 0.0 then
                p1 = quest:GetHero()
                resources:Speak(resource4, p1, "TEXT_QST_005_V2_ARENA_ROTH_FIRST_CHAT_PRE_CHAM", GROUP_SELECT_FIRST, false, true, false)
                scratchValue13 = resources:IsPerformingScriptTask(resource4)
                while scratchValue13 do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    else
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie2
                        ReleaseEverything3(); do return end
                        scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    end
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            end
            earlyTalk_ = true
            goto FLOW_native_label_1
            ReleaseEverything()
            return
        end
        if scratchValue13 == 5 and not chamTalk_ then
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            local scratchValue17 = resources:ScriptThing(resource4)
            if quest:GetHealth(scratchValue17) > 0.0 then
                p1 = quest:GetHero()
                resources:Speak(resource4, p1, "TEXT_QST_005_V2_ARENA_ROTH_FIRST_CHAT_POST_CHAM", GROUP_SELECT_FIRST, false, true, false)
                scratchValue13 = resources:IsPerformingScriptTask(resource4)
                while scratchValue13 do
                    if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                    scratchValue13 = resources:IsPerformingScriptTask(resource4)
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            end
            chamTalk_ = true
            goto FLOW_native_label_1
            ReleaseEverything()
            return
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
        switch3 = hintNumber_
        if not (switch3 == 0 or switch3 == 1 or switch3 == 2 or switch3 == 3 or switch3 == 4 or switch3 == 5) then
            switch3 = 0x7ffffffe
        end
        repeat
            if switch3 == 0 then
                local scratchValue18 = resources:ScriptThing(resource4)
                if quest:GetHealth(scratchValue18) > 0.0 then
                    p1 = quest:GetHero()
                    resources:Speak(resource4, p1, "TEXT_QST_005_V2_ARENA_ROTH_FIRST_HINT", GROUP_SELECT_FIRST, false, true, false)
                    scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    while scratchValue13 do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    end
                    goto LAB_00f22807
                end
                goto FLOW_hoist_lab_00f22807_1
            end
            goto FLOW_past_lab_00f22807
            ::LAB_00f22807::
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            ::FLOW_hoist_lab_00f22807_1::
            break
            ::FLOW_past_lab_00f22807::
            if switch3 == 1 then
                local scratchValue19 = resources:ScriptThing(resource4)
                if quest:GetHealth(scratchValue19) > 0.0 then
                    p1 = quest:GetHero()
                    resources:Speak(resource4, p1, "TEXT_QST_005_V2_ARENA_ROTH_SECOND_HINT", GROUP_SELECT_FIRST, false, true, false)
                    scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    while scratchValue13 do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    end
                    goto LAB_00f22807
                end
                break
            end
            if switch3 == 2 then
                local scratchValue20 = resources:ScriptThing(resource4)
                if quest:GetHealth(scratchValue20) > 0.0 then
                    p1 = quest:GetHero()
                    resources:Speak(resource4, p1, "TEXT_QST_005_V2_ARENA_ROTH_THIRD_HINT", GROUP_SELECT_FIRST, false, true, false)
                    scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    while scratchValue13 do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    end
                    goto LAB_00f22807
                end
                break
            end
            if switch3 == 3 then
                local scratchValue21 = resources:ScriptThing(resource4)
                if quest:GetHealth(scratchValue21) > 0.0 then
                    p1 = quest:GetHero()
                    resources:Speak(resource4, p1, "TEXT_QST_005_V2_ARENA_ROTH_FOURTH_HINT", GROUP_SELECT_FIRST, false, true, false)
                    scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    while scratchValue13 do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    end
                    goto LAB_00f22807
                end
                break
            end
            if switch3 == 4 then
                local scratchValue22 = resources:ScriptThing(resource4)
                if quest:GetHealth(scratchValue22) > 0.0 then
                    p1 = quest:GetHero()
                    resources:Speak(resource4, p1, "TEXT_QST_005_V2_ARENA_ROTH_FIFTH_HINT", GROUP_SELECT_FIRST, false, true, false)
                    scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    while scratchValue13 do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    end
                    goto LAB_00f22807
                end
                break
            end
            if switch3 == 5 then
                local scratchValue23 = resources:ScriptThing(resource4)
                if quest:GetHealth(scratchValue23) > 0.0 then
                    p1 = quest:GetHero()
                    resources:Speak(resource4, p1, "TEXT_QST_005_V2_ARENA_ROTH_SIXTH_HINT", GROUP_SELECT_FIRST, false, true, false)
                    scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    while scratchValue13 do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        scratchValue13 = resources:IsPerformingScriptTask(resource4)
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
                hintNumber_ = 4
                switch3 = 0x7ffffffe
            end
            if switch3 == 0x7ffffffe then goto FLOW_native_label_1 end
        until true
        hintNumber_ = hintNumber_ + 1
        ::FLOW_native_label_1::
        quest:PauseAllNonScriptedEntities(false)
        movie = movie2
        ::LAB_00f22bf5::
        resources:DestroyMovie(movie)
        ::LAB_00f22bfa::
        resources:PrepareResource(resource4)
        ::LAB_00f22c10::
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
    until false
end

-- Roth.Init (retail 0x00f14610)
function Init(quest, me)
    earlyTalk_ = false
    chamTalk_ = false
    hintNumber_ = 0
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

-- Roth.OnPersist (retail 0x00f21660)
function OnPersist(quest, me, context)
    local earlyTalk = earlyTalk_ or false
    earlyTalk_ = quest:PersistTransferBool(context, "EarlyTalk", earlyTalk)
    local chamTalk = chamTalk_ or false
    chamTalk_ = quest:PersistTransferBool(context, "ChamTalk", chamTalk)
    local hintNumber = hintNumber_ or 0
    hintNumber_ = quest:PersistTransferInt(context, "HintNumber", hintNumber)
end

-- Roth.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

