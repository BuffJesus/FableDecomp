#!/usr/bin/env python3
"""Retail observer registration, RB insertion and callback-driven clearing."""
import hashlib
import itertools
import json
import random
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x30000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,nodes,observer,vt,callback,result,query=0x20000000,0x20008000,0x20010000,0x20014000,0x20015000,0x20016000,0x20017000,0x20017010
    def put(p,v): uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p): return struct.unpack('<i',uc.mem_read(p,4))[0]
    def ident(p): return (p-nodes)//20 if p else -1
    for p in (0xBFEA0E,0xBFEA14): uc.mem_write(p,b'\xc3')
    put(vt+4,callback); put(vt+12,0x52DA20); uc.mem_write(callback,b'\xc2\x04\x00')
    # ProcessEvent callback optionally clears the real native tree.
    trampoline=callback+32
    code=b'\xb9'+struct.pack('<I',observer)+b'\xe8'+struct.pack('<i',0x52D9A0-(trampoline+5)-5)+b'\xc2\x04\x00'
    uc.mem_write(trampoline,code)
    events=[]; allocated=0; live=set(); mode=0
    def hook(uc,address,size,data):
        nonlocal allocated
        esp=uc.reg_read(UC_X86_REG_ESP)
        if address==0xBFEA0E:
            if word(esp+4)!=20 or allocated>=256: raise RuntimeError('Unexpected allocation')
            events.append(f'A{allocated}:20'); live.add(allocated); uc.reg_write(UC_X86_REG_EAX,nodes+20*allocated); allocated+=1
        elif address==0xBFEA14:
            i=ident(word(esp+4))
            if i not in live: raise RuntimeError('Bad native free')
            live.remove(i); events.append('F'+str(i))
        elif address==callback:
            events.append(f'P{word(esp+4)}:{word(observer+8)}')
            if mode: uc.reg_write(UC_X86_REG_EIP,trampoline)
    uc.hook_add(UC_HOOK_CODE,hook)
    sequences=[[],list(range(39)),list(reversed(range(39))),[25]*12,[-2147483648,2147483647,0,25,-1,25,0]]
    rng=random.Random(0xFAB1E)
    sequences += [[rng.choice(list(range(-5,40))+[-2147483648,2147483647]) for _ in range(48)] for _ in range(40)]
    cases=[(op,mode,prevent,seq) for op,mode,prevent,seq in itertools.product((0,1),(0,1),(0,255),sequences)]
    cases += [(2,mode,prevent,[0,0]) for mode,prevent in itertools.product((0,1),(0,255))]
    cases += [(op,0,prevent,seq) for op,prevent,seq in itertools.product((3,4),(0,255),sequences)]
    directory=ROOT/'work/ui_event_registration_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,(op,mode,prevent,len(seq),*seq))) for op,mode,prevent,seq in cases)+'\n')
    sources=['rebuild/tests/integration/UiEventRegistration_test.cpp']
    sources+=['rebuild/src/compiled/00/52/'+s+'.cpp' for s in ('CObserver_CObserver_0052d9e0','global_FindObservedEvent_0052df20','global_FreeEventTree_0052dca0','CObserver_ClearObservedEvents_0052d9a0','CObserver_ObserveAllEvents_0052d7b0','global_LinkEventNode_0052e0e0','global_InsertObservedEvent_0052e230','CObserver_ObserveEvent_0052da20')]
    sources+=['rebuild/src/compiled/00/42/'+s+'.cpp' for s in ('global_RotateEventTreeLeft_0042951b','global_RotateEventTreeRight_0042955b','global_BalanceEventTree_0042971d')]
    sources+=['rebuild/src/compiled/00/52/global_FindEventInRange_0052dec0.cpp','rebuild/src/compiled/00/52/CObserver_RemoveObservedEvent_0052d940.cpp','rebuild/src/compiled/00/42/global_EraseEventTreeNode_0042a34e.cpp']
    env=parity.env(); objects=[]
    for i,s in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete registration traces')
    def execute(address,receiver,args=()):
        put(stack,stop)
        for i,v in enumerate(args): put(stack+4+i*4,v)
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,receiver); uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop: raise RuntimeError('Oracle did not return')
    errors=[]
    for index,((op,mode,prevent,seq),actual) in enumerate(zip(cases,lines)):
        events.clear(); allocated=0; live.clear(); uc.mem_write(nodes,b'\xCD'*5120); uc.mem_write(observer,b'\xA5'*20)
        execute(0x52D9E0,observer); put(observer,vt); uc.mem_write(observer+16,bytes([prevent]))
        if op==4:
            for event in seq: execute(0x52DA20,observer,(event,))
        for position,event in enumerate(seq):
            if op==4 or (op==3 and position&1):
                execute(0x52D940,observer,(event,)); events.append('D'+str(word(observer+8)))
                for i in sorted(live):
                    n=nodes+20*i; events.append(f'V{i}:{uc.mem_read(n,1)[0]}:{ident(word(n+4))}:{ident(word(n+8))}:{ident(word(n+12))}')
            elif op==2: execute(0x52D7B0,observer)
            elif op==1:
                put(query,event); execute(0x52E230,observer+4,(result,query)); events.append(f'I{ident(word(result))}:{uc.mem_read(result+4,1)[0]}')
                if uc.reg_read(UC_X86_REG_EAX)!=result: raise RuntimeError('Wrong insert result')
            else: execute(0x52DA20,observer,(event,))
        events.append(f'C{word(observer+8)}:{uc.mem_read(observer+16,1)[0]}')
        for i in sorted(live):
            n=nodes+20*i; events.append(f'T{i}:{uc.mem_read(n,1)[0]}:{ident(word(n+4))}:{ident(word(n+8))}:{ident(word(n+12))}:{signed(n+16)}')
        execute(0x52D9A0,observer); live.remove(0); events.extend(('F0','LIVE'+str(len(live))))
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=(op,mode,prevent,seq),actual=actual,retail=expected))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,scope='complete registration/ObserveAll, insertion/balance/rotations, removal/erase/rebalance, constructor/find/clear; exact node identities/links/colors after each removal, allocation/free order and event25 callbacks; malloc/free/ProcessEvent controlled'),indent=2)+'\n')
    print(f"UI_EVENT_REGISTRATION {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__=='__main__': raise SystemExit(main())
