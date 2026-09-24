#!/usr/bin/env python3
"""Archive filename-list ownership vs retail."""
import hashlib,json,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from check_cgame_play import ROOT,parity,pe_oracle,run
FUNCTIONS=[
 ('00/9d/CSmallVector_ClearBankAliases_009d2110',0x9D2110,69,'FableUiClearBankAliases'),
 ('00/9d/CSmallVector_ResizeBankAliases_009d2160',0x9D2160,99,'FableUiResizeBankAliases'),
 ('00/9d/CSmallVector_CopyBankAliases_009d21d0',0x9D21D0,68,'FableUiCopyBankAliases'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]
DEPENDENCIES=['rebuild/src/compiled/00/99/'+n+'.cpp' for n in ('CCharString_ConstructEmpty_0099e4b0','CCharString_Assign_0099efb0','CCharString_Unassign_0099e9b0','CCharString_Destroy_0099eae0')]
def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':raise RuntimeError('Retail oracle changed')
    directory=ROOT/'work/ui_bank_aliases_check';directory.mkdir(parents=True,exist_ok=True);env=parity.env();objects=[]
    for i,source in enumerate(SOURCES+DEPENDENCIES+['rebuild/tests/integration/UiBankAliases_test.cpp']):
        obj=directory/f'part{i}.obj';run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env);objects.append(obj)
    exe=directory/'behavior.exe';run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe],env).splitlines()
    if len(lines)!=3072:raise RuntimeError('Incomplete storage traces')
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1100000);uc.mem_map(0x20000000,0x20000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image):uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,target,source,old,src,new,records=0x20000000,0x20008000,0x20010000,0x20010100,0x20011000,0x20012000,0x20013000,0x20014000
    def put(p,v):uc.mem_write(p,struct.pack('<I',v&0xffffffff))
    def word(p):return struct.unpack('<I',uc.mem_read(p,4))[0]
    def normalize(v):
        for i,(start,size) in enumerate(((old,1028),(src,1028),(new,1028),(records,136))):
            if start<=v<start+size:return 0x61000000+i*0x1000000+v-start
        return v
    def dump(p,n):return ''.join(f':{normalize(word(p+i)):08x}' for i in range(0,n,4))
    services=(0xBFEB22,0xBFEB1C,0xBFE9BC)
    for a in services:uc.mem_write(a,b'\xc3')
    events=[]
    def hook(uc,address,size,data):
        if address not in services:return
        esp=uc.reg_read(UC_X86_REG_ESP);arg=word(esp+4)
        if address==0xBFEB22:
            events.append(f'ALLOC{arg}:{word(0x13BD800)}');uc.reg_write(UC_X86_REG_EAX,0 if mode==1 and seed&256 else new)
        elif address==0xBFEB1C:events.append(f'FREE{normalize(arg):08x}:{word(0x13BD800)}')
        else:events.append('RF'+str((arg-records)//17))
    def setup(p,data,count,salt):
        put(p,data+4);uc.mem_write(p+4,bytes([count]));put(data,count)
        for i in range(count):
            rec=0 if (seed+i+salt)%4==0 else records+(seed+i+salt)%8*17
            put(data+4+i*4,rec)
            if rec:put(rec+13,word(rec+13)+1)
    uc.hook_add(UC_HOOK_CODE,hook);errors=[]
    for index,actual in enumerate(lines):
        mode,seed=divmod(index,1024);events.clear()
        uc.mem_write(old,b'\xa5'*1028);uc.mem_write(src,b'\xa6'*1028);uc.mem_write(new,b'\xcd'*1028)
        uc.mem_write(records,b'\0'*136);uc.mem_write(target,b'\xb1'*8);uc.mem_write(source,b'\xb2'*8);put(0x13BD800,1000)
        setup(target,old,seed%16,0);setup(source,src,(seed//16)%16,3)
        if mode==0:uc.mem_write(target+4,bytes([seed//4]))
        if seed&512:put(target,0);uc.mem_write(target+4,b'\0')
        args=[] if mode==0 else [seed%256] if mode==1 else [target if seed&256 else source]
        put(stack,stop)
        for i,v in enumerate(args):put(stack+4+i*4,v)
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,target);uc.emu_start(FUNCTIONS[mode][1],stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4:raise RuntimeError('Aliases stack mismatch')
        events.extend(('TARGET'+dump(target,8),'SOURCE'+dump(source,8),'OLD'+dump(old,1028),'SRC'+dump(src,1028),'NEW'+dump(new,1028),'RECORDS'+bytes(uc.mem_read(records,136)).hex(),'COUNT'+str(word(0x13BD800))))
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected:errors.append(dict(case=index,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol);relocs=parity.obj_relocs(obj,section);off=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address);matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[off:off+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()));(directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(lines),errors=errors,comparisons=comparisons,scope='Actual clear/resize/copy and string lifetime, controlled allocators. Clear cookie vs visible count, reverse release, zero and 255-element resize, failed resize allocation, shared/null records, self-copy, untouched padding. Copy restricted to nonnegative counts and successful allocation; corrupt cookies and negative copy counts excluded.'),indent=2)+'\n')
    print(f"UI_BANK_ALIASES {'FAIL' if errors else 'PASS'} cases={len(lines)} failures={len(errors)}");print(json.dumps(comparisons));return int(bool(errors))
if __name__=='__main__':raise SystemExit(main())
