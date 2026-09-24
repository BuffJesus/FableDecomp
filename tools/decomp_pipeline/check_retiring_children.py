#!/usr/bin/env python3
"""Exercise the native retiring-child ownership tail and counted-pointer helpers."""
import hashlib
import itertools
import json
import random
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_EDI, UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle executable changed')
    machine=Uc(UC_ARCH_X86,UC_MODE_32); machine.mem_map(0x400000,0x1100000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): machine.mem_write(0x400000+va,image[raw:raw+size])
    machine.mem_map(0x20000000,0x20000)
    stack,parent,table,head,children,entries,refs,callback=0x20008000,0x20010000,0x20011000,0x20012000,0x20013000,0x20014000,0x20015000,0x20016000
    stop=0x20000000
    def put(address,value): machine.mem_write(address,struct.pack('<I',value))
    def word(address): return struct.unpack('<I',machine.mem_read(address,4))[0]
    # Only the ownership tail is under test. The function prologue establishes
    # the actual frame; local updates are no-ops, and the preceding child pass
    # is bypassed at its entry with ESI/EDI in the same state as retail iteration.
    for slot in (0x88,0x94,0xA0): put(table+slot,callback)
    machine.mem_write(callback,b'\xc2\x04\x00')
    for slot,offset in ((0xC4,16),(0x158,32),(0xC0,48),(0xCC,64)): put(table+slot,callback+offset)
    for offset in (16,32,80): machine.mem_write(callback+offset,b'\xc3')
    for offset in (48,64): machine.mem_write(callback+offset,b'\xc2\x04\x00')
    machine.mem_write(0xBFE9BC,b'\xc3')
    machine.mem_write(0xBFEA0E,b'\xc3'); machine.mem_write(0xBFEA14,b'\xc3'); machine.mem_write(callback+96,b'\xc3'); put(table+0x144,callback+96)
    machine.mem_write(0xBFEAE6,b'\xc3')
    events,case=[],[]
    def hook(machine,address,size,data):
        if address==0x5327C0:
            machine.reg_write(UC_X86_REG_EDI,case[1]); machine.reg_write(UC_X86_REG_EIP,0x532C0D); return
        if address==0x532CD9: machine.emu_stop(); return
        receiver=machine.reg_read(UC_X86_REG_ECX); child=(receiver-children)//0x200
        if address==callback+16: events.append(f'H{child}'); machine.reg_write(UC_X86_REG_EAX,case[2])
        elif address==callback+32: events.append(f'S{child}'); machine.reg_write(UC_X86_REG_EAX,case[3])
        elif address==callback+48: events.append(f'R{child}:{word(machine.reg_read(UC_X86_REG_ESP)+4)}')
        elif address==callback+64: events.append(f'P{child}'); put(receiver+0xC8,word(machine.reg_read(UC_X86_REG_ESP)+4))
        elif address==callback+80: events.append(f'D{child}')
        elif address==0xBFE9BC: events.append(f'F{(word(machine.reg_read(UC_X86_REG_ESP)+4)-refs)//16}')
        elif address==0xBFEA0E: events.append('A'+str(word(machine.reg_read(UC_X86_REG_ESP)+4))); machine.reg_write(UC_X86_REG_EAX,0x20018000)
        elif address==0xBFEA14: events.append('V')
        elif address==callback+96: events.append(f'Y{child}')
        elif address==0xBFEAE6:
            esp=machine.reg_read(UC_X86_REG_ESP); destination,source,length=word(esp+4),word(esp+8),word(esp+12)
            machine.mem_write(destination,bytes(machine.mem_read(source,length))); machine.reg_write(UC_X86_REG_EAX,destination)
    machine.hook_add(UC_HOOK_CODE,hook)
    cases=[]
    for size,complete,current,deletion,external,pattern in itertools.product(range(1,5),range(2),(0,2,3),range(4),(0,2),range(4)):
        for index in range(size):
            data=[0,1,2,3] if pattern<2 else [0,1,0,1]
            info=[0,1,2,3] if pattern==0 else [4,4,4,4] if pattern==1 else [0,1,0,1] if pattern==2 else [0,0,0,0]
            cases.append([size,index,complete,current,deletion,external,*data,*info])
    rng=random.Random(0x532C0D)
    ownership_cases=len(cases)+700
    for i in range(700):
        size=rng.randrange(1,5)
        cases.append([size,rng.randrange(size),1,2,rng.randrange(4),rng.randrange(3),
            *[rng.randrange(4) for _ in range(4)],*[rng.randrange(5) for _ in range(4)]])
    for method,size,pattern in itertools.product((2,3),range(5),range(4)):
        for index in (range(4) if method==2 else range(size+1)):
            data=[0,1,2,3] if pattern<2 else [0,1,0,1]
            info=[0,1,2,3] if pattern==0 else [4]*4 if pattern==1 else [0,1,0,1] if pattern==2 else [0]*4
            cases.append([size,index,method,0,0,0,*data,*info])
    for method,size,pattern,external in itertools.product((4,5,6),range(5),range(4),(0,2)):
        if size==0 and method!=5: continue
        for index in (range(size+1) if method==5 else range(size)):
            for count,flag in (itertools.product((0,1,3),range(2)) if method==5 else itertools.product(range(3),range(2)) if method==6 else ((0,0),)):
                data=[0,1,2,3] if pattern<2 else [0,1,0,1]
                info=[0,1,2,3] if pattern==0 else [4]*4 if pattern==1 else [0,1,0,1] if pattern==2 else [0]*4
                cases.append([size,index,method,count,flag,external,*data,*info])
    directory=ROOT/'work/retiring_children_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,row)) for row in cases)+'\n')
    env,objects=parity.env(),[]
    sources=['rebuild/tests/integration/RetiringChildren_test.cpp']+['rebuild/src/compiled/00/53/'+name+'.cpp' for name in (
        'global_FindCountedChild_00534eb0','global_MoveCountedChildren_00535000','global_EraseCountedChild_005354e0',
        'global_ReallocateCountedChildren_005359d0','CComponent_RemoveChildAt_00533bc0')]
    for i,source in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete retirement results')
    errors=[]
    for index,(case,actual) in enumerate(zip(cases,lines)):
        events.clear(); machine.mem_write(parent,b'\0'*0x200); put(parent,table); put(parent+0x11C,head); put(head+8,head)
        for offset,value in ((0xB0,entries),(0xB4,entries),(0xBC,entries),(0xC0,entries+case[0]*8),(0xC4,entries+32)): put(parent+offset,value)
        for i in range(4):
            child=children+i*0x200; machine.mem_write(child,b'\0'*0x200); put(child,table); put(child+0xC8,parent); put(child+0xD4,case[4])
            put(refs+i*16,case[5]+case[10:10+case[0]].count(i)); put(refs+i*16+4,callback+80); put(refs+i*16+8,child)
            put(entries+i*8,children+case[6+i]*0x200); put(entries+i*8+4,refs+case[10+i]*16 if case[10+i]<4 else 0)
        if case[2]==6:
            put(parent+0xB4,entries+case[0]*8); put(parent+0xB8,entries+32)
            put(parent+0xBC,0x20017000); put(parent+0xC0,0x20017000+case[3]*8); put(parent+0xC4,0x20017000+(16 if case[4] else case[3])*8)
            for r in range(case[3]):
                machine.mem_write(0x20017000+r*8,bytes(machine.mem_read(entries+((r+2)%4)*8,8)))
                info=word(0x20017004+r*8)
                if info: put(info,word(info)+1)
            put(parent+0xE4,0x20019000); put(parent+0xE8,0x20019018); put(parent+0xEC,0x20019018)
            machine.mem_write(0x20019000,struct.pack('<6I',0,2,1,3,1,5))
        put(stack,0x20000000); put(stack+4,0)
        machine.reg_write(UC_X86_REG_ESP,stack); machine.reg_write(UC_X86_REG_ECX,parent); machine.reg_write(UC_X86_REG_FPCW,0x37F)
        if case[2] in (2,3):
            machine.reg_write(UC_X86_REG_EDX,entries+case[0]*8)
            machine.reg_write(UC_X86_REG_ECX,entries if case[2]==2 else entries+case[1]*8)
            put(stack+4,entries+case[1]*8 if case[2]==2 else entries); put(stack+8,0); put(stack+12,0)
            machine.emu_start(0x534EB0 if case[2]==2 else 0x535000,0x20000000,count=10000)
            if machine.reg_read(UC_X86_REG_EIP)!=0x20000000: raise RuntimeError('Counted child helper did not return')
            result=(machine.reg_read(UC_X86_REG_EAX)-entries)//8
        elif case[2] in (4,5,6):
            operation=case[2]
            machine.reg_write(UC_X86_REG_ECX,parent if operation==6 else parent+0xBC)
            args=[stop,case[1]] if operation==6 else [stop,entries+case[1]*8,entries,0,case[3],case[4]] if operation==5 else [stop,entries+case[1]*8]
            machine.mem_write(stack,struct.pack('<'+'I'*len(args),*args))
            try: machine.emu_start({4:0x5354E0,5:0x5359D0,6:0x533BC0}[operation],0x20000000,count=10000)
            except Exception as error:
                raise RuntimeError(f'Native operation failed: case={index} values={case} eip={machine.reg_read(UC_X86_REG_EIP):08X} events={events}') from error
            if machine.reg_read(UC_X86_REG_EIP)!=0x20000000: raise RuntimeError('Counted vector operation did not return')
            result=((word(parent+0xB4)-word(parent+0xB0)) if operation==6 else (word(parent+0xC0)-word(parent+0xBC)))//8
        else:
            machine.emu_start(0x531EC0,0x20000000,count=10000)
            if machine.reg_read(UC_X86_REG_EIP)!=0x532CD9: raise RuntimeError('Retirement tail did not finish')
            result=(word(parent+0xC0)-entries)//8
        values=[result]
        for i in range(4):
            obj,info=word(entries+i*8),word(entries+i*8+4)
            values.extend(((obj-children)//0x200+1 if obj else 0,(info-refs)//16+1 if info else 0))
        for i in range(4): values.extend((word(refs+i*16),int(bool(word(children+i*0x200+0xC8)))))
        expected='TRACE'+''.join(' '+event for event in events)+' END '+' '.join(map(str,values))
        if case[2]>=4:
            begin,end,capacity=word(parent+0xBC),word(parent+0xC0),word(parent+0xC4)
            extra=[(end-begin)//8,(capacity-begin)//8]
            for address in range(begin,end,8):
                obj,info=word(address),word(address+4); extra.extend(((obj-children)//0x200+1 if obj else 0,(info-refs)//16+1 if info else 0))
            expected+=' EXTRA '+' '.join(map(str,extra))
            if case[2]==6: expected+=' SHAPE'+''.join(' '+str(word(a)) for a in range(word(parent+0xE4),word(parent+0xE8),4))
        if actual!=expected: errors.append(dict(case=index,actual=actual,retail=expected))
    report=dict(accepted=not errors,cases=len(cases),ownership_cases=ownership_cases,helper_cases=len(cases)-ownership_cases,errors=errors,
        scope='retiring tail plus direct find/move/erase/reallocation and RemoveChildAt; prior child pass bypassed; state/Die/destructor/allocator boundaries observed doubles; CRT memmove doubled',comparison='exact traces, entries, reference counts, capacities, allocation sizes, shape indices, returned offsets and parent links')
    (directory/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f"RETIRING_CHILDREN {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__=='__main__': raise SystemExit(main())
