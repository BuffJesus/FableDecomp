-- Readable native conversion: BeggarBully. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- BeggarBully.Main (retail 0x00e5b3b0)
function Main(quest, me)
    local tauntTimer = quest:GetStateInt("TauntTimer")
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult3, predicateResult6, predicateResult, predicateResult23, predicateResult25
    local beggarHit, health, questionAnswer, questionAnswer2, p1, pppuVar, pppuVar20_b3
    local lookoutPointBeggar, scratchValue15, scratchValue, scratchValue17, movie, movie2, movie3
    local resource, resource2, scratchValue19, movie4
    if not quest:NewScriptFrame(me) then return end
    local resource3 = resources:NewResource()
    resources:PrepareResource(resource3)
    p1 = resource3
    while not resources:TryAcquire(resource3, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(lookoutPointBeggar); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(lookoutPointBeggar); return end
    lookoutPointBeggar = quest:GetThingWithScriptName("LookoutPointBeggar")
    -- TODO(native): xStack_164 = (undefined **)(uint)(uVar2 & 0xffff);
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetAsKillable(me, false, true)
    beggarHit = quest:GetStateBool("BeggarHit")
    predicateResult3 = false
    while ((not beggarHit and (not quest:GetStateBool("BullyHit"))) and (not quest:GetStateBool("BeggarLeft"))) and not quest:GetStateBool("BullyLeft") do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(lookoutPointBeggar); return end
        if me:IsTalkedToByHero() then
            predicateResult3 = true
        end
        scratchValue = unaff_EBP | 1
        if me:MsgIsHitByHero() then
            goto LAB_00e5b649
        else
            scratchValue = unaff_EBP | 3
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue = unaff_EBP | 7
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e5b649 end
            end
            predicateResult6 = false
        end
        goto FLOW_past_lab_00e5b649
        ::LAB_00e5b649::
        predicateResult6 = true
        ::FLOW_past_lab_00e5b649::
        if scratchValue & 4 ~= 0 then
            scratchValue = scratchValue & 0xfffffffb
        end
        if scratchValue & 2 ~= 0 then
            scratchValue = scratchValue & 0xfffffffd
        end
        if scratchValue & 1 ~= 0 then
            scratchValue = scratchValue & 0xfffffffe
        end
        if predicateResult6 then
            quest:SetStateBool("BullyHit", true)
        else
            if predicateResult3 then
                predicateResult3 = false
                if true or quest:GetStateBool("TaughtBelch") then
                    movie3 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if quest:GetStateBool("TaughtBelch") then
                        if not quest:IsActiveThreadTerminating() then
                            if quest:GetStateInt("BelchedAtBeggar") == 0 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e5ce6e end
                                quest:GetHealth(resources:ScriptThing(resource2))
                                -- TODO(native): pppuVar20_b3 = !(fVar15 <= (float10)0.0);
                                if pppuVar20_b3 then
                                    if not me:Speak(hero, "TEXT_QST_015_BULLY_FRIEND_BELCH_COMMAND", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5bab8 end
                                    if not quest:IsActiveThreadTerminating() then goto LAB_00e5c469 end
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    resources:ReleaseResource(lookoutPointBeggar)
                                    return
                                end
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00e5bab8 end
                                if scratchValue15 == 0 then
                                    -- TODO(native): xStack_164._0_3_ = CONCAT12(1,(undefined2)xStack_164);
                                    quest:GetHealth(resources:ScriptThing(resource2))
                                    -- TODO(native): pppuVar20_b3 = !(fVar15 <= (float10)0.0);
                                    if pppuVar20_b3 then
                                        if not me:Speak(hero, "TEXT_QST_015_BULLY_FRIEND_TALK", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5bab8 end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00e5ce8a end
                                    end
                                else
                                    quest:GetHealth(resources:ScriptThing(resource2))
                                    -- TODO(native): pppuVar20_b3 = !(fVar15 <= (float10)0.0);
                                    if not pppuVar20_b3 then goto LAB_00e5c469 end
                                    if not me:Speak(hero, "TEXT_QST_015_BULLY_FRIEND_SNEER_OFFER", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5ce8a end
                                    goto LAB_00e5c45a
                                end
                            end
                            goto FLOW_hoist_lab_00e5c45a_2
                        end
                        goto FLOW_hoist_lab_00e5c45a_3
                    end
                    goto FLOW_past_lab_00e5c45a
                    ::LAB_00e5c45a::
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5bab8 end
                    ::FLOW_hoist_lab_00e5c45a_2::
                    goto LAB_00e5c469
                    ::FLOW_hoist_lab_00e5c45a_3::
                    goto FLOW_hoist_lab_00e5c469_1
                    ::FLOW_past_lab_00e5c45a::
                    goto FLOW_past_lab_00e5c469
                    ::LAB_00e5c469::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    quest:SetTimer(tauntTimer, quest:GetTimer(tauntTimer) + 5)
                    p1 = pppuVar
                    goto LAB_00e5c948
                    ::FLOW_hoist_lab_00e5c469_1::
                    ::LAB_00e5ce6e::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    resources:ReleaseResource(lookoutPointBeggar)
                    do return end
                    ::FLOW_past_lab_00e5c469::
                    if not quest:IsActiveThreadTerminating() then
                        health = quest:GetHealth(resources:ScriptThing(resource2))
                        scratchValue17 = CONCAT13(1,int3p1)
                        if health <= 0.0 then
                            scratchValue17 = p1 & 0xffffff
                        end
                        if scratchValue17 >> 24 ~= 0 then
                            if not me:Speak(hero, "TEXT_QST_015_BULLY_FRIEND_TALK_20", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5ce8a end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e5bab8 end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_015_BULLY_REPEAT_BELCH_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer < 0 do
                            if not quest:NewScriptFrame(me) then goto LAB_00e5ce8a end
                            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if not quest:IsActiveThreadTerminating() then
                            predicateResult = quest:IsActiveThreadTerminating()
                            if questionAnswer == 1 then
                                if not predicateResult then
                                    health = quest:GetHealth(resources:ScriptThing(resource))
                                    pppuVar = CONCAT13(1,int3scratchValue17)
                                    if health <= 0.0 then
                                        pppuVar = scratchValue17 & 0xffffff
                                    end
                                    if pppuVar20_b3 then
                                        if not me:Speak(hero, "TEXT_QST_015_BULLY_TEACH_BELCH", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5bab8 end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00e5ce8a end
                                    end
                                    quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, 10)
                                    if quest:GetStateBool("ExpressionTutorialShown") then
                                        goto LAB_00e5c3a8
                                    end
                                    goto FLOW_past_lab_00e5c3a8
                                    ::LAB_00e5c3a8::
                                    quest:SetStateBool("TaughtBelch", true)
                                    goto LAB_00e5c469
                                    ::FLOW_past_lab_00e5c3a8::
                                    if quest:IsXbox() then
                                        if not quest:IsActiveThreadTerminating() then
                                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                            while not quest:MsgIsGameInfoClickedPast() do
                                                if not quest:NewScriptFrame(me) then goto LAB_00e5ce8a end
                                            end
                                            if not quest:IsActiveThreadTerminating() then goto LAB_00e5c3a1 end
                                        end
                                        goto LAB_00e5bab8
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                                        while not quest:MsgIsGameInfoClickedPast() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00e5bab8 end
                                        end
                                        if not quest:IsActiveThreadTerminating() then goto LAB_00e5c3a1 end
                                    end
                                    goto FLOW_past_lab_00e5c3a1
                                    ::LAB_00e5c3a1::
                                    quest:SetStateBool("ExpressionTutorialShown", true)
                                    goto LAB_00e5c3a8
                                    ::FLOW_past_lab_00e5c3a1::
                                end
                                goto LAB_00e5ce8a
                            end
                            goto FLOW_hoist_lab_00e5ce8a_1
                        end
                    end
                    goto FLOW_past_lab_00e5ce8a
                    ::LAB_00e5ce8a::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    resources:ReleaseResource(lookoutPointBeggar)
                    do return end
                    ::FLOW_hoist_lab_00e5ce8a_1::
                    if predicateResult then goto FLOW_past_lab_00e5ce8a end
                    health = quest:GetHealth(resources:ScriptThing(resource))
                    pppuVar = CONCAT13(1,int3scratchValue17)
                    if health <= 0.0 then
                        pppuVar = scratchValue17 & 0xffffff
                    end
                    if pppuVar20_b3 then
                        if not me:Speak(hero, "TEXT_QST_015_BULLY_REPEAT_BELCH_QUESTION_REFUSAL", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5ce8a end
                        goto LAB_00e5c45a
                    end
                    goto LAB_00e5c469
                    ::FLOW_past_lab_00e5ce8a::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    resources:ReleaseResource(lookoutPointBeggar)
                    return
                end
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then
                    goto LAB_00e5b824
                else
                    if not me:Speak(hero, "TEXT_QST_015_BULLY_FIRST_CHAT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5bab8 end
                    if not quest:IsActiveThreadTerminating() then goto LAB_00e5b824 end
                end
                goto FLOW_past_lab_00e5b824
                ::LAB_00e5b824::
                quest:GiveHeroYesNoQuestion("TEXT_QST_015_BULLY_TEACH_OFFER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                pppuVar = p1
                while questionAnswer2 < 0 do
                    if not quest:NewScriptFrame(me) then goto LAB_00e5bab8 end
                    questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                end
                if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e5b824 end
                if questionAnswer2 == 1 then
                    if quest:GetHealth(resources:ScriptThing(resource)) > 0.0 then
                        if not me:Speak(hero, "TEXT_QST_015_BULLY_TEACH_BELCH", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5bab8 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5bab8 end
                    end
                    quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, 10)
                    if quest:GetStateBool("ExpressionTutorialShown") then
                        goto LAB_00e5bb58
                    end
                    goto FLOW_past_lab_00e5bb58
                    ::LAB_00e5bb58::
                    quest:SetStateBool("TaughtBelch", true)
                    goto LAB_00e5bc19
                    ::FLOW_past_lab_00e5bb58::
                    if quest:IsXbox() then
                        if not quest:IsActiveThreadTerminating() then
                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e5bab8 end
                            end
                            if not quest:IsActiveThreadTerminating() then goto LAB_00e5bb51 end
                        end
                        goto LAB_00e5bab8
                    end
                    if not quest:IsActiveThreadTerminating() then
                        quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                        while not quest:MsgIsGameInfoClickedPast() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e5bab8 end
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
                    resources:ReleaseResource(lookoutPointBeggar)
                    return
                end
                if quest:GetHealth(resources:ScriptThing(resource2)) > 0.0 then
                    if not me:Speak(hero, "TEXT_QST_015_BULLY_TEACH_BELCH_REFUSED", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5bab8 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5bab8 end
                end
                goto LAB_00e5bc19
                goto FLOW_past_lab_00e5bc19
                ::LAB_00e5bc19::
                if not quest:GetStateBool("QuestCardGiven") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5bab8 end
                    quest:GiveHeroQuestCardDirectly(quest:GetActiveQuestName(), "OBJECT_QUEST_CARD_BEGGAR_AND_CHILD", scratchValue19)
                    quest:SetStateBool("QuestCardGiven", true)
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                quest:SetTimer(tauntTimer, quest:GetTimer(tauntTimer) + 5)
                goto LAB_00e5c948
                ::FLOW_past_lab_00e5bc19::
                ::FLOW_past_lab_00e5b824::
                ::LAB_00e5bab8::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                resources:ReleaseResource(lookoutPointBeggar)
                return
            end
            if 0 < quest:GetTimer(tauntTimer) then goto LAB_00e5c948 end
            quest:EntitySetFacingAngleTowardsThing(me, quest:GetThingWithScriptName("LookoutPointBeggar"))
            quest:SetTimer(tauntTimer, 10)
            local timerId = quest:RegisterTimer()
            quest:SetTimer(timerId, 2)
            repeat
                if quest:GetTimer(timerId) < 1 then break end
                if not quest:NewScriptFrame(me) then goto LAB_00e5cea1 end
                if me:IsTalkedToByHero() then
                    predicateResult3 = true
                end
                scratchValue17 = scratchValue | 8
                if me:MsgIsHitByHero() then
                    goto LAB_00e5c69b
                else
                    scratchValue17 = scratchValue | 24
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        scratchValue17 = scratchValue | 56
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e5c69b end
                    end
                    predicateResult23 = false
                    scratchValue = scratchValue17
                end
                goto FLOW_past_lab_00e5c69b
                ::LAB_00e5c69b::
                predicateResult23 = true
                scratchValue = scratchValue17
                ::FLOW_past_lab_00e5c69b::
                if scratchValue & 32 ~= 0 then
                    scratchValue = scratchValue & 0xffffffdf
                end
                if scratchValue & 16 ~= 0 then
                    scratchValue = scratchValue & 0xffffffef
                end
                if scratchValue & 8 ~= 0 then
                    scratchValue = scratchValue & 0xfffffff7
                end
                if predicateResult23 then
                    quest:SetStateBool("BullyHit", true)
                end
            until predicateResult3
            if quest:IsActiveThreadTerminating() then goto LAB_00e5cea1 end
            goto FLOW_past_lab_00e5cea1
            ::LAB_00e5cea1::
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(lookoutPointBeggar)
            do return end
            ::FLOW_past_lab_00e5cea1::
            if not predicateResult3 then
                local switch = math.random(0, 32767) % 5
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
                local conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, lookoutPointBeggar)
                quest:AddLineToConversation(conversationId, "TEXT_QST_015_BULLY_TAUNT", me, nil --[[missing]], false)
                quest:Pause(0.20000000298023224)
                quest:SetStateBool("BullyHasTaunted", true)
                quest:SetTimer(tauntTimer, 12)
                quest:SetTimer(quest:GetStateInt("BeggarReplyTimer"), 5)
            end
            quest:DeregisterTimer(timerId)
        end
        ::LAB_00e5c948::
        resource = me:MsgExpressionPerformedTo()
        if resource ~= nil then
            if quest:IsActiveThreadTerminating() then resources:ReleaseResource(lookoutPointBeggar); return end
            quest:EntitySetFacingAngleTowardsThing(me, hero)
            local timerId2 = quest:RegisterTimer()
            quest:SetTimer(timerId2, 2)
            while true do
                if not (not predicateResult3 and 0 < quest:GetTimer(timerId2)) then break end
                if not quest:NewScriptFrame(me) then goto LAB_00e5cec4 end
                if me:IsTalkedToByHero() then
                    predicateResult3 = true
                end
                scratchValue17 = scratchValue | 64
                if me:MsgIsHitByHero() then
                    goto LAB_00e5caf0
                else
                    scratchValue17 = scratchValue | 192
                    if me:MsgIsHitByAnySpecialAbilityFromHero() then
                        scratchValue17 = scratchValue | 448
                        if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e5caf0 end
                    end
                    predicateResult25 = false
                end
                goto FLOW_past_lab_00e5caf0
                ::LAB_00e5caf0::
                predicateResult25 = true
                ::FLOW_past_lab_00e5caf0::
                if scratchValue17 & 256 ~= 0 then
                    scratchValue17 = scratchValue17 & 0xfffffeff
                end
                if scratchValue17 & 128 ~= 0 then
                    scratchValue17 = scratchValue17 & 0xffffff7f
                end
                if scratchValue17 & 64 ~= 0 then
                    scratchValue17 = scratchValue17 & 0xffffffbf
                end
                scratchValue = scratchValue17
                if predicateResult25 then
                    goto FLOW_hoist_lab_00e5cec4_1
                end
                goto FLOW_past_lab_00e5cec4
                ::LAB_00e5cec4::
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(lookoutPointBeggar)
                do return end
                ::FLOW_hoist_lab_00e5cec4_1::
                quest:SetStateBool("BullyHit", true)
                ::FLOW_past_lab_00e5cec4::
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00e5cedc end
            if predicateResult3 then goto FLOW_native_label_2 end
            if resource == nil then
                goto FLOW_native_label_2
                goto LAB_00e5cd44
            else
                if resource ~= "EXPRESSION_BELCH" then
                    if resource == "EXPRESSION_FART" then goto LAB_00e5cd44 end
                    goto FLOW_native_label_2
                end
                -- TODO(native): xStack_138 = xStack_138 + 1;
                local conversationId2 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId2, hero)
                repeat
                    if movie3 == 1 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_015_BULLY_FIRST_BELCH", me, hero, false)
                        break
                    elseif movie3 == 2 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_015_BULLY_SECOND_BELCH", me, hero, false)
                        break
                    elseif movie3 == 3 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_015_BULLY_THIRD_BELCH", me, hero, false)
                        break
                    elseif movie3 == 4 then
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
            ::FLOW_native_label_2::
            quest:DeregisterTimer(timerId2)
            goto LAB_00e5cd74
            ::LAB_00e5cedc::
            quest:DeregisterTimer(timerId2)
            resources:ReleaseResource(lookoutPointBeggar)
            return
        end
        ::LAB_00e5cd74::
        beggarHit = quest:GetStateBool("BeggarHit")
        -- TODO(native): unaff_EBP = uVar12;
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(lookoutPointBeggar); return end
    resources:PrepareResource(lookoutPointBeggar)
    quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_BELCH_DUMMY", 2, -1)
    quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 2, -1)
    quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_SNEER_DUMMY", 2, -1)
    resources:ReleaseResource(lookoutPointBeggar)
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

