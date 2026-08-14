#!/usr/bin/env python3
"""Drive FrontendSaveRows_test.exe over every local profile and diff its rows
against the save_metadata.py golden. The native feeder caps at 4 rows (renderer
sink limit); the python reference is truncated to the same cap for comparison.

Usage: python tools/tests/run_frontend_save_rows.py <FrontendSaveRows_test.exe> [saves_dir]
"""
import os, sys, subprocess, json

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, ROOT)
from tools import save_metadata  # noqa: E402

CAP = 4


def golden_rows(profile_dir):
    data = save_metadata.enumerate_profile(profile_dir)
    # native feeder only emits rows whose file is present on disk
    rows = [(r.filename, r.action) for r in data.rows if r.exists]
    return rows[:CAP]


def native_rows(exe, profile_dir):
    r = subprocess.run([exe, str(profile_dir)], capture_output=True, text=True)
    rows = []
    sink_ok = False
    for line in r.stdout.splitlines():
        p = line.split("\t")
        if p[0] == "ROW":
            rows.append((p[1], int(p[2], 16)))
        elif p[0] == "SINK_OK":
            sink_ok = True
        elif p[0] in ("SINK_MISMATCH", "SINK_ROW_MISMATCH"):
            raise AssertionError("native sink check failed: " + line + "\n" + r.stdout)
    if r.returncode != 0:
        raise AssertionError("native exe rc=%d\n%s\n%s" % (r.returncode, r.stdout, r.stderr))
    return rows, sink_ok


def main():
    exe = sys.argv[1]
    saves = sys.argv[2] if len(sys.argv) > 2 else os.path.join(
        os.path.expanduser("~"), "Documents", "My Games", "Fable", "Saves")
    profiles = [os.path.join(saves, d) for d in os.listdir(saves)
                if os.path.isfile(os.path.join(saves, d, "Profile.bin"))]
    total = ok = 0
    for p in sorted(profiles):
        total += 1
        g = golden_rows(p)
        n, sink_ok = native_rows(exe, p)
        if g == n and sink_ok:
            ok += 1
            print("PASS  %-16s %d rows" % (os.path.basename(p), len(n)))
        else:
            print("FAIL  %-16s\n  golden=%s\n  native=%s sink_ok=%s"
                  % (os.path.basename(p), g, n, sink_ok))
    print("\n%d/%d profiles match" % (ok, total))
    return 0 if ok == total else 1


if __name__ == "__main__":
    raise SystemExit(main())
