-- Readable native conversion: TalkingTrader1. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local timesTalkedTo_, timesHit

-- TalkingTrader1.Main (retail 0x00ecbdf0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local ambientConversationIndex, predicateResult, dist, timerId, scratchValue3
    local nextAmbientConversationIndex, hero, scratchValue8, this_00, movie3, scratchValue9
    local scratchValue
    if not quest:NewScriptFrame(me) then return end
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
    local talkingTrader = quest:GetThingWithScriptName("TalkingTrader2")
    local timerId2 = quest:RegisterTimer()
    nextAmbientConversationIndex = 0
    scratchValue3 = 10
    if quest:GetStateBool("GotFishingSpotMushroom") then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId2)
            resources:ReleaseResource(resource)
            return
        end
        quest:RemoveThing(me, false, true)
    end
    predicateResult = quest:IsActiveThreadTerminating()
    ambientConversationIndex = 0
    while not predicateResult do
        if quest:GetTimer(timerId2) == 0 then
            dist = 20.0
            hero = quest:GetHero()
            local isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero, 20.0)
            local sequence = not isDistanceBetweenThingsUnder or not quest:IsDistanceBetweenThingsUnder(me, talkingTrader, 10.0)
            if sequence then goto LAB_00ecc220 end
            if not quest:IsActiveThreadTerminating() then
                local conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, talkingTrader)
                local switch1 = ambientConversationIndex
                repeat
                    if switch1 == 0 then
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERA_CHAT1_10", me, talkingTrader, false)
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERB_CHAT1_20", talkingTrader, me, false)
                        break
                    elseif switch1 == 1 then
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERA_CHAT2_10", me, talkingTrader, false)
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERB_CHAT2_20", talkingTrader, me, false)
                        break
                    elseif switch1 == 2 then
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERA_CHAT3_10", me, talkingTrader, false)
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERB_CHAT3_20", talkingTrader, me, false)
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERA_CHAT3_30", talkingTrader, me, false)
                        break
                    else
                        if switch1 == 3 then
                            quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERA_CHAT4_10", me, talkingTrader, false)
                            quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERB_CHAT4_20", talkingTrader, me, false)
                            goto LAB_00ecc1f4
                        else
                            if 2 < ambientConversationIndex then goto LAB_00ecc1f4 end
                        end
                        goto FLOW_past_lab_00ecc1f4
                        ::LAB_00ecc1f4::
                        if quest:IsActiveThreadTerminating() then goto LAB_00ecc928 end
                        nextAmbientConversationIndex = 0
                        goto LAB_00ecc20b
                        ::FLOW_past_lab_00ecc1f4::
                    end
                until true
                if not quest:IsActiveThreadTerminating() then nextAmbientConversationIndex = ((ambientConversationIndex + 1 + 0x80000000) & 0xffffffff) - 0x80000000; goto LAB_00ecc20b end
                goto FLOW_past_lab_00ecc20b
                ::LAB_00ecc20b::
                quest:SetTimer(timerId2, 16)
                goto LAB_00ecc220
                ::FLOW_past_lab_00ecc20b::
            end
            goto LAB_00ecc928
        end
        ::LAB_00ecc220::
        if me:IsTalkedToByHero() then
            if not quest:IsActiveThreadTerminating() then
                scratchValue = ""
                local timesTalkedTo = timesTalkedTo_
                if timesTalkedTo == 0 then
                    scratchValue8 = "TEXT_QST_B10_TRADERA_INTRO"
                    goto LAB_00ecc2be
                else
                    if timesTalkedTo == 1 then
                        scratchValue8 = "TEXT_QST_B10_TRADERA_REPEATA"
                        goto LAB_00ecc2be
                    end
                    if timesTalkedTo == 2 then
                        scratchValue = "TEXT_QST_B10_TRADERA_REPEATB_10"
                        timesTalkedTo_ = 0xffffffff
                    end
                end
                goto FLOW_past_lab_00ecc2be
                ::LAB_00ecc2be::
                scratchValue = scratchValue8
                ::FLOW_past_lab_00ecc2be::
                timerId = timerId2
                timesTalkedTo_ = timesTalkedTo_ + 1
                local timeRemaining = quest:GetTimer(timerId2)
                quest:SetTimer(timerId2, timeRemaining + 5)
                if not (talkingTrader ~= nil and talkingTrader:IsAlive()) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00ecc8f3 end
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    hero = nil
                    if resources:ResourceThingHealthAbove(resource, 0.0) then
                        hero = quest:GetHero()
                        if not me:Speak(hero, "TEXT_QST_B10_TRADERA_OTHERTRADERDEAD_10", GROUP_SELECT_FIRST, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00ecc923
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00ecc923
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00ecc8f3
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie
                else
                    goto FLOW_past_lab_00ecc53c
                end
                ::LAB_00ecc53c::
                resources:DestroyMovie(this_00)
                goto FLOW_past_lab_00ecc928
                ::FLOW_past_lab_00ecc53c::
                if not quest:IsActiveThreadTerminating() then
                    local movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    hero = nil
                    if resources:ResourceThingHealthAbove(resource, 0.0) then
                        local line = scratchValue
                        hero = quest:GetHero()
                        if not me:Speak(hero, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            goto LAB_00ecc923
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                goto LAB_00ecc923
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            goto LAB_00ecc923
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie2
                    goto LAB_00ecc53c
                end
                ::LAB_00ecc8f3::
                goto LAB_00ecc923
            end
            goto FLOW_hoist_lab_00ecc923_1
        end
        goto FLOW_past_lab_00ecc923
        ::LAB_00ecc923::
        goto LAB_00ecc928
        ::FLOW_hoist_lab_00ecc923_1::
        goto FLOW_hoist_lab_00ecc928_1
        ::FLOW_past_lab_00ecc923::
        goto FLOW_past_lab_00ecc928
        ::LAB_00ecc928::
        quest:DeregisterTimer(timerId2)
        resources:ReleaseResource(resource)
        do return end
        ::FLOW_hoist_lab_00ecc928_1::
        break
        ::FLOW_past_lab_00ecc928::
        if (resources:IsHitByHeroExceptAbilityAndHealthAbove(me, 14, 0.0) and 1 or 0) == 0 then quest:NewScriptFrame(me); predicateResult = quest:IsActiveThreadTerminating(); ambientConversationIndex = nextAmbientConversationIndex; goto continue_4 end
        if quest:IsActiveThreadTerminating() then goto LAB_00ecc928 end
        if 2 < timesHit then
            if quest:IsActiveThreadTerminating() then goto LAB_00ecc928 end
            quest:EntitySetAsKillable(me, true, true)
            quest:EntitySetAsToAddToComboMultiplierWhenHit(me, true)
        end
        scratchValue9 = "TEXT_QST_B10_TRADERA_ONHIT_" .. tostring(scratchValue3)
        if not quest:TextEntryExists(scratchValue9) then
            scratchValue3 = 10
            scratchValue9 = "TEXT_QST_B10_TRADERA_ONHIT_" .. tostring(10)
        end
        scratchValue3 = scratchValue3 + 10
        movie3 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        hero = nil
        if resources:ResourceThingHealthAbove(resource, 0.0) then
            local line2 = scratchValue9
            hero = quest:GetHero()
            if not me:Speak(hero, line2, 0, false, true, false) then
                quest:PauseAllNonScriptedEntities(false)
            else
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00ecc913 end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00ecc913 end
                goto FLOW_past_lab_00ecc913
            end
            ::LAB_00ecc913::
            resources:DestroyMovie(movie3)
            goto LAB_00ecc923
            ::FLOW_past_lab_00ecc913::
        end
        timesHit = timesHit + 1
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie3)
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
        ambientConversationIndex = nextAmbientConversationIndex
        ::continue_4::
    end
    quest:DeregisterTimer(timerId2)
    resources:ReleaseResource(resource)
end

-- TalkingTrader1.Init (retail 0x00ecbd50)
function Init(quest, me)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsAllowedToFollowHero(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    quest:SetThingPersistent(me, true)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    timesTalkedTo_ = 0
    timesHit = 0
end

-- TalkingTrader1.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TalkingTrader1.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

