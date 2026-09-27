-- Generated from the same native helper bodies as the quest draft.
local GetStatuePointingPosition, HasCameraMode
function GetStatuePointingPosition(quest, me)
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

-- ED43D0: bsim names this body CGameCameraManager::HasCameraMode (a homologous script member); no PDB name
function HasCameraMode(quest, me)
    local getTimeOfDay = quest:GetTimeOfDay()
    return ((getTimeOfDay % 100) * 0.016666668 + math.tointeger(math.modf(getTimeOfDay / 100))) * 0.0416666679084301
end

return {GetStatuePointingPosition = GetStatuePointingPosition, HasCameraMode = HasCameraMode}
