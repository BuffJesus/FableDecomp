local entityFields = {}
local entityState = {
    GetStateInt = function(_, name) return entityFields[name] end,
    SetStateInt = function(_, name, value) entityFields[name] = value end,
}
-- Disabled LiveFather composition: explicit state/helper injection, no quest registration.
function LiveFatherInit(quest, me, state)
    state:SetStateInt("PenniesGiven", 0)
    quest:WithRetailResources(function(resources)
        resources:InitializeLiveFatherActor(me)
    end)
end
function LiveFatherHandleHit(quest, me, resources, control, addBadDeed)
    if not resources:IsHitByHeroExceptAbility(me, 14) then return true end
    if quest:IsActiveThreadTerminating() then return false end
    resources:SetThingAsAlly(me, quest:GetHero())
    resources:SetThingAsAlly(quest:GetHero(), me)
    addBadDeed(2)
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
        local actor = resources:NewThingFromResource(control)
        local healthOk, positive = xpcall(function()
            return resources:ThingHealth(actor) > 0.0
        end, function(err) return err end)
        local closed, err = pcall(function() resources:DestroyThing(actor) end)
        if not healthOk then error(positive, 0) end
        if not closed then error(err, 0) end
        if positive then
            resources:Speak(control, quest:GetHero(), "TEXT_QST_048_DAD_TEMPER", 0, false, true, false)
            while resources:IsPerformingScriptTask(control) do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return false end
            end
            if quest:IsActiveThreadTerminating() then return false end
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
    return complete
end
-- Keep native integer overflow behavior at arithmetic boundaries.
local SIGNED_INT32_MAX = 2147483647
local UINT32_RANGE = 4294967296

local function wrapSignedInt32(value)
    local wrapped = value % UINT32_RANGE
    if wrapped > SIGNED_INT32_MAX then
        return wrapped - UINT32_RANGE
    end
    return wrapped
end

function LiveFatherPaymentSpeak(quest, resources, control, key)
    local actor = resources:NewThingFromResource(control)
    local ok, positive = xpcall(function()
        return resources:ThingHealth(actor) > 0.0
    end, function(err) return err end)
    local closed, err = pcall(function() resources:DestroyThing(actor) end)
    if not ok then error(positive, 0) end
    if not closed then error(err, 0) end
    if positive then
        resources:Speak(control, quest:GetHero(), key, 0, false, true, false)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
    end
    return true
end

function LiveFatherPaymentDialogue(quest, me, resources, control, state)
    local goodDeeds = wrapSignedInt32(quest:GetStateInt("GoodDeedsPerformed"))
    if goodDeeds == 0 and quest:GetStateInt("BadDeedsPerformed") == 0 then
        if quest:IsActiveThreadTerminating() then return false end
        return LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_DONE_NOTHING_YET")
    end
    if goodDeeds > wrapSignedInt32(state:GetStateInt("PenniesGiven")) then
        if quest:IsActiveThreadTerminating() then return false end
        local paid = wrapSignedInt32(state:GetStateInt("PenniesGiven"))
        local total = wrapSignedInt32(quest:GetStateInt("GoodDeedsPerformed"))
        local amount = wrapSignedInt32(total - paid)
        state:SetStateInt("PenniesGiven", wrapSignedInt32(paid + amount))
        resources:GiveRawHeroGold(amount)
        local badDeeds = quest:GetStateInt("BadDeedsPerformed")
        if quest:IsActiveThreadTerminating() then return false end
        local key = badDeeds == 0 and "TEXT_QST_048_DAD_GIVE_REWARD_JUST_GOOD" or "TEXT_QST_048_DAD_GIVE_REWARD_PART_BAD"
        if not LiveFatherPaymentSpeak(quest, resources, control, key) then return false end
        if resources:LiveFatherHeroHasChocolate() then
            if quest:IsActiveThreadTerminating() then return false end
            if not LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_GIVE_PRESENT") then return false end
            resources:ClearRawInformation(me)
        else
            local gold = wrapSignedInt32(quest:GetHeroGold())
            if quest:IsActiveThreadTerminating() then return false end
            if gold > 3 then
                if not LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_YOU_HAVE_ENOUGH") then return false end
                resources:ClearRawInformation(me)
            else
                if not LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_IS_ENOUGH") then return false end
                resources:SetActiveQuestObjective("TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01")
            end
        end
    else
        local badDeeds = wrapSignedInt32(quest:GetStateInt("BadDeedsPerformed"))
        if quest:IsActiveThreadTerminating() then return false end
        if badDeeds > 0 then
            return LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_ANTISOCIAL")
        end
        if not LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_DO_MORE") then return false end
        if resources:LiveFatherHeroHasChocolate() then
            if quest:IsActiveThreadTerminating() then return false end
            resources:ClearRawInformation(me)
            if not LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_GIVE_PRESENT_ALT") then return false end
        end
    end
    return true
end
function LiveFatherAcquire(quest, resources, control, target, priority, prepare)
    if prepare then resources:PrepareResource(control) end
    while not resources:TryAcquire(control, target(), priority) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

function LiveFatherMovie(quest, resources, body)
    local movie = resources:NewMovie()
    local pauseAttempted = false
    local ok, complete = xpcall(function()
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
    if not ok then error(complete, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return complete
end

function LiveFatherRoutine(quest, me, resources, control, state, addBadDeed)
    if quest:IsActiveThreadTerminating() then return false end
    repeat
        if not LiveFatherAcquire(quest, resources, control, function() return me end, 3, true) then return false end
        if resources:IsTalkedToByHero(me) then
            if quest:IsActiveThreadTerminating() then return false end
            if not LiveFatherAcquire(quest, resources, control, function() return me end, 4, true) then return false end
            if not LiveFatherMovie(quest, resources, function()
                return LiveFatherPaymentDialogue(quest, me, resources, control, state)
            end) then return false end
        end
        if not LiveFatherHandleHit(quest, me, resources, control, addBadDeed) then return false end
        quest:NewScriptFrame()
    until quest:IsActiveThreadTerminating()
    return false
end

function LiveFatherIntro(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    local hero = resources:NewResource()
    if not LiveFatherAcquire(quest, resources, hero, function() return quest:GetHero() end, 4, false) then
        resources:ReleaseResource(hero)
        return false
    end
    local actors = resources:NewActorMap()
    resources:SetActor(actors, "Hero", hero)
    resources:SetActor(actors, "Father", control)
    local complete = LiveFatherMovie(quest, resources, function()
        local cameraOk, cameraError = xpcall(function()
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_OAKVALE_INTRO_FATHER", actors, false, true)
        end, function(err) return err end)
        local cameraClosed, closeError = pcall(function() quest:FixMovieSequenceCamera(false) end)
        if not cameraOk then error(cameraError, 0) end
        if not cameraClosed then error(closeError, 0) end
        quest:SetAllSoundsAsMuted(false)
        quest:SetStateBool("DadFinishedIntro", true)
        quest:Pause(1.0)
        quest:CameraResetToViewBehindHero(0.0)
        quest:CameraDefault()
        local xbox = quest:IsXbox()
        if quest:IsActiveThreadTerminating() then return false end
        resources:DisplayRawGameInfo(xbox and "TEXT_QST_048_INSTRUCTION_HIGHLIGHTING" or "TEXT_QST_048_INSTRUCTION_HIGHLIGHTING_PC")
        while not quest:MsgIsGameInfoClickedPast() do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        local counterKey = resources:NewLiteralText("HUD_DEED_GOOD_ICON")
        local counterOk, counterError = xpcall(function()
            local counter = resources:AddLiveFatherGoodDeedCounter(counterKey)
            quest:SetStateInt("GUIGoodDeedCounter", counter)
        end, function(err) return err end)
        local keyClosed, keyError = pcall(function() resources:DestroyText(counterKey) end)
        if not counterOk then error(counterError, 0) end
        if not keyClosed then error(keyError, 0) end
        quest:DisplayQuestInfo(true)
        return true
    end)
    resources:DestroyActorMap(actors)
    resources:ReleaseResource(hero)
    return complete
end

function LiveFatherMain(quest, me, state, addBadDeed)
    quest:RegisterBoundConsciousCondition()
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        local control = resources:NewResource()
        if not LiveFatherAcquire(quest, resources, control, function() return me end, 4, true) then return end
        if not quest:GetStateBool("DadFinishedIntro") then
            if not LiveFatherIntro(quest, me, resources, control) then return end
        end
        LiveFatherRoutine(quest, me, resources, control, state, addBadDeed)
    end)
end

function Init(quest, me)
    LiveFatherInit(quest, me, entityState)
end

function Main(quest, me)
    LiveFatherMain(quest, me, entityState, function(deed)
        require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, deed)
    end)
end
