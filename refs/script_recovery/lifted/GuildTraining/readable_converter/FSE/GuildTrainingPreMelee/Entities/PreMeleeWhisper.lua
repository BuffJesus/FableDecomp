-- Readable native conversion: PreMeleeWhisper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- PreMeleeWhisper.Main (retail 0x00d524a0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local scratchValue3, scratchValue4, conversationId, switch2, hero5, getNearestWithScriptName
    local timerId, scratchValue7, scratchValue8, getAllThingsWithScriptName, scratchValue9
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(1)
    while not quest:GetStateBool("WhisperCutsceneFinished") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    scratchValue9 = resources:NewResource()
    while not resources:TryAcquire(scratchValue9, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(scratchValue9); return end
    end
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    me:FollowThing(quest:GetHero(), 0x3f800000, true)
    while not quest:GetStateBool("WhisperStopFollowing") do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(scratchValue9)
            return
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d52e1b end
            scratchValue8 = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            me:ClearCommands()
            quest:GetHealth(resources:ScriptThing(scratchValue9))
            if 0.0 < fret_0 then
                me:Speak(quest:GetHero(), "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CHAT", 0, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(scratchValue8)
                        quest:DeregisterTimer(xStack_a4)
                        resources:ReleaseResource(scratchValue9)
                        return
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(scratchValue8)
                    quest:DeregisterTimer(xStack_a4)
                    resources:ReleaseResource(scratchValue9)
                    return
                end
            end
            me:FollowThing(quest:GetHero(), 0x3f800000, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(scratchValue8)
        end
        getNearestWithScriptName = quest:GetNearestWithScriptName(me, "PreMeleeChatMarker")
        -- TODO(native): xStack_a0 = (float)puVar8[2] - *(float *)(pCVar6 + 0x8);
        if quest:IsDistanceBetweenThingsUnder(me, getNearestWithScriptName, 7.0) then
            hero5 = quest:GetHero()
            if not quest:IsDistanceBetweenThingsUnder(me, hero5, 7.0) or 5 < quest:GetTimer(timerId) or ABS(scratchValue9) < _DAT_0122ded8 == (ABS(scratchValue9) == _DAT_0122ded8) then goto LAB_00d52d56 end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(xStack_a4)
                resources:ReleaseResource(scratchValue9)
                return
            end
            -- TODO(native): xStack_a0 = (float)GFCharStringToInt(pvVar7);
            scratchValue7 = 0
            getAllThingsWithScriptName = quest:GetAllThingsWithScriptName("PreMeleeChatMarker")
            scratchValue3 = 0 - getAllThingsWithScriptName >> 31
            if (0 - getAllThingsWithScriptName) / 12 + scratchValue3 ~= scratchValue3 then
                scratchValue4 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(xStack_a4)
                        resources:ReleaseResource(scratchValue9)
                        return
                    end
                    if GFCharStringToInt((**(*(scratchValue4 + getAllThingsWithScriptName) + 12))(xStack_20)) == scratchValue9 then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(xStack_a4)
                            resources:ReleaseResource(scratchValue9)
                            return
                        end
                        quest:RemoveThing(hero5, getAllThingsWithScriptName + scratchValue4, false)
                    end
                    scratchValue7 = scratchValue7 + 1
                    scratchValue4 = scratchValue4 + 12
                until scratchValue7 >= ((0 - getAllThingsWithScriptName) / 12)
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(xStack_a4)
                resources:ReleaseResource(scratchValue9)
                return
            end
            conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, quest:GetHero())
            quest:SetTimer(timerId, 10)
            switch2 = scratchValue9
            repeat
                if switch2 == 2 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_LIBRARY", me, quest:GetHero(), false)
                    break
                elseif switch2 == 4 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SHOP", me, quest:GetHero(), false)
                    break
                elseif switch2 == 5 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CLOISTERS", me, quest:GetHero(), false)
                    break
                elseif switch2 == 6 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE1", me, quest:GetHero(), false)
                    break
                elseif switch2 == 7 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE2", me, quest:GetHero(), false)
                    break
                elseif switch2 == 8 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE3", me, quest:GetHero(), false)
                    break
                elseif switch2 == 9 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE4", me, quest:GetHero(), false)
                    break
                elseif switch2 == 10 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAZE", me, quest:GetHero(), false)
                    break
                elseif switch2 == 11 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WILL", me, quest:GetHero(), false)
                    break
                elseif switch2 == 13 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WOODS", me, quest:GetHero(), false)
                    break
                elseif switch2 == 14 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SKILL", me, quest:GetHero(), false)
                    break
                elseif switch2 == 15 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SERVANTS", me, quest:GetHero(), false)
                    break
                elseif switch2 == 16 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAIN_DORM", me, quest:GetHero(), false)
                    break
                elseif switch2 == 19 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DOOR", me, quest:GetHero(), false)
                    break
                elseif switch2 == 20 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DINING_ROOM", me, quest:GetHero(), false)
                else
                    goto FLOW_native_label_1
                end
            until true
            ::FLOW_native_label_1::
        end
        ::LAB_00d52d56::
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(scratchValue9)
        return
    end
    ::LAB_00d52e1b::
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(scratchValue9)
end

-- PreMeleeWhisper.Init (retail 0x00d522e0)
function Init(quest, me)
end

-- PreMeleeWhisper.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- PreMeleeWhisper.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

