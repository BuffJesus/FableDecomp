-- DISABLED resource candidate; requires unapplied Villager runtime/quest-owner proposal.
-- Generated native draft: NOVI_Villager. Review coverage report before use.
-- Not copied from the working port; registration remains disabled.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end


function Init(quest, me)
    __native_entity_state:SetStateBool("HeroDidHitMe", false)
    quest:WithRetailResources(function(resources)
        resources:InitializeVillager(me)
    end)
end

-- Native Main DAE1AA..DAE3B0, after control acquisition and termination check.
local function handleVillagerAttackedDialogue(quest, me, resources, control)
    local movie = resources:StartMovie("")
    local function movieBody()
        resources:Pause(true)
        local male = quest:EntityGetSex(me) == 1
        if quest:IsActiveThreadTerminating() then return false end
        local controlledVillager = resources:NewThingFromResource(control)
        local queried, alive = pcall(function()
            return resources:ThingHealth(controlledVillager) > 0.0
        end)
        local released, releaseError = pcall(resources.DestroyThing, resources, controlledVillager)
        if not queried then error(alive, 0) end
        if not released then error(releaseError, 0) end
        if alive then
            resources:SpeakVillagerAttacked(control, male)
            while resources:IsPerformingScriptTask(control) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return false end
            end
            if quest:IsActiveThreadTerminating() then return false end
        end
        return true
    end
    local ok, result = pcall(movieBody)
    local unpaused, pauseError = pcall(resources.Pause, resources, false)
    local released, releaseError = pcall(resources.DestroyMovie, resources, movie)
    if not ok then error(result, 0) end
    if not unpaused then error(pauseError, 0) end
    if not released then error(releaseError, 0) end
    return result
end


-- Native Main DAE3F3..DAE607. Caller has checked talk result and termination.
-- The outer resource scope owns control; this function owns only the suffix.
local function handleVillagerConversation(quest, me, resources, control, state)
    local suffix = resources:NewText()
    local function conversationBody()
        local male = quest:EntityGetSex(me) == 1
        if quest:IsActiveThreadTerminating() then return false end
        resources:AssignVillagerSuffix(suffix, male)
        resources:PrepareResource(control)
        while not resources:TryAcquire(control, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        local conversation = resources:StartVillagerTalkConversation(me)
        local heroHitVillager = state:GetStateBool("HeroDidHitMe")
        if quest:IsActiveThreadTerminating() then return false end
        resources:AddVillagerTalkLine(conversation, suffix, me, heroHitVillager)
        while quest:IsConversationActive(conversation) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        resources:PrepareResource(control)
        return true
    end
    local ok, result = pcall(conversationBody)
    -- Release the retained suffix before the caller releases the outer locals.
    local released, releaseError = pcall(resources.DestroyText, resources, suffix)
    if not ok then error(result, 0) end
    if not released then error(releaseError, 0) end
    return result
end


-- Native Main DAE6BC..DAE988. Conversation and retained key already exist.
-- speechLists must expose the quest's owned CString vectors, not copied Lua strings.
local function addVillagerAmbientLine(quest, me, resources, speechLists, key, conversation, getSpeechIndex)
    local badDeeds = quest:GetStateInt("BadDeedsPerformed")
    local category
    if badDeeds == 0 and quest:GetStateInt("GoodDeedsPerformed") > 0 then
        category = "good"
    elseif badDeeds > 0 and quest:GetStateInt("GoodDeedsPerformed") == 0 then
        category = "bad"
    elseif badDeeds > 0 and quest:GetStateInt("GoodDeedsPerformed") > 0 then
        category = "both"
    else
        category = "none"
    end
    if quest:IsActiveThreadTerminating() then return false end
    local male = quest:EntityGetSex(me) == 1
    if quest:IsActiveThreadTerminating() then return false end
    local index = getSpeechIndex(quest, me, speechLists:Count(category, male))
    -- The index helper can yield. Read the selected vector again at assignment.
    resources:AssignVillagerSpeechText(key, speechLists, category, male, index)
    resources:AddVillagerAmbientText(conversation, key, me)
    return true
end


function Main(quest, me)
    quest:RegisterBoundConsciousCondition()
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        local control = resources:NewResource()
        local ambientKey = resources:NewText()
        local function runVillager()
            while not quest:IsActiveThreadTerminating() do
                resources:PrepareResource(control)
                if resources:WasVillagerHit(me) then
                    if quest:IsActiveThreadTerminating() then return end
                    resources:SetVillagerHeroAllies(me)
                    require("OakvaleReborn.native_quest_helpers").AddBadDeed(quest, me, 2)
                    __native_entity_state:SetStateBool("HeroDidHitMe", true)
                    resources:PrepareResource(control)
                    while not resources:TryAcquire(control, me, 4) do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then return end
                    end
                    if quest:IsActiveThreadTerminating() then return end
                    if not handleVillagerAttackedDialogue(quest, me, resources, control) then return end
                else
                    local talkedTo = resources:WasVillagerTalkedTo(me)
                    if quest:IsActiveThreadTerminating() then return end
                    if talkedTo then
                        if not handleVillagerConversation(quest, me, resources, control, __native_entity_state) then return end
                    elseif resources:ShouldVillagerStartAmbientConversation(me, quest:GetStateInt("TalkIntermittentTimer")) then
                        if quest:IsActiveThreadTerminating() then return end
                        local conversation = resources:StartVillagerAmbientConversation(me, quest:GetStateInt("TalkIntermittentTimer"))
                        if not addVillagerAmbientLine(quest, me, resources, quest:GetVillagerSpeechLists(),
                                ambientKey, conversation, GetVillagerSpeechIndex) then return end
                    end
                end
                quest:NewScriptFrame(me)
            end
        end
        local ok, failure = pcall(runVillager)
        local keyReleased, keyError = pcall(resources.DestroyText, resources, ambientKey)
        local controlReleased, controlError = pcall(resources.ReleaseResource, resources, control)
        if not ok then error(failure, 0) end
        if not keyReleased then error(keyError, 0) end
        if not controlReleased then error(controlError, 0) end
    end)
end

function GetVillagerSpeechIndex(quest, me, param1)
    local randomChoice
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    local predicateResult = not alive
    while true do
        if predicateResult then
            return 0
        end
        randomChoice = quest:RetailRandModulo(param1)
        if randomChoice ~= quest:GetStateInt("lastVillagerSpeechIdx") then break end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        predicateResult = not alive
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return 0
    end
    quest:SetStateInt("lastVillagerSpeechIdx", randomChoice)
    alive = not quest:IsActiveThreadTerminating()
    return ((not (not alive)) and randomChoice or 0)
end

