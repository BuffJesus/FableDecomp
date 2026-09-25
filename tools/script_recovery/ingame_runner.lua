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
R.faded = R.faded or {}
R.fightNames = R.fightNames or {}   -- enemies a script needs killed BY the hero (the Python side attacks)
R.fightDefs = R.fightDefs or {}     -- ... and definitions whose deaths an engine AI counts (a boss's summoned minions)
R.fightDrain = R.fightDrain ~= false
local quest_cache = {fight = {}}
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

-- Killing without the hero's sword, all checked live 2026-09-25:
--   * ModifyThingHealth(e, -N, true) drains health to 0 ("can kill"; false stops at 1) but the creature does NOT
--     die: it stands at 0 health with IsAlive() true (Wasp drones, wb3). Scripts that wait on GetHealth see it
--     (the Darkwood rock troll's Main: without it the traders stayed scared, te6).
--   * SetThingAsKilled does nothing; FadeOutAndKillEntity does kill (IsAlive false), so a creature left standing
--     at 0 is faded a few frames later -- scripts testing IsAlive (Wasp's all-dead gate) see the death.
--   * neither sends a killed-by message: enemies in R.fightNames (a script waits on MsgIsKilledBy, the Wasp
--     queen) are only drained to 1 health here, and the Python side lands the last blow with a real attack.
--   * a boss AI does not see a fade either: the queen summons CREATURE_HORNET_PICNIC minions at each phase and
--     stays invulnerable until they die; faded ones never count (wb4 stuck at health 29, phase 2). Those
--     definitions go in R.fightDefs and are killed for real too.
local function isFight(e)
    local def = e:GetDefName()
    for _, d in ipairs(R.fightDefs) do if def == d then return true end end
    for _, n in ipairs(R.fightNames) do
        for _, f in ipairs(quest_cache.fight[n] or {}) do if e:IsEqualTo(f) then return true end end
    end
    return false
end

-- one step of a real fight (called by the Python side, which then attacks with the mouse / a spell): the nearest
-- living R.fightNames enemy, optionally drained to 1 health, with the hero placed 2.5 units off it and facing it.
-- Returns "name hp distance" or "none".
function R.fightStep(quest)
    local hero = quest:GetHero()
    local best, bd, bn
    local function pick(list, n)
        for _, e in ipairs(list or {}) do
            if e:IsAlive() and not e:IsUnconscious() and quest:GetHealth(e) > 0 then
                local p, b = e:GetPos(), R.fightBounds
                if not b or (p.x > b[1] and p.y > b[2] and p.x < b[3] and p.y < b[4]) then
                    local d = quest:GetDistanceBetweenThings(hero, e)
                    if not bd or d < bd then best, bd, bn = e, d, n end
                end
            end
        end
    end
    -- a boss's minions first (the Wasp queen stays invulnerable while hers live), then the named enemies
    for _, d in ipairs(R.fightDefs) do
        local ok, list = pcall(quest.GetAllThingsWithDefName, quest, d)
        if ok then pick(list, d) end
    end
    if not best then
        for _, n in ipairs(R.fightNames) do pick(quest:GetAllThingsWithScriptName(n), n) end
    end
    if not best then return 'none' end
    if R.fightDrain and quest:GetHealth(best) > 1 then quest:ModifyThingHealth(best, -100000, false) end
    if bd > 4 then
        local p, h = best:GetPos(), hero:GetPos()
        local a = math.atan(h.y - p.y, h.x - p.x)
        local landing = {x = p.x + 2.5 * math.cos(a), y = p.y + 2.5 * math.sin(a), z = p.z}
        if R.fightBounds then
            local b = R.fightBounds
            landing.x = math.max(b[1] + 0.5, math.min(b[3] - 0.5, landing.x))
            landing.y = math.max(b[2] + 0.5, math.min(b[4] - 0.5, landing.y))
            landing.z = landing.z + 0.5
        end
        quest:EntityTeleportToPosition(hero, landing, 0, true, true)
    end
    quest:EntitySetFacingAngleTowardsThing(hero, best, true)
    return string.format('%s %.1f %.1f', bn, quest:GetHealth(best), bd)
end

local function clearAround(quest, hero, fl, positions)
    quest_cache.fight = {}
    for _, n in ipairs(R.fightNames) do quest_cache.fight[n] = quest:GetAllThingsWithScriptName(n) or {} end
    local function consider(e)
        if not (alive(e) and not e:IsEqualTo(hero) and quest:AreEntitiesEnemies(hero, e) and not isParty(quest, e, fl)) then
            return
        end
        local hp = quest:GetHealth(e)
        local p = e:GetPos()
        local key = string.format('%.0f,%.0f', p.x, p.y)
        if isFight(e) then
            if R.fightDrain and hp > 1 then quest:ModifyThingHealth(e, -100000, false) end
        elseif hp > 0 and not R.fading[key] then
            R.fading[key] = R.frame
            quest:ModifyThingHealth(e, -100000, true)
            R.kills = R.kills + 1
            say('drained ' .. tostring(e:GetDefName()))
        elseif (hp <= 0 or R.frame - R.fading[key] > 90) and not R.faded[key] and R.frame - (R.fading[key] or 0) > 10 then
            R.faded[key] = true
            e:FadeOutAndKillEntity(true, 0.3, true)
            say('faded ' .. tostring(e:GetDefName()))
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
        -- the planner's z is the LEV heightfield, which knows no water or bridges: under Barrow Fields' bridge it
        -- is the riverbed (te8 put the hero in the river). Never land below the engine's water surface.
        local okW, water = pcall(quest.GetWaterHeightAtPosition, quest, {x = nextWp.x, y = nextWp.y, z = nextWp.z})
        if okW and type(water) == 'number' and water > nextWp.z + 0.3 then
            say(string.format('skipped a hop into water at %.0f,%.0f (surface %.1f, ground %.1f)', nextWp.x, nextWp.y, water, nextWp.z))
            R.lastHop = R.frame
        else
            local angle = math.atan(nextWp.y - here.y, nextWp.x - here.x)
            quest:EntityTeleportToPosition(hero, {x = nextWp.x, y = nextWp.y, z = nextWp.z + 0.5}, angle, true, true)
            R.lastHop, R.pullAt, R.hop = R.frame, R.frame + 6, nextWp
            if nextWp.exit then R.exitFrame, R.exitRegion, R.exitWp, R.exitPull = R.frame, region, nextWp, nil end
        end
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
