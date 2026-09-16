"""Restore bound-actor operands in Villager hit-response calls."""
import hashlib
import json
from pathlib import Path


def recover(source,data):
    regions=json.loads(Path(__file__).with_name('native_villager_bound_operands_witness.json').read_text())
    for region in regions:
        raw=data.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            raise ValueError('Villager bound actor native instructions changed')
    edits=[('quest:EntitySetThingAsAllyOfThing(uVar7, nil --[[missing]])','quest:EntitySetThingAsAllyOfThing(uVar7, me)'),
           ('quest:EntityGetSex(nil --[[missing]])','quest:EntityGetSex(me)')]
    for old,new in edits:
        if source.count(old)!=1:raise ValueError('Villager bound actor source correspondence changed')
        source=source.replace(old,new,1)
    return source,dict(status='recovered-bound-actor-operands',regions=regions,
        remaining='Cached hero wrappers and resource-derived health operands still require ownership-aware lowering.')
