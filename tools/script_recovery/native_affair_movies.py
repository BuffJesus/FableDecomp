"""Lower AffairMan's two nonoverlapping, CFG-checked native movie lifetimes."""
import hashlib
import json
import re
from dataclasses import asdict
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def recover_affair_movies(function, source, rdata, manifest):
    return _recover_movies(function, source, rdata, manifest, 'native_affair_movies_witness.json')


def recover_book_trader_movies(function, source, rdata, manifest):
    return _recover_movies(function, source, rdata, manifest, 'native_book_trader_movies_witness.json')


def recover_affair_wife_movies(function, source, rdata, manifest):
    return _recover_movies(function, source, rdata, manifest, 'native_affair_wife_movies_witness.json')


def _recover_movies(function, source, rdata, manifest, witness_file):
    witness = json.loads(Path(__file__).with_name(witness_file).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return source, []
    def reject(reason):
        return source, [{'status': 'rejected', 'reason': reason}]
    for value, key in ((function.get('decompile', ''), 'sourceSha256'), (source, 'annotatedSha256')):
        if hashlib.sha256(value.encode()).hexdigest() != witness[key]:
            return reject(key + ' changed')
    for item in [witness] + witness['callees']:
        raw = rdata.bytes_at(int(item['address'], 16), item['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != item['bytesSha256']:
            return reject('native movie evidence changed')
    if (rdata.bytes_at(0x1260F0C + 0x5C8, 4) != (0x89B110).to_bytes(4, 'little')
            or rdata.bytes_at(0x122D70E, 1) != b'\0'):
        return reject('movie binding or empty class changed')
    for name in ('StartMovieSequence', 'EndMovieSequence'):
        spec = manifest.get(name, {})
        if (spec.get('scope') != 'Quest' or spec.get('returnType') != 'void'
                or spec.get('parameters') != []):
            return reject('movie API contract changed')
    events = {}
    for event in witness['events']:
        site = event['setup']['address']
        setup = read_call_window(rdata, int(witness['address'], 16), witness['setupSize'], site,
                                 argument_count=2 if event['operation'] == 'start' else 0)
        if setup is None or json.loads(json.dumps(asdict(setup))) != event['setup']:
            return reject('native movie resource operand changed')
        resource = event['resource']
        events[site] = (event['operation'], tuple(resource) if isinstance(resource, list) else resource)
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(rdata.bytes_at(int(witness['address'], 16), witness['size']),
                                      int(witness['address'], 16)))
    selections = {int(site, 16): resource for site, resource in witness.get('selections', {}).items()}
    if not check_single_resource_lifetime(instructions, events,
            receiver_register=witness.get('receiverRegister'), selections=selections):
        return reject('movie lifetime does not match single host handle')
    constructors = witness.get('constructors') or [witness['constructor']]
    if source.count('GSI->StartMovieSequence();') != witness.get('startCount', 2) or any(source.count(item) != 1 for item in constructors):
        return reject('movie construction source changed')
    if any(len(re.findall(edit['pattern'], source)) != edit['count'] for edit in witness['destructors']):
        return reject('movie destruction source changed')
    result = source
    for constructor in constructors:
        result = result.replace(constructor, '', 1)
    for edit in witness['destructors']:
        result = re.sub(edit['pattern'], 'GSI->EndMovieSequence();', result)
    return result, [dict(witness, status='recovered', lifetimeChecked=True)]
