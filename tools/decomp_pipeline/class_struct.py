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

def parse_struct(body):
    """Yield (offset, type, name, size) for named non-pad members; return (fields, total_size)."""
    off = 0
    fields = []
    for m in MEMBER_RE.finditer(body):
        typ, name, _, arr = m.group(1).strip(), m.group(2), None, m.group(4)
        if typ in ("struct", "class", "return", "void") and not arr:
            continue
        if arr is not None:
            n = int(arr, 0)
            elt = type_size(typ)
            span = n * elt if elt else n
            if not is_pad(name):
                fields.append((off, typ, name, span))
            off += span
        else:
            sz = type_size(typ)
            if not is_pad(name):
                fields.append((off, typ, name, sz))
            off += sz
    return fields, off

def facts_for(cls):
    """Merge all verified facts for a class -> (offset->(type,name), class_size, n_files)."""
    files = [
        p
        for p in glob.glob(str(SRC / "**/*.cpp"), recursive=True)
        if os.path.basename(p).startswith(cls + "_")
    ]
    off_map = {}
    size = None
    for f in files:
        txt = open(f, encoding="utf-8", errors="ignore").read()
        base = os.path.basename(f)
        # class size from GetSizeofClass constant-return (only when the FILE is that method)
        if "GetSizeofClass" in base:
            mm = SIZEOFCLASS_RE.search(txt)
            if mm:
                size = max(size or 0, int(mm.group(1), 0))
        # merge fields from the struct whose name matches (or the first struct)
        structs = STRUCT_RE.findall(txt)
        chosen = None
        for sname, body in structs:
            if sname == cls:
                chosen = body; break
        if chosen is None and structs:
            # a same-class file with a differently-named local struct still models this class
            chosen = structs[0][1]
        if chosen is None:
            continue
        fields, total = parse_struct(chosen)
        for off, typ, name, sz in fields:
            if off not in off_map or off_map[off][0] in ("int", "unsigned", "unsigned long", "long"):
                off_map[off] = (typ, name)
        # NOTE: do NOT infer class size from an all-pad struct total -- agents use
        # `char pad[N]` as a placeholder for the `this` param, not the real class size.
        # Only GetSizeofClass returns (above) are trusted for size.
    return off_map, size, len(files)

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
