#!/usr/bin/env python3
"""Native observer construction, event filtering/lookup, ObserveAll and clearing."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x20000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,nodes,observer,tables,callback,result,query=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20013000,0x20014000,0x20014004
    def put(p,v): uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p): return struct.unpack('<i',uc.mem_read(p,4))[0]
    for p in (0xBFEA0E,0xBFEA14): uc.mem_write(p,b'\xc3')
    for i in range(2): put(tables+i*32+12,callback+i*16); uc.mem_write(callback+i*16,b'\xc2\x04\x00')
    events=[]; case=None
    def hook(uc,address,size,data):
        esp=uc.reg_read(UC_X86_REG_ESP)
        if address==0xBFEA0E: events.append('A'+str(word(esp+4))); uc.reg_write(UC_X86_REG_EAX,nodes)
        elif address==0xBFEA14: events.append('F'+str((word(esp+4)-nodes)//20))
        elif address in (callback,callback+16):
            index=(address-callback)//16; events.append(f'O{index}:{signed(esp+4)}')
            if case[4]: put(observer,tables+(1-index)*32)
    uc.hook_add(UC_HOOK_CODE,hook)
    values=(-2147483648,-2,0,1,25,37,2147483647); queries=(*values,-1,2,24,26,34,35,36,38)
    cases=list(itertools.product(range(8),range(3),queries,(0,1,255),(0,1),(0,1)))
    cases += [(n,s,0,p,f,2) for n,s,p,f in itertools.product(range(8),range(3),(0,1,255),(0,1))]
    directory=ROOT/'work/ui_observer_events_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c)) for c in cases)+'\n')
    sources=['rebuild/tests/integration/UiObserverEvents_test.cpp']
    sources+=['rebuild/src/compiled/00/52/'+s+'.cpp' for s in ('CObserver_CObserver_0052d9e0','global_FindEventInRange_0052dec0','global_FindObservedEvent_0052df20','CObserver_AcceptsEvent_0052d900','global_FreeEventTree_0052dca0','CObserver_ClearObservedEvents_0052d9a0','CObserver_ObserveAllEvents_0052d7b0')]
    env=parity.env(); objects=[]
    for i,s in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete observer event traces')
    def execute(address,receiver,args=()):
        put(stack,stop)
        for i,value in enumerate(args): put(stack+4+i*4,value)
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,receiver); uc.emu_start(address,stop,count=10000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop: raise RuntimeError('Oracle did not return')
    errors=[]
    for index,(case,actual) in enumerate(zip(cases,lines)):
        count,shape,event,prevent,flip,op=case; events.clear(); uc.mem_write(observer,b'\xA5'*20); uc.mem_write(nodes,b'\xCD'*160)
        execute(0x52D9E0,observer)
        if uc.reg_read(UC_X86_REG_EAX)!=observer or word(observer)!=0x1246020 or word(observer+8) or uc.mem_read(observer+16,1)!=b'\0': raise RuntimeError('Constructor mismatch')
        if word(nodes+16)!=0xCDCDCDCD or word(observer+12)!=0xA5A5A5A5 or uc.mem_read(observer+17,3)!=b'\xA5'*3: raise RuntimeError('Untouched bytes changed')
        order=list(range(7)) if shape==0 else list(reversed(range(7))) if shape==1 else [3,1,5,0,2,4,6]
        for i in order:
            if i>=count: continue
            n=nodes+(i+1)*20; put(n+16,values[i]); put(n+8,0); put(n+12,0); uc.mem_write(n,b'\x01')
            parent=nodes; link=nodes+4
            while word(link): parent=word(link); link=parent+(8 if values[i]<signed(parent+16) else 12)
            put(n+4,parent); put(link,n)
        if count: put(nodes+8,nodes+20); put(nodes+12,nodes+count*20)
        put(observer+8,count); uc.mem_write(observer+16,bytes([prevent])); put(observer,tables)
        if op==0: execute(0x52D900,observer,(event,)); events.append('R'+str(uc.reg_read(UC_X86_REG_EAX)&255))
        elif op==1:
            put(query,event); execute(0x52DF20,observer+4,(result,query)); events.append('R'+str((word(result)-nodes)//20))
            if uc.reg_read(UC_X86_REG_EAX)!=result: raise RuntimeError('Lookup result pointer mismatch')
        else: execute(0x52D7B0,observer)
        execute(0x52D9A0,observer)
        if word(observer+8) or word(nodes+4) or word(nodes+8)!=nodes or word(nodes+12)!=nodes: raise RuntimeError('Clear state mismatch')
        events.append(f'C{word(observer+8)}:{uc.mem_read(observer+16,1)[0]}'); events.append('F0')
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=case,actual=actual,retail=expected))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,scope='real observer construction/filter/exact lookup/ObserveAll/clear/tree disposal; valid manually built ordered trees, allocator/free and registration callback doubled'),indent=2)+'\n')
    print(f"UI_OBSERVER_EVENTS {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__=='__main__': raise SystemExit(main())
