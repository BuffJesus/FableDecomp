"""Verify Barrel Man's owning control resource across its bounded phase switch."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_affair_woman_resources import HELPERS
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_bounded_switch import resolve
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def verify(function,data,witness=None):
    witness=witness or json.loads(Path(__file__).with_name('native_barrel_man_resources_witness.json').read_text())
    start,size=witness['address'],witness['size']
    if (start,size)!=(0xDB5330,0xDB6B23-0xDB5330) or int(function['address'],16)!=start:
        raise ValueError('Barrel resource body coverage changed')
    if hashlib.sha256(function['decompile'].encode()).hexdigest()!=witness['sourceSha256']:
        raise ValueError('Barrel resource source changed')
    raw=data.bytes_at(start,size)
    if raw is None or hashlib.sha256(raw).hexdigest()!=witness['nativeSha256']:
        raise ValueError('Barrel resource bytes changed')
    for profile in witness['profiledCallees']:
        body=data.bytes_at(int(profile['address'],16),profile['size'])
        if body is None or hashlib.sha256(body).hexdigest()!=profile['bytesSha256']:
            raise ValueError('Barrel resource helper changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(raw,start));at={i.address:i for i in instructions}
    branches=resolve(instructions,data)
    switch=witness['switch']
    if (switch['site'],switch['table'])!=(0xDB5526,0xDB6B24) or branches!={switch['site']:tuple(switch['targets'])}:
        raise ValueError('Barrel phase switch changed')
    excluded=witness['excludedMovieConstructor']
    if (excluded['site'],excluded['vtable'],excluded['bytesStart'],excluded['bytesSize'])!=(0xDB5F8E,0x1260EF4,0xDB5F8A,38):
        raise ValueError('Barrel movie construction identity changed')
    if hashlib.sha256(data.bytes_at(excluded['bytesStart'],excluded['bytesSize'])).hexdigest()!=excluded['sha256']:
        raise ValueError('Barrel movie construction bytes changed')
    inventory={}
    for ins in instructions:
        if ins.mnemonic!='call':continue
        name=HELPERS.get(int(ins.op_str,16) if ins.op_str.startswith('0x') else 0)
        if ins.op_str.endswith('+ 0x20]'):name='acquire'
        if name and ins.address!=excluded['site']:inventory[ins.address]=name
    if len(witness['events'])!=len(inventory) or {e['site']:e['name'] for e in witness['events']}!=inventory:
        raise ValueError('Barrel resource event coverage changed')
    known={i.address:(0,'GetHero') for i in instructions if i.mnemonic=='call' and i.op_str.endswith('+ 0x118]')}
    if at[0xDB6271].op_str!='dword ptr [edx + 0x18]':raise ValueError('Barrel position getter changed')
    known[0xDB6271]=(0,'position')
    if {str(k):list(v) for k,v in known.items()}!=witness['knownCalls']:
        raise ValueError('Barrel profiled call setup changed')
    events={}
    for e in witness['events']:
        setup=read_call_window(data,start,size,e['site'],known,argument_count=e['argumentCount'])
        if setup is None or json.loads(json.dumps(asdict(setup)))!=e['setup']:
            raise ValueError('Barrel resource operands changed')
        identity=setup.stack_arguments[1] if e['name']=='acquire' else setup.ecx
        if identity!=('stack',20):raise ValueError('Barrel owning resource identity changed')
        if e['name']=='acquire' and setup.stack_arguments!=(('register','edi'),('stack',20),('constant',4)):
            raise ValueError('Barrel acquisition arguments changed')
        operation='start' if e['name']=='construct' else 'end' if e['name'].startswith('destroy') else 'use'
        if operation!=e['operation']:raise ValueError('Barrel resource operation changed')
        events[e['site']]=(operation,identity)
    if not check_single_resource_lifetime(instructions,events,indirect_branches=branches):
        raise ValueError('Barrel resource lifetime CFG changed')
    return dict(witness,status='verified-owning-resource',
        remaining='Temporary Things, movie/retained marker scopes, complete operand validation and resource-aware Lua lowering remain.')
