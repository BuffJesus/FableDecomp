-- Readable native conversion: PreMeleeWhisper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- PreMeleeWhisper.Main (retail 0x00d524a0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue3, switch, p4, p5, hero4, hero5, scratchValue5
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(1)
    while not quest:GetStateBool("WhisperCutsceneFinished") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local resource = resources:NewResource()
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    me:FollowThing(hero, 1.0, true)
    while not quest:GetStateBool("WhisperStopFollowing") do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d52e1b end
            local movie = resources:StartMovie("")
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities(true)
            me:ClearCommands()
            local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
            if 0.0 < fret_0 then
                p5 = 0
                p4 = 1
                me:Speak(hero, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CHAT", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
            end
            me:FollowThing(hero, 1.0, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        local preMeleeChatMarker = quest:GetNearestWithScriptName(me, "PreMeleeChatMarker")
        hero4 = hero
        -- TODO(native): xStack_a0 = (float)puVar8[2] - *(float *)(pCVar6 + 0x8);
        if quest:IsDistanceBetweenThingsUnder(me, preMeleeChatMarker, 7.0) then
            hero5 = hero
            local isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero, 7.0)
            local sequence = not isDistanceBetweenThingsUnder or 5 < quest:GetTimer(timerId) or ABS(resource) < 1.0 == (ABS(resource) == 1.0)
            if sequence then goto LAB_00d52d56 end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            -- TODO(native): xStack_a0 = (float)tonumber(pvVar7);
            scratchValue5 = 0
            local preMeleeChatMarker2 = quest:GetAllThingsWithScriptName("PreMeleeChatMarker")
            local scratchValue = 0 - preMeleeChatMarker2 >> 31
            if (0 - preMeleeChatMarker2) / 12 + scratchValue ~= scratchValue then
                scratchValue3 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        return
                    end
                    -- TODO(native): pvVar7 = (**(*(iVar10 + xStack_8c) + 0xc))(xStack_20)
    --[[unresolved native value]]
                    if tonumber(nil) ~= resource then scratchValue5 = scratchValue5 + 1; scratchValue3 = scratchValue3 + 12; goto continue_3 end
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        return
                    end
                    quest:RemoveThing(hero, preMeleeChatMarker2 + scratchValue3, false)
                    scratchValue5 = scratchValue5 + 1
                    scratchValue3 = scratchValue3 + 12
                    ::continue_3::
                until scratchValue5 >= ((0 - preMeleeChatMarker2) / 12)
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            local conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:SetTimer(timerId, 10)
            switch = resource
            repeat
                if resource == 2 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_LIBRARY", me, hero, false)
                    break
                elseif resource == 4 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SHOP", me, hero, false)
                    break
                elseif resource == 5 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CLOISTERS", me, hero, false)
                    break
                elseif resource == 6 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE1", me, hero, false)
                    break
                elseif resource == 7 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE2", me, hero, false)
                    break
                elseif resource == 8 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE3", me, hero, false)
                    break
                elseif resource == 9 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE4", me, hero, false)
                    break
                elseif resource == 10 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAZE", me, hero, false)
                    break
                elseif resource == 11 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WILL", me, hero, false)
                    break
                elseif resource == 13 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WOODS", me, hero, false)
                    break
                elseif resource == 14 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SKILL", me, hero, false)
                    break
                elseif resource == 15 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SERVANTS", me, hero, false)
                    break
                elseif resource == 16 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAIN_DORM", me, hero, false)
                    break
                elseif resource == 19 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DOOR", me, hero, false)
                    break
                elseif resource == 20 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DINING_ROOM", me, hero, false)
                else
                    goto FLOW_native_label_1
                end
            until true
            ::FLOW_native_label_1::
        end
        ::LAB_00d52d56::
    end
    if not quest:IsActiveThreadTerminating() then goto LAB_00d52e1b end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d52e1b::
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
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

