#!/usr/bin/env python3
"""Coordinate-mode setup connected to real coordinate getters and UI scale."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x20000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,system,display,output=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000
    def put(p,v): uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    uc.mem_write(0x9A4EC0,b'\xc3'); uc.mem_write(0x9BEDC0,b'\xc2\x04\x00'); put(system+0x60,display)
    events=[]; extent=(0,0)
    def hook(uc,address,size,data):
        if address==0x9A4EC0: events.append('S'); uc.reg_write(UC_X86_REG_EAX,system)
        elif address==0x9BEDC0:
            if uc.reg_read(UC_X86_REG_ECX)!=display: raise RuntimeError('Wrong display receiver')
            events.append('D'+str(uc.mem_read(0x13B8768,1)[0])); target=word(uc.reg_read(UC_X86_REG_ESP)+4)
            put(target,extent[0]); put(target+4,extent[1])
    uc.hook_add(UC_HOOK_CODE,hook)
    dimensions=((640,480),(1024,768),(1920,1080),(0,0),(-1,-1),(2147483647,-2147483648))
    cases=[(*c,*d) for c in itertools.product((0,1,255),range(8),range(2),range(2)) for d in dimensions]
    directory=ROOT/'work/ui_coordinate_setup_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c)) for c in cases)+'\n')
    sources=['rebuild/tests/integration/UiCoordinateSetup_test.cpp','rebuild/src/compiled/00/42/global_SetRelativeCoordinates_004299a8.cpp']
    sources+=['rebuild/src/compiled/00/41/'+s+'.cpp' for s in ('CManager_GetUIScale_0041cf47','global_GetCoordinateWidth_0041cc14','global_GetCoordinateHeight_0041cc2b')]
    env=parity.env(); objects=[]
    for i,s in enumerate(sources):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete coordinate traces')
    def execute(address,receiver,args=()):
        put(stack,stop)
        for i,v in enumerate(args): put(stack+4+i*4,v)
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,receiver); uc.reg_write(UC_X86_REG_FPCW,0x37F); uc.emu_start(address,stop,count=10000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop: raise RuntimeError('Oracle did not return')
    errors=[]
    for index,(case,actual) in enumerate(zip(cases,lines)):
        initial,sequence,context,seed,width,height=case; events.clear()
        uc.mem_write(0x13B8768,bytes([initial])); uc.mem_write(0x13B876C,struct.pack('<ff',-1 if seed else 1920,0 if seed else 1080)); uc.mem_write(0x1375CD4,struct.pack('<ff',1280,720)); put(0x13B86A0,display if context else 0)
        for i in range(3):
            extent=(width+i,height+i); execute(0x4299A8,(sequence>>i)&1); execute(0x41CF47,0,(output,))
            events.append(f'R{uc.mem_read(0x13B8768,1)[0]}:{word(0x13B876C):08x}:{word(0x13B8770):08x}:{word(output):08x}:{word(output+4):08x}')
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=case,actual=actual,retail=expected))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,scope='complete relative-coordinate setter plus dimension getters and manager scale; display/system services doubled; exact destination/scale float bits and query timing'),indent=2)+'\n')
    print(f"UI_COORDINATE_SETUP {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__=='__main__': raise SystemExit(main())
