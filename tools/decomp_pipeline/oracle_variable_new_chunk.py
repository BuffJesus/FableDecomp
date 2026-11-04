"""Retail OS-backed chunk retries with recorded Win32 and tree boundaries."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EAX,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
from oracle_variable_chunk_initialization import BASE,WIDTH,OWNER,CHUNK,OLD_CHUNK,AREAS
CASES=[(alignment,request,minimum,flags,failures,region,query_return,old,pattern)
       for alignment,request,minimum,flags,failures,region,query_return in [
       (4,64,4096,0,0,4096,28),(8,65,4096,2,0,8192,28),
       (16,128,65536,0,2,16384,28),(32,1,128,0x80000002,1,4096,0),
       (4,65536,128,0,1,0,28),(8,1,-1,2,0,0,28),
       (16,0x7fffffff,4096,0,0,0,28),(4,-1,4096,0,2,4096,28)]
       for old in [0,1]for pattern in [0x5a,0xa5]]

def collect(real_tree=False):
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];events=[];attempts=0;current=None;coverage=set()
    # Replace only the two retail IAT entries; execute genuine native helpers.
    uc.mem_write(0x143fd44,struct.pack('<I',0x200d000));uc.mem_write(0x143fe80,struct.pack('<I',0x200d010))
    def word(address,value):uc.mem_write(address,struct.pack('<I',value&0xffffffff))
    def hook(uc,address,size,user):
        nonlocal attempts
        if real_tree:coverage.add(address)
        if address not in [0x200d000,0x200d010,0xa64e70,0x9f1f00]:return
        esp=uc.reg_read(UC_X86_REG_ESP)
        if address==0x9f1f00:
            events.append(dict(kind='failure',this=uc.reg_read(UC_X86_REG_ECX)));return
        argc=4 if address==0x200d000 else 3 if address==0x200d010 else 2
        words=struct.unpack('<'+str(argc+1)+'I',bytes(uc.mem_read(esp,4*(argc+1))));args=list(words[1:])
        if address==0x200d000:
            attempts+=1;result=0 if attempts<=current[4]else CHUNK
            events.append(dict(kind='alloc',args=args,result=result))
        elif address==0x200d010:
            assert args[0]==CHUNK and args[2]==28
            uc.mem_write(args[1],struct.pack('<7I',CHUNK,CHUNK,4,current[5],0x1000,4,0x20000))
            result=current[6];events.append(dict(kind='query',args=[args[0],args[2]],result=result,region=current[5]))
        else:
            assert uc.reg_read(UC_X86_REG_ECX)==OWNER
            result=0;events.append(dict(kind='tree',args=[OWNER,*args],before=bytes(uc.mem_read(BASE,WIDTH)).hex()))
            if real_tree:return
        uc.reg_write(UC_X86_REG_EAX,result);uc.reg_write(UC_X86_REG_ESP,esp+4*(argc+1));uc.reg_write(UC_X86_REG_EIP,words[0])
    uc.hook_add(UC_HOOK_CODE,hook)
    cases=CASES
    if real_tree:
        cases=CASES+[(alignment,128,65536,0,0,4096 if old==2 else 12288,28,old,pattern)
                    for alignment in [4,8,16,32]for old in [2,3]for pattern in [0x5a,0xa5]]
    for current in cases:
        alignment,request,minimum,flags,failures,region,query_return,old,pattern=current
        events.clear();attempts=0;uc.mem_write(BASE,bytes([pattern])*WIDTH)
        shift={4:2,8:3,16:4,32:5}[alignment]
        for address,value in [(0x13d28d4,alignment),(0x13d28d8,shift),(OWNER+0xc14,alignment),(OWNER+0xc18,shift),(OWNER+0xc20,minimum),(OWNER+0xc28,flags),(OWNER+0xc1c,OLD_CHUNK if old else 0),(OWNER+0xc34,AREAS),(0x13caa90,OWNER+0x1000)]:word(address,value)
        if real_tree:
            word(OWNER+0xc34,AREAS if old else 0)
            if old:
                values=[128,64,256]if old==1 else[1024,512,8192,2048]if old==2 else[16384,8192,32768,12288]
                for index,value in enumerate(values):
                    node=AREAS+index*0x40
                    for offset in [4,0xc,0x10]:word(node+offset,0)
                    word(node+0x18,value);uc.mem_write(node+0x1c,bytes([0 if old==1 and index else 1]))
                word(AREAS+0x10,AREAS+0x40);word(AREAS+4,AREAS+0x80)
                word(AREAS+0x40+0xc,AREAS);word(AREAS+0x80+0xc,AREAS)
                if old>1:
                    parent=AREAS+(0x80 if old==2 else 0x40);child=AREAS+0xc0
                    word(parent+(0x10 if old==2 else 4),child);word(child+0xc,parent);uc.mem_write(child+0x1c,b'\0')
        uc.mem_write(stack,struct.pack('<2I',stop,request&0xffffffff));uc.reg_write(UC_X86_REG_ECX,OWNER);uc.reg_write(UC_X86_REG_ESP,stack)
        uc.emu_start(0xa65720,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
        if real_tree and (old or uc.reg_read(UC_X86_REG_EAX)):
            from oracle_variable_tree_insertion import validate
            assert validate(uc.mem_read)==(0 if not old else 3 if old==1 else 4)+(uc.reg_read(UC_X86_REG_EAX)!=0)
        rows.append(dict(args=current,result=uc.reg_read(UC_X86_REG_EAX),events=list(events),after=bytes(uc.mem_read(BASE,WIDTH)).hex()))
    if real_tree:
        required=[0xa64792,0xa647ae,0xa647eb,0xa64852,0xa6488f]
        assert all(address in coverage for address in required),[hex(a)for a in required if a not in coverage]
    return rows

if __name__=='__main__':
    rows=collect()
    print('VARIABLE_NEW_CHUNK_RETAIL cases=%d successful=%d failed=%d'%(len(rows),sum(r['result']!=0 for r in rows),sum(r['result']==0 for r in rows)))
