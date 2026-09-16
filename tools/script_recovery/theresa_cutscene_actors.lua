-- Meeting/acceptance use role "THER"; the outro uses "Theresa".
local function newTheresaCutsceneActors(resources, heroControl, theresaControl, theresaRole)
    local actors = resources:NewActorMap()
    local ok, failure = pcall(function()
        resources:SetActor(actors, "HERO", heroControl)
        resources:SetActor(actors, theresaRole, theresaControl)
    end)
    if not ok then
        pcall(resources.DestroyActorMap, resources, actors)
        error(failure, 0)
    end
    return actors
end

return newTheresaCutsceneActors
