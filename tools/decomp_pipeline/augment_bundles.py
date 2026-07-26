#!/usr/bin/env python3
"""Append each class's reconstructed struct (from class_struct.py) to its
candidates' disasm bundles, so authoring agents reuse byte-verified offsets
instead of re-deriving them. Idempotent: strips any prior injected block first.

Usage: python augment_bundles.py <bundles_dir> <manifest.json>
"""
import json, sys, os
from pathlib import Path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import class_struct

MARK = "\n## Byte-verified struct facts for the owning class (offsets are proven — prefer them)\n"

def main():
    bundles_dir = Path(sys.argv[1]); manifest = json.load(open(sys.argv[2]))
    cache = {}
    n = 0
    for item in manifest:
        mod = item.get("module", "_global")
        if mod in (None, "", "_global"):
            continue
        if mod not in cache:
            try:
                cache[mod] = class_struct.render(mod)
            except Exception:
                cache[mod] = None
        struct = cache[mod]
        if not struct:
            continue
        bp = Path(item["bundle"])
        if not bp.exists():
            continue
        txt = bp.read_text(encoding="utf-8", errors="ignore")
        if MARK in txt:
            txt = txt.split(MARK)[0]  # drop stale injection
        bp.write_text(txt.rstrip() + "\n" + MARK + "```cpp\n" + struct + "\n```\n", encoding="utf-8")
        n += 1
    print(f"[augment_bundles] injected reconstructed structs into {n}/{len(manifest)} bundles", file=sys.stderr)

if __name__ == "__main__":
    main()
