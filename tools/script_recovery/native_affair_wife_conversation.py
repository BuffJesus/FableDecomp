"""Recover the reviewed Where's Husband conversation operands."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path
from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_affair_wife_conversation(function, source, rdata, manifest):
    return _recover_conversation(function, source, rdata, manifest, 'native_affair_wife_conversation_witness.json')


def recover_affair_wife_running_line(function, source, rdata, manifest):
    return _recover_conversation(function, source, rdata, manifest, 'native_affair_wife_running_line_witness.json')


def _recover_conversation(function, source, rdata, manifest, filename):
    w = json.loads(Path(__file__).with_name(filename).read_text())
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
            return reject('native conversation evidence changed')
    if rdata.string_at(w.get('textAddress', 0x12D8DAC)) != w['textKey']:
        return reject('conversation text changed')
    if any(manifest.get(n) != c for n, c in w['contracts'].items()):
        return reject('conversation contract changed')
    for event in w['events']:
        profiles = {event['hero']: (0, 'hero')} if 'hero' in event else {}
        setup = read_call_window(rdata, 0xDB2B10, 4890, event['site'], profiles,
                                 argument_count=event['argumentCount'])
        if setup is None or json.loads(json.dumps(asdict(setup))) != event['setup']:
            return reject('conversation operand flow changed')
    if source.count(w['old']) != 1:
        return reject('conversation source changed')
    return source.replace(w['old'], w['new']), [dict(w, status='recovered')]
