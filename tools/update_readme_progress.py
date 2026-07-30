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


def replace_snapshot_row(text: str, label: str, value: str) -> str:
    pattern = re.compile(
        rf"(?m)^(\|\s*{re.escape(label)}\s*\|\s*)[^|]+(\|)$"
    )
    updated, count = pattern.subn(rf"\g<1>{value} \g<2>", text)
    if count != 1:
        raise RuntimeError(
            f"README snapshot row not found exactly once: {label!r}"
        )
    return updated


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open(encoding="utf-8-sig", newline="") as handle:
        return list(csv.DictReader(handle, delimiter="\t"))


def verified_public_counts(
    status: dict[str, object],
    parity: dict[str, object],
    coverage: dict[str, object],
) -> tuple[int, int]:
    """Return whole-project verified and byte-identical function counts."""
    candidate_matches = (
        int(parity["matching"]) + int(parity["relocation_matching"])
    )
    verified = (
        candidate_matches
        + int(status["lift_matching"])
        + int(status["lift_functional"])
    )
    byte_identical = (
        int(parity["matching"]) + int(status["lift_matching"])
    )
    if int(coverage["verified_lifted"]) != verified:
        raise RuntimeError(
            "coverage/README verified-function sources disagree: "
            f"coverage={coverage['verified_lifted']} computed={verified}"
        )
    if int(coverage["candidate_retail_matches"]) != int(parity["matching"]):
        raise RuntimeError(
            "coverage/parity exact-match sources disagree: "
            f"coverage={coverage['candidate_retail_matches']} "
            f"parity={parity['matching']}"
        )
    return verified, byte_identical


def replace_summary_denominator(text: str, total: int) -> str:
    """Keep the prose denominator synchronized with the dashboard table."""
    updated, count = re.subn(
        r"(functional-or-matching coverage is \*\*"
        r"\d+(?:\.\d+)?%\*\* of the )"
        r"\d{1,3}(?:,\d{3})*"
        r"(-function catalog\.)",
        rf"\g<1>{total:,}\g<2>",
        text,
        count=1,
    )
    if count != 1:
        raise RuntimeError(
            "current functional-or-matching catalog denominator not found")
    return updated


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
    naming_triage = json.loads(
        (
            root
            / "ghidra_out"
            / "naming_stragglers"
            / "quality_triage.json"
        ).read_text(encoding="utf-8-sig")
    )
    total = int(status["catalog_functions"])
    verified, byte_identical = verified_public_counts(
        status, parity, coverage
    )
    gfmain_calls = read_tsv(root / "rebuild" / "integration" / "gfmain_calls.tsv")
    gfmain_phases = read_tsv(root / "rebuild" / "integration" / "gfmain_phases.tsv")
    matching_grades = {"MATCH", "EXACT", "RELOCATION_MATCH"}
    proven_calls = sum(
        row.get("validated_grade") in matching_grades for row in gfmain_calls
    )
    current_phase = len(gfmain_phases)
    current_phase_calls = [
        row
        for row in gfmain_calls
        if row.get("phase") == str(current_phase)
    ]
    current_phase_proven = sum(
        row.get("validated_grade") in matching_grades
        for row in current_phase_calls
    )
    authored_phases = sum(
        (root / "rebuild" / "integration" / f"gfmain_phase{phase}.cpp").exists()
        for phase in range(1, len(gfmain_phases) + 1)
    )

    snapshot_values = {
        "Catalogued retail functions": f"**{total:,}**",
        "Verified functional or matching reconstruction": (
            f"**{verified:,} · {percentage(verified, total, 2)}**"
        ),
        "Byte-identical reconstruction": (
            f"**{byte_identical:,} · "
            f"{percentage(byte_identical, total, 2)}**"
        ),
    }
    values = {
        "Functions catalogued": f"**{total:,}**",
        "Mechanically named (no `FUN_*`)": percentage(
            int(coverage["mechanically_named"]), total
        ),
        "Accepted naming quality": percentage(
            int(coverage["accepted_name_quality"]), total
        ),
        "Usable reconstruction/navigation names": percentage(
            int(naming_triage["usable_navigation_names"]), total
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
        "Verified functional or matching C++": (
            f"**{verified:,}** ({percentage(verified, total, 2)})"
        ),
        "— of which byte-**identical** C++": (
            f"{byte_identical:,} ({percentage(byte_identical, total, 2)})"
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
        f"Current Phase {current_phase} direct calls proven": (
            f"**{current_phase_proven} / {len(current_phase_calls)}** "
            f"({percentage(current_phase_proven, len(current_phase_calls), 2)})"
        ),
    }

    text = readme.read_text(encoding="utf-8")
    for label, value in snapshot_values.items():
        text = replace_snapshot_row(text, label, value)
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

    strict_parity = percentage(verified, total, 2)
    text, count = re.subn(
        r"(functional-or-matching coverage is \*\*)"
        r"\d+(?:\.\d+)?%(\*\*)",
        rf"\g<1>{strict_parity}\g<2>",
        text,
        count=1,
    )
    if count != 1:
        raise RuntimeError(
            "current functional-or-matching coverage sentence not found")
    text = replace_summary_denominator(text, total)
    exact_parity = percentage(byte_identical, total, 2)
    text, count = re.subn(
        r"(Of that verified set, \*\*)"
        r"\d+(?:\.\d+)?%(\*\* is byte-identical C\+\+\.)",
        rf"\g<1>{exact_parity}\g<2>",
        text,
        count=1,
    )
    if count != 1:
        raise RuntimeError("current byte-identical coverage sentence not found")
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
