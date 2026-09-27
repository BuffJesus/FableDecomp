-- Readable native conversion: V_StatueMaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- V_StatueMaster.Main (retail 0x00ed3b30)
function Main(quest)
    local fret_0, fret_00, fret_01, fret_02, getPos, getPos2, getPos3, getPos4, getPos5, getPos6
    local getPos7, getPos8
    quest:AddEntityBinding("StatueMasterStatue", "V_StatueMaster/Entities/StatueMasterStatue")
    quest:AddEntityBinding("StatueMasterCellarDoors", "V_StatueMaster/Entities/StatueMasterCellarDoors")
    quest:AddEntityBinding("StatueMasterChest", "V_StatueMaster/Entities/StatueMasterChest")
    quest:FinalizeEntityBindings()
    local nothing = quest:GetThingWithScriptName("SM_Nothing")
    local bowerstone = quest:GetThingWithScriptName("SM_Bowerstone")
    local guild = quest:GetThingWithScriptName("SM_Guild")
    local greatwood = quest:GetThingWithScriptName("SM_Greatwood")
    local center = quest:GetThingWithScriptName("SM_Center")
    if center == nil then
        getPos5 = {x = 0, y = 0, z = 0}
    else
        getPos5 = center:GetPos()
    end
    if not (nothing ~= nil and not nothing:IsNull()) then
        getPos = {x = 0, y = 0, z = 0}
    else
        getPos = nothing:GetPos()
    end
    local scratchValue = math.atan(getPos.x - getPos5.x, getPos.y - getPos5.y) * 0.15915493667125702
    if scratchValue < 0.0 or 1.0 <= scratchValue then
        scratchValue = fret_0
        if fret_0 < 0.0 then
            scratchValue = fret_0 + 1.0
        end
    end
    quest:SetStateFloat("AnglesToFaceList_0", scratchValue)
    if not (center ~= nil and not center:IsNull()) then
        getPos6 = {x = 0, y = 0, z = 0}
    else
        getPos6 = center:GetPos()
    end
    if not (bowerstone ~= nil and not bowerstone:IsNull()) then
        getPos2 = {x = 0, y = 0, z = 0}
    else
        getPos2 = bowerstone:GetPos()
    end
    scratchValue = math.atan(getPos2.x - getPos6.x, getPos2.y - getPos6.y) * 0.15915493667125702
    if scratchValue < 0.0 or 1.0 <= scratchValue then
        scratchValue = fret_00
        if fret_00 < 0.0 then
            scratchValue = fret_00 + 1.0
        end
    end
    quest:SetStateFloat("AnglesToFaceList_1", scratchValue)
    if not (center ~= nil and not center:IsNull()) then
        getPos7 = {x = 0, y = 0, z = 0}
    else
        getPos7 = center:GetPos()
    end
    if not (guild ~= nil and not guild:IsNull()) then
        getPos3 = {x = 0, y = 0, z = 0}
    else
        getPos3 = guild:GetPos()
    end
    scratchValue = math.atan(getPos3.x - getPos7.x, getPos3.y - getPos7.y) * 0.15915493667125702
    if scratchValue < 0.0 or 1.0 <= scratchValue then
        scratchValue = fret_01
        if fret_01 < 0.0 then
            scratchValue = fret_01 + 1.0
        end
    end
    quest:SetStateFloat("AnglesToFaceList_2", scratchValue)
    if not (center ~= nil and not center:IsNull()) then
        getPos8 = {x = 0, y = 0, z = 0}
    else
        getPos8 = center:GetPos()
    end
    if not (greatwood ~= nil and not greatwood:IsNull()) then
        getPos4 = {x = 0, y = 0, z = 0}
    else
        getPos4 = greatwood:GetPos()
    end
    scratchValue = math.atan(getPos4.x - getPos8.x, getPos4.y - getPos8.y) * 0.15915493667125702
    if not ((scratchValue < 0.0) or (1.0 <= scratchValue)) then quest:SetStateFloat("AnglesToFaceList_3", scratchValue); return end
    scratchValue = fret_02
    if fret_02 < 0.0 then
        scratchValue = fret_02 + 1.0
    end
    quest:SetStateFloat("AnglesToFaceList_3", scratchValue)
end

-- V_StatueMaster.Init (retail 0x00ed3a80)
function Init(quest)
end

-- V_StatueMaster.GetStatuePointingPosition (retail 0x00ed4420)
function GetStatuePointingPosition(quest)
    local scratchValue
    local getTimeOfDay = quest:GetTimeOfDay()
    local f_st0 = ((getTimeOfDay % 100) * 0.01666666753590107 + math.tointeger(math.modf(getTimeOfDay / 100))) * 0.0416666679084301
    getTimeOfDay = math.tointeger(math.modf(f_st0))
    scratchValue = 1
    repeat
        if not (quest:GetStateFloat("AnglesToFaceList_" .. scratchValue) - 0.03999999910593033 < (f_st0 - getTimeOfDay)) then
            scratchValue = scratchValue + 1
        else
            if (f_st0 - getTimeOfDay) < quest:GetStateFloat("AnglesToFaceList_" .. scratchValue) + 0.03999999910593033 then
                return scratchValue
            end
            scratchValue = scratchValue + 1
        end
    until scratchValue >= 4
    return 0
end

-- V_StatueMaster.HasCameraMode (retail 0x00ed43d0)
-- ED43D0: bsim names this body CGameCameraManager::HasCameraMode (a homologous script member); no PDB name
function HasCameraMode(quest)
    local getTimeOfDay = quest:GetTimeOfDay()
    return ((getTimeOfDay % 100) * 0.016666668 + math.tointeger(math.modf(getTimeOfDay / 100))) * 0.0416666679084301
end

