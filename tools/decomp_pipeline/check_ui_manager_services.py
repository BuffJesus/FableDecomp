#!/usr/bin/env python3
"""Retail singleton and eight observable services differential checks; no GUI."""
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
    stop,stack,nodes,observers,manager,vt,callback,storage=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20013000,0x20014000,0x20015000
    others=nodes+16*12
    def put(p,v): uc.mem_write(p,struct.pack('<I',v))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    def ident(p): return 0 if not p else 1 if p==storage else 2
    for p in (0xBFEA1A,0x41E3F6,0xBFEA14,0xBFEA0E,callback): uc.mem_write(p,b'\xc3')
    events=[]; case=None
    def hook(uc,address,size,data):
        esp=uc.reg_read(UC_X86_REG_ESP); receiver=uc.reg_read(UC_X86_REG_ECX)
        if address==0xBFEA1A:
            events.append('A'+str(word(esp+4))); uc.reg_write(UC_X86_REG_EAX,0 if case[2] else storage)
        elif address==0x41E3F6:
            events.append('C'+str(ident(receiver))); uc.reg_write(UC_X86_REG_EAX,storage+(case[3]-1)*0xD0 if case[3] else 0)
        elif address==0xBFEA14: events.append('F'+str((word(esp+4)-nodes)//12))
        elif address==0xBFEA0E:
            events.append('A'+str(word(esp+4))); uc.reg_write(UC_X86_REG_EAX,nodes+7*12)
        elif address==callback:
            n=word(nodes); count=0
            while n!=nodes: count+=1; n=word(n)
            events.append(f'N{(receiver-observers)//4}:{count}')
            if case[4]==1:
                tail=word(nodes+4); extra=nodes+8*12
                put(extra,nodes); put(extra+4,tail); put(extra+8,observers+12); put(tail,extra); put(nodes+4,extra)
            if case[4]==2:
                current=word(nodes)
                while word(current+8)!=receiver: current=word(current)
                following=word(current)
                if following!=nodes:
                    events.append('X'+str((following-nodes)//12)); successor=word(following); put(current,successor); put(successor+4,current)
    uc.hook_add(UC_HOOK_CODE,hook)
    cases=[(0,*c) for c in itertools.product(range(2),range(2),range(3),range(1,5))]
    cases += [(op,*c) for op in (1,2) for c in itertools.product(range(7),range(4),range(3),range(3))]
    cases += [(op,*c,0) for op in range(3,9) for c in itertools.product(range(7),range(4),range(3))]
    directory=ROOT/'work/ui_manager_services_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c)) for c in cases)+'\n')
    sources=['rebuild/tests/integration/UiManagerServices_test.cpp','rebuild/src/compiled/00/41/CFrontEndManager_GetInstance_0041e5f2.cpp','rebuild/src/compiled/00/55/CObservable_RemoveObserver_0055c9c0.cpp']
    sources += ['rebuild/src/compiled/00/55/'+s+'.cpp' for s in ('CObservable_RemoveConcurrentExclusiveObserver_0055ca00','CObservable_AddObserver_0055ca40','CObservable_AddConcurrentExclusiveObserver_0055ca90','CObservable_SetExclusiveObserver_0055c930','CObservable_ClearExclusiveObserver_0055c940','CObservable_HasExclusiveObservers_0055cae0','CObservable_ClearObservers_0055c950')]
    env=parity.env(); objects=[]
    for i,s in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete service traces')
    errors=[]
    def execute(address):
        put(stack,stop); uc.reg_write(UC_X86_REG_ESP,stack); uc.emu_start(address,stop,count=10000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop: raise RuntimeError('Oracle did not return')
    for index,(case,actual) in enumerate(zip(cases,lines)):
        events.clear()
        if not case[0]:
            put(0x13B8710,storage+0xD0 if case[1] else 0)
            for _ in range(case[4]): execute(0x41E5F2); events.append('R'+str(ident(uc.reg_read(UC_X86_REG_EAX))))
            events.append('G'+str(ident(word(0x13B8710))))
        else:
            _,count,target,pattern,mutation=case
            uc.mem_write(nodes,b'\0'*300); put(nodes,nodes+12 if count else nodes); put(nodes+4,nodes+12*count)
            for i in range(1,count+1):
                n=nodes+12*i; put(n,nodes if i==count else n+12); put(n+4,n-12); put(n+8,observers+4*((i-1+pattern)%3))
            for i in range(4): put(observers+4*i,vt)
            put(others,others+12 if target else others); put(others+4,others+12*target)
            for i in range(1,target+1):
                n=others+12*i; put(n,others if i==target else n+12); put(n+4,n-12); put(n+8,observers+4*(i-1))
            put(vt+0x14,callback); put(manager+4,others if case[0] in (2,4) else nodes); put(manager+12,nodes if case[0] in (2,4) else others)
            put(manager+8,observers+4*(pattern-1) if pattern else 0)
            put(stack+4,observers+4*target); uc.reg_write(UC_X86_REG_ECX,manager)
            execute({1:0x55C9C0,2:0x55CA00,3:0x55CA40,4:0x55CA90,5:0x55C930,6:0x55C940,7:0x55CAE0,8:0x55C950}[case[0]])
            if case[0]==7: events.append('H'+str(uc.reg_read(UC_X86_REG_EAX)&255))
            events.append('L'); n=word(nodes)
            while n!=nodes:
                if word(word(n)+4)!=n or word(word(n+4))!=n: raise RuntimeError('Bad retail list links')
                events.append(f'{(n-nodes)//12}:{(word(n+8)-observers)//4}'); n=word(n)
            events.append('O'); n=word(others)
            while n!=others:
                if word(word(n)+4)!=n or word(word(n+4))!=n: raise RuntimeError('Bad retail second-list links')
                events.append(f'{(n-nodes)//12}:{(word(n+8)-observers)//4}'); n=word(n)
            events.append('E'+str((word(manager+8)-observers)//4 if word(manager+8) else -1))
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=case,actual=actual,retail=expected))
    report=dict(accepted=not errors,cases=len(cases),errors=errors,scope='complete singleton plus normal/concurrent add/remove, exclusive set/clear/query and bulk clear; allocator/constructor/free and notification doubled; exact results, list links and call order; callback append/unlink mutations')
    (directory/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f"UI_MANAGER_SERVICES {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__=='__main__': raise SystemExit(main())
