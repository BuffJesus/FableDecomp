"""Lower the verified movie lifetime, leaving actor resources unresolved."""
import hashlib
import json
from pathlib import Path

from tools.script_recovery.native_post_attack_resources import map_post_attack_resources


def recover_post_attack_movie_lifetime(function, source, rdata, manifest):
    witness = json.loads(Path(__file__).with_name('native_post_attack_movie_lifetime_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for text, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(text.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for item in witness['callees']:
        raw = rdata.bytes_at(int(item['address'], 16), item['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != item['bytesSha256']:
            return reject('native movie lifetime helper changed')
    graph = map_post_attack_resources(function, rdata)
    if not graph or graph[0]['status'] != 'mapped':
        return reject('native resource graph unavailable')
    events = {e['name']: e['setup'] for e in graph[0]['events']}
    if (events['movie_construct']['ecx'] != ['stack', 56]
            or events['movie_start']['stack_arguments'] != [['stack', 12], ['stack', 56]]
            or events['movie_destroy']['ecx'] != ['stack', 56]
            or not (events['movie_construct']['address'] < events['movie_start']['address']
                    < events['movie_destroy']['address'] < events['map_destroy']['address']
                    < events['actor_destroy']['address'])):
        return reject('movie resource or cleanup order changed')
    if rdata.bytes_at(0x122D70E, 1) != b'\0':
        return reject('movie class is no longer empty')
    for name in ('StartMovieSequence', 'EndMovieSequence'):
        contract = manifest.get(name, {})
        if (contract.get('scope') != 'Quest' or contract.get('returnType') != 'void'
                or contract.get('parameters') != []):
            return reject('movie lifetime API contract changed')
    if any(source.count(witness[key]) != 1 for key in ('constructor', 'destructor')):
        return reject('movie lifetime source correspondence changed')
    if source.count('GSI->StartMovieSequence();') != 1:
        return reject('movie start source correspondence changed')
    result = source.replace(witness['constructor'], '', 1)
    result = result.replace(witness['destructor'], 'GSI->EndMovieSequence();', 1)
    return result, [dict(witness, status='recovered', resource='movie',
                         nativeStart=events['movie_start']['address'], nativeEnd=events['movie_destroy']['address'])]
