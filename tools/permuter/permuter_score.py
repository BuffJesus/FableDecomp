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

ROOT = Path(__file__).resolve().parents[2]
VC = Path(r"D:\Tools\vc71")
OBJDUMP = os.environ.get("OBJDUMP",
    r"C:\Users\Cornelio\AppData\Local\Microsoft\WinGet\Packages\BrechtSanders.WinLibs.POSIX.UCRT_Microsoft.Winget.Source_8wekyb3d8bbwe\mingw64\bin\objdump.exe")
DEFAULT_ORACLE = ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv"

_DS = re.compile(r"^\s*[0-9a-fA-F]+\s+<(.+)>:$")
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
            cur = {"symbol": s.group(1), "section": sec, "bytes": bytearray()}; fns.append(cur); continue
        e = _DB.match(line)
        if e and cur is not None:
            cur["bytes"].extend(bytes.fromhex(e.group(1)))
    if not fns:
        raise RuntimeError("no functions in object")
    named = [f for f in fns if leaf and leaf in str(f["symbol"])]
    sel = max(named or fns, key=lambda f: len(f["bytes"]))
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


def _env():
    e = dict(os.environ)
    e["PATH"] = str(VC / "bin") + ";" + e["PATH"]
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
                 flags: list | None = None, prepend: str = "") -> dict:
    addr = addr.lower().replace("0x", "")
    workdir = workdir or (cpp.parent / "_score")
    workdir.mkdir(parents=True, exist_ok=True)
    orc = {r["address"].lower(): r for r in _read_tsv(oracle)}
    o = orc.get(addr)
    if o is None:
        return {"score": COMPILE_FAIL, "status": "NO_ORACLE"}
    leaf = (name or o["name"]).rsplit("::", 1)[-1].lstrip("~")
    src = cpp
    if prepend:
        src = workdir / f"{addr}.pre.cpp"
        src.write_text(prepend + "\n" + cpp.read_text(encoding="utf-8"), encoding="utf-8")
    obj = workdir / f"{addr}.obj"
    if obj.exists():
        obj.unlink()
    cp = subprocess.run([str(VC / "bin" / "cl.exe"), "/nologo", "/c", "/W3"]
                        + (flags or DEFAULT_FLAGS) + [f"/Fo{obj}", str(src)],
                        capture_output=True, text=True, env=_env())
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


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("addr"); ap.add_argument("cpp", type=Path)
    ap.add_argument("--oracle", type=Path, default=DEFAULT_ORACLE)
    ap.add_argument("--name", default=None)
    a = ap.parse_args()
    r = score_source(a.cpp, a.addr, a.name, a.oracle)
    print(r)
    return 0 if r["status"] in ("MATCH", "RELOCATION_MATCH") else 1


if __name__ == "__main__":
    raise SystemExit(main())
