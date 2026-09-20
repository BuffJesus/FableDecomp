-- Oakvale Reborn: the rewritten childhood intro, run in place of the native
-- Q_NewOakValeIntro (retail_override.lua). Seeded from the v4-proven readable
-- stage; the story changes live in stranger.lua / scenes.lua and the OVR_*
-- entities. Invariants for the Guild handoff: the quest name, the card, the
-- PreAttack/PostAttack sections, AttackOver persisted, SetQuestAsCompleted.
local Stranger = require("OakvaleReborn.stranger")

local villagerSpeechInitializers = {
    { category = "bad", male = false, keys = {
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_10",
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_20",
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_30",
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_40",
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_50",
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_60",
    } },
    { category = "bad", male = true, keys = {
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_10",
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_20",
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_30",
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_40",
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_50",
        "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_60",
    } },
    { category = "both", male = false, keys = {
        "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_10",
        "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_20",
        "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_30",
        "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_40",
    } },
    { category = "both", male = true, keys = {
        "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_10",
        "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_20",
        "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_30",
        "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_40",
    } },
    { category = "good", male = false, keys = {
        "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_10",
        "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_20",
        "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_30",
        "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_40",
        "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_50",
        "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_60",
    } },
    { category = "good", male = true, keys = {
        "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_10",
        "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_20",
        "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_30",
        "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_40",
        "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_50",
        "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_60",
    } },
    { category = "none", male = false, keys = {
        "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_10",
        "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_20",
        "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_30",
        "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_40",
        "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_50",
    } },
    { category = "none", male = true, keys = {
        "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_10",
        "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_20",
        "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_30",
        "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_40",
        "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_50",
    } },
}

-- LuaQuestHost::RegisterMain owns native Main registration (empty section).
-- Main is invoked through the native virtual thunk; no second Lua thread is created.

function Main(quest)
    quest:AddEntityBinding("NOVI_LiveFather", "OakvaleReborn/Entities/OVR_LiveFather")
    quest:AddEntityBinding("NOVI_Theresa", "OakvaleReborn/Entities/OVR_Theresa")
    quest:AddEntityBinding("NOVI_Guard", "OakvaleReborn/Entities/OVR_Guard")
    quest:AddEntityBinding("NOVI_Villager", "OakvaleReborn/Entities/OVR_Villager")
    quest:AddEntityBinding("NOVI_Bully", "OakvaleReborn/Entities/OVR_Bully")
    quest:AddEntityBinding("NOVI_Victim", "OakvaleReborn/Entities/OVR_Victim")
    quest:AddEntityBinding("NOVI_TeddyGirl", "OakvaleReborn/Entities/OVR_TeddyGirl")
    quest:AddEntityBinding("NOVI_AffairMan", "OakvaleReborn/Entities/OVR_AffairMan")
    quest:AddEntityBinding("NOVI_AffairWoman", "OakvaleReborn/Entities/OVR_AffairWoman")
    quest:AddEntityBinding("NOVI_AffairWife", "OakvaleReborn/Entities/OVR_AffairWife")
    quest:AddEntityBinding("NOVI_BookTrader", "OakvaleReborn/Entities/OVR_BookTrader")
    quest:AddEntityBinding("NOVI_BarrelMan", "OakvaleReborn/Entities/OVR_BarrelMan")
    quest:AddEntityBinding("NOVI_BarrelThug", "OakvaleReborn/Entities/OVR_BarrelThug")
    quest:AddEntityBinding("NOVI_Barrel", "OakvaleReborn/Entities/OVR_Barrel")
    quest:AddEntityBinding("NOVI_CreatedBeetle", "OakvaleReborn/Entities/OVR_CreatedBeetle")
    quest:AddEntityBinding("OVI_DeadFather", "OakvaleReborn/Entities/OVR_DeadFather")
    quest:FinalizeEntityBindings()
    quest:CreateThread("StrangerRoutine")
    if quest:GetStateBool("AttackOver") then
        if quest:IsActiveThreadTerminating() then return end
        quest:DeactivateQuest("Q__OakValeIntro_PostAttack", 0)
    end
    quest:WithRetailResources(function(resources)
        resources:SetInitialOakvaleObjective()
    end)
    quest:CreateThread("StartBarrelTimer")
    DoMission(quest)
end

function Init(quest)
    quest:SetStateInt("TalkIntermittentTimer", quest:RegisterTimer())
    quest:SetStateInt("WatchTimer", quest:RegisterTimer())
    quest:SetTimer(quest:GetStateInt("TalkIntermittentTimer"), 0)
    quest:SetStateBool("VictimShake", false)
    quest:SetStateBool("DadFound", false)
    quest:SetStateBool("AttackOver", false)
    quest:SetStateBool("StrangerOfferMade", false)
    quest:SetStateBool("StrangerAccepted", false)
    quest:SetStateBool("StrangerKilled", false)
    quest:SetStateBool("BullyRushed", false)
    quest:SetStateBool("StrangerPresent", false)
    quest:SetStateBool("ColdOpenDone", false)
    quest:SetStateInt("MassacreKills", 0)
    quest:SetStateBool("DadFinishedIntro", false)
    quest:SetStateBool("BullySubdued", false)
    quest:SetStateBool("BullyRanOff", false)
    quest:SetStateBool("GivenHeroTeddy", false)
    quest:SetStateBool("HeroAttackedVictim", false)
    quest:SetStateInt("GoodDeedsPerformed", 0)
    quest:SetStateInt("BadDeedsPerformed", 0)
    quest:SetStateInt("GuardsDealtWithBadDeeds", 0)
    quest:SetStateBool("HeroDiscoveredInfidelity", false)
    quest:SetStateBool("BarrelManLeftHeroInCharge", false)
    quest:SetStateBool("BarrelManSpokenToHeroOnReturn", false)
    quest:SetStateBool("InstructionGiven_Barrels", false)
    quest:SetStateBool("ReceiveKiss", false)
    quest:SetStateBool("ReceiveHug", false)
    quest:SetStateBool("BarrelBrokenPersistent", false)
    quest:WithRetailResources(function(resources)
        resources:SetVillagerAmbientTimer(quest:GetStateInt("TalkIntermittentTimer"), 0)
    end)
    quest:SetStateInt("GUIBullyHealthCounter", -999)
    quest:SetStateBool("TeddyRuined", false)
    quest:SetStateBool("GuardsSpokenOnce", false)
    quest:SetStateBool("TalkingToWoman", false)
    quest:SetStateBool("GivenSweets", false)
    quest:SetStateBool("GivenTheresaChocs", false)
    quest:SetStateBool("VictimComplainsAboutLosingTeddy", false)
    quest:SetStateBool("WhichBadDeedsPerformed_" .. (0), false)
    quest:SetStateBool("WhichBadDeedsPerformed_" .. (1), false)
    quest:SetStateBool("WhichBadDeedsPerformed_" .. (2), false)
    quest:SetStateBool("WhichBadDeedsPerformed_" .. (3), false)
    quest:SetStateBool("WhichBadDeedsPerformed_" .. (4), false)
    local speechLists = quest:GetVillagerSpeechLists()
    for _, group in ipairs(villagerSpeechInitializers) do
        for _, key in ipairs(group.keys) do
            speechLists:Append(group.category, group.male, key)
        end
    end
end

function OnPersist(quest, context)
    quest:SetStateInt("MassacreKills", quest:PersistTransferInt(context, "MassacreKills", quest:GetStateInt("MassacreKills") or 0))
    for _, key in ipairs({ "AttackOver", "StrangerOfferMade", "StrangerAccepted", "StrangerKilled", "ColdOpenDone" }) do
        local value = quest:GetStateBool(key) or false
        value = quest:PersistTransferBool(context, key, value)
        quest:SetStateBool(key, value)
    end
end

function StrangerRoutine(quest)
    Stranger.Routine(quest)
end

function DoMission(quest)
    while not quest:IsRegionLoaded("StartOakVale") do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
    if quest:IsActiveThreadTerminating() then return end

    if not quest:GetStateBool("AttackOver") then
        if quest:IsActiveThreadTerminating() then return end
        quest:FadeScreenOutUntilNextCallToFadeScreenIn(2.0, 0.0)
        quest:WithRetailResources(function(resources)
            resources:TurnOakvaleHeroIntoChild()
        end)
        quest:CreateThread("WatchBarrels")
        quest:CreateThread("WatchForGotGold")
        quest:CreateThread("ManageQuestCoreMarkers")
        quest:CacheMusicSet(46)
        quest:ActivateQuest("Q_NewOakValeIntro_PreAttack")
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end

        quest:WithRetailResources(function(resources)
            resources:SetOakvaleHeroKillable(false)
        end)
        quest:SetTimeAsStopped(true)
        quest:SetTimeOfDay(12.0)
        quest:SetHeroSleepingAsEnabled(false)
        quest:DisplayMoneyBag(true)
        quest:WithRetailResources(function(resources)
            resources:PrepareOakvaleHouseAndStartScreen()
            -- beat 0: the Stranger on the hill, before the Father wakes the hero
            if not quest:GetStateBool("ColdOpenDone") then Stranger.ColdOpen(quest, resources) end
        end)
        while not quest:GetStateBool("AttackOver") do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
    end

    if quest:IsActiveThreadTerminating() then return end
    AttackStuff(quest)
    PostAttackStuff(quest)
    -- Retail continues this tail when PostAttackStuff returns after cancellation.
    quest:FadeScreenOutUntilNextCallToFadeScreenIn(0.5, 0.0)
    quest:WithRetailResources(function(resources)
        resources:SetOakvaleHeroKillable(true)
    end)
    quest:SetHeroSleepingAsEnabled(true)
    quest:WithRetailResources(function(resources)
        resources:FinishOakvaleActiveQuest(false)
        resources:FinishOakvaleActiveQuest(true)
    end)
end

function AttackStuff(quest)
    quest:ActivateQuest("Q__OakValeIntro_PostAttack")
    quest:DeactivateQuest("Q_NewOakValeIntro_PreAttack", 0)
    quest:SetTimeOfDay(23.0)
    quest:TransitionToTheme("ENVIRONMENT_OV_POSTATTACK", 0.0)
    quest:WithRetailResources(function(resources)
        resources:SetOakvaleProgressObjective(false)
    end)
end

-- DBEDA5..DBEEBC: one Hero acquisition attempt, even when it fails.
local function playPostAttackDadCutscene(quest, resources)
    quest:SetStateBool("DadFound", true)
    local heroControl, actors, movie
    local paused, fixedCamera = false, false
    local ok, failure = pcall(function()
        heroControl = resources:NewResource()
        resources:TryAcquirePostAttackHero(heroControl, 4)
        actors = resources:NewActorMap()
        resources:SetActor(actors, "HERO", heroControl)
        movie = resources:StartMovie("")
        resources:Pause(true)
        paused = true
        quest:FixMovieSequenceCamera(true)
        fixedCamera = true
        -- The aftermath: retail dead-father scene until the branch macros are written.
        local aftermath = "CS_OVR_AFTERMATH_GOOD"
        if quest:GetStateBool("StrangerKilled") then aftermath = "CS_OVR_AFTERMATH_KILLED"
        elseif quest:GetStateBool("StrangerAccepted") then aftermath = "CS_OVR_AFTERMATH_EVIL" end
        resources:RunMacro(aftermath, actors, false, true)
    end)
    local cleanupError
    local function cleanup(method, receiver, ...)
        local closed, err = pcall(method, receiver, ...)
        if not closed and cleanupError == nil then cleanupError = err end
    end
    if fixedCamera then cleanup(quest.FixMovieSequenceCamera, quest, false) end
    if paused then cleanup(resources.Pause, resources, false) end
    if movie then cleanup(resources.DestroyMovie, resources, movie) end
    if actors then cleanup(resources.DestroyActorMap, resources, actors) end
    if heroControl then cleanup(resources.ReleaseResource, resources, heroControl) end
    if not ok then error(failure, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
end


-- Structured PostAttackStuff; atomic lookup adapters retain native temporary scopes.
local function runPostAttack(quest, resources)
    while not resources:PostAttackStartIsAlive() do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:CacheMusicSet(45)
    resources:TeleportToPostAttackStart()
    resources:SetPostAttackVillageLimbo(true)
    quest:DisplayMoneyBag(false)
    quest:TakeObjectFromHero("OBJECT_TEDDY_BEAR_UNGIVEABLE")
    quest:AddLogbookStoryEntry(20)
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    quest:CameraResetToViewBehindHero(0.0)
    quest:CameraDefault()
    quest:CacheMusicSet(57)
    quest:FadeScreenIn()
    local trigger = resources:NewThingFromScriptName("MK_OVI_DADTRIGGER")
    local function finishPostAttack()
        while not resources:PostAttackHeroNearTrigger(trigger) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        playPostAttackDadCutscene(quest, resources)
        resources:SetPostAttackVillageLimbo(false)
        quest:SetTimeAsStopped(false)
        quest:DeactivateQuest("Q__OakValeIntro_PostAttack", 0)
        quest:ResetToDefaultTheme(0)
        quest:StopOverrideMusic(false)
    end
    finishPostAttack()
    resources:DestroyThing(trigger)
end

function PostAttackStuff(quest)
    quest:WithRetailResources(function(resources) runPostAttack(quest, resources) end)
end


function ManageQuestCoreMarkers(quest)
    quest:WithRetailResources(function(resources)
        local father = resources:NewThingFromScriptName("NOVI_LiveFather")
        local trader = resources:NewThingFromScriptName("NOVI_BookTrader")
        local theresa = resources:NewThingFromScriptName("NOVI_Theresa")
        local function wait_until(predicate)
            while not predicate() do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return false end
            end
            return not quest:IsActiveThreadTerminating()
        end
        local function run()
            resources:AddCoreQuestMarker(father)
            if not wait_until(function() return quest:GetHeroGold() >= 3 end) then return end
            if not wait_until(function() return quest:IsHeroControlledByPlayer() end) then return end
            resources:RemoveCoreQuestMarker(father)
            resources:AddCoreQuestMarker(trader)
            if quest:DisplayTutorial(19) then
                if quest:IsActiveThreadTerminating() then return end
                if not wait_until(function() return quest:MsgIsTutorialClickedPast() end) then return end
            end
            if not wait_until(function() return quest:GetStateBool("GivenSweets") end) then return end
            resources:RemoveCoreQuestMarker(trader)
            resources:AddCoreQuestMarker(theresa)
            if not wait_until(function() return quest:GetStateBool("GivenTheresaChocs") end) then return end
            resources:RemoveCoreQuestMarker(theresa)
            resources:AddCoreQuestMarker(father)
        end
        run()
        resources:DestroyThing(theresa)
        resources:DestroyThing(trader)
        resources:DestroyThing(father)
    end)
end

function StartBarrelTimer(quest)
    quest:WithRetailResources(function(resources)
        while resources:ReadBarrelWatchTimer(quest:GetStateInt("WatchTimer")) <= 0 do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        resources:AddBarrelTimerBar(function(id) quest:SetStateInt("GUIBarrelCounter", id) end)
        local guard = resources:NewThingFromScriptName("M_WHouse_GuardPoint")
        local function run()
            while not quest:GetStateBool("BarrelManSpokenToHeroOnReturn") do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return end
                local near = resources:IsHeroNearBarrelGuard(guard)
                resources:ColourBarrelTimer(quest:GetStateInt("GUIBarrelCounter"), near)
                resources:UpdateBarrelTimer(function() return quest:GetStateInt("WatchTimer") end,
                    function() return quest:GetStateInt("GUIBarrelCounter") end)
            end
            if not quest:IsActiveThreadTerminating() then
                resources:RemoveBarrelTimer(quest:GetStateInt("GUIBarrelCounter"))
            end
        end
        run()
        resources:DestroyThing(guard)
    end)
end

-- Native DBE960..DBEAD4 after snapshot count and the preceding termination query.
-- Caller owns the retained barrel snapshot. Reward adapters own temporary Things.
local function processBarrelBreaks(quest, resources, total, addBadDeed)
    quest:SetStateBool("BarrelBrokenInstantaneous", false)
    local broken = 0
    if quest:IsActiveThreadTerminating() then return end
    while not quest:GetStateBool("AttackOver") do
        if quest:GetStateBool("BarrelBrokenInstantaneous") then
            broken = (broken + 1) % 4294967296
            if broken >= 2147483648 then broken = broken - 4294967296 end
            quest:SetStateBool("BarrelBrokenInstantaneous", false)
            if broken == 1 then
                addBadDeed(0)
            elseif broken == total - 1 then
                -- Retail puts the gold pickup into the final surviving barrel.
                local last_barrel = nil
                if quest.GetThingWithScriptName then
                    last_barrel = quest:GetThingWithScriptName("NOVI_Barrel")
                end
                if last_barrel ~= nil then
                    quest:AddItemToContainer(last_barrel, "OBJECT_GOLD_1")
                end
            elseif broken > total - 4 then
                resources:SpawnBarrelBeetle({
                    x = quest:GetStateFloat("BarrelBrokenPos_x"),
                    y = quest:GetStateFloat("BarrelBrokenPos_y"),
                    z = quest:GetStateFloat("BarrelBrokenPos_z"),
                })
            end
        end
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
end

-- Native DBE890..DBEB17. One retained vector spans every refresh and the loop.
local function watchBarrelsWithSnapshot(quest, resources, addBadDeed)
    local barrels = resources:NewBarrelWatchSnapshot()
    local ok, failure = pcall(function()
        while barrels:Refresh() <= 0 do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        processBarrelBreaks(quest, resources, barrels:Count(), addBadDeed)
    end)
    local closed, closeError = pcall(barrels.Close, barrels)
    if not ok then error(failure, 0) end
    if not closed then error(closeError, 0) end
end

function WatchBarrels(quest)
    quest:WithRetailResources(function(resources)
        watchBarrelsWithSnapshot(quest, resources, function(deed) AddBadDeed(quest, deed) end)
    end)
end

function WatchForGotGold(quest)
    while quest:GetHeroGold() <= 2 do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        resources:SetOakvaleProgressObjective(true)
    end)
end

-- Native deed helpers share control flow between quest and entity callers.
local function deedFrame(quest, me)
    if me ~= nil then quest:NewScriptFrame(me) else quest:NewScriptFrame() end
end

local function incrementDeed(quest, key)
    local MAX_SIGNED_INT32 = 2147483647
    local MIN_SIGNED_INT32 = -2147483648
    local count = quest:GetStateInt(key)
    -- Match the native counter's overflow without obscuring the increment.
    if count == MAX_SIGNED_INT32 then
        count = MIN_SIGNED_INT32
    else
        count = count + 1
    end
    quest:SetStateInt(key, count)
end

local function waitForDeedMessage(quest, me)
    while not quest:MsgIsGameInfoClickedPast() do
        deedFrame(quest, me)
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

local function addGoodDeed(quest, me)
    quest:WithRetailResources(function(resources)
        incrementDeed(quest, "GoodDeedsPerformed")
        resources:ApplyOakvaleDeedMorality(true)
        local first = quest:GetStateInt("GoodDeedsPerformed") == 1 and quest:GetStateInt("BadDeedsPerformed") == 0
        if quest:IsActiveThreadTerminating() then return end
        resources:DisplayRawGameInfo(first and "TEXT_QST_048_SCRMSG_DID_FIRST_GOOD_DEED" or "TEXT_QST_048_SCRMSG_DID_GOOD_DEED")
        if not waitForDeedMessage(quest, me) then return end
        if first then quest:AddLogbookTutorialEntry("TEXT_QST_LOG_BASICS_MAP") end
        if quest:GetHeroGold() < 3 and not quest:GetStateBool("GivenSweets") then
            if quest:IsActiveThreadTerminating() then return end
            resources:SetOakvaleDeedObjective()
        end
        quest:UpdateQuestInfoCounter(quest:GetStateInt("GUIGoodDeedCounter"), quest:GetStateInt("GoodDeedsPerformed"), -1)
    end)
end

local function addBadDeed(quest, me, deed)
    quest:WithRetailResources(function(resources)
        incrementDeed(quest, "BadDeedsPerformed")
        resources:ApplyOakvaleDeedMorality(false)
        local first = quest:GetStateInt("BadDeedsPerformed") == 1 and quest:GetStateInt("GoodDeedsPerformed") == 0
        if quest:IsActiveThreadTerminating() then return end
        if first then
            resources:DisplayRawGameInfo("TEXT_QST_048_SCRMSG_DID_FIRST_BAD_DEED")
            if not waitForDeedMessage(quest, me) then return end
            quest:AddLogbookTutorialEntry("TEXT_QST_LOG_BASICS_MAP")
        elseif not quest:GetStateBool("WhichBadDeedsPerformed_" .. deed) then
            if quest:IsActiveThreadTerminating() then return end
            resources:DisplayRawGameInfo("TEXT_QST_048_SCRMSG_DID_BAD_DEED")
            if not waitForDeedMessage(quest, me) then return end
        end
        quest:SetStateBool("WhichBadDeedsPerformed_" .. deed, true)
    end)
end

function AddGoodDeed(quest)
    addGoodDeed(quest)
end

function AddBadDeed(quest, deed)
    addBadDeed(quest, nil, deed)
end

-- Destruction belongs to LuaQuestHost under nativeLifetime="NewOakValeIntro".
-- The host closes Lua, owned timers and speech vectors, then the native script
-- base. Scalar flags control allocation release. See the C++ lifecycle ledger.
