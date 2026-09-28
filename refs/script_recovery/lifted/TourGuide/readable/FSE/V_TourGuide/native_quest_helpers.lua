-- Generated from the same native helper bodies as the quest draft.
local MoveToNextWaypoint
-- EE6850: bsim names this body NScript::CV_TourGuideScript::MoveToNextWaypoint (a homologous script member); no PDB name
function MoveToNextWaypoint(quest, me, param1, param2)
    local resources = quest:RetailResources()
    local scratchValue, isActiveThreadTerminating, sequence1, getThingWithScriptName, getPos
    local getThingWithScriptName2
    isActiveThreadTerminating = param1 ~= nil and param1:IsAlive()
    if isActiveThreadTerminating then
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then return end
        local scratchValue6 = 1
        local scratchValue5 = 0
        local scratchValue4 = 0
        local scratchValue3 = 1.0
        local position = param1:GetPos()
        resources:MoveToPosition(param2, position, scratchValue3, scratchValue4, scratchValue5 ~= 0, scratchValue6 ~= 0)
        return
    else
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then return end
        -- TODO(native): xStack_18 = *(int **)(native_arg_param_1 + 4);
        -- TODO(native): xStack_18 = *(int **)(native_arg_param_1 + 8);
        getThingWithScriptName2 = nil
        if getThingWithScriptName2 ~= nil then
            -- TODO(native): *xStack_18 = *xStack_18 + 1;
        end
        local waypointCounter = quest:GetStateInt("WaypointCounter")
        while true do
            scratchValue = getThingWithScriptName2 == nil
            if not scratchValue then
                local scratchValue2 = getThingWithScriptName2 ~= nil and getThingWithScriptName2:IsAlive()
                scratchValue = not scratchValue2
            end
            if not scratchValue then break end
            quest:NewScriptFrame(me)
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then return end
            local getStateInt = quest:GetStateInt("WaypointCounter") + 1
            quest:SetStateInt("WaypointCounter", getStateInt)
            if getStateInt < 18 then
                sequence1 = false
                sequence1 = getStateInt == waypointCounter
                if sequence1 then
                    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                    sequence1 = isActiveThreadTerminating
                end
                if sequence1 then return end
            else
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if isActiveThreadTerminating then return end
                quest:SetStateInt("WaypointCounter", 0)
            end
            getThingWithScriptName = quest:GetThingWithScriptName(quest:GetStateString("WaypointInfo_" .. quest:GetStateInt("WaypointCounter") .. "_locMarker"))
            getThingWithScriptName2 = getThingWithScriptName
            getThingWithScriptName = nil
        end
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then return end
        if getThingWithScriptName2 == nil then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = getThingWithScriptName2:GetPos()
        end
        resources:MoveToPosition(param2, getPos, 1.0, 0, false, true)
        return
    end
end

return {MoveToNextWaypoint = MoveToNextWaypoint}
