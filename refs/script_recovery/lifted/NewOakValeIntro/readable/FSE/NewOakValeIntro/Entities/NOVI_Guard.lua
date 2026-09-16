-- Disabled native Guard candidate. Pending runtime integration and lifecycle review.
function GuardControlledHealthPositive(resources, control)
    local actor = resources:NewThingFromResource(control)
    local health = resources:ThingHealth(actor)
    local positive = health > 0.0
    resources:DestroyThing(actor)
    return positive
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

function GuardApproach(quest, me, resources, control)
    local function hasUnpunishedDeeds()
        local badDeeds = quest:GetStateInt("BadDeedsPerformed")
        local punishedDeeds = quest:GetStateInt("GuardsDealtWithBadDeeds")
        local unpunishedDeeds = wrapSignedInt32(badDeeds - punishedDeeds)
        return unpunishedDeeds > 0
    end
    if not hasUnpunishedDeeds() then return "skip" end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    local count = quest:GetStateInt("BadDeedsPerformed")
    if count >= 3 then count = 3 end
    local range = resources:ReadGuardAlertRange(count)
    if not resources:IsDistanceBetweenThingsUnder(me, quest:GetHero(), range) then return "skip" end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    resources:SetThingAsAlly(me, quest:GetHero())
    resources:SetThingAsAlly(quest:GetHero(), me)
    resources:PrepareResource(control)
    while not resources:TryAcquire(control, me, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return "cancel" end
    end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    range = resources:ReadGuardLectureRange()
    if not resources:IsDistanceBetweenThingsUnder(me, quest:GetHero(), range) then
        if quest:IsActiveThreadTerminating() then return "cancel" end
        resources:GuardFaceHero(me, true, false)
        local conversation = resources:NewConversation(me, false, false)
        resources:AddRawConversationPerson(conversation, quest:GetHero())
        resources:GuardAddHeroConversationLine(conversation, "TEXT_QST_048_SCRMSG_GUARD_GOING_AFTER", me)
        resources:FollowThing(control, quest:GetHero(), 1.0, true)
    end
    resources:SetRawCutsceneBehaviour(me, 1)
    range = resources:ReadGuardLectureRange()
    while not resources:IsDistanceBetweenThingsUnder(me, quest:GetHero(), range) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return "cancel" end
        range = resources:ReadGuardLectureRange()
    end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    resources:SetRawCutsceneBehaviour(me, 2)
    if not hasUnpunishedDeeds() then return "claimed" end
    if quest:IsActiveThreadTerminating() then return "cancel" end
    return "lecture"
end
function GuardLecture(quest, resources, control)
    local function speak(key)
        if GuardControlledHealthPositive(resources, control) then
            resources:Speak(control, quest:GetHero(), key, 0, false, true, false)
            while resources:IsPerformingScriptTask(control) do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return false end
            end
            if quest:IsActiveThreadTerminating() then return false end
        end
        return true
    end
    local repeated = quest:GetStateBool("GuardsSpokenOnce")
    if quest:IsActiveThreadTerminating() then return false end
    if repeated then
        if not speak("TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN") then return false end
    else
        if not speak("TEXT_QST_048_GUARD_CAUGHT_YOU_10") then return false end
        if not speak("TEXT_QST_048_GUARD_CAUGHT_YOU_20") then return false end
    end
    local crimes = {"BARREL_BREAKING", "DERELICTION_OF_DUTY", "VIOLENCE", "TEDDY_TO_BULLY", "CONCEALED_AFFAIR"}
    for index, crime in ipairs(crimes) do
        if quest:GetStateBool("WhichBadDeedsPerformed_" .. (index - 1)) then
            if quest:IsActiveThreadTerminating() then return false end
            if not speak("TEXT_QST_048_GUARD_CRIME_" .. crime) then return false end
        end
    end
    if repeated then
        if not speak("TEXT_QST_048_GUARD_AFTER_READ_LIST") then return false end
    else
        for _, suffix in ipairs({30, 40, 50, 60}) do
            if not speak("TEXT_QST_048_GUARD_CAUGHT_YOU_" .. suffix) then return false end
        end
        quest:SetStateBool("GuardsSpokenOnce", true)
    end
    return true
end
function GuardTalk(quest, me, resources, control)
    if not me:IsTalkedToByHero() then return true end
    if quest:IsActiveThreadTerminating() then return false end
    resources:GuardFaceHero(me, false, false)
    local conversation = resources:NewConversation(me, false, false)
    resources:AddRawConversationPerson(conversation, quest:GetHero())
    local bad = quest:GetStateInt("BadDeedsPerformed")
    local key
    if bad == 0 and quest:GetStateInt("GoodDeedsPerformed") > 0 then
        key = "TEXT_QST_048_GUARD_ON_TALK_GOOD"
    elseif bad > 0 then
        key = "TEXT_QST_048_GUARD_ON_TALK_BAD"
    else
        key = "TEXT_QST_048_GUARD_ON_TALK_NEUTRAL"
    end
    if quest:IsActiveThreadTerminating() then return false end
    resources:GuardAddHeroConversationLine(conversation, key, me)
    while quest:IsConversationActive(conversation) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    if quest:IsActiveThreadTerminating() then return false end
    resources:PrepareResource(control)
    return true
end

function GuardAcquire(quest, me, resources, control)
    resources:PrepareResource(control)
    while not resources:TryAcquire(control, me, 4) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

function GuardHit(quest, me, resources, control)
    if not resources:IsHitByHeroExceptAbility(me, 14) then return true end
    if quest:IsActiveThreadTerminating() then return false end
    resources:SetThingAsAlly(me, quest:GetHero())
    resources:SetThingAsAlly(quest:GetHero(), me)
    if not GuardAcquire(quest, me, resources, control) then return false end
    local movie = resources:StartMovie("")
    resources:Pause(true)
    local active = true
    if GuardControlledHealthPositive(resources, control) then
        resources:Speak(control, quest:GetHero(), "TEXT_QST_048_GUARD_ON_HIT", 0, false, true, false)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then active = false; break end
        end
        if active then active = not quest:IsActiveThreadTerminating() end
    end
    if active then resources:PrepareResource(control) end
    resources:Pause(false)
    resources:DestroyMovie(movie)
    return active
end
function Init(quest, me)
    quest:WithRetailResources(function(resources)
        resources:InitializeGuardActor(me)
    end)
end
local function resourceBody(quest, me, resources)
    local control = resources:NewResource()
    if quest:IsActiveThreadTerminating() then return end
    while true do
        local phase = GuardApproach(quest, me, resources, control)
        if phase == "cancel" then return end
        if phase == "lecture" then
            quest:SetStateInt("GuardsDealtWithBadDeeds", quest:GetStateInt("BadDeedsPerformed"))
            if not GuardAcquire(quest, me, resources, control) then return end
            while not quest:IsHeroControlledByPlayer() do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            local movie = resources:StartMovie("")
            resources:Pause(true)
            resources:GuardFaceHero(me, false, true)
            local active = GuardLecture(quest, resources, control)
            resources:Pause(false)
            resources:DestroyMovie(movie)
            if not active then return end
            resources:PrepareResource(control)
        elseif phase == "claimed" then
            resources:PrepareResource(control)
        end
        if not GuardTalk(quest, me, resources, control) then return end
        if not GuardHit(quest, me, resources, control) then return end
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
end

function Main(quest, me)
    quest:WithRetailResources(function(resources)
        resourceBody(quest, me, resources)
    end)
end
