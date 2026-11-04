-- Readable native conversion: Needle. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local earlyTalk_, chamTalk_, hintNumber_

-- Needle.Main (retail 0x00f15660)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, scratchValue2, center, switch, resource, this_00, movie, movie2
    local resource2
    local function ReleaseControl()
        resources:ReleaseResource(resource2)
    end
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        local this_00 = movie2
        resources:DestroyMovie(this_00)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything2()
        local this_00 = movie
        resources:DestroyMovie(this_00)
        resources:ReleaseResource(resource)
    end
    local function ReleaseEverything3()
        resources:DestroyMovie(this_00)
        resources:ReleaseResource(resource)
    end
    resource2 = resources:NewResource()
    resource = resource2
    quest:EntityUnsetAsOpinionSource(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    while quest:GetStateInt("ArenaState") == 2 do
        if not quest:NewScriptFrame(me) then ReleaseControl(); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    quest:SetCreatureBrain(me, "BRAIN_ARENA_CELLS")
    local position = quest:GetThingWithScriptName("NeedlePoint"):GetPos()
    center = position.x
    quest:SetWanderCentrePoint(me, position)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 5.0)
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            ReleaseControl()
            return
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource2)
            while not resources:TryAcquire(resource2, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if resources:IsPerformingScriptTask(resource2) then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                resources:ClearCommands(resource2)
            end
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local arenaState = quest:GetStateInt("ArenaState")
            if arenaState == 3 and not earlyTalk_ then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                local scratchValue = resources:ScriptThing(resource2)
                if 0.0 < quest:GetHealth(scratchValue) then
                    local hero = quest:GetHero()
                    resources:Speak(resource2, hero, "TEXT_QST_005_V2_ARENA_NEEDLE_FIRST_CHAT_PRE_CHAM", GROUP_SELECT_FIRST, false, true, false)
                    while resources:IsPerformingScriptTask(resource2) do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie2
                            ReleaseEverything3(); do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
                earlyTalk_ = true
                goto FLOW_native_label_1
                ReleaseEverything()
                return
            end
            if arenaState == 5 and not chamTalk_ then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                local scratchValue11 = resources:ScriptThing(resource2)
                if 0.0 < quest:GetHealth(scratchValue11) then
                    local hero2 = quest:GetHero()
                    resources:Speak(resource2, hero2, "TEXT_QST_005_V2_ARENA_NEEDLE_FIRST_CHAT_POST_CHAM", GROUP_SELECT_FIRST, false, true, false)
                    while resources:IsPerformingScriptTask(resource2) do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
                chamTalk_ = true
                goto FLOW_native_label_1
                ReleaseEverything()
                return
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            switch = hintNumber_
            if not (switch == 0 or switch == 1 or switch == 2 or switch == 3) then
                switch = 0x7ffffffe
            end
            repeat
                if switch == 0 then
                    local scratchValue12 = resources:ScriptThing(resource2)
                    if 0.0 < quest:GetHealth(scratchValue12) then
                        local hero3 = quest:GetHero()
                        resources:Speak(resource2, hero3, "TEXT_QST_005_V2_ARENA_NEEDLE_FIRST_HINT", GROUP_SELECT_FIRST, false, true, false)
                        while resources:IsPerformingScriptTask(resource2) do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        goto LAB_00f15cec
                    end
                    goto FLOW_hoist_lab_00f15cec_1
                end
                goto FLOW_past_lab_00f15cec
                ::LAB_00f15cec::
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                ::FLOW_hoist_lab_00f15cec_1::
                break
                ::FLOW_past_lab_00f15cec::
                if switch == 1 then
                    local scratchValue13 = resources:ScriptThing(resource2)
                    if 0.0 < quest:GetHealth(scratchValue13) then
                        local hero4 = quest:GetHero()
                        resources:Speak(resource2, hero4, "TEXT_QST_005_V2_ARENA_NEEDLE_SECOND_HINT", GROUP_SELECT_FIRST, false, true, false)
                        while resources:IsPerformingScriptTask(resource2) do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        goto LAB_00f15cec
                    end
                    break
                end
                if switch == 2 then
                    local scratchValue14 = resources:ScriptThing(resource2)
                    if 0.0 < quest:GetHealth(scratchValue14) then
                        local hero5 = quest:GetHero()
                        resources:Speak(resource2, hero5, "TEXT_QST_005_V2_ARENA_NEEDLE_THIRD_HINT", GROUP_SELECT_FIRST, false, true, false)
                        while resources:IsPerformingScriptTask(resource2) do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        goto LAB_00f15cec
                    end
                    break
                end
                if switch == 3 then
                    local scratchValue15 = resources:ScriptThing(resource2)
                    if 0.0 < quest:GetHealth(scratchValue15) then
                        local hero6 = quest:GetHero()
                        resources:Speak(resource2, hero6, "TEXT_QST_005_V2_ARENA_NEEDLE_FOURTH_HINT", GROUP_SELECT_FIRST, false, true, false)
                        while resources:IsPerformingScriptTask(resource2) do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    end
                    hintNumber_ = 2
                    switch = 0x7ffffffe
                end
                if switch == 0x7ffffffe then goto FLOW_native_label_1 end
            until true
            hintNumber_ = hintNumber_ + 1
            ::FLOW_native_label_1::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        else
            if me:MsgIsHitByHero() then
                goto LAB_00f15e52
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f15e52 end
                scratchValue2 = 0
            end
            goto FLOW_past_lab_00f15e52
            ::LAB_00f15e52::
            scratchValue2 = 1
            ::FLOW_past_lab_00f15e52::
            if scratchValue2 == 0 then goto LAB_00f160cb end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            resources:PrepareResource(resource2)
            while not resources:TryAcquire(resource2, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            if resources:IsPerformingScriptTask(resource2) then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                resources:ClearCommands(resource2)
            end
            if not quest:GetStateBool("InHitCutsceneAlready") then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                quest:SetStateBool("InHitCutsceneAlready", true)
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue16 = resources:ScriptThing(resource2)
                if 0.0 < quest:GetHealth(scratchValue16) then
                    local hero7 = quest:GetHero()
                    resources:Speak(resource2, hero7, "TEXT_QST_005_V2_ARENA_NEEDLE_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
                    while resources:IsPerformingScriptTask(resource2) do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            ReleaseEverything2(); do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        ReleaseEverything2()
                        return
                    end
                end
                quest:ModifyThingHealth(me, 10000.0, false)
                local hero8 = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(me, hero8)
                local pThing1 = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(pThing1, me)
                quest:SetStateBool("InHitCutsceneAlready", false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                goto LAB_00f160b5
                resources:ReleaseResource(resource)
                return
            end
        end
        ::LAB_00f160b5::
        resources:PrepareResource(resource2)
        ::LAB_00f160cb::
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
    until false
end

-- Needle.Init (retail 0x00f15600)
function Init(quest, me)
    earlyTalk_ = false
    chamTalk_ = false
    hintNumber_ = 0
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

-- Needle.OnPersist (retail 0x00f21720)
function OnPersist(quest, me, context)
    local earlyTalk = earlyTalk_ or false
    earlyTalk_ = quest:PersistTransferBool(context, "EarlyTalk", earlyTalk)
    local chamTalk = chamTalk_ or false
    chamTalk_ = quest:PersistTransferBool(context, "ChamTalk", chamTalk)
    local hintNumber = hintNumber_ or 0
    hintNumber_ = quest:PersistTransferInt(context, "HintNumber", hintNumber)
end

-- Needle.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

