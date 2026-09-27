-- Readable native conversion: ArenaCellDoorGuard2. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local earlyTalk, chamTalk

-- ArenaCellDoorGuard2.Main (retail 0x00f19bb0)
function Main(quest, me)
    local hero_ = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, scratchValue, scratchValue7, scratchValue8, meControl
    scratchValue8 = 0
    local resource = resources:NewResource()
    quest:SetStateThing("CellsVillage", quest:GetNearestWithDefName(me, "VILLAGE_ARENA_CELLS"))
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    while quest:GetStateInt("ArenaState") == 2 do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    resources:PrepareResource(meControl)
    while not resources:TryAcquire(meControl, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    predicateResult = quest:IsActiveThreadTerminating()
    if predicateResult or quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    repeat
        if quest:GetStateInt("ArenaState") == 4 then
            if quest:IsActiveThreadTerminating() then break end
            quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
            resources:PrepareResource(meControl)
            while quest:GetStateInt("ArenaState") == 4 do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then break end
            quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
            resources:PrepareResource(meControl)
            while not resources:TryAcquire(meControl, me, 4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then break end
            local guardingDoorMarkerLeft = quest:GetThingWithScriptName("GuardingDoorMarkerLeft")
            me:MoveToPosition(guardingDoorMarkerLeft:GetPos(), 0, ENTITY_MOVE_RUN, false, true)
            predicateResult = false
        end
        if quest:GetStateBool("NeedBertForSpeech") then
            if quest:IsActiveThreadTerminating() then break end
            quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
            resources:PrepareResource(meControl)
            while quest:GetStateBool("NeedBertForSpeech") do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then break end
            resources:PrepareResource(meControl)
            while not resources:TryAcquire(meControl, me, 4) do
                if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
            end
            if quest:IsActiveThreadTerminating() then break end
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then break end
            local movie2 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local arenaState = quest:GetStateInt("ArenaState")
            if arenaState == 3 and not earlyTalk then
                if quest:IsActiveThreadTerminating() then goto LAB_00f1a7b7 end
                goto FLOW_past_lab_00f1a7b7
                ::LAB_00f1a7b7::
                quest:PauseAllNonScriptedEntities(false)
                -- LAB_00f1a7ed: (native jump target)
                resources:DestroyMovie(movie2)
                break
                ::FLOW_past_lab_00f1a7b7::
                local fret_0 = quest:GetHealth(resources:ScriptThing(meControl))
                if 0.0 < fret_0 then
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00f1a7a3 end
                    end
                    if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00f1a7b7
                end
                earlyTalk = true
            else
                if arenaState == 5 and not chamTalk then
                    if not quest:IsActiveThreadTerminating() then
                        local fret_00 = quest:GetHealth(resources:ScriptThing(meControl))
                        if 0.0 < fret_00 then
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00f1a7b7
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00f1a7a3 end
                        end
                        chamTalk = true
                        goto LAB_00f1a343
                    end
                    goto LAB_00f1a7a3
                end
                goto FLOW_hoist_lab_00f1a7a3_1
            end
            goto FLOW_past_lab_00f1a7a3
            ::LAB_00f1a7a3::
            quest:PauseAllNonScriptedEntities(false)
            -- TODO(native): goto LAB_00f1a7ed
            ::FLOW_hoist_lab_00f1a7a3_1::
            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00f1a7b7
            if 0.0 < quest:GetHealth(resources:ScriptThing(meControl)) then
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00f1a7a3 end
                end
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00f1a7b7
            end
            ::FLOW_past_lab_00f1a7a3::
            ::LAB_00f1a343::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie2)
        else
            scratchValue = scratchValue8 | 3
            if scratchValue & 2 ~= 0 then
                scratchValue = scratchValue & 0xfffffffd
            end
            if scratchValue & 1 ~= 0 then
                scratchValue = scratchValue & 0xfffffffe
            end
            if in_stack_ffffff34 & 0xffffff >> 24 == 0 then
                scratchValue7 = scratchValue | 4
                scratchValue8 = scratchValue7
                if not me:MsgIsHitByHero() then
                    scratchValue7 = scratchValue | 12
                    scratchValue8 = scratchValue7
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        scratchValue7 = scratchValue | 28
                        scratchValue8 = scratchValue7
                    end
                end
                if scratchValue7 & 16 ~= 0 then
                    scratchValue7 = scratchValue7 & 0xffffffef
                    scratchValue8 = scratchValue7
                end
                if scratchValue7 & 8 ~= 0 then
                    scratchValue7 = scratchValue7 & 0xfffffff7
                    scratchValue8 = scratchValue7
                end
                if scratchValue7 & 4 ~= 0 then
                    scratchValue8 = scratchValue7 & 0xfffffffb
                end
                if in_stack_ffffff34 & 0xffffff >> 24 == 0 then
                    if not predicateResult then
                        if quest:IsActiveThreadTerminating() then break end
                        if not me:IsPerformingScriptTask() then
                            predicateResult = true
                            local guardingDoorMarkerLeft3 = quest:GetThingWithScriptName("GuardingDoorMarkerLeft")
                            quest:EntitySetFacingAngle(me, guardingDoorMarkerLeft3:GetAngleXY(), false)
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then break end
                    if not quest:GetStateBool("InHitCutsceneAlready") then
                        quest:SetStateBool("InHitCutsceneAlready", true)
                        local movie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        local fret_02 = quest:GetHealth(resources:ScriptThing(meControl))
                        if 0.0 < fret_02 then
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00f1a7e9 end
                            end
                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00f1a7e9 end
                            goto FLOW_past_lab_00f1a7e9
                            ::LAB_00f1a7e9::
                            -- TODO(native): goto LAB_00f1a7ed
                            ::FLOW_past_lab_00f1a7e9::
                        end
                        quest:ModifyThingHealth(me, 10000.0, false)
                        quest:EntitySetThingAsAllyOfThing(me, hero_)
                        quest:EntitySetThingAsAllyOfThing(hero_, me)
                        quest:SetStateBool("InHitCutsceneAlready", false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                    end
                end
            else
                if quest:IsActiveThreadTerminating() then break end
                local guardingDoorMarkerLeft4 = quest:GetThingWithScriptName("GuardingDoorMarkerLeft")
                me:MoveToPosition(guardingDoorMarkerLeft4:GetPos(), 0, ENTITY_MOVE_WALK, false, true)
                predicateResult = false
            end
        end
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:DestroyMovie(meControl)
            do return end
        end
    until false
    resources:ReleaseResource(resource)
end

-- ArenaCellDoorGuard2.Init (retail 0x00f19b60)
function Init(quest, me)
    earlyTalk = false
    chamTalk = false
    quest:EntitySetAllStategroupsEnabled(me, false)
    quest:EntitySetCombatEnabled(me, false)
end

-- ArenaCellDoorGuard2.OnPersist (retail 0x00f21830)
function OnPersist(quest, me, context)
    quest:SetStateBool("EarlyTalk", quest:PersistTransferBool(context, "EarlyTalk", quest:GetStateBool("EarlyTalk")))
    quest:SetStateBool("ChamTalk", quest:PersistTransferBool(context, "ChamTalk", quest:GetStateBool("ChamTalk")))
end

-- ArenaCellDoorGuard2.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

