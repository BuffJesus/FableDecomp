"""Retail destructor/DeleteChunk replay, with recorded tree and OS boundaries."""
import struct,itertools
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP,UC_X86_REG_EAX
from pe_oracle import EXE,pe_sections
BASE=0x2004000;WIDTH=0x8000
def collect(native_tree=False):
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];events=[];statusCount=0;statusPointer=0;chain=[];flip=0;freeResult=0
    def word(a,v):uc.mem_write(a,struct.pack('<I',v&0xffffffff))
    def read(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def event(kind,a,b,c):events.append((kind,a,b,c,read(0x13d28d4),read(0x13d28d8),bytes(uc.mem_read(BASE,WIDTH))))
    removed=[];all_nodes=[]
    def check_tree():
        if not native_tree:return
        alive=set(all_nodes)-set(removed)
        if alive:
            from oracle_variable_tree_insertion import validate
            assert validate(uc.mem_read)==len(alive)
            pending=[read(BASE+0xc34)];seen=set()
            while pending:
                node=pending.pop()
                if node:seen.add(node);pending.extend([read(node+4),read(node+0x10)])
            assert seen==alive
        else:assert read(BASE+0xc34)==0
        for node in removed:assert all(read(node+off)==0 for off in [4,0xc,0x10])
    def hook(uc,address,size,user):
        nonlocal statusCount,statusPointer
        sp=uc.reg_read(UC_X86_REG_ESP)
        if address==0xa64ef0:
            if native_tree:
                check_tree();removed.append(read(sp+4));return
            assert uc.reg_read(UC_X86_REG_ECX)==BASE;event(1,BASE,read(sp+4),0);n=2
        elif address==0x200d000:
            check_tree()
            chunk=chain[statusCount//2];pointer=read(sp+4)
            if statusCount%2==0:statusPointer=pointer
            else:assert pointer==statusPointer
            event(3,chunk,0,0)
            uc.mem_write(pointer,struct.pack('<8I',32,17,1000,900,800,700,600,500))
            if statusCount%2==0 and flip:
                uc.mem_write(chunk+0x1d,bytes([bytes(uc.mem_read(chunk+0x1d,1))[0]^1]))
                word(BASE+0xc14,128);word(BASE+0xc18,7);word(0x13d28d4,64);word(0x13d28d8,6)
            statusCount+=1;n=2
        elif address==0x200d100:
            a,b,c=read(sp+4),read(sp+8),read(sp+12);assert b==0 and c==0x8000
            event(4,a,b,c);uc.reg_write(UC_X86_REG_EAX,freeResult);n=4
        else:return
        uc.reg_write(UC_X86_REG_EIP,read(sp));uc.reg_write(UC_X86_REG_ESP,sp+n*4)
    word(0x143fe20,0x200d000);word(0x143fd40,0x200d100);uc.hook_add(UC_HOOK_CODE,hook)
    cases=itertools.product([4,128],[0,1,3,7],[0,1,2],[0,1],[0,1],[0,1],[-1,0,1,2,3])
    for case,(alignment,count,layout,permanent,demand,flip,flags)in enumerate(cases):
        shift=2 if alignment==4 else 7;pattern=0x5a if case%2 else 0xa5;freeResult=case%2
        uc.mem_write(BASE,bytes([pattern])*WIDTH);word(BASE,0x11111111);word(BASE+0xc14,alignment);word(BASE+0xc18,shift)
        chain=[0x2006000+i*0x100 for i in range(count)];word(BASE+0xc1c,chain[0]if chain else 0)
        treeOrder=[]
        if native_tree:word(BASE+0xc34,0)
        for i,chunk in enumerate(chain):
            word(chunk+0x20,chain[i+1]if i+1<count else 0);word(chunk+0x24,chain[i-1]if i else 0)
            uc.mem_write(chunk+0x1c,bytes([permanent,demand]));n=layout if layout<2 else i%3
            nodes=[0x2008000+(i*3+j)*0x80 for j in range(n)]
            word(chunk+0x28,nodes[0]if nodes else 0)
            for j,node in enumerate(nodes):
                word(node,nodes[j+1]if j+1<n else 0)
                if native_tree:
                    for off in [4,0xc,0x10]:word(node+off,0)
                    word(node+0x18,[64,0x80000000,0xffffffff][(i+j)%3])
            treeOrder.extend(nodes)
        if native_tree:
            for node in (treeOrder if case%2 else reversed(treeOrder)):
                uc.mem_write(stack,struct.pack('<3I',stop,read(BASE+0xc34),node));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
                uc.emu_start(0xa64e70,stop,count=20000)
            all_nodes=treeOrder[:];removed=[];check_tree()
        initial=bytes(uc.mem_read(BASE,WIDTH));events=[];statusCount=0
        word(0x13d28d4,256);word(0x13d28d8,8)
        args=[]if flags==-1 else[flags];uc.mem_write(stack,struct.pack('<'+str(len(args)+1)+'I',stop,*args));uc.reg_write(UC_X86_REG_ECX,BASE);uc.reg_write(UC_X86_REG_ESP,stack)
        uc.emu_start(0xa65690 if flags==-1 else 0xa65c60,stop,count=20000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+4*(len(args)+1)
        if flags!=-1:assert uc.reg_read(UC_X86_REG_EAX)==BASE
        expected=bytearray(initial);struct.pack_into('<I',expected,0,0x129db1c);struct.pack_into('<I',expected,0xc1c,0)
        for chunk in chain:
            struct.pack_into('<I',expected,chunk-BASE+0x24,0)
            if flip:expected[chunk-BASE+0x1d]^=1
        if flip and count:struct.pack_into('<2I',expected,0xc14,128,7)
        if native_tree:
            check_tree();assert removed==treeOrder
            actual=bytes(uc.mem_read(BASE,WIDTH));normalized=bytearray(actual)
            # Only the tree root and tree links/colour may additionally change.
            for off in [0xc34,*[node-BASE+field for node in all_nodes for field in [4,0xc,0x10]]]:normalized[off:off+4]=expected[off:off+4]
            for node in all_nodes:normalized[node-BASE+0x1c]=expected[node-BASE+0x1c]
            assert normalized==expected
            expected=bytearray(actual)
        else:assert bytes(uc.mem_read(BASE,WIDTH))==expected
        assert (native_tree or [e[2]for e in events if e[0]==1]==treeOrder) and statusCount==count*2
        expectedReleases=chain if not(demand^flip)else[]
        if flags!=-1 and flags&1:expectedReleases=expectedReleases+[BASE]
        assert [e[1]for e in events if e[0]==4]==expectedReleases
        globals_=(64,6)if flip and count else(alignment,shift)
        assert (read(0x13d28d4),read(0x13d28d8))==globals_
        rows.append(dict(args=(flags&0xffffffff,flip,freeResult,count,len(events),*globals_),initial=initial,after=bytes(expected),events=events))
    return rows
