-- NOVI_BarrelThug — retail NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelThug
-- Init 0x00DB6BF0, Main 0x00DB6C60
-- Evidence level: reconstructed-source (Ghidra decompile of retail Fable.exe + PDB names)
--
-- The thug who tempts the hero into smashing the barrel man's stock. Once the barrel man leaves the
-- hero in charge he teleports to M_WHouse_ManStart, explains the dare, follows the hero, and nags
-- with timed tempt/well-done lines until the barrel man is back; hitting him is a bad deed.

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

-- Constants
local HERO_SCRIPT_NAME    = "SCRIPT_NAME_HERO"
local START_MARKER        = "M_WHouse_ManStart"
local SPEAK_MIN_HEALTH    = 0.0     -- _DAT_0122dedc (retail .rdata = 0.0)
local SPEECH_METHOD       = 0
local SCRMSG_METHOD       = 2       -- ETextGroupSelectionMethod immediate 2 on SCRMSG_TEMPT / SCRMSG_WELLDONE
local FOLLOW_DISTANCE     = 1.0     -- DAT_3f800000
local BAD_DEED_HIT_ME     = 2
local ACTION_PRIORITY     = 4
local DEFAULT_PRIORITY    = nil
local EXCLUDED_HIT_ABILITY = 14   -- sibling NOVI hit predicates pass 0x0e
local LAST_TIME_INIT      = 9999    -- LastTimeSpoken initial value (Init)

-- Timer thresholds: line fires when timer < T and LastTimeSpoken > T (timer counts down)
local TEMPT_TIERS = {
    { 10, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_10" },
    { 20, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_20" },
    { 25, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_30" },
    { 30, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_40" },
    { 34, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_50" },
    { 38, "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_60" },
}
local WELLDONE_TIERS = {
    { 10, "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_10" },
    { 25, "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_20" },
    { 35, "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_30" },
}
-- Last tier uses a different comparison: timer <= 44 and LastTimeSpoken >= 46
local LAST_TIER_TIMER_MAX = 44
local LAST_TIER_LAST_MIN  = 46
local TEXT_TEMPT_70       = "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT_70"
local TEXT_WELLDONE_40    = "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE_40"

-- Text keys (spoken)
local TEXT_EXPLAIN        = "TEXT_QST_048_BARRELTHUG_EXPLAIN"
local TEXT_SCRMSG_TEMPT   = "TEXT_QST_048_BARRELTHUG_SCRMSG_TEMPT"
local TEXT_SCRMSG_WELLDONE = "TEXT_QST_048_BARRELTHUG_SCRMSG_WELLDONE"
local TEXT_WHY_NOT_SMASH  = "TEXT_QST_048_BARRELTHUG_WHY_NOT_SMASH"
local TEXT_OUTRO          = "TEXT_QST_048_BARRELTHUG_OUTRO"
local TEXT_WHY_HIT        = "TEXT_QST_048_BARRELTHUG_WHY_HIT"

-- Retail entity fields
local DoneIntro      = false            -- 0x1c
local LastTimeSpoken = LAST_TIME_INIT   -- 0x20

function Init(quest, me)
    DoneIntro = false
    LastTimeSpoken = LAST_TIME_INIT
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
end

local function speak_if_alive(quest, me, key, method)
    if quest:GetHealth(me) > SPEAK_MIN_HEALTH then
        me:SpeakAndWait(key, method or SPEECH_METHOD)
    end
end

local function begin_cutscene(quest)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
end

local function end_cutscene(quest)
    quest:PauseAllNonScriptedEntities(false)
    quest:EndMovieSequence()   -- inference: retail movie object destructor
end

local function hero_hit_me(quest, me)
    if me:MsgIsHitByHero() then return true end
    if me:MsgIsHitByAnySpecialAbilityFromHero() then
        if not me:MsgIsHitByHeroSpecialAbility(EXCLUDED_HIT_ABILITY) then return true end
    end
    return false
end

-- Phase: wait for the barrel man to leave, appear at the marker, explain, follow the hero
local function intro(quest, me)
    NOVI.acquire(quest, me, DEFAULT_PRIORITY)
    while not F.get(quest, F.BarrelManLeftHeroInCharge) do
        if not NOVI.frame(quest, me) then return false end
    end
    quest:EntityTeleportToThing(me, quest:GetThingWithScriptName(START_MARKER))
    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero())
    NOVI.unsupported(quest, "Pause", { "<duration dropped>" })
    begin_cutscene(quest)
    speak_if_alive(quest, me, TEXT_EXPLAIN)
    DoneIntro = true
    me:FollowThing(quest:GetHero(), FOLLOW_DISTANCE, true)   -- retail FollowThing(hero, 1.0, 1)
    end_cutscene(quest)
    return true
end

-- Phase: hero talks to him
local function chat(quest, me)
    begin_cutscene(quest)
    NOVI.acquire(quest, me, DEFAULT_PRIORITY)
    local man_back = F.get(quest, F.BarrelManSpokenToHeroOnReturn)
    local broken = F.get(quest, F.BarrelBrokenPersistent)
    if not man_back then
        if not broken then
            speak_if_alive(quest, me, TEXT_SCRMSG_TEMPT, SCRMSG_METHOD)
        else
            speak_if_alive(quest, me, TEXT_SCRMSG_WELLDONE, SCRMSG_METHOD)
        end
    else
        if not broken then
            speak_if_alive(quest, me, TEXT_WHY_NOT_SMASH)
        else
            speak_if_alive(quest, me, TEXT_OUTRO)
        end
    end
    end_cutscene(quest)
end

local function pick_tier_line(tiers, last_line, timer)
    for _, tier in ipairs(tiers) do
        if timer < tier[1] and LastTimeSpoken > tier[1] then return tier[2] end
    end
    if timer <= LAST_TIER_TIMER_MAX and LastTimeSpoken >= LAST_TIER_LAST_MIN then return last_line end
    return nil
end

-- Phase: timed nag lines while the barrel man is away (timer > 0)
local function nag(quest, me)
    local nag_timer = F.get(quest, F.WatchTimer)
    local timer = quest:GetTimer(nag_timer)
    if F.get(quest, F.BarrelManSpokenToHeroOnReturn) or timer <= 0 then return end
    local conv = quest:AddNewConversation(me)   -- retail args dropped
    quest:AddPersonToConversation(conv, quest:GetHero())
    local line
    if not F.get(quest, F.BarrelBrokenPersistent) then
        line = pick_tier_line(TEMPT_TIERS, TEXT_TEMPT_70, quest:GetTimer(nag_timer))
    else
        line = pick_tier_line(WELLDONE_TIERS, TEXT_WELLDONE_40, quest:GetTimer(nag_timer))
    end
    if line == nil then return end   -- retail skips the LastTimeSpoken update too (conversation left open)
    quest:AddLineToConversation(conv, line, me, quest:GetHero())
    LastTimeSpoken = quest:GetTimer(nag_timer)
end

-- Phase: hero hit him
local function react_to_hit(quest, me)
    local hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, hero)    -- twice in retail, args dropped
    quest:EntitySetThingAsAllyOfThing(hero, me)
    Deeds.add_bad(quest, me, BAD_DEED_HIT_ME)
    begin_cutscene(quest)
    NOVI.acquire(quest, me, ACTION_PRIORITY)
    speak_if_alive(quest, me, TEXT_WHY_HIT)
    end_cutscene(quest)
end

function Main(quest, me)
    if not NOVI.frame(quest, me) then return end
    while true do
        if not DoneIntro then
            if not intro(quest, me) then NOVI.release(quest, me); return end
        end
        if me:IsTalkedToByHero() then
            chat(quest, me)
        end
        nag(quest, me)
        if hero_hit_me(quest, me) then
            react_to_hit(quest, me)
        end
        if not NOVI.frame(quest, me) then NOVI.release(quest, me); return end
    end
end
