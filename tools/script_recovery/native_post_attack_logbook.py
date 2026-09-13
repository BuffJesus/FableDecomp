"""Recover the numeric logbook API obscured by an incorrect BSim prototype."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_post_attack_logbook(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_post_attack_logbook_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for item in (witness, witness['callee']):
        raw = rdata.bytes_at(int(item['address'], 16), item['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != item['bytesSha256']:
            return reject('native logbook evidence changed')
    contract = manifest.get('AddLogbookStoryEntry', {})
    if (contract.get('scope') != 'Quest' or contract.get('returnType') != 'void'
            or [p.get('type') for p in contract.get('parameters', [])] != ['int|string']):
        return reject('numeric logbook API contract changed')
    setup = read_call_window(rdata, int(witness['address'], 16), witness['size'],
                             int(witness['site'], 16), argument_count=0)
    if (setup is None or setup.target != ('constant', int(witness['callee']['address'], 16))
            or setup.ecx != ('constant', witness['value']) or setup.stack_arguments != ()):
        return reject('numeric logbook operand data flow changed')
    if source.count(witness['original']) != 1:
        return reject('logbook source correspondence changed')
    result = source.replace(witness['original'], f"GSI->AddLogbookStoryEntry({witness['value']});", 1)
    return result, [dict(witness, status='recovered', setup=asdict(setup))]
