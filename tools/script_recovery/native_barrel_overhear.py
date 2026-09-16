"""Replace scratch predicates with the native phase/random/distance gate."""
import hashlib
import json
from pathlib import Path

REPLACEMENT='''        if __native_entity_state:GetStateInt("MyPhase") == 0
            and resources:ShouldBarrelOverhear(me, __native_entity_state:GetStateBool("OverheardYet")) then
            if quest:IsActiveThreadTerminating() then goto LAB_00db6afd end
            __native_entity_state:SetStateBool("OverheardYet", true)
            resources:AddBarrelConversation(me, "TEXT_QST_048_BARRELMAN_OVERHEAR")
        end
'''


def recover(source,data):
    witness=json.loads(Path(__file__).with_name('native_barrel_overhear_witness.json').read_text())
    raw=data.bytes_at(witness['address'],witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=witness['sha256']:
        raise ValueError('Barrel overhear native instructions changed')
    if source.count(witness['oldLua'])!=1:raise ValueError('Barrel overhear source correspondence changed')
    return source.replace(witness['oldLua'],REPLACEMENT,1),dict(witness,status='recovered',semantics='Phase zero only; first attempt skips rand; later signed remainder uses live divisor after retail rand; fresh hero distance-under15 before termination check and state write.')
