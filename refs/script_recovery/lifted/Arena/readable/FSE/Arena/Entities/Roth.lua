-- Readable native conversion: Roth. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local earlyTalk, chamTalk, hintNumber

-- Roth.Main (retail 0x00f21880)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, predicateResult8, fret_0, fret_00, fret_01, fret_02, fret_03, fret_04
    local fret_05, fret_06, scratchValue, registerTimer, switch, switch3, movie, scratchValue23
    local pCVar22_b3, thing, movie2, meControl, movie3, movie4, actorMap, actorMap2
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(thing ~= 0)
        local movie = movie2
        resources:DestroyMovie(movie)
        quest:DeregisterTimer(registerTimer)
        resources:DestroyMovie(meControl)
    end
    local function ReleaseEverything2()
        quest:PauseAllNonScriptedEntities(scratchValue23 ~= 0)
        local movie = movie2
        resources:DestroyMovie(movie)
        quest:DeregisterTimer(registerTimer)
        resources:DestroyMovie(meControl)
    end
    local function ReleaseEverything3()
        local movie = movie4
        resources:DestroyMovie(movie)
        quest:DeregisterTimer(registerTimer)
        resources:DestroyMovie(meControl)
    end
    local function ReleaseEverything4()
        resources:DestroyMovie(movie)
        quest:DeregisterTimer(registerTimer)
        resources:DestroyMovie(meControl)
    end
    resources:NewResource()
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    while quest:GetStateInt("ArenaState") == 2 do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then resources:DestroyMovie(meControl); return end
    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    thing = quest:GetThingWithScriptName("RothPoint")
    quest:SetWanderCentrePoint(me, thing:GetPos())
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 4.0)
    quest:SetScriptingStateGroup(me, 4)
    quest:SetThingHasInformation(me, false, true, false)
    registerTimer = quest:RegisterTimer()
    scratchValue = 0
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            quest:DeregisterTimer(registerTimer)
            return
        end
        if quest:GetStateBool("PlayerLeaving") then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(registerTimer)
                resources:ReleaseResource(meControl)
                return
            end
            quest:SetQuestCardObjective(quest:GetActiveQuestName(), "TEXT_QUEST_ARENA_OBJECTIVE_05", "Arena", "KnotholeGlade")
            resources:PrepareResource(meControl)
            while not resources:TryAcquire(meControl, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(registerTimer)
                    resources:ReleaseResource(meControl)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(registerTimer)
                resources:ReleaseResource(meControl)
                return
            end
            local resource = resources:NewResource()
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, hero, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00f22c81 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00f22c81 end
            actorMap = resources:NewActorMap()
            resources:SetActor(actorMap, "Hero", resource)
            -- TODO(native): resources:SetActor(xStack_bc, "Roth", &xStack_144)
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
            thing = 0
            resources:RunMacroWithStrings("CS_ARENA_ROTHWIMP", actorMap, actorMap2, false, true)
            quest:FixMovieSequenceCamera(false)
            quest:SetStateBool("PlayerLeaving", false)
            earlyTalk = true
            chamTalk = true
            resources:PrepareResource(meControl)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
            resources:DestroyStringMap(actorMap2)
            resources:DestroyActorMap(actorMap)
            resources:ReleaseResource(resource)
            goto LAB_00f21f12
            ::LAB_00f22c81::
            resources:ReleaseResource(resource)
            quest:DeregisterTimer(registerTimer)
            resources:DestroyMovie(meControl)
            return
        end
        ::LAB_00f21f12::
        if quest:GetStateInt("ArenaState") ~= 5 then goto LAB_00f221ae end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(registerTimer)
            resources:ReleaseResource(meControl)
            return
        end
        predicateResult8 = quest:IsActiveThreadTerminating()
        if scratchValue == 2 then
            if predicateResult8 then quest:DeregisterTimer(registerTimer); resources:DestroyMovie(meControl); return end
            if quest:IsDistanceBetweenThingsUnder(me, hero, 5.0) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registerTimer); resources:DestroyMovie(meControl); return end
                local conversationId = quest:AddNewConversation(me, false, false)
                thing = hero
                quest:AddPersonToConversation(conversationId, hero)
                thing = "TEXT_QST_005_V2_ARENA_ROTH_COME_ON"
                quest:AddLineToConversation(conversationId, thing, me, hero, false)
                scratchValue = 1
                quest:SetTimer(registerTimer, 10)
                local position = quest:GetThingWithScriptName("RothPoint"):GetPos()
                -- TODO(native): center._4_4_ = uVar17;
                -- TODO(native): center._0_4_ = uVar14;
                -- TODO(native): center._8_4_ = uVar21;
                quest:SetWanderCentrePoint(me, position)
                goto LAB_00f221ae
            else
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registerTimer); resources:DestroyMovie(meControl); return end
                scratchValue = quest:GetTimer(registerTimer)
                if 0 < scratchValue then goto LAB_00f221ae end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(registerTimer); resources:DestroyMovie(meControl); return end
                local position3 = hero:GetPos()
                -- TODO(native): CVar2._4_4_ = uVar17;
                -- TODO(native): CVar2._0_4_ = uVar14;
                -- TODO(native): CVar2._8_4_ = uVar21;
                quest:SetWanderCentrePoint(me, position3)
                goto LAB_00f220f0
            end
            quest:DeregisterTimer(registerTimer)
            resources:DestroyMovie(meControl)
            return
        end
        if predicateResult8 then
            quest:DeregisterTimer(registerTimer)
            resources:DestroyMovie(meControl)
            return
        end
        if quest:GetTimer(registerTimer) < 1 and scratchValue == 1 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(registerTimer)
                resources:DestroyMovie(meControl)
                return
            end
            local position4 = hero:GetPos()
            -- TODO(native): center_00._4_4_ = uVar17;
            -- TODO(native): center_00._0_4_ = uVar14;
            -- TODO(native): center_00._8_4_ = uVar21;
            quest:SetWanderCentrePoint(me, position4)
            scratchValue = 2
            goto LAB_00f220f0
        elseif scratchValue == 0 then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(registerTimer)
                resources:DestroyMovie(meControl)
                return
            end
            quest:SetTimer(registerTimer, 10)
            scratchValue = 1
        end
        goto FLOW_past_lab_00f220f0
        ::LAB_00f220f0::
        quest:SetTimer(registerTimer, 3)
        ::FLOW_past_lab_00f220f0::
        ::LAB_00f221ae::
        local scratchValue24 = CONCAT13(me:IsTalkedToByHero(),int3in_stack_fffffeb4)
        if not pCVar22_b3 then
            if scratchValue24 & 0xffffff >> 24 ~= 0 then
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(registerTimer)
                    resources:DestroyMovie(meControl)
                    return
                end
                resources:PrepareResource(meControl)
                while not resources:TryAcquire(meControl, me, 4) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(registerTimer)
                        resources:DestroyMovie(meControl)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(registerTimer)
                    resources:DestroyMovie(meControl)
                    return
                end
                scratchValue = me:IsPerformingScriptTask()
                if scratchValue then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(registerTimer)
                        resources:DestroyMovie(meControl)
                        return
                    end
                end
                if not quest:GetStateBool("InHitCutsceneAlready") then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(registerTimer)
                        resources:DestroyMovie(meControl)
                        return
                    end
                    quest:SetStateBool("InHitCutsceneAlready", true)
                    movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    thing = resources:ScriptThing(meControl)
                    local fret_07 = quest:GetHealth(thing)
                    CONCAT13(1,int3(scratchValue24 & 0xffffff))
                    if pCVar22_b3 then
                        scratchValue23 = 0
                        scratchValue = me:IsPerformingScriptTask()
                        while scratchValue do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                -- TODO(native): (**(code **)(*(int *)xStack_12c + 0x5ec))();
                                ReleaseEverything3(); return
                            end
                            scratchValue = me:IsPerformingScriptTask()
                        end
                        if quest:IsActiveThreadTerminating() then
                            -- TODO(native): (**(code **)(*(int *)xStack_12c + 0x5ec))();
                            ReleaseEverything3()
                            return
                        end
                    end
                    quest:ModifyThingHealth(me, 10000.0, false)
                    quest:EntitySetThingAsAllyOfThing(me, hero)
                    quest:EntitySetThingAsAllyOfThing(hero, me)
                    quest:SetStateBool("InHitCutsceneAlready", false)
                    -- TODO(native): (**(code **)(*(int *)xStack_12c + 0x5ec))();
                    movie = movie4
                    goto LAB_00f22bf5
                end
                goto LAB_00f22bfa
            end
            goto LAB_00f22c10
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(registerTimer)
            resources:ReleaseResource(meControl)
            return
        end
        resources:PrepareResource(meControl)
        while not resources:TryAcquire(meControl, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(registerTimer)
                resources:DestroyMovie(meControl)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(registerTimer)
            resources:DestroyMovie(meControl)
            return
        end
        if me:IsPerformingScriptTask() then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(registerTimer)
                resources:DestroyMovie(meControl)
                return
            end
        end
        movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        scratchValue = quest:GetStateInt("ArenaState")
        if scratchValue == 3 and not earlyTalk then
            if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
            quest:GetHealth(resources:ScriptThing(meControl))
            -- TODO(native): pCVar22_b3 = !(fret_0 <= (float10)0.0);
            if pCVar22_b3 then
                scratchValue23 = 0
                thing = 0
                scratchValue = me:IsPerformingScriptTask()
                while scratchValue do
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue = me:IsPerformingScriptTask()
                    else
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie2
                        ReleaseEverything4(); do return end
                        scratchValue = me:IsPerformingScriptTask()
                    end
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
            end
            earlyTalk = true
            goto FLOW_native_label_1
            ReleaseEverything2()
            return
        end
        if scratchValue == 5 and not chamTalk then
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            quest:GetHealth(resources:ScriptThing(meControl))
            -- TODO(native): pCVar22_b3 = !(fret_00 <= (float10)0.0);
            if pCVar22_b3 then
                scratchValue23 = 0
                thing = 0
                scratchValue = me:IsPerformingScriptTask()
                while scratchValue do
                    if not quest:NewScriptFrame(me) then ReleaseEverything2(); return end
                    scratchValue = me:IsPerformingScriptTask()
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            end
            chamTalk = true
            goto FLOW_native_label_1
            ReleaseEverything()
            return
        end
        if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
        switch3 = hintNumber
        if not (switch3 == 0 or switch3 == 1 or switch3 == 2 or switch3 == 3 or switch3 == 4 or switch3 == 5) then
            switch3 = 0x7ffffffe
        end
        repeat
            if switch3 == 0 then
                quest:GetHealth(resources:ScriptThing(meControl))
                -- TODO(native): pCVar22_b3 = !(fret_01 <= (float10)0.0);
                if pCVar22_b3 then
                    scratchValue23 = 0
                    thing = 0
                    scratchValue = me:IsPerformingScriptTask()
                    while scratchValue do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        scratchValue = me:IsPerformingScriptTask()
                    end
                    goto LAB_00f22807
                end
                goto FLOW_hoist_lab_00f22807_1
            end
            goto FLOW_past_lab_00f22807
            ::LAB_00f22807::
            if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
            ::FLOW_hoist_lab_00f22807_1::
            break
            ::FLOW_past_lab_00f22807::
            if switch3 == 1 then
                quest:GetHealth(resources:ScriptThing(meControl))
                -- TODO(native): pCVar22_b3 = !(fret_02 <= (float10)0.0);
                if pCVar22_b3 then
                    scratchValue23 = 0
                    thing = 0
                    scratchValue = me:IsPerformingScriptTask()
                    while scratchValue do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        scratchValue = me:IsPerformingScriptTask()
                    end
                    goto LAB_00f22807
                end
                break
            end
            if switch3 == 2 then
                quest:GetHealth(resources:ScriptThing(meControl))
                -- TODO(native): pCVar22_b3 = !(fret_03 <= (float10)0.0);
                if pCVar22_b3 then
                    scratchValue23 = 0
                    thing = 0
                    scratchValue = me:IsPerformingScriptTask()
                    while scratchValue do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        scratchValue = me:IsPerformingScriptTask()
                    end
                    goto LAB_00f22807
                end
                break
            end
            if switch3 == 3 then
                quest:GetHealth(resources:ScriptThing(meControl))
                -- TODO(native): pCVar22_b3 = !(fret_04 <= (float10)0.0);
                if pCVar22_b3 then
                    scratchValue23 = 0
                    thing = 0
                    scratchValue = me:IsPerformingScriptTask()
                    while scratchValue do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        scratchValue = me:IsPerformingScriptTask()
                    end
                    goto LAB_00f22807
                end
                break
            end
            if switch3 == 4 then
                quest:GetHealth(resources:ScriptThing(meControl))
                -- TODO(native): pCVar22_b3 = !(fret_05 <= (float10)0.0);
                if pCVar22_b3 then
                    scratchValue23 = 0
                    thing = 0
                    scratchValue = me:IsPerformingScriptTask()
                    while scratchValue do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        scratchValue = me:IsPerformingScriptTask()
                    end
                    goto LAB_00f22807
                end
                break
            end
            if switch3 == 5 then
                quest:GetHealth(resources:ScriptThing(meControl))
                -- TODO(native): pCVar22_b3 = !(fret_06 <= (float10)0.0);
                if pCVar22_b3 then
                    scratchValue23 = 0
                    thing = 0
                    scratchValue = me:IsPerformingScriptTask()
                    while scratchValue do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        scratchValue = me:IsPerformingScriptTask()
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything2(); return end
                end
                hintNumber = 4
                switch3 = 0x7ffffffe
            end
            if switch3 == 0x7ffffffe then goto FLOW_native_label_1 end
        until true
        hintNumber = hintNumber + 1
        ::FLOW_native_label_1::
        quest:PauseAllNonScriptedEntities(switch3 ~= 0)
        movie = movie2
        ::LAB_00f22bf5::
        resources:ReleaseResource(movie)
        ::LAB_00f22bfa::
        resources:PrepareResource(meControl)
        ::LAB_00f22c10::
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
    until false
end

-- Roth.Init (retail 0x00f14610)
function Init(quest, me)
    earlyTalk = false
    chamTalk = false
    hintNumber = 0
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

-- Roth.OnPersist (retail 0x00f21660)
function OnPersist(quest, me, context)
    quest:SetStateBool("EarlyTalk", quest:PersistTransferBool(context, "EarlyTalk", quest:GetStateBool("EarlyTalk")))
    quest:SetStateBool("ChamTalk", quest:PersistTransferBool(context, "ChamTalk", quest:GetStateBool("ChamTalk")))
    quest:SetStateInt("HintNumber", quest:PersistTransferInt(context, "HintNumber", quest:GetStateInt("HintNumber") or 0))
end

-- Roth.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

