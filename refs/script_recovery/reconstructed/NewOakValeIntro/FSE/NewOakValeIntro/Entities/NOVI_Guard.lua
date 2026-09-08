-- NOVI_Guard - Oakvale prologue guard.
-- Retail class : NScript::CQ_NewOakValeIntroScript::CNOVI_Guard   (vtable 0x012d83c0)
-- Retail Init  : 0x00DAC650   Retail Main : 0x00DAC760   (no OnPredicateFail override)
-- Evidence     : reconstructed-source (Ghidra decompile of retail Fable.exe + Ego_r PDB names)
--
-- What he does: wanders near his post with weapons sheathed. Every frame he compares the
-- quest's BadDeedsPerformed counter with GuardsDealtWithBadDeeds; when the hero has committed
-- a bad deed the guards have not yet punished and is close enough, he runs after the hero,
-- waits for the hero to be player-controlled, freezes the town in a movie sequence and reads
-- out the list of crimes (a longer speech the first time, a short "again" version after that).
-- When talked to he replies according to the hero's good/bad tally; when hit he complains but
-- stays an ally of the hero (retail never makes him hostile).

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

-- Init constants (native-decompile immediates, 0x00DAC650)
local WANDER_MIN_DISTANCE = 0.0          -- SetWanderMinDistance(me, 0)
local WANDER_MAX_DISTANCE = 6.0          -- SetWanderMaxDistance(me, 0x40c00000)
local SCRIPTING_STATE_GROUP = 4          -- SetScriptingStateGroup(me, 4)

-- Main constants
local CONTROL_PRIORITY = 4               -- StartScriptingEntity(me, res, 4)
local MAX_CHASE_TIER = 3                 -- `if (2 < bad) bad = 3`
-- DAT_013ac844[0..3], read from retail .rdata (0x013ac844..0x013ac850): 0.0, 5.0, 10.0, 15.0
local CHASE_DISTANCE_BY_BAD_DEEDS = { [0] = 0.0, [1] = 5.0, [2] = 10.0, [3] = 15.0 }
local LECTURE_DISTANCE = 3.0             -- DAT_013ac840 (retail .rdata 0x013ac840 = 3.0)
local FOLLOW_DISTANCE = 1.0              -- _FollowThing_ float arg (0x3f800000)
local FOLLOW_AVOID_OBSTACLES = true      -- _FollowThing_ bool arg (1)
local ALIVE_HEALTH = 0.0                 -- _DAT_0122dedc (retail .rdata = 0.0): speak only while GetHealth(me) > 0
local SPEAK_METHOD = 0                   -- ETextGroupSelectionMethod literal passed to _Speak_ (0)
-- 0xe literal in the sibling CNOVI_Villager::Main hit test; the Guard decompile dropped the arg.
local EXCLUDED_HIT_ABILITY = 14
local CUTSCENE_BEHAVIOUR_ON = 1          -- EntitySetCutsceneBehaviour(me, 1) (register held 0x00000001)
local CUTSCENE_BEHAVIOUR_OFF = 0         -- inference: second call restores (arg dropped by decompiler)

-- EBadDeeds index -> crime line (native order of the WhichBadDeedsPerformed[0..4] checks)
local CRIME_LINES = {
  [0] = "TEXT_QST_048_GUARD_CRIME_BARREL_BREAKING",
  [1] = "TEXT_QST_048_GUARD_CRIME_DERELICTION_OF_DUTY",
  [2] = "TEXT_QST_048_GUARD_CRIME_VIOLENCE",
  [3] = "TEXT_QST_048_GUARD_CRIME_TEDDY_TO_BULLY",
  [4] = "TEXT_QST_048_GUARD_CRIME_CONCEALED_AFFAIR",
}

-- Retail "hit by hero" test (CGameScriptThing slots 0x54 / 0xa8 / 0xa4 with "SCRIPT_NAME_HERO"):
-- hit by hero, or hit by any hero special ability except ability 14.
local function hero_hit_me(me)
  if me:MsgIsHitByHero() then return true end
  if me:MsgIsHitByAnySpecialAbilityFromHero()
     and not me:MsgIsHitByHeroSpecialAbility(EXCLUDED_HIT_ABILITY) then
    return true
  end
  return false
end

-- Retail: GetHealth(me) > 0 then _Speak_(hero, key, 0, false, true, false) + IsPerformingScriptTask wait.
-- me:Speak == FSE Speak_Blocking(target, key, method, false, true, false) -> same args, host waits.
local function speak_if_alive(quest, me, hero, key)
  if quest:GetHealth(me) > ALIVE_HEALTH then
    me:Speak(hero, key, SPEAK_METHOD)
  end
end

local function speak_crime_list(quest, me, hero)
  for index = 0, 4 do
    if F.get(quest, F.WhichBadDeedsPerformed, index) then
      speak_if_alive(quest, me, hero, CRIME_LINES[index])
    end
  end
end

-- Retail wraps the whole lecture in one StartMovieSequence("") + PauseAllNonScriptedEntities(true)
-- and closes it with PauseAllNonScriptedEntities(false) + movie destructor (== EndMovieSequence).
local function lecture_hero(quest, me, hero)
  quest:StartMovieSequence()
  quest:PauseAllNonScriptedEntities(true)
  quest:EntitySetFacingAngleTowardsThing(me, hero)
  if F.get(quest, F.GuardsSpokenOnce) then
    speak_if_alive(quest, me, hero, "TEXT_QST_048_GUARD_CAUGHT_YOU_AGAIN")
    speak_crime_list(quest, me, hero)
    speak_if_alive(quest, me, hero, "TEXT_QST_048_GUARD_AFTER_READ_LIST")
  else
    speak_if_alive(quest, me, hero, "TEXT_QST_048_GUARD_CAUGHT_YOU_10")
    speak_if_alive(quest, me, hero, "TEXT_QST_048_GUARD_CAUGHT_YOU_20")
    speak_crime_list(quest, me, hero)
    speak_if_alive(quest, me, hero, "TEXT_QST_048_GUARD_CAUGHT_YOU_30")
    speak_if_alive(quest, me, hero, "TEXT_QST_048_GUARD_CAUGHT_YOU_40")
    speak_if_alive(quest, me, hero, "TEXT_QST_048_GUARD_CAUGHT_YOU_50")
    speak_if_alive(quest, me, hero, "TEXT_QST_048_GUARD_CAUGHT_YOU_60")
    F.set(quest, F.GuardsSpokenOnce, true)
  end
  quest:PauseAllNonScriptedEntities(false)
  quest:EndMovieSequence()
end

local function chase_distance(quest)
  local tier = F.get(quest, F.BadDeedsPerformed)
  if tier > MAX_CHASE_TIER - 1 then tier = MAX_CHASE_TIER end
  return CHASE_DISTANCE_BY_BAD_DEEDS[tier]
end

local function unpunished_bad_deeds(quest)
  return F.get(quest, F.BadDeedsPerformed) > F.get(quest, F.GuardsDealtWithBadDeeds)
end

-- Phase: chase the hero, then read the crime list. Returns false when the thread was terminated.
local function chase_and_lecture(quest, me)
  local hero = quest:GetHero()
  quest:EntitySetThingAsAllyOfThing(me, hero)
  quest:EntitySetThingAsAllyOfThing(hero, me)
  if not NOVI.acquire(quest, me, CONTROL_PRIORITY) then return false end

  if not NOVI.hero_within(quest, me, LECTURE_DISTANCE) then
    quest:EntitySetFacingAngleTowardsThing(me, hero)
    local conv = quest:AddNewConversation(me, 0, 0)
    quest:AddPersonToConversation(conv, hero)
    quest:AddLineToConversation(conv, "TEXT_QST_048_SCRMSG_GUARD_GOING_AFTER", me, hero, false)
    me:FollowThing(hero, FOLLOW_DISTANCE, FOLLOW_AVOID_OBSTACLES)
  end

  quest:EntitySetCutsceneBehaviour(me, CUTSCENE_BEHAVIOUR_ON)
  while not NOVI.hero_within(quest, me, LECTURE_DISTANCE) do
    if not NOVI.frame(quest, me) then NOVI.release(quest, me); return false end
  end
  quest:EntitySetCutsceneBehaviour(me, CUTSCENE_BEHAVIOUR_OFF)

  -- Retail re-checks the counters: another guard may have dealt with the deeds meanwhile.
  if not unpunished_bad_deeds(quest) then
    NOVI.release(quest, me)
    return true
  end
  F.set(quest, F.GuardsDealtWithBadDeeds, F.get(quest, F.BadDeedsPerformed))

  NOVI.release(quest, me)
  if not NOVI.acquire(quest, me, CONTROL_PRIORITY) then return false end
  while not quest:IsHeroControlledByPlayer() do
    if not NOVI.frame(quest, me) then NOVI.release(quest, me); return false end
  end

  lecture_hero(quest, me, hero)
  NOVI.release(quest, me)
  return true
end

-- Phase: reply when the hero talks to me (no scripted control is taken for this).
local function answer_hero(quest, me)
  local hero = quest:GetHero()
  quest:EntitySetFacingAngleTowardsThing(me, hero)
  local conv = quest:AddNewConversation(me, 0, 0)
  quest:AddPersonToConversation(conv, hero)
  local bad = F.get(quest, F.BadDeedsPerformed)
  local good = F.get(quest, F.GoodDeedsPerformed)
  if bad == 0 and good > 0 then
    quest:AddLineToConversation(conv, "TEXT_QST_048_GUARD_ON_TALK_GOOD", me, hero)
  elseif bad < 1 then
    quest:AddLineToConversation(conv, "TEXT_QST_048_GUARD_ON_TALK_NEUTRAL", me, hero)
  else
    quest:AddLineToConversation(conv, "TEXT_QST_048_GUARD_ON_TALK_BAD", me, hero)
  end
  while quest:IsConversationActive(conv) do
    if not NOVI.frame(quest, me) then return false end
  end
  -- retail never calls RemoveConversation here
  return true
end

-- Phase: complain when hit; retail keeps the guard allied with the hero.
local function react_to_hit(quest, me)
  local hero = quest:GetHero()
  quest:EntitySetThingAsAllyOfThing(me, hero)
  quest:EntitySetThingAsAllyOfThing(hero, me)
  if not NOVI.acquire(quest, me, CONTROL_PRIORITY) then return false end
  quest:StartMovieSequence()
  quest:PauseAllNonScriptedEntities(true)
  speak_if_alive(quest, me, hero, "TEXT_QST_048_GUARD_ON_HIT")
  NOVI.release(quest, me)
  quest:PauseAllNonScriptedEntities(false)
  quest:EndMovieSequence()
  return true
end

function Init(quest, me)
  quest:EntitySetAsDamageable(me, false)
  quest:EntitySetAsKillable(me, false)
  quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
  quest:SetWanderCentrePoint(me, me:GetHomePos())
  quest:SetWanderMinDistance(me, WANDER_MIN_DISTANCE)
  quest:SetWanderMaxDistance(me, WANDER_MAX_DISTANCE)
  quest:SetScriptingStateGroup(me, SCRIPTING_STATE_GROUP)
  quest:EntitySheatheWeapons(me, false)
end

function Main(quest, me)
  while true do
    if unpunished_bad_deeds(quest)
       and NOVI.hero_within(quest, me, chase_distance(quest)) then
      if not chase_and_lecture(quest, me) then return end
    end

    if me:IsTalkedToByHero() then
      if not answer_hero(quest, me) then return end
    end

    if hero_hit_me(me) then
      if not react_to_hit(quest, me) then return end
    end

    if not NOVI.frame(quest, me) then return end
  end
end
