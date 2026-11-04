"""Execute retail tree deletions, checking exact membership and red-black invariants."""
import random,struct
from unicorn import Uc,UC_ARCH_X86,UC_MODE_32,UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX,UC_X86_REG_ESP,UC_X86_REG_EIP
from pe_oracle import EXE,pe_sections
from oracle_variable_tree_insertion import collect as insertions,validate,BASE,WIDTH,OWNER,NODES

def collect(fixup_rows=None):
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[];coverage=set()
    active=None;descriptor=0x200bf00
    def hook(uc,address,size,user):
        nonlocal active
        coverage.add(address)
        if fixup_rows is None:return
        if address==0xa64910:
            assert active is None and uc.reg_read(UC_X86_REG_ECX)==OWNER
            sp=uc.reg_read(UC_X86_REG_ESP);ret,area,info=struct.unpack('<3I',bytes(uc.mem_read(sp,12)))
            data=bytes(uc.mem_read(info,12))if info else b''
            before=bytearray(uc.mem_read(BASE,WIDTH))
            if info:before[descriptor-BASE:descriptor-BASE+12]=data
            active=(ret,area,descriptor if info else 0,data,bytes(before))
        elif active is not None and address==active[0]:
            ret,area,info,data,before=active;after=bytearray(uc.mem_read(BASE,WIDTH))
            if info:after[descriptor-BASE:descriptor-BASE+12]=data
            fixup_rows.append(dict(area=area,info=info,before=before,after=bytes(after)));active=None
    uc.hook_add(UC_HOOK_CODE,hook)
    def word(a):return struct.unpack('<I',bytes(uc.mem_read(a,4)))[0]
    def members():
        pending=[word(OWNER+0xc34)];result=set()
        while pending:
            a=pending.pop()
            if not a:continue
            assert a not in result;result.add(a);pending.extend([word(a+4),word(a+0x10)])
        return result
    for tree in insertions():
        initial=bytes.fromhex(tree['snapshots'][-1]);nodes=[NODES+index*0x40 for index,size in tree['sequence']]
        for mode in ['ascending','descending','root','random']:
            uc.mem_write(BASE,initial);remaining=set(nodes);snapshots=[];operations=[]
            order=sorted(nodes,reverse=mode=='descending')
            if mode=='random':random.Random(73).shuffle(order)
            for i in range(len(nodes)):
                area=word(OWNER+0xc34)if mode=='root'else order[i]
                before=bytes(uc.mem_read(BASE,WIDTH));remaining.remove(area)
                uc.mem_write(stack,struct.pack('<2I',stop,area));uc.reg_write(UC_X86_REG_ECX,OWNER);uc.reg_write(UC_X86_REG_ESP,stack)
                uc.emu_start(0xa64ef0,stop,count=20000)
                assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+8
                assert members()==remaining
                if remaining:assert validate(uc.mem_read)==len(remaining)
                else:assert word(OWNER+0xc34)==0
                assert all(word(area+offset)==0 for offset in [4,0xc,0x10])
                after=bytes(uc.mem_read(BASE,WIDTH))
                for offset,length in [(0,4),(8,4),(0x14,8),(0x1d,3)]:
                    assert after[area-BASE+offset:area-BASE+offset+length]==before[area-BASE+offset:area-BASE+offset+length]
                operations.append(area);snapshots.append(after)
            rows.append(dict(initial=initial,operations=operations,snapshots=snapshots,mode=mode))
    required=[0xa6497a,0xa649ef,0xa64a10,0xa64a1e,0xa64aa5,0xa64b15,0xa64b36,0xa64b77]
    assert all(a in coverage for a in required),[hex(a)for a in required if a not in coverage]
    assert active is None
    return rows

def collect_fixups():
    rows=[];collect(fixup_rows=rows)
    assert rows
    return rows+collect_guards()

def collect_guards():
    raw=EXE.read_bytes();uc=Uc(UC_ARCH_X86,UC_MODE_32);uc.mem_map(0x400000,0x1200000)
    for _,va,vs,offset,size in pe_sections(raw):
        if size:uc.mem_write(0x400000+va,raw[offset:offset+size])
    uc.mem_map(0x2000000,0x20000);stack=0x2010000;stop=0x200f000;rows=[]
    def word(a,v):uc.mem_write(a,struct.pack('<I',v))
    descriptor=0x200b000
    for pattern in [0x5a,0xa5]:
      for area_mode in [0,1,2]:
       for info_mode in [0,1,2,3]:
        uc.mem_write(BASE,bytes([pattern])*WIDTH);word(OWNER+0xc34,NODES)
        uc.mem_write(NODES+0x1c,bytes([0 if area_mode==1 else 1]))
        word(descriptor+4,NODES if info_mode==3 else 0)
        uc.mem_write(descriptor+8,bytes([0 if info_mode==3 else 1,0 if info_mode==1 else 1]))
        area=NODES if area_mode else 0;info=descriptor if info_mode else 0
        before=bytes(uc.mem_read(BASE,WIDTH));expected=bytearray(before)
        if area:expected[area-BASE+0x1c]=1
        uc.mem_write(stack,struct.pack('<3I',stop,area,info));uc.reg_write(UC_X86_REG_ECX,OWNER);uc.reg_write(UC_X86_REG_ESP,stack)
        uc.emu_start(0xa64910,stop,count=10000)
        assert uc.reg_read(UC_X86_REG_EIP)==stop and uc.reg_read(UC_X86_REG_ESP)==stack+12
        after=bytes(uc.mem_read(BASE,WIDTH));assert after==expected
        rows.append(dict(area=area,info=info,before=before,after=after))
    return rows

if __name__=='__main__':
    rows=collect();print('VARIABLE_TREE_DELETION_RETAIL sequences=%d operations=%d'%(len(rows),sum(len(r['operations'])for r in rows)))
