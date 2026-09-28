-- Readable native conversion: ManWithDoorName. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)
local ENTITY_MOVE_RUN = 1  -- EScriptEntityMoveType (Ego_r.pdb)

-- ManWithDoorName.Main (retail 0x00ed17d0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, scratchValue, scratchValue2, scratchValue3, center, scratchValue4
    local isPerformingScriptTask, scratchValue5, scratchValue6, scratchValue7, p0_00, getHero, line
    local speechResult, speechResult2, speechResult3, speechResult4, speechResult5, speechResult6
    local this_00, movie, movie2, movie3, movie4, scratchValue8
    quest:NewScriptFrame(me)
    scratchValue2 = quest:IsActiveThreadTerminating()
    if scratchValue2 then
        return
    end
    local resource = resources:NewResource()
    quest:SetThingHasInformation(me, false, true, false)
    local position = me:GetPos()
    center = position.x
    quest:SetWanderCentrePoint(me, position)
    quest:SetWanderMinDistance(me, 0.0)
    local distance = quest:ReadGlobalGameDataFloat(1912)
    quest:SetWanderMaxDistance(me, distance)
    quest:SetScriptingStateGroup(me, 4)
    scratchValue2 = quest:IsActiveThreadTerminating()
    repeat
        if scratchValue2 then
            resources:ReleaseResource(resource)
            return
        end
        scratchValue2 = me:MsgIsHitByHero()
        if scratchValue2 then
            goto LAB_00ed19df
        else
            scratchValue2 = me:MsgIsHitByAnySpecialAbilityFromHero()
            if scratchValue2 then
                scratchValue2 = me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL)
                if not scratchValue2 then goto LAB_00ed19df end
            end
            scratchValue2 = false
        end
        goto FLOW_past_lab_00ed19df
        ::LAB_00ed19df::
        scratchValue2 = true
        ::FLOW_past_lab_00ed19df::
        if scratchValue2 then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then resources:ReleaseResource(resource); return end
            local fret_0 = quest:GetHealth(me)
            if 9.999999747378752e-05 < fret_0 then
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then resources:ReleaseResource(resource); return end
                resources:PrepareResource(resource)
                scratchValue2 = resources:TryAcquire(resource, me, 4)
                while not scratchValue2 do
                    quest:NewScriptFrame(me)
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then resources:ReleaseResource(resource); return end
                    scratchValue2 = resources:TryAcquire(resource, me, 4)
                end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then resources:ReleaseResource(resource); return end
                local movie5 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue10 = resources:ScriptThing(resource)
                getHero = scratchValue10
                local fret_00 = quest:GetHealth(getHero)
                scratchValue4 = 0.0
                if scratchValue4 < fret_00 then
                    scratchValue7 = 0
                    scratchValue6 = 1
                    scratchValue5 = 0
                    isPerformingScriptTask = 0
                    line = "TEXT_QST_060_MAN_WITH_DOOR_NAME_HIT"
                    getHero = hero
                    speechResult = me:Speak(getHero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue6 ~= 0, scratchValue7 ~= 0)
                    isPerformingScriptTask = me:IsPerformingScriptTask()
                    scratchValue3 = isPerformingScriptTask
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if not scratchValue2 then isPerformingScriptTask = me:IsPerformingScriptTask(); scratchValue3 = isPerformingScriptTask; goto continue_1 end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie5)
                        resources:ReleaseResource(resource)
                        do return end
                        isPerformingScriptTask = me:IsPerformingScriptTask()
                        scratchValue3 = isPerformingScriptTask
                        ::continue_1::
                    end
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie5)
                        resources:ReleaseResource(resource)
                        return
                    end
                end
                resources:PrepareResource(resource)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie5)
            end
            quest:SetStateBool("DoorManAttackedByHero", true)
        end
        if quest:GetStateBool("DoorManHasBribe") and not quest:GetStateBool("DoorManComplete") then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then resources:ReleaseResource(resource); return end
            local movie6 = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:PrepareResource(resource)
            scratchValue2 = resources:TryAcquire(resource, me, 4)
            while not scratchValue2 do
                quest:NewScriptFrame(me)
                scratchValue2 = quest:IsActiveThreadTerminating()
                if not scratchValue2 then
                    scratchValue2 = resources:TryAcquire(resource, me, 4)
                else
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie6)
                    resources:ReleaseResource(resource)
                    do return end
                    scratchValue2 = resources:TryAcquire(resource, me, 4)
                end
            end
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then
                goto LAB_00ed276d
            end
            goto FLOW_past_lab_00ed276d
            ::LAB_00ed276d::
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie6)
            resources:ReleaseResource(resource)
            do return end
            ::FLOW_past_lab_00ed276d::
            local scratchValue13 = resources:ScriptThing(resource)
            getHero = scratchValue13
            local fret_01 = quest:GetHealth(getHero)
            scratchValue4 = 0.0
            if scratchValue4 < fret_01 then
                scratchValue7 = 0
                scratchValue6 = 1
                scratchValue5 = 0
                isPerformingScriptTask = 0
                line = "TEXT_QST_060_MAN_WITH_DOOR_NAME_SPEAKS"
                getHero = hero
                speechResult2 = me:Speak(getHero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue6 ~= 0, scratchValue7 ~= 0)
                isPerformingScriptTask = me:IsPerformingScriptTask()
                scratchValue3 = isPerformingScriptTask
                while scratchValue3 do
                    quest:NewScriptFrame(me)
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if not scratchValue2 then isPerformingScriptTask = me:IsPerformingScriptTask(); scratchValue3 = isPerformingScriptTask; goto continue_2 end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie6)
                    resources:ReleaseResource(resource)
                    do return end
                    isPerformingScriptTask = me:IsPerformingScriptTask()
                    scratchValue3 = isPerformingScriptTask
                    ::continue_2::
                end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then goto LAB_00ed276d end
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie6)
            quest:SetStateBool("DoorManComplete", true)
            local getNearestWithDefName = quest:GetNearestWithDefName(me, "REGION_EXIT_POINT")
            isPerformingScriptTask = getNearestWithDefName ~= nil and getNearestWithDefName:IsAlive()
            if isPerformingScriptTask then
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then resources:ReleaseResource(resource); return end
                scratchValue2 = not resources:ScriptThing(resource):IsNull()
                if scratchValue2 then
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then resources:ReleaseResource(resource); return end
                    if not (getNearestWithDefName ~= nil and not getNearestWithDefName:IsNull()) then
                        p0_00 = {x = 0, y = 0, z = 0}
                    else
                        p0_00 = getNearestWithDefName:GetPos()
                    end
                    me:MoveToPosition(p0_00, 1.0, ENTITY_MOVE_RUN, false, true)
                end
                scratchValue2 = quest:IsDistanceBetweenThingsUnder(me, getNearestWithDefName, 2.0)
                while true do
                    predicateResult = not scratchValue2
                    if predicateResult then
                        scratchValue2 = quest:MsgOnRegionLoaded()
                        predicateResult = not scratchValue2
                    end
                    if not predicateResult then break end
                    quest:NewScriptFrame(me)
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then resources:ReleaseResource(resource); return end
                    scratchValue2 = quest:IsDistanceBetweenThingsUnder(me, getNearestWithDefName, 2.0)
                end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if not scratchValue2 then
                    quest:FadeOutAndKillEntity(me, true, 1.0, true)
                else
                    resources:ReleaseResource(resource)
                    return
                end
            end
        end
        scratchValue2 = me:IsTalkedToByHero()
        if scratchValue2 then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then resources:ReleaseResource(resource); return end
            if not quest:GetStateBool("DoorManIntroComplete") then
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then resources:ReleaseResource(resource); return end
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(resource)
                scratchValue2 = resources:TryAcquire(resource, me, 4)
                while not scratchValue2 do
                    quest:NewScriptFrame(me)
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then goto LAB_00ed2793 end
                    scratchValue2 = resources:TryAcquire(resource, me, 4)
                end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                    resources:ReleaseResource(resource)
                    return
                end
                local scratchValue14 = resources:ScriptThing(resource)
                getHero = scratchValue14
                local fret_02 = quest:GetHealth(getHero)
                scratchValue4 = 0.0
                if scratchValue4 < fret_02 then
                    scratchValue7 = 0
                    scratchValue6 = 1
                    scratchValue5 = 0
                    isPerformingScriptTask = 0
                    line = "TEXT_QST_060_MAN_WITH_DOOR_NAME_INTRO"
                    getHero = hero
                    speechResult3 = me:Speak(getHero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue6 ~= 0, scratchValue7 ~= 0)
                    isPerformingScriptTask = me:IsPerformingScriptTask()
                    scratchValue3 = isPerformingScriptTask
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then goto LAB_00ed2793 end
                        isPerformingScriptTask = me:IsPerformingScriptTask()
                        scratchValue3 = isPerformingScriptTask
                    end
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then goto LAB_00ed2793 end
                end
                resources:PrepareResource(resource)
                quest:SetStateBool("DoorManIntroComplete", true)
                quest:PauseAllNonScriptedEntities(false)
                this_00 = movie2
            elseif not quest:GetStateBool("DoorManAttackedByHero") then
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then resources:ReleaseResource(resource); return end
                movie4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(resource)
                scratchValue2 = resources:TryAcquire(resource, me, 4)
                while not scratchValue2 do
                    quest:NewScriptFrame(me)
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then goto LAB_00ed27cd end
                    scratchValue2 = resources:TryAcquire(resource, me, 4)
                end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    resources:ReleaseResource(resource)
                    return
                end
                local scratchValue11 = resources:ScriptThing(resource)
                getHero = scratchValue11
                local fret_03 = quest:GetHealth(getHero)
                scratchValue4 = 0.0
                if scratchValue4 < fret_03 then
                    scratchValue7 = 0
                    scratchValue6 = 1
                    scratchValue5 = 0
                    isPerformingScriptTask = 2
                    line = "TEXT_QST_060_MAN_WITH_DOOR_NAME_REMINDER"
                    getHero = hero
                    speechResult4 = me:Speak(getHero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue6 ~= 0, scratchValue7 ~= 0)
                    isPerformingScriptTask = me:IsPerformingScriptTask()
                    scratchValue3 = isPerformingScriptTask
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then goto LAB_00ed27cd end
                        isPerformingScriptTask = me:IsPerformingScriptTask()
                        scratchValue3 = isPerformingScriptTask
                    end
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then goto LAB_00ed27cd end
                end
                resources:PrepareResource(resource)
                quest:SetStateBool("DoorManIntroComplete", true)
                quest:PauseAllNonScriptedEntities(false)
                this_00 = movie4
            else
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then resources:ReleaseResource(resource); return end
                movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                resources:PrepareResource(resource)
                scratchValue2 = resources:TryAcquire(resource, me, 4)
                while not scratchValue2 do
                    quest:NewScriptFrame(me)
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then goto LAB_00ed2807 end
                    scratchValue2 = resources:TryAcquire(resource, me, 4)
                end
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie3)
                    resources:ReleaseResource(resource)
                    return
                end
                local scratchValue9 = resources:ScriptThing(resource)
                getHero = scratchValue9
                local fret_04 = quest:GetHealth(getHero)
                scratchValue4 = 0.0
                if scratchValue4 < fret_04 then
                    scratchValue7 = 0
                    scratchValue6 = 1
                    scratchValue5 = 0
                    isPerformingScriptTask = 2
                    line = "TEXT_QST_060_MAN_WITH_DOOR_NAME_ATTACKED"
                    getHero = hero
                    speechResult5 = me:Speak(getHero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue6 ~= 0, scratchValue7 ~= 0)
                    isPerformingScriptTask = me:IsPerformingScriptTask()
                    scratchValue3 = isPerformingScriptTask
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then goto LAB_00ed2807 end
                        isPerformingScriptTask = me:IsPerformingScriptTask()
                        scratchValue3 = isPerformingScriptTask
                    end
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then break end
                end
                resources:PrepareResource(resource)
                quest:PauseAllNonScriptedEntities(false)
                this_00 = movie3
            end
            resources:DestroyMovie(this_00)
        end
        scratchValue = not quest:GetStateBool("DoorManHasBribe")
        if scratchValue then
            scratchValue8 = ""
            scratchValue2 = me:MsgIsPresentedWithItem()
            if scratchValue2 then scratchValue8 = _G.g_PresentedItemName end
            scratchValue = scratchValue2
        end
        if scratchValue then
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then resources:ReleaseResource(resource); return end
            if scratchValue8 == nil then
                scratchValue2 = false
                if scratchValue2 then
                    goto LAB_00ed26a8
                end
            else
                isPerformingScriptTask = scratchValue8 == "OBJECT_GEMSTONE_RUBY" and 0 or 1
                if isPerformingScriptTask == 0 then goto LAB_00ed26a8 end
            end
            goto FLOW_past_lab_00ed26a8
            ::LAB_00ed26a8::
            scratchValue2 = quest:IsActiveThreadTerminating()
            if not scratchValue2 then
                quest:AddItemToContainer(me, "OBJECT_GEMSTONE_RUBY")
                quest:SetStateBool("DoorManHasBribe", true)
                goto LAB_00ed2618
            end
            resources:ReleaseResource(resource)
            do return end
            ::FLOW_past_lab_00ed26a8::
            scratchValue2 = quest:IsActiveThreadTerminating()
            if scratchValue2 then resources:ReleaseResource(resource); return end
            movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            resources:PrepareResource(resource)
            scratchValue2 = resources:TryAcquire(resource, me, 4)
            while not scratchValue2 do
                quest:NewScriptFrame(me)
                scratchValue2 = quest:IsActiveThreadTerminating()
                if scratchValue2 then goto LAB_00ed283b end
                scratchValue2 = resources:TryAcquire(resource, me, 4)
            end
            scratchValue2 = quest:IsActiveThreadTerminating()
            if not scratchValue2 then
                local scratchValue12 = resources:ScriptThing(resource)
                getHero = scratchValue12
                local fret_05 = quest:GetHealth(getHero)
                scratchValue4 = 0.0
                if scratchValue4 < fret_05 then
                    scratchValue7 = 0
                    scratchValue6 = 1
                    scratchValue5 = 0
                    isPerformingScriptTask = 2
                    line = "TEXT_QST_060_MAN_WITH_DOOR_WRONG_ITEM"
                    getHero = hero
                    speechResult6 = me:Speak(getHero, line, isPerformingScriptTask, scratchValue5 ~= 0, scratchValue6 ~= 0, scratchValue7 ~= 0)
                    isPerformingScriptTask = me:IsPerformingScriptTask()
                    scratchValue3 = isPerformingScriptTask
                    while scratchValue3 do
                        quest:NewScriptFrame(me)
                        scratchValue2 = quest:IsActiveThreadTerminating()
                        if scratchValue2 then goto LAB_00ed283b end
                        isPerformingScriptTask = me:IsPerformingScriptTask()
                        scratchValue3 = isPerformingScriptTask
                    end
                    scratchValue2 = quest:IsActiveThreadTerminating()
                    if scratchValue2 then goto LAB_00ed283b end
                end
                resources:PrepareResource(resource)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                goto LAB_00ed2618
            end
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
            resources:ReleaseResource(resource)
            do return end
            resources:ReleaseResource(resource)
            return
        end
        ::LAB_00ed2618::
        quest:NewScriptFrame(me)
        scratchValue2 = quest:IsActiveThreadTerminating()
    until false
    ::LAB_00ed2807::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie3)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00ed27cd::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie4)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00ed2793::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie2)
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00ed283b::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:ReleaseResource(resource)
end

-- ManWithDoorName.Init (retail 0x00ed1790)
function Init(quest, me)
    quest:SetThingPersistent(me, true)
end

-- ManWithDoorName.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- ManWithDoorName.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

