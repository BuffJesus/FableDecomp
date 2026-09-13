-- Reviewed root, watcher and complete entity-body reference. No FSE registration.
local Q = {}

function Q.init(h)
    h:set_piece_over(false)
end

function Q.main(h)
    h:bind("GTDI_Maze", 0)
    -- Both CScriptBase::PostAddScriptedEntities and CGSI+0x100.
    h:finalize_bindings()
    h:spawn("WaitForPieceOver", "")
    h:objective(h:active(), "TEXT_QUEST_GUARDIAN_TROPHY_DEALER_INFO_OBJECTIVE_01",
                "HeroGuildComplexInside", "HeroGuildComplexInside")
end

function Q.persist(h, context)
    h:set_piece_over(h:persist_bool(context, "PieceOver", h:piece_over(), false))
end

function Q.waiter(h)
    while not h:piece_over() do
        h:frame()
        if h:terminating() then return end
    end
    if h:terminating() then return end
    local maze = h:lookup("GTDI_Maze")
    while h:alive(maze) do
        h:frame()
        if h:terminating() then h:release_lookup(maze); return end
    end
    if h:terminating() then h:release_lookup(maze); return end
    h:deactivate(h:active(), 0)
    h:release_lookup(maze)
end

-- Native E2814A (hit) / E28411 (talk) to E2878A or cleanup.
-- The caller supplies the already acquired outer Maze resource. On success it
-- remains owned for the repeat loop; on cancellation this function releases it.
function Q.encounter(h, branch, maze, initial_angle)
    assert(branch == "hit" or branch == "talk")
    if h:terminating() then h:release(maze); return false end
    if branch == "hit" then
        h:ally("maze", "hero")
        h:ally("hero", "maze")
    end
    local hero = h:new_resource("hero")
    local function cancel()
        h:release(hero)
        h:release(maze)
        return false
    end
    h:prepare(hero)
    while not h:acquire(hero, "hero", 4) do
        h:frame()
        if h:terminating() then return cancel() end
    end
    if h:terminating() then return cancel() end
    h:prepare(maze)
    while not h:acquire(maze, "maze", 4) do
        h:frame()
        if h:terminating() then return cancel() end
    end
    if h:terminating() then return cancel() end
    local actors = h:new_actor_map()
    h:map_actor(actors, "MAZE", maze)
    h:map_actor(actors, "HERO", hero)
    local movie = h:start_movie("")
    h:pause(true)
    h:fix_camera(true)
    h:macro("CS_MAZE_TROPHY_INFO_SETUP", actors, false)
    if branch == "hit" then h:macro("CS_MAZE_TROPHY_INFO_HITME", actors, true) end
    h:macro("CS_MAZE_TROPHY_INFO", actors, true)
    h:fix_camera(false)
    h:pause(false)
    h:end_movie(movie)
    h:end_actor_map(actors)
    h:release(hero)
    if h:terminating() then h:release(maze); return false end
    h:quest_card("OBJECT_QUEST_CARD_FIND_TROPHY_DEALER", "V_TrophyDealer", false)
    local teleporter = h:lookup("WitchwoodTeleporter")
    h:teleporter(teleporter, true)
    h:release_lookup(teleporter)
    h:clear_information()
    h:remove_marker()
    h:facing(initial_angle, false)
    h:logbook(130)
    h:complete(h:active(), false, false, false)
    h:set_piece_over(true)
    if h:terminating() then h:release(maze); return false end
    return true
end

-- Native E28864 (hit) / E28AF8 (talk), after trigger selection. The caller
-- handles the subsequent use check / outer-loop frame. Resource ownership has
-- the same success/cancellation contract as encounter().
function Q.repeat_interaction(h, branch, maze)
    assert(branch == "hit" or branch == "talk")
    local function cancel() h:release(maze); return false end
    if h:terminating() then return cancel() end
    if branch == "hit" then
        h:ally("maze", "hero")
        h:ally("hero", "maze")
        h:prepare(maze)
        while not h:acquire(maze, "maze", 4) do
            h:frame()
            if h:terminating() then return cancel() end
        end
        if h:terminating() then return cancel() end
    end
    local movie = h:start_movie("")
    h:pause(true)
    h:face_hero(false)
    local function close()
        h:pause(false)
        h:end_movie(movie)
    end
    local function speak_if_alive(line)
        local thing = h:resource_thing(maze)
        local health = h:health(thing)
        h:release_thing(thing)
        if health > 0 then
            h:speak(maze, line, 0, false, true, false)
            while h:busy(maze) do
                h:frame()
                if h:terminating() then return false end
            end
            if h:terminating() then return false end
        end
        return true
    end
    if branch == "hit" and not speak_if_alive("TEXT_QST_077_MAZE_ON_HIT") then
        close(); return cancel()
    end
    if not speak_if_alive("TEXT_QST_077_MAZE_REPEAT") then close(); return cancel() end
    close()
    return true
end

function Q.entity_init(h)
    return h:angle()
end

function Q.entity_main(h, initial_angle)
    h:register_alive_condition()
    h:frame()
    if h:terminating() then return end
    h:information(false, true, false)
    local maze = h:new_resource("maze")
    h:prepare(maze)
    while not h:acquire(maze, "maze", 4) do
        h:frame()
        if h:terminating() then h:release(maze); return end
    end
    if h:terminating() then h:release(maze); return end
    h:killable(false, true)
    h:damageable(false)
    h:pushable(false)
    h:start_screen(h:active(), false, false)
    h:add_marker("HUD_ORB_QUEST_CORE")
    if h:terminating() then h:release(maze); return end
    local function was_hit()
        return h:hit() or (h:special() and not h:heal(14))
    end
    while true do
        if was_hit() then
            if not Q.encounter(h, "hit", maze, initial_angle) then return end
            break
        end
        if h:used() then
            if not Q.encounter(h, "talk", maze, initial_angle) then return end
            break
        end
        h:frame()
        if h:terminating() then h:release(maze); return end
    end
    while true do
        if was_hit() then
            if not Q.repeat_interaction(h, "hit", maze) then return end
        end
        -- Retail checks use even after a repeat hit in the same iteration.
        if h:used() then
            if not Q.repeat_interaction(h, "talk", maze) then return end
        end
        h:frame()
        if h:terminating() then h:release(maze); return end
    end
end

return Q
