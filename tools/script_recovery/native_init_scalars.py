"""Recover Init's typed timer ID and signed GUI-counter sentinel."""
import hashlib
import json
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_init_scalars(function, source, rdata, state, manifest):
    witness = json.loads(Path(__file__).with_name('native_init_scalars_witness.json').read_text())
    if str(function.get('address', '')).lower() != '0x00daadd0':
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(0xDAADD0, 3306)
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native Init scalar evidence changed')
    if (state.get('0x104') != ('TalkIntermittentTimer', 'Int')
            or state.get('0x64') != ('GUIBullyHealthCounter', 'Int')):
        return reject('typed Init state mapping changed')
    if manifest.get('SetTimer') != witness['timerContract']:
        return reject('timer API contract changed')
    setup = read_call_window(rdata, 0xDAADD0, 3306, 0xDAAE2A, argument_count=2)
    receiver = ('memory', ('constant', 0x143E8F8))
    target = ('memory', ('address', ('memory', ('address', receiver, 0)), 0x164))
    if (setup is None or setup.ecx != receiver or setup.target != target
            or setup.stack_arguments != (('memory', ('address', ('register', 'ecx'), 0x104)), ('constant', 0))):
        return reject('Init timer operands changed')
    if any(source.count(edit['old']) != 1 for edit in witness['edits']):
        return reject('Init scalar correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered')]


def recover_init_bad_deeds(function, source, rdata, state_arrays):
    """Expand native four-byte plus one-byte zero stores into five bool elements."""
    witness = json.loads(Path(__file__).with_name('native_init_bad_deeds_witness.json').read_text())
    if str(function.get('address', '')).lower() != '0x00daadd0':
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(0xDAADD0, 3306)
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['bytesSha256']:
        return reject('native packed bool initialization changed')
    if state_arrays.get('0xfc') != ('WhichBadDeedsPerformed', 'Bool'):
        return reject('bad-deed array mapping changed')
    if source.count(witness['old']) != 1:
        return reject('packed bool source correspondence changed')
    return source.replace(witness['old'], witness['new']), [dict(witness, status='recovered', elements=5)]
