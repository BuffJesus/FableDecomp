#!/usr/bin/env python3
"""Validate cleanup/lifetime evidence structure and its operation references."""
from __future__ import annotations

import json
import re
from pathlib import Path


def _sequence_references(text: str) -> set[int]:
    refs: set[int] = set()
    for match in re.finditer(r"(?i)\bseq(?:uence)?s?\s+([0-9][0-9, /-]*)", text):
        fragment = match.group(1)
        for start, end in re.findall(r"(\d+)(?:\s*-\s*(\d+))?", fragment):
            first = int(start)
            last = int(end) if end else first
            refs.update(range(min(first, last), max(first, last) + 1))
    return refs


def audit(entities_dir: Path) -> dict:
    errors: list[str] = []
    records = files = referenced = 0
    for inventory in sorted(entities_dir.glob("*.json")):
        data = json.loads(inventory.read_text(encoding="utf-8-sig"))
        rows = data.get("cleanupPaths", [])
        if not rows:
            continue
        files += 1
        valid_sequences = {int(row["seq"]) for row in data.get("operations", [])}
        for index, row in enumerate(rows):
            records += 1
            label = f"{inventory.name} cleanupPaths[{index}]"
            required = {"resource", "acquired", "releasedOn", "missingOn"}
            missing = required - set(row)
            if missing:
                errors.append(f"{label}: missing keys {sorted(missing)}")
                continue
            if not isinstance(row["resource"], str) or not row["resource"].strip():
                errors.append(f"{label}: resource must be non-empty text")
            if not isinstance(row["acquired"], str) or not row["acquired"].strip():
                errors.append(f"{label}: acquired must be non-empty text")
                continue
            for key in ("releasedOn", "missingOn"):
                value = row[key]
                if not isinstance(value, list) or not all(
                        isinstance(item, str) and item.strip() for item in value):
                    errors.append(f"{label}: {key} must be a string list")
            refs = _sequence_references(row["acquired"])
            referenced += len(refs)
            unknown = refs - valid_sequences
            if unknown:
                errors.append(f"{label}: acquisition references unknown operations {sorted(unknown)}")
            if not refs and not re.search(
                    r"(?i)\b(n/a|elsewhere|quest-owned|owned by other entities)\b", row["acquired"]):
                errors.append(f"{label}: acquisition has neither operation evidence nor ownership classification")
            if not row["releasedOn"] and not row["missingOn"]:
                note = str(row.get("note", ""))
                if not re.search(r"(?i)\b(n/a|owned|only reads/sets)\b", row["acquired"] + " " + note):
                    errors.append(f"{label}: neither release nor missing path is classified")
    return {
        "schema": "new-oakvale-cleanup-path-audit/0.1",
        "filesWithCleanupEvidence": files,
        "cleanupRecords": records,
        "acquisitionSequenceReferences": referenced,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(root / "refs/script_recovery/new_oakvale_intro/entities"), indent=2))
