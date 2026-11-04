-- Readable native conversion: ApprenticeSpeedTest. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_RaceTime = 3804,  -- 50.0
    GUI_RaceGold = 3808,  -- 25.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local raceMode_

-- ApprenticeSpeedTest.Main (retail 0x00d3e2e0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local isActiveThreadTerminating, predicateResult, timerId, questionAnswer, raceMode
    local questionAnswer2, questionAnswer3, questionAnswer4, conversationId2, scratchValue29
    local timerId3, timerId4, switch81, movie, raceMarker5, raceMarker6, line, speedFriend
    local getNearestWithDefName, scratchValue44, scratchValue45, movie4, addQuestInfoTimer, movie7
    local scriptThing, scriptThing3
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
        getNearestWithDefName = quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE")
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
        scratchValue45 = 0
        timerId = quest:RegisterTimer()
        timerId4 = quest:RegisterTimer()
        timerId3 = quest:RegisterTimer()
        quest:SetTimer(timerId4, 1)
        addQuestInfoTimer = 0xffffffff
        predicateResult = false
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        while not isActiveThreadTerminating do
            if raceMode_ == 0 then
                scratchValue29 = scratchValue45 - 1
                repeat
                    local scratchValue12 = scratchValue29
                    if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                    if quest:GetStateInt("GameState") == 3 and not predicateResult then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        quest:SetThingHasInformation(me, false, true, false)
                        predicateResult = true
                    end
                    local hero = quest:GetHero()
                    local isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(timerId4) < 1
                    if isDistanceBetweenThingsUnder then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        conversationId2 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId2, speedFriend)
                        if scratchValue12 < 5 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            local switch = scratchValue12
                            repeat
                                if switch == 0 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, speedFriend, false)
                                    break
                                elseif switch == 1 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", speedFriend, me, false)
                                    break
                                elseif switch == 2 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, speedFriend, false)
                                    break
                                elseif switch == 3 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", speedFriend, me, false)
                                    break
                                elseif switch == 4 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, speedFriend, false)
                                    break
                                else
                                    goto FLOW_native_label_1
                                end
                            until true
                        else
                            scratchValue44 = scratchValue45 & 0x80000001
                            isActiveThreadTerminating = scratchValue44 == 0
                            if scratchValue44 < 0 then
                                isActiveThreadTerminating = (scratchValue44 - 1 | 0xfffffffe) == 0xffffffff
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
                        scratchValue45 = scratchValue45 + 1
                        scratchValue29 = scratchValue12 + 1
                    end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        movie4 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        local scratchValue30 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scratchValue30) then
                            conversationId2 = 0
                            line = "TEXT_QST_028_FAST_APPRENTICE_BOAST"
                            local hero2 = quest:GetHero()
                            if not me:Speak(hero2, line, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                quest:DeregisterTimer(timerId3)
                                quest:DeregisterTimer(timerId4)
                                quest:DeregisterTimer(timerId)
                                goto LAB_00d40749
                            end
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
                                getNearestWithDefName = nil
                                speedFriend = nil
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
                            local scratchValue53 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue53) then
                                conversationId2 = 0
                                line = "TEXT_QST_028_FAST_APPRENTICE_RUN"
                                local hero3 = quest:GetHero()
                                if not me:Speak(hero3, line, 0, false, true, false) then
                                    goto LAB_00d405d6
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d405d6 end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    goto LAB_00d405fc
                                end
                            end
                            raceMode_ = 1
                            quest:SetStateBool("ReachedPlatform", false)
                            local raceMarker = quest:GetThingWithScriptName("RaceMarker")
                            quest:MiniMapAddMarker(raceMarker, "HUD_ORB_QUEST_VIGNETTE")
                            conversationId2 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime)))
                            quest:SetTimer(timerId, conversationId2)
                            conversationId2 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime) + 20.0))
                            quest:SetTimer(timerId3, conversationId2)
                            addQuestInfoTimer = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                            quest:DisplayQuestInfo(true)
                            if not predicateResult then
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    goto LAB_00d405fc
                                end
                                quest:SetThingHasInformation(me, false, true, false)
                                predicateResult = true
                            end
                        else
                            if isActiveThreadTerminating then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00d405fc
                            end
                            local scratchValue50 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue50) then
                                conversationId2 = 0
                                line = "TEXT_QST_028_FAST_APPRENTICE_REFUSE"
                                local hero4 = quest:GetHero()
                                if not me:Speak(hero4, line, 0, false, true, false) then
                                    goto LAB_00d405d6
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00d405d6 end
                                end
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
            scratchValue44 = scratchValue45
            while true do
                scratchValue45 = scratchValue44
                if raceMode ~= 1 then break end
                if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                local hero5 = quest:GetHero()
                local isDistanceBetweenThingsUnder2 = quest:IsDistanceBetweenThingsUnder(hero5, me, 10.0) and quest:GetTimer(timerId4) < 1
                if isDistanceBetweenThingsUnder2 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                    local conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, speedFriend)
                    if scratchValue44 < 6 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        local switch6 = scratchValue44
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
                        local scratchValue43 = scratchValue44 & 0x80000001
                        isActiveThreadTerminating = scratchValue43 == 0
                        if scratchValue43 < 0 then
                            isActiveThreadTerminating = (scratchValue43 - 1 | 0xfffffffe) == 0xffffffff
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
                    scratchValue45 = scratchValue44 + 1
                end
                if me:IsTalkedToByHero() then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                    quest:RemoveQuestInfoElement((addQuestInfoTimer + 2147483648) % 4294967296 - 2147483648)
                    quest:DisplayQuestInfo(false)
                    if not quest:GetStateBool("ReachedPlatform") then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        local movie5 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        local scratchValue49 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scratchValue49) then
                            conversationId2 = 0
                            line = "TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM"
                            local hero6 = quest:GetHero()
                            if not me:Speak(hero6, line, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie5)
                                goto LAB_00d405fc
                            end
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
                        local scratchValue48 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scratchValue48) then
                            conversationId2 = 0
                            line = "TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW"
                            local hero7 = quest:GetHero()
                            if not me:Speak(hero7, line, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                goto LAB_00d405fc
                            end
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
                            local raceMarker2 = quest:GetThingWithScriptName("RaceMarker")
                            quest:MiniMapRemoveMarker(raceMarker2)
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie2
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        local movie6 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        local scratchValue46 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scratchValue46) then
                            conversationId2 = 0
                            line = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED"
                            local hero8 = quest:GetHero()
                            if not me:Speak(hero8, line, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie6)
                                goto LAB_00d405fc
                            end
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
                        conversationId2 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceGold)))
                        quest:GiveHeroGold(conversationId2)
                        quest:ClearThingHasInformation(me)
                        predicateResult = false
                        quest:PauseAllNonScriptedEntities(false)
                        movie = movie6
                    end
                    resources:DestroyMovie(movie)
                end
                if not (quest:GetTimer(timerId3) < 1) then scratchValue44 = scratchValue45; raceMode = raceMode_; goto continue_7 end
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                quest:RemoveQuestInfoElement((addQuestInfoTimer + 2147483648) % 4294967296 - 2147483648)
                quest:DisplayQuestInfo(false)
                if not quest:GetStateBool("ReachedPlatform") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                    local raceMarker3 = quest:GetThingWithScriptName("RaceMarker")
                    quest:MiniMapRemoveMarker(raceMarker3)
                end
                raceMode_ = 2
                scratchValue44 = scratchValue45
                raceMode = raceMode_
                ::continue_7::
            end
            if quest:IsActiveThreadTerminating() then break end
            if raceMode_ == 2 then
                scratchValue29 = scratchValue45 - 1
                repeat
                    if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                    local hero9 = quest:GetHero()
                    local isDistanceBetweenThingsUnder3 = quest:IsDistanceBetweenThingsUnder(hero9, me, 10.0) and quest:GetTimer(timerId4) < 1
                    if isDistanceBetweenThingsUnder3 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        conversationId2 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId2, speedFriend)
                        local scratchValue19 = scratchValue29
                        if scratchValue29 < 5 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            local switch7 = scratchValue19
                            repeat
                                if switch7 == 0 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, speedFriend, false)
                                    break
                                elseif switch7 == 1 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", speedFriend, me, false)
                                    break
                                elseif switch7 == 2 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, speedFriend, false)
                                    break
                                elseif switch7 == 3 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", speedFriend, me, false)
                                    break
                                elseif switch7 == 4 then
                                    quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, speedFriend, false)
                                    break
                                else
                                    goto FLOW_native_label_3
                                end
                            until true
                        else
                            scratchValue44 = scratchValue45 & 0x80000001
                            isActiveThreadTerminating = scratchValue44 == 0
                            if scratchValue44 < 0 then
                                isActiveThreadTerminating = (scratchValue44 - 1 | 0xfffffffe) == 0xffffffff
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
                        scratchValue45 = scratchValue45 + 1
                        scratchValue29 = scratchValue19 + 1
                    end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        local movie3 = resources:StartMovie("")
                        quest:PauseAllNonScriptedEntities(true)
                        local scratchValue47 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scratchValue47) then
                            conversationId2 = 0
                            line = "TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL"
                            local hero10 = quest:GetHero()
                            if not me:Speak(hero10, line, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00d405fc
                            end
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
                            local scratchValue52 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue52) then
                                conversationId2 = 0
                                line = "TEXT_QST_028_FAST_APPRENTICE_RUN"
                                local hero11 = quest:GetHero()
                                if not me:Speak(hero11, line, 0, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
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
                            conversationId2 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime)))
                            quest:SetTimer(timerId, conversationId2)
                            conversationId2 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime) + 20.0))
                            quest:SetTimer(timerId3, conversationId2)
                            addQuestInfoTimer = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                            local raceMarker4 = quest:GetThingWithScriptName("RaceMarker")
                            quest:MiniMapAddMarker(raceMarker4, "HUD_ORB_QUEST_VIGNETTE")
                        else
                            if isActiveThreadTerminating then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00d405fc
                            end
                            local scratchValue51 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue51) then
                                conversationId2 = 0
                                line = "TEXT_QST_028_FAST_APPRENTICE_REFUSE"
                                local hero12 = quest:GetHero()
                                if not me:Speak(hero12, line, 0, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
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
            if quest:IsActiveThreadTerminating() then break end
            if raceMode_ == 3 then
                scratchValue29 = scratchValue45 - 1
                local scratchValue23 = scratchValue29
                quest:NewScriptFrame(me)
                if not quest:IsActiveThreadTerminating() then
                    local hero13 = quest:GetHero()
                    local isDistanceBetweenThingsUnder4 = quest:IsDistanceBetweenThingsUnder(hero13, me, 10.0) and quest:GetTimer(timerId4) < 1
                    if isDistanceBetweenThingsUnder4 then
                        if quest:IsActiveThreadTerminating() then break end
                        conversationId2 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(conversationId2, speedFriend)
                        if scratchValue23 < 5 then
                            if quest:IsActiveThreadTerminating() then break end
                            switch81 = scratchValue23
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
                            scratchValue44 = scratchValue45 & 0x80000001
                            isActiveThreadTerminating = scratchValue44 == 0
                            if scratchValue44 < 0 then
                                isActiveThreadTerminating = (scratchValue44 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if not isActiveThreadTerminating then
                                if quest:IsActiveThreadTerminating() then break end
                                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", speedFriend, me, false)
                            else
                                if quest:IsActiveThreadTerminating() then break end
                                quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", me, speedFriend, false)
                            end
                        end
                        ::FLOW_native_label_4::
                        quest:SetTimer(timerId4, 7)
                        scratchValue45 = scratchValue45 + 1
                        scratchValue29 = scratchValue23 + 1
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
                                    scriptThing3 = resources:ScriptThing(resource)
                                    if 0.0 < quest:GetHealth(scriptThing3) then
                                        conversationId2 = 0
                                        line = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO"
                                        local hero14 = quest:GetHero()
                                        if not me:Speak(hero14, line, 0, false, true, false) then
                                            goto LAB_00d402b3
                                        end
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00d402b3 end
                                        end
                                        if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d402b3 end
                                    end
                                    goto LAB_00d40379
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                goto FLOW_after_lab_00d402b3
                            end
                            if not isActiveThreadTerminating then
                                scriptThing = resources:ScriptThing(resource)
                                if 0.0 < quest:GetHealth(scriptThing) then
                                    conversationId2 = 0
                                    line = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES"
                                    local hero15 = quest:GetHero()
                                    if not me:Speak(hero15, line, 0, false, true, false) then
                                        quest:PauseAllNonScriptedEntities(false)
                                        goto FLOW_after_lab_00d402b3
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d402b3 end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d402b3 end
                                end
                                goto LAB_00d40379
                            end
                            goto FLOW_past_lab_00d40379
                            ::LAB_00d40379::
                            raceMarker5 = quest:GetThingWithScriptName("RaceMarker")
                            quest:MiniMapRemoveMarker(raceMarker5)
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie7)
                            goto LAB_00d403e1
                            ::FLOW_past_lab_00d40379::
                            ::LAB_00d402b3::
                            quest:PauseAllNonScriptedEntities(false)
                        end
                        ::FLOW_after_lab_00d402b3::
                        movie = movie7
                        resources:DestroyMovie(movie)
                    end
                end
                break
            end
            if quest:IsActiveThreadTerminating() then break end
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
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        end
        quest:DeregisterTimer(timerId3)
        quest:DeregisterTimer(timerId4)
        quest:DeregisterTimer(timerId)
        goto FLOW_after_lab_00d405fc
    end
    local scratchValue26 = scratchValue29
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        local hero16 = quest:GetHero()
        local isDistanceBetweenThingsUnder5 = quest:IsDistanceBetweenThingsUnder(hero16, me, 10.0) and quest:GetTimer(timerId4) < 1
        if isDistanceBetweenThingsUnder5 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c31 end
            conversationId2 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId2, speedFriend)
            if scratchValue26 < 5 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c31 end
                local switch82 = scratchValue26
                repeat
                    if switch82 == 0 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", speedFriend, me, false)
                        break
                    elseif switch82 == 1 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", me, speedFriend, false)
                        break
                    elseif switch82 == 2 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", speedFriend, me, false)
                        break
                    elseif switch82 == 3 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", me, speedFriend, false)
                        break
                    elseif switch82 == 4 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", speedFriend, me, false)
                        break
                    else
                        goto FLOW_native_label_4_c31
                    end
                until true
            else
                scratchValue44 = scratchValue45 & 0x80000001
                isActiveThreadTerminating = scratchValue44 == 0
                if scratchValue44 < 0 then
                    isActiveThreadTerminating = (scratchValue44 - 1 | 0xfffffffe) == 0xffffffff
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
            scratchValue45 = scratchValue45 + 1
            scratchValue29 = scratchValue26 + 1
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
                        scriptThing3 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scriptThing3) then
                            conversationId2 = 0
                            line = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO"
                            local hero17 = quest:GetHero()
                            if not me:Speak(hero17, line, 0, false, true, false) then
                                ReleaseEverything(); goto LAB_00d405fc_c31
                            end
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
                    scriptThing = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scriptThing) then
                        conversationId2 = 0
                        line = "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES"
                        local hero18 = quest:GetHero()
                        if not me:Speak(hero18, line, 0, false, true, false) then
                            ReleaseEverything(); goto LAB_00d405fc_c31
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then ReleaseEverything(); goto LAB_00d405fc_c31 end
                        end
                        if quest:IsActiveThreadTerminating() then ReleaseEverything(); goto LAB_00d405fc_c31 end
                    end
                    goto LAB_00d40379_c31
                end
                goto FLOW_past_lab_00d40379_c31
                ::LAB_00d40379_c31::
                raceMarker6 = quest:GetThingWithScriptName("RaceMarker")
                quest:MiniMapRemoveMarker(raceMarker6)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie7)
                goto LAB_00d403e1
                ::FLOW_past_lab_00d40379_c31::
                quest:PauseAllNonScriptedEntities(false)
            end
            movie = movie7
            resources:DestroyMovie(movie)
        end
    end
    goto LAB_00d405fc_c31
    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c31 end
    quest:NewScriptFrame(me)
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
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
    local raceMode = raceMode_ or 0
    raceMode_ = quest:PersistTransferInt(context, "RaceMode", raceMode)
end

-- ApprenticeSpeedTest.OnPredicateFail (retail 0x00d3e2b0)
function OnPredicateFail(quest, me)
end

