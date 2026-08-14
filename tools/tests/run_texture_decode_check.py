#!/usr/bin/env python3
"""Cross-check the native fable_texture_decode against the Python decoder on every
MINIMAP_*_FRONT_END entry in the real frontend.big (parity/purity: the runtime
container-read path must equal the verified reference decode byte-for-byte).

Usage: python tools/tests/run_texture_decode_check.py <texdec_check.exe> [frontend.big]
Proven 2026-08-13: 95/95 RGBA-identical.
"""
import os, sys, subprocess, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, "tools"))
from texture_build import load_big, decode_entry  # noqa: E402
from render_fable_frontend_menu import find_entry  # noqa: E402
import numpy as np  # noqa: E402

DEFAULT_BIG = (r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters"
               r"\data\graphics\pc\frontend.big")


def main():
    exe = sys.argv[1]
    big = sys.argv[2] if len(sys.argv) > 2 else DEFAULT_BIG
    buf, parsed = load_big(big)
    names = []

    def walk(p):
        if isinstance(p, dict):
            for k, v in p.items():
                if k == "name" and isinstance(v, str):
                    names.append(v)
                walk(v)
        elif isinstance(p, (list, tuple)):
            for v in p:
                walk(v)
    walk(parsed)
    mm = sorted(set(n for n in names if n.upper().startswith("MINIMAP_")))
    tot = ok = bad = 0
    for n in mm:
        _, e = find_entry(parsed, n)[0]
        info, pay = e["info"], buf[e["offset"]:e["offset"] + e["size"]]
        _, mips = decode_entry(info, pay, max_mips=1)
        ref = np.asarray(mips[0], np.uint8)
        with tempfile.TemporaryDirectory() as td:
            ip, pp, op = (os.path.join(td, x) for x in "ipo")
            open(ip, "wb").write(bytes(info))
            open(pp, "wb").write(bytes(pay))
            r = subprocess.run([exe, ip, pp, op], capture_output=True, text=True)
            tot += 1
            if r.returncode != 0:
                bad += 1
                print("FAIL", n, r.stdout.strip())
                continue
            got = np.frombuffer(open(op, "rb").read(), np.uint8)
            if got.size == ref.size and np.array_equal(got.reshape(ref.shape), ref):
                ok += 1
            else:
                bad += 1
                print("FAIL", n, "pixel mismatch")
    print("%d/%d minimaps RGBA-identical" % (ok, tot))
    return 0 if bad == 0 and tot > 0 else 1


if __name__ == "__main__":
    raise SystemExit(main())
