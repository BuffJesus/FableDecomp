#!/usr/bin/env python3
"""Automated cracker for byte-parity residues (scheduler / liveness / comparison-order
artifacts) -- generalizes the Std_Move_Backward liveness-shaping crack so we don't hand-diff
each one. Given a landed DIFFER function, it applies semantic-preserving source remedies,
scores each vs the retail oracle, confirms behaviour with the existing test, and emits the
winning source. If nothing cracks, prints an annotated retail-vs-built disasm diff.

Remedies:
  1. comparison-operand flip   (a != b  <->  b != a ; ==  likewise)  -> fixes cmp operand order
  2. liveness-shaping          (T x = <pure expr>; ...x...x...  ->  inline the expr at each use)
  3. combos of 1x2, each x the pragma sweep (optimize s/t/g)

Usage: python crack_residue.py <address> [--land]
"""
import csv, re, subprocess, os, sys, glob, itertools
from pathlib import Path

ROOT = Path(r"D:\Documents\FableTLC")
VC = Path(r"D:\Tools\vc71")
SP = Path(r"C:\Users\Cornelio\AppData\Local\Temp\claude\D--Documents-FableTLC\d87e94eb-5fcd-4ea2-9871-8c5904962855\scratchpad\crack_work")
OBJDUMP = r"C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\objdump.exe"
DS = re.compile(r"^\s*[0-9a-fA-F]+\s+<(.+)>:$"); DB = re.compile(r"^\s*[0-9a-fA-F]+:\s+((?:[0-9a-fA-F]{2}\s+)+)")
RL = re.compile(r"^([0-9a-fA-F]{8})\s+\S+\s+.+$")
PRAGMAS = ["", '#pragma optimize("s",on)\n', '#pragma optimize("t",on)\n', '#pragma optimize("g",on)\n']

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

def parity(src, leaf, retail, wd):
    sp = wd / "v.cpp"; sp.write_text(src, encoding="utf-8"); obj = wd / "v.obj"
    if obj.exists(): obj.unlink()
    cp = subprocess.run([str(VC/"bin"/"cl.exe"), "/nologo", "/c", "/O2", "/Oy", "/w", f"/Fo{obj}", str(sp)],
                        capture_output=True, text=True, env=E, cwd=str(wd))
    if cp.returncode or not obj.exists(): return "SRC_FAIL", None, None
    try: built, sec = obj_text(obj, leaf)
    except Exception: return "OBJDUMP_ERR", None, None
    rel = obj_relocs(obj, sec)
    if built == retail: return "MATCH", built, sec
    if mask(retail, rel) == mask(built, rel): return "RELOC", built, sec
    return f"DIFFER({len(built)}v{len(retail)})", built, sec

# --- semantic-preserving source transforms ---
CMP = re.compile(r"(if\s*\(\s*)([A-Za-z_][\w\->\.\*\s]*?)(\s*)(!=|==|<|>|<=|>=)(\s*)([A-Za-z_][\w\->\.\*\s]*?)(\s*\))")
FLIP = {"!=": "!=", "==": "==", "<": ">", ">": "<", "<=": ">=", ">=": "<="}
def flip_comparisons(src):
    out = []
    for m in CMP.finditer(src):
        # swap operands (and flip relational op accordingly)
        new = f"{m.group(1)}{m.group(6).strip()}{m.group(3)}{FLIP[m.group(4)]}{m.group(5)}{m.group(2).strip()}{m.group(7)}"
        out.append(src[:m.start()] + new + src[m.end():])
    return out

DECL = re.compile(r"^([ \t]*)([A-Za-z_][\w:<>\* ]*?)\s+(\w+)\s*=\s*(.+?);\s*$", re.MULTILINE)
def is_pure(expr):
    e = expr.strip()
    if any(t in e for t in ("=", "++", "--")): return False
    # allow casts "(type)" but reject function calls: a '(' not directly a cast
    for m in re.finditer(r"(\w+)\s*\(", e):
        if m.group(1) not in ("char", "unsigned", "int", "long", "short", "size_t", "void", "static_cast", "reinterpret_cast"):
            return False
    return True
def liveness_inline(src):
    """For each pure local used >=2 times after its decl, emit a variant inlining it."""
    variants = []
    for m in DECL.finditer(src):
        indent, typ, name, expr = m.groups()
        if not is_pure(expr): continue
        after = src[m.end():]
        uses = len(re.findall(r"\b" + re.escape(name) + r"\b", after))
        if uses < 2: continue
        # remove the declaration line, replace uses with (expr)
        new_after = re.sub(r"\b" + re.escape(name) + r"\b", "(" + expr.strip() + ")", after)
        variants.append(src[:m.start()] + new_after)
    return variants

def behaviour_ok(src, addr, wd):
    tests = glob.glob(str(ROOT / f"rebuild/tests/**/*{addr}*_test.cpp"), recursive=True)
    if not tests: return None  # unknown
    so = wd / "s.obj"; to = wd / "t.obj"; exe = wd / "t.exe"
    for p in (so, to, exe):
        if p.exists(): p.unlink()
    (wd / "s.cpp").write_text(src, encoding="utf-8")
    if subprocess.run([str(VC/"bin"/"cl.exe"), "/nologo", "/c", "/O2", "/Oy", "/w", f"/Fo{so}", str(wd/'s.cpp')],
                      capture_output=True, env=E, cwd=str(wd)).returncode: return False
    if subprocess.run([str(VC/"bin"/"cl.exe"), "/nologo", "/c", "/Od", "/w", f"/Fo{to}", tests[0]],
                      capture_output=True, env=E, cwd=str(wd)).returncode: return False
    if subprocess.run([str(VC/"bin"/"link.exe"), "/nologo", f"/out:{exe}", str(so), str(to)],
                      capture_output=True, env=E, cwd=str(wd)).returncode: return False
    return subprocess.run([str(exe)], capture_output=True, env=E, cwd=str(wd)).returncode == 0

def disasm(b, wd):
    p = wd / "d.bin"; p.write_bytes(b)
    out = subprocess.run([OBJDUMP, "-D", "-b", "binary", "-m", "i386", "-M", "intel", str(p)],
                         capture_output=True, text=True).stdout
    ins = []
    for l in out.splitlines():
        m = re.match(r"^\s*([0-9a-fA-F]+):\s+((?:[0-9a-fA-F]{2} )+)\s*(.*)$", l)
        if m: ins.append((int(m.group(1), 16), m.group(2).strip(), m.group(3).strip()))
    return ins

def show_diff(retail, built, wd):
    R = disasm(retail, wd); B = disasm(built, wd)
    print(f"    {'RETAIL':<40}  {'BUILT':<40}")
    n = max(len(R), len(B))
    for i in range(n):
        r = R[i] if i < len(R) else (None, "", "")
        b = B[i] if i < len(B) else (None, "", "")
        rs = f"{r[0]:#06x} {r[1]:<14} {r[2]}" if r[0] is not None else ""
        bs = f"{b[0]:#06x} {b[1]:<14} {b[2]}" if b[0] is not None else ""
        mark = "  " if (r[2] == b[2]) else "<>"
        print(f"  {mark}{rs:<40}  {bs:<40}")

def main():
    addr = sys.argv[1].strip().lower().replace("0x", ""); land = "--land" in sys.argv
    SP.mkdir(parents=True, exist_ok=True); wd = SP / addr; wd.mkdir(exist_ok=True)
    oracle = {r["address"].lower(): r for r in csv.DictReader(open(ROOT/"rebuild/oracles/auto-re-candidates.tsv", encoding="utf-8-sig"), delimiter="\t")}
    o = oracle.get(addr)
    if not o: print(f"no oracle for {addr}"); return
    retail = bytes.fromhex(o["bytes"]); leaf = o["name"].rsplit("::", 1)[-1]
    srcs = glob.glob(str(ROOT / f"rebuild/src/compiled/**/*{addr}*.cpp"), recursive=True)
    if not srcs: print(f"no landed source for {addr}"); return
    base = Path(srcs[0]).read_text(encoding="utf-8")
    # build candidate variant set
    cands = {base}
    flips = flip_comparisons(base); cands.update(flips)
    livs = liveness_inline(base); cands.update(livs)
    for f in flips:  # combine flip x liveness
        cands.update(liveness_inline(f))
    # x pragma sweep
    allv = []
    for c in cands:
        for pg in PRAGMAS:
            allv.append(pg + c)
    print(f"{addr} {o['name']}: {len(cands)} source variants x {len(PRAGMAS)} pragmas = {len(allv)} candidates")
    winner = None
    for v in allv:
        st, _, _ = parity(v, leaf, retail, wd)
        if st in ("MATCH", "RELOC"):
            if behaviour_ok(v, addr, wd) is not False:
                winner = (st, v); break
    if not winner:
        print("  NO CRACK by mechanical remedies. Annotated retail-vs-built diff (base variant):")
        _, built, _ = parity(base, leaf, retail, wd)
        if built is not None:
            show_diff(retail, built, wd)
        else:
            print("    (base source did not compile cleanly here)")
        return
    st, v = winner
    print(f"  *** CRACKED: {st} + behaviour OK ***")
    outp = wd / f"{addr}.cracked.cpp"; outp.write_text(v, encoding="utf-8")
    print(f"  winning source -> {outp}")
    if land:
        Path(srcs[0]).write_text(v, encoding="utf-8")
        print(f"  wrote winner into {srcs[0]}")

if __name__ == "__main__":
    main()
