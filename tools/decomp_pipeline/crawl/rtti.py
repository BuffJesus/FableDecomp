#!/usr/bin/env python3
"""MSVC RTTI / vtable enumeration over the retail image.

Fable.exe is a fixed-base PE with no `.reloc` directory, so there is no relocation table to
confirm which dwords are addresses. MSVC RTTI gives a stronger, structural source of truth:

  vftable[-1]  -> RTTICompleteObjectLocator {
                    DWORD signature;            // 0 for 32-bit
                    DWORD offset;               // this-adjust of the subobject
                    DWORD cdOffset;
                    TypeDescriptor*  pTypeDescriptor;
                    ClassHierarchyDescriptor* pClassDescriptor;
                  }
  TypeDescriptor { void* pVFTable; void* spare; char name[]; }   // ".?AVCFoo@@"

Everything from `vftable[0]` onwards is a *virtual function pointer* until the run of
.text-pointing dwords ends. Those are function starts by construction -- much stronger
evidence than "a dword somewhere happens to look like a code address" -- and each one comes
with the owning class name, which is what lets a discovered function be named instead of
left as `sub_<addr>`.

Outputs (with --write): rebuild/manifest/rtti-vtables.tsv
    class  vtable_va  slot  func_va

API: vtable_slots() -> {func_va: (class_name, vtable_va, slot_index)}
"""
import csv
import struct
import sys
from pathlib import Path

EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
ROOT = Path(r"D:\Documents\FableTLC")
IMAGE_BASE = 0x400000
OUT = ROOT / "rebuild/manifest/rtti-vtables.tsv"

_cache = None


def _load():
    data = EXE.read_bytes()
    e = struct.unpack_from("<I", data, 0x3C)[0]
    coff = e + 4
    nsec = struct.unpack_from("<H", data, coff + 2)[0]
    optsz = struct.unpack_from("<H", data, coff + 16)[0]
    sb = coff + 20 + optsz
    secs = []
    for i in range(nsec):
        o = sb + i * 40
        name = data[o:o + 8].rstrip(b"\0").decode("latin1")
        vs, va, rs, rp = struct.unpack_from("<IIII", data, o + 8)
        secs.append((name, va, max(vs, rs), rp, rs))
    return data, secs


def _mk_off(secs):
    def off(va):
        rva = va - IMAGE_BASE
        for name, sva, vsz, rp, rs in secs:
            if sva <= rva < sva + vsz:
                d = rp + (rva - sva)
                # only file-backed bytes are readable
                if rva - sva < rs:
                    return d
        return None
    return off


def _range(secs, want):
    for name, sva, vsz, rp, rs in secs:
        if name == want:
            return IMAGE_BASE + sva, IMAGE_BASE + sva + vsz
    return 0, 0


def _cstr(data, o, limit=512):
    end = data.find(b"\0", o, o + limit)
    if end < 0:
        return ""
    return data[o:end].decode("latin1", "replace")


def build():
    global _cache
    if _cache is not None:
        return _cache
    data, secs = _load()
    off = _mk_off(secs)
    tlo, thi = _range(secs, ".text")
    rlo, rhi = _range(secs, ".rdata")
    dlo, dhi = _range(secs, ".data")

    def readable_ptr(va):
        return off(va) is not None

    # 1) find TypeDescriptors: pVFTable, spare(0), then a ".?A" name
    typedesc = {}
    for name, sva, vsz, rp, rs in secs:
        if name not in (".data", ".rdata"):
            continue
        base_va = IMAGE_BASE + sva
        blob = data[rp:rp + rs]
        pos = 0
        while True:
            pos = blob.find(b".?A", pos)
            if pos < 0:
                break
            # the name starts at td+8
            td_va = base_va + pos - 8
            if off(td_va) is not None:
                nm = _cstr(data, rp + pos)
                if nm.startswith(".?A"):
                    typedesc[td_va] = nm
            pos += 3

    # 2) find CompleteObjectLocators: sig==0, pTypeDescriptor in typedesc
    cols = {}
    for name, sva, vsz, rp, rs in secs:
        if name not in (".data", ".rdata"):
            continue
        base_va = IMAGE_BASE + sva
        blob = data[rp:rp + rs]
        n = len(blob) - 20
        o = 0
        while o < n:
            sig, offs, cdoff, ptd, pcd = struct.unpack_from("<IIIII", blob, o)
            if sig == 0 and ptd in typedesc and readable_ptr(pcd):
                cols[base_va + o] = typedesc[ptd]
            o += 4

    # 3) a vtable is a dword pointing at a COL, followed by .text pointers
    slots = {}
    vtables = []
    for name, sva, vsz, rp, rs in secs:
        if name not in (".data", ".rdata"):
            continue
        base_va = IMAGE_BASE + sva
        blob = data[rp:rp + rs]
        n = len(blob) - 4
        o = 0
        while o < n:
            v, = struct.unpack_from("<I", blob, o)
            if v in cols:
                cls = cols[v]
                vt_va = base_va + o + 4
                k = o + 4
                idx = 0
                while k + 4 <= len(blob):
                    fn, = struct.unpack_from("<I", blob, k)
                    if not (tlo <= fn < thi):
                        break
                    slots.setdefault(fn, (cls, vt_va, idx))
                    idx += 1
                    k += 4
                if idx:
                    vtables.append((cls, vt_va, idx))
                o = k
                continue
            o += 4

    _cache = (slots, vtables, cols, typedesc)
    return _cache


def vtable_slots():
    return build()[0]


def demangle_class(nm):
    """MSVC type-descriptor name -> a readable class name.

    '.?AVCFoo@@'        -> 'CFoo'
    '.?AVCBar@NS@@'     -> 'NS::CBar'
    '.?AV?$CBaz@...@@'  -> 'CBaz'   (template: keep the base name, drop the argument
                                     soup -- reversing scopes through template args
                                     produces nonsense like '_N::?$CBaz')
    """
    s = nm
    if s.startswith(".?A"):
        s = s[3:]
    if s[:1] in ("V", "U", "W"):        # class / struct / enum
        s = s[1:]
    parts = [p for p in s.split("@") if p]
    for p in parts:
        if p.startswith("?$"):
            return p[2:]
    return "::".join(reversed(parts)) if parts else nm


if __name__ == "__main__":
    slots, vtables, cols, typedesc = build()
    print("TypeDescriptors      : %d" % len(typedesc))
    print("CompleteObjectLocators: %d" % len(cols))
    print("vtables              : %d" % len(vtables))
    print("virtual function slots: %d (unique targets)" % len(slots))
    if "--write" in sys.argv:
        with open(OUT, "w", encoding="utf-8", newline="") as f:
            w = csv.writer(f, delimiter="\t", lineterminator="\n")
            w.writerow(["class", "vtable_va", "slot", "func_va"])
            for fn, (cls, vt, idx) in sorted(slots.items()):
                w.writerow([demangle_class(cls), "%08x" % vt, idx, "%08x" % fn])
        print("wrote %s" % OUT)
