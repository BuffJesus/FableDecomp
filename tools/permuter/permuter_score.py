#!/usr/bin/env python3
"""VC7.1/x86 scorer for the permuter: compile a candidate source and score its .text
distance to authoritative retail bytes (relocation-masked), mirroring
tools/compare_candidate_objects.py. Lower score = closer; 0 = exact/relocation match.

Usage (CLI):  python permuter_score.py <hexaddr> <cpp> [--oracle path.tsv]
Importable:   from permuter_score import score_source -> dict
"""
from __future__ import annotations
import csv, re, subprocess, os, sys, argparse
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from compare_candidate_objects import symbol_leaf as _symbol_leaf  # noqa: E402

ROOT = Path(__file__).resolve().parents[2]
VC = Path(r"D:\Tools\vc71")
# Alternate VC7.1 QFE-4035 toolset (13.10.4035 c1xx.dll+c2.dll from WinDDK 3790.1830).
# Only the compiler *binaries* differ; headers/libs (INCLUDE/LIB) stay RTM-compatible.
# See docs/pipeline/QFE4035_COMPILER_GATE.md. Override the location with $VC71_QFE.
QFE = Path(os.environ.get("VC71_QFE", r"D:\Tools\vc71-qfe4035"))
OBJDUMP = os.environ.get("OBJDUMP",
    r"C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\objdump.exe")
DEFAULT_ORACLE = ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv"

_DS = re.compile(r"^\s*([0-9a-fA-F]+)\s+<(.+)>:$")
_DB = re.compile(r"^\s*[0-9a-fA-F]+:\s+((?:[0-9a-fA-F]{2}\s+)+)")
_RL = re.compile(r"^([0-9a-fA-F]{8})\s+\S+\s+.+$")


def _read_tsv(p):
    with open(p, encoding="utf-8-sig", newline="") as f:
        return list(csv.DictReader(f, delimiter="\t"))


def _obj_text(obj, leaf):
    pr = subprocess.run([OBJDUMP, "-d", str(obj)], capture_output=True, text=True, check=True)
    fns, sec, cur = [], -1, None
    for line in pr.stdout.splitlines():
        if line.startswith("Disassembly of section .text:"):
            sec += 1; cur = None; continue
        s = _DS.match(line)
        if s:
            if int(s.group(1), 16) == 0 or cur is None:
                cur = {"symbol": s.group(2), "section": sec, "bytes": bytearray()}; fns.append(cur)
            continue
        e = _DB.match(line)
        if e and cur is not None:
            cur["bytes"].extend(bytes.fromhex(e.group(1)))
    if not fns:
        raise RuntimeError("no functions in object")
    named = [f for f in fns if leaf and leaf in str(f["symbol"])]
    pool = named or fns
    # Same rule as the comparer: `??_G`/`??_E` are compiler-generated scalar/vector DELETING
    # destructors, emitted only because the source declares a dtor, and they are often the
    # same length as the real body -- so "longest wins" scores the wrong function and
    # reports a difference for a byte-exact source.
    if "deleting_destructor" not in (leaf or ""):
        real = [f for f in pool if not str(f["symbol"]).startswith(("??_G", "??_E"))]
        pool = real or pool
    sel = max(pool, key=lambda f: len(f["bytes"]))
    return bytes(sel["bytes"]), int(sel["section"]), str(sel["symbol"])


def _obj_relocs(obj, sel):
    hp = subprocess.run([OBJDUMP, "-h", str(obj)], capture_output=True, text=True, check=True)
    flags, hl = [], hp.stdout.splitlines()
    for i, l in enumerate(hl):
        if re.match(r"^\s*\d+\s+\.text\s+", l):
            flags.append("RELOC" in (hl[i + 1] if i + 1 < len(hl) else ""))
    if sel >= len(flags) or not flags[sel]:
        return []
    bi = sum(flags[:sel + 1]) - 1
    rp = subprocess.run([OBJDUMP, "-r", str(obj)], capture_output=True, text=True, check=True)
    blocks, intext, ts = [], False, -1
    for l in rp.stdout.splitlines():
        if l.startswith("RELOCATION RECORDS FOR ["):
            intext = "[.text]" in l
            if intext:
                ts += 1; blocks.append([])
            continue
        if intext:
            m = _RL.match(l.strip())
            if m:
                blocks[ts].append(int(m.group(1), 16))
    return blocks[bi] if bi < len(blocks) else []


def _mask(payload, offs):
    r = bytearray(payload)
    for o in offs:
        for i in range(o, min(o + 4, len(r))):
            r[i] = 0
    return bytes(r)


def resolve_toolset(qfe: bool):
    """(cl.exe path, bin dir) for the requested toolset. The QFE compiler binaries live in
    their own bin dir (cl.exe auto-loads c1xx.dll/c2.dll from beside itself); headers/libs
    are always the RTM VC's. Fails loudly if --qfe is requested but not installed."""
    if qfe:
        clexe = QFE / "bin" / "cl.exe"
        if not clexe.exists():
            raise FileNotFoundError(
                f"QFE-4035 toolset not found: {clexe}\n"
                f"Install the 13.10.4035 compiler (c1xx.dll+c2.dll+cl.exe from WinDDK "
                f"3790.1830) there, or set $VC71_QFE. See docs/pipeline/QFE4035_COMPILER_GATE.md")
        return clexe, QFE / "bin"
    return VC / "bin" / "cl.exe", VC / "bin"


def _env(bin_dir: Path | None = None):
    e = dict(os.environ)
    # the chosen compiler's bin first, then RTM bin (helper DLLs), then the rest.
    parts = [str(bin_dir)] if bin_dir else []
    parts += [str(VC / "bin"), e["PATH"]]
    e["PATH"] = ";".join(parts)
    e["INCLUDE"] = f"{VC/'include'};{ROOT/'rebuild'/'include'}"
    e["LIB"] = str(VC / "lib")
    return e


# Scoring: 0 == byte/relocation-exact. Otherwise a positive distance where a longer
# matching prefix and equal length score lower. Penalties chosen so "same length,
# few differing bytes" (the regalloc endgame) ranks far below "wrong length".
COMPILE_FAIL = 10 ** 9


DEFAULT_FLAGS = ["/O2", "/Oy"]


def score_source(cpp: Path, addr: str, name: str | None = None,
                 oracle: Path = DEFAULT_ORACLE, workdir: Path | None = None,
                 flags: list | None = None, prepend: str = "", qfe: bool = False) -> dict:
    addr = addr.lower().replace("0x", "")
    workdir = workdir or (cpp.parent / "_score")
    workdir.mkdir(parents=True, exist_ok=True)
    orc = {r["address"].lower(): r for r in _read_tsv(oracle)}
    o = orc.get(addr)
    if o is None:
        return {"score": COMPILE_FAIL, "status": "NO_ORACLE"}
    # Same leaf rule as tools/compare_candidate_objects.py: manifest names come in mangled
    # (`?Foo@C@@UBE...`) and templated (`std::_Dest_val<...A::B...>`) shapes, and a naive
    # rsplit("::") yields garbage for both -- which silently scores the WRONG function in
    # the object (a ctor, or a ??_G destructor thunk of the same length).
    leaf = _symbol_leaf(name or o["name"])
    src = cpp
    if prepend:
        src = workdir / f"{addr}.pre.cpp"
        src.write_text(prepend + "\n" + cpp.read_text(encoding="utf-8"), encoding="utf-8")
    obj = workdir / f"{addr}.obj"
    if obj.exists():
        obj.unlink()
    clexe, bin_dir = resolve_toolset(qfe)
    cp = subprocess.run([str(clexe), "/nologo", "/c", "/W3"]
                        + (flags or DEFAULT_FLAGS) + [f"/Fo{obj}", str(src)],
                        capture_output=True, text=True, env=_env(bin_dir))
    if cp.returncode != 0 or not obj.exists():
        return {"score": COMPILE_FAIL, "status": "COMPILE_FAIL",
                "detail": cp.stdout[-400:] + cp.stderr[-200:]}
    retail = bytes.fromhex(o["bytes"])
    built, sec, sym = _obj_text(obj, leaf)
    rel = _obj_relocs(obj, sec)
    mr, mb = _mask(retail, rel), _mask(built, rel)
    if retail == built:
        return {"score": 0, "status": "MATCH", "sym": sym, "built": len(built), "retail": len(retail)}
    if mr == mb:
        return {"score": 0, "status": "RELOCATION_MATCH", "sym": sym, "built": len(built), "retail": len(retail)}
    common = min(len(mr), len(mb))
    prefix = next((i for i in range(common) if mr[i] != mb[i]), common)
    if len(built) != len(retail):
        score = 100000 + abs(len(built) - len(retail)) * 1000 + (len(retail) - prefix)
    else:
        diff = sum(1 for i in range(len(mr)) if mr[i] != mb[i])
        score = diff  # equal length: distance is #differing bytes (0 handled above)
    return {"score": score, "status": "DIFFER", "sym": sym, "built": len(built),
            "retail": len(retail), "prefix": prefix}


# Known QFE-4035-gated fixtures: (addr, source file, leaf). RTM 3077 cannot byte-match
# these; the 13.10.4035 toolset should. See docs/pipeline/QFE4035_COMPILER_GATE.md.
QFE_FIXTURES = [
    ("00643e09", "isactive.cpp",    "IsActive"),
    ("00643e2e", "setasactive.cpp", "SetAsActive"),
    ("0042f75e", "init.cpp",        "FrontendInit"),
]


def selfcheck(qfe: bool) -> int:
    """Score the known QFE-gated trio under RTM (and, if --qfe, the 4035 toolset) and
    report which now byte-match. Used to verify a fresh 13.10.4035 drop-in."""
    fixdir = ROOT / "tools" / "permuter" / "examples" / "qfe"
    oracle = fixdir / "oracle.tsv"
    if qfe:
        clexe, _ = resolve_toolset(True)  # fail loudly early if absent
        print(f"[toolset] QFE-4035: {clexe}")
    hdr = f"{'addr':10} {'name':14} {'RTM 3077':22}"
    if qfe:
        hdr += f" {'QFE 4035':22}"
    print(hdr)
    matched = 0
    for addr, fname, leaf in QFE_FIXTURES:
        cpp = fixdir / fname
        rtm = score_source(cpp, addr, leaf, oracle, qfe=False)
        row = f"{addr:10} {leaf:14} {rtm['status']}({rtm.get('built')}v{rtm.get('retail')})".ljust(46)
        if qfe:
            q = score_source(cpp, addr, leaf, oracle, qfe=True)
            tag = f"{q['status']}({q.get('built')}v{q.get('retail')})"
            row += f" {tag}"
            if q["status"] in ("MATCH", "RELOCATION_MATCH"):
                matched += 1
        print(row)
    if qfe:
        print(f"\nQFE-4035 matched {matched}/{len(QFE_FIXTURES)} fixtures")
        return 0 if matched == len(QFE_FIXTURES) else 1
    print("\n(run with --qfe once the 13.10.4035 toolset is installed to see it close these)")
    return 0


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("addr", nargs="?"); ap.add_argument("cpp", nargs="?", type=Path)
    ap.add_argument("--oracle", type=Path, default=DEFAULT_ORACLE)
    ap.add_argument("--name", default=None)
    ap.add_argument("--qfe", action="store_true", help="compile with the 13.10.4035 QFE toolset")
    ap.add_argument("--selfcheck", action="store_true",
                    help="score the known QFE-gated fixture trio and report matches")
    a = ap.parse_args()
    if a.selfcheck:
        return selfcheck(a.qfe)
    if not a.addr or not a.cpp:
        ap.error("addr and cpp are required (unless --selfcheck)")
    r = score_source(a.cpp, a.addr, a.name, a.oracle, qfe=a.qfe)
    print(r)
    return 0 if r["status"] in ("MATCH", "RELOCATION_MATCH") else 1


if __name__ == "__main__":
    raise SystemExit(main())
