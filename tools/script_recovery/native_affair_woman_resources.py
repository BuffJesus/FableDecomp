"""Verify every controlled-resource call and lifetime path in AffairWoman.Main."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime

HELPERS = {0x99A380:'construct', 0x99A430:'destroy_inline', 0xCD23B9:'has',
    0xCD2770:'reset', 0x7E7490:'get_thing', 0x7E7390:'speak', 0x7E7450:'task',
    0x7E72F0:'move', 0x7E73D0:'animate', 0x7E74D0:'destroy',
    0x7E7400:'clear_actions', 0x7E7360:'clear_commands'}


def verify(function, data):
    witness = json.loads(Path(__file__).with_name('native_affair_woman_resources_witness.json').read_text())
    if int(function['address'],16) != witness['address'] or hashlib.sha256(function['decompile'].encode()).hexdigest() != witness['sourceSha256']:
        raise ValueError('AffairWoman resource source changed')
    raw = data.bytes_at(witness['address'],witness['size'])
    if raw is None or hashlib.sha256(raw).hexdigest() != witness['nativeSha256']:
        raise ValueError('AffairWoman resource bytes changed')
    for callee in witness['profiledCallees']:
        body = data.bytes_at(int(callee['address'],16),callee['size'])
        if body is None or hashlib.sha256(body).hexdigest() != callee['bytesSha256']:
            raise ValueError('AffairWoman resource helper bytes changed')
    decoder = Cs(CS_ARCH_X86,CS_MODE_32); decoder.detail = True
    instructions = list(decoder.disasm(raw,witness['address']))
    inventory = {}
    for ins in instructions:
        if ins.mnemonic != 'call':
            continue
        target = int(ins.op_str,16) if ins.op_str.startswith('0x') else 0
        if target in HELPERS:
            inventory[ins.address] = HELPERS[target]
        elif ins.op_str.endswith('+ 0x20]'):
            inventory[ins.address] = 'acquire'
    if len(witness['events']) != len(inventory) or {e['site']:e['name'] for e in witness['events']} != inventory:
        raise ValueError('AffairWoman resource event coverage changed')
    events = {}
    for event in witness['events']:
        known = {int(site):tuple(value) for site,value in event['knownCalls'].items()}
        setup = read_call_window(data,witness['address'],witness['size'],event['site'],known,argument_count=event['argumentCount'])
        if setup is None or json.loads(json.dumps(asdict(setup))) != event['setup']:
            raise ValueError('AffairWoman resource operands changed')
        if event['name'] == 'acquire':
            if setup.stack_arguments != (('register','ebp'),('stack',16),('constant',4)):
                raise ValueError('AffairWoman acquisition identity changed')
        elif setup.ecx != ('stack',16):
            raise ValueError('AffairWoman resource identity changed')
        operation = 'start' if event['name']=='construct' else 'end' if event['name'].startswith('destroy') else 'use'
        if operation != event['operation']:
            raise ValueError('AffairWoman resource operation changed')
        events[event['site']] = (operation,('stack',16))
    if not check_single_resource_lifetime(instructions,events):
        raise ValueError('AffairWoman resource lifetime CFG changed')
    temporary_events = {}
    getters = {e['site']:e for e in witness['events'] if e['name']=='get_thing'}
    if {t['create'] for t in witness['temporaryThings']} != set(getters) or len(witness['temporaryThings']) != len(getters):
        raise ValueError('AffairWoman temporary getter coverage changed')
    for thing in witness['temporaryThings']:
        identity = tuple(thing['output'])
        if getters[thing['create']]['setup']['stack_arguments'] != [thing['output']]:
            raise ValueError('AffairWoman temporary output changed')
        for site,profiles,count,key in [(thing['query'],{thing['create']:(4,'thing')},1,'querySetup'),
                                        (thing['destroy'],{},0,'destroySetup')]:
            setup = read_call_window(data,witness['address'],witness['size'],site,profiles,argument_count=count)
            if setup is None or json.loads(json.dumps(asdict(setup))) != thing[key]:
                raise ValueError('AffairWoman temporary operands changed')
        if tuple(thing['destroySetup']['ecx']) != identity:
            raise ValueError('AffairWoman temporary destructor identity changed')
        for operation,key in [('start','create'),('use','query'),('end','destroy')]:
            site = thing[key]
            if site in temporary_events:
                raise ValueError('AffairWoman temporary events overlap')
            temporary_events[site] = (operation,identity)
    if not check_single_resource_lifetime(instructions,temporary_events):
        raise ValueError('AffairWoman temporary lifetime CFG changed')
    return dict(witness,status='verified',lifetime='one constructor; all39 uses/events and every constructed exit checked',
        temporaryLifetime='five getter/query/destruction lifetimes checked across the full CFG',
        remaining='movies, named-Thing lifetimes and Lua lowering')
