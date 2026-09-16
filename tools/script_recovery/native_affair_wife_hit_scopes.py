"""Pin both native short-circuit hero hit scopes and their expanded Lua input."""
import hashlib
import json
from pathlib import Path


def verify(data):
    witness=json.loads(Path(__file__).with_name('native_affair_wife_hit_scopes_witness.json').read_text())
    if witness['ability']!=14 or data.string_at(witness['literal'])!='SCRIPT_NAME_HERO':
        raise ValueError('AffairWife hero-hit identity changed')
    if [(b['address'],b['size']) for b in witness['blocks']]!=[(0xDB2C15,206),(0xDB36DE,206)]:
        raise ValueError('AffairWife hero-hit scope coverage changed')
    for block in witness['blocks']:
        raw=data.bytes_at(block['address'],block['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=block['nativeSha256']:
            raise ValueError('AffairWife hero-hit bytes changed')
        if hashlib.sha256(block['oldLua'].encode()).hexdigest()!=block['oldLuaSha256']:
            raise ValueError('AffairWife hero-hit Lua correspondence changed')
    return witness
