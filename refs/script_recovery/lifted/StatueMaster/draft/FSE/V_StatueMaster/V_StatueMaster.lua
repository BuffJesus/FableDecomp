-- Generated native draft: V_StatueMaster. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local fret_0, fret_00, fret_01, fret_02, pfVar2, pfVar3
    quest:AddEntityBinding("StatueMasterStatue", "V_StatueMaster/Entities/StatueMasterStatue")
    quest:AddEntityBinding("StatueMasterCellarDoors", "V_StatueMaster/Entities/StatueMasterCellarDoors")
    quest:AddEntityBinding("StatueMasterChest", "V_StatueMaster/Entities/StatueMasterChest")
    quest:FinalizeEntityBindings()
    local r1 = quest:GetThingWithScriptName("SM_Nothing")
    local r2 = quest:GetThingWithScriptName("SM_Bowerstone")
    local r3 = quest:GetThingWithScriptName("SM_Guild")
    local r4 = quest:GetThingWithScriptName("SM_Greatwood")
    local r5 = quest:GetThingWithScriptName("SM_Center")
    if not (r5 ~= nil and not r5:IsNull()) then
        pfVar3 = {x = 0, y = 0, z = 0}
    else
        pfVar3 = r5:GetPos()
    end
    if not (r1 ~= nil and not r1:IsNull()) then
        pfVar2 = {x = 0, y = 0, z = 0}
    else
        pfVar2 = r1:GetPos()
    end
    local fVar6 = math.atan(pfVar2.x - pfVar3.x, pfVar2.y - pfVar3.y)
    fVar6 = fVar6 * 0.15915493667125702
    local f_stk_40 = fVar6
    if (fVar6 < 0.0) or (1.0 <= fVar6) then
        fVar6 = fret_0
        if fret_0 < 0.0 then
            fVar6 = fret_0 + 1.0
        end
    end
    quest:SetStateFloat("AnglesToFaceList_0", fVar6)
    if not (r5 ~= nil and not r5:IsNull()) then
        pfVar3 = {x = 0, y = 0, z = 0}
    else
        pfVar3 = r5:GetPos()
    end
    if not (r2 ~= nil and not r2:IsNull()) then
        pfVar2 = {x = 0, y = 0, z = 0}
    else
        pfVar2 = r2:GetPos()
    end
    fVar6 = math.atan(pfVar2.x - pfVar3.x, pfVar2.y - pfVar3.y)
    fVar6 = fVar6 * 0.15915493667125702
    f_stk_40 = fVar6
    if (fVar6 < 0.0) or (1.0 <= fVar6) then
        fVar6 = fret_00
        if fret_00 < 0.0 then
            fVar6 = fret_00 + 1.0
        end
    end
    quest:SetStateFloat("AnglesToFaceList_1", fVar6)
    if not (r5 ~= nil and not r5:IsNull()) then
        pfVar3 = {x = 0, y = 0, z = 0}
    else
        pfVar3 = r5:GetPos()
    end
    if not (r3 ~= nil and not r3:IsNull()) then
        pfVar2 = {x = 0, y = 0, z = 0}
    else
        pfVar2 = r3:GetPos()
    end
    fVar6 = math.atan(pfVar2.x - pfVar3.x, pfVar2.y - pfVar3.y)
    fVar6 = fVar6 * 0.15915493667125702
    f_stk_40 = fVar6
    if (fVar6 < 0.0) or (1.0 <= fVar6) then
        fVar6 = fret_01
        if fret_01 < 0.0 then
            fVar6 = fret_01 + 1.0
        end
    end
    quest:SetStateFloat("AnglesToFaceList_2", fVar6)
    if not (r5 ~= nil and not r5:IsNull()) then
        pfVar3 = {x = 0, y = 0, z = 0}
    else
        pfVar3 = r5:GetPos()
    end
    if not (r4 ~= nil and not r4:IsNull()) then
        pfVar2 = {x = 0, y = 0, z = 0}
    else
        pfVar2 = r4:GetPos()
    end
    fVar6 = math.atan(pfVar2.x - pfVar3.x, pfVar2.y - pfVar3.y)
    fVar6 = fVar6 * 0.15915493667125702
    f_stk_40 = fVar6
    if (fVar6 < 0.0) or (1.0 <= fVar6) then
        fVar6 = fret_02
        if fret_02 < 0.0 then
            fVar6 = fret_02 + 1.0
        end
    end
    quest:SetStateFloat("AnglesToFaceList_3", fVar6)
    r5 = nil
    r4 = nil
    r3 = nil
    r2 = nil
    r1 = nil
end

function Init(quest)
end

function GetStatuePointingPosition(quest)
    local iVar2
    local iVar1 = quest:GetTimeOfDay()
    local f_st0 = ((iVar1 % 100) * 0.01666666753590107 + math.tointeger(math.modf(iVar1 / 100))) * 0.0416666679084301
    iVar1 = math.tointeger(math.modf(f_st0))
    iVar2 = 1
    repeat
        if quest:GetStateFloat(("AnglesToFaceList_" .. iVar2)) - 0.03999999910593033 < (f_st0 - iVar1) then
            if (f_st0 - iVar1) < quest:GetStateFloat(("AnglesToFaceList_" .. iVar2)) + 0.03999999910593033 then
                return iVar2
            end
        end
        iVar2 = iVar2 + 1
    until not (iVar2 < 4)
    return 0
end

function helper_ED43D0(quest)
    local iVar1 = quest:GetTimeOfDay()
    return ((iVar1 % 100) * 0.016666668 + math.tointeger(math.modf(iVar1 / 100))) * 0.0416666679084301
end

