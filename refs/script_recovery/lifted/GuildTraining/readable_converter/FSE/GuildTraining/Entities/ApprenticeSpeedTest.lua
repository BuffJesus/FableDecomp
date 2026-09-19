-- Readable native conversion: ApprenticeSpeedTest. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_RaceTime = 3804,  -- 50.0
    GUI_RaceGold = 3808,  -- 25.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local raceMode_

-- ApprenticeSpeedTest.Main (retail 0x00d3e2e0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local isActiveThreadTerminating, questionAnswer, raceMode, questionAnswer2, questionAnswer3
    local questionAnswer4, i_stk_210_1, i_stk_210_2, i_stk_210_3, switch, switch7, switch8, movie
    local scratchValue59, speedFriend, scratchValue61, scratchValue62, movie3, movie4
    local addQuestInfoTimer, timerId, timerId3, timerId4, movie7
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        local movie = movie7
        resources:DestroyMovie(movie)
    end
    local function ReleaseEverything2()
        local scratchValue59 = quest:GetThingWithScriptName("RaceMarker")
        quest:MiniMapRemoveMarker(scratchValue59)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie7)
    end
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        if not me:AcquireControl(4) then goto LAB_00d4075b end
        if not quest:IsActiveThreadTerminating() then
            quest:SetIsPushableByHero(me, false)
            speedFriend = quest:GetThingWithScriptName("SpeedFriend")
            local getNearestWithDefName = quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE")
            quest:EntityAttachToVillage(me, getNearestWithDefName)
            quest:EntityAttachToVillage(speedFriend, getNearestWithDefName)
            quest:EntitySetAsKillable(me, false, true)
            quest:EntitySetAsKillable(speedFriend, false, true)
            quest:SetThingHasInformation(me, false, true, false)
            me:SetFriendsWithEverythingFlag(me)
            if speedFriend ~= nil and not speedFriend:IsNull() then
                speedFriend:SetFriendsWithEverythingFlag(1)
            end
            raceMode_ = 0
            scratchValue62 = 0
            timerId3 = quest:RegisterTimer()
            timerId4 = quest:RegisterTimer()
            timerId = quest:RegisterTimer()
            quest:SetTimer(timerId4, 1)
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            while not isActiveThreadTerminating do
                if raceMode_ == 0 then
                    i_stk_210_1 = scratchValue62 - 1
                    repeat
                        local scratchValue24 = i_stk_210_1
                        if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                        if quest:GetStateInt("GameState") == 3 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            quest:SetThingHasInformation(me, false, true, false)
                        end
                        local isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(timerId4) < 1
                        if isDistanceBetweenThingsUnder then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            local conversationId2 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(conversationId2, speedFriend)
                            if scratchValue24 < 5 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                switch = scratchValue24
                                repeat
                                    if scratchValue24 == 0 then
                                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, speedFriend, false)
                                        break
                                    elseif scratchValue24 == 1 then
                                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", speedFriend, me, false)
                                        break
                                    elseif scratchValue24 == 2 then
                                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, speedFriend, false)
                                        break
                                    elseif scratchValue24 == 3 then
                                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", speedFriend, me, false)
                                        break
                                    elseif scratchValue24 == 4 then
                                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, speedFriend, false)
                                        break
                                    else
                                        goto FLOW_native_label_1
                                    end
                                until true
                            else
                                scratchValue61 = scratchValue62 & 0x80000001
                                isActiveThreadTerminating = scratchValue61 == 0
                                if scratchValue61 < 0 then
                                    isActiveThreadTerminating = (scratchValue61 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not isActiveThreadTerminating then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, speedFriend, false)
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", speedFriend, me, false)
                                end
                            end
                            ::FLOW_native_label_1::
                            quest:SetTimer(timerId4, 3)
                            scratchValue62 = scratchValue62 + 1
                            i_stk_210_1 = scratchValue24 + 1
                        end
                        if not me:IsTalkedToByHero() then goto continue_1 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        movie4 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(me) then
                            me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_BOAST", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    quest:DeregisterTimer(timerId)
                                    quest:DeregisterTimer(timerId4)
                                    quest:DeregisterTimer(timerId3)
                                    goto LAB_00d40749
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00d405fc
                            end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_BOAST_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer < 0 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                            else
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                quest:DeregisterTimer(timerId)
                                quest:DeregisterTimer(timerId4)
                                quest:DeregisterTimer(timerId3)
                                me:ReleaseControl()
                                do return end
                                questionAnswer = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie4)
                            goto LAB_00d405fc
                        end
                        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                        if questionAnswer == 1 then
                            if isActiveThreadTerminating then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00d405fc
                            end
                            if 0.0 < quest:GetHealth(me) then
                                if not me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_RUN", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d405d6 end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    goto LAB_00d405fc
                                end
                            end
                            raceMode_ = 1
                            quest:SetStateBool("ReachedPlatform", false)
                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("RaceMarker"), "HUD_ORB_QUEST_VIGNETTE")
                            quest:SetTimer(timerId3, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime))))
                            quest:SetTimer(timerId, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime) + 20.0)))
                            addQuestInfoTimer = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                            quest:DisplayQuestInfo(true)
                        else
                            if isActiveThreadTerminating then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00d405fc
                            end
                            if 0.0 < quest:GetHealth(me) then
                                if not me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_REFUSE", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d405d6 end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    goto LAB_00d405fc
                                end
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie4)
                        ::continue_1::
                    until raceMode_ ~= 0
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    quest:DeregisterTimer(timerId4)
                    quest:DeregisterTimer(timerId3)
                    goto LAB_00d40749
                end
                raceMode = raceMode_
                scratchValue61 = scratchValue62
                while true do
                    scratchValue62 = scratchValue61
                    if raceMode ~= 1 then break end
                    if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                    local isDistanceBetweenThingsUnder2 = quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(timerId4) < 1
                    if isDistanceBetweenThingsUnder2 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        local conversationId = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId, speedFriend)
                        if scratchValue61 < 6 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            local switch6 = scratchValue61
                            repeat
                                if switch6 == 1 then
                                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, speedFriend, false)
                                    break
                                elseif switch6 == 2 then
                                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", speedFriend, me, false)
                                    break
                                elseif switch6 == 3 then
                                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, speedFriend, false)
                                    break
                                elseif switch6 == 4 then
                                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", speedFriend, me, false)
                                    break
                                elseif switch6 == 5 then
                                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, speedFriend, false)
                                    break
                                else
                                    goto FLOW_native_label_2
                                end
                            until true
                        else
                            local scratchValue60 = scratchValue61 & 0x80000001
                            isActiveThreadTerminating = scratchValue60 == 0
                            if scratchValue60 < 0 then
                                isActiveThreadTerminating = (scratchValue60 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if not isActiveThreadTerminating then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                quest:AddLineToConversation(conversationId, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, speedFriend, false)
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                quest:AddLineToConversation(conversationId, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", speedFriend, me, false)
                            end
                        end
                        ::FLOW_native_label_2::
                        quest:SetTimer(timerId4, 3)
                        scratchValue62 = scratchValue61 + 1
                    end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        quest:RemoveQuestInfoElement(addQuestInfoTimer)
                        quest:DisplayQuestInfo(false)
                        if not quest:GetStateBool("ReachedPlatform") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            local movie5 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(me) then
                                me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM", GROUP_SELECT_FIRST, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie5)
                                        goto LAB_00d405fc
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie5)
                                    goto LAB_00d405fc
                                end
                            end
                            raceMode_ = 2
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie5
                        elseif quest:GetTimer(timerId3) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            local movie2 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(me) then
                                me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW", GROUP_SELECT_FIRST, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie2)
                                        goto LAB_00d405fc
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    goto LAB_00d405fc
                                end
                            end
                            raceMode_ = 2
                            if not quest:GetStateBool("ReachedPlatform") then
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    goto LAB_00d405fc
                                end
                                quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("RaceMarker"))
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie2
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            local movie6 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(me) then
                                me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED", GROUP_SELECT_FIRST, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie6)
                                        goto LAB_00d405fc
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie6)
                                    goto LAB_00d405fc
                                end
                            end
                            raceMode_ = 3
                            quest:GiveHeroGold(math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceGold))))
                            quest:ClearThingHasInformation(me)
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie6
                        end
                        resources:DestroyMovie(movie)
                    end
                    if quest:GetTimer(timerId) >= 1 then scratchValue61 = scratchValue62; raceMode = raceMode_; goto continue_7 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                    quest:RemoveQuestInfoElement(addQuestInfoTimer)
                    quest:DisplayQuestInfo(false)
                    if not quest:GetStateBool("ReachedPlatform") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("RaceMarker"))
                    end
                    raceMode_ = 2
                    scratchValue61 = scratchValue62
                    raceMode = raceMode_
                    ::continue_7::
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                if raceMode_ == 2 then
                    i_stk_210_2 = scratchValue62 - 1
                    repeat
                        if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                        local isDistanceBetweenThingsUnder3 = quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(timerId4) < 1
                        if isDistanceBetweenThingsUnder3 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            local conversationId3 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(conversationId3, speedFriend)
                            local scratchValue31 = i_stk_210_2
                            if i_stk_210_2 < 5 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                switch7 = scratchValue31
                                repeat
                                    if scratchValue31 == 0 then
                                        quest:AddLineToConversation(conversationId3, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, speedFriend, false)
                                        break
                                    elseif scratchValue31 == 1 then
                                        quest:AddLineToConversation(conversationId3, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", speedFriend, me, false)
                                        break
                                    elseif scratchValue31 == 2 then
                                        quest:AddLineToConversation(conversationId3, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, speedFriend, false)
                                        break
                                    elseif scratchValue31 == 3 then
                                        quest:AddLineToConversation(conversationId3, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", speedFriend, me, false)
                                        break
                                    elseif scratchValue31 == 4 then
                                        quest:AddLineToConversation(conversationId3, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, speedFriend, false)
                                        break
                                    else
                                        goto FLOW_native_label_3
                                    end
                                until true
                            else
                                scratchValue61 = scratchValue62 & 0x80000001
                                isActiveThreadTerminating = scratchValue61 == 0
                                if scratchValue61 < 0 then
                                    isActiveThreadTerminating = (scratchValue61 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not isActiveThreadTerminating then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(conversationId3, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, speedFriend, false)
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(conversationId3, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", speedFriend, me, false)
                                end
                            end
                            ::FLOW_native_label_3::
                            quest:SetTimer(timerId4, 3)
                            scratchValue62 = scratchValue62 + 1
                            i_stk_210_2 = scratchValue31 + 1
                        end
                        if not me:IsTalkedToByHero() then goto continue_8 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        movie3 = resources:StartMovie("")
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(me) then
                            me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00d405fc
                            end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                        questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while questionAnswer2 < 0 do
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                            else
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00d405fc
                                questionAnswer2 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie3)
                            goto LAB_00d405fc
                        end
                        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                        if questionAnswer2 == 1 then
                            if isActiveThreadTerminating then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00d405fc
                            end
                            if 0.0 < quest:GetHealth(me) then
                                me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_RUN", GROUP_SELECT_FIRST, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie3)
                                        goto LAB_00d405fc
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
                            end
                            raceMode_ = 1
                            quest:SetStateBool("ReachedPlatform", false)
                            quest:SetTimer(timerId3, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime))))
                            quest:SetTimer(timerId, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime) + 20.0)))
                            addQuestInfoTimer = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("RaceMarker"), "HUD_ORB_QUEST_VIGNETTE")
                        else
                            if isActiveThreadTerminating then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00d405fc
                            end
                            if 0.0 < quest:GetHealth(me) then
                                me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_REFUSE", GROUP_SELECT_FIRST, false, true, false)
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie3)
                                        goto LAB_00d405fc
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie3)
                        ::continue_8::
                    until raceMode_ ~= 2
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                if raceMode_ == 3 then
                    i_stk_210_3 = scratchValue62 - 1
                    local scratchValue35 = i_stk_210_3
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        local isDistanceBetweenThingsUnder4 = quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(timerId4) < 1
                        if isDistanceBetweenThingsUnder4 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            local conversationId4 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(conversationId4, speedFriend)
                            if scratchValue35 < 5 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                switch8 = scratchValue35
                                repeat
                                    if scratchValue35 == 0 then
                                        quest:AddLineToConversation(conversationId4, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", speedFriend, me, false)
                                        break
                                    elseif scratchValue35 == 1 then
                                        quest:AddLineToConversation(conversationId4, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", me, speedFriend, false)
                                        break
                                    elseif scratchValue35 == 2 then
                                        quest:AddLineToConversation(conversationId4, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", speedFriend, me, false)
                                        break
                                    elseif scratchValue35 == 3 then
                                        quest:AddLineToConversation(conversationId4, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", me, speedFriend, false)
                                        break
                                    elseif scratchValue35 == 4 then
                                        quest:AddLineToConversation(conversationId4, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", speedFriend, me, false)
                                        break
                                    else
                                        goto FLOW_native_label_4
                                    end
                                until true
                            else
                                scratchValue61 = scratchValue62 & 0x80000001
                                isActiveThreadTerminating = scratchValue61 == 0
                                if scratchValue61 < 0 then
                                    isActiveThreadTerminating = (scratchValue61 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not isActiveThreadTerminating then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(conversationId4, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", speedFriend, me, false)
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(conversationId4, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", me, speedFriend, false)
                                end
                            end
                            ::FLOW_native_label_4::
                            quest:SetTimer(timerId4, 7)
                            scratchValue62 = scratchValue62 + 1
                            i_stk_210_3 = scratchValue35 + 1
                        end
                        if not me:IsTalkedToByHero() then goto LAB_00d403e1 end
                        if not quest:IsActiveThreadTerminating() then
                            movie7 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            questionAnswer3 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while questionAnswer3 < 0 do
                                quest:NewScriptFrame(me)
                                if not quest:IsActiveThreadTerminating() then
                                    questionAnswer3 = quest:MsgIsQuestionAnsweredYesOrNo()
                                else
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto FLOW_after_lab_00d402b3
                                    questionAnswer3 = quest:MsgIsQuestionAnsweredYesOrNo()
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                            else
                                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                                if questionAnswer3 ~= 1 then
                                    if not isActiveThreadTerminating then
                                        if 0.0 < quest:GetHealth(me) then
                                            if not me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d402b3 end
                                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d402b3 end
                                        end
                                        quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("RaceMarker"))
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie7)
                                        goto LAB_00d403e1
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto FLOW_after_lab_00d402b3
                                end
                                if not isActiveThreadTerminating then
                                    if 0.0 < quest:GetHealth(me) then
                                        me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false)
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d402b3 end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d402b3 end
                                    end
                                    quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("RaceMarker"))
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie7)
                                    goto LAB_00d403e1
                                end
                                ::LAB_00d402b3::
                                quest:PauseAllNonScriptedEntities(false)
                            end
                            ::FLOW_after_lab_00d402b3::
                            resources:DestroyMovie(movie7)
                        end
                    end
                    goto LAB_00d405fc
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                quest:NewScriptFrame(me)
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            end
            ::LAB_00d405fc::
            quest:DeregisterTimer(timerId)
            quest:DeregisterTimer(timerId4)
            quest:DeregisterTimer(timerId3)
            ::LAB_00d40749::
        end
        ::LAB_00d4075b::
        me:ReleaseControl()
    end
    ::FLOW_after_lab_00d405fc::
    do return end
    ::LAB_00d405d6::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie4)
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId4)
    quest:DeregisterTimer(timerId3)
    me:ReleaseControl()
    goto FLOW_after_lab_00d405fc
    ::LAB_00d403e1::
    if raceMode_ ~= 3 then
        if not quest:IsActiveThreadTerminating() then
            quest:NewScriptFrame(me)
        end
        quest:DeregisterTimer(timerId)
        quest:DeregisterTimer(timerId4)
        quest:DeregisterTimer(timerId3)
        me:ReleaseControl()
        goto FLOW_after_lab_00d405fc
    end
    local scratchValue38 = i_stk_210_3
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        local isDistanceBetweenThingsUnder5 = quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(timerId4) < 1
        if isDistanceBetweenThingsUnder5 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
            local conversationId5 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId5, speedFriend)
            if scratchValue38 < 5 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
                switch8 = scratchValue38
                repeat
                    if scratchValue38 == 0 then
                        quest:AddLineToConversation(conversationId5, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", speedFriend, me, false)
                        break
                    elseif scratchValue38 == 1 then
                        quest:AddLineToConversation(conversationId5, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", me, speedFriend, false)
                        break
                    elseif scratchValue38 == 2 then
                        quest:AddLineToConversation(conversationId5, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", speedFriend, me, false)
                        break
                    elseif scratchValue38 == 3 then
                        quest:AddLineToConversation(conversationId5, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", me, speedFriend, false)
                        break
                    elseif scratchValue38 == 4 then
                        quest:AddLineToConversation(conversationId5, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", speedFriend, me, false)
                        break
                    else
                        goto FLOW_native_label_4_c32
                    end
                until true
            else
                scratchValue61 = scratchValue62 & 0x80000001
                isActiveThreadTerminating = scratchValue61 == 0
                if scratchValue61 < 0 then
                    isActiveThreadTerminating = (scratchValue61 - 1 | 0xfffffffe) == 0xffffffff
                end
                if not isActiveThreadTerminating then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
                    quest:AddLineToConversation(conversationId5, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", speedFriend, me, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
                    quest:AddLineToConversation(conversationId5, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", me, speedFriend, false)
                end
            end
            ::FLOW_native_label_4_c32::
            quest:SetTimer(timerId4, 7)
            scratchValue62 = scratchValue62 + 1
            i_stk_210_3 = scratchValue38 + 1
        end
        if not me:IsTalkedToByHero() then goto LAB_00d403e1 end
        if not quest:IsActiveThreadTerminating() then
            movie7 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            questionAnswer4 = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer4 < 0 do
                if not quest:NewScriptFrame(me) then ReleaseEverything(); goto LAB_00d405fc_c32 end
                questionAnswer4 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
            else
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if questionAnswer4 ~= 1 then
                    if not isActiveThreadTerminating then
                        if 0.0 < quest:GetHealth(me) then
                            me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then ReleaseEverything(); goto LAB_00d405fc_c32 end
                            end
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); goto LAB_00d405fc_c32 end
                        end
                        ReleaseEverything2(); goto LAB_00d403e1
                    end
                    ReleaseEverything(); goto LAB_00d405fc_c32
                end
                if not isActiveThreadTerminating then
                    if 0.0 < quest:GetHealth(me) then
                        me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); goto LAB_00d405fc_c32 end
                        end
                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); goto LAB_00d405fc_c32 end
                    end
                    ReleaseEverything2()
                    goto LAB_00d403e1
                end
                quest:PauseAllNonScriptedEntities(false)
            end
            resources:DestroyMovie(movie7)
        end
    end
    goto LAB_00d405fc_c32
    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
    quest:NewScriptFrame(me)
    ::LAB_00d405fc_c32::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId4)
    quest:DeregisterTimer(timerId3)
    me:ReleaseControl()
    goto FLOW_after_lab_00d405fc
end

-- ApprenticeSpeedTest.Init (retail 0x00d3e2a0)
function Init(quest, me)
end

-- ApprenticeSpeedTest.OnPersist (retail 0x00d446a0)
function OnPersist(quest, context)
end

-- ApprenticeSpeedTest.OnPredicateFail (retail 0x00d3e2b0)
function OnPredicateFail(quest, me)
end

