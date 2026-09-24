#!/usr/bin/env python3
"""Connected CCharString lifetime/allocated-byte comparisons against retail."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_EDX, UC_X86_REG_ECX, UC_X86_REG_ESP, UC_X86_REG_EIP
from check_cgame_play import ROOT, parity, pe_oracle, run

STRING_FUNCTIONS = [
    ('00/99/CCharString_ConstructText_0099ebf0',0x99EBF0,61,'FableUiConstructBankName'),
    ('00/99/CCharString_Destroy_0099eae0',0x99EAE0,12,'FableUiDestroyBankName'),
    ('00/99/CCharString_Unassign_0099e9b0',0x99E9B0,87,'FableUiUnassignString'),
    ('00/99/CCharString_AllocateData_0099ea60',0x99EA60,122,'FableUiAllocateStringData'),
    ('00/9a/CBasicString_ConstructText_009a0590',0x9A0590,72,'FableUiConstructStringData'),
    ('00/9a/CBasicString_AssignBytes_009a0300',0x9A0300,135,'FableUiAssignStringBytes'),
    ('00/99/CCharString_ConstructEmpty_0099e4b0',0x99E4B0,15,'FableUiConstructEmptyString'),
    ('00/99/CCharString_Copy_0099ec30',0x99EC30,61,'FableUiCopyString'),
    ('00/99/CCharString_Assign_0099efb0',0x99EFB0,40,'FableUiAssignString'),
    ('00/99/CCharString_MakeUnique_0099eaf0',0x99EAF0,78,'FableUiMakeStringUnique'),
    ('00/9a/CBasicString_ReserveBytes_009a01c0',0x9A01C0,316,'FableUiReserveStringBytes'),
    ('00/9a/CBasicString_AppendBytes_009a04e0',0x9A04E0,107,'FableUiAppendStringBytes'),
    ('00/99/CCharString_AppendLiteral_0099f100',0x99F100,122,'FableUiAppendStringLiteral'),
    ('00/99/CCharString_ConcatLiteral_0099f600',0x99F600,130,'FableUiConcatStringLiteral'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in STRING_FUNCTIONS]

def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10': raise RuntimeError('Retail oracle changed')
    uc=Uc(UC_ARCH_X86,UC_MODE_32); uc.mem_map(0x400000,0x1100000); uc.mem_map(0x20000000,0x30000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image): uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,values,records,buffers,source,other=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20015000,0x20016000
    def put(p,v): uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p): return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p): return struct.unpack('<i',uc.mem_read(p,4))[0]
    def ident(p,base,stride): return (p-base)//stride+1 if p else 0
    for p in (0xBFEA1A,0xBFE9BC,0xBFEB22,0xBFEB1C): uc.mem_write(p,b'\xc3')
    events=[]; sizes=[]; record_count=attempt=fail_at=0; case=None
    def hook(uc,address,size,data):
        nonlocal record_count,attempt
        esp=uc.reg_read(UC_X86_REG_ESP)
        if address==0xBFEA1A:
            size=word(esp+4); events.append('A'+str(size)); attempt+=1
            if size!=17: raise RuntimeError('Wrong record allocation')
            if attempt==fail_at: events.append('FAIL'); uc.reg_write(UC_X86_REG_EAX,0)
            else: uc.reg_write(UC_X86_REG_EAX,records+record_count*32); record_count+=1
        elif address==0xBFEB22:
            size=word(esp+4); events.append('B'+str(size))
            if size>256 or len(sizes)>=32: raise RuntimeError('Wrong buffer allocation')
            uc.reg_write(UC_X86_REG_EAX,buffers+len(sizes)*256); sizes.append(size)
        elif address==0xBFE9BC: events.append('R'+str(ident(word(esp+4),records,32)))
        elif address==0xBFEB1C: events.append('F'+str(ident(word(esp+4),buffers,256)))
    uc.hook_add(UC_HOOK_CODE,hook)
    cases=list(itertools.product(range(6),(-1,0,1,3,7,31),(0,0xA5,0xFF),(0,1,2)))
    directory=ROOT/'work/ui_strings_check'; directory.mkdir(parents=True,exist_ok=True)
    inputs=directory/'cases.txt'; inputs.write_text('\n'.join(' '.join(map(str,c)) for c in cases)+'\n')
    env=parity.env(); objects=[]
    for i,s in enumerate(SOURCES+['rebuild/tests/integration/UiStrings_test.cpp']):
        obj=directory/f'part{i}.obj'; run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/s],env); objects.append(obj)
    exe=directory/'behavior.exe'; run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe,inputs],env).splitlines()
    if len(lines)!=len(cases): raise RuntimeError('Incomplete string traces')
    def execute(address,receiver,args=(),check_return=False,edx=0):
        put(stack,stop)
        for i,arg in enumerate(args): put(stack+4+i*4,arg)
        uc.reg_write(UC_X86_REG_EDX,edx); uc.reg_write(UC_X86_REG_ESP,stack); uc.reg_write(UC_X86_REG_ECX,receiver); uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4: raise RuntimeError('String return/stack mismatch')
        if check_return and uc.reg_read(UC_X86_REG_EAX)!=receiver: raise RuntimeError('Wrong string return value')
    def snapshot():
        events.append('S:'+str(signed(0x13BD800))+''.join(':'+str(ident(word(values+i*4),records,32)) for i in range(4)))
        for i in range(record_count):
            p=records+i*32
            events.append(f'H{i}:{ident(word(p),buffers,256)}:{word(p+4)}:{word(p+8):08x}:{uc.mem_read(p+12,1)[0]:02x}:{signed(p+13)}'+bytes(uc.mem_read(p+17,15)).hex())
        for i,size in enumerate(sizes): events.append(f'T{i}:'+bytes(uc.mem_read(buffers+i*256,size+4)).hex())
    errors=[]
    for index,(case,actual) in enumerate(zip(cases,lines)):
        text_case,length,pattern,fail_at=case; events.clear(); sizes.clear(); record_count=attempt=0
        text=bytearray(33+i%90 for i in range(160))
        if text_case in (0,1): text[0]=0
        if text_case in (2,3): name=b'GBANK_MAIN\0' if text_case==2 else b'GBANK_FRONT_END\0'; text[:len(name)]=name
        if text_case==4: text[:3]=b'ab\0'
        text[159]=0
        uc.mem_write(source,bytes(text)); uc.mem_write(other,b'different\0'); uc.mem_write(records,bytes([pattern])*1024); uc.mem_write(buffers,b'\xCD'*8192); uc.mem_write(values,b'\0'*16); put(0x13BD800,123)
        execute(0x99EBF0,values,(source if text_case else 0,length),True); snapshot()
        execute(0x99EC30,values+4,(values,),True); snapshot()
        if word(values):
            uc.mem_write(other+32,b'replacement\0'); execute(0x9A0300,word(values),(other+32,3))
        snapshot()
        execute(0x99E4B0,values+8,check_return=True)
        execute(0x99EBF0,values+12,(other,-1),True); snapshot()
        execute(0x99EFB0,values+8,(values+4,),True); snapshot()
        execute(0x99EFB0,values,(values+8,),True); snapshot()
        execute(0x99EFB0,values+4,(values+12,),True); snapshot()
        execute(0x99EFB0,values,(values,),True); execute(0x99EFB0,values+8,(values+8,),True); snapshot()
        execute(0x99EFB0,values+8,(values+12,),True); snapshot()
        execute(0x99EAF0,values+8); snapshot(); fail_at=0
        uc.mem_write(other+64,b'x\0'); execute(0x99F100,values+8,(other+64,),True); snapshot()
        uc.mem_write(other+64,b'\0'); execute(0x99F100,values+8,(other+64,),True); snapshot()
        uc.mem_write(other+64,b'abcdefghijklmnop\0'); execute(0x99F100,values,(other+64,),True); snapshot()
        execute(0x99EAE0,values+4)
        uc.mem_write(other+64,b' bank not found!\0'); execute(0x99F600,values+4,(other+64,),True,values+8); snapshot()
        p=word(values+4)
        if p: uc.mem_write(p+12,bytes([uc.mem_read(p+12,1)[0]^1]))
        uc.mem_write(other+64,b'0123456789abcdefghijklmnopqrstuvwxyz\0'); execute(0x99F100,values+4,(other+64,),True); snapshot()
        for i in (3,1,2,0): execute(0x99EAE0,values+i*4); snapshot()
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected: errors.append(dict(case=index,input=case,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,STRING_FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol); relocs=parity.obj_relocs(obj,section)
        offset=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address)
        matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[offset:offset+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()))
        (directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(cases),errors=errors,comparisons=comparisons,scope='14 connected string methods, append/reserve/concatenation with exact and power-of-two growth; allocation/free observed, record allocation failures; full record fields/buffer padding/shared ownership, ABI returns and cleanup'),indent=2)+'\n')
    print(f"UI_STRINGS {'FAIL' if errors else 'PASS'} cases={len(cases)} failures={len(errors)}"); print(json.dumps(comparisons))
    return int(bool(errors))

if __name__=='__main__': raise SystemExit(main())
