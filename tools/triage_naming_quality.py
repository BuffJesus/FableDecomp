#!/usr/bin/env python3
"""Separate usable reconstruction labels from unresolved original identities."""
from __future__ import annotations

import argparse
import csv
import json
from collections import Counter
from datetime import datetime
from pathlib import Path


ACCEPTED_FAMILIES = {
    "initializer-destructor",
    "allocator-wrapper",
    "container-template",
    "thunk-stub-wrapper",
}


def read_tsv(path: Path) -> list[dict[str, str]]:
    with path.open("r", encoding="utf-8-sig", newline="") as stream:
        return list(csv.DictReader(stream, delimiter="\t"))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1])
    args = parser.parse_args()
    root = args.root.resolve()
    naming = root / "ghidra_out" / "naming_stragglers"
    manifest = read_tsv(naming / "manifest.tsv")
    proposals = {row["address"]: row for row in read_tsv(naming / "proposals.tsv")} if (naming / "proposals.tsv").exists() else {}
    status = json.loads((root / "rebuild" / "manifest" / "status.json").read_text(encoding="utf-8"))

    decisions: list[dict[str, str]] = []
    for row in manifest:
        address = row["address"]
        category = row["category"]
        if address in proposals:
            disposition = "donor-proposal-review"
            rationale = "unique strict donor signature; proposal not yet applied"
        elif category in ACCEPTED_FAMILIES:
            disposition = "accepted-reconstruction-family"
            rationale = "stable descriptive family label; original template/deleting-helper identity remains unknown"
        elif category == "invalid-boundary":
            disposition = "invalid-boundary-review"
            rationale = "verify or remove the function boundary before naming"
        else:
            disposition = "semantic-review"
            rationale = "semantic owner or behavior still needs stronger evidence"
        decisions.append(
            {
                "address": address,
                "current_name": row["current_name"],
                "category": category,
                "disposition": disposition,
                "proposed_name": proposals.get(address, {}).get("proposed_name", ""),
                "rationale": rationale,
            }
        )

    output = naming / "quality_triage.tsv"
    temp = output.with_suffix(".tsv.tmp")
    with temp.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, delimiter="\t", fieldnames=list(decisions[0]), lineterminator="\n")
        writer.writeheader()
        writer.writerows(decisions)
    temp.replace(output)

    dispositions = Counter(row["disposition"] for row in decisions)
    total = status["catalog_functions"]
    evidence_accepted = total - status["naming_quality_review"]
    reconstruction_accepted = dispositions["accepted-reconstruction-family"]
    usable = evidence_accepted + reconstruction_accepted
    summary = {
        "generated_at": datetime.now().astimezone().isoformat(timespec="seconds"),
        "catalog_functions": total,
        "evidence_accepted": evidence_accepted,
        "accepted_reconstruction_family": reconstruction_accepted,
        "usable_navigation_names": usable,
        "remaining_semantic_review": dispositions["semantic-review"],
        "donor_proposal_review": dispositions["donor-proposal-review"],
        "invalid_boundary_review": dispositions["invalid-boundary-review"],
        "dispositions": dict(dispositions),
    }
    temp_json = naming / "quality_triage.json.tmp"
    temp_json.write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
    temp_json.replace(naming / "quality_triage.json")

    coverage = 100.0 * usable / total
    lines = [
        "# Naming quality triage",
        "",
        f"Generated: `{summary['generated_at']}`",
        "",
        f"Evidence-accepted/original-quality names: **{evidence_accepted:,} / {total:,}**.",
        f"Usable navigation names after accepting explicit reconstruction-family labels: **{usable:,} / {total:,} ({coverage:.3f}%)**.",
        "",
        "This does not claim that compiler-generated/template helper names are original Lionhead symbols. It accepts their current descriptive family labels for reconstruction navigation while retaining the missing original identity in the ledger.",
        "",
        "## Remaining focused work",
        "",
        f"- Semantic/accessor review: {summary['remaining_semantic_review']}",
        f"- Strict donor proposals awaiting review: {summary['donor_proposal_review']}",
        f"- Invalid function boundary: {summary['invalid_boundary_review']}",
        "",
        "Every tail decision is recorded in `quality_triage.tsv`.",
        "",
    ]
    temp_md = naming / "QUALITY_TRIAGE.md.tmp"
    temp_md.write_text("\n".join(lines), encoding="utf-8")
    temp_md.replace(naming / "QUALITY_TRIAGE.md")
    print(json.dumps(summary, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
