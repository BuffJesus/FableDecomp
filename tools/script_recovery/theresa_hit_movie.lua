-- Native DBACE6..DBADED after ally updates and the bad deed.
local function playTheresaHitResponse(quest, me, resources, control)
    local movie = resources:StartMovie("")
    local ok, result = pcall(function()
        resources:Pause(true)
        return speakTheresaIfAlive(quest, me, resources, control, "TEXT_QST_048_THERESA_DONT_HIT")
    end)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end

return playTheresaHitResponse
