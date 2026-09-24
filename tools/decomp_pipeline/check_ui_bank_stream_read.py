#!/usr/bin/env python3
"""Buffered/direct archive byte reads and file callback ordering vs retail."""
import hashlib,json,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from check_cgame_play import ROOT,parity,pe_oracle,run
FUNCTIONS=[
 ('CDataInputStream_ReadSlow_00993ca0',0x993CA0,351,'FableUiReadBankStreamSlow'),
 ('CFileDataInputStream_GetSource_00994360',0x994360,69,'FableUiGetBankStreamSource'),
 ('CFileDataInputStream_UseBuffer_009943b0',0x9943B0,19,'FableUiBankStreamUseBuffer'),
 ('CFileDataInputStream_ReadDirect_009943d0',0x9943D0,41,'FableUiReadBankStreamDirect'),
]
SOURCES=['rebuild/src/compiled/00/99/'+f[0]+'.cpp' for f in FUNCTIONS]
def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':raise RuntimeError('Retail oracle changed')
    directory=ROOT/'work/ui_bank_stream_read_check';directory.mkdir(parents=True,exist_ok=True);env=parity.env();objects=[]
    for i,source in enumerate(SOURCES+['rebuild/tests/integration/UiBankStreamRead_test.cpp']):
        obj=directory/f'part{i}.obj';run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env);objects.append(obj)
    exe=directory/'behavior.exe';run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe],env).splitlines()
    if len(lines)!=2048:raise RuntimeError('Incomplete read traces')
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1100000);uc.mem_map(0x20000000,0x20000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image):uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,storage,buffer,dest,files,table,ft,temp=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20013000,0x20014000,0x20015000,0x20016000
    stream=storage+8;setpos,read,position=0x20017000,0x20017010,0x20017020
    def put(p,v):uc.mem_write(p,struct.pack('<I',v&0xffffffff))
    def word(p):return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p):return struct.unpack('<i',uc.mem_read(p,4))[0]
    for p,n in ((setpos,4),(read,12),(position,0)):uc.mem_write(p,b'\xc2'+struct.pack('<H',n))
    for off,p in ((8,position),(32,0x994360),(36,0x9943B0),(40,0x9943D0)):put(table+off,p)
    put(ft+20,setpos);put(ft+12,read);events=[];seed=0
    def hook(uc,address,size,data):
        if address==position:
            if seed&256:put(stream+32,2)
            uc.reg_write(UC_X86_REG_EAX,word(stream+4));return
        if address not in (setpos,read):return
        esp=uc.reg_read(UC_X86_REG_ESP);f=uc.reg_read(UC_X86_REG_ECX);id_=word(f+8)
        if address==setpos:
            pos=word(esp+4);events.append(f'SET{id_}:{pos}');put(f+4,pos)
            if seed&32:put(stream+24,files+12)
        else:
            out,n,flag=word(esp+4),signed(esp+8),word(esp+12)&255
            ident=1000+out-buffer if buffer<=out<buffer+128 else 2000+out-dest
            events.append(f'READ{id_}:{ident}:{n}:{flag}')
            if not 0<=n<=64:raise RuntimeError('Invalid fixture read')
            if n:uc.mem_write(out,bytes(((word(f+4)+i)*13+id_)&255 for i in range(n)))
            put(f+4,word(f+4)+n)
            if seed&16:put(stream+28,buffer+64)
            if seed&8:put(stream+4,0x1234)
    uc.hook_add(UC_HOOK_CODE,hook);errors=[]
    for index,actual in enumerate(lines):
        mode,seed=divmod(index,512);events.clear();uc.mem_write(storage,b'\xa5'*52);uc.mem_write(buffer,b'\xcd'*128);uc.mem_write(buffer+8,bytes(range(0xa0,0xb0)));uc.mem_write(dest,b'\xee'*128)
        for i in range(2):put(files+i*12,ft);put(files+i*12+4,77+i);put(files+i*12+8,i+1)
        pos=(0,17,0x7ffffff0,0xfffffff0)[(seed>>6)%4];available=seed%4;n=(0,1,3,7,8,15,16,31)[(seed>>3)%8]
        for i,v in enumerate((table,pos,pos+256,buffer+8,pos-2,available,files,buffer+8,4<<((seed>>2)%4))):put(stream+i*4,v)
        if mode==0:address=0x993CA0;args=[dest+8,n+available]
        elif mode==1:address=0x994360;args=[temp,temp+4];put(temp,0);put(temp+4,-1)
        elif mode==2:address=0x9943D0;args=[dest+8,n]
        else:address=0x9943B0;args=[-1 if seed&256 else n]
        put(stack,stop)
        for i,v in enumerate(args):put(stack+4+i*4,v)
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,stream);uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4:raise RuntimeError('Read return/stack mismatch')
        if mode==1:events.append(f'SOURCE{word(temp)-buffer}:{signed(temp+4)}')
        if mode==3:events.append('USE'+str(uc.reg_read(UC_X86_REG_EAX)&255))
        state='STATE'
        for i in range(0,52,4):
            v=word(storage+i)
            if i==8:v=0x70000300
            if i in (20,36) and v:v=(0x66000000+v-buffer)&0xffffffff
            if i==32:v=word(v+8)
            state+=f':{v:08x}'
        events.append(state);events.append(f'FILES:{word(files+4)}:{word(files+16)}');events.append('BUFFER'+bytes(uc.mem_read(buffer,128)).hex());events.append('OUT'+bytes(uc.mem_read(dest,128)).hex())
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected:errors.append(dict(case=index,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol);relocs=parity.obj_relocs(obj,section);off=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address)
        matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[off:off+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()));(directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(lines),errors=errors,comparisons=comparisons,scope='Four connected stream read methods; file seek/read controlled. Full stream/guard/buffer/destination comparison; threshold and position wrap, partial buffered bytes, callbacks replacing file/buffer and changing stream position. No archive decoder or OS disk read claim.'),indent=2)+'\n')
    print(f"UI_BANK_STREAM_READ {'FAIL' if errors else 'PASS'} cases={len(lines)} failures={len(errors)}");print(json.dumps(comparisons));return int(bool(errors))
if __name__=='__main__':raise SystemExit(main())
