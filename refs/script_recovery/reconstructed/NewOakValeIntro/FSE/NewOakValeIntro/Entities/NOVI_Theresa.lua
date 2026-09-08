-- NOVI_Theresa -- the Hero's sister Theresa during the Oakvale prologue (Q_NewOakValeIntro).
-- Retail class : NScript::CQ_NewOakValeIntroScript::CNOVI_Theresa
-- Retail Init  : 0x00DAC4F0 (Ghidra label is the wrong donor "CV_SickChildScript::CTalkingTrader1::Init";
--                the CNOVI_Theresa vtable at 0x012D83A4 slot [2] proves ownership)
-- Retail Main  : 0x00DB97A0   (Fable.exe, ImageBase 0x400000)
-- Evidence     : reconstructed-source (Ghidra decompile + retail disassembly + PDB names;
--                see refs/script_recovery/new_oakvale_intro/entities/NOVI_Theresa.json)
--
-- What she does: skips ("SKIP" combat animation) until the Hero comes within 5m, plays
-- CS_OAKVALE_INTRO_THERESA_MEET (with an immediate yes/no chocolate hand-over if the Hero already
-- carries the box), then loops: accepts the chocolate box when presented / offered, nags for it when
-- talked to, complains about other gifts, protests when hit (bad deed 2), and once she has the box
-- waits for the Hero to reach marker M_TriggerOutro to play CS_OAKVALE_INTRO_THERESA, the raid AVI
-- and set AttackOver.

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

-- Retail literals (decompile / disassembly immediates).
local SCRIPT_PRIORITY = 4                -- EScriptAIPriority passed to StartScriptingEntity (push 4)
local SPEAK_METHOD = 0                   -- ETextGroupSelectionMethod GROUP_SELECT_FIRST (push 0)
local DEAD_HEALTH = 0.0                  -- DAT_0122dedc; speak only while GetHealth(me) > 0.0
local GREET_DISTANCE = 5.0               -- IsDistanceBetweenThingsUnder(me, hero, 5.0f) ends the skipping
local OUTRO_TRIGGER_DISTANCE = 2.0       -- IsDistanceBetweenThingsUnder(hero, M_TriggerOutro, 2.0f)
local ANSWER_YES = 1                     -- MsgIsQuestionAnsweredYesOrNo() == 1
local BAD_DEED_HIT_THERESA = 2           -- AddBadDeed(PARENT, 2)
local HEAL_LIFE_ABILITY = 14             -- EHeroAbility 0xE excluded from the "hit by special ability" check
local FADE_SECONDS = 0.5                 -- FadeScreenOut(0.5f, 0.0f, CRGBColour(0,0,0,255))
local FADE_HOLD_SECONDS = 0.0
local MUSIC_SET_RAID = 25                -- OverrideMusic(EMusicSetType 0x19, false, false)

local ANIM_SKIP = "SKIP"
local MARKER_TRIGGER_OUTRO = "M_TriggerOutro"
local SCRIPT_NAME_GUARD = "NOVI_Guard"
local OBJECT_CHOCOLATE_BOX = "OBJECT_CHOCOLATE_BOX_UNGIVEABLE"
local CUTSCENE_MEET = "CS_OAKVALE_INTRO_THERESA_MEET"
local CUTSCENE_MEET_YES = "CS_OAKVALE_INTRO_THERESA_MEET_YES"
local CUTSCENE_OUTRO = "CS_OAKVALE_INTRO_THERESA"
local RAID_MOVIE = "Data\\Video\\1_raid_on_oak_vale_comp.xmv"
local ACTOR_HERO = "HERO"
local ACTOR_THERESA_MEET = "THER"
local ACTOR_THERESA_OUTRO = "Theresa"

local TEXT_GIVE_CHOCOLATE_BOX = "TEXT_QST_048_GIVE_CHOCOLATE_BOX"
local TEXT_ANSWER_YES = "TEXT_OBJECT_HERO_ANSWER_YES"
local TEXT_ANSWER_NO = "TEXT_OBJECT_HERO_ANSWER_NO"
local TEXT_HELLO = "TEXT_QST_048_THERESA_HELLO"
local TEXT_GET_PRESENT = "TEXT_QST_048_THERESA_GET_PRESENT"
local TEXT_REALLY_GET_PRESENT = "TEXT_QST_048_THERESA_REALLY_GET_PRESENT"
local TEXT_BETTER_PRESENT = "TEXT_QST_048_THERESA_BETTER_PRESENT"
local TEXT_DONT_HIT = "TEXT_QST_048_THERESA_DONT_HIT"
local TEXT_OBJECTIVE_05 = "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_05"

-- Entity-local retail fields.
local spoken_already = false   -- this+0x1c: "get me a present" line already said once
local DoneIntro = false        -- this+0x1d: meet cutscene played

function Init(quest, me)
    DoneIntro = false
    spoken_already = false
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false)                 -- retail passes a trailing (false); ForgeFSE hardcodes true
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false)              -- retail passes trailing (true, false); ForgeFSE hardcodes (false, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
end

-- Retail ends every movie block (normal and terminate paths) with PauseAllNonScriptedEntities(false)
-- followed by the CScriptGameResourceObjectMovieBase destructor (= EndMovieSequence).
local function end_movie(quest)
    quest:PauseAllNonScriptedEntities(false)
    quest:EndMovieSequence()
end

-- Retail: if (GetHealth(me) > 0.0f) { Speak(hero, key, 0, false, true, false); wait IsPerformingScriptTask }.
-- ForgeFSE me:Speak(target, key, method) is blocking with the same (false, true, false) trailing bools,
-- so the per-frame wait (and its terminate check) is host-managed inside the binding.
local function speak_if_alive(quest, me, key)
    if quest:GetHealth(me) > DEAD_HEALTH then
        me:Speak(quest:GetHero(), key, SPEAK_METHOD)
    end
end

local function hero_has_chocolates(quest)
    return quest:IsObjectInThingsPossession(OBJECT_CHOCOLATE_BOX, quest:GetHero())
end

-- Retail: MsgIsHitBy(HERO) || (MsgIsHitByAnySpecialAbilityFrom(HERO) && !MsgIsHitBySpecialAbilityFrom(0xE, HERO)).
local function hero_attacked_me(me)
    if me:MsgIsHitByHero() then
        return true
    end
    return me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(HEAL_LIFE_ABILITY)
end

-- Retail CScriptThing::MsgIsPresentedWithItem(CCharString& itemName); ForgeFSE returns the bool and
-- publishes the item def name in the global g_PresentedItemName.
local function presented_item(me)
    if me:MsgIsPresentedWithItem() then
        return true, g_PresentedItemName
    end
    return false, nil
end

-- Retail KillAllThingsInVector(GSI, GetAllThingsWithScriptName("NOVI_Guard"), false):
-- RemoveThing(guard, false, true) for every guard that IsAlive() (ForgeFSE RemoveThing passes the same bools).
local function remove_guards(quest)
    local guards = quest:GetAllThingsWithScriptName(SCRIPT_NAME_GUARD)
    for _, guard in ipairs(guards) do
        if guard:IsAlive() then
            quest:RemoveThing(guard)
        end
    end
end

-- Shared tail of every "chocolates accepted" path.
local function accept_chocolates(quest, me)
    F.set(quest, F.GivenTheresaChocs, true)
    quest:TakeObjectFromHero(OBJECT_CHOCOLATE_BOX)
    quest:SetQuestCardObjective(quest:GetActiveQuestName(), TEXT_OBJECTIVE_05, "", "")
    quest:ClearThingHasInformation(me)
end

local function run_cutscene(quest, name, actors)
    -- Retail RunCutscene(name, actors, flags=null, null, setupCond=false, skippable=true); the movie
    -- sequence / pause / camera fix are issued explicitly by the caller as retail does.
    quest:RunCutsceneWithSetup(name, actors, nil, false, true, false)
end

-- Phase 1: skip in place until the Hero is within 5m.
local function skip_until_hero_near(quest, me)
    while true do
        -- Retail PlayCombatAnimation("SKIP", true, false, false, true, false, false) (non-blocking).
        me:PlayCombatAnimation(ANIM_SKIP)
        while not NOVI.hero_within(quest, me, GREET_DISTANCE) and me:IsPerformingScriptTask() do
            if not NOVI.frame(quest, me) then
                return false
            end
        end
        if NOVI.hero_within(quest, me, GREET_DISTANCE) then
            return true
        end
        if not NOVI.frame(quest, me) then
            return false
        end
    end
end

-- Phase 2: meet cutscene (+ optional immediate chocolate hand-over). Returns ok, chocolates_taken.
local function meet_hero(quest, me)
    if not skip_until_hero_near(quest, me) then
        return false, false
    end
    local hero = quest:GetHero()
    -- Retail acquires the Hero once (StartScriptingEntity, priority 4, no wait loop); ForgeFSE
    -- RunCutsceneWithSetup acquires its actors itself (host-managed).
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    run_cutscene(quest, CUTSCENE_MEET, { [ACTOR_HERO] = hero, [ACTOR_THERESA_MEET] = me })

    local taken = false
    if hero_has_chocolates(quest) then
        -- Retail GiveHeroYesNoQuestion(q, yes, no, "", true) + MsgIsQuestionAnsweredYesOrNo() frame-wait;
        -- the ForgeFSE binding blocks and returns the answer (host-managed wait).
        local answer = quest:GiveHeroYesNoQuestion(TEXT_GIVE_CHOCOLATE_BOX, TEXT_ANSWER_YES, TEXT_ANSWER_NO, "")
        if answer == ANSWER_YES then
            remove_guards(quest)
            run_cutscene(quest, CUTSCENE_MEET_YES, { [ACTOR_HERO] = hero, [ACTOR_THERESA_MEET] = me })
            accept_chocolates(quest, me)
            taken = true
        end
    end

    quest:FixMovieSequenceCamera(false)
    DoneIntro = true
    end_movie(quest)
    return true, taken
end

-- Phase 3a: Hero talked to her while carrying the box -> yes/no offer. Returns chocolates_taken.
local function offer_chocolates(quest, me)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    local taken = false
    local answer = quest:GiveHeroYesNoQuestion(TEXT_GIVE_CHOCOLATE_BOX, TEXT_ANSWER_YES, TEXT_ANSWER_NO, "")
    if answer == ANSWER_YES then
        remove_guards(quest)
        speak_if_alive(quest, me, TEXT_HELLO)
        accept_chocolates(quest, me)
        taken = true
    else
        speak_if_alive(quest, me, TEXT_REALLY_GET_PRESENT)
    end
    end_movie(quest)
    return taken
end

-- Phase 3b: the box was presented (gift) to her.
local function accept_presented_chocolates(quest, me)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    remove_guards(quest)
    speak_if_alive(quest, me, TEXT_HELLO)
    accept_chocolates(quest, me)
    end_movie(quest)
end

-- Phase 3c: something other than the box was presented.
local function reject_present(quest, me)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    speak_if_alive(quest, me, TEXT_BETTER_PRESENT)
    end_movie(quest)
end

-- Phase 3d: talked to without the box.
local function nag_for_present(quest, me, given_chocs)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    if not given_chocs then
        if not spoken_already then
            spoken_already = true
            speak_if_alive(quest, me, TEXT_GET_PRESENT)
        else
            speak_if_alive(quest, me, TEXT_REALLY_GET_PRESENT)
        end
    else
        speak_if_alive(quest, me, TEXT_HELLO)
    end
    end_movie(quest)
end

-- Phase 3e: the Hero hit her (weapon or non-heal spell).
local function react_to_attack(quest, me)
    local hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, hero)
    quest:EntitySetThingAsAllyOfThing(hero, me)
    Deeds.add_bad(quest, me, BAD_DEED_HIT_THERESA)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    speak_if_alive(quest, me, TEXT_DONT_HIT)
    end_movie(quest)
end

-- Phase 4: Hero reached M_TriggerOutro after handing over the box. Main ends after this.
local function play_outro(quest, me)
    local hero = quest:GetHero()
    quest:DisplayQuestInfo(false)
    quest:RemoveQuestInfoElement(F.get(quest, F.GUIBullyHealthCounter))
    quest:RemoveQuestInfoElement(F.get(quest, F.GUIGoodDeedCounter))
    quest:RemoveQuestInfoElement(F.get(quest, F.GUIBarrelCounter))
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    -- Retail acquires the Hero once (priority 4) here; no FixMovieSequenceCamera around this cutscene.
    run_cutscene(quest, CUTSCENE_OUTRO, { [ACTOR_HERO] = hero, [ACTOR_THERESA_OUTRO] = me })
    quest:PlayAVIMovie(RAID_MOVIE)
    quest:FadeScreenOut(FADE_SECONDS, FADE_HOLD_SECONDS)     -- retail colour arg CRGBColour(0,0,0,255) is hardcoded black in ForgeFSE
    quest:OverrideMusic(MUSIC_SET_RAID, false, false)
    F.set(quest, F.AttackOver, true)
    end_movie(quest)
end

-- Per-frame dispatch once the meet cutscene has played. Returns chocolates_taken_this_frame.
local function handle_frame(quest, me, given_chocs)
    local talked_with_chocs = me:IsTalkedToByHero() and hero_has_chocolates(quest)
    if talked_with_chocs then
        return offer_chocolates(quest, me)
    end

    local presented, item = presented_item(me)
    if presented and item == OBJECT_CHOCOLATE_BOX then
        accept_presented_chocolates(quest, me)
        return true
    end
    presented, item = presented_item(me)          -- retail re-queries the message here
    if presented and item ~= OBJECT_CHOCOLATE_BOX then
        reject_present(quest, me)
        return false
    end

    if me:IsTalkedToByHero() then
        nag_for_present(quest, me, given_chocs)
        return false
    end
    if hero_attacked_me(me) then
        react_to_attack(quest, me)
        return false
    end
    -- Idle: keep skipping until the box has been handed over.
    if not given_chocs and not me:IsPerformingScriptTask() then
        me:PlayCombatAnimation(ANIM_SKIP)
    end
    return false
end

local function run(quest, me)
    if not NOVI.frame(quest, me) then
        return
    end
    local outro_marker = quest:GetThingWithScriptName(MARKER_TRIGGER_OUTRO)
    -- Retail stack local (NOT persisted): true once the box was handed over during this run of Main.
    local given_chocs = false
    while true do
        if NOVI.acquire(quest, me, SCRIPT_PRIORITY) == false then
            return
        end
        if not DoneIntro then
            local ok, taken = meet_hero(quest, me)
            if not ok then
                return
            end
            if taken then
                given_chocs = true
            end
        else
            if handle_frame(quest, me, given_chocs) then
                given_chocs = true
            end
            if given_chocs and quest:IsDistanceBetweenThingsUnder(quest:GetHero(), outro_marker, OUTRO_TRIGGER_DISTANCE) then
                play_outro(quest, me)
                return
            end
        end
        if not NOVI.frame(quest, me) then
            return
        end
    end
end

function Main(quest, me)
    run(quest, me)
    -- Retail releases the scripted-thing resource and the marker handle on every exit path of Main.
    NOVI.release(quest, me)
end
