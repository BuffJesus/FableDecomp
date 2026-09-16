-- Native DB79FC..DB7BB0 after the hit predicate and its string cleanup.
local function handleBarrelThugHit(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    resources:SetVillagerHeroAllies(me)
    require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
    local movie = resources:StartMovie("")
    local ok, result = pcall(function()
        resources:Pause(true)
        resources:PrepareResource(control)
        while not resources:TryAcquire(control, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        local thing = resources:NewThingFromResource(control)
        local queried, alive = pcall(function() return resources:ThingHealth(thing) > 0.0 end)
        local released, releaseError = pcall(resources.DestroyThing, resources, thing)
        if not queried then error(alive, 0) end
        if not released then error(releaseError, 0) end
        if not alive then return true end
        resources:SpeakBarrelThug(control, "TEXT_QST_048_BARRELTHUG_WHY_HIT", 0)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        return not quest:IsActiveThreadTerminating()
    end)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end

return handleBarrelThugHit
