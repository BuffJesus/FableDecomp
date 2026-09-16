"""Recover Scythe's native counted, cloned bound-alive condition."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData


def recover(source, rdata=None):
    rdata = rdata or RData()
    root = Path(__file__).parent
    witness = json.loads((root/'scythe_condition_witness.json').read_text())
    for region in witness['regions']:
        data = rdata.bytes_at(int(region['address'],16),region['size'])
        if data is None or hashlib.sha256(data).hexdigest() != region['sha256']:
            raise ValueError('Scythe alive condition native evidence changed')
    for file, key in [('scythe_condition_host_snapshot.h','hostSha256'),
                      ('scythe_alive_registration.inc','bindingSha256')]:
        if hashlib.sha256((root/file).read_text().encode()).hexdigest() != witness[key]:
            raise ValueError('Scythe alive condition checked host changed')
    if hashlib.sha256(source.encode()).hexdigest() != witness['sourceSha256']:
        raise ValueError('Scythe alive condition Lua correspondence changed')
    old = '    local alive = true\n    alive = quest:NewScriptFrame(me)'
    if source.count(old) != 1:
        raise ValueError('Scythe alive condition entry changed')
    return source.replace(old, '    quest:RegisterBoundAliveCondition()\n' + old), dict(witness,status='recovered')
