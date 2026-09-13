-- Bridge the reviewed reference to the offline runtime resource API.
-- Not installed: this requires the source build containing WithRetailResources.
local A = {}

function A.new(quest, resources)
    local h = {}
    function h:bind(name, path, flags) quest:AddEntityBinding(name, path, flags) end
    function h:finalize_bindings() quest:FinalizeEntityBindings() end
    function h:set_objective(...) quest:SetQuestCardObjective(...) end
    function h:active_quest() return quest:GetActiveQuestName() end
    function h:region_loaded(region) return quest:IsRegionLoaded(region) end
    function h:terminating() return quest:IsActiveThreadTerminating() end
    function h:hero() return quest:GetHero() end
    function h:remove(...) quest:RemoveThing(...) end
    function h:complete(...) quest:SetQuestAsCompleted(...) end
    function h:deactivate_later(...) quest:DeactivateQuestLater(...) end
    function h:fade_in() quest:FadeScreenIn() end

    function h:new_resource(_) return resources:NewResource() end
    function h:prepare(id) resources:PrepareResource(id) end
    function h:acquire(actor, id, priority) return resources:TryAcquire(id, actor, priority) end
    function h:release(id) resources:ReleaseResource(id) end
    function h:frame() resources:Frame() end
    function h:new_map() return resources:NewActorMap() end
    function h:map_actor(map, name, resource) resources:SetActor(map, name, resource) end
    function h:destroy_map(map) resources:DestroyActorMap(map) end
    function h:run_macro(name, map, flags, input, setup, skippable)
        assert(flags == nil and input == nil, "Meet Sister uses no flags or substitutions")
        resources:RunMacro(name, map, setup, skippable)
    end
    function h:start_movie(name) return resources:StartMovie(name) end
    function h:pause(value) resources:Pause(value) end
    function h:destroy_movie(movie) resources:DestroyMovie(movie) end
    return h
end

function A.run(reference, entry, quest, me)
    if entry == "main" or entry == "init" then
        return reference[entry](A.new(quest, nil))
    end
    quest:WithRetailResources(function(resources)
        reference[entry](A.new(quest, resources), me)
    end)
end

return A
