-- NOVI_LiveFather -- the Hero's father during the Oakvale prologue (Q_NewOakValeIntro).
-- Retail class : NScript::CQ_NewOakValeIntroScript::CNOVI_LiveFather
-- Retail Init  : 0x00DAC390   Retail Main : 0x00DB86B0   (Fable.exe, ImageBase 0x400000)
-- Evidence     : reconstructed-source (Ghidra decompile + retail disassembly + PDB names;
--                see refs/script_recovery/new_oakvale_intro/entities/NOVI_LiveFather.json)
--
-- What he does: plays the CS_OAKVALE_INTRO_FATHER cutscene once, shows the "highlighting"
-- game-info card and the good-deed HUD counter, then waits forever. Each frame he re-acquires
-- scripted control of himself, chats when the Hero talks to him (paying one gold per new good
-- deed, asking for more deeds, or scolding bad deeds) and loses his temper when the Hero hits
-- him (registers bad deed 2).

local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

-- Retail literals (decompile / disassembly immediates).
local SCRIPT_PRIORITY = 4                -- EScriptAIPriority passed to StartScriptingEntity (push 4)
local SPEAK_METHOD = 0                   -- ETextGroupSelectionMethod GROUP_SELECT_FIRST (push 0)
local DEAD_HEALTH = 0.0                  -- DAT_0122dedc; speak only while GetHealth(me) > 0.0
local INTRO_PAUSE_SECONDS = 1.0          -- Pause(1.0f) after the cutscene
local CAMERA_RESET_SECONDS = 0.0         -- CameraResetToViewBehindHero(0)
local DEED_COUNTER_START = 0             -- AddQuestInfoCounter count
local DEED_COUNTER_SCALE = 1.0           -- AddQuestInfoCounter scale (1.0f)
local REWARD_GOLD_THRESHOLD = 3          -- "you have enough" when GetHeroGold() > 3
local BAD_DEED_HIT_DAD = 2               -- AddBadDeed(PARENT, 2)
local HEAL_LIFE_ABILITY = 14             -- EHeroAbility 0xE excluded from the "hit by special ability" check

local CUTSCENE_INTRO = "CS_OAKVALE_INTRO_FATHER"
local ACTOR_HERO = "Hero"
local ACTOR_FATHER = "Father"
local OBJECT_CHOCOLATE_BOX = "OBJECT_CHOCOLATE_BOX_UNGIVEABLE"
local HUD_DEED_GOOD_ICON = "HUD_DEED_GOOD_ICON"

local TEXT_INSTRUCTION_HIGHLIGHTING = "TEXT_QST_048_INSTRUCTION_HIGHLIGHTING"       -- IsXbox() == true
local TEXT_INSTRUCTION_HIGHLIGHTING_PC = "TEXT_QST_048_INSTRUCTION_HIGHLIGHTING_PC" -- IsXbox() == false
local TEXT_DONE_NOTHING_YET = "TEXT_QST_048_DAD_DONE_NOTHING_YET"
local TEXT_DO_MORE = "TEXT_QST_048_DAD_DO_MORE"
local TEXT_GIVE_PRESENT_ALT = "TEXT_QST_048_DAD_GIVE_PRESENT_ALT"
local TEXT_ANTISOCIAL = "TEXT_QST_048_DAD_ANTISOCIAL"
local TEXT_GIVE_REWARD_JUST_GOOD = "TEXT_QST_048_DAD_GIVE_REWARD_JUST_GOOD"
local TEXT_GIVE_REWARD_PART_BAD = "TEXT_QST_048_DAD_GIVE_REWARD_PART_BAD"
local TEXT_GIVE_PRESENT = "TEXT_QST_048_DAD_GIVE_PRESENT"
local TEXT_YOU_HAVE_ENOUGH = "TEXT_QST_048_DAD_YOU_HAVE_ENOUGH"
local TEXT_IS_ENOUGH = "TEXT_QST_048_DAD_IS_ENOUGH"
local TEXT_TEMPER = "TEXT_QST_048_DAD_TEMPER"
local TEXT_OBJECTIVE_01 = "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01"

-- Entity-local retail field (this+0x1c, long): gold already paid out for good deeds.
local PenniesGiven = 0

function Init(quest, me)
    PenniesGiven = 0
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false)                 -- retail passes a trailing (false); ForgeFSE hardcodes true
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false)              -- retail passes trailing (true, false); ForgeFSE hardcodes (false, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetDeedReactionsEnabled(me, false)
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

-- Phase 1: one-off intro cutscene + tutorial card + HUD counter (runs while DadFinishedIntro is false).
local function intro_cutscene(quest, me)
    local hero = quest:GetHero()
    -- Retail acquires the Hero (StartScriptingEntity, priority 4, frame-wait loop) before the movie;
    -- ForgeFSE RunCutsceneWithSetup acquires its actors itself (host-managed).
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    -- Retail RunCutscene(name, actors, flags=null, null, setupCond=false, skippable=true).
    quest:RunCutsceneWithSetup(CUTSCENE_INTRO, { [ACTOR_HERO] = hero, [ACTOR_FATHER] = me }, nil, false, true, false)
    quest:FixMovieSequenceCamera(false)
    quest:SetAllSoundsAsMuted(false)
    F.set(quest, F.DadFinishedIntro, true)
    quest:Pause(INTRO_PAUSE_SECONDS)
    quest:CameraResetToViewBehindHero(CAMERA_RESET_SECONDS)
    quest:CameraDefault()

    -- Retail branches on CGameScriptInterface::IsXbox(); ForgeFSE has no binding (returns nil -> PC text).
    local on_xbox = NOVI.is_xbox()
    local info_key = TEXT_INSTRUCTION_HIGHLIGHTING_PC
    if on_xbox then
        info_key = TEXT_INSTRUCTION_HIGHLIGHTING
    end
    quest:DisplayGameInfo(info_key)
    while not quest:MsgIsGameInfoClickedPast() do
        if not NOVI.frame(quest, me) then
            end_movie(quest)
            return false
        end
    end

    F.set(quest, F.GUIGoodDeedCounter, quest:AddQuestInfoCounter(HUD_DEED_GOOD_ICON, DEED_COUNTER_START, DEED_COUNTER_SCALE))
    quest:DisplayQuestInfo(true)
    end_movie(quest)
    return true
end

-- Phase 2a: the Hero talked to Dad. Pays out new good deeds, nags, or scolds.
local function talk_to_hero(quest, me)
    if NOVI.acquire(quest, me, SCRIPT_PRIORITY) == false then   -- retail re-acquires before chatting
        return false
    end
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)

    local good_deeds = F.get(quest, F.GoodDeedsPerformed)
    local bad_deeds = F.get(quest, F.BadDeedsPerformed)
    if good_deeds == 0 and bad_deeds == 0 then
        speak_if_alive(quest, me, TEXT_DONE_NOTHING_YET)
    elseif good_deeds <= PenniesGiven then
        if bad_deeds < 1 then
            speak_if_alive(quest, me, TEXT_DO_MORE)
            if hero_has_chocolates(quest) then
                quest:ClearThingHasInformation(me)
                speak_if_alive(quest, me, TEXT_GIVE_PRESENT_ALT)
            end
        else
            speak_if_alive(quest, me, TEXT_ANTISOCIAL)
        end
    else
        local reward = good_deeds - PenniesGiven          -- one gold per unpaid good deed
        PenniesGiven = PenniesGiven + reward
        quest:GiveHeroGold(reward)
        if bad_deeds == 0 then
            speak_if_alive(quest, me, TEXT_GIVE_REWARD_JUST_GOOD)
        else
            speak_if_alive(quest, me, TEXT_GIVE_REWARD_PART_BAD)
        end
        if hero_has_chocolates(quest) then
            speak_if_alive(quest, me, TEXT_GIVE_PRESENT)
            quest:ClearThingHasInformation(me)
        elseif quest:GetHeroGold() > REWARD_GOLD_THRESHOLD then
            speak_if_alive(quest, me, TEXT_YOU_HAVE_ENOUGH)
            quest:ClearThingHasInformation(me)
        else
            speak_if_alive(quest, me, TEXT_IS_ENOUGH)
            quest:SetQuestCardObjective(quest:GetActiveQuestName(), TEXT_OBJECTIVE_01, "", "")
        end
    end

    end_movie(quest)
    return true
end

-- Phase 2b: the Hero hit Dad (weapon or non-heal spell).
local function react_to_attack(quest, me)
    local hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, hero)
    quest:EntitySetThingAsAllyOfThing(hero, me)
    Deeds.add_bad(quest, me, BAD_DEED_HIT_DAD)
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    if NOVI.acquire(quest, me, SCRIPT_PRIORITY) == false then
        end_movie(quest)
        return false
    end
    speak_if_alive(quest, me, TEXT_TEMPER)
    end_movie(quest)
    return true
end

local function run(quest, me)
    if not NOVI.frame(quest, me) then
        return
    end
    if NOVI.acquire(quest, me, SCRIPT_PRIORITY) == false then
        return
    end
    if not F.get(quest, F.DadFinishedIntro) then
        if not intro_cutscene(quest, me) then
            return
        end
    end
    -- Retail main loop: re-acquire control every frame, then poll talk / hit messages.
    while true do
        if NOVI.acquire(quest, me, SCRIPT_PRIORITY) == false then
            return
        end
        if me:IsTalkedToByHero() then
            if not talk_to_hero(quest, me) then
                return
            end
        end
        if hero_attacked_me(me) then
            if not react_to_attack(quest, me) then
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
    -- Retail releases the scripted-thing resource (destructor) on every exit path of Main.
    NOVI.release(quest, me)
end
