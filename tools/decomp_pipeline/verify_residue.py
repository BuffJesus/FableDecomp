#!/usr/bin/env python3
"""Verify re-authored byte-parity RESIDUES and overwrite the landed source on a win.

Unlike verify_and_land.py (which SKIPS already-cataloged addresses), this is for functions
that are ALREADY landed but compiled DIFFERent from retail. It compiles each authored candidate
under VC7.1 /O2 /Oy, compares to the retail oracle bytes, runs the behaviour test, and with
--land OVERWRITES the existing rebuild/src/compiled/*<addr>*.cpp (already in the catalog).

Usage: python verify_residue.py <workflow_output.json> <residue_oracle.tsv> [--land]
"""
import csv, json, re, subprocess, os, html, sys, glob
from pathlib import Path

ROOT = Path(r"D:\Documents\FableTLC")
VC = Path(r"D:\Tools\vc71")
WORK = Path(os.environ.get("RESIDUE_WORK", ROOT / "rebuild" / "_residue_verify"))
WORK.mkdir(parents=True, exist_ok=True)
OBJDUMP = r"C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\objdump.exe"
DS = re.compile(r"^\s*[0-9a-fA-F]+\s+<(.+)>:$"); DB = re.compile(r"^\s*[0-9a-fA-F]+:\s+((?:[0-9a-fA-F]{2}\s+)+)")
RL = re.compile(r"^([0-9a-fA-F]{8})\s+\S+\s+.+$")
PRAGMAS = ["", '#pragma optimize("s",on)', '#pragma optimize("t",on)', '#pragma optimize("g",on)']

def vc71(s):
    s = html.unescape(s).replace("__thiscall", "__fastcall")
    return "\n".join(l for l in s.splitlines() if "static_assert" not in l)

def obj_text(path, leaf):
    pr = subprocess.run([OBJDUMP, "-d", str(path)], capture_output=True, text=True, check=True)
    fns, sec, cur = [], -1, None
    for line in pr.stdout.splitlines():
        if line.startswith("Disassembly of section .text:"): sec += 1; cur = None; continue
        s = DS.match(line)
        if s: cur = {"symbol": s.group(1), "section": sec, "bytes": bytearray()}; fns.append(cur); continue
        e = DB.match(line)
        if e and cur is not None: cur["bytes"].extend(bytes.fromhex(e.group(1)))
    if not fns: raise RuntimeError("no funcs")
    named = [f for f in fns if leaf and leaf in str(f["symbol"])]
    sel = max(named or fns, key=lambda f: len(f["bytes"]))
    return bytes(sel["bytes"]), int(sel["section"])

def obj_relocs(path, sel):
    hp = subprocess.run([OBJDUMP, "-h", str(path)], capture_output=True, text=True, check=True)
    flags = []; hl = hp.stdout.splitlines()
    for i, l in enumerate(hl):
        if re.match(r"^\s*\d+\s+\.text\s+", l): flags.append("RELOC" in (hl[i + 1] if i + 1 < len(hl) else ""))
    if sel >= len(flags) or not flags[sel]: return []
    bi = sum(flags[:sel + 1]) - 1
    rp = subprocess.run([OBJDUMP, "-r", str(path)], capture_output=True, text=True, check=True)
    blocks = []; intext = False; ts = -1
    for l in rp.stdout.splitlines():
        if l.startswith("RELOCATION RECORDS FOR ["):
            intext = "[.text]" in l
            if intext: ts += 1; blocks.append([])
            continue
        if intext:
            m = RL.match(l.strip())
            if m: blocks[ts].append(int(m.group(1), 16))
    return blocks[bi] if bi < len(blocks) else []

def mask(p, offs):
    r = bytearray(p)
    for o in offs:
        for i in range(o, min(o + 4, len(r))): r[i] = 0
    return bytes(r)

def env():
    e = dict(os.environ); e["PATH"] = str(VC / "bin") + ";" + e["PATH"]
    e["INCLUDE"] = f"{VC/'include'};{ROOT/'rebuild'/'include'}"; e["LIB"] = str(VC / "lib"); return e
E = env()

def cl(args): return subprocess.run([str(VC/"bin"/"cl.exe")] + args, capture_output=True, text=True, env=E)

def parity(srctext, addr, leaf, retail):
    sp = WORK / f"{addr}.cpp"; sp.write_text(srctext, encoding="utf-8")
    obj = WORK / f"{addr}.obj"
    if obj.exists(): obj.unlink()
    cp = cl(["/nologo", "/c", "/O2", "/Oy", "/W3", f"/Fo{obj}", str(sp)])
    if cp.returncode or not obj.exists(): return "SRC_FAIL", None
    try: built, sec = obj_text(obj, leaf)
    except Exception: return "OBJDUMP_ERR", None
    rel = obj_relocs(obj, sec)
    if retail == built: return "MATCH", built
    if mask(retail, rel) == mask(built, rel): return "RELOCATION_MATCH", built
    return f"DIFFER({len(built)}v{len(retail)})", built

def behaviour(tsttext, addr, patt):
    tp = WORK / f"{addr}.test.cpp"; tp.write_text(tsttext, encoding="utf-8")
    tobj = WORK / f"{addr}.t.obj"; exe = WORK / f"{addr}.exe"
    for p in (tobj, exe):
        if p.exists(): p.unlink()
    tc = cl(["/nologo", "/c", "/Od", "/W3", f"/Fo{tobj}", str(tp)])
    if tc.returncode or not tobj.exists(): return "TCC_FAIL"
    lk = subprocess.run([str(VC/"bin"/"link.exe"), "/nologo", f"/out:{exe}", str(tobj)], capture_output=True, text=True, env=E)
    if lk.returncode or not exe.exists(): return "LINK_FAIL"
    rn = subprocess.run([str(exe)], capture_output=True, text=True, env=E)
    return "PASS" if (rn.returncode == 0 and re.search(re.escape(patt), rn.stdout or "")) else "FAIL"

def main():
    outf = Path(sys.argv[1]); oraclef = Path(sys.argv[2]); land = "--land" in sys.argv
    data = json.loads(outf.read_text(encoding="utf-8"))["result"]["authored"]
    orc = {r["address"].lower(): r for r in csv.DictReader(open(oraclef, encoding="utf-8-sig"), delimiter="\t")}
    wins = []
    print(f"{'addr':10} {'parity':18} {'behav':6} name")
    for c in data:
        addr = c["address"].lower().replace("0x", "")
        name = c["name"]; leaf = name.rsplit("::", 1)[-1].split("@")[1] if name.startswith("?") else name.rsplit("::", 1)[-1]
        o = orc.get(addr)
        if not o:
            print(f"{addr:10} {'NO_ORACLE':18} {'-':6} {name}"); continue
        retail = bytes.fromhex(o["bytes"])
        src0 = vc71(c["source_cpp"]); tst = vc71(c["test_cpp"]); patt = c["pass_pattern"]
        src = src0; st, _ = parity(src, addr, leaf, retail)
        if not st.startswith(("MATCH", "RELOCATION")):
            for P in PRAGMAS[1:]:
                cand = P + "\n" + src0
                st2, _ = parity(cand, addr, leaf, retail)
                if st2.startswith(("MATCH", "RELOCATION")):
                    src = cand; st = st2 + f"[{P.split('(')[1].split(',')[0]}]"; break
        beh = behaviour(tst, addr, patt)
        print(f"{addr:10} {st:18} {beh:6} {name}")
        if st.startswith(("MATCH", "RELOCATION_MATCH")) and beh == "PASS":
            landed = glob.glob(
                str(ROOT / f"rebuild/src/compiled/**/*{addr}*.cpp"),
                recursive=True,
            )
            wins.append({"addr": addr, "name": name, "status": st, "src": src, "landed": landed[0] if landed else None})
    print(f"\nWINS: {len(wins)}")
    for w in wins: print(f"  {w['addr']} {w['status']} -> {w['landed']}")
    if land:
        for w in wins:
            if w["landed"]:
                Path(w["landed"]).write_text(w["src"], encoding="utf-8")
                print(f"  OVERWROTE {w['landed']}")

if __name__ == "__main__":
    main()
