-- Readable native conversion: AppleGirl. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- AppleGirl.Main (retail 0x00d3d150)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult6, predicateResult, ctr_64, scratchValue, conversationId, conversationId2
    local conversationId3, conversationId4, conversationId5, conversationId6, questionAnswer
    local conversationId7, switch, movie, scratchValue8
    if not me:AcquireControl(4) then return end
    if quest:IsActiveThreadTerminating() then me:ReleaseControl(); return end
    me:SetFriendsWithEverythingFlag(me)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntityAttachToVillage(me, quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE"))
    state:SetInt("AppleMode", 0)
    scratchValue = quest:RegisterTimer()
    scratchValue8 = scratchValue
    quest:SetTimer(scratchValue, 15)
    state:SetInt("CurrentApples", 0)
    state:SetBool("ChildAppleMode", false)
    quest:SetThingHasInformation(me, false, true, false)
    while state:GetInt("AppleMode") == 0 do
        if not quest:NewScriptFrame(me) then goto LAB_00d3e1b6 end
        if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(scratchValue) < 1 then
            scratchValue = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(scratchValue, hero)
            quest:AddLineToConversation(scratchValue, "TEXT_QST_028_APPLEGIRL_HELP", me, hero, false)
            quest:SetTimer(scratchValue8, 15)
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            movie = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            if not state:GetBool("HaveChatted") then
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e06c end
                state:SetBool("HaveChatted", true)
                if 0.0 < quest:GetHealth(me) then
                    scratchValue = 0
                    me:Speak(hero, "TEXT_QST_028_APPLEGIRL_CHAT", 0, false, true, false)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d3d948 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d3e06c end
                end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPLEGIRL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e087 end
                quest:GiveHeroYesNoQuestion("TEXT_QST_028_APPLEGIRL_QUESTION_AGAIN", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            end
            questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer < 0 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    quest:DeregisterTimer(scratchValue8)
                    return
                end
                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e087 end
            if questionAnswer == 1 then
                state:SetInt("AppleMode", 1)
                if quest:IsQuestActive("Q_GuildTrainingPreMelee") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d3e087 end
                    state:SetBool("ChildAppleMode", true)
                end
            elseif 0.0 < quest:GetHealth(me) then
                scratchValue = 0
                me:Speak(hero, "TEXT_QST_028_APPLEGIRL_IMPLORE", 0, false, true, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00d3d948 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e06c end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
    end
    if not quest:IsActiveThreadTerminating() then
        while state:GetInt("AppleMode") == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d3e1b6 end
            if state:GetBool("ChildAppleMode") then
                if quest:IsQuestActive("Q_GuildTrainingPreMelee") then
                    predicateResult6 = false
                    goto FLOW_after_lab_00d3d987
                end
                predicateResult6 = true
            else
                predicateResult6 = false
            end
            ::FLOW_after_lab_00d3d987::
            if predicateResult6 then
                quest:RemoveThing(me, false, true)
            end
            if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(scratchValue8) < 1 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                if state:GetInt("CurrentApples") == 0 then
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, hero)
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPLEGIRL_ANY_APPLES", me, hero, false)
                else
                    conversationId2 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId2, hero)
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_APPLEGIRL_MORE_APPLES", me, hero, false)
                end
                quest:SetTimer(scratchValue8, 15)
            end
            if not me:IsTalkedToByHero() then goto FLOW_native_label_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            ctr_64 = 0
            while true do
                if not (quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", hero) and (state:GetInt("CurrentApples") + ctr_64 < 4)) then break end
                if not quest:NewScriptFrame(me) then goto LAB_00d3e1b6 end
                ctr_64 = ctr_64 + 1
                quest:TakeObjectFromHero("OBJECT_APPLE_RED_01")
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            if ctr_64 == 1 then
                conversationId3 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId3, hero)
                quest:AddLineToConversation(conversationId3, "TEXT_QST_028_APPLEGIRL_ONE_MORE_APPLE", me, hero, false)
            elseif ctr_64 == nil then
                conversationId4 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId4, hero)
                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPLEGIRL_NO_MORE_APPLES", me, hero, false)
            else
                conversationId5 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId5, hero)
                quest:AddLineToConversation(conversationId5, "TEXT_QST_028_APPLEGIRL_MANY_MORE_APPLES", me, hero, false)
            end
            quest:Pause(1.0)
            state:SetInt("CurrentApples", state:GetInt("CurrentApples") + ctr_64)
            if ctr_64 == nil then goto FLOW_native_label_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            conversationId7 = quest:AddNewConversation(me, false, false)
            switch = state:GetInt("CurrentApples")
            repeat
                if switch == 1 then
                    quest:AddPersonToConversation(conversationId7, hero)
                    quest:AddLineToConversation(conversationId7, "TEXT_QST_028_APPLEGIRL_THREE_NEEDED", me, hero, false)
                    break
                elseif switch == 2 then
                    quest:AddPersonToConversation(conversationId7, hero)
                    quest:AddLineToConversation(conversationId7, "TEXT_QST_028_APPLEGIRL_TWO_NEEDED", me, hero, false)
                    break
                elseif switch == 3 then
                    quest:AddPersonToConversation(conversationId7, hero)
                    quest:AddLineToConversation(conversationId7, "TEXT_QST_028_APPLEGIRL_ONE_NEEDED", me, hero, false)
                    break
                elseif switch == 4 then
                    movie = resources:StartMovie("")
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    if 0.0 < quest:GetHealth(me) then
                        me:Speak(hero, "TEXT_QST_028_APPLEGIRL_THANKS", 0, false, true, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d3e06c end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d3e087 end
                    end
                    quest:GiveHeroObject("OBJECT_PIE_BLUEBERRY_01", -1)
                    state:SetInt("AppleMode", 2)
                    quest:ClearThingHasInformation(me)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto FLOW_native_label_1
                else
                    goto FLOW_native_label_1
                end
            until true
            quest:Pause(1.0)
            ::FLOW_native_label_1::
        end
        while not quest:IsActiveThreadTerminating() do
            if state:GetBool("ChildAppleMode") then
                if quest:IsQuestActive("Q_GuildTrainingPreMelee") then
                    predicateResult = false
                    goto FLOW_after_lab_00d3e0a0
                end
                predicateResult = true
            else
                predicateResult = false
            end
            ::FLOW_after_lab_00d3e0a0::
            if predicateResult then
                quest:RemoveThing(me, false, true)
            end
            if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(scratchValue8) < 1 then
                if quest:IsActiveThreadTerminating() then break end
                conversationId6 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId6, hero)
                quest:AddLineToConversation(conversationId6, "TEXT_QST_028_APPLEGIRL_THANKS_AGAIN", me, hero, false)
                quest:SetTimer(scratchValue8, 15)
            end
            quest:NewScriptFrame(me)
        end
    end
    ::LAB_00d3e1b6::
    quest:DeregisterTimer(scratchValue8)
    ::LAB_00d3e1bf::
    me:ReleaseControl()
    do return end
    ::LAB_00d3d948::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    quest:DeregisterTimer(scratchValue8)
    goto LAB_00d3e1bf
    ::LAB_00d3e06c::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    goto LAB_00d3e1b6
    ::LAB_00d3e087::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(ctr_64)
    goto LAB_00d3e1b6
end

-- AppleGirl.Init (retail 0x00d3d110)
function Init(quest, me)
    state:SetBool("HaveChatted", false)
end

-- AppleGirl.OnPersist (retail 0x00d44650)
function OnPersist(quest, context)
end

-- AppleGirl.OnPredicateFail (retail 0x00d3d120)
function OnPredicateFail(quest, me)
end

