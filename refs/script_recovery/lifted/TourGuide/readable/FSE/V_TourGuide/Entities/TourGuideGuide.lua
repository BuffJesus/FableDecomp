-- Readable native conversion: TourGuideGuide. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local saveWaypointIdx, lastRandomSpeechIdx, self0X14, self0X164

-- TourGuideGuide.Main (retail 0x00ee57b0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, addNewConversation, getTimer, getRandomThingWithScriptName2, scratchValue
    local scratchValue15, closingTimeExit
    scratchValue15 = 0
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
                    local getThingWithScriptName = quest:GetThingWithScriptName(nil --[[missing]])
                    quest:SetStateThing("NextTourWaypoint", getThingWithScriptName)
                end
            else
                quest:SetStateInt("WaypointCounter", saveWaypointIdx + 1)
                if 17 < quest:GetStateInt("WaypointCounter") then
                    quest:SetStateInt("WaypointCounter", 0)
                end
                local getThingWithScriptName2 = quest:GetThingWithScriptName(nil --[[missing]])
                quest:SetStateThing("NextTourWaypoint", getThingWithScriptName2)
                saveWaypointIdx = 0xffffffff
            end
            -- TODO(native): MoveToNextWaypoint(quest, me, *(this + 0x14), (*(this + 0x14) + 0x168), xStack_9c)
            if not quest:GetStateBool("SpawnedQuestFinishThread") then
                if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
                quest:CreateThread("WatchForNoFollowers")  -- native thread body NScript::CV_TourGuideScript::WatchForNoFollowers: lift it as function WatchForNoFollowers(quest)
                if scratchValue15 & 4 ~= 0 then
                    scratchValue = scratchValue15 & 0xfffffffb
                    scratchValue15 = scratchValue
                end
                if scratchValue & 2 ~= 0 then
                    scratchValue = scratchValue & 0xfffffffd
                    scratchValue15 = scratchValue
                end
                if scratchValue & 1 ~= 0 then
                    scratchValue15 = scratchValue & 0xfffffffe
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
            -- TODO(native): CCharString::CCharString(&xStack_68,(CCharString *)(*(int *)(this + 0x14) + 0x4c + *(int *)(*(int *)(this + 0x14) + 0x164) * 0xc));
            if not quest:GetStateBool("TourGuideKilled") then
                if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
                getRandomThingWithScriptName2 = quest:GetRandomThingWithScriptName("TourGuideFollower")
                addNewConversation = getRandomThingWithScriptName2 ~= nil and getRandomThingWithScriptName2:IsAlive()
                if addNewConversation then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, getRandomThingWithScriptName2)
                    quest:AddLineToConversation(addNewConversation, "", me, getRandomThingWithScriptName2, false)
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
                                -- TODO(native): CCharString::operator= (&xStack_80,(CCharString *)(*(int *)(this + 0x14) + 0x50 + *(int *)(*(int *)(this + 0x14) + 0x164) * 0xc));
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
                                if not quest:IsActiveThreadTerminating() then
                                    -- TODO(native): CCharString::operator= (&xStack_80,(CCharString *)(*(int *)(this + 0x14) + 0x128 + iVar6 * 4));
                                    goto LAB_00ee5fd7
                                end
                            end
                        end
                        goto FLOW_past_lab_00ee5fd7
                        ::LAB_00ee5fd7::
                        if not quest:GetStateBool("TourGuideKilled") then
                            if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                            local conversationId = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(conversationId, hero)
                            quest:AddLineToConversation(conversationId, "", me, hero, false)
                            quest:SetTimer(timerId, quest:ReadGlobalGameData(2272))
                        end
                        resources:PrepareResource(resource)
                        while not resources:TryAcquire(resource, me, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00ee6558 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            -- TODO(native): MoveToNextWaypoint(quest, me, *(this + 0x14), (*(this + 0x14) + 0x168), xStack_9c)
                            goto LAB_00ee60d9
                        end
                        ::FLOW_past_lab_00ee5fd7::
                    end
                end
                goto LAB_00ee6817
            end
        end
        ::LAB_00ee60d9::
        if quest:IsDistanceBetweenThingsUnder(me, self0X14 + 360, 2.0) then
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
                        -- TODO(native): CCharString::CCharString(&xStack_58,(CCharString *)(*(int *)(this + 0x14) + 0x128 + iVar6 * 4));
                        if quest:GetStateBool("TourGuideKilled") then
                            quest:NewScriptFrame(me)
                        else
                            if quest:IsActiveThreadTerminating() then goto LAB_00ee6558 end
                            local conversationId2 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(conversationId2, hero)
                            quest:AddLineToConversation(conversationId2, nil --[[missing]], me, hero, false)
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
    if false and (closingTimeExit ~= nil and closingTimeExit:IsAlive()) then
        -- TODO(native): xStack_64._0_4_ = puVar10.x;
        -- TODO(native): xStack_64._4_4_ = puVar10.y;
        -- TODO(native): xStack_64._8_4_ = puVar10.z;
        resources:ScriptThing(resource)
        -- TODO(native): iVar6 = IsDistanceFromThingToPositionOver(pvVar11,&xStack_64,iVar6);
    --[[unresolved native result]]
        while nil do
            if not quest:NewScriptFrame(me) then goto LAB_00ee6817 end
            me:MoveToPosition(getRandomThingWithScriptName2, 1.0, ENTITY_MOVE_WALK, false, true)
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto LAB_00ee6817 end
            end
            if quest:IsActiveThreadTerminating() then goto LAB_00ee6817 end
            resources:ScriptThing(resource)
            -- TODO(native): iVar6 = IsDistanceFromThingToPositionOver(pvVar11,&xStack_64,iVar6);
    --[[unresolved native result]]
        end
        if not quest:IsActiveThreadTerminating() then
            quest:FadeOutAndKillEntity(me, true, 1.0, true)
        end
    else
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

-- TourGuideGuide.MoveToNextWaypoint (retail 0x00ee6850)
-- EE6850: bsim names this body NScript::CV_TourGuideScript::MoveToNextWaypoint (a homologous script member); no PDB name
function MoveToNextWaypoint(quest, me, param1, param2)
    local getPos, getThingWithScriptName2
    if param1 ~= nil and param1:IsAlive() then
        if quest:IsActiveThreadTerminating() then return end
        -- TODO(native): iVar1 = *native_arg_param_2
--[[unresolved native value]]
        -- TODO(native): (**(code **)(iVar1 + 0x10))(pCVar6,uVar10,uVar11,uVar12,uVar13);
        return
    else
        if quest:IsActiveThreadTerminating() then return end
        -- TODO(native): xStack_18 = *(int **)(native_arg_param_1 + 4);
        -- TODO(native): xStack_18 = *(int **)(native_arg_param_1 + 8);
        getThingWithScriptName2 = nil
        if nil ~= nil then
            -- TODO(native): *xStack_18 = *xStack_18 + 1;
        end
        local self0X = self0X164
        while true do
            if not (getThingWithScriptName2 == nil or not (getThingWithScriptName2 ~= nil and getThingWithScriptName2:IsAlive())) then break end
            if not quest:NewScriptFrame(me) then goto LAB_00ee6a24 end
            local getStateInt = self0X164 + 1
            -- TODO(native): name field 0x164 (int)
            self0X164 = getStateInt
            if getStateInt < 18 then
                if getStateInt == self0X and quest:IsActiveThreadTerminating() then goto LAB_00ee6a24 end
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00ee6a24 end
                -- TODO(native): name field 0x164 (undefined4)
                self0X164 = 0
            end
            local getThingWithScriptName = quest:GetThingWithScriptName(nil --[[missing]])
            getThingWithScriptName2 = getThingWithScriptName
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00ee6a24 end
        if getThingWithScriptName2 == nil then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = getThingWithScriptName2:GetPos()
        end
        param2:SetDataString(getPos)
        ::LAB_00ee6a24::
        return
    end
end

