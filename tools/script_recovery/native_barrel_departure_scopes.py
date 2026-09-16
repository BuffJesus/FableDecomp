"""Replace departure convenience lookups with the reviewed atomic owned scopes."""
import hashlib
import json
from pathlib import Path


def recover(source,data):
    w=json.loads(Path(__file__).with_name('native_barrel_departure_scopes_witness.json').read_text())
    raw=data.bytes_at(w['address'],w['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['sha256']:
        raise ValueError('Barrel departure scope instructions changed')
    for address,value in w['strings'].items():
        if data.string_at(int(address,16))!=value:raise ValueError('Barrel departure marker changed')
    if source.count(w['oldLua'])!=1:raise ValueError('Barrel departure scope source correspondence changed')
    return source.replace(w['oldLua'],'            resources:TeleportBarrelDepartureActors(me)\n'),dict(w,status='recovered',
        semantics='Name construct -> marker lookup -> optional fresh borrowed hero -> teleport -> marker destroy -> name destroy, separately for guard and hidden markers.',
        requires='TeleportBarrelDepartureActors')
