#!/usr/bin/env python3
"""Recompile every landed src/compiled/*.cpp under VC7.1 and compare its .text to
the retail oracle. Finds functions that landed (passed behavior) but do NOT
actually byte-match retail -- the Std_Move_Backward class of false byte-matches.

Retail was NOT built with one global optimization flag: some TUs omit the frame
pointer (/O2 /Oy) and some keep it (/O2 /Oy-). A parity tool that only builds one
of the two variants STRUCTURALLY cannot byte-match the other class of TU. So every
candidate is compiled+classified under BOTH /O2 /Oy AND /O2 /Oy-, and the BETTER
verdict is kept (EXACT > RELOC > DIFFER). On a tie /Oy wins (historical default).
The winning flag is recorded in a 4th 'flag' column (Oy | Oy-).

Usage: python catalog_parity_audit.py [--jobs N]
Writes rebuild/compile-gate/parity_audit.tsv and prints a summary + the DIFFER list.
"""
import csv, re, subprocess, os, sys, glob
from pathlib import Path
from concurrent.futures import ThreadPoolExecutor

ROOT = Path(r"D:\Documents\FableTLC")
VC = Path(r"D:\Tools\vc71")
SP = Path(r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\d87e94eb-5fcd-4ea2-9871-8c5904962855\scratchpad\audit_work")
OBJDUMP = r"C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\objdump.exe"
DS = re.compile(r"^\s*[0-9a-fA-F]+\s+<(.+)>:$"); DB = re.compile(r"^\s*[0-9a-fA-F]+:\s+((?:[0-9a-fA-F]{2}\s+)+)")
RL = re.compile(r"^([0-9a-fA-F]{8})\s+\S+\s+.+$")

# Frame-pointer variants to try. /Oy omits the frame pointer, /Oy- keeps it.
# Order matters: on a verdict tie the FIRST-listed flag wins, so /Oy is the
# historical default and stays first.
# Per-TU flag variants, tried in order (primary first). Retail Fable was NOT built
# with one global flag: most TUs are /Oy (frame omitted, e.g. cmouse), but some keep
# frames (/Oy-, e.g. the NUISystem/CFrontEndManager TU at 0x0059xxxx) and need /G7
# (Pentium-4 scheduling) too. classify() short-circuits on the first EXACT/RELOC, so
# most files pay ONE compile; only genuine non-matches try all three.
FLAG_VARIANTS = [("Oy", ["/Oy"]), ("Oy-", ["/Oy-"]), ("Oy-G7", ["/Oy-", "/G7"]),
                 ("Os", ["/Os"])]  # /Os: size-opt TUs (e.g. vtable GetSizeofClass push/pop thunks)
# Verdict precedence for picking the better of the two builds (higher = better).
VERDICT_RANK = {"EXACT": 3, "RELOC": 2}  # DIFFER/SRC_FAIL/etc fall through to 0


def env():
    e = dict(os.environ); e["PATH"] = str(VC / "bin") + ";" + e["PATH"]
    e["INCLUDE"] = f"{VC/'include'};{ROOT/'rebuild'/'include'}"; e["LIB"] = str(VC / "lib"); return e
E = env()

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


def _build_and_verdict(cpppath, retail, leaf, addr, fkey, fflags):
    """Compile cpppath under /O2 + fflags (a list) and return a verdict string
    (EXACT / RELOC / DIFFER(...) / SRC_FAIL / OBJDUMP_ERR). No oracle lookup here."""
    wd = SP / addr / fkey; wd.mkdir(parents=True, exist_ok=True)
    obj = wd / "a.obj"
    if obj.exists(): obj.unlink()
    cp = subprocess.run([str(VC / "bin" / "cl.exe"), "/nologo", "/c", "/O2", *fflags, "/w",
                         f"/Fo{obj}", str(cpppath)], capture_output=True, text=True, env=E, cwd=str(wd))
    if cp.returncode or not obj.exists(): return "SRC_FAIL"
    try:
        built, sec = obj_text(obj, leaf)
    except Exception:
        return "OBJDUMP_ERR"
    if built == retail: return "EXACT"
    rel = obj_relocs(obj, sec)
    if mask(retail, rel) == mask(built, rel): return "RELOC"
    return f"DIFFER({len(built)}v{len(retail)})"


def classify(cpppath, oracle):
    """Compile under BOTH /Oy and /Oy-, keep the better verdict (EXACT>RELOC>DIFFER),
    and record which flag won. Returns (addr, verdict, file, flag)."""
    addr = os.path.basename(cpppath).split("_")[-1].replace(".cpp", "").lower()
    o = oracle.get(addr)
    fn = os.path.basename(cpppath)
    if not o: return (addr, "NO_ORACLE", fn, "")
    retail = bytes.fromhex(o["bytes"]); leaf = o["name"].rsplit("::", 1)[-1]

    # Build under every flag variant; keep the best-ranked verdict. FLAG_VARIANTS
    # is ordered so that on a rank tie the first (Oy) is retained as the winner.
    # Fast path with fallback: try /Oy first (correct for most TUs). Break on the
    # first EXACT/RELOC so most files compile ONCE; only genuine non-matches retry
    # the frame-kept variants /Oy- then /Oy- /G7. (A rare RELOC-under-/Oy that could
    # be EXACT under /Oy- is accepted as RELOC -- not worth 3x-ing every file.)
    best_v, best_flag = None, ""
    for fkey, fflags in FLAG_VARIANTS:
        v = _build_and_verdict(cpppath, retail, leaf, addr, fkey, fflags)
        if best_v is None or VERDICT_RANK.get(v, 0) > VERDICT_RANK.get(best_v, 0):
            best_v, best_flag = v, fkey
        if VERDICT_RANK.get(v, 0) >= 2:  # EXACT or RELOC -> matched; stop early
            break
    return (addr, best_v, fn, best_flag)

def main():
    jobs = int(sys.argv[sys.argv.index("--jobs") + 1]) if "--jobs" in sys.argv else 6
    SP.mkdir(parents=True, exist_ok=True)
    oracle = {r["address"].lower(): r for r in csv.DictReader(open(ROOT / "rebuild/oracles/auto-re-candidates.tsv", encoding="utf-8-sig"), delimiter="\t")}
    files = sorted(glob.glob(str(ROOT / "rebuild/src/compiled/**/*.cpp"), recursive=True))
    results = []
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        for r in ex.map(lambda f: classify(f, oracle), files):
            results.append(r)
    out = ROOT / "rebuild/compile-gate/parity_audit.tsv"
    with open(out, "w", encoding="utf-8", newline="") as f:
        w = csv.writer(f, delimiter="\t", lineterminator="\n"); w.writerow(["address", "verdict", "file", "flag"])
        for r in sorted(results): w.writerow(r)
    from collections import Counter
    tally = Counter(r[1].split("(")[0] for r in results)
    print(f"AUDIT of {len(results)} landed functions:")
    for k in ("EXACT", "RELOC", "DIFFER", "SRC_FAIL", "OBJDUMP_ERR", "NO_ORACLE"):
        if tally.get(k): print(f"  {k:12} {tally[k]}")
    # Breakdown of EXACT/RELOC matches by winning flag (frame-kept TUs show up as
    # Oy- / Oy-G7 -- these would be false DIFFERs under a single global /Oy).
    oym = Counter(r[3] for r in results if r[1].split("(")[0] in ("EXACT", "RELOC"))
    print("  (matches by winning flag: " +
          "  ".join(f"{k or 'n/a'}={oym[k]}" for k in sorted(oym)) + ")")
    bad = [r for r in results if r[1].startswith(("DIFFER", "SRC_FAIL", "OBJDUMP_ERR", "NO_ORACLE"))]
    print(f"\nNON-MATCHING landed functions ({len(bad)}):")
    for a, v, fn, fl in sorted(bad): print(f"  {a}  {v:14} {fn}")
    print(f"\n-> {out}")

if __name__ == "__main__":
    main()
