-- Readable native conversion: TourGuideGuide. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

-- CScriptDef fields read by this script (offsets into the global game data; retail values in the comments)
local SCRIPT_DEF = {
    TourGuideOverhearDistance = 2260,  -- 6.0
    TourGuideTalkDistance = 2264,  -- 3.0
    TourGuideStandStillTimer = 2268,  -- 8
    TourGuideTalkToHeroTimer = 2272,  -- 3
}

local helpers = require("V_TourGuide.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local saveWaypointIdx, lastRandomSpeechIdx

-- TourGuideGuide.Main (retail 0x00ee57b0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local predicateResult, outsideDistance, scratchValue4, conversationId, scratchValue6, hero2
    local getPos, scratchValue12, getRandomThingWithScriptName, scratchValue13, scratchValue14
    local getStateString3, closingTimeExit
    scratchValue14 = 0
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
            if ((saveWaypointIdx + 2147483648) % 4294967296 - 2147483648) == -1 then
                if not quest:GetStateThing("NextTourWaypoint"):IsAlive() or 17 < quest:GetStateInt("WaypointCounter") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
                    local locationStart = quest:GetThingWithScriptName("M_TG_LocationStart")
                    quest:SetStateThing("NextTourWaypoint", locationStart)
                    quest:SetStateInt("WaypointCounter", 0)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
                    local getThingWithScriptName = quest:GetThingWithScriptName(quest:GetStateString("WaypointInfo_" .. quest:GetStateInt("WaypointCounter") .. "_locMarker"))
                    quest:SetStateThing("NextTourWaypoint", getThingWithScriptName)
                end
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
                quest:SetStateInt("WaypointCounter", saveWaypointIdx + 1)
                if 17 < quest:GetStateInt("WaypointCounter") then
                    if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
                    quest:SetStateInt("WaypointCounter", 0)
                end
                local getThingWithScriptName2 = quest:GetThingWithScriptName(quest:GetStateString("WaypointInfo_" .. quest:GetStateInt("WaypointCounter") .. "_locMarker"))
                quest:SetStateThing("NextTourWaypoint", getThingWithScriptName2)
                saveWaypointIdx = 0xffffffff
            end
            helpers.MoveToNextWaypoint(quest, me, quest:GetStateThing("NextTourWaypoint"), resource)
            if not quest:GetStateBool("SpawnedQuestFinishThread") then
                if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
                quest:CreateThread("WatchForNoFollowers")  -- native thread body NScript::CV_TourGuideScript::WatchForNoFollowers: lift it as function WatchForNoFollowers(quest)
                scratchValue13 = scratchValue14
                if (scratchValue14 & 4) ~= 0 then
                    scratchValue13 = scratchValue14 & 0xfffffffb
                    scratchValue14 = scratchValue13
                end
                if (scratchValue13 & 2) ~= 0 then
                    scratchValue13 = scratchValue13 & 0xfffffffd
                    scratchValue14 = scratchValue13
                end
                if (scratchValue13 & 1) ~= 0 then
                    scratchValue14 = scratchValue13 & 0xfffffffe
                end
                quest:SetStateBool("SpawnedQuestFinishThread", true)
            end
            quest:SetStateBool("Initialise", false)
            quest:SetStateBool("GuideSpokenToHeroThisWaypoint", false)
            quest:SetStateBool("OverheardTourGuideThisWaypoint", false)
        end
        conversationId = quest:RetailRandModulo(32768)
        if conversationId % 50 == 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
            scratchValue12 = quest:GetRandomThingWithScriptName("TourGuideFollower")
            conversationId = scratchValue12 ~= nil and scratchValue12:IsAlive()
            if conversationId then
                if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
                quest:EntityForceToLookAtThing(me, scratchValue12)
            end
        end
        local scratchValue3 = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.TourGuideOverhearDistance)
        local hero = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(me, hero, scratchValue3) and not quest:GetStateBool("OverheardTourGuideThisWaypoint") then
            if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
            local getStateString2 = quest:GetStateString("WaypointInfo_" .. quest:GetStateInt("WaypointCounter") .. "_locTextOverheard")
            if not quest:GetStateBool("TourGuideKilled") then
                if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
                getRandomThingWithScriptName = quest:GetRandomThingWithScriptName("TourGuideFollower")
                conversationId = getRandomThingWithScriptName ~= nil and getRandomThingWithScriptName:IsAlive()
                if conversationId then
                    if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                    conversationId = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(conversationId, getRandomThingWithScriptName)
                    quest:AddLineToConversation(conversationId, getStateString2, me, getRandomThingWithScriptName, false)
                end
            end
            quest:SetStateBool("OverheardTourGuideThisWaypoint", true)
        end
        scratchValue4 = quest:ReadGlobalGameDataFloat(SCRIPT_DEF.TourGuideTalkDistance)
        hero2 = quest:GetHero()
        if quest:IsDistanceBetweenThingsUnder(me, hero2, scratchValue4) then
            if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
            if me:IsTalkedToByHero() then
                if not quest:IsActiveThreadTerminating() then
                    resources:PrepareResource(resource)
                    while not resources:TryAcquire(resource, me, 4) do
                        if not quest:NewScriptFrame(me) then goto LAB_00ee6817 end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        me:ClearCommands()
                        local hero3 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, hero3, false)
                        if not quest:GetStateBool("GuideSpokenToHeroThisWaypoint") then
                            if not quest:IsActiveThreadTerminating() then
                                getStateString3 = quest:GetStateString("WaypointInfo_" .. quest:GetStateInt("WaypointCounter") .. "_locTextRequested")
                                quest:SetStateBool("GuideSpokenToHeroThisWaypoint", true)
                                goto LAB_00ee5fd7
                            end
                        elseif not quest:IsActiveThreadTerminating() then
                            while true do
                                if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                                conversationId = quest:RetailRandModulo(5)
                                if conversationId ~= lastRandomSpeechIdx then break end
                                quest:NewScriptFrame(me)
                            end
                            if not quest:IsActiveThreadTerminating() then
                                lastRandomSpeechIdx = conversationId
                                if not quest:IsActiveThreadTerminating() then getStateString3 = quest:GetStateString("RandomGuideResponse_" .. conversationId); goto LAB_00ee5fd7 end
                            end
                        end
                        goto FLOW_past_lab_00ee5fd7
                        ::LAB_00ee5fd7::
                        if not quest:GetStateBool("TourGuideKilled") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                            local conversationId2 = quest:AddNewConversation(me, false, false)
                            local hero4 = quest:GetHero()
                            quest:AddPersonToConversation(conversationId2, hero4)
                            local hero5 = quest:GetHero()
                            quest:AddLineToConversation(conversationId2, getStateString3, me, hero5, false)
                            quest:SetTimer(timerId, quest:ReadGlobalGameData(SCRIPT_DEF.TourGuideTalkToHeroTimer))
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
                quest:SetTimer(timerId4, quest:ReadGlobalGameData(SCRIPT_DEF.TourGuideStandStillTimer))
                while true do
                    if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                    scratchValue6 = quest:GetTimer(timerId4)
                    if scratchValue6 == 0 then break end
                    if not me:IsTalkedToByHero() then
                        quest:NewScriptFrame(me)
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, me, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00ee6558 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                        me:ClearCommands()
                        local hero6 = quest:GetHero()
                        quest:EntitySetFacingAngleTowardsThing(me, hero6, false)
                        while true do
                            if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                            scratchValue6 = quest:RetailRandModulo(5)
                            if scratchValue6 ~= lastRandomSpeechIdx then break end
                            quest:NewScriptFrame(me)
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                        lastRandomSpeechIdx = scratchValue6
                        if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                        local getStateString = quest:GetStateString("RandomGuideResponse_" .. scratchValue6)
                        if quest:GetStateBool("TourGuideKilled") then
                            quest:NewScriptFrame(me)
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                            local conversationId3 = quest:AddNewConversation(me, false, false)
                            local hero7 = quest:GetHero()
                            quest:AddPersonToConversation(conversationId3, hero7)
                            local hero8 = quest:GetHero()
                            quest:AddLineToConversation(conversationId3, getStateString, me, hero8, false)
                            quest:NewScriptFrame(me)
                        end
                    end
                end
                if not quest:IsActiveThreadTerminating() and not quest:IsActiveThreadTerminating() then
                    quest:SetStateInt("WaypointCounter", quest:GetStateInt("WaypointCounter") + 1)
                    quest:SetStateBool("Initialise", true)
                    goto FLOW_past_lab_00ee6558
                end
                ::FLOW_past_lab_00ee6232::
            end
            goto LAB_00ee6558
        end
        goto FLOW_past_lab_00ee6558
        ::LAB_00ee6558::
        goto LAB_00ee6817
        ::FLOW_past_lab_00ee6558::
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
    if not (quest:IsActiveThreadTerminating()) then
        closingTimeExit = quest:GetThingWithScriptName("M_TG_ClosingTimeExit")
        if not resources:ScriptThing(resource):IsNull() and (closingTimeExit ~= nil and closingTimeExit:IsAlive()) then
            if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
            if not (closingTimeExit ~= nil and not closingTimeExit:IsNull()) then
                getPos = {x = 0, y = 0, z = 0}
            else
                getPos = closingTimeExit:GetPos()
            end
            getRandomThingWithScriptName = {x = getPos.x, y = getPos.y, z = getPos.z}
            scratchValue12 = resources:ScriptThing(resource)
            local scratchValue = scratchValue12
            outsideDistance = scratchValue ~= nil and scratchValue:IsDistanceFromPositionOver(getRandomThingWithScriptName, 3.0)
            while outsideDistance do
                if not quest:NewScriptFrame(me) then goto LAB_00ee6817 end
                me:MoveToPosition(getRandomThingWithScriptName, 1.0, ENTITY_MOVE_WALK, false, true)
                while me:IsPerformingScriptTask() do
                    if not quest:NewScriptFrame(me) then goto LAB_00ee6817 end
                end
                if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
                scratchValue12 = resources:ScriptThing(resource)
                local scratchValue11 = scratchValue12
                outsideDistance = scratchValue11 ~= nil and scratchValue11:IsDistanceFromPositionOver(getRandomThingWithScriptName, 3.0)
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

