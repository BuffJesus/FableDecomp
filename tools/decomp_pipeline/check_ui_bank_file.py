#!/usr/bin/env python3
"""Retail bank/async/cache/packed construction, allocation-time memory and guards."""
import hashlib
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run
FUNCTIONS=[
 ('00/9c/CBankFile_Construct_009cd480',0x9CD480,402,'FableUiConstructBankFile'),
 ('00/9d/CBankFileAsync_Construct_009d5f80',0x9D5F80,224,'FableUiConstructBankFileBase'),
 ('00/a6/CChecksumCacheFile_Construct_00a60c90',0xA60C90,73,'FableUiConstructChecksumCache'),
 ('00/a6/CPackedUIntArray_Construct_00a629c0',0xA629C0,16,'FableUiConstructPackedUIntArray'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]+['rebuild/src/compiled/00/99/CWideString_Constructor_0099aed0.cpp']
DEPENDENCIES=['rebuild/src/compiled/00/99/CBase_ConstructUiResourceBase_0099a2f0.cpp','rebuild/src/compiled/00/99/CCharString_ConstructEmpty_0099e4b0.cpp']
def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    directory=ROOT/'work/ui_bank_file_check'; directory.mkdir(parents=True,exist_ok=True)
    env=parity.env(); objects=[]
    for i,s in enumerate(SOURCES+DEPENDENCIES+['rebuild/tests/integration/UiBankFile_test.cpp']):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe],env).splitlines()
    if len(lines)!=1024: raise RuntimeError('Incomplete bank-file traces')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x20000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,storage,heap,critical=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000
    def put(p,v): uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    uc.mem_write(0xBFEA0E,b'\xc3'); uc.mem_write(critical,b'\xc2\x04\x00'); put(0x143FEE4,critical)
    sizes=[]; events=[]; seed=0
    def normalize(value):
        for address,target in ((0x129A7C4,0x70000100),(0x129B54C,0x70000110),(0x129B8D4,0x70000111)):
            if value==address: return target
        if heap<=value<heap+512: return 0x61000000+value-heap
        if storage<=value<storage+0x180: return 0x60000000+value-storage
        return value
    def dump(p,n): return ''.join(f':{normalize(word(p+i)):08x}' for i in range(0,n,4))
    def hook(uc,address,size,data):
        esp=uc.reg_read(UC_X86_REG_ESP)
        if address==0xBFEA0E:
            n=word(esp+4); i=len(sizes); events.append(f'A{i}:{n}'+dump(storage,0x180)); sizes.append(n)
            if n>64 or i>=8: raise RuntimeError('Unexpected bank-file allocation')
            uc.reg_write(UC_X86_REG_EAX,heap+i*64)
        elif address==critical:
            p=word(esp+4); events.append(f'CS{p-storage}'+dump(storage,0x180)); uc.mem_write(p,bytes((seed+i*3)&255 for i in range(24)))
    uc.hook_add(UC_HOOK_CODE,hook)
    errors=[]
    for index,actual in enumerate(lines):
        mode,seed=divmod(index,256); events.clear(); sizes.clear()
        uc.mem_write(storage,bytes((seed+i*17)&255 for i in range(0x180))); uc.mem_write(heap,bytes((seed+i*11)&255 for i in range(512)))
        put(0x13BCA20,seed*3); put(0x13BD800,seed*5); put(stack,stop); put(stack+4,seed*0x01010101)
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,storage+8)
        uc.emu_start(FUNCTIONS[mode][1],stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+(8 if mode==2 else 4) or uc.reg_read(UC_X86_REG_EAX)!=storage+8: raise RuntimeError('Bank-file return/stack mismatch')
        events.append('DATA'+dump(storage,0x180))
        for i,n in enumerate(sizes): events.append(f'H{i}:{n}'+dump(heap+i*64,64))
        events.append(f'COUNTS{word(0x13BCA20)}:{word(0x13BD800)}')
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol); relocs=parity.obj_relocs(obj,section)
        offset=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address); matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[offset:offset+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()))
        (directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=1024,errors=errors,comparisons=comparisons,scope='bank/async/cache/packed constructors, real narrow/wide empty strings, allocation-time snapshots, object/heap guards; allocators and Win32 critical section controlled'),indent=2)+'\n')
    print(f"UI_BANK_FILE {'FAIL' if errors else 'PASS'} cases=1024 failures={len(errors)}"); print(json.dumps(comparisons))
    return int(bool(errors))
if __name__=='__main__': raise SystemExit(main())
