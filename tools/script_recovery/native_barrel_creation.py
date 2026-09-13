"""Recover the reviewed seven native timer-bar creation arguments."""
import hashlib
import json
from pathlib import Path


def recover_barrel_creation(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_barrel_creation_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(int(witness['address'], 16), witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native bar construction changed')
    for address, value in witness['strings'].items():
        matches = (rdata.bytes_at(int(address, 16), 1) == b'\0' if value == '' else
                   rdata.string_at(int(address, 16)) == value)
        if not matches:
            return reject('native bar string changed')
    contract = manifest.get('AddQuestInfoBar', {})
    if (contract.get('scope') != 'Quest' or contract.get('returnType') != 'int'
            or [p.get('type') for p in contract.get('parameters', [])] !=
                ['float', 'float', 'sol::table', 'sol::table', 'const std::string&', 'const std::string&', 'float']):
        return reject('bar creation contract changed')
    if source.count(witness['original']) != 1:
        return reject('bar construction source correspondence changed')
    # Native pushes right-to-left: scale 1, empty text, HUD texture, two
    # green BGRA references, max 0, current 45. The two string temporaries
    # are destroyed after the native call; Lua supplies immutable strings.
    replacement = ('uVar6 = GSI->AddQuestInfoBar(45.0,0.0,'
                   '__native_colour_bgra32(0xff00ff00),__native_colour_bgra32(0xff00ff00),'
                   '"HUD_CLOCK_ICON","",1.0);\n'
                   '*(int *)(param_1 + 0x60) = (int)uVar6;\n')
    return source.replace(witness['original'], replacement, 1), [dict(witness, status='recovered')]
