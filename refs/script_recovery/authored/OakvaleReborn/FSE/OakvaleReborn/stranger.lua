-- Oakvale Reborn: the Stranger and the two roads out of Oakvale ("The Stranger's Gift").
--
-- STORY.md beats, locked 2026-09-19:
--   0  ColdOpen   CS_OVR_COLDOPEN before the Father wakes the hero (OVR_LiveFather waits for it)
--   2  watch      after the first deed he stands in the square and comments as the hero passes
--   3  offer      CS_OVR_OFFER, then the yes/no, then his answer line
--   4a Massacre   accept: the child fights; Father and Theresa are protected; six dead -> night
--   4c Hunted     accept, then strike the giver in the window: he goes down laughing, the gift
--                 dies with him, the guards saw -> hunted until night
--   4b (refuse)   OVR_Theresa runs CS_OVR_REFUSE at the departure trigger instead of the raid + FMV
-- Both roads raise AttackOver; the retail flow (section swap, aftermath macro chosen per branch,
-- quest completion, the Guild handoff) runs from there.
local Scene = require("OakvaleReborn.scenes")

local Stranger = {}

-- Casting and staging: retail data only (names.bin / the StartOakValeWest TNG).
Stranger.CREATURE = "CREATURE_PROPHET_01"           -- the Snowspire prophets' hooded robe (alt: CREATURE_ASSASSIN)
Stranger.SCRIPT_NAME = "OVR_Stranger"
Stranger.SPAWN_ANCHOR = "NOVI_BookTrader"           -- the trader stands in the square; he appears beside him
Stranger.SPAWN_OFFSET = { x = 2.5, y = 0.0, z = 0.0 }
Stranger.SWORD = "OBJECT_HERO_SWORD_FIRST"
Stranger.KILLS_TO_END = 6
Stranger.GROWN_FOR_THE_NIGHT = false                -- S6 fallback: TurnCreatureInto(CREATURE_HERO)
Stranger.PROTECTED = { NOVI_LiveFather = true, NOVI_Theresa = true }   -- the aftermath needs them
Stranger.WATCH_RADIUS = 5.0                         -- metres; a passing comment
Stranger.WATCH_COOLDOWN = 12.0                      -- seconds between comments
Stranger.OFFER_RADIUS = 4.0                         -- with the chocolates, walking past him is enough
Stranger.KILL_WINDOW = 8.0                          -- seconds after the gift in which he can be struck
Stranger.HUNT_SECONDS = 40.0                        -- hunted by the guards before night falls
Stranger.MASSACRE_MAX_SECONDS = 180.0               -- villagers flee; night comes anyway after this
Stranger.STAND_RADIUS = 2.5                         -- standing this close for STAND_SECONDS opens the offer
Stranger.STAND_SECONDS = 2.0                        --   (the prophet body may not be talkable)
Stranger.SWORD_SURVIVES_NIGHT = false               -- his gift does not outlast his fire (Guild gets a clean hero)
Stranger.BULLY_RUSH_AT = 3                          -- kills before the Bully is the one villager who fights back
Stranger.TITLE_MASSACRE = "OBJECT_HERO_TITLE_BUTCHER_OF_OAKVALE"   -- manifest `titles` (appended to game.bin)
Stranger.TITLE_KILLED = "OBJECT_HERO_TITLE_GIFTBREAKER"

Stranger.TEXT = {
    coldOpen      = "CS_OVR_COLDOPEN",
    offerMacro    = "CS_OVR_OFFER",
    watchGood     = "TEXT_OVR_WATCH_GOOD_010",
    watchBad      = "TEXT_OVR_WATCH_BAD_010",
    watchAgain    = "TEXT_OVR_WATCH_020",
    offerQuestion = "TEXT_OVR_OFFER_QUESTION",
    accepted      = "TEXT_OVR_OFFER_ACCEPT_010",
    refused       = "TEXT_OVR_OFFER_REFUSE_010",
    massacreInfo  = "TEXT_OVR_MASSACRE_INFO",
    killedLine    = "TEXT_OVR_KILLED_010",
    killedInfo    = "TEXT_OVR_KILLED_INFO",
    killCounter   = "TEXT_OVR_HUD_KILLS",
}

local function waitUntil(quest, predicate)
    while not predicate() do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

local function deedsDone(quest)
    return quest:GetStateInt("GoodDeedsPerformed") + quest:GetStateInt("BadDeedsPerformed")
end

local function distance(a, b)
    local dx, dy, dz = a.x - b.x, a.y - b.y, a.z - b.z
    return math.sqrt(dx * dx + dy * dy + dz * dz)
end

local function heroNear(quest, thing, radius)
    return distance(quest:GetHero():GetPos(), thing:GetPos()) <= radius
end

-- Beat 0. Called from DoMission once the house is prepared and the screen is
-- still faded out; the Father's entity holds its intro until ColdOpenDone.
function Stranger.ColdOpen(quest, resources)
    local hero = Scene.AcquireHero(quest, resources, 4)
    if not hero then return false end
    local ok, failure = pcall(function()
        Scene.RunMacro(quest, resources, Stranger.TEXT.coldOpen, { HERO = hero })
    end)
    pcall(resources.ReleaseResource, resources, hero)
    quest:SetTimeOfDay(12.0)               -- the macro's SetTime 6 must not leak into the day
    quest:SetStateBool("ColdOpenDone", true)
    if not ok then error(failure, 0) end
    return true
end

-- The Stranger steps out once the errands have started and waits.
local function summonStranger(quest)
    local existing = quest:GetThingWithScriptName(Stranger.SCRIPT_NAME)
    if existing ~= nil and not existing:IsNull() then
        quest:Log("OVR stranger: already present (reload)")
        return existing
    end
    local anchor = quest:GetThingWithScriptName(Stranger.SPAWN_ANCHOR)
    local base = (anchor ~= nil and not anchor:IsNull()) and anchor:GetPos() or quest:GetHero():GetPos()
    local pos = { x = base.x + Stranger.SPAWN_OFFSET.x, y = base.y + Stranger.SPAWN_OFFSET.y, z = base.z + Stranger.SPAWN_OFFSET.z }
    local thing = quest:CreateCreature(Stranger.CREATURE, pos, Stranger.SCRIPT_NAME)
    quest:NewScriptFrame()                  -- let the script name register before anyone looks it up
    quest:SetStateBool("StrangerPresent", true)
    quest:Log("OVR stranger: created " .. Stranger.CREATURE .. " beside " .. Stranger.SPAWN_ANCHOR)
    return thing
end

-- The hero is free (no cutscene, no conversation, not scripted away).
local function heroFree(quest)
    return quest:IsHeroControlledByPlayer() and not quest:IsInMovieSequence() and not quest:IsConversationActive()
end

-- Beat 2. One comment when the hero passes, chosen by the latest deed; a cooldown
-- so he never nags. Returns true once the offer should open.
local function watch(quest, resources, strangerThing, control)
    local seenGood, seenBad = quest:GetStateInt("GoodDeedsPerformed"), quest:GetStateInt("BadDeedsPerformed")
    local lastKey, nextAllowed = nil, 0.0
    local clock, standing = 0.0, 0.0
    while true do
        local free = heroFree(quest)
        if free and resources:IsTalkedToByHero(strangerThing) then return true end
        if free and quest:GetStateBool("GivenTheresaChocs") then return true end   -- forced: the hill is next
        if free and quest:GetStateBool("GivenSweets") and heroNear(quest, strangerThing, Stranger.OFFER_RADIUS) then return true end
        standing = (free and heroNear(quest, strangerThing, Stranger.STAND_RADIUS)) and (standing + 1.0 / 30.0) or 0.0
        if standing >= Stranger.STAND_SECONDS then return true end
        if free and clock >= nextAllowed and heroNear(quest, strangerThing, Stranger.WATCH_RADIUS) then
            local good, bad = quest:GetStateInt("GoodDeedsPerformed"), quest:GetStateInt("BadDeedsPerformed")
            local key
            if bad > seenBad then key = Stranger.TEXT.watchBad
            elseif good > seenGood then key = Stranger.TEXT.watchGood
            else key = Stranger.TEXT.watchAgain end
            seenGood, seenBad = good, bad
            if key ~= lastKey or key == Stranger.TEXT.watchAgain then
                Scene.Say(quest, resources, control, key)
                lastKey = key
            end
            nextAllowed = clock + Stranger.WATCH_COOLDOWN
        end
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
        clock = clock + 1.0 / 30.0
    end
end

-- Beat 3. The macro carries the speech; the question and his answer are Lua.
-- Returns true/false for the answer, nil when the thread is torn down.
local function offer(quest, resources, strangerThing, control)
    local hero = Scene.AcquireHero(quest, resources, 4)
    if not hero then return nil end
    local accepted
    local ok, failure = pcall(function()
        Scene.RunMacro(quest, resources, Stranger.TEXT.offerMacro, { HERO = hero, Stranger = control })
        local movie = resources:StartMovie("")
        resources:Pause(true)
        local aok, aerr = pcall(function() accepted = Scene.Ask(quest, Stranger.TEXT.offerQuestion) end)
        pcall(resources.Pause, resources, false)
        pcall(resources.DestroyMovie, resources, movie)
        if not aok then error(aerr, 0) end
        if accepted == nil then return end
        Scene.Lua(quest, resources, "offer-answer", function()
            quest:CameraMoveToPosAndLookAtThing(
                { x = strangerThing:GetPos().x + 1.5, y = strangerThing:GetPos().y + 1.5, z = strangerThing:GetPos().z + 1.6 },
                strangerThing, 1.0)
            Scene.Say(quest, resources, control, accepted and Stranger.TEXT.accepted or Stranger.TEXT.refused)
        end)
    end)
    -- the blade leaves his hand either way; on accept GiveHeroWeapon puts it in the hero's
    pcall(quest.GiveThingItemInHand, quest, strangerThing, Stranger.SWORD, false)
    pcall(resources.ReleaseResource, resources, hero)
    if not ok then error(failure, 0) end
    return accepted
end

local function isGuard(thing)
    local def = thing:GetDefName()
    return type(def) == "string" and def:find("GUARD", 1, true) ~= nil
end

local function guards(quest)
    local list = {}
    for _, thing in ipairs(quest:GetAllCreaturesExcludingHero()) do
        if isGuard(thing) then list[#list + 1] = thing end
    end
    return list
end

-- Beat 4c, the window. The hero holds the gift; the giver stands a moment before
-- he goes. One strike with the sword and he goes down laughing.
-- Returns true when he was struck.
local function giftWindow(quest, resources, strangerThing, control)
    quest:GiveHeroWeapon(Stranger.SWORD, true)
    quest:SetHeroWeaponsAsUsable(true)
    local frames = math.floor(Stranger.KILL_WINDOW * 30)
    for _ = 1, frames do
        if strangerThing:MsgIsHitByHeroWithWeapon(Stranger.SWORD) then
            quest:Log("OVR stranger: STRUCK by the gift")
            Scene.Say(quest, resources, control, Stranger.TEXT.killedLine)
            quest:SetThingAsKilled(strangerThing)
            return true
        end
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    return false
end

-- Beat 4c, the hunt. The gift dies with the giver; the guards saw a child kill a
-- man; night comes anyway.
function Stranger.Hunted(quest, resources)
    quest:Log("OVR hunted: begin")
    quest:RemoveAllHeroWeapons()
    quest:SetHeroWeaponsAsUsable(false)
    quest:GiveHeroMorality(-50)
    for _, guard in ipairs(guards(quest)) do quest:SetAttackHeroOnSight(guard, true) end
    resources:DisplayRawGameInfo(Stranger.TEXT.killedInfo)
    if not waitUntil(quest, function() return quest:MsgIsGameInfoClickedPast() end) then return end
    local frames = math.floor(Stranger.HUNT_SECONDS * 30)
    for _ = 1, frames do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
    quest:Log("OVR hunted: night falls")
    quest:FadeScreenOut(0.5, 0.0)
    quest:GiveHeroTitle(Stranger.TITLE_KILLED)
    quest:Log("OVR title: " .. Stranger.TITLE_KILLED)
    quest:OverrideMusic(25, false, false)
    quest:SetStateBool("AttackOver", true)
end

-- Beat 4a. Arm the child, make the village mortal (Father and Theresa excepted),
-- count the dead, then hand the retail flow its AttackOver.
function Stranger.Massacre(quest, resources)
    quest:Log("OVR massacre: begin")
    local hero = quest:GetHero()
    if Stranger.GROWN_FOR_THE_NIGHT then
        hero = quest:TurnCreatureInto(hero, "CREATURE_HERO") or hero
    end
    quest:SetHeroWeaponsAsUsable(true)   -- the sword came with the gift window
    quest:GiveHeroMorality(-200)

    local targets = {}
    for _, thing in ipairs(quest:GetAllCreaturesExcludingHero()) do
        local name = thing:GetName()
        if name ~= Stranger.SCRIPT_NAME and not Stranger.PROTECTED[name] then
            quest:EntitySetAsKillable(thing, true)
            if isGuard(thing) then quest:SetAttackHeroOnSight(thing, true) end
            targets[#targets + 1] = thing
        end
    end
    resources:DisplayRawGameInfo(Stranger.TEXT.massacreInfo)
    if not waitUntil(quest, function() return quest:MsgIsGameInfoClickedPast() end) then return end

    local counter = quest:AddQuestInfoCounter(Stranger.TEXT.killCounter, 0, 1.0)
    quest:SetStateInt("GUIKillCounter", counter)
    quest:DisplayQuestInfo(true)
    local kills = quest:GetStateInt("MassacreKills")
    local elapsed = 0.0
    local finished = waitUntil(quest, function()
        elapsed = elapsed + 1.0 / 30.0
        if elapsed >= Stranger.MASSACRE_MAX_SECONDS then
            quest:Log("OVR massacre: time is up, his fire does the rest")
            return true
        end
        local dead = 0
        for _, thing in ipairs(targets) do
            if thing:IsDead() then dead = dead + 1 end
        end
        if dead ~= kills then
            kills = dead
            quest:SetStateInt("MassacreKills", kills)
            quest:UpdateQuestInfoCounter(counter, kills, Stranger.KILLS_TO_END)
            if kills >= Stranger.BULLY_RUSH_AT and not quest:GetStateBool("BullyRushed") then
                -- the one villager who fights back: the Bully, if he is still around
                quest:SetStateBool("BullyRushed", true)
                local bully = quest:GetThingWithScriptName("NOVI_Bully")
                if bully ~= nil and not bully:IsNull() and not bully:IsDead() then
                    quest:SetAttackHeroOnSight(bully, true)
                    quest:Log("OVR massacre: the Bully rushes the hero")
                end
            end
        end
        return kills >= Stranger.KILLS_TO_END
    end)
    quest:RemoveQuestInfoElement(counter)
    if not finished then return end

    quest:Log("OVR massacre: " .. kills .. " dead, night falls")
    quest:FadeScreenOut(0.5, 0.0)
    quest:GiveHeroTitle(Stranger.TITLE_MASSACRE)
    quest:Log("OVR title: " .. Stranger.TITLE_MASSACRE)
    if not Stranger.SWORD_SURVIVES_NIGHT then quest:RemoveAllHeroWeapons() end
    quest:SetHeroWeaponsAsUsable(false)
    quest:OverrideMusic(25, false, false)
    quest:SetStateBool("AttackOver", true)
end

-- Quest thread: beats 2-4a up to the branch flag.
function Stranger.Routine(quest)
    if quest:GetStateBool("AttackOver") then return end
    if quest:GetStateBool("StrangerOfferMade") then
        -- a save made after the answer: pick the road up where it was
        quest:WithRetailResources(function(resources)
            if quest:GetStateBool("StrangerKilled") then
                quest:Log("OVR stranger: resuming the hunt after a reload")
                Stranger.Hunted(quest, resources)
            elseif quest:GetStateBool("StrangerAccepted") then
                quest:Log("OVR stranger: resuming the massacre after a reload")
                quest:GiveHeroWeapon(Stranger.SWORD, true)
                Stranger.Massacre(quest, resources)
            end
        end)
        return
    end
    if not waitUntil(quest, function()
        return quest:GetStateBool("DadFinishedIntro") and (deedsDone(quest) >= 1 or quest:GetHeroGold() >= 3)
    end) then return end

    quest:WithRetailResources(function(resources)
        local strangerThing = summonStranger(quest)
        local markerOk, marker = pcall(resources.NewThingFromScriptName, resources, Stranger.SCRIPT_NAME)
        if not markerOk then quest:Log("OVR stranger: no quest marker (" .. tostring(marker) .. ")"); marker = nil end
        local control = resources:NewResource()
        local accepted
        local ok, failure = pcall(function()
            if marker then resources:AddCoreQuestMarker(marker) end
            if not Scene.Acquire(quest, resources, control, strangerThing, 3, true) then return end
            local open = watch(quest, resources, strangerThing, control)
            if marker then resources:RemoveCoreQuestMarker(marker) end
            if not open then return end
            accepted = offer(quest, resources, strangerThing, control)
        end)
        pcall(resources.ReleaseResource, resources, control)
        if marker then pcall(resources.DestroyThing, resources, marker) end
        if not ok then error(failure, 0) end
        if accepted == nil then return end

        quest:SetStateBool("StrangerOfferMade", true)
        quest:SetStateBool("StrangerAccepted", accepted)
        quest:Log("OVR stranger: offer " .. (accepted and "ACCEPTED" or "REFUSED"))

        local struck = false
        if accepted then
            local wok, werr = pcall(function()
                if not Scene.Acquire(quest, resources, control, strangerThing, 3, true) then return end
                struck = giftWindow(quest, resources, strangerThing, control)
            end)
            pcall(resources.ReleaseResource, resources, control)
            if not wok then error(werr, 0) end
        end
        quest:SetStateBool("StrangerKilled", struck)
        if not struck then quest:FadeOutAndKillEntity(strangerThing, true, 2.0, false) end
        quest:SetStateBool("StrangerPresent", false)

        if struck then
            Stranger.Hunted(quest, resources)
        elseif accepted then
            Stranger.Massacre(quest, resources)
        end
    end)
end

return Stranger
