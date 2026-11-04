-- Readable native conversion: CampHostageGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local dropPass

-- CampHostageGuard.Main (retail 0x00d09010)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult6, predicateResult9, predicateResult, predicateResult15, hostagesRescued
    local taskRunning, scratchValue4, scratchValue5, registerTimer, conversationId, timerId
    local timerId2, switch1, guardFirstMarker5, position, line, campHostage, speechResult
    local scratchValue18, movie, movie2, scratchValue21
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d0a6ae end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d0a6ae end
    timerId2 = quest:RegisterTimer()
    quest:SetTimer(timerId2, 0)
    scratchValue18 = 0
    scratchValue4 = 0
    campHostage = quest:GetNearestWithScriptName(me, "CampHostage")
    quest:EntitySetFacingAngleTowardsThing(me, campHostage, false)
    scratchValue21 = 1
    scratchValue5 = 0
    registerTimer = quest:RegisterTimer()
    dropPass = true
    hostagesRescued = quest:GetStateBool("HostagesRescued")
    timerId = registerTimer
    while not hostagesRescued do
        if not quest:NewScriptFrame(me) then goto LAB_00d0a693 end
        if quest:MsgOnHeroPickedPocket() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
            dropPass = false
        end
        local hero = quest:GetHero()
        if not quest:IsDistanceBetweenThingsUnder(me, hero, 7.0) or not quest:IsDistanceBetweenThingsUnder(me, campHostage, 12.0) or 0 < quest:GetTimer(timerId2) then goto LAB_00d09458 end
        if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
        conversationId = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId, campHostage)
        switch1 = scratchValue18
        repeat
            if switch1 == 0 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_FIRST", me, campHostage, false)
                scratchValue18 = 1
                break
            elseif switch1 == 1 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_SECOND", me, campHostage, false)
                scratchValue18 = 2
                break
            elseif switch1 == 2 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_THIRD", me, campHostage, false)
                scratchValue18 = 3
                break
            elseif switch1 == 3 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_FOURTH", me, campHostage, false)
                scratchValue18 = 4
                break
            else
                if switch1 == 4 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_FIFTH", me, campHostage, false)
                    goto LAB_00d09432
                elseif switch1 == 5 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_SIXTH", me, campHostage, false)
                    scratchValue18 = 6
                    break
                elseif switch1 == 6 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_SEVENTH", me, campHostage, false)
                    goto LAB_00d09432
                end
                goto FLOW_past_lab_00d09432
                ::LAB_00d09432::
                scratchValue18 = 5
                ::FLOW_past_lab_00d09432::
            end
        until true
        quest:SetTimer(timerId2, 10)
        registerTimer = timerId
        ::LAB_00d09458::
        if scratchValue21 == 1 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
            local guardFirstMarker = quest:GetThingWithScriptName("GuardFirstMarker")
            if not ((not quest:IsDistanceBetweenThingsOver(me, guardFirstMarker, 2.0)) or me:IsPerformingScriptTask()) then
                if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
                local guardFirstMarker2 = quest:GetThingWithScriptName("GuardFirstMarker")
                position = guardFirstMarker2:GetPos()
                me:MoveToPosition(position, 1.0, ENTITY_MOVE_WALK, false, true)
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
                if quest:GetTimer(registerTimer) < 1 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
                    if scratchValue5 ~= 0 then
                        if not quest:IsActiveThreadTerminating() then scratchValue21 = 2; goto LAB_00d09756 end
                        goto LAB_00d0a693
                    end
                    goto LAB_00d095b5
                end
            end
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
            local guardSecondMarker = quest:GetThingWithScriptName("GuardSecondMarker")
            if not ((not quest:IsDistanceBetweenThingsOver(me, guardSecondMarker, 2.0)) or me:IsPerformingScriptTask()) then
                if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
                local guardSecondMarker2 = quest:GetThingWithScriptName("GuardSecondMarker")
                position = guardSecondMarker2:GetPos()
                me:MoveToPosition(position, 1.0, ENTITY_MOVE_WALK, false, true)
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
                if quest:GetTimer(registerTimer) < 1 then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
                    if scratchValue5 == 0 then goto LAB_00d095b5 end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
                    scratchValue21 = 1
                    goto LAB_00d09756
                end
            end
        end
        goto FLOW_past_lab_00d095b5
        ::LAB_00d095b5::
        if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
        scratchValue5 = 1
        quest:SetTimer(registerTimer, 10)
        ::FLOW_past_lab_00d095b5::
        goto FLOW_past_lab_00d09756
        ::LAB_00d09756::
        scratchValue5 = 0
        ::FLOW_past_lab_00d09756::
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
            me:ClearCommands()
            if scratchValue4 == 0 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
                movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue19 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue19) then
                    line = "TEXT_QST_009_HOSTAGE_GUARD_FIRST_CHAT"
                    local hero2 = quest:GetHero()
                    if not me:Speak(hero2, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            quest:DeregisterTimer(timerId)
                            quest:DeregisterTimer(timerId2)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d09dba end
                end
                quest:EntitySetFacingAngleTowardsThing(me, campHostage, false)
                scratchValue4 = 1
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
                movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local scratchValue20 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue20) then
                    line = "TEXT_QST_009_HOSTAGE_GUARD_SECOND_CHAT"
                    local hero3 = quest:GetHero()
                    if not me:Speak(hero3, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId2)
                        resources:ReleaseResource(resource)
                        return
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            quest:DeregisterTimer(timerId)
                            quest:DeregisterTimer(timerId2)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0a4c8 end
                end
                quest:EntitySetFacingAngleTowardsThing(me, campHostage, false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            end
            registerTimer = timerId
        end
        if me:MsgIsHitByHero() then
            goto LAB_00d09aee
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d09aee end
            predicateResult6 = false
            if quest:GetStateBool("HostageKilled") then goto LAB_00d09aee end
        end
        goto FLOW_past_lab_00d09aee
        ::LAB_00d09aee::
        predicateResult6 = true
        ::FLOW_past_lab_00d09aee::
        if not predicateResult6 then
            hostagesRescued = quest:GetStateBool("HostagesRescued")
        else
            if not quest:IsActiveThreadTerminating() then
                quest:ClearThingHasInformation(me)
                local hero4 = quest:GetHero()
                quest:GiveThingBestEnemyTarget(me, hero4)
                resources:PrepareResource(resource)
                repeat
                    quest:NewScriptFrame(me)
                until quest:IsActiveThreadTerminating()
                quest:DeregisterTimer(registerTimer)
                quest:DeregisterTimer(timerId2)
                resources:ReleaseResource(resource)
                return
            end
            goto LAB_00d0a693
            hostagesRescued = quest:GetStateBool("HostagesRescued")
        end
    end
    predicateResult9 = quest:IsActiveThreadTerminating()
    if not predicateResult9 then
        scratchValue4 = predicateResult9 and 1 or 0
        quest:ClearThingHasInformation(me)
        while not quest:IsActiveThreadTerminating() do
            local guardFirstMarker3 = quest:GetThingWithScriptName("GuardFirstMarker")
            if not ((not quest:IsDistanceBetweenThingsOver(me, guardFirstMarker3, 2.0)) or me:IsPerformingScriptTask()) then
                if quest:IsActiveThreadTerminating() then break end
                local guardFirstMarker4 = quest:GetThingWithScriptName("GuardFirstMarker")
                position = guardFirstMarker4:GetPos()
                me:MoveToPosition(position, 1.0, ENTITY_MOVE_WALK, false, true)
            end
            local hero5 = quest:GetHero()
            if not (not quest:IsDistanceBetweenThingsUnder(me, hero5, 7.0)) then
                guardFirstMarker5 = quest:GetThingWithScriptName("GuardFirstMarker")
                if not quest:IsDistanceBetweenThingsUnder(me, guardFirstMarker5, 12.0) then goto LAB_00d09cf7 end
                predicateResult = true
                if 0 < quest:GetTimer(timerId2) then goto LAB_00d09cf7 end
                goto FLOW_past_lab_00d09cf7
            end
            ::LAB_00d09cf7::
            predicateResult = false
            ::FLOW_past_lab_00d09cf7::
            if predicateResult then
                if quest:IsActiveThreadTerminating() then break end
                local conversationId2 = quest:AddNewConversation(me, false, false)
                local hero6 = quest:GetHero()
                quest:AddPersonToConversation(conversationId2, hero6)
                local switch = scratchValue18
                repeat
                    if switch == 0 then
                        local hero7 = quest:GetHero()
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIRST", me, hero7, false)
                        scratchValue18 = 1
                        break
                    elseif switch == 1 then
                        local hero8 = quest:GetHero()
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SECOND", me, hero8, false)
                        scratchValue18 = 2
                        break
                    elseif switch == 2 then
                        local hero9 = quest:GetHero()
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_THIRD", me, hero9, false)
                        scratchValue18 = 3
                        break
                    elseif switch == 3 then
                        local hero10 = quest:GetHero()
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FOURTH", me, hero10, false)
                        scratchValue18 = 4
                        break
                    elseif switch == 4 then
                        local hero11 = quest:GetHero()
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIFTH", me, hero11, false)
                        scratchValue18 = 5
                        break
                    elseif switch == 5 then
                        local hero12 = quest:GetHero()
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SIXTH", me, hero12, false)
                        local hero13 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(me, hero13)
                        resources:PrepareResource(resource)
                        repeat
                            quest:NewScriptFrame(me)
                        until quest:IsActiveThreadTerminating()
                        goto LAB_00d0a693
                    end
                until true
                quest:SetTimer(timerId2, 10)
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then break end
                me:ClearCommands()
                if scratchValue4 == 0 then
                    if quest:IsActiveThreadTerminating() then break end
                    movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue) then
                        line = "TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_FIRST"
                        local hero14 = quest:GetHero()
                        if not me:Speak(hero14, line, 0, false, true, false) then
                            goto LAB_00d0a4c8
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00d0a4c8 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d0a4c8 end
                    end
                    scratchValue4 = 1
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie2)
                else
                    if quest:IsActiveThreadTerminating() then break end
                    movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue15 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue15) then
                        line = "TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_SECOND"
                        local hero15 = quest:GetHero()
                        if not me:Speak(hero15, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            goto LAB_00d09dc7
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d09dc7 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d09dba end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                end
            end
            if me:MsgIsHitByHero() then
                goto LAB_00d0a3d3
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0a3d3 end
                predicateResult15 = false
            end
            goto FLOW_past_lab_00d0a3d3
            ::LAB_00d0a3d3::
            predicateResult15 = true
            ::FLOW_past_lab_00d0a3d3::
            if not predicateResult15 then
                quest:NewScriptFrame(me)
            else
                if not quest:IsActiveThreadTerminating() then
                    me:ClearCommands()
                    movie2 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local scratchValue17 = resources:ScriptThing(resource)
                    if quest:GetHealth(scratchValue17) <= 0.0 then goto LAB_00d0a62b end
                    line = "TEXT_QST_009_HOSTAGE_GUARD_LATE_HIT"
                    local hero16 = quest:GetHero()
                    speechResult = me:Speak(hero16, line, 0, false, true, false)
                    taskRunning = me:IsPerformingScriptTask()
                    goto LAB_00d0a5d1
                end
                break
                quest:NewScriptFrame(me)
            end
        end
    end
    ::LAB_00d0a693::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId2)
    ::LAB_00d0a6ae::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d09dba::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d09dc7::
    resources:DestroyMovie(movie)
    goto LAB_00d0a693
    ::LAB_00d0a5d1::
    if not taskRunning then goto LAB_00d0a5f7 end
    if not (not quest:NewScriptFrame(me)) then
        taskRunning = me:IsPerformingScriptTask()
        goto LAB_00d0a5d1
    end
    ::LAB_00d0a4c8::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d0a60f::
    resources:DestroyMovie(movie2)
    goto LAB_00d0a693
    ::LAB_00d0a5f7::
    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d0a60f end
    ::LAB_00d0a62b::
    local hero17 = quest:GetHero()
    quest:GiveThingBestEnemyTarget(me, hero17)
    resources:PrepareResource(resource)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie2)
    repeat
        quest:NewScriptFrame(me)
    until quest:IsActiveThreadTerminating()
    goto LAB_00d0a693
end

-- CampHostageGuard.Init (retail 0x00d08f00)
function Init(quest, me)
    quest:AddItemToContainer(me, "OBJECT_BANDIT_CAMP_HOSTAGE_KEY")
    quest:EntitySetInFaction(me, "FACTION_TWINBLADE_CAMP_BANDITS")
    quest:EntitySetAsOpinionSource(me, "OPINION_SOURCE_BANDIT_HOSTAGE_GUARD")
    quest:EntitySetDeathContainerAsEnabled(me, false)
    quest:SetThingHasInformation(me, false, true, false)
end

-- CampHostageGuard.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- CampHostageGuard.OnPredicateFail (retail 0x00d08fb0)
function OnPredicateFail(quest, me)
    if dropPass then
        quest:GiveHeroObject("OBJECT_BANDIT_CAMP_HOSTAGE_KEY", -1, false)
    end
end

