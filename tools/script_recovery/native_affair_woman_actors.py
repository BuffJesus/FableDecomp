"""Verify retained actor identities, their uses, and the scoped hero-hit predicate."""
import hashlib
import json
from dataclasses import asdict
from pathlib import Path

from capstone import Cs,CS_ARCH_X86,CS_MODE_32
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime


def verify(function,data):
    w=json.loads(Path(__file__).with_name('native_affair_woman_actors_witness.json').read_text())
    if int(function['address'],16)!=w['address'] or hashlib.sha256(function['decompile'].encode()).hexdigest()!=w['sourceSha256']:
        raise ValueError('AffairWoman actor source changed')
    raw=data.bytes_at(w['address'],w['size'])
    if raw is None or hashlib.sha256(raw).hexdigest()!=w['nativeSha256']:
        raise ValueError('AffairWoman actor/hit bytes changed')
    decoder=Cs(CS_ARCH_X86,CS_MODE_32);decoder.detail=True
    instructions=list(decoder.disasm(raw,w['address']))
    lookups={i.address for i in instructions if i.mnemonic=='call' and i.op_str.endswith('+ 0x120]')}
    if len(w['actors'])!=len(lookups) or {a['create'] for a in w['actors']}!=lookups:
        raise ValueError('AffairWoman named-Thing coverage changed')
    calls={}
    for event in w['calls']:
        known={int(site):tuple(value) for site,value in event['knownCalls'].items()}
        setup=read_call_window(data,w['address'],w['size'],event['site'],known,argument_count=event['argumentCount'])
        if setup is None or json.loads(json.dumps(asdict(setup)))!=event['setup']:
            raise ValueError('AffairWoman actor operands changed')
        calls[event['site']]=setup
    required_uses={'NOVI_AffairWife':[0xDB26CF],'NOVI_AffairMan':[0xDB24A5,0xDB25C1],
                   'AffairWomanRunOffPoint':[0xDB27DD]}
    if {a['name'] for a in w['actors']}!=set(required_uses):
        raise ValueError('AffairWoman retained actor names changed')
    for actor in w['actors']:
        if data.string_at(actor['literal'])!=actor['name'] or actor['uses']!=required_uses[actor['name']]:
            raise ValueError('AffairWoman actor key/use coverage changed')
        identity=tuple(actor['output'])
        if calls[actor['create']].stack_arguments[0]!=identity or calls[actor['destroy']].ecx!=identity:
            raise ValueError('AffairWoman retained Thing identity changed')
        events={actor['create']:('start',identity),actor['destroy']:('end',identity)}
        events.update({site:('use',identity) for site in actor['uses']})
        if not check_single_resource_lifetime(instructions,events):
            raise ValueError('AffairWoman retained Thing lifetime CFG changed')
    if data.string_at(w['hitScope']['literal'])!=w['hitScope']['name']:
        raise ValueError('AffairWoman hero hit name changed')
    return dict(w,status='verified',remaining='Lua lowering and runtime integration')
