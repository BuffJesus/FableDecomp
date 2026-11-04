"""Fixed validation using retail ID chains and native/recorded internal validator."""
import struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
from oracle_fixed_enumeration import collect as enumeration_cases,BASE,WIDTH
MAIN=0x2007800;ALTERNATE=0x2007900
def collect():
    seeds=[r['initial']for r in enumeration_cases()if r['args'][0]==0 and r['args'][1]==0 and r['args'][2]==0]
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];events=[];boundary=0;chunk=0
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v))
    def hook(uc,address,size,user):
        if address!=0x9f1f40:return
        sp=uc.reg_read(UC_X86_REG_ESP);data=read(sp+4)
        events.append((uc.reg_read(UC_X86_REG_ECX),data,bytes(uc.mem_read(BASE,WIDTH))))
        if boundary==2 and len(events)==1:
            uc.mem_write(data-3,b'\xff');next_=read(chunk+8);word(chunk+8,read(next_+8)if next_ else 0);word(0x13caa90,ALTERNATE)
        if boundary:uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+8)
    uc.hook_add(UC_HOOK_CODE,hook)
    for initial in seeds:
        def get(a):return struct.unpack_from('<I',initial,a-BASE)[0]
        chain=[];p=get(BASE+0xc1c)
        while p:chain.append(p);p=get(p+8)
        nodeLists=[]
        for ch in chain:
            nodes=[];p=get(ch+0x14)
            while p:
                nodes.append(p);id_,next_=initial[p-BASE:p-BASE+2]
                p=0 if next_==255 else p+(next_-id_)*get(BASE+0xc2c)
            nodeLists.append(nodes)
        for boundary in [0,1,2]:
         for query in [0,1]:
            if query and not chain:continue
            selected=(len(chain)-1)//2 if query else 0;chunk=chain[selected]if chain else 0
            expectedNodes=nodeLists[selected][:]if query else[n for group in nodeLists for n in group]
            expected=bytearray(initial);mutated=boundary==2 and bool(expectedNodes)
            if mutated:
                first=expectedNodes[0];expected[first-BASE+1]=255
                following=chain[selected+2]if selected+2<len(chain)else 0;struct.pack_into('<I',expected,chunk-BASE+8,following)
                expectedNodes=[first]if query else[first]+[n for group in nodeLists[2:]for n in group]
            uc.mem_write(BASE,initial);word(0x13caa90,MAIN);events=[]
            args=[chunk]if query else[];uc.mem_write(stack,struct.pack('<'+str(len(args)+1)+'I',stop,*args));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
            uc.emu_start(0xa66260 if query else 0xa663a0,stop,count=3000)
            assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4*(len(args)+1)
            assert bytes(uc.mem_read(BASE,WIDTH))==expected and [e[1]for e in events]==[n+4 for n in expectedNodes]
            for i,(this,data,state)in enumerate(events):assert this==(ALTERNATE if mutated and i else MAIN)and state==(bytes(expected)if mutated and i else initial)
            final=ALTERNATE if mutated else MAIN;assert read(0x13caa90)==final
            rows.append(dict(args=(boundary,query,chunk,len(events)if boundary else 0,final),initial=initial,after=bytes(expected),events=events if boundary else [],native_calls=len(events)if not boundary else 0))
    return rows
