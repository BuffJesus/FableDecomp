#!/usr/bin/env python3
"""Parse a PE's undocumented 'Rich' header -- the compiler/linker build fingerprint
Microsoft's link.exe stamps between the DOS stub and the PE header. Each entry is
(prodID, buildNumber, count): every MS tool (c1xx.dll frontend, c2.dll backend, masm,
cvtres, link) that contributed object files, with how many objects each produced.

For matching-decomp this is ground truth: it tells you EXACTLY which compiler build(s)
compiled the target, so you can reproduce byte-exact codegen with the same toolset.

Usage: python rich_header.py [path-to-exe]   (defaults to retail Fable.exe)
"""
import struct, sys
from pathlib import Path

DEFAULT = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")

# VC prodID -> tool (partial; the ones that appear in VC7.1-era binaries).
PRODID = {
    0x00: "Unknown", 0x01: "Import", 0x02: "Linker510", 0x06: "Cvtres",
    0x0f: "Linker", 0x15: "Linker", 0x19: "Masm", 0x5a: "Utc1310_LTCG_C",
    0x5c: "Utc1310_POGO_I_C", 0x5d: "Utc1310_C (QFE frontend)",
    0x5e: "Utc1310_C", 0x5f: "Utc1310_CPP", 0x60: "Utc1310_CPP (QFE frontend)",
    0x69: "Import", 0x6d: "Utc1300_C", 0x6e: "Utc1300_CPP",
}
# VC7.1 build numbers of interest.
BUILD_NOTE = {3077: "RTM", 4035: "QFE-4035 <==", 6030: "SP1", 3052: "beta2"}


def parse(data: bytes):
    ri = data.find(b"Rich")
    if ri < 0:
        return None
    key = struct.unpack_from("<I", data, ri + 4)[0]
    start = None
    i = ri - 4
    while i >= 0x40:
        if struct.unpack_from("<I", data, i)[0] ^ key == 0x536e6144:  # 'DanS'
            start = i
            break
        i -= 4
    if start is None:
        return None
    rows = []
    p = start + 16  # skip DanS + 3 pad dwords
    while p < ri:
        v = struct.unpack_from("<I", data, p)[0] ^ key
        cnt = struct.unpack_from("<I", data, p + 4)[0] ^ key
        rows.append(((v >> 16) & 0xFFFF, v & 0xFFFF, cnt))
        p += 8
    return {"start": start, "end": ri, "key": key, "entries": rows}


def main():
    exe = Path(sys.argv[1]) if len(sys.argv) > 1 else DEFAULT
    r = parse(exe.read_bytes())
    if not r:
        print("no Rich header"); return 1
    print(f"{exe.name}: Rich @ 0x{r['start']:x}-0x{r['end']:x} key=0x{r['key']:08x}")
    print(f"{'prodID':>7} {'build':>6} {'objs':>6}  tool / note")
    qfe = 0
    for prod, build, cnt in r["entries"]:
        note = PRODID.get(prod, "")
        bn = BUILD_NOTE.get(build, "")
        if build == 4035:
            qfe += cnt
        print(f"0x{prod:04x} {build:6d} {cnt:6d}  {note} {bn}".rstrip())
    print(f"\nTotal objects built with build 13.10.4035 (QFE): {qfe}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
