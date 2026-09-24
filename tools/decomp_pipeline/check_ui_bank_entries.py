#!/usr/bin/env python3
"""Connected archive entry decoder versus retail, with named finalization boundaries."""
import hashlib,json,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX,UC_X86_REG_ECX,UC_X86_REG_EDX,UC_X86_REG_ESP,UC_X86_REG_EIP
from check_cgame_play import ROOT,parity,pe_oracle,run
from check_ui_bank_storage import SOURCES as STORAGE,DEPENDENCIES as STORAGE_DEPS
from check_ui_bank_aliases import SOURCES as ALIASES
from check_ui_bank_decode import SOURCES as DECODE
from check_ui_strings import SOURCES as STRINGS
FUNCTIONS=[('00/9c/CBankFile_ReadEntries_009cfbc0',0x9CFBC0,1969,'FableUiReadBankEntries')]
SOURCES=['rebuild/src/compiled/'+f[0]+'.cpp' for f in FUNCTIONS]
DEPENDENCIES=list(dict.fromkeys(STORAGE+STORAGE_DEPS+ALIASES+DECODE+STRINGS+['rebuild/src/compiled/00/99/CDataInputStream_ReadSlow_00993ca0.cpp','rebuild/src/compiled/00/40/Global_GetStreamPosition_00405fa0.cpp']))
def main():
    image=pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest()!='41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10':raise RuntimeError('Retail oracle changed')
    directory=ROOT/'work/ui_bank_entries_check';directory.mkdir(parents=True,exist_ok=True);env=parity.env();objects=[]
    for i,source in enumerate(SOURCES+DEPENDENCIES+['rebuild/tests/integration/UiBankEntries_test.cpp']):
        obj=directory/f'part{i}.obj';run([parity.CL_EXE,'/nologo','/c','/W3','/MT','/GS','/O2','/Oy','/I'+str(ROOT/'rebuild/include'),'/Fo'+str(obj),ROOT/source],env);objects.append(obj)
    exe=directory/'behavior.exe';run([parity.VC/'bin/link.exe','/nologo','/subsystem:console','/out:'+str(exe),*objects],env)
    lines=run([exe],env).splitlines()
    if len(lines)!=512:raise RuntimeError('Incomplete entry traces')
    uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1100000);uc.mem_map(0x20000000,0x40000)
    for _,va,_,raw,size in pe_oracle.pe_sections(image):uc.mem_write(0x400000+va,image[raw:raw+size])
    stop,stack,bank,stream,payload,arrays,records,buffers,bt,st=0x20000000,0x20008000,0x20010000,0x20011000,0x20012000,0x20014000,0x20019000,0x20020000,0x20030000,0x20030100
    table,entry,finish,seek,use,direct=[0x20031000+i*16 for i in range(6)]
    def put(p,v):uc.mem_write(p,struct.pack('<I',v&0xffffffff))
    def word(p):return struct.unpack('<I',uc.mem_read(p,4))[0]
    def byte(p):return uc.mem_read(p,1)[0]
    def flag(p,v):uc.mem_write(p,bytes([int(v)]))
    def text(p):
        record=word(p)
        if not record:return ''
        return bytes(uc.mem_read(word(record),word(record+4))).split(b'\0')[0].decode('ascii')
    def ident(p,base,stride):return (p-base)//stride+1 if p else 0
    def hexdata(p,n):return bytes(uc.mem_read(p,n)).hex() if n else ''
    services={0xBFEA0E:0,0xBFEA14:0,0xBFEA1A:0,0xBFE9BC:0,0xBFEB22:0,0xBFEB1C:0,0x9CE050:12,0x9B85A0:4,0x9B7B10:0,0x9CD740:0,table:8,entry:12,finish:0,seek:4,use:4,direct:8}
    for p,n in services.items():uc.mem_write(p,b'\xc2'+struct.pack('<H',n))
    for i,p in ((10,table),(12,entry),(15,finish)):put(bt+i*4,p)
    for i,p in ((1,seek),(2,0x405FA0),(9,use),(10,direct)):put(st+i*4,p)
    events=[];ac=rc=bc=seed=used=0
    def hook(uc,address,size,data):
        nonlocal ac,rc,bc
        if address not in services:return
        esp=uc.reg_read(UC_X86_REG_ESP);arg=word(esp+4)
        if address==0xBFEA0E:
            events.append('A'+str(arg));uc.reg_write(UC_X86_REG_EAX,arrays+ac*256);ac+=1
        elif address==0xBFEA1A:
            events.append('R'+str(arg));uc.reg_write(UC_X86_REG_EAX,records+rc*32);rc+=1
        elif address==0xBFEB22:
            events.append('B'+str(arg));uc.reg_write(UC_X86_REG_EAX,buffers+bc*256);bc+=1
        elif address==0xBFEA14:events.append('AF'+str(ident(arg,arrays,256)))
        elif address==0xBFE9BC:events.append('RF'+str(ident(arg,records,32)))
        elif address==0xBFEB1C:events.append('BF'+str(ident(arg,buffers,256)))
        elif address==table:
            pairs=word(esp+8);events.append(f'TABLE{arg}:'+hexdata(word(pairs),word(pairs+4)-word(pairs)))
        elif address==entry:
            blob=word(esp+12);events.append(f'ENTRY{arg}:{word(esp+8)}:'+hexdata(word(blob),word(blob+4)-word(blob)));put(stream+4,word(stream+4)+seed%7)
        elif address==finish:events.append('FINISH'+str(byte(bank+0x94)))
        elif address==seek:
            events.append(f'SEEK{word(stream+4)}:{arg}');put(stream+4,arg);put(stream+12,payload+arg);put(stream+20,used-arg)
        elif address==use:uc.reg_write(UC_X86_REG_EAX,0)
        elif address==direct:
            n=word(esp+8);pos=word(stream+4);events.append(f'READ{pos}:{n}')
            if pos+n>used:raise RuntimeError('Past payload')
            if n:uc.mem_write(arg,bytes(uc.mem_read(payload+pos,n)))
        elif address==0x9CE050:
            events.append(f'INDEX{arg}:{text(word(esp+8))}:{text(word(esp+12))}')
            if seed&128:flag(bank+0xDD,True)
        elif address==0x9B85A0:events.append(f'SORT{int(uc.reg_read(UC_X86_REG_ECX)==0)}:{int(uc.reg_read(UC_X86_REG_EDX)==0)}:{arg}')
        elif address==0x9B7B10:
            events.append('COMPACT'+str(byte(bank+0xDD)))
            if seed&256:flag(bank+0x8D,not byte(bank+0x8D))
        elif address==0x9CD740:events.append('PACK'+str(word(bank+4)))
    uc.hook_add(UC_HOOK_CODE,hook);errors=[]
    for seed,actual in enumerate(lines):
        events.clear();ac=rc=bc=0;entry_count=(seed//8)%4;data=bytearray()
        def add(v):data.extend(struct.pack('<I',v))
        def string(n,salt):add(n);data.extend(65+(seed+salt+i)%26 for i in range(n))
        pairs=(seed//4)%3;add(pairs)
        for p in range(pairs):add(seed+p);add(seed*17+p)
        for e in range(entry_count):
            for v in (0xDEAD0000+e,entry_count-e,0x12340000+seed+e,seed*11+e,seed*23+e,seed*37+e):add(v)
            string((seed+e)%5,e);add(seed*41+e);aliases=(seed+e)%3;add(aliases)
            for a in range(aliases):string((seed+a)%5,a+e)
            n=(seed+e)%9;add(n);data.extend((seed+b+e)&255 for b in range(n))
        used=len(data);uc.mem_write(payload,bytes(data));uc.mem_write(bank,b'\0'*0x110);uc.mem_write(stream,b'\0'*128)
        uc.mem_write(arrays,b'\xcd'*(64*256));uc.mem_write(records,b'\xcd'*(32*32));uc.mem_write(buffers,b'\xcd'*(64*256));put(0x13BD800,0)
        put(bank,bt);put(bank+0x78,(seed%8)*8);flag(bank+0x8D,bool(seed&32));flag(bank+0xDD,bool(seed&64));flag(bank+0xDC,seed%3)
        put(stream,st);put(stream+8,used);put(stream+12,payload);put(stream+20,used if seed%3==0 else 0 if seed%3==1 else 3)
        for i,v in enumerate((stop,stream,entry_count+1,entry_count)):put(stack+i*4,v)
        uc.reg_write(UC_X86_REG_ESP,stack);uc.reg_write(UC_X86_REG_ECX,bank);uc.emu_start(0x9CFBC0,stop,count=300000)
        if uc.reg_read(UC_X86_REG_EIP)!=stop or uc.reg_read(UC_X86_REG_ESP)!=stack+16:raise RuntimeError('Entry stack mismatch')
        events.append(f'RESULT{uc.reg_read(UC_X86_REG_EAX)&255}:{word(bank+4)}:{byte(bank+0x94)}:{byte(bank+0xDD)}:{byte(bank+0x8D)}:{word(stream+4)}:{word(0x13BD800)}')
        flags=word(bank+0x78)
        for i in range(word(bank+4)):
            v=word(bank+0x20)+i*12;events.append(f'DATA{word(v)}:{word(v+4)}:{byte(v+8)}:{byte(v+9)}')
            if flags&8:events.append('SYMBOL:'+text(word(bank+8)+i*4))
            if flags&16:events.append('CRC'+str(word(word(bank+0x14)+i*4)))
            if flags&32:
                u=word(word(bank+0x2C)+i*4)
                if not u:events.append('UPDATE0')
                else:
                    events.append(f'UPDATE{word(u)}:{word(u+4)}:{byte(u+20)}:{byte(u+21)}:'+hexdata(word(u+8),word(u+4)))
                    for a in range(byte(u+16)):events.append('ALIAS:'+text(word(u+12)+a*4))
        expected='TRACE'+''.join(' '+e for e in events)+' END'
        if actual!=expected:errors.append(dict(case=seed,actual=actual,retail=expected))
    comparisons=[]
    for obj,(_,address,size,symbol) in zip(objects,FUNCTIONS):
        code,section,_=parity.obj_text(obj,'?'+symbol);relocs=parity.obj_relocs(obj,section)
        comparisons.append(dict(address=f'{address:08x}',compiled_bytes=len(code),retail_bytes=size,grade='DIFFER',masked_sha256=hashlib.sha256(parity.mask(code,relocs)).hexdigest()));(directory/f'{address:08x}-disassembly.txt').write_text(run([parity.OBJDUMP,'-dr',obj],env))
    (directory/'report.json').write_text(json.dumps(dict(accepted=not errors,cases=len(lines),errors=errors,comparisons=comparisons,scope='Actual decoder, storage preparation/resizing, strings, alias lists, temporary arrays and slow stream read. Controlled allocations, direct backing reads/seek, index registration 009CE050, sort 009B85A0, compact 009B7B10, pack 009CD740 and bank virtual callbacks. Zero/multiple entries and pairs, optional symbol/checksum/update flags, empty strings/lists/blobs, runtime type truncation, callbacks moving stream position, finalization flag order. Valid payloads/indexes and successful allocation only. Not real archive/file or rendering proof.'),indent=2)+'\n')
    print(f"UI_BANK_ENTRIES {'FAIL' if errors else 'PASS'} cases={len(lines)} failures={len(errors)}");print(json.dumps(comparisons));return int(bool(errors))
if __name__=='__main__':raise SystemExit(main())
