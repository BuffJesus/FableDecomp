"""Connect reviewed cached entities to native quest-marker operations."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_quest_markers(function, source, rdata):
    witness = json.loads(Path(__file__).with_name('native_quest_markers_witness.json').read_text())
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
        return reject('native marker body changed')
    for actor in witness['actors'].values():
        if rdata.string_at(int(actor['stringAddress'], 16)) != actor['scriptName']:
            return reject('native actor string changed')
        if actor['output'] in source:
            return reject('generated output slot collision')
    if rdata.string_at(int(witness['textureAddress'], 16)) != witness['texture']:
        return reject('native marker texture changed')
    result, evidence = source, []
    for call in witness['calls']:
        actor = witness['actors'][call['actor']]
        setup = read_call_window(rdata, start, size, int(call['site'], 16))
        gsi = ('memory', ('address', ('register', 'esi'), 0x40))
        target = ('memory', ('address', ('memory', ('address', gsi, 0)), call['slot']))
        expected = (('stack', actor['offset']),)
        if call['slot'] != 0x580:
            expected += (('stack', 8),)
        if setup is None or setup.ecx != gsi or setup.target != target or setup.stack_arguments != expected:
            return reject('native marker operand setup changed at ' + call['site'])
        output = actor['output']
        if call['slot'] == 0x120:
            replacement = f'GSI->GetThingWithScriptName(&{output},{json.dumps(actor["scriptName"])});'
        elif call['slot'] == 0x578:
            replacement = f'GSI->MiniMapAddMarker({output},{json.dumps(witness["texture"])});'
        else:
            replacement = f'GSI->MiniMapRemoveMarker({output});'
        if call['original'] not in result:
            return reject('native marker source correspondence changed')
        result = result.replace(call['original'], replacement, 1)
        evidence.append(dict(call, status='recovered', entity=actor['scriptName'], setup=asdict(setup)))
    return result, evidence
