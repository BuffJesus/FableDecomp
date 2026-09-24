#!/usr/bin/env python3
"""Bank storage setup and real string/runtime-vector cleanup vs retail."""
import hashlib,json,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from check_cgame_play import ROOT,parity,pe_oracle,run
FUNCTIONS=[
 ('00/41/CCharString_DestroyRange_00414e00',0x414E00,33,'FableUiDestroyStringRange'),
 ('00/43/StringVector_EraseRange_0043336a',0x43336A,73,'FableUiEraseStringRange'),
 ('00/9d/StringVector_ClearBankSymbols_009d3fb0',0x9D3FB0,95,'FableUiClearBankSymbols'),
 ('00/9d/RuntimeVector_ClearBankEntries_009d3d90',0x9D3D90,90,'FableUiClearBankRuntime'),
 ('00/9c/CBankFile_PrepareEntryStorage_009ceae0',0x9CEAE0,375,'FableUiPrepareBankStorage'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]
from check_ui_bank_runtime_vector import SOURCES as RUNTIME
from check_ui_bank_storage_helpers import SOURCES as HELPERS
DEPENDENCIES=RUNTIME+HELPERS+['rebuild/src/compiled/00/99/CCharString_ConstructEmpty_0099e4b0.cpp','rebuild/src/compiled/00/99/CCharString_Copy_0099ec30.cpp']+['rebuild/src/compiled/00/99/'+n+'.cpp' for n in ('CCharString_Assign_0099efb0','CCharString_Unassign_0099e9b0','CCharString_Destroy_0099eae0')]
def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':raise RuntimeError('Retail oracle changed')
    directory=ROOT/'work/ui_bank_storage_check';directory.mkdir(parents=True,exist_ok=True);env=parity.env();objects=[]
    for i,source in enumerate(SOURCES+DEPENDENCIES+['rebuild/tests/integration/UiBankStorage_test.cpp']):
        obj=directory/f'part{i}.obj';run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env);objects.append(obj)
    exe=directory/'behavior.exe';run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe],env).splitlines()
    if len(lines)!=1024:raise RuntimeError('Incomplete storage traces')
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1100000);uc.mem_map(0x20000000,0x20000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image):uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,bank,old,new,records,heads=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20013000,0x20014000
    def put(p,v):uc.mem_write(p,struct.pack('<I',v&0xffffffff))
    def word(p):return struct.unpack('<I',uc.mem_read(p,4))[0]
    nodes=0x20015000
    services={0xBFEA14:0,0xBFE9BC:0,0xBFEA0E:0}
    for p,n in services.items():uc.mem_write(p,b'\xc2'+struct.pack('<H',n))
    seed=allocated=0;events=[]
    def resize(a,count,index,width):
        put(a,new+index*128);put(a+4,new+index*128+count*width);put(a+8,word(a+4))
    def hook(uc,address,size,data):
        nonlocal allocated
        if address not in services:return
        esp=uc.reg_read(UC_X86_REG_ESP);a=uc.reg_read(UC_X86_REG_ECX);arg=word(esp+4)
        if address==0xBFEA14:
            if nodes<=arg<nodes+144:
                id_=(arg-nodes)//24;events.append('NF'+str(id_))
                if seed&128:put(bank+(0xac if id_<3 else 0xc4),heads+(2 if id_<3 else 3)*16)
                return
            id_=(arg-old)//128;events.append('FREE'+str(id_))
            if id_==2 and seed&64:put(bank+0x78,word(bank+0x78)^0x38)
        elif address==0xBFE9BC:events.append('RF'+str((arg-records)//17))
        elif address==0xBFEA0E:
            events.append('ALLOC'+str(arg));uc.reg_write(UC_X86_REG_EAX,new+allocated*128)
            if not allocated and seed&256:put(bank+0x78,word(bank+0x78)^0x18)
            allocated+=1
    uc.hook_add(UC_HOOK_CODE,hook)
    def normalize(v):
        for i,(base,size) in enumerate(((old,512),(new,512),(records,136),(heads,64),(nodes,144))):
            if base<=v<base+size:return 0x61000000+i*0x1000000+v-base
        return v
    def dump(p,n):return ''.join(f':{normalize(word(p+i)):08x}' for i in range(0,n,4))
    errors=[]
    for index,actual in enumerate(lines):
        mode,seed=divmod(index,512);events.clear();allocated=0;uc.mem_write(nodes,b'\0'*144);uc.mem_write(bank,b'\xa5'*272);uc.mem_write(old,b'\xda'*512);uc.mem_write(new,b'\x5b'*512);uc.mem_write(heads,b'\xa5'*64);uc.mem_write(records,b'\0'*136);put(0x13BD800,100);put(bank+0x78,(seed%8)<<3)
        for i,offset in enumerate((8,20,32,44)):
            p=bank+offset;put(p,old+i*128);put(p+4,old+i*128);put(p+8,old+i*128+((48 if i==2 else 16) if seed&(8<<i) else 0))
        count=seed%7 if mode else seed%4 if seed&8 else 0;put(bank+12,old+count*4)
        for i in range(count):
            rec=records+((i+seed)%5)*17;put(old+i*4,rec);put(rec+13,word(rec+13)+1)
        for i in range(8):put(records+i*17+8,0x80000000);uc.mem_write(records+i*17+12,b'\xa5')
        put(bank+0xac,heads);put(bank+0xb0,3 if seed&4 else 0);put(bank+0xc4,heads+16);put(bank+0xc8,2 if seed&2 else 0);put(heads+4,heads+16);put(heads+20,heads)
        if not mode:
            for t in range(2):
                n=(2 if seed&2 else 0) if t else (3 if seed&4 else 0)
                if n:put(heads+t*16+4,nodes+t*72)
                for j in range(n):
                    idx=t*3+j;put(nodes+idx*24+12,nodes+(idx+1)*24 if j+1<n else 0);rec=records+(seed+idx)%8*17;put(nodes+idx*24+16,rec);put(rec+13,word(rec+13)+1)
        if mode:
            first=(seed//7)%(count+1);last=first+(seed//49)%(count-first+1);address=0x43336A;receiver=bank+8;args=[old+first*4,old+last*4]
        else:address=0x9CEAE0;receiver=bank;args=[(seed//8)%8]
        put(stack,stop)
        for i,v in enumerate(args):put(stack+4+i*4,v)
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,receiver);uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4:raise RuntimeError('Storage stack mismatch')
        if mode and uc.reg_read(UC_X86_REG_EAX)!=args[0]:raise RuntimeError('Erase return mismatch')
        events.extend(('BANK'+dump(bank,272),'OLD'+dump(old,512),'HEADS'+dump(heads,64),'NEW'+bytes(0 if i<128 and i%12>=10 else uc.mem_read(new+i,1)[0] for i in range(512)).hex(),'NODES'+dump(nodes,144),'RECORDS'+bytes(uc.mem_read(records,136)).hex(),'COUNT'+str(word(0x13BD800))))
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected:errors.append(dict(case=index,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol);relocs=parity.obj_relocs(obj,section);off=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address);matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[off:off+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()));(directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(lines),errors=errors,comparisons=comparisons,scope='Real bank setup, runtime/symbol clears, string erase and range destruction; all runtime/word/string resizes and tree deletion real; allocator/free boundaries controlled. Full bank/array/record/head memory and callback order; flags and tree-head mutation. Runtime-entry padding excluded.'),indent=2)+'\n')
    print(f"UI_BANK_STORAGE {'FAIL' if errors else 'PASS'} cases={len(lines)} failures={len(errors)}");print(json.dumps(comparisons));return int(bool(errors))
if __name__=='__main__':raise SystemExit(main())
