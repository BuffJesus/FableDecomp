#!/usr/bin/env python3
"""Build an address-keyed queue for decompiling conversion-relevant native helpers."""

from __future__ import annotations

import argparse
import csv
import json
from collections import Counter
from pathlib import Path
from typing import Any

try:
    from tools.script_recovery.analyze_native_conversion_readiness import native_helper_category
except ModuleNotFoundError:  # direct execution from this directory
    from analyze_native_conversion_readiness import native_helper_category


def build(readiness_path: Path, ir_dir: Path) -> list[dict[str, Any]]:
    readiness = json.loads(readiness_path.read_text(encoding="utf-8-sig"))
    unresolved = {row["name"] for row in readiness["nativeHelperBacklog"]}
    targets: dict[str, dict[str, Any]] = {}
    for path in sorted(ir_dir.glob("*.json")):
        ir = json.loads(path.read_text(encoding="utf-8-sig"))
        for life in ir["lifecycle"]:
            for call in life["calls"]:
                address = call.get("targetAddress")
                if call["callee"] not in unresolved or not address:
                    continue
                row = targets.setdefault(address, {
                    "targetAddress": address, "calls": 0, "helperNames": set(),
                    "scripts": set(), "roles": set(), "sites": [],
                })
                row["calls"] += 1
                row["helperNames"].add(call["callee"])
                row["scripts"].add(ir["script"])
                row["roles"].add(life["role"])
                row["sites"].append({"script": ir["script"], "role": life["role"],
                                     "site": call["directCallSite"], "callee": call["callee"]})
    result = []
    for row in targets.values():
        names = sorted(row["helperNames"])
        roles = sorted(row["roles"])
        categories = Counter(native_helper_category(name, set(roles)) for name in names)
        result.append({
            "targetAddress": row["targetAddress"], "category": categories.most_common(1)[0][0],
            "calls": row["calls"], "helperNames": names,
            "scripts": sorted(row["scripts"]), "roles": roles, "sites": row["sites"],
        })
    result.sort(key=lambda row: (-len(row["scripts"]), -row["calls"], row["targetAddress"]))
    return result


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--readiness", type=Path, required=True)
    parser.add_argument("--ir", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    rows = build(args.readiness, args.ir)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    with args.output.open("w", encoding="utf-8", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("target_address", "category", "calls", "scripts", "roles", "helper_names"))
        for row in rows:
            writer.writerow((row["targetAddress"], row["category"], row["calls"],
                             len(row["scripts"]), ",".join(row["roles"]),
                             " | ".join(row["helperNames"])))
    print(json.dumps({"targets": len(rows), "calls": sum(row["calls"] for row in rows),
                      "output": str(args.output.resolve())}, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
