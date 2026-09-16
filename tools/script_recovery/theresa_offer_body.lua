-- Native DB9FA0..DBA3DB. Main owns the presented-item text outside this scope.
local function offerTheresaChocolates(quest, me, resources, control, progress)
    if quest:IsActiveThreadTerminating() then return false end
    local movie = resources:StartMovie("")
    local guards
    local ok, result = pcall(function()
        resources:Pause(true)
        local accepted = askTheresaOfferChoice(quest, me, resources)
        if accepted == nil then return false end
        if accepted then
            guards = resources:NewTheresaGuardVector()
            guards:RemoveLivingGuards()
            if not speakTheresaIfAlive(quest, me, resources, control, "TEXT_QST_048_THERESA_HELLO") then
                return false
            end
            commitTheresaChocolates(quest, me, resources, progress)
            return true
        end
        return speakTheresaIfAlive(quest, me, resources, control, "TEXT_QST_048_THERESA_REALLY_GET_PRESENT")
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

return offerTheresaChocolates
