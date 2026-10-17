"""Type Fable.exe's globals from a running game via MSVC RTTI (read-only, no debugger).

For every dword in the image's .data/.bss:
  - value is a vtable in the image whose [vtable-4] is a valid complete-object locator (offset 0)
    -> the address is a STATIC INSTANCE of that class;
  - value points at a heap/static object whose first dword is such a vtable
    -> the address is a POINTER GLOBAL of that class (at sampling time; may be 0 in other states).
Writes ghidra_out/rtti_live_globals.tsv (kind, address, class, vtable, state label).
Usage: python tools/modern/rtti_live_globals.py <pid> [--label frontend]
"""
from __future__ import annotations

import argparse
import ctypes
import struct
from ctypes import wintypes as W
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "ghidra_out" / "rtti_live_globals.tsv"
IMAGE = (0x400000, 0x400000 + 17219584)
DATA = (0x1374000, 0x143E9A4)  # .data incl. .bss (retail section table)

k = ctypes.WinDLL("kernel32", use_last_error=True)
k.OpenProcess.restype = ctypes.c_void_p


def demangle(td: str) -> str:
    # ".?AVCFoo@@" -> "CFoo"; nested/template names keep their mangled tail
    s = td[4:] if td.startswith(".?A") else td
    return s[:-2] if s.endswith("@@") and "@" not in s[:-2] else s


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("pid", type=int)
    ap.add_argument("--label", default="frontend")
    args = ap.parse_args()
    h = ctypes.c_void_p(k.OpenProcess(0x0410, False, args.pid))

    def rd(a, n):
        b = ctypes.create_string_buffer(n)
        g = ctypes.c_size_t()
        return b.raw[:g.value] if k.ReadProcessMemory(h, ctypes.c_void_p(a), b, n, ctypes.byref(g)) else None

    def u32(a):
        b = rd(a, 4)
        return struct.unpack("<I", b)[0] if b and len(b) == 4 else None

    vt_cache: dict[int, str | None] = {}

    def vt_class(vt: int) -> str | None:
        if vt in vt_cache:
            return vt_cache[vt]
        name = None
        if IMAGE[0] <= vt < IMAGE[1]:
            col = u32(vt - 4)
            if col and IMAGE[0] <= col < IMAGE[1]:
                hdr = rd(col, 16)
                if hdr and len(hdr) == 16:
                    sig, off, _cd, td = struct.unpack("<IIII", hdr)
                    if sig == 0 and off == 0 and IMAGE[0] <= td < IMAGE[1]:
                        raw = rd(td + 8, 128) or b""
                        s = raw.split(b"\0")[0].decode("latin1")
                        if s.startswith(".?AV") or s.startswith(".?AU"):
                            name = demangle(s)
        vt_cache[vt] = name
        return name

    data = rd(DATA[0], DATA[1] - DATA[0])
    rows = []
    for i in range(0, len(data) - 3, 4):
        a = DATA[0] + i
        v = struct.unpack_from("<I", data, i)[0]
        c = vt_class(v)
        if c:
            rows.append(("static", a, c, v))
            continue
        if v >= 0x10000 and not (IMAGE[0] <= v < IMAGE[1]) or DATA[0] <= v < DATA[1]:
            pv = u32(v)
            if pv is not None:
                c = vt_class(pv)
                if c:
                    rows.append(("pointer", a, c, pv))
    with OUT.open("w", encoding="utf-8", newline="\n") as f:
        f.write("kind\taddress\tclass\tvtable\tstate\n")
        for kind, a, c, v in rows:
            f.write(f"{kind}\t{a:08X}\t{c}\t{v:08X}\t{args.label}\n")
    n = {kd: sum(1 for r in rows if r[0] == kd) for kd in ("static", "pointer")}
    print(f"{n} rows -> {OUT}")


if __name__ == "__main__":
    main()
