-- Readable native conversion: TalkingTrader1. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local timesTalkedTo_, timesHit

-- TalkingTrader1.Main (retail 0x00ecbdf0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, scratchValue3, fret_02, scratchValue5, scratchValue, this_00
    local scratchValue15, scratchValue16, movie3, line, line2
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
    local timerId = quest:RegisterTimer()
    scratchValue5 = 10
    if quest:GetStateBool("GotFishingSpotMushroom") then
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        quest:RemoveThing(me, false, true)
    end
    predicateResult = quest:IsActiveThreadTerminating()
    while not predicateResult do
        if quest:GetTimer(timerId) == 0 then
            local isDistanceBetweenThingsUnder = quest:IsDistanceBetweenThingsUnder(me, hero, 20.0)
            local sequence = not isDistanceBetweenThingsUnder or not quest:IsDistanceBetweenThingsUnder(me, talkingTrader, 10.0)
            if sequence then goto LAB_00ecc220 end
            if not quest:IsActiveThreadTerminating() then
                local conversationId = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId, talkingTrader)
                local switch1 = 0
                repeat
                    if switch1 == 0 then
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERA_CHAT1_10", me, talkingTrader, false)
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERB_CHAT1_20", talkingTrader, me, false)
                        break
                    elseif switch1 == 1 then
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERA_CHAT2_10", hero, nil --[[missing]])
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERB_CHAT2_20", me, nil --[[missing]], false)
                        break
                    elseif switch1 == 2 then
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERA_CHAT3_10", nil --[[missing]], nil --[[missing]])
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERB_CHAT3_20", me, nil --[[missing]], false)
                        quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERA_CHAT3_30", me, nil --[[missing]], false)
                        break
                    else
                        if switch1 == 3 then
                            quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERA_CHAT4_10", nil --[[missing]], nil --[[missing]])
                            quest:AddLineToConversation(conversationId, "TEXT_QST_B10_TRADERB_CHAT4_20", me, nil --[[missing]], false)
                            goto LAB_00ecc1f4
                        else
                            if 2 < 0 then goto LAB_00ecc1f4 end
                        end
                        goto FLOW_past_lab_00ecc1f4
                        ::LAB_00ecc1f4::
                        if quest:IsActiveThreadTerminating() then goto LAB_00ecc928 end
                        goto LAB_00ecc20b
                        ::FLOW_past_lab_00ecc1f4::
                    end
                until true
                if not quest:IsActiveThreadTerminating() then
                    -- TODO(native): xStack_ac = (CCharString)((int)CVar4 + 1);
                    goto LAB_00ecc20b
                end
                goto FLOW_past_lab_00ecc20b
                ::LAB_00ecc20b::
                quest:SetTimer(timerId, 16)
                goto LAB_00ecc220
                ::FLOW_past_lab_00ecc20b::
            end
            goto LAB_00ecc928
        end
        ::LAB_00ecc220::
        if me:IsTalkedToByHero() then
            if not quest:IsActiveThreadTerminating() then
                local timesTalkedTo = timesTalkedTo_
                if timesTalkedTo == 0 then
                    scratchValue = "TEXT_QST_B10_TRADERA_INTRO"
                    goto LAB_00ecc2be
                else
                    if timesTalkedTo == 1 then
                        scratchValue = "TEXT_QST_B10_TRADERA_REPEATA"
                        goto LAB_00ecc2be
                    end
                    if timesTalkedTo == 2 then
                        -- TODO(native): CCharString::operator=(&xStack_d8,"TEXT_QST_B10_TRADERA_REPEATB_10");
                        timesTalkedTo_ = 0xffffffff
                    end
                end
                goto FLOW_past_lab_00ecc2be
                ::LAB_00ecc2be::
                line2 = scratchValue
                ::FLOW_past_lab_00ecc2be::
                timesTalkedTo_ = timesTalkedTo_ + 1
                quest:SetTimer(timerId, quest:GetTimer(timerId) + 5)
                if not (talkingTrader ~= nil and talkingTrader:IsAlive()) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00ecc8f3 end
                    local movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_00 then
                        me:Speak(hero, "TEXT_QST_B10_TRADERA_OTHERTRADERDEAD_10", GROUP_SELECT_FIRST, false, true, false)
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
                            goto LAB_00ecc8f3
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie2
                    goto LAB_00ecc53c
                end
                goto FLOW_past_lab_00ecc53c
                ::LAB_00ecc53c::
                resources:DestroyMovie(this_00)
                goto LAB_00ecc54a
                ::FLOW_past_lab_00ecc53c::
                if not quest:IsActiveThreadTerminating() then
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_0 then
                        me:Speak(hero, line2, 0, false, true, false)
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
                            goto LAB_00ecc923
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie
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
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource)
        do return end
        ::FLOW_hoist_lab_00ecc928_1::
        break
        ::FLOW_past_lab_00ecc928::
        ::LAB_00ecc54a::
        -- TODO(native): uStack_b0 = uStack_b0 | 1;
        if me:MsgIsHitByHero() then goto LAB_00ecc5ce end
        scratchValue15 = scratchValue16 | 3
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            scratchValue15 = scratchValue16 | 7
            local scratchValue2 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
            if not scratchValue2 then goto LAB_00ecc5ce end
        end
        goto LAB_00ecc5ec
        goto FLOW_past_lab_00ecc5ec
        ::LAB_00ecc5ec::
        scratchValue3 = 0
        ::FLOW_past_lab_00ecc5ec::
        goto FLOW_past_lab_00ecc5ce
        ::LAB_00ecc5ce::
        scratchValue3 = 1
        if quest:GetHealth(me) <= 0.0 then goto LAB_00ecc5ec end
        ::FLOW_past_lab_00ecc5ce::
        if scratchValue15 & 4 ~= 0 then
            scratchValue15 = scratchValue15 & 0xfffffffb
        end
        if scratchValue15 & 2 ~= 0 then
            scratchValue15 = scratchValue15 & 0xfffffffd
        end
        if scratchValue15 & 1 ~= 0 then
            -- TODO(native): uStack_b0 = uVar13 & 0xfffffffe;
        end
        if scratchValue3 == 0 then quest:NewScriptFrame(me); predicateResult = quest:IsActiveThreadTerminating(); goto continue_4 end
        if quest:IsActiveThreadTerminating() then goto LAB_00ecc928 end
        if 2 < timesHit then
            if quest:IsActiveThreadTerminating() then goto LAB_00ecc928 end
            quest:EntitySetAsKillable(me, true, true)
            quest:EntitySetAsToAddToComboMultiplierWhenHit(me, true)
        end
        line = "TEXT_QST_B10_TRADERA_ONHIT_" .. tostring(scratchValue5)
        if not quest:TextEntryExists(line) then
            scratchValue5 = 10
            line = "TEXT_QST_B10_TRADERA_ONHIT_" .. tostring(10)
        end
        scratchValue5 = scratchValue5 + 10
        movie3 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        fret_02 = quest:GetHealth(resources:ScriptThing(resource))
        if 0.0 < fret_02 then
            me:Speak(hero, line, 0, false, true, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00ecc913 end
            end
            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00ecc913 end
            goto FLOW_past_lab_00ecc913
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
        ::continue_4::
    end
    quest:DeregisterTimer(timerId)
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

