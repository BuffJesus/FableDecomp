"""Recover reviewed distance operands staged before a nested GetHero call."""
import hashlib
import json
import struct
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_affair_man_hero_distances(function, source, rdata):
    return recover_affair_wife_hero_distances(function, source, rdata,
        witness_file='native_affair_man_hero_distance_witness.json')


def recover_affair_wife_hero_distances(function, source, rdata, *,
                                     witness_file='native_affair_wife_hero_distance_witness.json'):
    witness = json.loads(Path(__file__).with_name(witness_file).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for region in witness['regions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('native hero distance evidence changed')
    receiver = ('memory', ('address', ('register', 'esi'), 4))
    target = ('memory', ('address', ('memory', ('address', receiver, 0)), 0x118))
    address=int(witness['address'],16)
    size=witness.get('size',4887)
    for check in witness['checks']:
        hero = read_call_window(rdata, address, size, check['hero'], argument_count=0)
        distance = read_call_window(rdata, address, size, check['distance'],
                                   {check['hero']: (0, 'hero')}, argument_count=1)
        word = struct.unpack('<I', struct.pack('<f', check['threshold']))[0]
        if hero is None or hero.ecx != receiver or hero.target != target:
            return reject('native hero receiver changed')
        if (distance is None or distance.target != ('constant', 0xCBE2FF)
                or distance.ecx != ('register', witness.get('selfRegister','edi')) or distance.edx != ('result', check['hero'], 'hero')
                or distance.stack_arguments != (('constant', word),)):
            return reject('native hero distance operands changed')
    if any(source.count(edit['old']) != 1 for edit in witness['edits']):
        return reject('hero distance source correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered')]
