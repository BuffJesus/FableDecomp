#!/usr/bin/env python3
"""Deterministic gate for a hand-retyped landed function.

    python retype_check.py <address> <candidate.cpp> [--apply] [--test <candidate_test.cpp>]
                           [--baseline <original.cpp> | --baseline-head] [--diff-bytes]

1. Reproduces the CURRENT landed file's parity status (flag ladder, jump-table extractor) —
   BASELINE_FAIL if it cannot.
2. Compiles the candidate and requires the SAME status (MATCH stays MATCH, RELOCATION_MATCH
   stays RELOCATION_MATCH).
3. Runs the behaviour test (candidate test if given, else the landed one) — must PASS.
4. Static policy: candidate must #include "engine/<Class>.h" for its trusted class, must not
   redeclare that class, must be genuine (no __asm/naked/_emit), must not declare a generic
   throwaway struct (T/Sub/Owner/...).
Prints one line `RESULT <OK|reason> baseline=<st> candidate=<st> behaviour=<b>` and exits 0 on OK.
With --apply and OK, overwrites the landed source (and test if --test given) and logs to
rebuild/backlog/retype_log.tsv with action `applied-manual`.
"""
from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import label_trust  # noqa: E402
import retype_landed as R  # noqa: E402
import verify_and_land as v  # noqa: E402
from crawl import purity  # noqa: E402

GENERIC = {"T", "Sub", "Owner", "Slot", "Info", "Obj", "S", "Self", "This", "Inner", "Foo",
           "Holder", "Mid", "MidObj", "CamObj"}


def print_byte_diff(retail: bytes, built: bytes | None) -> None:
    if built is None:
        return
    print("BYTE_EXPECTED " + retail.hex())
    print("BYTE_BUILT    " + built.hex())
    width = max(len(retail), len(built))
    diffs = []
    for offset in range(width):
        expected = retail[offset] if offset < len(retail) else None
        actual = built[offset] if offset < len(built) else None
        if expected != actual:
            diffs.append(
                f"{offset:04x}:{'--' if expected is None else f'{expected:02x}'}>"
                f"{'--' if actual is None else f'{actual:02x}'}"
            )
    print("BYTE_DIFF " + " ".join(diffs))


def main() -> int:
    if len(sys.argv) < 3:
        print(__doc__); return 2
    addr = sys.argv[1].lower().replace("0x", "").zfill(8)
    cand_path = Path(sys.argv[2])
    apply = "--apply" in sys.argv
    test_cand = Path(sys.argv[sys.argv.index("--test") + 1]) if "--test" in sys.argv else None
    baseline_path = Path(sys.argv[sys.argv.index("--baseline") + 1]) if "--baseline" in sys.argv else None
    mf = label_trust.Manifest()
    row = mf.by_addr.get(addr)
    if not row or not row.get("compiled_source"):
        print("RESULT NOT_LANDED"); return 1
    src_path = Path(row["compiled_source"])
    cls = mf.trusted_module(addr) or row["module"]
    cand = cand_path.read_text(encoding="utf-8")
    # static policy
    if not purity.is_genuine(cand):
        print("RESULT NOT_GENUINE"); return 1
    if f'#include "engine/{cls}.h"' not in cand:
        print(f"RESULT NO_ENGINE_INCLUDE expected engine/{cls}.h"); return 1
    if not (R.ENGINE / f"{cls}.h").exists():
        print(f"RESULT NO_HEADER engine/{cls}.h"); return 1
    if re.search(r"\b(?:struct|class)\s+" + re.escape(cls) + r"\s*(?::[^{]*)?\{", cand):
        print(f"RESULT REDECLARES_CLASS {cls}"); return 1
    generic = [m for m in re.findall(r"\bstruct\s+([A-Za-z_]\w*)\s*\{", cand) if m in GENERIC]
    if generic:
        print(f"RESULT GENERIC_STRUCT {','.join(generic)}"); return 1
    # parity
    orc = R.oracle_bytes()
    retail = orc.get(addr)
    if retail is None:
        print("RESULT NO_ORACLE"); return 1
    cat = R.catalog_block(addr)
    if cat["qfe"]:
        try:
            v.use_qfe()
        except FileNotFoundError:
            print("RESULT QFE_TOOLSET_MISSING"); return 1
    e = v.env()
    leaf = label_trust.Manifest.leaf(row["name"])
    work = R.WORK / ("manual_" + addr)
    work.mkdir(parents=True, exist_ok=True)
    if "--baseline-head" in sys.argv:
        original = subprocess.check_output(
            ["git", "show", "HEAD:" + src_path.relative_to(v.ROOT).as_posix()],
            cwd=v.ROOT,
        ).decode("utf-8")
    else:
        original = (baseline_path.read_text(encoding="utf-8") if baseline_path else
                    src_path.read_text(encoding="utf-8"))
    base = R.find_baseline(v.vc71(original), addr, leaf, retail, work, e, cat)
    if base is None:
        print("RESULT BASELINE_FAIL"); return 1
    baseline, flags, extra, ext = base
    st, built, _ = v.parity_of(v.vc71(cand), addr, leaf, retail, work, e, extra=extra,
                               base_flags=flags, text_extractor=ext)
    if st != baseline:
        # give the candidate the same flag ladder chance the baseline had
        alt = R.find_baseline(v.vc71(cand), addr, leaf, retail, work, e, cat)
        if alt and alt[0] == baseline:
            st, flags, extra, ext = alt
            v.parity_of(v.vc71(cand), addr, leaf, retail, work, e, extra=extra, base_flags=flags, text_extractor=ext)
        else:
            if "--diff-bytes" in sys.argv:
                print_byte_diff(retail, built)
            print(f"RESULT PARITY_CHANGED baseline={baseline} candidate={st}"); return 1
    rel = src_path.relative_to(v.ROOT / "rebuild" / "src" / "compiled")
    test_path = (v.ROOT / "rebuild" / "tests" / rel).with_name(src_path.stem + "_test.cpp")
    tst = (test_cand.read_text(encoding="utf-8") if test_cand else
           (test_path.read_text(encoding="utf-8") if test_path.exists() else None))
    beh = "n/a"
    if tst is not None:
        beh = v.behaviour_of(v.vc71(tst), addr, cat["pattern"], work, e, work / (addr + ".obj"))
        if beh != "PASS" and test_cand and test_path.exists():
            beh = v.behaviour_of(v.vc71(test_path.read_text(encoding="utf-8")), addr, cat["pattern"], work, e, work / (addr + ".obj"))
            if beh == "PASS":
                test_cand = None  # keep the landed test
        if beh != "PASS":
            print(f"RESULT BEHAV_FAIL baseline={baseline} candidate={st} behaviour={beh}"); return 1
    if apply:
        src_path.write_text(cand, encoding="utf-8")
        if test_cand is not None and test_path.exists():
            test_path.write_text(test_cand.read_text(encoding="utf-8"), encoding="utf-8")
        with open(R.LOG, "a", encoding="utf-8", newline="") as f:
            f.write("\t".join([addr, cls, src_path.name, "applied-manual", baseline, st, beh, "APPLIED_MANUAL"]) + "\n")
    print(f"RESULT OK baseline={baseline} candidate={st} behaviour={beh} file={src_path}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
