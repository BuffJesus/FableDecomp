"""Typed packed-colour literals and reviewed native colour operand recovery.

__native_colour_bgra32 is a converter IR literal, not a retail callable API.
"""
import hashlib
import json
import re
from pathlib import Path


def colour_literal(expression):
    match = re.fullmatch(r'__native_colour_bgra32\(\s*(0x[0-9a-fA-F]{1,8})\s*\)', expression.strip())
    if not match:
        return None
    word = int(match[1], 16)
    return '{r=%d, g=%d, b=%d, a=%d}' % ((word >> 16) & 255, (word >> 8) & 255, word & 255, word >> 24)


def recover_barrel_colours(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_barrel_colours_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(int(witness['address'], 16), witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native colour branches changed')
    contract = manifest.get('ChangeQuestInfoBarColour', {})
    if (contract.get('scope') != 'Quest' or contract.get('returnType') != 'void'
            or [p.get('type') for p in contract.get('parameters', [])] != ['int', 'sol::table', 'sol::table']):
        return reject('colour API contract changed')
    if source.count(witness['original']) != 1:
        return reject('colour branch source correspondence changed')
    # Both native branches assign two identical packed colours to temporary
    # stack slots, then join at a single colour call. The reviewed slice has no
    # intervening effects; move that call into each arm and drop those temporaries.
    replacement = ('if (bVar2) {\n'
                   'GSI->ChangeQuestInfoBarColour(*(undefined4 *)(param_1 + 0x60),'
                   '__native_colour_bgra32(0xff00ff00),__native_colour_bgra32(0xff00ff00));\n'
                   '}\nelse {\n'
                   'GSI->ChangeQuestInfoBarColour(*(undefined4 *)(param_1 + 0x60),'
                   '__native_colour_bgra32(0xffff0000),__native_colour_bgra32(0xffff0000));\n}')
    return source.replace(witness['original'], replacement, 1), [dict(witness, status='recovered')]
