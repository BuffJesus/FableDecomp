"""Connect reviewed native thing queries to previously recovered cached outputs."""
import hashlib
import json
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def recover_cached_thing_queries(function, source, rdata):
    witness = json.loads(Path(__file__).with_name('native_cached_thing_queries_witness.json').read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for item in (witness, witness['callee']):
        raw = rdata.bytes_at(int(item['address'], 16), item['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != item['bytesSha256']:
            return reject('native cached query changed')
    for query in witness['queries']:
        lookup = read_call_window(rdata, int(witness['address'], 16), witness['size'],
                                  int(query['lookupSite'], 16), argument_count=2)
        call = read_call_window(rdata, int(witness['address'], 16), witness['size'],
                                int(query['site'], 16), argument_count=0)
        if (lookup is None or call is None or len(lookup.stack_arguments) != 2
                or lookup.stack_arguments[0][0] != 'stack'
                or call.ecx != lookup.stack_arguments[0]
                or call.target != ('constant', int(witness['callee']['address'], 16))
                or call.stack_arguments):
            return reject('cached query receiver relationship changed')
        if (source.count(query['old']) != 1
                or source.count('GSI->GetThingWithScriptName(&' + query['slot'] + ',') != 1):
            return reject('cached query source correspondence changed')
    for query in witness['queries']:
        source = source.replace(query['old'], query['new'], 1)
    return source, [dict(witness, status='recovered')]
