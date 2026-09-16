-- Native DB99CA..DB9EC5, with cancellation cleanup at DBAEDF..DBAF24.
-- Called after waitForHeroToApproachTheresa; outer Main owns Theresa's control.
local function meetTheresa(quest, me, resources, theresaControl, state, progress)
    if quest:IsActiveThreadTerminating() then return false end
    if quest:IsActiveThreadTerminating() then return false end
    local heroControl = resources:NewResource()
    local actors, movie
    local ok, result = pcall(function()
        -- Native ignores the result of this one acquisition attempt.
        resources:TryAcquireTheresaHero(heroControl, 4)
        actors = newTheresaCutsceneActors(resources, heroControl, theresaControl, "THER")
        movie = resources:StartMovie("")
        resources:Pause(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_OAKVALE_INTRO_THERESA_MEET", actors, false, true)
        if resources:DoesTheresaHeroHaveChocolates() then
            local accepted = askTheresaAboutChocolates(quest, me, resources)
            if accepted == nil then return false end
            if accepted then
                acceptTheresaChocolates(quest, me, resources, heroControl, theresaControl, progress)
            end
        end
        quest:FixMovieSequenceCamera(false)
        state:SetStateBool("DoneIntro", true)
        return true
    end)
    local cleanupError
    local function cleanup(method, ...)
        local closed, failure = pcall(method, resources, ...)
        if not closed and cleanupError == nil then cleanupError = failure end
    end
    if movie then
        cleanup(resources.Pause, false)
        cleanup(resources.DestroyMovie, movie)
    end
    if actors then cleanup(resources.DestroyActorMap, actors) end
    cleanup(resources.ReleaseResource, heroControl)
    if not ok then error(result, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return result
end

return meetTheresa
