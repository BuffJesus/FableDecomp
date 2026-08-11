#!/usr/bin/env python3
# Structural diff: Xbox default.xbe vs PC Fable.exe (both VC7.1 x86-32).
import struct, re, sys, collections

PC  = r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe"
XBE = r"D:\tmp\fable_xbox\extracted\default.xbe"

def load(p): return open(p,'rb').read()

# ---------- XBE section parse ----------
def xbe_sections(d):
    base=struct.unpack_from('<I',d,0x104)[0]
    nsec=struct.unpack_from('<I',d,0x11C)[0]
    sec_addr=struct.unpack_from('<I',d,0x120)[0]
    sec_off=sec_addr-base
    secs=[]
    for i in range(nsec):
        o=sec_off+i*0x38
        flags,va,vsz,raw,rawsz=struct.unpack_from('<IIIII',d,o)
        nameaddr=struct.unpack_from('<I',d,o+0x14)[0]
        nm=d[nameaddr-base:nameaddr-base+24].split(b'\x00')[0].decode('latin-1','replace')
        secs.append((nm,va,vsz,raw,rawsz,flags))
    return base,secs

# ---------- PE section parse ----------
def pe_sections(d):
    e_lfanew=struct.unpack_from('<I',d,0x3C)[0]
    assert d[e_lfanew:e_lfanew+4]==b'PE\x00\x00'
    nsec=struct.unpack_from('<H',d,e_lfanew+6)[0]
    opt_sz=struct.unpack_from('<H',d,e_lfanew+20)[0]
    base=struct.unpack_from('<I',d,e_lfanew+24+28)[0]  # ImageBase in opt hdr
    sec_tab=e_lfanew+24+opt_sz
    secs=[]
    for i in range(nsec):
        o=sec_tab+i*40
        nm=d[o:o+8].split(b'\x00')[0].decode('latin-1','replace')
        vsz=struct.unpack_from('<I',d,o+8)[0]
        va =struct.unpack_from('<I',d,o+12)[0]
        rawsz=struct.unpack_from('<I',d,o+16)[0]
        raw=struct.unpack_from('<I',d,o+20)[0]
        secs.append((nm,base+va,vsz,raw,rawsz,0))
    return base,secs

# ---------- RTTI class names ----------
RTTI=re.compile(rb'\.\?A[UVW][\x21-\x7e]{1,200}?@@')
def classes(d):
    out=set()
    for m in RTTI.finditer(d):
        out.add(m.group(0).decode('latin-1'))
    return out

# ---------- ASCII strings ----------
ASCII=re.compile(rb'[\x20-\x7e]{5,}')
def strings(d):
    return set(m.group(0).decode('latin-1') for m in ASCII.finditer(d))

def demangle_class(s):
    # ".?AVCFoo@@" -> "CFoo" ; ".?AVFoo@Bar@@" -> "Bar::Foo"
    body=s[4:-2]
    parts=body.split('@')
    parts=[p for p in parts if p]
    return '::'.join(reversed(parts)) if parts else s

def main():
    pc=load(PC); xbe=load(XBE)
    pbase,psecs=pe_sections(pc)
    xbase,xsecs=xbe_sections(xbe)

    print("="*70)
    print("SECTION MAP")
    print("="*70)
    print(f"PC Fable.exe  base=0x{pbase:X}  sections={len(psecs)}")
    for nm,va,vsz,raw,rawsz,fl in psecs:
        print(f"   {nm:<10} va=0x{va:08X} vsz=0x{vsz:07X} rawsz=0x{rawsz:07X}")
    print(f"\nXbox default.xbe base=0x{xbase:X}  sections={len(xsecs)}")
    # summarize xbe section name families
    fam=collections.Counter()
    for nm,*_ in xsecs:
        key=re.sub(r'\d+$','#',nm)
        key=re.sub(r'^(S_[QV])[A-Z0-9_]+',r'\1*',key)
        fam[key]+=1
    for k,c in fam.most_common():
        print(f"   {k:<16} x{c}")

    pc_cls=classes(pc); xbe_cls=classes(xbe)
    print("\n"+"="*70)
    print("RTTI CLASS UNIVERSE")
    print("="*70)
    print(f"PC classes:  {len(pc_cls)}")
    print(f"XBE classes: {len(xbe_cls)}")
    common=pc_cls&xbe_cls
    only_pc=pc_cls-xbe_cls
    only_xbe=xbe_cls-pc_cls
    print(f"common:      {len(common)}")
    print(f"PC-only:     {len(only_pc)}")
    print(f"XBE-only:    {len(only_xbe)}")

    def dump(title, s, fn):
        names=sorted(demangle_class(x) for x in s)
        with open(fn,'w',encoding='utf-8') as f:
            f.write(f"# {title} ({len(names)})\n")
            for n in names: f.write(n+"\n")
    OUT=r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\43e0bee2-6846-40b3-ae3e-1246e3739a84\scratchpad"
    dump("PC-only classes", only_pc, OUT+r"\classes_pc_only.txt")
    dump("XBE-only classes", only_xbe, OUT+r"\classes_xbe_only.txt")
    dump("common classes", common, OUT+r"\classes_common.txt")

    # keyword buckets on XBE-only and PC-only
    def bucket(s, kws):
        d=collections.defaultdict(list)
        for c in s:
            dn=demangle_class(c)
            for kw in kws:
                if kw.lower() in dn.lower(): d[kw].append(dn)
        return d
    kws=['Xbox','X360','Live','Dvd','Controller','Pad','Gamepad','Joystick','Voice','Live','Save','Memory','Sound','Xaudio','Render','D3D','DX','Shader','Net','Online','Achievement','Debug']
    print("\nXBE-only class keyword buckets:")
    for kw,items in sorted(bucket(only_xbe,kws).items()):
        print(f"   {kw:<12} {len(items):>3}  e.g. {', '.join(sorted(items)[:4])}")
    print("\nPC-only class keyword buckets:")
    for kw,items in sorted(bucket(only_pc,kws).items()):
        print(f"   {kw:<12} {len(items):>3}  e.g. {', '.join(sorted(items)[:4])}")

    # strings quick diff on selected keywords
    print("\n"+"="*70)
    print("STRING SIGNALS")
    print("="*70)
    ps=strings(pc); xs=strings(xbe)
    for kw in ['xbox','xlive','xonline','joystick.ini','gamepad','d3d9','d3d8','directinput','xinput','msvcr71','msvcr','.pdb']:
        pin=sorted([s for s in ps if kw.lower() in s.lower()])[:3]
        xin=sorted([s for s in xs if kw.lower() in s.lower()])[:3]
        print(f"  [{kw}] PC:{len(pin)} XBE:{len(xin)}")
        if pin: print(f"        PC  e.g. {pin}")
        if xin: print(f"        XBE e.g. {xin}")

if __name__=='__main__':
    main()
