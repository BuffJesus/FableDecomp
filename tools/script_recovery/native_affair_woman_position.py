"""Recover the run-off point snapshot and reviewed movement setup operands."""
import hashlib
import json
from pathlib import Path


def recover_affair_woman_position(function, source, rdata, manifest):
    w = json.loads(Path(__file__).with_name('native_affair_woman_position_witness.json').read_text())
    if str(function.get('address', '')).lower() != w['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != w[key]:
            return reject(key + ' changed')
    raw = rdata.bytes_at(int(w['address'], 16), w['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != w['nativeSha256']:
        return reject('native run-off point or movement changed')
    if rdata.string_at(int(w['string']['address'], 16)) != w['string']['value']:
        return reject('native run-off point name changed')
    for name in ('EntitySetAsUseMovementInActions', 'SetIsPushableByHero'):
        spec = manifest.get(name, {})
        if (spec.get('scope') != 'Quest' or spec.get('returnType') != 'void'
                or [p.get('type') for p in spec.get('parameters', [])] !=
                ['const std::shared_ptr<CScriptThing>&', 'bool']):
            return reject(name + ' contract changed')
    if any(source.count(e['old']) != e['count'] for e in w['edits']):
        return reject('run-off position source correspondence changed')
    for edit in w['edits']:
        source = source.replace(edit['old'], edit['new'])
    return source, [{'status': 'recovered', 'nativeSha256': w['nativeSha256'],
        'sites': w['nativeSites'], 'runtimeRequirement': 'proposed RetailThingPosition global binding',
        'remaining': 'resource Thing distance queries, ownership, camera/removal and other Main branches remain unresolved'}]
