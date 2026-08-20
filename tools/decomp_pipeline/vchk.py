#!/usr/bin/env python3
"""Standalone byte-parity check for a hand-authored source vs a retail oracle-hex.
Tries a flag x pragma matrix; masks call rel32 (e8) so declared-not-defined helpers
and self-recursion match. Usage: python vchk.py <src.cpp> <leaf_symbol_substr> <oracle_hex>
Prints the first MATCHing (flags,pragma) or the closest DIFFER."""
import sys, subprocess, re
from pathlib import Path
VC = Path(r"D:\Tools\vc71")
OBJD = r"C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\objdump.exe"
src_path, leaf, oracle = sys.argv[1], sys.argv[2], sys.argv[3].lower()
base_src = Path(src_path).read_text(encoding="utf-8")
def mask(h):
    b = bytearray.fromhex(h); i = 0
    while i < len(b):
        if b[i] == 0xe8 and i+5 <= len(b): b[i+1:i+5] = b'\0\0\0\0'; i += 5
        else: i += 1
    return b.hex()
retail = mask(oracle)
env = dict(__import__("os").environ); env["PATH"] = str(VC/"bin") + ";" + env["PATH"]
FLAGS = ["/O2 /Oy", "/Ox /Oy", "/O1 /Oy"]
PRAGS = ["", 'g', 's', 't', 'gs', 'gt', 'ga', 'gsy']
work = Path(src_path).parent
best = None
for fl in FLAGS:
    for pr in PRAGS:
        pragma = f'#pragma optimize("{pr}",on)\n' if pr else ""
        cpp = work / "_vchk.cpp"; cpp.write_text(pragma + base_src, encoding="utf-8")
        obj = work / "_vchk.obj"; obj.unlink(missing_ok=True)
        cp = subprocess.run([str(VC/"bin"/"cl.exe"), "/nologo", "/c"] + fl.split() + ["/W3", f"/Fo{obj}", str(cpp)],
                            capture_output=True, text=True, env=env)
        if cp.returncode or not obj.exists(): continue
        dd = subprocess.run([OBJD, "-d", str(obj)], capture_output=True, text=True).stdout
        # collect bytes of the function whose symbol contains leaf
        lines = dd.splitlines(); cur = None; got = None
        for ln in lines:
            m = re.match(r"^[0-9a-f]+ <(.+)>:$", ln.strip())
            if m:
                if got is not None: break
                cur = m.group(1)
                if leaf in cur: got = bytearray()
                continue
            if got is not None:
                mm = re.match(r"^\s*[0-9a-f]+:\s+((?:[0-9a-f]{2} )+)", ln)
                if mm: got += bytearray.fromhex(mm.group(1).replace(" ", ""))
        if got is None: continue
        h = mask(got.hex())
        if h == retail:
            print(f"MATCH  flags=[{fl}] pragma=[{pr}]  len={len(got)}")
            sys.exit(0)
        d = sum(1 for a,b in zip(h, retail) if a!=b) + abs(len(got)-len(oracle)//2)
        if best is None or (len(got), d) < best[0]:
            best = ((len(got), d), fl, pr, got.hex())
if best:
    (_, fl, pr, gh) = best
    print(f"DIFFER best flags=[{fl}] pragma=[{pr}] len={best[0][0]}v{len(oracle)//2}")
    print(f"  mine:   {gh}")
    print(f"  retail: {oracle}")
else:
    print("NO_BUILD")
