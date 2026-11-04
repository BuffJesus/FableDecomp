-- Readable native conversion: ArenaCellDoorGuard2. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local earlyTalk_, chamTalk_

-- ArenaCellDoorGuard2.Main (retail 0x00f19bb0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, hb_stk_ffffff34_2, getNearestWithDefName, scratchValue4
    local resource = resources:NewResource()
    getNearestWithDefName = quest:GetNearestWithDefName(me, "VILLAGE_ARENA_CELLS")
    quest:SetStateThing("CellsVillage", getNearestWithDefName)
    getNearestWithDefName = nil
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    while quest:GetStateInt("ArenaState") == 2 do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    predicateResult = quest:IsActiveThreadTerminating()
    if predicateResult or quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    repeat
        if quest:GetStateInt("ArenaState") == 4 then
            if quest:IsActiveThreadTerminating() then break end
            quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
            resources:ClearCommands(resource)
            resources:PrepareResource(resource)
            while quest:GetStateInt("ArenaState") == 4 do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then break end
            resources:ClearCommands(resource)
            local guardingDoorMarkerLeft = quest:GetThingWithScriptName("GuardingDoorMarkerLeft")
            local position = guardingDoorMarkerLeft:GetPos()
            me:MoveToPosition(position, 0, ENTITY_MOVE_RUN, false, true)
            predicateResult = false
        end
        if quest:GetStateBool("NeedBertForSpeech") then
            if quest:IsActiveThreadTerminating() then break end
            quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
            resources:ClearCommands(resource)
            resources:PrepareResource(resource)
            while quest:GetStateBool("NeedBertForSpeech") do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then break end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then break end
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then break end
            if resources:IsPerformingScriptTask(resource) then
                if quest:IsActiveThreadTerminating() then break end
                resources:ClearCommands(resource)
            end
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local arenaState = quest:GetStateInt("ArenaState")
            if arenaState == 3 and not earlyTalk_ then
                if not (quest:IsActiveThreadTerminating()) then
                    goto FLOW_past_lab_00f1a7b7
                end
                ::LAB_00f1a7b7::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                break
                ::FLOW_past_lab_00f1a7b7::
                local scratchValue = resources:ScriptThing(resource)
                local fret_0 = quest:GetHealth(scratchValue)
                if 0.0 < fret_0 then
                    local hero = quest:GetHero()
                    resources:Speak(resource, hero, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_EARLY_FIRST", GROUP_SELECT_FIRST, false, true, false)
                    while resources:IsPerformingScriptTask(resource) do
                        if not quest:NewScriptFrame(me) then goto LAB_00f1a7a3 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00f1a7b7 end
                end
                earlyTalk_ = true
            else
                if arenaState == 5 and not chamTalk_ then
                    if not quest:IsActiveThreadTerminating() then
                        local scratchValue3 = resources:ScriptThing(resource)
                        local fret_00 = quest:GetHealth(scratchValue3)
                        if 0.0 < fret_00 then
                            local hero2 = quest:GetHero()
                            resources:Speak(resource, hero2, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_LATE", GROUP_SELECT_FIRST, false, true, false)
                            while resources:IsPerformingScriptTask(resource) do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    resources:ReleaseResource(resource)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00f1a7a3 end
                        end
                        chamTalk_ = true
                        goto FLOW_past_lab_00f1a7a3
                    end
                    goto LAB_00f1a7a3
                end
                goto FLOW_hoist_lab_00f1a7a3_1
            end
            goto FLOW_past_lab_00f1a7a3
            ::LAB_00f1a7a3::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
            break
            ::FLOW_hoist_lab_00f1a7a3_1::
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
                break
            end
            scratchValue4 = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scratchValue4) then
                local hero3 = quest:GetHero()
                resources:Speak(resource, hero3, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_VERY_LATE", GROUP_SELECT_FIRST, false, true, false)
                while resources:IsPerformingScriptTask(resource) do
                    if not quest:NewScriptFrame(me) then goto LAB_00f1a7a3 end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    break
                end
            end
            ::FLOW_past_lab_00f1a7a3::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        else
            local guardingDoorMarkerLeft2 = quest:GetThingWithScriptName("GuardingDoorMarkerLeft")
            if not quest:IsDistanceBetweenThingsOver(me, guardingDoorMarkerLeft2, 1.0) or resources:IsPerformingScriptTask(resource) then
                if me:MsgIsHitByHero() then
                    goto LAB_00f1a4e2
                else
                    if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f1a4e2 end
                    hb_stk_ffffff34_2 = false
                end
                goto FLOW_past_lab_00f1a4e2
                ::LAB_00f1a4e2::
                hb_stk_ffffff34_2 = true
                ::FLOW_past_lab_00f1a4e2::
                if not hb_stk_ffffff34_2 then
                    if not predicateResult then
                        if quest:IsActiveThreadTerminating() then break end
                        if not resources:IsPerformingScriptTask(resource) then
                            if quest:IsActiveThreadTerminating() then break end
                            predicateResult = true
                            local guardingDoorMarkerLeft3 = quest:GetThingWithScriptName("GuardingDoorMarkerLeft")
                            local fret_03 = guardingDoorMarkerLeft3:GetAngleXY()
                            quest:EntitySetFacingAngle(me, fret_03, false)
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then break end
                    if resources:IsPerformingScriptTask(resource) then
                        if quest:IsActiveThreadTerminating() then break end
                        resources:ClearCommands(resource)
                    end
                    if not quest:GetStateBool("InHitCutsceneAlready") then
                        if quest:IsActiveThreadTerminating() then break end
                        quest:SetStateBool("InHitCutsceneAlready", true)
                        local movie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        local scratchValue5 = resources:ScriptThing(resource)
                        local fret_02 = quest:GetHealth(scratchValue5)
                        if 0.0 < fret_02 then
                            local hero4 = quest:GetHero()
                            resources:Speak(resource, hero4, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
                            while resources:IsPerformingScriptTask(resource) do
                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00f1a7e9 end
                            end
                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00f1a7e9 end
                            goto FLOW_past_lab_00f1a7e9
                            ::LAB_00f1a7e9::
                            resources:DestroyMovie(movie)
                            break
                            ::FLOW_past_lab_00f1a7e9::
                        end
                        quest:ModifyThingHealth(me, 10000.0, false)
                        local hero5 = quest:GetHero()
                        quest:EntitySetThingAsAllyOfThing(me, hero5)
                        local pThing1 = quest:GetHero()
                        quest:EntitySetThingAsAllyOfThing(pThing1, me)
                        quest:SetStateBool("InHitCutsceneAlready", false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                    end
                end
            else
                if quest:IsActiveThreadTerminating() then break end
                local guardingDoorMarkerLeft4 = quest:GetThingWithScriptName("GuardingDoorMarkerLeft")
                local position2 = guardingDoorMarkerLeft4:GetPos()
                me:MoveToPosition(position2, 0, ENTITY_MOVE_WALK, false, true)
                predicateResult = false
            end
        end
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    until false
    resources:ReleaseResource(resource)
end

-- ArenaCellDoorGuard2.Init (retail 0x00f19b60)
function Init(quest, me)
    earlyTalk_ = false
    chamTalk_ = false
    quest:EntitySetAllStategroupsEnabled(me, false)
    quest:EntitySetCombatEnabled(me, false)
end

-- ArenaCellDoorGuard2.OnPersist (retail 0x00f21830)
function OnPersist(quest, me, context)
    local earlyTalk = earlyTalk_ or false
    earlyTalk_ = quest:PersistTransferBool(context, "EarlyTalk", earlyTalk)
    local chamTalk = chamTalk_ or false
    chamTalk_ = quest:PersistTransferBool(context, "ChamTalk", chamTalk)
end

-- ArenaCellDoorGuard2.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

