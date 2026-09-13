"""Guardian native lifetime evidence using converter analyses read-only."""
import argparse
from collections import deque
from dataclasses import asdict
import hashlib,json
from pathlib import Path
import capstone
from capstone import x86 as x
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.native_resource_lifetime import check_single_resource_lifetime

ROOT=Path(__file__).resolve().parents[2]
START,END=0xe27e90,0xe28c9b


def check_movie_identity(instructions,events):
    """Track the shared end's two ECX origins along each native CFG path.

Fixed call operands come from shared call-window analysis. Only LEA ECX,[ESP+n]
without an intervening ESP write/call may supply the shared destructor operand.
"""
    positions={i.address:n for n,i in enumerate(instructions)}
    queue=deque([(0,None,None)]);seen=set();shared=set()
    while queue:
        index,active,ecx=queue.popleft()
        if (index,active,ecx) in seen:continue
        seen.add((index,active,ecx));ins=instructions[index]
        if ins.address in events:
            operation,resource=events[ins.address]
            if operation=='start':
                if active is not None:return None
                active=resource
            else:
                actual=ecx if resource is None else resource
                if actual is None or active!=actual:return None
                if resource is None:shared.add(actual)
                active=None
        if ins.mnemonic.startswith('ret'):
            if active is not None:return None
            continue
        writes={ins.reg_name(reg) for reg in ins.regs_access()[1]}
        if ins.mnemonic=='call' or writes & {'esp','sp','ecx','cx','cl','ch'}:ecx=None
        if (ins.mnemonic=='lea' and ins.operands[0].type==x.X86_OP_REG
                and ins.reg_name(ins.operands[0].reg)=='ecx'):
            mem=ins.operands[1].mem
            if ins.reg_name(mem.base)=='esp' and not mem.index:ecx=mem.disp
        following=[index+1] if index+1<len(instructions) else []
        if ins.mnemonic.startswith(('j','loop')):
            if ins.operands[0].type!=x.X86_OP_IMM:return None
            target=positions.get(ins.operands[0].imm)
            if target is None:return None
            following=[target] if ins.mnemonic=='jmp' else following+[target]
        if not following:return None
        queue.extend((n,active,ecx) for n in following)
    return {'visited_states':len(seen),'shared_end_resources':sorted(shared)}


def audit():
    r=RData();raw=r.bytes_at(START,END-START)
    d=capstone.Cs(capstone.CS_ARCH_X86,capstone.CS_MODE_32);d.detail=True
    ins=list(d.disasm(raw,START));events={};records=[];profiles={}
    for i in ins:
        if i.mnemonic=='call' and '0x118]' in i.op_str:
            profiles[i.address]=(0,'GetHero')
        elif i.mnemonic=='call' and i.op_str=='0xcd3d2e':
            profiles[i.address]=(4,'actor_map_entry')
    for i in ins:
        if i.mnemonic!='call':continue
        kind=None;argc=0
        if '0x5c8]' in i.op_str:kind='movie_start';argc=2
        elif i.op_str=='0x6e7b80':kind='movie_end'
        elif '0x20]' in i.op_str:kind='acquire';argc=3
        elif i.op_str=='0x7e74d0':kind='resource_end'
        elif i.op_str=='0xcd3d2e':kind='map_entry';argc=2
        elif i.op_str=='0x8abd10':kind='resource_copy';argc=1
        elif i.op_str=='0xcdbfb0':kind='map_end'
        elif i.op_str=='0xcbfb7d':kind='macro';argc=4
        if not kind:continue
        setup=read_call_window(r,START,END-START,i.address,profiles,argument_count=argc)
        if setup is None:raise ValueError(f'Unresolved {kind} at {i.address:x}')
        records.append({'kind':kind,'setup':asdict(setup)})
        if kind=='movie_start':
            resource=setup.stack_arguments[1];assert resource[0]=='stack'
            events[i.address]=('start',resource[1])
        elif kind=='movie_end':
            if setup.ecx[0]=='stack':resource=setup.ecx[1]
            else:assert i.address==0xe28c85;resource=None
            events[i.address]=('end',resource)
    generic={a:(op,'movie') for a,(op,_) in events.items()}
    assert check_single_resource_lifetime(ins,generic)
    identity=check_movie_identity(ins,events);assert identity and identity['shared_end_resources']==[104,120]
    # An incorrect attribution of the shared cleanup to one fixed slot must fail.
    negative={**events,0xe28c85:('end',120)}
    assert check_movie_identity(ins,negative) is None
    paths=[Path(__file__),*(ROOT/'tools/script_recovery'/n for n in
            ['native_call_setup_ir.py','native_resource_lifetime.py','lift_native_lua.py'])]
    return {'scope':__doc__,'function':hex(START),'end_exclusive':hex(END),
            'native_sha256':hashlib.sha256(raw).hexdigest(),'native_bytes':raw.hex(),
            'analysis_sources':[{'path':str(p),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()} for p in paths],
            'records':records,'movies':{hex(a):v for a,v in events.items()},
            'single_movie_all_paths':True,'identity_check':identity,'wrong_shared_slot_rejected':True}


if __name__=='__main__':
    parser=argparse.ArgumentParser(description=__doc__);parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    if args.output.exists():raise SystemExit('Output exists')
    report=audit();args.output.write_text(json.dumps(report,indent=2)+'\n')
    print(json.dumps({'records':len(report['records']),'movie_events':len(report['movies']),**report['identity_check']}))
