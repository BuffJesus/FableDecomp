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
 ('CWideString_Unassign_0099b4d0',0x99B4D0,62,'FableUiUnassignWideString'),
 ('CWideString_Destroy_0099b510',0x99B510,68,'FableUiDestroyWideString'),
 ('CWideString_AllocateData_0099b3c0',0x99B3C0,121,'FableUiAllocateWideData'),
 ('CWideString_MakeUnique_0099b560',0x99B560,128,'FableUiMakeWideStringUnique'),
 ('CWideString_ConstructText_0099b6b0',0x99B6B0,104,'FableUiConstructWideText'),
 ('CWideString_Copy_0099b720',0x99B720,66,'FableUiCopyWideString'),
 ('CWideString_Assign_0099b7d0',0x99B7D0,47,'FableUiAssignWideString'),
 ('CWideString_Append_0099b8d0',0x99B8D0,104,'FableUiAppendWideString'),
 ('CWideString_Concat_0099be70',0x99BE70,177,'FableUiConcatWideStrings'),
 ('CWideStringData_ConstructRange_0099c670',0x99C670,97,'FableUiConstructWideRange'),
 ('CWideStringData_AppendRange_0099c7e0',0x99C7E0,305,'FableUiAppendWideRange'),
]
SOURCES=['rebuild/src/compiled/00/99/'+f[0]+'.cpp' for f in FUNCTIONS]

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
    for p in (0xBFEA1A,0xBFE9BC,0xBFEA0E,0xBFEA14,0xBFEAE6): uc.mem_write(p,b'\xc3')
    events=[]; sizes=[]; record_count=attempt=fail_at=0
    def hook(uc,address,size,data):
        nonlocal record_count,attempt
        esp=uc.reg_read(UC_X86_REG_ESP)
        if address==0xBFEA1A:
            n=word(esp+4); events.append('A'+str(n)); attempt+=1
            if n!=16 or record_count==32: raise RuntimeError('Wrong record allocation')
            if attempt==fail_at: events.append('FAIL'); uc.reg_write(UC_X86_REG_EAX,0)
            else: uc.reg_write(UC_X86_REG_EAX,records+record_count*32); record_count+=1
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
    uc.hook_add(UC_HOOK_CODE,hook)
    cases=list(itertools.product(range(8),range(6),(0,0xa5,0xff),(0,1,2,3,4)))
    directory=ROOT/'work/ui_wide_strings_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c)) for c in cases)+'\n')
    env=parity.env(); objects=[]
    for i,s in enumerate(SOURCES+['rebuild/tests/integration/UiWideStrings_test.cpp']):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete wide traces')
    def execute(address,receiver,args=(),return_self=False,edx=0):
        put(stack,stop)
        for i,arg in enumerate(args): put(stack+4+i*4,arg)
        uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,receiver); uc.reg_write(UC_X86_REG_EDX,edx); uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4: raise RuntimeError('Wide return/stack mismatch')
        if return_self and uc.reg_read(UC_X86_REG_EAX)!=receiver: raise RuntimeError('Wrong wide return value')
    def snapshot():
        events.append('S:'+str(signed(0x13BCA20))+''.join(':'+str(ident(word(values+i*4),records,32)) for i in range(5)))
        for i in range(record_count):
            p=records+i*32
            events.append(f'H{i}:{ident(word(p),buffers,1)}:{ident(word(p+4),buffers,1)}:{ident(word(p+8),buffers,1)}:{signed(p+12)}'+bytes(uc.mem_read(p+16,16)).hex())
        for i,n in enumerate(sizes): events.append(f'T{i}:'+bytes(uc.mem_read(buffers+i*1024,n+4)).hex())
    texts=['','','a','data\\','textures.big','\u0080\uffff\ud800','ab\0suffix','longer asset directory/']
    for i,t in enumerate(texts): uc.mem_write(textbase+i*256,(t+'\0').encode('utf-16le',errors='surrogatepass'))
    src=[0x80+i for i in range(64)]; src[2]=0; uc.mem_write(source,struct.pack('<64H',*src))
    errors=[]
    for index,(case,actual) in enumerate(zip(cases,lines)):
        a,b,pattern,fail_at=case; events.clear(); sizes.clear(); record_count=attempt=0
        uc.mem_write(records,bytes([pattern])*1024); uc.mem_write(buffers,b'\xcd'*32768); uc.mem_write(values,b'\0'*20); put(0x13BCA20,123)
        execute(0x99B6B0,values,(textbase+a*256 if a else 0,),True); snapshot()
        execute(0x99B720,values+4,(values,),True); snapshot()
        execute(0x99B6B0,values+8,(textbase+b*256 if b else 0,),True); snapshot()
        put(0x13BCA20,word(0x13BCA20)+1)
        execute(0x99B3C0,values+12,(source,(0,1,3,7,15,31)[b])); put(values+12,uc.reg_read(UC_X86_REG_EAX)); snapshot()
        execute(0x99B560,values+4); snapshot(); fail_at=0
        execute(0x99B8D0,values,(values+8,),True); snapshot()
        execute(0x99B8D0,values,(values,),True); snapshot()
        for _ in range(2): execute(0x99B8D0,values+12,(values+8,),True); snapshot()
        execute(0x99BE70,values+16,(values+12,),True,values+4); snapshot()
        execute(0x99B7D0,values+8,(values+16,),True); snapshot()
        execute(0x99B7D0,values+4,(values+8,),True); snapshot()
        execute(0x99B7D0,values,(values,),True); execute(0x99B7D0,values+4,(values+8,),True); snapshot()
        for k in range(4,-1,-1): execute(0x99B510,values+k*4); snapshot()
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=case,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol); relocs=parity.obj_relocs(obj,section)
        offset=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address)
        matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[offset:offset+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()))
        (directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,comparisons=comparisons,scope='11 connected wide-string bodies; full buffer/record/guard snapshots, copy-on-write, self append, capacity growth and spare-capacity reuse, explicit embedded-NUL ranges, allocator order/failures, stack cleanup. CRT memcpy and allocation are controlled. Length-error service remains external.'),indent=2)+'\n')
    print(f"UI_WIDE_STRINGS {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}"); print(json.dumps(comparisons))
    return int(bool(errors))

if __name__=='__main__': raise SystemExit(main())
