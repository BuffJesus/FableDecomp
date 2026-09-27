-- Readable native conversion: CampHostageGuard. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local dropPass

-- CampHostageGuard.Main (retail 0x00d09010)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult6, predicateResult9, predicateResult, getStateBool, scratchValue4
    local scratchValue5, registerTimer, conversationId, timerId, switch1, campHostage, scratchValue
    local movie, movie3, scratchValue20
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d0a6ae end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d0a6ae end
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    scratchValue = 0
    scratchValue4 = 0
    campHostage = quest:GetNearestWithScriptName(me, "CampHostage")
    quest:EntitySetFacingAngleTowardsThing(me, campHostage, false)
    scratchValue20 = 1
    scratchValue5 = 0
    registerTimer = quest:RegisterTimer()
    dropPass = true
    getStateBool = quest:GetStateBool("HostagesRescued")
    while not getStateBool do
        if not quest:NewScriptFrame(me) then goto LAB_00d0a693 end
        if quest:MsgOnHeroPickedPocket() then
            dropPass = false
        end
        if not quest:IsDistanceBetweenThingsUnder(me, hero, 7.0) or not quest:IsDistanceBetweenThingsUnder(me, campHostage, 12.0) or 0 < quest:GetTimer(timerId) then goto LAB_00d09458 end
        conversationId = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId, campHostage)
        switch1 = scratchValue
        repeat
            if switch1 == 0 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_FIRST", me, campHostage, false)
                scratchValue = 1
                break
            elseif switch1 == 1 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_SECOND", me, campHostage, false)
                scratchValue = 2
                break
            elseif switch1 == 2 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_THIRD", me, campHostage, false)
                scratchValue = 3
                break
            elseif switch1 == 3 then
                quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_FOURTH", me, campHostage, false)
                scratchValue = 4
                break
            else
                if switch1 == 4 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_FIFTH", me, campHostage, false)
                    goto LAB_00d09432
                elseif switch1 == 5 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_SIXTH", me, campHostage, false)
                    scratchValue = 6
                    break
                elseif switch1 == 6 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_009_HOSTAGE_GUARD_SEVENTH", me, campHostage, false)
                    goto LAB_00d09432
                end
                goto FLOW_past_lab_00d09432
                ::LAB_00d09432::
                scratchValue = 5
                ::FLOW_past_lab_00d09432::
            end
        until true
        quest:SetTimer(timerId, 10)
        ::LAB_00d09458::
        if scratchValue20 == 1 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
            if not ((not quest:IsDistanceBetweenThingsOver(me, quest:GetThingWithScriptName("GuardFirstMarker"), 2.0)) or me:IsPerformingScriptTask()) then
                local guardFirstMarker = quest:GetThingWithScriptName("GuardFirstMarker")
                me:MoveToPosition(guardFirstMarker:GetPos(), 1.0, ENTITY_MOVE_WALK, false, true)
            elseif quest:GetTimer(registerTimer) < 1 then
                if scratchValue5 ~= 0 then
                    if not quest:IsActiveThreadTerminating() then scratchValue20 = 2; goto LAB_00d09756 end
                    goto LAB_00d0a693
                end
                goto LAB_00d095b5
            end
        else
            if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
            if not ((not quest:IsDistanceBetweenThingsOver(me, quest:GetThingWithScriptName("GuardSecondMarker"), 2.0)) or me:IsPerformingScriptTask()) then
                local guardSecondMarker = quest:GetThingWithScriptName("GuardSecondMarker")
                me:MoveToPosition(guardSecondMarker:GetPos(), 1.0, ENTITY_MOVE_WALK, false, true)
            elseif quest:GetTimer(registerTimer) < 1 then
                if scratchValue5 == 0 then goto LAB_00d095b5 end
                scratchValue20 = 1
                goto LAB_00d09756
            end
        end
        goto FLOW_past_lab_00d095b5
        ::LAB_00d095b5::
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
                local fret_0 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 < fret_0 then
                    me:Speak(hero, "TEXT_QST_009_HOSTAGE_GUARD_FIRST_CHAT", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:DeregisterTimer(registerTimer)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d09dba end
                end
                quest:EntitySetFacingAngleTowardsThing(me, campHostage, false)
                scratchValue4 = 1
                quest:PauseAllNonScriptedEntities(false)
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d0a693 end
                movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                local fret_00 = quest:GetHealth(resources:ScriptThing(resource))
                if 0.0 < fret_00 then
                    me:Speak(hero, "TEXT_QST_009_HOSTAGE_GUARD_SECOND_CHAT", GROUP_SELECT_FIRST, false, true, false)
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            quest:DeregisterTimer(registerTimer)
                            quest:DeregisterTimer(timerId)
                            resources:ReleaseResource(resource)
                            do return end
                        end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00d0a4c8 end
                end
                quest:EntitySetFacingAngleTowardsThing(me, campHostage, false)
                quest:PauseAllNonScriptedEntities(false)
            end
        end
        if me:MsgIsHitByHero() then
            goto LAB_00d09aee
        else
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d09aee end
            end
            predicateResult6 = false
            if quest:GetStateBool("HostageKilled") then goto LAB_00d09aee end
        end
        goto FLOW_past_lab_00d09aee
        ::LAB_00d09aee::
        predicateResult6 = true
        ::FLOW_past_lab_00d09aee::
        if not predicateResult6 then
            getStateBool = quest:GetStateBool("HostagesRescued")
        else
            if not quest:IsActiveThreadTerminating() then
                quest:ClearThingHasInformation(me)
                quest:GiveThingBestEnemyTarget(me, hero)
                resources:PrepareResource(resource)
                repeat
                    quest:NewScriptFrame(me)
                until quest:IsActiveThreadTerminating()
                quest:DeregisterTimer(registerTimer)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            goto LAB_00d0a693
            getStateBool = quest:GetStateBool("HostagesRescued")
        end
    end
    predicateResult9 = quest:IsActiveThreadTerminating()
    if not predicateResult9 then
        scratchValue4 = predicateResult9
        quest:ClearThingHasInformation(me)
        while not quest:IsActiveThreadTerminating() do
            if not ((not quest:IsDistanceBetweenThingsOver(me, quest:GetThingWithScriptName("GuardFirstMarker"), 2.0)) or me:IsPerformingScriptTask()) then
                local guardFirstMarker4 = quest:GetThingWithScriptName("GuardFirstMarker")
                me:MoveToPosition(guardFirstMarker4:GetPos(), 1.0, ENTITY_MOVE_WALK, false, true)
            end
            if not ((not quest:IsDistanceBetweenThingsUnder(me, hero, 7.0)) or not quest:IsDistanceBetweenThingsUnder(me, quest:GetThingWithScriptName("GuardFirstMarker"), 12.0) or (0 < quest:GetTimer(timerId))) then
                if quest:IsActiveThreadTerminating() then break end
                local conversationId2 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(conversationId2, hero)
                local switch = scratchValue
                repeat
                    if switch == 0 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIRST", me, hero, false)
                        scratchValue = 1
                        break
                    elseif switch == 1 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SECOND", me, hero, false)
                        scratchValue = 2
                        break
                    elseif switch == 2 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_THIRD", me, hero, false)
                        scratchValue = 3
                        break
                    elseif switch == 3 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FOURTH", me, hero, false)
                        scratchValue = 4
                        break
                    elseif switch == 4 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_FIFTH", me, hero, false)
                        scratchValue = 5
                        break
                    elseif switch == 5 then
                        quest:AddLineToConversation(conversationId2, "TEXT_QST_009_HOSTAGE_GUARD_LATE_CONVO_SIXTH", me, hero, false)
                        quest:GiveThingBestEnemyTarget(me, hero)
                        resources:PrepareResource(resource)
                        repeat
                            quest:NewScriptFrame(me)
                        until quest:IsActiveThreadTerminating()
                        goto LAB_00d0a693
                    end
                until true
                quest:SetTimer(timerId, 10)
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then break end
                me:ClearCommands()
                if not scratchValue4 then
                    if quest:IsActiveThreadTerminating() then break end
                    movie3 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local fret_01 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_01 then
                        if not me:Speak(hero, "TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_FIRST", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d0a4c8 end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d0a4c8 end
                    end
                    scratchValue4 = 1
                    quest:PauseAllNonScriptedEntities(false)
                else
                    if quest:IsActiveThreadTerminating() then break end
                    movie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local fret_02 = quest:GetHealth(resources:ScriptThing(resource))
                    if 0.0 < fret_02 then
                        me:Speak(hero, "TEXT_QST_009_HOSTAGE_GUARD_LATE_SPEAK_SECOND", GROUP_SELECT_FIRST, false, true, false)
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then quest:PauseAllNonScriptedEntities(false); goto LAB_00d09dc7 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00d09dba end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                end
            end
            if me:MsgIsHitByHero() then
                goto LAB_00d0a3d3
            else
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00d0a3d3 end
                end
                predicateResult = false
            end
            goto FLOW_past_lab_00d0a3d3
            ::LAB_00d0a3d3::
            predicateResult = true
            ::FLOW_past_lab_00d0a3d3::
            if not predicateResult then
                quest:NewScriptFrame(me)
            else
                if not quest:IsActiveThreadTerminating() then
                    me:ClearCommands()
                    movie3 = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    if quest:GetHealth(resources:ScriptThing(resource)) <= 0.0 then goto LAB_00d0a62b end
                    me:Speak(hero, "TEXT_QST_009_HOSTAGE_GUARD_LATE_HIT", GROUP_SELECT_FIRST, false, true, false)
                    getStateBool = me:IsPerformingScriptTask()
                    goto LAB_00d0a5d1
                end
                break
                quest:NewScriptFrame(me)
            end
        end
    end
    ::LAB_00d0a693::
    quest:DeregisterTimer(registerTimer)
    quest:DeregisterTimer(timerId)
    ::LAB_00d0a6ae::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00d09dba::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d09dc7::
    resources:DestroyMovie(movie)
    goto LAB_00d0a693
    ::LAB_00d0a5d1::
    if not getStateBool then goto LAB_00d0a5f7 end
    if not quest:NewScriptFrame(me) then goto LAB_00d0a4c8 end
    getStateBool = me:IsPerformingScriptTask()
    goto LAB_00d0a5d1
    ::LAB_00d0a4c8::
    quest:PauseAllNonScriptedEntities(false)
    ::LAB_00d0a60f::
    resources:DestroyMovie(movie3)
    goto LAB_00d0a693
    ::LAB_00d0a5f7::
    if quest:IsActiveThreadTerminating() then quest:PauseAllNonScriptedEntities(false); goto LAB_00d0a60f end
    ::LAB_00d0a62b::
    quest:GiveThingBestEnemyTarget(me, hero)
    resources:PrepareResource(resource)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie3)
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

