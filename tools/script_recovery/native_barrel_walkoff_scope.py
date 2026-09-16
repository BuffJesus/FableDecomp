"""Own the phase-one walk-off marker through the entire movement loop."""
import hashlib
import json
from pathlib import Path

HELPER='''    local function walkOffFromWarehouse()
        local marker = resources:NewThingFromScriptName("M_BarrelManWalkOff")
        local position = resources:ThingPosition(marker)
        local function moveUntilNear()
            while controlled_distance(position, 2.0) do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return false end
                resources:MoveToPosition(barrel_resource, position, 0.0, 1, false, false)
                if not waitForBarrelSpeech() then return false end
            end
            return not quest:IsActiveThreadTerminating()
        end
        local continued = moveUntilNear()
        if continued then __native_entity_state:SetStateInt("MyPhase", 2) end
        resources:DestroyThing(marker)
        return continued
    end
'''


def recover(source,data):
    w=json.loads(Path(__file__).with_name('native_barrel_walkoff_scope_witness.json').read_text())
    raw=data.bytes_at(w['address'],w['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['sha256']:
        raise ValueError('Barrel walk-off native instructions changed')
    if source.count(w['oldLua'])!=1:raise ValueError('Barrel walk-off source correspondence changed')
    source=source.replace(w['oldLua'],'                if not walkOffFromWarehouse() then goto LAB_00db6afd end\n')
    source=source.replace('    local bVar3, cVar2,',HELPER+'    local bVar3, cVar2,',1)
    return source,dict(w,status='recovered',semantics='One owned walk-off marker; position snapshot precedes distance queries, successful phase2 write precedes marker destruction, cancellation skips phase write.')
