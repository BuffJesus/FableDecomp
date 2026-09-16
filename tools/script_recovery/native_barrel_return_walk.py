"""Recover return movement using the Main-owned warehouse marker snapshot."""
import hashlib
import json
from pathlib import Path

HELPER = '''    local function returnToWarehouse(marker)
        local position = resources:ThingPosition(marker)
        while controlled_distance(position, 2.0) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
            resources:MoveToPosition(barrel_resource, position, 0.0, 1, false, false)
            if not waitForBarrelSpeech() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        __native_entity_state:SetStateInt("MyPhase", 4)
        return true
    end
'''


def recover(source, data):
    witness=json.loads(Path(__file__).with_name('native_barrel_return_walk_witness.json').read_text())
    raw=data.bytes_at(witness['address'],witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=witness['sha256']:
        raise ValueError('Barrel return walk instructions changed')
    edits=[
        (witness['oldLua'],'                        if not returnToWarehouse(warehouseStartMarker) then goto LAB_00db6afd end\n'),
        ('    local bVar3, cVar2,',HELPER+'    local bVar3, cVar2,'),
        ('    local barrel_resource, barrel_interaction_movie\n','    local barrel_resource, barrel_interaction_movie\n    local warehouseStartMarker, warehouseGuardMarker\n'),
        ('    r1 = quest:GetThingWithScriptName("M_WHouse_ManStart")','    warehouseStartMarker = resources:NewThingFromScriptName("M_WHouse_ManStart")'),
        ('    r2 = quest:GetThingWithScriptName("M_WHouse_GuardPoint")','    warehouseGuardMarker = resources:NewThingFromScriptName("M_WHouse_GuardPoint")'),
        ('    ::LAB_00db6afd::\n','    ::LAB_00db6afd::\n    resources:DestroyThing(warehouseGuardMarker); warehouseGuardMarker = nil\n    resources:DestroyThing(warehouseStartMarker); warehouseStartMarker = nil\n'),
    ]
    for old,new in edits:
        if source.count(old)!=1:raise ValueError('Barrel return walk source correspondence changed')
        source=source.replace(old,new,1)
    return source,dict(witness,status='recovered',semantics='Main marker lifetimes verified by markerMap; snapshot once, phase4 after final termination check, main cleanup guard then start then resource. Host callback handles remaining early returns/errors.')
