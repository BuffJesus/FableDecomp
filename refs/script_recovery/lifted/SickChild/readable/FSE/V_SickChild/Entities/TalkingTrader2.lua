-- Readable native conversion: TalkingTrader2. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- per-entity fields (native class members; one Lua state per entity instance)
local timesTalkedTo_, timesHit

-- TalkingTrader2.Main (retail 0x00eccad0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local ctr_90, hitLine, hero3, scratchValue, this_00, movie3, scratchValue8
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
    local talkingTrader1 = quest:GetThingWithScriptName("TalkingTrader1")
    ctr_90 = 10
    if quest:GetStateBool("GotFishingSpotMushroom") then
        if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
        quest:RemoveThing(me, false, true)
    end
    while not quest:IsActiveThreadTerminating() do
        if me:IsTalkedToByHero() then
            if not quest:IsActiveThreadTerminating() then
                scratchValue8 = ""
                local timesTalkedTo = timesTalkedTo_
                if timesTalkedTo == 0 then
                    scratchValue = "TEXT_QST_B10_TRADERB_INTRO"
                    goto LAB_00ecccaa
                else
                    if timesTalkedTo == 1 then
                        scratchValue = "TEXT_QST_B10_TRADERB_REPEATA"
                        goto LAB_00ecccaa
                    end
                    if timesTalkedTo == 2 then
                        scratchValue8 = "TEXT_QST_B10_TRADERB_REPEATB_10"
                        timesTalkedTo_ = 0xffffffff
                    end
                end
                goto FLOW_past_lab_00ecccaa
                ::LAB_00ecccaa::
                scratchValue8 = scratchValue
                ::FLOW_past_lab_00ecccaa::
                timesTalkedTo_ = (timesTalkedTo_ + 1 + 2147483648) % 4294967296 - 2147483648
                if not (talkingTrader1 ~= nil and not talkingTrader1:IsNull()) or not (talkingTrader1 ~= nil and talkingTrader1:IsAlive()) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00ecd272 end
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    hero3 = nil
                    if resources:ResourceThingHealthAbove(resource, 0.0) then
                        hero3 = quest:GetHero()
                        if not me:Speak(hero3, "TEXT_QST_B10_TRADERB_OTHERTRADERDEAD_10", GROUP_SELECT_FIRST, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00ecd29f
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                goto LAB_00ecd29f
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00ecd272
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie
                else
                    goto FLOW_past_lab_00eccefc
                end
                ::LAB_00eccefc::
                resources:DestroyMovie(this_00)
                goto FLOW_past_lab_00ecd29f
                ::FLOW_past_lab_00eccefc::
                if not quest:IsActiveThreadTerminating() then
                    local movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    hero3 = nil
                    if resources:ResourceThingHealthAbove(resource, 0.0) then
                        local line = scratchValue8
                        local hero = quest:GetHero()
                        if not me:Speak(hero, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            goto LAB_00ecd29f
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                goto LAB_00ecd29f
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            goto LAB_00ecd29f
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    this_00 = movie2
                    goto LAB_00eccefc
                end
                ::LAB_00ecd272::
            end
            goto LAB_00ecd29f
        end
        goto FLOW_past_lab_00ecd29f
        ::LAB_00ecd29f::
        break
        ::FLOW_past_lab_00ecd29f::
        if (resources:IsHitByHeroExceptAbilityAndHealthAbove(me, 14, 0.0) and 1 or 0) == 0 then quest:NewScriptFrame(me); goto continue_4 end
        if quest:IsActiveThreadTerminating() then break end
        if 2 < timesHit then
            if quest:IsActiveThreadTerminating() then break end
            quest:EntitySetAsKillable(me, true, true)
            quest:EntitySetAsToAddToComboMultiplierWhenHit(me, true)
        end
        hitLine = "TEXT_QST_B10_TRADERB_ONHIT_" .. tostring(ctr_90)
        if not quest:TextEntryExists(hitLine) then
            ctr_90 = 10
            hitLine = "TEXT_QST_B10_TRADERB_ONHIT_" .. tostring(10)
        end
        ctr_90 = ctr_90 + 10
        movie3 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        hero3 = nil
        if resources:ResourceThingHealthAbove(resource, 0.0) then
            local line2 = hitLine
            local hero2 = quest:GetHero()
            if not me:Speak(hero2, line2, 0, false, true, false) then
                quest:PauseAllNonScriptedEntities(false)
            else
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00ecd292 end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00ecd292 end
                goto FLOW_past_lab_00ecd292
            end
            ::LAB_00ecd292::
            resources:DestroyMovie(movie3)
            goto LAB_00ecd29f
            ::FLOW_past_lab_00ecd292::
        end
        timesHit = (timesHit + 1 + 2147483648) % 4294967296 - 2147483648
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie3)
        quest:NewScriptFrame(me)
        ::continue_4::
    end
    resources:ReleaseResource(resource)
end

-- TalkingTrader2.Init (retail 0x00ecca30)
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

-- TalkingTrader2.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TalkingTrader2.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

