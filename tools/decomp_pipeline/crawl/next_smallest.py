#!/usr/bin/env python3
"""Assemble the next binary-wide byte-parity batch: smallest un-landed manifest
functions that have a complete prototype + known calling convention.
Usage: python next_smallest.py <N> <out_prefix> [min_len]"""
import csv, struct, re, json, glob, sys
from pathlib import Path
N=int(sys.argv[1]) if len(sys.argv)>1 else 16
PREFIX=sys.argv[2] if len(sys.argv)>2 else "gen_batch"
MINLEN=int(sys.argv[3]) if len(sys.argv)>3 else 12
ROOT=Path(r"D:\Documents\FableTLC")
SCR=Path(r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\fd03f1b5-528d-4e74-abe5-2ce4dba32713\scratchpad")
EXE=Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
data=EXE.read_bytes()
e_lfanew=struct.unpack_from("<I",data,0x3C)[0]; coff=e_lfanew+4
nsec=struct.unpack_from("<H",data,coff+2)[0]; opt=struct.unpack_from("<H",data,coff+16)[0]
secbase=coff+20+opt; IMG=0x400000; secs=[]
for i in range(nsec):
    o=secbase+i*40
    va,=struct.unpack_from("<I",data,o+12); rp,=struct.unpack_from("<I",data,o+20)
    rs,=struct.unpack_from("<I",data,o+16); vs,=struct.unpack_from("<I",data,o+8)
    secs.append((va,max(vs,rs),rp))
def off(va):
    rva=va-IMG
    for va0,sz,rp in secs:
        if va0<=rva<va0+sz: return rp+(rva-va0)
    return None
rows=list(csv.DictReader(open(ROOT/"rebuild/manifest/functions.tsv",encoding="utf-8-sig"),delimiter="\t"))
addrs=sorted(int(r["address"],16) for r in rows if r.get("address"))
nextof={addrs[i]:addrs[i+1] for i in range(len(addrs)-1)}
meta={int(r["address"],16):r for r in rows if r.get("address")}
funcset=set(addrs)
cat=(ROOT/"rebuild/build_candidates.ps1").read_text(encoding="utf-8")
landed=set(int(m,16) for m in re.findall(r"Address\s*=\s*'([0-9a-fA-F]{8})'",cat))
# global 'tried' ledger to avoid re-attempting NOWINs across runs
tried=set()
tp=SCR/"gen_tried.txt"
if tp.exists(): tried=set(int(x,16) for x in tp.read_text().split())
def body(va):
    o=off(va); nx=nextof.get(va)
    if o is None or nx is None: return b""
    raw=data[o:o+(nx-va)]; e=len(raw)
    while e>0 and raw[e-1] in (0xCC,0x90): e-=1
    return raw[:e]
cand=[]
for r in rows:
    a=r.get("address"); 
    if not a: continue
    ai=int(a,16)
    if ai in landed or ai in tried: continue
    if r.get("retail_parity"): continue  # already matched somewhere
    if r.get("prototype_complete")!="1": continue
    cc=r.get("calling_convention","")
    if cc in ("","unknown"): continue
    b=body(ai)
    if len(b)<MINLEN: continue
    # skip pure IAT thunks (ff25) already handled, and giant fns
    if len(b)>140: continue
    cand.append((len(b),ai,r,b))
cand.sort(key=lambda x:x[0])
batch=[]
for ln,ai,r,b in cand:
    if len(batch)>=N: break
    ah="%08x"%ai
    if b.startswith(b"\xff\x25"): continue  # import thunk
    nm=(r.get("name") or ("sub_"+ah)).replace("[","_").replace("]","_").replace("<","_").replace(">","_")
    batch.append({"address":ah,"name":nm,"module":r.get("module") or "_global",
        "oracle":str(SCR/(PREFIX+"_oracle.tsv")),
        "sig":f"cc={r.get('calling_convention')} ret={r.get('return_type')} params={r.get('parameter_types')}"})
    # write oracle
with open(SCR/(PREFIX+"_oracle.tsv"),"w",encoding="utf-8",newline="") as f:
    w=csv.writer(f,delimiter="\t",lineterminator="\n"); w.writerow(["address","name","length","bytes"])
    for t in batch:
        ai=int(t["address"],16); b=body(ai)
        w.writerow([t["address"],t["name"],str(len(b)),b.hex()])
(SCR/(PREFIX+".json")).write_text(json.dumps(batch))
print(f"eligible pool: {len(cand)}  | {PREFIX}: {len(batch)}")
for t in batch:
    ai=int(t["address"],16)
    print(" ",t["address"],len(body(ai)),t["module"][:24],t["name"][:36])
