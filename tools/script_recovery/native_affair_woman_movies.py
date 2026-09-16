"""Check both woman movie scopes, including pause ordering on cancellation."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def verify(function,data):
    w=json.loads(Path(__file__).with_name('native_affair_woman_movies_witness.json').read_text())
    if int(function['address'],16)!=w['address'] or hashlib.sha256(function['decompile'].encode()).hexdigest()!=w['sourceSha256']:
        raise ValueError('AffairWoman movie source changed')
    raw=data.bytes_at(w['address'],w['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['nativeSha256']:
        raise ValueError('AffairWoman movie bytes changed')
    for offset,target in w['bindings'].items():
        if data.bytes_at(0x1260F0C+int(offset,16),4)!=target.to_bytes(4,'little'):
            raise ValueError('AffairWoman movie/pause binding changed')
    if data.bytes_at(0x122D70E,1)!=b'\0':
        raise ValueError('AffairWoman empty movie class changed')
    for callee in w['callees']:
        raw_callee=data.bytes_at(int(callee['address'],16),callee['size'])
        if raw_callee is None or hashlib.sha256(raw_callee).hexdigest()!=callee['bytesSha256']:
            raise ValueError('AffairWoman movie helper changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(raw,w['address']))
    expected={};pause_sites=set()
    for ins in instructions:
        if ins.mnemonic!='call':continue
        if ins.op_str in ('0x6e7b60','0x6e7b80') or ins.op_str.endswith('+ 0x5c8]'):
            expected[ins.address]='construct' if ins.op_str=='0x6e7b60' else 'destroy' if ins.op_str=='0x6e7b80' else 'start_movie'
        if ins.op_str.endswith('+ 0x5ec]'):pause_sites.add(ins.address)
    if len(w['events'])!=len(expected) or {e['site']:e['name'] for e in w['events']}!=expected:
        raise ValueError('AffairWoman movie coverage changed')
    if len(w['pauses'])!=len(pause_sites) or {e['site'] for e in w['pauses']}!=pause_sites:
        raise ValueError('AffairWoman pause coverage changed')
    events={};pause_events={}
    for e in w['events']+w['pauses']:
        count=e.get('argumentCount',1)
        setup=read_call_window(data,w['address'],w['size'],e['site'],{},argument_count=count)
        if setup is None or json.loads(json.dumps(asdict(setup)))!=e['setup']:
            raise ValueError('AffairWoman movie/pause operands changed')
        identity=tuple(e['identity'])
        if e['site'] in pause_sites:
            flag=setup.stack_arguments
            if flag not in ((('constant',0),),(('constant',1),)):
                raise ValueError('AffairWoman pause flag changed')
            pause_events[e['site']]=('start' if flag[0][1] else 'end',('paused',1))
            events[e['site']]=('use',identity)
        else:
            if identity!=(setup.stack_arguments[1] if e['name']=='start_movie' else setup.ecx):
                raise ValueError('AffairWoman movie identity changed')
            operation={'construct':'start','destroy':'end','start_movie':'use'}[e['name']]
            if e['operation']!=operation:raise ValueError('AffairWoman movie operation changed')
            events[e['site']]=(operation,identity)
    if not check_single_resource_lifetime(instructions,events) or not check_single_resource_lifetime(instructions,pause_events):
        raise ValueError('AffairWoman movie/pause lifetime CFG changed')
    return dict(w,status='verified',lifetime='two nonoverlapping movie scopes; eight pause calls paired before destruction',
                remaining='explicit Lua movie lowering and runtime integration')
