"""Recover reviewed timer/bar call boundaries and typed update arguments."""
import hashlib
import json
import struct
from dataclasses import asdict
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_barrel_update(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_barrel_update_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    start, size = int(witness['address'], 16), witness['size']
    raw = rdata.bytes_at(start, size)
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native timer/update sequence changed')
    for name, result, types in (('GetTimer', 'int', ['int']),
                                ('UpdateQuestInfoBar', 'void', ['int', 'float', 'float', 'float'])):
        contract = manifest.get(name, {})
        if (contract.get('scope') != 'Quest' or contract.get('returnType') != result
                or [p.get('type') for p in contract.get('parameters', [])] != types):
            return reject(name + ' contract changed')
    # Without an ABI argument count this snapshot includes all staged pushes:
    # one belongs to GetTimer, the older two belong to the following update.
    setup = read_call_window(rdata, start, size, int(witness['timerSite'], 16))
    expected = (('memory', ('address', ('register', 'esi'), 0x108)),
                ('constant', 0xbf800000), ('constant', 0xbf800000))
    if setup is None or setup.stack_arguments != expected:
        return reject('staged timer/update operands unresolved')
    defaults = [struct.unpack('<f', struct.pack('<I', item[1]))[0]
                for item in setup.stack_arguments[1:]]
    original = witness['original']
    if source.count(original) != 1 or 'native_bar_timer_value' in source:
        return reject('timer/update source correspondence changed')
    # Retail FILD/FSTP converts signed int32 to float32 here. Forge's inspected
    # UpdateQuestInfoBar(int,float,float,float) binding performs that conversion
    # at the Lua/API boundary, so the readable Lua can retain the integer local.
    replacement = ('native_bar_timer_value = GSI->GetTimer(*(undefined4 *)(param_1 + 0x108));\n'
                   'GSI->UpdateQuestInfoBar(*(undefined4 *)(param_1 + 0x60),'
                   f'native_bar_timer_value,{defaults[0]},{defaults[1]});')
    return source.replace(original, replacement, 1), [dict(witness, status='recovered',
            timerSetup=asdict(setup), defaults=defaults, conversion='signed int32 to float32 at Forge binding')]
