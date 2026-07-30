#!/usr/bin/env python3
"""Extract retail function bytes directly from Fable.exe (no Ghidra).
Length = next-manifest-function-address - addr, minus trailing 0xCC/0x90 padding.
Validates against the 94 known oracle rows before trusting."""
import csv, struct, sys
from pathlib import Path

ROOT = Path(r"D:\Documents\FableTLC")
EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
IMAGE_BASE = 0x400000

def read_tsv(p):
    with open(p, encoding="utf-8-sig", newline="") as f:
        return list(csv.DictReader(f, delimiter="\t"))

def pe_sections(data):
    e_lfanew = struct.unpack_from("<I", data, 0x3C)[0]
    assert data[e_lfanew:e_lfanew+4] == b"PE\0\0"
    coff = e_lfanew+4
    nsec = struct.unpack_from("<H", data, coff+2)[0]
    opt = struct.unpack_from("<H", data, coff+16)[0]
    sec = coff+20+opt
    secs=[]
    for i in range(nsec):
        off=sec+i*40
        name=data[off:off+8].rstrip(b"\0").decode("latin1")
        vsize,va,rawsize,rawptr=struct.unpack_from("<IIII", data, off+8)
        secs.append((name,va,vsize,rawptr,rawsize))
    return secs

def va_to_off(secs, va):
    rva = va - IMAGE_BASE
    for name,sva,vsize,rawptr,rawsize in secs:
        if sva <= rva < sva+max(vsize,rawsize):
            return rawptr + (rva - sva)
    return None

def main():
    data = EXE.read_bytes()
    secs = pe_sections(data)
    funcs = read_tsv(ROOT/"rebuild/manifest/functions.tsv")
    addrs = sorted(int(r["address"],16) for r in funcs if r.get("address"))
    nextof = {addrs[i]:addrs[i+1] for i in range(len(addrs)-1)}
    def body(va):
        off = va_to_off(secs, va)
        if off is None: return None
        nxt = nextof.get(va)
        if nxt is None: return None
        raw = data[off:off+(nxt-va)]
        # strip trailing int3/nop padding
        end=len(raw)
        while end>0 and raw[end-1] in (0xCC,0x90): end-=1
        raw=raw[:end]
        # Trim at the TRUE epilogue: the next manifest address can sit past the
        # real function when unlisted trailing thunks are packed in the gap. The
        # first terminator -- `ret` (c3 / c2 imm16) or a tail `jmp` (e9 rel32 /
        # eb rel8) -- followed by an int3 (0xCC) padding run that is itself
        # followed by more code marks this function's end; everything after is a
        # separate, unmanifested function. Validated on the retail oracle:
        # 4450 -> 4843 exact matches, zero regressions. (See c8c730-boundary-note.)
        L=len(raw); i=0
        while i<L:
            c=raw[i]
            rl=1 if c==0xc3 else (3 if (c==0xc2 and i+2<L) else 0)
            if rl==0:
                rl=5 if (c==0xe9 and i+4<L) else (2 if (c==0xeb and i+1<L) else 0)
            if rl and i+rl<L and raw[i+rl]==0xCC:
                j=i+rl
                while j<L and raw[j]==0xCC: j+=1
                if j<L: return raw[:i+rl]
            i+=1
        return raw
    # validate vs known oracle
    oracle={r["address"].lower():r for r in read_tsv(ROOT/"rebuild/oracles/auto-re-candidates.tsv")}
    ok=bad=0; mism=[]
    for a,r in oracle.items():
        try: va=int(a,16)
        except: continue
        exp=bytes.fromhex(r["bytes"])
        got=body(va)
        if got==exp: ok+=1
        else:
            bad+=1
            mism.append((a, r["name"], len(exp), len(got) if got else None))
    print(f"VALIDATION: {ok} match / {bad} mismatch out of {ok+bad} known oracle rows")
    for a,n,le,lg in mism[:15]:
        print(f"  MISMATCH {a} {n[:40]} explen={le} gotlen={lg}")
    if len(sys.argv)>1 and sys.argv[1]=="extract":
        # extract for addresses listed in argv[2] (comma-separated hex), write TSV to argv[3]
        want=[x.strip().lower().replace("0x","") for x in sys.argv[2].split(",")]
        out=[]
        for a in want:
            va=int(a,16); b=body(va)
            out.append({"address":a,"length":len(b) if b else 0,"bytes":b.hex() if b else ""})
        with open(sys.argv[3],"w",encoding="utf-8",newline="") as f:
            w=csv.DictWriter(f,delimiter="\t",fieldnames=["address","length","bytes"],lineterminator="\n")
            w.writeheader(); w.writerows(out)
        print(f"extracted {len(out)} -> {sys.argv[3]}")

if __name__=="__main__":
    main()
