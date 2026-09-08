-- NewOakValeIntro/common.lua
--
-- Small host-facing helpers shared by the quest module and its entity scripts. Each helper stands for
-- one retail idiom of NScript::CQ_NewOakValeIntroScript so the callers read like the decompile:
--
--   retail                                            here
--   NewScriptFrame(); if (IsActiveThreadTerminating) NOVI.frame(quest[, me]) -> false when terminating
--   IsDistanceBetweenThingsUnder(me, GetHero(), d)   NOVI.hero_within(quest, me, d)
--   StartScriptingEntity(me, res, prio) wait loop    NOVI.acquire(quest, me, prio)
--   <no ForgeFSE binding>                            NOVI.unsupported(quest, "RetailName", args)
--
-- Evidence level of the package: reconstructed-source. Nothing here mutates the game on its own.

local NOVI = {}

NOVI.PACKAGE = "NewOakValeIntro"
NOVI.RETAIL_SCRIPT = "Q_NewOakValeIntro"
NOVI.EVIDENCE_LEVEL = "reconstructed-source"

-- Retail script-name / region / section literals (native-decompile).
NOVI.REGION_START_OAKVALE     = "StartOakVale"
NOVI.SECTION_PRE_ATTACK       = "Q_NewOakValeIntro_PreAttack"
NOVI.SECTION_POST_ATTACK      = "Q__OakValeIntro_PostAttack"   -- double underscore is retail spelling
NOVI.VILLAGE_OAKVALE          = "V_OakVale"
NOVI.MARKER_POST_ATTACK_START = "M_PostAttackStart"
NOVI.MARKER_DAD_TRIGGER       = "MK_OVI_DADTRIGGER"
NOVI.MARKER_DEAD_DAD          = "MK_OVID_DAD"
NOVI.MARKER_WAREHOUSE_GUARD   = "M_WHouse_GuardPoint"
NOVI.HOUSE_HERO               = "HerosOldHouse"
NOVI.HUD_QUEST_CORE_ORB       = "HUD_ORB_QUEST_CORE"
NOVI.HUD_CLOCK_ICON           = "HUD_CLOCK_ICON"
NOVI.OBJECT_TEDDY             = "OBJECT_TEDDY_BEAR_UNGIVEABLE"
NOVI.OBJECT_GOLD_1            = "OBJECT_GOLD_1"
NOVI.CREATURE_HERO_CHILD      = "CREATURE_HERO_CHILD"
NOVI.CREATURE_STAG_BEETLE     = "CREATURE_OAKVALE_STAG_BEETLE"
NOVI.SCRIPT_CREATED_BEETLE    = "NOVI_CreatedBeetle"
NOVI.SCRIPT_BARREL            = "NOVI_Barrel"
NOVI.SCRIPT_HERO              = "SCRIPT_NAME_HERO"

-- One script frame. Returns false when the host is terminating this thread, in which case the caller
-- must unwind exactly like the retail `IsActiveThreadTerminating` early-return.
function NOVI.frame(quest, me)
    local alive
    if me ~= nil then
        alive = quest:NewScriptFrame(me)
    else
        alive = quest:NewScriptFrame()
    end
    return alive and true or false
end

-- Retail IsDistanceBetweenThingsUnder(a, hero, distance) with the hero looked up each call, as retail does.
function NOVI.hero_within(quest, thing, distance)
    local hero = quest:GetHero()
    if not hero or not thing then
        return false
    end
    return quest:IsDistanceBetweenThingsUnder(thing, hero, distance) and true or false
end

function NOVI.things_within(quest, a, b, distance)
    if not a or not b then
        return false
    end
    return quest:IsDistanceBetweenThingsUnder(a, b, distance) and true or false
end

-- Retail scripts loop `while (!StartScriptingEntity(me, resource, priority)) NewScriptFrame()`. ForgeFSE
-- keeps that scheduler host-managed (the entity binding already owns the scripted resource); the
-- registered entity method `AcquireControl` is the documented equivalent. The priority is kept for
-- the evidence trail even though ForgeFSE's wrapper does not take it.
-- Returns true once control is held; false only when the host reports termination mid-wait (the
-- retail loop's `NewScriptFrame + IsActiveThreadTerminating` exit). ForgeFSE's wrapper blocks, so the
-- false path can only be reached through a mock host.
function NOVI.acquire(quest, me, priority)
    if me and me.AcquireControl then
        local result = me:AcquireControl()
        if result == false then
            return false
        end
        return true
    end
    NOVI.unsupported(quest, "StartScriptingEntity", { priority = priority })
    return true
end

function NOVI.release(quest, me)
    if me and me.ReleaseControl then
        me:ReleaseControl()
    else
        NOVI.unsupported(quest, "ReleaseScriptingEntity", {})
    end
end

-- A retail float constant the decompiler only shows as a .rdata address. Callers pass the symbol so the
-- gap is visible in traces; the value is NOT guessed.
function NOVI.UNKNOWN_DISTANCE(symbol)
    return NOVI.unsupported_value("UNKNOWN_FLOAT", symbol)
end

NOVI.unsupported_calls = {}

-- Explicit marker for a retail operation with no ForgeFSE binding (or an unresolved argument). Logs
-- through the quest so mock traces and the in-game log both show the gap; returns nil.
function NOVI.unsupported(quest, retail_name, args)
    table.insert(NOVI.unsupported_calls, retail_name)
    if quest and quest.Log then
        quest:Log("NewOakValeIntro: UNSUPPORTED retail call " .. tostring(retail_name))
    end
    return nil
end

function NOVI.unsupported_value(kind, symbol)
    table.insert(NOVI.unsupported_calls, kind .. ":" .. tostring(symbol))
    return nil
end

-- Wait one frame at a time until `predicate()` is true; false means the thread was terminated.
function NOVI.wait_until(quest, me, predicate)
    while not predicate() do
        if not NOVI.frame(quest, me) then
            return false
        end
    end
    return true
end

-- Retail info-box idiom: DisplayGameInfo(key) then wait for MsgIsGameInfoClickedPast.
function NOVI.game_info_blocking(quest, me, text_key)
    quest:DisplayGameInfo(text_key)
    return NOVI.wait_until(quest, me, function()
        return quest:MsgIsGameInfoClickedPast()
    end)
end

-- ARGB colours used by StartBarrelTimer's info bar (native-decompile: 0xff00ff00 / 0xffff0000 words).
function NOVI.argb(a, r, g, b)
    return { a = a, r = r, g = g, b = b }
end

NOVI.COLOUR_GREEN = NOVI.argb(0xff, 0x00, 0xff, 0x00)
NOVI.COLOUR_RED   = NOVI.argb(0xff, 0xff, 0x00, 0x00)
NOVI.COLOUR_BLACK = NOVI.argb(0xff, 0x00, 0x00, 0x00)

return NOVI
