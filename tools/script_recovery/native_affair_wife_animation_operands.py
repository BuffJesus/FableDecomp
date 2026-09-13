"""Recover animation operands without freezing the writable native flag."""
import hashlib
import json
from pathlib import Path


def recover_affair_wife_animation_operands(function, source, rdata):
    w=json.loads(Path(__file__).with_name('native_affair_wife_animation_operands_witness.json').read_text())
    if str(function.get('address','')).lower()!=w['address'].lower():
        return source,[]
    def reject(reason):
        return source,[{'status':'rejected','reason':reason}]
    for value,key in ((function.get('decompile',''),'sourceSha256'),(source,'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest()!=w[key]:
            return reject(key+' changed')
    for region in w['regions']:
        raw=rdata.bytes_at(region['address'],region['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=region['sha256']:
            return reject('native animation operands changed')
    if any(source.count(e['old'])!=1 for e in w['edits']):
        return reject('animation source correspondence changed')
    for edit in w['edits']:
        source=source.replace(edit['old'],edit['new'])
    return source,[dict(w,status='recovered')]
