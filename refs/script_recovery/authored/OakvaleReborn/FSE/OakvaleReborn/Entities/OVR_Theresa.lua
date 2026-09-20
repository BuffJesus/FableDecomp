-- DISABLED Theresa candidate: full native dispatcher and merged host validation pending.
-- Entity state: DoneIntro = native byte1D; AskedForPresent = native byte1C.
local entityFields = {}
local entityState = {
    GetStateBool = function(_, key) return entityFields[key] end,
    SetStateBool = function(_, key, value) entityFields[key] = value end,
}
-- Complete native Init DAC4F0: entity flags reset before the first engine call.
local function initializeTheresa(quest, me, resources, state)
    state:SetStateBool("DoneIntro", false)
    state:SetStateBool("AskedForPresent", false)
    resources:InitializeTheresaActor(me)
end


-- Native DB98E4..DB99CA. Control is already acquired by the caller.
-- Requires explicit-resource PlayTheresaSkip and raw-Hero proximity adapters.
local function waitForHeroToApproachTheresa(quest, me, resources, control)
    while true do
        if quest:IsActiveThreadTerminating() then return false end
        resources:PlayTheresaSkip(control)
        while not resources:IsTheresaNearHero(me, 5.0) do
            if not resources:IsPerformingScriptTask(control) then break end
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        if resources:IsTheresaNearHero(me, 5.0) then return true end
        quest:NewScriptFrame(me)
    end
end


-- Meeting/acceptance use role "THER"; the outro uses "Theresa".
local function newTheresaCutsceneActors(resources, heroControl, theresaControl, theresaRole)
    local actors = resources:NewActorMap()
    local ok, failure = pcall(function()
        resources:SetActor(actors, "HERO", heroControl)
        resources:SetActor(actors, theresaRole, theresaControl)
    end)
    if not ok then
        pcall(resources.DestroyActorMap, resources, actors)
        error(failure, 0)
    end
    return actors
end


-- Native DB9B7F..DB9C87. nil means cancellation; only answer 1 accepts.
local function askTheresaAboutChocolates(quest, me, resources)
    if quest:IsActiveThreadTerminating() then return nil end
    resources:ShowTheresaChocolateQuestion()
    local answer = quest:MsgIsQuestionAnsweredYesOrNo()
    while answer < 0 do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return nil end
        answer = quest:MsgIsQuestionAnsweredYesOrNo()
    end
    if quest:IsActiveThreadTerminating() then return nil end
    if answer ~= 1 then return false end
    if quest:IsActiveThreadTerminating() then return nil end
    return true
end


-- progress belongs to this Main invocation; it is not persistent quest state.
local function commitTheresaChocolates(quest, me, resources, progress)
    quest:SetStateBool("GivenTheresaChocs", true)
    resources:TakeTheresaChocolatesAndUpdateObjective()
    progress.givenChocolates = true
    resources:ClearTheresaInformation(me)
end


-- Native DB9C87..DB9E86: entered after affirmative-answer cancellation checks.
local function acceptTheresaChocolates(quest, me, resources, heroControl, theresaControl, progress)
    local guards = resources:NewTheresaGuardVector()
    local actors
    local ok, failure = pcall(function()
        guards:RemoveLivingGuards()
        actors = newTheresaCutsceneActors(resources, heroControl, theresaControl, "THER")
        resources:RunMacro("CS_OAKVALE_INTRO_THERESA_MEET_YES", actors, false, true)
        commitTheresaChocolates(quest, me, resources, progress)
    end)
    local actorsClosed, actorsError = true, nil
    if actors then
        actorsClosed, actorsError = pcall(resources.DestroyActorMap, resources, actors)
    end
    local guardsClosed, guardsError = pcall(guards.Close, guards)
    if not ok then error(failure, 0) end
    if not actorsClosed then error(actorsError, 0) end
    if not guardsClosed then error(guardsError, 0) end
end


-- Native DB99CA..DB9EC5, with cancellation cleanup at DBAEDF..DBAF24.
-- Called after waitForHeroToApproachTheresa; outer Main owns Theresa's control.
local function meetTheresa(quest, me, resources, theresaControl, state, progress)
    if quest:IsActiveThreadTerminating() then return false end
    if quest:IsActiveThreadTerminating() then return false end
    local heroControl = resources:NewResource()
    local actors, movie
    local ok, result = pcall(function()
        -- Native ignores the result of this one acquisition attempt.
        resources:TryAcquireTheresaHero(heroControl, 4)
        actors = newTheresaCutsceneActors(resources, heroControl, theresaControl, "THER")
        movie = resources:StartMovie("")
        resources:Pause(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_OAKVALE_INTRO_THERESA_MEET", actors, false, true)
        if resources:DoesTheresaHeroHaveChocolates() then
            local accepted = askTheresaAboutChocolates(quest, me, resources)
            if accepted == nil then return false end
            if accepted then
                acceptTheresaChocolates(quest, me, resources, heroControl, theresaControl, progress)
            end
        end
        quest:FixMovieSequenceCamera(false)
        state:SetStateBool("DoneIntro", true)
        return true
    end)
    local cleanupError
    local function cleanup(method, ...)
        local closed, failure = pcall(method, resources, ...)
        if not closed and cleanupError == nil then cleanupError = failure end
    end
    if movie then
        cleanup(resources.Pause, false)
        cleanup(resources.DestroyMovie, movie)
    end
    if actors then cleanup(resources.DestroyActorMap, actors) end
    cleanup(resources.ReleaseResource, heroControl)
    if not ok then error(result, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return result
end


-- Native DBA004..DBA10F/DBA31A, inside the later offer movie.
local function askTheresaOfferChoice(quest, me, resources)
    resources:ShowTheresaChocolateQuestion()
    local answer = quest:MsgIsQuestionAnsweredYesOrNo()
    while answer < 0 do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return nil end
        answer = quest:MsgIsQuestionAnsweredYesOrNo()
    end
    if quest:IsActiveThreadTerminating() then return nil end
    if quest:IsActiveThreadTerminating() then return nil end
    return answer == 1
end


-- All eight native health gates use ordered > 0 before temporary destruction.
local function speakTheresaIfAlive(quest, me, resources, control, line)
    local thing = resources:NewThingFromResource(control)
    local queried, alive = pcall(function() return resources:ThingHealth(thing) > 0.0 end)
    local released, releaseError = pcall(resources.DestroyThing, resources, thing)
    if not queried then error(alive, 0) end
    if not released then error(releaseError, 0) end
    if not alive then return true end
    resources:SpeakTheresa(control, line)
    while resources:IsPerformingScriptTask(control) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end


-- Native DB9FA0..DBA3DB. Main owns the presented-item text outside this scope.
local function offerTheresaChocolates(quest, me, resources, control, progress)
    if quest:IsActiveThreadTerminating() then return false end
    local movie = resources:StartMovie("")
    local guards
    local ok, result = pcall(function()
        resources:Pause(true)
        local accepted = askTheresaOfferChoice(quest, me, resources)
        if accepted == nil then return false end
        if accepted then
            guards = resources:NewTheresaGuardVector()
            guards:RemoveLivingGuards()
            if not speakTheresaIfAlive(quest, me, resources, control, "TEXT_QST_048_THERESA_HELLO") then
                return false
            end
            commitTheresaChocolates(quest, me, resources, progress)
            return true
        end
        return speakTheresaIfAlive(quest, me, resources, control, "TEXT_QST_048_THERESA_REALLY_GET_PRESENT")
    end)
    local guardsClosed, guardsError = true, nil
    if guards then guardsClosed, guardsError = pcall(guards.Close, guards) end
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not guardsClosed then error(guardsError, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end


-- Native DBA5A4..DBA814: the presented-item predicate has already matched.
-- Main owns that item's CString and the retained departure trigger.
local function acceptPresentedTheresaChocolates(quest, me, resources, control, progress)
    if quest:IsActiveThreadTerminating() then return false end
    local movie = resources:StartMovie("")
    local guards
    local ok, result = pcall(function()
        resources:Pause(true)
        guards = resources:NewTheresaGuardVector()
        guards:RemoveLivingGuards()
        if not speakTheresaIfAlive(quest, me, resources, control, "TEXT_QST_048_THERESA_HELLO") then
            return false
        end
        commitTheresaChocolates(quest, me, resources, progress)
        return true
    end)
    local guardsClosed, guardsError = true, nil
    if guards then guardsClosed, guardsError = pcall(guards.Close, guards) end
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not guardsClosed then error(guardsError, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end

-- Native DBA4F5..DBA583, DBA819..DBA8A6, shared destructor DBA3D6.
local function rejectTheresaPresent(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    local movie = resources:StartMovie("")
    local ok, result = pcall(function()
        resources:Pause(true)
        return speakTheresaIfAlive(quest, me, resources, control, "TEXT_QST_048_THERESA_BETTER_PRESENT")
    end)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end


-- Native DBA8E0..DBABAE, after the talk-message predicate succeeds.
-- entityState.askedForPresent is native entity byte 0x1C; progress is Main-local.
local function talkToTheresa(quest, me, resources, control, progress, entityState)
    if quest:IsActiveThreadTerminating() then return false end
    local movie = resources:StartMovie("")
    local ok, result = pcall(function()
        resources:Pause(true)
        local line
        local markAsked = false
        if progress.givenChocolates then
            line = "TEXT_QST_048_THERESA_HELLO"
        elseif not entityState.askedForPresent then
            markAsked = true
            line = "TEXT_QST_048_THERESA_GET_PRESENT"
        else
            line = "TEXT_QST_048_THERESA_REALLY_GET_PRESENT"
        end
        if quest:IsActiveThreadTerminating() then return false end
        if markAsked then entityState.askedForPresent = true end
        return speakTheresaIfAlive(quest, me, resources, control, line)
    end)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end


-- Native DBACE6..DBADED after ally updates and the bad deed.
local function playTheresaHitResponse(quest, me, resources, control)
    local movie = resources:StartMovie("")
    local ok, result = pcall(function()
        resources:Pause(true)
        return speakTheresaIfAlive(quest, me, resources, control, "TEXT_QST_048_THERESA_DONT_HIT")
    end)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local closed, closeError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not closed then error(closeError, 0) end
    return result
end


-- Native DBACA1 onward, entered only after the hit-message predicate is true.
local function handleTheresaHit(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    resources:SetVillagerHeroAllies(me)
    require("OakvaleReborn.native_quest_helpers").AddBadDeed(quest, me, 2)
    return playTheresaHitResponse(quest, me, resources, control)
end


-- Native DBB0E4..DBB2D8. Main still owns presented text and Theresa control.
local function finishTheresaChildhood(quest, me, resources, theresaControl)
    if quest:IsActiveThreadTerminating() then return false end
    quest:DisplayQuestInfo(false)
    quest:RemoveQuestInfoElement(quest:GetStateInt("GUIBullyHealthCounter"))
    quest:RemoveQuestInfoElement(quest:GetStateInt("GUIGoodDeedCounter"))
    quest:RemoveQuestInfoElement(quest:GetStateInt("GUIBarrelCounter"))
    local movie = resources:StartMovie("")
    local heroControl, actors
    local ok, failure = pcall(function()
        resources:Pause(true)
        heroControl = resources:NewResource()
        -- Native performs one attempt and proceeds even if acquisition fails.
        resources:TryAcquireTheresaHero(heroControl, 4)
        actors = newTheresaCutsceneActors(resources, heroControl, theresaControl, "Theresa")
        -- Oakvale Reborn: the raid FMV is gone. Refuse = the other road (CS_OVR_REFUSE);
        -- accept never reaches here (Stranger.Massacre raises AttackOver first).
        resources:RunMacro("CS_OVR_REFUSE", actors, false, true)
        quest:FadeScreenOut(0.5, 0.0)
        quest:OverrideMusic(25, false, false)
        quest:SetStateBool("AttackOver", true)
    end)
    local cleanupError
    local function cleanup(method, ...)
        local closed, err = pcall(method, resources, ...)
        if not closed and cleanupError == nil then cleanupError = err end
    end
    if actors then cleanup(resources.DestroyActorMap, actors) end
    if heroControl then cleanup(resources.ReleaseResource, heroControl) end
    cleanup(resources.Pause, false)
    cleanup(resources.DestroyMovie, movie)
    if not ok then error(failure, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return true
end


-- Native DBA490..DBA4F5/DBA5A4/DBA8AB, with string compare at DBA588.
-- The caller owns one output across both polls; each poll may replace its text.
local function classifyTheresaPresentedItem(resources, presented)
    local chocolates = "OBJECT_CHOCOLATE_BOX_UNGIVEABLE"
    if resources:PollPresentedItem(presented) and resources:PresentedItemMatches(presented, chocolates) then
        return "chocolates"
    end
    if resources:PollPresentedItem(presented) and not resources:PresentedItemMatches(presented, chocolates) then
        return "other-present"
    end
    return "none"
end


-- DB97F0 onward. Caller registers the native bound-conscious condition before entry.
-- State is the entity-state owner; quest holds parent quest state.
local function runTheresaMainAfterCondition(quest, me, resources, state)
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then return end
    local control = resources:NewResource()
    local trigger, presented
    local progress = {givenChocolates = false}
    local talkState = setmetatable({}, {
        __index = function(_, key)
            assert(key == "askedForPresent")
            return state:GetStateBool("AskedForPresent")
        end,
        __newindex = function(_, key, value)
            assert(key == "askedForPresent")
            state:SetStateBool("AskedForPresent", value)
        end,
    })
    local function finishIteration()
        if presented then
            resources:DestroyPresentedItemOutput(presented)
            presented = nil
        end
    end
    local ok, failure = pcall(function()
        trigger = resources:NewTheresaDepartureTrigger()
        if quest:IsActiveThreadTerminating() then return end
        while true do
            resources:PrepareResource(control)
            while not resources:TryAcquire(control, me, 4) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            if not state:GetStateBool("DoneIntro") then
                if quest:IsActiveThreadTerminating() then return end
                if not waitForHeroToApproachTheresa(quest, me, resources, control) then return end
                if not meetTheresa(quest, me, resources, control, state, progress) then return end
            else
                if quest:IsActiveThreadTerminating() then return end
                presented = resources:NewPresentedItemOutput(me)
                local checkDeparture = false
                if resources:TheresaTalkOffersChocolates(me) then
                    if not offerTheresaChocolates(quest, me, resources, control, progress) then return end
                    checkDeparture = progress.givenChocolates
                else
                    local gift = classifyTheresaPresentedItem(resources, presented)
                    if gift == "chocolates" then
                        if not acceptPresentedTheresaChocolates(quest, me, resources, control, progress) then return end
                        checkDeparture = true
                    elseif gift == "other-present" then
                        if not rejectTheresaPresent(quest, me, resources, control) then return end
                        checkDeparture = progress.givenChocolates
                    elseif resources:WasVillagerTalkedTo(me) then
                        if not talkToTheresa(quest, me, resources, control, progress, talkState) then return end
                        checkDeparture = progress.givenChocolates
                    elseif resources:IsHitByHeroExceptAbility(me, 14) then
                        if not handleTheresaHit(quest, me, resources, control) then return end
                        checkDeparture = progress.givenChocolates
                    elseif progress.givenChocolates then
                        checkDeparture = true
                    elseif not resources:IsPerformingScriptTask(control) then
                        if quest:IsActiveThreadTerminating() then return end
                        resources:PlayTheresaSkip(control)
                    end
                end
                if checkDeparture and quest:GetStateBool("StrangerOfferMade")
                        and not quest:GetStateBool("StrangerAccepted")
                        and resources:IsTheresaHeroNearTrigger(trigger) then
                    if not finishTheresaChildhood(quest, me, resources, control) then return end
                    finishIteration()
                    quest:IsActiveThreadTerminating()
                    return
                end
                finishIteration()
            end
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return end
        end
    end)
    local cleanupError
    local function cleanup(method, ...)
        local closed, err = pcall(method, resources, ...)
        if not closed and cleanupError == nil then cleanupError = err end
    end
    if presented then cleanup(resources.DestroyPresentedItemOutput, presented) end
    if trigger then cleanup(resources.DestroyThing, trigger) end
    cleanup(resources.ReleaseResource, control)
    if not ok then error(failure, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
end


function Init(quest, me)
    quest:WithRetailResources(function(resources)
        initializeTheresa(quest, me, resources, entityState)
    end)
end

function Main(quest, me)
    quest:RegisterBoundConsciousCondition()
    quest:WithRetailResources(function(resources)
        runTheresaMainAfterCondition(quest, me, resources, entityState)
    end)
end
