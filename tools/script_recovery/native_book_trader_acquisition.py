"""Recover initial priority-three acquisition operands from the resource map."""
import hashlib
import json
from pathlib import Path


def recover_book_trader_acquisition(function, source, resource_evidence):
    witness = json.loads(Path(__file__).with_name('native_book_trader_acquisition_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    if (len(resource_evidence) != 1 or resource_evidence[0].get('status') != 'mapped'
            or resource_evidence[0].get('sourceSha256') != witness['sourceSha256']):
        return reject('checked resource map unavailable')
    events = {event['site']: event for event in resource_evidence[0]['events']}
    for site in ('0xdb404d', '0xdb4078'):
        event = events.get(site, {})
        if (event.get('name') != 'acquire' or event.get('setup', {}).get('stack_arguments') !=
                [['register', 'ebp'], ['stack', 20], ['constant', 3]]):
            return reject('initial self acquisition operands changed')
    if source.count(witness['old']) != 1:
        return reject('initial acquisition source correspondence changed')
    return source.replace(witness['old'], witness['new']), [dict(witness, status='recovered',
        resourceLifetime='unresolved', scheduling='runtime AcquireControl retries internally')]
