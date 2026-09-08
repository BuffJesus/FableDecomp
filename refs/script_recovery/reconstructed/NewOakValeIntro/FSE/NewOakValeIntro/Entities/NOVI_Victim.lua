-- NOVI_Victim — retail class NScript::CQ_NewOakValeIntroScript::CNOVI_Victim
-- Retail: Init 0x00DAEEB0 (Ghidra label "CNOVI_BookTrader::Init" is a wrong donor name; the
--         NOVI_Victim vtable slot [2] = 0xdaee b0 proves it is Victim's Init), Main 0x00DBCD60
-- Evidence level: reconstructed-source (Ghidra decompile of retail Fable.exe, normalized)
--
-- The boy being bullied. He flinches (VictimShake) while the bully taunts him, pleads with the
-- hero when talked to, shows the "hitting friends" tutorial once, reacts to being hit by the hero
-- (bad deed, CS_OAKVALEINTRO_BRATHIT cutscene the first time, an "evil bros" exchange after), thanks
-- the hero once the bully is subdued, and complains when the hero gives his teddy to the bully.
-- Evidence JSON: refs/script_recovery/new_oakvale_intro/entities/NOVI_Victim.json

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

-- Constants
local ALIVE_HEALTH_THRESHOLD = 0.0            -- _DAT_0122dedc (read from retail .rdata)
local BAD_DEED_HIT_VICTIM = 2                 -- Main: AddBadDeed(PARENT, 2)
local PLEA_PRIORITY = 4                       -- Main: StartScriptingEntity(me, res, 4) in the plea path
local ACQUIRE_PRIORITY = nil                  -- other StartScriptingEntity priorities dropped; host-managed
local SPEAK_SELECTION = 0                     -- ETextGroupSelectionMethod immediate
local SPEAK_SELECTION_EVIL_BROS = 2           -- Main: Speak(hero, VICTIM_EVIL_BROS, 2, ...)

local BULLY_SCRIPT_NAME = "NOVI_Bully"
local CS_BRATHIT = "CS_OAKVALEINTRO_BRATHIT"

local TEXT = {
  PLEA = "TEXT_QST_048_VICTIM_PLEA",
  PLEA_AFTER_ATTACK = "TEXT_QST_048_VICTIM_PLEA_AFTER_ATTACK",
  THANKS = "TEXT_QST_048_VICTIM_THANKS",
  THANKS_AFTER_HIT = "TEXT_QST_048_VICTIM_THANKS_AFTER_HIT",
  INSTRUCTION_HITTING_FRIENDS_PC = "TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS_PC",
  INSTRUCTION_HITTING_FRIENDS = "TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS",
  EVIL_BROS = "TEXT_QST_048_VICTIM_EVIL_BROS",
  BULLY_HERO_ATTACKS_VICTIM = "TEXT_QST_048_BULLY_HERO_ATTACKS_VICTIM",
  EVIL_BROS_10 = "TEXT_QST_048_VICTIM_EVIL_BROS_10",
}

-- Entity-local retail fields (this+0x1c..), PDB names
local DisplayedGameInfo                       -- 0x1c
local DoneThanks                              -- 0x1d

function Init(quest, me)
  DoneThanks = false
  DisplayedGameInfo = false
  quest:EntitySetAsDamageable(me, false)
  quest:EntitySetAsKillable(me, false)              -- retail passes a 3rd arg (0); ForgeFSE has 2
  quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
  quest:SetThingHasInformation(me, false)           -- retail (me, 0, 0, 0); ForgeFSE has 2 args
  quest:SetIsPushableByHero(me, false)
  quest:EntitySetAsUseMovementInActions(me, false)
  quest:EntitySetAsScared(me, true)
end

-- Retail idiom: `if (GetHealth(me) > 0.0) { Speak(hero, key, sel, ...); while (IsPerformingScriptTask) frame; }`
-- me:Speak is blocking in ForgeFSE, so the per-frame terminate checks inside the wait are host-managed.
local function speak_if_alive(quest, me, key, selection)
  if quest:GetHealth(me) > ALIVE_HEALTH_THRESHOLD then
    me:Speak(quest:GetHero(), key, selection or SPEAK_SELECTION)
  end
end

-- Retail movie-sequence scope object: ctor = StartMovieSequence("") + PauseAllNonScriptedEntities(1),
-- dtor = PauseAllNonScriptedEntities(0) (EndMovieSequence inside the dtor is an inference).
local function begin_movie(quest)
  quest:StartMovieSequence()
  quest:PauseAllNonScriptedEntities(true)
end
local function end_movie(quest)
  quest:PauseAllNonScriptedEntities(false)
  quest:EndMovieSequence()
end

-- Retail inline "was I hit by the hero" test (same inline as NOVI_Bully / NOVI_Villager):
--   MsgIsHitBy(HERO) || (MsgIsHitByAnySpecialAbilityFrom(HERO) && !MsgIsHitBySpecialAbilityFrom(<ability>, HERO))
-- The <ability> immediate is dropped here; NOVI_Villager passes 0xe (cross-script evidence).
local EXCLUDED_ABILITY = 0xe
local function hit_by_hero(me)
  if me:MsgIsHitByHero() then return true end
  if me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(EXCLUDED_ABILITY) then
    return true
  end
  return false
end

-- Phase "shake": while the bully is not subdued, flinch towards him each time he taunts.
local function react_to_taunt(quest, me, bully)
  if not F.get(quest, F.VictimShake) then return true end
  F.set(quest, F.VictimShake, false)
  quest:EntitySetFacingAngleTowardsThing(me, bully)   -- args dropped; target inferred (bully)
  return true
end

-- Phase "freed": bully subdued -> stop being scared, wait for him to run off, become a normal villager.
local function thanks_setup(quest, me)
  if DoneThanks then return true end
  DoneThanks = true
  quest:EntitySetAsScared(me, false)                  -- args dropped; value inferred (undo Init's true)
  while not F.get(quest, F.BullyRanOff) do
    if not NOVI.frame(quest, me) then return false end
  end
  quest:SetIsPushableByHero(me, true)                 -- args dropped; value inferred (undo Init's false)
  quest:EntitySetAsUseMovementInActions(me, true)     -- args dropped; value inferred (undo Init's false)
  quest:ClearThingHasInformation(me)
  return true
end

-- Hero talked to me before the bully is subdued: plea inside a movie sequence.
local function plea(quest, me, bully)
  quest:EntitySetAsScared(me, false)                  -- args dropped; value inferred
  quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero())
  begin_movie(quest)
  NOVI.acquire(quest, me, PLEA_PRIORITY)
  if not F.get(quest, F.HeroAttackedVictim) then
    speak_if_alive(quest, me, TEXT.PLEA)
  else
    speak_if_alive(quest, me, TEXT.PLEA_AFTER_ATTACK)
  end
  quest:EntitySetAsScared(me, true)
  quest:EntitySetFacingAngleTowardsThing(me, bully)   -- retail (me, <thing>, 0); target inferred (bully)
  end_movie(quest)
  return true
end

-- Hero talked to me after the bully is subdued: thanks.
local function thanks(quest, me)
  begin_movie(quest)
  NOVI.acquire(quest, me, ACQUIRE_PRIORITY)
  if not F.get(quest, F.HeroAttackedVictim) then
    speak_if_alive(quest, me, TEXT.THANKS)
  else
    speak_if_alive(quest, me, TEXT.THANKS_AFTER_HIT)
  end
  end_movie(quest)
  return true
end

-- One-shot tutorial after the first talk (PC vs Xbox text). Returns false on terminate.
local function show_hitting_friends_info(quest, me)
  if DisplayedGameInfo then return true end
  local key = TEXT.INSTRUCTION_HITTING_FRIENDS_PC
  if NOVI.unsupported(quest, "IsXbox", {}) then key = TEXT.INSTRUCTION_HITTING_FRIENDS end
  quest:DisplayGameInfo(key)
  while not quest:MsgIsGameInfoClickedPast() do
    if not NOVI.frame(quest, me) then return false end
  end
  DisplayedGameInfo = true
  return true
end

local function handle_talk(quest, me, bully)
  if not me:IsTalkedToByHero() then return true end
  if not F.get(quest, F.BullySubdued) then
    plea(quest, me, bully)
  else
    thanks(quest, me)
  end
  return show_hitting_friends_info(quest, me)
end

-- First hit by the hero: CS_OAKVALEINTRO_BRATHIT cutscene (HERO + BRAT).
local function brat_hit_cutscene(quest, me, bully)
  NOVI.acquire(quest, me, ACQUIRE_PRIORITY)
  local hero = quest:GetHero()
  NOVI.acquire(quest, hero, ACQUIRE_PRIORITY)       -- retail StartScriptingEntity(GetHero(), ...)
  local actors = { HERO = hero, BRAT = me }
  begin_movie(quest)
  quest:FixMovieSequenceCamera(true)
  quest:RunCutsceneWithSetup(CS_BRATHIT, actors, {})
  quest:FixMovieSequenceCamera(false)
  quest:ClearThingHasInformation(me)
  quest:EntitySetFacingAngleTowardsThing(me, bully)   -- retail (me, <thing>, 0); target inferred (bully)
  end_movie(quest)
  return true
end

-- Later hits: victim + bully exchange (or the victim alone once the bully is gone).
local function evil_bros_exchange(quest, me)
  local conv = quest:AddNewConversation(me, false, false)   -- retail args dropped
  local bully = quest:GetThingWithScriptName(BULLY_SCRIPT_NAME)
  if not bully:IsAlive() then
    begin_movie(quest)
    NOVI.acquire(quest, me, ACQUIRE_PRIORITY)
    speak_if_alive(quest, me, TEXT.EVIL_BROS, SPEAK_SELECTION_EVIL_BROS)
    end_movie(quest)
    return true
  end
  quest:AddPersonToConversation(conv, me)                  -- args dropped; person inferred
  quest:AddLineToConversation(conv, TEXT.EVIL_BROS, me, bully)                 -- args dropped; inferred
  quest:AddLineToConversation(conv, TEXT.BULLY_HERO_ATTACKS_VICTIM, bully, me) -- retail (conv, key, 0, me, <bully>)
  return true
end

local function handle_hit(quest, me, bully)
  if not hit_by_hero(me) then return true end
  local hero = quest:GetHero()
  -- two ally calls with dropped args; NOVI_Villager shows (me, hero) then (hero, me)
  quest:EntitySetThingAsAllyOfThing(me, hero)
  quest:EntitySetThingAsAllyOfThing(hero, me)
  Deeds.add_bad(quest, me, BAD_DEED_HIT_VICTIM)
  F.set(quest, F.HeroAttackedVictim, true)
  if not F.get(quest, F.GivenHeroTeddy) then
    F.set(quest, F.GivenHeroTeddy, true)
    return brat_hit_cutscene(quest, me, bully)
  end
  return evil_bros_exchange(quest, me)
end

-- The bully set VictimComplainsAboutLosingTeddy; complain once, then clear it so he continues.
local function complain_about_teddy(quest, me)
  if not F.get(quest, F.VictimComplainsAboutLosingTeddy) then return true end
  NOVI.acquire(quest, me, ACQUIRE_PRIORITY)
  speak_if_alive(quest, me, TEXT.EVIL_BROS_10)
  F.set(quest, F.VictimComplainsAboutLosingTeddy, false)
  return true
end

function Main(quest, me)
  if not NOVI.frame(quest, me) then return end
  NOVI.acquire(quest, me, ACQUIRE_PRIORITY)
  local bully = quest:GetThingWithScriptName(BULLY_SCRIPT_NAME)
  while true do
    if not F.get(quest, F.BullySubdued) then
      react_to_taunt(quest, me, bully)
    else
      if not thanks_setup(quest, me) then return end
    end
    if not handle_talk(quest, me, bully) then return end
    handle_hit(quest, me, bully)
    complain_about_teddy(quest, me)
    if not NOVI.frame(quest, me) then return end
  end
  -- retail never releases the scripted resource or removes its conversations on termination;
  -- the movie scope objects unwind via their destructors only.
end
