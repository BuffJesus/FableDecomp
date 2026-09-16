-- Native Main DAE1AA..DAE3B0, after control acquisition and termination check.
local function handleVillagerAttackedDialogue(quest, me, resources, control)
    local movie = resources:StartMovie("")
    local function movieBody()
        resources:Pause(true)
        local male = quest:EntityGetSex(me) == 1
        if quest:IsActiveThreadTerminating() then return false end
        local controlledVillager = resources:NewThingFromResource(control)
        local queried, alive = pcall(function()
            return resources:ThingHealth(controlledVillager) > 0.0
        end)
        local released, releaseError = pcall(resources.DestroyThing, resources, controlledVillager)
        if not queried then error(alive, 0) end
        if not released then error(releaseError, 0) end
        if alive then
            resources:SpeakVillagerAttacked(control, male)
            while resources:IsPerformingScriptTask(control) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return false end
            end
            if quest:IsActiveThreadTerminating() then return false end
        end
        return true
    end
    local ok, result = pcall(movieBody)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local released, releaseError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not released then error(releaseError, 0) end
    return result
end

return handleVillagerAttackedDialogue
