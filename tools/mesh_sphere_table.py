#!/usr/bin/env python3
"""Resolve each map's local-detail palette types to their mesh bounding spheres.

The palette in a map's STB common record gives type -> dense MBANK_ALLMESHES id
(the bank is global: CLocalDetailObjectCollectionType::Load hardcodes bank 0 at
FableWin 0x02E46A61). The mesh's authored bounding sphere lives in that entry's
Info descriptor: u32 flags, then f32 origin[10] where origin[0..2] is the centre
and origin[3] the radius.

Output TSV: map, collectionType, meshIdx, centreX, centreY, centreZ, radius

Usage:
    mesh_sphere_table.py <graphics.big> <out.tsv> <map-name>=<common-record.bin> ...
"""

from __future__ import annotations

import json
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from parse_bigb import parse_header, parse_footer, parse_toc

FORGE = Path("D:/Code/FableForge/build/forge.exe")


def mesh_spheres(graphics_big: Path) -> dict[int, tuple[float, float, float, float]]:
    data = graphics_big.read_bytes()
    _, _, footer_offset, _ = parse_header(data)
    subs, _ = parse_footer(data, footer_offset)
    bank = [s for s in subs if s["name"] == "MBANK_ALLMESHES"][0]
    entries, _, _, _ = parse_toc(data, bank, footer_offset)
    out: dict[int, tuple[float, float, float, float]] = {}
    for entry in entries:
        info = entry["info"]
        if len(info) < 4 + 10 * 4:
            continue  # animations and other entries carry no origin block
        origin = struct.unpack_from("<10f", info, 4)
        if not (origin[3] > 0.0) or origin[3] > 1e6:
            continue
        out[entry["id"]] = (origin[0], origin[1], origin[2], origin[3])
    return out


def palette(record: Path) -> list[dict]:
    result = subprocess.run([str(FORGE), "foliage", "read", str(record), "--json"],
                            capture_output=True, text=True, check=True)
    return json.loads(result.stdout)["types"]


def main() -> int:
    if len(sys.argv) < 4:
        raise SystemExit(__doc__)
    graphics_big, out_path = Path(sys.argv[1]), Path(sys.argv[2])
    spheres = mesh_spheres(graphics_big)
    print("%d mesh bounding spheres in %s" % (len(spheres), graphics_big.name))

    rows = 0
    unresolved: list[str] = []
    with out_path.open("w", encoding="utf-8", newline="\n") as out:
        out.write("map\tcollectionType\tmeshIdx\tcentreX\tcentreY\tcentreZ\tradius\n")
        for spec in sys.argv[3:]:
            name, _, record = spec.partition("=")
            for entry in palette(Path(record)):
                mesh_id = entry["meshIdx"]
                sphere = spheres.get(mesh_id)
                if sphere is None:
                    unresolved.append("%s/type%d/mesh%d"
                                      % (name, entry["paletteIndex"], mesh_id))
                    continue
                out.write("%s\t%d\t%d\t%r\t%r\t%r\t%r\n"
                          % (name, entry["paletteIndex"], mesh_id, *sphere))
                rows += 1
    print("wrote %d row(s) to %s" % (rows, out_path))
    if unresolved:
        print("unresolved (%d): %s" % (len(unresolved), ", ".join(unresolved[:12])))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
