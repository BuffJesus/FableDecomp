"""Recover hit and approach/talk conditions, including temporary string cleanup."""
import hashlib
import json
from pathlib import Path


def verify(data):
    witness=json.loads(Path(__file__).with_name('native_barrel_man_interactions_witness.json').read_text())
    if data.string_at(witness['literal'])!='SCRIPT_NAME_HERO':
        raise ValueError('Barrel interaction hero identity changed')
    for block in witness['blocks']:
        raw=data.bytes_at(block['address'],block['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=block['sha256']:
            raise ValueError('Barrel interaction native bytes changed')
    return witness


def recover(source,data):
    witness=verify(data)
    for block in witness['blocks']:
        if source.count(block['oldLua'])!=1:
            raise ValueError('Barrel interaction source correspondence changed')
        source=source.replace(block['oldLua'],block['newLua'])
    return source,dict(witness,status='recovered',
                      requires=['IsHitByHeroExceptAbility','IsHeroWithinBarrelApproachDistance'],
                      runtimeValidation='work/barrel_approach_runtime_checks/result.json',
                      remaining='Approach adapter is staged separately and requires host integration; whole Main remains incomplete.')
