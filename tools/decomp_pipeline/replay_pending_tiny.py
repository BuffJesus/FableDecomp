#!/usr/bin/env python3
"""Re-run deterministic authoring patterns over staged, unlanded oracle rows.

The selector intentionally keeps attempted rows under rebuild/oracles/pending so
later pattern improvements can recover them. This replay step combines those
rows, filters addresses already present in the canonical oracle catalog, and
writes one ordinary oracle/authored pair for verify_and_land.py.

Usage:
  python replay_pending_tiny.py <out_oracle.tsv> <out_authored.json> [limit]
"""

import csv
import json
import sys
from pathlib import Path

from auto_author_tiny import candidate


ROOT = Path(r"D:\Documents\FableTLC")
PENDING = ROOT / "rebuild" / "oracles" / "pending"
CANONICAL = ROOT / "rebuild" / "oracles" / "auto-re-candidates.tsv"
MANIFEST = ROOT / "rebuild" / "manifest" / "functions.tsv"
FIELDS = ["address", "name", "length", "bytes"]


def rows(path: Path):
    with open(path, encoding="utf-8-sig", newline="") as stream:
        yield from csv.DictReader(stream, delimiter="\t")


def main():
    if len(sys.argv) not in (3, 4):
        raise SystemExit(__doc__)
    out_oracle = Path(sys.argv[1])
    out_authored = Path(sys.argv[2])
    limit = int(sys.argv[3]) if len(sys.argv) == 4 else 500

    landed = {
        row["address"].lower().replace("0x", "")
        for row in rows(CANONICAL)
    }
    manifest = {
        row["address"].lower().replace("0x", "")
        for row in rows(MANIFEST)
        if row.get("address")
    }
    seen = set(landed)
    selected = []
    authored = []

    for path in sorted(PENDING.glob("*_oracle.tsv")):
        if path.resolve() == out_oracle.resolve():
            continue
        for row in rows(path):
            address = row["address"].lower().replace("0x", "")
            if address in seen or address not in manifest:
                continue
            authored_candidate = candidate(row)
            if authored_candidate is None:
                continue
            seen.add(address)
            selected.append({field: row.get(field, "") for field in FIELDS})
            authored.append(authored_candidate)
            if len(authored) >= limit:
                break
        if len(authored) >= limit:
            break

    out_oracle.parent.mkdir(parents=True, exist_ok=True)
    with open(out_oracle, "w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(
            stream,
            delimiter="\t",
            fieldnames=FIELDS,
            lineterminator="\n",
        )
        writer.writeheader()
        writer.writerows(selected)
    out_authored.parent.mkdir(parents=True, exist_ok=True)
    with open(out_authored, "w", encoding="utf-8") as stream:
        json.dump({"result": {"authored": authored}}, stream, indent=1)

    print(
        f"replayed {len(authored)} deterministic candidates from pending oracles "
        f"-> {out_authored}"
    )


if __name__ == "__main__":
    main()
