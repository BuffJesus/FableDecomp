-- Readable native conversion: ArenaCellExitGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- ArenaCellExitGuard.Main (retail 0x00f1b9c0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult6, predicateResult8, predicateResult24, predicateResult31, questionAnswer
    local questionAnswer2, resource, this_00, movie2
    local function ReleaseEverything()
        local this_00 = movie2
        resources:DestroyMovie(this_00)
        resources:ReleaseResource(resource)
    end
    local function ReleaseControl()
        resources:ReleaseResource(resource)
    end
    local resource2 = resources:NewResource()
    resource = resource2
    quest:EntitySetInFaction(me, "FACTION_HERO")
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    predicateResult6 = false
    predicateResult31 = false
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    predicateResult8 = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult8 then
            resources:ReleaseResource(resource2)
            return
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then ReleaseControl(); return end
            if resources:IsPerformingScriptTask(resource2) then
                if quest:IsActiveThreadTerminating() then ReleaseControl(); return end
                resources:ClearCommands(resource2)
            end
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if quest:GetStateInt("ArenaState") == 3 then
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie
                else
                    if predicateResult6 then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie
                            goto FLOW_after_lab_00f1c9b3
                        end
                        local scratchValue = resources:ScriptThing(resource2)
                        if 0.0 < quest:GetHealth(scratchValue) then
                            local hero = quest:GetHero()
                            resources:Speak(resource2, hero, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_SECOND", GROUP_SELECT_FIRST, false, true, false)
                            while resources:IsPerformingScriptTask(resource2) do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = movie
                                    goto FLOW_after_lab_00f1c9d8
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = movie
                                goto FLOW_after_lab_00f1c9b3
                            end
                        end
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie
                            goto FLOW_after_lab_00f1c9d8
                        end
                        local scratchValue14 = resources:ScriptThing(resource2)
                        if 0.0 < quest:GetHealth(scratchValue14) then
                            local hero2 = quest:GetHero()
                            resources:Speak(resource2, hero2, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_FIRST", GROUP_SELECT_FIRST, false, true, false)
                            while resources:IsPerformingScriptTask(resource2) do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = movie
                                    goto FLOW_after_lab_00f1c9b3
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = movie
                                goto FLOW_after_lab_00f1c9d8
                            end
                        end
                        local scratchValue15 = resources:ScriptThing(resource2)
                        if 0.0 < quest:GetHealth(scratchValue15) then
                            local hero3 = quest:GetHero()
                            resources:Speak(resource2, hero3, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_FIRST_EXTRA", GROUP_SELECT_FIRST, false, true, false)
                            while resources:IsPerformingScriptTask(resource2) do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = movie
                                    goto FLOW_after_lab_00f1c9b3
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = movie
                                goto FLOW_after_lab_00f1c9d8
                            end
                        end
                        predicateResult6 = true
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_EXIT_ARENA_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00f1bfb3 end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        local predicateResult = quest:IsActiveThreadTerminating()
                        if questionAnswer == 1 then
                            if not predicateResult then
                                local scratchValue16 = resources:ScriptThing(resource2)
                                if 0.0 < quest:GetHealth(scratchValue16) then
                                    local hero4 = quest:GetHero()
                                    resources:Speak(resource2, hero4, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_STAYING", GROUP_SELECT_FIRST, false, true, false)
                                    while resources:IsPerformingScriptTask(resource2) do
                                        if not quest:NewScriptFrame(me) then goto LAB_00f1c9c5 end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00f1bfb3 end
                                end
                                goto LAB_00f1c512
                            end
                            goto LAB_00f1bfb3
                        end
                        if not predicateResult then
                            local scratchValue17 = resources:ScriptThing(resource2)
                            if 0.0 < quest:GetHealth(scratchValue17) then
                                local hero5 = quest:GetHero()
                                resources:Speak(resource2, hero5, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_LEAVING", GROUP_SELECT_FIRST, false, true, false)
                                while resources:IsPerformingScriptTask(resource2) do
                                    if not quest:NewScriptFrame(me) then goto LAB_00f1bfb3 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f1c9c5 end
                            end
                            local getActiveQuestName = quest:GetActiveQuestName()
                            quest:SetQuestAsFailed(getActiveQuestName, true, "", true)
                            quest:SetStateBool("MissionFailed", true)
                            goto LAB_00f1c512
                        end
                    end
                    ::LAB_00f1c9c5::
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie
                end
                ::FLOW_after_lab_00f1c9b3::
            else
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie
                else
                    if predicateResult6 then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie
                            goto FLOW_after_lab_00f1c9d8_376
                        end
                        local scratchValue18 = resources:ScriptThing(resource2)
                        if 0.0 < quest:GetHealth(scratchValue18) then
                            local hero6 = quest:GetHero()
                            resources:Speak(resource2, hero6, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_SECOND", GROUP_SELECT_FIRST, false, true, false)
                            while resources:IsPerformingScriptTask(resource2) do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = movie
                                    goto FLOW_after_lab_00f1c9d8
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = movie
                                goto FLOW_after_lab_00f1c9d8_376
                            end
                        end
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie
                            goto FLOW_after_lab_00f1c9d8
                        end
                        local scratchValue19 = resources:ScriptThing(resource2)
                        if 0.0 < quest:GetHealth(scratchValue19) then
                            local hero7 = quest:GetHero()
                            resources:Speak(resource2, hero7, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_FIRST", GROUP_SELECT_FIRST, false, true, false)
                            while resources:IsPerformingScriptTask(resource2) do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = movie
                                    goto FLOW_after_lab_00f1c9d8_376
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = movie
                                goto FLOW_after_lab_00f1c9d8
                            end
                        end
                        predicateResult6 = true
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_EXIT_ARENA_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer2 < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00f1bfb3 end
                        questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        local predicateResult22 = quest:IsActiveThreadTerminating()
                        if questionAnswer2 == 1 then
                            if predicateResult22 then
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = movie
                                goto LAB_00f1ca0d
                            end
                            local scratchValue20 = resources:ScriptThing(resource2)
                            if 0.0 < quest:GetHealth(scratchValue20) then
                                local hero8 = quest:GetHero()
                                resources:Speak(resource2, hero8, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_STAYING", GROUP_SELECT_FIRST, false, true, false)
                                while resources:IsPerformingScriptTask(resource2) do
                                    if not quest:NewScriptFrame(me) then goto LAB_00f1bfb3 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f1bfb3 end
                            end
                        else
                            if predicateResult22 then goto LAB_00f1bfb3 end
                            local scratchValue21 = resources:ScriptThing(resource2)
                            if 0.0 < quest:GetHealth(scratchValue21) then
                                local hero9 = quest:GetHero()
                                resources:Speak(resource2, hero9, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_LEAVING", GROUP_SELECT_FIRST, false, true, false)
                                while resources:IsPerformingScriptTask(resource2) do
                                    if not quest:NewScriptFrame(me) then goto LAB_00f1bfb3 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f1bfb3 end
                            end
                            quest:SetStateBool("MissionFailed", true)
                            local getActiveQuestName2 = quest:GetActiveQuestName()
                            quest:SetQuestAsFailed(getActiveQuestName2, true, "", true)
                        end
                        goto LAB_00f1c512
                    end
                    goto FLOW_hoist_lab_00f1c512_1
                end
                ::FLOW_after_lab_00f1c9d8_376::
            end
            ::FLOW_after_lab_00f1c9d8::
            goto FLOW_past_lab_00f1c512
            ::LAB_00f1c512::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            goto LAB_00f1c953
            ::FLOW_hoist_lab_00f1c512_1::
            ::LAB_00f1bfb3::
            quest:PauseAllNonScriptedEntities(false)
            this_00 = movie
            ::FLOW_past_lab_00f1c512::
            ::LAB_00f1ca0d::
            resources:DestroyMovie(this_00)
            ReleaseControl(); return
        end
        if me:MsgIsHitByHero() then
            goto LAB_00f1c5ba
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f1c5ba end
            predicateResult24 = false
        end
        goto FLOW_past_lab_00f1c5ba
        ::LAB_00f1c5ba::
        predicateResult24 = true
        ::FLOW_past_lab_00f1c5ba::
        if predicateResult24 then
            if quest:IsActiveThreadTerminating() then ReleaseControl(); return end
            if resources:IsPerformingScriptTask(resource2) then
                if quest:IsActiveThreadTerminating() then ReleaseControl(); return end
                resources:ClearCommands(resource2)
            end
            if not quest:GetStateBool("InHitCutsceneAlready") then
                if quest:IsActiveThreadTerminating() then ReleaseControl(); return end
                quest:SetStateBool("InHitCutsceneAlready", true)
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue22 = resources:ScriptThing(resource2)
                if 0.0 < quest:GetHealth(scratchValue22) then
                    local hero10 = quest:GetHero()
                    resources:Speak(resource2, hero10, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_ATTACKED", GROUP_SELECT_FIRST, false, true, false)
                    while resources:IsPerformingScriptTask(resource2) do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            ReleaseEverything(); do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        ReleaseEverything()
                        return
                    end
                end
                quest:ModifyThingHealth(me, 10000.0, false)
                local hero11 = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(me, hero11)
                local hero12 = quest:GetHero()
                quest:EntitySetThingAsAllyOfThing(hero12, me)
                quest:SetStateBool("InHitCutsceneAlready", false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            end
        else
            local cellExitMarker = quest:GetThingWithScriptName("CellExitMarker")
            if not ((not quest:IsDistanceBetweenThingsOver(me, cellExitMarker, 1.0)) or resources:IsPerformingScriptTask(resource2)) then
                if quest:IsActiveThreadTerminating() then ReleaseControl(); return end
                local p0 = quest:GetThingWithScriptName("CellExitMarker"):GetPos()
                me:MoveToPosition(p0, 0, ENTITY_MOVE_WALK, false, true)
                predicateResult31 = false
            elseif not predicateResult31 then
                if quest:IsActiveThreadTerminating() then ReleaseControl(); return end
                if not resources:IsPerformingScriptTask(resource2) then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
                    predicateResult31 = true
                    local fret_09 = quest:GetThingWithScriptName("CellExitMarker"):GetAngleXY()
                    quest:EntitySetFacingAngle(me, fret_09, true)
                end
            end
        end
        ::LAB_00f1c953::
        quest:NewScriptFrame(me)
        predicateResult8 = quest:IsActiveThreadTerminating()
    until false
end

-- ArenaCellExitGuard.Init (retail 0x00f1b990)
function Init(quest, me)
end

-- ArenaCellExitGuard.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- ArenaCellExitGuard.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

