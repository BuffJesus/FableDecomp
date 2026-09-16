local function BullyAcquirePrepared(quest, me, acquire)
    while not acquire() do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

function BullyRunoffControls(quest, resources, me, control, retainedVictim)
    local hero = resources:NewResource()
    resources:PrepareResource(hero)
    local victim
    local function run()
        if not BullyAcquirePrepared(quest, me, function()
            return resources:TryAcquire(hero, quest:GetHero(), 4)
        end) then return false end
        victim = resources:NewResource()
        resources:PrepareResource(victim)
        if not BullyAcquirePrepared(quest, me, function()
            return resources:TryAcquireThing(victim, retainedVictim, 4)
        end) then return false end
        return BullyRunoffMovie(quest, resources, hero, victim, control, retainedVictim)
    end
    local completed = run()
    if victim ~= nil then resources:ReleaseResource(victim) end
    resources:ReleaseResource(hero)
    if completed then
        quest:SetStateBool("BullyRanOff", true)
        require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
        quest:RemoveThing(me, false, true)
    end
end

function BullyReturnHomePhase(quest, me, resources, control)
    local home = me:GetHomePos()
    while true do
        local actor = resources:NewThingFromResource(control)
        local outside = resources:ThingIsDistanceFromPositionOver(actor, home, 2.0)
        resources:DestroyThing(actor)
        if not outside then return true end
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
        resources:MoveToPosition(control, home, 0.0, 0, false, true)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
    end
end

function BullyControlledHealthAboveThreshold(resources, control)
    local actor = resources:NewThingFromResource(control)
    local health = resources:ThingHealth(actor)
    local positive = health > 0.0
    resources:DestroyThing(actor)
    return positive
end

function BullyMainDialogue(quest, resources, control, state)
    local function speak(key)
        if not BullyControlledHealthAboveThreshold(resources, control) then return true end
        resources:Speak(control, quest:GetHero(), key, 0, false, true, false)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        return not quest:IsActiveThreadTerminating()
    end
    if not state:GetStateBool("DoneIntro") then
        if quest:IsActiveThreadTerminating() then return false end
        if not speak("TEXT_QST_048_BULLY_GET_LOST") then return false end
        state:SetStateBool("DoneIntro", true)
        return true
    end
    if quest:IsActiveThreadTerminating() then return false end
    if quest:GetStateBool("HeroAttackedVictim") then
        if quest:IsActiveThreadTerminating() then return false end
        if not state:GetStateBool("SaidPieceAboutAttackingVictim") then
            if quest:IsActiveThreadTerminating() then return false end
            if not speak("TEXT_QST_048_BULLY_IN_COMMON") then return false end
            state:SetStateBool("SaidPieceAboutAttackingVictim", true)
            return true
        end
        if quest:IsActiveThreadTerminating() then return false end
        local fewHits = state:GetStateInt("HitsTaken") < 3
        if quest:IsActiveThreadTerminating() then return false end
        return speak(fewHits and "TEXT_QST_048_BULLY_NASTY_STREAK" or "TEXT_QST_048_BULLY_DONT_HIT_ME")
    end
    if quest:IsActiveThreadTerminating() then return false end
    return speak("TEXT_QST_048_BULLY_BADGERING")
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


function BullyProximity(quest, resources, me, victim, control, state)
    resources:FaceRetainedThing(me, victim, true)
    if quest:GetTimer(quest:GetStateInt("TalkIntermittentTimer")) ~= 0 then return true end
    if state:GetStateBool("SpokenOnFirstProximity") then
        local roll = quest:RetailRandModulo(resources:ReadBullyRandomModulus())
        if roll ~= 0 then return true end
    end
    local range = resources:ReadBullyProximityRange()
    local hero = quest:GetHero()
    if not resources:IsDistanceBetweenThingsUnder(me, hero, range) then return true end
    if state:GetStateInt("HitsTaken") ~= 0 then return true end
    if quest:IsActiveThreadTerminating() then return false end
    state:SetStateBool("SpokenOnFirstProximity", true)
    quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 3)
    quest:SetStateBool("VictimShake", true)
    local conversation = resources:NewConversation(me, false, false)
    resources:AddConversationPerson(conversation, victim)
    resources:AddConversationLine(conversation, "TEXT_QST_048_BULLY_BADGERING", me, victim, false)
    local nextLine = wrapSignedInt32(state:GetStateInt("IntimidateSpeechLoop") + 10)
    state:SetStateInt("IntimidateSpeechLoop", nextLine)
    if nextLine > 40 then
        if quest:IsActiveThreadTerminating() then
            return false
        end
        state:SetStateInt("IntimidateSpeechLoop", 10)
    end
    local even = quest:RetailRandModulo(2) == 0
    if quest:IsActiveThreadTerminating() then
        return false
    end
    resources:PlayAnimationWithNativeArgument5(control,
        even and "ST_OPINION_DISAPPROVAL_SHAKE_FIST" or "ST_OPINION_DISAPPROVAL_POINT_AT",
        false, false, false, true, false, false)
    return true
end

function BullyHitConversation(resources, me, retainedVictim)
    local conversation = resources:NewConversation(me, false, false)
    resources:AddConversationPerson(conversation, retainedVictim)
    resources:AddConversationLine(conversation, "TEXT_QST_048_BULLY_SCRMSG_GET_OFF", me, retainedVictim, false)
    resources:AddConversationLine(conversation, "TEXT_QST_048_VICTIM_REVENGE", me, retainedVictim, false)
end

function BullySetHeroAlliance(quest, resources, me)
    local hero = quest:GetHero()
    resources:SetThingAsAlly(me, hero)
    hero = quest:GetHero()
    resources:SetThingAsAlly(hero, me)
end

function BullyRunoffMovie(quest, resources, hero, victim, bully, retainedVictim)
    local actors = resources:NewActorMap()
    resources:SetActor(actors, "HERO", hero)
    resources:SetActor(actors, "BRAT", victim)
    resources:SetActor(actors, "BULLY", bully)
    local inputs = resources:NewStringMap()
    local attackedVictim = quest:GetStateBool("HeroAttackedVictim")
    if quest:IsActiveThreadTerminating() then
        resources:DestroyStringMap(inputs)
        resources:DestroyActorMap(actors)
        return false
    end
    resources:SetString(inputs, "$BRATLINE", attackedVictim
        and "TEXT_QST_048_VICTIM_THANKS_AFTER_HIT" or "TEXT_QST_048_VICTIM_THANKS")
    local movie = resources:StartMovie("")
    resources:Pause(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings("CS_OAKVALEINTRO_BULLYRUN1", actors, inputs, false, true)
    local givenTeddy = quest:GetStateBool("GivenHeroTeddy")
    if quest:IsActiveThreadTerminating() then
        -- Native cancellation does not issue FixMovieSequenceCamera(false).
        resources:Pause(false)
        resources:DestroyMovie(movie)
        resources:DestroyStringMap(inputs)
        resources:DestroyActorMap(actors)
        return false
    end
    if not givenTeddy then
        resources:RunMacro("CS_OAKVALEINTRO_BULLYRUN2", actors, false, true)
        resources:ClearThingHasInformation(retainedVictim)
        quest:SetStateBool("GivenHeroTeddy", true)
    else
        resources:RunMacro("CS_OAKVALEINTRO_BULLYRUNDUMMY", actors, false, true)
    end
    quest:FixMovieSequenceCamera(false)
    resources:Pause(false)
    resources:DestroyMovie(movie)
    resources:DestroyStringMap(inputs)
    resources:DestroyActorMap(actors)
    return true
end

-- Generated native draft: NOVI_Bully. Review coverage report before use.
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
    __native_entity_state:SetStateBool("DoneIntro", false)
    __native_entity_state:SetStateInt("HitsTaken", 0)
    __native_entity_state:SetStateInt("InitialHealth", 4)
    __native_entity_state:SetStateBool("SpokenOnFirstProximity", false)
    quest:SetStateBool("SpokeAboutFindingTeddy", false)
    __native_entity_state:SetStateBool("SaidPieceAboutAttackingVictim", false)
    __native_entity_state:SetStateInt("IntimidateSpeechLoop", 10)
    quest:WithRetailResources(function(resources)
        resources:InitializeBullyActor(me)
    end)
end

local function resourceBody(quest, me, resources)
    local bully_control, bully_hero_control, bully_victim_control, bully_movie, bully_presented
    local function finish_bully_movie()
        assert(bully_movie ~= nil, "Native Bully movie must be live at cleanup")
        resources:Pause(false)
        resources:DestroyMovie(bully_movie)
        bully_movie = nil
    end
    local function release_presented()
        if bully_presented ~= nil then
            resources:DestroyPresentedItemOutput(bully_presented)
            bully_presented = nil
        end
    end
    local function handle_bully_item()
        if quest:IsActiveThreadTerminating() then return false end
        bully_presented = resources:NewPresentedItemOutput(me)
        local function speak_if_healthy(key)
            if BullyControlledHealthAboveThreshold(resources, bully_control) then
                resources:Speak(bully_control, quest:GetHero(), key, 0, false, true, false)
                while resources:IsPerformingScriptTask(bully_control) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then return false end
                end
                if quest:IsActiveThreadTerminating() then return false end
            end
            return true
        end
        local function wait_for_complaint()
            quest:SetStateBool("VictimComplainsAboutLosingTeddy", true)
            while quest:GetStateBool("VictimComplainsAboutLosingTeddy") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return false end
            end
            return not quest:IsActiveThreadTerminating()
        end
        local function start_movie()
            bully_movie = resources:StartMovie("")
            resources:Pause(true)
        end
        local function accept_teddy()
            if not wait_for_complaint() then return false end
            if not speak_if_healthy("TEXT_QST_048_BULLY_FOUND_TEDDY_TWO") then return false end
            GivenTeddy(quest, me)
            quest:ClearThingHasInformation(me)
            return true
        end
        if resources:BullyTalkedWithTeddy(me) then
            if quest:IsActiveThreadTerminating() then return false end
            start_movie()
            if not speak_if_healthy("TEXT_QST_048_BULLY_FOUND_TEDDY_ONE") then
                finish_bully_movie()
                return false
            end
            resources:GiveBullyTeddyQuestion()
            local answer = quest:MsgIsQuestionAnsweredYesOrNo()
            while answer < 0 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    finish_bully_movie()
                    return false
                end
                answer = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then
                finish_bully_movie()
                return false
            end
            local completed = true
            if answer == 1 then
                completed = not quest:IsActiveThreadTerminating()
                if completed then completed = accept_teddy() end
            end
            finish_bully_movie()
            return completed
        end
        if resources:PollPresentedItem(bully_presented) and
                resources:PresentedItemMatches(bully_presented, "OBJECT_TEDDY_BEAR_UNGIVEABLE") then
            if quest:IsActiveThreadTerminating() then return false end
            start_movie()
            local completed = accept_teddy()
            finish_bully_movie()
            return completed
        end
        if not resources:PollPresentedItem(bully_presented) or
                resources:PresentedItemMatches(bully_presented, "OBJECT_TEDDY_BEAR_UNGIVEABLE") then
            return true
        end
        if quest:IsActiveThreadTerminating() then return false end
        resources:PrepareResource(bully_control)
        while not resources:TryAcquire(bully_control, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        start_movie()
        local completed = speak_if_healthy("TEXT_QST_048_BULLY_DONT_WANT")
        finish_bully_movie()
        return completed
    end
    quest:RegisterBoundConsciousCondition()
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then return end
    bully_control = resources:NewResource()
    resources:PrepareResource(bully_control)
    local victim
    local function acquire_self()
        return BullyAcquirePrepared(quest, me, function()
            return resources:TryAcquire(bully_control, me, 4)
        end)
    end
    local function run()
        if not acquire_self() then return end
        if not BullyReturnHomePhase(quest, me, resources, bully_control) then return end
        if quest:IsActiveThreadTerminating() then return end
        victim = resources:NewThingFromScriptName("NOVI_Victim")
        while not quest:IsActiveThreadTerminating() do
            resources:PrepareResource(bully_control)
            if not acquire_self() then return end
            if __native_entity_state:GetStateBool("DoneIntro") then
                if not handle_bully_item() then return end
            end
            release_presented()
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then return end
                resources:PrepareResource(bully_control)
                if not acquire_self() then return end
                bully_movie = resources:StartMovie("")
                resources:Pause(true)
                local completed = BullyMainDialogue(quest, resources, bully_control, __native_entity_state)
                finish_bully_movie()
                if not completed then return end
            end
            if resources:IsHitByHeroExceptAbility(me, 14) then
                if quest:IsActiveThreadTerminating() then return end
                if quest:GetStateInt("GUIBullyHealthCounter") == -999 then
                    if quest:IsActiveThreadTerminating() then return end
                    local counter = resources:AddBullyHealthBar(__native_entity_state:GetStateInt("InitialHealth"))
                    quest:SetStateInt("GUIBullyHealthCounter", counter)
                end
                BullySetHeroAlliance(quest, resources, me)
                local hits = __native_entity_state:GetStateInt("HitsTaken")
                __native_entity_state:SetStateInt("HitsTaken", hits + 1)
                if __native_entity_state:GetStateInt("InitialHealth") <= hits + 1 then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:SetStateBool("BullySubdued", true)
                    quest:RemoveQuestInfoElement(quest:GetStateInt("GUIBullyHealthCounter"))
                    resources:PrepareResource(bully_control)
                    if acquire_self() then
                        BullyRunoffControls(quest, resources, me, bully_control, victim)
                    end
                    return
                end
                if quest:IsActiveThreadTerminating() then return end
                BullyHitConversation(resources, me, victim)
                quest:UpdateQuestInfoBar(quest:GetStateInt("GUIBullyHealthCounter"),
                    __native_entity_state:GetStateInt("InitialHealth") - __native_entity_state:GetStateInt("HitsTaken"), -1.0, -1.0)
            end
            if not BullyProximity(quest, resources, me, victim, bully_control, __native_entity_state) then return end
            quest:NewScriptFrame(me)
        end
    end
    run()
    release_presented()
    if victim ~= nil then resources:DestroyThing(victim) end
    resources:ReleaseResource(bully_control)
end

function GivenTeddy(quest, me)
    quest:GiveHeroGold(1)
    quest:TakeObjectFromHero("OBJECT_TEDDY_BEAR_UNGIVEABLE")
    quest:SetStateBool("SpokeAboutFindingTeddy", true)
    quest:SetStateBool("TeddyRuined", true)
    require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 3)
end


function Main(quest, me)
    quest:WithRetailResources(function(resources)
        resourceBody(quest, me, resources)
    end)
end
