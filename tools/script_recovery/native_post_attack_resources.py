"""Map reviewed native resources without claiming an equivalent Lua lowering."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from tools.script_recovery.native_call_setup_ir import read_call_window


def map_post_attack_resources(function, rdata):
    return _map_resources(function, rdata, 'native_post_attack_resources_witness.json')


def map_book_trader_resources(function, rdata):
    return _map_resources(function, rdata, 'native_book_trader_resources_witness.json')


def map_affair_wife_resources(function, rdata):
    return _map_resources(function, rdata, 'native_affair_wife_resources_witness.json')


def map_affair_man_resources(function, rdata):
    evidence = _map_resources(function, rdata, 'native_affair_man_resources_witness.json')
    if not evidence or evidence[0]['status'] != 'mapped':
        return evidence
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime
    witness = evidence[0]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(rdata.bytes_at(int(witness['address'],16),witness['size']),int(witness['address'],16)))
    events = {int(e['site'],16): ('start' if e['name']=='construct' else 'end' if e['name']=='destroy' else 'use', ('stack',16))
              for e in witness['events']}
    if not check_single_resource_lifetime(instructions,events):
        return [{'status':'rejected','reason':'husband resource lifetime CFG changed'}]
    witness['lifetimeStatus']='constructor/preparation/acquisition/action/task/getter/destructor paths verified'
    thing_events = {}
    for thing in witness['temporaryThings']:
        identity = tuple(thing['output'])
        for operation,site in (('start',thing['create']),('use',thing['query']),('end',thing['destroy'])):
            if site in thing_events:
                return [{'status':'rejected','reason':'overlapping temporary Thing events'}]
            thing_events[site]=(operation,identity)
        for site,profiles,count,key in ((thing['query'],{thing['create']:(4,'thing')},1,'querySetup'),
                                         (thing['destroy'],{},0,'destroySetup')):
            setup=read_call_window(rdata,int(witness['address'],16),witness['size'],site,profiles,argument_count=count)
            if setup is None or json.loads(json.dumps(asdict(setup)))!=thing[key]:
                return [{'status':'rejected','reason':'temporary Thing operands changed'}]
    if not check_single_resource_lifetime(instructions,thing_events):
        return [{'status':'rejected','reason':'temporary Thing lifetime CFG changed'}]
    witness['temporaryThingStatus']='ten getter/query/destructor lifetimes verified'
    return evidence


def _map_resources(function, rdata, witness_file):
    witness = json.loads(Path(__file__).with_name(witness_file).read_text())
    if str(function.get('address', '')).lower() != witness['address'].lower():
        return []
    def reject(reason):
        return [{'status': 'rejected', 'reason': reason}]
    if hashlib.sha256(function.get('decompile', '').encode()).hexdigest() != witness['sourceSha256']:
        return reject('resource source changed')
    for item in [witness] + witness['profiledCallees']:
        raw = rdata.bytes_at(int(item['address'], 16), item['size'])
        if raw is None or hashlib.sha256(raw).hexdigest() != item['bytesSha256']:
            return reject('native resource evidence changed')
    for address, value in witness['strings'].items():
        if rdata.string_at(int(address, 16)) != value:
            return reject('resource actor key or cutscene name changed')
    for event in witness['events']:
        profiles = {int(site, 16): tuple(profile) for site, profile in event['knownCalls'].items()}
        setup = read_call_window(rdata, int(witness['address'], 16), witness['size'],
                                 int(event['site'], 16), profiles, argument_count=event['argumentCount'])
        # JSON normalization compares nested symbolic tuples with witness arrays.
        if setup is None or json.loads(json.dumps(asdict(setup))) != event['setup']:
            return reject('resource event data flow changed: ' + event['name'])
    return [dict(witness, status='mapped', loweringStatus='unresolved')]
