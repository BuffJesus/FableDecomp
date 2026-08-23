#!/usr/bin/env python3
"""Remove a map slot from one compiled-world region's contains/sees lists."""

from __future__ import annotations

import argparse
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import wld_bwd


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("source", type=Path)
    ap.add_argument("output", type=Path)
    ap.add_argument("map_slot", type=int)
    ap.add_argument("region_name")
    args = ap.parse_args()

    world = wld_bwd.Bwd.parse(args.source.read_bytes())
    region = next((r for r in world.regions if r.name == args.region_name), None)
    if region is None:
        raise SystemExit(f"region not found: {args.region_name}")
    region.contains = [slot for slot in region.contains if slot != args.map_slot]
    region.sees = [slot for slot in region.sees if slot != args.map_slot]

    blob = world.serialize()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(blob)

    check = wld_bwd.Bwd.parse(blob)
    checked = next(r for r in check.regions if r.name == args.region_name)
    if args.map_slot in checked.contains or args.map_slot in checked.sees:
        raise SystemExit("round-trip validation failed")
    print(
        f"removed {args.region_name}: map {args.map_slot} from contains/sees; "
        f"{len(blob)} bytes"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
