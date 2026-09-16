-- DBEDA5..DBEEBC: one Hero acquisition attempt, even when it fails.
local function playPostAttackDadCutscene(quest, resources)
    quest:SetStateBool("DadFound", true)
    local heroControl, actors, movie
    local paused, fixedCamera = false, false
    local ok, failure = pcall(function()
        heroControl = resources:NewResource()
        resources:TryAcquirePostAttackHero(heroControl, 4)
        actors = resources:NewActorMap()
        resources:SetActor(actors, "HERO", heroControl)
        movie = resources:StartMovie("")
        resources:Pause(true)
        paused = true
        quest:FixMovieSequenceCamera(true)
        fixedCamera = true
        resources:RunMacro("CS_OAKVALEINTRO_HESDEADJIM", actors, false, true)
    end)
    local cleanupError
    local function cleanup(method, receiver, ...)
        local closed, err = pcall(method, receiver, ...)
        if not closed and cleanupError == nil then cleanupError = err end
    end
    if fixedCamera then cleanup(quest.FixMovieSequenceCamera, quest, false) end
    if paused then cleanup(resources.Pause, resources, false) end
    if movie then cleanup(resources.DestroyMovie, resources, movie) end
    if actors then cleanup(resources.DestroyActorMap, resources, actors) end
    if heroControl then cleanup(resources.ReleaseResource, resources, heroControl) end
    if not ok then error(failure, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
end

return playPostAttackDadCutscene
