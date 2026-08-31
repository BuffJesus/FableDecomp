#!/usr/bin/env python3
"""Create a deterministic, evidence-gated native game-script conversion queue."""

from __future__ import annotations

import argparse
import csv
import json
from pathlib import Path
from typing import Any


KIND_PRIORITY = {
    "quest": 20, "repeatable_quest": 25, "village": 30, "global": 40,
    "expression": 42, "personal": 45, "cutscene_host": 48, "master": 50,
    "script": 60, "test": 90,
}


def build(catalog_path: Path, json_output: Path, tsv_output: Path) -> dict[str, int]:
    catalog = json.loads(catalog_path.read_text(encoding="utf-8-sig"))
    seeds = {row["nativeName"]: row for row in catalog.get("seedCorrelations", []) if row["status"] == "matched"}
    queue: list[dict[str, Any]] = []
    for script in catalog["scripts"]:
        seeded = script["name"] in seeds
        address_known = bool(script["allocatorAddress"])
        queue.append({
            "priority": (0 if seeded else KIND_PRIORITY.get(script["kind"], 80)) + (0 if address_known else 5),
            "name": script["name"],
            "kind": script["kind"],
            "section": script["section"] or "",
            "allocatorAddress": script["allocatorAddress"] or "",
            "seedPackage": seeds[script["name"]]["package"] if seeded else "",
            "stage": "resolve-object-boundaries" if address_known else "resolve-allocator-address",
            "evidence": "registry-fact",
            "requiredGates": "typed-decompile;api-map;state-map;persistence-map;static-validate;trace-review",
        })
    queue.sort(key=lambda row: (row["priority"], row["name"]))
    payload = {"schema": "fable-script-conversion-queue/0.1", "source": str(catalog_path.resolve()), "queue": queue}
    json_output.parent.mkdir(parents=True, exist_ok=True)
    json_output.write_text(json.dumps(payload, indent=2) + "\n", encoding="utf-8")
    with tsv_output.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.DictWriter(stream, fieldnames=list(queue[0]), delimiter="\t", lineterminator="\n")
        writer.writeheader()
        writer.writerows(queue)
    return {"total": len(queue), "seeded": len(seeds),
            "allocatorAddressKnown": sum(bool(row["allocatorAddress"]) for row in queue)}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--catalog", type=Path, required=True)
    parser.add_argument("--json", type=Path, required=True)
    parser.add_argument("--tsv", type=Path, required=True)
    args = parser.parse_args()
    print(json.dumps(build(args.catalog.resolve(), args.json.resolve(), args.tsv.resolve()), sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
