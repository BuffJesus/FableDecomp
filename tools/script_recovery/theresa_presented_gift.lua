-- Native DBA5A4..DBA814: the presented-item predicate has already matched.
-- Main owns that item's CString and the retained departure trigger.
local function acceptPresentedTheresaChocolates(quest, me, resources, control, progress)
    if quest:IsActiveThreadTerminating() then return false end
    local movie = resources:StartMovie("")
    local guards
    local ok, result = pcall(function()
        resources:Pause(true)
        guards = resources:NewTheresaGuardVector()
        guards:RemoveLivingGuards()
        if not speakTheresaIfAlive(quest, me, resources, control, "TEXT_QST_048_THERESA_HELLO") then
            return false
        end
        commitTheresaChocolates(quest, me, resources, progress)
        return true
    end)
    local guardsClosed, guardsError = true, nil
    if guards then guardsClosed, guardsError = pcall(guards.Close, guards) end
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not guardsClosed then error(guardsError, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end

-- Native DBA4F5..DBA583, DBA819..DBA8A6, shared destructor DBA3D6.
local function rejectTheresaPresent(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    local movie = resources:StartMovie("")
    local ok, result = pcall(function()
        resources:Pause(true)
        return speakTheresaIfAlive(quest, me, resources, control, "TEXT_QST_048_THERESA_BETTER_PRESENT")
    end)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end

return acceptPresentedTheresaChocolates, rejectTheresaPresent
