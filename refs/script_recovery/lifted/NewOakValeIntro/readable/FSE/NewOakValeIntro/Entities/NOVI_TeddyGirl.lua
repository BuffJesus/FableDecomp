-- DISABLED TeddyGirl native candidate: pending host/state/scheduler integration.
-- Generated native draft: NOVI_TeddyGirl. Review coverage report before use.
-- Not copied from the working port; registration remains disabled.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function TeddyGirlControlledHealthPositive(resources, control)
    local actor = resources:NewThingFromResource(control)
    local ok, positive = xpcall(function()
        return resources:ThingHealth(actor) > 0.0
    end, function(err) return err end)
    local closed, err = pcall(function() resources:DestroyThing(actor) end)
    if not ok then error(positive, 0) end
    if not closed then error(err, 0) end
    return positive
end

function TeddyGirlPresentedKind(resources, output)
    if resources:PollPresentedItem(output) and resources:PresentedItemMatches(output, "OBJECT_TEDDY_BEAR_UNGIVEABLE") then
        return "teddy"
    end
    if resources:PollPresentedItem(output) and not resources:PresentedItemMatches(output, "OBJECT_TEDDY_BEAR_UNGIVEABLE") then
        return "other"
    end
    return "none"
end

function TeddyGirlInitialize(quest, resources, me, state)
    state:SetStateBool("DoneIntro", false)
    state:SetStateBool("FoundTeddy", false)
    state:SetStateBool("SpokeAboutFindingTeddy", false)
    state:SetStateBool("HeroHitMe", false)
    resources:InitializeTeddyGirlActor(me)
end

function TeddyGirlGiven(quest, resources, me, state, addGoodDeed)
    resources:TakeTeddyFromHero()
    state:SetStateBool("FoundTeddy", true)
    addGoodDeed()
    resources:ClearRawInformation(me)
    quest:SetMasterGameState("TeddySolution", "B")
end

function TeddyGirlSpeak(quest, resources, control, key)
    if TeddyGirlControlledHealthPositive(resources, control) then
        resources:Speak(control, quest:GetHero(), key, 0, false, true, false)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
    end
    return true
end

function TeddyGirlQuestion(quest, resources, control, state, given)
    if not state:GetStateBool("DoneIntro") then
        local hit = state:GetStateBool("HeroHitMe")
        if quest:IsActiveThreadTerminating() then return false end
        local key = hit and "TEXT_QST_048_TEDDYGIRL_PLEA_POST_BEATEN" or "TEXT_QST_048_TEDDYGIRL_LOST_TEDDY"
        if not TeddyGirlSpeak(quest, resources, control, key) then return false end
        state:SetStateBool("DoneIntro", true)
    end
    resources:GiveTeddyGirlQuestion()
    local answer = quest:MsgIsQuestionAnsweredYesOrNo()
    while answer < 0 do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
        answer = quest:MsgIsQuestionAnsweredYesOrNo()
    end
    if quest:IsActiveThreadTerminating() then return false end
    if quest:IsActiveThreadTerminating() then return false end
    if answer == 1 then
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_FOUND_TEDDY") then return false end
        given()
    else
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_REPEAT_PLEA") then return false end
    end
    return true
end

function TeddyGirlTalk(quest, me, resources, control, state)
    local found = state:GetStateBool("FoundTeddy")
    if not found and state:GetStateBool("HeroHitMe") then
        if quest:IsActiveThreadTerminating() then return false end
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_PLEA_POST_BEATEN") then return false end
        state:SetStateBool("DoneIntro", true)
    elseif not state:GetStateBool("DoneIntro") then
        if quest:IsActiveThreadTerminating() then return false end
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_LOST_TEDDY") then return false end
        state:SetStateBool("DoneIntro", true)
    elseif found then
        if quest:IsActiveThreadTerminating() then return false end
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_REPEAT_FOUND") then return false end
    elseif quest:GetStateBool("TeddyRuined") then
        if quest:IsActiveThreadTerminating() then return false end
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_BAD_FEELING") then return false end
        quest:ClearThingHasInformation(me)
    else
        if quest:IsActiveThreadTerminating() then return false end
        if not TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_REPEAT_PLEA") then return false end
    end
    return true
end

function TeddyGirlWithMovie(quest, resources, body)
    local movie = resources:NewMovie()
    local pauseAttempted = false
    local ok, result = xpcall(function()
        resources:StartOwnedMovie(movie, "")
        pauseAttempted = true
        quest:PauseAllNonScriptedEntities(true)
        return body()
    end, function(err) return err end)
    local cleanupError
    if pauseAttempted then
        local closed, err = pcall(function() quest:PauseAllNonScriptedEntities(false) end)
        if not closed then cleanupError = err end
    end
    local closed, err = pcall(function() resources:DestroyMovie(movie) end)
    if not closed and cleanupError == nil then cleanupError = err end
    if not ok then error(result, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return result
end

function TeddyGirlAcquire(quest, resources, me, control)
    resources:PrepareResource(control)
    while not resources:TryAcquire(control, me, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

function TeddyGirlPresentedResponse(quest, resources, me, control, state, kind, given)
    if kind == "none" then return true end
    if quest:IsActiveThreadTerminating() then return false end
    if kind == "other" then
        if state:GetStateBool("FoundTeddy") then return true end
        if quest:IsActiveThreadTerminating() then return false end
    end
    if not TeddyGirlAcquire(quest, resources, me, control) then return false end
    if kind == "teddy" then state:SetStateBool("DoneIntro", true) end
    return TeddyGirlWithMovie(quest, resources, function()
        local key = kind == "teddy" and "TEXT_QST_048_TEDDYGIRL_FOUND_TEDDY" or "TEXT_QST_048_TEDDYGIRL_DONT_WANT"
        local complete = TeddyGirlSpeak(quest, resources, control, key)
        if complete and kind == "teddy" then given() end
        return complete
    end)
end

function TeddyGirlHit(quest, resources, me, control, state, addBadDeed)
    if quest:IsActiveThreadTerminating() then return false end
    resources:SetThingAsAlly(me, quest:GetHero())
    resources:SetThingAsAlly(quest:GetHero(), me)
    state:SetStateBool("HeroHitMe", true)
    addBadDeed(2)
    if not TeddyGirlAcquire(quest, resources, me, control) then return false end
    return TeddyGirlWithMovie(quest, resources, function()
        return TeddyGirlSpeak(quest, resources, control, "TEXT_QST_048_TEDDYGIRL_DONT_HIT")
    end)
end

function TeddyGirlDeparture(quest, resources, me, control, bully)
    if not quest:GetStateBool("SpokeAboutFindingTeddy") then return true end
    if not resources:IsDistanceUnderThing(me, bully, 10.0) then return true end
    if quest:IsActiveThreadTerminating() then return false end
    local conversation = resources:NewConversation(me, false, false)
    resources:AddRawConversationPerson(conversation, quest:GetHero())
    resources:TeddyGirlAddHeroLine(conversation, me, "TEXT_QST_048_TEDDYGIRL_TEDDY_RUINED")
    quest:SetMasterGameState("TeddySolution", "C")
    if not TeddyGirlAcquire(quest, resources, me, control) then return false end
    local target = resources:NewThingFromScriptName("NOVI_AffairWife")
    local ok, result = xpcall(function()
        resources:TeddyGirlMoveToThing(control, target, 3.0, 1, nil, false, false, true)
        while resources:IsActorPositionOnScreen(me) do
            if resources:IsDistanceOver(me, quest:GetHero(), 20.0) then break end
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        resources:RemoveRawThing(me, false, true)
        return true
    end, function(err) return err end)
    local closed, err = pcall(function() resources:DestroyThing(target) end)
    if not ok then error(result, 0) end
    if not closed then error(err, 0) end
    return result
end
local function resourceBody(quest, me, resources)
    local control = resources:NewResource()
    local bully = resources:NewThingFromScriptName("NOVI_Bully")
    if quest:IsActiveThreadTerminating() then return end
    local function given()
        TeddyGirlGiven(quest, resources, me, __native_entity_state, function()
            require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
        end)
    end
    while true do
        local output = resources:NewPresentedItemOutput(me)
        if resources:TeddyGirlTalkedWithTeddy(me) then
            if quest:IsActiveThreadTerminating() then return end
            if not TeddyGirlAcquire(quest, resources, me, control) then return end
            if not TeddyGirlWithMovie(quest, resources, function()
                return TeddyGirlQuestion(quest, resources, control, __native_entity_state, given)
            end) then return end
        else
            local kind = TeddyGirlPresentedKind(resources, output)
            if not TeddyGirlPresentedResponse(quest, resources, me, control, __native_entity_state, kind, given) then return end
        end
        if not TeddyGirlDeparture(quest, resources, me, control, bully) then return end
        if resources:IsTalkedToByHero(me) then
            if quest:IsActiveThreadTerminating() then return end
            if not TeddyGirlAcquire(quest, resources, me, control) then return end
            if not TeddyGirlWithMovie(quest, resources, function()
                return TeddyGirlTalk(quest, me, resources, control, __native_entity_state)
            end) then return end
        end
        if resources:IsHitByHeroExceptAbility(me, 14) then
            if not TeddyGirlHit(quest, resources, me, control, __native_entity_state, function(amount)
                require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, amount)
            end) then return end
        end
        resources:PrepareResource(control)
        resources:DestroyPresentedItemOutput(output)
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
end

function Main(quest, me)
    quest:RegisterBoundConsciousCondition()
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        resourceBody(quest, me, resources)
    end)
end

function Init(quest, me)
    quest:WithRetailResources(function(resources)
        TeddyGirlInitialize(quest, resources, me, __native_entity_state)
    end)
end

function GivenTeddy(quest, me)
    quest:WithRetailResources(function(resources)
        TeddyGirlGiven(quest, resources, me, __native_entity_state, function()
            require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
        end)
    end)
end
