"""Execute complete retail tree insertion and verify red-black invariants after each insert."""
import random,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000;OWNER=BASE;NODES=0x2008000

def sequences():
    import itertools
    rows=[[(index,index)for index in order]for order in itertools.permutations(range(3))]
    rows += [[(index,index)for index in range(32)],[(index,index)for index in reversed(range(32))],
             [(index,7)for index in range(24)],[(index,7)for index in reversed(range(24))],
             [(index,value)for index,value in enumerate([-0x80000000,0x7fffffff,0,-1,1,-0x80000000,0x7fffffff])]]
    for seed in [13,97]:
        rng=random.Random(seed);indices=list(range(64));rng.shuffle(indices)
        rows.append([(index,rng.choice([-0x80000000,0x7fffffff,-8,0,32,128,rng.randint(-1000,1000)]))for index in indices])
    return rows

def validate(read):
    def word(address):return struct.unpack('<I',bytes(read(address,4)))[0]
    root=word(OWNER+0xc34)
    assert root and word(root+0xc)==0 and bytes(read(root+0x1c,1))==b'\1'
    seen=set()
    def walk(node,parent,lower,upper):
        if not node:return 1
        assert node not in seen;seen.add(node);assert word(node+0xc)==parent
        key=(struct.unpack('<i',bytes(read(node+0x18,4)))[0],node)
        assert (lower is None or lower<key)and(upper is None or key<upper)
        colour=bytes(read(node+0x1c,1))[0];assert colour in [0,1]
        left=word(node+0x10);right=word(node+4)
        if colour==0:assert all(not child or bytes(read(child+0x1c,1))==b'\1'for child in [left,right])
        a=walk(left,node,lower,key);b=walk(right,node,key,upper);assert a==b
        return a+(colour==1)
    walk(root,0,None,None);return len(seen)

def collect():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];coverage=set()
    uc.hook_add(UC_HOOK_CODE,lambda uc,address,size,user:coverage.add(address))
    def word(address,value):uc.mem_write(address,struct.pack('<I',value&0xffffffff))
    for sequence in sequences():
        for pattern in [0x5a,0xa5]:
            uc.mem_write(BASE,bytes([pattern])*WIDTH);word(OWNER+0xc34,0)
            for index,value in sequence:
                node=NODES+index*0x40
                for offset in [4,0xc,0x10]:word(node+offset,0)
                word(node+0x18,value)
            snapshots=[]
            for count,(index,value)in enumerate(sequence,1):
                node=NODES+index*0x40;root=struct.unpack('<I',bytes(uc.mem_read(OWNER+0xc34,4)))[0]
                uc.mem_write(stack,struct.pack('<3I',stop,root,node));uc.reg_write(UC_X86_REG_ECX,OWNER);uc.reg_write(UC_X86_REG_ESP,stack)
                uc.emu_start(0xa64e70,stop,count=20000)
                assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+12
                assert validate(uc.mem_read)==count
                snapshots.append(bytes(uc.mem_read(BASE,WIDTH)).hex())
            rows.append(dict(sequence=sequence,pattern=pattern,snapshots=snapshots))
    required=[0xa64792,0xa647ae,0xa647eb,0xa64852,0xa6488f,0xa648f2]
    assert all(address in coverage for address in required),[hex(a)for a in required if a not in coverage]
    return rows

def collect_operations():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[]
    def word(address,value):uc.mem_write(address,struct.pack('<I',value&0xffffffff))
    import itertools
    cases=list(itertools.product([1,2],[0,1,2],[0,1],[0x5a,0xa5]))+[(3,0,0,p)for p in [0x5a,0xa5]]+[(3,1,0,p)for p in [0x5a,0xa5]]
    for mode,parent_side,transfer,pattern in cases:
        uc.mem_write(BASE,bytes([pattern])*WIDTH)
        for index in range(4):
            for offset in [4,0xc,0x10]:word(NODES+index*0x40+offset,0)
        area=NODES;child=NODES+0x40;parent=NODES+0x80;middle=NODES+0xc0
        if mode in [1,2]:
            word(area+(4 if mode==1 else 0x10),child);word(child+0xc,area)
            word(child+(0x10 if mode==1 else 4),middle if transfer else 0)
            if transfer:word(middle+0xc,child)
            word(area+0xc,parent if parent_side else 0)
            if parent_side:word(parent+(0x10 if parent_side==1 else 4),area)
            word(OWNER+0xc34,parent if parent_side else area)
            entry=0xa64680 if mode==1 else 0xa646e0;argument=area
        else:
            word(OWNER+0xc34,area);uc.mem_write(area+0x1c,b'\0')
            argument=area
            if parent_side:
                word(area+4,child);word(child+0xc,area);uc.mem_write(child+0x1c,b'\0');argument=child
            entry=0xa64740
        before=bytes(uc.mem_read(BASE,WIDTH)).hex()
        uc.mem_write(stack,struct.pack('<2I',stop,argument));uc.reg_write(UC_X86_REG_ECX,OWNER);uc.reg_write(UC_X86_REG_ESP,stack)
        uc.emu_start(entry,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
        rows.append(dict(mode=mode,argument=argument,before=before,after=bytes(uc.mem_read(BASE,WIDTH)).hex()))
    return rows

if __name__=='__main__':
    rows=collect();print('VARIABLE_TREE_RETAIL sequences=%d snapshots=%d operations=%d'%(len(rows),sum(len(row['snapshots'])for row in rows),len(collect_operations())))
