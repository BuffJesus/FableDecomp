local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

--[[
  NOVI_CreatedBeetle  (retail NScript::CQ_NewOakValeIntroScript::CNOVI_CreatedBeetle)
  Init 0x00CDEBB0 (shared empty default)   Main 0x00DB80C0
  Evidence level: reconstructed-source (Ghidra decompile + retail disassembly of Fable.exe).

  The stag beetle that WatchBarrels (0x00DBE890) spawns at BarrelBrokenPos when the hero has broken
  most of the barrels (CreateCreature "CREATURE_OAKVALE_STAG_BEETLE" with script name
  NOVI_CreatedBeetle). It lives for LIFETIME_SECONDS and then removes itself.
]]

local LIFETIME_SECONDS = 5   -- SetTimer(timer, 5)

function Init(quest, me)
    -- retail: shared empty default 0x00CDEBB0
end

function Main(quest, me)
    if not NOVI.frame(quest, me) then return end
    local timer = quest:RegisterTimer()
    quest:SetTimer(timer, LIFETIME_SECONDS)
    while quest:GetTimer(timer) ~= 0 do
        if not NOVI.frame(quest, me) then
            quest:DeregisterTimer(timer)
            return
        end
    end
    quest:RemoveThing(me)   -- retail: RemoveThing(me, true, true); the two bools are dropped by the binding
    quest:DeregisterTimer(timer)
end
