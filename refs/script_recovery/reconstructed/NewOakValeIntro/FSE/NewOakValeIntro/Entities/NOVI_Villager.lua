-- NOVI_Villager - generic Oakvale prologue villager.
-- Retail class : NScript::CQ_NewOakValeIntroScript::CNOVI_Villager   (vtable 0x012d8678)
-- Retail Init  : 0x00DADF00   Retail Main : 0x00DADF80
-- Retail helper: CNOVI_Villager::GetVillagerSpeechIndex(long) 0x00DAEBF0
-- Evidence     : reconstructed-source (Ghidra decompile of retail Fable.exe + Ego_r PDB names)
--
-- What they do: undamageable, unkillable allies of the child hero. If the hero hits one it logs a
-- "violence" bad deed, remembers it (HeroDidHitMe) and complains in a movie sequence. When talked
-- to it greets the hero (or reproaches them if it was hit). Otherwise, roughly once per 100 frames
-- while the shared TalkIntermittentTimer is idle and the hero is within 5m, it mutters one random
-- line about the hero's reputation (good / bad / both / no deeds, by villager sex).

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

-- Reputation lines. Retail keeps these in eight std::vector<CCharString> members of the QUEST
-- object (PARENT+0x9c..0xf0, filled by CQ_NewOakValeIntroScript::Init 0x00DAADD0). ForgeFSE entity
-- scripts cannot read quest-object vectors, so they are mirrored here (design decision: inference).
local VILLAGER_SPEECH_MALE_GOOD = {
  "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_10",
  "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_20",
  "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_30",
  "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_40",
  "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_50",
  "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_MALE_60",
}
local VILLAGER_SPEECH_MALE_BAD = {
  "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_10",
  "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_20",
  "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_30",
  "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_40",
  "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_50",
  "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_MALE_60",
}
local VILLAGER_SPEECH_MALE_BOTH = {
  "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_10",
  "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_20",
  "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_30",
  "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_MALE_40",
}
local VILLAGER_SPEECH_MALE_NEITHER = {
  "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_10",
  "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_20",
  "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_30",
  "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_40",
  "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_MALE_50",
}
local VILLAGER_SPEECH_FEMALE_GOOD = {
  "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_10",
  "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_20",
  "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_30",
  "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_40",
  "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_50",
  "TEXT_QST_048_VILLAGER_DONE_GOOD_DEEDS_FEMALE_60",
}
local VILLAGER_SPEECH_FEMALE_BAD = {
  "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_10",
  "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_20",
  "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_30",
  "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_40",
  "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_50",
  "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS_FEMALE_60",
}
local VILLAGER_SPEECH_FEMALE_BOTH = {
  "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_10",
  "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_20",
  "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_30",
  "TEXT_QST_048_VILLAGER_DONE_BOTH_DEEDS_FEMALE_40",
}
local VILLAGER_SPEECH_FEMALE_NEITHER = {
  "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_10",
  "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_20",
  "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_30",
  "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_40",
  "TEXT_QST_048_VILLAGER_DONE_NO_DEEDS_FEMALE_50",
}

-- Constants (native-decompile immediates)
local CONTROL_PRIORITY = 4               -- StartScriptingEntity(me, res, 4)
local SEX_MALE = 1                       -- EntityGetSex(me) == 1
local ALIVE_HEALTH = 0.0                 -- _DAT_0122dedc (retail .rdata = 0.0)
local SPEAK_METHOD = 1                   -- ETextGroupSelectionMethod literal passed to _Speak_ (1)
local EXCLUDED_HIT_ABILITY = 14          -- MsgIsHitBySpecialAbilityFrom(0xe, "SCRIPT_NAME_HERO")
local BAD_DEED_VIOLENCE = 2              -- AddBadDeed(PARENT, 2)
local MUTTER_CHANCE_ONE_IN = 100         -- rand() % 100 == 0
local MUTTER_DISTANCE = 5.0              -- IsDistanceBetweenThingsUnder(me, hero, 5.0)
local MUTTER_TIMER_VALUE = 3             -- SetTimer(TalkIntermittentTimer, 3)
local TIMER_IDLE = 0                     -- GetTimer(TalkIntermittentTimer) == 0

-- Entity-local retail field (this+0x1c)
local HeroDidHitMe = false

local function sex_suffix(quest, me)
  if quest:EntityGetSex(me) == SEX_MALE then return "_MALE" end
  return "_FEMALE"
end

-- Retail hit test (CGameScriptThing slots 0x54 / 0xa8 / 0xa4 with "SCRIPT_NAME_HERO").
local function hero_hit_me(me)
  if me:MsgIsHitByHero() then return true end
  if me:MsgIsHitByAnySpecialAbilityFromHero()
     and not me:MsgIsHitByHeroSpecialAbility(EXCLUDED_HIT_ABILITY) then
    return true
  end
  return false
end

-- CNOVI_Villager::GetVillagerSpeechIndex(count) 0x00DAEBF0: rand() % count, re-rolled on a NEW
-- FRAME until it differs from the quest's lastVillagerSpeechIdx, then stored there. Returns a
-- 0-based index (nil when the thread is terminated; retail returns 0 and its caller ignores it).
local function get_villager_speech_index(quest, me, count)
  local last = F.get(quest, F.lastVillagerSpeechIdx)
  local index = math.random(0, count - 1)
  while index == last do
    if not NOVI.frame(quest, me) then return nil end
    index = math.random(0, count - 1)
  end
  F.set(quest, F.lastVillagerSpeechIdx, index)
  return index
end

-- Retail nesting: (bad==0 && good>0) -> Good; else (bad>=1 && good==0) -> Bad;
-- else (bad==0 && good==0) -> Neither; else -> Both. Sex read via EntityGetSex(me).
local function pick_reputation_table(quest, me)
  local bad = F.get(quest, F.BadDeedsPerformed)
  local good = F.get(quest, F.GoodDeedsPerformed)
  local male = quest:EntityGetSex(me) == SEX_MALE
  if bad == 0 and good > 0 then
    return male and VILLAGER_SPEECH_MALE_GOOD or VILLAGER_SPEECH_FEMALE_GOOD
  elseif bad < 1 or good ~= 0 then
    if bad < 1 or good < 1 then
      return male and VILLAGER_SPEECH_MALE_NEITHER or VILLAGER_SPEECH_FEMALE_NEITHER
    end
    return male and VILLAGER_SPEECH_MALE_BOTH or VILLAGER_SPEECH_FEMALE_BOTH
  end
  return male and VILLAGER_SPEECH_MALE_BAD or VILLAGER_SPEECH_FEMALE_BAD
end

-- Phase: the hero hit me.
local function react_to_hit(quest, me)
  local hero = quest:GetHero()
  quest:EntitySetThingAsAllyOfThing(me, hero)
  quest:EntitySetThingAsAllyOfThing(hero, me)
  Deeds.add_bad(quest, me, BAD_DEED_VIOLENCE)
  HeroDidHitMe = true
  if not NOVI.acquire(quest, me, CONTROL_PRIORITY) then return false end
  quest:StartMovieSequence()
  quest:PauseAllNonScriptedEntities(true)
  local key = "TEXT_QST_048_VILLAGER_ATTACKED" .. sex_suffix(quest, me)
  if quest:GetHealth(me) > ALIVE_HEALTH then
    me:Speak(hero, key, SPEAK_METHOD)
  end
  quest:PauseAllNonScriptedEntities(false)
  quest:EndMovieSequence()
  -- retail releases the scripted resource at the top of the next loop iteration
  NOVI.release(quest, me)
  return true
end

-- Phase: the hero talked to me.
local function answer_hero(quest, me)
  local suffix = sex_suffix(quest, me)
  if not NOVI.acquire(quest, me, CONTROL_PRIORITY) then return false end
  local hero = quest:GetHero()
  quest:EntitySetFacingAngleTowardsThing(me, hero)
  local conv = quest:AddNewConversation(me, 0, 0)
  quest:AddPersonToConversation(conv, hero)
  if not HeroDidHitMe then
    quest:AddLineToConversation(conv, "TEXT_QST_048_VILLAGER_SPOKEN_TO" .. suffix, me, hero, false)
  else
    quest:AddLineToConversation(conv, "TEXT_QST_048_VILLAGER_DONE_BAD_DEEDS" .. suffix, me, hero, false)
  end
  while quest:IsConversationActive(conv) do
    if not NOVI.frame(quest, me) then NOVI.release(quest, me); return false end
  end
  -- retail never calls RemoveConversation
  NOVI.release(quest, me)
  return true
end

-- Phase: random reputation mutter (no control taken, conversation not waited on).
local function mutter_about_hero(quest, me)
  local timer = F.get(quest, F.TalkIntermittentTimer)
  if quest:GetTimer(timer) ~= TIMER_IDLE then return true end
  if math.random(0, MUTTER_CHANCE_ONE_IN - 1) ~= 0 then return true end
  if not NOVI.hero_within(quest, me, MUTTER_DISTANCE) then return true end

  quest:SetTimer(timer, MUTTER_TIMER_VALUE)
  local hero = quest:GetHero()
  local conv = quest:AddNewConversation(me, 0, 0)
  quest:AddPersonToConversation(conv, hero)
  local lines = pick_reputation_table(quest, me)
  local index = get_villager_speech_index(quest, me, #lines)
  if index == nil then return false end
  quest:AddLineToConversation(conv, lines[index + 1], me, hero, false)
  return true
end

function Init(quest, me)
  HeroDidHitMe = false
  quest:EntitySetAsDamageable(me, false)
  quest:EntitySetAsKillable(me, false)
  quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
  quest:EntitySetThingAsAllyOfThing(me, quest:GetHero())
end

function Main(quest, me)
  if not NOVI.frame(quest, me) then return end   -- retail: one frame before the loop
  while true do
    if hero_hit_me(me) then
      if not react_to_hit(quest, me) then return end
    elseif me:IsTalkedToByHero() then
      if not answer_hero(quest, me) then return end
    else
      if not mutter_about_hero(quest, me) then return end
    end
    if not NOVI.frame(quest, me) then return end
  end
end
