"""Recover the bound Barrel Man's initial brain and wander operands."""
import hashlib
import json
from pathlib import Path

LOWERED='''    quest:SetCreatureBrain(me, "BRAIN_PASSIVE_OVERRIDE")
    local barrelHomePosition = me:GetHomePos()
    quest:SetWanderCentrePoint(me, barrelHomePosition)
    quest:SetWanderMinDistance(me, 0.0)
    quest:SetWanderMaxDistance(me, 1.0)
    quest:SetScriptingStateGroup(me, 4)
'''


def verify(data):
    witness=json.loads(Path(__file__).with_name('native_barrel_man_setup_witness.json').read_text())
    if (witness['address'],witness['size'],witness['brainLiteral'])!=(0xDB5391,261,0x12D921C):
        raise ValueError('Barrel Man setup coverage changed')
    raw=data.bytes_at(witness['address'],witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=witness['sha256']:
        raise ValueError('Barrel Man setup instructions changed')
    if data.string_at(witness['brainLiteral'])!='BRAIN_PASSIVE_OVERRIDE':
        raise ValueError('Barrel Man brain identity changed')
    if hashlib.sha256(witness['oldLua'].encode()).hexdigest()!=witness['oldLuaSha256']:
        raise ValueError('Barrel Man setup draft correspondence changed')
    return witness


def recover(source,data):
    witness=verify(data)
    if source.count(witness['oldLua'])!=1:raise ValueError('Barrel Man setup draft changed')
    return source.replace(witness['oldLua'],LOWERED,1),{
        'nativeAddress':witness['address'],'nativeSha256':witness['sha256'],
        'correctedMaximumDistance':1.0,'actor':'bound me',
        'runtimeValidation':'work/barrel_man_setup_runtime_checks/result.json',
        'runtime':'Existing retained-by-value wander/state APIs; complete actor resource conversion remains pending.'}
