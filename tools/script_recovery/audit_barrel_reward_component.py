#!/usr/bin/env python3
"""Prove that every New Oakvale barrel uses a def with reward-on-death machinery."""
from __future__ import annotations

import hashlib
import json
import re
import struct
import subprocess
from pathlib import Path


GAME_ROOT = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters")
FORGE = Path(r"D:\Code\FableForge\build\forge.exe")
SCHEMA = Path(r"D:\Code\FableForge\docs\re_reference\def_schema.json")
GAME_BIN_SHA256 = "1241FB989A9D85A8CBD912A05D5BF86F71382F65C26B8C7E1EA5944EF0023A09"
NAMES_BIN_SHA256 = "56D70F5D81231FC7A1A515A93FBC2F1F2AE8C3FF59E4C56DDDFA91995FCD523A"
EXPECTED_COMPONENTS = [
    "CTCHitLocations",
    "CTCPhysicsStandard",
    "CTCGraphicAppearance",
    "CTCGraphicAppearanceStaticMesh",
    "CTCTargeted",
    "CTCSinglePersonOccupiable",
    "CTCSmashableBarrier",
    "CTCContainerRewardHero",
    "CTCVillageMember",
]


def _sha(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest().upper()


def _name_at_ref(names: bytes, ref: int) -> str:
    start = 20 + ref
    end = names.index(b"\0", start)
    return names[start:end].decode("ascii")


def audit(workspace: Path, game_root: Path = GAME_ROOT) -> dict:
    errors: list[str] = []
    baseline_path = workspace / "refs/script_recovery/new_oakvale_intro/retail_tng_npc_baseline.json"
    baseline = json.loads(baseline_path.read_text(encoding="utf-8-sig"))
    barrels = [entry for entry in baseline["records"] if entry.get("scriptName") == "NOVI_Barrel"]
    if len(barrels) != 9:
        errors.append(f"TNG baseline contains {len(barrels)} NOVI_Barrel records, expected 9")
    definitions = sorted({entry.get("definitionType") for entry in barrels})
    if definitions != ["OBJECT_BARREL_BREAKABLE"]:
        errors.append(f"New Oakvale barrel definitions differ: {definitions}")

    source_hashes = {source["map"]: source["sha256"] for source in baseline["sources"]}
    for map_name, expected in source_hashes.items():
        path = game_root / "data/Levels/FinalAlbion" / map_name
        if not path.exists() or _sha(path) != expected:
            errors.append(f"installed TNG differs from baseline: {map_name}")

    compiled = game_root / "data/CompiledDefs"
    game_bin = compiled / "game.bin.retail-bak"
    names_bin = compiled / "names.bin"
    if not game_bin.exists() or _sha(game_bin) != GAME_BIN_SHA256:
        errors.append("pristine retail game.bin backup hash differs or is missing")
    if not names_bin.exists() or _sha(names_bin) != NAMES_BIN_SHA256:
        errors.append("installed names.bin hash differs or is missing")

    components: list[str] = []
    refs: list[int] = []
    if not errors and FORGE.exists() and SCHEMA.exists():
        process = subprocess.run(
            [str(FORGE), "defs", "decode", str(game_root), str(SCHEMA),
             "OBJECT_BARREL_BREAKABLE", "game.bin.retail-bak"],
            capture_output=True, text=True, encoding="utf-8", errors="replace", check=False,
        )
        if process.returncode:
            errors.append(f"forge def decode failed: {process.stderr.strip()}")
        else:
            match = re.search(r"^\s*Components\s+CThingComponentSet\s+([0-9a-fA-F]+)$", process.stdout, re.MULTILINE)
            if not match:
                errors.append("OBJECT_BARREL_BREAKABLE decode lacks CThingComponentSet")
            else:
                raw = bytes.fromhex(match.group(1))
                count = struct.unpack_from("<I", raw)[0]
                # CThingComponentSet serialises each name reference followed by a
                # five-byte default/null component payload marker.
                stride = 9
                if len(raw) < 4 + count * stride:
                    errors.append("component set is truncated")
                else:
                    names = names_bin.read_bytes()
                    for index in range(count):
                        offset = 4 + index * stride
                        ref = struct.unpack_from("<I", raw, offset)[0]
                        marker = raw[offset + 4:offset + stride]
                        refs.append(ref)
                        if marker != b"\0" * 5:
                            errors.append(f"component {index} has unexpected marker {marker.hex()}")
                        components.append(_name_at_ref(names, ref))
                    if components != EXPECTED_COMPONENTS:
                        errors.append(f"barrel component list differs: {components}")
    else:
        if not FORGE.exists():
            errors.append(f"FableForge decoder missing: {FORGE}")
        if not SCHEMA.exists():
            errors.append(f"definition schema missing: {SCHEMA}")

    return {
        "schema": "new-oakvale-barrel-reward-component-audit/0.1",
        "barrelCount": len(barrels),
        "definitionTypes": definitions,
        "definitionIndex": 3684,
        "components": components,
        "componentNameRefs": [f"0x{ref:X}" for ref in refs],
        "hasContainerRewardHero": "CTCContainerRewardHero" in components,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
