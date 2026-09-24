#!/usr/bin/env python3
"""Connected native Die/recursive cleanup/singleton/observer unlink comparison."""
import hashlib
import itertools
import json
import struct
import sys
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    real_cleanup='--events' in sys.argv
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x20000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,nodes,entries,links,manager,ct,ot,mt,callback=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20013000,0x20014000,0x20015000,0x20016000,0x20017000
    event_trees=0x20019000
    def put(p,v): uc.mem_write(p,struct.pack('<I',v))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    put(ct+0x214,callback); put(ct+0xC0,callback+16); put(ot+0x14,0x52D9A0 if real_cleanup else callback+32); put(mt+0x14,0x55C9C0)
    uc.mem_write(callback,b'\xc3'); uc.mem_write(callback+16,b'\xc2\x04\x00'); uc.mem_write(callback+32,b'\xc3'); uc.mem_write(0xBFEA14,b'\xc3')
    events=[]
    def hook(uc,address,size,data):
        receiver=uc.reg_read(UC_X86_REG_ECX); esp=uc.reg_read(UC_X86_REG_ESP)
        if address==callback: events.append('H'+str((receiver-nodes)//0x200))
        elif address==callback+16: events.append(f'S{(receiver-nodes)//0x200}:{word(esp+4)}')
        elif address==callback+32: events.append('N'+str((receiver-nodes-4)//0x200))
        elif address==0xBFEA14:
            p=word(esp+4)
            if event_trees<=p<event_trees+200: events.append('T'+str((p-event_trees)//40))
            else: events.append('F'+str((p-links)//12))
    uc.hook_add(UC_HOOK_CODE,hook)
    cases=list(itertools.product(range(2),range(4),range(32),range(2)))
    directory=ROOT/('work/ui_observer_event_cleanup_check' if real_cleanup else 'work/ui_observer_chain_check'); directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c)) for c in cases)+'\n')
    sources=['rebuild/tests/integration/UiObserverChain_test.cpp','rebuild/src/compiled/00/41/CFrontEndManager_GetInstance_0041e5f2.cpp','rebuild/src/compiled/00/55/CObservable_RemoveObserver_0055c9c0.cpp']
    sources+=['rebuild/src/compiled/00/53/'+s+'.cpp' for s in ('CComponent_Die_00530720','CComponent_RemoveObserverRecursive_005303f0')]
    if real_cleanup: sources+=['rebuild/src/compiled/00/52/'+s+'.cpp' for s in ('CObserver_ClearObservedEvents_0052d9a0','global_FreeEventTree_0052dca0')]
    env=parity.env(); objects=[]
    for i,s in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy',*(['/DREAL_OBSERVER_CLEANUP'] if real_cleanup else []),'/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete connected traces')
    errors=[]
    for index,((operation,shape,mask,duplicate),actual) in enumerate(zip(cases,lines)):
        events.clear(); uc.mem_write(nodes,b'\0'*0xA00); uc.mem_write(entries,b'\0'*160); uc.mem_write(links,b'\0'*84)
        uc.mem_write(event_trees,b'\0'*200)
        put(manager,mt); put(manager+4,links); put(0x13B8710,manager); put(links,links); put(links+4,links)
        def append(i,node):
            n=links+12*i; tail=word(links+4); put(n,links); put(n+4,tail); put(n+8,node+4); put(tail,n); put(links+4,n)
        for i in range(5):
            n=nodes+i*0x200; e=entries+i*32; put(n,ct); put(n+4,ot); put(n+0xB0,e); put(n+0xB4,e); put(n+0xB8,e+32)
            if real_cleanup:
                head=event_trees+i*40; leaf=head+20; put(n+8,head); put(n+12,1)
                for offset in (4,8,12): put(head+offset,leaf)
                put(leaf+4,head); put(leaf+16,25)
            if mask&(1<<i): append(i+1,n)
        if duplicate: append(6,nodes)
        def child(parent,index,child):
            put(entries+parent*32+index*8,nodes+child*0x200); put(nodes+parent*0x200+0xB4,entries+parent*32+(index+1)*8)
        if shape==1:
            for i in range(1,5): child(0,i-1,i)
        if shape==2:
            for i in range(4): child(i,0,i+1)
        if shape==3:
            child(0,0,1); child(0,1,2); child(1,0,3); child(2,0,4)
        put(stack,stop); uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,nodes)
        uc.emu_start(0x530720 if operation else 0x5303F0,stop,count=10000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop: raise RuntimeError('Oracle did not return')
        events.append('L'); n=word(links)
        while n!=links:
            if word(word(n)+4)!=n or word(word(n+4))!=n: raise RuntimeError('Bad retail list links')
            events.append(f'{(n-links)//12}:{(word(n+8)-nodes-4)//0x200}'); n=word(n)
        if real_cleanup:
            for i in range(5):
                count=word(nodes+i*0x200+12); head=event_trees+i*40
                if not count and (word(head+4) or word(head+8)!=head or word(head+12)!=head): raise RuntimeError('Bad cleared event tree')
                events.append('E'+str(count))
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,actual=actual,retail=expected))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,real_event_cleanup=real_cleanup,scope='real Die/recursive cleanup/existing-instance singleton/unlink; events mode also executes real observer event-set clearing and tree free; state request/component hooks/free controlled'),indent=2)+'\n')
    print(f"UI_OBSERVER_CHAIN {'FAIL' if errors else 'PASS'} events={real_cleanup} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__=='__main__': raise SystemExit(main())
