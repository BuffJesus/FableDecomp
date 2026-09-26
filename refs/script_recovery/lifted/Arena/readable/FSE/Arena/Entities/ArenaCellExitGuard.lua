-- Readable native conversion: ArenaCellExitGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- ArenaCellExitGuard.Main (retail 0x00f1b9c0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult2, predicateResult23, predicateResult27, isActiveThreadTerminating
    local questionAnswer, questionAnswer2, thing, scratchValue45, resource, this_00, meControl
    local movie2
    local function ReleaseEverything()
        local this_00 = movie2
        resources:DestroyMovie(this_00)
        resources:ReleaseResource(resource)
    end
    local function ReleaseControl()
        resources:ReleaseResource(resource)
    end
    resource = resources:NewResource()
    quest:EntitySetInFaction(me, "FACTION_HERO")
    resources:PrepareResource(meControl)
    while not resources:TryAcquire(meControl, me, 4) do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    predicateResult2 = false
    isActiveThreadTerminating = false
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    repeat
        if quest:IsActiveThreadTerminating() then return end
        if me:IsTalkedToByHero() then
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if quest:GetStateInt("ArenaState") == 3 then
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie
                else
                    if predicateResult2 then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie
                            goto FLOW_after_lab_00f1c9b3
                        end
                        -- TODO(native): pCVar11 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_12c);
                        local fret_01 = quest:GetHealth(nil --[[missing]])
                        if 0.0 < fret_01 then
                            thing = hero
                            me:Speak(hero, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_SECOND", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
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
                        -- TODO(native): pCVar11 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_12c);
                        local fret_0 = quest:GetHealth(nil --[[missing]])
                        if 0.0 < fret_0 then
                            thing = hero
                            me:Speak(hero, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_FIRST", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
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
                        -- TODO(native): pCVar11 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_12c);
                        local fret_00 = quest:GetHealth(nil --[[missing]])
                        if 0.0 < fret_00 then
                            thing = hero
                            me:Speak(hero, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_FIRST_EXTRA", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
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
                        predicateResult2 = true
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_EXIT_ARENA_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00f1bfb3 end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        local predicateResult9 = quest:IsActiveThreadTerminating()
                        if questionAnswer == 1 then
                            if not predicateResult9 then
                                -- TODO(native): pCVar11 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_12c);
                                local fret_02 = quest:GetHealth(nil --[[missing]])
                                if 0.0 < fret_02 then
                                    thing = hero
                                    if not me:Speak(hero, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_STAYING", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00f1c9c5 end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00f1bfb3 end
                                end
                                goto LAB_00f1c512
                            end
                            goto LAB_00f1bfb3
                        end
                        if not predicateResult9 then
                            -- TODO(native): pCVar11 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_12c);
                            local fret_03 = quest:GetHealth(nil --[[missing]])
                            if 0.0 < fret_03 then
                                thing = hero
                                if not me:Speak(hero, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_LEAVING", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00f1bfb3 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f1c9c5 end
                            end
                            -- TODO(native): CCharString__AssignFromWide("");
                            quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, "", scratchValue45)
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
                    if predicateResult2 then
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie
                            goto FLOW_after_lab_00f1c9d8_375
                        end
                        -- TODO(native): pCVar11 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_12c);
                        local fret_05 = quest:GetHealth(nil --[[missing]])
                        if 0.0 < fret_05 then
                            thing = hero
                            me:Speak(hero, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_SECOND", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
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
                                goto FLOW_after_lab_00f1c9d8_375
                            end
                        end
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie
                            goto FLOW_after_lab_00f1c9d8
                        end
                        -- TODO(native): pCVar11 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_12c);
                        local fret_04 = quest:GetHealth(nil --[[missing]])
                        if 0.0 < fret_04 then
                            thing = hero
                            me:Speak(hero, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_FIRST", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    this_00 = movie
                                    goto FLOW_after_lab_00f1c9d8_375
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = movie
                                goto FLOW_after_lab_00f1c9d8
                            end
                        end
                        predicateResult2 = true
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_EXIT_ARENA_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer2 < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00f1bfb3 end
                        questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if not quest:IsActiveThreadTerminating() then
                        local predicateResult = quest:IsActiveThreadTerminating()
                        if questionAnswer2 == 1 then
                            if predicateResult then
                                quest:PauseAllNonScriptedEntities(false)
                                this_00 = movie
                                goto LAB_00f1ca0d
                            end
                            -- TODO(native): pCVar11 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_12c);
                            local fret_06 = quest:GetHealth(nil --[[missing]])
                            if 0.0 < fret_06 then
                                thing = hero
                                if not me:Speak(hero, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_STAYING", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00f1bfb3 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f1bfb3 end
                            end
                        else
                            if predicateResult then goto LAB_00f1bfb3 end
                            -- TODO(native): pCVar11 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_12c);
                            local fret_07 = quest:GetHealth(nil --[[missing]])
                            if 0.0 < fret_07 then
                                thing = hero
                                if not me:Speak(hero, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_LEAVING", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00f1bfb3 end
                                if quest:IsActiveThreadTerminating() then goto LAB_00f1bfb3 end
                            end
                            quest:SetStateBool("MissionFailed", true)
                            -- TODO(native): CCharString__AssignFromWide("");
                            quest:SetQuestAsFailed(quest:GetActiveQuestName(), true, nil --[[missing]], scratchValue45)
                        end
                        goto LAB_00f1c512
                    end
                    goto FLOW_hoist_lab_00f1c512_1
                end
                ::FLOW_after_lab_00f1c9d8_375::
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
        if thing:MsgIsHitByHero() then
            goto LAB_00f1c5ba
        else
            if thing:MsgIsHitByAnySpecialAbilityFromHero() then
                if not thing:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00f1c5ba end
            end
            predicateResult23 = false
        end
        goto FLOW_past_lab_00f1c5ba
        ::LAB_00f1c5ba::
        predicateResult23 = true
        ::FLOW_past_lab_00f1c5ba::
        if predicateResult23 then
            if not quest:GetStateBool("InHitCutsceneAlready") then
                quest:SetStateBool("InHitCutsceneAlready", true)
                movie2 = resources:StartMovie("")
                -- TODO(native): CWideScreenMagicPauseEntities::CWideScreenMagicPauseEntities((CWideScreenMagicPauseEntities *)&xStack_104,*(int *)(this + 4));
                -- TODO(native): pCVar11 = (CScriptThing *) CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&xStack_12c);
                local fret_08 = quest:GetHealth(nil --[[missing]])
                if 0.0 < fret_08 then
                    thing = hero
                    if not me:Speak(hero, "TEXT_QST_005_V2_ARENA_EXIT_CELL_GUARD_ATTACKED", GROUP_SELECT_FIRST, false, true, false) then
                        -- TODO(native): (**(code **)(*(int *)xStack_104 + 0x5ec))(0);
                        ReleaseEverything(); return
                    end
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): (**(code **)(*(int *)xStack_104 + 0x5ec))(0);
                        ReleaseEverything()
                        return
                    end
                end
                quest:ModifyThingHealth(me, 10000.0, false)
                quest:EntitySetThingAsAllyOfThing(me, hero)
                quest:EntitySetThingAsAllyOfThing(hero, me)
                quest:SetStateBool("InHitCutsceneAlready", false)
                -- TODO(native): (**(code **)(*(int *)xStack_104 + 0x5ec))(0);
                resources:DestroyMovie(movie2)
            end
        else
            if not quest:IsDistanceBetweenThingsOver(me, quest:GetThingWithScriptName("CellExitMarker"), 1.0) then goto LAB_00f1c818 end
            predicateResult27 = true
            if me:IsPerformingScriptTask() then goto LAB_00f1c818 end
            goto FLOW_past_lab_00f1c818
            ::LAB_00f1c818::
            predicateResult27 = false
            ::FLOW_past_lab_00f1c818::
            if predicateResult27 then
                thing = quest:GetThingWithScriptName("CellExitMarker")
                me:MoveToPosition(thing:GetPos(), 0, ENTITY_MOVE_WALK, false, true)
                isActiveThreadTerminating = false
            elseif not isActiveThreadTerminating then
                if not me:IsPerformingScriptTask() then
                    isActiveThreadTerminating = true
                    quest:EntitySetFacingAngle(me, quest:GetThingWithScriptName("CellExitMarker"):GetAngleXY(), true)
                end
            end
        end
        ::LAB_00f1c953::
        quest:NewScriptFrame(me)
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

