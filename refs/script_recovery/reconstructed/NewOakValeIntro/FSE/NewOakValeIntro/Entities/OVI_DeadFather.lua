local NOVI = require("NewOakValeIntro.common")
local F = require("NewOakValeIntro.fields")
local Deeds = require("NewOakValeIntro.deeds")

--[[
  OVI_DeadFather  (retail NScript::CQ_NewOakValeIntroScript::COVI_DeadFather)
  Init 0x00DB8290   Main 0x00DB8300   OnPredicateFail 0x00DB8260 (empty override)
  Evidence level: reconstructed-source (Ghidra decompile + retail disassembly of Fable.exe).

  The hero's (apparently) dead father in the burning-village aftermath. Init pins a quest-core orb
  on him and stops the hero pushing him; Main takes scripted control, snaps him onto the MK_OVID_DAD
  marker (position and facing), loops the CS_DEAD_DAD pose and waits for the quest flag DadFound,
  after which it removes the minimap marker and idles forever.
]]

local SCRIPT_PRIORITY   = 4                    -- StartScriptingEntity(me, res, 4)
local MINIMAP_MARKER    = "HUD_ORB_QUEST_CORE"
local MARKER_DAD        = "MK_OVID_DAD"
local ANIM_DEAD_DAD     = "CS_DEAD_DAD"
local LOOP_FOREVER      = -1                   -- PlayLoopingAnimation loop count 0xffffffff
local FACE_MARKER_FLAG  = true                 -- EntitySetFacingAngle(me, angle, true)

function Init(quest, me)
    quest:MiniMapAddMarker(me, MINIMAP_MARKER)
    quest:SetIsPushableByHero(me, false)
end

function Main(quest, me)
    if not NOVI.frame(quest, me) then return end
    if not NOVI.acquire(quest, me, SCRIPT_PRIORITY) then NOVI.release(quest, me); return end
    local marker = quest:GetThingWithScriptName(MARKER_DAD)
    quest:EntityTeleportToThing(me, marker)              -- retail: (me, marker, false)
    marker = quest:GetThingWithScriptName(MARKER_DAD)    -- retail looks the marker up a second time
    quest:EntitySetFacingAngle(me, marker:GetAngleXY(), FACE_MARKER_FLAG)   -- CScriptThing vtable +0x28 = GetAngleXY
    -- retail: PlayLoopingAnimation("CS_DEAD_DAD", -1, <bool flags>); only the loop count maps to the binding
    me:PlayLoopingAnimation(ANIM_DEAD_DAD, LOOP_FOREVER)
    while not F.get(quest, F.DadFound) do
        if not NOVI.frame(quest, me) then NOVI.release(quest, me); return end
    end
    quest:MiniMapRemoveMarker(me)
    while NOVI.frame(quest, me) do end
    NOVI.release(quest, me)
end

function OnPredicateFail(quest, me)
    -- retail: empty override 0x00DB8260
end
