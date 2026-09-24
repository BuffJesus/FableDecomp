#!/usr/bin/env python3
"""Archive temporary arrays and length-prefixed string reads vs retail."""
import hashlib,json,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from check_cgame_play import ROOT,parity,pe_oracle,run
from check_ui_strings import SOURCES as STRINGS
from check_ui_bank_stream_read import SOURCES as READS
FUNCTIONS=[
 ('00/41/ByteVector_ConstructArchiveBuffer_00411910',0x411910,77,'FableUiConstructArchiveBytes'),
 ('00/9d/PairVector_ConstructArchiveTable_009d2af0',0x9D2AF0,94,'FableUiConstructArchivePairs'),
 ('00/99/CDataInputStream_ReadArchiveString_00996390',0x996390,334,'FableUiReadArchiveString'),
]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]
def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':raise RuntimeError('Retail oracle changed')
    directory=ROOT/'work/ui_bank_decode_check';directory.mkdir(parents=True,exist_ok=True);env=parity.env();objects=[]
    for i,source in enumerate(SOURCES+STRINGS+READS+['rebuild/src/compiled/00/40/Global_GetStreamPosition_00405fa0.cpp','rebuild/tests/integration/UiBankDecode_test.cpp']):
        obj=directory/f'part{i}.obj';run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env);objects.append(obj)
    exe=directory/'behavior.exe';run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe],env).splitlines()
    if len(lines)!=1536:raise RuntimeError('Incomplete decoder dependency traces')
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1100000);uc.mem_map(0x20000000,0x40000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image):uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,values,records,buffers,temporary,payload,refill,storage,file,ft,table,array=[0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20021000,0x20022000,0x20023000,0x20024000,0x20025000,0x20026000,0x20027000,0x20028000]
    stream=storage+8;seek,read=0x20029000,0x20029010
    def put(p,v):uc.mem_write(p,struct.pack('<I',v&0xffffffff))
    def word(p):return struct.unpack('<I',uc.mem_read(p,4))[0]
    def signed(p):return struct.unpack('<i',uc.mem_read(p,4))[0]
    def ident(p,base,stride):return (p-base)//stride+1 if p else 0
    for p,n in ((seek,4),(read,12),(0xBFEA0E,0),(0xBFEA14,0),(0xBFEA1A,0),(0xBFE9BC,0),(0xBFEB22,0),(0xBFEB1C,0)):uc.mem_write(p,b'\xc2'+struct.pack('<H',n))
    for off,p in ((8,0x405FA0),(32,0x994360),(36,0x9943B0),(40,0x9943D0)):put(table+off,p)
    put(file,ft);put(ft+20,seek);put(ft+12,read)
    mode=seed=array_count=array_size=record_count=attempt=file_position=0;events=[];sizes=[]
    def hook(uc,address,size,data):
        nonlocal array_count,array_size,record_count,attempt,file_position
        esp=uc.reg_read(UC_X86_REG_ESP)
        if address==0xBFEA0E:
            n=word(esp+4);events.append('TA'+str(n))
            if mode==1 and seed&128 and seed%32==1:events.append('FAIL');uc.reg_write(UC_X86_REG_EAX,0)
            else:
                if n>1000:raise RuntimeError('Oversize fixture allocation')
                array_size=n;array_count+=1;uc.reg_write(UC_X86_REG_EAX,temporary)
        elif address==0xBFEA14:events.append('TF'+str(int(word(esp+4)==temporary)))
        elif address==0xBFEA1A:
            n=word(esp+4);events.append('A'+str(n));attempt+=1
            if n!=17:raise RuntimeError('Wrong record size')
            if seed&512 and attempt==1:events.append('FAIL');uc.reg_write(UC_X86_REG_EAX,0)
            else:uc.reg_write(UC_X86_REG_EAX,records+record_count*32);record_count+=1
        elif address==0xBFE9BC:events.append('R'+str(ident(word(esp+4),records,32)))
        elif address==0xBFEB22:
            n=word(esp+4);events.append('B'+str(n));uc.reg_write(UC_X86_REG_EAX,buffers+len(sizes)*1024);sizes.append(n)
        elif address==0xBFEB1C:events.append('F'+str(ident(word(esp+4),buffers,1024)))
        elif address==seek:file_position=word(esp+4);events.append('SEEK'+str(file_position))
        elif address==read:
            out,n,flag=word(esp+4),word(esp+8),word(esp+12)&255;events.append(f'READ{n}:{flag}')
            if file_position+n>512:raise RuntimeError('Read past fixture payload')
            if n:uc.mem_write(out,bytes(uc.mem_read(payload+file_position,n)))
            file_position+=n
    uc.hook_add(UC_HOOK_CODE,hook)
    def execute(address,receiver,args):
        put(stack,stop)
        for i,v in enumerate(args):put(stack+4+i*4,v)
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,receiver);uc.emu_start(address,stop,count=100000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+4+len(args)*4:raise RuntimeError('Decoder dependency stack mismatch')
        return uc.reg_read(UC_X86_REG_EAX)
    def normalize(v):
        for base,size,logical in ((temporary,1024,0x61000000),(payload,512,0x62000000),(refill,256,0x63000000)):
            if base<=v<=base+size:return logical+v-base
        return v
    def snapshot():
        events.append('S:123'+''.join(':'+str(ident(word(values+i*4),records,32)) for i in range(5)))
        for i in range(record_count):
            p=records+i*32;events.append(f'C{i}:{ident(word(p),buffers,1)}:{word(p+4)}:{word(p+8):08x}:{uc.mem_read(p+12,1)[0]:02x}:{signed(p+13)}'+bytes(uc.mem_read(p+17,15)).hex())
        for i,n in enumerate(sizes):events.append(f'T{i}:'+bytes(uc.mem_read(buffers+i*1024,n+4)).hex())
    errors=[]
    for index,actual in enumerate(lines):
        mode,seed=(index//256,index%256) if index<512 else (2,index-512)
        events.clear();array_count=array_size=0;uc.mem_write(temporary,b'\xcd'*1024)
        if mode<2:
            uc.mem_write(array,bytes([(0,0xa5,0xff,17)[(seed>>5)%4]])*12)
            if execute(0x9D2AF0 if mode else 0x411910,array,[seed%32])!=array:raise RuntimeError('Wrong array return')
            events.append('ARRAY'+''.join(f':{normalize(word(array+i*4)):08x}' for i in range(3)))
        else:
            record_count=attempt=file_position=0;sizes.clear();uc.mem_write(records,b'\xa5'*1024);uc.mem_write(buffers,b'\xcd'*32768);uc.mem_write(values,b'\0'*20);put(0x13BD800,100)
            length=(0,1,3,7,15,31,63,127)[seed%8];kind=(seed>>7)%4;data=bytearray(b'\xa7'*512);struct.pack_into('<I',data,0,length)
            for i in range(length):data[4+i]=0x80+i%127 if kind==3 else 33+i%80
            if length and kind==1:data[4]=0
            if length and kind==2:data[4+length//2]=0
            uc.mem_write(payload,bytes(data));uc.mem_write(storage,b'\xa5'*52);uc.mem_write(refill,b'\xda'*256)
            for i,v in enumerate((table,0,512,payload,0,(0,2,4,256)[(seed>>3)%4],file,refill,4<<((seed>>5)%4))):put(stream+i*4,v)
            if execute(0x996390,stream,[values])!=values:raise RuntimeError('Wrong string return')
            snapshot();events.append('N'+str(word(0x13BD800)));state='STREAM'
            for i in range(0,52,4):
                v=0x70000000 if i==8 else 0x70000100 if i==32 else normalize(word(storage+i));state+=f':{v:08x}'
            events.append(state);events.append('FILE'+str(file_position));events.append('REFILL'+bytes(uc.mem_read(refill,256)).hex())
            execute(0x99EAE0,values,[]);snapshot();events.append('N'+str(word(0x13BD800)))
        events.append(f'TEMP{array_count}:'+bytes(uc.mem_read(temporary,array_size+4 if array_count else 4)).hex())
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected:errors.append(dict(case=index,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol);relocs=parity.obj_relocs(obj,section);off=pe_oracle.va_to_off(pe_oracle.pe_sections(image),address);matched=len(code)==size and parity.mask(code,relocs)==parity.mask(image[off:off+size],relocs)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='RELOCATION_MATCH' if matched else 'DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()));(directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(lines),errors=errors,comparisons=comparisons,scope='Byte and pair vector construction plus length-prefixed archive string; real narrow lifetime and stream slow/refill/direct paths. File and allocator boundaries controlled; full payload/temp/allocation/stream/guards. Invalid length/position, exhausted-file and crashing allocation paths excluded.'),indent=2)+'\n')
    print(f"UI_BANK_DECODE {'FAIL' if errors else 'PASS'} cases={len(lines)} failures={len(errors)}");print(json.dumps(comparisons));return int(bool(errors))
if __name__=='__main__':raise SystemExit(main())
