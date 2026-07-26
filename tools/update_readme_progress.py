#!/usr/bin/env python3
"""Synchronize public README progress rows with canonical rebuild reports."""
from __future__ import annotations

import argparse
import json
import re
from datetime import datetime
from pathlib import Path


def percentage(value: int, total: int, digits: int = 3) -> str:
    return f"{(100.0 * value / total) if total else 0.0:.{digits}f}%"


def replace_row(text: str, label: str, value: str) -> str:
    pattern = re.compile(
        rf"(?m)^(\|\s*[^|]+\|\s*{re.escape(label)}\s*\|\s*)[^|]+(\|)$"
    )
    updated, count = pattern.subn(rf"\g<1>{value} \g<2>", text)
    if count != 1:
        raise RuntimeError(f"README row not found exactly once: {label!r}")
    return updated


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()

    root = args.root.resolve()
    readme = root / "README.md"
    coverage = json.loads((root / "rebuild" / "COVERAGE.json").read_text(encoding="utf-8-sig"))
    parity = json.loads(
        (root / "rebuild" / "compile-gate" / "retail-parity.json").read_text(
            encoding="utf-8-sig"
        )
    )
    total = int(coverage["catalog_functions"])
    matches = int(parity["matching"]) + int(parity["relocation_matching"])

    values = {
        "Functions catalogued": f"**{total:,}**",
        "Mechanically named (no `FUN_*`)": percentage(
            int(coverage["mechanically_named"]), total
        ),
        "Usable reconstruction/navigation names": percentage(
            int(coverage["accepted_name_quality"]), total
        ),
        "Calling convention known": percentage(
            int(coverage["calling_convention_known"]), total
        ),
        "Complete non-`undefined` prototype": percentage(
            int(coverage["prototype_complete"]), total
        ),
        "Curated sources, VC7.1-compiled **and** behaviour-gated": (
            f"**{int(coverage['compiled_candidates']):,}**"
        ),
        "Retail `.text` match (exact + relocation-normalized)": (
            f"**{matches:,}** ({percentage(matches, total, 2)})"
        ),
        "— of which byte-**identical** (no relocation masking)": (
            f"{int(parity['matching']):,} ({percentage(int(parity['matching']), total, 2)})"
        ),
        "Compiled sources still honestly `DIFFER`": f"{int(parity['differing']):,}",
        "Compiled rows lacking a Ghidra function-start oracle": (
            f"{int(parity['oracle_missing']):,}"
        ),
        "Generated candidates / structural checker PASS": (
            f"{int(coverage['agent_candidates']):,} / {int(coverage['agent_pass']):,}"
        ),
    }

    text = readme.read_text(encoding="utf-8")
    for label, value in values.items():
        text = replace_row(text, label, value)

    generated = datetime.fromisoformat(coverage["generated_at"]).date().isoformat()
    text, count = re.subn(
        r"Counts above are from the \d{4}-\d{2}-\d{2} canonical refresh:",
        f"Counts above are from the {generated} canonical refresh:",
        text,
        count=1,
    )
    if count != 1:
        raise RuntimeError("canonical refresh date sentence not found")

    temporary = readme.with_suffix(".md.tmp")
    temporary.write_text(text, encoding="utf-8")
    temporary.replace(readme)
    print(
        f"readme_progress=updated catalog={total} compiled={coverage['compiled_candidates']} "
        f"agent={coverage['agent_candidates']}/{coverage['agent_pass']}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
