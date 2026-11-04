-- Readable native conversion: Flick. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local earlyTalk_, chamTalk_, hintNumber_

-- Flick.Main (retail 0x00f14a90)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue2, center, resource, this_00, movie, movie2
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
    local resource2 = resources:NewResource()
    resource = resource2
    quest:EntityUnsetAsOpinionSource(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    while quest:GetStateInt("ArenaState") == 2 do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    quest:SetCreatureBrain(me, "BRAIN_ARENA_CELLS")
    local position = quest:GetThingWithScriptName("FlickPoint"):GetPos()
    center = position.x
    quest:SetWanderCentrePoint(me, position)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 5.0)
    quest:SetScriptingStateGroup(me, 4)
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    ::LAB_00f14ca0::
    if me:IsTalkedToByHero() then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
        resources:PrepareResource(resource2)
        while not resources:TryAcquire(resource2, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource2)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
        if resources:IsPerformingScriptTask(resource2) then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
            resources:ClearCommands(resource2)
        end
        movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        local arenaState = quest:GetStateInt("ArenaState")
        if arenaState == 3 and not earlyTalk_ then
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            local scratchValue9 = resources:ScriptThing(resource2)
            if 0.0 < quest:GetHealth(scratchValue9) then
                local hero = quest:GetHero()
                resources:Speak(resource2, hero, "TEXT_QST_005_V2_ARENA_FLICK_FIRST_CHAT_PRE_CHAM", GROUP_SELECT_FIRST, false, true, false)
                while resources:IsPerformingScriptTask(resource2) do
                    if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            end
            earlyTalk_ = true
        elseif arenaState == 5 and not chamTalk_ then
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            local scratchValue = resources:ScriptThing(resource2)
            if 0.0 < quest:GetHealth(scratchValue) then
                local hero2 = quest:GetHero()
                resources:Speak(resource2, hero2, "TEXT_QST_005_V2_ARENA_FLICK_FIRST_CHAT_POST_CHAM", GROUP_SELECT_FIRST, false, true, false)
                while resources:IsPerformingScriptTask(resource2) do
                    if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            end
            chamTalk_ = true
        else
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            local hintNumber = hintNumber_
            if hintNumber == 0 then
                local scratchValue11 = resources:ScriptThing(resource2)
                if 0.0 < quest:GetHealth(scratchValue11) then
                    local hero3 = quest:GetHero()
                    resources:Speak(resource2, hero3, "TEXT_QST_005_V2_ARENA_FLICK_FIRST_HINT", GROUP_SELECT_FIRST, false, true, false)
                    while resources:IsPerformingScriptTask(resource2) do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                    end
                    goto LAB_00f1515c
                end
            else
                if hintNumber ~= 1 then
                    if hintNumber == 2 then
                        local scratchValue12 = resources:ScriptThing(resource2)
                        if 0.0 < quest:GetHealth(scratchValue12) then
                            local hero4 = quest:GetHero()
                            resources:Speak(resource2, hero4, "TEXT_QST_005_V2_ARENA_FLICK_THIRD_HINT", GROUP_SELECT_FIRST, false, true, false)
                            while resources:IsPerformingScriptTask(resource2) do
                                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                            end
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                        end
                        hintNumber_ = 1
                    end
                    goto LAB_00f1516e
                end
                local scratchValue13 = resources:ScriptThing(resource2)
                if 0.0 < quest:GetHealth(scratchValue13) then
                    local hero5 = quest:GetHero()
                    resources:Speak(resource2, hero5, "TEXT_QST_005_V2_ARENA_FLICK_SECOND_HINT", GROUP_SELECT_FIRST, false, true, false)
                    while resources:IsPerformingScriptTask(resource2) do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                    end
                    goto LAB_00f1515c
                end
            end
            goto FLOW_past_lab_00f1515c
            ::LAB_00f1515c::
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            ::FLOW_past_lab_00f1515c::
            hintNumber_ = (hintNumber_ + 1 + 2147483648) % 4294967296 - 2147483648
        end
        ::LAB_00f1516e::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
    else
        if me:MsgIsHitByHero() then
            goto LAB_00f15214
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f15214 end
            scratchValue2 = 0
        end
        goto FLOW_past_lab_00f15214
        ::LAB_00f15214::
        scratchValue2 = 1
        ::FLOW_past_lab_00f15214::
        if scratchValue2 == 0 then goto LAB_00f1548b end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
        resources:PrepareResource(resource2)
        while not resources:TryAcquire(resource2, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource2)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
        if resources:IsPerformingScriptTask(resource2) then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
            resources:ClearCommands(resource2)
        end
        if not quest:GetStateBool("InHitCutsceneAlready") then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
            quest:SetStateBool("InHitCutsceneAlready", true)
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local scratchValue14 = resources:ScriptThing(resource2)
            if 0.0 < quest:GetHealth(scratchValue14) then
                local hero6 = quest:GetHero()
                resources:Speak(resource2, hero6, "TEXT_QST_005_V2_ARENA_FLICK_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
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
            local hero7 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, hero7)
            local pThing1 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(pThing1, me)
            quest:SetStateBool("InHitCutsceneAlready", false)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
    end
    resources:PrepareResource(resource2)
    ::LAB_00f1548b::
    if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource2); return end
    goto LAB_00f14ca0
end

-- Flick.Init (retail 0x00f14a30)
function Init(quest, me)
    earlyTalk_ = false
    chamTalk_ = false
    hintNumber_ = 0
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

-- Flick.OnPersist (retail 0x00f216c0)
function OnPersist(quest, me, context)
    local earlyTalk = earlyTalk_ or false
    earlyTalk_ = quest:PersistTransferBool(context, "EarlyTalk", earlyTalk)
    local chamTalk = chamTalk_ or false
    chamTalk_ = quest:PersistTransferBool(context, "ChamTalk", chamTalk)
    local hintNumber = hintNumber_ or 0
    hintNumber_ = quest:PersistTransferInt(context, "HintNumber", hintNumber)
end

-- Flick.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

