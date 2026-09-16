-- Native DB6CFB..DB6F4E. Main owns the retained control throughout.
local function introduceBarrelThug(quest, me, resources, control, state)
    if not prepareBarrelThugIntroduction(quest, me, resources, control) then return false end
    local movie = resources:StartMovie("")
    local ok, result = pcall(function()
        resources:Pause(true)
        local thing = resources:NewThingFromResource(control)
        local queried, alive = pcall(function() return resources:ThingHealth(thing) > 0.0 end)
        local released, releaseError = pcall(resources.DestroyThing, resources, thing)
        if not queried then error(alive, 0) end
        if not released then error(releaseError, 0) end
        if alive then
            resources:SpeakBarrelThug(control, "TEXT_QST_048_BARRELTHUG_EXPLAIN", 0)
            while resources:IsPerformingScriptTask(control) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return false end
            end
            if quest:IsActiveThreadTerminating() then return false end
        end
        state:SetStateBool("DoneIntro", true)
        resources:FollowBarrelThugHero(control)
        return true
    end)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end

return introduceBarrelThug
