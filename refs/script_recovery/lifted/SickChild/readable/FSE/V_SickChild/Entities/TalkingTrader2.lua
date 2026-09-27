-- Readable native conversion: TalkingTrader2. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local timesTalkedTo_, timesHit

-- TalkingTrader2.Main (retail 0x00eccad0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue4, ctr_90, scratchValue, this_00, scratchValue13, scratchValue15, movie3
    local scratchValue17, line, line2
    scratchValue15 = 0
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
        quest:RemoveThing(me, false, true)
    end
    while not quest:IsActiveThreadTerminating() do
        if me:IsTalkedToByHero() then
            if not quest:IsActiveThreadTerminating() then
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
                        -- TODO(native): CCharString::operator=(&xStack_b0,"TEXT_QST_B10_TRADERB_REPEATB_10");
                        timesTalkedTo_ = 0xffffffff
                    end
                end
                goto FLOW_past_lab_00ecccaa
                ::LAB_00ecccaa::
                line = scratchValue
                ::FLOW_past_lab_00ecccaa::
                timesTalkedTo_ = timesTalkedTo_ + 1
                if not (talkingTrader1 ~= nil and not talkingTrader1:IsNull()) or not (talkingTrader1 ~= nil and talkingTrader1:IsAlive()) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00ecd272 end
                    local movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_00 then
                        me:Speak(hero, "TEXT_QST_B10_TRADERB_OTHERTRADERDEAD_10", GROUP_SELECT_FIRST, false, true, false)
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
                    goto LAB_00eccefc
                end
                goto FLOW_past_lab_00eccefc
                ::LAB_00eccefc::
                resources:DestroyMovie(this_00)
                goto LAB_00eccf0a
                ::FLOW_past_lab_00eccefc::
                if not quest:IsActiveThreadTerminating() then
                    local movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_0 then
                        me:Speak(hero, line, 0, false, true, false)
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
            goto FLOW_hoist_lab_00ecd29f_1
        end
        goto FLOW_past_lab_00ecd29f
        ::LAB_00ecd29f::
        ::FLOW_hoist_lab_00ecd29f_1::
        break
        ::FLOW_past_lab_00ecd29f::
        ::LAB_00eccf0a::
        local scratchValue14 = scratchValue15
        scratchValue15 = scratchValue15 | 1
        if me:MsgIsHitByHero() then goto LAB_00eccf8f end
        scratchValue13 = scratchValue14 | 3
        scratchValue15 = scratchValue13
        if me:MsgIsHitByAnySpecialAbilityFromHero() then
            scratchValue13 = scratchValue14 | 7
            scratchValue15 = scratchValue13
            if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00eccf8f end
        end
        goto LAB_00eccfad
        goto FLOW_past_lab_00eccfad
        ::LAB_00eccfad::
        scratchValue4 = 0
        ::FLOW_past_lab_00eccfad::
        goto FLOW_past_lab_00eccf8f
        ::LAB_00eccf8f::
        scratchValue4 = 1
        if quest:GetHealth(me) <= 0.0 then goto LAB_00eccfad end
        ::FLOW_past_lab_00eccf8f::
        if scratchValue13 & 4 ~= 0 then
            scratchValue13 = scratchValue13 & 0xfffffffb
            scratchValue15 = scratchValue13
        end
        if scratchValue13 & 2 ~= 0 then
            scratchValue13 = scratchValue13 & 0xfffffffd
            scratchValue15 = scratchValue13
        end
        if scratchValue13 & 1 ~= 0 then
            scratchValue15 = scratchValue13 & 0xfffffffe
        end
        if scratchValue4 == 0 then quest:NewScriptFrame(me); goto continue_4 end
        if quest:IsActiveThreadTerminating() then break end
        if 2 < timesHit then
            quest:EntitySetAsKillable(me, true, true)
            quest:EntitySetAsToAddToComboMultiplierWhenHit(me, true)
        end
        if not quest:TextEntryExists("TEXT_QST_B10_TRADERB_ONHIT_" .. tostring(ctr_90)) then
            ctr_90 = 10
            scratchValue17 = "TEXT_QST_B10_TRADERB_ONHIT_" .. tostring(10)
        end
        ctr_90 = ctr_90 + 10
        movie3 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
            me:Speak(hero, line2, 0, false, true, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00ecd292 end
            end
            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00ecd292 end
            goto FLOW_past_lab_00ecd292
            ::LAB_00ecd292::
            resources:DestroyMovie(movie3)
            goto LAB_00ecd29f
            ::FLOW_past_lab_00ecd292::
        end
        timesHit = timesHit + 1
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

