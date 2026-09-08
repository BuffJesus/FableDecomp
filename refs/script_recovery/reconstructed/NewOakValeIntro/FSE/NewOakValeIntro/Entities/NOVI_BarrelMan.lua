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
  back to his start point and judges the hero: "thanks" (good deed) if the hero is still there,
  "where did you go" (bad deed 1) otherwise. Hitting him is bad deed 2. Barrel breakage itself is
  reported by NOVI_Barrel via BarrelBrokenPersistent, which he only quotes afterwards.
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
local MOVE_TYPE                = 1        -- EScriptEntityMoveType 1 (name unknown)
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
local IGNORED_HIT_ABILITY      = 14       -- EHeroAbility 0xe: a hit by this ability alone does not count

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
-- from what each case does.  Note: value 1 is compared in Main but never written by this class.
local PHASE = {
    AT_WAREHOUSE = 0,   -- Init; idle by the barrels, waiting for the hero
    WALKING_OFF  = 1,   -- walk on foot to M_BarrelManWalkOff (never set here; external/unused)
    AWAY         = 2,   -- hidden; waiting for WatchTimer to reach WATCH_TIMER_RETURN_AT
    RETURNING    = 3,   -- placed at a walk-off marker, walking back to M_WHouse_ManStart
    JUDGE_HERO   = 4,   -- arrived: thank the hero or complain that he left
    DONE         = 5,   -- post-return chatter only
}

-- Entity-local retail fields (this+0x1c..0x20), initialised in Init
local ComplainedAboutStock   -- 0x1c: written in Init only, never read by Main (retail)
local HeroLetMeDown          -- 0x1d
local OverheardYet           -- 0x1e
local MyPhase                -- 0x20 (EMyPhase)

function Init(quest, me)
    quest:SetTimer(F.get(quest, F.WatchTimer), 0)
    ComplainedAboutStock = false
    MyPhase = PHASE.AT_WAREHOUSE
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false)                 -- retail: (me, false, false); 2nd bool dropped by binding
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false)              -- retail: (me, false, true, false); binding keeps 1 bool
    F.set(quest, F.WarehouseMeetPoint, me:GetHomePos())  -- CScriptThing vtable +0x1c = GetHomePos
    HeroLetMeDown = false
    OverheardYet = false
    quest:EntitySetSightRadius(me, SIGHT_RADIUS)
end

-- Retail helper IsDistanceFromThingToPositionOver (0x00CBE45C) has no ForgeFSE binding.
-- Reimplemented as plain maths on me:GetPos(); 2D-vs-3D metric of the retail helper is unverified.
local function distance_over(me, pos, dist)
    local p = me:GetPos()
    if p == nil or p.x == nil or pos == nil or pos.x == nil then return false end
    local dx, dy, dz = p.x - pos.x, p.y - pos.y, (p.z or 0) - (pos.z or 0)
    return (dx * dx + dy * dy + dz * dz) > dist * dist
end

local function is_alive(quest, me)
    return quest:GetHealth(me) > HEALTH_DEAD
end

-- Retail: Speak(hero, key, 0, false, true, false) + IsPerformingScriptTask frame loop; ForgeFSE's
-- me:Speak is blocking (host-managed wait), so the per-frame terminate checks live in the host.
local function speak_to_hero(quest, me, key)
    if is_alive(quest, me) then
        me:Speak(quest:GetHero(), key, SPEECH_SELECTION_METHOD)
    end
end

local function abort(quest, me)
    NOVI.release(quest, me)
    return false
end

-- Walk (re-issuing MoveToPosition until within ARRIVE_TOLERANCE) exactly as retail does in phases 1/3.
local function walk_to(quest, me, target)
    while distance_over(me, target, ARRIVE_TOLERANCE) do
        if not NOVI.frame(quest, me) then return false end
        me:MoveToPosition(target, MOVE_RADIUS_EXACT, MOVE_TYPE)   -- retail also passes (false, false)
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
    if not walk_to(quest, me, walkOff:GetPos()) then return abort(quest, me) end
    MyPhase = PHASE.AWAY
    return true
end

-- case 2
local function wait_away(quest, me)
    local watchTimer = F.get(quest, F.WatchTimer)
    while quest:GetTimer(watchTimer) ~= WATCH_TIMER_RETURN_AT do
        if not NOVI.frame(quest, me) then return abort(quest, me) end
    end
    local walkOff = quest:GetThingWithScriptName(MARKER_WALK_OFF)
    local walkOffAlt = quest:GetThingWithScriptName(MARKER_WALK_OFF_ALT)
    -- Appear at whichever walk-off marker the camera cannot currently see.
    if quest:IsCameraPosOnScreen(walkOff:GetPos()) then
        quest:EntityTeleportToThing(me, walkOffAlt)
    else
        quest:EntityTeleportToThing(me, walkOff)
    end
    MyPhase = PHASE.RETURNING
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
    F.set(quest, F.BarrelManSpokenToHeroOnReturn, true)
    local hero = quest:GetHero()
    quest:EntitySetFacingAngleTowardsThing(me, hero)
    quest:EntitySetCutsceneBehaviour(me, CUTSCENE_BEHAVIOUR_OFF)
    quest:EntitySetTargetable(me, true)
    if quest:CanThingBe_Seen_ByOtherThing(me, hero) or NOVI.hero_within(quest, me, SEE_RANGE) then
        if not NOVI.acquire(quest, me, SCRIPT_PRIORITY) then return abort(quest, me) end
        quest:StartMovieSequence()
        quest:PauseAllNonScriptedEntities(true)
        speak_to_hero(quest, me, TEXT_THANKS)
        Deeds.add_good(quest, me)
        quest:PauseAllNonScriptedEntities(false)
        quest:EndMovieSequence()
    else
        local conv = quest:AddNewConversation(me, false, false)
        quest:AddLineToConversation(conv, TEXT_WHERE_GONE, me, nil)   -- retail: (conv, key, false, me, <null thing>)
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
    local stepTimer = quest:RegisterTimer()          -- retail scoped CTimer (RegisterTimer/DeregisterTimer)
    local hero = quest:GetHero()
    while distance_over(me, hero:GetPos(), APPROACH_HERO_DISTANCE) do
        if not NOVI.frame(quest, me) then quest:DeregisterTimer(stepTimer); return false end
        me:MoveToPosition(hero:GetPos(), APPROACH_HERO_RADIUS, MOVE_TYPE)   -- retail also passes (false, true)
        quest:SetTimer(stepTimer, APPROACH_STEP_SECONDS)
        while me:IsPerformingScriptTask() and quest:GetTimer(stepTimer) > 0 do
            if not NOVI.frame(quest, me) then quest:DeregisterTimer(stepTimer); return false end
        end
    end
    speak_to_hero(quest, me, TEXT_FAVOUR)
    quest:FadeScreenOut(FADE_OUT_SECONDS, FADE_HOLD_SECONDS)   -- retail colour arg 0xff000000 (black) dropped
    quest:Pause(FADE_PAUSE_SECONDS)
    quest:EntityTeleportToThing(hero, quest:GetThingWithScriptName(MARKER_GUARD_POINT))
    quest:EntityTeleportToThing(me, quest:GetThingWithScriptName(MARKER_HIDDEN_POS))
    MyPhase = PHASE.AWAY
    quest:ClearThingHasInformation(me)
    quest:FadeScreenIn()
    quest:SetTimer(F.get(quest, F.WatchTimer), WATCH_TIMER_START)
    F.set(quest, F.BarrelManLeftHeroInCharge, true)
    quest:DeregisterTimer(stepTimer)
    return true
end

local function post_return_remark(quest, me)
    if not HeroLetMeDown then
        speak_to_hero(quest, me, TEXT_NO_TIME)
    elseif F.get(quest, F.BarrelBrokenPersistent) then
        speak_to_hero(quest, me, TEXT_LETDOWN_BROKEN)
    else
        speak_to_hero(quest, me, TEXT_LETDOWN_NOT_BROKE)
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
        post_return_remark(quest, me)
    elseif MyPhase == PHASE.JUDGE_HERO then
        -- retail: no line while he is still judging the hero
    else
        speak_to_hero(quest, me, TEXT_NOT_LARKING)   -- phases WALKING_OFF / AWAY / RETURNING
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
    speak_to_hero(quest, me, TEXT_CAREFUL)
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
        and (not OverheardYet or math.random(0, OVERHEAR_REPEAT_MODULUS - 1) == 0)
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
