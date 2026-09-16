"""Verify all controlled-resource uses and exits of retail AffairWife.Main.

This covers the owning resource and temporary health Things, not movie scopes.
"""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.native_affair_woman_resources import HELPERS
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def verify(function, data):
    witness = json.loads(Path(__file__).with_name('native_affair_wife_resource_scope_witness.json').read_text())
    start, size = witness['address'], witness['size']
    if int(function['address'], 16) != start or hashlib.sha256(function['decompile'].encode()).hexdigest() != witness['sourceSha256']:
        raise ValueError('AffairWife resource source changed')
    raw = data.bytes_at(start, size)
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['nativeSha256']:
        raise ValueError('AffairWife resource bytes changed')
    for profile in witness['profiledCallees']:
        body = data.bytes_at(int(profile['address'], 16), profile['size'])
        if body is None or hashlib.sha256(body).hexdigest() != profile['bytesSha256']:
            raise ValueError('AffairWife resource helper changed')
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    decoder.detail = True
    instructions = list(decoder.disasm(raw, start))
    inventory = {}
    for ins in instructions:
        if ins.mnemonic != 'call':
            continue
        target = int(ins.op_str, 16) if ins.op_str.startswith('0x') else 0
        name = HELPERS.get(target)
        if ins.op_str.endswith('+ 0x20]'):
            name = 'acquire'
        if name:
            inventory[ins.address] = name
    if len(witness['events']) != len(inventory) or {e['site']:e['name'] for e in witness['events']} != inventory:
        raise ValueError('AffairWife resource event coverage changed')
    events = {}
    for event in witness['events']:
        known = {int(site):tuple(value) for site,value in event['knownCalls'].items()}
        setup = read_call_window(data, start, size, event['site'], known, argument_count=event['argumentCount'])
        if setup is None or json.loads(json.dumps(asdict(setup))) != event['setup']:
            raise ValueError('AffairWife resource operands changed')
        if event['name'] == 'acquire':
            priority = 3 if event['site'] in (0xDB2BC3, 0xDB2BF4) else 4
            if setup.stack_arguments != (('register','edi'),('stack',16),('constant',priority)):
                raise ValueError('AffairWife acquisition identity or priority changed')
        elif setup.ecx != ('stack',16):
            raise ValueError('AffairWife resource identity changed')
        operation = 'start' if event['name']=='construct' else 'end' if event['name'].startswith('destroy') else 'use'
        if event['operation'] != operation:
            raise ValueError('AffairWife resource operation changed')
        events[event['site']] = (operation, ('stack',16))
    if not check_single_resource_lifetime(instructions, events):
        raise ValueError('AffairWife resource lifetime CFG changed')
    getters = {e['site']:e for e in witness['events'] if e['name']=='get_thing'}
    temporaries = witness['temporaryThings']
    if len(temporaries)!=len(getters) or {t['create'] for t in temporaries}!=set(getters):
        raise ValueError('AffairWife temporary coverage changed')
    temporary_events = {}
    for thing in temporaries:
        identity = tuple(thing['output'])
        if getters[thing['create']]['setup']['stack_arguments'] != [thing['output']]:
            raise ValueError('AffairWife temporary output changed')
        for site,known,count,key in [(thing['query'],{thing['create']:(4,'thing')},1,'querySetup'),
                                     (thing['destroy'],{},0,'destroySetup')]:
            setup = read_call_window(data,start,size,site,known,argument_count=count)
            if setup is None or json.loads(json.dumps(asdict(setup))) != thing[key]:
                raise ValueError('AffairWife temporary operands changed')
        if tuple(thing['destroySetup']['ecx']) != identity:
            raise ValueError('AffairWife temporary destructor identity changed')
        for operation,key in [('start','create'),('use','query'),('end','destroy')]:
            if thing[key] in temporary_events:
                raise ValueError('AffairWife temporary events overlap')
            temporary_events[thing[key]] = (operation,identity)
    if not check_single_resource_lifetime(instructions,temporary_events):
        raise ValueError('AffairWife temporary lifetime CFG changed')
    return dict(witness, status='verified',
                lifetime='one resource, 60 events, seven destruction joins; every constructed exit checked',
                temporaryLifetime='six controlled Thing health queries; all paths destroy each temporary',
                remaining='retained actors, movie lifetimes and resource-aware Lua lowering')
