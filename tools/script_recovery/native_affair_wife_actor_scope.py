"""Verify the wife's retained husband Thing through movement and conversation."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime

USES=[0xDB3473,0xDB3486,0xDB34A2,0xDB3586,0xDB36D8,0xDB3B8A,0xDB3C24,0xDB3C6F]
DESTROYS=[0xDB3CC6,0xDB3CF9,0xDB3D2C,0xDB3D6E,0xDB3D94]


def verify(function,data):
    w=json.loads(Path(__file__).with_name('native_affair_wife_actor_scope_witness.json').read_text())
    if int(function['address'],16)!=w['address'] or hashlib.sha256(function['decompile'].encode()).hexdigest()!=w['sourceSha256']:
        raise ValueError('AffairWife actor source changed')
    raw=data.bytes_at(w['address'],w['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['nativeSha256']:
        raise ValueError('AffairWife actor bytes changed')
    if w['name']!='NOVI_AffairMan' or w['literal']!=0x12D830C or data.string_at(w['literal'])!=w['name']:
        raise ValueError('AffairWife husband key changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(raw,w['address']))
    lookups={i.address for i in instructions if i.mnemonic=='call' and i.op_str.endswith('+ 0x120]')}
    if lookups!={w['create']} or w['create']!=0xDB3445 or w['uses']!=USES or w['destroys']!=DESTROYS:
        raise ValueError('AffairWife actor coverage changed')
    required={0xDB3431,0xDB3445,0xDB344F,*USES,*DESTROYS}
    if len(w['calls'])!=len(required) or {e['site'] for e in w['calls']}!=required:
        raise ValueError('AffairWife actor operand coverage changed')
    calls={}
    for event in w['calls']:
        known={int(site):tuple(value) for site,value in event['knownCalls'].items()}
        setup=read_call_window(data,w['address'],w['size'],event['site'],known,argument_count=event['argumentCount'])
        if setup is None or json.loads(json.dumps(asdict(setup)))!=event['setup']:
            raise ValueError('AffairWife actor operands changed')
        calls[event['site']]=setup
    identity=('stack',48)
    if tuple(w['output'])!=identity or calls[w['create']].stack_arguments!=(identity,('stack',44)):
        raise ValueError('AffairWife retained actor identity changed')
    if any(calls[site].ecx!=identity for site in DESTROYS):
        raise ValueError('AffairWife actor destructor identity changed')
    for site in (0xDB34A2,0xDB3586):
        if calls[site].edx!=identity or calls[site].stack_arguments!=(('constant',0x40400000),):
            raise ValueError('AffairWife husband distance changed')
    if calls[0xDB36D8].stack_arguments!=(('register','edi'),identity,('constant',0)):
        raise ValueError('AffairWife facing direction changed')
    if calls[0xDB3B8A].stack_arguments[1]!=identity:
        raise ValueError('AffairWife conversation participant changed')
    if calls[0xDB3C24].stack_arguments[3:]!=(('register','edi'),identity) or calls[0xDB3C6F].stack_arguments[3:]!=(identity,('register','edi')):
        raise ValueError('AffairWife conversation speaker/listener changed')
    events={w['create']:('start',identity),**{site:('use',identity) for site in USES},**{site:('end',identity) for site in DESTROYS}}
    if not check_single_resource_lifetime(instructions,events):
        raise ValueError('AffairWife husband lifetime CFG changed')
    strings={0xDB3431:('start',('stack',44)),0xDB3445:('use',('stack',44)),0xDB344F:('end',('stack',44))}
    if not check_single_resource_lifetime(instructions,strings):
        raise ValueError('AffairWife husband lookup string lifetime changed')
    return dict(w,status='verified',remaining='scoped hit/conversation string recovery, Lua lowering and runtime integration')
