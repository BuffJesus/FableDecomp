local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

--[[
  NOVI_BarrelMan  (retail NScript::CQ_NewOakValeIntroScript::CNOVI_BarrelMan)
  Init 0x00DB5260   Main 0x00DB5330   (Init/OnPredicateFail otherwise default)
  Evidence level: reconstructed-source (Ghidra decompile + retail disassembly of Fable.exe).

  The warehouse man in the Oakvale prologue. He idles by his barrels; when the hero talks to him
  (or walks within TALK_RANGE) he asks the hero to watch the warehouse, the screen fades and he is
  teleported away. After the quest WatchTimer counts down he is placed at a walk-off marker, walks
  back to his start point and judges the hero: "thanks" (good deed) if he can see the hero or the
  hero is within 10 m, "where did you go" (bad deed 1) otherwise. Hitting him is bad deed 2. Barrel
  breakage itself is reported by NOVI_Barrel via BarrelBrokenPersistent, which he only quotes in
  phase-DONE follow-up dialogue; the automatic judgment has no barrel-flag test.
]]

-- Retail constants (immediates in the decompile; float literals verified in Fable.exe .rdata)
local BRAIN_PASSIVE            = "BRAIN_PASSIVE_OVERRIDE"
local BRAIN_AFTER_RETURN       = "BRAIN_GOOD_VILLAGER_BASE"
local SIGHT_RADIUS             = 10.0     -- Init: EntitySetSightRadius(me, 0x41200000)
local WANDER_MIN_DISTANCE      = 0.0
local WANDER_MAX_DISTANCE      = 1.0      -- DAT_3f800000
local SCRIPTING_STATE_GROUP    = 4
local SCRIPT_PRIORITY          = 4        -- every StartScriptingEntity(me, res, 4)
local WATCH_TIMER_START        = 45       -- SetTimer(WatchTimer, 0x2d)
local WATCH_TIMER_RETURN_AT    = 15       -- phase AWAY waits for GetTimer(WatchTimer) == 0xf
local ARRIVE_TOLERANCE         = 2.0      -- IsDistanceFromThingToPositionOver(me, target, 2.0)
local MOVE_RADIUS_EXACT        = 0.0
local MOVE_TYPE                = 1        -- ENTITY_MOVE_RUN
local APPROACH_HERO_DISTANCE   = 4.0
local APPROACH_HERO_RADIUS     = 2.0
local APPROACH_STEP_SECONDS    = 2        -- SetTimer(stepTimer, 2) between MoveToPosition re-issues
local TALK_RANGE               = 10.0     -- DAT_013ac858 (read from .rdata: 10.0f)
local SEE_RANGE                = 10.0     -- phase JUDGE_HERO: IsDistanceBetweenThingsUnder(me, hero, 10.0)
local OVERHEAR_RANGE           = 15.0
local OVERHEAR_REPEAT_MODULUS  = 100      -- DAT_013ac854 (read from .rdata: 100); rand() % 100 == 0
local FADE_OUT_SECONDS         = 1.0
local FADE_HOLD_SECONDS        = 1.0
local FADE_PAUSE_SECONDS       = 2.0
local HEALTH_DEAD              = 0.0      -- _DAT_0122dedc; speech only while GetHealth(me) > 0
local CUTSCENE_BEHAVIOUR_ON    = 1        -- ECutsceneBehaviour 1 while walking back
local CUTSCENE_BEHAVIOUR_OFF   = 2        -- ECutsceneBehaviour 2 once arrived
local SPEECH_SELECTION_METHOD  = 0        -- ETextGroupSelectionMethod 0 in every retail Speak
local IGNORED_HIT_ABILITY      = 14       -- HERO_ABILITY_HEAL_LIFE_SPELL: this ability alone does not count as a hit
local ZERO_POSITION            = { x = 0.0, y = 0.0, z = 0.0 } -- retail DAT_0143e8e0 null-thing fallback

local MARKER_MAN_START         = "M_WHouse_ManStart"
local MARKER_GUARD_POINT       = "M_WHouse_GuardPoint"
local MARKER_WALK_OFF          = "M_BarrelManWalkOff"
local MARKER_WALK_OFF_ALT      = "M_BarrelManWalkOffAlt"
local MARKER_HIDDEN_POS        = "M_BarrelManHiddenPos"

local TEXT_THANKS              = "TEXT_QST_048_BARRELMAN_THANKS"
local TEXT_WHERE_GONE          = "TEXT_QST_048_SCRMSG_BARRELMAN_WHERE_GONE"
local TEXT_LEFT_UNATTENDED     = "TEXT_QST_048_INSTRUCTION_LEFT_WAREHOUSE_UNATTENDED"
local TEXT_CAREFUL             = "TEXT_QST_048_BARRELMAN_CAREFUL"
local TEXT_FAVOUR              = "TEXT_QST_048_BARRELMAN_FAVOUR"
local TEXT_NOT_LARKING         = "TEXT_QST_048_BARRELMAN_NOT_LARKING"
local TEXT_NO_TIME             = "TEXT_QST_048_BARRELMAN_NO_TIME"
local TEXT_LETDOWN_BROKEN      = "TEXT_QST_048_BARRELMAN_LETDOWN_BROKEN"
local TEXT_LETDOWN_NOT_BROKE   = "TEXT_QST_048_BARRELMAN_LETDOWN_NOT_BROKE"
local TEXT_OVERHEAR            = "TEXT_QST_048_BARRELMAN_OVERHEAR"

-- EMyPhase (this+0x20). Values are the retail writes/compares in Main; the names are inference
-- from what each case does. Value 1 is dead/unreachable from a clean lifecycle: donor construction
-- does not set it, Init sets 0, and retail Main's complete write set is {2, 3, 4, 5}.
local PHASE = {
    AT_WAREHOUSE = 0,   -- Init; idle by the barrels, waiting for the hero
    WALKING_OFF  = 1,   -- preserved dead case: walk on foot to M_BarrelManWalkOff
    AWAY         = 2,   -- hidden; waiting for WatchTimer to reach WATCH_TIMER_RETURN_AT
    RETURNING    = 3,   -- placed at a walk-off marker, walking back to M_WHouse_ManStart
    JUDGE_HERO   = 4,   -- arrived: thank the hero or complain that he left
    DONE         = 5,   -- post-return chatter only
}

-- Entity-local retail fields (this+0x1c..0x20), initialised in Init
local ComplainedAboutStock = false               -- 0x1c: written in Init only, never read by Main (retail)
local HeroLetMeDown = false                       -- 0x1d
local OverheardYet = false                        -- 0x1e
local MyPhase = PHASE.AT_WAREHOUSE                -- 0x20 (EMyPhase)

function Init(quest, me)
    quest:SetTimer(F.get(quest, F.WatchTimer), 0)
    quest:Log("NOVI_PROBE BarrelMan Init watchTimer=" .. tostring(F.get(quest, F.WatchTimer)))
    ComplainedAboutStock = false
    MyPhase = PHASE.AT_WAREHOUSE
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, true, false)
    F.set(quest, F.WarehouseMeetPoint, me:GetHomePos())  -- CScriptThing vtable +0x1c = GetHomePos
    HeroLetMeDown = false
    OverheardYet = false
    quest:EntitySetSightRadius(me, SIGHT_RADIUS)
end

local function is_alive(quest, me)
    return quest:GetHealth(me) > HEALTH_DEAD
end

-- Retail: Speak(hero, key, 0, false, true, false) + IsPerformingScriptTask frame loop; ForgeFSE's
-- me:Speak is blocking (host-managed wait), so the per-frame terminate checks live in the host.
local function speak_to_hero(quest, me, key)
    if is_alive(quest, me) then
        return me:Speak(quest:GetHero(), key, SPEECH_SELECTION_METHOD) ~= false
    end
    return true
end

local function abort(quest, me)
    NOVI.release(quest, me)
    return false
end

-- Walk (re-issuing MoveToPosition until within ARRIVE_TOLERANCE) exactly as retail does in phases 1/3.
local function walk_to(quest, me, target)
    while NOVI.distance_from_thing_to_position_over(me, target, ARRIVE_TOLERANCE) do
        if not NOVI.frame(quest, me) then return false end
        me:MoveToPosition(target, MOVE_RADIUS_EXACT, MOVE_TYPE, false, false)
        while me:IsPerformingScriptTask() do
            if not NOVI.frame(quest, me) then return false end
        end
    end
    return true
end

-- case 1
local function walk_off(quest, me)
    quest:SetTimer(F.get(quest, F.WatchTimer), WATCH_TIMER_START)
    quest:EntitySetTargetable(me, false)
    if not NOVI.acquire(quest, me, SCRIPT_PRIORITY) then return abort(quest, me) end
    local walkOff = quest:GetThingWithScriptName(MARKER_WALK_OFF)
    local target = walkOff and walkOff:GetPos() or ZERO_POSITION
    if not walk_to(quest, me, target) then return abort(quest, me) end
    MyPhase = PHASE.AWAY
    return true
end

-- case 2
local function wait_away(quest, me)
    local watchTimer = F.get(quest, F.WatchTimer)
    quest:Log("NOVI_PROBE BarrelMan AWAY watchTimer=" .. tostring(watchTimer) .. " value=" .. tostring(quest:GetTimer(watchTimer)))
    while quest:GetTimer(watchTimer) ~= WATCH_TIMER_RETURN_AT do
        if not NOVI.frame(quest, me) then return abort(quest, me) end
    end
    local walkOff = quest:GetThingWithScriptName(MARKER_WALK_OFF)
    local walkOffAlt = quest:GetThingWithScriptName(MARKER_WALK_OFF_ALT)
    -- Appear at whichever walk-off marker the camera cannot currently see.
    if quest:IsCameraPosOnScreen(walkOff:GetPos()) then
        quest:EntityTeleportToThing(me, walkOffAlt, false)
    else
        quest:EntityTeleportToThing(me, walkOff, false)
    end
    MyPhase = PHASE.RETURNING
    quest:Log("NOVI_PROBE BarrelMan RETURNING timer=" .. tostring(quest:GetTimer(watchTimer)))
    return true
end

-- case 3
local function walk_back(quest, me, manStart)
    if not NOVI.acquire(quest, me, SCRIPT_PRIORITY) then return abort(quest, me) end
    quest:EntitySetCutsceneBehaviour(me, CUTSCENE_BEHAVIOUR_ON)
    quest:EntitySetTargetable(me, false)
    if not walk_to(quest, me, manStart:GetPos()) then return abort(quest, me) end
    MyPhase = PHASE.JUDGE_HERO
    return true
end

-- case 4
local function judge_hero(quest, me)
    quest:Log("NOVI_PROBE BarrelMan JUDGE bad=" .. tostring(F.get(quest, F.BadDeedsPerformed)))
    F.set(quest, F.BarrelManSpokenToHeroOnReturn, true)
    quest:Log("NOVI_PROBE BarrelMan JUDGE after spoken flag")
    local hero = quest:GetHero()
    quest:Log("NOVI_PROBE BarrelMan JUDGE after GetHero")
    local mePos = me:GetPos()
    local heroPos = hero:GetPos()
    quest:Log(string.format(
        "NOVI_PROBE BarrelMan JUDGE positions man=(%.3f,%.3f,%.3f) hero=(%.3f,%.3f,%.3f) distance=%.3f",
        mePos.x or 0, mePos.y or 0, mePos.z or 0,
        heroPos.x or 0, heroPos.y or 0, heroPos.z or 0,
        quest:GetDistanceBetweenThings(me, hero) or -1))
    quest:EntitySetFacingAngleTowardsThing(me, hero, false)
    quest:Log("NOVI_PROBE BarrelMan JUDGE after facing")
    quest:EntitySetCutsceneBehaviour(me, CUTSCENE_BEHAVIOUR_OFF)
    quest:Log("NOVI_PROBE BarrelMan JUDGE after cutscene behaviour")
    quest:EntitySetTargetable(me, true)
    quest:Log("NOVI_PROBE BarrelMan JUDGE after targetable")
    -- 0x00DB5A88-0x00DB5ABC is a real short-circuit OR: the 10 m helper is called only when
    -- CanThingBe_Seen_ByOtherThing(hero, me) returned false. Keep that ordering because the
    -- first operand uses the man's perception/sight-radius state, whereas the fallback is a
    -- pure distance test. Retail TLC does not read the barrel flag in this branch. New Oakvale's
    -- compatibility layer does: Anniversary's visible broken-stock rebuke and playtest expectation
    -- require destroyed stock to suppress the contradictory thanks/good-deed result even when the
    -- Hero is inside the return radius. This does not award the separate unattended-warehouse deed.
    local heroDetected = quest:CanThingBe_Seen_ByOtherThing(hero, me)
    quest:Log("NOVI_PROBE BarrelMan JUDGE after visibility visible=" .. tostring(heroDetected))
    if not heroDetected then
        heroDetected = NOVI.hero_within(quest, me, SEE_RANGE)
        quest:Log("NOVI_PROBE BarrelMan JUDGE after distance nearby=" .. tostring(heroDetected))
    end
    local stockBroken = F.get(quest, F.BarrelBrokenPersistent)
    if heroDetected and stockBroken then
        HeroLetMeDown = true
        if not NOVI.acquire(quest, me, SCRIPT_PRIORITY) then return abort(quest, me) end
        quest:StartMovieSequence()
        quest:PauseAllNonScriptedEntities(true)
        if not speak_to_hero(quest, me, TEXT_LETDOWN_BROKEN) then
            quest:PauseAllNonScriptedEntities(false)
            quest:EndMovieSequence()
            return abort(quest, me)
        end
        quest:PauseAllNonScriptedEntities(false)
        quest:EndMovieSequence()
    elseif heroDetected then
        if not NOVI.acquire(quest, me, SCRIPT_PRIORITY) then return abort(quest, me) end
        quest:StartMovieSequence()
        quest:PauseAllNonScriptedEntities(true)
        if not speak_to_hero(quest, me, TEXT_THANKS) then
            quest:PauseAllNonScriptedEntities(false)
            quest:EndMovieSequence()
            return abort(quest, me)
        end
        Deeds.add_good(quest, me)
        quest:PauseAllNonScriptedEntities(false)
        quest:EndMovieSequence()
    else
        local conv = quest:AddNewConversation(me, false, false)
        quest:AddLineToConversation(conv, TEXT_WHERE_GONE, me, nil, false) -- retail: (conv, key, false, me, <null thing>)
        HeroLetMeDown = true
        quest:StartMovieSequence()
        quest:PauseAllNonScriptedEntities(true)
        quest:DisplayGameInfo(TEXT_LEFT_UNATTENDED)
        while not quest:MsgIsGameInfoClickedPast() do
            if not NOVI.frame(quest, me) then
                quest:PauseAllNonScriptedEntities(false)
                quest:EndMovieSequence()
                return abort(quest, me)
            end
        end
        Deeds.add_bad(quest, me, 1)
        quest:PauseAllNonScriptedEntities(false)
        quest:EndMovieSequence()
    end
    quest:SetCreatureBrain(me, BRAIN_AFTER_RETURN)
    MyPhase = PHASE.DONE
    return true
end

-- Phase AT_WAREHOUSE talk: approach the hero, ask the favour, fade, swap positions, start the watch.
local function ask_favour_and_leave(quest, me)
    quest:Log("NOVI_PROBE BarrelMan ASK_FAVOUR entered")
    local stepTimer = quest:RegisterTimer()          -- retail scoped CTimer (RegisterTimer/DeregisterTimer)
    local hero = quest:GetHero()
    while NOVI.distance_from_thing_to_position_over(me, hero:GetPos(), APPROACH_HERO_DISTANCE) do
        if not NOVI.frame(quest, me) then quest:DeregisterTimer(stepTimer); return false end
        me:MoveToPosition(hero:GetPos(), APPROACH_HERO_RADIUS, MOVE_TYPE, false, true)
        quest:SetTimer(stepTimer, APPROACH_STEP_SECONDS)
        while me:IsPerformingScriptTask() and quest:GetTimer(stepTimer) > 0 do
            if not NOVI.frame(quest, me) then quest:DeregisterTimer(stepTimer); return false end
        end
    end
    if not speak_to_hero(quest, me, TEXT_FAVOUR) then
        quest:DeregisterTimer(stepTimer)
        return false
    end
    -- Retail does not ask a yes/no question here.  The 0x00DB5330 instruction stream proceeds
    -- directly from Speak(TEXT_QST_048_BARRELMAN_FAVOUR) at 0x00DB6385 to FadeScreenOut via the
    -- interface slot at 0x5d4; the function has no call through GiveHeroYesNoQuestion's 0x1c8 slot
    -- and contains no question/answer text keys.  Watching the warehouse is a mandatory handoff.
    quest:FadeScreenOut(FADE_OUT_SECONDS, FADE_HOLD_SECONDS)   -- Forge supplies retail-exact opaque black
    quest:Pause(FADE_PAUSE_SECONDS)
    quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName(MARKER_GUARD_POINT), false)
    quest:EntityTeleportToThing(me, quest:GetThingWithScriptName(MARKER_HIDDEN_POS), false)
    MyPhase = PHASE.AWAY
    quest:ClearThingHasInformation(me)
    quest:FadeScreenIn()
    quest:SetTimer(F.get(quest, F.WatchTimer), WATCH_TIMER_START)
    quest:Log("NOVI_PROBE BarrelMan TIMER_ARMED id=" .. tostring(F.get(quest, F.WatchTimer)) .. " value=" .. tostring(quest:GetTimer(F.get(quest, F.WatchTimer))))
    F.set(quest, F.BarrelManLeftHeroInCharge, true)
    quest:DeregisterTimer(stepTimer)
    return true
end

local function post_return_remark(quest, me)
    if not HeroLetMeDown then
        return speak_to_hero(quest, me, TEXT_NO_TIME)
    elseif F.get(quest, F.BarrelBrokenPersistent) then
        return speak_to_hero(quest, me, TEXT_LETDOWN_BROKEN)
    else
        return speak_to_hero(quest, me, TEXT_LETDOWN_NOT_BROKE)
    end
end

local function talked_to(quest, me)
    if not NOVI.acquire(quest, me, SCRIPT_PRIORITY) then return abort(quest, me) end
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    local ok = true
    if MyPhase == PHASE.AT_WAREHOUSE then
        ok = ask_favour_and_leave(quest, me)
    elseif MyPhase == PHASE.DONE then
        ok = post_return_remark(quest, me)
    elseif MyPhase == PHASE.JUDGE_HERO then
        -- retail: no line while he is still judging the hero
    else
        ok = speak_to_hero(quest, me, TEXT_NOT_LARKING) -- phases WALKING_OFF / AWAY / RETURNING
    end
    quest:PauseAllNonScriptedEntities(false)
    quest:EndMovieSequence()
    if not ok then return abort(quest, me) end
    return true
end

local function hit_by_hero(quest, me)
    local hero = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, hero)
    quest:EntitySetThingAsAllyOfThing(hero, me)
    Deeds.add_bad(quest, me, 2)
    if not NOVI.acquire(quest, me, SCRIPT_PRIORITY) then return abort(quest, me) end
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    if not speak_to_hero(quest, me, TEXT_CAREFUL) then
        quest:PauseAllNonScriptedEntities(false)
        quest:EndMovieSequence()
        return abort(quest, me)
    end
    quest:PauseAllNonScriptedEntities(false)
    quest:EndMovieSequence()
    return true
end

-- Per-frame handler for phases AT_WAREHOUSE and DONE (retail switch default).
local function idle_handler(quest, me)
    local hit = me:MsgIsHitByHero()
        or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHeroSpecialAbility(IGNORED_HIT_ABILITY))
    if hit then
        return hit_by_hero(quest, me)
    end
    local talked = (MyPhase == PHASE.AT_WAREHOUSE and NOVI.hero_within(quest, me, TALK_RANGE))
        or me:IsTalkedToByHero()   -- ForgeFSE name for retail MsgIsTalkedToBy(hero)
    if talked then
        return talked_to(quest, me)
    end
    if MyPhase == PHASE.AT_WAREHOUSE
        and (not OverheardYet or quest:RetailRandModulo(OVERHEAR_REPEAT_MODULUS) == 0)
        and NOVI.hero_within(quest, me, OVERHEAR_RANGE) then
        OverheardYet = true
        local conv = quest:AddNewConversation(me, false, false)
        quest:AddLineToConversation(conv, TEXT_OVERHEAR, me, nil)   -- retail never removes this conversation
    end
    return true
end

function Main(quest, me)
    if not NOVI.frame(quest, me) then return end
    quest:SetCreatureBrain(me, BRAIN_PASSIVE)
    quest:SetWanderCentrePoint(me, me:GetHomePos())
    quest:SetWanderMinDistance(me, WANDER_MIN_DISTANCE)
    quest:SetWanderMaxDistance(me, WANDER_MAX_DISTANCE)
    quest:SetScriptingStateGroup(me, SCRIPTING_STATE_GROUP)
    local manStart = quest:GetThingWithScriptName(MARKER_MAN_START)
    local guardPoint = quest:GetThingWithScriptName(MARKER_GUARD_POINT)   -- retail fetches it here but never uses it
    quest:Log("NOVI_PROBE BarrelMan Main phase=" .. tostring(MyPhase))
    while true do
        local ok
        if MyPhase == PHASE.WALKING_OFF then
            ok = walk_off(quest, me)
        elseif MyPhase == PHASE.AWAY then
            ok = wait_away(quest, me)
        elseif MyPhase == PHASE.RETURNING then
            ok = walk_back(quest, me, manStart)
        elseif MyPhase == PHASE.JUDGE_HERO then
            ok = judge_hero(quest, me)
        else
            ok = idle_handler(quest, me)
        end
        if not ok then return end
        if not NOVI.frame(quest, me) then NOVI.release(quest, me); return end
    end
end
