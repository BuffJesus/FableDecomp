-- Generated from the same native helper bodies as the quest draft.
local GetStatuePointingPosition, helper_ED43D0
function GetStatuePointingPosition(quest, me)
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

function helper_ED43D0(quest, me)
    local iVar1 = quest:GetTimeOfDay()
    return ((iVar1 % 100) * 0.016666668 + math.tointeger(math.modf(iVar1 / 100))) * 0.0416666679084301
end

return {GetStatuePointingPosition = GetStatuePointingPosition, helper_ED43D0 = helper_ED43D0}
