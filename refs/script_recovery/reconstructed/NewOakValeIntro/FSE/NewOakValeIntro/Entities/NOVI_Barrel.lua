local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

--[[
  NOVI_Barrel  (retail NScript::CQ_NewOakValeIntroScript::CNOVI_Barrel)
  Init 0x00CDEBB0 (shared empty default)   Main 0x00DB7E10   OnPredicateFail 0x00DB7DB0
  Evidence level: reconstructed-source (Ghidra decompile + retail disassembly of Fable.exe).

  One warehouse barrel. Main only shows the "break the barrels" instruction the first time the hero
  comes within HERO_TRIGGER_DISTANCE of any barrel (quest flag InstructionGiven_Barrels), then idles.
  When the barrel is destroyed the script predicate fails and OnPredicateFail reports the break to the
  quest (BarrelBrokenInstantaneous / BarrelBrokenPersistent / BarrelBrokenPos). The quest-level thread
  WatchBarrels (0x00DBE890) consumes BarrelBrokenInstantaneous each frame to count breaks.
]]

local HERO_TRIGGER_DISTANCE = 2.0    -- IsDistanceBetweenThingsUnder(me, hero, 2.0)
local TEXT_BREAK_BARRELS    = "TEXT_QST_048_INSTRUCTION_BREAK_BARRELS"      -- IsXbox() == true
local TEXT_BREAK_BARRELS_PC = "TEXT_QST_048_INSTRUCTION_BREAK_BARRELS_PC"   -- IsXbox() == false

function Init(quest, me)
    -- retail: shared empty default 0x00CDEBB0
end

local function show_break_instruction(quest, me)
    local onXbox = NOVI.is_xbox()
    local key = onXbox and TEXT_BREAK_BARRELS or TEXT_BREAK_BARRELS_PC
    quest:DisplayGameInfo(key)
    while not quest:MsgIsGameInfoClickedPast() do
        if not NOVI.frame(quest, me) then return false end
    end
    F.set(quest, F.InstructionGiven_Barrels, true)
    return true
end

function Main(quest, me)
    if not NOVI.frame(quest, me) then return end
    while not F.get(quest, F.InstructionGiven_Barrels) do
        if not NOVI.frame(quest, me) then return end
        if NOVI.hero_within(quest, me, HERO_TRIGGER_DISTANCE) then
            if not show_break_instruction(quest, me) then return end
        end
    end
    -- retail idles forever; the break is reported through OnPredicateFail, not from this loop
    while NOVI.frame(quest, me) do end
end

function OnPredicateFail(quest, me)
    F.set(quest, F.BarrelBrokenInstantaneous, true)
    F.set(quest, F.BarrelBrokenPersistent, true)
    F.set(quest, F.BarrelBrokenPos, me:GetPos())   -- CScriptThing vtable +0x18 = GetPos
end
