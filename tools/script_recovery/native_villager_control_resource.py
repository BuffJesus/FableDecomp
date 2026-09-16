"""Verify Villager's control-resource operands and all constructed lifetime paths."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_affair_woman_resources import HELPERS
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def verify(data,witness=None):
    w=witness or json.loads(Path(__file__).with_name('native_villager_control_resource_witness.json').read_text())
    if (w['address'],w['size'])!=(0xDADF80,2787):raise ValueError('Villager control coverage changed')
    raw=data.bytes_at(w['address'],w['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['sha256']:raise ValueError('Villager control bytes changed')
    for p in w['profiledCallees']:
        body=data.bytes_at(int(p['address'],16),p['size'])
        if body is None or hashlib.sha256(body).hexdigest()!=p['bytesSha256']:raise ValueError('Villager control helper changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(raw,w['address']));inventory={}
    for i in instructions:
        if i.mnemonic!='call':continue
        name=HELPERS.get(int(i.op_str,16) if i.op_str.startswith('0x') else 0)
        if i.op_str.endswith('+ 0x20]'):name='acquire'
        # This constructor's subsequent vtable is the movie type, not control.
        if name and i.address!=0xDAE1AE:inventory[i.address]=name
    if len(w['events'])!=len(inventory) or {e['site']:e['name'] for e in w['events']}!=inventory:
        raise ValueError('Villager control event coverage changed')
    known={i.address:(0,'GetHero') for i in instructions if i.mnemonic=='call' and i.op_str.endswith('+ 0x118]')}
    events={}
    for e in w['events']:
        setup=read_call_window(data,w['address'],w['size'],e['site'],known,argument_count=e['argumentCount'])
        if setup is None or json.loads(json.dumps(asdict(setup)))!=e['setup']:raise ValueError('Villager control operands changed')
        identity=setup.stack_arguments[1] if e['name']=='acquire' else setup.ecx
        if identity!=('stack',92):raise ValueError('Villager control local changed')
        if e['name']=='acquire' and setup.stack_arguments!=(('register','edi'),('stack',92),('constant',4)):
            raise ValueError('Villager acquire arguments changed')
        operation='start' if e['name']=='construct' else 'end' if e['name'].startswith('destroy') else 'use'
        events[e['site']]=(operation,identity)
    if not check_single_resource_lifetime(instructions,events):raise ValueError('Villager control lifetime changed')
    return dict(w,status='verified-control-resource',remaining='Temporary health Things, movies, retained conversation key and full candidate lowering remain.')
