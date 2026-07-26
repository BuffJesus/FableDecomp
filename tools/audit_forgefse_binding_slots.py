#!/usr/bin/env python3
"""Compare ForgeFSE CGSI vtable entries with the retail slot/address oracle."""
from __future__ import annotations

import argparse
import csv
import json
import re
from collections import defaultdict
from pathlib import Path


ASSIGNMENT_RE = re.compile(
    r"\b([A-Za-z_][A-Za-z0-9_]*)_API\s*=\s*\([^;\n]+?\)pVTable\[(\d+)\]"
)


def address(value: str) -> str:
    return f"0x{int(value, 16):08X}"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    parser.add_argument("--forgefse-root", type=Path, default=Path(r"D:\Code\ForgeFSE"))
    parser.add_argument("--output", type=Path)
    parser.add_argument("--markdown", type=Path)
    parser.add_argument("--allow-mismatch", action="store_true")
    args = parser.parse_args()

    root = args.root.resolve()
    forgefse_root = args.forgefse_root.resolve()
    source_path = forgefse_root / "FableScriptExtender" / "GameInterface.cpp"
    if not source_path.exists():
        print(f"SKIP ForgeFSE source not found: {source_path}")
        return 0

    overlay = json.loads((root / "rebuild" / "sdk" / "fse_native_overlay.json").read_text(encoding="utf-8-sig"))
    if not overlay.get("policy", {}).get("recommendationIsNotHookApproval"):
        raise SystemExit("native overlay safety policy is missing")

    slots: dict[int, dict[str, str]] = {}
    with (root / "ghidra_out" / "cgsi_slots_aligned.tsv").open(
        "r", encoding="utf-8-sig", newline=""
    ) as stream:
        for row in csv.DictReader(stream, delimiter="\t"):
            slots[int(row["# slot"])] = row

    assignments: dict[str, list[int]] = defaultdict(list)
    source = source_path.read_text(encoding="utf-8-sig", errors="replace")
    for match in ASSIGNMENT_RE.finditer(source):
        assignments[match.group(1)].append(int(match.group(2)))

    rows: list[dict[str, object]] = []
    for function in overlay.get("functions", []):
        if function.get("scope") != "Quest" or not function.get("recommendedAddress"):
            continue
        name = str(function.get("name", ""))
        candidate_slots: list[int] = []
        for variable, variable_slots in assignments.items():
            if variable == name or variable.startswith(name + "_"):
                candidate_slots.extend(variable_slots)
        candidate_slots = sorted(set(candidate_slots))
        recommended = address(str(function["recommendedAddress"]))
        matching_slots = [
            slot for slot in candidate_slots
            if slot in slots and address(slots[slot]["addr"]) == recommended
        ]
        if matching_slots:
            status = "MATCH"
        elif candidate_slots:
            status = "MISMATCH"
        else:
            status = "MISSING"
        rows.append(
            {
                "api_name": name,
                "recommended_address": recommended,
                "binding_slots": ";".join(str(slot) for slot in candidate_slots),
                "binding_addresses": ";".join(
                    address(slots[slot]["addr"]) for slot in candidate_slots if slot in slots
                ),
                "matching_slots": ";".join(str(slot) for slot in matching_slots),
                "status": status,
            }
        )

    output = (args.output or root / "rebuild" / "validation" / "forgefse-binding-audit.tsv").resolve()
    markdown = (args.markdown or root / "rebuild" / "validation" / "FORGEFSE_BINDING_AUDIT.md").resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    fields = [
        "api_name", "recommended_address", "binding_slots", "binding_addresses",
        "matching_slots", "status",
    ]
    with output.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=fields, delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(rows)

    matched = sum(row["status"] == "MATCH" for row in rows)
    mismatched = sum(row["status"] == "MISMATCH" for row in rows)
    missing = sum(row["status"] == "MISSING" for row in rows)
    lines = [
        "# ForgeFSE CGSI binding audit",
        "",
        "Compares ForgeFSE's `pVTable[N]` assignments with the retail CGSI slot map and the",
        "current owner-aligned overlay recommendation. A match validates the entry address/slot;",
        "it does not by itself approve the hook or prove wrapper behavior.",
        "",
        f"- Quest recommendations audited: {len(rows)}",
        f"- Exact slot/address matches: {matched}",
        f"- Address mismatches: {mismatched}",
        f"- Missing assignment families: {missing}",
        "",
        "| FSE API | Recommended | ForgeFSE slots | ForgeFSE addresses | Status |",
        "|---|---|---|---|---|",
    ]
    for row in rows:
        if row["status"] != "MATCH":
            lines.append(
                f"| `{row['api_name']}` | `{row['recommended_address']}` | "
                f"`{row['binding_slots']}` | `{row['binding_addresses']}` | {row['status']} |"
            )
    markdown.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(
        f"forgefse_quest_bindings={len(rows)} matched={matched} "
        f"mismatched={mismatched} missing={missing}"
    )
    if not args.allow_mismatch and (mismatched or missing):
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
