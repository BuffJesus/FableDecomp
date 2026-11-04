-- Readable native conversion: ArenaCellDoorGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local chamTalk_, earlyTalk_

-- ArenaCellDoorGuard.Main (retail 0x00f17c70)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult2, predicateResult12, scratchValue2, scratchValue4, scratchValue33
    local resource, resource2, movie2, resource3, resource4
    local function ReleaseControl()
        resources:ReleaseResource(resource4)
    end
    resource4 = resources:NewResource()
    resource = resource4
    quest:EntitySetOpinionReactionsEnabled(me, false)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    resources:PrepareResource(resource4)
    while not resources:TryAcquire(resource4, me, 4) do
        if not quest:NewScriptFrame(me) then ReleaseControl(); return end
    end
    local predicateResult = quest:IsActiveThreadTerminating()
    if not predicateResult then
        scratchValue2 = predicateResult and 1 or 0
        quest:SetThingHasInformation(me, false, true, false)
        predicateResult2 = quest:IsActiveThreadTerminating()
        repeat
            if predicateResult2 then
                ReleaseControl()
                return
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then break end
                if resources:IsPerformingScriptTask(resource4) then
                    if quest:IsActiveThreadTerminating() then break end
                    resources:ClearCommands(resource4)
                end
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                scratchValue4 = quest:GetStateInt("ArenaState")
                if scratchValue4 ~= 3 then
                    if scratchValue4 ~= 5 or chamTalk_ then
                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                        local scratchValue5 = resources:ScriptThing(resource4)
                        if 0.0 < quest:GetHealth(scratchValue5) then
                            local hero = quest:GetHero()
                            resources:Speak(resource4, hero, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_SECOND_QUESTION", GROUP_SELECT_FIRST, false, true, false)
                            while resources:IsPerformingScriptTask(resource4) do
                                if not quest:NewScriptFrame(me) then goto LAB_00f19a17 end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                resources:ReleaseResource(resource4)
                                return
                            end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_ENTER_ARENA_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        scratchValue4 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while scratchValue4 < 0 do
                            if not quest:NewScriptFrame(me) then goto LAB_00f19a3c end
                            scratchValue4 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                        if scratchValue4 ~= 1 then
                            do
                                local scratchValue6 = resources:ScriptThing(resource4)
                                if 0.0 < quest:GetHealth(scratchValue6) then
                                    local hero2 = quest:GetHero()
                                    resources:Speak(resource4, hero2, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_ANOTHER_REFUSAL", GROUP_SELECT_FIRST, false, true, false)
                                    while resources:IsPerformingScriptTask(resource4) do
                                        if not quest:NewScriptFrame(me) then goto LAB_00f19a3c end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                                end
                                goto LAB_00f194b7
                            end
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
                                    local scratchValue7 = resources:ScriptThing(resource4)
                                    if 0.0 < quest:GetHealth(scratchValue7) then
                                        resources:Speak(resource4, arenaCellDoorGuard, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND0", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource4) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    local scratchValue8 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue8) then
                                        resources:Speak(resource3, me, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND0", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource3) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    goto FLOW_hoist_lab_00f19379_1
                                elseif switch1 == 1 then
                                    local scratchValue9 = resources:ScriptThing(resource4)
                                    if 0.0 < quest:GetHealth(scratchValue9) then
                                        resources:Speak(resource4, arenaCellDoorGuard, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND1", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource4) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    local scratchValue = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue) then
                                        resources:Speak(resource3, me, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND1", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource3) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    local scratchValue11 = resources:ScriptThing(resource4)
                                    if 0.0 < quest:GetHealth(scratchValue11) then
                                        local hero3 = quest:GetHero()
                                        resources:Speak(resource4, hero3, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND1_SECOND", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource4) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    break
                                elseif switch1 == 2 then
                                    local scratchValue12 = resources:ScriptThing(resource4)
                                    if 0.0 < quest:GetHealth(scratchValue12) then
                                        resources:Speak(resource4, arenaCellDoorGuard, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND2", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource4) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    local scratchValue13 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue13) then
                                        resources:Speak(resource3, me, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND2", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource3) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    break
                                elseif switch1 == 3 then
                                    local scratchValue14 = resources:ScriptThing(resource4)
                                    if 0.0 < quest:GetHealth(scratchValue14) then
                                        resources:Speak(resource4, arenaCellDoorGuard, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND3", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource4) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    local scratchValue15 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue15) then
                                        resources:Speak(resource3, me, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND3", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource3) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    break
                                elseif switch1 == 4 then
                                    local scratchValue16 = resources:ScriptThing(resource4)
                                    if 0.0 < quest:GetHealth(scratchValue16) then
                                        resources:Speak(resource4, arenaCellDoorGuard, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND4", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource4) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    local scratchValue17 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue17) then
                                        resources:Speak(resource3, me, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND4", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource3) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    break
                                elseif switch1 == 5 then
                                    local scratchValue18 = resources:ScriptThing(resource4)
                                    if 0.0 < quest:GetHealth(scratchValue18) then
                                        resources:Speak(resource4, arenaCellDoorGuard, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND5", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource4) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    local scratchValue19 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue19) then
                                        resources:Speak(resource3, me, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND5", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource3) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    break
                                elseif switch1 == 6 then
                                    local scratchValue20 = resources:ScriptThing(resource4)
                                    if 0.0 < quest:GetHealth(scratchValue20) then
                                        resources:Speak(resource4, arenaCellDoorGuard, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND6", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource4) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    local scratchValue21 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue21) then
                                        resources:Speak(resource3, me, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND6", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource3) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        goto LAB_00f19379
                                    end
                                    break
                                elseif switch1 == 7 then
                                    local scratchValue22 = resources:ScriptThing(resource4)
                                    if 0.0 < quest:GetHealth(scratchValue22) then
                                        resources:Speak(resource4, arenaCellDoorGuard, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND7", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource4) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    local scratchValue23 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue23) then
                                        resources:Speak(resource3, me, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND7", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource3) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    local scratchValue24 = resources:ScriptThing(resource4)
                                    if 0.0 < quest:GetHealth(scratchValue24) then
                                        resources:Speak(resource4, arenaCellDoorGuard, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND7_SECOND", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource4) do
                                            if not quest:NewScriptFrame(me) then goto LAB_00f19a09 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a2a end
                                    end
                                    local scratchValue25 = resources:ScriptThing(resource3)
                                    if 0.0 < quest:GetHealth(scratchValue25) then
                                        local hero4 = quest:GetHero()
                                        resources:Speak(resource3, hero4, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND7_SECOND", GROUP_SELECT_FIRST, false, true, false)
                                        while resources:IsPerformingScriptTask(resource3) do
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
                            quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
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
                        resources:DestroyMovie(movie2)
                        resources:ReleaseResource(resource4)
                        return
                    end
                    local scratchValue26 = resources:ScriptThing(resource4)
                    if 0.0 < quest:GetHealth(scratchValue26) then
                        local hero5 = quest:GetHero()
                        resources:Speak(resource4, hero5, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_LATE_QUESTION", GROUP_SELECT_FIRST, false, true, false)
                        while resources:IsPerformingScriptTask(resource4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00f19a17 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_ENTER_ARENA_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    scratchValue4 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while scratchValue4 < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00f19a17 end
                        scratchValue4 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        local predicateResult9 = quest:IsActiveThreadTerminating()
                        if scratchValue4 == 1 then
                            if predicateResult9 then goto LAB_00f19a17 end
                            quest:SetStateBool("NeedBertForSpeech", true)
                            local arenaCellDoorGuard23 = quest:GetThingWithScriptName("ArenaCellDoorGuard2")
                            resource2 = resources:NewResource()
                            resources:PrepareResource(resource2)
                            while not resources:TryAcquire(resource2, arenaCellDoorGuard23, 4) do
                                if not quest:NewScriptFrame(me) then goto LAB_00f199ae end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00f199b9 end
                            goto FLOW_past_lab_00f199b9
                            ::LAB_00f199b9::
                            resources:ReleaseResource(resource2)
                            goto LAB_00f19a17
                            ::FLOW_past_lab_00f199b9::
                            local scratchValue27 = resources:ScriptThing(resource4)
                            if 0.0 < quest:GetHealth(scratchValue27) then
                                resources:Speak(resource4, arenaCellDoorGuard23, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_YES_ROUND0", GROUP_SELECT_FIRST, false, true, false)
                                while resources:IsPerformingScriptTask(resource4) do
                                    if not quest:NewScriptFrame(me) then goto LAB_00f199ae end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f199b9 end
                            end
                            local scratchValue28 = resources:ScriptThing(resource2)
                            if 0.0 < quest:GetHealth(scratchValue28) then
                                resources:Speak(resource2, me, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD2_YES_ROUND0", GROUP_SELECT_FIRST, false, true, false)
                                while resources:IsPerformingScriptTask(resource2) do
                                    if not quest:NewScriptFrame(me) then goto LAB_00f199ae end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f199b9 end
                            end
                            quest:FadeScreenOut(0.5, 0.5)
                            quest:Pause(0.5)
                            quest:SetStateInt("ArenaState", 6)
                            quest:ClearHeroEnemyOfGuards(quest:GetStateThing("CellsVillage"))
                            resources:PrepareResource(resource2)
                            quest:SetStateBool("NeedBertForSpeech", false)
                            resources:ReleaseResource(resource2)
                            chamTalk_ = true
                            goto LAB_00f194b7
                        end
                        if not predicateResult9 then
                            local scratchValue29 = resources:ScriptThing(resource4)
                            if 0.0 < quest:GetHealth(scratchValue29) then
                                local hero6 = quest:GetHero()
                                resources:Speak(resource4, hero6, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_NO", GROUP_SELECT_FIRST, false, true, false)
                                while resources:IsPerformingScriptTask(resource4) do
                                    if not quest:NewScriptFrame(me) then goto LAB_00f19a17 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f19a3c end
                            end
                            chamTalk_ = true
                            goto LAB_00f194b7
                        end
                    end
                    goto LAB_00f19a3c
                end
                if not quest:IsActiveThreadTerminating() then
                    if not earlyTalk_ then
                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                        local scratchValue30 = resources:ScriptThing(resource4)
                        if 0.0 < quest:GetHealth(scratchValue30) then
                            local hero7 = quest:GetHero()
                            resources:Speak(resource4, hero7, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_EARLY_FIRST", GROUP_SELECT_FIRST, false, true, false)
                            while resources:IsPerformingScriptTask(resource4) do
                                if not quest:NewScriptFrame(me) then goto LAB_00f19a17 end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                        end
                        earlyTalk_ = true
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00f19a17 end
                        local scratchValue31 = resources:ScriptThing(resource4)
                        if 0.0 < quest:GetHealth(scratchValue31) then
                            local hero8 = quest:GetHero()
                            resources:Speak(resource4, hero8, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_EARLY_SECOND", GROUP_SELECT_FIRST, false, true, false)
                            while resources:IsPerformingScriptTask(resource4) do
                                if not quest:NewScriptFrame(me) then goto LAB_00f19a3c end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                resources:ReleaseResource(resource4)
                                return
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
                resources:DestroyMovie(movie2)
                resources:ReleaseResource(resource4)
                return
            end
            if me:MsgIsHitByHero() then
                goto LAB_00f19571
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f19571 end
                predicateResult12 = false
            end
            goto FLOW_past_lab_00f19571
            ::LAB_00f19571::
            predicateResult12 = true
            ::FLOW_past_lab_00f19571::
            if predicateResult12 then
                if quest:IsActiveThreadTerminating() then break end
                if resources:IsPerformingScriptTask(resource4) then
                    if quest:IsActiveThreadTerminating() then break end
                    resources:ClearCommands(resource4)
                end
                if not quest:GetStateBool("InHitCutsceneAlready") then
                    if quest:IsActiveThreadTerminating() then break end
                    quest:SetStateBool("InHitCutsceneAlready", true)
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue32 = resources:ScriptThing(resource4)
                    if 0.0 < quest:GetHealth(scratchValue32) then
                        local hero9 = quest:GetHero()
                        resources:Speak(resource4, hero9, "TEXT_QST_005_V2_ARENA_MAIN_CELL_GUARD_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
                        while resources:IsPerformingScriptTask(resource4) do
                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00f19a6b end
                        end
                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00f19a6b end
                        goto FLOW_past_lab_00f19a6b
                        ::LAB_00f19a6b::
                        resources:DestroyMovie(movie)
                        resources:ReleaseResource(resource4)
                        do return end
                        ::FLOW_past_lab_00f19a6b::
                    end
                    quest:ModifyThingHealth(me, 10000.0, false)
                    scratchValue33 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(me, scratchValue33)
                    scratchValue33 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(scratchValue33, me)
                    quest:SetStateBool("InHitCutsceneAlready", false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                end
            else
                scratchValue33 = quest:GetThingWithScriptName("GuardingDoorMarkerRight")
                if not ((not quest:IsDistanceBetweenThingsOver(me, scratchValue33, 1.0)) or resources:IsPerformingScriptTask(resource4)) then
                    if quest:IsActiveThreadTerminating() then break end
                    local p0 = quest:GetThingWithScriptName("GuardingDoorMarkerRight"):GetPos()
                    me:MoveToPosition(p0, 0, ENTITY_MOVE_WALK, false, true)
                    scratchValue2 = 0
                elseif scratchValue2 == 0 then
                    if quest:IsActiveThreadTerminating() then break end
                    if not resources:IsPerformingScriptTask(resource4) then
                        if quest:IsActiveThreadTerminating() then break end
                        scratchValue2 = 1
                        local fret_27 = quest:GetThingWithScriptName("GuardingDoorMarkerRight"):GetAngleXY()
                        quest:EntitySetFacingAngle(me, fret_27, false)
                    end
                end
            end
            ::LAB_00f19935::
            quest:NewScriptFrame(me)
            predicateResult2 = quest:IsActiveThreadTerminating()
        until false
    end
    resources:ReleaseResource(resource4)
    do return end
    ::LAB_00f19a09::
    resources:ReleaseResource(resource3)
    ::LAB_00f19a17::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie2)
    resources:ReleaseResource(resource4)
    do return end
    ::LAB_00f199ae::
    resources:ReleaseResource(resource2)
    ::LAB_00f19a37::
    ::LAB_00f19a3c::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie2)
    resources:ReleaseResource(resource4)
end

-- ArenaCellDoorGuard.Init (retail 0x00f17c20)
function Init(quest, me)
    earlyTalk_ = false
    chamTalk_ = false
    quest:EntitySetAllStategroupsEnabled(me, false)
    quest:EntitySetCombatEnabled(me, false)
end

-- ArenaCellDoorGuard.OnPersist (retail 0x00f217e0)
function OnPersist(quest, me, context)
    local earlyTalk = earlyTalk_ or false
    earlyTalk_ = quest:PersistTransferBool(context, "EarlyTalk", earlyTalk)
    local chamTalk = chamTalk_ or false
    chamTalk_ = quest:PersistTransferBool(context, "ChamTalk", chamTalk)
end

-- ArenaCellDoorGuard.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

