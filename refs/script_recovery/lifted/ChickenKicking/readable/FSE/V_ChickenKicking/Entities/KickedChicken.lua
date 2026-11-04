-- Readable native conversion: KickedChicken. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local function __native_named_marker_scope(quest, scratchValue, scratchValue2, scratchValue3, scratchValue4, scratchValue5, scratchValue6, scratchValue7, scratchValue8, scratchValue9)
    return quest:WithRetailResources(function(markerResources)
        local markerPositions = markerResources:NewThingPositionList()
        markerPositions:AppendScriptName("FirstAngleMarker")
        markerPositions:AppendScriptName("SecondAngleMarker")
        markerPositions:AppendScriptName("ThirdAngleMarker")
        markerPositions:AppendScriptName("FourthAngleMarker")
        local function storeFloat32(value)
            return (("<f"):unpack(("<f"):pack(value)))
        end
        quest:SetStateInt("FinalSector", 0)
        local epsilon = 9.999999747378752e-05
        for index = 1, markerPositions:Count() - 1 do
            local pair = markerPositions:GetPosPair(index - 1, scratchValue)
            local previous = {
                x = storeFloat32(pair.first.x - pair.second.x),
                y = storeFloat32(pair.first.y - pair.second.y),
                z = 0,
            }
            pair = markerPositions:GetPosPair(index, scratchValue)
            local currentRawY = pair.first.y - pair.second.y
            local current = {
                x = storeFloat32(pair.first.x - pair.second.x),
                y = storeFloat32(currentRawY),
                z = 0,
            }
            if epsilon * epsilon < previous.y * previous.y + previous.x * previous.x
                and epsilon * epsilon < currentRawY * currentRawY + current.x * current.x then
                previous = markerResources:NormalizeVector3(previous)
                current = markerResources:NormalizeVector3(current)
                local rawDot = previous.z * current.z + current.y * previous.y + current.x * previous.x
                local dot = storeFloat32(rawDot)
                if rawDot < -1 then
                    dot = -1
                elseif dot > 1 then
                    dot = 1
                end
                local halfTurns = markerResources:ArcCosTurns(dot, 0.5)
                local rotated = markerResources:RotateVector3(previous, {x = 0, y = 0, z = 1}, halfTurns)
                local product = (rotated.y * scratchValue4 + rotated.z * scratchValue5) + rotated.x * scratchValue3
                if markerResources:CosTurnsLessThan(halfTurns, product) then
                    quest:SetStateInt("FinalSector", index)
                end
            end
        end
        if scratchValue ~= nil and not scratchValue:IsNull() then
            scratchValue:GetPos()
        end
        local distance = markerResources:DistanceFromThingToPosition(scratchValue, scratchValue2)
        local storedDistance = ("<f"):unpack(("<f"):pack(distance))
        local distanceBand
        if distance > scratchValue9 then
            distanceBand = 4
        elseif storedDistance > scratchValue8 then
            distanceBand = 3
        elseif storedDistance > scratchValue7 then
            distanceBand = 2
        elseif storedDistance > scratchValue6 then
            distanceBand = 1
        else
            distanceBand = 0
        end
        if quest:IsActiveThreadTerminating() then return true end
        quest:SetStateInt("DistanceBand", distanceBand)
    end)
end

local function __native_arena_main(quest, me, arenaResources)
    local resources = quest:RetailResources()
    local scratchValue4, f_stk_1c_2, f_stk_20_2, thing, retailRandModulo, vec_30, scratchValue12
    local chickenKickingArena = quest:GetThingWithScriptName("ChickenKickingArena")
    arenaResources:TrackThingOutput(chickenKickingArena)
    local markerOutputToken
    thing = quest:GetThingWithScriptName("FirstLineMarker")
    markerOutputToken = arenaResources:TrackThingOutput(thing)
    local scratchValue5 = ("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(chickenKickingArena, thing)))
    arenaResources:RetireThingOutput(markerOutputToken)
    thing = nil
    thing = quest:GetThingWithScriptName("SecondLineMarker")
    markerOutputToken = arenaResources:TrackThingOutput(thing)
    local scratchValue6 = ("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(chickenKickingArena, thing)))
    arenaResources:RetireThingOutput(markerOutputToken)
    thing = nil
    thing = quest:GetThingWithScriptName("ThirdLineMarker")
    markerOutputToken = arenaResources:TrackThingOutput(thing)
    local scratchValue7 = ("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(chickenKickingArena, thing)))
    arenaResources:RetireThingOutput(markerOutputToken)
    thing = nil
    thing = quest:GetThingWithScriptName("FourthLineMarker")
    markerOutputToken = arenaResources:TrackThingOutput(thing)
    local scratchValue8 = ("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(chickenKickingArena, thing)))
    arenaResources:RetireThingOutput(markerOutputToken)
    thing = nil
    thing = quest:GetThingWithScriptName("RearRight")
    markerOutputToken = arenaResources:TrackThingOutput(thing)
    resources:DistanceBetweenThings(chickenKickingArena, thing)
    arenaResources:RetireThingOutput(markerOutputToken)
    thing = nil
    thing = quest:GetThingWithScriptName("MidRight")
    markerOutputToken = arenaResources:TrackThingOutput(thing)
    resources:DistanceBetweenThings(chickenKickingArena, thing)
    arenaResources:RetireThingOutput(markerOutputToken)
    thing = nil
    thing = quest:GetThingWithScriptName("MidRight2")
    markerOutputToken = arenaResources:TrackThingOutput(thing)
    resources:DistanceBetweenThings(chickenKickingArena, thing)
    arenaResources:RetireThingOutput(markerOutputToken)
    thing = nil
    thing = quest:GetThingWithScriptName("FrontRight")
    markerOutputToken = arenaResources:TrackThingOutput(thing)
    resources:DistanceBetweenThings(chickenKickingArena, thing)
    arenaResources:RetireThingOutput(markerOutputToken)
    thing = nil
    thing = quest:GetThingWithScriptName("FoulLineMarkerA")
    markerOutputToken = arenaResources:TrackThingOutput(thing)
    local getThingPosition = arenaResources:GetThingPosition(thing)
    local vec_3c = {x = getThingPosition.x, y = getThingPosition.y, z = getThingPosition.z}
    arenaResources:RetireThingOutput(markerOutputToken)
    thing = nil
    thing = quest:GetThingWithScriptName("FoulLineMarkerB")
    markerOutputToken = arenaResources:TrackThingOutput(thing)
    local getThingPosition2 = arenaResources:GetThingPosition(thing)
    local vec_24 = {x = getThingPosition2.x, y = getThingPosition2.y, z = getThingPosition2.z}
    arenaResources:RetireThingOutput(markerOutputToken)
    thing = nil
    quest:SetStateInt("DistanceBand", 0)
    repeat
        if not quest:NewScriptFrame(me) then return end
        if me:MsgIsKicked() then
            if quest:IsActiveThreadTerminating() then return end
            retailRandModulo = quest:RetailRandModulo(32768) & 0x80000003
            if retailRandModulo < 0 then
                retailRandModulo = (retailRandModulo - 1 | 0xfffffffc) + 1
            end
            local scratchValue11 = "SND_MM_CHICKEN_BUKARK_0" .. tostring(retailRandModulo + 1)
            quest:PlaySoundOnThing(me, scratchValue11)
        end
        local hero = quest:GetHero()
        local getThingPosition3 = arenaResources:GetThingPosition(hero)
        vec_30 = {x = getThingPosition3.x, y = getThingPosition3.y, z = getThingPosition3.z}
        local __native_vector_result_1 = quest:EntityGetShotStrikePos(me)
        local scratchValue = __native_vector_result_1 ~= nil
        if __native_vector_result_1 ~= nil then
            scratchValue12 = {x = __native_vector_result_1.x, y = __native_vector_result_1.y, z = __native_vector_result_1.z}
        end
        if scratchValue then
            if not quest:IsActiveThreadTerminating() then goto LAB_00e64831 end
            do return end
        end
    until (vec_24.y - vec_30.y) * (vec_3c.x - vec_30.x) - (vec_3c.y - vec_30.y) * (vec_24.x - vec_30.x) > 0.0
    if not quest:IsActiveThreadTerminating() then quest:SetStateInt("DistanceBand", 0xffffffff); goto LAB_00e64831 end
    do return end
    ::LAB_00e64831::
    if quest:IsActiveThreadTerminating() then return end
    if quest:GetStateInt("DistanceBand") == 0 then
        if quest:IsActiveThreadTerminating() then return end
        local getThingPosition4 = arenaResources:GetThingPosition(chickenKickingArena)
        local f_stk_20_1 = string.unpack("<f", string.pack("<f", scratchValue12.y - getThingPosition4.y))
        local scratchValue3 = string.unpack("<f", string.pack("<f", scratchValue12.x - getThingPosition4.x))
        do
            local inverseLength = 1.0 / math.sqrt(0.0 * 0.0 + f_stk_20_1 * f_stk_20_1 + scratchValue3 * scratchValue3)
            scratchValue4 = string.unpack("<f", string.pack("<f", scratchValue3 * inverseLength))
            f_stk_20_2 = string.unpack("<f", string.pack("<f", f_stk_20_1 * inverseLength))
            f_stk_1c_2 = string.unpack("<f", string.pack("<f", 0.0 * inverseLength))
        end
        if __native_named_marker_scope(quest, chickenKickingArena, scratchValue12, scratchValue4, f_stk_20_2, f_stk_1c_2, scratchValue5, scratchValue6, scratchValue7, scratchValue8) then return end
    end
    quest:SetStateBool("ChickenLanded", true)
    resources:PrepareResource(resources:MemberResource("seh_Chicken"))
    while not resources:TryAcquire(resources:MemberResource("seh_Chicken"), me, 4) do
        if not quest:NewScriptFrame(me) then return end
    end
    quest:IsActiveThreadTerminating()
end

-- KickedChicken.Main (retail 0x00e64210)
function Main(quest, me)
    return quest:WithRetailResources(function(arenaResources)
        return __native_arena_main(quest, me, arenaResources)
    end)
end

-- KickedChicken.Init (retail 0x00cdebb0)
function Init(quest, me)
end

-- KickedChicken.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- KickedChicken.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

