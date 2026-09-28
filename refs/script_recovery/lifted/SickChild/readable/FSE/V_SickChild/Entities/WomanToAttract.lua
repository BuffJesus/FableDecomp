-- Readable native conversion: WomanToAttract. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local givenObject, timesMadeLaugh_

-- WomanToAttract.Main (retail 0x00ecf7f0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, taskRunning, predicateResult43, predicateResult44, timesMadeLaugh2
    local scratchValue, getHero, scratchValue39, line, movie, movie2, movie3, scratchValue49
    if not quest:NewScriptFrame(me) then return end
    local this_00 = resources:MemberResource("seh_me", me)
    resources:PrepareResource(this_00)
    while not me:AcquireControl(4) do
        if not quest:NewScriptFrame(me) then return end
        scratchValue49 = this_00
    end
    if quest:IsActiveThreadTerminating() then return end
    if quest:GetStateBool("LaughingWomanKilled") then
        quest:RemoveThing(me, false, true)
    end
    -- TODO(native): xStack_a8 = *(CScriptThing **)(this + 0xc);
    -- TODO(native): xStack_c8 = *(C3DMeshInfo **)(this + 0x10);
    if scratchValue49 ~= nil then
        -- TODO(native): *(int *)xStack_c8 = *(int *)xStack_c8 + 1;
    end
    quest:SetIsPushableByHero(nil --[[missing]], false)
    if not givenObject then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetThingHasInformation(me, false, false, false)
    end
    local registerTimer = quest:RegisterTimer()
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            quest:DeregisterTimer(registerTimer)
            return
        end
        if quest:GetTimer(registerTimer) == 0 then
            if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(predicateResult44); return end
                local conversationId = quest:AddNewConversation(hero, unaff_EBP, false)
                quest:AddPersonToConversation(conversationId, hero)
                local timesMadeLaugh = timesMadeLaugh_
                if timesMadeLaugh == 0 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_NOT_LAUGHED", hero, nil --[[missing]], false)
                    me:PlayAnimation("STANDARD_TALK_SADNESS", false, false, true, false, true, false, false)
                else
                    if timesMadeLaugh == 1 then
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_LAUGHED_A_BIT", hero, nil --[[missing]], false)
                        me:PlayAnimation("ST_OPINION_RIDICULE_SNIGGER", false, false, true, false, true, false, false)
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE", hero, nil --[[missing]], false)
                        goto LAB_00ecfbad
                    end
                    if timesMadeLaugh == 2 then
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL", hero, nil --[[missing]], false)
                        me:PlayAnimation("ST_OPINION_RIDICULE_POINT_AND_LAUGH", false, false, true, false, true, false, false)
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE", hero, nil --[[missing]], false)
                        goto LAB_00ecfbad
                    end
                end
                ::LAB_00ecfbad::
                quest:SetTimer(20, 15.0)
            end
        end
        -- TODO(native): cVar4 = (**(*pCVar9 + 0x74))("")
    --[[unresolved native value]]
        if nil and not givenObject then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(predicateResult44); return end
            if scratchValue == nil then
                goto LAB_00ecff84
            end
            goto FLOW_past_lab_00ecff84
            ::LAB_00ecff84::
            if scratchValue == nil then
                if false then
                    goto LAB_00ed002a
                elseif false then
                    goto LAB_00ed005f
                elseif false then
                    goto LAB_00ed0091
                else
                    goto LAB_00ed00e0
                    goto LAB_00ed00c3
                end
                goto FLOW_hoist_lab_00ed00c3_3
            end
            goto FLOW_past_lab_00ed00c3
            ::LAB_00ed00c3::
            if not quest:IsActiveThreadTerminating() then line = "TEXT_QST_B10_LAUGHING_WOMAN_ARMPUMP"; goto LAB_00ed00db end
            ::FLOW_hoist_lab_00ed00c3_3::
            goto FLOW_hoist_lab_00ed0091_3
            ::FLOW_past_lab_00ed00c3::
            goto FLOW_past_lab_00ed0091
            ::LAB_00ed0091::
            if not quest:IsActiveThreadTerminating() then line = "TEXT_QST_B10_LAUGHING_WOMAN_HEROPOSE"; goto LAB_00ed00db end
            ::FLOW_hoist_lab_00ed0091_3::
            goto FLOW_hoist_lab_00ed005f_2
            ::FLOW_past_lab_00ed0091::
            goto FLOW_past_lab_00ed005f
            ::LAB_00ed005f::
            if not quest:IsActiveThreadTerminating() then line = "TEXT_QST_B10_LAUGHING_WOMAN_FLIRT"; goto LAB_00ed00db end
            ::FLOW_hoist_lab_00ed005f_2::
            goto FLOW_hoist_lab_00ed002a_1
            ::FLOW_past_lab_00ed005f::
            goto FLOW_past_lab_00ed002a
            ::LAB_00ed002a::
            if not quest:IsActiveThreadTerminating() then line = "TEXT_QST_B10_LAUGHING_WOMAN_RUDE"; goto LAB_00ed00db end
            ::FLOW_hoist_lab_00ed002a_1::
            goto FLOW_past_lab_00ed00db
            ::LAB_00ed00db::
            goto LAB_00ed00e0
            ::FLOW_past_lab_00ed00db::
            quest:DeregisterTimer(predicateResult44)
            do return end
            ::FLOW_past_lab_00ed002a::
            if scratchValue == "EXPRESSION_SHIT" then goto LAB_00ed002a end
            if scratchValue == "EXPRESSION_FLIRT" then goto LAB_00ed005f end
            if scratchValue == "EXPRESSION_HEROIC_STANCE" then goto LAB_00ed0091 end
            if scratchValue == "EXPRESSION_VICTORY_PUMP" then goto LAB_00ed00c3 end
            ::LAB_00ed00e0::
            resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            movie3 = resources:ScriptThing(this_00)
            if 0.0 < quest:GetHealth(movie3) then
                -- TODO(native): pCVar19 = *(this + 4)
                scratchValue = nil --[[unresolved native value]]
                me:Speak(hero, line, 0, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie("TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL")
                        quest:DeregisterTimer(predicateResult44)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie("TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL")
                    quest:DeregisterTimer(predicateResult44)
                    return
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie("TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL")
            ::LAB_00ed0694::
            -- TODO(native): unaff_EBP = pCVar9;
            goto LAB_00ed069d
            ::FLOW_past_lab_00ecff84::
            if scratchValue ~= "EXPRESSION_PELVIC_THRUST" then
                if scratchValue ~= "EXPRESSION_COCK_A_DOODLE_DO" then
                    if scratchValue ~= "EXPRESSION_CROTCH_GRAB" then
                        if scratchValue ~= "EXPRESSION_KISS_MY_ASS" then
                            if scratchValue ~= "EXPRESSION_FLAMENCO" then
                                if scratchValue ~= "EXPRESSION_COSSACK" then
                                    if scratchValue ~= "EXPRESSION_AIR_GUITAR" then
                                        if scratchValue ~= "EXPRESSION_BALLET" then
                                            if scratchValue ~= "EXPRESSION_SATURDAY_NIGHT_FEVER" then
                                                if scratchValue ~= "EXPRESSION_TAP" then
                                                    if scratchValue ~= "EXPRESSION_GIGGLE" then goto LAB_00ecff84 end
                                                end
                                            end
                                        end
                                    end
                                end
                            end
                        end
                    end
                end
            end
            timesMadeLaugh_ = timesMadeLaugh_ + 1
            if scratchValue == nil then
                if false then
                    goto LAB_00ed0245
                elseif false then
                    goto LAB_00ed027a
                else
                    goto LAB_00ed02ff
                    goto LAB_00ed04c2
                    goto FLOW_hoist_lab_00ed04c2_6
                    goto FLOW_hoist_lab_00ed04c2_7
                end
                goto FLOW_hoist_lab_00ed04c2_9
                goto FLOW_hoist_lab_00ed04c2_10
            end
            goto FLOW_past_lab_00ed04c2
            ::LAB_00ed04c2::
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(predicateResult44); return end
            line = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_GIGGLE_ONCE"
            goto LAB_00ed02fa
            ::FLOW_hoist_lab_00ed04c2_6::
            goto LAB_00ed02e2
            ::FLOW_hoist_lab_00ed04c2_7::
            goto LAB_00ed02ac
            ::FLOW_hoist_lab_00ed04c2_9::
            goto FLOW_hoist_lab_00ed02ac_2
            ::FLOW_hoist_lab_00ed04c2_10::
            goto FLOW_hoist_lab_00ed02ac_3
            ::FLOW_past_lab_00ed04c2::
            goto FLOW_past_lab_00ed02ac
            ::LAB_00ed02ac::
            if not quest:IsActiveThreadTerminating() then line = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_KISSARSE_ONCE"; goto LAB_00ed02fa end
            ::FLOW_hoist_lab_00ed02ac_2::
            goto FLOW_hoist_lab_00ed027a_2
            ::FLOW_hoist_lab_00ed02ac_3::
            goto FLOW_hoist_lab_00ed027a_3
            ::FLOW_past_lab_00ed02ac::
            goto FLOW_past_lab_00ed027a
            ::LAB_00ed027a::
            if not quest:IsActiveThreadTerminating() then line = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_CROTCH_ONCE"; goto LAB_00ed02fa end
            ::FLOW_hoist_lab_00ed027a_2::
            goto FLOW_hoist_lab_00ed0245_1
            ::FLOW_hoist_lab_00ed027a_3::
            goto FLOW_hoist_lab_00ed0245_2
            ::FLOW_past_lab_00ed027a::
            goto FLOW_past_lab_00ed0245
            ::LAB_00ed0245::
            if not quest:IsActiveThreadTerminating() then line = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_COCK_ONCE"; goto LAB_00ed02fa end
            ::FLOW_hoist_lab_00ed0245_1::
            quest:DeregisterTimer(predicateResult44)
            do return end
            ::FLOW_hoist_lab_00ed0245_2::
            goto LAB_00ed0210
            ::FLOW_past_lab_00ed0245::
            goto FLOW_past_lab_00ed0210
            ::LAB_00ed0210::
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(predicateResult44); return end
            line = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_VULGAR_ONCE"
            ::LAB_00ed02fa::
            ::LAB_00ed02ff::
            movie2 = resources:StartMovie("")
            quest:StartMovieSequence()
            -- TODO(native): pCVar9 = *(this + 4)
            scratchValue39 = nil --[[unresolved native value]]
            -- TODO(native): (**(code **)(*(int *)pCVar9 + 0x5ec))(1);
            timesMadeLaugh2 = timesMadeLaugh_
            if timesMadeLaugh2 ~= 1 then
                if timesMadeLaugh2 == 2 then
                    if 0.0 < quest:GetHealth(resources:ScriptThing(this_00)) then
                        if not me:Speak(hero, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00ed0a36 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00ed0411 end
                    end
                elseif timesMadeLaugh2 == 3 then
                    DoLeavingCutscene(quest, me)
                    givenObject = true
                    quest:GiveHeroMorality(quest:ReadGlobalGameData(1908))
                elseif 0.0 < quest:GetHealth(resources:ScriptThing(this_00)) then
                    if not me:Speak(hero, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00ed0a36 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00ed0411 end
                end
                goto FLOW_past_lab_00ed0411
                ::LAB_00ed0411::
                -- TODO(native): iVar5 = *pCVar9
--[[unresolved native value]]
                goto LAB_00ed0a5a
                ::FLOW_past_lab_00ed0411::
                goto LAB_00ed0666
            end
            goto FLOW_past_lab_00ed0666
            ::LAB_00ed0666::
            quest:SetTimer(0x1238c8c, 20)
            -- TODO(native): (**(code **)(*(int *)pCVar6 + 0x5ec))(0);
            resources:DestroyMovie(movie3)
            goto LAB_00ed0694
            ::FLOW_past_lab_00ed0666::
            if 0.0 >= quest:GetHealth(resources:ScriptThing(this_00)) then goto LAB_00ed0666 end
            me:Speak(hero, line, 0, false, true, false)
            taskRunning = me:IsPerformingScriptTask()
            getHero = scratchValue
            while taskRunning do
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then
                    taskRunning = me:IsPerformingScriptTask()
                else
                    -- TODO(native): (**(code **)(*(int *)pCVar6 + 0x5ec))(0);
                    resources:DestroyMovie(movie2)
                    quest:DeregisterTimer(predicateResult44)
                    do return end
                    taskRunning = me:IsPerformingScriptTask()
                end
            end
            scratchValue = getHero
            if not quest:IsActiveThreadTerminating() then goto LAB_00ed0666 end
            -- TODO(native): iVar5 = *pCVar6
--[[unresolved native value]]
            ::LAB_00ed0a5a::
            -- TODO(native): (**(code **)(iVar5 + 0x5ec))(0);
            resources:DestroyMovie(movie2)
            quest:DeregisterTimer(predicateResult44)
            do return end
            ::FLOW_past_lab_00ed0210::
            if scratchValue == "EXPRESSION_PELVIC_THRUST" then goto LAB_00ed0210 end
            if scratchValue == "EXPRESSION_COCK_A_DOODLE_DO" then goto LAB_00ed0245 end
            if scratchValue == "EXPRESSION_CROTCH_GRAB" then goto LAB_00ed027a end
            if scratchValue == "EXPRESSION_KISS_MY_ASS" then goto LAB_00ed02ac end
            if scratchValue ~= "EXPRESSION_FLAMENCO" then
                if scratchValue == "EXPRESSION_COSSACK" then goto LAB_00ed02e2 end
                if scratchValue == "EXPRESSION_AIR_GUITAR" then goto LAB_00ed02e2 end
                if scratchValue == "EXPRESSION_BALLET" then goto LAB_00ed02e2 end
                if scratchValue == "EXPRESSION_SATURDAY_NIGHT_FEVER" then goto LAB_00ed02e2 end
                if scratchValue == "EXPRESSION_TAP" then goto LAB_00ed02e2 end
                if scratchValue == "EXPRESSION_GIGGLE" then goto LAB_00ed04c2 end
                goto LAB_00ed02ff
            end
            ::LAB_00ed02e2::
            if not quest:IsActiveThreadTerminating() then line = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE"; goto LAB_00ed02fa end
            quest:DeregisterTimer(predicateResult44)
            return
        end
        ::LAB_00ed069d::
        -- TODO(native): cVar4 = (**(*pCVar9 + 0x6c))("SCRIPT_NAME_HERO")
    --[[unresolved native value]]
        if not nil then
            goto LAB_00ed06d4
        else
            predicateResult43 = true
            if givenObject then goto LAB_00ed06d4 end
        end
        goto FLOW_past_lab_00ed06d4
        ::LAB_00ed06d4::
        predicateResult43 = false
        ::FLOW_past_lab_00ed06d4::
        if predicateResult43 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(predicateResult44); return end
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            local timesMadeLaugh3 = timesMadeLaugh_
            if timesMadeLaugh3 == 0 then
                predicateResult44 = quest:GetHealth(resources:ScriptThing(this_00)) > 0.0
                scratchValue39 = scratchValue
                scratchValue = scratchValue39
                if predicateResult44 then
                    -- TODO(native): iVar5 = *this_00
--[[unresolved native value]]
                    if not me:Speak(hero, "TEXT_QST_B10_LAUGHING_WOMAN_INTRO", GROUP_SELECT_FIRST, false, true, false) then break end
                    goto LAB_00ed0952
                end
            elseif timesMadeLaugh3 == 1 then
                predicateResult44 = quest:GetHealth(resources:ScriptThing(this_00)) > 0.0
                scratchValue39 = scratchValue
                scratchValue = scratchValue39
                if predicateResult44 then
                    -- TODO(native): iVar5 = *this_00
--[[unresolved native value]]
                    if not me:Speak(hero, "TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_ONCE", GROUP_SELECT_FIRST, false, true, false) then break end
                    goto LAB_00ed0952
                end
            elseif timesMadeLaugh3 == 2 then
                predicateResult44 = quest:GetHealth(resources:ScriptThing(this_00)) > 0.0
                scratchValue39 = scratchValue
                scratchValue = scratchValue39
                if predicateResult44 then
                    if not me:Speak(hero, "TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_TWICE", GROUP_SELECT_FIRST, false, true, false) then break end
                    goto LAB_00ed0952
                end
            end
            goto FLOW_past_lab_00ed0952
            ::LAB_00ed0952::
            scratchValue = scratchValue39
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                quest:DeregisterTimer(predicateResult44)
                return
            end
            ::FLOW_past_lab_00ed0952::
            quest:SetTimer(registerTimer, quest:GetTimer(registerTimer) + 5)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        if givenObject then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(predicateResult44); return end
            resources:PrepareResource(this_00)
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveThing(me, false, true)
            end
            quest:DeregisterTimer(predicateResult44)
            return
        end
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
    until false
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    quest:DeregisterTimer(predicateResult44)
    do return end
    ::LAB_00ed0a36::
    -- TODO(native): (**(code **)(*(int *)pCVar9 + 0x5ec))(0);
    resources:DestroyMovie(movie2)
    quest:DeregisterTimer(predicateResult44)
end

-- WomanToAttract.Init (retail 0x00ecd380)
function Init(quest, me)
    givenObject = false
    timesMadeLaugh_ = 0
end

-- WomanToAttract.OnPersist (retail 0x00ecd9b0)
function OnPersist(quest, me, context)
    quest:SetStateBool("GivenObject", quest:PersistTransferBool(context, "GivenObject", quest:GetStateBool("GivenObject")))
end

-- WomanToAttract.OnPredicateFail (retail 0x00ecd390)
function OnPredicateFail(quest, me)
    local predicateResult
    if not givenObject then
        predicateResult = true
        if me:MsgIsKilledBy("") then goto LAB_00ecd3d4 end
    end
    predicateResult = false
    ::LAB_00ecd3d4::
    if not predicateResult then return end
    quest:GiveHeroObject(quest:ReadGlobalGameDataString(1848), -1, false)
    quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(1896))
    quest:SetStateBool("LaughingWomanKilled", true)
end

-- WomanToAttract.DoLeavingCutscene (retail 0x00ed0b10)
-- ED0B10: bsim names this body NScript::CV_SickChildScript::CWomanToAttract::DoLeavingCutscene (a homologous script member); no PDB name
function DoLeavingCutscene(quest, me)
    local resources = quest:RetailResources()
    local pOther, actorMap
    local resource = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
    local actorMap2 = resources:NewActorMap()
    resources:SetActor(actorMap2, "HERO", resource)
    resources:SetActor(actorMap2, "WOMAN", resources:MemberResource("seh_me", me))
    -- TODO(native): xStack_1c = malloc(0x18);
    -- TODO(native): *xStack_1c = 0;
    -- TODO(native): *(undefined4 *)(xStack_1c + 4) = 0;
    -- TODO(native): *(undefined1 **)(xStack_1c + 8) = xStack_1c;
    -- TODO(native): *(undefined1 **)(xStack_1c + 0xc) = xStack_1c;
    quest:ReadGlobalGameDataString(1848)
    resources:SetString(actorMap, "$ITEM", pOther)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings("CS_SICK_LAUGHING_WOMAN_LEAVES", actorMap2, actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    if actorMap ~= nil then
        -- TODO(native): free(xStack_1c);
    end
    resources:DestroyActorMap(actorMap2)
    resources:ReleaseResource(resource)
end

