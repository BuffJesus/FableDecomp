-- Readable native conversion: LookoutPointBeggar. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local self0Xc

-- LookoutPointBeggar.Main (retail 0x00e58d40)
function Main(quest, me)
    local resources = quest:RetailResources()
    local isActiveThreadTerminating, predicateResult, predicateResult31, scratchValue6
    local scratchValue7, scratchValue8, predicateResult32, battleCryReplyCount, beggarHit, getTimer
    local timerId, timerId2, initialReplySelected, switch4, scriptThing, line, this_00, hero5
    local retailRandModulo, conversationId, movie
    local tauntTimer = quest:GetStateInt("TauntTimer")
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    conversationId = self0Xc
    quest:SetIsPushableByHero(me, false)
    local beggarBully = quest:GetThingWithScriptName("BeggarBully")
    initialReplySelected = false
    battleCryReplyCount = 0
    quest:SetThingHasInformation(me, false, false, false)
    quest:EntitySetAsKillable(me, false, true)
    beggarHit = quest:GetStateBool("BeggarHit")
    isActiveThreadTerminating = false
    while ((not beggarHit and (not quest:GetStateBool("BullyHit"))) and (not quest:GetStateBool("BeggarLeft"))) and not quest:GetStateBool("BullyLeft") do
        if not quest:NewScriptFrame(me) then goto LAB_00e5b19b end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
            isActiveThreadTerminating = true
        end
        if me:MsgIsHitByHero() then
            goto LAB_00e58fce
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e58fce end
            predicateResult = false
        end
        goto FLOW_past_lab_00e58fce
        ::LAB_00e58fce::
        predicateResult = true
        ::FLOW_past_lab_00e58fce::
        if predicateResult then
            if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
            quest:SetStateBool("BeggarHit", true)
        else
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
                isActiveThreadTerminating = false
                if not initialReplySelected and not quest:GetStateBool("TaughtBattleCry") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
                    initialReplySelected = true
                    if quest:GetStateInt("BelchedAtBeggar") == 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
                        local movie2 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        scriptThing = resources:ScriptThing(resource)
                        if not (not (0.0 < quest:GetHealth(scriptThing))) then
                            line = "TEXT_QST_015_BEGGAR_FIRST_SPEAK"
                            timerId = quest:GetHero()
                            if not me:Speak(timerId, line, 0, false, true, false) then
                                goto FLOW_past_lab_00e596a9
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e596a9 end
                            end
                            if not quest:IsActiveThreadTerminating() then goto LAB_00e596a9 end
                            goto FLOW_past_lab_00e596a9
                        end
                        ::LAB_00e596a9::
                        quest:GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                        while timerId < 0 do
                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e596a9 end
                            timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if not quest:IsActiveThreadTerminating() then
                            local predicateResult4 = quest:IsActiveThreadTerminating()
                            if timerId == 1 then
                                if not predicateResult4 then
                                    getTimer = resources:ScriptThing(resource)
                                    if 0.0 < quest:GetHealth(getTimer) then
                                        line = "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY"
                                        timerId = quest:GetHero()
                                        if not me:Speak(timerId, line, 0, false, true, false) then
                                            goto FLOW_past_lab_00e596a9
                                        end
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e596a9 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e596a9 end
                                    end
                                    quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
                                    if not (quest:GetStateBool("ExpressionTutorialShown")) then
                                        goto FLOW_past_lab_00e599d8
                                    end
                                    ::LAB_00e599d8::
                                    quest:SetStateBool("TaughtBattleCry", true)
                                    goto LAB_00e59a93
                                    ::FLOW_past_lab_00e599d8::
                                    if quest:IsXbox() then
                                        if not quest:IsActiveThreadTerminating() then
                                            quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                            while not quest:MsgIsGameInfoClickedPast() do
                                                if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e596a9 end
                                            end
                                            if not quest:IsActiveThreadTerminating() then goto LAB_00e599d1 end
                                        end
                                        goto FLOW_past_lab_00e596a9
                                    end
                                    if not quest:IsActiveThreadTerminating() then
                                        quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                                        while not quest:MsgIsGameInfoClickedPast() do
                                            if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e596a9 end
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
                                getTimer = resources:ScriptThing(resource)
                                if 0.0 < quest:GetHealth(getTimer) then
                                    line = "TEXT_QST_015_BEGGAR_TEACH_REFUSED"
                                    timerId = quest:GetHero()
                                    if not me:Speak(timerId, line, 0, false, true, false) then
                                        goto FLOW_past_lab_00e596a9
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto FLOW_past_lab_00e596a9 end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e596a9 end
                                end
                            else
                                goto FLOW_past_lab_00e59a93
                            end
                            ::LAB_00e59a93::
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie2
                            goto LAB_00e59aa3
                            ::FLOW_past_lab_00e59a93::
                        end
                        ::FLOW_past_lab_00e596a9::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        goto LAB_00e5b19b
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
                    movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    scriptThing = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scriptThing) then
                        line = "TEXT_QST_015_BEGGAR_FIRST_SPEAK_BELCH"
                        timerId = quest:GetHero()
                        if not me:Speak(timerId, line, 0, false, true, false) then
                            goto LAB_00e5b1b8
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e5b1b8 end
                        end
                        if not quest:IsActiveThreadTerminating() then goto FLOW_past_lab_00e59429 end
                    else
                        goto FLOW_past_lab_00e59429
                    end
                    ::LAB_00e59429::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00e5b19b
                    ::FLOW_past_lab_00e59429::
                    quest:GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                    timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                    while timerId < 0 do
                        if not quest:NewScriptFrame(me) then goto LAB_00e5b1b8 end
                        timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e59429 end
                    if timerId == 1 then
                        do
                            getTimer = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(getTimer) then
                                line = "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY"
                                timerId = quest:GetHero()
                                if not me:Speak(timerId, line, 0, false, true, false) then
                                    goto LAB_00e59429
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e59429 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e5b1b8 end
                            end
                            quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
                            if not (quest:GetStateBool("ExpressionTutorialShown")) then
                                goto FLOW_past_lab_00e594c9
                            end
                            ::LAB_00e594c9::
                            quest:SetStateBool("TaughtBattleCry", true)
                            quest:PauseAllNonScriptedEntities(false)
                            this_00 = movie
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
                        end
                        goto LAB_00e5b1b8
                    end
                    goto FLOW_past_lab_00e5b1b8
                    ::LAB_00e5b1b8::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00e5b19b
                    ::FLOW_past_lab_00e5b1b8::
                    getTimer = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(getTimer) then
                        line = "TEXT_QST_015_BEGGAR_TEACH_REFUSED"
                        timerId = quest:GetHero()
                        if not me:Speak(timerId, line, 0, false, true, false) then
                            goto LAB_00e5b1b8
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e5b1b8 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e59429 end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie
                    ::LAB_00e59aa3::
                    resources:DestroyMovie(this_00)
                    if not quest:GetStateBool("QuestCardGiven") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
                        conversationId = quest:GetActiveQuestName()
                        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_BEGGAR_AND_CHILD", conversationId, false)
                        quest:SetStateBool("QuestCardGiven", true)
                    end
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
                    if quest:GetStateInt("BelchedAtBeggar") ~= 0 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
                        local movie4 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if not quest:GetStateBool("TaughtBattleCry") then
                            if not quest:IsActiveThreadTerminating() then
                                getTimer = resources:ScriptThing(resource)
                                if 0.0 < quest:GetHealth(getTimer) then
                                    line = "TEXT_QST_015_BEGGAR_REPEAT_BELCH_RETURN"
                                    timerId = quest:GetHero()
                                    if not me:Speak(timerId, line, 0, false, true, false) then
                                        goto LAB_00e5b1e6
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00e5b1e6 end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e5b1fc end
                                end
                                quest:GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                                timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                                while timerId < 0 do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e5b1e6 end
                                    timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    local predicateResult17 = quest:IsActiveThreadTerminating()
                                    if timerId == 1 then
                                        if not predicateResult17 then
                                            getTimer = resources:ScriptThing(resource)
                                            if 0.0 < quest:GetHealth(getTimer) then
                                                line = "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY"
                                                timerId = quest:GetHero()
                                                if not me:Speak(timerId, line, 0, false, true, false) then
                                                    goto LAB_00e5b1e6
                                                end
                                                while me:IsPerformingScriptTask() do
                                                    if not quest:NewScriptFrame(me) then goto LAB_00e5b1e6 end
                                                end
                                                if quest:IsActiveThreadTerminating() then goto LAB_00e5b1e6 end
                                            end
                                            quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
                                            if not (quest:GetStateBool("ExpressionTutorialShown")) then
                                                goto FLOW_past_lab_00e5a06d
                                            end
                                            ::LAB_00e5a06d::
                                            quest:SetStateBool("TaughtBattleCry", true)
                                            goto LAB_00e5a126
                                            ::FLOW_past_lab_00e5a06d::
                                            if quest:IsXbox() then
                                                if not quest:IsActiveThreadTerminating() then
                                                    quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS")
                                                    while not quest:MsgIsGameInfoClickedPast() do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00e5b1e6 end
                                                    end
                                                    if not quest:IsActiveThreadTerminating() then goto LAB_00e5a066 end
                                                end
                                                goto LAB_00e5b1e6
                                            end
                                            if not quest:IsActiveThreadTerminating() then
                                                quest:DisplayGameInfo("TEXT_QST_015_TUTORIAL_EXPRESSIONS_PC")
                                                while not quest:MsgIsGameInfoClickedPast() do
                                                    if not quest:NewScriptFrame(me) then goto LAB_00e5b1e6 end
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
                                        getTimer = resources:ScriptThing(resource)
                                        if 0.0 < quest:GetHealth(getTimer) then
                                            line = "TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT_FAIL_BELCH"
                                            timerId = quest:GetHero()
                                            if not me:Speak(timerId, line, 0, false, true, false) then
                                                goto LAB_00e5b1e6
                                            end
                                            while me:IsPerformingScriptTask() do
                                                if not quest:NewScriptFrame(me) then goto LAB_00e5b1e6 end
                                            end
                                            if quest:IsActiveThreadTerminating() then goto LAB_00e5b1e6 end
                                        end
                                    else
                                        goto FLOW_past_lab_00e5a126
                                    end
                                    ::LAB_00e5a126::
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    goto LAB_00e5a6a9
                                    ::FLOW_past_lab_00e5a126::
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00e5b19b
                            end
                            ::LAB_00e5b1fc::
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                        else
                            if not quest:IsActiveThreadTerminating() then
                                scriptThing = resources:ScriptThing(resource)
                                if 0.0 < quest:GetHealth(scriptThing) then
                                    line = "TEXT_QST_015_BEGGAR_REPEAT_BATTLE_CRY"
                                    timerId = quest:GetHero()
                                    if not me:Speak(timerId, line, 0, false, true, false) then
                                        goto LAB_00e5b1e6
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00e5b1e6 end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e5b1e6 end
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00e5a6a9
                            end
                            goto LAB_00e5b1e6
                        end
                        goto FLOW_past_lab_00e5b1e6
                        ::LAB_00e5b1e6::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        ::FLOW_past_lab_00e5b1e6::
                        goto LAB_00e5b19b
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
                    local movie3 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if not quest:GetStateBool("TaughtBattleCry") then
                        if not (quest:IsActiveThreadTerminating()) then
                            goto FLOW_past_lab_00e5a527
                        end
                        ::LAB_00e5a527::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        goto LAB_00e5b19b
                        ::FLOW_past_lab_00e5a527::
                        quest:GiveHeroYesNoQuestion("TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                        while timerId < 0 do
                            if not quest:NewScriptFrame(me) then goto LAB_00e5b231 end
                            timerId = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5a527 end
                        if timerId == 1 then
                            do
                                getTimer = resources:ScriptThing(resource)
                                if 0.0 < quest:GetHealth(getTimer) then
                                    line = "TEXT_QST_015_BEGGAR_TEACH_BATTLE_CRY"
                                    timerId = quest:GetHero()
                                    if not me:Speak(timerId, line, 0, false, true, false) then
                                        goto LAB_00e5a527
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00e5a527 end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e5b231 end
                                end
                                quest:SetPreferredQuickAccessItem("OBJECT_EXPRESSION_FART_DUMMY", 3, 10)
                                if not (quest:GetStateBool("ExpressionTutorialShown")) then
                                    goto FLOW_past_lab_00e5a5c8
                                end
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
                            end
                            goto LAB_00e5b231
                        end
                        goto FLOW_past_lab_00e5b231
                        ::LAB_00e5b231::
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        goto LAB_00e5b19b
                        ::FLOW_past_lab_00e5b231::
                        getTimer = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(getTimer) then
                            line = "TEXT_QST_015_BEGGAR_TEACH_OFFER_REPEAT_FAIL"
                            timerId = quest:GetHero()
                            if not me:Speak(timerId, line, 0, false, true, false) then
                                goto LAB_00e5b231
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e5b231 end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00e5a527 end
                        end
                    else
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            goto LAB_00e5b19b
                        end
                        getTimer = resources:ScriptThing(resource)
                        if quest:GetHealth(getTimer) <= 0.0 then goto LAB_00e5a683 end
                        line = "TEXT_QST_015_BEGGAR_REPEAT_BATTLE_CRY"
                        timerId = quest:GetHero()
                        if not me:Speak(timerId, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            goto LAB_00e5b19b
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00e5b19b
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            goto LAB_00e5b19b
                        end
                    end
                    ::LAB_00e5a683::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                end
                ::LAB_00e5a6a9::
                timerId = quest:GetTimer(tauntTimer)
                quest:SetTimer(tauntTimer, timerId + 5)
                goto LAB_00e5ac46
            end
            if not quest:GetStateBool("BullyHasTaunted") or 0 < quest:GetTimer(quest:GetStateInt("BeggarReplyTimer")) then goto LAB_00e5ac46 end
            if quest:IsActiveThreadTerminating() then goto LAB_00e5b19b end
            conversationId = quest:GetThingWithScriptName("BeggarBully")
            quest:EntitySetFacingAngleTowardsThing(me, conversationId)
            quest:SetStateBool("BullyHasTaunted", false)
            timerId = quest:RegisterTimer()
            timerId2 = timerId
            quest:SetTimer(timerId, 2)
            repeat
                if quest:GetTimer(timerId) < 1 then break end
                if not quest:NewScriptFrame(me) then goto LAB_00e5b24b end
                if me:IsTalkedToByHero() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5b24b end
                    isActiveThreadTerminating = true
                end
                if me:MsgIsHitByHero() then
                    goto LAB_00e5a8ab
                else
                    if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e5a8ab end
                    predicateResult31 = false
                end
                goto FLOW_past_lab_00e5a8ab
                ::LAB_00e5a8ab::
                predicateResult31 = true
                ::FLOW_past_lab_00e5a8ab::
                if predicateResult31 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e5b24b end
                    quest:SetStateBool("BeggarHit", true)
                end
            until isActiveThreadTerminating
            if not (quest:IsActiveThreadTerminating()) then
                if not isActiveThreadTerminating then
                    local switch = quest:RetailRandModulo(32768) % 5
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
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, beggarBully)
                    retailRandModulo = quest:RetailRandModulo(32768) & 0x80000003
                    scratchValue6 = retailRandModulo == 0
                    if retailRandModulo < 0 then
                        scratchValue6 = (retailRandModulo - 1 | 0xfffffffc) == 0xffffffff
                    end
                    if scratchValue6 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e5b24b end
                        quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_01", me, beggarBully, false)
                    else
                        retailRandModulo = quest:RetailRandModulo(32768) & 0x80000003
                        scratchValue7 = retailRandModulo == 0
                        if retailRandModulo < 0 then
                            scratchValue7 = (retailRandModulo - 1 | 0xfffffffc) == 0xffffffff
                        end
                        if scratchValue7 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00e5b24b end
                            quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_02", me, beggarBully, false)
                        else
                            retailRandModulo = quest:RetailRandModulo(32768) & 0x80000003
                            scratchValue8 = retailRandModulo == 0
                            if retailRandModulo < 0 then
                                scratchValue8 = (retailRandModulo - 1 | 0xfffffffc) == 0xffffffff
                            end
                            if scratchValue8 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e5b24b end
                                goto FLOW_hoist_lab_00e5b24b_1
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00e5b24b end
                                quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_04", me, beggarBully, false)
                            end
                        end
                    end
                end
                goto FLOW_past_lab_00e5b24b
            end
            ::LAB_00e5b24b::
            quest:DeregisterTimer(timerId2)
            goto LAB_00e5b19b
            ::FLOW_hoist_lab_00e5b24b_1::
            quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_TAUNT_REPLY_03", me, beggarBully, false)
            ::FLOW_past_lab_00e5b24b::
            quest:DeregisterTimer(timerId2)
        end
        ::LAB_00e5ac46::
        local scratchValue16 = me:MsgExpressionPerformedTo()
        if scratchValue16 == nil then goto LAB_00e5b163 end
        if not (quest:IsActiveThreadTerminating()) then
            goto FLOW_past_lab_00e5b29c
        end
        ::LAB_00e5b29c::
        goto LAB_00e5b19b
        ::FLOW_past_lab_00e5b29c::
        conversationId = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(me, conversationId)
        timerId = quest:RegisterTimer()
        timerId2 = timerId
        quest:SetTimer(timerId, 2)
        while true do
            if not (not isActiveThreadTerminating and 0 < quest:GetTimer(timerId)) then break end
            if not quest:NewScriptFrame(me) then goto LAB_00e5b276 end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); goto LAB_00e5b19b end
                isActiveThreadTerminating = true
            end
            if me:MsgIsHitByHero() then
                goto LAB_00e5addd
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e5addd end
                predicateResult32 = false
            end
            goto FLOW_past_lab_00e5addd
            ::LAB_00e5addd::
            predicateResult32 = true
            ::FLOW_past_lab_00e5addd::
            if predicateResult32 then
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
        if not (quest:IsActiveThreadTerminating()) then
            goto FLOW_past_lab_00e5b293
        end
        ::LAB_00e5b293::
        quest:DeregisterTimer(timerId2)
        goto LAB_00e5b29c
        ::FLOW_past_lab_00e5b293::
        if isActiveThreadTerminating then goto LAB_00e5b14b end
        if quest:IsActiveThreadTerminating() then goto LAB_00e5b293 end
        if scratchValue16 ~= nil then
            isActiveThreadTerminating = false
            if scratchValue16 ~= "EXPRESSION_BELCH" then
                isActiveThreadTerminating = false
                if scratchValue16 ~= "EXPRESSION_FART" then goto LAB_00e5b14b end
                if quest:IsActiveThreadTerminating() then goto LAB_00e5b293 end
                battleCryReplyCount = (battleCryReplyCount + 1) & 0xffffffff
                conversationId = quest:AddNewConversation(me, false, false)
                local hero = quest:GetHero()
                quest:AddPersonToConversation(conversationId, hero)
                if battleCryReplyCount == 1 then
                    local hero2 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_FIRST_BATTLE_CRY_REPLY", me, hero2, false)
                elseif battleCryReplyCount == 2 then
                    local hero3 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_SECOND_BATTLE_CRY_REPLY", me, hero3, false)
                else
                    local hero4 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_THIRD_BATTLE_CRY_REPLY", me, hero4, false)
                end
                goto LAB_00e5b14b
            end
        else
            do
                goto LAB_00e5b14b
            end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e5b293 end
        quest:SetStateInt("BelchedAtBeggar", quest:GetStateInt("BelchedAtBeggar") + 1)
        conversationId = quest:AddNewConversation(me, false, false)
        hero5 = quest:GetHero()
        quest:AddPersonToConversation(conversationId, hero5)
        switch4 = quest:GetStateInt("BelchedAtBeggar")
        if not (switch4 == 1 or switch4 == 2 or switch4 == 3 or switch4 == 4) then
            switch4 = 0x7ffffffe
        end
        repeat
            if switch4 == 1 then
                local hero6 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_FIRST_BELCH_REPLY", me, hero6, false)
                break
            end
            if switch4 == 2 then
                local hero7 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_SECOND_BELCH_REPLY", me, hero7, false)
                break
            end
            if switch4 == 3 then
                local hero8 = quest:GetHero()
                quest:AddLineToConversation(conversationId, "TEXT_QST_015_BEGGAR_THIRD_BELCH_REPLY", me, hero8, false)
                break
            end
            if switch4 == 4 then
                quest:SetStateBool("BeggarLeft", true)
                quest:SetStateBool("BelchedAtBully", true)
                switch4 = 0x7ffffffe
            end
            if switch4 == 0x7ffffffe then goto LAB_00e5b14b end
        until true
        ::LAB_00e5b14b::
        quest:DeregisterTimer(timerId2)
        ::LAB_00e5b163::
        beggarHit = quest:GetStateBool("BeggarHit")
    end
    if not quest:IsActiveThreadTerminating() and not resources:ScriptThing(resource):IsNull() then
        resources:PrepareResource(resource)
    end
    ::LAB_00e5b19b::
    resources:ReleaseResource(resource)
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

