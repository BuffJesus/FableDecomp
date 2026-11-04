-- Readable native conversion: SickChild. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    SickChildSleepingAnim = 1864,  -- 'CS_SICK_CHILD_SHIVER'
}

-- SickChild.Main (retail 0x00ec5de0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, predicateResult8, finishedQuest, c_stk_81_1, c_stk_81_2, dist
    local scratchValue7, jabberIndex, jabberNextIndex, hero, hero3, scratchValue9, this_00, movie2
    local scratchValue15
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
    local sickChildBed = quest:GetThingWithScriptName("SickChildBed")
    local getSleepingPositionAndOrientationFromBed = quest:GetSleepingPositionAndOrientationFromBed(me, sickChildBed)
    local scratchValue16 = getSleepingPositionAndOrientationFromBed and getSleepingPositionAndOrientationFromBed.pos or {x = 0.0, y = 0.0, z = 0.0}
    local scratchValue4 = getSleepingPositionAndOrientationFromBed and getSleepingPositionAndOrientationFromBed.orient.x or 0.0
    local scratchValue6 = getSleepingPositionAndOrientationFromBed and getSleepingPositionAndOrientationFromBed.orient.y or 0.0
    local scratchValue5 = math.atan(scratchValue4,scratchValue6)
    scratchValue7 = scratchValue5 * 0.15915493667125702
    if (scratchValue5 * 0.15915493667125702 < 0.0) or 1.0 <= scratchValue7 then
        local fret_0 = math.fmod(scratchValue7, 1.0)
        scratchValue7 = fret_0
        if fret_0 < 0.0 then
            scratchValue7 = scratchValue7 + 1.0
        end
    end
    quest:EntityTeleportToPosition(me, scratchValue16, scratchValue7, false, false)
    local scratchValue = quest:ReadGlobalGameDataString(SCRIPT_DEF.SickChildSleepingAnim)
    resources:PlayLoopingAnimation(resource, scratchValue, -1, false, false, false, true, true, false, false)
    local sickChildBed2 = quest:GetThingWithScriptName("SickChildBed")
    quest:SetBedAvailability(sickChildBed2, false)
    local sickChildBed3 = quest:GetThingWithScriptName("SickChildBed")
    quest:SetThingAsUsable(sickChildBed3, false)
    quest:EntitySetTargetable(me, false)
    quest:EntitySetAsRespondToHit(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    finishedQuest = quest:GetStateBool("FinishedQuest")
    jabberNextIndex = 10
    while not finishedQuest do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        if quest:GetTimer(timerId) ~= 0 then goto LAB_00ec6108 end
        dist = 14.0
        hero3 = quest:GetHero()
        if not quest:IsDistanceBetweenThingsUnder(me, hero3, 14.0) then goto LAB_00ec6108 end
        goto LAB_00ec6137
        goto FLOW_past_lab_00ec6137
        ::LAB_00ec6137::
        predicateResult = true
        ::FLOW_past_lab_00ec6137::
        goto FLOW_past_lab_00ec6108
        ::LAB_00ec6108::
        if me:IsTalkedToByHero() then goto LAB_00ec6137 end
        predicateResult = false
        ::FLOW_past_lab_00ec6108::
        if not predicateResult then
            finishedQuest = quest:GetStateBool("FinishedQuest")
        else
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            local conversationId = quest:AddNewConversation(me, false, false)
            local hero4 = quest:GetHero()
            quest:AddPersonToConversation(conversationId, hero4)
            jabberIndex = jabberNextIndex
            scratchValue15 = "TEXT_QST_B10_BOY_JABBERS_" .. tostring(jabberIndex)
            if not quest:TextEntryExists(scratchValue15) then
                jabberIndex = 10
                scratchValue15 = "TEXT_QST_B10_BOY_JABBERS_" .. tostring(10)
            end
            jabberNextIndex = ((jabberIndex + 10 + 0x80000000) & 0xffffffff) - 0x80000000
            local hero5 = quest:GetHero()
            quest:AddLineToConversation(conversationId, scratchValue15, me, hero5, false)
            quest:SetTimer(timerId, 20)
            finishedQuest = quest:GetStateBool("FinishedQuest")
        end
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
    local sickChildBed4 = quest:GetThingWithScriptName("SickChildBed")
    quest:SetBedAvailability(sickChildBed4, true)
    scratchValue9 = quest:GetThingWithScriptName("MK_SC_BOY")
    quest:EntityTeleportToThing(me, scratchValue9, false)
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
        if me:MsgIsHitByHero() then
            goto LAB_00ec6567
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00ec6567 end
            c_stk_81_1 = 0
        end
        goto FLOW_past_lab_00ec6567
        ::LAB_00ec6567::
        c_stk_81_1 = 1
        ::FLOW_past_lab_00ec6567::
        if c_stk_81_1 == 0 then
            quest:NewScriptFrame(me);
            predicateResult8 = quest:IsActiveThreadTerminating();
            goto continue_3
        end
        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
        movie2 = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        scratchValue9 = nil
        if resources:ResourceThingHealthAbove(resource, 0.0) then
            local hero6 = quest:GetHero()
            if not me:Speak(hero6, "TEXT_QST_B10_MOTHER_HIT", GROUP_SELECT_FIRST, false, true, false) then
                quest:PauseAllNonScriptedEntities(false)
            else
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00ec677d end
                end
                if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00ec677d end
                goto FLOW_past_lab_00ec677d
            end
            ::LAB_00ec677d::
            this_00 = movie2
            goto LAB_00ec6781
            ::FLOW_past_lab_00ec677d::
        end
        scratchValue9 = quest:GetHero()
        quest:EntitySetThingAsAllyOfThing(me, scratchValue9)
        hero = quest:GetHero()
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
        scratchValue9 = nil
        if not (not resources:ResourceThingHealthAbove(resource, 0.0)) then
            goto FLOW_past_lab_00ec64c5
        end
        ::LAB_00ec64c5::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        if me:MsgIsHitByHero() then
            goto LAB_00ec6567_c2
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00ec6567_c2 end
            c_stk_81_2 = 0
        end
        goto FLOW_past_lab_00ec6567_c2
        ::LAB_00ec6567_c2::
        c_stk_81_2 = 1
        ::FLOW_past_lab_00ec6567_c2::
        if c_stk_81_2 ~= 0 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            local movie3 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            scratchValue9 = nil
            if resources:ResourceThingHealthAbove(resource, 0.0) then
                local hero7 = quest:GetHero()
                if not me:Speak(hero7, "TEXT_QST_B10_MOTHER_HIT", GROUP_SELECT_FIRST, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                else
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00ec677d_c2 end
                    end
                    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00ec677d_c2 end
                    goto FLOW_past_lab_00ec677d_c2
                end
                ::LAB_00ec677d_c2::
                this_00 = movie3
                goto LAB_00ec6781
                ::FLOW_past_lab_00ec677d_c2::
            end
            scratchValue9 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(me, scratchValue9)
            local hero2 = quest:GetHero()
            quest:EntitySetThingAsAllyOfThing(hero2, me)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie3)
        end
        quest:NewScriptFrame(me)
        goto FLOW_after_lab_00ec64df
        ::FLOW_past_lab_00ec64c5::
        scratchValue9 = quest:GetHero()
        if not me:Speak(scratchValue9, "TEXT_QST_B10_BOY_CURED_10", GROUP_SELECT_FIRST, false, true, false) then
            quest:PauseAllNonScriptedEntities(false)
            this_00 = movie
            goto LAB_00ec6781
        end
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

