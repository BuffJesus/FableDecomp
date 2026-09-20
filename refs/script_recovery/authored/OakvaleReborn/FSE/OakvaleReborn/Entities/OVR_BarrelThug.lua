-- Disabled native-backed BarrelThug phase composition. Runtime proposal remains unapplied.
local entityFields = {}
local entityState = {}
for _, kind in ipairs({"Bool", "Int"}) do
    entityState["GetState" .. kind] = function(_, name) return entityFields[name] end
    entityState["SetState" .. kind] = function(_, name, value) entityFields[name] = value end
end
-- Native DB6BF0..DB6C31. Entity state stores precede all actor calls.
local function initializeBarrelThug(quest, me, resources, state)
    state:SetStateBool("DoneIntro", false)
    state:SetStateInt("LastTimeSpoken", 9999)
    resources:InitializeBarrelThugActor(me)
end

-- Native DB6CFB..DB6E20. Main owns control; the intro movie begins next.
local function prepareBarrelThugIntroduction(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    resources:PrepareResource(control)
    while not resources:TryAcquire(control, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return false end
    end
    if quest:IsActiveThreadTerminating() then return false end
    while not quest:GetStateBool("BarrelManLeftHeroInCharge") do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return false end
    end
    if quest:IsActiveThreadTerminating() then return false end
    resources:PlaceBarrelThugAtStart(me)
    quest:Pause(3.0)
    return true
end

-- Native DB6CFB..DB6F4E. Main owns the retained control throughout.
local function introduceBarrelThug(quest, me, resources, control, state)
    if not prepareBarrelThugIntroduction(quest, me, resources, control) then return false end
    local movie = resources:StartMovie("")
    local ok, result = pcall(function()
        resources:Pause(true)
        local thing = resources:NewThingFromResource(control)
        local queried, alive = pcall(function() return resources:ThingHealth(thing) > 0.0 end)
        local released, releaseError = pcall(resources.DestroyThing, resources, thing)
        if not queried then error(alive, 0) end
        if not released then error(releaseError, 0) end
        if alive then
            resources:SpeakBarrelThug(control, "TEXT_QST_048_BARRELTHUG_EXPLAIN", 0)
            while resources:IsPerformingScriptTask(control) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return false end
            end
            if quest:IsActiveThreadTerminating() then return false end
        end
        state:SetStateBool("DoneIntro", true)
        resources:FollowBarrelThugHero(control)
        return true
    end)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end

-- Native DB7052..DB738A. Caller owns movie and control and handles cleanup.
local function speakBarrelThugConversation(quest, me, resources, control)
    local returned = quest:GetStateBool("BarrelManSpokenToHeroOnReturn")
    if quest:IsActiveThreadTerminating() then return false end
    local broken = quest:GetStateBool("BarrelBrokenPersistent")
    if quest:IsActiveThreadTerminating() then return false end
    local line, selection
    if returned then
        line = broken and "TEXT_QST_048_BARRELTHUG_OUTRO" or "TEXT_QST_048_BARRELTHUG_WHY_NOT_SMASH"
        selection = 0
    else
        line = broken and "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE" or "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT"
        selection = 2
    end
    local thing = resources:NewThingFromResource(control)
    local queried, alive = pcall(function() return resources:ThingHealth(thing) > 0.0 end)
    local released, releaseError = pcall(resources.DestroyThing, resources, thing)
    if not queried then error(alive, 0) end
    if not released then error(releaseError, 0) end
    if not alive then return true end
    resources:SpeakBarrelThug(control, line, selection)
    while resources:IsPerformingScriptTask(control) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

-- Native DB6F86..DB73A0, including the movie's cancellation exits.
-- Main retains control ownership and releases it after a false result.
local function runBarrelThugConversation(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    local movie = resources:StartMovie("")
    local ok, result = pcall(function()
        resources:Pause(true)
        resources:PrepareResource(control)
        while not resources:TryAcquire(control, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        return speakBarrelThugConversation(quest, me, resources, control)
    end)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end

-- Native DB73A0..DB7924. Timer reads are intentionally repeated per threshold.
local function updateBarrelThugTimedRemarks(quest, me, resources, control, state)
    local function remaining()
        return resources:GetBarrelWatchTimer(quest:GetStateInt("WatchTimer"))
    end
    if quest:GetStateBool("BarrelManSpokenToHeroOnReturn") or remaining() <= 0 then
        if quest:IsActiveThreadTerminating() then return false end
        resources:ResetResource(control)
        return true
    end
    if quest:IsActiveThreadTerminating() then return false end
    local broken = quest:GetStateBool("BarrelBrokenPersistent")
    if quest:IsActiveThreadTerminating() then return false end
    local conversation = resources:NewBarrelThugRemarkConversation(me)
    local thresholds = broken and {10, 25, 35, 45} or {10, 20, 25, 30, 34, 38, 45}
    local prefix = broken and "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_" or "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_"
    for index, threshold in ipairs(thresholds) do
        if remaining() < threshold and state:GetStateInt("LastTimeSpoken") > threshold then
            if quest:IsActiveThreadTerminating() then return false end
            resources:AddBarrelThugRemark(conversation, me, prefix .. tostring(index * 10))
            state:SetStateInt("LastTimeSpoken", remaining())
            break
        end
    end
    return true
end

-- Native DB79FC..DB7BB0 after the hit predicate and its string cleanup.
local function handleBarrelThugHit(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    resources:SetVillagerHeroAllies(me)
    require("OakvaleReborn.native_quest_helpers").AddBadDeed(quest, me, 2)
    local movie = resources:StartMovie("")
    local ok, result = pcall(function()
        resources:Pause(true)
        resources:PrepareResource(control)
        while not resources:TryAcquire(control, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        local thing = resources:NewThingFromResource(control)
        local queried, alive = pcall(function() return resources:ThingHealth(thing) > 0.0 end)
        local released, releaseError = pcall(resources.DestroyThing, resources, thing)
        if not queried then error(alive, 0) end
        if not released then error(releaseError, 0) end
        if not alive then return true end
        resources:SpeakBarrelThug(control, "TEXT_QST_048_BARRELTHUG_WHY_HIT", 0)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        return not quest:IsActiveThreadTerminating()
    end)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end

-- Caller registers the bound-conscious condition. Original entry resumes DB6CB0.
local function runBarrelThugMainAfterCondition(quest, me, resources, state)
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then return end
    local control = resources:NewResource()
    local ok, failure = pcall(function()
        if quest:IsActiveThreadTerminating() then return end
        while true do
            if not state:GetStateBool("DoneIntro") then
                if not introduceBarrelThug(quest, me, resources, control, state) then return end
            end
            if resources:WasVillagerTalkedTo(me) then
                if not runBarrelThugConversation(quest, me, resources, control) then return end
            end
            if not updateBarrelThugTimedRemarks(quest, me, resources, control, state) then return end
            if resources:IsHitByHeroExceptAbility(me, 14) then
                if not handleBarrelThugHit(quest, me, resources, control) then return end
            end
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return end
        end
    end)
    local released, releaseError = pcall(resources.ReleaseResource, resources, control)
    if not ok then error(failure, 0) end
    if not released then error(releaseError, 0) end
end

function Init(quest, me)
    quest:WithRetailResources(function(resources)
        initializeBarrelThug(quest, me, resources, entityState)
    end)
end

function Main(quest, me)
    quest:RegisterBoundConsciousCondition()
    quest:WithRetailResources(function(resources)
        runBarrelThugMainAfterCondition(quest, me, resources, entityState)
    end)
end
