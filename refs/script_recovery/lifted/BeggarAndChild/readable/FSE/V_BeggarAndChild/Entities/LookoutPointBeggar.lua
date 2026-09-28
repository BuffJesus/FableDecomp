-- Readable native conversion: LookoutPointBeggar. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- LookoutPointBeggar.Main (retail 0x00e58d40)
function Main(quest, me)
    local tauntTimer = quest:GetStateInt("TauntTimer")
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isActiveThreadTerminating, predicateResult, predicateResult30, scratchValue7
    local scratchValue8, scratchValue9, predicateResult31, beggarHit, scratchValue11, timerId
    local i_stk_1fc_2, switch4, p0, beggarBully, this_00, scratchValue19, scratchValue20
    local conversationId, movie, movie2, movie3, movie4, scratchValue21, resource, scratchValue25
    if not quest:NewScriptFrame(me) then return end
    local resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(beggarBully); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(beggarBully); return end
    beggarBully = quest:GetThingWithScriptName("BeggarBully")
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetAsKillable(me, false, true)
    beggarHit = quest:GetStateBool("BeggarHit")
    isActiveThreadTerminating = false
    while ((not beggarHit and (not quest:GetStateBool("BullyHit"))) and (not quest:GetStateBool("BeggarLeft"))) and not quest:GetStateBool("BullyLeft") do
        if not quest:NewScriptFrame(me) then goto LAB_00e5b19b end
        if me:IsTalkedToByHero() then
            isActiveThreadTerminating = true
        end
        -- TODO(native): xStack_1e4 = xStack_1e4 | 1;
        -- TODO(native): bVar5 = (**(*me + 0x54))(me,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
        if nil then
            goto LAB_00e58fce
        else
            scratchValue19 = 0 | 3
            scratchValue21 = scratchValue19
            -- TODO(native): bVar5 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
            if nil then
                scratchValue19 = 0 | 7
                scratchValue21 = scratchValue19
                -- TODO(native): bVar5 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                if not nil then goto LAB_00e58fce end
            end
            predicateResult = false
        end
        goto FLOW_past_lab_00e58fce
        ::LAB_00e58fce::
        predicateResult = true
        ::FLOW_past_lab_00e58fce::
        if scratchValue19 & 4 ~= 0 then
            scratchValue19 = scratchValue19 & 0xfffffffb
            scratchValue21 = scratchValue19
        end
        if scratchValue19 & 2 ~= 0 then
            scratchValue19 = scratchValue19 & 0xfffffffd
            scratchValue21 = scratchValue19
        end
        if scratchValue19 & 1 ~= 0 then
            -- TODO(native): xStack_1e4 = uVar13 & 0xfffffffe;
        end
        if predicateResult then
            if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
            quest:SetStateBool("BeggarHit", true)
        else
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
                isActiveThreadTerminating = false
                if scratchValue11 == 0 and not quest:GetStateBool("TaughtBattleCry") then
                    scratchValue11 = 1
                    if quest:GetStateInt("BelchedAtBeggar") == 0 then
                        movie3 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 >= quest:GetHealth(resources:ScriptThing(beggarBully)) then goto LAB_00e596a9 end
                        if not me:Speak(hero, "TEXT_QST_015_BEGGAR_FIRST_SPEAK", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e59935 end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00e596a9 end
                        goto FLOW_past_lab_00e596a9
                        ::LAB_00e596a9::
                        quest:GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                        while timerId < 0 do
                            if not quest:NewScriptFrame(me) then goto LAB_00e59935 end
                            timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if not quest:IsActiveThreadTerminating() then
                            local predicateResult4 = quest:IsActiveThreadTerminating()
                            if timerId == 1 then
                                if not predicateResult4 then
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(0)) then
                                        if not me:Speak(hero, "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e59935 end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00e59935 end
                                    end
                                    quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
                                    if quest:GetStateBool("ExpressionTutorialShown") then
                                        goto LAB_00e599d8
                                    end
                                    goto FLOW_past_lab_00e599d8
                                    ::LAB_00e599d8::
                                    quest:SetStateBool("TaughtBattleCry", true)
                                    goto LAB_00e59a93
                                    ::FLOW_past_lab_00e599d8::
                                    if quest:IsXbox() then
                                        if not quest:IsActiveThreadTerminating() then
                                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                            while not quest:MsgIsGameInfoClickedPast() do
                                                if not quest:NewScriptFrame(me) then goto LAB_00e59935 end
                                            end
                                            if not quest:IsActiveThreadTerminating() then goto LAB_00e599d1 end
                                        end
                                        goto LAB_00e59935
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                                        while not quest:MsgIsGameInfoClickedPast() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00e59935 end
                                        end
                                        if not quest:IsActiveThreadTerminating() then goto LAB_00e599d1 end
                                    end
                                    goto FLOW_past_lab_00e599d1
                                    ::LAB_00e599d1::
                                    quest:SetStateBool("ExpressionTutorialShown", true)
                                    goto LAB_00e599d8
                                    ::FLOW_past_lab_00e599d1::
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                goto LAB_00e5b19b
                            end
                            if not predicateResult4 then
                                if 0.0 < quest:GetHealth(resources:ScriptThing(0)) then
                                    if not me:Speak(hero, "TEXT_QST_015_BEGGAR_TEACH_REFUSED", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e59935 end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e59935 end
                                end
                                goto LAB_00e59a93
                            end
                            goto FLOW_past_lab_00e59a93
                            ::LAB_00e59a93::
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00e59aa3
                            ::FLOW_past_lab_00e59a93::
                        end
                        ::FLOW_past_lab_00e596a9::
                        ::LAB_00e59935::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        goto LAB_00e5b19b
                    end
                    movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(beggarBully)) then
                        if not me:Speak(hero, "TEXT_QST_015_BEGGAR_FIRST_SPEAK_BELCH", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5b1b8 end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00e591a3 end
                        goto LAB_00e59429
                    end
                    goto FLOW_past_lab_00e59429
                    ::LAB_00e59429::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00e5b19b
                    ::FLOW_past_lab_00e59429::
                    ::LAB_00e591a3::
                    quest:GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                    while timerId < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00e5b1b8 end
                        timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e59429 end
                    if timerId == 1 then
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e59429 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e5b1b8 end
                        end
                        quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
                        if quest:GetStateBool("ExpressionTutorialShown") then
                            goto LAB_00e594c9
                        end
                        goto FLOW_past_lab_00e594c9
                        ::LAB_00e594c9::
                        quest:SetStateBool("TaughtBattleCry", true)
                        quest:PauseAllNonScriptedEntities(false)
                        goto LAB_00e59aa3
                        ::FLOW_past_lab_00e594c9::
                        if quest:IsXbox() then
                            if not quest:IsActiveThreadTerminating() then
                                quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                while not quest:MsgIsGameInfoClickedPast() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e5b1b8 end
                                end
                                if not quest:IsActiveThreadTerminating() then goto LAB_00e594c2 end
                            end
                            goto LAB_00e59429
                        end
                        if not quest:IsActiveThreadTerminating() then
                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e59429 end
                            end
                            if not quest:IsActiveThreadTerminating() then goto LAB_00e594c2 end
                        end
                        goto FLOW_past_lab_00e594c2
                        ::LAB_00e594c2::
                        quest:SetStateBool("ExpressionTutorialShown", true)
                        goto LAB_00e594c9
                        ::FLOW_past_lab_00e594c2::
                        goto LAB_00e5b1b8
                    end
                    goto FLOW_past_lab_00e5b1b8
                    ::LAB_00e5b1b8::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00e5b19b
                    ::FLOW_past_lab_00e5b1b8::
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        if not me:Speak(hero, "TEXT_QST_015_BEGGAR_TEACH_REFUSED", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5b1b8 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e59429 end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    ::LAB_00e59aa3::
                    resources:DestroyMovie(this_00)
                    if not quest:GetStateBool("QuestCardGiven") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
                        quest:GiveHeroQuestCardDirectly(quest:GetActiveQuestName(), "OBJECT_QUEST_CARD_BEGGAR_AND_CHILD", scratchValue25)
                        quest:SetStateBool("QuestCardGiven", true)
                    end
                else
                    if quest:GetStateInt("BelchedAtBeggar") ~= 0 then
                        local movie5 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if not quest:GetStateBool("TaughtBattleCry") then
                            if not quest:IsActiveThreadTerminating() then
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                    if not me:Speak(hero, "TEXT_QST_015_BEGGAR_REPEAT_BELCH_RETURN", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5b1e6 end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e5b1fc end
                                end
                                quest:GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                                timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                                while timerId < 0 do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e5a527 end
                                    timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    local predicateResult17 = quest:IsActiveThreadTerminating()
                                    if timerId == 1 then
                                        if not predicateResult17 then
                                            if 0.0 < quest:GetHealth(resources:ScriptThing(0)) then
                                                if not me:Speak(hero, "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5a527 end
                                                if quest:IsActiveThreadTerminating() then goto LAB_00e5a527 end
                                            end
                                            quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
                                            if quest:GetStateBool("ExpressionTutorialShown") then
                                                goto LAB_00e5a06d
                                            end
                                            goto FLOW_past_lab_00e5a06d
                                            ::LAB_00e5a06d::
                                            quest:SetStateBool("TaughtBattleCry", true)
                                            goto LAB_00e5a126
                                            ::FLOW_past_lab_00e5a06d::
                                            if quest:IsXbox() then
                                                if not quest:IsActiveThreadTerminating() then
                                                    quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                                    while not quest:MsgIsGameInfoClickedPast() do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00e5a527 end
                                                    end
                                                    if not quest:IsActiveThreadTerminating() then goto LAB_00e5a066 end
                                                end
                                                goto LAB_00e5a527
                                            end
                                            if not quest:IsActiveThreadTerminating() then
                                                quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                                                while not quest:MsgIsGameInfoClickedPast() do
                                                    if not quest:NewScriptFrame(me) then goto LAB_00e5a527 end
                                                end
                                                if not quest:IsActiveThreadTerminating() then goto LAB_00e5a066 end
                                            end
                                            goto FLOW_past_lab_00e5a066
                                            ::LAB_00e5a066::
                                            quest:SetStateBool("ExpressionTutorialShown", true)
                                            goto LAB_00e5a06d
                                            ::FLOW_past_lab_00e5a066::
                                        end
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie4)
                                        goto LAB_00e5b19b
                                    end
                                    if not predicateResult17 then
                                        if 0.0 < quest:GetHealth(resources:ScriptThing(0)) then
                                            if not me:Speak(hero, "TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT_FAIL_BELCH", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5a527 end
                                            if quest:IsActiveThreadTerminating() then goto LAB_00e5a527 end
                                        end
                                        goto LAB_00e5a126
                                    end
                                    goto FLOW_past_lab_00e5a126
                                    ::LAB_00e5a126::
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie5)
                                    goto LAB_00e5a6a9
                                    ::FLOW_past_lab_00e5a126::
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00e5b19b
                            end
                            ::LAB_00e5b1fc::
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie5)
                        else
                            if not quest:IsActiveThreadTerminating() then
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                    if not me:Speak(hero, "TEXT_QST_015_BEGGAR_REPEAT_BATTLE_CRY", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5a527 end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e5a527 end
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie5)
                                goto LAB_00e5a6a9
                            end
                            goto LAB_00e5b1e6
                        end
                        goto FLOW_past_lab_00e5b1e6
                        ::LAB_00e5b1e6::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie5)
                        ::FLOW_past_lab_00e5b1e6::
                        goto LAB_00e5b19b
                    end
                    movie4 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if not quest:GetStateBool("TaughtBattleCry") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5a527 end
                        goto FLOW_hoist_lab_00e5a527_1
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            goto LAB_00e5b19b
                        end
                        if quest:GetHealth(resources:ScriptThing(beggarBully)) <= 0.0 then goto LAB_00e5a683 end
                        if not me:Speak(hero, "TEXT_QST_015_BEGGAR_REPEAT_BATTLE_CRY", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5a527 end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            goto LAB_00e5b19b
                        end
                    end
                    goto FLOW_past_lab_00e5a527
                    ::LAB_00e5a527::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00e5b19b
                    ::FLOW_hoist_lab_00e5a527_1::
                    quest:GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                    while timerId < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00e5b231 end
                        timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5a527 end
                    if timerId == 1 then
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            if not me:Speak(hero, "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5a527 end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e5b231 end
                        end
                        quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
                        if quest:GetStateBool("ExpressionTutorialShown") then
                            goto LAB_00e5a5c8
                        end
                        goto FLOW_past_lab_00e5a5c8
                        ::LAB_00e5a5c8::
                        quest:SetStateBool("TaughtBattleCry", true)
                        goto LAB_00e5a683
                        ::FLOW_past_lab_00e5a5c8::
                        if quest:IsXbox() then
                            if not quest:IsActiveThreadTerminating() then
                                quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                while not quest:MsgIsGameInfoClickedPast() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e5b231 end
                                end
                                if not quest:IsActiveThreadTerminating() then goto LAB_00e5a5c1 end
                            end
                            goto LAB_00e5a527
                        end
                        if not quest:IsActiveThreadTerminating() then
                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                            while not quest:MsgIsGameInfoClickedPast() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e5a527 end
                            end
                            if not quest:IsActiveThreadTerminating() then goto LAB_00e5a5c1 end
                        end
                        goto FLOW_past_lab_00e5a5c1
                        ::LAB_00e5a5c1::
                        quest:SetStateBool("ExpressionTutorialShown", true)
                        goto LAB_00e5a5c8
                        ::FLOW_past_lab_00e5a5c1::
                        goto LAB_00e5b231
                    end
                    goto FLOW_past_lab_00e5b231
                    ::LAB_00e5b231::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00e5b19b
                    ::FLOW_past_lab_00e5b231::
                    if 0.0 < quest:GetHealth(resources:ScriptThing(beggarBully)) then
                        if not me:Speak(hero, "TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT_FAIL", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00e5b231 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5a527 end
                    end
                    ::FLOW_past_lab_00e5a527::
                    ::LAB_00e5a683::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                end
                ::LAB_00e5a6a9::
                quest:SetTimer(tauntTimer, quest:GetTimer(tauntTimer) + 5)
                goto LAB_00e5ac46
            end
            if not quest:GetStateBool("BullyHasTaunted") or 0 < quest:GetTimer(quest:GetStateInt("BeggarReplyTimer")) then goto LAB_00e5ac46 end
            if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
            quest:EntitySetFacingAngleTowardsThing(quest:GetThingWithScriptName("BeggarBully"), beggarBully)
            quest:SetStateBool("BullyHasTaunted", false)
            timerId = quest:RegisterTimer()
            local i_stk_1fc_1 = timerId
            quest:SetTimer(timerId, 2)
            repeat
                if quest:GetTimer(timerId) < 1 then break end
                if not quest:NewScriptFrame(me) then goto LAB_00e5b24b end
                local scratchValue4 = quest:MsgOnQuestFailed("SCRIPT_NAME_HERO")
                if scratchValue4 then
                    isActiveThreadTerminating = true
                end
                scratchValue20 = scratchValue21
                -- TODO(native): xStack_1e4 = xStack_1e4 | 8;
                -- TODO(native): MsgIsRegionUnloaded is not a ForgeFSE binding
                quest:MsgIsRegionUnloaded("SCRIPT_NAME_HERO")
                if scratchValue4 then
                    goto LAB_00e5a8ab
                else
                    scratchValue19 = scratchValue20 | 24
                    scratchValue21 = scratchValue19
                    if quest:MsgIsActionModeButtonPressed() then
                        scratchValue19 = scratchValue20 | 56
                        scratchValue21 = scratchValue19
                        if not quest:MsgIsTutorialClickedPast() then goto LAB_00e5a8ab end
                    end
                    predicateResult30 = false
                end
                goto FLOW_past_lab_00e5a8ab
                ::LAB_00e5a8ab::
                predicateResult30 = true
                ::FLOW_past_lab_00e5a8ab::
                if scratchValue19 & 32 ~= 0 then
                    scratchValue19 = scratchValue19 & 0xffffffdf
                    scratchValue21 = scratchValue19
                end
                if scratchValue19 & 16 ~= 0 then
                    scratchValue19 = scratchValue19 & 0xffffffef
                    scratchValue21 = scratchValue19
                end
                if scratchValue19 & 8 ~= 0 then
                    -- TODO(native): xStack_1e4 = uVar13 & 0xfffffff7;
                end
                if predicateResult30 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5b24b end
                    quest:SetStateBool("BeggarHit", true)
                end
            until isActiveThreadTerminating
            if quest:IsActiveThreadTerminating() then goto LAB_00e5b24b end
            if not isActiveThreadTerminating then
                local switch = math.random(0, 32767) % 5
                repeat
                    if switch == 0 then
                        me:PlayAnimation("ST_OPINION_DISAPPROVAL_DISMISSING_HAND_SWIPE", false, false, false, true, true, false, false)
                        break
                    elseif switch == 1 then
                        me:PlayAnimation("ST_OPINION_DISAPPROVAL_CALLING_OVER_FOR_FIGHT", false, false, false, true, true, false, false)
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
                conversationId = quest:AddNewConversation(nil --[[missing]], false, false)
                quest:AddPersonToConversation(conversationId, nil --[[missing]])
                scratchValue20 = math.random(0, 32767) & 0x80000003
                scratchValue7 = scratchValue20 == 0
                if scratchValue20 < 0 then
                    scratchValue7 = (scratchValue20 - 1 | 0xfffffffc) == 0xffffffff
                end
                if scratchValue7 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5b24b end
                    quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_01", nil --[[missing]], nil --[[missing]], false)
                else
                    scratchValue20 = math.random(0, 32767) & 0x80000003
                    scratchValue8 = scratchValue20 == 0
                    if scratchValue20 < 0 then
                        scratchValue8 = (scratchValue20 - 1 | 0xfffffffc) == 0xffffffff
                    end
                    if scratchValue8 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5b24b end
                        quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_02", nil --[[missing]], nil --[[missing]], false)
                    else
                        scratchValue20 = math.random(0, 32767) & 0x80000003
                        scratchValue9 = scratchValue20 == 0
                        if scratchValue20 < 0 then
                            scratchValue9 = (scratchValue20 - 1 | 0xfffffffc) == 0xffffffff
                        end
                        if scratchValue9 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e5b24b end
                            goto FLOW_hoist_lab_00e5b24b_1
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00e5b24b end
                            quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_04", nil --[[missing]], nil --[[missing]], false)
                        end
                    end
                end
            end
            goto FLOW_past_lab_00e5b24b
            ::LAB_00e5b24b::
            quest:DeregisterTimer(i_stk_1fc_1)
            goto LAB_00e5b19b
            ::FLOW_hoist_lab_00e5b24b_1::
            quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_03", nil --[[missing]], nil --[[missing]], false)
            ::FLOW_past_lab_00e5b24b::
            quest:DeregisterTimer(i_stk_1fc_1)
        end
        ::LAB_00e5ac46::
        local scratchValue22 = me:MsgExpressionPerformedTo()
        if scratchValue22 == nil then goto LAB_00e5b163 end
        if quest:IsActiveThreadTerminating() then goto LAB_00e5b29c end
        goto FLOW_past_lab_00e5b29c
        ::LAB_00e5b29c::
        goto LAB_00e5b19b
        ::FLOW_past_lab_00e5b29c::
        quest:EntitySetFacingAngleTowardsThing(hero, nil --[[missing]])
        timerId = quest:RegisterTimer()
        i_stk_1fc_2 = timerId
        quest:SetTimer(timerId, 2)
        while true do
            if not (not isActiveThreadTerminating and 0 < quest:GetTimer(timerId)) then break end
            if not quest:NewScriptFrame(me) then goto LAB_00e5b276 end
            -- TODO(native): bVar5 = (**(*me + 0x6c))(me,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
            if nil then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); goto LAB_00e5b19b end
                isActiveThreadTerminating = true
            end
            scratchValue20 = scratchValue21
            -- TODO(native): xStack_1e4 = xStack_1e4 | 0x40;
            -- TODO(native): bVar5 = (**(*me + 0x54))(me,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
            if nil then
                goto LAB_00e5addd
            else
                scratchValue19 = scratchValue20 | 192
                scratchValue21 = scratchValue19
                -- TODO(native): bVar5 = (**(*me + 0xa8))(me,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                if nil then
                    scratchValue19 = scratchValue20 | 448
                    scratchValue21 = scratchValue19
                    -- TODO(native): bVar5 = (**(*me + 0xa4))(me,0xe,"SCRIPT_NAME_HERO")
    --[[unresolved native value]]
                    if not nil then goto LAB_00e5addd end
                end
                predicateResult31 = false
            end
            goto FLOW_past_lab_00e5addd
            ::LAB_00e5addd::
            predicateResult31 = true
            ::FLOW_past_lab_00e5addd::
            if scratchValue19 & 256 ~= 0 then
                scratchValue19 = scratchValue19 & 0xfffffeff
                scratchValue21 = scratchValue19
            end
            if scratchValue19 & 128 ~= 0 then
                scratchValue19 = scratchValue19 & 0xffffff7f
                scratchValue21 = scratchValue19
            end
            if scratchValue19 & 64 ~= 0 then
                -- TODO(native): xStack_1e4 = uVar13 & 0xffffffbf;
            end
            if predicateResult31 then
                if quest:IsActiveThreadTerminating() then goto LAB_00e5b276 end
                goto FLOW_hoist_lab_00e5b276_1
            end
            goto FLOW_past_lab_00e5b276
            ::LAB_00e5b276::
            quest:DeregisterTimer(timerId)
            goto LAB_00e5b19b
            ::FLOW_hoist_lab_00e5b276_1::
            quest:SetStateBool("BeggarHit", true)
            ::FLOW_past_lab_00e5b276::
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e5b293 end
        goto FLOW_past_lab_00e5b293
        ::LAB_00e5b293::
        quest:DeregisterTimer(i_stk_1fc_2)
        goto LAB_00e5b29c
        ::FLOW_past_lab_00e5b293::
        if isActiveThreadTerminating then goto FLOW_native_label_2 end
        if scratchValue22 == nil then goto FLOW_native_label_2 end
        -- TODO(native): p0 = *xStack_1e8
        p0 = nil --[[unresolved native value]]
        -- TODO(native): iVar9 = CBasicString<char>::Compare(p0,"EXPRESSION_BELCH");
        isActiveThreadTerminating = false
        if timerId ~= 0 then
            -- TODO(native): iVar9 = CBasicString<char>::Compare(p0,"EXPRESSION_FART");
            isActiveThreadTerminating = false
            if timerId ~= 0 then goto FLOW_native_label_2 end
            if quest:IsActiveThreadTerminating() then goto LAB_00e5b293 end
            conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            if beggarBully == 1 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_FIRST_BATTLE_CRY_REPLY", me, hero, false)
            elseif beggarBully == 2 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_SECOND_BATTLE_CRY_REPLY", me, hero, false)
            else
                quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_THIRD_BATTLE_CRY_REPLY", me, hero, false)
            end
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00e5b293 end
            quest:SetStateInt("BelchedAtBeggar", quest:GetStateInt("BelchedAtBeggar") + 1)
            conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            switch4 = quest:GetStateInt("BelchedAtBeggar")
            if not (switch4 == 1 or switch4 == 2 or switch4 == 3 or switch4 == 4) then
                switch4 = 0x7ffffffe
            end
            repeat
                if switch4 == 1 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_FIRST_BELCH_REPLY", me, hero, false)
                    break
                end
                if switch4 == 2 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_SECOND_BELCH_REPLY", me, hero, false)
                    break
                end
                if switch4 == 3 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_THIRD_BELCH_REPLY", me, hero, false)
                    break
                end
                if switch4 == 4 then
                    quest:SetStateBool("BeggarLeft", true)
                    quest:SetStateBool("BelchedAtBully", true)
                    switch4 = 0x7ffffffe
                end
                if switch4 == 0x7ffffffe then goto FLOW_native_label_2 end
            until true
        end
        ::FLOW_native_label_2::
        quest:DeregisterTimer(i_stk_1fc_2)
        ::LAB_00e5b163::
        beggarHit = quest:GetStateBool("BeggarHit")
    end
    if not quest:IsActiveThreadTerminating() and not resources:ScriptThing(beggarBully):IsNull() then
        resources:PrepareResource(beggarBully)
    end
    ::LAB_00e5b19b::
    resources:ReleaseResource(beggarBully)
end

-- LookoutPointBeggar.Init (retail 0x00e58d10)
function Init(quest, me)
end

-- LookoutPointBeggar.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- LookoutPointBeggar.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

