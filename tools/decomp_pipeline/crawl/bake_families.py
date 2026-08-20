import csv,struct,re
from pathlib import Path
from collections import defaultdict
import sys as _sys
_sys.path.insert(0, str(Path(__file__).resolve().parent))
from rowtrim import trim_body
from purity import is_genuine
ROOT=Path(r"D:\Documents\FableTLC")
EXE=Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
d=EXE.read_bytes();e=struct.unpack_from("<I",d,0x3C)[0];coff=e+4
nsec=struct.unpack_from("<H",d,coff+2)[0];opt=struct.unpack_from("<H",d,coff+16)[0]
sb=coff+20+opt;secs=[]
for i in range(nsec):
    o=sb+i*40
    va,=struct.unpack_from("<I",d,o+12);rp,=struct.unpack_from("<I",d,o+20)
    rs,=struct.unpack_from("<I",d,o+16);vs,=struct.unpack_from("<I",d,o+8)
    secs.append((va,max(vs,rs),rp))
def off(va):
    r=va-0x400000
    for va0,sz,rp in secs:
        if va0<=r<va0+sz: return rp+(r-va0)
rows=list(csv.DictReader(open(ROOT/"rebuild/manifest/functions.tsv",encoding="utf-8-sig"),delimiter="\t"))
ad=sorted(int(r["address"],16) for r in rows if r.get("address"))
nx={ad[i]:ad[i+1] for i in range(len(ad)-1)}
name={"%08x"%int(r["address"],16):(r.get("name") or "") for r in rows if r.get("address")}
def body(va):
    o=off(va);n=nx.get(va)
    if o is None or n is None: return b""
    raw=d[o:o+(n-va)];ee=len(raw)
    while ee>0 and raw[ee-1] in (0xCC,0x90): ee-=1
    return trim_body(raw[:ee], va)[0]
def mask(b):
    m=bytearray(b);i=0
    while i<len(m):
        if m[i]==0xe8 and i+5<=len(m): m[i+1:i+5]=b'\0\0\0\0';i+=5
        else: i+=1
    return bytes(m)
cat=(ROOT/"rebuild/build_candidates.ps1").read_text(encoding="utf-8")
blocks=re.findall(r"Address\s*=\s*'([0-9a-fA-F]{8})'\s*\n\s*Module\s*=\s*'(?:[^']*)'\s*\n\s*Source\s*=\s*'([^']+)'",cat)
groups=defaultdict(list)
baked=0
for a,src in blocks:
    p=ROOT/"rebuild/src/compiled"/src
    if not p.exists(): continue
    if is_genuine(p.read_text(encoding="utf-8",errors="ignore")): continue
    baked+=1
    b=body(int(a,16))
    groups[(len(b),mask(b))].append(a.lower())
print("baked total:",baked,"families:",len(groups))
fams=sorted(groups.items(),key=lambda kv:-len(kv[1]))
for (ln,_sk),addrs in fams[:20]:
    t=sorted(addrs)[0]
    print(f"{len(addrs):4}  len={ln:4}  template={t}  {name.get(t,'')[:70]}")
