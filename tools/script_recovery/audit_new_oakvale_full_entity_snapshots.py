#!/usr/bin/env python3
"""Require complete reproducible retail snapshots for every New Oakvale entity."""
from __future__ import annotations
import json
from pathlib import Path

FULL_NAMES = {"fullInitMain", "fullDispatcher", "fullLifecycle"}

def audit(root: Path) -> dict:
    entity_root = root / "refs/script_recovery/new_oakvale_intro/entities"
    rows, errors = [], []
    for path in sorted(entity_root.glob("NOVI_*.json")):
        inventory = json.loads(path.read_text(encoding="utf-8-sig"))
        snapshots = inventory.get("retailByteSnapshots", [])
        complete = [row for row in snapshots if row.get("name") in FULL_NAMES]
        valid = False
        files = []
        for row in complete:
            snapshot_path = root / row.get("file", "")
            files.append(row.get("file"))
            if snapshot_path.is_file():
                snapshot = json.loads(snapshot_path.read_text(encoding="utf-8-sig"))
                regions = snapshot.get("regions", [])
                valid = valid or bool(regions) and all(region.get("terminal") == "ret" for region in regions)
        if not valid:
            errors.append(f"{path.stem}: missing valid complete retail snapshot")
        rows.append({"entity": path.stem, "complete": valid, "files": files})
    return {"schema":"new-oakvale-full-entity-snapshots-audit/0.1", "entities":len(rows),
            "complete":sum(row["complete"] for row in rows), "rows":rows, "errors":errors, "ok":not errors}

if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
