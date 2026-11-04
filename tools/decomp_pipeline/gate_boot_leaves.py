#!/usr/bin/env python3
"""Re-verify the bootstrap's verified leaves without running the whole bootstrap.

Parses the `Invoke-VerifiedLeaf` blocks in rebuild/build_bootstrap.ps1 and, for each
leaf, does what the bootstrap does: compile the source with the bootstrap's Release
options, run tools/check_boot_object.py (retail parity + residue gate), compile and
link the behaviour fixture against the source object, and match the pass pattern.
Also reports whether each source is genuine C++ (crawl/purity.py).

Useful for fast checks after a shared-header edit or a de-bake. The full bootstrap
also checks runtime links and the isolated WinMain fixture.

Usage: python tools/decomp_pipeline/gate_boot_leaves.py [addr ...] [--jobs N] [--purity]
       (no addresses = every leaf; --purity only lists non-genuine leaves)
"""
import argparse
import os
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
VC = Path(r"D:\Tools\vc71")
sys.path.insert(0, str(Path(__file__).resolve().parent / "crawl"))
import purity  # noqa: E402

OPTIONS = ["/nologo", "/c", "/W3", "/MT", "/GS", "/O2", "/Oy"]


def parse_bootstrap():
    text = (ROOT / "rebuild" / "build_bootstrap.ps1").read_text(encoding="utf-8")
    paths = dict(re.findall(r"^\$(\w+) = Join-Path \$rebuildRoot '([^']+)'", text, re.M))
    strings = dict(re.findall(r"^\$(\w+) = '([^']*)'", text, re.M))
    leaves = []
    for block in re.findall(r"Invoke-VerifiedLeaf `\n((?:\s+-\w+ [^\n]+\n)+)", text):
        args = dict(re.findall(r"-(\w+) ([^`\n]+?)\s*`?$", block, re.M))

        def value(key):
            raw = args.get(key, "").strip()
            if raw.startswith("$"):
                name = raw[1:]
                return paths.get(name) or strings.get(name)
            return raw.strip("'") or None

        leaves.append({
            "address": value("Address").lower(),
            "description": value("Description"),
            "source": value("Source"),
            "behavior": value("BehaviorSource"),
            "pattern": value("PassPattern"),
            "allow_move": value("AllowMove"),
        })
    return leaves


def env():
    e = dict(os.environ)
    e["PATH"] = str(VC / "bin") + ";" + e["PATH"]
    e["INCLUDE"] = f"{VC / 'include'};{ROOT / 'rebuild' / 'include'}"
    e["LIB"] = str(VC / "lib")
    return e


def gate(leaf, work):
    e = env()
    src = ROOT / "rebuild" / leaf["source"]
    beh = ROOT / "rebuild" / leaf["behavior"]
    genuine = purity.is_genuine(src.read_text(encoding="utf-8", errors="replace"))
    d = Path(tempfile.mkdtemp(prefix=leaf["address"] + "_", dir=work))
    obj, tobj, exe = d / "s.obj", d / "t.obj", d / "t.exe"
    r = subprocess.run([str(VC / "bin" / "cl.exe")] + OPTIONS + [f"/Fo{obj}", str(src)],
                       capture_output=True, text=True, env=e)
    if r.returncode or not obj.exists():
        return leaf, genuine, "SRC_FAIL", "-", r.stdout[-800:]
    check = [sys.executable, str(ROOT / "tools" / "check_boot_object.py"), "--root", str(ROOT),
             "--object", str(obj), "--address", leaf["address"]]
    if leaf["allow_move"]:
        check += ["--allow-move", leaf["allow_move"]]
    c = subprocess.run(check, capture_output=True, text=True, env=e)
    parity = (c.stdout.strip().splitlines() or ["?"])[-1] if c.returncode == 0 else "GATE_FAIL"
    r = subprocess.run([str(VC / "bin" / "cl.exe")] + OPTIONS + [f"/Fo{tobj}", str(beh)],
                       capture_output=True, text=True, env=e)
    if r.returncode or not tobj.exists():
        return leaf, genuine, parity, "TCC_FAIL", r.stdout[-800:]
    r = subprocess.run([str(VC / "bin" / "link.exe"), "/nologo", "/subsystem:console",
                        f"/out:{exe}", str(obj), str(tobj)], capture_output=True, text=True, env=e)
    if r.returncode or not exe.exists():
        return leaf, genuine, parity, "LINK_FAIL", r.stdout[-800:]
    try:
        run = subprocess.run([str(exe)], capture_output=True, text=True, env=e, timeout=120)
        ok = run.returncode == 0 and leaf["pattern"] in (run.stdout + run.stderr)
        behaviour = "PASS" if ok else "FAIL"
    except subprocess.TimeoutExpired:
        behaviour = "TIMEOUT"
    return leaf, genuine, parity, behaviour, (c.stdout + c.stderr)[-800:] if parity == "GATE_FAIL" else ""


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("addresses", nargs="*")
    ap.add_argument("--jobs", type=int, default=8)
    ap.add_argument("--purity", action="store_true")
    a = ap.parse_args()
    leaves = parse_bootstrap()
    if a.addresses:
        want = {x.lower().replace("0x", "").rjust(8, "0") for x in a.addresses}
        leaves = [l for l in leaves if l["address"] in want]
    if a.purity:
        bad = [l for l in leaves if not purity.is_genuine(
            (ROOT / "rebuild" / l["source"]).read_text(encoding="utf-8", errors="replace"))]
        for l in bad:
            print(l["address"], l["source"])
        print(f"{len(bad)} of {len(leaves)} verified leaves are not genuine C++")
        return 0
    work = Path(tempfile.mkdtemp(prefix="bootgate_"))
    failed = 0
    with ThreadPoolExecutor(a.jobs) as pool:
        for leaf, genuine, parity, behaviour, detail in pool.map(lambda l: gate(l, work), leaves):
            bad = "MATCH" not in parity or behaviour != "PASS"
            failed += bad
            tag = "genuine" if genuine else "ASM"
            print(f"{leaf['address']} {tag:7} {behaviour:9} {parity}")
            if bad and detail:
                print("    " + detail.replace("\n", "\n    "))
    print(f"{len(leaves) - failed}/{len(leaves)} leaves pass")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
