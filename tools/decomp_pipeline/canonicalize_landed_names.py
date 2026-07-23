#!/usr/bin/env python3
"""Canonicalize landed source/test filenames from a batch targets manifest.

Some authoring agents return fallback names like ``candidate_00440ec0`` after
still producing byte-identical source. The lander uses those names for filenames
and catalog entries. This helper renames only those generated files to the
manifest-derived module/function basename and updates build_candidates.ps1.

Usage:
  python tools/decomp_pipeline/canonicalize_landed_names.py <targets.json>
"""

import json
import re
import sys
from pathlib import Path


ROOT = Path(r"D:\Documents\FableTLC")


def sanitize(s):
    return re.sub(r"[^A-Za-z0-9]", "", s or "")


def make_base(module, leaf, addr):
    m = sanitize(module or "_global")
    l = sanitize(leaf or "function")
    return f"{m[:80]}_{l[:48]}_{addr}"[:180]


def main(argv):
    if len(argv) != 2:
        print(__doc__, file=sys.stderr)
        return 2

    targets_path = Path(argv[1])
    targets = json.loads(targets_path.read_text(encoding="utf-8"))
    by_addr = {str(t["address"]).lower().replace("0x", ""): t for t in targets}

    catp = ROOT / "rebuild" / "build_candidates.ps1"
    cat = catp.read_text(encoding="utf-8")
    renamed = []

    for addr, t in sorted(by_addr.items()):
        src_matches = list((ROOT / "rebuild" / "src" / "compiled").glob(f"*candidate{addr}_{addr}.cpp"))
        test_matches = list((ROOT / "rebuild" / "tests").glob(f"*candidate{addr}_{addr}_test.cpp"))
        src_matches += list((ROOT / "rebuild" / "src" / "compiled").glob(f"*candidate_{addr}_{addr}.cpp"))
        test_matches += list((ROOT / "rebuild" / "tests").glob(f"*candidate_{addr}_{addr}_test.cpp"))
        src_matches = sorted(set(src_matches))
        test_matches = sorted(set(test_matches))
        if not src_matches and not test_matches:
            continue
        if len(src_matches) != 1 or len(test_matches) != 1:
            raise SystemExit(f"ambiguous generated files for {addr}: src={src_matches} test={test_matches}")

        leaf = str(t.get("name", "")).rsplit("::", 1)[-1]
        newbase = make_base(t.get("module", "_global"), leaf, addr)
        src_old, test_old = src_matches[0], test_matches[0]
        src_new = src_old.parent / (newbase + ".cpp")
        test_new = test_old.parent / (newbase + "_test.cpp")
        if src_new.exists() and src_new.resolve() != src_old.resolve():
            raise SystemExit(f"target source exists for {addr}: {src_new.name}")
        if test_new.exists() and test_new.resolve() != test_old.resolve():
            raise SystemExit(f"target test exists for {addr}: {test_new.name}")

        src_old.rename(src_new)
        test_old.rename(test_new)
        cat = cat.replace(src_old.name, src_new.name).replace(test_old.name, test_new.name)
        renamed.append((addr, src_old.name, src_new.name))

    catp.write_text(cat, encoding="utf-8")
    print(f"renamed {len(renamed)}")
    for addr, old, new in renamed:
        print(f"{addr} {old} => {new}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
