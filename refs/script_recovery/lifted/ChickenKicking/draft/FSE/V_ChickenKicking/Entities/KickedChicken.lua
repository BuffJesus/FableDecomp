-- Generated native draft: KickedChicken. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

local function __native_named_marker_scope(quest, r1, xStack_c, fStack_24, f_stk_20, f_stk_1c, f_stk_58, f_stk_5c, f_stk_60, f_stk_64)
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
            local pair = markerPositions:GetPosPair(index - 1, r1)
            local previous = {
                x = storeFloat32(pair.first.x - pair.second.x),
                y = storeFloat32(pair.first.y - pair.second.y),
                z = 0,
            }
            pair = markerPositions:GetPosPair(index, r1)
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
                local product = (rotated.y * f_stk_20 + rotated.z * f_stk_1c) + rotated.x * fStack_24
                if markerResources:CosTurnsLessThan(halfTurns, product) then
                    quest:SetStateInt("FinalSector", index)
                end
            end
        end
        if (r1 ~= nil and not r1:IsNull()) then
            r1:GetPos()
        end
        local distance = markerResources:DistanceFromThingToPosition(r1, xStack_c)
        local storedDistance = (("<f"):unpack(("<f"):pack(distance)))
        local distanceBand
        if distance > f_stk_64 then
            distanceBand = 4
        elseif storedDistance > f_stk_60 then
            distanceBand = 3
        elseif storedDistance > f_stk_5c then
            distanceBand = 2
        elseif storedDistance > f_stk_58 then
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
    local __native_vector_result_1, bVar1, cVar2, ctr_68, fStack_24, fVar10, fVar9, f_p1, f_stk_1c, f_stk_20, f_stk_2c, f_stk_40, f_stk_44, f_stk_58, f_stk_5c, f_stk_60, f_stk_64, native_arg_sequence_1, pCVar3, pCVar4, pRight, pfVar6, pfVar7, piVar8, r1, r2, uVar5, vec_24, vec_30, vec_3c, xStack_68, xStack_c
    local alive = true
    r1 = quest:GetThingWithScriptName("ChickenKickingArena")
    arenaResources:TrackThingOutput(r1)
    local markerOutputToken
    pCVar3 = quest:GetThingWithScriptName("FirstLineMarker")
    markerOutputToken = arenaResources:TrackThingOutput(pCVar3)
    f_stk_58 = (("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(r1, pCVar3))))
    arenaResources:RetireThingOutput(markerOutputToken)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("SecondLineMarker")
    markerOutputToken = arenaResources:TrackThingOutput(pCVar3)
    f_stk_5c = (("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(r1, pCVar3))))
    arenaResources:RetireThingOutput(markerOutputToken)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("ThirdLineMarker")
    markerOutputToken = arenaResources:TrackThingOutput(pCVar3)
    f_stk_60 = (("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(r1, pCVar3))))
    arenaResources:RetireThingOutput(markerOutputToken)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("FourthLineMarker")
    markerOutputToken = arenaResources:TrackThingOutput(pCVar3)
    f_stk_64 = (("<f"):unpack(("<f"):pack(resources:DistanceBetweenThings(r1, pCVar3))))
    arenaResources:RetireThingOutput(markerOutputToken)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("RearRight")
    markerOutputToken = arenaResources:TrackThingOutput(pCVar3)
    resources:DistanceBetweenThings(r1, pCVar3)
    arenaResources:RetireThingOutput(markerOutputToken)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("MidRight")
    markerOutputToken = arenaResources:TrackThingOutput(pCVar3)
    resources:DistanceBetweenThings(r1, pCVar3)
    arenaResources:RetireThingOutput(markerOutputToken)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("MidRight2")
    markerOutputToken = arenaResources:TrackThingOutput(pCVar3)
    resources:DistanceBetweenThings(r1, pCVar3)
    arenaResources:RetireThingOutput(markerOutputToken)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("FrontRight")
    markerOutputToken = arenaResources:TrackThingOutput(pCVar3)
    resources:DistanceBetweenThings(r1, pCVar3)
    arenaResources:RetireThingOutput(markerOutputToken)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("FoulLineMarkerA")
    markerOutputToken = arenaResources:TrackThingOutput(pCVar3)
    pCVar4 = arenaResources:GetThingPosition(pCVar3)
    vec_3c = {x = pCVar4.x, y = pCVar4.y, z = pCVar4.z}
    arenaResources:RetireThingOutput(markerOutputToken)
    pCVar3 = nil
    f_stk_2c = 0.0
    pCVar3 = quest:GetThingWithScriptName("FoulLineMarkerB")
    markerOutputToken = arenaResources:TrackThingOutput(pCVar3)
    pCVar4 = arenaResources:GetThingPosition(pCVar3)
    vec_24 = {x = pCVar4.x, y = pCVar4.y, z = pCVar4.z}
    arenaResources:RetireThingOutput(markerOutputToken)
    pCVar3 = nil
    f_stk_2c = 0.0
    quest:SetStateInt("DistanceBand", 0)
    repeat
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then goto LAB_00e64cf3 end
        cVar2 = me:MsgIsKicked()
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00e64cf3 end
            uVar5 = quest:RetailRandModulo(32768)
            uVar5 = uVar5 & 0x80000003
            if uVar5 < 0 then
                uVar5 = (uVar5 - 1 | 0xfffffffc) + 1
            end
            pRight = tostring(uVar5 + 1)
            xStack_68 = ("SND_MM_CHICKEN_BUKARK_0" .. pRight)
            r2 = quest:PlaySoundOnThing(me, xStack_68)
        end
        pCVar3 = quest:GetHero()
        pCVar4 = arenaResources:GetThingPosition(pCVar3)
        vec_30 = {x = pCVar4.x, y = pCVar4.y, z = pCVar4.z}
        __native_vector_result_1 = quest:EntityGetShotStrikePos(me)
        bVar1 = (__native_vector_result_1 ~= nil)
        if __native_vector_result_1 ~= nil then
            xStack_c = {x = __native_vector_result_1.x, y = __native_vector_result_1.y, z = __native_vector_result_1.z}
        end
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then goto LAB_00e64831 end
            goto LAB_00e64cf3
        end
    until (vec_24.y - vec_30.y) * (vec_3c.x - vec_30.x) - (vec_3c.y - vec_30.y) * (vec_24.x - vec_30.x) > 0.0
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        quest:SetStateInt("DistanceBand", 0xffffffff)
        goto LAB_00e64831
    end
    goto FLOW_past_lab_00e64831
    ::LAB_00e64831::
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        if quest:GetStateInt("DistanceBand") == 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00e64cf3 end
            pfVar6 = arenaResources:GetThingPosition(r1)
            f_stk_20 = (string.unpack("<f", string.pack("<f", xStack_c.y - pfVar6.y)))
            fStack_24 = (string.unpack("<f", string.pack("<f", xStack_c.x - pfVar6.x)))
            f_stk_1c = 0.0
            do
                local inverseLength = 1.0 / math.sqrt(f_stk_1c * f_stk_1c + f_stk_20 * f_stk_20 + fStack_24 * fStack_24)
                fStack_24 = (string.unpack("<f", string.pack("<f", fStack_24 * inverseLength)))
                f_stk_20 = (string.unpack("<f", string.pack("<f", f_stk_20 * inverseLength)))
                f_stk_1c = (string.unpack("<f", string.pack("<f", f_stk_1c * inverseLength)))
            end
            if __native_named_marker_scope(quest, r1, xStack_c, fStack_24, f_stk_20, f_stk_1c, f_stk_58, f_stk_5c, f_stk_60, f_stk_64) then return end
        end
        quest:SetStateBool("ChickenLanded", true)
        resources:PrepareResource(resources:MemberResource("seh_Chicken"))
        bVar1 = resources:TryAcquire(resources:MemberResource("seh_Chicken"), me, 4)
        while not bVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then goto LAB_00e64cf3 end
            bVar1 = resources:TryAcquire(resources:MemberResource("seh_Chicken"), me, 4)
        end
        alive = not quest:IsActiveThreadTerminating()
    end
    ::FLOW_past_lab_00e64831::
    ::LAB_00e64cf3::
end

function Main(quest, me)
    return quest:WithRetailResources(function(arenaResources)
        return __native_arena_main(quest, me, arenaResources)
    end)
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

