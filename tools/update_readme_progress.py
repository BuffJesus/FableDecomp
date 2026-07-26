#!/usr/bin/env python3
"""Synchronize public README progress rows with canonical rebuild reports."""
from __future__ import annotations

import argparse
import csv
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


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8-sig", newline="") as handle:
        return list(csv.DictReader(handle, delimiter="\t"))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()

    root = args.root.resolve()
    readme = root / "README.md"
    coverage = json.loads((root / "rebuild" / "COVERAGE.json").read_text(encoding="utf-8-sig"))
    status = json.loads(
        (root / "rebuild" / "manifest" / "status.json").read_text(
            encoding="utf-8-sig"
        )
    )
    parity = json.loads(
        (root / "rebuild" / "compile-gate" / "retail-parity.json").read_text(
            encoding="utf-8-sig"
        )
    )
    total = int(status["catalog_functions"])
    matches = int(parity["matching"]) + int(parity["relocation_matching"])
    gfmain_calls = read_tsv(root / "rebuild" / "integration" / "gfmain_calls.tsv")
    gfmain_phases = read_tsv(root / "rebuild" / "integration" / "gfmain_phases.tsv")
    matching_grades = {"MATCH", "EXACT", "RELOCATION_MATCH"}
    proven_calls = sum(
        row.get("validated_grade") in matching_grades for row in gfmain_calls
    )
    phase3_calls = [row for row in gfmain_calls if row.get("phase") == "3"]
    phase3_proven = sum(
        row.get("validated_grade") in matching_grades for row in phase3_calls
    )
    authored_phases = sum(
        (root / "rebuild" / "integration" / f"gfmain_phase{phase}.cpp").exists()
        for phase in range(1, len(gfmain_phases) + 1)
    )

    values = {
        "Functions catalogued": f"**{total:,}**",
        "Mechanically named (no `FUN_*`)": percentage(
            int(coverage["mechanically_named"]), total
        ),
        "Usable reconstruction/navigation names": percentage(
            int(coverage["accepted_name_quality"]), total
        ),
        "Calling convention known": percentage(
            int(status["calling_convention_known"]), total
        ),
        "Complete non-`undefined` prototype": percentage(
            int(status["prototype_complete"]), total
        ),
        "Curated sources, VC7.1-compiled **and** behaviour-gated": (
            f"**{int(status['compiled_candidates']):,}**"
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
            f"{int(status['agent_candidates']):,} / {int(status['agent_pass']):,}"
        ),
        "GFMain direct-call sites proven": (
            f"**{proven_calls:,} / {len(gfmain_calls):,}** "
            f"({percentage(proven_calls, len(gfmain_calls), 2)})"
        ),
        "Callable authored GFMain phases": (
            f"**{authored_phases} / {len(gfmain_phases)}** "
            f"({percentage(authored_phases, len(gfmain_phases), 2)})"
        ),
        "Current Phase 3 direct calls proven": (
            f"**{phase3_proven} / {len(phase3_calls)}** "
            f"({percentage(phase3_proven, len(phase3_calls), 2)})"
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

    strict_parity = percentage(matches, total, 2)
    text, count = re.subn(
        r"(current verified retail\s+parity is \*\*)\d+(?:\.\d+)?%(\*\*)",
        rf"\g<1>{strict_parity}\g<2>",
        text,
        count=1,
    )
    if count != 1:
        raise RuntimeError("current verified parity sentence not found")
    text, count = re.subn(
        r"(The )\d+(?:\.\d+)?%( figure is intentionally the strict,)",
        rf"\g<1>{strict_parity}\g<2>",
        text,
        count=1,
    )
    if count != 1:
        raise RuntimeError("strict denominator sentence not found")

    temporary = readme.with_suffix(".md.tmp")
    temporary.write_text(text, encoding="utf-8")
    temporary.replace(readme)
    print(
        f"readme_progress=updated catalog={total} compiled={status['compiled_candidates']} "
        f"agent={status['agent_candidates']}/{status['agent_pass']}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
