-- Native DBA8E0..DBABAE, after the talk-message predicate succeeds.
-- entityState.askedForPresent is native entity byte 0x1C; progress is Main-local.
local function talkToTheresa(quest, me, resources, control, progress, entityState)
    if quest:IsActiveThreadTerminating() then return false end
    local movie = resources:StartMovie("")
    local ok, result = pcall(function()
        resources:Pause(true)
        local line
        local markAsked = false
        if progress.givenChocolates then
            line = "TEXT_QST_048_THERESA_HELLO"
        elseif not entityState.askedForPresent then
            markAsked = true
            line = "TEXT_QST_048_THERESA_GET_PRESENT"
        else
            line = "TEXT_QST_048_THERESA_REALLY_GET_PRESENT"
        end
        if quest:IsActiveThreadTerminating() then return false end
        if markAsked then entityState.askedForPresent = true end
        return speakTheresaIfAlive(quest, me, resources, control, line)
    end)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end

return talkToTheresa
