"""Recover unpause calls through AffairMan's reviewed saved game interface."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_affair_pause(function, source, rdata, manifest):
    return _recover_pause(function, source, rdata, manifest, 'native_affair_pause_witness.json')


def recover_book_trader_pause(function, source, rdata, manifest):
    return _recover_pause(function, source, rdata, manifest, 'native_book_trader_pause_witness.json')


def recover_affair_wife_pause(function, source, rdata, manifest):
    return _recover_pause(function, source, rdata, manifest, 'native_affair_wife_pause_witness.json')


def _recover_pause(function, source, rdata, manifest, witness_file):
    witness = json.loads(Path(__file__).with_name(witness_file).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for item in (witness, witness['callee']):
        raw = rdata.bytes_at(int(item['address'], 16), item['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != item['bytesSha256']:
            return reject('native saved-interface evidence changed')
    if rdata.bytes_at(0x1260F0C + 0x5EC, 4) != (0x890AB0).to_bytes(4, 'little'):
        return reject('native unpause binding changed')
    spec = manifest.get('PauseAllNonScriptedEntities', {})
    if (spec.get('scope') != 'Quest' or spec.get('returnType') != 'void'
            or [p.get('type') for p in spec.get('parameters', [])] != ['bool']):
        return reject('unpause API contract changed')
    receiver = ('memory', ('stack', 56))
    target = ('memory', ('address', ('memory', ('address', receiver, 0)), 0x5EC))
    for site in witness['checkedSites']:
        setup = read_call_window(rdata, int(witness['address'], 16), witness['size'], int(site, 16), argument_count=1)
        if (setup is None or setup.ecx != receiver or setup.target != target
                or setup.stack_arguments != (('constant', 0),)):
            return reject('saved-interface unpause operands changed')
    for expected in witness.get('checkedSetups', []):
        setup = read_call_window(rdata, int(witness['address'], 16), witness['size'],
                                 expected['address'], argument_count=1)
        if setup is None or json.loads(json.dumps(asdict(setup))) != expected:
            return reject('saved-interface pause operands changed')
    edits = witness.get('edits') or [{'old': witness['old'], 'count': witness['count'],
                                      'new': 'GSI->PauseAllNonScriptedEntities(false);'}]
    if any(source.count(edit['old']) != edit['count'] for edit in edits):
        return reject('unpause source correspondence changed')
    for edit in edits:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered')]
