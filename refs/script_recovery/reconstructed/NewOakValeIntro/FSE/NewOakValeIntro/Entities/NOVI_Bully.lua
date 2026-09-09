-- NOVI_Bully — retail class NScript::CQ_NewOakValeIntroScript::CNOVI_Bully
-- Retail: Init 0x00DAED30, Main 0x00DBB310, GivenTeddy (private) 0x00DBCD00
-- Evidence level: reconstructed-source (Ghidra decompile of retail Fable.exe, normalized)
--
-- The boy bullying the victim in Oakvale. He walks home, then loops every frame:
--   * once the intro line is done, accepts (or refuses) the teddy bear from the hero,
--   * answers the hero's talk with a scripted line chosen by quest state,
--   * counts hero hits on a quest info bar; after InitialHealth hits he is subdued,
--     runs off in the CS_OAKVALEINTRO_BULLYRUN cutscenes, awards a good deed and is removed,
--   * intimidates the victim (conversation line + shake-fist/point animation) on a timer.
-- Evidence JSON: refs/script_recovery/new_oakvale_intro/entities/NOVI_Bully.json

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

-- Constants (all immediates from the decompile unless noted)
local INITIAL_HEALTH = 4                      -- Init: this+0x1c = 4 (hits needed to subdue)
local NASTY_STREAK_MAX_HITS = 3               -- Main: HitsTaken < 3 -> NASTY_STREAK line
local INTIMIDATE_LOOP_START = 10              -- Init: this+0x28 = 10
local INTIMIDATE_LOOP_STEP = 10               -- Main: IntimidateSpeechLoop += 10
local INTIMIDATE_LOOP_MAX = 40                -- Main: 0x28 < value -> reset
local HOME_ARRIVE_RADIUS = 2.0                -- Main: IsDistanceFromThingToPositionOver(me, home, 2.0)
local INTIMIDATE_DISTANCE = 15.0              -- DAT_013ac85c (read from retail .rdata)
local INTIMIDATE_RAND_MOD = 75                -- DAT_013ac860 (read from retail .rdata)
local ALIVE_HEALTH_THRESHOLD = 0.0            -- _DAT_0122dedc (read from retail .rdata)
local HEALTH_COUNTER_UNSET = -999             -- PARENT+0x64 sentinel (init value, see quest Init)
local TEDDY_REWARD_GOLD = 1                   -- GivenTeddy: GiveHeroGold(1)
local BAD_DEED_TEDDY_TO_BULLY = 3             -- GivenTeddy: AddBadDeed(PARENT, 3)
local INTIMIDATE_TIMER_VALUE = nil            -- UNKNOWN: SetTimer value dropped by the decompiler; do not guess
local INFO_BAR_MAX = 0                        -- AddQuestInfoBar 2nd immediate (0)
local INFO_BAR_UNCHANGED = -1.0               -- UpdateQuestInfoBar(handle, remaining, -1.0, -1.0)
local INFO_BAR_SCALE = 1.0                    -- UNKNOWN: retail colour/scale args dropped
local INFO_BAR_COLOUR = { r = 255, g = 255, b = 255, a = 255 }  -- UNKNOWN: retail colours dropped
local ACQUIRE_PRIORITY = nil                  -- retail EScriptAIPriority dropped; host-managed

local TEDDY_OBJECT = "OBJECT_TEDDY_BEAR_UNGIVEABLE"
local VICTIM_SCRIPT_NAME = "NOVI_Victim"
local INFO_BAR_ICON = "HUD_QUEST_ICON_GRANDSON"
local ANIM_SHAKE_FIST = "ST_OPINION_DISAPPROVAL_SHAKE_FIST"
local ANIM_POINT_AT = "ST_OPINION_DISAPPROVAL_POINT_AT"
local CS_BULLYRUN1 = "CS_OAKVALEINTRO_BULLYRUN1"
local CS_BULLYRUN2 = "CS_OAKVALEINTRO_BULLYRUN2"
local CS_BULLYRUN_DUMMY = "CS_OAKVALEINTRO_BULLYRUNDUMMY"

local TEXT = {
  FOUND_TEDDY_ONE = "TEXT_QST_048_BULLY_FOUND_TEDDY_ONE",
  FOUND_TEDDY_TWO = "TEXT_QST_048_BULLY_FOUND_TEDDY_TWO",
  DONT_WANT = "TEXT_QST_048_BULLY_DONT_WANT",
  GIVE_TEDDY_QUESTION = "TEXT_QST_048_GIVE_TEDDY_TO_BULLY",
  ANSWER_YES = "TEXT_OBJECT_HERO_ANSWER_YES",
  ANSWER_NO = "TEXT_OBJECT_HERO_ANSWER_NO",
  IN_COMMON = "TEXT_QST_048_BULLY_IN_COMMON",
  NASTY_STREAK = "TEXT_QST_048_BULLY_NASTY_STREAK",
  DONT_HIT_ME = "TEXT_QST_048_BULLY_DONT_HIT_ME",
  BADGERING = "TEXT_QST_048_BULLY_BADGERING",
  GET_LOST = "TEXT_QST_048_BULLY_GET_LOST",
  SCRMSG_GET_OFF = "TEXT_QST_048_BULLY_SCRMSG_GET_OFF",
  VICTIM_REVENGE = "TEXT_QST_048_VICTIM_REVENGE",
  INTIMIDATING_FMT = "TEXT_QST_048_BULLY_SCRMSG_INTIMIDATING_%d",
  VICTIM_THANKS = "TEXT_QST_048_VICTIM_THANKS",
  VICTIM_THANKS_AFTER_HIT = "TEXT_QST_048_VICTIM_THANKS_AFTER_HIT",
}
local SPEAK_SELECTION = 0                     -- ETextGroupSelectionMethod immediate in every Speak
local ANSWER_YES = 1                          -- MsgIsQuestionAnsweredYesOrNo() == 1
local ANSWER_PENDING = -1                     -- MsgIsQuestionAnsweredYesOrNo() < 0

-- Entity-local retail fields (this+0x1c..), PDB names
local InitialHealth = INITIAL_HEALTH          -- 0x1c (long)
local HitsTaken = 0                           -- 0x20 (long)
local DoneIntro = false                       -- 0x24
local SaidPieceAboutAttackingVictim = false   -- 0x25
local SpokenOnFirstProximity = false          -- 0x26
local IntimidateSpeechLoop = INTIMIDATE_LOOP_START -- 0x28 (long)

function Init(quest, me)
  DoneIntro = false
  HitsTaken = 0
  InitialHealth = INITIAL_HEALTH
  SpokenOnFirstProximity = false
  F.set(quest, F.SpokeAboutFindingTeddy, false)
  SaidPieceAboutAttackingVictim = false
  IntimidateSpeechLoop = INTIMIDATE_LOOP_START
  quest:EntitySetAsDamageable(me, false)
  quest:EntitySetAsKillable(me, false)              -- retail passes a 3rd arg (0); ForgeFSE has 2
  quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
  quest:SetThingHasInformation(me, false)           -- retail (me, 0, 0, 0); ForgeFSE has 2 args
  quest:EntitySetThingAsAllyOfThing(me, quest:GetHero())
  quest:SetIsPushableByHero(me, false)
end

-- CNOVI_Bully::GivenTeddy @0x00DBCD00
local function given_teddy(quest, me)
  quest:GiveHeroGold(TEDDY_REWARD_GOLD)
  quest:TakeObjectFromHero(TEDDY_OBJECT)
  F.set(quest, F.SpokeAboutFindingTeddy, true)
  F.set(quest, F.TeddyRuined, true)
  Deeds.add_bad(quest, me, BAD_DEED_TEDDY_TO_BULLY)
end

-- Retail idiom: `if (GetHealth(me) > 0.0) { Speak(hero, key, 0, ...); while (IsPerformingScriptTask) frame; }`
-- me:Speak is blocking in ForgeFSE, so the per-frame terminate checks inside the wait are host-managed.
local function speak_if_alive(quest, me, key)
  if quest:GetHealth(me) > ALIVE_HEALTH_THRESHOLD then
    me:Speak(quest:GetHero(), key, SPEAK_SELECTION)
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

-- Retail inline "was I hit by the hero" test (same inline in NOVI_Victim / NOVI_Villager):
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

-- Phase 0: walk to my home position until within HOME_ARRIVE_RADIUS (retail IsDistanceFromThingToPositionOver).
local function walk_home(quest, me)
  local home = me:GetHomePos()
  while NOVI.distance_from_thing_to_position_over(me, home, HOME_ARRIVE_RADIUS) do
    if not NOVI.frame(quest, me) then return false end
    -- retail MoveToPosition(home, <float>, <EScriptEntityMoveType>, bool, bool); only the position is known
    me:MoveToPosition(home, HOME_ARRIVE_RADIUS, 0)
    while me:IsPerformingScriptTask() do
      if not NOVI.frame(quest, me) then return false end
    end
  end
  return true
end

-- Wait for the victim to finish complaining (PARENT+0x96 is set here and cleared by NOVI_Victim).
local function wait_victim_complaint(quest, me)
  F.set(quest, F.VictimComplainsAboutLosingTeddy, true)
  while F.get(quest, F.VictimComplainsAboutLosingTeddy) do
    if not NOVI.frame(quest, me) then return false end
  end
  return true
end

-- Teddy handed over directly (MsgIsPresentedWithItem == OBJECT_TEDDY_BEAR_UNGIVEABLE). Returns false on terminate.
local function accept_presented_teddy(quest, me)
  begin_movie(quest)
  if not wait_victim_complaint(quest, me) then end_movie(quest); return false end
  speak_if_alive(quest, me, TEXT.FOUND_TEDDY_TWO)
  given_teddy(quest, me)
  quest:ClearThingHasInformation(me)
  end_movie(quest)
  return true
end

-- Hero presented some other item.
local function refuse_item(quest, me)
  if not NOVI.acquire(quest, me, ACQUIRE_PRIORITY) then return false end
  begin_movie(quest)
  speak_if_alive(quest, me, TEXT.DONT_WANT)
  end_movie(quest)
  return true
end

-- Hero talked to me while carrying the teddy: ask whether to hand it over.
local function offer_teddy_dialogue(quest, me)
  begin_movie(quest)
  speak_if_alive(quest, me, TEXT.FOUND_TEDDY_ONE)
  quest:GiveHeroYesNoQuestion(TEXT.GIVE_TEDDY_QUESTION, TEXT.ANSWER_YES, TEXT.ANSWER_NO, "")
  local answer = quest:MsgIsQuestionAnsweredYesOrNo()
  while answer < 0 do
    if not NOVI.frame(quest, me) then end_movie(quest); return false end
    answer = quest:MsgIsQuestionAnsweredYesOrNo()
  end
  if answer == ANSWER_YES then
    if not wait_victim_complaint(quest, me) then end_movie(quest); return false end
    speak_if_alive(quest, me, TEXT.FOUND_TEDDY_TWO)
    given_teddy(quest, me)
    quest:ClearThingHasInformation(me)
  end
  end_movie(quest)
  return true
end

-- Phase "teddy" (only once DoneIntro). Returns false on terminate.
local function handle_teddy(quest, me)
  local talked_with_teddy = me:IsTalkedToByHero()
    and quest:IsObjectInThingsPossession(quest:GetHero(), TEDDY_OBJECT)
  if talked_with_teddy then
    return offer_teddy_dialogue(quest, me)
  end
  if me:MsgIsPresentedWithItem() and g_PresentedItemName == TEDDY_OBJECT then
    return accept_presented_teddy(quest, me)
  end
  -- retail re-queries MsgIsPresentedWithItem here; the second half of its condition is garbled
  -- in the decompile (inference: any presented item that is not the teddy).
  if me:MsgIsPresentedWithItem() then
    return refuse_item(quest, me)
  end
  return true
end

-- Phase "talk": hero talked to me -> one line chosen by quest state (inside a movie sequence).
local function handle_talk(quest, me)
  if not me:IsTalkedToByHero() then return true end
  if not NOVI.acquire(quest, me, ACQUIRE_PRIORITY) then return false end
  begin_movie(quest)
  if DoneIntro then
    if F.get(quest, F.HeroAttackedVictim) then
      if not SaidPieceAboutAttackingVictim then
        speak_if_alive(quest, me, TEXT.IN_COMMON)
        SaidPieceAboutAttackingVictim = true
      elseif HitsTaken < NASTY_STREAK_MAX_HITS then
        speak_if_alive(quest, me, TEXT.NASTY_STREAK)
      else
        speak_if_alive(quest, me, TEXT.DONT_HIT_ME)
      end
    else
      speak_if_alive(quest, me, TEXT.BADGERING)
    end
  else
    speak_if_alive(quest, me, TEXT.GET_LOST)
    DoneIntro = true
  end
  end_movie(quest)
  return true
end

-- Subdued: run-off cutscene, good deed, remove me. Ends the script.
local function run_off(quest, me, victim)
  NOVI.acquire(quest, me, ACQUIRE_PRIORITY)
  local hero = quest:GetHero()
  NOVI.acquire(quest, hero, ACQUIRE_PRIORITY)     -- retail StartScriptingEntity(GetHero(), ...)
  NOVI.acquire(quest, victim, ACQUIRE_PRIORITY)   -- retail StartScriptingEntity(<victim>, ...)
  local actors = { HERO = hero, BRAT = victim, BULLY = me }
  local brat_line = TEXT.VICTIM_THANKS
  if F.get(quest, F.HeroAttackedVictim) then brat_line = TEXT.VICTIM_THANKS_AFTER_HIT end
  local flags = { ["$BRATLINE"] = brat_line }
  begin_movie(quest)
  quest:FixMovieSequenceCamera(true)
  quest:RunCutsceneWithSetup(CS_BULLYRUN1, actors, flags)
  if not F.get(quest, F.GivenHeroTeddy) then
    quest:RunCutsceneWithSetup(CS_BULLYRUN2, actors, flags)
    quest:ClearThingHasInformation(me)
    F.set(quest, F.GivenHeroTeddy, true)
  else
    quest:RunCutsceneWithSetup(CS_BULLYRUN_DUMMY, actors, flags)
  end
  quest:FixMovieSequenceCamera(false)
  end_movie(quest)
  F.set(quest, F.BullyRanOff, true)
  Deeds.add_good(quest, me)
  quest:RemoveThing(me)                            -- retail RemoveThing(me, 0, 1)
end

-- Phase "hit": count hero hits on the info bar. Returns "removed" after run_off, false on terminate.
local function handle_hit(quest, me, victim)
  if not hit_by_hero(me) then return true end
  local hero = quest:GetHero()
  if F.get(quest, F.GUIBullyHealthCounter) == HEALTH_COUNTER_UNSET then
    local handle = quest:AddQuestInfoBar(InitialHealth, INFO_BAR_MAX, INFO_BAR_COLOUR, INFO_BAR_COLOUR,
      INFO_BAR_ICON, "", INFO_BAR_SCALE)
    F.set(quest, F.GUIBullyHealthCounter, handle)
  end
  -- two ally calls with dropped args; NOVI_Villager shows (me, hero) then (hero, me)
  quest:EntitySetThingAsAllyOfThing(me, hero)
  quest:EntitySetThingAsAllyOfThing(hero, me)
  HitsTaken = HitsTaken + 1
  if InitialHealth <= HitsTaken then
    F.set(quest, F.BullySubdued, true)
    quest:RemoveQuestInfoElement(F.get(quest, F.GUIBullyHealthCounter))
    run_off(quest, me, victim)
    return "removed"
  end
  local conv = quest:AddNewConversation(me, false, false)
  quest:AddPersonToConversation(conv, me)
  quest:AddLineToConversation(conv, TEXT.SCRMSG_GET_OFF, me, hero)     -- listener inferred
  quest:AddLineToConversation(conv, TEXT.VICTIM_REVENGE, me, victim)   -- listener inferred
  quest:UpdateQuestInfoBar(F.get(quest, F.GUIBullyHealthCounter), InitialHealth - HitsTaken,
    INFO_BAR_UNCHANGED, INFO_BAR_UNCHANGED)
  return true
end

-- Phase "intimidate": timer-gated taunt at the victim while no hit has landed yet.
local function intimidate(quest, me)
  local timer = F.get(quest, F.TalkIntermittentTimer)   -- GetTimer/SetTimer id dropped; inference
  if quest:GetTimer(timer) ~= 0 then return true end
  if SpokenOnFirstProximity and math.random(0, INTIMIDATE_RAND_MOD - 1) ~= 0 then return true end
  if not (NOVI.hero_within(quest, me, INTIMIDATE_DISTANCE) and HitsTaken == 0) then return true end
  SpokenOnFirstProximity = true
  NOVI.unsupported(quest, "SetTimer", { timer, INTIMIDATE_TIMER_VALUE })
  F.set(quest, F.VictimShake, true)
  local conv = quest:AddNewConversation(me, false, false)
  quest:AddPersonToConversation(conv, me)
  quest:AddLineToConversation(conv, string.format(TEXT.INTIMIDATING_FMT, IntimidateSpeechLoop), me, me)
  IntimidateSpeechLoop = IntimidateSpeechLoop + INTIMIDATE_LOOP_STEP
  if IntimidateSpeechLoop > INTIMIDATE_LOOP_MAX then IntimidateSpeechLoop = INTIMIDATE_LOOP_START end
  -- retail PlayAnimation(anim, 0, 0, 0, 1, true, false): non-blocking
  if math.random(0, 1) == 0 then
    me:PlayAnimation(ANIM_SHAKE_FIST, false)
  else
    me:PlayAnimation(ANIM_POINT_AT, false)
  end
  return true
end

function Main(quest, me)
  if not NOVI.frame(quest, me) then return end
  if not NOVI.acquire(quest, me, ACQUIRE_PRIORITY) then NOVI.release(quest, me); return end
  if not walk_home(quest, me) then NOVI.release(quest, me); return end
  local victim = quest:GetThingWithScriptName(VICTIM_SCRIPT_NAME)
  while true do
    if not NOVI.acquire(quest, me, ACQUIRE_PRIORITY) then NOVI.release(quest, me); return end
    if DoneIntro then
      if not handle_teddy(quest, me) then NOVI.release(quest, me); return end
    end
    if not handle_talk(quest, me) then NOVI.release(quest, me); return end
    local hit = handle_hit(quest, me, victim)
    if hit == "removed" or hit == false then return end
    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero())   -- args dropped; target inferred
    if not intimidate(quest, me) then return end
    if not NOVI.frame(quest, me) then NOVI.release(quest, me); return end
  end
  -- retail never releases the scripted resource, removes the conversations or the info bar on
  -- termination; the movie scope objects unwind via their destructors only.
end
