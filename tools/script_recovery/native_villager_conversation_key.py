"""Verify the Main-owned conversation key, assignment and cleanup order."""
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
    w=witness or json.loads(Path(__file__).with_name('native_villager_conversation_key_witness.json').read_text())
    for p in w['helpers']:
        raw=data.bytes_at(p['address'],p['size'])
        if raw is None or hashlib.sha256(raw).hexdigest()!=p['sha256']:raise ValueError('Villager key helper changed')
    expected={0xDADFF4:'construct',0xDAE96B:'assign',0xDAE988:'line',0xDAE9A9:'destroy',0xDAEA4D:'destroy'}
    if len(w['events'])!=5 or {e['site']:e['name'] for e in w['events']}!=expected:raise ValueError('Villager key coverage changed')
    start,size=control['address'],control['size'];events={}
    for e in w['events']:
        setup=read_call_window(data,start,size,e['site'],{0xDAE975:(0,'hero')},argument_count=e['argumentCount'])
        if setup is None or json.loads(json.dumps(asdict(setup)))!=e['setup']:raise ValueError('Villager key operands changed')
        identity=setup.stack_arguments[1] if e['name']=='line' else setup.ecx
        if identity!=('stack',20):raise ValueError('Villager retained key identity changed')
        if e['name']=='line' and setup.stack_arguments[2:]!=(('constant',0),('register','edi'),('result',0xDAE975,'hero')):
            raise ValueError('Villager conversation actor/flag changed')
        events[e['site']]=('start' if e['name']=='construct' else 'end' if e['name']=='destroy' else 'use',identity)
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(data.bytes_at(start,size),start))
    if not check_single_resource_lifetime(instructions,events):raise ValueError('Villager retained key lifetime changed')
    return dict(w,status='verified-retained-key',controlSha256=control['sha256'],remaining='Talk-response key and temporary literal scopes still need composition.')
