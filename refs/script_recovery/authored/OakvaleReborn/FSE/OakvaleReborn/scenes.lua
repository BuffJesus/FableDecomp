-- Oakvale Reborn: scene helpers shared by the quest and its entities.
--
-- Two mechanisms, both proven by the retail-shaped childhood scripts:
--   Scene.RunMacro  a script.bin CCutsceneDef (retail or CS_OVR_* appended by
--                   forge script cutscene-set) run through the retail macro
--                   interpreter inside one movie session; the shape is
--                   NewOakValeIntro.lua playPostAttackDadCutscene.
--   Scene.Lua       a small beat driven from Lua (letterbox, camera moves,
--                   Speak) for moments that do not deserve a def.
--
-- Entity control follows the v13 rule: one TryAcquire per actor per scene at
-- the scene's priority, released in the cleanup, never re-acquired mid-scene.
local Scene = {}

-- Acquire `thing` into `control` at `priority`, retrying frame by frame.
-- Returns false when the thread is torn down while waiting.
function Scene.Acquire(quest, resources, control, thing, priority, prepare)
    if prepare then resources:PrepareResource(control) end
    while not resources:TryAcquire(control, thing, priority) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

-- Acquire the hero for a scene. Retail scenes use priority 4 for the hero.
function Scene.AcquireHero(quest, resources, priority)
    local control = resources:NewResource()
    if not Scene.Acquire(quest, resources, control, quest:GetHero(), priority or 4, false) then
        resources:ReleaseResource(control)
        return nil
    end
    return control
end

local function runCleanup(steps)
    local firstError
    for _, step in ipairs(steps) do
        local closed, err = pcall(step)
        if not closed and firstError == nil then firstError = err end
    end
    return firstError
end

-- Run macro `name` with `actors` = { HERO = heroControl, Stranger = control, ... }
-- (already acquired controls). Pauses every non-scripted entity, fixes the
-- movie camera, and unwinds everything even when the macro throws.
function Scene.RunMacro(quest, resources, name, actors)
    local actorMap, movie
    local paused, fixedCamera = false, false
    quest:Log("OVR scene: macro " .. name)
    local ok, failure = pcall(function()
        actorMap = resources:NewActorMap()
        for actorName, control in pairs(actors) do
            resources:SetActor(actorMap, actorName, control)
        end
        movie = resources:StartMovie("")
        resources:Pause(true)
        paused = true
        quest:FixMovieSequenceCamera(true)
        fixedCamera = true
        resources:RunMacro(name, actorMap, false, true)
    end)
    local cleanupError = runCleanup({
        function() if fixedCamera then quest:FixMovieSequenceCamera(false) end end,
        function() if paused then resources:Pause(false) end end,
        function() if movie then resources:DestroyMovie(movie) end end,
        function() if actorMap then resources:DestroyActorMap(actorMap) end end,
    })
    quest:Log("OVR scene: macro " .. name .. " done ok=" .. tostring(ok))
    if not ok then error(failure, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return true
end

-- A Lua-driven beat: letterbox on, `body(scene)` runs, letterbox off and the
-- camera returns behind the hero whatever happened inside.
function Scene.Lua(quest, resources, label, body)
    quest:Log("OVR scene: lua " .. label)
    local started = false
    local ok, failure = pcall(function()
        quest:StartMovieSequence()
        started = true
        body()
    end)
    local cleanupError = runCleanup({
        function() if started then quest:EndMovieSequence() end end,
        function() quest:CameraResetToViewBehindHero(0.0) end,
        function() quest:CameraDefault() end,
    })
    quest:Log("OVR scene: lua " .. label .. " done ok=" .. tostring(ok))
    if not ok then error(failure, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return true
end

-- One spoken line from an acquired control to the hero (blocking retail Speak,
-- ETextGroupSelectionMethod 0 = the key itself).
function Scene.Say(quest, resources, control, key)
    resources:Speak(control, quest:GetHero(), key, 0, false, true, false)
end

-- Ask the hero a yes/no question and wait for the answer.
-- Returns true (yes), false (no), or nil when the thread is torn down.
function Scene.Ask(quest, questionKey)
    quest:GiveHeroYesNoQuestion(questionKey, "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true)
    local answer = quest:MsgIsQuestionAnsweredYesOrNo()
    while answer < 0 do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return nil end
        answer = quest:MsgIsQuestionAnsweredYesOrNo()
    end
    return answer == 1
end

-- Wait `seconds` of script frames without leaving the thread unresponsive.
function Scene.Wait(quest, seconds)
    quest:Pause(seconds)
    return not quest:IsActiveThreadTerminating()
end

return Scene
