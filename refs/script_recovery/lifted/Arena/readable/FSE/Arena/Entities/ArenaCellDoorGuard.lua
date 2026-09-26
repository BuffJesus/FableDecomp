-- Readable native conversion: ArenaCellDoorGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local chamTalk, earlyTalk

-- ArenaCellDoorGuard.Main (retail 0x00f17c70)
function Main(quest, me)
    local cellsVillage = quest:GetStateThing("CellsVillage")
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult11, predicateResult14, scratchValue, scratchValue32, scratchValue33
    local this_01, scratchValue34, scratchValue35, scratchValue36, resource, movie2, resource3
    local meControl
    scratchValue36 = 0
    resources:NewResource()
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    resources:PrepareResource(meControl)
    while not resources:TryAcquire(meControl, me, 4) do
        if not quest:NewScriptFrame(me) then return end
    end
    local predicateResult = quest:IsActiveThreadTerminating()
    if not predicateResult then
        scratchValue = predicateResult
        quest:SetThingHasInformation(me, false, true, false)
        repeat
            if quest:IsActiveThreadTerminating() then return end
            if me:IsTalkedToByHero() then
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                scratchValue32 = quest:GetStateInt("ArenaState")
                if scratchValue32 ~= 3 then
                    if scratchValue32 ~= 5 or chamTalk then
                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                        scratchValue33 = resources:ScriptThing(meControl)
                        if 0.0 < quest:GetHealth(scratchValue33) then
                            scratchValue33 = hero
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00f19a17 end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                this_01 = movie2
                                goto LAB_00f19a72
                            end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_ENTER_ARENA_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        scratchValue32 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue32 < 0 do
                            if not quest:NewScriptFrame(me) then goto LAB_00f19a3c end
                            scratchValue32 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                        if scratchValue32 ~= 1 then
                            scratchValue33 = resources:ScriptThing(meControl)
                            if 0.0 < quest:GetHealth(scratchValue33) then
                                scratchValue33 = hero
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00f19a3c end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                            end
                            goto LAB_00f194b7
                            goto LAB_00f19a17
                        end
                        quest:SetStateBool("NeedBertForSpeech", true)
                        local arenaCellDoorGuard = quest:GetThingWithScriptName("ArenaCellDoorGuard2")
                        resource3 = resources:NewResource()
                        resources:PrepareResource(resource3)
                        while not resources:TryAcquire(resource3, arenaCellDoorGuard, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            local switch1 = quest:GetStateInt("ArenaRound")
                            repeat
                                if switch1 == 0 then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    scratchValue33 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue33) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    goto FLOW_hoist_lab_00f19379_1
                                elseif switch1 == 1 then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    scratchValue33 = resources:ScriptThing(meControl)
                                    if 0.0 < quest:GetHealth(scratchValue33) then
                                        scratchValue33 = hero
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    break
                                elseif switch1 == 2 then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    scratchValue33 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue33) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    break
                                elseif switch1 == 3 then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    scratchValue33 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue33) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    break
                                elseif switch1 == 4 then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    scratchValue33 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue33) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    break
                                elseif switch1 == 5 then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    scratchValue33 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue33) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    break
                                elseif switch1 == 6 then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    scratchValue33 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue33) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    break
                                elseif switch1 == 7 then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource3)) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    scratchValue33 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue33) then
                                        scratchValue33 = hero
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                end
                                goto FLOW_past_lab_00f19379
                                ::LAB_00f19379::
                                if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                ::FLOW_hoist_lab_00f19379_1::
                                break
                                ::FLOW_past_lab_00f19379::
                            until true
                            quest:FadeScreenOut(0.5, 0.5)
                            quest:Pause(0.5)
                            quest:SetStateInt("ArenaState", 6)
                            quest:ClearHeroEnemyOfGuards(cellsVillage)
                            resources:PrepareResource(resource3)
                            quest:SetStateBool("NeedBertForSpeech", false)
                            resources:ReleaseResource(resource3)
                            goto LAB_00f194b7
                        end
                        ::LAB_00f19a2a::
                        resources:ReleaseResource(resource3)
                        goto LAB_00f19a37
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        this_01 = movie2
                        goto LAB_00f19a72
                    end
                    if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00f19a17 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_ENTER_ARENA_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    scratchValue32 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while scratchValue32 < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00f19a17 end
                        scratchValue32 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        local predicateResult8 = quest:IsActiveThreadTerminating()
                        if scratchValue32 == 1 then
                            if predicateResult8 then goto LAB_00f19a17 end
                            quest:SetStateBool("NeedBertForSpeech", true)
                            local arenaCellDoorGuard22 = quest:GetThingWithScriptName("ArenaCellDoorGuard2")
                            resource = resources:NewResource()
                            resources:PrepareResource(resource)
                            while not resources:TryAcquire(resource, arenaCellDoorGuard22, 4) do
                                if not quest:NewScriptFrame(me) then goto LAB_00f199ae end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00f199b9 end
                            goto FLOW_past_lab_00f199b9
                            ::LAB_00f199b9::
                            resources:ReleaseResource(resource)
                            goto LAB_00f19a17
                            ::FLOW_past_lab_00f199b9::
                            if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00f199ae end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f199b9 end
                            end
                            scratchValue33 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue33) then
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00f199ae end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f199b9 end
                            end
                            quest:FadeScreenOut(0.5, 0.5)
                            quest:Pause(0.5)
                            quest:SetStateInt("ArenaState", 6)
                            quest:ClearHeroEnemyOfGuards(cellsVillage)
                            resources:PrepareResource(resource)
                            quest:SetStateBool("NeedBertForSpeech", false)
                            resources:ReleaseResource(resource)
                            chamTalk = true
                            goto LAB_00f194b7
                        end
                        if not predicateResult8 then
                            scratchValue33 = resources:ScriptThing(meControl)
                            if 0.0 < quest:GetHealth(scratchValue33) then
                                scratchValue33 = hero
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00f19a17 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f19a3c end
                            end
                            chamTalk = true
                            goto LAB_00f194b7
                        end
                    end
                    goto LAB_00f19a3c
                end
                if not quest:IsActiveThreadTerminating() then
                    if not earlyTalk then
                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                        scratchValue33 = resources:ScriptThing(meControl)
                        if 0.0 < quest:GetHealth(scratchValue33) then
                            scratchValue33 = hero
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00f19a17 end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                        end
                        earlyTalk = true
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                        scratchValue33 = resources:ScriptThing(meControl)
                        if 0.0 < quest:GetHealth(scratchValue33) then
                            scratchValue33 = hero
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00f19a3c end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                this_01 = movie2
                                goto LAB_00f19a72
                            end
                        end
                    end
                    goto LAB_00f194b7
                end
                goto FLOW_past_lab_00f194b7
                ::LAB_00f194b7::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                goto LAB_00f19935
                ::FLOW_past_lab_00f194b7::
                quest:PauseAllNonScriptedEntities(false)
                this_01 = movie2
                goto LAB_00f19a72
            end
            scratchValue34 = scratchValue36
            scratchValue36 = scratchValue36 | 1
            if scratchValue33:MsgIsHitByHero() then
                goto LAB_00f19571
            else
                scratchValue35 = scratchValue34 | 3
                if scratchValue33:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue35 = scratchValue34 | 7
                    if not scratchValue33:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f19571 end
                end
                predicateResult11 = false
            end
            goto FLOW_past_lab_00f19571
            ::LAB_00f19571::
            predicateResult11 = true
            ::FLOW_past_lab_00f19571::
            if scratchValue35 & 4 ~= 0 then
                scratchValue35 = scratchValue35 & 0xfffffffb
            end
            if scratchValue35 & 2 ~= 0 then
                scratchValue35 = scratchValue35 & 0xfffffffd
            end
            if scratchValue35 & 1 ~= 0 then
                scratchValue35 = scratchValue35 & 0xfffffffe
            end
            if predicateResult11 then
                if not quest:GetStateBool("InHitCutsceneAlready") then
                    quest:SetStateBool("InHitCutsceneAlready", true)
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    scratchValue33 = resources:ScriptThing(meControl)
                    if 0.0 < quest:GetHealth(scratchValue33) then
                        scratchValue33 = hero
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00f19a6b end
                        end
                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00f19a6b end
                        goto FLOW_past_lab_00f19a6b
                        ::LAB_00f19a6b::
                        this_01 = movie
                        goto LAB_00f19a72
                        ::FLOW_past_lab_00f19a6b::
                    end
                    quest:ModifyThingHealth(me, 10000.0, false)
                    quest:EntitySetThingAsAllyOfThing(me, hero)
                    quest:EntitySetThingAsAllyOfThing(hero, me)
                    quest:SetStateBool("InHitCutsceneAlready", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                end
            else
                scratchValue35 = scratchValue35 | 24
                if not quest:IsDistanceBetweenThingsOver(me, quest:GetThingWithScriptName("GuardingDoorMarkerRight"), 1.0) then goto LAB_00f197e9 end
                predicateResult14 = true
                if me:IsPerformingScriptTask() then goto LAB_00f197e9 end
                goto FLOW_past_lab_00f197e9
                ::LAB_00f197e9::
                predicateResult14 = false
                ::FLOW_past_lab_00f197e9::
                if scratchValue35 & 16 ~= 0 then
                    scratchValue35 = scratchValue35 & 0xffffffef
                end
                if scratchValue35 & 8 ~= 0 then
                    scratchValue36 = scratchValue35 & 0xfffffff7
                end
                if predicateResult14 then
                    scratchValue33 = quest:GetThingWithScriptName("GuardingDoorMarkerRight")
                    me:MoveToPosition(scratchValue33:GetPos(), 0, ENTITY_MOVE_WALK, false, true)
                    scratchValue = 0
                elseif scratchValue == 0 then
                    if not me:IsPerformingScriptTask() then
                        scratchValue = 1
                        local guardingDoorMarkerRight = quest:GetThingWithScriptName("GuardingDoorMarkerRight")
                        quest:EntitySetFacingAngle(me, guardingDoorMarkerRight:GetAngleXY(), false)
                    end
                end
            end
            ::LAB_00f19935::
            quest:NewScriptFrame(me)
        until false
    end
    goto LAB_00f19a7b
    ::LAB_00f19a09::
    resources:ReleaseResource(resource3)
    ::LAB_00f19a17::
    quest:PauseAllNonScriptedEntities(false)
    this_01 = movie2
    goto LAB_00f19a72
    ::LAB_00f199ae::
    resources:ReleaseResource(resource)
    ::LAB_00f19a37::
    ::LAB_00f19a3c::
    quest:PauseAllNonScriptedEntities(false)
    this_01 = movie2
    ::LAB_00f19a72::
    resources:DestroyMovie(this_01)
    ::LAB_00f19a7b::
    resources:ReleaseResource(meControl)
end

-- ArenaCellDoorGuard.Init (retail 0x00f17c20)
function Init(quest, me)
    earlyTalk = false
    chamTalk = false
    quest:EntitySetAllStategroupsEnabled(me, false)
    quest:EntitySetCombatEnabled(me, false)
end

-- ArenaCellDoorGuard.OnPersist (retail 0x00f217e0)
function OnPersist(quest, me, context)
    quest:SetStateBool("EarlyTalk", quest:PersistTransferBool(context, "EarlyTalk", quest:GetStateBool("EarlyTalk")))
    quest:SetStateBool("ChamTalk", quest:PersistTransferBool(context, "ChamTalk", quest:GetStateBool("ChamTalk")))
end

-- ArenaCellDoorGuard.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

