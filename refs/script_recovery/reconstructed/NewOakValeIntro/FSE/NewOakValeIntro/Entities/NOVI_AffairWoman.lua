-- NOVI_AffairWoman — reconstructed retail entity script (evidence level: reconstructed-source)
--
-- Retail class : NScript::CQ_NewOakValeIntroScript::CNOVI_AffairWoman
-- Retail Init  : 0x00DB1E80   Retail Main : 0x00DB1F00 (Ghidra label FUN_00db1f00, vtable slot [1])
-- Evidence     : refs/script_recovery/new_oakvale_intro/entities/NOVI_AffairWoman.json
--
-- The lover. She stays at her home position beside NOVI_AffairMan, receives the kiss/hug the man
-- initiates (quest flags ReceiveKiss / ReceiveHug), tells the hero she is "busy" when talked to,
-- and complains and counts a bad deed when hit. The moment NOVI_AffairWife comes within 5 units she
-- runs to the marker AffairWomanRunOffPoint, waits until she is off-screen and removes herself.
-- She holds no entity-local retail fields; TalkingToWoman (quest field) is raised while the hero
-- interacts with her so the man skips his kiss/hug.

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

-- Script names / text keys / animations (native-decompile literals)
local SCRIPT_NAME_WIFE = "NOVI_AffairWife"
local SCRIPT_NAME_MAN = "NOVI_AffairMan"
local SCRIPT_NAME_RUN_OFF_POINT = "AffairWomanRunOffPoint"
local ANIM_RECEIVE_KISS = "RECEIVE_KISS"
local ANIM_RECEIVE_HUG = "RECEIVE_HUG"
local TEXT_ON_HIT = "TEXT_QST_048_AFFAIRWOMAN_ON_HIT"
local TEXT_BUSY = "TEXT_QST_048_AFFAIRWOMAN_BUSY"

-- Numeric constants (immediates in the decompile unless noted)
local SPEAK_HEALTH_THRESHOLD = 0.0        -- DAT_0122dedc (.rdata float, read from retail exe): speak only while health > 0
local HOME_LEAVE_TOLERANCE = 0.1          -- farther than this from home pos -> walk home
local HOME_ARRIVE_RADIUS = 2.0            -- MoveToPosition loop ends inside this radius
local RUN_OFF_ARRIVE_RADIUS = 2.0         -- same radius for the run-off point
local WIFE_FLEE_DISTANCE = 5.0            -- wife this close -> run off
local MAIN_CONTROL_PRIORITY = 4           -- DAT_00000004 loaded before StartScriptingEntity
local CUTSCENE_CONTROL_PRIORITY = 4       -- StartScriptingEntity(me, res, 4) in the talked-to branch
local BAD_DEED_HIT_WOMAN = 2              -- AddBadDeed(PARENT, 2)
local HERO_ABILITY_IGNORED_ON_HIT = 14    -- inference: EHeroAbility 0xe, literal visible only in the AffairWife decompile
local SPEAK_SELECTION_METHOD = 0          -- _Speak_ third argument literal 0
local MOVE_WALK = 0                       -- EScriptEntityMoveType literal 0 (walk home)
local MOVE_RUN = 1                        -- EScriptEntityMoveType literal 1 (run off)
local MOVE_RADIUS = 0.0                   -- _MoveToPosition_ float literal 0
local ZERO_POSITION = { x = 0.0, y = 0.0, z = 0.0 }   -- DAT_0143e8e0: used when the run-off marker is missing

function Init(quest, me)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false)              -- retail passes (me, 0, 0); ForgeFSE binding takes one bool
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    me:SetIsPushableByHero(false)
    quest:EntitySetAsUseMovementInActions(me, false)
end

---------------------------------------------------------------------------------------------------
-- helpers
---------------------------------------------------------------------------------------------------

-- Retail idiom: MsgIsHitBy(hero) || (MsgIsHitByAnySpecialAbilityFrom(hero) && !MsgIsHitBySpecialAbilityFrom(14, hero))
local function hit_by_hero(me)
    if me:MsgIsHitByHero() then return true end
    if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_IGNORED_ON_HIT) then
        return true
    end
    return false
end

-- Retail: if (GetHealth(scripted me) > 0) { Speak(hero, key); while IsPerformingScriptTask: frame }.
local function speak_if_alive(quest, me, key)
    if quest:GetHealth(me) > SPEAK_HEALTH_THRESHOLD then
        me:Speak(quest:GetHero(), key, SPEAK_SELECTION_METHOD, false, true, false)
        while me:IsPerformingScriptTask() do
            if not NOVI.frame(quest, me) then return false end
        end
    end
    return true
end

-- Retail: PauseAllNonScriptedEntities(false) then the movie/resource objects are destroyed
-- (inference: the CScriptGameResourceObjectMovieBase destructor ends the movie sequence).
local function end_talk_cutscene(quest, me)
    quest:PauseAllNonScriptedEntities(false)
    NOVI.release(quest, me)
    quest:EndMovieSequence()
end

-- Shared move loop: frame, MoveToPosition, wait for the task, re-check distance. Returns false on termination.
local function move_until_within(quest, me, pos, radius, move_type)
    while NOVI.distance_from_thing_to_position_over(me, pos, radius) do
        if not NOVI.frame(quest, me) then return false end
        me:MoveToPosition(pos, MOVE_RADIUS, move_type)
        while me:IsPerformingScriptTask() do
            if not NOVI.frame(quest, me) then return false end
        end
    end
    return true
end

---------------------------------------------------------------------------------------------------
-- phases
---------------------------------------------------------------------------------------------------

-- Phase WALK_HOME: retail walks back whenever she is more than 0.1 from her home position.
local function walk_home(quest, me)
    return move_until_within(quest, me, me:GetHomePos(), HOME_ARRIVE_RADIUS, MOVE_WALK)
end

-- Phase HIT: hero struck her. Returns false on termination.
local function react_to_hit(quest, me)
    F.set(quest, F.TalkingToWoman, true)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)           -- retail argument dropped; inference: true (matches the sibling scripts)
    if not NOVI.acquire(quest, me, CUTSCENE_CONTROL_PRIORITY) then end_talk_cutscene(quest, me); return false end
    if not speak_if_alive(quest, me, TEXT_ON_HIT) then end_talk_cutscene(quest, me); return false end
    Deeds.add_bad(quest, me, BAD_DEED_HIT_WOMAN)
    F.set(quest, F.TalkingToWoman, false)
    end_talk_cutscene(quest, me)
    return true
end

-- Phase TALK: hero talked to her. Returns false on termination.
local function talked_to_by_hero(quest, me, man)
    F.set(quest, F.TalkingToWoman, true)
    me:ClearAllActions()
    me:ClearCommands()
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero())   -- retail args dropped; inference: face the hero
    if not NOVI.acquire(quest, me, CUTSCENE_CONTROL_PRIORITY) then end_talk_cutscene(quest, me); return false end
    if not speak_if_alive(quest, me, TEXT_BUSY) then end_talk_cutscene(quest, me); return false end
    -- retail EntitySetFacingAngleTowardsThing(<thing>, me, 0): inference — the man is turned back to face her
    if man then quest:EntitySetFacingAngleTowardsThing(man, me) end
    F.set(quest, F.TalkingToWoman, false)
    end_talk_cutscene(quest, me)
    return true
end

-- Phase RECEIVE: consume the kiss/hug flags the man raised.
local function receive_affection(quest, me)
    if F.get(quest, F.ReceiveKiss) and not me:IsPerformingScriptTask() then
        me:PlayAnimation(ANIM_RECEIVE_KISS)               -- retail flags (0,1,0,1,DAT_01375748=true,0)
        F.set(quest, F.ReceiveKiss, false)
    end
    if F.get(quest, F.ReceiveHug) and not me:IsPerformingScriptTask() then
        me:PlayAnimation(ANIM_RECEIVE_HUG)                -- retail flags (0,1,0,1,DAT_01375748=true,0)
        F.set(quest, F.ReceiveHug, false)
    end
end

-- Phase RUN_OFF: wife arrived — run to the marker, wait until off-screen, remove self. Ends Main.
local function run_off(quest, me)
    local point = quest:GetThingWithScriptName(SCRIPT_NAME_RUN_OFF_POINT)
    quest:EntitySetAsUseMovementInActions(me, true)      -- retail args dropped; inference: (me, true) so she can run
    me:SetIsPushableByHero(false)                        -- retail arg dropped; inference: stays unpushable while fleeing
    local target = ZERO_POSITION
    if point then target = point:GetPos() end
    if not move_until_within(quest, me, target, RUN_OFF_ARRIVE_RADIUS, MOVE_RUN) then return false end
    NOVI.release(quest, me)                              -- retail drops the scripted resource here (HasPhysicsMesh/Clear idiom)
    while quest:IsCameraPosOnScreen(me:GetPos()) do
        if not NOVI.frame(quest, me) then return false end
    end
    quest:RemoveThing(me)
    return true
end

---------------------------------------------------------------------------------------------------
-- Main
---------------------------------------------------------------------------------------------------

function Main(quest, me)
    if not NOVI.frame(quest, me) then return end
    if not NOVI.acquire(quest, me, MAIN_CONTROL_PRIORITY) then return end
    local wife = quest:GetThingWithScriptName(SCRIPT_NAME_WIFE)
    local man = quest:GetThingWithScriptName(SCRIPT_NAME_MAN)

    -- retail: the main scripted resource is only released by its destructor when Main returns
    while true do
        if NOVI.distance_from_thing_to_position_over(me, me:GetHomePos(), HOME_LEAVE_TOLERANCE) then
            if not walk_home(quest, me) then NOVI.release(quest, me); return end
        end
        if hit_by_hero(me) then
            if not react_to_hit(quest, me) then NOVI.release(quest, me); return end
        end
        if me:IsTalkedToByHero() then
            if not talked_to_by_hero(quest, me, man) then NOVI.release(quest, me); return end
        end
        receive_affection(quest, me)
        if quest:IsDistanceBetweenThingsUnder(me, wife, WIFE_FLEE_DISTANCE) then
            run_off(quest, me)                           -- retail returns from Main after RemoveThing (or on termination)
            NOVI.release(quest, me)
            return
        end
        if not NOVI.frame(quest, me) then NOVI.release(quest, me); return end
    end
end
