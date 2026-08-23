#!/usr/bin/env python3
"""Add a map slot to an existing compiled-world host region.

The retail runtime's GetRegionNumberMapIsIn lookup reads FinalAlbion.bwd
region membership. A map may also have a WLD/BWD region record of its own,
but that record alone does not make the map force-loadable from Lua.
"""

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
    ap.add_argument("host_region")
    args = ap.parse_args()

    world = wld_bwd.Bwd.parse(args.source.read_bytes())
    host = next((r for r in world.regions if r.name == args.host_region), None)
    if host is None:
        raise SystemExit(f"host region not found: {args.host_region}")
    if args.map_slot not in host.contains:
        host.contains.append(args.map_slot)
    if args.map_slot not in host.sees:
        host.sees.append(args.map_slot)

    blob = world.serialize()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(blob)

    check = wld_bwd.Bwd.parse(blob)
    checked = next(r for r in check.regions if r.name == args.host_region)
    if args.map_slot not in checked.contains or args.map_slot not in checked.sees:
        raise SystemExit("round-trip validation failed")
    print(
        f"patched {args.host_region}: map {args.map_slot} in contains/sees; "
        f"{len(blob)} bytes"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
