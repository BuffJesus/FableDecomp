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
    local isActiveThreadTerminating, timerId, questionAnswer, raceMode, questionAnswer2
    local questionAnswer3, questionAnswer4, conversationId2, i_stk_210_1, i_stk_210_2, i_stk_210_3
    local timerId3, timerId4, switch, switch7, switch8, movie, speedFriend, scratchValue43
    local scratchValue44, movie4, addQuestInfoTimer, movie7
    local function ReleaseEverything()
        quest:PauseAllNonScriptedEntities(false)
        local movie = movie7
        resources:DestroyMovie(movie)
    end
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d4075b end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:SetIsPushableByHero(me, false)
        speedFriend = quest:GetThingWithScriptName("SpeedFriend")
        local getNearestWithDefName = quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE")
        quest:EntityAttachToVillage(me, getNearestWithDefName)
        quest:EntityAttachToVillage(speedFriend, getNearestWithDefName)
        quest:EntitySetAsKillable(me, false, true)
        quest:EntitySetAsKillable(speedFriend, false, true)
        quest:SetThingHasInformation(me, false, true, false)
        me:SetFriendsWithEverythingFlag(true)
        if speedFriend ~= nil and not speedFriend:IsNull() then
            speedFriend:SetFriendsWithEverythingFlag(true)
        end
        raceMode_ = 0
        scratchValue44 = 0
        timerId = quest:RegisterTimer()
        timerId4 = quest:RegisterTimer()
        timerId3 = quest:RegisterTimer()
        quest:SetTimer(timerId4, 1)
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        while not isActiveThreadTerminating do
            if raceMode_ == 0 then
                i_stk_210_1 = scratchValue44 - 1
                repeat
                    local scratchValue12 = i_stk_210_1
                    if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                    if quest:GetStateInt("GameState") == 3 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        quest:SetThingHasInformation(me, false, true, false)
                    end
                    local isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(timerId4) < 1
                    if isDistanceBetweenThingsUnder then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        conversationId2 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId2, speedFriend)
                        if scratchValue12 < 5 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            switch = scratchValue12
                            repeat
                                if scratchValue12 == 0 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, speedFriend, false)
                                    break
                                elseif scratchValue12 == 1 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", speedFriend, me, false)
                                    break
                                elseif scratchValue12 == 2 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, speedFriend, false)
                                    break
                                elseif scratchValue12 == 3 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", speedFriend, me, false)
                                    break
                                elseif scratchValue12 == 4 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, speedFriend, false)
                                    break
                                else
                                    goto FLOW_native_label_1
                                end
                            until true
                        else
                            scratchValue43 = scratchValue44 & 0x80000001
                            isActiveThreadTerminating = scratchValue43 == 0
                            if scratchValue43 < 0 then
                                isActiveThreadTerminating = (scratchValue43 - 1 | 0xfffffffe) == 0xffffffff
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
                        scratchValue44 = scratchValue44 + 1
                        i_stk_210_1 = scratchValue12 + 1
                    end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        movie4 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_BOAST", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    quest:DeregisterTimer(timerId3)
                                    quest:DeregisterTimer(timerId4)
                                    quest:DeregisterTimer(timerId)
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
                                quest:DeregisterTimer(timerId3)
                                quest:DeregisterTimer(timerId4)
                                quest:DeregisterTimer(timerId)
                                resources:ReleaseResource(resource)
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
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
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
                            quest:SetTimer(timerId, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime))))
                            quest:SetTimer(timerId4, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime) + 20.0)))
                            addQuestInfoTimer = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                            quest:DisplayQuestInfo(true)
                        else
                            if isActiveThreadTerminating then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00d405fc
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
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
                    end
                until raceMode_ ~= 0
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId3)
                quest:DeregisterTimer(timerId4)
                quest:DeregisterTimer(timerId)
                goto LAB_00d40749
            end
            raceMode = raceMode_
            scratchValue43 = scratchValue44
            while true do
                scratchValue44 = scratchValue43
                if raceMode ~= 1 then break end
                if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                local isDistanceBetweenThingsUnder2 = quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(timerId4) < 1
                if isDistanceBetweenThingsUnder2 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                    local conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, speedFriend)
                    if scratchValue43 < 6 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        local switch6 = scratchValue43
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
                        local scratchValue42 = scratchValue43 & 0x80000001
                        isActiveThreadTerminating = scratchValue42 == 0
                        if scratchValue42 < 0 then
                            isActiveThreadTerminating = (scratchValue42 - 1 | 0xfffffffe) == 0xffffffff
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
                    scratchValue44 = scratchValue43 + 1
                end
                if me:IsTalkedToByHero() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                    quest:RemoveQuestInfoElement(addQuestInfoTimer)
                    quest:DisplayQuestInfo(false)
                    if not quest:GetStateBool("ReachedPlatform") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        local movie5 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
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
                    elseif quest:GetTimer(timerId) < 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        local movie2 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
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
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
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
                if quest:GetTimer(timerId3) >= 1 then scratchValue43 = scratchValue44; raceMode = raceMode_; goto continue_7 end
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                quest:RemoveQuestInfoElement(addQuestInfoTimer)
                quest:DisplayQuestInfo(false)
                if not quest:GetStateBool("ReachedPlatform") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                    quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("RaceMarker"))
                end
                raceMode_ = 2
                scratchValue43 = scratchValue44
                raceMode = raceMode_
                ::continue_7::
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
            if raceMode_ == 2 then
                i_stk_210_2 = scratchValue44 - 1
                repeat
                    if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                    local isDistanceBetweenThingsUnder3 = quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(timerId4) < 1
                    if isDistanceBetweenThingsUnder3 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        conversationId2 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId2, speedFriend)
                        local scratchValue19 = i_stk_210_2
                        if i_stk_210_2 < 5 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            switch7 = scratchValue19
                            repeat
                                if scratchValue19 == 0 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, speedFriend, false)
                                    break
                                elseif scratchValue19 == 1 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", speedFriend, me, false)
                                    break
                                elseif scratchValue19 == 2 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, speedFriend, false)
                                    break
                                elseif scratchValue19 == 3 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", speedFriend, me, false)
                                    break
                                elseif scratchValue19 == 4 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, speedFriend, false)
                                    break
                                else
                                    goto FLOW_native_label_3
                                end
                            until true
                        else
                            scratchValue43 = scratchValue44 & 0x80000001
                            isActiveThreadTerminating = scratchValue43 == 0
                            if scratchValue43 < 0 then
                                isActiveThreadTerminating = (scratchValue43 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if not isActiveThreadTerminating then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, speedFriend, false)
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", speedFriend, me, false)
                            end
                        end
                        ::FLOW_native_label_3::
                        quest:SetTimer(timerId4, 3)
                        scratchValue44 = scratchValue44 + 1
                        i_stk_210_2 = scratchValue19 + 1
                    end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        local movie3 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
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
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
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
                            quest:SetTimer(timerId, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime))))
                            quest:SetTimer(timerId4, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime) + 20.0)))
                            addQuestInfoTimer = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                            quest:MiniMapAddMarker(quest:GetThingWithScriptName("RaceMarker"), "HUD_ORB_QUEST_VIGNETTE")
                        else
                            if isActiveThreadTerminating then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00d405fc
                            end
                            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
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
                    end
                until raceMode_ ~= 2
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
            if raceMode_ == 3 then
                i_stk_210_3 = scratchValue44 - 1
                local scratchValue23 = i_stk_210_3
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then
                    local isDistanceBetweenThingsUnder4 = quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(timerId4) < 1
                    if isDistanceBetweenThingsUnder4 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        conversationId2 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId2, speedFriend)
                        if scratchValue23 < 5 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            switch8 = scratchValue23
                            repeat
                                if scratchValue23 == 0 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", speedFriend, me, false)
                                    break
                                elseif scratchValue23 == 1 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", me, speedFriend, false)
                                    break
                                elseif scratchValue23 == 2 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", speedFriend, me, false)
                                    break
                                elseif scratchValue23 == 3 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", me, speedFriend, false)
                                    break
                                elseif scratchValue23 == 4 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", speedFriend, me, false)
                                    break
                                else
                                    goto FLOW_native_label_4
                                end
                            until true
                        else
                            scratchValue43 = scratchValue44 & 0x80000001
                            isActiveThreadTerminating = scratchValue43 == 0
                            if scratchValue43 < 0 then
                                isActiveThreadTerminating = (scratchValue43 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if not isActiveThreadTerminating then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", speedFriend, me, false)
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", me, speedFriend, false)
                            end
                        end
                        ::FLOW_native_label_4::
                        quest:SetTimer(timerId4, 7)
                        scratchValue44 = scratchValue44 + 1
                        i_stk_210_3 = scratchValue23 + 1
                    end
                    if not me:IsTalkedToByHero() then goto LAB_00d403e1 end
                    if not quest:IsActiveThreadTerminating() then
                        movie7 = resources:StartMovie("")
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
                                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                        if not me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d402b3 end
                                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d402b3 end
                                    end
                                    goto LAB_00d40379
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d402b3
                            end
                            if not isActiveThreadTerminating then
                                if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                                    me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false)
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d402b3 end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d402b3 end
                                end
                                goto LAB_00d40379
                            end
                            goto FLOW_past_lab_00d40379
                            ::LAB_00d40379::
                            quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("RaceMarker"))
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie7)
                            goto LAB_00d403e1
                            ::FLOW_past_lab_00d40379::
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
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId4)
        quest:DeregisterTimer(timerId)
        ::LAB_00d40749::
    end
    ::FLOW_after_lab_00d405fc::
    ::LAB_00d4075b::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d405d6::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie4)
    quest:DeregisterTimer(timerId3)
    quest:DeregisterTimer(timerId4)
    quest:DeregisterTimer(timerId)
    goto FLOW_after_lab_00d405fc
    ::LAB_00d403e1::
    if raceMode_ ~= 3 then
        if not quest:IsActiveThreadTerminating() then
            quest:NewScriptFrame(me)
        end
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId4)
        quest:DeregisterTimer(timerId)
        goto FLOW_after_lab_00d405fc
    end
    local scratchValue26 = i_stk_210_3
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        local isDistanceBetweenThingsUnder5 = quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(timerId4) < 1
        if isDistanceBetweenThingsUnder5 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c31 end
            conversationId2 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId2, speedFriend)
            if scratchValue26 < 5 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c31 end
                switch8 = scratchValue26
                repeat
                    if scratchValue26 == 0 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", speedFriend, me, false)
                        break
                    elseif scratchValue26 == 1 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", me, speedFriend, false)
                        break
                    elseif scratchValue26 == 2 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", speedFriend, me, false)
                        break
                    elseif scratchValue26 == 3 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", me, speedFriend, false)
                        break
                    elseif scratchValue26 == 4 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", speedFriend, me, false)
                        break
                    else
                        goto FLOW_native_label_4_c31
                    end
                until true
            else
                scratchValue43 = scratchValue44 & 0x80000001
                isActiveThreadTerminating = scratchValue43 == 0
                if scratchValue43 < 0 then
                    isActiveThreadTerminating = (scratchValue43 - 1 | 0xfffffffe) == 0xffffffff
                end
                if not isActiveThreadTerminating then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c31 end
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", speedFriend, me, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c31 end
                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", me, speedFriend, false)
                end
            end
            ::FLOW_native_label_4_c31::
            quest:SetTimer(timerId4, 7)
            scratchValue44 = scratchValue44 + 1
            i_stk_210_3 = scratchValue26 + 1
        end
        if not me:IsTalkedToByHero() then goto LAB_00d403e1 end
        if not quest:IsActiveThreadTerminating() then
            movie7 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            questionAnswer4 = quest:MsgIsQuestionAnsweredYesOrNo()
            while questionAnswer4 < 0 do
                if not quest:NewScriptFrame(me) then ReleaseEverything(); goto LAB_00d405fc_c31 end
                questionAnswer4 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
            else
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if questionAnswer4 ~= 1 then
                    if not isActiveThreadTerminating then
                        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                            me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then ReleaseEverything(); goto LAB_00d405fc_c31 end
                            end
                            if quest:IsActiveThreadTerminating() then ReleaseEverything(); goto LAB_00d405fc_c31 end
                        end
                        goto LAB_00d40379_c31
                    end
                    ReleaseEverything(); goto LAB_00d405fc_c31
                end
                if not isActiveThreadTerminating then
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); goto LAB_00d405fc_c31 end
                        end
                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); goto LAB_00d405fc_c31 end
                    end
                    goto LAB_00d40379_c31
                end
                goto FLOW_past_lab_00d40379_c31
                ::LAB_00d40379_c31::
                quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("RaceMarker"))
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie7)
                goto LAB_00d403e1
                ::FLOW_past_lab_00d40379_c31::
                quest:PauseAllNonScriptedEntities(false)
            end
            resources:DestroyMovie(movie7)
        end
    end
    goto LAB_00d405fc_c31
    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c31 end
    quest:NewScriptFrame(me)
    ::LAB_00d405fc_c31::
    quest:DeregisterTimer(timerId3)
    quest:DeregisterTimer(timerId4)
    quest:DeregisterTimer(timerId)
    goto FLOW_after_lab_00d405fc
end

-- ApprenticeSpeedTest.Init (retail 0x00d3e2a0)
function Init(quest, me)
end

-- ApprenticeSpeedTest.OnPersist (retail 0x00d446a0)
function OnPersist(quest, me, context)
    quest:SetStateInt("RaceMode", quest:PersistTransferInt(context, "RaceMode", quest:GetStateInt("RaceMode") or 0))
end

-- ApprenticeSpeedTest.OnPredicateFail (retail 0x00d3e2b0)
function OnPredicateFail(quest, me)
end

