#!/usr/bin/env python3
"""Readable bank stream construction, seek, close and destruction vs retail."""
import hashlib,json,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from check_cgame_play import ROOT,parity,pe_oracle,run
FUNCTIONS=[
 ('00/99/CFileDataInputStream_ConstructForBank_00994700',0x994700,113,'FableUiConstructBankStream'),
 ('00/99/CDataInputStream_SeekForBank_00993bc0',0x993BC0,79,'FableUiSeekBankStream'),
 ('00/99/CFileDataInputStream_CloseForBank_00994300',0x994300,86,'FableUiCloseBankStream'),
 ('00/99/CFileDataInputStream_DestroyForBank_00994780',0x994780,28,'FableUiDestroyBankStream'),
 ('00/99/CBase_DestroyForStream_0099a300',0x99A300,7,'FableUiDestroyStreamBase'),
 ('00/40/Global_GetStreamPosition_00405fa0',0x405FA0,4,'FableUiBankStreamPosition'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]
def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':raise RuntimeError('Retail oracle changed')
    directory=ROOT/'work/ui_bank_stream_check';directory.mkdir(parents=True,exist_ok=True);env=parity.env();objects=[]
    for i,source in enumerate(SOURCES+['rebuild/src/compiled/00/99/CBase_ConstructUiResourceBase_0099a2f0.cpp','rebuild/tests/integration/UiBankStream_test.cpp']):
        obj=directory/f'part{i}.obj';run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env);objects.append(obj)
    exe=directory/'behavior.exe';run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe],env).splitlines()
    if len(lines)!=1024:raise RuntimeError('Incomplete stream traces')
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1100000);uc.mem_map(0x20000000,0x20000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image):uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,storage,buffer,files,table=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20013000;stream=storage+8
    length,position,seekable,setpos=0x20014000,0x20014010,0x20014020,0x20014030
    def put(p,v):uc.mem_write(p,struct.pack('<I',v&0xFFFFFFFF))
    def word(p):return struct.unpack('<I',uc.mem_read(p,4))[0]
    services={length:0,position:0,seekable:0,setpos:4,0xBFEB22:0,0xBFEB1C:0}
    for p,n in services.items():uc.mem_write(p,b'\xc2'+struct.pack('<H',n) if n else b'\xc3')
    for offset,cb in ((20,setpos),(28,position),(36,length),(40,seekable)):put(table+offset,cb)
    put(files,table);put(files+8,table)
    seed=0;events=[]
    def hook(uc,address,size,data):
        if address not in services:return
        esp=uc.reg_read(UC_X86_REG_ESP);receiver=uc.reg_read(UC_X86_REG_ECX);id_=(receiver-files)//8+1;result=0
        if address==length:
            events.append('LEN'+str(id_));result=(seed*0x1010101)&0xFFFFFFFF
            if seed&8:put(stream+24,files+8)
        elif address==position:events.append('POS'+str(id_));result=seed*13 if seed&2 else 0
        elif address==seekable:
            events.append('SEEKABLE'+str(id_));result=int(bool(seed&4))
            if seed&16:put(stream+24,files+8)
        elif address==setpos:events.append(f'SET{id_}:{word(esp+4)}')
        elif address==0xBFEB22:events.append('ALLOC'+str(word(esp+4)));result=0 if seed&32 else buffer
        elif address==0xBFEB1C:
            events.append('FREE'+str(int(word(esp+4)==buffer)))
            if seed&8:put(stream+24,files+8)
        uc.reg_write(UC_X86_REG_EAX,result)
    uc.hook_add(UC_HOOK_CODE,hook)
    errors=[];symbols={0x129A728:0x70000300,0x129A69C:0x70000301,0x1231710:0x70000302,files:0x65000000,files+8:0x65000008,buffer:0x66000000}
    for index,actual in enumerate(lines):
        mode,seed=divmod(index,256);events.clear();uc.mem_write(storage,b'\xA5'*52);args=[]
        if mode==0:address=0x994700;args=[files,(0,1,64,0xFFFFFFFF)[seed%4]]
        else:
            pos=(seed%16)*4;chunk=0 if seed&64 else (pos-4)&0xFFFFFFFF
            for i,v in enumerate((0x129A728,pos,seed*0x1010101,0 if seed&1 else buffer+128,chunk,-4 if seed&128 else 16,0 if seed&2 else files,0 if seed&1 else buffer,12345)):put(stream+i*4,v)
            address=(0,0x994300,0x994780,0x993BC0)[mode]
            if mode==3:args=[(pos,pos+16,pos+17,chunk,(pos-1)&0xFFFFFFFF,0xFFFFFFFF,0,(seed*0x1010101)&0xFFFFFFFF)[(seed>>1)%8]]
        put(stack,stop)
        for i,v in enumerate(args):put(stack+4+i*4,v)
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,stream);uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+4*len(args):raise RuntimeError('Stream stack mismatch')
        if mode==0 and uc.reg_read(UC_X86_REG_EAX)!=stream:raise RuntimeError('Stream constructor return mismatch')
        state='DATA'
        for i in range(0,52,4):
            v=word(storage+i)
            if i==20 and v:v=(0x66000000+v-buffer)&0xFFFFFFFF
            else:v=symbols.get(v,v)
            state+=f':{v:08x}'
        events.append(state);expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected:errors.append(dict(case=index,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol);relocs=parity.obj_relocs(obj,section);offset=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address);matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[offset:offset+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()))
        (directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=1024,errors=errors,comparisons=comparisons,scope='readable construction/seek/close/destruction/getter; shared layouts and real base; file virtual methods and buffer allocation controlled'),indent=2)+'\n')
    print(f"UI_BANK_STREAM {'FAIL' if errors else 'PASS'} cases=1024 failures={len(errors)}");print(json.dumps(comparisons));return int(bool(errors))
if __name__=='__main__':raise SystemExit(main())
