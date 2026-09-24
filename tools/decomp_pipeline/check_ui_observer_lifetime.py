#!/usr/bin/env python3
"""Observable construction, snapshot dispatch and destruction against retail."""
import hashlib
import itertools
import json
import struct
import sys
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    real_filter='--filter' in sys.argv
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x30000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,pool,manager,observers,vt,callback=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20013000,0x20014000
    observer_stride=64; event_nodes=0x20018000
    def put(p,v): uc.mem_write(p,struct.pack('<I',v))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    for p in (0xBFEA0E,0xBFEA14): uc.mem_write(p,b'\xc3')
    # Callback bodies may invoke real retail helpers, then return to dispatch.
    # The hook selects a prebuilt trampoline rather than reentering Unicorn.
    uc.mem_write(callback,b'\xc2\x04\x00'); uc.mem_write(callback+16,b'\xc2\x04\x00')
    put(vt+8,callback); put(vt+4,callback+16)
    events=[]; allocated=0; live=set(); mutated=False; case=None
    tramp=callback+0x100
    def call_bytes(address,at): return b'\xe8'+struct.pack('<i',address-at-5)
    def mutation_code(mode,event,accept):
        code=b''
        def emit_call(address):
            nonlocal code
            code+=call_bytes(address,tramp+len(code))
        def receiver():
            nonlocal code
            code+=b'\xb9'+struct.pack('<I',manager)
        if mode==1: receiver(); emit_call(0x55C950)
        if mode==2:
            for address in (0x55CA40,0x55CA90):
                code+=b'\x68'+struct.pack('<I',observers+4*observer_stride); receiver(); emit_call(address)
        if mode==3: code+=b'\xc7\x05'+struct.pack('<II',manager+8,observers+4*observer_stride)
        if mode==5:
            code+=b'\x68'+struct.pack('<I',(event+1)&0xFFFFFFFF); receiver(); emit_call(0x55CB10)
        code+=b'\xb8'+struct.pack('<I',accept)+b'\xc2\x04\x00'
        return code
    # Reserve all event/mutation/return combinations once, avoiding cache edits.
    trampolines={}
    for mode,event,accept in itertools.product(range(6),(0,0xFFFFFFFF),(0,1)):
        trampolines[mode,event,accept]=tramp
        code=mutation_code(mode,event,accept); uc.mem_write(tramp,code); tramp+=0x80
    def allocate(size):
        nonlocal allocated
        if size!=12 or allocated>=64: raise RuntimeError('Unexpected allocation')
        n=pool+allocated*12; events.append(f'A{allocated}:{size}'); live.add(allocated); allocated+=1; return n
    def hook(uc,address,size,data):
        nonlocal mutated
        esp=uc.reg_read(UC_X86_REG_ESP); receiver=uc.reg_read(UC_X86_REG_ECX)
        if address==0xBFEA0E: uc.reg_write(UC_X86_REG_EAX,allocate(word(esp+4)))
        elif address==0xBFEA14:
            i=(word(esp+4)-pool)//12
            if i not in live: raise RuntimeError('Native double/invalid free')
            live.remove(i); events.append('F'+str(i))
        elif address==callback:
            i=(receiver-observers)//observer_stride; event=word(esp+4); events.append(f'Q{i}:{event}'); accept=int(bool(case[4]&(1<<i)))
            if real_filter:
                uc.reg_write(UC_X86_REG_EIP,0x52D900)
                return
            if not mutated and case[5]!=4:
                mutated=True; uc.reg_write(UC_X86_REG_EIP,trampolines[case[5],event,accept])
            else: uc.reg_write(UC_X86_REG_EAX,accept)
        elif address==callback+16:
            events.append(f'P{(receiver-observers)//observer_stride}:{word(esp+4)}')
            if case[5]==4: put(manager+8,observers+4*observer_stride)
    uc.hook_add(UC_HOOK_CODE,hook)
    cases=[c for c in itertools.product(range(5),range(3),range(2),range(3),(0,1,0x15,0x1F),range(6),(0,0xFFFFFFFF)) if not(c[3] and c[5]==1)]
    if real_filter: cases=[c for c in cases if c[5] in (0,4)]
    directory=ROOT/('work/ui_observer_filter_chain_check' if real_filter else 'work/ui_observer_lifetime_check'); directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c)) for c in cases)+'\n')
    sources=['rebuild/tests/integration/UiObserverLifetime_test.cpp']
    sources+=['rebuild/src/compiled/00/42/'+s+'.cpp' for s in ('global_ConstructObserverList_0042ac0a','global_ClearObserverList_0042a1e3','global_DestroyObserverList_0042ac25','CObservable_CObservable_0042be7b','CObservable_Destroy_0042bec0','CObservable_GetExclusiveObserver_0042bea9')]
    sources+=['rebuild/src/compiled/00/55/'+s+'.cpp' for s in ('global_InsertObserverRange_0055ce90','global_CopyObserverList_0055cf50','CObservable_DispatchEvent_0055cb10','CObservable_ClearObservers_0055c950','CObservable_AddObserver_0055ca40','CObservable_AddConcurrentExclusiveObserver_0055ca90')]
    if real_filter: sources+=['rebuild/src/compiled/00/52/'+s+'.cpp' for s in ('CObserver_AcceptsEvent_0052d900','global_FindEventInRange_0052dec0')]
    env=parity.env(); objects=[]
    for i,s in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy',*(['/DREAL_EVENT_FILTER'] if real_filter else []),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete lifetime traces')
    errors=[]
    def execute(address,args=()):
        put(stack,stop)
        for i,value in enumerate(args): put(stack+4+i*4,value)
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,manager); uc.emu_start(address,stop,count=30000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop: raise RuntimeError('Oracle did not return')
    def append(head,id):
        n=allocate(12); tail=word(head+4); put(n,head); put(n+4,tail); put(n+8,observers+id*observer_stride); put(tail,n); put(head+4,n)
    def dump(head):
        n=word(head)
        while n!=head:
            if word(word(n)+4)!=n or word(word(n+4))!=n: raise RuntimeError('Bad retail list links')
            events.append(f'{(n-pool)//12}:{(word(n+8)-observers)//observer_stride}'); n=word(n)
    for index,(case,actual) in enumerate(zip(cases,lines)):
        normal,concurrent,pattern,exclusive,mask,mutation,event=case
        events.clear(); allocated=0; live.clear(); mutated=False; uc.mem_write(pool,b'\xCD'*768); uc.mem_write(manager,b'\xA5'*16)
        uc.mem_write(observers,b'\0'*(5*observer_stride)); uc.mem_write(event_nodes,b'\0'*200)
        for i in range(5):
            o=observers+i*observer_stride; head=event_nodes+i*40; node=head+20; registered=bool(mask&(1<<i))
            put(o,vt); put(o+4,head); put(o+8,int(registered)); uc.mem_write(o+16,bytes([int(i==pattern)]))
            put(head+4,node if registered else 0); put(head+8,node if registered else head); put(head+12,node if registered else head)
            put(node+4,head); put(node+16,event)
        execute(0x42BE7B)
        if word(manager)!=0x1230044 or word(manager+8) or uc.reg_read(UC_X86_REG_EAX)!=manager: raise RuntimeError('Constructor contract failed')
        if word(word(manager+4)+8)!=0xCDCDCDCD or word(word(manager+12)+8)!=0xCDCDCDCD: raise RuntimeError('Sentinel payload changed')
        for i in range(normal): append(word(manager+4),(i+pattern)%3)
        for i in range(concurrent): append(word(manager+12),(i+pattern+1)%3)
        put(manager+8,observers+observer_stride*(exclusive-1) if exclusive else 0)
        execute(0x55CB10,(event,)); events.append('L'); dump(word(manager+4)); events.append('C'); dump(word(manager+12))
        execute(0x42BEA9); target=uc.reg_read(UC_X86_REG_EAX); events.append('E'+str((target-observers)//observer_stride if target else -1))
        execute(0x42BEC0); events.append('LIVE'+str(len(live)))
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=case,actual=actual,retail=expected))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,real_event_filter=real_filter,scope='real observable constructor/list lifetime/snapshot dispatch; filter mode also executes actual AcceptsEvent and tree lookup with controlled event sets; other mode doubles queries and tests mutation/nested dispatch; malloc/free and event handlers controlled'),indent=2)+'\n')
    print(f"UI_OBSERVER_LIFETIME {'FAIL' if errors else 'PASS'} filter={real_filter} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__=='__main__': raise SystemExit(main())
