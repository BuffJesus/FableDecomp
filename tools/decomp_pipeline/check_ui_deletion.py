#!/usr/bin/env python3
"""Native deletion snapshots/list ownership, including allocation/free histories."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDI, UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle executable changed')
    machine=Uc(UC_ARCH_X86,UC_MODE_32); machine.mem_map(0x400000,0x1100000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): machine.mem_write(0x400000+va,image[raw:raw+size])
    machine.mem_map(0x20000000,0x40000)
    stop,stack,parent,child,table,head,entries,other,callback,arena=0x20000000,0x20008000,0x20010000,0x20010400,0x20011000,0x20012000,0x20013000,0x20014000,0x20015000,0x20020000
    values=(parent,0x20010800,0x20010C00)
    def put(address,value): machine.mem_write(address,struct.pack('<I',value))
    def word(address): return struct.unpack('<I',machine.mem_read(address,4))[0]
    for slot in (0x88,0x94,0xA0): put(table+slot,callback)
    put(table+0xFC,0x52E850); put(table+0xF8,0x531E90); put(table+0x100,callback+16)
    machine.mem_write(callback,b'\xc2\x04\x00'); machine.mem_write(callback+16,b'\xc2\x04\x00')
    machine.mem_write(0xBFEA0E,b'\xc3'); machine.mem_write(0xBFEA14,b'\xc3')
    allocations,active,events,recording,case=0,set(),[],False,[]
    def allocate(size):
        nonlocal allocations
        if size!=12 or allocations>=512: raise RuntimeError('Unexpected deletion allocation')
        result=arena+allocations*16; active.add(allocations)
        if recording: events.append('A'+str(allocations))
        allocations+=1; return result
    def free(pointer):
        index=(pointer-arena)//16
        if index not in active: raise RuntimeError('Native invalid/double free')
        active.remove(index)
        if recording: events.append('F'+str(index))
    def destroy(h):
        node=word(h)
        while node!=h:
            next_node=word(node); free(node); node=next_node
        put(h,h); put(h+4,h); free(h)
    def make(address,method,count,pattern):
        put(address,method); h=allocate(12); put(address+4,h); put(h,h); put(h+4,h)
        for i in range(count):
            node=allocate(12); last=word(h+4); put(node,h); put(node+4,last); put(node+8,values[(i+pattern)%3]); put(last,node); put(h+4,node)
    def hook(machine,address,size,data):
        if case[0]==0 and address==0x532200:
            machine.reg_write(UC_X86_REG_EDI,0); machine.reg_write(UC_X86_REG_EIP,0x5325AB); return
        if case[0]==0 and address==0x532789: machine.emu_stop(); return
        esp=machine.reg_read(UC_X86_REG_ESP)
        if address==0xBFEA0E: machine.reg_write(UC_X86_REG_EAX,allocate(word(esp+4)))
        elif address==0xBFEA14: free(word(esp+4))
        elif case[0]==0 and address==0x52E850: events.append('G')
        elif case[0]==0 and address==0x531E90: events.append('S'+str(word(esp+4)))
        elif address==callback+16:
            events.append('R'+str(word(esp+4)))
            if case[5]:
                destroy(word(child+0xD8)); put(child+0xD4,9); h=allocate(12); put(h,h); put(h+4,h); put(child+0xD8,h)
    machine.hook_add(UC_HOOK_CODE,hook)
    cases=list(itertools.product(range(6),range(5),range(5),range(5),range(3),range(2)))
    directory=ROOT/'work/ui_deletion_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,row)) for row in cases)+'\n')
    sources=['rebuild/tests/integration/UiDeletion_test.cpp','rebuild/src/compiled/00/53/global_AssignDeletionParents_00535800.cpp',
        'rebuild/src/compiled/00/42/global_DestroyDeletionParents_0042abca.cpp','rebuild/src/compiled/00/42/global_CopyDeletion_0042cd84.cpp',
        'rebuild/src/compiled/00/52/CComponent_GetDeletion_0052e850.cpp','rebuild/src/compiled/00/53/CComponent_SetDeletion_00531e90.cpp']
    env,objects=parity.env(),[]
    for i,source in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete deletion results')
    errors=[]
    def dump(address):
        h=word(address+4); result=f' {word(address)}:H{(h-arena)//16}'; node=word(h); previous=h; count=0
        while node!=h:
            if word(node+4)!=previous or count>32: raise RuntimeError('Invalid deletion links')
            result+=f' {values.index(word(node+8))}:N{(node-arena)//16}'; previous=node; node=word(node); count+=1
        if word(h+4)!=previous: raise RuntimeError('Invalid deletion tail')
        return result+' /'
    for index,(case,actual) in enumerate(zip(cases,lines)):
        allocations=0; active.clear(); events.clear(); recording=False; machine.mem_write(arena,b'\xCC'*8192)
        machine.mem_write(parent,b'\0'*0x200); machine.mem_write(child,b'\0'*0x200); put(parent,table); put(child,table)
        put(parent+0x11C,head); put(head+8,head); put(parent+0xB0,entries); put(parent+0xB4,entries+8); put(entries,child)
        make(child+0xD4,case[1],case[2],case[4])
        if case[0]!=3: make(other,case[1]+1,case[3],case[4]+1)
        operation=case[0]; addresses=(0x531EC0,0x535800,0x531E90,0x42CD84,0x42ABCA,0x535800)
        receiver=parent if operation==0 else child+0xD8 if operation in (1,4,5) else child if operation==2 else other
        args=[stop,word(other),word(other+4)] if operation==2 else [stop,other+4 if operation==1 else child+0xD8 if operation==5 else child+0xD4 if operation==3 else 0]
        machine.mem_write(stack,struct.pack('<'+'I'*len(args),*args)); machine.reg_write(UC_X86_REG_ESP,stack); machine.reg_write(UC_X86_REG_ECX,receiver); machine.reg_write(UC_X86_REG_FPCW,0x37F)
        recording=True; machine.emu_start(addresses[operation],stop,count=50000)
        if machine.reg_read(UC_X86_REG_EIP)!=(0x532789 if operation==0 else stop): raise RuntimeError('Deletion operation did not finish')
        if operation in (1,3,5) and machine.reg_read(UC_X86_REG_EAX)!=receiver: raise RuntimeError('Unexpected deletion return pointer')
        expected='TRACE'+''.join(' '+event for event in events)+' END'+(dump(child+0xD4) if operation!=4 else ' X /')+(dump(other) if operation!=2 else ' X /')+' LIVE'+''.join(' '+str(i) for i in sorted(active))
        if actual!=expected: errors.append(dict(case=index,actual=actual,retail=expected))
    getter,_,_=parity.obj_text(objects[4],'?FableUiGetDeletion@@')
    offset=pe_oracle.va_to_off(pe_oracle.pe_sections(image),0x52E850)
    getter_match=getter==image[offset:offset+7]
    report=dict(accepted=not errors and getter_match,cases=len(cases),errors=errors,getter_parity='MATCH' if getter_match else 'DIFFER',scope='complete native deletion getter/setter/copy/assign/destructor plus base Update deletion-request stage; removal callback doubled',comparison='exact allocation/free/callback order, node reuse, list values/links, methods and live allocation sets')
    (directory/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f"UI_DELETION {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}")
    print('UI_DELETION_GETTER '+report['getter_parity'])
    return int(bool(errors) or not getter_match)


if __name__=='__main__': raise SystemExit(main())
