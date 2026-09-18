-- Readable native conversion: ApprenticeSpeedTest. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_RaceTime = 3804,  -- 50.0
    GUI_RaceGold = 3808,  -- 25.0
}

-- per-entity fields (native class members; one Lua state per entity instance)
local raceMode

-- ApprenticeSpeedTest.Main (retail 0x00d3e2e0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue6, scratchValue, scratchValue14, scratchValue15, switch, switch6, switch7
    local switch8, movie, scratchValue16, speedFriend, getNearestWithDefName, scratchValue18
    local scratchValue19, scratchValue20, movie2, movie3, movie4, addQuestInfoTimer, timerId
    local timerId2, scratchValue21, movie5, movie6, movie7
    local function __region_LAB_00d40379_c32()
        local scratchValue16 = quest:GetThingWithScriptName("RaceMarker")
        quest:MiniMapRemoveMarker(scratchValue16)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie7)
    end
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        if not me:AcquireControl(4) then goto LAB_00d4075b end
        if not quest:IsActiveThreadTerminating() then
            quest:SetIsPushableByHero(me, false)
            speedFriend = quest:GetThingWithScriptName("SpeedFriend")
            getNearestWithDefName = quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE")
            quest:EntityAttachToVillage(me, getNearestWithDefName)
            quest:EntityAttachToVillage(speedFriend, getNearestWithDefName)
            quest:EntitySetAsKillable(me, false, true)
            quest:EntitySetAsKillable(speedFriend, false, true)
            quest:SetThingHasInformation(me, false, true, false)
            me:SetFriendsWithEverythingFlag(me)
            if speedFriend ~= nil and not speedFriend:IsNull() then
                speedFriend:SetFriendsWithEverythingFlag(1)
            end
            raceMode = 0
            scratchValue20 = 0
            timerId2 = quest:RegisterTimer()
            scratchValue21 = quest:RegisterTimer()
            timerId = quest:RegisterTimer()
            quest:SetTimer(scratchValue21, 1)
            scratchValue6 = quest:IsActiveThreadTerminating()
            while not scratchValue6 do
                if raceMode == 0 then
                    scratchValue15 = scratchValue20 - 1
                    repeat
                        scratchValue = scratchValue15
                        if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                        if quest:GetStateInt("GameState") == 3 then
                            quest:SetThingHasInformation(me, false, true, false)
                        end
                        if quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(scratchValue21) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            scratchValue14 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue14, speedFriend)
                            if scratchValue < 5 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                switch = scratchValue
                                repeat
                                    if switch == 0 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, speedFriend, false)
                                        break
                                    elseif switch == 1 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", speedFriend, me, false)
                                        break
                                    elseif switch == 2 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, speedFriend, false)
                                        break
                                    elseif switch == 3 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", speedFriend, me, false)
                                        break
                                    elseif switch == 4 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, speedFriend, false)
                                        break
                                    else
                                        goto FLOW_native_label_1
                                    end
                                until true
                            else
                                scratchValue19 = scratchValue20 & 0x80000001
                                scratchValue6 = scratchValue19 == 0
                                if scratchValue19 < 0 then
                                    scratchValue6 = (scratchValue19 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not scratchValue6 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, speedFriend, false)
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", speedFriend, me, false)
                                end
                            end
                            ::FLOW_native_label_1::
                            quest:SetTimer(scratchValue21, 3)
                            scratchValue20 = scratchValue20 + 1
                            scratchValue15 = scratchValue + 1
                        end
                        if me:IsTalkedToByHero() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            movie4 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(me) then
                                if not me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_BOAST", GROUP_SELECT_FIRST, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    quest:DeregisterTimer(timerId)
                                    quest:DeregisterTimer(scratchValue21)
                                    quest:DeregisterTimer(timerId2)
                                    goto LAB_00d40749
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    goto LAB_00d405fc
                                end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_BOAST_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                            while scratchValue < 0 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    quest:DeregisterTimer(timerId)
                                    quest:DeregisterTimer(scratchValue21)
                                    quest:DeregisterTimer(timerId2)
                                    me:ReleaseControl()
                                    return
                                end
                                scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00d405fc
                            end
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue == 1 then
                                if scratchValue6 then
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
                                raceMode = 1
                                quest:SetStateBool("ReachedPlatform", false)
                                quest:MiniMapAddMarker(quest:GetThingWithScriptName("RaceMarker"), "HUD_ORB_QUEST_VIGNETTE")
                                quest:SetTimer(timerId2, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime))))
                                quest:SetTimer(timerId, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime) + 20.0)))
                                addQuestInfoTimer = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                                quest:DisplayQuestInfo(true)
                            else
                                if scratchValue6 then
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
                        end
                    until raceMode ~= 0
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    quest:DeregisterTimer(scratchValue21)
                    quest:DeregisterTimer(timerId2)
                    goto LAB_00d40749
                end
                scratchValue = raceMode
                scratchValue19 = scratchValue20
                while true do
                    scratchValue20 = scratchValue19
                    if scratchValue ~= 1 then break end
                    if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                    if quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(scratchValue21) < 1 then
                        scratchValue = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue, speedFriend)
                        if scratchValue19 < 6 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            switch6 = scratchValue19
                            repeat
                                if switch6 == 1 then
                                    quest:AddLineToConversation(scratchValue, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, speedFriend, false)
                                    break
                                elseif switch6 == 2 then
                                    quest:AddLineToConversation(scratchValue, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", speedFriend, me, false)
                                    break
                                elseif switch6 == 3 then
                                    quest:AddLineToConversation(scratchValue, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, speedFriend, false)
                                    break
                                elseif switch6 == 4 then
                                    quest:AddLineToConversation(scratchValue, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", speedFriend, me, false)
                                    break
                                elseif switch6 == 5 then
                                    quest:AddLineToConversation(scratchValue, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, speedFriend, false)
                                    break
                                else
                                    goto FLOW_native_label_2
                                end
                            until true
                        else
                            scratchValue18 = scratchValue19 & 0x80000001
                            scratchValue6 = scratchValue18 == 0
                            if scratchValue18 < 0 then
                                scratchValue6 = (scratchValue18 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if not scratchValue6 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                quest:AddLineToConversation(scratchValue, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, speedFriend, false)
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                quest:AddLineToConversation(scratchValue, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", speedFriend, me, false)
                            end
                        end
                        ::FLOW_native_label_2::
                        quest:SetTimer(scratchValue21, 3)
                        scratchValue20 = scratchValue19 + 1
                    end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        quest:RemoveQuestInfoElement(addQuestInfoTimer)
                        quest:DisplayQuestInfo(false)
                        if not quest:GetStateBool("ReachedPlatform") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            movie5 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(me) then
                                if not me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM", GROUP_SELECT_FIRST, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie5)
                                    goto LAB_00d405fc
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie5)
                                    goto LAB_00d405fc
                                end
                            end
                            raceMode = 2
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie5
                        elseif quest:GetTimer(timerId2) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            movie2 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(me) then
                                if not me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW", GROUP_SELECT_FIRST, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    goto LAB_00d405fc
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    goto LAB_00d405fc
                                end
                            end
                            raceMode = 2
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
                            movie6 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(me) then
                                if not me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED", GROUP_SELECT_FIRST, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie6)
                                    goto LAB_00d405fc
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie6)
                                    goto LAB_00d405fc
                                end
                            end
                            raceMode = 3
                            quest:GiveHeroGold(math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceGold))))
                            quest:ClearThingHasInformation(me)
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie6
                        end
                        resources:DestroyMovie(movie)
                    end
                    if quest:GetTimer(timerId) < 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        quest:RemoveQuestInfoElement(addQuestInfoTimer)
                        quest:DisplayQuestInfo(false)
                        if not quest:GetStateBool("ReachedPlatform") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            quest:MiniMapRemoveMarker(quest:GetThingWithScriptName("RaceMarker"))
                        end
                        raceMode = 2
                    end
                    scratchValue19 = scratchValue20
                    scratchValue = raceMode
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                if raceMode == 2 then
                    scratchValue15 = scratchValue20 - 1
                    repeat
                        if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                        if quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(scratchValue21) < 1 then
                            scratchValue14 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue14, speedFriend)
                            scratchValue = scratchValue15
                            if scratchValue15 < 5 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                switch7 = scratchValue
                                repeat
                                    if switch7 == 0 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, speedFriend, false)
                                        break
                                    elseif switch7 == 1 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", speedFriend, me, false)
                                        break
                                    elseif switch7 == 2 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, speedFriend, false)
                                        break
                                    elseif switch7 == 3 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", speedFriend, me, false)
                                        break
                                    elseif switch7 == 4 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, speedFriend, false)
                                        break
                                    else
                                        goto FLOW_native_label_3
                                    end
                                until true
                            else
                                scratchValue19 = scratchValue20 & 0x80000001
                                scratchValue6 = scratchValue19 == 0
                                if scratchValue19 < 0 then
                                    scratchValue6 = (scratchValue19 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not scratchValue6 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, speedFriend, false)
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", speedFriend, me, false)
                                end
                            end
                            ::FLOW_native_label_3::
                            quest:SetTimer(scratchValue21, 3)
                            scratchValue20 = scratchValue20 + 1
                            scratchValue15 = scratchValue + 1
                        end
                        if me:IsTalkedToByHero() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            movie3 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            if 0.0 < quest:GetHealth(me) then
                                if not me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL", GROUP_SELECT_FIRST, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                            while scratchValue < 0 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
                                scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00d405fc
                            end
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue == 1 then
                                if scratchValue6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
                                if 0.0 < quest:GetHealth(me) then
                                    if not me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_RUN", GROUP_SELECT_FIRST, false, true, false) then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie3)
                                        goto LAB_00d405fc
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie3)
                                        goto LAB_00d405fc
                                    end
                                end
                                raceMode = 1
                                quest:SetStateBool("ReachedPlatform", false)
                                quest:SetTimer(timerId2, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime))))
                                quest:SetTimer(timerId, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime) + 20.0)))
                                addQuestInfoTimer = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                                quest:MiniMapAddMarker(quest:GetThingWithScriptName("RaceMarker"), "HUD_ORB_QUEST_VIGNETTE")
                            else
                                if scratchValue6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
                                if 0.0 < quest:GetHealth(me) then
                                    if not me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_REFUSE", GROUP_SELECT_FIRST, false, true, false) then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie3)
                                        goto LAB_00d405fc
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
                    until raceMode ~= 2
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                if raceMode == 3 then
                    scratchValue15 = scratchValue20 - 1
                    scratchValue = scratchValue15
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        if quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(scratchValue21) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            scratchValue14 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue14, speedFriend)
                            if scratchValue < 5 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                switch8 = scratchValue
                                repeat
                                    if switch8 == 0 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", speedFriend, me, false)
                                        break
                                    elseif switch8 == 1 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", me, speedFriend, false)
                                        break
                                    elseif switch8 == 2 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", speedFriend, me, false)
                                        break
                                    elseif switch8 == 3 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", me, speedFriend, false)
                                        break
                                    elseif switch8 == 4 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", speedFriend, me, false)
                                        break
                                    else
                                        goto FLOW_native_label_4
                                    end
                                until true
                            else
                                scratchValue19 = scratchValue20 & 0x80000001
                                scratchValue6 = scratchValue19 == 0
                                if scratchValue19 < 0 then
                                    scratchValue6 = (scratchValue19 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not scratchValue6 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", speedFriend, me, false)
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", me, speedFriend, false)
                                end
                            end
                            ::FLOW_native_label_4::
                            quest:SetTimer(scratchValue21, 7)
                            scratchValue20 = scratchValue20 + 1
                            scratchValue15 = scratchValue + 1
                        end
                        if not me:IsTalkedToByHero() then goto LAB_00d403e1 end
                        if not quest:IsActiveThreadTerminating() then
                            movie7 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                            while scratchValue < 0 do
                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d402b3 end
                                scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if quest:IsActiveThreadTerminating() then
                                -- LAB_00d40716: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                            else
                                scratchValue6 = quest:IsActiveThreadTerminating()
                                if scratchValue ~= 1 then
                                    if not scratchValue6 then
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
                                if not scratchValue6 then
                                    if 0.0 < quest:GetHealth(me) then
                                        if not me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false) then
                                            quest:PauseAllNonScriptedEntities(false)
                                            goto FLOW_after_lab_00d402b3
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
                quest:NewScriptFrame(me)
                scratchValue6 = quest:IsActiveThreadTerminating()
            end
            ::LAB_00d405fc::
            quest:DeregisterTimer(timerId)
            quest:DeregisterTimer(scratchValue21)
            quest:DeregisterTimer(timerId2)
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
    quest:DeregisterTimer(scratchValue21)
    quest:DeregisterTimer(timerId2)
    me:ReleaseControl()
    goto FLOW_after_lab_00d405fc
    ::LAB_00d403e1::
    if raceMode ~= 3 then
        if not quest:IsActiveThreadTerminating() then
            quest:NewScriptFrame(me)
        end
        quest:DeregisterTimer(timerId)
        quest:DeregisterTimer(scratchValue21)
        quest:DeregisterTimer(timerId2)
        me:ReleaseControl()
        goto FLOW_after_lab_00d405fc
    end
    scratchValue = scratchValue15
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        if quest:IsDistanceBetweenThingsUnder(hero, me, 10.0) and quest:GetTimer(scratchValue21) < 1 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
            scratchValue14 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(scratchValue14, speedFriend)
            if scratchValue < 5 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
                switch8 = scratchValue
                repeat
                    if switch8 == 0 then
                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", speedFriend, me, false)
                        break
                    elseif switch8 == 1 then
                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", me, speedFriend, false)
                        break
                    elseif switch8 == 2 then
                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", speedFriend, me, false)
                        break
                    elseif switch8 == 3 then
                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", me, speedFriend, false)
                        break
                    elseif switch8 == 4 then
                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", speedFriend, me, false)
                        break
                    else
                        goto FLOW_native_label_4_c32
                    end
                until true
            else
                scratchValue19 = scratchValue20 & 0x80000001
                scratchValue6 = scratchValue19 == 0
                if scratchValue19 < 0 then
                    scratchValue6 = (scratchValue19 - 1 | 0xfffffffe) == 0xffffffff
                end
                if not scratchValue6 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", speedFriend, me, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", me, speedFriend, false)
                end
            end
            ::FLOW_native_label_4_c32::
            quest:SetTimer(scratchValue21, 7)
            scratchValue20 = scratchValue20 + 1
            scratchValue15 = scratchValue + 1
        end
        if not me:IsTalkedToByHero() then goto LAB_00d403e1 end
        if not quest:IsActiveThreadTerminating() then
            movie7 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
            while scratchValue < 0 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d402b3_c32
                scratchValue = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d40716_c32: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
            else
                scratchValue6 = quest:IsActiveThreadTerminating()
                if scratchValue ~= 1 then
                    if not scratchValue6 then
                        if 0.0 < quest:GetHealth(me) then
                            me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO", GROUP_SELECT_FIRST, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d402b3_c32
                            end
                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d40716_c32
                        end
                        __region_LAB_00d40379_c32(); goto LAB_00d403e1
                    end
                    -- TODO(native): goto LAB_00d40716_c32
                end
                if not scratchValue6 then
                    if 0.0 < quest:GetHealth(me) then
                        me:Speak(hero, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d40716_c32
                        end
                        if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d402b3_c32
                    end
                    __region_LAB_00d40379_c32()
                    goto LAB_00d403e1
                end
                -- LAB_00d402b3_c32: (native jump target)
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
    quest:DeregisterTimer(scratchValue21)
    quest:DeregisterTimer(timerId2)
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

