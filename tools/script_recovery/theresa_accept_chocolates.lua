-- Native DB9C87..DB9E86: entered after affirmative-answer cancellation checks.
local function acceptTheresaChocolates(quest, me, resources, heroControl, theresaControl, progress)
    local guards = resources:NewTheresaGuardVector()
    local actors
    local ok, failure = pcall(function()
        guards:RemoveLivingGuards()
        actors = newTheresaCutsceneActors(resources, heroControl, theresaControl, "THER")
        resources:RunMacro("CS_OAKVALE_INTRO_THERESA_MEET_YES", actors, false, true)
        commitTheresaChocolates(quest, me, resources, progress)
    end)
    local actorsClosed, actorsError = true, nil
    if actors then
        actorsClosed, actorsError = pcall(resources.DestroyActorMap, resources, actors)
    end
    local guardsClosed, guardsError = pcall(guards.Close, guards)
    if not ok then error(failure, 0) end
    if not actorsClosed then error(actorsError, 0) end
    if not guardsClosed then error(guardsError, 0) end
end

return acceptTheresaChocolates
