-- Readable native conversion: WomanToAttract. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    IngredientObject = 1848,  -- 'OBJECT_MUSHROOM_BLUE_01'
    KilledSomeoneForMushroom = 1896,  -- -0.029999999329447746
    MadeWomanLaugh = 1908,  -- 0.019999999552965164
}

-- per-entity fields (native class members; one Lua state per entity instance)
local givenObject_, timesMadeLaugh_

-- WomanToAttract.Main (retail 0x00ecf7f0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult
    local predicateResult12, predicateResult14, predicateResult17, predicateResult18
    local predicateResult19, predicateResult20, taskRunning, predicateResult32, predicateResult33
    local predicateResult34, predicateResult35, predicateResult36, dist, hb_stk_9c_2
    local timesMadeLaugh2, scratchValue38, scratchValue39, scratchValue40
    local scratchValue41, scratchValue42, line, speechResult, scratchValue43, scratchValue44
    local conversationId, uVar7_pushed, womanFunnyMovie, womanNotFunnyMovie, womanTalkMovie
    local scratchValue47, scratchValue48, scratchValue49
    if not quest:NewScriptFrame(me) then return end
    local this_00 = resources:MemberResource("seh_me", me)
    resources:PrepareResource(this_00)
    scratchValue47 = scratchValue41
    while not me:AcquireControl(4) do
        if not quest:NewScriptFrame(me) then return end
        scratchValue48 = scratchValue41
        scratchValue49 = this_00
    end
    if quest:IsActiveThreadTerminating() then return end
    if quest:GetStateBool("LaughingWomanKilled") then
        if quest:IsActiveThreadTerminating() then return end
        quest:RemoveThing(me, false, true)
    end
    if scratchValue49 ~= nil then
    end
    quest:SetIsPushableByHero(me, false)
    if not givenObject_ then
        if quest:IsActiveThreadTerminating() then return end
        quest:SetThingHasInformation(me, false, false, false)
    end
    local womanTimerId = quest:RegisterTimer()
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            quest:DeregisterTimer(womanTimerId)
            return
        end
        if quest:GetTimer(womanTimerId) == 0 then
            dist = 15.0
            local hero = quest:GetHero()
            if quest:IsDistanceBetweenThingsUnder(me, hero, 15.0) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(womanTimerId); return end
                conversationId = quest:AddNewConversation(me, false, false)
                local hero2 = quest:GetHero()
                quest:AddPersonToConversation(conversationId, hero2)
                local timesMadeLaugh = timesMadeLaugh_
                if timesMadeLaugh == 0 then
                    local hero3 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_NOT_LAUGHED", me, hero3, false)
                    me:PlayAnimation("STANDARD_TALK_SADNESS", false, false, true, false, true, false, false)
                else
                    if timesMadeLaugh == 1 then
                        local hero4 = quest:GetHero()
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_LAUGHED_A_BIT", me, hero4, false)
                        me:PlayAnimation("ST_OPINION_RIDICULE_SNIGGER", false, false, true, false, true, false, false)
                        local hero5 = quest:GetHero()
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE", me, hero5, false)
                        goto LAB_00ecfbad
                    end
                    if timesMadeLaugh == 2 then
                        local hero6 = quest:GetHero()
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_NEARLY_HYSTERICAL", me, hero6, false)
                        me:PlayAnimation("ST_OPINION_RIDICULE_POINT_AND_LAUGH", false, false, true, false, true, false, false)
                        local hero7 = quest:GetHero()
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE", me, hero7, false)
                    end
                end
                ::LAB_00ecfbad::
                quest:SetTimer(womanTimerId, 20)
            end
        end
        local local_womanExpressionName = me:MsgExpressionPerformedTo()
        if local_womanExpressionName ~= nil and not givenObject_ then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(womanTimerId); return end
            if not (local_womanExpressionName == nil) then
                goto FLOW_past_lab_00ecff84
            end
            ::LAB_00ecff84::
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(womanTimerId); return end
            scratchValue43 = "TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY"
            if local_womanExpressionName == nil then
                do goto LAB_00ed00e0 end
                goto LAB_00ed00c3
                goto FLOW_hoist_lab_00ed00c3_3
            end
            goto FLOW_past_lab_00ed00c3
            ::LAB_00ed00c3::
            if not quest:IsActiveThreadTerminating() then scratchValue42 = "TEXT_QST_B10_LAUGHING_WOMAN_ARMPUMP"; goto LAB_00ed00db end
            ::FLOW_hoist_lab_00ed00c3_3::
            goto FLOW_hoist_lab_00ed0091_3
            ::FLOW_past_lab_00ed00c3::
            goto FLOW_past_lab_00ed0091
            ::LAB_00ed0091::
            if not quest:IsActiveThreadTerminating() then scratchValue42 = "TEXT_QST_B10_LAUGHING_WOMAN_HEROPOSE"; goto LAB_00ed00db end
            ::FLOW_hoist_lab_00ed0091_3::
            goto FLOW_hoist_lab_00ed005f_2
            ::FLOW_past_lab_00ed0091::
            goto FLOW_past_lab_00ed005f
            ::LAB_00ed005f::
            if not quest:IsActiveThreadTerminating() then scratchValue42 = "TEXT_QST_B10_LAUGHING_WOMAN_FLIRT"; goto LAB_00ed00db end
            ::FLOW_hoist_lab_00ed005f_2::
            goto FLOW_hoist_lab_00ed002a_1
            ::FLOW_past_lab_00ed005f::
            goto FLOW_past_lab_00ed002a
            ::LAB_00ed002a::
            if not quest:IsActiveThreadTerminating() then scratchValue42 = "TEXT_QST_B10_LAUGHING_WOMAN_RUDE"; goto LAB_00ed00db end
            ::FLOW_hoist_lab_00ed002a_1::
            goto FLOW_past_lab_00ed00db
            ::LAB_00ed00db::
            scratchValue43 = scratchValue42
            goto LAB_00ed00e0
            ::FLOW_past_lab_00ed00db::
            quest:DeregisterTimer(womanTimerId)
            do return end
            ::FLOW_past_lab_00ed002a::
            line = nil --[[reviewed native staging]]
            predicateResult17 = local_womanExpressionName == "EXPRESSION_SHIT"
            if predicateResult17 then goto LAB_00ed002a end
            predicateResult18 = local_womanExpressionName == "EXPRESSION_FLIRT"
            if predicateResult18 then goto LAB_00ed005f end
            predicateResult19 = local_womanExpressionName == "EXPRESSION_HEROIC_STANCE"
            if predicateResult19 then goto LAB_00ed0091 end
            predicateResult20 = local_womanExpressionName == "EXPRESSION_VICTORY_PUMP"
            if predicateResult20 then goto LAB_00ed00c3 end
            ::LAB_00ed00e0::
            womanNotFunnyMovie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            scratchValue41 = nil
            if resources:ResourceThingHealthAbove(this_00, 0.0) then
                scratchValue38 = nil --[[reviewed native staging]]
                line = scratchValue43
                uVar7_pushed = 0
                conversationId = quest:GetHero()
                if not me:Speak(conversationId, line, uVar7_pushed, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(womanNotFunnyMovie)
                    quest:DeregisterTimer(womanTimerId)
                    return
                end
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(womanNotFunnyMovie)
                        quest:DeregisterTimer(womanTimerId)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(womanNotFunnyMovie)
                    quest:DeregisterTimer(womanTimerId)
                    return
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(womanNotFunnyMovie)
            ::LAB_00ed0694::
            goto LAB_00ed069d
            ::FLOW_past_lab_00ecff84::
            line = nil --[[reviewed native staging]]
            local predicateResult21 = local_womanExpressionName == "EXPRESSION_PELVIC_THRUST"
            if not predicateResult21 then
                local predicateResult22 = local_womanExpressionName == "EXPRESSION_COCK_A_DOODLE_DO"
                if not predicateResult22 then
                    local predicateResult23 = local_womanExpressionName == "EXPRESSION_CROTCH_GRAB"
                    if not predicateResult23 then
                        local predicateResult24 = local_womanExpressionName == "EXPRESSION_KISS_MY_ASS"
                        if not predicateResult24 then
                            local predicateResult25 = local_womanExpressionName == "EXPRESSION_FLAMENCO"
                            if not predicateResult25 then
                                local predicateResult26 = local_womanExpressionName == "EXPRESSION_COSSACK"
                                if not predicateResult26 then
                                    local predicateResult27 = local_womanExpressionName == "EXPRESSION_AIR_GUITAR"
                                    if not predicateResult27 then
                                        local predicateResult28 = local_womanExpressionName == "EXPRESSION_BALLET"
                                        if not predicateResult28 then
                                            local predicateResult29 = local_womanExpressionName == "EXPRESSION_SATURDAY_NIGHT_FEVER"
                                            if not predicateResult29 then
                                                local predicateResult30 = local_womanExpressionName == "EXPRESSION_TAP"
                                                if not predicateResult30 then
                                                    local predicateResult31 = local_womanExpressionName == "EXPRESSION_GIGGLE"
                                                    if not predicateResult31 then goto LAB_00ecff84 end
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
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(womanTimerId); return end
            timesMadeLaugh_ = timesMadeLaugh_ + 1
            scratchValue44 = "TEXT_QST_B10_LAUGHING_WOMAN_NOT_FUNNY"
            if local_womanExpressionName == nil then
                do
                    do
                        do
                            do
                                do
                                    do
                                        do
                                            do
                                                do goto LAB_00ed02ff end
                                                goto LAB_00ed04c2
                                            end
                                        end
                                    end
                                end
                            end
                        end
                        goto FLOW_hoist_lab_00ed04c2_6
                    end
                    goto FLOW_hoist_lab_00ed04c2_7
                    goto FLOW_hoist_lab_00ed04c2_9
                end
                goto FLOW_hoist_lab_00ed04c2_10
            end
            goto FLOW_past_lab_00ed04c2
            ::LAB_00ed04c2::
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(womanTimerId); return end
            scratchValue42 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_GIGGLE_ONCE"
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
            if not quest:IsActiveThreadTerminating() then scratchValue42 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_KISSARSE_ONCE"; goto LAB_00ed02fa end
            ::FLOW_hoist_lab_00ed02ac_2::
            goto FLOW_hoist_lab_00ed027a_2
            ::FLOW_hoist_lab_00ed02ac_3::
            goto FLOW_hoist_lab_00ed027a_3
            ::FLOW_past_lab_00ed02ac::
            goto FLOW_past_lab_00ed027a
            ::LAB_00ed027a::
            if not quest:IsActiveThreadTerminating() then scratchValue42 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_CROTCH_ONCE"; goto LAB_00ed02fa end
            ::FLOW_hoist_lab_00ed027a_2::
            goto FLOW_hoist_lab_00ed0245_1
            ::FLOW_hoist_lab_00ed027a_3::
            goto FLOW_hoist_lab_00ed0245_2
            ::FLOW_past_lab_00ed027a::
            goto FLOW_past_lab_00ed0245
            ::LAB_00ed0245::
            if not quest:IsActiveThreadTerminating() then scratchValue42 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_COCK_ONCE"; goto LAB_00ed02fa end
            ::FLOW_hoist_lab_00ed0245_1::
            quest:DeregisterTimer(womanTimerId)
            do return end
            ::FLOW_hoist_lab_00ed0245_2::
            goto LAB_00ed0210
            ::FLOW_past_lab_00ed0245::
            goto FLOW_past_lab_00ed0210
            ::LAB_00ed0210::
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(womanTimerId); return end
            scratchValue42 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_VULGAR_ONCE"
            ::LAB_00ed02fa::
            scratchValue44 = scratchValue42
            ::LAB_00ed02ff::
            womanFunnyMovie = resources:StartMovie("")
            scratchValue41 = nil --[[reviewed native staging]]
            quest:PauseAllNonScriptedEntities(true)
            timesMadeLaugh2 = timesMadeLaugh_
            scratchValue39 = scratchValue41
            if timesMadeLaugh2 ~= 1 then
                if timesMadeLaugh2 == 2 then
                    if resources:ResourceThingHealthAbove(this_00, 0.0) then
                        conversationId = quest:GetHero()
                        if not me:Speak(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_TWICE", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00ed0a36
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00ed0a36 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00ed0411 end
                    end
                elseif timesMadeLaugh2 == 3 then
                    DoLeavingCutscene(quest, me)
                    givenObject_ = true
                    quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.MadeWomanLaugh))
                else
                    if resources:ResourceThingHealthAbove(this_00, 0.0) then
                        conversationId = quest:GetHero()
                        if not me:Speak(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_MADE_LAUGH_ONCE", GROUP_SELECT_FIRST, false, true, false) then
                            goto LAB_00ed0a36
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00ed0a36 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00ed0411 end
                    end
                end
                goto FLOW_past_lab_00ed0411
                ::LAB_00ed0411::
                timesMadeLaugh2 = nil --[[reviewed native staging]]
                goto LAB_00ed0a36
                ::FLOW_past_lab_00ed0411::
                goto LAB_00ed0666
            end
            goto FLOW_past_lab_00ed0666
            ::LAB_00ed0666::
            quest:SetTimer(womanTimerId, 20)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(womanFunnyMovie)
            goto LAB_00ed0694
            ::FLOW_past_lab_00ed0666::
            if not resources:ResourceThingHealthAbove(this_00, 0.0) then goto LAB_00ed0666 end
            line = scratchValue44
            uVar7_pushed = 0
            conversationId = quest:GetHero()
            speechResult = me:Speak(conversationId, line, uVar7_pushed, false, true, false)
            taskRunning = me:IsPerformingScriptTask()
            scratchValue40 = scratchValue38
            while taskRunning do
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then
                    taskRunning = me:IsPerformingScriptTask()
                else
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(womanFunnyMovie)
                    quest:DeregisterTimer(womanTimerId)
                    do return end
                    taskRunning = me:IsPerformingScriptTask()
                end
            end
            predicateResult12 = quest:IsActiveThreadTerminating()
            scratchValue38 = scratchValue40
            if not predicateResult12 then goto LAB_00ed0666 end
            timesMadeLaugh2 = nil --[[reviewed native staging]]
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(womanFunnyMovie)
            quest:DeregisterTimer(womanTimerId)
            do return end
            ::FLOW_past_lab_00ed0210::
            line = nil --[[reviewed native staging]]
            predicateResult32 = local_womanExpressionName == "EXPRESSION_PELVIC_THRUST"
            if predicateResult32 then goto LAB_00ed0210 end
            predicateResult33 = local_womanExpressionName == "EXPRESSION_COCK_A_DOODLE_DO"
            if predicateResult33 then goto LAB_00ed0245 end
            predicateResult34 = local_womanExpressionName == "EXPRESSION_CROTCH_GRAB"
            if predicateResult34 then goto LAB_00ed027a end
            predicateResult35 = local_womanExpressionName == "EXPRESSION_KISS_MY_ASS"
            if predicateResult35 then goto LAB_00ed02ac end
            predicateResult36 = local_womanExpressionName == "EXPRESSION_FLAMENCO"
            if not predicateResult36 then
                local predicateResult37 = local_womanExpressionName == "EXPRESSION_COSSACK"
                if predicateResult37 then goto LAB_00ed02e2 end
                local predicateResult38 = local_womanExpressionName == "EXPRESSION_AIR_GUITAR"
                if predicateResult38 then goto LAB_00ed02e2 end
                local predicateResult39 = local_womanExpressionName == "EXPRESSION_BALLET"
                if predicateResult39 then goto LAB_00ed02e2 end
                local predicateResult40 = local_womanExpressionName == "EXPRESSION_SATURDAY_NIGHT_FEVER"
                if predicateResult40 then goto LAB_00ed02e2 end
                local predicateResult41 = local_womanExpressionName == "EXPRESSION_TAP"
                if predicateResult41 then goto LAB_00ed02e2 end
                local predicateResult42 = local_womanExpressionName == "EXPRESSION_GIGGLE"
                if predicateResult42 then goto LAB_00ed04c2 end
                goto LAB_00ed02ff
            end
            ::LAB_00ed02e2::
            if not quest:IsActiveThreadTerminating() then scratchValue42 = "TEXT_QST_B10_LAUGHING_WOMAN_LAUGH_DANCE_ONCE"; goto LAB_00ed02fa end
            quest:DeregisterTimer(womanTimerId)
            return
        end
        ::LAB_00ed069d::
        if not me:IsTalkedToByHero() then
            hb_stk_9c_2 = false
        else
            hb_stk_9c_2 = true
            if givenObject_ then hb_stk_9c_2 = false end
        end
        if hb_stk_9c_2 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(womanTimerId); return end
            womanTalkMovie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            timesMadeLaugh2 = timesMadeLaugh_
            if timesMadeLaugh2 == 0 then
                conversationId = nil
                local hb_stk_a8_1 = resources:ResourceThingHealthAbove(this_00, 0.0)
                scratchValue41 = scratchValue38
                scratchValue38 = scratchValue41
                if hb_stk_a8_1 then
                    timesMadeLaugh2 = nil --[[reviewed native staging]]
                    conversationId = quest:GetHero()
                    if not me:Speak(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_INTRO", GROUP_SELECT_FIRST, false, true, false) then
                        break
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00ed0a79 end
                    end
                    goto LAB_00ed0952
                end
            elseif timesMadeLaugh2 == 1 then
                conversationId = nil
                local hb_stk_a8_2 = resources:ResourceThingHealthAbove(this_00, 0.0)
                scratchValue41 = scratchValue38
                scratchValue38 = scratchValue41
                if hb_stk_a8_2 then
                    timesMadeLaugh2 = nil --[[reviewed native staging]]
                    conversationId = quest:GetHero()
                    if not me:Speak(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_ONCE", GROUP_SELECT_FIRST, false, true, false) then
                        break
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00ed0a79 end
                    end
                    goto LAB_00ed0952
                end
            elseif timesMadeLaugh2 == 2 then
                conversationId = nil
                local hb_stk_a8_3 = resources:ResourceThingHealthAbove(this_00, 0.0)
                scratchValue41 = scratchValue38
                scratchValue38 = scratchValue41
                if hb_stk_a8_3 then
                    conversationId = quest:GetHero()
                    if not me:Speak(conversationId, "TEXT_QST_B10_LAUGHING_WOMAN_ON_SPEAK_MADE_LAUGH_TWICE", GROUP_SELECT_FIRST, false, true, false) then
                        break
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00ed0a79 end
                    end
                    goto LAB_00ed0952
                end
            end
            goto FLOW_past_lab_00ed0952
            ::LAB_00ed0952::
            predicateResult14 = quest:IsActiveThreadTerminating()
            scratchValue38 = scratchValue41
            if predicateResult14 then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(womanTalkMovie)
                quest:DeregisterTimer(womanTimerId)
                return
            end
            ::FLOW_past_lab_00ed0952::
            local timeRemaining = quest:GetTimer(womanTimerId)
            quest:SetTimer(womanTimerId, timeRemaining + 5)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(womanTalkMovie)
        end
        if not givenObject_ then
            quest:NewScriptFrame(me);
            predicateResult = quest:IsActiveThreadTerminating();
        else
            if not quest:IsActiveThreadTerminating() then
                resources:PrepareResource(this_00)
                if not quest:IsActiveThreadTerminating() then
                    quest:RemoveThing(me, false, true)
                end
            end
            quest:DeregisterTimer(womanTimerId)
            do return end
            quest:NewScriptFrame(me)
            predicateResult = quest:IsActiveThreadTerminating()
        end
        ::continue_3::
    until false
    ::LAB_00ed0a79::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(womanTalkMovie)
    quest:DeregisterTimer(womanTimerId)
    do return end
    ::LAB_00ed0a36::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(womanFunnyMovie)
    quest:DeregisterTimer(womanTimerId)
end

-- WomanToAttract.Init (retail 0x00ecd380)
function Init(quest, me)
    givenObject_ = false
    timesMadeLaugh_ = 0
end

-- WomanToAttract.OnPersist (retail 0x00ecd9b0)
function OnPersist(quest, me, context)
    local givenObject = givenObject_ or false
    givenObject_ = quest:PersistTransferBool(context, "GivenObject", givenObject)
end

-- WomanToAttract.OnPredicateFail (retail 0x00ecd390)
function OnPredicateFail(quest, me)
    local predicateResult, predicateResult2
    predicateResult2 = false
    if not givenObject_ then
        predicateResult2 = true
        predicateResult = true
        if me:MsgIsKilledBy("") then goto LAB_00ecd3d4 end
    end
    predicateResult = false
    ::LAB_00ecd3d4::
    if predicateResult2 then
    end
    if not predicateResult then return end
    local scratchValue = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject)
    quest:GiveHeroObject(scratchValue, -1, false)
    quest:GiveHeroMorality(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.KilledSomeoneForMushroom))
    quest:SetStateBool("LaughingWomanKilled", true)
end

-- WomanToAttract.DoLeavingCutscene (retail 0x00ed0b10)
-- ED0B10: bsim names this body NScript::CV_SickChildScript::CWomanToAttract::DoLeavingCutscene (a homologous script member); no PDB name
function DoLeavingCutscene(quest, me)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    local pThing = quest:GetHero()
    resources:TryAcquire(resource, pThing, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "WOMAN", resources:MemberResource("seh_me", me))
    local actorMap2 = resources:NewStringMap()
    local pOther = quest:ReadGlobalGameDataString(SCRIPT_DEF.IngredientObject) or ""
    resources:SetString(actorMap2, "$ITEM", pOther)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings("CS_SICK_LAUGHING_WOMAN_LEAVES", actorMap, actorMap2, false, true)
    quest:FixMovieSequenceCamera(false)
    resources:DestroyStringMap(actorMap2)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

