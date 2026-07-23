#!/usr/bin/env python3
"""
parse_pdb_xml.py — extract (absolute_addr, mangled_name) from a Ghidra pdb.exe XML dump.
The <function address="0xRVA"> values are RVAs; absolute = image_base + RVA.
Writes a rename plan (addr_hex \t name) for ApplyNames.java, deduped by address.

Usage: parse_pdb_xml.py <pdb.xml> <out_plan.tsv> [image_base_hex=0x400000]
"""
import sys, re

xml, out = sys.argv[1], sys.argv[2]
base = int(sys.argv[3], 16) if len(sys.argv) > 3 else 0x400000

FUNC = re.compile(r'<function name="(.*?)" address="0x([0-9a-fA-F]+)"')
seen = {}
rvas = []
n = 0
with open(xml, "r", encoding="utf-8", errors="replace") as f:
    for line in f:
        m = FUNC.search(line)
        if not m:
            continue
        name, rva = m.group(1), int(m.group(2), 16)
        n += 1
        rvas.append(rva)
        addr = base + rva
        # keep first name per address; prefer proper mangled (?...) over `dynamic initializer`
        if addr in seen:
            if not seen[addr].startswith("?") and name.startswith("?"):
                seen[addr] = name
            continue
        seen[addr] = name

# unescape common XML entities in names
def unesc(s):
    return (s.replace("&amp;", "&").replace("&lt;", "<").replace("&gt;", ">")
             .replace("&quot;", '"').replace("&apos;", "'"))

with open(out, "w", encoding="utf-8") as o:
    for addr in sorted(seen):
        o.write(f"{addr:08x}\t{unesc(seen[addr])}\n")

print(f"function elements={n}  unique addresses={len(seen)}")
if rvas:
    print(f"RVA range: 0x{min(rvas):x} .. 0x{max(rvas):x}  "
          f"(abs 0x{base+min(rvas):x} .. 0x{base+max(rvas):x})")
print(f"plan -> {out}")
