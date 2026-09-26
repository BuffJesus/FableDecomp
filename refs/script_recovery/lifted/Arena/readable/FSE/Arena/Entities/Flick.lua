-- Readable native conversion: Flick. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local earlyTalk, chamTalk, hintNumber_

-- Flick.Main (retail 0x00f14a90)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue2, this_00, scratchValue, scratchValue12, movie, movie2, meControl
    local function ReleaseEverything()
        local this_00 = movie2
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(meControl)
    end
    local function ReleaseEverything2()
        local this_00 = movie
        resources:DestroyMovie(this_00)
        resources:DestroyMovie(meControl)
    end
    scratchValue12 = 0
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
    quest:SetWanderCentrePoint(me, quest:GetThingWithScriptName("FlickPoint"):GetPos())
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 5.0)
    quest:SetScriptingStateGroup(me, 4)
    if quest:IsActiveThreadTerminating() then return end
    ::LAB_00f14ca0::
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
        -- TODO(native): CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities((CWideScreenMagicPauseEntities *)&xStack_90,*(int *)(this + 4));
        local arenaState = quest:GetStateInt("ArenaState")
        if arenaState == 3 and not earlyTalk then
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_a0);
            local fret_0 = quest:GetHealth(nil --[[missing]])
            if 0.0 < fret_0 then
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            end
            earlyTalk = true
        elseif arenaState == 5 and not chamTalk then
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_a0);
            local fret_00 = quest:GetHealth(nil --[[missing]])
            if 0.0 < fret_00 then
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                end
                if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            end
            chamTalk = true
        else
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            local hintNumber = hintNumber_
            if hintNumber == 0 then
                -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_a0);
                local fret_03 = quest:GetHealth(nil --[[missing]])
                if 0.0 < fret_03 then
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                    end
                    goto LAB_00f1515c
                end
            else
                if not hintNumber then
                    if hintNumber == 2 then
                        -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_a0);
                        local fret_01 = quest:GetHealth(nil --[[missing]])
                        if 0.0 < fret_01 then
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                            end
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
                        end
                        hintNumber_ = 1
                    end
                    goto LAB_00f1516e
                end
                -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_a0);
                local fret_02 = quest:GetHealth(nil --[[missing]])
                if 0.0 < fret_02 then
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then ReleaseEverything(); return end
                    end
                    goto LAB_00f1515c
                end
            end
            goto FLOW_past_lab_00f1515c
            ::LAB_00f1515c::
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            ::FLOW_past_lab_00f1515c::
            hintNumber_ = hintNumber_ + 1
        end
        ::LAB_00f1516e::
        -- TODO(native): (**(code **)(*xStack_90 + 0x5ec))(0);
        resources:DestroyMovie(movie2)
    else
        local scratchValue11 = scratchValue12
        scratchValue12 = scratchValue12 | 1
        if me:MsgIsHitByHero() then
            goto LAB_00f15214
        else
            scratchValue = scratchValue11 | 3
            scratchValue12 = scratchValue
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue = scratchValue11 | 7
                scratchValue12 = scratchValue
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f15214 end
            end
            scratchValue2 = 0
        end
        goto FLOW_past_lab_00f15214
        ::LAB_00f15214::
        scratchValue2 = 1
        ::FLOW_past_lab_00f15214::
        if scratchValue & 4 ~= 0 then
            scratchValue = scratchValue & 0xfffffffb
            scratchValue12 = scratchValue
        end
        if scratchValue & 2 ~= 0 then
            scratchValue = scratchValue & 0xfffffffd
            scratchValue12 = scratchValue
        end
        if scratchValue & 1 ~= 0 then
            scratchValue12 = scratchValue & 0xfffffffe
        end
        if scratchValue2 == 0 then goto LAB_00f1548b end
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
            -- TODO(native): CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities((CWideScreenMagicPauseEntities *)&xStack_8c,*(int *)(this + 4));
            -- TODO(native): pCVar7 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_a0);
            local fret_04 = quest:GetHealth(nil --[[missing]])
            if 0.0 < fret_04 then
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): (**(code **)(*(int *)xStack_8c + 0x5ec))(0);
                        ReleaseEverything2(); return
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    -- TODO(native): (**(code **)(*(int *)xStack_8c + 0x5ec))(0);
                    ReleaseEverything2()
                    return
                end
            end
            quest:ModifyThingHealth(me, 10000.0, false)
            quest:EntitySetThingAsAllyOfThing(me, hero)
            quest:EntitySetThingAsAllyOfThing(hero, me)
            quest:SetStateBool("InHitCutsceneAlready", false)
            -- TODO(native): (**(code **)(*(int *)xStack_8c + 0x5ec))(0);
            resources:DestroyMovie(movie)
        end
    end
    resources:PrepareResource(meControl)
    ::LAB_00f1548b::
    if not quest:NewScriptFrame(me) then return end
    goto LAB_00f14ca0
end

-- Flick.Init (retail 0x00f14a30)
function Init(quest, me)
    earlyTalk = false
    chamTalk = false
    hintNumber_ = 0
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
end

-- Flick.OnPersist (retail 0x00f216c0)
function OnPersist(quest, me, context)
    quest:SetStateBool("EarlyTalk", quest:PersistTransferBool(context, "EarlyTalk", quest:GetStateBool("EarlyTalk")))
    quest:SetStateBool("ChamTalk", quest:PersistTransferBool(context, "ChamTalk", quest:GetStateBool("ChamTalk")))
    quest:SetStateInt("HintNumber", quest:PersistTransferInt(context, "HintNumber", quest:GetStateInt("HintNumber") or 0))
end

-- Flick.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

