#!/usr/bin/env python3
"""Tag hand-written machine-code landings with grade `asm_bake` in the build catalog.

    python tag_asm_bakes.py            # report only
    python tag_asm_bakes.py --apply    # annotate rebuild/build_candidates.ps1 + write the list

A landed source that is `__declspec(naked)` / `__asm` / `_emit` bytes passes the byte-parity
gate trivially but is an oracle, not a reconstruction. The catalog entry gets
`Grade = 'asm_bake'` (idempotent), the dashboard subtracts these from the genuine C++ counts
(tools/write_decomp_dashboard.py via header_metrics.json), and the list is written to
rebuild/backlog/asm_bake_list.tsv as the de-bake worklist. Files stay where they are so the 28
tools that address `rebuild/src/compiled` keep working; the grade, not the path, is the marker.
"""
from __future__ import annotations

import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
import label_trust  # noqa: E402

CATALOG = ROOT / "rebuild" / "build_candidates.ps1"
LIST = ROOT / "rebuild" / "backlog" / "asm_bake_list.tsv"


def main() -> int:
    apply = "--apply" in sys.argv
    mf = label_trust.Manifest()
    bakes = []
    for r in mf.landed():
        p = Path(r["compiled_source"])
        if not p.exists():
            continue
        kind = label_trust.classify(p)
        if kind != "genuine":
            bakes.append((r["address"].lower().zfill(8), r["module"], r["name"], kind,
                          r.get("retail_parity", ""), p.name))
    text = CATALOG.read_text(encoding="utf-8")
    tagged = already = missing = 0
    for addr, *_ in bakes:
        m = re.search(r"(\[pscustomobject\]@\{\s*Address = '" + addr + r"'.*?)(\n\s*\})", text, re.S)
        if not m:
            missing += 1
            continue
        blk = m.group(1)
        if "Grade = 'asm_bake'" in blk:
            already += 1
            continue
        indent = re.search(r"\n(\s*)Source = ", blk)
        ind = indent.group(1) if indent else "        "
        new_blk = blk + f"\n{ind}Grade = 'asm_bake'"
        text = text[:m.start(1)] + new_blk + text[m.end(1):]
        tagged += 1
    print(f"asm bakes: {len(bakes)} (naked_asm {sum(1 for b in bakes if b[3]=='naked_asm')}, "
          f"emit_bake {sum(1 for b in bakes if b[3]=='emit_bake')}); catalog tagged now={tagged} "
          f"already={already} no_catalog_block={missing}")
    if apply:
        CATALOG.write_text(text, encoding="utf-8")
        LIST.write_text("address\tmodule\tname\tkind\tretail_parity\tfile\n" +
                        "\n".join("\t".join(b) for b in sorted(bakes)) + "\n", encoding="utf-8")
        print(f"wrote {LIST}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
