-- Generated from the same native helper bodies as the quest draft.
local MoveToNextWaypoint
-- EE6850: bsim names this body NScript::CV_TourGuideScript::MoveToNextWaypoint (a homologous script member); no PDB name
function MoveToNextWaypoint(quest, me, param1, param2)
    local resources = quest:RetailResources()
    return quest:WithRetailResources(function(owned)
    local waypointOwner, resultOwner = 0, 0
    local scratchValue, isActiveThreadTerminating, sequence1, getThingWithScriptName, getPos
    local copyThingOutput
    isActiveThreadTerminating = param1 ~= nil and param1:IsAlive()
    if isActiveThreadTerminating then
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if not isActiveThreadTerminating then
            local scratchValue7 = 1
            local scratchValue6 = 0
            local scratchValue5 = 0
            local scratchValue4 = 1.0
            local position = param1:GetPos()
            resources:MoveToPosition(param2, position, scratchValue4, scratchValue5, scratchValue6 ~= 0, scratchValue7 ~= 0)
            return
        end
    else
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if not isActiveThreadTerminating then
            copyThingOutput = owned:CopyThingOutput(param1)
            waypointOwner = owned:TrackThingOutput(copyThingOutput)
            if copyThingOutput ~= nil then
            end
            local waypointCounter = quest:GetStateInt("WaypointCounter")
            while true do
                scratchValue = copyThingOutput == nil
                if not scratchValue then
                    local scratchValue2 = copyThingOutput ~= nil and copyThingOutput:IsAlive()
                    scratchValue = not scratchValue2
                end
                if not scratchValue then break end
                quest:NewScriptFrame(me)
                isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                if isActiveThreadTerminating then goto LAB_00ee6a24 end
                local scratchValue3 = (quest:GetStateInt("WaypointCounter") + 1 + 2147483648) % 4294967296 - 2147483648
                quest:SetStateInt("WaypointCounter", scratchValue3)
                if scratchValue3 < 18 then
                    sequence1 = false
                    sequence1 = scratchValue3 == waypointCounter
                    if sequence1 then
                        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                        sequence1 = isActiveThreadTerminating
                    end
                    if sequence1 then goto LAB_00ee6a24 end
                else
                    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                    if isActiveThreadTerminating then goto LAB_00ee6a24 end
                    quest:SetStateInt("WaypointCounter", 0)
                end
                getThingWithScriptName = quest:GetThingWithScriptName(quest:GetStateString("WaypointInfo_" .. quest:GetStateInt("WaypointCounter") .. "_locMarker"))
                resultOwner = owned:TrackThingOutput(getThingWithScriptName)
                owned:AssignThingOutput(waypointOwner, getThingWithScriptName)
                owned:RetireThingOutput(resultOwner)
                resultOwner = 0
                getThingWithScriptName = nil
            end
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then goto LAB_00ee6a24 end
            if copyThingOutput == nil then
                getPos = {x = 0, y = 0, z = 0}
            else
                getPos = copyThingOutput:GetPos()
            end
            resources:MoveToPosition(param2, getPos, 1.0, 0, false, true)
            ::LAB_00ee6a24::
            owned:RetireThingOutput(waypointOwner)
            waypointOwner = 0
            copyThingOutput = nil
            return
        end
    end
    end)
end

return {MoveToNextWaypoint = MoveToNextWaypoint}
