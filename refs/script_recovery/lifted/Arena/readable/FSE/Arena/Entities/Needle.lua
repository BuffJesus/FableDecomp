-- Readable native conversion: Needle. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local earlyTalk, chamTalk, hintNumber

-- Needle.Main (retail 0x00f15660)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue2, switch, this_00, scratchValue, scratchValue20, movie, movie2, meControl
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        local this_00 = movie2
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(meControl)
    end
    local function ReleaseEverything2()
        local this_00 = movie
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(meControl)
    end
    local function ReleaseEverything3()
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(meControl)
    end
    scratchValue20 = 0
    resources:NewResource()
    quest:EntityUnsetAsOpinionSource(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    while quest:GetStateInt("ArenaState") == 2 do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then resources:DestroyMovie(meControl); return end
    quest:SetCreatureBrain(me, "BRAIN_ARENA_CELLS")
    quest:SetWanderCentrePoint(me, quest:GetThingWithScriptName("NeedlePoint"):GetPos())
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 5.0)
    repeat
        if quest:IsActiveThreadTerminating() then return end
        if me:IsTalkedToByHero() then
            resources:PrepareResource(meControl)
            while not resources:TryAcquire(meControl, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:DestroyMovie(meControl)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:DestroyMovie(meControl); return end
            movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local arenaState = quest:GetStateInt("ArenaState")
            if arenaState == 3 and not earlyTalk then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie2
                            ReleaseEverything3(); do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
                earlyTalk = true
                goto FLOW_native_label_1
                ReleaseEverything()
                return
            end
            if arenaState == 5 and not chamTalk then
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                    end
                    if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                end
                chamTalk = true
                goto FLOW_native_label_1
                ReleaseEverything()
                return
            end
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            switch = hintNumber
            if not (switch == 0 or switch == 1 or switch == 2 or switch == 3) then
                switch = 0x7ffffffe
            end
            repeat
                if switch == 0 then
                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                        while me:IsPerformingScriptTask() do
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
                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        goto LAB_00f15cec
                    end
                    break
                end
                if switch == 2 then
                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        goto LAB_00f15cec
                    end
                    break
                end
                if switch == 3 then
                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                        end
                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                    end
                    hintNumber = 2
                    switch = 0x7ffffffe
                end
                if switch == 0x7ffffffe then goto FLOW_native_label_1 end
            until true
            hintNumber = hintNumber + 1
            ::FLOW_native_label_1::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        else
            local scratchValue19 = scratchValue20
            scratchValue20 = scratchValue20 | 1
            if me:MsgIsHitByHero() then
                goto LAB_00f15e52
            else
                scratchValue = scratchValue19 | 3
                scratchValue20 = scratchValue
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue = scratchValue19 | 7
                    scratchValue20 = scratchValue
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f15e52 end
                end
                scratchValue2 = 0
            end
            goto FLOW_past_lab_00f15e52
            ::LAB_00f15e52::
            scratchValue2 = 1
            ::FLOW_past_lab_00f15e52::
            if scratchValue & 4 ~= 0 then
                scratchValue = scratchValue & 0xfffffffb
                scratchValue20 = scratchValue
            end
            if scratchValue & 2 ~= 0 then
                scratchValue = scratchValue & 0xfffffffd
                scratchValue20 = scratchValue
            end
            if scratchValue & 1 ~= 0 then
                scratchValue20 = scratchValue & 0xfffffffe
            end
            if scratchValue2 == 0 then goto LAB_00f160cb end
            resources:PrepareResource(meControl)
            while not resources:TryAcquire(meControl, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    resources:DestroyMovie(meControl)
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then resources:DestroyMovie(meControl); return end
            if not quest:GetStateBool("InHitCutsceneAlready") then
                quest:SetStateBool("InHitCutsceneAlready", true)
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                    while me:IsPerformingScriptTask() do
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
                quest:EntitySetThingAsAllyOfThing(me, hero)
                quest:EntitySetThingAsAllyOfThing(hero, me)
                quest:SetStateBool("InHitCutsceneAlready", false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                goto LAB_00f160b5
                resources:DestroyMovie(meControl)
                return
            end
        end
        ::LAB_00f160b5::
        resources:PrepareResource(meControl)
        ::LAB_00f160cb::
        quest:NewScriptFrame(me)
    until false
end

-- Needle.Init (retail 0x00f15600)
function Init(quest, me)
    earlyTalk = false
    chamTalk = false
    hintNumber = 0
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

-- Needle.OnPersist (retail 0x00f21720)
function OnPersist(quest, me, context)
    quest:SetStateBool("EarlyTalk", quest:PersistTransferBool(context, "EarlyTalk", quest:GetStateBool("EarlyTalk")))
    quest:SetStateBool("ChamTalk", quest:PersistTransferBool(context, "ChamTalk", quest:GetStateBool("ChamTalk")))
    quest:SetStateInt("HintNumber", quest:PersistTransferInt(context, "HintNumber", quest:GetStateInt("HintNumber") or 0))
end

-- Needle.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

