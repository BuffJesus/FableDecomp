-- Native DB7052..DB738A. Caller owns movie and control and handles cleanup.
local function speakBarrelThugConversation(quest, me, resources, control)
    local returned = quest:GetStateBool("BarrelManSpokenToHeroOnReturn")
    if quest:IsActiveThreadTerminating() then return false end
    local broken = quest:GetStateBool("BarrelBrokenPersistent")
    if quest:IsActiveThreadTerminating() then return false end
    local line, selection
    if returned then
        line = broken and "TEXT_QST_048_BARRELTHUG_OUTRO" or "TEXT_QST_048_BARRELTHUG_WHY_NOT_SMASH"
        selection = 0
    else
        line = broken and "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE" or "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT"
        selection = 2
    end
    local thing = resources:NewThingFromResource(control)
    local queried, alive = pcall(function() return resources:ThingHealth(thing) > 0.0 end)
    local released, releaseError = pcall(resources.DestroyThing, resources, thing)
    if not queried then error(alive, 0) end
    if not released then error(releaseError, 0) end
    if not alive then return true end
    resources:SpeakBarrelThug(control, line, selection)
    while resources:IsPerformingScriptTask(control) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

return speakBarrelThugConversation
