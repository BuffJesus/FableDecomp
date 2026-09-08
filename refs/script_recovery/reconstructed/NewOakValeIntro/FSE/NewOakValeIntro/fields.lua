-- NewOakValeIntro/fields.lua
--
-- Quest-object fields of the retail NScript::CQ_NewOakValeIntroScript, exposed to every module of the
-- package through ForgeFSE quest state (SetStateBool/SetStateInt/SetStateString). Names are the PDB
-- member names (Ego_r.pdb, struct_layouts_egor.tsv). The retail offset in each comment is the retail
-- Fable.exe layout (PDB offset - 0x14), verified against the constructor @0x00DAAC00 and Init @0x00DAADD0.
--
-- Only `AttackOver` is persisted by retail (OnPersist @0x00DAADA0); every other field is rebuilt by
-- Init on load. This module deliberately mirrors that: it offers no persistence of its own.
--
-- Evidence: PDB-name (names), native-decompile (offsets, init values).

local F = {}

local BOOL, INT, STRING = "bool", "int", "string"

local function key(name, kind, offset, init)
    return { name = name, kind = kind, offset = offset, init = init }
end

-- bookkeeping written by the quest and read by entities (PARENT + offset)
F.VictimShake                    = key("VictimShake",                    BOOL,   0x48, false)
F.StopTimeIndex                  = key("StopTimeIndex",                  INT,    0x4c, 0)
F.AttackOver                     = key("AttackOver",                     BOOL,   0x50, false)  -- persisted
F.DadFound                       = key("DadFound",                       BOOL,   0x51, false)
F.DadFinishedIntro               = key("DadFinishedIntro",               BOOL,   0x52, false)
F.DadOfferedRewards              = key("DadOfferedRewards",              BOOL,   0x53, false)
F.GoodDeedsPerformed             = key("GoodDeedsPerformed",             INT,    0x54, 0)
F.BadDeedsPerformed              = key("BadDeedsPerformed",              INT,    0x58, 0)
F.GUIGoodDeedCounter             = key("GUIGoodDeedCounter",             INT,    0x5c, nil)    -- quest-info handle
F.GUIBarrelCounter               = key("GUIBarrelCounter",               INT,    0x60, nil)    -- quest-info-bar handle
F.GUIBullyHealthCounter          = key("GUIBullyHealthCounter",          INT,    0x64, -999)
F.GuardsDealtWithBadDeeds        = key("GuardsDealtWithBadDeeds",        INT,    0x68, 0)
F.BullySubdued                   = key("BullySubdued",                   BOOL,   0x6c, false)
F.BullyRanOff                    = key("BullyRanOff",                    BOOL,   0x6d, false)
F.GivenHeroTeddy                 = key("GivenHeroTeddy",                 BOOL,   0x6e, false)
F.HeroAttackedVictim             = key("HeroAttackedVictim",             BOOL,   0x6f, false)
F.SpokeAboutFindingTeddy         = key("SpokeAboutFindingTeddy",         BOOL,   0x70, nil)    -- not reset by Init
F.HeroDiscoveredInfidelity       = key("HeroDiscoveredInfidelity",       BOOL,   0x71, false)
F.BarrelManLeftHeroInCharge      = key("BarrelManLeftHeroInCharge",      BOOL,   0x72, false)
F.BarrelManSpokenToHeroOnReturn  = key("BarrelManSpokenToHeroOnReturn",  BOOL,   0x73, false)
F.BarrelBrokenInstantaneous      = key("BarrelBrokenInstantaneous",      BOOL,   0x74, nil)    -- not reset by Init
F.BarrelBrokenPersistent         = key("BarrelBrokenPersistent",         BOOL,   0x75, false)
F.BarrelBrokenPos                = key("BarrelBrokenPos",                STRING, 0x76, nil)    -- C3DVector, serialised "x,y,z"
F.InstructionGiven_Barrels       = key("InstructionGiven_Barrels",       BOOL,   0x82, false)
F.ReceiveKiss                    = key("ReceiveKiss",                    BOOL,   0x83, false)
F.ReceiveHug                     = key("ReceiveHug",                     BOOL,   0x84, false)
F.WarehouseMeetPoint             = key("WarehouseMeetPoint",             STRING, 0x85, nil)    -- C3DVector, serialised "x,y,z"
F.TeddyRuined                    = key("TeddyRuined",                    BOOL,   0x91, false)
F.GuardsSpokenOnce               = key("GuardsSpokenOnce",               BOOL,   0x92, false)
F.TalkingToWoman                 = key("TalkingToWoman",                 BOOL,   0x93, false)
F.GivenSweets                    = key("GivenSweets",                    BOOL,   0x94, false)
F.GivenTheresaChocs              = key("GivenTheresaChocs",              BOOL,   0x95, false)
F.VictimComplainsAboutLosingTeddy = key("VictimComplainsAboutLosingTeddy", BOOL, 0x96, false)
F.lastVillagerSpeechIdx          = key("lastVillagerSpeechIdx",          INT,    0x98, 0)

-- WhichBadDeedsPerformed[5] (bool[5] @0xfc, indexed by EBadDeeds). The enum names are not in the
-- PDB tables available here; indices are recorded where retail passes them (see deeds.lua).
F.WhichBadDeedsPerformed = {}
for index = 0, 4 do
    F.WhichBadDeedsPerformed[index] = key("WhichBadDeedsPerformed_" .. index, BOOL, 0xfc + index, false)
end

-- Timers are retail CTimer handles registered in the constructor (RegisterTimer x2). ForgeFSE hands
-- out its own ids, so the id itself lives in quest state under the PDB name.
F.TalkIntermittentTimer          = key("TalkIntermittentTimer",          INT,    0x104, nil)
F.WatchTimer                     = key("WatchTimer",                     INT,    0x108, nil)

-- CQ_SunnyvaleMasterData members touched by this quest (offsets exact from struct_layouts_egor.tsv).
F.master = {
    TeddySolution = { name = "TeddySolution", kind = STRING, offset = 0x54 },  -- "A"/"B": how the teddy quest ended
}

local function assert_key(k)
    if type(k) ~= "table" or not k.name then
        error("fields: expected a field key table, got " .. tostring(k), 3)
    end
end

-- `F.get(quest, F.WhichBadDeedsPerformed, index)` addresses the retail bool[5] by EBadDeeds index.
local function resolve(k, index)
    if k == F.WhichBadDeedsPerformed then
        if index == nil or k[index] == nil then
            error("fields: WhichBadDeedsPerformed needs an index 0..4, got " .. tostring(index), 3)
        end
        return k[index]
    end
    return k
end

-- Retail C3DVector members are carried through the string slot as "x,y,z" (ForgeFSE has no vector
-- state type); readers get a {x=,y=,z=} table back, which is what CreateCreature/positions accept.
local function encode_vector(value)
    if type(value) == "table" and value.x ~= nil then
        return string.format("%.6f,%.6f,%.6f", value.x, value.y or 0, value.z or 0)
    end
    return value
end

local function decode_vector(value)
    if type(value) == "string" then
        local x, y, z = value:match("^%s*([%-%d%.eE]+)%s*,%s*([%-%d%.eE]+)%s*,%s*([%-%d%.eE]+)%s*$")
        if x then
            return { x = tonumber(x), y = tonumber(y), z = tonumber(z) }
        end
    end
    return value
end

function F.get(quest, k, index)
    k = resolve(k, index)
    assert_key(k)
    if k.kind == BOOL then
        return quest:GetStateBool(k.name)
    elseif k.kind == INT then
        return quest:GetStateInt(k.name)
    end
    return decode_vector(quest:GetStateString(k.name))
end

function F.set(quest, k, value, index)
    k = resolve(k, index)
    assert_key(k)
    if k.kind == BOOL then
        quest:SetStateBool(k.name, value and true or false)
    elseif k.kind == INT then
        quest:SetStateInt(k.name, value)
    else
        quest:SetStateString(k.name, encode_vector(value))
    end
end

-- Retail integer fields are compared with 0 / incremented directly; nil from a mock host counts as 0.
function F.get_int(quest, k)
    return F.get(quest, k) or 0
end

function F.increment(quest, k)
    local value = F.get_int(quest, k) + 1
    F.set(quest, k, value)
    return value
end

function F.get_master(quest, mk)
    return quest:GetMasterGameState(mk.name)
end

function F.set_master(quest, mk, value)
    quest:SetMasterGameState(mk.name, value)
end

-- Apply the retail Init @0x00DAADD0 field resets (everything with a non-nil `init`), in PDB order.
function F.reset_for_init(quest)
    local ordered = {
        F.VictimShake, F.DadFound, F.AttackOver, F.DadFinishedIntro,
        F.BullySubdued, F.BullyRanOff, F.GivenHeroTeddy, F.HeroAttackedVictim,
        F.GoodDeedsPerformed, F.BadDeedsPerformed, F.GuardsDealtWithBadDeeds,
        F.HeroDiscoveredInfidelity, F.BarrelManLeftHeroInCharge, F.BarrelManSpokenToHeroOnReturn,
        F.InstructionGiven_Barrels, F.ReceiveKiss, F.ReceiveHug, F.BarrelBrokenPersistent,
    }
    for _, k in ipairs(ordered) do
        F.set(quest, k, k.init)
    end
end

-- Second half of the retail Init resets (after the WatchTimer SetTimer call).
function F.reset_for_init_tail(quest)
    F.set(quest, F.GUIBullyHealthCounter, F.GUIBullyHealthCounter.init)
    local ordered = {
        F.TeddyRuined, F.GuardsSpokenOnce, F.TalkingToWoman, F.GivenSweets, F.GivenTheresaChocs,
        F.VictimComplainsAboutLosingTeddy,
    }
    for _, k in ipairs(ordered) do
        F.set(quest, k, k.init)
    end
    -- retail zeroes the 5-byte WhichBadDeedsPerformed array as one dword plus one byte
    for index = 0, 4 do
        F.set(quest, F.WhichBadDeedsPerformed[index], false)
    end
end

return F
