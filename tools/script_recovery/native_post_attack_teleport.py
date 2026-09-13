"""Recover the post-attack teleport's staged lookup and hero results."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_post_attack_teleport(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_post_attack_teleport_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for item in [witness] + witness['callees']:
        raw = rdata.bytes_at(int(item['address'], 16), item['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != item['bytesSha256']:
            return reject('native teleport evidence changed')
    contract = manifest.get('EntityTeleportToThing', {})
    if (contract.get('scope') != 'Quest' or contract.get('returnType') != 'void'
            or [p.get('type') for p in contract.get('parameters', [])] !=
                ['CScriptThing*', 'CScriptThing*', 'sol::optional<bool>']):
        return reject('teleport API contract changed')
    for slot, address in ((0x120, 0x8A7D60), (0x118, 0x891CA0), (0x760, 0x88E540)):
        if rdata.bytes_at(0x1260F0C + slot, 4) != address.to_bytes(4, 'little'):
            return reject('teleport dependency vtable changed')
    receiver = ('memory', ('address', ('register', 'esi'), 0x40))
    vtable = ('memory', ('address', receiver, 0))
    if rdata.string_at(0x12D9E04) != 'M_PostAttackStart':
        return reject('teleport lookup name changed')
    for site, slot, known, count in ((0xDBEC16, 0x120, {}, 2),
                                     (0xDBEC21, 0x118, {0xDBEC16: (8, 'lookup')}, 0)):
        dependency = read_call_window(rdata, 0xDBEB20, witness['size'], site, known, argument_count=count)
        if (dependency is None or dependency.ecx != receiver
                or dependency.target != ('memory', ('address', vtable, slot))):
            return reject('teleport dependency data flow changed')
    setup = read_call_window(rdata, 0xDBEB20, witness['size'], 0xDBEC2B,
                             {0xDBEC16: (8, 'lookup'), 0xDBEC21: (0, 'hero')}, argument_count=3)
    if (setup is None or setup.ecx != receiver
            or setup.target != ('memory', ('address', vtable, 0x760))
            or setup.stack_arguments != (('result', 0xDBEC21, 'hero'),
                                         ('result', 0xDBEC16, 'lookup'), ('constant', 0))):
        return reject('teleport operand data flow changed')
    if source.count(witness['original']) != 1:
        return reject('teleport source correspondence changed')
    replacement = ('    CVar4 = GSI->GetThingWithScriptName();\n'
                   '    ppVar6 = GSI->GetHero();\n'
                   '    GSI->EntityTeleportToThing(ppVar6,CVar4,0);\n')
    return source.replace(witness['original'], replacement, 1), [dict(witness, status='recovered', setup=asdict(setup))]
