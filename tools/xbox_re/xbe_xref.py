import sys, struct, re
from capstone import Cs, CS_ARCH_X86, CS_MODE_32

path='default.xbe'
d=open(path,'rb').read()
base=struct.unpack_from('<I',d,0x104)[0]
nsec=struct.unpack_from('<I',d,0x11C)[0]
sec_addr=struct.unpack_from('<I',d,0x120)[0]
sec_off=sec_addr-base
secs=[]
for i in range(nsec):
    o=sec_off+i*0x38
    flags,va,vsz,raw,rawsz=struct.unpack_from('<IIIII',d,o)
    nameaddr=struct.unpack_from('<I',d,o+0x14)[0]
    nm=d[nameaddr-base:nameaddr-base+20].split(b'\x00')[0].decode('latin-1','replace')
    secs.append((nm,va,vsz,raw,rawsz))

def va_to_off(va):
    for nm,v,vsz,raw,rawsz in secs:
        if v<=va<v+max(vsz,rawsz):
            return raw+(va-v)
    return None
def off_to_va(off):
    for nm,v,vsz,raw,rawsz in secs:
        if raw<=off<raw+rawsz:
            return v+(off-raw)
    return None

# find a string VA by content
def find_str_va(needle):
    nb=needle.encode()
    idx=d.find(nb)
    res=[]
    while idx!=-1:
        va=off_to_va(idx)
        if va: res.append((va,idx))
        idx=d.find(nb, idx+1)
    return res

# .text section
tx=[s for s in secs if s[0]=='.text'][0]
txname,txva,txvsz,txraw,txrawsz=tx
text=d[txraw:txraw+txrawsz]

md=Cs(CS_ARCH_X86, CS_MODE_32)

def find_imm_refs(target_va):
    pat=struct.pack('<I',target_va)
    hits=[]
    start=0
    while True:
        i=text.find(pat,start)
        if i==-1: break
        hits.append(txva+i)
        start=i+1
    return hits

targets = sys.argv[1:] or [
 'GAME_ACTION_LOCK_TARGET','GAME_ACTION_FIRE_RANGED_WEAPON',
 'You pressed the lock target button!',
 'was unable to initialise a joystick',
]
for t in targets:
    print('#'*70)
    print('STRING:',repr(t))
    for va,off in find_str_va(t):
        print(f'  str@ VA=0x{va:08X}')
        refs=find_imm_refs(va)
        print(f'    {len(refs)} immediate refs in .text: '+', '.join(f'0x{r:08X}' for r in refs[:12]))
        # disasm a window before each ref
        for r in refs[:3]:
            roff=r-txva
            wstart=max(0,roff-64)
            code=text[wstart:roff+16]
            print(f'    --- disasm around 0x{r:08X} ---')
            for ins in md.disasm(code, txva+wstart):
                mark='  <<< str ref' if ins.address<=r<ins.address+ins.size else ''
                print(f'      0x{ins.address:08X}  {ins.mnemonic:7s} {ins.op_str}{mark}')
