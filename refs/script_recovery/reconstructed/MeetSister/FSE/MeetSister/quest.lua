-- Executable reconstruction of QS_MeetSister's native resource semantics.
-- This is a reference port, not an FSE-loadable package: h is the explicit
-- resource adapter specified in PORT_REVIEW_2026-09-12.md. No game deployment.
-- Native evidence: retail-entity-byte-recovery-20260912.json.
local Q = {}

function Q.init(h) end -- native 0xE28D30; entity Init 0xCDEBB0 is also empty.

function Q.main(h)
    -- Binding+0x18 -> CActiveEntityScriptBase+0x3C (Flags), not a binding ID.
    h:bind("MeetSisterMessenger", "MeetSister/Entities/MeetSisterMessenger", 1)
    h:bind("MeetSisterSister", "MeetSister/Entities/MeetSisterSister", 0)
    h:finalize_bindings()
    h:set_objective(h:active_quest(), "TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY",
                    "HauntedHouse", "BarrowFields")
end

local function acquire_bound(h, me, resource)
    h:prepare(resource)
    while not h:acquire(me, resource, 4) do
        h:frame()
        if h:terminating() then return false end
    end
    return not h:terminating()
end

function Q.messenger(h, me)
    local actors = h:new_map()
    while not h:region_loaded("ArenaExterior") do
        h:frame()
        if h:terminating() then
            h:destroy_map(actors)
            return
        end
    end
    if h:terminating() then
        h:destroy_map(actors)
        return
    end
    local own = h:new_resource("bound")
    local hero = h:new_resource("hero")
    if acquire_bound(h, me, own) then
        -- One call, return value deliberately ignored (0xE29494).
        h:acquire(h:hero(), hero, 4)
        h:map_actor(actors, "Hero", hero)
        h:map_actor(actors, "Messenger", own)
        local movie = h:start_movie("")
        h:pause(true)
        h:run_macro("CS_ARENA_LEAVE_THERESA_01", actors, nil, nil, false, true)
        -- Native has no post-macro termination gate. Remove is inside movie.
        h:remove(me, false, true)
        h:pause(false)
        h:destroy_movie(movie)
    end
    h:release(hero)
    h:release(own)
    h:destroy_map(actors)
end

function Q.sister(h, me)
    local own = h:new_resource("bound")
    local hero = h:new_resource("hero")
    if not acquire_bound(h, me, own) then
        h:release(hero)
        h:release(own)
        return
    end
    -- Again one unchecked native attempt (0xE29730), not a retry loop.
    h:acquire(h:hero(), hero, 4)
    local actors = h:new_map()
    h:map_actor(actors, "Hero", hero)
    h:map_actor(actors, "Sister", own)
    local movie = h:start_movie("")
    h:pause(true)
    h:run_macro("CS_ARENA_LEAVE_THERESA_02", actors, nil, nil, false, true)
    h:pause(false)
    h:destroy_movie(movie)
    h:destroy_map(actors)
    h:release(hero)
    h:remove(me, false, true)
    -- Native obtains the active name separately for both operations.
    h:complete(h:active_quest(), false, false, false)
    h:deactivate_later(h:active_quest(), 0)
    h:fade_in()
    h:release(own)
end

return Q
