#!/usr/bin/env python3
"""Runtime entry-vector resize, including aliased fill values, vs retail."""
import hashlib,json,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from check_cgame_play import ROOT,parity,pe_oracle,run
SOURCES=['rebuild/src/compiled/00/9d/RuntimeVector_ResizeBankEntries_009d4010.cpp']
def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':raise RuntimeError('Retail oracle changed')
    directory=ROOT/'work/ui_bank_runtime_vector_check';directory.mkdir(parents=True,exist_ok=True);env=parity.env();objects=[]
    for i,source in enumerate(SOURCES+['rebuild/tests/integration/UiBankRuntimeVector_test.cpp']):
        obj=directory/f'part{i}.obj';run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env);objects.append(obj)
    exe=directory/'behavior.exe';run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env);lines=run([exe],env).splitlines()
    if len(lines)!=1024:raise RuntimeError('Incomplete vector traces')
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1100000);uc.mem_map(0x20000000,0x20000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image):uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,array,old,new,fill=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20013000
    def put(p,v):uc.mem_write(p,struct.pack('<I',v&0xffffffff))
    def word(p):return struct.unpack('<I',uc.mem_read(p,4))[0]
    uc.mem_write(0xBFEA0E,b'\xc3');uc.mem_write(0xBFEA14,b'\xc3');events=[];seed=0
    def hook(uc,address,size,data):
        esp=uc.reg_read(UC_X86_REG_ESP)
        if address==0xBFEA0E:events.append(f'ALLOC{word(esp+4)}:{(word(array+4)-old)//12}');uc.reg_write(UC_X86_REG_EAX,new)
        elif address==0xBFEA14:
            events.append('FREE'+str(int(word(esp+4)==old)))
            if seed&256:uc.mem_write(array,b'\0'*12)
    uc.hook_add(UC_HOOK_CODE,hook)
    def norm(v):
        if old<=v<old+288:return 0x61000000+v-old
        if new<=v<new+384:return 0x62000000+v-new
        return v
    errors=[]
    for seed,actual in enumerate(lines):
        events.clear();length=seed%8;spare=(seed//8)%4;count=(seed//32)%16
        uc.mem_write(new,b'\xcd'*384);uc.mem_write(old,bytes((i*7+seed)&255 for i in range(288)));uc.mem_write(fill,b'\xa5'*12)
        put(array,old);put(array+4,old+length*12);put(array+8,old+(length+spare)*12)
        value=old+(length-1)*12 if seed&512 and length else fill
        put(stack,stop);put(stack+4,count);put(stack+8,value);uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,array);uc.emu_start(0x9D4010,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+12:raise RuntimeError('Resize stack mismatch')
        events.extend(('ARRAY'+''.join(f':{norm(word(array+i*4)):08x}' for i in range(3)),'OLD'+bytes(uc.mem_read(old,288)).hex(),'NEW'+bytes(uc.mem_read(new,384)).hex()))
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected:errors.append(dict(case=seed,actual=actual,retail=expected))
    code,section,_=parity.obj_text(objects[0],'?FableUiResizeBankRuntime');relocs=parity.obj_relocs(objects[0],section)
    comparison=dict(address='009d4010',compiled_bytes=len(code),retail_bytes=87,grade='DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest())
    (directory/'009d4010-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',objects[0]],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(lines),errors=errors,comparisons=[comparison],scope='Complete runtime-vector resize with append/reallocation behavior inlined; valid sizes, shrink/no-op/grow, retained capacity, aliased fill source, full old/new buffers, free callback changing vector header. Allocation failure and invalid vectors excluded.'),indent=2)+'\n')
    print(f"UI_BANK_RUNTIME_VECTOR {'FAIL' if errors else 'PASS'} cases={len(lines)} failures={len(errors)}");print(json.dumps(comparison));return int(bool(errors))
if __name__=='__main__':raise SystemExit(main())
