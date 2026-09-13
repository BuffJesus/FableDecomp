-- Source-build-only adapter; no package registration or activation.
local A = {}

function A.new(quest, resources, me)
    local h = {}
    local function actor(name)
        if name == "maze" then return me end
        assert(name == "hero")
        return quest:GetHero()
    end
    function h:piece_over() return quest:GetStateBool("PieceOver") end
    function h:set_piece_over(v) quest:SetStateBool("PieceOver", v) end
    function h:persist_bool(...) return quest:PersistTransferBool(...) end
    function h:bind(name, flags)
        assert(name == "GTDI_Maze")
        quest:AddEntityBinding(name, "GuardianTrophyDealerInfo/Entities/GTDI_Maze", flags)
    end
    function h:finalize_bindings() quest:FinalizeEntityBindings() end
    function h:spawn(name, region) quest:CreateThread(name, {region = region}) end
    function h:active() return quest:GetActiveQuestName() end
    function h:objective(...) quest:SetQuestCardObjective(...) end
    function h:deactivate(...) quest:DeactivateQuestLater(...) end
    function h:register_alive_condition() quest:RegisterBoundAliveCondition() end
    function h:terminating() return quest:IsActiveThreadTerminating() end
    function h:frame() resources:Frame() end
    function h:angle() return me:GetAngleXY() end
    function h:hit() return me:MsgIsHitByHero() end
    function h:special() return me:MsgIsHitByAnySpecialAbilityFromHero() end
    function h:heal(id) assert(id == 14); return me:MsgIsHitByHealLifeFromHero() end
    function h:used() return me:IsTalkedToByHero() end
    function h:information(...) quest:SetThingHasInformation(me, ...) end
    function h:killable(...) quest:EntitySetAsKillable(me, ...) end
    function h:damageable(v) quest:EntitySetAsDamageable(me, v) end
    function h:pushable(v) quest:SetIsPushableByHero(me, v) end
    function h:start_screen(...) quest:KickOffQuestStartScreen(...) end
    function h:add_marker(key) quest:MiniMapAddMarker(me, key) end
    function h:remove_marker() quest:MiniMapRemoveMarker(me) end
    function h:clear_information() quest:ClearThingHasInformation(me) end
    function h:ally(a, b) quest:EntitySetThingAsAllyOfThing(actor(a), actor(b)) end
    function h:new_resource() return resources:NewResource() end
    function h:prepare(r) resources:PrepareResource(r) end
    function h:acquire(r, name, priority) return resources:TryAcquire(r, actor(name), priority) end
    function h:release(r) resources:ReleaseResource(r) end
    function h:new_actor_map() return resources:NewActorMap() end
    function h:map_actor(...) resources:SetActor(...) end
    function h:end_actor_map(m) resources:DestroyActorMap(m) end
    function h:start_movie(name) return resources:StartMovie(name) end
    function h:end_movie(m) resources:DestroyMovie(m) end
    function h:pause(v) resources:Pause(v) end
    function h:fix_camera(v) quest:FixMovieSequenceCamera(v) end
    function h:macro(name, actors, skippable) resources:RunMacro(name, actors, false, skippable) end
    function h:quest_card(...) quest:GiveQuestCardDirectly(...) end
    function h:lookup(name) return resources:NewThingFromScriptName(name) end
    function h:release_lookup(t) resources:DestroyThing(t) end
    function h:alive(t) return resources:ThingAlive(t) end
    function h:teleporter(t, v) resources:SetTeleporterActive(t, v) end
    function h:facing(...) quest:EntitySetFacingAngle(me, ...) end
    function h:face_hero(v) quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero(), v) end
    function h:logbook(id) quest:AddLogbookStoryEntry(id) end
    function h:complete(...) quest:SetQuestAsCompleted(...) end
    function h:resource_thing(r) return resources:NewThingFromResource(r) end
    function h:health(t) return resources:ThingHealth(t) end
    function h:release_thing(t) resources:DestroyThing(t) end
    function h:speak(r, key, selection, listen, sound2D, fade)
        resources:Speak(r, quest:GetHero(), key, selection, listen, sound2D, fade)
    end
    function h:busy(r) return resources:IsPerformingScriptTask(r) end
    return h
end

function A.run(reference, entry, quest, me, initial_angle)
    quest:WithRetailResources(function(resources)
        reference[entry](A.new(quest, resources, me), initial_angle)
    end)
end

return A
