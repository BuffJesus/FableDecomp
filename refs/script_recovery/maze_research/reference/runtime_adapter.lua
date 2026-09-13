-- Source-build-only bridge. Quest owns flags/retained sword across isolated VMs.
local A = {}
local KEY = "MazeResearch"

function A.new(quest, resources)
    local h = {}
    function h:get_bool(key) return quest:GetStateBool(key) end
    function h:set_bool(key, value) quest:SetStateBool(key, value) end
    function h:persist_bool(...) return quest:PersistTransferBool(...) end
    function h:bind(...) quest:AddEntityBinding(...) end
    function h:finalize_bindings() quest:FinalizeEntityBindings() end
    function h:active_quest() return quest:GetActiveQuestName() end
    function h:objective(...) quest:SetQuestCardObjective(...) end
    function h:terminating() return quest:IsActiveThreadTerminating() end
    function h:frame() resources:Frame() end
    function h:usable(...) quest:SetThingAsUsable(...) end
    function h:add_marker(...) quest:MiniMapAddMarker(...) end
    function h:remove_marker(...) quest:MiniMapRemoveMarker(...) end
    function h:used(me) return me:MsgIsUsedByHero() end
    function h:lookup(name) return quest:GetThingWithScriptName(name) end
    function h:display(key) quest:DisplayGameInfo(key) end
    function h:clicked() return quest:MsgIsGameInfoClickedPast() end
    function h:master(key) return quest:GetMasterGameState(key) end
    function h:retain_sword(sword) quest:RetainRetailThing(KEY, sword) end
    function h:limbo(...) quest:EntitySetInLimbo(...) end
    function h:alpha(...) quest:EntitySetAlpha(...) end
    function h:flag(flags, name) return flags:Get(name) end
    function h:new_resource() return resources:NewResource() end
    function h:hero() return quest:GetHero() end
    function h:acquire(actor, resource, priority) return resources:TryAcquire(resource, actor, priority) end
    function h:new_map() return resources:NewActorMap() end
    function h:map_actor(...) resources:SetActor(...) end
    function h:start_movie(name) return resources:StartMovie(name) end
    function h:pause(value) resources:Pause(value) end
    function h:skippable(value) quest:SetCutsceneSkippable(value) end
    function h:spawn_unlimbo() quest:CreateThread("UnLimboSword", {region = ""}) end
    function h:run_macro(name, actors, setup, skip)
        resources:RunMacroWithFlags(name, actors, quest:RetailFlags(KEY), setup, skip)
    end
    function h:destroy_movie(movie) resources:DestroyMovie(movie) end
    function h:destroy_map(map) resources:DestroyActorMap(map) end
    function h:release(resource) resources:ReleaseResource(resource) end
    function h:deactivate(...) quest:DeactivateQuestLater(...) end
    return h
end

function A.grave_init(quest)
    quest:RetailFlags(KEY):Set("UNLIMBO", false)
end

function A.run(reference, entry, quest, me)
    quest:WithRetailResources(function(resources)
        local h = A.new(quest, resources)
        if entry == "unlimbo" then
            reference.unlimbo(h, quest:RetailFlags(KEY), quest:GetRetainedRetailThing(KEY))
        else
            reference[entry](h, me)
        end
    end)
end

return A
