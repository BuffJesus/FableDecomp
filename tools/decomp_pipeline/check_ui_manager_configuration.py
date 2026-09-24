#!/usr/bin/env python3
"""SetInput and SetMetaLayer retail comparisons with controlled map services."""
import hashlib
import itertools
import json
import struct
import sys
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    real_maps='--maps' in sys.argv
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x20000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,manager,keys,layers=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000
    pool=0x20013000; allocated=0; tracing=False; key_argument=0x20015000
    def put(p,v): uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p): return struct.unpack('<i',uc.mem_read(p,4))[0]
    if not real_maps:
        for p in (0x42D201,0x42D246): uc.mem_write(p,b'\xc2\x04\x00')
    else: uc.mem_write(0xBFEA0E,b'\xc3')
    events=[]
    def hook(uc,address,size,data):
        nonlocal allocated
        if real_maps:
            if address==0xBFEA0E:
                size=word(uc.reg_read(UC_X86_REG_ESP)+4)
                if size!=24 or allocated>=128: raise RuntimeError('Unexpected map allocation')
                if tracing: events.append(f'A{allocated}:{size}')
                uc.reg_write(UC_X86_REG_EAX,pool+24*allocated); allocated+=1
            return
        if address not in (0x42D201,0x42D246): return
        iskey=address==0x42D201; receiver=uc.reg_read(UC_X86_REG_ECX); esp=uc.reg_read(UC_X86_REG_ESP); key=signed(word(esp+4))
        if receiver!=manager+(0x24 if iskey else 0x70): raise RuntimeError('Wrong map receiver')
        events.append(('K' if iskey else 'L')+f'{key}:{signed(manager+(0x30 if iskey else 0x7C))}')
        uc.reg_write(UC_X86_REG_EAX,(keys if iskey else layers)+(key+3)*4)
    uc.hook_add(UC_HOOK_CODE,hook)
    values=(-2147483648,-1,0,1,2,4,5,2147483647)
    cases=list(itertools.product(range(2),(0,1000,0xFFFFFFF0),values,values,values))
    directory=ROOT/('work/ui_manager_maps_check' if real_maps else 'work/ui_manager_configuration_check'); directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c)) for c in cases)+'\n')
    sources=['rebuild/tests/integration/UiManagerConfiguration_test.cpp','rebuild/src/compiled/00/41/CManager_SetInput_0041df10.cpp','rebuild/src/compiled/00/41/CManager_SetMetaLayer_0041e1cd.cpp']
    if real_maps:
        sources[0]='rebuild/tests/integration/UiManagerMaps_test.cpp'
        sources+=['rebuild/src/compiled/00/42/'+s+'.cpp' for s in ('global_LookupInputKey_0042d201','global_LookupLayer_0042d246','global_RotateEventTreeLeft_0042951b','global_RotateEventTreeRight_0042955b','global_BalanceEventTree_0042971d')]
    env=parity.env(); objects=[]
    for i,s in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete configuration traces')
    errors=[]
    def execute(address,receiver,args):
        put(stack,stop)
        for i,value in enumerate(args): put(stack+4+i*4,value)
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,receiver); uc.emu_start(address,stop,count=30000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop: raise RuntimeError('Oracle did not return')
    def node_id(p): return (p-pool)//24 if p else -1
    for index,(case,actual) in enumerate(zip(cases,lines)):
        op,seed,*sequence=case; events.clear(); uc.mem_write(manager,b'\xA5'*0x80)
        if real_maps:
            allocated=2; tracing=False; uc.mem_write(pool,b'\xCD'*(128*24))
            for i,offset in enumerate((0x24,0x70)):
                head=pool+24*i; put(manager+offset,head); put(manager+offset+4,0); uc.mem_write(head,b'\0'); put(head+4,0); put(head+8,head); put(head+12,head)
            for i in range(0,24,2):
                put(key_argument,i-3)
                execute(0x42D201,manager+0x24,(key_argument,)); put(uc.reg_read(UC_X86_REG_EAX),seed+i)
                execute(0x42D246,manager+0x70,(key_argument,)); put(uc.reg_read(UC_X86_REG_EAX),seed-i)
            tracing=True
        else:
            for i in range(24): put(keys+4*i,seed+i); put(layers+4*i,seed-i)
        for value in sequence:
            execute(0x41E1CD if op else 0x41DF10,manager,(value,))
        events.extend((f'I{signed(manager+0x30)}',f'M{signed(manager+0x7C)}'))
        if real_maps:
            events.extend((f'K{word(manager+0x28)}',f'L{word(manager+0x74)}'))
            for i in range(allocated):
                n=pool+24*i; events.append(f'T{i}:{uc.mem_read(n,1)[0]}:{node_id(word(n+4))}:{node_id(word(n+8))}:{node_id(word(n+12))}:{signed(n+16)}:{signed(n+20)}')
        else:
            events.append('K'); events.extend(str(signed(keys+4*i)) for i in range(24)); events.append('L'); events.extend(str(signed(layers+4*i)) for i in range(24))
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=case,actual=actual,retail=expected))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,real_maps=real_maps,scope='complete SetInput/SetMetaLayer; base mode observes controlled map services; maps mode executes real map lookup/insertion/rotations and compares all node links/colors/keys/values and allocations, using manually initialized sentinels'),indent=2)+'\n')
    print(f"UI_MANAGER_CONFIGURATION {'FAIL' if errors else 'PASS'} maps={real_maps} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__=='__main__': raise SystemExit(main())
