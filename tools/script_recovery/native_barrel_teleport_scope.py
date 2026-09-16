"""Recover the timer-gated camera choice with both marker lifetimes intact."""
import hashlib
import json
from pathlib import Path

HELPER = '''    local function teleportWalkOff()
        while resources:GetBarrelWatchTimer(quest:GetStateInt("WatchTimer")) ~= 15 do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        local primary = resources:NewThingFromScriptName("M_BarrelManWalkOff")
        local alternate = resources:NewThingFromScriptName("M_BarrelManWalkOffAlt")
        local visible = resources:IsOwnedThingPositionOnScreen(primary)
        local continued = not quest:IsActiveThreadTerminating()
        if continued then
            resources:TeleportActorToOwnedThing(me, visible and alternate or primary)
            __native_entity_state:SetStateInt("MyPhase", 3)
        end
        resources:DestroyThing(alternate)
        resources:DestroyThing(primary)
        return continued
    end
'''


def recover(source, data):
    witness = json.loads(Path(__file__).with_name('native_barrel_teleport_scope_witness.json').read_text())
    raw = data.bytes_at(witness['address'], witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['sha256']:
        raise ValueError('Barrel teleport native instructions changed')
    anchor = '    local bVar3, cVar2,'
    if source.count(witness['oldLua']) != 1 or source.count(anchor) != 1:
        raise ValueError('Barrel teleport source correspondence changed')
    source = source.replace(witness['oldLua'], '                    if not teleportWalkOff() then goto LAB_00db6afd end\n')
    source = source.replace(anchor, HELPER + anchor, 1)
    return source, dict(witness, status='recovered', semantics='Fresh global timer query and WatchTimer ID per poll; equality 15; camera query before cancellation; alternate then primary destruction after phase write or cancellation.')
