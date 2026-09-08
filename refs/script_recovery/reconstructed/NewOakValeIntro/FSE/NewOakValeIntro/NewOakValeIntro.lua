-- NewOakValeIntro/NewOakValeIntro.lua
--
-- ForgeFSE reconstruction of the retail prologue quest Q_NewOakValeIntro
-- (native NScript::CQ_NewOakValeIntroScript, section S_QNOVI, allocator @0x00DBEF70,
--  constructor @0x00DAAC00, RegisterMain @0x00DAACE0, Main @0x00DABAC0, Init @0x00DAADD0,
--  OnPersist @0x00DAADA0, DoMission @0x00DBDE40, AttackStuff @0x00DBE3C0, PostAttackStuff @0x00DBEB20,
--  ManageQuestCoreMarkers @0x00DBE4E0, StartBarrelTimer @0x00DB4F70, WatchBarrels @0x00DBE890,
--  WatchForGotGold @0x00DBE2E0).
--
-- Evidence level: reconstructed-source. Mode: shadow. Mutating calls and save writes are disabled by
-- the override manifest; nothing in this file is proof of retail parity.
--
-- Lifecycle (retail):
--   RegisterMain spawns "Main"; Main registers the 16 entity bindings, handles the reload-after-raid
--   case, sets objective 1, spawns StartBarrelTimer and runs DoMission on the same thread.
--   DoMission waits for the StartOakVale region, runs the childhood (pre-attack) setup and spawns the
--   watcher threads, then blocks until AttackOver, runs AttackStuff (region swap to the raided village)
--   and PostAttackStuff (dead father scene), and finally completes the quest.
--   Only `AttackOver` is persisted; a save made after the raid resumes directly in the post-attack path.

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")

Quest = nil

-- entity bindings in retail registration order (Main @0x00DABAC0, native-decompile)
local ENTITY_BINDINGS = {
    { "NOVI_LiveFather",    "NewOakValeIntro/Entities/NOVI_LiveFather" },
    { "NOVI_Theresa",       "NewOakValeIntro/Entities/NOVI_Theresa" },
    { "NOVI_Guard",         "NewOakValeIntro/Entities/NOVI_Guard" },
    { "NOVI_Villager",      "NewOakValeIntro/Entities/NOVI_Villager" },
    { "NOVI_Bully",         "NewOakValeIntro/Entities/NOVI_Bully" },
    { "NOVI_Victim",        "NewOakValeIntro/Entities/NOVI_Victim" },
    { "NOVI_TeddyGirl",     "NewOakValeIntro/Entities/NOVI_TeddyGirl" },
    { "NOVI_AffairMan",     "NewOakValeIntro/Entities/NOVI_AffairMan" },
    { "NOVI_AffairWoman",   "NewOakValeIntro/Entities/NOVI_AffairWoman" },
    { "NOVI_AffairWife",    "NewOakValeIntro/Entities/NOVI_AffairWife" },
    { "NOVI_BookTrader",    "NewOakValeIntro/Entities/NOVI_BookTrader" },
    { "NOVI_BarrelMan",     "NewOakValeIntro/Entities/NOVI_BarrelMan" },
    { "NOVI_BarrelThug",    "NewOakValeIntro/Entities/NOVI_BarrelThug" },
    { "NOVI_Barrel",        "NewOakValeIntro/Entities/NOVI_Barrel" },
    { "NOVI_CreatedBeetle", "NewOakValeIntro/Entities/NOVI_CreatedBeetle" },
    { "OVI_DeadFather",     "NewOakValeIntro/Entities/OVI_DeadFather" },
}

-- quest-card objectives (Xbox text header lists OBJECTIVE_01..06; retail uses 01, 02, 03, 06 here)
local TEXT_OBJECTIVE_START      = "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01"
local TEXT_OBJECTIVE_GOT_GOLD   = "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03"
local TEXT_OBJECTIVE_POST_ATTACK = "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_06"

local THEME_POST_ATTACK     = "ENVIRONMENT_OV_POSTATTACK"
local CUTSCENE_DEAD_FATHER  = "CS_OAKVALEINTRO_HESDEADJIM"
local CUTSCENE_ACTOR_HERO   = "HERO"

-- immediates recovered from the retail call sites (capstone disassembly of Fable.exe)
local FADE_OUT_INTO_CHILDHOOD_SECONDS = 2.0
local FADE_OUT_AT_COMPLETION_SECONDS  = 0.5
local FADE_HOLD_SECONDS               = 0.0
local TIME_OF_DAY_CHILDHOOD           = 12.0
local TIME_OF_DAY_RAID                = 23.0
local THEME_TRANSITION_SECONDS        = 0.0
local MUSIC_SET_CHILDHOOD             = 46    -- CacheMusicSet(0x2e)
local MUSIC_SET_POST_ATTACK_ENTRY     = 45    -- CacheMusicSet(0x2d)
local MUSIC_SET_POST_ATTACK_FADE_IN   = 57    -- CacheMusicSet(0x39)
local MUSIC_OVERRIDE_CHILDHOOD        = 19    -- OverrideMusic(0x13, false, true)
local TUTORIAL_QUEST_CORE_MARKER      = 19    -- DisplayTutorial(0x13)
local GOLD_NEEDED_FOR_SWEETS          = 3     -- GetHeroGold() < 3 / > 2 checks
local DAD_TRIGGER_DISTANCE            = 5.0
local WAREHOUSE_GUARD_POINT_DISTANCE  = 2.0
local BARREL_WATCH_SECONDS            = 45.0  -- AddQuestInfoBar max value
local INFO_BAR_SCALE                  = 1.0
local INFO_BAR_UNCHANGED              = -1.0  -- UpdateQuestInfoBar(id, value, -1, -1)
local BEETLE_MAX_HEALTH               = 2.0
local BEETLES_FROM_LAST_N_BARRELS     = 3     -- broken > count-4 spawns a beetle (except the gold barrel)
local LOGBOOK_STORY_ENTRY_RAID        = 20    -- Gameflow adds 10 before and 30 after this quest
local HERO_SCRIPT_PRIORITY            = 4     -- StartScriptingEntity(hero, resource, 4)
local DEACTIVATE_NOW                  = 0

local function active_name()
    return Quest:GetActiveQuestName()
end

local function count(things)
    if type(things) == "table" then
        return #things
    end
    return 0
end

------------------------------------------------------------------------------------------------------
-- lifecycle
------------------------------------------------------------------------------------------------------

-- retail Init @0x00DAADD0: reset all bookkeeping, zero the talk timer, seed the villager speech lists
function Init(questObject)
    Quest = questObject
    F.reset_for_init(Quest)
    -- retail registers both CTimer handles in the constructor; ForgeFSE has no constructor hook
    if not F.get(Quest, F.TalkIntermittentTimer) then
        F.set(Quest, F.TalkIntermittentTimer, Quest:RegisterTimer())
    end
    if not F.get(Quest, F.WatchTimer) then
        F.set(Quest, F.WatchTimer, Quest:RegisterTimer())
    end
    Quest:SetTimer(F.get_int(Quest, F.TalkIntermittentTimer), 0)
    F.reset_for_init_tail(Quest)
    -- villager speech lists: see villager_speech.lua (retail fills eight quest vectors here)
end

-- retail OnPersist @0x00DAADA0: the only persisted member is AttackOver
function OnPersist(questObject, context)
    Quest = questObject
    local attack_over = Quest:PersistTransferBool(context, "AttackOver", F.get(Quest, F.AttackOver) or false)
    F.set(Quest, F.AttackOver, attack_over)
end

-- retail Main @0x00DABAC0
function Main(questObject)
    Quest = questObject
    for _, binding in ipairs(ENTITY_BINDINGS) do
        Quest:AddEntityBinding(binding[1], binding[2])
    end
    -- retail: CTCVillage::OnInitialActivate(this) then PostAddScriptedEntities (host-managed in ForgeFSE)
    Quest:FinalizeEntityBindings()

    if F.get(Quest, F.AttackOver) then
        -- reload after the raid: the post-attack section is torn down before DoMission re-enters it.
        -- Retail checks IsActiveThreadTerminating here without a frame; ForgeFSE exposes no such query.
        Quest:DeactivateQuest(NOVI.SECTION_POST_ATTACK, DEACTIVATE_NOW)
    end

    Quest:SetQuestCardObjective(active_name(), TEXT_OBJECTIVE_START, "", "")
    Quest:CreateThread("StartBarrelTimer")
    DoMission(Quest)
end

------------------------------------------------------------------------------------------------------
-- DoMission @0x00DBDE40
------------------------------------------------------------------------------------------------------

local function wait_for_start_region()
    return NOVI.wait_until(Quest, nil, function()
        return Quest:IsRegionLoaded(NOVI.REGION_START_OAKVALE)
    end)
end

-- childhood setup: turn the hero into the child, freeze the clock at noon, open the family house
local function begin_childhood()
    Quest:FadeScreenOutUntilNextCallToFadeScreenIn(FADE_OUT_INTO_CHILDHOOD_SECONDS, FADE_HOLD_SECONDS)
    Quest:TurnCreatureInto(Quest:GetHero(), NOVI.CREATURE_HERO_CHILD)

    Quest:CreateThread("WatchBarrels")
    Quest:CreateThread("WatchForGotGold")
    Quest:CreateThread("ManageQuestCoreMarkers")

    Quest:CacheMusicSet(MUSIC_SET_CHILDHOOD)
    Quest:ActivateQuest(NOVI.SECTION_PRE_ATTACK)
    if not Quest:NewScriptFrame() then return false end

    local hero = Quest:GetHero()
    Quest:EntitySetAsKillable(hero, false)
    Quest:SetTimeAsStopped(true)                       -- retail also receives &StopTimeIndex
    Quest:SetTimeOfDay(TIME_OF_DAY_CHILDHOOD)
    Quest:SetHeroSleepingAsEnabled(false)
    Quest:DisplayMoneyBag(true)

    local house = Quest:GetThingWithScriptName(NOVI.HOUSE_HERO)
    Quest:OverrideAutomaticHouseLocking(house, true)
    Quest:OpenHouseDoors(house)

    Quest:KickOffQuestStartScreen(active_name(), false, true)
    Quest:OverrideMusic(MUSIC_OVERRIDE_CHILDHOOD, false, true)
    return true
end

local function wait_for_attack()
    return NOVI.wait_until(Quest, nil, function()
        return F.get(Quest, F.AttackOver)
    end)
end

-- AttackStuff @0x00DBE3C0: swap the village sections, jump to night, raided-village theme, objective 6
function AttackStuff()
    Quest:ActivateQuest(NOVI.SECTION_POST_ATTACK)
    Quest:DeactivateQuest(NOVI.SECTION_PRE_ATTACK, DEACTIVATE_NOW)
    Quest:SetTimeOfDay(TIME_OF_DAY_RAID)
    Quest:TransitionToTheme(THEME_POST_ATTACK, THEME_TRANSITION_SECONDS)
    Quest:SetQuestCardObjective(active_name(), TEXT_OBJECTIVE_POST_ATTACK, "", "")
end

local function wait_for_post_attack_section()
    return NOVI.wait_until(Quest, nil, function()
        Quest:GetThingWithScriptName(NOVI.MARKER_POST_ATTACK_START)
        return count(Quest:GetAllThingsWithScriptName(NOVI.MARKER_POST_ATTACK_START)) > 0
    end)
end

-- the dead-father cutscene as retail stages it (movie sequence + macro), then the raid cleanup
local function play_dead_father_scene(hero)
    NOVI.acquire(Quest, hero, HERO_SCRIPT_PRIORITY)
    Quest:StartMovieSequence()
    Quest:PauseAllNonScriptedEntities(true)
    Quest:FixMovieSequenceCamera(true)
    -- retail RunCutsceneMacro_Func(CS_OAKVALEINTRO_HESDEADJIM, {HERO=hero}, 1, 0, 0, 0)
    Quest:PlayCutscene(CUTSCENE_DEAD_FATHER, { [CUTSCENE_ACTOR_HERO] = hero })
    Quest:FixMovieSequenceCamera(false)
    Quest:PauseAllNonScriptedEntities(false)
end

-- PostAttackStuff @0x00DBEB20
function PostAttackStuff()
    if not wait_for_post_attack_section() then return false end

    Quest:CacheMusicSet(MUSIC_SET_POST_ATTACK_ENTRY)
    local start = Quest:GetThingWithScriptName(NOVI.MARKER_POST_ATTACK_START)
    Quest:EntityTeleportToThing(Quest:GetHero(), start)
    local village = Quest:GetThingWithScriptName(NOVI.VILLAGE_OAKVALE)
    Quest:SetVillageLimbo(village, true)
    Quest:DisplayMoneyBag(false)
    Quest:TakeObjectFromHero(NOVI.OBJECT_TEDDY)
    Quest:AddLogbookStoryEntry(LOGBOOK_STORY_ENTRY_RAID)
    if not Quest:NewScriptFrame() then return false end

    Quest:CameraResetToViewBehindHero(0.0)
    Quest:CameraDefault()
    Quest:CacheMusicSet(MUSIC_SET_POST_ATTACK_FADE_IN)
    Quest:FadeScreenIn()

    local trigger = Quest:GetThingWithScriptName(NOVI.MARKER_DAD_TRIGGER)
    local reached = NOVI.wait_until(Quest, nil, function()
        return NOVI.hero_within(Quest, trigger, DAD_TRIGGER_DISTANCE)
    end)
    if not reached then return false end

    F.set(Quest, F.DadFound, true)
    play_dead_father_scene(Quest:GetHero())

    village = Quest:GetThingWithScriptName(NOVI.VILLAGE_OAKVALE)
    Quest:SetVillageLimbo(village, false)
    Quest:SetTimeAsStopped(false)                      -- retail passes &StopTimeIndex back
    Quest:DeactivateQuest(NOVI.SECTION_POST_ATTACK, DEACTIVATE_NOW)
    Quest:ResetToDefaultTheme(0.0)
    Quest:StopOverrideMusic(false)
    return true
end

local function complete_quest()
    Quest:FadeScreenOutUntilNextCallToFadeScreenIn(FADE_OUT_AT_COMPLETION_SECONDS, FADE_HOLD_SECONDS)
    Quest:EntitySetAsKillable(Quest:GetHero(), true)
    Quest:SetHeroSleepingAsEnabled(true)
    Quest:SetQuestAsCompleted(active_name(), false, false, false)
    Quest:DeactivateQuestLater(active_name(), DEACTIVATE_NOW)
end

function DoMission(questObject)
    Quest = questObject or Quest
    if not wait_for_start_region() then return end

    if not F.get(Quest, F.AttackOver) then
        if not begin_childhood() then return end
        if not wait_for_attack() then return end
    end

    -- retail re-checks termination here; ForgeFSE reports it through the last NewScriptFrame result
    AttackStuff()
    if not PostAttackStuff() then return end
    complete_quest()
end

------------------------------------------------------------------------------------------------------
-- spawned threads (retail CSpawnedFunc<CQ_NewOakValeIntroScript>)
------------------------------------------------------------------------------------------------------

-- StartBarrelTimer @0x00DB4F70: once BarrelMan arms WatchTimer, show a 45 s clock bar that is green
-- while the hero stands at the warehouse guard point and red otherwise; remove it when he returns.
function StartBarrelTimer(questObject)
    local quest = questObject or Quest
    local watch_timer = F.get_int(quest, F.WatchTimer)

    local armed = NOVI.wait_until(quest, nil, function()
        return quest:GetTimer(watch_timer) > 0
    end)
    if not armed then return end

    local bar = quest:AddQuestInfoBar(BARREL_WATCH_SECONDS, BARREL_WATCH_SECONDS,
        NOVI.COLOUR_BLACK, NOVI.COLOUR_BLACK, NOVI.HUD_CLOCK_ICON, "", INFO_BAR_SCALE)
    F.set(quest, F.GUIBarrelCounter, bar)
    local guard_point = quest:GetThingWithScriptName(NOVI.MARKER_WAREHOUSE_GUARD)

    while not F.get(quest, F.BarrelManSpokenToHeroOnReturn) do
        if not quest:NewScriptFrame() then return end   -- retail leaves the bar on termination
        local colour = NOVI.COLOUR_RED
        if NOVI.hero_within(quest, guard_point, WAREHOUSE_GUARD_POINT_DISTANCE) then
            colour = NOVI.COLOUR_GREEN
        end
        quest:ChangeQuestInfoBarColour(bar, colour, colour)
        quest:UpdateQuestInfoBar(bar, quest:GetTimer(watch_timer), INFO_BAR_UNCHANGED, INFO_BAR_UNCHANGED)
    end
    quest:RemoveQuestInfoElement(bar)
end

-- WatchBarrels @0x00DBE890: count barrel breaks until the raid; the first is a bad deed, the last
-- three barrels hide two stag beetles and finally a gold coin.
function WatchBarrels(questObject)
    local quest = questObject or Quest
    local Deeds = require("NewOakValeIntro.deeds")

    local total = 0
    local found = NOVI.wait_until(quest, nil, function()
        total = count(quest:GetAllThingsWithScriptName(NOVI.SCRIPT_BARREL))
        return total >= 1
    end)
    if not found then return end

    F.set(quest, F.BarrelBrokenInstantaneous, false)
    local broken = 0
    while true do
        if F.get(quest, F.AttackOver) then return end
        if F.get(quest, F.BarrelBrokenInstantaneous) then
            broken = broken + 1
            F.set(quest, F.BarrelBrokenInstantaneous, false)
            if broken == 1 then
                Deeds.add_bad(quest, nil, Deeds.BAD_DEED_BARREL_BROKEN)
            elseif broken == total - 1 then
                local last_barrel = quest:GetThingWithScriptName(NOVI.SCRIPT_BARREL)
                quest:AddItemToContainer(last_barrel, NOVI.OBJECT_GOLD_1)
            elseif broken > total - (BEETLES_FROM_LAST_N_BARRELS + 1) then
                local beetle = quest:CreateCreature(NOVI.CREATURE_STAG_BEETLE,
                    F.get(quest, F.BarrelBrokenPos), NOVI.SCRIPT_CREATED_BEETLE)
                quest:EntitySetMaxHealth(beetle, BEETLE_MAX_HEALTH, true)
            end
        end
        if not quest:NewScriptFrame() then return end
    end
end

-- WatchForGotGold @0x00DBE2E0: objective 3 once the hero holds the sweets money
function WatchForGotGold(questObject)
    local quest = questObject or Quest
    local enough = NOVI.wait_until(quest, nil, function()
        return quest:GetHeroGold() > GOLD_NEEDED_FOR_SWEETS - 1
    end)
    if not enough then return end
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), TEXT_OBJECTIVE_GOT_GOLD, "", "")
end

-- ManageQuestCoreMarkers @0x00DBE4E0: the quest-core orb moves father -> book trader -> Theresa -> father
function ManageQuestCoreMarkers(questObject)
    local quest = questObject or Quest
    local father = quest:GetThingWithScriptName("NOVI_LiveFather")
    local trader = quest:GetThingWithScriptName("NOVI_BookTrader")
    local theresa = quest:GetThingWithScriptName("NOVI_Theresa")

    quest:MiniMapAddMarker(father, NOVI.HUD_QUEST_CORE_ORB)
    if not NOVI.wait_until(quest, nil, function() return quest:GetHeroGold() >= GOLD_NEEDED_FOR_SWEETS end) then return end
    if not NOVI.wait_until(quest, nil, function() return quest:IsHeroControlledByPlayer() end) then return end

    quest:MiniMapRemoveMarker(father)
    quest:MiniMapAddMarker(trader, NOVI.HUD_QUEST_CORE_ORB)
    if quest:DisplayTutorial(TUTORIAL_QUEST_CORE_MARKER) then
        if not NOVI.wait_until(quest, nil, function() return quest:MsgIsTutorialClickedPast() end) then return end
    end
    if not NOVI.wait_until(quest, nil, function() return F.get(quest, F.GivenSweets) end) then return end

    quest:MiniMapRemoveMarker(trader)
    quest:MiniMapAddMarker(theresa, NOVI.HUD_QUEST_CORE_ORB)
    if not NOVI.wait_until(quest, nil, function() return F.get(quest, F.GivenTheresaChocs) end) then return end

    quest:MiniMapRemoveMarker(theresa)
    quest:MiniMapAddMarker(father, NOVI.HUD_QUEST_CORE_ORB)
end
