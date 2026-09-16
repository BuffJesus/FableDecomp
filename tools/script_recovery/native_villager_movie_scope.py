"""Verify the Villager hit movie's construction/use and all cleanup joins."""
import hashlib
import json
from pathlib import Path
from dataclasses import asdict
from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_villager_control_resource import verify as verify_control
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def verify(data,witness=None):
    control=verify_control(data)
    w=witness or json.loads(Path(__file__).with_name('native_villager_movie_scope_witness.json').read_text())
    for offset,target in w['bindings'].items():
        if data.bytes_at(0x1260F0C+int(offset,16),4)!=target.to_bytes(4,'little'):raise ValueError('Villager movie binding changed')
    for p in w['callees']:
        raw=data.bytes_at(int(p['address'],16),p['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=p['bytesSha256']:raise ValueError('Villager movie helper changed')
    if data.bytes_at(0x122D70E,1)!=b'\0':raise ValueError('Villager empty movie key changed')
    start,size=control['address'],control['size'];decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(data.bytes_at(start,size),start));expected={};pauses=set()
    for i in instructions:
        if i.mnemonic!='call':continue
        if i.address==0xDAE1AE:expected[i.address]='construct'
        elif i.op_str=='0x6e7b80':expected[i.address]='destroy'
        elif i.op_str.endswith('+ 0x5c8]'):expected[i.address]='start'
        if i.op_str.endswith('+ 0x5ec]'):pauses.add(i.address)
    if len(w['events'])!=len(expected) or {e['site']:e['name'] for e in w['events']}!=expected or set(w['pauses'])!=pauses:
        raise ValueError('Villager movie event coverage changed')
    events={}
    for e in w['events']:
        setup=read_call_window(data,start,size,e['site'],{},argument_count=e['argumentCount'])
        if setup is None or json.loads(json.dumps(asdict(setup)))!=e['setup']:raise ValueError('Villager movie operands changed')
        identity=setup.stack_arguments[1] if e['name']=='start' else setup.ecx
        if identity!=('stack',108):raise ValueError('Villager movie identity changed')
        events[e['site']]=({'construct':'start','start':'use','destroy':'end'}[e['name']],identity)
    for site in pauses:events[site]=('use',('stack',108))
    if not check_single_resource_lifetime(instructions,events):raise ValueError('Villager movie lifetime changed')
    return dict(w,status='verified-movie-lifetime',controlSha256=control['sha256'],
        pauseValidation='test_native_villager_movie_pause: original instructions prove early EBX zero and late immediate-zero cleanup; 18 cases.',
        remaining='Retained CString lifetime and resource-aware candidate lowering remain.')
