-- Offline reference. Explicit adapter operations preserve native query order.
-- No activation; native evidence is in ../runtime_evidence.
local Q = {}

function Q.init(h)
    h:set_bool("SwordTaken", false)
    h:set_bool("BookRead", false)
end

function Q.main(h)
    h:bind("EmptyGrave", "MazeResearch/Entities/EmptyGrave", 0)
    h:bind("HistoryBookcase", "MazeResearch/Entities/HistoryBookcase", 0)
    h:finalize_bindings()
    h:objective(h:active_quest(), "TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01", "HeroGuildComplexInside", "")
end

function Q.persist(h, context)
    for _, key in ipairs({"SwordTaken", "BookRead"}) do
        h:set_bool(key, h:persist_bool(context, key, h:get_bool(key)))
    end
end

function Q.history(h, me)
    if not h:get_bool("BookRead") then
        if h:terminating() then return end
        h:usable(me, true)
        h:add_marker(me, "HUD_ORB_QUEST_VIGNETTE")
        while not h:used(me) do
            h:frame()
            if h:terminating() then return end
        end
        if h:terminating() then return end
        h:objective(h:active_quest(), "TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_02",
                    "HeroGuildComplexInside", "")
        h:remove_marker(me)
        h:add_marker(h:lookup("EmptyGrave"), "HUD_ORB_QUEST_VIGNETTE")
        h:set_bool("BookRead", true)
        for _, suffix in ipairs({"10", "20", "30", "40"}) do
            h:display("TEXT_QST_B01_JOURNAL_ENTRY_" .. suffix)
            while not h:clicked() do
                h:frame()
                if h:terminating() then return end
            end
            if h:terminating() then return end
        end
    end
    repeat h:frame() until h:terminating()
end

function Q.unlimbo(h, flags, sword)
    while not h:flag(flags, "UNLIMBO") do
        h:frame()
        if h:terminating() then return end
    end
    if h:terminating() then return end
    h:limbo(sword, false, true)
    h:alpha(sword, 0.0, true)
end

function Q.grave(h, me)
    if h:get_bool("BookRead") then
        if h:terminating() then return end
        h:add_marker(me, "HUD_ORB_QUEST_VIGNETTE")
    end
    local sword = h:lookup("GoodSword")
    h:retain_sword(sword)
    h:limbo(sword, true, true)
    if h:terminating() then return end
    repeat
        if h:used(me) then
            if h:terminating() then return end
            if h:get_bool("BookRead") and not h:get_bool("SwordTaken")
                and h:master("PostSavePosition") > 1700
                and h:master("JackBossBattleResult") == 2 then
                if h:terminating() then return end
                local hero = h:new_resource()
                h:acquire(h:hero(), hero, 4) -- one unchecked call
                local actors = h:new_map()
                h:map_actor(actors, "HERO", hero)
                local movie = h:start_movie("")
                h:pause(true)
                h:skippable(true)
                h:spawn_unlimbo()
                local good = h:master("JackBossBattleHeroGoodAtEnd")
                if h:terminating() then
                    -- Native does not reset skippable on this cleanup path.
                    h:pause(false)
                    h:destroy_movie(movie)
                    h:destroy_map(actors)
                    h:release(hero)
                    return
                end
                local macro = good and "CS_GET_SWORD_OF_AEONS_SAINT" or "CS_GET_SWORD_OF_AEONS_REDEEM"
                h:run_macro(macro, actors, false, true)
                h:skippable(false)
                h:remove_marker(me)
                h:pause(false)
                h:destroy_movie(movie)
                h:destroy_map(actors)
                h:release(hero)
                h:set_bool("SwordTaken", true)
                h:remove_marker(me)
                h:deactivate(h:active_quest(), 0)
            end
        end
        h:frame()
    until h:terminating()
end

return Q
