#!/usr/bin/env python3
"""
fse_manifest_to_ghidra.py — turn FableScriptExtender's fse_api_manifest.json into:
  1) fse_api.h        — a C header (opaque structs + function prototypes) to feed into
                        Ghidra via  File -> Parse C Source  -> Apply Function Datatypes.
  2) fse_api_index.md — a category-sorted catalog of the reversed API surface, used as
                        the reverse-engineering roadmap (each entry = one engine function
                        FSE already located/typed, i.e. a known target to name in Ghidra).

The manifest is FSE's map of the game's internal C++ API (CScriptThing / CWorld /
CThingManager / CHero, etc.). It is the single biggest head-start for decompiling
Fable.exe: instead of blank sub_XXXXXX everywhere, we seed Ghidra with real names,
return types and parameter lists, then use xrefs/strings to pin each to its address.

Usage:
    python fse_manifest_to_ghidra.py <path-to-fse_api_manifest.json> [out_dir]
Defaults: manifest = ../../refs/fse_api_manifest.json ; out_dir = .
"""
import json, sys, os, re
from collections import defaultdict

HERE = os.path.dirname(os.path.abspath(__file__))
manifest = sys.argv[1] if len(sys.argv) > 1 else os.path.join(HERE, "..", "..", "refs", "fse_api_manifest.json")
out_dir  = sys.argv[2] if len(sys.argv) > 2 else HERE

with open(manifest, "r", encoding="utf-8-sig") as f:
    data = json.load(f)

funcs = data.get("functions", [])

# --- collect the opaque engine types referenced anywhere in the API -----------------
type_re = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
def base_types(t):
    # strip const / std wrappers / pointers / refs, yield bare identifiers that look like engine types
    for m in type_re.findall(t):
        if m[0] == "C" and m[1:2].isupper():   # CScriptThing, CWorld, CHero, ...
            yield m
        if m.startswith("E") and m[1:2].isupper():  # enums E...
            yield m

engine_types = set()
for fn in funcs:
    for p in fn.get("parameters", []):
        engine_types.update(base_types(p.get("type", "")))
    engine_types.update(base_types(fn.get("returnType", "")))

# --- emit a C header Ghidra can parse ------------------------------------------------
def c_type(t):
    # Ghidra's C parser is picky; degrade C++ template/ref types to void* so parse never fails.
    t = t.strip()
    if "std::" in t or "<" in t or "&" in t:
        return "void *"
    return t

lines = ["// Auto-generated from fse_api_manifest.json — feed to Ghidra: File > Parse C Source.",
         "// Opaque forward decls for engine types (real layouts get filled in as RE progresses).", ""]
for t in sorted(engine_types):
    lines.append(f"typedef struct {t} {t};")
lines.append("")
lines.append("// --- API prototypes (FSE-reversed engine surface) ---")
for fn in funcs:
    name = fn.get("name")
    if not name:
        continue
    ret = c_type(fn.get("returnType", "void"))
    params = fn.get("parameters", [])
    ps = ", ".join(f"{c_type(p.get('type','void*'))} {p.get('name','a')}" for p in params) or "void"
    lines.append(f"{ret} {name}({ps});")
with open(os.path.join(out_dir, "fse_api.h"), "w", encoding="utf-8") as f:
    f.write("\n".join(lines))

# --- emit a category-sorted RE roadmap ----------------------------------------------
by_cat = defaultdict(list)
for fn in funcs:
    by_cat[fn.get("category", "Uncategorized")].append(fn)

md = ["# FSE-reversed API surface (RE roadmap)",
      "",
      f"Source: `fse_api_manifest.json` — **{len(funcs)} functions** across **{len(by_cat)} categories**, "
      f"referencing **{len(engine_types)} engine types**.",
      "",
      "Each row is a function FableScriptExtender already reverse-engineered well enough to call. "
      "For decompilation, each is a *named target*: find its address in Ghidra (via strings/xrefs "
      "or the FSE DLL's call sites), apply the name + signature, then decompile outward from it.",
      ""]
for cat in sorted(by_cat):
    md.append(f"## {cat}  ({len(by_cat[cat])})")
    md.append("")
    for fn in sorted(by_cat[cat], key=lambda x: x.get("name","")):
        scope = fn.get("scope", "")
        block = " [blocking]" if fn.get("blocking") else ""
        md.append(f"- `{fn.get('returnType','void')} {fn.get('name')}(...)` — {scope}{block}: "
                  f"{fn.get('description','').strip()}")
    md.append("")
with open(os.path.join(out_dir, "fse_api_index.md"), "w", encoding="utf-8") as f:
    f.write("\n".join(md))

print(f"functions: {len(funcs)}   categories: {len(by_cat)}   engine types: {len(engine_types)}")
print("wrote fse_api.h and fse_api_index.md to", out_dir)
