"""Recover reviewed operands that the decompiler attached to an adjacent nested call."""
import hashlib
import json
import re
from pathlib import Path


WITNESSES = Path(__file__).with_name('native_call_operand_witnesses.json')


def recover_reviewed_call_operands(function, annotated, read_bytes):
    witnesses = json.loads(WITNESSES.read_text(encoding='utf-8'))['witnesses']
    matches = [w for w in witnesses if w['functionAddress'].lower() == str(function.get('address', '')).lower()]
    if not matches:
        return annotated, []
    if len(matches) != 1:
        return annotated, [{'status': 'rejected', 'reason': 'ambiguous operand witnesses'}]
    witness = matches[0]
    evidence = {'id': witness['id'], 'status': 'rejected'}

    def reject(reason):
        return annotated, [dict(evidence, reason=reason)]

    for source, key in ((function.get('decompile') or '', 'decompileSha256'), (annotated, 'annotatedSha256')):
        if hashlib.sha256(source.encode()).hexdigest() != witness[key]:
            return reject('source changed: ' + key)
    for region in witness['nativeRegions']:
        data = read_bytes(int(region['address'], 16), region['size'])
        if data is None or hashlib.sha256(data).hexdigest() != region['sha256']:
            return reject('native bytes unavailable or changed: ' + region['name'])
    for slot in witness['vtableSlots']:
        data = read_bytes(int(slot['address'], 16), 4)
        if data is None or len(data) != 4 or int.from_bytes(data, 'little') != int(slot['target'], 16):
            return reject('native target unavailable or changed: ' + slot['name'])
    # This reviewed case is deliberately narrower than a general argument propagation heuristic.
    pattern = (r'GSI->GetActiveQuestName\(&stack0xfffffff4,0\);\s*'
               r'GSI->DeactivateQuestLater\(\);')
    if len(list(re.finditer(pattern, annotated))) != 1:
        return reject('nested getter/deactivation shape changed')
    result = re.sub(pattern, 'GSI->GetActiveQuestName(&stack0xfffffff4);\n'
                    '      GSI->DeactivateQuestLater(ppVar5,0);', annotated)
    return result, [dict(witness, status='recovered')]
