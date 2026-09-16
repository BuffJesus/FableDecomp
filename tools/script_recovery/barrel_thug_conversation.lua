-- Native DB6F86..DB73A0, including the movie's cancellation exits.
-- Main retains control ownership and releases it after a false result.
local function runBarrelThugConversation(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    local movie = resources:StartMovie("")
    local ok, result = pcall(function()
        resources:Pause(true)
        resources:PrepareResource(control)
        while not resources:TryAcquire(control, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        return speakBarrelThugConversation(quest, me, resources, control)
    end)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end

return runBarrelThugConversation
