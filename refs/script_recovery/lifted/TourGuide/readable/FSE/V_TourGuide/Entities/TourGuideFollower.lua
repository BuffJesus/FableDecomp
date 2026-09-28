-- Readable native conversion: TourGuideFollower. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local self0X14, followerSpokenToHeroThisWaypoint

-- TourGuideFollower.Main (retail 0x00ee4dd0)
function Main(quest, me)
    local self_0x = self0X14
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult, predicateResult6, outsideDistance, scratchValue4, scratchValue7
    local conversationId, pOther, getPos, scratchValue12
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 2) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            quest:DeregisterTimer(timerId)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); quest:DeregisterTimer(timerId); return end
    local tourGuideGuide = quest:GetThingWithScriptName("TourGuideGuide")
    if not quest:GetStateBool("TourGuideKilled") then
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            quest:DeregisterTimer(timerId)
            return
        end
        me:FollowThing(tourGuideGuide, quest:ReadGlobalGameData(2256), true)
    end
    predicateResult = quest:IsActiveThreadTerminating()
    while true do
        if predicateResult then
            resources:ReleaseResource(resource)
            quest:DeregisterTimer(timerId)
            return
        end
        if quest:GetStateBool("TourGuideKilled") or quest:GetStateBool("TourFinished") then break end
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            quest:DeregisterTimer(timerId)
            return
        end
        local talkedToByHero = me:IsTalkedToByHero() and quest:GetTimer(timerId) == 0
        predicateResult6 = talkedToByHero
        if not talkedToByHero then quest:NewScriptFrame(me); predicateResult = quest:IsActiveThreadTerminating(); goto continue_2 end
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            quest:DeregisterTimer(timerId)
            return
        end
        me:ClearCommands()
        quest:EntitySetFacingAngleTowardsThing(me, hero, false)
        scratchValue4 = math.random(0, 32767)
        scratchValue7 = quest:EntityGetSex(me)
        if scratchValue7 == 1 then
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource)
                quest:DeregisterTimer(timerId)
                return
            end
            pOther = self_0x + 316 + (scratchValue4 % 5) * 4
            goto LAB_00ee50ab
        elseif scratchValue7 == 2 then
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource)
                quest:DeregisterTimer(timerId)
                return
            end
            pOther = self_0x + 336 + (scratchValue4 % 5) * 4
            goto LAB_00ee50ab
        end
        goto FLOW_past_lab_00ee50ab
        ::LAB_00ee50ab::
        scratchValue12 = pOther
        ::FLOW_past_lab_00ee50ab::
        conversationId = quest:AddNewConversation(me, false, false)
        quest:AddPersonToConversation(conversationId, hero)
        quest:AddLineToConversation(conversationId, scratchValue12, me, hero, false)
        quest:SetTimer(timerId, quest:ReadGlobalGameData(2276))
        me:FollowThing(nil --[[missing]], quest:ReadGlobalGameData(2256), true)
        quest:NewScriptFrame(me)
        predicateResult = quest:IsActiveThreadTerminating()
        ::continue_2::
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); quest:DeregisterTimer(timerId); return end
    quest:ClearThingHasInformation(me)
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            quest:DeregisterTimer(timerId)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); quest:DeregisterTimer(timerId); return end
    while me:IsPerformingScriptTask() do
        if not quest:NewScriptFrame(me) then goto LAB_00ee5535 end
    end
    if not quest:IsActiveThreadTerminating() then
        local closingTimeExit = quest:GetThingWithScriptName("M_TG_ClosingTimeExit")
        local predicateResult2 = not resources:ScriptThing(resource):IsNull() and (closingTimeExit ~= nil and closingTimeExit:IsAlive())
        if predicateResult2 then
            if not quest:IsActiveThreadTerminating() then
                if not (closingTimeExit ~= nil and not closingTimeExit:IsNull()) then
                    getPos = {x = 0, y = 0, z = 0}
                else
                    getPos = closingTimeExit:GetPos()
                end
                local vec_28 = {x = getPos.x, y = getPos.y, z = getPos.z}
                local scratchValue10 = resources:ScriptThing(resource)
                outsideDistance = scratchValue10 ~= nil and scratchValue10:IsDistanceFromPositionOver(vec_28, 3.0)
                while outsideDistance do
                    if not quest:NewScriptFrame(me) then goto LAB_00ee5535 end
                    me:MoveToPosition(vec_28, 1.0, ENTITY_MOVE_WALK, false, true)
                    while me:IsPerformingScriptTask() do
                        if not quest:NewScriptFrame(me) then goto LAB_00ee5535 end
                    end
                    if quest:IsActiveThreadTerminating() then goto LAB_00ee5535 end
                    local scratchValue11 = resources:ScriptThing(resource)
                    outsideDistance = scratchValue11 ~= nil and scratchValue11:IsDistanceFromPositionOver(vec_28, 3.0)
                end
                if not quest:IsActiveThreadTerminating() then
                    quest:FadeOutAndKillEntity(me, true, 1.0, true)
                end
            end
        elseif not quest:IsActiveThreadTerminating() then
            resources:PrepareResource(resource)
            repeat
                quest:NewScriptFrame(me)
            until quest:IsActiveThreadTerminating()
        end
    end
    ::LAB_00ee5535::
    resources:ReleaseResource(resource)
    quest:DeregisterTimer(timerId)
    do return end
    resources:ReleaseResource(resource)
    quest:DeregisterTimer(timerId)
end

-- TourGuideFollower.Init (retail 0x00ee4da0)
function Init(quest, me)
    followerSpokenToHeroThisWaypoint = false
end

-- TourGuideFollower.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- TourGuideFollower.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

