-- In-game test runner: the per-frame reflexes of a hands-free playtest, running as a thread of a quest host
-- (loaded by tools/script_recovery/ingame_runner.py through the autopilot channel). The Python side plans and
-- reads `Runner.status`; everything that must happen within a frame happens here:
--   * keep the hero alive (top up health below `healBelow`)
--   * clear hostiles near the hero AND around the next waypoint before hopping there (Darkwood's exploding
--     mushrooms went off when the hero landed on them), through the engine -- never the party
--   * walk a planned route in hops facing the direction of travel (angle 0 left the camera behind walls),
--     pulling the followers to the hero after every hop instead of waiting for them to walk (they stalled
--     behind obstacles and scares; the quest scripts react to regions and events, not to the walk itself)
--   * step off and back onto a region exit that did not fire
-- Loaded with the host environment as _ENV, so `Runner` and `RunnerMain` are host globals and the thread
-- can be registered with quest:CreateThread("RunnerMain").

Runner = Runner or {}
local R = Runner
R.route = R.route or {}          -- waypoints {x=, y=, z=} (world space, z = ground), consumed in order
R.log = R.log or {}              -- recent events, newest last
R.kills = R.kills or 0
R.frame = R.frame or 0
R.healBelow = R.healBelow or 0.9
R.clearRadius = R.clearRadius or 18
R.hopEvery = R.hopEvery or 40    -- frames between hops
R.enemyNames = R.enemyNames or {}  -- script names to clear even when the area query misses them
R.partyNames = R.partyNames or {}  -- script names never cleared (escortees)
R.clear = (R.clear == nil) and true or R.clear
R.fading = R.fading or {}
R.minParty = R.minParty or 0     -- followers the quest expects (the planner keeps this current)
R.regionOf = R.regionOf or {}    -- map name -> region (from the planner): a route belongs to one region
R.running = true

local function say(s)
    R.log[#R.log + 1] = R.frame .. ' ' .. s
    if #R.log > 60 then table.remove(R.log, 1) end
end
R.say = say

local function alive(t)
    return t ~= nil and not t:IsNull() and t:IsAlive()
end

local function followers(quest, hero)
    local out = {}
    for _, t in ipairs(quest:GetFollowingEntityList(hero) or {}) do
        if alive(t) then out[#out + 1] = t end
    end
    return out
end

local function isParty(quest, e, fl)
    for _, f in ipairs(fl) do if e:IsEqualTo(f) then return true end end
    for _, n in ipairs(R.partyNames) do
        for _, p in ipairs(quest:GetAllThingsWithScriptName(n) or {}) do if e:IsEqualTo(p) then return true end end
    end
    return false
end

-- Kill by damage: ModifyThingHealth(e, -N, true) takes health to 0 and the creature dies (the third argument
-- is "can kill"; false stops at 1 health -- checked live 2026-09-25). Quest scripts see an ordinary death: the
-- Darkwood rock troll's Main waits for GetHealth(me) == 0 to stop the traders being scared, and a fade-kill
-- skipped that (the traders stayed scared in Darkwood6, run te6). FadeOutAndKillEntity stays as the fallback
-- for a target still standing on the next pass (SetThingAsKilled left a balverine at full health).
local function clearAround(quest, hero, fl, positions)
    local function consider(e)
        -- a creature killed by damage still reports IsAlive() while it lies there: health 0 is dead
        -- (without this the te7 run counted each corpse again whenever it slid onto a new rounded position)
        if alive(e) and quest:GetHealth(e) > 0 and not e:IsEqualTo(hero) and quest:AreEntitiesEnemies(hero, e)
                and not isParty(quest, e, fl) then
            local p = e:GetPos()
            local key = string.format('%.0f,%.0f', p.x, p.y)
            if not R.fading[key] then
                R.fading[key] = R.frame
                quest:ModifyThingHealth(e, -100000, true)
                R.kills = R.kills + 1
                say('killed ' .. tostring(e:GetDefName()))
            elseif R.frame - R.fading[key] > 90 and quest:GetHealth(e) > 0 then
                R.fading[key] = R.frame
                e:FadeOutAndKillEntity(true, 0.3, true)
                say('faded ' .. tostring(e:GetDefName()))
            end
        end
    end
    for _, pos in ipairs(positions) do
        for _, e in ipairs(quest:GetAllCreaturesInAreaWithScriptName('', pos, R.clearRadius) or {}) do consider(e) end
    end
    for _, n in ipairs(R.enemyNames) do
        for _, e in ipairs(quest:GetAllThingsWithScriptName(n) or {}) do
            if quest:GetDistanceBetweenThings(hero, e) < R.clearRadius * 1.5 then consider(e) end
        end
    end
end

function R.tick(quest)
    R.frame = R.frame + 1
    local hero = quest:GetHero()
    if hero == nil then return end
    local scene = quest:IsInCutscene() or quest:IsInMovieSequence()
    if quest:GetHeroHealthPercentage() < R.healBelow then quest:ChangeHeroHealthBy(1000, false, false) end
    local fl = followers(quest, hero)
    local here = hero:GetPos()
    local map = tostring(hero:GetCurrentMapName())
    local region = R.regionOf[map] or map
    if region ~= R.region then
        -- a region change (an exit fired, a cutscene moved us): the old route and the exit retry are void
        if R.region then say('region ' .. tostring(R.region) .. ' -> ' .. region) end
        R.region, R.route, R.exitFrame = region, {}, nil
    end
    if R.routeRegion and R.routeRegion ~= region then R.route, R.routeRegion = {}, nil end
    local nextWp = R.route[1]
    if R.clear and not scene and R.frame % 8 == 0 then
        local positions = {here}
        for _, f in ipairs(fl) do positions[#positions + 1] = f:GetPos() end
        if nextWp then positions[#positions + 1] = {x = nextWp.x, y = nextWp.y, z = nextWp.z} end
        clearAround(quest, hero, fl, positions)
    end
    if R.pullAt and R.frame >= R.pullAt then
        quest:TeleportAllFollowersToHeroPosition()
        R.pullAt = nil
    end
    local partyOk = #fl >= R.minParty
    if not partyOk and R.frame % 150 == 0 then say(string.format('waiting for party %d/%d', #fl, R.minParty)) end
    -- an exit takes the party only if it is at the hero's side: pull it first, step on a few frames later
    if not scene and nextWp and nextWp.exit and partyOk and not R.exitPull then
        quest:TeleportAllFollowersToHeroPosition()
        R.exitPull = R.frame
        nextWp = nil
    elseif R.exitPull and R.frame - R.exitPull < 12 then
        nextWp = nil
    end
    if not scene and nextWp and partyOk and (not R.lastHop or R.frame - R.lastHop >= R.hopEvery) then
        -- clear around the landing spot on this very frame too (the periodic sweep may be up to 8 frames old)
        if R.clear then clearAround(quest, hero, fl, {{x = nextWp.x, y = nextWp.y, z = nextWp.z}}) end
        table.remove(R.route, 1)
        local angle = math.atan(nextWp.y - here.y, nextWp.x - here.x)
        quest:EntityTeleportToPosition(hero, {x = nextWp.x, y = nextWp.y, z = nextWp.z + 0.5}, angle, true, true)
        R.lastHop, R.pullAt, R.hop = R.frame, R.frame + 6, nextWp
        if nextWp.exit then R.exitFrame, R.exitRegion, R.exitWp, R.exitPull = R.frame, region, nextWp, nil end
    end
    -- an exit that did not fire (the hero was already standing in it): step back, then onto it again
    if R.exitFrame and R.exitRegion == region and R.frame - R.exitFrame > 150 and #R.route == 0 and not scene then
        local w = R.exitWp
        local back = {x = w.x - 4 * math.cos(math.atan(w.y - here.y, w.x - here.x)), y = w.y - 4 * math.sin(math.atan(w.y - here.y, w.x - here.x)), z = w.z}
        if math.abs(here.x - w.x) < 3 and math.abs(here.y - w.y) < 3 then
            back = {x = w.x + 4, y = w.y, z = w.z}
        end
        R.route, R.routeRegion = {back, w}, region
        R.exitFrame = nil
        say('exit retry')
    end
    R.status = string.format('frame=%d map=%s region=%s pos=%.1f,%.1f,%.1f route=%d followers=%d party=%s kills=%d scene=%s hp=%.2f',
        R.frame, map, region, here.x, here.y, here.z, #R.route, #fl, partyOk and 'ok' or 'short', R.kills, tostring(scene), quest:GetHeroHealthPercentage())
end

function RunnerMain(quest)
    say('runner started')
    while R.running do
        if not quest:NewScriptFrame() then break end
        local ok, err = pcall(R.tick, quest)
        if not ok then
            R.status = 'ERROR ' .. tostring(err)
            say('error ' .. tostring(err))
        end
    end
    say('runner stopped')
end
