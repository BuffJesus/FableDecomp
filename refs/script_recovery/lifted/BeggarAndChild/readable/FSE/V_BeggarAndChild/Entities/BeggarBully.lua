-- Readable native conversion: BeggarBully. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local self0Xc

-- BeggarBully.Main (retail 0x00e5b3b0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult3, predicateResult6, predicateResult, predicateResult25, belchCount
    local beggarHit, friendReplySelected, scriptThing, timeRemaining, questionAnswer
    local questionAnswer2, timeRemaining2, initialReplySelected, p1, line, pppuVar, conversationId
    local tauntTimer = quest:GetStateInt("TauntTimer")
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    p1 = resource
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    conversationId = self0Xc
    quest:SetIsPushableByHero(me, false)
    local lookoutPointBeggar = quest:GetThingWithScriptName("LookoutPointBeggar")
    friendReplySelected = false
    belchCount = 0; initialReplySelected = false
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetAsKillable(me, false, true)
    beggarHit = quest:GetStateBool("BeggarHit")
    predicateResult3 = false
    while ((not beggarHit and (not quest:GetStateBool("BullyHit"))) and (not quest:GetStateBool("BeggarLeft"))) and not quest:GetStateBool("BullyLeft") do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            predicateResult3 = true
        end
        if me:MsgIsHitByHero() then
            goto LAB_00e5b649
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e5b649 end
            predicateResult6 = false
        end
        goto FLOW_past_lab_00e5b649
        ::LAB_00e5b649::
        predicateResult6 = true
        ::FLOW_past_lab_00e5b649::
        if predicateResult6 then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            quest:SetStateBool("BullyHit", true)
        else
            if predicateResult3 then
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                predicateResult3 = false
                if initialReplySelected or quest:GetStateBool("TaughtBelch") then
                    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                    local movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if quest:GetStateBool("TaughtBelch") then
                        if not quest:IsActiveThreadTerminating() then
                            if quest:GetStateInt("BelchedAtBeggar") == 0 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e5c30a end
                                scriptThing = resources:ScriptThing(resource)
                                if quest:GetHealth(scriptThing) > 0.0 then
                                    line = "TEXT_QST_015_BULLY_FRIEND_BELCH_COMMAND"
                                    local hero = quest:GetHero()
                                    if not me:Speak(hero, line, 0, false, true, false) then
                                        goto LAB_00e5c30a
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00e5c30a end
                                    end
                                    if not quest:IsActiveThreadTerminating() then goto LAB_00e5c469 end
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00e5c30a end
                                if not friendReplySelected then
                                    friendReplySelected = true
                                    scriptThing = resources:ScriptThing(resource)
                                    if quest:GetHealth(scriptThing) > 0.0 then
                                        line = "TEXT_QST_015_BULLY_FRIEND_TALK"
                                        local hero2 = quest:GetHero()
                                        if not me:Speak(hero2, line, 0, false, true, false) then
                                            goto LAB_00e5c30a
                                        end
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00e5c30a end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00e5c30a end
                                    end
                                else
                                    scriptThing = resources:ScriptThing(resource)
                                    if not (quest:GetHealth(scriptThing) > 0.0) then goto LAB_00e5c469 end
                                    line = "TEXT_QST_015_BULLY_FRIEND_SNEER_OFFER"
                                    local hero3 = quest:GetHero()
                                    if not me:Speak(hero3, line, 0, false, true, false) then
                                        goto LAB_00e5c30a
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00e5c30a end
                                    end
                                    goto LAB_00e5c45a
                                end
                            end
                            goto FLOW_hoist_lab_00e5c45a_2
                        end
                        goto FLOW_hoist_lab_00e5c45a_3
                    end
                    goto FLOW_past_lab_00e5c45a
                    ::LAB_00e5c45a::
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5c30a end
                    ::FLOW_hoist_lab_00e5c45a_2::
                    goto LAB_00e5c469
                    ::FLOW_hoist_lab_00e5c45a_3::
                    goto FLOW_hoist_lab_00e5c469_1
                    ::FLOW_past_lab_00e5c45a::
                    goto FLOW_past_lab_00e5c469
                    ::LAB_00e5c469::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    timeRemaining = quest:GetTimer(tauntTimer)
                    quest:SetTimer(tauntTimer, timeRemaining + 5)
                    p1 = pppuVar
                    goto LAB_00e5c948
                    ::FLOW_hoist_lab_00e5c469_1::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    resources:ReleaseResource(resource)
                    do return end
                    ::FLOW_past_lab_00e5c469::
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5c30a end
                    scriptThing = resources:ScriptThing(resource)
                    if quest:GetHealth(scriptThing) > 0.0 then
                        line = "TEXT_QST_015_BULLY_FRIEND_TALK_20"
                        local hero4 = quest:GetHero()
                        if not me:Speak(hero4, line, 0, false, true, false) then
                            goto LAB_00e5c30a
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e5c30a end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5c30a end
                    end
                    quest:GiveHeroYesNoQuestion("TEXT_QST_015_BULLY_REPEAT_BELCH_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    while questionAnswer < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00e5c30a end
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5c30a end
                    if questionAnswer == 1 then
                        scriptThing = resources:ScriptThing(resource)
                        if quest:GetHealth(scriptThing) > 0.0 then
                            line = "TEXT_QST_015_BULLY_TEACH_BELCH"
                            local hero5 = quest:GetHero()
                            if not me:Speak(hero5, line, 0, false, true, false) then
                                goto LAB_00e5c30a
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e5c30a end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e5c30a end
                        end
                        quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, 10)
                        if not (quest:GetStateBool("ExpressionTutorialShown")) then
                            goto FLOW_past_lab_00e5c3a8
                        end
                        ::LAB_00e5c3a8::
                        quest:SetStateBool("TaughtBelch", true)
                        goto LAB_00e5c469
                        ::FLOW_past_lab_00e5c3a8::
                        if quest:IsXbox() then
                            if not quest:IsActiveThreadTerminating() then
                                quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                while not quest:MsgIsGameInfoClickedPast() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e5c30a end
                                end
                                if not quest:IsActiveThreadTerminating() then goto LAB_00e5c3a1 end
                            end
                            goto LAB_00e5c30a
                        end
                        if not quest:IsActiveThreadTerminating() then
                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e5c30a end
                            end
                            if not quest:IsActiveThreadTerminating() then goto LAB_00e5c3a1 end
                        end
                        goto FLOW_past_lab_00e5c3a1
                        ::LAB_00e5c3a1::
                        quest:SetStateBool("ExpressionTutorialShown", true)
                        goto LAB_00e5c3a8
                        ::FLOW_past_lab_00e5c3a1::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    scriptThing = resources:ScriptThing(resource)
                    if quest:GetHealth(scriptThing) > 0.0 then
                        line = "TEXT_QST_015_BULLY_REPEAT_BELCH_QUESTION_REFUSAL"
                        local hero6 = quest:GetHero()
                        if not me:Speak(hero6, line, 0, false, true, false) then
                            goto LAB_00e5c30a
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e5c30a end
                        end
                        goto LAB_00e5c45a
                    end
                    goto LAB_00e5c469
                    ::LAB_00e5c30a::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    resources:ReleaseResource(resource)
                    return
                end
                if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
                initialReplySelected = true
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue = resources:ScriptThing(resource)
                if not (quest:GetHealth(scratchValue) > 0.0) then
                    goto LAB_00e5b824
                else
                    line = "TEXT_QST_015_BULLY_FIRST_CHAT"
                    local hero7 = quest:GetHero()
                    if not me:Speak(hero7, line, 0, false, true, false) then
                        goto FLOW_past_lab_00e5bc19
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e5bc19 end
                    end
                    if not quest:IsActiveThreadTerminating() then goto LAB_00e5b824 end
                end
                goto FLOW_past_lab_00e5bc19
                ::LAB_00e5b824::
                quest:GiveHeroYesNoQuestion("TEXT_QST_015_BULLY_TEACH_OFFER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                pppuVar = p1
                while questionAnswer2 < 0 do
                    if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e5bc19 end
                    questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e5bc19 end
                if questionAnswer2 == 1 then
                    scriptThing = resources:ScriptThing(resource)
                    if quest:GetHealth(scriptThing) > 0.0 then
                        line = "TEXT_QST_015_BULLY_TEACH_BELCH"
                        local hero8 = quest:GetHero()
                        if not me:Speak(hero8, line, 0, false, true, false) then
                            goto FLOW_past_lab_00e5bc19
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e5bc19 end
                        end
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e5bc19 end
                    end
                    quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, 10)
                    if not (quest:GetStateBool("ExpressionTutorialShown")) then
                        goto FLOW_past_lab_00e5bb58
                    end
                    ::LAB_00e5bb58::
                    quest:SetStateBool("TaughtBelch", true)
                    goto LAB_00e5bc19
                    ::FLOW_past_lab_00e5bb58::
                    if quest:IsXbox() then
                        if not quest:IsActiveThreadTerminating() then
                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e5bc19 end
                            end
                            if not quest:IsActiveThreadTerminating() then goto LAB_00e5bb51 end
                        end
                        goto FLOW_past_lab_00e5bc19
                    end
                    if not quest:IsActiveThreadTerminating() then
                        quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e5bc19 end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00e5bb51 end
                    end
                    goto FLOW_past_lab_00e5bb51
                    ::LAB_00e5bb51::
                    quest:SetStateBool("ExpressionTutorialShown", true)
                    goto LAB_00e5bb58
                    ::FLOW_past_lab_00e5bb51::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(resource)
                    return
                end
                do
                    scriptThing = resources:ScriptThing(resource)
                    if quest:GetHealth(scriptThing) > 0.0 then
                        line = "TEXT_QST_015_BULLY_TEACH_BELCH_REFUSED"
                        local hero9 = quest:GetHero()
                        if not me:Speak(hero9, line, 0, false, true, false) then
                            goto FLOW_past_lab_00e5bc19
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e5bc19 end
                        end
                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e5bc19 end
                    end
                    goto LAB_00e5bc19
                end
                goto FLOW_past_lab_00e5bc19
                ::LAB_00e5bc19::
                if not quest:GetStateBool("QuestCardGiven") then
                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e5bc19 end
                    conversationId = quest:GetActiveQuestName()
                    quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BEGGAR_AND_CHILD", conversationId, false)
                    quest:SetStateBool("QuestCardGiven", true)
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                timeRemaining2 = quest:GetTimer(tauntTimer)
                quest:SetTimer(tauntTimer, timeRemaining2 + 5)
                p1 = pppuVar
                goto LAB_00e5c948
                ::FLOW_past_lab_00e5bc19::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:ReleaseResource(resource)
                return
            end
            if 0 < quest:GetTimer(tauntTimer) then goto LAB_00e5c948 end
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
            conversationId = quest:GetThingWithScriptName("LookoutPointBeggar")
            quest:EntitySetFacingAngleTowardsThing(me, conversationId)
            quest:SetTimer(tauntTimer, 10)
            local timerId = quest:RegisterTimer()
            local i_stk_154_1 = timerId
            quest:SetTimer(timerId, 2)
            repeat
                if quest:GetTimer(timerId) < 1 then break end
                if not quest:NewScriptFrame(me) then goto LAB_00e5cea1 end
                if me:IsTalkedToByHero() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5cea1 end
                    predicateResult3 = true
                end
                if me:MsgIsHitByHero() then
                    goto LAB_00e5c69b
                else
                    if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e5c69b end
                    predicateResult = false
                end
                goto FLOW_past_lab_00e5c69b
                ::LAB_00e5c69b::
                predicateResult = true
                ::FLOW_past_lab_00e5c69b::
                if predicateResult then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5cea1 end
                    quest:SetStateBool("BullyHit", true)
                end
            until predicateResult3
            if not (quest:IsActiveThreadTerminating()) then
                goto FLOW_past_lab_00e5cea1
            end
            ::LAB_00e5cea1::
            quest:DeregisterTimer(i_stk_154_1)
            resources:ReleaseResource(resource)
            do return end
            ::FLOW_past_lab_00e5cea1::
            if not predicateResult3 then
                local switch = quest:RetailRandModulo(32768) % 5
                repeat
                    if switch == 0 then
                        me:PlayAnimation("ST_OPINION_DISAPPROVAL_DISMISSING_HAND_SWIPE", false, false, false, true, true, false, false)
                        break
                    elseif switch == 1 then
                        me:PlayAnimation("ST_OPINION_DISAPPROVAL_NYAH_NYAH", false, false, false, true, true, false, false)
                        break
                    elseif switch == 2 then
                        me:PlayAnimation("ST_OPINION_DISAPPROVAL_POINTING_AWAY_GET_OUT", false, false, false, true, true, false, false)
                        break
                    elseif switch == 3 then
                        me:PlayAnimation("ST_OPINION_DISAPPROVAL_SHAKE_FIST", false, false, false, true, true, false, false)
                        break
                    elseif switch == 4 then
                        me:PlayAnimation("ST_OPINION_DISAPPROVAL_POINT_AT", false, false, false, true, true, false, false)
                        break
                    else
                        break
                    end
                until true
                conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, lookoutPointBeggar)
                quest:AddLineToConversation(conversationId, "TEXT_QST_015_BULLY_TAUNT", me, lookoutPointBeggar, false)
                quest:Pause(0.20000000298023224)
                quest:SetStateBool("BullyHasTaunted", true)
                quest:SetTimer(tauntTimer, 12)
                quest:SetTimer(quest:GetStateInt("BeggarReplyTimer"), 5)
            end
            quest:DeregisterTimer(timerId)
        end
        ::LAB_00e5c948::
        local scratchValue16 = me:MsgExpressionPerformedTo()
        if scratchValue16 ~= nil then
            if not quest:IsActiveThreadTerminating() then
                conversationId = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, conversationId)
                local timerId2 = quest:RegisterTimer()
                local i_stk_154_2 = timerId2
                quest:SetTimer(timerId2, 2)
                while true do
                    if not (not predicateResult3 and 0 < quest:GetTimer(timerId2)) then break end
                    if not quest:NewScriptFrame(me) then goto LAB_00e5cec4 end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId2); goto LAB_00e5ce35 end
                        predicateResult3 = true
                    end
                    if me:MsgIsHitByHero() then
                        goto LAB_00e5caf0
                    else
                        if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e5caf0 end
                        predicateResult25 = false
                    end
                    goto FLOW_past_lab_00e5caf0
                    ::LAB_00e5caf0::
                    predicateResult25 = true
                    ::FLOW_past_lab_00e5caf0::
                    if predicateResult25 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5cec4 end
                        goto FLOW_hoist_lab_00e5cec4_1
                    end
                    goto FLOW_past_lab_00e5cec4
                    ::LAB_00e5cec4::
                    quest:DeregisterTimer(timerId2)
                    goto LAB_00e5ce35
                    ::FLOW_hoist_lab_00e5cec4_1::
                    quest:SetStateBool("BullyHit", true)
                    ::FLOW_past_lab_00e5cec4::
                end
                if not quest:IsActiveThreadTerminating() then
                    if predicateResult3 then goto FLOW_past_lab_00e5cd44 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5cedc end
                    if scratchValue16 == nil then
                        do goto FLOW_past_lab_00e5cd44 end
                        goto LAB_00e5cd44
                    else
                        if scratchValue16 ~= "EXPRESSION_BELCH" then
                            if scratchValue16 == "EXPRESSION_FART" then goto LAB_00e5cd44 end
                            goto FLOW_past_lab_00e5cd44
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5cedc end
                        belchCount = belchCount + 1
                        conversationId = quest:AddNewConversation(me, false, false)
                        local hero10 = quest:GetHero()
                        quest:AddPersonToConversation(conversationId, hero10)
                        local switch4 = belchCount
                        repeat
                            if switch4 == 1 then
                                local hero11 = quest:GetHero()
                                quest:AddLineToConversation(conversationId, "TEXT_QST_015_BULLY_FIRST_BELCH", me, hero11, false)
                                break
                            elseif switch4 == 2 then
                                local hero12 = quest:GetHero()
                                quest:AddLineToConversation(conversationId, "TEXT_QST_015_BULLY_SECOND_BELCH", me, hero12, false)
                                break
                            elseif switch4 == 3 then
                                local hero13 = quest:GetHero()
                                quest:AddLineToConversation(conversationId, "TEXT_QST_015_BULLY_THIRD_BELCH", me, hero13, false)
                                break
                            elseif switch4 == 4 then
                                quest:SetStateBool("BelchedAtBully", true)
                                goto LAB_00e5cd53
                            end
                        until true
                    end
                    goto FLOW_past_lab_00e5cd44
                    ::LAB_00e5cd44::
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5cedc end
                    ::LAB_00e5cd53::
                    quest:SetStateBool("BullyLeft", true)
                    ::FLOW_past_lab_00e5cd44::
                    quest:DeregisterTimer(i_stk_154_2)
                    goto LAB_00e5cd74
                end
                ::LAB_00e5cedc::
                quest:DeregisterTimer(i_stk_154_2)
            end
            goto LAB_00e5ce35
        end
        ::LAB_00e5cd74::
        beggarHit = quest:GetStateBool("BeggarHit")
    end
    if not (quest:IsActiveThreadTerminating()) then
        resources:PrepareResource(resource)
        quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, -1)
        quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 2, -1)
        quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_SNEER_DUMMY", 2, -1)
    end
    ::LAB_00e5ce35::
    resources:ReleaseResource(resource)
end

-- BeggarBully.Init (retail 0x00e5b380)
function Init(quest, me)
end

-- BeggarBully.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- BeggarBully.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

