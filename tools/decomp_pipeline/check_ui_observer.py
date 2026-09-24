#!/usr/bin/env python3
"""Native Die -> observer removal, including mutation during recursive traversal."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle executable changed')
    machine=Uc(UC_ARCH_X86,UC_MODE_32); machine.mem_map(0x400000,0x1100000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): machine.mem_write(0x400000+va,image[raw:raw+size])
    machine.mem_map(0x20000000,0x20000)
    stop,stack,nodes,entries,table,managers,manager_table,replacement,callback=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20013000,0x20013100,0x20014000,0x20015000
    def put(address,value): machine.mem_write(address,struct.pack('<I',value))
    def word(address): return struct.unpack('<I',machine.mem_read(address,4))[0]
    put(table+0x214,callback); put(table+0xC0,callback+16); put(manager_table+0x14,callback+32)
    put(managers,manager_table); put(managers+4,manager_table)
    machine.mem_write(callback,b'\xc3'); machine.mem_write(callback+16,b'\xc2\x04\x00'); machine.mem_write(callback+32,b'\xc2\x04\x00'); machine.mem_write(0x41E5F2,b'\xc3')
    events,calls,case=[],0,[]
    def hook(machine,address,size,data):
        nonlocal calls
        receiver=machine.reg_read(UC_X86_REG_ECX); esp=machine.reg_read(UC_X86_REG_ESP)
        if address==0x41E5F2:
            manager=calls%2 if case[3] else 0; calls+=1; events.append('M'+str(manager)); machine.reg_write(UC_X86_REG_EAX,managers+manager*4)
        elif address==callback+32:
            node=word(esp+4)-4; events.append(f'R{(receiver-managers)//4}:{(node-nodes)//0x200}')
            if case[2]==3 and node==nodes:
                end=word(nodes+0xB4); machine.mem_write(end,bytes(machine.mem_read(replacement,8))); put(nodes+0xB4,end+8)
        elif address==callback:
            events.append('H'+str((receiver-nodes)//0x200))
            if case[2]==1 and receiver==nodes: put(nodes+0xB0,replacement); put(nodes+0xB4,replacement+16)
            if case[2]==2 and receiver==nodes+0x200: put(nodes+0xB4,word(nodes+0xB0)+8)
        elif address==callback+16:
            events.append(f'S{(receiver-nodes)//0x200}:{word(esp+4)}')
            if case[2]==4: put(nodes+0xB0,replacement); put(nodes+0xB4,replacement+8)
    machine.hook_add(UC_HOOK_CODE,hook)
    cases=list(itertools.product(range(2),range(4),range(5),range(2)))
    directory=ROOT/'work/ui_observer_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,row)) for row in cases)+'\n')
    sources=['rebuild/tests/integration/UiObserver_test.cpp']+['rebuild/src/compiled/00/53/'+name+'.cpp' for name in ('CComponent_Die_00530720','CComponent_RemoveObserverRecursive_005303f0')]
    env,objects=parity.env(),[]
    for i,source in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete observer traces')
    errors=[]
    for index,(case,actual) in enumerate(zip(cases,lines)):
        events.clear(); calls=0; machine.mem_write(nodes,b'\0'*0xA00); machine.mem_write(entries,b'\0'*0x200)
        put(replacement,nodes+0x800); put(replacement+8,nodes+0x400)
        for i in range(5):
            n=nodes+i*0x200; e=entries+i*32; put(n,table)
            for offset in (0xB0,0xB4): put(n+offset,e)
            put(n+0xB8,e+32); put(n+0xBC,replacement); put(n+0xC0,replacement+8)
        if case[1]:
            put(entries,nodes+0x200); put(entries+8,nodes+0x400); put(nodes+0xB4,entries+16)
            if case[1]==2: put(entries+32,nodes+0x600); put(nodes+0x200+0xB4,entries+40)
            if case[1]==3:
                put(nodes+0xB4,entries+8)
                for i in range(1,3): put(entries+i*32,nodes+(i+1)*0x200); put(nodes+i*0x200+0xB4,entries+i*32+8)
        put(stack,stop); machine.reg_write(UC_X86_REG_ESP,stack); machine.reg_write(UC_X86_REG_ECX,nodes)
        machine.emu_start(0x530720 if case[0] else 0x5303F0,stop,count=10000)
        if machine.reg_read(UC_X86_REG_EIP)!=stop: raise RuntimeError('Observer traversal did not finish')
        expected='TRACE'+''.join(' '+event for event in events)+' END'
        if actual!=expected: errors.append(dict(case=index,actual=actual,retail=expected))
    report=dict(accepted=not errors,cases=len(cases),errors=errors,scope='complete Die and RemoveObserverRecursive; manager lookup/unregistration, state request and observer hook doubled; exact callback traces including secondary-interface pointer and live collection mutation')
    (directory/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f"UI_OBSERVER {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__=='__main__': raise SystemExit(main())
