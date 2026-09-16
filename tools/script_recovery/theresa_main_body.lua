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
                if checkDeparture and resources:IsTheresaHeroNearTrigger(trigger) then
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

return runTheresaMainAfterCondition
