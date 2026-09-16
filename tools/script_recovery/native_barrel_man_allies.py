"""Recover both directed alliances using fresh borrowed hero queries."""
import hashlib
import json
from pathlib import Path


def recover(source,data):
    witness=json.loads(Path(__file__).with_name('native_barrel_man_allies_witness.json').read_text())
    raw=data.bytes_at(witness['address'],witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=witness['sha256']:
        raise ValueError('Barrel ally native instructions changed')
    for address,value in witness['bindings'].items():
        if data.bytes_at(int(address,16),4)!=bytes.fromhex(value):
            raise ValueError('Barrel ally virtual binding changed')
    if source.count(witness['oldLua'])!=1:
        raise ValueError('Barrel ally source correspondence changed')
    return source.replace(witness['oldLua'],witness['newLua']),dict(witness,status='recovered',
        requires='SetBarrelManHeroAllies',
        semantics='Bound actor -> first borrowed hero; second borrowed hero -> bound actor. Each direction queries GetHero separately.')
