#!/usr/bin/env python3
"""Aggregate byte-verified accessors into a reconstructed struct per class.

Every landed src/compiled/<Class>_*.cpp models a piece of its class: field
accessors pin (offset,type) via their local struct decl; GetSizeofClass pins
the total class size. This merges all of a class's verified facts into one
reconstructed struct header, so authoring the class's *harder* methods no
longer re-derives offsets from scratch.

Usage:
  python class_struct.py <Class>            # print reconstructed struct
  python class_struct.py --map <out.json>   # {module: struct_text} for all classes
"""
import glob, os, re, json, sys
from pathlib import Path

ROOT = Path(r"D:\Documents\FableTLC")
SRC = ROOT / "rebuild/src/compiled"
SIZEOF = {"bool": 1, "char": 1, "unsigned char": 1, "signed char": 1, "u8": 1,
          "short": 2, "unsigned short": 2, "wchar_t": 2,
          "int": 4, "unsigned": 4, "unsigned int": 4, "long": 4, "unsigned long": 4,
          "u32": 4, "ulong": 4, "float": 4, "dword": 4, "DWORD": 4,
          "double": 8, "__int64": 8, "unsigned __int64": 8}

STRUCT_RE = re.compile(r"struct\s+([A-Za-z_]\w*)\s*\{(.*?)\}\s*;", re.DOTALL)
MEMBER_RE = re.compile(r"([A-Za-z_][\w:<>\* ]*?)\s+(\w+)\s*(\[\s*(0x[0-9a-fA-F]+|\d+)\s*\])?\s*;")
SIZEOFCLASS_RE = re.compile(r"return\s+(0x[0-9a-fA-F]+|\d+)\s*;")

def type_size(t):
    t = t.strip()
    if t.endswith("*") or "*" in t:
        return 4
    return SIZEOF.get(t, SIZEOF.get(t.replace("const ", "").strip(), 4))

def is_pad(name):
    n = name.lower()
    return n.startswith("pad") or n.startswith("_pad") or n in ("dummy", "_dummy", "vtbl", "_vfptr", "_vfp", "vfptr")

PACK1_RE = re.compile(r"#pragma\s+pack\s*\(\s*push\s*,\s*1\s*\)|#pragma\s+pack\s*\(\s*1\s*\)")

def is_packed(text, struct_start):
    """True when a `#pragma pack(push,1)` / `pack(1)` is open at struct_start."""
    opens = [m.start() for m in PACK1_RE.finditer(text) if m.start() < struct_start]
    if not opens:
        return False
    last_open = opens[-1]
    pops = [m.start() for m in re.finditer(r"#pragma\s+pack\s*\(\s*pop\s*\)", text)
            if last_open < m.start() < struct_start]
    return not pops

def type_align(t):
    return min(type_size(t), 4)

def parse_struct(body, packed=True):
    """Return (fields, total_size) with fields = [(offset, type, name, size)] for named,
    non-pad members.  `packed=False` applies VC7.1 natural alignment (align = min(size,4))
    to member offsets and the total; the previous behaviour assumed pack(1) everywhere,
    which mis-offsets every un-packed local struct with mixed member widths."""
    off = 0
    fields = []
    max_align = 1
    for m in MEMBER_RE.finditer(body):
        typ, name, _, arr = m.group(1).strip(), m.group(2), None, m.group(4)
        if typ in ("struct", "class", "return", "void") and not arr:
            continue
        if typ.startswith(("virtual", "static", "typedef")):
            continue
        elt = type_size(typ)
        al = 1 if packed else max(1, type_align(typ))
        if not packed and off % al:
            off += al - (off % al)
        max_align = max(max_align, al)
        if arr is not None:
            n = int(arr, 0)
            span = n * elt if elt else n
        else:
            span = elt
        if not is_pad(name):
            fields.append((off, typ, name, span))
        off += span
    if not packed and off % max_align:
        off += max_align - (off % max_align)
    return fields, off

def landed_files_for(cls):
    return [
        p
        for p in glob.glob(str(SRC / "**/*.cpp"), recursive=True)
        if os.path.basename(p).startswith(cls + "_")
    ]

def facts_for(cls, files=None, trust=None, provenance=None):
    """Merge all verified facts for a class -> (offset->(type,name), class_size, n_files).

    files: explicit list of landed .cpp paths (default: every `<cls>_*.cpp`).
    trust: optional predicate(path) -> bool; files failing it are skipped (use
           label_trust to drop mislabelled families / non-genuine bakes).
    provenance: optional dict filled with offset -> list of (type, name, size, file).
    Files whose struct uses natural alignment and whose packed/natural layouts differ are
    skipped as ambiguous (AMBIG_ALIGN) rather than guessed.
    """
    if files is None:
        files = landed_files_for(cls)
    off_map = {}
    size = None
    used = 0
    for f in files:
        if trust is not None and not trust(f):
            continue
        txt = open(f, encoding="utf-8", errors="ignore").read()
        base = os.path.basename(f)
        if "GetSizeofClass" in base:
            mm = SIZEOFCLASS_RE.search(txt)
            if mm:
                size = max(size or 0, int(mm.group(1), 0))
        chosen = None
        chosen_start = 0
        for m in STRUCT_RE.finditer(txt):
            if m.group(1) == cls:
                chosen, chosen_start = m.group(2), m.start(); break
        if chosen is None:
            m = STRUCT_RE.search(txt)
            if m:
                chosen, chosen_start = m.group(2), m.start()
        if chosen is None:
            continue
        packed = is_packed(txt, chosen_start)
        fields, total = parse_struct(chosen, packed=True)
        if not packed:
            nat_fields, nat_total = parse_struct(chosen, packed=False)
            if nat_fields != fields:
                # natural alignment moved something: only trust it if the struct is
                # homogeneous in width (then both agree); otherwise skip as ambiguous.
                continue
        used += 1
        for off, typ, name, sz in fields:
            if provenance is not None:
                provenance.setdefault(off, []).append((typ, name, sz, base))
            if off not in off_map or off_map[off][0] in ("int", "unsigned", "unsigned long", "long"):
                off_map[off] = (typ, name)
    return off_map, size, used

def render(cls):
    off_map, size, n = facts_for(cls)
    if not off_map and not size:
        return None
    lines = [f"// Reconstructed from {n} byte-verified {cls} method(s). Offsets are FACTS (byte-matched)."]
    lines.append(f"struct {cls} {{")
    cur = 0
    for off in sorted(off_map):
        if off > cur:
            lines.append(f"    unsigned char _pad_{cur:#x}[{off - cur:#x}]; // to {off:#x}")
        typ, name = off_map[off]
        lines.append(f"    {typ} {name}; // +{off:#x}")
        cur = off + type_size(typ)
    consistent = size and size >= cur  # drop sizes contradicted by observed fields
    if consistent and size > cur:
        lines.append(f"    unsigned char _tail[{size - cur:#x}]; // class size {size:#x}")
    lines.append("};")
    if consistent:
        lines.append(f"// sizeof({cls}) == {size:#x}")
    return "\n".join(lines)

def main():
    if sys.argv[1] == "--map":
        out = Path(sys.argv[2])
        classes = set()
        for p in glob.glob(str(SRC / "**/*.cpp"), recursive=True):
            b = os.path.basename(p)
            if "_" in b:
                classes.add(b.split("_")[0])
        m = {}
        for c in sorted(classes):
            off_map, size, n = facts_for(c)
            if (len(off_map) >= 1 or size) and n >= 2:
                r = render(c)
                if r:
                    m[c] = r
        json.dump(m, open(out, "w"))
        print(f"[class_struct] {len(m)} classes with reconstructed structs -> {out}", file=sys.stderr)
    else:
        r = render(sys.argv[1])
        print(r if r else f"(no verified facts for {sys.argv[1]})")

if __name__ == "__main__":
    main()
