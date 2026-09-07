-- ScytheMarker.lua
-- Entity script for ScytheMarker (QS_ScytheInfo.c)
-- Functional parity port of NScript::CQS_ScytheInfoScript::CScytheMarker

local CREATURE_DEF = "CREATURE_RIVAL_HERO_SCYTHE"
local SPAWNED_SCRIPT_NAME = "ScytheNearOracle"

function Main(quest, me)
    local pos = me:GetPos()
    quest:CreateCreature(CREATURE_DEF, pos, SPAWNED_SCRIPT_NAME)
    quest:RemoveThing(me)
end
