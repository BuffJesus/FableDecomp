#!/usr/bin/env python3
"""Bank constructor/initializer, resource lists and texture-manager ownership."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run

FUNCTIONS=[
 ('00/9f/CGraphicDataBank_Construct_009fea20',0x9FEA20,337,'FableUiConstructGraphicsBank'),
 ('00/9f/CGraphicDataBank_Initialise_009fd4e0',0x9FD4E0,343,'FableUiInitialiseGraphicsBank'),
 ('00/9f/CResourceBank_Construct_009fc5f0',0x9FC5F0,156,'FableUiConstructResourceBank'),
 ('00/99/CBase_ConstructUiResourceBase_0099a2f0',0x99A2F0,9,'FableUiConstructBase'),
 ('00/a0/CCountedPointer_ResetTextureManager_00a002e0',0xA002E0,103,'FableUiResetTextureManager'),
 ('00/9f/CCountedPointer_DeleteTextureManager_009ffd20',0x9FFD20,11,'FableUiDeleteTextureManager'),
 ('00/9d/CBankFile_SetPreloadPolicy_009d5230',0x9D5230,3,'FableUiSetBankPreloadPolicy'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]

def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x40000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,bank,managers,refs,init,vtable,delete_cb,alternate=0x20000000,0x20008000,0x20010000,0x20011000,0x20013000,0x20014000,0x20015000,0x20016000,0x20017000
    def put(p,v): uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p): return struct.unpack('<i',uc.mem_read(p,4))[0]
    def mid(p): return (p-managers)//0x5D4+1 if p else 0
    for address,cleanup in ((0x9D5F80,0),(0xA6A360,0),(0xBFEA1A,0),(0xBFE9BC,0),(0x9FA280,28),(0x9F9E00,8),(0x9F40A0,0),(0x9F2E20,0),(delete_cb,4)):
        uc.mem_write(address,b'\xc2'+struct.pack('<H',cleanup) if cleanup else b'\xc3')
    put(vtable,delete_cb)
    events=[]; nr=0; case=None
    def hook(uc,address,size,data):
        nonlocal nr
        esp=uc.reg_read(UC_X86_REG_ESP); receiver=uc.reg_read(UC_X86_REG_ECX)
        if address==0x9D5F80: events.append('BASE'); uc.mem_write(receiver,b'\x3C'*0x164)
        elif address==0xBFEA1A:
            size=word(esp+4); events.append('A'+str(size))
            if size==0x5D4: p=0 if case[2]==1 else managers
            elif size==12:
                p=0 if case[2]==2 else refs+nr*12
                if p: nr+=1
            else: raise RuntimeError('Unexpected runtime allocation')
            uc.reg_write(UC_X86_REG_EAX,p)
        elif address==0xA6A360:
            events.append('TM'+str(mid(receiver))); put(receiver,vtable); uc.reg_write(UC_X86_REG_EAX,0 if case[2]==3 else receiver)
        elif address==delete_cb: events.append(f'DELETE{mid(receiver)}:{word(esp+4)}')
        elif address==0xBFE9BC: events.append('FREE'+str((word(esp+4)-refs)//12))
        elif address==0x9FA280:
            i=(receiver-bank-0x2B4)//8; dimensions,levels,fmt,usage,managed,dynamic,extra=[word(esp+j*4) for j in range(1,8)]
            events.append(f'TEX{i}:{signed(dimensions)}:{signed(dimensions+4)}:{struct.unpack("<i",struct.pack("<I",levels))[0]}:{signed(fmt)}:{usage}:{managed&255}:{dynamic&255}:{extra}')
            put(receiver,0x81000000+i*4); put(receiver+4,0x10000000+i)
            if case[3]==3 and i+1<11: put(bank+0x288+(i+1)*4,-1)
            uc.reg_write(UC_X86_REG_EAX,int(case[3]!=1))
        elif address==0x9F9E00:
            i=(receiver-bank-0x2B4)//8; out,level=word(esp+4),word(esp+8); events.append(f'SURF{i}:{level}')
            for p,id_ in ((out,i+1),(alternate,i+101)):
                put(p,0x12345678); put(p+4,id_); put(p+8,0x1234); put(p+12,0x5678)
            uc.reg_write(UC_X86_REG_EAX,alternate if case[3]==2 else out)
        elif address==0x9F40A0: events.append('CLEAR'+str(word(receiver+4)))
        elif address==0x9F2E20: events.append(f'RELEASE{int(word(receiver)==0x122F84C)}:{word(receiver+4)}:{word(receiver+8)}:{word(receiver+12)}')
    uc.hook_add(UC_HOOK_CODE,hook)
    cases=list(itertools.product(range(128),(0xA5,),range(4),(0,3)))
    cases+=list(itertools.product((0,127),(0,255),range(4),range(4)))
    directory=ROOT/'work/ui_bank_runtime_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c)) for c in cases)+'\n')
    env=parity.env(); objects=[]
    for i,s in enumerate(SOURCES+['rebuild/tests/integration/UiBankRuntime_test.cpp']):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete bank runtime traces')
    symbols={0x129A7C4:0x70000100,0x129C814:0x70000101,0x129C938:0x70000102,0x129C808:0x70000103,0x12354A4:0x70000104,0x129C888:0x70000105,0x129C8CC:0x70000106,0x9FFD20:0x70000107}
    def normalize(value):
        if value in symbols: return symbols[value]
        for base,size,target in ((bank,0x30C,0x60000000),(managers,0xBA8,0x61000000),(refs,48,0x62000000)):
            if base<=value<base+size: return target+value-base
        return value
    def dump():
        events.append('BANK'+''.join(f':{normalize(word(bank+i)):08x}' for i in range(0,0x30C,4)))
        for i in range(nr): events.append(f'REF{i}:{signed(refs+i*12)}:{normalize(word(refs+i*12+4)):08x}:{normalize(word(refs+i*12+8)):08x}')
    def execute(address,receiver,args=(),result=None):
        put(stack,stop)
        for i,arg in enumerate(args): put(stack+4+i*4,arg)
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,receiver); uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4: raise RuntimeError('Bank runtime return/stack mismatch')
        if result is not None and uc.reg_read(UC_X86_REG_EAX)!=result: raise RuntimeError('Wrong bank constructor return')
    errors=[]
    for index,(case,actual) in enumerate(zip(cases,lines)):
        mask,pattern,allocation,texture=case; events.clear(); nr=0; uc.mem_write(bank,bytes([pattern])*0x30C); uc.mem_write(refs,b'\xA5'*48)
        execute(0x9FEA20,bank,result=bank); dump()
        uc.mem_write(init,b'\xA5'*44)
        for i in range(7): put(init+i*4,i*17 if mask&(1<<i) else -1)
        execute(0x9FD4E0,bank,(init,)); dump()
        put(managers+0x5D4,vtable); execute(0xA002E0,bank+0x280,(managers+0x5D4,)); dump()
        execute(0xA002E0,bank+0x280,(0,)); execute(0x9FFD20,0); dump()
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=case,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol); relocs=parity.obj_relocs(obj,section)
        offset=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address); matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[offset:offset+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()))
        (directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,comparisons=comparisons,scope='complete bank ctor/init, resource ctor, base, texture-manager reset/delete, empty policy; bank-file base/texture manager/texture-surface device operations controlled'),indent=2)+'\n')
    print(f"UI_BANK_RUNTIME {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}"); print(json.dumps(comparisons))
    return int(bool(errors))

if __name__=='__main__': raise SystemExit(main())
