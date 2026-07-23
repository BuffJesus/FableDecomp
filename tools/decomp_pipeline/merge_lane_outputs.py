#!/usr/bin/env python3
"""Merge chunked authoring-lane outputs into verify_and_land.py input.

Usage:
  python tools/decomp_pipeline/merge_lane_outputs.py <batch> <lane_dir> [parts=1,2,3,4]

Each input is expected at:
  <lane_dir>/<batch>_partN_output.json

Writes:
  <lane_dir>/<batch>_all_merged.json
"""

import json
import sys
from pathlib import Path


def rows_from_payload(payload):
    if isinstance(payload, dict):
        result = payload.get("result")
        if isinstance(result, dict) and isinstance(result.get("authored"), list):
            return result["authored"]
        if isinstance(payload.get("authored"), list):
            return payload["authored"]
    raise ValueError("output JSON does not contain result.authored or authored")


def main(argv):
    if len(argv) not in (3, 4):
        print(__doc__, file=sys.stderr)
        return 2

    batch = argv[1]
    lane_dir = Path(argv[2])
    parts = [int(p) for p in (argv[3] if len(argv) == 4 else "1,2,3,4").split(",") if p.strip()]

    authored = []
    summaries = []
    for part in parts:
        path = lane_dir / f"{batch}_part{part}_output.json"
        with path.open("r", encoding="utf-8") as f:
            payload = json.load(f)
        rows = rows_from_payload(payload)
        authored.extend(rows)
        summaries.append(payload.get("summary", {"part": part, "authored": len(rows)}))

    out = lane_dir / f"{batch}_all_merged.json"
    merged = {
        "result": {"authored": authored},
        "summary": {
            "batch": batch,
            "parts": parts,
            "authored": len(authored),
            "self_wins": sum(1 for row in authored if row.get("win")),
            "inputs": summaries,
        },
    }
    with out.open("w", encoding="utf-8") as f:
        json.dump(merged, f, indent=2)
        f.write("\n")
    print(f"{out} authored={len(authored)} self_wins={merged['summary']['self_wins']}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv))
