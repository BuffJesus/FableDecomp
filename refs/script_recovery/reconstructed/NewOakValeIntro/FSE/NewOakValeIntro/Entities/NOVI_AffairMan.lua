-- NOVI_AffairMan — reconstructed retail entity script (evidence level: reconstructed-source)
--
-- Retail class : NScript::CQ_NewOakValeIntroScript::CNOVI_AffairMan
-- Retail Init  : 0x00DB0950   Retail Main : 0x00DB09E0 (Ghidra label FUN_00db09e0, vtable slot [1])
-- Evidence     : refs/script_recovery/new_oakvale_intro/entities/NOVI_AffairMan.json
--
-- The unfaithful husband. He idles beside NOVI_AffairWoman (the lover) and, when the hero is in
-- earshot, the pair exchange numbered "dirty words" conversation lines and kiss/hug each other.
-- If the hero talks to him he asks the hero to keep quiet (yes/no question): "yes" is bribed with
-- one gold coin and counted as a bad deed, "no" is remembered. If NOVI_AffairWife is near he cowers,
-- and if the hero talks to him with the wife present he reacts according to the earlier answer.
-- Being hit by the hero triggers a short movie-sequence complaint and a bad deed.

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

-- Script names / text keys / animations (native-decompile literals)
local SCRIPT_NAME_WOMAN = "NOVI_AffairWoman"
local SCRIPT_NAME_WIFE = "NOVI_AffairWife"
local ANIM_COWER = "ST_OPINION_FEAR_IDLE_COWERING"
local ANIM_GIVE_KISS = "GIVE_KISS"
local ANIM_GIVE_HUG = "GIVE_HUG"
local TEXT_ON_HIT = "TEXT_QST_048_AFFAIRMAN_ON_HIT"
local TEXT_HAD_A_DEAL = "TEXT_QST_048_AFFAIRMAN_HAD_A_DEAL"
local TEXT_SOME_NERVE = "TEXT_QST_048_AFFAIRMAN_SOME_NERVE"
local TEXT_HOW_FIND_OUT = "TEXT_QST_048_AFFAIRMAN_HOW_FIND_OUT"
local TEXT_INTRO = "TEXT_QST_048_AFFAIRMAN_INTRO"
local TEXT_QUESTION_WILL_YOU_TELL = "TEXT_QST_048_AFFAIRMAN_QUESTION_WILL_YOU_TELL"
local TEXT_ANSWER_YES = "TEXT_OBJECT_HERO_ANSWER_YES"
local TEXT_ANSWER_NO = "TEXT_OBJECT_HERO_ANSWER_NO"
local TEXT_GOOD_LAD = "TEXT_QST_048_AFFAIRMAN_GOOD_LAD"
local TEXT_DONT_TELL = "TEXT_QST_048_AFFAIRMAN_DONT_TELL"
local TEXT_SHOO = "TEXT_QST_048_AFFAIRMAN_SHOO"
local TEXT_MAN_DIRTY_WORDS_NEAR = "TEXT_QST_048_AFFAIRMAN_DIRTY_WORDS_NEAR_"       -- + BadgerIndex
local TEXT_WOMAN_DIRTY_WORDS_NEAR = "TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_NEAR_"   -- + BadgerIndex
local TEXT_MAN_SCRMSG_DIRTY_WORDS = "TEXT_QST_048_AFFAIRMAN_SCRMSG_DIRTY_WORDS_"   -- + BadgerIndex
local TEXT_WOMAN_DIRTY_WORDS = "TEXT_QST_048_AFFAIRWOMAN_DIRTY_WORDS_"           -- + BadgerIndex

-- Numeric constants (immediates in the decompile unless noted)
local SPEAK_HEALTH_THRESHOLD = 0.0        -- DAT_0122dedc (.rdata float, read from retail exe): speak only while health > 0
local WIFE_COWER_DISTANCE = 5.0           -- wife this close -> cower
local COWER_CHANCE_ONE_IN = 50            -- rand() % 0x32 == 0
local HOME_LEAVE_TOLERANCE = 0.1          -- farther than this from home pos -> walk home
local HOME_ARRIVE_RADIUS = 2.0            -- MoveToPosition loop ends inside this radius
local LOVERS_TOGETHER_DISTANCE = 2.0      -- woman must be this close for any affair behaviour
local REPEAT_COMMENT_CHANCE_ONE_IN = 100  -- after the first ranged comment: rand() % 100 == 0
local HERO_NEAR_DISTANCE = 5.0            -- near conversation / question
local HERO_RANGED_DISTANCE = 13.0         -- ranged (screen message) conversation
local WIFE_CONFRONT_DISTANCE = 5.0        -- wife present when the hero talks to him
local BADGER_STEP = 10                    -- BadgerIndex += 10 per exchange
local BADGER_MAX = 50                     -- 0x32; above this the index wraps to BADGER_RESET
local BADGER_RESET = 10
local HUSH_MONEY_GOLD = 1                 -- GiveHeroGold(1)
local BAD_DEED_HIT_MAN = 2                -- AddBadDeed(PARENT, 2)
local BAD_DEED_TOOK_BRIBE = 4             -- AddBadDeed(PARENT, 4)
local CUTSCENE_CONTROL_PRIORITY = 4       -- StartScriptingEntity(me, res, 4) in the talked-to branch
local MAIN_CONTROL_PRIORITY = 4           -- inference: argument dropped by the decompiler; sibling calls use 4
local HERO_ABILITY_IGNORED_ON_HIT = 14    -- inference: EHeroAbility 0xe, literal visible only in the AffairWife decompile
local SPEAK_SELECTION_METHOD = 0          -- _Speak_ third argument literal 0
local MOVE_WALK = 0                       -- EScriptEntityMoveType literal 0
local MOVE_RADIUS = 0.0                   -- _MoveToPosition_ float literal 0
local YESNO_ANSWER_YES = 1                -- MsgIsQuestionAnsweredYesOrNo() == 1
local PAUSE_SECONDS_UNKNOWN = 0.0         -- unknown: retail Pause() argument dropped by the decompiler

-- Entity-local retail fields (this+0x1c..0x20, PDB names)
local EncounterOver            -- 0x1c
local SaidFirstRangedComment   -- 0x1d
local HeroAgreedToKeepQuiet    -- 0x1e
local HeroSaidHeWouldReportMan -- 0x1f
local BadgerIndex              -- 0x20 (long)
local last_conversation        -- retail stack local holding the last AddNewConversation id (inference)

function Init(quest, me)
    BadgerIndex = 0
    EncounterOver = false
    SaidFirstRangedComment = false
    HeroAgreedToKeepQuiet = false
    HeroSaidHeWouldReportMan = false
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false)              -- retail passes (me, 0, 0); ForgeFSE binding takes one bool
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    me:SetIsPushableByHero(false)
    quest:EntitySetAsUseMovementInActions(me, false)
    quest:SetThingHasInformation(me, false)           -- retail passes (me, 0, 0, 0); ForgeFSE binding takes one bool
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
-- ForgeFSE me:Speak is blocking, so the frame loop is host-side. Returns false on termination.
local function speak_if_alive(quest, me, key)
    if quest:GetHealth(me) > SPEAK_HEALTH_THRESHOLD then
        me:Speak(quest:GetHero(), key, SPEAK_SELECTION_METHOD, false, true, false)
        while me:IsPerformingScriptTask() do
            if not NOVI.frame(quest, me) then return false end
        end
    end
    return true
end

-- Retail: StartMovieSequence("") + PauseAllNonScriptedEntities(true) + StartScriptingEntity(me, res, 4)
local function begin_talk_cutscene(quest, me)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    return NOVI.acquire(quest, me, CUTSCENE_CONTROL_PRIORITY)
end

-- Retail: PauseAllNonScriptedEntities(false) then the movie/resource objects are destroyed
-- (inference: the CScriptGameResourceObjectMovieBase destructor ends the movie sequence).
local function end_talk_cutscene(quest, me)
    quest:PauseAllNonScriptedEntities(false)
    NOVI.release(quest, me)
    quest:EndMovieSequence()
end

local function next_badger_index()
    BadgerIndex = BadgerIndex + BADGER_STEP
    if BadgerIndex > BADGER_MAX then
        BadgerIndex = BADGER_RESET
    end
    return tostring(BadgerIndex)   -- GFIntToCharString(BadgerIndex) appended to the text key stem
end

---------------------------------------------------------------------------------------------------
-- phases
---------------------------------------------------------------------------------------------------

-- Phase HIT: hero struck him. Returns false on termination.
local function react_to_hit(quest, me)
    if not begin_talk_cutscene(quest, me) then end_talk_cutscene(quest, me); return false end
    if not speak_if_alive(quest, me, TEXT_ON_HIT) then end_talk_cutscene(quest, me); return false end
    Deeds.add_bad(quest, me, BAD_DEED_HIT_MAN)
    end_talk_cutscene(quest, me)
    return true
end

-- Sub-phase of TALK: the wife is standing nearby, so the hero must have told her.
local function confronted_with_wife(quest, me)
    local key
    if HeroAgreedToKeepQuiet then
        key = TEXT_HAD_A_DEAL
    elseif HeroSaidHeWouldReportMan then
        key = TEXT_SOME_NERVE
    else
        key = TEXT_HOW_FIND_OUT
    end
    if not speak_if_alive(quest, me, key) then return false end
    quest:ClearThingHasInformation(me)
    return true
end

-- Sub-phase of TALK: first conversation — "will you tell?" yes/no question.
local function ask_hero_to_keep_quiet(quest, me)
    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero())
    if not speak_if_alive(quest, me, TEXT_INTRO) then return false end
    F.set(quest, F.HeroDiscoveredInfidelity, true)
    -- retail GiveHeroYesNoQuestion(question, yes, no, "", 1); the trailing 1 has no ForgeFSE parameter
    quest:GiveHeroYesNoQuestion(TEXT_QUESTION_WILL_YOU_TELL, TEXT_ANSWER_YES, TEXT_ANSWER_NO, "")
    local answer = quest:MsgIsQuestionAnsweredYesOrNo()
    while answer < 0 do
        if not NOVI.frame(quest, me) then return false end
        answer = quest:MsgIsQuestionAnsweredYesOrNo()
    end
    if answer == YESNO_ANSWER_YES then
        if not speak_if_alive(quest, me, TEXT_GOOD_LAD) then return false end
        Deeds.add_bad(quest, me, BAD_DEED_TOOK_BRIBE)
        quest:GiveHeroGold(HUSH_MONEY_GOLD)
        HeroAgreedToKeepQuiet = true
    else
        if not speak_if_alive(quest, me, TEXT_DONT_TELL) then return false end
        HeroSaidHeWouldReportMan = true
    end
    quest:ClearThingHasInformation(me)
    EncounterOver = true
    return true
end

-- Phase TALK: hero talked to him. Returns false on termination.
local function talked_to_by_hero(quest, me, woman, wife)
    if not begin_talk_cutscene(quest, me) then end_talk_cutscene(quest, me); return false end
    me:ClearAllActions()
    me:ClearCommands()
    local ok
    if quest:IsDistanceBetweenThingsUnder(me, wife, WIFE_CONFRONT_DISTANCE) then
        ok = confronted_with_wife(quest, me)
    elseif not EncounterOver then
        ok = ask_hero_to_keep_quiet(quest, me)
    else
        ok = speak_if_alive(quest, me, TEXT_SHOO)
    end
    if not ok then end_talk_cutscene(quest, me); return false end
    -- inference: the partner (woman if alive, else wife) is turned to face him before the scene ends
    if woman and woman:IsAlive() then
        quest:EntitySetFacingAngleTowardsThing(woman, me)
    elseif wife and wife:IsAlive() then
        quest:EntitySetFacingAngleTowardsThing(wife, me)
    end
    end_talk_cutscene(quest, me)
    return true
end

-- Phase WALK_HOME: retail walks back whenever he is more than 0.1 from his home position.
local function walk_home(quest, me)
    local home = me:GetHomePos()
    while NOVI.distance_from_thing_to_position_over(me, home, HOME_ARRIVE_RADIUS) do
        if not NOVI.frame(quest, me) then return false end
        me:MoveToPosition(home, MOVE_RADIUS, MOVE_WALK)
        while me:IsPerformingScriptTask() do
            if not NOVI.frame(quest, me) then return false end
        end
    end
    return true
end

-- Sub-phase of AFFAIR: hero within 5 — the lovers whisper a numbered line pair; sets HeroDiscoveredInfidelity.
local function near_conversation(quest, me, woman)
    local conv = quest:AddNewConversation(me, false, false)   -- retail args dropped by the decompiler
    last_conversation = conv
    quest:AddPersonToConversation(conv, woman)                -- retail args dropped; inference: the woman joins
    local n = next_badger_index()
    local man_line = TEXT_MAN_DIRTY_WORDS_NEAR .. n
    local woman_line = TEXT_WOMAN_DIRTY_WORDS_NEAR .. n
    -- retail: the very first exchange (BadgerIndex == 10) uses the reversed speaker order (inference on the
    -- dropped first-line arguments; the second line's speaker/listener order is visible)
    if BadgerIndex == BADGER_RESET then
        quest:AddLineToConversation(conv, woman_line, woman, me)
        quest:AddLineToConversation(conv, man_line, me, woman)
    else
        quest:AddLineToConversation(conv, man_line, me, woman)
        quest:AddLineToConversation(conv, woman_line, woman, me)
    end
    F.set(quest, F.HeroDiscoveredInfidelity, true)
end

-- Sub-phase of AFFAIR: hero within 13 — screen-message version; marks SaidFirstRangedComment.
local function ranged_conversation(quest, me, woman)
    SaidFirstRangedComment = true
    local conv = quest:AddNewConversation(me, false, false)   -- retail args dropped by the decompiler
    last_conversation = conv
    quest:AddPersonToConversation(conv, woman)                -- retail args dropped; inference: the woman joins
    local n = next_badger_index()
    quest:AddLineToConversation(conv, TEXT_MAN_SCRMSG_DIRTY_WORDS .. n, me, woman)   -- first line args dropped; inference
    quest:AddLineToConversation(conv, TEXT_WOMAN_DIRTY_WORDS .. n, woman, me)
end

-- Sub-phase of AFFAIR: kiss or hug the woman (skipped while the hero is talking to her).
local function kiss_or_hug(quest, me, woman)
    quest:Pause(PAUSE_SECONDS_UNKNOWN)
    quest:EntitySetFacingAngleTowardsThing(me, woman)     -- retail args dropped; inference: face each other
    quest:EntitySetFacingAngleTowardsThing(woman, me)
    if math.random(0, 1) == 0 then                        -- rand() & 1 == 0
        F.set(quest, F.ReceiveKiss, true)
        me:PlayAnimation(ANIM_GIVE_KISS)                  -- retail flags (0,1,0,1,DAT_01375748=true,0)
    else
        F.set(quest, F.ReceiveHug, true)
        me:PlayAnimation(ANIM_GIVE_HUG)                   -- retail flags (0,1,0,1,DAT_01375748=true,0)
    end
end

-- Phase AFFAIR: at home with the woman beside him. Returns false on termination.
local function affair_at_home(quest, me, woman)
    -- retail gate: conversation active, or (first ranged comment said and rand()%100 != 0), or woman not within 2
    if last_conversation and quest:IsConversationActive(last_conversation) then return true end  -- retail arg dropped
    if SaidFirstRangedComment and math.random(0, REPEAT_COMMENT_CHANCE_ONE_IN - 1) ~= 0 then return true end
    if not quest:IsDistanceBetweenThingsUnder(me, woman, LOVERS_TOGETHER_DISTANCE) then return true end

    if NOVI.hero_within(quest, me, HERO_NEAR_DISTANCE) then
        near_conversation(quest, me, woman)
    elseif NOVI.hero_within(quest, me, HERO_RANGED_DISTANCE) then
        ranged_conversation(quest, me, woman)
    end
    if not F.get(quest, F.TalkingToWoman) then
        kiss_or_hug(quest, me, woman)
    end
    return true
end

-- Phase IDLE dispatcher (only reached when no scripted task is running). Returns false on termination.
local function idle_behaviour(quest, me, woman, wife)
    if quest:IsDistanceBetweenThingsUnder(me, wife, WIFE_COWER_DISTANCE) then
        quest:EntitySetFacingAngleTowardsThing(me, wife)  -- retail args dropped; inference: face the wife
        if math.random(0, COWER_CHANCE_ONE_IN - 1) == 0 then
            me:PlayAnimation(ANIM_COWER)                  -- retail flags (0,0,0,1,DAT_01375748=true,0)
        end
        return true
    end
    if NOVI.distance_from_thing_to_position_over(me, me:GetHomePos(), HOME_LEAVE_TOLERANCE) then
        return walk_home(quest, me)
    end
    return affair_at_home(quest, me, woman)
end

---------------------------------------------------------------------------------------------------
-- Main
---------------------------------------------------------------------------------------------------

function Main(quest, me)
    if not NOVI.frame(quest, me) then return end
    if not NOVI.acquire(quest, me, MAIN_CONTROL_PRIORITY) then return end
    local woman = quest:GetThingWithScriptName(SCRIPT_NAME_WOMAN)
    local wife = quest:GetThingWithScriptName(SCRIPT_NAME_WIFE)

    -- retail: the main scripted resource is only released by its destructor when Main returns
    while true do
        local ok = true
        if hit_by_hero(me) then
            ok = react_to_hit(quest, me)
        elseif me:IsTalkedToByHero() then
            ok = talked_to_by_hero(quest, me, woman, wife)
        elseif not me:IsPerformingScriptTask() then
            ok = idle_behaviour(quest, me, woman, wife)
        end
        if not ok then NOVI.release(quest, me); return end
        if not NOVI.frame(quest, me) then NOVI.release(quest, me); return end
    end
end
