"""Check all Barrel Man movie/pause scopes through the bounded phase switch."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime
from tools.script_recovery.native_bounded_switch import resolve


def verify(function,data,witness=None):
    w=witness or json.loads(Path(__file__).with_name('native_barrel_man_movies_witness.json').read_text())
    if int(function['address'],16)!=w['address'] or hashlib.sha256(function['decompile'].encode()).hexdigest()!=w['sourceSha256']:
        raise ValueError('BarrelMan movie source changed')
    raw=data.bytes_at(w['address'],w['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['nativeSha256']:
        raise ValueError('BarrelMan movie bytes changed')
    for offset,target in w['bindings'].items():
        if data.bytes_at(0x1260F0C+int(offset,16),4)!=target.to_bytes(4,'little'):
            raise ValueError('BarrelMan movie/pause binding changed')
    if data.bytes_at(0x122D70E,1)!=b'\0':
        raise ValueError('BarrelMan empty movie class changed')
    for callee in w['callees']:
        raw_callee=data.bytes_at(int(callee['address'],16),callee['size'])
        if raw_callee is None or hashlib.sha256(raw_callee).hexdigest()!=callee['bytesSha256']:
            raise ValueError('BarrelMan movie helper changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(raw,w['address']))
    branches=resolve(instructions,data)
    expected={};pause_sites=set()
    for ins in instructions:
        if ins.mnemonic!='call':continue
        if ins.address==0xDB5F8E or ins.op_str in ('0x6e7b60','0x6e7b80') or ins.op_str.endswith('+ 0x5c8]'):
            expected[ins.address]='construct' if ins.address==0xDB5F8E or ins.op_str=='0x6e7b60' else 'destroy' if ins.op_str=='0x6e7b80' else 'start_movie'
        if ins.op_str.endswith('+ 0x5ec]'):pause_sites.add(ins.address)
    if len(w['events'])!=len(expected) or {e['site']:e['name'] for e in w['events']}!=expected:
        raise ValueError('BarrelMan movie coverage changed')
    if len(w['pauses'])!=len(pause_sites) or {e['site'] for e in w['pauses']}!=pause_sites:
        raise ValueError('BarrelMan pause coverage changed')
    events={};pause_events={}
    for e in w['events']+w['pauses']:
        count=e.get('argumentCount',1)
        setup=read_call_window(data,w['address'],w['size'],e['site'],{},argument_count=count)
        if setup is None or json.loads(json.dumps(asdict(setup)))!=e['setup']:
            raise ValueError('BarrelMan movie/pause operands changed')
        identity=tuple(e['identity'])
        if e['site'] in pause_sites:
            flag=setup.stack_arguments
            if flag not in ((('constant',0),),(('constant',1),)):
                raise ValueError('BarrelMan pause flag changed')
            pause_events[e['site']]=('start' if flag[0][1] else 'end',('paused',1))
            events[e['site']]=('use',identity)
        else:
            if identity!=(setup.stack_arguments[1] if e['name']=='start_movie' else setup.ecx):
                raise ValueError('BarrelMan movie identity changed')
            operation={'construct':'start','destroy':'end','start_movie':'use'}[e['name']]
            if e['operation']!=operation:raise ValueError('BarrelMan movie operation changed')
            events[e['site']]=(operation,identity)
    selections={int(site):tuple(identity) for site,identity in w['selections'].items()}
    indexed={ins.address:ins for ins in instructions}
    for site,identity in selections.items():
        ins=indexed.get(site)
        if identity[0]!='stack' or ins is None or ins.mnemonic!='lea' or ins.op_str!=f'ecx, [esp + {hex(identity[1])}]':
            raise ValueError('BarrelMan movie receiver selection changed')
    if not check_single_resource_lifetime(instructions,events,receiver_register='ecx',selections=selections,indirect_branches=branches) or not check_single_resource_lifetime(instructions,pause_events,indirect_branches=branches):
        raise ValueError('BarrelMan movie/pause lifetime CFG changed')
    starts={e['site']:e for e in w['events'] if e['name']=='start_movie'}
    strings=w['classStrings']
    if len(strings)!=len(starts) or {s['use'] for s in strings}!=set(starts):
        raise ValueError('BarrelMan movie class coverage changed')
    string_events={}
    for string in strings:
        identity=tuple(string['identity'])
        for site,count,key in [(string['create'],2,'constructSetup'),(string['destroy'],0,'destroySetup')]:
            setup=read_call_window(data,w['address'],w['size'],site,{},argument_count=count)
            if setup is None or json.loads(json.dumps(asdict(setup)))!=string[key] or setup.ecx!=identity:
                raise ValueError('BarrelMan movie class operands changed')
        if string['constructSetup']['stack_arguments']!=[['constant',0x122D70E],['constant',0xFFFFFFFF]]:
            raise ValueError('BarrelMan movie class literal changed')
        if starts[string['use']]['setup']['stack_arguments'][0]!=string['identity']:
            raise ValueError('BarrelMan movie class receiver changed')
        for operation,key in [('start','create'),('use','use'),('end','destroy')]:
            if string[key] in string_events:raise ValueError('BarrelMan movie class events overlap')
            string_events[string[key]]=(operation,identity)
    if not check_single_resource_lifetime(instructions,string_events,indirect_branches=branches):
        raise ValueError('BarrelMan movie class lifetime CFG changed')
    return dict(w,status='verified',lifetime='four nonoverlapping movie scopes; seventeen pause calls paired before destruction',
                classLifetime='four empty-class CString temporaries destroyed after StartMovie, before Pause',
                remaining='explicit Lua movie lowering and runtime integration')
