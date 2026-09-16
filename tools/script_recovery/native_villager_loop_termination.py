"""Recover the actual termination results lost as decompiler extraout_AL names."""
import hashlib
import json
from pathlib import Path


def recover(source,data):
    regions=json.loads(Path(__file__).with_name('native_villager_loop_termination_witness.json').read_text())
    for region in regions:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            raise ValueError('Villager loop termination native bytes changed')
    edits=[('cVar6 = extraout_AL_00','cVar6 = not alive'),
           ('cVar6 = extraout_AL_32','cVar6 = not alive'),
           ('if cVar6 ~= 0 then','if cVar6 then')]
    for old,new in edits:
        if source.count(old)!=1:raise ValueError('Villager loop termination source correspondence changed')
        source=source.replace(old,new,1)
    return source,dict(status='recovered-loop-termination',regions=regions,
        remaining='Owning resource, conversation key and speech temporary cleanup still require full candidate lowering.')
