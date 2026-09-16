"""Verify all named marker lookup, name-string and cleanup boundaries."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_barrel_man_resources import verify as verify_resource
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_bounded_switch import resolve
from tools.script_recovery.native_resource_lifetime import check_resource_lifetimes
from tools.script_recovery.native_barrel_man_inline_cleanup import verify as verify_inline_cleanup


def verify(function,data,witness=None):
    resource=verify_resource(function,data)
    inline_cleanup=verify_inline_cleanup(data)
    w=witness or json.loads(Path(__file__).with_name('native_barrel_man_markers_witness.json').read_text())
    start,size=resource['address'],resource['size'];decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(data.bytes_at(start,size),start));at={i.address:i for i in instructions}
    lookups={i.address for i in instructions if i.mnemonic=='call' and i.op_str.endswith('+ 0x120]')}
    if len(w['markers'])!=len(lookups) or {m['create'] for m in w['markers']}!=lookups:
        raise ValueError('Barrel named lookup coverage changed')
    base=w['baseDestructor']
    if (base['address'],base['size'])!=(0x99A2E0,7) or hashlib.sha256(data.bytes_at(base['address'],base['size'])).hexdigest()!=base['sha256']:
        raise ValueError('Barrel marker base destructor changed')
    events={};string_events={};slots=set()
    for marker in w['markers']:
        identity=tuple(marker['identity']);slots.add(identity)
        setup=read_call_window(data,start,size,marker['create'],{},argument_count=2)
        if setup is None or json.loads(json.dumps(asdict(setup)))!=marker['setup'] or setup.stack_arguments[0]!=identity:
            raise ValueError('Barrel marker lookup operands changed')
        if data.string_at(marker['literal'])!=marker['name']:
            raise ValueError('Barrel marker name changed')
        string=marker['string'];string_identity=tuple(string['identity'])
        if setup.stack_arguments[1]!=string_identity or string['use']!=marker['create']:
            raise ValueError('Barrel marker name-string receiver changed')
        for key,count,record in (('create',2,'constructSetup'),('destroy',0,'destroySetup')):
            value=read_call_window(data,start,size,string[key],{},argument_count=count)
            if value is None or json.loads(json.dumps(asdict(value)))!=string[record] or value.ecx!=string_identity:
                raise ValueError('Barrel marker name-string setup changed')
        if string['constructSetup']['stack_arguments']!=[['constant',marker['literal']],['constant',0xFFFFFFFF]]:
            raise ValueError('Barrel marker name literal changed')
        events[marker['create']]=('start',identity)
        for operation,key in (('start','create'),('use','use'),('end','destroy')):
            if string[key] in string_events:raise ValueError('Barrel marker string scopes overlap')
            string_events[string[key]]=(operation,string_identity)
    expected_ends=set()
    for ins in instructions:
        if ins.mnemonic=='call' and ins.op_str in ('0x4aa840','0x99a2e0'):
            setup=read_call_window(data,start,size,ins.address,{},argument_count=0)
            if setup and (setup.ecx in slots or ins.address in (0xDB572A,0xDB6A32)):expected_ends.add(ins.address)
    if len(w['ends'])!=len(expected_ends) or {e['site'] for e in w['ends']}!=expected_ends:
        raise ValueError('Barrel marker cleanup coverage changed')
    for end in w['ends']:
        setup=read_call_window(data,start,size,end['site'],{},argument_count=0)
        if setup is None or json.loads(json.dumps(asdict(setup)))!=end['setup'] or setup.ecx!=tuple(end['identity']):
            raise ValueError('Barrel marker cleanup receiver changed')
        events[end['site']]=('end',tuple(end['identity']))
    selections={int(k):tuple(v) for k,v in w['selections'].items()}
    for site,identity in selections.items():
        if identity[0]!='stack' or site not in at or at[site].mnemonic!='lea' or at[site].op_str!=f'ecx, [esp + {hex(identity[1])}]':
            raise ValueError('Barrel marker cleanup selection changed')
    branches=resolve(instructions,data)
    if (not check_resource_lifetimes(instructions,events,receiver_register='ecx',selections=selections,indirect_branches=branches)
            or not check_resource_lifetimes(instructions,string_events,indirect_branches=branches)):
        raise ValueError('Barrel marker/string lifetime CFG changed')
    return dict(w,status='verified-lookup-cleanup-boundaries',owningResourceSha256=resource['nativeSha256'],
                inlineCleanup=inline_cleanup,
                remaining='All marker query/position uses require further review before Lua lowering; inline destructor equivalence is verified separately.')
