-- Readable native conversion: ApprenticeSpeedTest. Review coverage report before use.
-- Registration remains disabled until the package is verified.

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
    local resources = quest:RetailResources()
    local scratchValue6, scratchValue7, scratchValue13, scratchValue14, scratchValue15, switch5
    local switch6, switch7, switch8, scratchValue16, scratchValue18, scratchValue20, scratchValue21
    local scratchValue22, scratchValue23, scratchValue24, scratchValue25, scratchValue26
    local scratchValue27, scratchValue28, scratchValue29, timerId, timerId2, scratchValue35
    local scratchValue36, scratchValue38
    local function __region_LAB_00d40379_c32()
        scratchValue18 = quest:GetThingWithScriptName("RaceMarker")
        quest:MiniMapRemoveMarker(scratchValue18)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(scratchValue38)
    end
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        scratchValue29 = resources:NewResource()
        scratchValue6 = resources:TryAcquire(scratchValue29, me, 4)
        while not scratchValue6 do
            if not quest:NewScriptFrame(me) then goto LAB_00d4075b end
            scratchValue6 = resources:TryAcquire(scratchValue29, me, 4)
        end
        if not quest:IsActiveThreadTerminating() then
            quest:SetIsPushableByHero(me, false)
            scratchValue20 = quest:GetThingWithScriptName("SpeedFriend")
            scratchValue21 = quest:GetNearestWithDefName(me, "VILLAGE_GUILD_COMPLEX_INSIDE")
            quest:EntityAttachToVillage(me, scratchValue21)
            quest:EntityAttachToVillage(scratchValue20, scratchValue21)
            quest:EntitySetAsKillable(me, false, true)
            quest:EntitySetAsKillable(scratchValue20, false, true)
            quest:SetThingHasInformation(me, false, true, false)
            me:SetFriendsWithEverythingFlag(me)
            if scratchValue20 ~= nil and not scratchValue20:IsNull() then
                scratchValue20:SetFriendsWithEverythingFlag(1)
            end
            state:SetInt("RaceMode", 0)
            scratchValue24 = 0
            timerId2 = quest:RegisterTimer()
            scratchValue13 = quest:RegisterTimer()
            timerId = quest:RegisterTimer()
            quest:SetTimer(scratchValue13, 1)
            scratchValue6 = quest:IsActiveThreadTerminating()
            while not scratchValue6 do
                if state:GetInt("RaceMode") == 0 then
                    scratchValue15 = scratchValue24 - 1
                    repeat
                        scratchValue13 = scratchValue15
                        if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                        if quest:GetStateInt("GameState") == 3 then
                            quest:SetThingHasInformation(me, false, true, false)
                        end
                        scratchValue18 = quest:GetHero()
                        if quest:IsDistanceBetweenThingsUnder(scratchValue18, me, 10.0) and quest:GetTimer(scratchValue13) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            scratchValue14 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue14, scratchValue20)
                            if scratchValue13 < 5 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                switch5 = scratchValue13
                                repeat
                                    if switch5 == 0 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, scratchValue20, false)
                                        break
                                    elseif switch5 == 1 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", scratchValue20, me, false)
                                        break
                                    elseif switch5 == 2 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, scratchValue20, false)
                                        break
                                    elseif switch5 == 3 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", scratchValue20, me, false)
                                        break
                                    elseif switch5 == 4 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, scratchValue20, false)
                                        break
                                    else
                                        goto FLOW_native_label_1
                                    end
                                until true
                            else
                                scratchValue23 = scratchValue24 & 0x80000001
                                scratchValue6 = scratchValue23 == 0
                                if scratchValue23 < 0 then
                                    scratchValue6 = (scratchValue23 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not scratchValue6 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, scratchValue20, false)
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", scratchValue20, me, false)
                                end
                            end
                            ::FLOW_native_label_1::
                            quest:SetTimer(xStack_258, 3)
                            scratchValue24 = scratchValue24 + 1
                            scratchValue15 = scratchValue13 + 1
                        end
                        if me:IsTalkedToByHero() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            scratchValue27 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            scratchValue18 = resources:ScriptThing(scratchValue29)
                            if (0.0 < quest:GetHealth(scratchValue18)) ~= 0 then
                                scratchValue18 = quest:GetHero()
                                me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_BOAST", 0, false, true, false)
                                scratchValue7 = me:IsPerformingScriptTask()
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue27)
                                        quest:DeregisterTimer(timerId)
                                        quest:DeregisterTimer(xStack_258)
                                        quest:DeregisterTimer(timerId2)
                                        goto LAB_00d40749
                                    end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue27)
                                    goto LAB_00d405fc
                                end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_BOAST_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while scratchValue13 < 0 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue27)
                                    quest:DeregisterTimer(timerId)
                                    quest:DeregisterTimer(xStack_258)
                                    quest:DeregisterTimer(timerId2)
                                    resources:ReleaseResource(scratchValue29)
                                    return
                                end
                                scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue27)
                                goto LAB_00d405fc
                            end
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue13 == 1 then
                                if scratchValue6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue27)
                                    goto LAB_00d405fc
                                end
                                scratchValue18 = resources:ScriptThing(scratchValue29)
                                if (0.0 < quest:GetHealth(scratchValue18)) ~= 0 then
                                    scratchValue18 = quest:GetHero()
                                    me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_RUN", 0, false, true, false)
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    while scratchValue7 do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d405d6 end
                                        scratchValue7 = me:IsPerformingScriptTask()
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue27)
                                        goto LAB_00d405fc
                                    end
                                end
                                state:SetInt("RaceMode", 1)
                                quest:SetStateBool("ReachedPlatform", false)
                                scratchValue18 = quest:GetThingWithScriptName("RaceMarker")
                                quest:MiniMapAddMarker(scratchValue18, "HUD_ORB_QUEST_VIGNETTE")
                                quest:SetTimer(timerId2, math.modf(quest:ReadGlobalGameDataFloat(3804)))
                                quest:SetTimer(timerId, math.modf(quest:ReadGlobalGameDataFloat(3804) + 20.0))
                                scratchValue28 = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                                quest:DisplayQuestInfo(true)
                            else
                                if scratchValue6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue27)
                                    goto LAB_00d405fc
                                end
                                scratchValue18 = resources:ScriptThing(scratchValue29)
                                if (0.0 < quest:GetHealth(scratchValue18)) ~= 0 then
                                    scratchValue18 = quest:GetHero()
                                    me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_REFUSE", 0, false, true, false)
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    while scratchValue7 do
                                        if not quest:NewScriptFrame(me) then goto LAB_00d405d6 end
                                        scratchValue7 = me:IsPerformingScriptTask()
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue27)
                                        goto LAB_00d405fc
                                    end
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue27)
                        end
                    until state:GetInt("RaceMode") ~= 0
                end
                if quest:IsActiveThreadTerminating() then
                    quest:DeregisterTimer(timerId)
                    quest:DeregisterTimer(xStack_258)
                    quest:DeregisterTimer(timerId2)
                    goto LAB_00d40749
                end
                scratchValue13 = state:GetInt("RaceMode")
                scratchValue23 = scratchValue24
                while true do
                    scratchValue24 = scratchValue23
                    if scratchValue13 ~= 1 then break end
                    if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                    scratchValue18 = quest:GetHero()
                    if quest:IsDistanceBetweenThingsUnder(scratchValue18, me, 10.0) and quest:GetTimer(xStack_258) < 1 then
                        scratchValue13 = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(scratchValue13, scratchValue20)
                        if scratchValue23 < 6 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            switch6 = scratchValue23
                            repeat
                                if switch6 == 1 then
                                    quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, scratchValue20, false)
                                    break
                                elseif switch6 == 2 then
                                    quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", scratchValue20, me, false)
                                    break
                                elseif switch6 == 3 then
                                    quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, scratchValue20, false)
                                    break
                                elseif switch6 == 4 then
                                    quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", scratchValue20, me, false)
                                    break
                                elseif switch6 == 5 then
                                    quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, scratchValue20, false)
                                    break
                                else
                                    goto FLOW_native_label_2
                                end
                            until true
                        else
                            scratchValue22 = scratchValue23 & 0x80000001
                            scratchValue6 = scratchValue22 == 0
                            if scratchValue22 < 0 then
                                scratchValue6 = (scratchValue22 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if not scratchValue6 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, scratchValue20, false)
                            else
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                quest:AddLineToConversation(scratchValue13, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", scratchValue20, me, false)
                            end
                        end
                        ::FLOW_native_label_2::
                        quest:SetTimer(xStack_258, 3)
                        scratchValue24 = scratchValue23 + 1
                    end
                    if me:IsTalkedToByHero() then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        quest:RemoveQuestInfoElement(scratchValue28)
                        quest:DisplayQuestInfo(false)
                        if not quest:GetStateBool("ReachedPlatform") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            scratchValue35 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            scratchValue18 = resources:ScriptThing(scratchValue29)
                            if 0.0 < fret_02 then
                                scratchValue18 = quest:GetHero()
                                me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_NO_PLATFORM", 0, false, true, false)
                                scratchValue7 = me:IsPerformingScriptTask()
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue35)
                                        goto LAB_00d405fc
                                    end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue35)
                                    goto LAB_00d405fc
                                end
                            end
                            state:SetInt("RaceMode", 2)
                            quest:PauseAllNonScriptedEntities(false)
                            scratchValue16 = scratchValue35
                        elseif quest:GetTimer(timerId2) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            scratchValue25 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            scratchValue18 = resources:ScriptThing(scratchValue29)
                            if (0.0 < quest:GetHealth(scratchValue18)) ~= 0 then
                                scratchValue18 = quest:GetHero()
                                me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_TOO_SLOW", 0, false, true, false)
                                scratchValue7 = me:IsPerformingScriptTask()
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue25)
                                        goto LAB_00d405fc
                                    end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue25)
                                    goto LAB_00d405fc
                                end
                            end
                            state:SetInt("RaceMode", 2)
                            if not quest:GetStateBool("ReachedPlatform") then
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue25)
                                    goto LAB_00d405fc
                                end
                                scratchValue18 = quest:GetThingWithScriptName("RaceMarker")
                                quest:MiniMapRemoveMarker(scratchValue18)
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            scratchValue16 = scratchValue25
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            scratchValue36 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            scratchValue18 = resources:ScriptThing(scratchValue29)
                            if 0.0 < fret_04 then
                                scratchValue18 = quest:GetHero()
                                me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED", 0, false, true, false)
                                scratchValue7 = me:IsPerformingScriptTask()
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue36)
                                        goto LAB_00d405fc
                                    end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue36)
                                    goto LAB_00d405fc
                                end
                            end
                            state:SetInt("RaceMode", 3)
                            quest:GiveHeroGold(math.modf(quest:ReadGlobalGameDataFloat(3808)))
                            quest:ClearThingHasInformation(me)
                            quest:PauseAllNonScriptedEntities(false)
                            scratchValue16 = scratchValue36
                        end
                        resources:DestroyMovie(scratchValue16)
                    end
                    if quest:GetTimer(timerId) < 1 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                        quest:RemoveQuestInfoElement(scratchValue28)
                        quest:DisplayQuestInfo(false)
                        if not quest:GetStateBool("ReachedPlatform") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            scratchValue18 = quest:GetThingWithScriptName("RaceMarker")
                            quest:MiniMapRemoveMarker(scratchValue18)
                        end
                        state:SetInt("RaceMode", 2)
                    end
                    scratchValue23 = scratchValue24
                    scratchValue13 = state:GetInt("RaceMode")
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                if state:GetInt("RaceMode") == 2 then
                    scratchValue15 = scratchValue24 - 1
                    repeat
                        if not quest:NewScriptFrame(me) then goto LAB_00d405fc end
                        scratchValue18 = quest:GetHero()
                        if quest:IsDistanceBetweenThingsUnder(scratchValue18, me, 10.0) and quest:GetTimer(xStack_258) < 1 then
                            scratchValue14 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue14, scratchValue20)
                            scratchValue13 = scratchValue15
                            if scratchValue15 < 5 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                switch7 = scratchValue13
                                repeat
                                    if switch7 == 0 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIRST_LINE", me, scratchValue20, false)
                                        break
                                    elseif switch7 == 1 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SECOND_LINE", scratchValue20, me, false)
                                        break
                                    elseif switch7 == 2 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_THIRD_LINE", me, scratchValue20, false)
                                        break
                                    elseif switch7 == 3 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_FOURTH_LINE", scratchValue20, me, false)
                                        break
                                    elseif switch7 == 4 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_FIFTH_LINE", me, scratchValue20, false)
                                        break
                                    else
                                        goto FLOW_native_label_3
                                    end
                                until true
                            else
                                scratchValue23 = scratchValue24 & 0x80000001
                                scratchValue6 = scratchValue23 == 0
                                if scratchValue23 < 0 then
                                    scratchValue6 = (scratchValue23 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not scratchValue6 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_PRE_ARGUE_SEVENTH_LINE", me, scratchValue20, false)
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_PRE_ARGUE_SIXTH_LINE", scratchValue20, me, false)
                                end
                            end
                            ::FLOW_native_label_3::
                            quest:SetTimer(xStack_258, 3)
                            scratchValue24 = scratchValue24 + 1
                            scratchValue15 = scratchValue13 + 1
                        end
                        if me:IsTalkedToByHero() then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            scratchValue26 = resources:StartMovie("")
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            scratchValue18 = resources:ScriptThing(scratchValue29)
                            if 0.0 < fret_05 then
                                scratchValue18 = quest:GetHero()
                                me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL", 0, false, true, false)
                                scratchValue7 = me:IsPerformingScriptTask()
                                while scratchValue7 do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue26)
                                        goto LAB_00d405fc
                                    end
                                    scratchValue7 = me:IsPerformingScriptTask()
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue26)
                                    goto LAB_00d405fc
                                end
                            end
                            quest:GiveHeroYesNoQuestion("TEXT_QST_028_FAST_APPRENTICE_RETURN_FAIL_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
                            scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                            while scratchValue13 < 0 do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue26)
                                    goto LAB_00d405fc
                                end
                                scratchValue13 = quest:MsgIsQuestionAnsweredYesOrNo()
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue26)
                                goto LAB_00d405fc
                            end
                            scratchValue6 = quest:IsActiveThreadTerminating()
                            if scratchValue13 == 1 then
                                if scratchValue6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue26)
                                    goto LAB_00d405fc
                                end
                                scratchValue18 = resources:ScriptThing(scratchValue29)
                                if 0.0 < fret_06 then
                                    scratchValue18 = quest:GetHero()
                                    me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_RUN", 0, false, true, false)
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    while scratchValue7 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(scratchValue26)
                                            goto LAB_00d405fc
                                        end
                                        scratchValue7 = me:IsPerformingScriptTask()
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue26)
                                        goto LAB_00d405fc
                                    end
                                end
                                state:SetInt("RaceMode", 1)
                                quest:SetStateBool("ReachedPlatform", false)
                                quest:SetTimer(timerId2, math.modf(quest:ReadGlobalGameDataFloat(3804)))
                                quest:SetTimer(timerId, math.modf(quest:ReadGlobalGameDataFloat(3804) + 20.0))
                                scratchValue28 = quest:AddQuestInfoTimer(timerId, "HUD_CLOCK_ICON", 1.0)
                                scratchValue18 = quest:GetThingWithScriptName("RaceMarker")
                                quest:MiniMapAddMarker(scratchValue18, "HUD_ORB_QUEST_VIGNETTE")
                            else
                                if scratchValue6 then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue26)
                                    goto LAB_00d405fc
                                end
                                scratchValue18 = resources:ScriptThing(scratchValue29)
                                if 0.0 < fret_07 then
                                    scratchValue18 = quest:GetHero()
                                    me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_REFUSE", 0, false, true, false)
                                    scratchValue7 = me:IsPerformingScriptTask()
                                    while scratchValue7 do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(scratchValue26)
                                            goto LAB_00d405fc
                                        end
                                        scratchValue7 = me:IsPerformingScriptTask()
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue26)
                                        goto LAB_00d405fc
                                    end
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(scratchValue26)
                        end
                    until state:GetInt("RaceMode") ~= 2
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                if state:GetInt("RaceMode") == 3 then
                    scratchValue15 = scratchValue24 - 1
                    scratchValue13 = scratchValue15
                    quest:NewScriptFrame(me)
                    if not quest:IsActiveThreadTerminating() then
                        scratchValue18 = quest:GetHero()
                        if quest:IsDistanceBetweenThingsUnder(scratchValue18, me, 10.0) and quest:GetTimer(xStack_258) < 1 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                            scratchValue14 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue14, scratchValue20)
                            if scratchValue13 < 5 then
                                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                switch8 = scratchValue13
                                repeat
                                    if switch8 == 0 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", scratchValue20, me, false)
                                        break
                                    elseif switch8 == 1 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", me, scratchValue20, false)
                                        break
                                    elseif switch8 == 2 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", scratchValue20, me, false)
                                        break
                                    elseif switch8 == 3 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", me, scratchValue20, false)
                                        break
                                    elseif switch8 == 4 then
                                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", scratchValue20, me, false)
                                        break
                                    else
                                        goto FLOW_native_label_4
                                    end
                                until true
                            else
                                scratchValue23 = scratchValue24 & 0x80000001
                                scratchValue6 = scratchValue23 == 0
                                if scratchValue23 < 0 then
                                    scratchValue6 = (scratchValue23 - 1 | 0xfffffffe) == 0xffffffff
                                end
                                if not scratchValue6 then
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", scratchValue20, me, false)
                                else
                                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc end
                                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", me, scratchValue20, false)
                                end
                            end
                            ::FLOW_native_label_4::
                            quest:SetTimer(xStack_258, 7)
                            scratchValue24 = scratchValue24 + 1
                            scratchValue15 = scratchValue13 + 1
                        end
                        if not me:IsTalkedToByHero() then goto LAB_00d403e1 end
                        if not quest:IsActiveThreadTerminating() then
                            scratchValue38 = resources:StartMovie("")
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
                                        scratchValue18 = resources:ScriptThing(scratchValue29)
                                        if (0.0 < quest:GetHealth(scratchValue18)) ~= 0 then
                                            scratchValue18 = quest:GetHero()
                                            me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO", 0, false, true, false)
                                            scratchValue7 = me:IsPerformingScriptTask()
                                            while scratchValue7 do
                                                if not quest:NewScriptFrame(me) then goto LAB_00d402b3 end
                                                scratchValue7 = me:IsPerformingScriptTask()
                                            end
                                            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d402b3 end
                                        end
                                        scratchValue18 = quest:GetThingWithScriptName("RaceMarker")
                                        quest:MiniMapRemoveMarker(scratchValue18)
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(scratchValue38)
                                        goto LAB_00d403e1
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    goto FLOW_after_lab_00d402b3
                                end
                                if not scratchValue6 then
                                    scratchValue18 = resources:ScriptThing(scratchValue29)
                                    if (0.0 < quest:GetHealth(scratchValue18)) ~= 0 then
                                        scratchValue18 = quest:GetHero()
                                        me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES", 0, false, true, false)
                                        scratchValue7 = me:IsPerformingScriptTask()
                                        while scratchValue7 do
                                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto FLOW_after_lab_00d402b3 end
                                            scratchValue7 = me:IsPerformingScriptTask()
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d402b3 end
                                    end
                                    scratchValue18 = quest:GetThingWithScriptName("RaceMarker")
                                    quest:MiniMapRemoveMarker(scratchValue18)
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(scratchValue38)
                                    goto LAB_00d403e1
                                end
                                ::LAB_00d402b3::
                                quest:PauseAllNonScriptedEntities(false)
                            end
                            ::FLOW_after_lab_00d402b3::
                            resources:DestroyMovie(scratchValue38)
                        end
                    end
                    goto LAB_00d405fc
                end
                quest:NewScriptFrame(me)
                scratchValue6 = quest:IsActiveThreadTerminating()
            end
            ::LAB_00d405fc::
            quest:DeregisterTimer(timerId)
            quest:DeregisterTimer(xStack_258)
            quest:DeregisterTimer(timerId2)
            ::LAB_00d40749::
        end
        ::LAB_00d4075b::
        resources:ReleaseResource(scratchValue29)
    end
    ::FLOW_after_lab_00d405fc::
    do return end
    ::LAB_00d405d6::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(scratchValue27)
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(xStack_258)
    quest:DeregisterTimer(timerId2)
    resources:ReleaseResource(scratchValue29)
    goto FLOW_after_lab_00d405fc
    ::LAB_00d403e1::
    if state:GetInt("RaceMode") ~= 3 then
        if not quest:IsActiveThreadTerminating() then
            quest:NewScriptFrame(me)
        end
        quest:DeregisterTimer(timerId)
        quest:DeregisterTimer(xStack_258)
        quest:DeregisterTimer(timerId2)
        resources:ReleaseResource(scratchValue29)
        goto FLOW_after_lab_00d405fc
    end
    scratchValue13 = scratchValue15
    quest:NewScriptFrame(me)
    if not quest:IsActiveThreadTerminating() then
        scratchValue18 = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(scratchValue18, me, 10.0) and quest:GetTimer(xStack_258) < 1 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
            scratchValue14 = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(scratchValue14, scratchValue20)
            if scratchValue13 < 5 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
                switch8 = scratchValue13
                repeat
                    if switch8 == 0 then
                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIRST_LINE", scratchValue20, me, false)
                        break
                    elseif switch8 == 1 then
                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SECOND_LINE", me, scratchValue20, false)
                        break
                    elseif switch8 == 2 then
                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_THIRD_LINE", scratchValue20, me, false)
                        break
                    elseif switch8 == 3 then
                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_FOURTH_LINE", me, scratchValue20, false)
                        break
                    elseif switch8 == 4 then
                        quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_FIFTH_LINE", scratchValue20, me, false)
                        break
                    else
                        goto FLOW_native_label_4_c32
                    end
                until true
            else
                scratchValue23 = scratchValue24 & 0x80000001
                scratchValue6 = scratchValue23 == 0
                if scratchValue23 < 0 then
                    scratchValue6 = (scratchValue23 - 1 | 0xfffffffe) == 0xffffffff
                end
                if not scratchValue6 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_FRIEND_POST_ARGUE_SEVENTH_LINE", scratchValue20, me, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
                    quest:AddLineToConversation(scratchValue14, "TEXT_QST_028_FAST_APPRENTICE_POST_ARGUE_SIXTH_LINE", me, scratchValue20, false)
                end
            end
            ::FLOW_native_label_4_c32::
            quest:SetTimer(xStack_258, 7)
            scratchValue24 = scratchValue24 + 1
            scratchValue15 = scratchValue13 + 1
        end
        if not me:IsTalkedToByHero() then goto LAB_00d403e1 end
        if not quest:IsActiveThreadTerminating() then
            scratchValue38 = resources:StartMovie("")
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
                        scratchValue18 = resources:ScriptThing(scratchValue29)
                        if (0.0 < quest:GetHealth(scratchValue18)) ~= 0 then
                            scratchValue18 = quest:GetHero()
                            me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_NO", 0, false, true, false)
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
                    scratchValue18 = resources:ScriptThing(scratchValue29)
                    if (0.0 < quest:GetHealth(scratchValue18)) ~= 0 then
                        scratchValue18 = quest:GetHero()
                        me:Speak(scratchValue18, "TEXT_QST_028_FAST_APPRENTICE_SUCCEED_QUESTION_YES", 0, false, true, false)
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
            resources:DestroyMovie(scratchValue38)
        end
    end
    goto LAB_00d405fc_c32
    if quest:IsActiveThreadTerminating() then goto LAB_00d405fc_c32 end
    quest:NewScriptFrame(me)
    ::LAB_00d405fc_c32::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(xStack_258)
    quest:DeregisterTimer(timerId2)
    resources:ReleaseResource(scratchValue29)
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

