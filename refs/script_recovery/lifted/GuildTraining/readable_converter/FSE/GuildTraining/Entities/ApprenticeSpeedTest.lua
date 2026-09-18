-- Readable native conversion: ApprenticeSpeedTest. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    GUI_RaceTime = 3804,  -- 50.0
    GUI_RaceGold = 3808,  -- 25.0
}

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- ApprenticeSpeedTest.Main (retail 0x00d3e2e0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue6, scratchValue7, scratchValue, scratchValue13, scratchValue14, scratchValue15
    local switch, switch6, switch7, switch8, movie, scratchValue16, speedFriend
    local getNearestWithDefName, scratchValue18, scratchValue19, scratchValue20, movie2, movie3
    local movie4, addQuestInfoTimer, resource, timerId, timerId2, scratchValue21, movie5, movie6
    local movie7
    local function __region_LAB_00d40379_c32()
        scratchValue16 = quest:GetThingWithScriptName("RaceMarker")
        quest:MiniMapRemoveMarker(scratchValue16)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie7)
    end
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        resource = resources:NewResource()
        scratchValue6 = resources:TryAcquire(resource, me, 4)
        while not scratchValue6 do
            if not quest:NewScriptFrame(me) then goto LAB_00d4075b end
            scratchValue6 = resources:TryAcquire(resource, me, 4)
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
            me:SetFriendsWithEverythingFlag(me)
            if speedFriend ~= nil and not speedFriend:IsNull() then
                speedFriend:SetFriendsWithEverythingFlag(1)
            end
            state:SetInt("RaceMode", 0)
            scratchValue20 = 0
            timerId2 = quest:RegisterTimer()
            scratchValue13 = quest:RegisterTimer()
            scratchValue21 = scratchValue13
            timerId = quest:RegisterTimer()
            quest:SetTimer(scratchValue13, 1)
            scratchValue6 = quest:IsActiveThreadTerminating()
            while not scratchValue6 do
                if state:GetInt("RaceMode") == 0 then
                    scratchValue15 = scratchValue20 - 1
                    repeat
                        scratchValue13 = scratchValue15
                        if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                        if quest:GetStateInt("GameState") == 3 then
                            quest:SetThingHasInformation(me, false, true, false)
                        end
                        scratchValue16 = hero
                        if quest:IsDistanceBetweenThingsUnder(scratchValue16, me, 10.0) and quest:GetTimer(scratchValue21) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            scratchValue14 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue14, speedFriend)
                            if scratchValue13 < 5 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                switch = scratchValue13
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
                            scratchValue15 = scratchValue13 + 1
                        end
                        if me:IsTalkedToByHero() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            movie4 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            scratchValue16 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue16) then
                                scratchValue16 = hero
                                me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_BOAST", 0, false, true, false)
                                scratchValue7 = me:IsPerformingScriptTask()
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie4)
                                        quest:DeregisterTimer(timerId)
                                        quest:DeregisterTimer(scratchValue21)
                                        quest:DeregisterTimer(timerId2)
                                        goto LAB_00d40749
                                    end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    goto LAB_00d405fc
                                end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_BOAST_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while scratchValue13 < 0 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    quest:DeregisterTimer(timerId)
                                    quest:DeregisterTimer(scratchValue21)
                                    quest:DeregisterTimer(timerId2)
                                    resources:ReleaseResource(resource)
                                    return
                                end
                                scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00d405fc
                            end
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue13 == 1 then
                                if scratchValue6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    goto LAB_00d405fc
                                end
                                scratchValue16 = resources:ScriptThing(resource)
                                if 0.0 < quest:GetHealth(scratchValue16) then
                                    scratchValue16 = hero
                                    me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_RUN", 0, false, true, false)
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    while scratchValue7 do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d405d6 end
                                        scratchValue7 = me:IsPerformingScriptTask()
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie4)
                                        goto LAB_00d405fc
                                    end
                                end
                                state:SetInt("RaceMode", 1)
                                quest:SetStateBool("ReachedPlatform", false)
                                scratchValue16 = quest:GetThingWithScriptName("RaceMarker")
                                quest:MiniMapAddMarker(scratchValue16, "HUD_ORB_QUEST_VIGNETTE")
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
                                scratchValue16 = resources:ScriptThing(resource)
                                if 0.0 < quest:GetHealth(scratchValue16) then
                                    scratchValue16 = hero
                                    me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_REFUSE", 0, false, true, false)
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    while scratchValue7 do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d405d6 end
                                        scratchValue7 = me:IsPerformingScriptTask()
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
                    until state:GetInt("RaceMode") ~= 0
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    quest:DeregisterTimer(scratchValue21)
                    quest:DeregisterTimer(timerId2)
                    goto LAB_00d40749
                end
                scratchValue13 = state:GetInt("RaceMode")
                scratchValue19 = scratchValue20
                while true do
                    scratchValue20 = scratchValue19
                    if scratchValue13 ~= 1 then break end
                    if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                    scratchValue16 = hero
                    if quest:IsDistanceBetweenThingsUnder(scratchValue16, me, 10.0) and quest:GetTimer(scratchValue21) < 1 then
                        scratchValue13 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue13, speedFriend)
                        if scratchValue19 < 6 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            switch6 = scratchValue19
                            repeat
                                if switch6 == 1 then
                                    quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, speedFriend, false)
                                    break
                                elseif switch6 == 2 then
                                    quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", speedFriend, me, false)
                                    break
                                elseif switch6 == 3 then
                                    quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, speedFriend, false)
                                    break
                                elseif switch6 == 4 then
                                    quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", speedFriend, me, false)
                                    break
                                elseif switch6 == 5 then
                                    quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, speedFriend, false)
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
                                quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, speedFriend, false)
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", speedFriend, me, false)
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
                            quest:PauseAllNonScriptedEntities(10.0 ~= 0)
                            scratchValue16 = resources:ScriptThing(resource)
                            scratchValue = 0.0
                            if 0.0 < quest:GetHealth(scratchValue16) then
                                scratchValue16 = hero
                                me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM", 0, false, true, false)
                                scratchValue7 = me:IsPerformingScriptTask()
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie5)
                                        goto LAB_00d405fc
                                    end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie5)
                                    goto LAB_00d405fc
                                end
                            end
                            state:SetInt("RaceMode", 2)
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie5
                        elseif quest:GetTimer(timerId2) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            movie2 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            scratchValue16 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue16) then
                                scratchValue16 = hero
                                me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW", 0, false, true, false)
                                scratchValue7 = me:IsPerformingScriptTask()
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie2)
                                        goto LAB_00d405fc
                                    end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    goto LAB_00d405fc
                                end
                            end
                            state:SetInt("RaceMode", 2)
                            if not quest:GetStateBool("ReachedPlatform") then
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie2)
                                    goto LAB_00d405fc
                                end
                                scratchValue16 = quest:GetThingWithScriptName("RaceMarker")
                                quest:MiniMapRemoveMarker(scratchValue16)
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            movie = movie2
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            movie6 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(scratchValue ~= 0)
                            scratchValue16 = resources:ScriptThing(resource)
                            scratchValue = 0.0
                            if 0.0 < quest:GetHealth(scratchValue16) then
                                scratchValue16 = hero
                                me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED", 0, false, true, false)
                                scratchValue7 = me:IsPerformingScriptTask()
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie6)
                                        goto LAB_00d405fc
                                    end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie6)
                                    goto LAB_00d405fc
                                end
                            end
                            state:SetInt("RaceMode", 3)
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
                            scratchValue16 = quest:GetThingWithScriptName("RaceMarker")
                            quest:MiniMapRemoveMarker(scratchValue16)
                        end
                        state:SetInt("RaceMode", 2)
                    end
                    scratchValue19 = scratchValue20
                    scratchValue13 = state:GetInt("RaceMode")
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                if state:GetInt("RaceMode") == 2 then
                    scratchValue15 = scratchValue20 - 1
                    repeat
                        if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                        scratchValue16 = hero
                        if quest:IsDistanceBetweenThingsUnder(scratchValue16, me, 10.0) and quest:GetTimer(scratchValue21) < 1 then
                            scratchValue14 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue14, speedFriend)
                            scratchValue13 = scratchValue15
                            if scratchValue15 < 5 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                switch7 = scratchValue13
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
                            scratchValue15 = scratchValue13 + 1
                        end
                        if me:IsTalkedToByHero() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            movie3 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            scratchValue16 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue16) then
                                scratchValue16 = hero
                                me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL", 0, false, true, false)
                                scratchValue7 = me:IsPerformingScriptTask()
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie3)
                                        goto LAB_00d405fc
                                    end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while scratchValue13 < 0 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
                                scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00d405fc
                            end
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue13 == 1 then
                                if scratchValue6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
                                scratchValue16 = resources:ScriptThing(resource)
                                scratchValue = 0.0
                                if 0.0 < quest:GetHealth(scratchValue16) then
                                    scratchValue16 = hero
                                    me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_RUN", 0, false, true, false)
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    while scratchValue7 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(movie3)
                                            goto LAB_00d405fc
                                        end
                                        scratchValue7 = me:IsPerformingScriptTask()
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie3)
                                        goto LAB_00d405fc
                                    end
                                end
                                state:SetInt("RaceMode", 1)
                                quest:SetStateBool("ReachedPlatform", false)
                                quest:SetTimer(timerId2, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime))))
                                quest:SetTimer(timerId, math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(SCRIPT_DEF.GUI_RaceTime) + 20.0)))
                                addQuestInfoTimer = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                                scratchValue16 = quest:GetThingWithScriptName("RaceMarker")
                                quest:MiniMapAddMarker(scratchValue16, "HUD_ORB_QUEST_VIGNETTE")
                            else
                                if scratchValue6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00d405fc
                                end
                                scratchValue16 = resources:ScriptThing(resource)
                                scratchValue = 0.0
                                if 0.0 < quest:GetHealth(scratchValue16) then
                                    scratchValue16 = hero
                                    me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_REFUSE", 0, false, true, false)
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    while scratchValue7 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(movie3)
                                            goto LAB_00d405fc
                                        end
                                        scratchValue7 = me:IsPerformingScriptTask()
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
                    until state:GetInt("RaceMode") ~= 2
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                if state:GetInt("RaceMode") == 3 then
                    scratchValue15 = scratchValue20 - 1
                    scratchValue13 = scratchValue15
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue16 = hero
                        if quest:IsDistanceBetweenThingsUnder(scratchValue16, me, 10.0) and quest:GetTimer(scratchValue21) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            scratchValue14 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue14, speedFriend)
                            if scratchValue13 < 5 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                switch8 = scratchValue13
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
                            scratchValue15 = scratchValue13 + 1
                        end
                        if not me:IsTalkedToByHero() then goto LAB_00d403e1 end
                        if not quest:IsActiveThreadTerminating() then
                            movie7 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while scratchValue13 < 0 do
                                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d402b3 end
                                scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if quest:IsActiveThreadTerminating() then
                                -- LAB_00d40716: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                            else
                                scratchValue6 = quest:IsActiveThreadTerminating()
                                if scratchValue13 ~= 1 then
                                    if not scratchValue6 then
                                        scratchValue16 = resources:ScriptThing(resource)
                                        if 0.0 < quest:GetHealth(scratchValue16) then
                                            scratchValue16 = hero
                                            me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO", 0, false, true, false)
                                            scratchValue7 = me:IsPerformingScriptTask()
                                            while scratchValue7 do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d402b3 end
                                                scratchValue7 = me:IsPerformingScriptTask()
                                            end
                                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d402b3 end
                                        end
                                        scratchValue16 = quest:GetThingWithScriptName("RaceMarker")
                                        quest:MiniMapRemoveMarker(scratchValue16)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(movie7)
                                        goto LAB_00d403e1
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto FLOW_after_lab_00d402b3
                                end
                                if not scratchValue6 then
                                    scratchValue16 = resources:ScriptThing(resource)
                                    if 0.0 < quest:GetHealth(scratchValue16) then
                                        scratchValue16 = hero
                                        me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES", 0, false, true, false)
                                        scratchValue7 = me:IsPerformingScriptTask()
                                        while scratchValue7 do
                                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d402b3 end
                                            scratchValue7 = me:IsPerformingScriptTask()
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d402b3 end
                                    end
                                    scratchValue16 = quest:GetThingWithScriptName("RaceMarker")
                                    quest:MiniMapRemoveMarker(scratchValue16)
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
        resources:ReleaseResource(resource)
    end
    ::FLOW_after_lab_00d405fc::
    do return end
    ::LAB_00d405d6::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie4)
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(scratchValue21)
    quest:DeregisterTimer(timerId2)
    resources:ReleaseResource(resource)
    goto FLOW_after_lab_00d405fc
    ::LAB_00d403e1::
    if state:GetInt("RaceMode") ~= 3 then
        if not quest:IsActiveThreadTerminating() then
            quest:NewScriptFrame(me)
        end
        quest:DeregisterTimer(timerId)
        quest:DeregisterTimer(scratchValue21)
        quest:DeregisterTimer(timerId2)
        resources:ReleaseResource(resource)
        goto FLOW_after_lab_00d405fc
    end
    scratchValue13 = scratchValue15
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        scratchValue16 = hero
        if quest:IsDistanceBetweenThingsUnder(scratchValue16, me, 10.0) and quest:GetTimer(scratchValue21) < 1 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
            scratchValue14 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(scratchValue14, speedFriend)
            if scratchValue13 < 5 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
                switch8 = scratchValue13
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
            scratchValue15 = scratchValue13 + 1
        end
        if not me:IsTalkedToByHero() then goto LAB_00d403e1 end
        if not quest:IsActiveThreadTerminating() then
            movie7 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
            scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
            while scratchValue13 < 0 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d402b3_c32
                scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then
                -- LAB_00d40716_c32: (native jump target)
                quest:PauseAllNonScriptedEntities(false)
            else
                scratchValue6 = quest:IsActiveThreadTerminating()
                if scratchValue13 ~= 1 then
                    if not scratchValue6 then
                        scratchValue16 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scratchValue16) then
                            scratchValue16 = hero
                            me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO", 0, false, true, false)
                            scratchValue7 = me:IsPerformingScriptTask()
                            while scratchValue7 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d402b3_c32
                                scratchValue7 = me:IsPerformingScriptTask()
                            end
                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d40716_c32
                        end
                        __region_LAB_00d40379_c32(); goto LAB_00d403e1
                    end
                    -- TODO(native): goto LAB_00d40716_c32
                end
                if not scratchValue6 then
                    scratchValue16 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue16) then
                        scratchValue16 = hero
                        me:Speak(scratchValue16, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES", 0, false, true, false)
                        scratchValue7 = me:IsPerformingScriptTask()
                        while scratchValue7 do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then return end  -- TODO(native): goto LAB_00d40716_c32
                            scratchValue7 = me:IsPerformingScriptTask()
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
    resources:ReleaseResource(resource)
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

