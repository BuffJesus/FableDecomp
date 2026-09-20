local entityFields = {}
local entityState = {
    GetStateBool = function(_, name) return entityFields[name] end,
    SetStateBool = function(_, name, value) entityFields[name] = value end,
}
-- Disabled Victim Init/Main library; entity state and quest helper are injected. No registration.
function VictimInit(quest, me, state)
    state:SetStateBool("DoneThanks", false)
    state:SetStateBool("DisplayedGameInfo", false)
    quest:WithRetailResources(function(resources)
        resources:InitializeVictimActor(me)
    end)
end
function VictimUpdateSubdued(quest, me, resources, control, bully, state)
    if quest:GetStateBool("BullySubdued") then
        if quest:IsActiveThreadTerminating() then return false end
        if not state:GetStateBool("DoneThanks") then
            if quest:IsActiveThreadTerminating() then return false end
            state:SetStateBool("DoneThanks", true)
            resources:PrepareResource(control)
            resources:SetRawScared(me, false)
            while not quest:GetStateBool("BullyRanOff") do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return false end
            end
            if quest:IsActiveThreadTerminating() then return false end
            resources:SetVictimReleasedState(me)
        end
    elseif quest:GetStateBool("VictimShake") then
        if quest:IsActiveThreadTerminating() then return false end
        quest:SetStateBool("VictimShake", false)
        resources:FaceTowardsRetainedThing(me, bully, false)
    end
    return true
end
function VictimSpeak(quest, resources, control, key, selection)
    local actor = resources:NewThingFromResource(control)
    local ok, positive = xpcall(function() return resources:ThingHealth(actor) > 0.0 end, function(err) return err end)
    local closed, err = pcall(function() resources:DestroyThing(actor) end)
    if not ok then error(positive, 0) end
    if not closed then error(err, 0) end
    if positive then
        resources:Speak(control, quest:GetHero(), key, selection or 0, false, true, false)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
    end
    return true
end

function VictimTalk(quest, me, resources, control, bully, state)
    if not resources:IsTalkedToByHero(me) then return true end
    if quest:IsActiveThreadTerminating() then return false end
    local subdued = quest:GetStateBool("BullySubdued")
    if quest:IsActiveThreadTerminating() then return false end
    if not subdued then
        resources:SetRawScared(me, false)
        resources:VictimFaceHero(me, false)
    end
    local movie = resources:NewMovie()
    local pauseAttempted = false
    local ok, complete = xpcall(function()
        resources:StartOwnedMovie(movie, "")
        pauseAttempted = true
        quest:PauseAllNonScriptedEntities(true)
        resources:PrepareResource(control)
        while not resources:TryAcquire(control, me, 4) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        local attacked = quest:GetStateBool("HeroAttackedVictim")
        if quest:IsActiveThreadTerminating() then return false end
        local key
        if subdued then
            key = attacked and "TEXT_QST_048_VICTIM_THANKS_AFTER_HIT" or "TEXT_QST_048_VICTIM_THANKS"
        else
            key = attacked and "TEXT_QST_048_VICTIM_PLEA_AFTER_ATTACK" or "TEXT_QST_048_VICTIM_PLEA"
        end
        if not VictimSpeak(quest, resources, control, key) then return false end
        if not subdued then
            resources:SetRawScared(me, true)
            resources:FaceTowardsRetainedThing(me, bully, false)
        end
        return true
    end, function(err) return err end)
    local cleanupError
    if pauseAttempted then
        local closed, err = pcall(function() quest:PauseAllNonScriptedEntities(false) end)
        if not closed then cleanupError = err end
    end
    local closed, err = pcall(function() resources:DestroyMovie(movie) end)
    if not closed and cleanupError == nil then cleanupError = err end
    if not ok then error(complete, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    if not complete then return false end
    if not state:GetStateBool("DisplayedGameInfo") then
        if quest:IsActiveThreadTerminating() then return false end
        local xbox = quest:IsXbox()
        if quest:IsActiveThreadTerminating() then return false end
        resources:DisplayRawGameInfo(xbox and "TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS" or "TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS_PC")
        while not quest:MsgIsGameInfoClickedPast() do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        state:SetStateBool("DisplayedGameInfo", true)
    end
    return true
end
function VictimAcquire(quest, resources, control, target)
    resources:PrepareResource(control)
    while not resources:TryAcquire(control, target(), 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

function VictimFirstHitMovie(quest, me, resources, control, bully)
    local hero = resources:NewResource()
    local actors, movie
    local pauseAttempted, cameraAttempted = false, false
    local ok, complete = xpcall(function()
        if not VictimAcquire(quest, resources, hero, function() return quest:GetHero() end) then return false end
        actors = resources:NewActorMap()
        resources:SetActor(actors, "HERO", hero)
        resources:SetActor(actors, "BRAT", control)
        movie = resources:NewMovie()
        resources:StartOwnedMovie(movie, "")
        pauseAttempted = true
        quest:PauseAllNonScriptedEntities(true)
        cameraAttempted = true
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_OAKVALEINTRO_BRATHIT", actors, false, true)
        quest:FixMovieSequenceCamera(false)
        cameraAttempted = false
        resources:ClearRawInformation(me)
        resources:FaceTowardsRetainedThing(me, bully, false)
        return true
    end, function(err) return err end)
    local cleanupError
    local function close(callback)
        local closed, err = pcall(callback)
        if not closed and cleanupError == nil then cleanupError = err end
    end
    if cameraAttempted then close(function() quest:FixMovieSequenceCamera(false) end) end
    if pauseAttempted then close(function() quest:PauseAllNonScriptedEntities(false) end) end
    if movie then close(function() resources:DestroyMovie(movie) end) end
    if actors then close(function() resources:DestroyActorMap(actors) end) end
    close(function() resources:ReleaseResource(hero) end)
    if not ok then error(complete, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return complete
end

function VictimBeginHit(quest, me, resources, control, bully, addBadDeed)
    if not resources:IsHitByHeroExceptAbility(me, 14) then return "continue" end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    resources:SetThingAsAlly(me, quest:GetHero())
    resources:SetThingAsAlly(quest:GetHero(), me)
    addBadDeed(2)
    quest:SetStateBool("HeroAttackedVictim", true)
    if quest:GetStateBool("GivenHeroTeddy") then return "repeat" end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    quest:SetStateBool("GivenHeroTeddy", true)
    if not VictimAcquire(quest, resources, control, function() return me end) then return "cancel" end
    if not VictimFirstHitMovie(quest, me, resources, control, bully) then return "cancel" end
    return "continue"
end
function VictimRepeatHit(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    local conversation = resources:NewConversation(me, false, false)
    local bully = resources:NewThingFromScriptName("NOVI_Bully")
    local movie
    local pauseAttempted = false
    local ok, complete = xpcall(function()
        if resources:ThingAlive(bully) then
            if quest:IsActiveThreadTerminating() then return false end
            resources:AddConversationPerson(conversation, bully)
            resources:AddVictimRepeatConversationLines(conversation, me, bully)
        else
            if quest:IsActiveThreadTerminating() then return false end
            movie = resources:NewMovie()
            resources:StartOwnedMovie(movie, "")
            pauseAttempted = true
            quest:PauseAllNonScriptedEntities(true)
            if not VictimAcquire(quest, resources, control, function() return me end) then return false end
            if not VictimSpeak(quest, resources, control, "TEXT_QST_048_VICTIM_EVIL_BROS", 2) then return false end
            resources:PrepareResource(control)
        end
        return true
    end, function(err) return err end)
    local cleanupError
    local function close(callback)
        local closed, err = pcall(callback)
        if not closed and cleanupError == nil then cleanupError = err end
    end
    if pauseAttempted then close(function() quest:PauseAllNonScriptedEntities(false) end) end
    if movie then close(function() resources:DestroyMovie(movie) end) end
    close(function() resources:DestroyThing(bully) end)
    if not ok then error(complete, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return complete
end
function VictimComplaint(quest, me, resources, control)
    if not quest:GetStateBool("VictimComplainsAboutLosingTeddy") then return true end
    if quest:IsActiveThreadTerminating() then return false end
    if not VictimAcquire(quest, resources, control, function() return me end) then return false end
    if not VictimSpeak(quest, resources, control, "TEXT_QST_048_VICTIM_EVIL_BROS_10") then return false end
    quest:SetStateBool("VictimComplainsAboutLosingTeddy", false)
    return true
end
function VictimMain(quest, me, state, addBadDeed)
    quest:RegisterBoundConsciousCondition()
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        local control = resources:NewResource()
        if not VictimAcquire(quest, resources, control, function() return me end) then return end
        local bully = resources:NewThingFromScriptName("NOVI_Bully")
        if quest:IsActiveThreadTerminating() then return end
        repeat
            if not VictimUpdateSubdued(quest, me, resources, control, bully, state) then return end
            if not VictimTalk(quest, me, resources, control, bully, state) then return end
            local hit = VictimBeginHit(quest, me, resources, control, bully, addBadDeed)
            if hit == "cancel" then return end
            if hit == "repeat" and not VictimRepeatHit(quest, me, resources, control) then return end
            if not VictimComplaint(quest, me, resources, control) then return end
            quest:NewScriptFrame()
        until quest:IsActiveThreadTerminating()
    end)
end

function Init(quest, me)
    VictimInit(quest, me, entityState)
end

function Main(quest, me)
    VictimMain(quest, me, entityState, function(deed)
        require("OakvaleReborn.native_quest_helpers").AddBadDeed(quest, me, deed)
    end)
end
