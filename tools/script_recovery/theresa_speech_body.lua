-- All eight native health gates use ordered > 0 before temporary destruction.
local function speakTheresaIfAlive(quest, me, resources, control, line)
    local thing = resources:NewThingFromResource(control)
    local queried, alive = pcall(function() return resources:ThingHealth(thing) > 0.0 end)
    local released, releaseError = pcall(resources.DestroyThing, resources, thing)
    if not queried then error(alive, 0) end
    if not released then error(releaseError, 0) end
    if not alive then return true end
    resources:SpeakTheresa(control, line)
    while resources:IsPerformingScriptTask(control) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

return speakTheresaIfAlive
