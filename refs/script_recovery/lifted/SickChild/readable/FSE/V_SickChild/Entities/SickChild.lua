-- Readable native conversion: SickChild. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- SickChild.Main (retail 0x00ec5de0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, predicateResult8, scratchValue3, scratchValue4, scratchValue6, fret_0
    local fret_01, this_00, scratchValue, scratchValue19, movie2, scratchValue21
    scratchValue19 = 0
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
    local timerId = quest:RegisterTimer()
    local getSleepingPositionAndOrientationFromBed = quest:GetSleepingPositionAndOrientationFromBed(me, quest:GetThingWithScriptName("SickChildBed"))
    local scratchValue5 = math.atan(scratchValue4,scratchValue6)
    if (scratchValue5 * 0.15915493667125702 < 0.0) or 1.0 <= (scratchValue5 * 0.15915493667125702) then
        if fret_0 < 0.0 then
            -- TODO(native): xStack_8c = (CCharString)(f_stk_8c + 1.0);
        end
    end
    quest:EntityTeleportToPosition(me, nil --[[missing]], false, false)
    quest:ReadGlobalGameDataString(1864)
    me:PlayLoopingAnimation(getSleepingPositionAndOrientationFromBed, -1, false, false, false)
    quest:SetBedAvailability(quest:GetThingWithScriptName("SickChildBed"), false)
    quest:SetThingAsUsable(quest:GetThingWithScriptName("SickChildBed"), false)
    quest:EntitySetTargetable(me, false)
    quest:EntitySetAsRespondToHit(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    while not quest:GetStateBool("FinishedQuest") do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        if quest:GetTimer(timerId) ~= 0 then goto LAB_00ec6108 end
        if not quest:IsDistanceBetweenThingsUnder(me, hero, 14.0) then goto LAB_00ec6108 end
        goto LAB_00ec6137
        goto FLOW_past_lab_00ec6137
        ::LAB_00ec6137::
        predicateResult = true
        ::FLOW_past_lab_00ec6137::
        goto FLOW_past_lab_00ec6108
        ::LAB_00ec6108::
        scratchValue19 = scratchValue19 | 1
        if me:IsTalkedToByHero() then goto LAB_00ec6137 end
        predicateResult = false
        ::FLOW_past_lab_00ec6108::
        if scratchValue19 & 1 ~= 0 then
            scratchValue19 = scratchValue19 & 0xfffffffe
        end
        if predicateResult then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            local conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            scratchValue21 = "TEXT_QST_B10_BOY_JABBERS_" .. tostring(10)
            if not quest:TextEntryExists(scratchValue21) then
                scratchValue21 = "TEXT_QST_B10_BOY_JABBERS_" .. tostring(10)
            end
            -- TODO(native): xStack_80 = (CCharString)((int)CVar10 + 0xa);
            quest:AddLineToConversation(conversationId, scratchValue21, me, hero, false)
            quest:SetTimer(timerId, 20)
        end
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
    quest:SetBedAvailability(quest:GetThingWithScriptName("SickChildBed"), true)
    quest:EntityTeleportToThing(me, quest:GetThingWithScriptName("MK_SC_BOY"), false)
    me:ClearAllActionsIncludingLoopingAnimations()
    quest:EntitySetAsRespondToHit(me, true)
    quest:EntitySetTargetable(me, true)
    predicateResult8 = quest:IsActiveThreadTerminating()
    while true do
        if predicateResult8 then
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        if me:IsTalkedToByHero() then break end
        local scratchValue17 = scratchValue19
        scratchValue19 = scratchValue19 | 2
        if me:MsgIsHitByHero() then
            goto LAB_00ec6567
        else
            scratchValue = scratchValue17 | 6
            scratchValue19 = scratchValue
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue = scratchValue17 | 14
                scratchValue19 = scratchValue
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00ec6567 end
            end
            scratchValue3 = 0
        end
        goto FLOW_past_lab_00ec6567
        ::LAB_00ec6567::
        scratchValue3 = 1
        ::FLOW_past_lab_00ec6567::
        if scratchValue & 8 ~= 0 then
            scratchValue = scratchValue & 0xfffffff7
            scratchValue19 = scratchValue
        end
        if scratchValue & 4 ~= 0 then
            scratchValue = scratchValue & 0xfffffffb
            scratchValue19 = scratchValue
        end
        if scratchValue & 2 ~= 0 then
            scratchValue19 = scratchValue & 0xfffffffd
        end
        if scratchValue3 == 0 then quest:NewScriptFrame(me); predicateResult8 = quest:IsActiveThreadTerminating(); goto continue_3 end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        fret_01 = quest:GetHealth(resources:ScriptThing(resource))
        if 0.0 < fret_01 then
            me:Speak(hero, "TEXT_QST_B10_MOTHER_HIT", GROUP_SELECT_FIRST, false, true, false)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00ec677d end
            end
            if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00ec677d end
            goto FLOW_past_lab_00ec677d
            ::LAB_00ec677d::
            this_00 = movie2
            goto LAB_00ec6781
            ::FLOW_past_lab_00ec677d::
        end
        quest:EntitySetThingAsAllyOfThing(me, hero)
        quest:EntitySetThingAsAllyOfThing(hero, me)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie2)
        quest:NewScriptFrame(me)
        predicateResult8 = quest:IsActiveThreadTerminating()
        ::continue_3::
    end
    ::FLOW_after_lab_00ec64df::
    if not quest:IsActiveThreadTerminating() then
        local movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
        if fret_00 <= 0.0 then
            goto LAB_00ec64c5
        end
        goto FLOW_past_lab_00ec64c5
        ::LAB_00ec64c5::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        scratchValue19 = scratchValue19 | 2
        if me:MsgIsHitByHero() then
            scratchValue3 = 1
        end
        if scratchValue & 8 ~= 0 then
            scratchValue = scratchValue & 0xfffffff7
            scratchValue19 = scratchValue
        end
        if scratchValue & 4 ~= 0 then
            scratchValue = scratchValue & 0xfffffffb
            scratchValue19 = scratchValue
        end
        if scratchValue & 2 ~= 0 then
            scratchValue19 = scratchValue & 0xfffffffd
        end
        if scratchValue3 ~= 0 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            local movie3 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            fret_01 = quest:GetHealth(resources:ScriptThing(resource))
            if 0.0 < fret_01 then
                me:Speak(hero, "TEXT_QST_B10_MOTHER_HIT", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00ec677d_c2 end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00ec677d_c2 end
                goto FLOW_past_lab_00ec677d_c2
                ::LAB_00ec677d_c2::
                this_00 = movie3
                goto LAB_00ec6781
                ::FLOW_past_lab_00ec677d_c2::
            end
            quest:EntitySetThingAsAllyOfThing(me, hero)
            quest:EntitySetThingAsAllyOfThing(hero, me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
        end
        quest:NewScriptFrame(me)
        goto FLOW_after_lab_00ec64df
        ::FLOW_past_lab_00ec64c5::
        me:Speak(hero, "TEXT_QST_B10_BOY_CURED_10", GROUP_SELECT_FIRST, false, true, false)
        while me:IsPerformingScriptTask() do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                this_00 = movie
                goto LAB_00ec6781
            end
        end
        if not quest:IsActiveThreadTerminating() then goto LAB_00ec64c5 end
        quest:PauseAllNonScriptedEntities(false)
        this_00 = movie
        goto LAB_00ec6781
    end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00ec6781::
    resources:DestroyMovie(this_00)
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
end

-- SickChild.Init (retail 0x00ec5da0)
function Init(quest, me)
    quest:SetIsPushableByHero(me, false)
end

-- SickChild.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- SickChild.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

