-- Readable native conversion: AppleGirl. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local appleMode_, currentApples_, childAppleMode, haveChatted

-- AppleGirl.Main (retail 0x00d3d150)
function Main(quest, me)
    local resources = quest:RetailResources()
    local ctr_64, questionAnswer, conversationId8, switch, movie
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
    local getNearestWithDefName = quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE")
    quest:EntityAttachToVillage(me, getNearestWithDefName)
    appleMode_ = 0
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 15)
    currentApples_ = 0
    childAppleMode = false
    quest:SetThingHasInformation(me, false, true, false)
    while appleMode_ == 0 do
        if not quest:NewScriptFrame(me) then goto LAB_00d3e1b6 end
        local hero2 = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(hero2, me, 5.5) and quest:GetTimer(timerId) < 1 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            local conversationId = quest:AddNewConversation(me, false, false)
            local hero3 = quest:GetHero()
            quest:AddPersonToConversation(conversationId, hero3)
            local hero4 = quest:GetHero()
            quest:AddLineToConversation(conversationId, "TEXT_QST_028_APPLEGIRL_HELP", me, hero4, false)
            quest:SetTimer(timerId, 15)
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            if not haveChatted then
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e06c end
                haveChatted = true
                local scratchValue = resources:ScriptThing(resource)
                local fret_0 = quest:GetHealth(scratchValue)
                if 0.0 < fret_0 then
                    local hero = quest:GetHero()
                    if not me:Speak(hero, "TEXT_QST_028_APPLEGIRL_CHAT", GROUP_SELECT_FIRST, false, true, false) then
                        goto LAB_00d3d948
                    end
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
                appleMode_ = 1
                if quest:IsQuestActive("Q_GuildTrainingPreMelee") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d3e087 end
                    childAppleMode = true
                end
            else
                local x_stk_c_1 = resources:ScriptThing(resource)
                local fret_00 = quest:GetHealth(x_stk_c_1)
                if 0.0 < fret_00 then
                    local hero5 = quest:GetHero()
                    if not me:Speak(hero5, "TEXT_QST_028_APPLEGIRL_IMPLORE", GROUP_SELECT_FIRST, false, true, false) then
                        goto LAB_00d3d948
                    end
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00d3d948 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d3e06c end
                end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
    end
    if not quest:IsActiveThreadTerminating() then
        while appleMode_ == 1 do
            if not quest:NewScriptFrame(me) then goto LAB_00d3e1b6 end
            if not ((not childAppleMode) or quest:IsQuestActive("Q_GuildTrainingPreMelee")) then
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                quest:RemoveThing(me, false, true)
            end
            local hero6 = quest:GetHero()
            if quest:IsDistanceBetweenThingsUnder(hero6, me, 5.5) and quest:GetTimer(timerId) < 1 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                if currentApples_ == 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                    local conversationId2 = quest:AddNewConversation(me, false, false)
                    local hero7 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId2, hero7)
                    local hero8 = quest:GetHero()
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_APPLEGIRL_ANY_APPLES", me, hero8, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                    local conversationId3 = quest:AddNewConversation(me, false, false)
                    local hero9 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId3, hero9)
                    local hero10 = quest:GetHero()
                    quest:AddLineToConversation(conversationId3, "TEXT_QST_028_APPLEGIRL_MORE_APPLES", me, hero10, false)
                end
                quest:SetTimer(timerId, 15)
            end
            if not (not me:IsTalkedToByHero()) then
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                ctr_64 = 0
                while true do
                    local hero11 = quest:GetHero()
                    if not (quest:IsObjectInThingsPossession("OBJECT_APPLE_RED_01", hero11) and (currentApples_ + ctr_64 < 4)) then break end
                    if not quest:NewScriptFrame(me) then goto LAB_00d3e1b6 end
                    ctr_64 = ctr_64 + 1
                    quest:TakeObjectFromHero("OBJECT_APPLE_RED_01")
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                if ctr_64 == 1 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                    local conversationId4 = quest:AddNewConversation(me, false, false)
                    local hero12 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId4, hero12)
                    local hero13 = quest:GetHero()
                    quest:AddLineToConversation(conversationId4, "TEXT_QST_028_APPLEGIRL_ONE_MORE_APPLE", me, hero13, false)
                elseif ctr_64 == 0 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                    local conversationId5 = quest:AddNewConversation(me, false, false)
                    local hero14 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId5, hero14)
                    local hero15 = quest:GetHero()
                    quest:AddLineToConversation(conversationId5, "TEXT_QST_028_APPLEGIRL_NO_MORE_APPLES", me, hero15, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                    local conversationId6 = quest:AddNewConversation(me, false, false)
                    local hero16 = quest:GetHero()
                    quest:AddPersonToConversation(conversationId6, hero16)
                    local hero17 = quest:GetHero()
                    quest:AddLineToConversation(conversationId6, "TEXT_QST_028_APPLEGIRL_MANY_MORE_APPLES", me, hero17, false)
                end
                quest:Pause(1.0)
                currentApples_ = currentApples_ + ctr_64
                if ctr_64 == 0 then goto FLOW_native_label_1 end
                if quest:IsActiveThreadTerminating() then goto LAB_00d3e1b6 end
                conversationId8 = quest:AddNewConversation(me, false, false)
                switch = currentApples_
                repeat
                    if switch == 1 then
                        local hero18 = quest:GetHero()
                        quest:AddPersonToConversation(conversationId8, hero18)
                        local hero19 = quest:GetHero()
                        quest:AddLineToConversation(conversationId8, "TEXT_QST_028_APPLEGIRL_THREE_NEEDED", me, hero19, false)
                        break
                    elseif switch == 2 then
                        local hero20 = quest:GetHero()
                        quest:AddPersonToConversation(conversationId8, hero20)
                        local hero21 = quest:GetHero()
                        quest:AddLineToConversation(conversationId8, "TEXT_QST_028_APPLEGIRL_TWO_NEEDED", me, hero21, false)
                        break
                    elseif switch == 3 then
                        local hero22 = quest:GetHero()
                        quest:AddPersonToConversation(conversationId8, hero22)
                        local hero23 = quest:GetHero()
                        quest:AddLineToConversation(conversationId8, "TEXT_QST_028_APPLEGIRL_ONE_NEEDED", me, hero23, false)
                        break
                    elseif switch == 4 then
                        movie = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        local x_stk_c_2 = resources:ScriptThing(resource)
                        local fret_01 = quest:GetHealth(x_stk_c_2)
                        if 0.0 < fret_01 then
                            local hero24 = quest:GetHero()
                            if not me:Speak(hero24, "TEXT_QST_028_APPLEGIRL_THANKS", GROUP_SELECT_FIRST, false, true, false) then
                                goto LAB_00d3e06c
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00d3e06c end
                            end
                            if quest:IsActiveThreadTerminating() then goto LAB_00d3e087 end
                        end
                        quest:GiveHeroObject("OBJECT_PIE_BLUEBERRY_01", -1, false)
                        appleMode_ = 2
                        quest:ClearThingHasInformation(me)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto FLOW_native_label_1
                    else
                        goto FLOW_native_label_1
                    end
                until true
                quest:Pause(1.0)
            end
            ::FLOW_native_label_1::
        end
        while not quest:IsActiveThreadTerminating() do
            if not ((not childAppleMode) or quest:IsQuestActive("Q_GuildTrainingPreMelee")) then
                if quest:IsActiveThreadTerminating() then break end
                quest:RemoveThing(me, false, true)
            end
            local hero25 = quest:GetHero()
            if not (quest:IsDistanceBetweenThingsUnder(hero25, me, 5.5) and quest:GetTimer(timerId) < 1) then
                quest:NewScriptFrame(me)
            else
                if quest:IsActiveThreadTerminating() then break end
                local conversationId7 = quest:AddNewConversation(me, false, false)
                local hero26 = quest:GetHero()
                quest:AddPersonToConversation(conversationId7, hero26)
                local hero27 = quest:GetHero()
                quest:AddLineToConversation(conversationId7, "TEXT_QST_028_APPLEGIRL_THANKS_AGAIN", me, hero27, false)
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
    local appleMode = appleMode_ or 0
    appleMode_ = quest:PersistTransferInt(context, "AppleMode", appleMode)
    local currentApples = currentApples_ or 0
    currentApples_ = quest:PersistTransferInt(context, "CurrentApples", currentApples)
end

-- AppleGirl.OnPredicateFail (retail 0x00d3d120)
function OnPredicateFail(quest, me)
end

