-- NOVI_AffairWife — reconstructed retail entity script (evidence level: reconstructed-source)
--
-- Retail class : NScript::CQ_NewOakValeIntroScript::CNOVI_AffairWife
-- Retail Init  : 0x00DB2A70 (Ghidra carries the wrong donor label CV_SickChildScript::CTalkingTrader1::Init;
--                the NOVI_AffairWife vtable slot [2] proves it is this class's Init)
-- Retail Main  : 0x00DB2B10 (Ghidra label FUN_00db2b10, vtable slot [1])
-- Evidence     : refs/script_recovery/new_oakvale_intro/entities/NOVI_AffairWife.json
--
-- The betrayed wife. She idles asking "where's my husband?" once (then rarely) when the hero is near.
-- When talked to she calls him a layabout and, once the hero has discovered the infidelity
-- (quest flag HeroDiscoveredInfidelity raised by NOVI_AffairMan), asks whether the hero saw him:
-- "yes" is a good deed and sends her running to NOVI_AffairMan; "no" gets a plea. On reaching him she
-- stands arguing (pointing animations, numbered "what's this?" lines, his "in trouble" reply).
-- Hitting her at any stage makes her the hero's ally, counts a bad deed and plays a complaint.

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

-- Script names / text keys / animations (native-decompile literals)
local SCRIPT_NAME_MAN = "NOVI_AffairMan"
local ANIM_POINT_AWAY = "ST_ARGUING_POINT_AWAY"
local ANIM_POINT_AT = "ST_ARGUING_POINT_AT"
local TEXT_ON_HIT = "TEXT_QST_048_AFFAIR_WIFE_ON_HIT"
local TEXT_LAYABOUT = "TEXT_QST_048_AFFAIR_WIFE_LAYABOUT"
local TEXT_QUESTION = "TEXT_QST_048_AFFAIR_WIFE_QUESTION"
local TEXT_ANSWER_YES = "TEXT_OBJECT_HERO_ANSWER_YES"
local TEXT_ANSWER_NO = "TEXT_OBJECT_HERO_ANSWER_NO"
local TEXT_THANKYOU = "TEXT_QST_048_AFFAIR_WIFE_THANKYOU"
local TEXT_PLEA = "TEXT_QST_048_AFFAIR_WIFE_PLEA"
local TEXT_WHERES_HUSBAND = "TEXT_QST_048_AFFAIR_WIFE_WHERES_HUSBAND"
local TEXT_RUNNING_TO_HUBBY = "TEXT_QST_048_AFFAIR_WIFE_RUNNING_TO_HUBBY"
local TEXT_THANKYOU_SINGLE = "TEXT_QST_048_AFFAIR_WIFE_THANKYOU_SINGLE"
local TEXT_WHATS_THIS = "TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_"       -- + counter (10, 20, ...)
local TEXT_WHATS_THIS_FALLBACK = "TEXT_QST_048_AFFAIR_WIFE_WHATS_THIS_10"
-- Proven by the installed English text.big index and retail text headers. `_50` is absent, which is
-- the condition that makes retail's TextEntryExists check wrap the counter back to `_10`.
local WHATS_THIS_KEYS = {
    [10] = true, [20] = true, [30] = true, [40] = true,
}
local TEXT_MAN_IN_TROUBLE = "TEXT_QST_048_AFFAIRMAN_IN_TROUBLE"

-- Numeric constants (immediates in the decompile unless noted)
local SPEAK_HEALTH_THRESHOLD = 0.0        -- DAT_0122dedc (.rdata float, read from retail exe): speak only while health > 0
local HERO_ASK_DISTANCE = 5.0             -- "where's my husband" range
local ASK_AGAIN_CHANCE_ONE_IN = 500       -- rand() % 500 == 0 once ForceFirstTimeSpeak is spent
local RUN_TO_HUSBAND_RADIUS = 2.0         -- 0x40000000 = 2.0f passed to MoveToPosition
local HUSBAND_REACHED_DISTANCE = 3.0      -- IsDistanceBetweenThingsUnder(me, man, 3.0)
local RUNNING_LINE_DISTANCE = 10.0        -- farther than this from home -> say the running line
local ARGUE_HERO_DISTANCE = 15.0          -- inference: 0x41700000 (15.0f) leaked into the GetHero() call
local WHATS_THIS_STEP = 10                -- counter += 10 per argument line
local WHATS_THIS_RESET = 10               -- counter reset when the text entry is missing
local BAD_DEED_HIT_WIFE = 2               -- AddBadDeed(PARENT, 2)
local CUTSCENE_CONTROL_PRIORITY = 4       -- StartScriptingEntity(me, res, 4)
local MAIN_CONTROL_PRIORITY = 4           -- inference: argument dropped by the decompiler; sibling calls use 4
local HERO_ABILITY_IGNORED_ON_HIT = 14    -- EHeroAbility 0xe literal (confront-loop hit check)
local SPEAK_SELECTION_METHOD = 0          -- _Speak_ third argument literal 0
local MOVE_RUN = 1                        -- EScriptEntityMoveType literal 1
local YESNO_ANSWER_YES = 1                -- MsgIsQuestionAnsweredYesOrNo() == 1
local TIMER_VALUE = 3                     -- native push 3 at 0x00db3328
local ZERO_POSITION = { x = 0.0, y = 0.0, z = 0.0 }   -- DAT_0143e8e0: used when the husband thing is missing

-- Entity-local retail fields (this+0x1c..0x1e, PDB names)
local GoingForHusband      -- 0x1c
local ForceFirstTimeSpeak  -- 0x1d
local SaidRunningLine      -- 0x1e
local whats_this_counter   -- retail Main stack local (starts 0)
local last_conversation    -- retail stack local holding the last AddNewConversation id (inference)

function Init(quest, me)
    GoingForHusband = false
    ForceFirstTimeSpeak = true
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false)              -- retail passes (me, 0, 0); ForgeFSE binding takes one bool
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false)           -- retail passes (me, 0, 1, 0); ForgeFSE binding takes one bool
    quest:EntitySetAsUseMovementInActions(me, false)
    me:SetIsPushableByHero(false)
    SaidRunningLine = false
    quest:EntitySetDeedReactionsEnabled(me, false)
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

-- Retail: StartMovieSequence("") + PauseAllNonScriptedEntities(true)
local function begin_movie(quest)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
end

-- Retail: PauseAllNonScriptedEntities(false) then the movie/resource objects are destroyed
-- (inference: the CScriptGameResourceObjectMovieBase destructor ends the movie sequence).
local function end_movie(quest, me)
    quest:PauseAllNonScriptedEntities(false)
    NOVI.release(quest, me)
    quest:EndMovieSequence()
end

-- Retail: EntitySetThingAsAllyOfThing twice around GetHero() (both argument lists dropped).
-- inference: (me, hero) then (hero, me) — she stops being a valid target after being hit.
local function become_hero_ally(quest, me)
    local hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, hero)
    quest:EntitySetThingAsAllyOfThing(hero, me)
end

---------------------------------------------------------------------------------------------------
-- phases: idle at home
---------------------------------------------------------------------------------------------------

-- Phase HIT (idle): ally the hero, bad deed, complaint cutscene. Returns false on termination.
local function react_to_hit(quest, me)
    become_hero_ally(quest, me)
    Deeds.add_bad(quest, me, BAD_DEED_HIT_WIFE)
    if not NOVI.acquire(quest, me, CUTSCENE_CONTROL_PRIORITY) then return false end
    begin_movie(quest)
    if not speak_if_alive(quest, me, TEXT_ON_HIT) then end_movie(quest, me); return false end
    end_movie(quest, me)
    return true
end

-- Sub-phase of TALK: "did you see him?" yes/no question (only once HeroDiscoveredInfidelity is set).
local function ask_hero_about_husband(quest, me)
    -- retail GiveHeroYesNoQuestion(question, yes, no, "", 1); the trailing 1 has no ForgeFSE parameter
    quest:GiveHeroYesNoQuestion(TEXT_QUESTION, TEXT_ANSWER_YES, TEXT_ANSWER_NO, "")
    local answer = quest:MsgIsQuestionAnsweredYesOrNo()
    while answer < 0 do
        if not NOVI.frame(quest, me) then return false end
        answer = quest:MsgIsQuestionAnsweredYesOrNo()
    end
    if answer == YESNO_ANSWER_YES then
        if not speak_if_alive(quest, me, TEXT_THANKYOU) then return false end
        Deeds.add_good(quest, me)
        GoingForHusband = true
    else
        if not speak_if_alive(quest, me, TEXT_PLEA) then return false end
    end
    return true
end

-- Phase TALK (idle): layabout complaint, then the question if the hero knows. Returns false on termination.
local function talked_to_by_hero(quest, me)
    begin_movie(quest)                                    -- PauseAllNonScriptedEntities argument dropped; inference: true
    if not NOVI.acquire(quest, me, CUTSCENE_CONTROL_PRIORITY) then end_movie(quest, me); return false end
    if not speak_if_alive(quest, me, TEXT_LAYABOUT) then end_movie(quest, me); return false end
    if F.get(quest, F.HeroDiscoveredInfidelity) then
        if not ask_hero_about_husband(quest, me) then end_movie(quest, me); return false end
    end
    end_movie(quest, me)
    return true
end

-- Phase ASK: timed "where's my husband?" conversation line to a nearby hero.
local function ask_wheres_husband(quest, me)
    local timer = F.get(quest, F.TalkIntermittentTimer)
    if quest:GetTimer(timer) ~= 0 then return end
    if not (ForceFirstTimeSpeak or math.random(0, ASK_AGAIN_CHANCE_ONE_IN - 1) == 0) then return end
    if not NOVI.hero_within(quest, me, HERO_ASK_DISTANCE) then return end
    quest:SetTimer(timer, TIMER_VALUE)
    ForceFirstTimeSpeak = false
    local hero = quest:GetHero()
    local conv = quest:AddNewConversation(me, false, false)   -- retail args dropped by the decompiler
    quest:AddPersonToConversation(conv, hero)
    quest:AddLineToConversation(conv, TEXT_WHERES_HUSBAND, me, hero)
end

---------------------------------------------------------------------------------------------------
-- phases: going for the husband
---------------------------------------------------------------------------------------------------

-- Phase RUN: one MoveToPosition to where the husband stands; returns the husband thing.
local function start_running_to_husband(quest, me)
    local man = quest:GetThingWithScriptName(SCRIPT_NAME_MAN)
    quest:EntitySetAsUseMovementInActions(me, true)      -- retail args dropped; literal 1 nearby (inference: (me, true))
    local target = ZERO_POSITION
    if man then target = man:GetPos() end
    me:MoveToPosition(target, RUN_TO_HUSBAND_RADIUS, MOVE_RUN)
    quest:ClearThingHasInformation(me)
    return man
end

-- Phase RUN wait loop: say the running line once she is 10 from home; wait until within 3 of him.
local function wait_until_husband_reached(quest, me, man)
    while not quest:IsDistanceBetweenThingsUnder(me, man, HUSBAND_REACHED_DISTANCE) do
        if not SaidRunningLine and NOVI.distance_from_thing_to_position_over(me, me:GetHomePos(), RUNNING_LINE_DISTANCE) then
            local conv = quest:AddNewConversation(me, false, false)   -- retail AddNewConversation(me, 0)
            quest:AddLineToConversation(conv, TEXT_RUNNING_TO_HUBBY, me, nil)   -- retail listener is a null CScriptThing
            SaidRunningLine = true
        end
        if not NOVI.frame(quest, me) then return false end
    end
    return true
end

-- Sub-phase of ARGUE: hit while arguing.
local function argue_react_to_hit(quest, me)
    become_hero_ally(quest, me)
    Deeds.add_bad(quest, me, BAD_DEED_HIT_WIFE)
    if not NOVI.acquire(quest, me, CUTSCENE_CONTROL_PRIORITY) then return false end
    begin_movie(quest)
    if not speak_if_alive(quest, me, TEXT_ON_HIT) then end_movie(quest, me); return false end
    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero())   -- retail (me, <thing>, 1); inference: the hero
    end_movie(quest, me)
    return true
end

-- Sub-phase of ARGUE: talked to while arguing.
local function argue_talked_to(quest, me)
    if not NOVI.acquire(quest, me, CUTSCENE_CONTROL_PRIORITY) then return false end   -- retail priority dropped
    me:ClearAllActions()
    me:ClearCommands()
    begin_movie(quest)
    if not speak_if_alive(quest, me, TEXT_THANKYOU_SINGLE) then end_movie(quest, me); return false end
    quest:EntitySetFacingAngleTowardsThing(me, quest:GetHero())   -- retail (me, <thing>, 1); inference: the hero
    end_movie(quest, me)
    return true
end

-- Sub-phase of ARGUE: the numbered "what's this?" line, sometimes answered by the husband.
local function argue_conversation(quest, me, man)
    local conv = quest:AddNewConversation(me, false, false)       -- retail AddNewConversation(me, 0)
    last_conversation = conv
    quest:AddPersonToConversation(conv, man)                      -- retail person dropped; inference: the husband
    whats_this_counter = whats_this_counter + WHATS_THIS_STEP
    local key = TEXT_WHATS_THIS .. tostring(whats_this_counter)
    local exists = WHATS_THIS_KEYS[whats_this_counter] == true
    if not exists then
        whats_this_counter = WHATS_THIS_RESET
        key = TEXT_WHATS_THIS_FALLBACK
    end
    quest:AddLineToConversation(conv, key, me, nil)               -- retail listener argument dropped
    if math.random(0, 1) == 0 then                                -- rand() & 1 == 0
        quest:AddLineToConversation(conv, TEXT_MAN_IN_TROUBLE, man, nil)   -- retail listener argument dropped
    end
end

-- Phase ARGUE: stands by the husband pointing and scolding while the hero is within 15. Never exits normally.
local function argue_with_husband(quest, me, man)
    me:ClearCommands()
    quest:EntitySetAsUseMovementInActions(me, false)
    while true do
        if NOVI.hero_within(quest, me, ARGUE_HERO_DISTANCE) then
            if math.random(0, 1) == 0 then                        -- rand() & 1 == 0
                me:PlayAnimation(ANIM_POINT_AWAY)                 -- retail flags (0,0,0,1,DAT_01375748=true,0)
            else
                me:PlayAnimation(ANIM_POINT_AT)                   -- retail flags (0,0,0,1,DAT_01375748=true,0)
            end
            while me:IsPerformingScriptTask() do
                if not NOVI.frame(quest, me) then return false end
                quest:EntitySetFacingAngleTowardsThing(me, man)
                if hit_by_hero(me) then
                    if not argue_react_to_hit(quest, me) then return false end
                end
                if me:IsTalkedToByHero() then
                    if not argue_talked_to(quest, me) then return false end
                end
                if not (last_conversation and quest:IsConversationActive(last_conversation)) then   -- retail arg dropped
                    argue_conversation(quest, me, man)
                end
            end
        end
        if not NOVI.frame(quest, me) then return false end
    end
end

---------------------------------------------------------------------------------------------------
-- Main
---------------------------------------------------------------------------------------------------

function Main(quest, me)
    if not NOVI.frame(quest, me) then return end
    whats_this_counter = 0
    local man

    -- retail: the main scripted resource is only released by its destructor when Main returns
    while true do
        -- retail re-runs the StartScriptingEntity wait loop at the top of every idle iteration
        if not NOVI.acquire(quest, me, MAIN_CONTROL_PRIORITY) then NOVI.release(quest, me); return end
        if GoingForHusband then
            man = start_running_to_husband(quest, me)
            break
        end
        if hit_by_hero(me) then
            if not react_to_hit(quest, me) then NOVI.release(quest, me); return end
        elseif me:IsTalkedToByHero() then
            if not talked_to_by_hero(quest, me) then NOVI.release(quest, me); return end
        end
        ask_wheres_husband(quest, me)
        if not NOVI.frame(quest, me) then NOVI.release(quest, me); return end
    end

    if not wait_until_husband_reached(quest, me, man) then NOVI.release(quest, me); return end
    argue_with_husband(quest, me, man)   -- only returns on termination
    NOVI.release(quest, me)
end
