"""Recover the persistent native argument conversation ID and participant."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path
from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_affair_wife_argument_conversation(function, source, rdata, manifest):
    w = json.loads(Path(__file__).with_name('native_affair_wife_argument_conversation_witness.json').read_text())
    if str(function.get('address', '')).lower() != w['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != w[key]:
            return reject(key + ' changed')
    for region in w['regions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('native conversation ID evidence changed')
    if any(manifest.get(name) != contract for name, contract in w['contracts'].items()):
        return reject('conversation contract changed')
    for event in w['events']:
        setup = read_call_window(rdata, 0xDB2B10, 4890, event['site'], argument_count=event['argumentCount'])
        if setup is None or json.loads(json.dumps(asdict(setup))) != event['setup']:
            return reject('conversation ID operands changed')
    if any(source.count(e['old']) != 1 for e in w['edits']):
        return reject('conversation source correspondence changed')
    for edit in w['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(w, status='recovered', textSequence='unresolved')]
