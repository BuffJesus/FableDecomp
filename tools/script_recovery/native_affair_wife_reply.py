"""Recover the byte-reviewed optional husband reply, after wife text recovery."""
import hashlib
import json
from pathlib import Path


def recover_affair_wife_reply(function, source, rdata, manifest):
    w = json.loads(Path(__file__).with_name('native_affair_wife_reply_witness.json').read_text())
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
            return reject('native reply evidence changed')
    if manifest.get('AddLineToConversation') != w['contract']:
        return reject('dialogue contract changed')
    if source.count(w['old']) != 1:
        return reject('reply source correspondence changed')
    return source.replace(w['old'], w['new']), [dict(w, status='recovered')]
