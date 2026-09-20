-- Readable native conversion: AppleGirl. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local appleMode, currentApples, childAppleMode, haveChatted

-- AppleGirl.Main (retail 0x00d3d150)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult6, predicateResult, ctr_64, questionAnswer, conversationId8, switch, movie
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    me:SetFriendsWithEverythingFlag(true)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntityAttachToVillage(me, quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE"))
    appleMode = 0
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 15)
    currentApples = 0
    childAppleMode = false
    quest:SetThingHasInformation(me, false, true, false)
    while appleMode == 0 do
        if not quest:NewScriptFrame(me) then goto LAB_00d3e1b6 end
        if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1 then
            local conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPLEGIRL_HELP", me, hero, false)
            quest:SetTimer(timerId, 15)
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if not haveChatted then
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e06c end
                haveChatted = true
                local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 < fret_0 then
                    if not me:Speak(hero, "TEXT_QST_028_APPLEGIRL_CHAT", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d3d948 end
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
                if not quest:IsActiveThreadTerminating() then
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                else
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    do return end
                    questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e087 end
            if questionAnswer == 1 then
                appleMode = 1
                if quest:IsQuestActive("Q_GuildTrainingPreMelee") then
                    childAppleMode = true
                end
            else
                local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 < fret_00 then
                    if not me:Speak(hero, "TEXT_QST_028_APPLEGIRL_IMPLORE", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d3d948 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d3e06c end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
    end
    if not quest:IsActiveThreadTerminating() then
        while appleMode == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d3e1b6 end
            if not childAppleMode then goto LAB_00d3d987 end
            if quest:IsQuestActive("Q_GuildTrainingPreMelee") then goto LAB_00d3d987 end
            predicateResult6 = true
            goto FLOW_past_lab_00d3d987
            ::LAB_00d3d987::
            predicateResult6 = false
            ::FLOW_past_lab_00d3d987::
            if predicateResult6 then
                quest:RemoveThing(me, false, true)
            end
            if quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                if currentApples == 0 then
                    local conversationId2 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId2, hero)
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_APPLEGIRL_ANY_APPLES", me, hero, false)
                else
                    local conversationId3 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId3, hero)
                    quest:AddLineToConversation(conversationId3, "TEXT_QST_028_APPLEGIRL_MORE_APPLES", me, hero, false)
                end
                quest:SetTimer(timerId, 15)
            end
            if not me:IsTalkedToByHero() then goto FLOW_native_label_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            ctr_64 = 0
            while true do
                if not (quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", hero) and (currentApples + ctr_64 < 4)) then break end
                if not quest:NewScriptFrame(me) then goto LAB_00d3e1b6 end
                ctr_64 = ctr_64 + 1
                quest:TakeObjectFromHero("OBJECT_APPLE_RED_01")
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            if ctr_64 == 1 then
                local conversationId4 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId4, hero)
                quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPLEGIRL_ONE_MORE_APPLE", me, hero, false)
            elseif ctr_64 == 0 then
                local conversationId5 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId5, hero)
                quest:AddLineToConversation(conversationId5, "TEXT_QST_028_APPLEGIRL_NO_MORE_APPLES", me, hero, false)
            else
                local conversationId6 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId6, hero)
                quest:AddLineToConversation(conversationId6, "TEXT_QST_028_APPLEGIRL_MANY_MORE_APPLES", me, hero, false)
            end
            quest:Pause(1.0)
            currentApples = currentApples + ctr_64
            if ctr_64 == 0 then goto FLOW_native_label_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            conversationId8 = quest:AddNewConversation(me, false, false)
            switch = currentApples
            repeat
                if switch == 1 then
                    quest:AddPersonToConversation(conversationId8, hero)
                    quest:AddLineToConversation(conversationId8, "TEXT_QST_028_APPLEGIRL_THREE_NEEDED", me, hero, false)
                    break
                elseif switch == 2 then
                    quest:AddPersonToConversation(conversationId8, hero)
                    quest:AddLineToConversation(conversationId8, "TEXT_QST_028_APPLEGIRL_TWO_NEEDED", me, hero, false)
                    break
                elseif switch == 3 then
                    quest:AddPersonToConversation(conversationId8, hero)
                    quest:AddLineToConversation(conversationId8, "TEXT_QST_028_APPLEGIRL_ONE_NEEDED", me, hero, false)
                    break
                elseif switch == 4 then
                    movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local fret_01 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_01 then
                        if not me:Speak(hero, "TEXT_QST_028_APPLEGIRL_THANKS", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d3e06c end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d3e087 end
                    end
                    quest:GiveHeroObject("OBJECT_PIE_BLUEBERRY_01", -1)
                    appleMode = 2
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
            if not childAppleMode then goto LAB_00d3e0a0 end
            if quest:IsQuestActive("Q_GuildTrainingPreMelee") then goto LAB_00d3e0a0 end
            predicateResult = true
            goto FLOW_past_lab_00d3e0a0
            ::LAB_00d3e0a0::
            predicateResult = false
            ::FLOW_past_lab_00d3e0a0::
            if predicateResult then
                quest:RemoveThing(me, false, true)
            end
            if not (quest:IsDistanceBetweenThingsUnder(hero, me, 5.5) and quest:GetTimer(timerId) < 1) then
                quest:NewScriptFrame(me)
            else
                if quest:IsActiveThreadTerminating() then break end
                local conversationId7 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId7, hero)
                quest:AddLineToConversation(conversationId7, "TEXT_QST_028_APPLEGIRL_THANKS_AGAIN", me, hero, false)
                quest:SetTimer(timerId, 15)
                quest:NewScriptFrame(me)
            end
        end
    end
    ::LAB_00d3e1b6::
    quest:DeregisterTimer(timerId)
    ::LAB_00d3e1bf::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d3d948::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    quest:DeregisterTimer(timerId)
    goto LAB_00d3e1bf
    ::LAB_00d3e06c::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    goto LAB_00d3e1b6
    ::LAB_00d3e087::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    goto LAB_00d3e1b6
end

-- AppleGirl.Init (retail 0x00d3d110)
function Init(quest, me)
    haveChatted = false
end

-- AppleGirl.OnPersist (retail 0x00d44650)
function OnPersist(quest, me, context)
    quest:SetStateInt("AppleMode", quest:PersistTransferInt(context, "AppleMode", quest:GetStateInt("AppleMode") or 0))
    quest:SetStateInt("CurrentApples", quest:PersistTransferInt(context, "CurrentApples", quest:GetStateInt("CurrentApples") or 0))
end

-- AppleGirl.OnPredicateFail (retail 0x00d3d120)
function OnPredicateFail(quest, me)
end

