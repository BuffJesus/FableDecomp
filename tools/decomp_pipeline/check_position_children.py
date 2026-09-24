#!/usr/bin/env python3
"""Check extracted PositionChildren pass against its actual base-Update instructions."""
import hashlib
import itertools
import json
import math
import random
import struct
import sys
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, parity, pe_oracle, run
from check_ui_transform import bits, number


def main():
    retiring='--retiring' in sys.argv[1:]
    frame='--frame' in sys.argv[1:]
    live='--live' in sys.argv[1:] or retiring or frame
    stop_addresses=(0x532C0D,0x532CD9) if retiring else (0x5325AB,0x532789)
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != '41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle executable changed')
    machine=Uc(UC_ARCH_X86,UC_MODE_32); machine.mem_map(0x400000,0x1100000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): machine.mem_write(0x400000+va,image[raw:raw+size])
    machine.mem_map(0x20000000,0x20000)
    stack,parent,table,head,nodes,children,query=0x20008000,0x20010000,0x20011000,0x20012000,0x20012100,0x20013000,0x20014000
    def put(address,value): machine.mem_write(address,struct.pack('<I',value))
    def word(address): return struct.unpack('<I',machine.mem_read(address,4))[0]
    setters={0x52E8D0:'P',0x52E910:'Z',0x52F230:'p',0x52F250:'z'}
    slots={0x48:0x52E8D0,0x50:0x52E910,0x1C8:0x52F230,0x1CC:0x52F250,
           0x198:query,0x19C:query+16,0x1D4:query+32,0x88:query+48,0x94:query+48,0xA0:query+48,
           0xD0:query+64,0xCC:query+80,0x1F8:query+96,0x190:query+112,0x58:query+128,0x04:query+144,
           0xC4:query+208,0xFC:0x52E850,0xF8:0x531E90,0x100:0x533BC0,0x144:query+240,0x158:query+256,0xC0:query+272}
    if frame: slots.update({0x94:query+160,0x88:query+176,0xA0:query+192})
    for slot,address in slots.items(): put(table+slot,address)
    for offset in (0,16,32): machine.mem_write(query+offset,b'\xc3')
    machine.mem_write(query+48,b'\xc2\x04\x00')
    for offset in (64,96,112): machine.mem_write(query+offset,b'\xc3')
    for offset in (80,128,144): machine.mem_write(query+offset,b'\xc2\x04\x00')
    for offset in (160,176,192,224): machine.mem_write(query+offset,b'\xc2\x04\x00')
    machine.mem_write(query+208,b'\xc3'); machine.mem_write(0xBFEA0E,b'\xc3'); machine.mem_write(0xBFEA14,b'\xc3')
    machine.mem_write(query+240,b'\xc3')
    machine.mem_write(query+256,b'\xc3'); machine.mem_write(query+272,b'\xc2\x04\x00')
    events,case,zoom_calls,allocation=[],[],0,0
    def hook(machine,address,size,data):
        nonlocal zoom_calls,allocation
        if live and not frame and address in stop_addresses: machine.emu_stop(); return
        if address in (query+160,query+176,query+192): events.append({query+160:'J',query+176:'K',query+192:'L'}[address])
        elif address==query+208:
            events.append('H'+str((machine.reg_read(UC_X86_REG_ECX)-children)//0x200)); machine.reg_write(UC_X86_REG_EAX,int(frame and case[5]==1))
        elif address==query+256:
            events.append('O'+str((machine.reg_read(UC_X86_REG_ECX)-children)//0x200)); machine.reg_write(UC_X86_REG_EAX,2)
        elif address==query+272:
            events.append('X'+str((machine.reg_read(UC_X86_REG_ECX)-children)//0x200)+':'+str(word(machine.reg_read(UC_X86_REG_ESP)+4)))
        elif address==0x52E850: events.append('D'+str((machine.reg_read(UC_X86_REG_ECX)-children)//0x200))
        elif address==0x531E90: events.append('B'+str((machine.reg_read(UC_X86_REG_ECX)-children)//0x200)+':'+str(word(machine.reg_read(UC_X86_REG_ESP)+4)))
        elif address==0xBFEA0E:
            if word(machine.reg_read(UC_X86_REG_ESP)+4)!=12: raise RuntimeError('Unexpected frame allocation')
            events.append('a'); machine.reg_write(UC_X86_REG_EAX,0x20018000+allocation*16); allocation+=1
        elif address==0xBFEA14: events.append('f')
        elif address==0x533BC0: events.append('E'+str(word(machine.reg_read(UC_X86_REG_ESP)+4)))
        elif address==query+240: events.append('Y'+str((machine.reg_read(UC_X86_REG_ECX)-children)//0x200))
        if address in (query+64,query+80,query+96,query+112,query+128,query+144):
            receiver=machine.reg_read(UC_X86_REG_ECX); child=(receiver-children)//0x200
            if address==query+64: events.append(f'G{child}'); machine.reg_write(UC_X86_REG_EAX,word(receiver+0xC8))
            elif address==query+80: events.append(f'T{child}'); put(receiver+0xC8,word(machine.reg_read(UC_X86_REG_ESP)+4))
            elif address==query+96: events.append(f'A{child}'); machine.reg_write(UC_X86_REG_EAX,word(receiver+0x118))
            elif address==query+112: events.append(f'F{child}'); machine.reg_write(UC_X86_REG_EAX,(case[0]>>3)&1)
            elif address==query+128: events.append(f'C{child}:{word(word(machine.reg_read(UC_X86_REG_ESP)+4))}')
            else: events.append(f'U{child}:{word(machine.reg_read(UC_X86_REG_ESP)+4)}')
        elif address==query:
            events.append('I'); machine.reg_write(UC_X86_REG_EAX,case[1])
            if case[5]==2: put(parent+0x74,bits(3))
        elif address==query+16:
            events.append(f'Q{zoom_calls}'); machine.reg_write(UC_X86_REG_EAX,(case[2]>>(zoom_calls%2))&1); zoom_calls+=1
        elif address==query+32:
            events.append('R'); machine.reg_write(UC_X86_REG_EAX,case[3])
            if case[5]==2: put(parent+0x38,bits(29))
        elif address in setters:
            pointer=word(machine.reg_read(UC_X86_REG_ESP)+4); child=(machine.reg_read(UC_X86_REG_ECX)-children)//0x200; kind=setters[address]
            events.append(f'{kind}{child}:{word(pointer)}:{word(pointer+4)}:{int(pointer==parent+0x34)}')
            if kind=='Z' and case[5]==1: put(parent+0x34,bits(17)); put(parent+0x38,bits(-23))
            if kind=='Z' and case[5]==3: put(nodes+0x10,children+0x600); put(0x20015000,children+0x600)
    machine.hook_add(UC_HOOK_CODE,hook)
    base=list(map(bits,(3,7,11,13,2,0.5,17,19,1.5,0.25,4,5,6,7,640,480,1280,720)))
    cases=[[*config,*base] for config in itertools.product(range(64 if frame else 16 if live else 5),range(2),range(4),range(2),range(2),range(4))]
    rng=random.Random(0x531EFC)
    for i in range(1200):
        values=[rng.uniform(-1000,1000) for _ in range(14)]+[640,480,1280,720]
        cases.append([1,rng.randrange(2),rng.randrange(4),rng.randrange(2),rng.randrange(2),0,*map(bits,values)])
    for location in range(6,20):
        for independent,zoom in itertools.product(range(2),range(4)):
            row=[1,independent,zoom,0,0,0,*base]; row[location]=0x80000000; cases.append(row)
    directory=ROOT/('work/base_component_update_check' if frame else 'work/retiring_child_update_check' if retiring else 'work/live_child_update_check' if live else 'work/position_children_check'); directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,row)) for row in cases)+'\n')
    env,objects=parity.env(),[]
    sources=['rebuild/tests/integration/PositionChildren_test.cpp','rebuild/src/compiled/00/52/global_ConvertCoordinates_0052e580.cpp',
        'rebuild/src/compiled/00/52/CComponent_GetDeletion_0052e850.cpp']
    sources+=['rebuild/src/compiled/00/53/'+name+'.cpp' for name in ('CComponent_Update_00531ec0','CComponent_SetDeletion_00531e90',
        'global_AssignDeletionParents_00535800','global_FindCountedChild_00534eb0','global_MoveCountedChildren_00535000',
        'global_EraseCountedChild_005354e0','global_ReallocateCountedChildren_005359d0','CComponent_RemoveChildAt_00533bc0')]
    sources+=['rebuild/src/compiled/00/42/'+name+'.cpp' for name in ('global_CopyDeletion_0042cd84','global_DestroyDeletionParents_0042abca')]
    for i,source in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs,'--frame' if frame else '--retiring' if retiring else '--live'] if live else [exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete child propagation traces')
    errors=[]; worst=0
    for index,(case,actual) in enumerate(zip(cases,lines)):
        events.clear(); zoom_calls=0; allocation=0; machine.mem_write(parent,b'\0'*0x200); put(parent,table); put(parent+0x11C,head)
        for offset,values in ((0x34,case[6:8]),(0x4C,case[8:10]),(0x74,case[10:12]),(0x100,case[12:14]),(0x110,case[14:16]),(0x7C,case[16:18]),(0x108,case[18:20])):
            machine.mem_write(parent+offset,struct.pack('<II',*values))
        machine.mem_write(0x13B8768,bytes((case[4],))); machine.mem_write(0x1375CD4,struct.pack('<II',*case[20:22])); machine.mem_write(0x13B876C,struct.pack('<II',*case[22:24]))
        machine.mem_write(head,b'\0'*0x200)
        for i in range(4): put(children+i*0x200,table)
        for i in range(3): put(nodes+i*0x20+0x10,children+i*0x200)
        for offset in (4,8,12): put(head+offset,head)
        if case[0]:
            for offset in (4,8,12): put(head+offset,nodes)
            put(nodes+4,head)
            if case[0]==2:
                put(head+4,nodes+0x20); put(head+12,nodes+0x40); put(nodes+0x24,head)
                put(nodes+0x28,nodes); put(nodes+0x2C,nodes+0x40); put(nodes+4,nodes+0x20); put(nodes+0x44,nodes+0x20)
            if case[0]==3:
                put(head+12,nodes+0x40); put(nodes+12,nodes+0x20); put(nodes+0x2C,nodes+0x40); put(nodes+0x24,nodes); put(nodes+0x44,nodes+0x20)
            if case[0]==4:
                put(head+8,nodes+0x40); put(nodes+8,nodes+0x20); put(nodes+0x28,nodes+0x40); put(nodes+0x24,nodes); put(nodes+0x44,nodes+0x20)
        if live:
            put(head+8,head); put(parent+0xB0,0x20015000); put(parent+0xB4,0x20015008); put(parent+0xB8,0x20015008)
            put(0x20015000,children); put(parent+0x94,0x12345678)
            for i in range(4):
                child=children+i*0x200
                put(child+0xC8,0 if case[0]&3==0 else (parent if case[0]&3==1 else child))
                put(child+0x118,parent if case[0]&4 else (child if case[0]&3==3 else 0))
            if retiring:
                put(parent+0xB4,0x20015000)
                for offset,value in ((0xBC,0x20015000),(0xC0,0x20015008),(0xC4,0x20015008)): put(parent+offset,value)
            if frame:
                put(parent+0xB4,0x20015018); put(parent+0xB8,0x20015020)
                put(parent+0xBC,0x20015100); put(parent+0xC0,0x20015108); put(parent+0xC4,0x20015140)
                put(0x20015100,children+0x600); put(0x20015104,0)
                for i in range(4):
                    child=children+i*0x200; put(0x20015000+i*8,child); put(0x20015004+i*8,0)
                    method=(case[0]>>4)&3; put(child+0xD4,method); h=child+0x180; put(child+0xD8,h); put(h,h); put(h+4,h)
                    if method==3:
                        node=h+16; put(h,node); put(h+4,node); put(node,h); put(node+4,h); put(node+8,parent)
        put(stack,0x20000000); put(stack+4,bits(0.25) if live else 0)
        machine.reg_write(UC_X86_REG_ESP,stack); machine.reg_write(UC_X86_REG_ECX,parent); machine.reg_write(UC_X86_REG_FPCW,0x37F)
        # Enter the real function, then stop exactly before its live-child pass.
        machine.emu_start(0x531EC0,0x20000000 if frame else 0x532CF1 if live else 0x5321E0,count=30000)
        if machine.reg_read(UC_X86_REG_EIP) not in ((0x20000000,) if frame else stop_addresses if live else (0x5321E0,)): raise RuntimeError('Child pass did not finish')
        expected='TRACE'+''.join(' '+event for event in events)+' END'
        if frame:
            live_size=(word(parent+0xB4)-word(parent+0xB0))//8; retiring_size=(word(parent+0xC0)-word(parent+0xBC))//8
            expected+=f' {live_size}:{retiring_size}:{word(parent+0x30)}'
            expected+=''.join(' V'+str((word(word(parent+0xB0)+i*8)-children)//0x200) for i in range(live_size))
            expected+=''.join(' W'+str((word(word(parent+0xBC)+i*8)-children)//0x200) for i in range(retiring_size))
        actual_tokens,expected_tokens=actual.split(),expected.split()
        equal=len(actual_tokens)==len(expected_tokens)
        for a,b in zip(actual_tokens,expected_tokens):
            if a==b: continue
            if ':' not in a or ':' not in b: equal=False; continue
            av,bv=a.split(':'),b.split(':')
            if len(av)!=4 or len(bv)!=4 or av[0]!=bv[0] or av[3]!=bv[3]: equal=False; continue
            for xbits,ybits in zip(av[1:3],bv[1:3]):
                x,y=number(int(xbits)),number(int(ybits))
                if math.isfinite(x) and math.isfinite(y):
                    error=abs(x-y); worst=max(worst,error)
                    if error>0.0001+abs(y)*0.000002 or (x==y==0 and xbits!=ybits): equal=False
                elif not ((math.isnan(x) and math.isnan(y)) or x==y): equal=False
        if not equal: errors.append(dict(case=index,actual=actual,retail=expected))
    report=dict(accepted=not errors,cases=len(cases),errors=errors,maximum_absolute_error=worst,
        scope=('complete base Update with three live and one initially retiring child; dynamic removal and both traversal loops' if frame else 'retiring child preparation/update before completion/removal' if retiring else 'live child preparation/update before deletion' if live else 'PositionChildren pass 00531EFC..005321E0 only'),
        dependencies='real coordinate conversion and vector setters; full-frame mode also uses actual deletion/list and RemoveChildAt/counting helpers; queries/local update/child colour/child update/Die/completion callbacks doubled',
        comparison='exact callback order, receiver and direct-position pointer identity; vector tolerance 0.0001+abs(retail)*0.000002; signed zero exact')
    (directory/'report.json').write_text(json.dumps(report,indent=2)+'\n')
    print(f"CHILD_PROPAGATION {'FAIL' if errors else 'PASS'} live={live} retiring={retiring} frame={frame} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__=='__main__': raise SystemExit(main())
