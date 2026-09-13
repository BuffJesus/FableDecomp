"""Recover the two reviewed presented-item name inequality operands."""
import hashlib
import json
from pathlib import Path


def recover_presented_item_comparison(function, source, rdata):
    witnesses = json.loads(Path(__file__).with_name('native_presented_item_witnesses.json').read_text())
    witness = next((w for w in witnesses if w['address'].lower() == str(function.get('address', '')).lower()), None)
    if witness is None:
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for region in witness['regions']:
        data = rdata.bytes_at(int(region['address'], 16), region['size'])
        if data is None or hashlib.sha256(data).hexdigest() != region['sha256']:
            return reject('native bytes changed at ' + region['address'])
    key = rdata.string_at(int(witness['stringAddress'], 16))
    if key != witness['itemName']:
        return reject('native item literal changed')
    old = 'CCharString__NotEqual()'
    if source.count(old) != 1:
        return reject('comparison shape changed')
    # In both reviewed sequences, slot 0x8c fills [esp+0x10]. The following push
    # supplies the literal and ECX=[esp+0x14] refers to that same output after the
    # push. 0x99E960 negates the exact byte equality routine at 0x4115A0.
    # Forge's bool-returning binding publishes this output as g_PresentedItemName.
    # No call/yield intervenes before the conditional read of that global.
    replacement = 'CCharString::NotEqual(g_PresentedItemName,' + json.dumps(key) + ')'
    return source.replace(old, replacement), [dict(witness, status='recovered',
        hostOutput='g_PresentedItemName', callee='0x0099E960')]
