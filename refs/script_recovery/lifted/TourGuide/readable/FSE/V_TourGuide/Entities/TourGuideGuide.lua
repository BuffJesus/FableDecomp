-- Readable native conversion: TourGuideGuide. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local helpers = require("V_TourGuide.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local saveWaypointIdx, lastRandomSpeechIdx

-- TourGuideGuide.Main (retail 0x00ee57b0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, outsideDistance, addNewConversation, getTimer, getPos, scratchValue16
    local scratchValue17, getStateString3, closingTimeExit
    scratchValue17 = 0
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
    local timerId4 = quest:RegisterTimer()
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    quest:SetStateBool("Initialise", true)
    quest:SetStateBool("TourGuideKilled", false)
    quest:SetStateInt("WaypointCounter", 0)
    predicateResult = quest:IsActiveThreadTerminating()
    while true do
        if predicateResult then
            quest:DeregisterTimer(timerId)
            quest:DeregisterTimer(timerId4)
            resources:ReleaseResource(resource)
            return
        end
        if quest:GetStateBool("AllFollowersDead") or quest:GetStateBool("TourFinished") then break end
        if quest:GetStateBool("Initialise") then
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                quest:DeregisterTimer(timerId4)
                resources:ReleaseResource(resource)
                return
            end
            resources:PrepareResource(resource)
            while not resources:TryAcquire(resource, me, 4) do
                if not quest:NewScriptFrame(me) then goto LAB_00ee6817 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
            if saveWaypointIdx == -1 then
                if not quest:GetStateThing("NextTourWaypoint"):IsAlive() or 17 < quest:GetStateInt("WaypointCounter") then
                    quest:SetStateThing("NextTourWaypoint", quest:GetThingWithScriptName("M_TG_LocationStart"))
                    quest:SetStateInt("WaypointCounter", 0)
                else
                    quest:SetStateThing("NextTourWaypoint", quest:GetThingWithScriptName(quest:GetStateString("WaypointInfo_" .. quest:GetStateInt("WaypointCounter") .. "_locMarker")))
                end
            else
                quest:SetStateInt("WaypointCounter", saveWaypointIdx + 1)
                if 17 < quest:GetStateInt("WaypointCounter") then
                    quest:SetStateInt("WaypointCounter", 0)
                end
                quest:SetStateThing("NextTourWaypoint", quest:GetThingWithScriptName(quest:GetStateString("WaypointInfo_" .. quest:GetStateInt("WaypointCounter") .. "_locMarker")))
                saveWaypointIdx = 0xffffffff
            end
            helpers.MoveToNextWaypoint(quest, me, quest:GetStateThing("NextTourWaypoint"), resource)
            if not quest:GetStateBool("SpawnedQuestFinishThread") then
                if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
                quest:CreateThread("WatchForNoFollowers")  -- native thread body NScript::CV_TourGuideScript::WatchForNoFollowers: lift it as function WatchForNoFollowers(quest)
                if scratchValue17 & 4 ~= 0 then
                    scratchValue16 = scratchValue17 & 0xfffffffb
                    scratchValue17 = scratchValue16
                end
                if scratchValue16 & 2 ~= 0 then
                    scratchValue16 = scratchValue16 & 0xfffffffd
                    scratchValue17 = scratchValue16
                end
                if scratchValue16 & 1 ~= 0 then
                    scratchValue17 = scratchValue16 & 0xfffffffe
                end
                quest:SetStateBool("SpawnedQuestFinishThread", true)
            end
            quest:SetStateBool("Initialise", false)
            quest:SetStateBool("GuideSpokenToHeroThisWaypoint", false)
            quest:SetStateBool("OverheardTourGuideThisWaypoint", false)
        end
        addNewConversation = math.random(0, 32767)
        if addNewConversation % 50 == 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
            local getRandomThingWithScriptName = quest:GetRandomThingWithScriptName("TourGuideFollower")
            addNewConversation = getRandomThingWithScriptName ~= nil and getRandomThingWithScriptName:IsAlive()
            if addNewConversation then
                quest:EntityForceToLookAtThing(me, getRandomThingWithScriptName)
            end
        end
        if quest:IsDistanceBetweenThingsUnder(me, hero, quest:ReadGlobalGameDataFloat(2260)) and not quest:GetStateBool("OverheardTourGuideThisWaypoint") then
            if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
            local getStateString2 = quest:GetStateString("WaypointInfo_" .. quest:GetStateInt("WaypointCounter") .. "_locTextOverheard")
            if not quest:GetStateBool("TourGuideKilled") then
                local getRandomThingWithScriptName2 = quest:GetRandomThingWithScriptName("TourGuideFollower")
                addNewConversation = getRandomThingWithScriptName2 ~= nil and getRandomThingWithScriptName2:IsAlive()
                if addNewConversation then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, getRandomThingWithScriptName2)
                    quest:AddLineToConversation(addNewConversation, getStateString2, me, getRandomThingWithScriptName2, false)
                end
            end
            quest:SetStateBool("OverheardTourGuideThisWaypoint", true)
        end
        if quest:IsDistanceBetweenThingsUnder(me, hero, quest:ReadGlobalGameDataFloat(2264)) then
            if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
            if me:IsTalkedToByHero() then
                if not quest:IsActiveThreadTerminating() then
                    resources:PrepareResource(resource)
                    while not resources:TryAcquire(resource, me, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00ee6817 end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        me:ClearCommands()
                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                        if not quest:GetStateBool("GuideSpokenToHeroThisWaypoint") then
                            if not quest:IsActiveThreadTerminating() then
                                getStateString3 = quest:GetStateString("WaypointInfo_" .. quest:GetStateInt("WaypointCounter") .. "_locTextRequested")
                                quest:SetStateBool("GuideSpokenToHeroThisWaypoint", true)
                                goto LAB_00ee5fd7
                            end
                        elseif not quest:IsActiveThreadTerminating() then
                            while true do
                                if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                                addNewConversation = math.random(0, 32767) % 5
                                if addNewConversation ~= lastRandomSpeechIdx then break end
                                quest:NewScriptFrame(me)
                            end
                            if not quest:IsActiveThreadTerminating() then
                                lastRandomSpeechIdx = addNewConversation
                                if not quest:IsActiveThreadTerminating() then getStateString3 = quest:GetStateString("RandomGuideResponse_" .. addNewConversation); goto LAB_00ee5fd7 end
                            end
                        end
                        goto FLOW_past_lab_00ee5fd7
                        ::LAB_00ee5fd7::
                        if not quest:GetStateBool("TourGuideKilled") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                            local conversationId = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(conversationId, hero)
                            quest:AddLineToConversation(conversationId, getStateString3, me, hero, false)
                            quest:SetTimer(timerId, quest:ReadGlobalGameData(2272))
                        end
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, me, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00ee6558 end
                        end
                        if not quest:IsActiveThreadTerminating() then helpers.MoveToNextWaypoint(quest, me, quest:GetStateThing("NextTourWaypoint"), resource); goto LAB_00ee60d9 end
                        ::FLOW_past_lab_00ee5fd7::
                    end
                end
                goto LAB_00ee6817
            end
        end
        ::LAB_00ee60d9::
        if quest:IsDistanceBetweenThingsUnder(me, quest:GetStateThing("NextTourWaypoint"), 2.0) then
            if not quest:IsActiveThreadTerminating() then
                local tourGuideFollower = quest:GetNearestWithScriptName(me, "TourGuideFollower")
                if not (tourGuideFollower ~= nil and tourGuideFollower:IsAlive()) then
                    goto LAB_00ee6232
                elseif not quest:IsActiveThreadTerminating() then
                    resources:PrepareResource(resource)
                    while not resources:TryAcquire(resource, me, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00ee6558 end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        me:ClearCommands()
                        quest:NewScriptFrame(me)
                        if not quest:IsActiveThreadTerminating() then
                            quest:NewScriptFrame(me)
                            if not quest:IsActiveThreadTerminating() then
                                quest:EntitySetFacingAngleTowardsThing(me, tourGuideFollower, false)
                                quest:NewScriptFrame(me)
                                if not quest:IsActiveThreadTerminating() then
                                    quest:NewScriptFrame(me)
                                    if not quest:IsActiveThreadTerminating() then goto LAB_00ee6232 end
                                end
                            end
                        end
                    end
                end
                goto FLOW_past_lab_00ee6232
                ::LAB_00ee6232::
                quest:SetTimer(timerId4, quest:ReadGlobalGameData(2268))
                while true do
                    if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                    getTimer = quest:GetTimer(timerId4)
                    if getTimer == 0 then break end
                    if not me:IsTalkedToByHero() then
                        quest:NewScriptFrame(me)
                    else
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, me, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00ee6558 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                        me:ClearCommands()
                        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
                        while true do
                            if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                            getTimer = math.random(0, 32767) % 5
                            if getTimer ~= lastRandomSpeechIdx then break end
                            quest:NewScriptFrame(me)
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                        lastRandomSpeechIdx = getTimer
                        local getStateString = quest:GetStateString("RandomGuideResponse_" .. getTimer)
                        if quest:GetStateBool("TourGuideKilled") then
                            quest:NewScriptFrame(me)
                        else
                            local conversationId2 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(conversationId2, hero)
                            quest:AddLineToConversation(conversationId2, getStateString, me, hero, false)
                            quest:NewScriptFrame(me)
                        end
                    end
                end
                if not quest:IsActiveThreadTerminating() and not quest:IsActiveThreadTerminating() then
                    quest:SetStateInt("WaypointCounter", quest:GetStateInt("WaypointCounter") + 1)
                    quest:SetStateBool("Initialise", true)
                    goto LAB_00ee6456
                end
                ::FLOW_past_lab_00ee6232::
            end
            goto FLOW_hoist_lab_00ee6558_1
        end
        goto FLOW_past_lab_00ee6558
        ::LAB_00ee6558::
        ::FLOW_hoist_lab_00ee6558_1::
        goto LAB_00ee6817
        ::FLOW_past_lab_00ee6558::
        ::LAB_00ee6456::
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId)
        quest:DeregisterTimer(timerId4)
        resources:ReleaseResource(resource)
        return
    end
    quest:ClearThingHasInformation(me)
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            quest:DeregisterTimer(timerId4)
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then
        quest:DeregisterTimer(timerId)
        quest:DeregisterTimer(timerId4)
        resources:ReleaseResource(resource)
        return
    end
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then goto LAB_00ee6817 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
    closingTimeExit = quest:GetThingWithScriptName("M_TG_ClosingTimeExit")
    if not resources:ScriptThing(resource):IsNull() and (closingTimeExit ~= nil and closingTimeExit:IsAlive()) then
        if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
        if not (closingTimeExit ~= nil and not closingTimeExit:IsNull()) then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = closingTimeExit:GetPos()
        end
        local scratchValue15 = {x = getPos.x, y = getPos.y, z = getPos.z}
        local scratchValue = resources:ScriptThing(resource)
        outsideDistance = scratchValue ~= nil and scratchValue:IsDistanceFromPositionOver(scratchValue15, 3.0)
        while outsideDistance do
            if not quest:NewScriptFrame(me) then goto LAB_00ee6817 end
            me:MoveToPosition(scratchValue15, 1.0, ENTITY_MOVE_WALK, false, true)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto LAB_00ee6817 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
            local scratchValue12 = resources:ScriptThing(resource)
            outsideDistance = scratchValue12 ~= nil and scratchValue12:IsDistanceFromPositionOver(scratchValue15, 3.0)
        end
        if not quest:IsActiveThreadTerminating() then
            quest:FadeOutAndKillEntity(me, true, 1.0, true)
        end
    else
        if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
        resources:PrepareResource(resource)
        repeat
            quest:NewScriptFrame(me)
        until quest:IsActiveThreadTerminating()
    end
    ::LAB_00ee6817::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId4)
    resources:ReleaseResource(resource)
end

-- TourGuideGuide.Init (retail 0x00ee4c60)
function Init(quest, me)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySetPersonalityOverrideByString(me, "OPINION_PERSONALITY_PERMANENT_FRIEND")
    saveWaypointIdx = 0xffffffff
    lastRandomSpeechIdx = 0xffffffff
end

-- TourGuideGuide.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TourGuideGuide.OnPredicateFail (retail 0x00ee4cc0)
function OnPredicateFail(quest, me)
end

