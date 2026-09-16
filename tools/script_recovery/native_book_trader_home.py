"""Recover BookTrader's two home snapshots and native movement operands."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_book_trader_home(function, source, rdata, manifest):
    """Run immediately after recover_book_trader_acquisition; fail closed."""
    witness = json.loads(Path(__file__).with_name('native_book_trader_home_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []

    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]

    for value, key in ((function.get('decompile', ''), 'sourceSha256'),
                       (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for region in witness['regions']:
        raw = rdata.bytes_at(region['address'], region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != region['sha256']:
            return reject('native home evidence changed')
    if any(manifest.get(name) != contract for name, contract in witness['contracts'].items()):
        return reject('host home contract changed')
    # The whole-function guard also pins EBP=this+8, EDI=0, balanced stack
    # frames, and the backedge after the second snapshot. The call windows
    # independently check actor, vector, threshold, and movement placement.
    setups = []
    for call in witness['calls']:
        known = {int(site): tuple(value) for site, value in call['knownCalls'].items()}
        setup = read_call_window(rdata, int(witness['address'], 16), witness['size'],
                                 call['site'], known, argument_count=call['argumentCount'])
        if setup is None:
            return reject('native home call unavailable')
        actual = json.loads(json.dumps(asdict(setup)))
        if any(actual.get(key) != value for key, value in call['expected'].items()):
            return reject('native home operands changed')
        setups.append(actual)
    if any(source.count(edit['old']) != edit['count'] for edit in witness['edits']):
        return reject('home source correspondence changed')
    for edit in witness['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [dict(witness, status='recovered', callSetups=setups,
        resourceLifetime='unresolved; native controlled-actor temporary cleanup retained',
        loweringStatus='two home snapshots, self actor, distances and five movement operands recovered')]
