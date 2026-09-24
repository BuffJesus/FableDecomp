#!/usr/bin/env python3
"""Connected UTF-16 string storage, copy-on-write and concatenation vs retail."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run

FUNCTIONS=[
 ('00/9a/CBankRegistry_FindPath_009a7ca0',0x9A7CA0,144,'FableUiFindBankPath'),
 ('00/99/CCharString_GetText_0099e4c0',0x99E4C0,15,'FableUiGetStringText'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]
from check_ui_wide_strings import SOURCES as WIDE
from check_ui_bank_registry import SOURCES as REGISTRY
from check_ui_strings import SOURCES as STRINGS
DEPENDENCIES=WIDE+[REGISTRY[i] for i in (0,2,3)]+STRINGS

def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x40000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,values,records,buffers,source,textbase=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20021000,0x20022000
    def put(p,v): uc.mem_write(p,struct.pack('<I',v&0xffffffff))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p): return struct.unpack('<i',uc.mem_read(p,4))[0]
    def ident(p,base,stride): return (p-base)//stride+1 if p else 0
    for p in (0xBFEA1A,0xBFE9BC,0xBFEA0E,0xBFEA14,0xBFEAE6,0xBFEB22,0xBFEB1C): uc.mem_write(p,b'\xc3')
    registry,heads,nodes,keys,names,query=0x20024000,0x20025000,0x20026000,0x20027000,0x20028000,0x20029000
    uc.mem_write(0xBFEB84,b'\xc3')
    events=[]; sizes=[]; record_sizes=[]; record_count=attempt=fail_at=0
    thrown=False
    def hook(uc,address,size,data):
        nonlocal record_count,attempt,thrown
        esp=uc.reg_read(UC_X86_REG_ESP)
        if address==0xBFEA1A:
            n=word(esp+4); events.append('A'+str(n)); attempt+=1
            if n not in (16,17) or record_count==32: raise RuntimeError('Wrong record allocation')
            if attempt==fail_at: events.append('FAIL'); uc.reg_write(UC_X86_REG_EAX,0)
            else: uc.reg_write(UC_X86_REG_EAX,records+record_count*32); record_count+=1; record_sizes.append(n)
        elif address==0xBFEA0E:
            n=word(esp+4); events.append('B'+str(n))
            if n>1000 or len(sizes)==32: raise RuntimeError('Wrong wide buffer allocation')
            uc.reg_write(UC_X86_REG_EAX,buffers+len(sizes)*1024); sizes.append(n)
        elif address==0xBFE9BC: events.append('R'+str(ident(word(esp+4),records,32)))
        elif address==0xBFEA14: events.append('F'+str(ident(word(esp+4),buffers,1024)))
        elif address==0xBFEAE6:
            dest,src,n=(word(esp+i) for i in (4,8,12))
            uc.mem_write(dest,bytes(uc.mem_read(src,n))); uc.reg_write(UC_X86_REG_EAX,dest)
        elif address==0x99C550: raise RuntimeError('Unexpected length error')
        elif address==0xBFEB22:
            n=word(esp+4); events.append('NB'+str(n))
            if n>1000 or len(sizes)==32: raise RuntimeError('Wrong narrow buffer allocation')
            uc.reg_write(UC_X86_REG_EAX,buffers+len(sizes)*1024); sizes.append(n)
        elif address==0xBFEB1C: events.append('NF'+str(ident(word(esp+4),buffers,1024)))
        elif address==0xBFEB84:
            if word(esp+8)!=0x13692F8: raise RuntimeError('Wrong throw type')
            events.append('THROW_GENERIC'); thrown=True; uc.emu_stop()
    uc.hook_add(UC_HOOK_CODE,hook)
    cases=list(itertools.product(range(64),range(6)))
    directory=ROOT/'work/ui_bank_path_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c)) for c in cases)+'\n')
    env=parity.env(); objects=[]
    for i,s in enumerate(SOURCES+DEPENDENCIES+['rebuild/tests/integration/UiBankPath_test.cpp']):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete wide traces')
    def execute(address,receiver,args=(),return_self=False,edx=0):
        put(stack,stop)
        for i,arg in enumerate(args): put(stack+4+i*4,arg)
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,receiver); uc.reg_write(UC_X86_REG_EDX,edx); uc.emu_start(address,stop,count=100000)
        if thrown: return
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4: raise RuntimeError('Wide return/stack mismatch')
        if return_self and uc.reg_read(UC_X86_REG_EAX)!=receiver: raise RuntimeError('Wrong wide return value')
    def snapshot():
        events.append('S:'+str(signed(0x13BCA20))+''.join(':'+str(ident(word(values+i*4),records,32)) for i in range(5)))
        for i in range(record_count):
            p=records+i*32
            if record_sizes[i]==17:
                events.append(f'C{i}:{ident(word(p),buffers,1)}:{word(p+4)}:{word(p+8):08x}:{uc.mem_read(p+12,1)[0]:02x}:{signed(p+13)}'+bytes(uc.mem_read(p+17,15)).hex())
                continue
            events.append(f'H{i}:{ident(word(p),buffers,1)}:{ident(word(p+4),buffers,1)}:{ident(word(p+8),buffers,1)}:{signed(p+12)}'+bytes(uc.mem_read(p+16,16)).hex())
        for i,n in enumerate(sizes): events.append(f'T{i}:'+bytes(uc.mem_read(buffers+i*1024,n+4)).hex())
    paths=['','','data\\','/','textures.big','\u0080\uffff','ab\0tail','a longer directory/']
    for i,t in enumerate(paths): uc.mem_write(textbase+i*256,(t+'\0').encode('utf-16le'))
    for i,t in enumerate((b'',b'',b'a',b'b',b'missing',b'\x80')): uc.mem_write(names+i*32,t+b'\0')
    errors=[]
    for index,((seed,q),actual) in enumerate(zip(cases,lines)):
        events.clear(); sizes.clear(); record_sizes.clear(); record_count=attempt=0; thrown=False
        uc.mem_write(records,b'\xa5'*1024); uc.mem_write(buffers,b'\xcd'*32768); uc.mem_write(values,b'\0'*20); put(0x13BCA20,123); put(0x13BD800,200)
        for i in range(1,6): put(keys+i*17,names+i*32); put(keys+i*17+13,10)
        a=seed%8; b=(seed//8)%8
        execute(0x99B6B0,values,(textbase+a*256 if a else 0,),True)
        execute(0x99B6B0,values+4,(textbase+b*256 if b else 0,),True)
        uc.mem_write(registry,b'\0'*40); uc.mem_write(heads,b'\0'*32); uc.mem_write(nodes,b'\0'*48)
        put(registry+4,heads); put(registry+24,heads+16); put(registry+36,word(values))
        put(heads+4,nodes if seed&16 else 0); key=(seed//4)%4; put(nodes+16,keys+key*17 if key else 0); put(nodes+20,word(values+4))
        put(heads+20,nodes+24 if seed&32 else 0); put(nodes+40,keys+2*17); target=seed%4; put(nodes+44,keys+target*17 if target else 0); put(query,keys+q*17 if q else 0)
        execute(0x99E4C0,query)
        if uc.reg_read(UC_X86_REG_EAX)!=(names+q*32 if q else 0x129AAF4): raise RuntimeError('Wrong narrow character pointer')
        execute(0x9A7CA0,registry,(values+16,query))
        found=not thrown
        if found:
            if uc.reg_read(UC_X86_REG_EAX)!=values+16: raise RuntimeError('Wrong path return')
            events.append('FOUND')
        thrown=False
        snapshot(); events.append('N:'+str(word(0x13BD800))+''.join(':'+str(signed(keys+i*17+13)) for i in range(1,6)))
        if found: execute(0x99B510,values+16)
        execute(0x99B510,values+4); execute(0x99B510,values); snapshot()
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=[seed,q],actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol); relocs=parity.obj_relocs(obj,section)
        offset=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address)
        matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[offset:offset+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()))
        (directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,comparisons=comparisons,scope='Real path alias/map resolution, narrow lifetime and connected wide concatenation. Missing-name narrow concatenation, copy-on-write, growth and diagnostic bytes are real; exception runtime is controlled, throw metadata and no-cleanup lifetime observed. Empty exception padding is unspecified and not compared.'),indent=2)+'\n')
    print(f"UI_BANK_PATH {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}"); print(json.dumps(comparisons))
    return int(bool(errors))

if __name__=='__main__': raise SystemExit(main())
