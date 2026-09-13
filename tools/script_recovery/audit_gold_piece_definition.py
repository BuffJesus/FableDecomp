#!/usr/bin/env python3
"""Prove OBJECT_GOLD_1 is a one-gold collectible and resolve its CGoldDef link."""
from __future__ import annotations

import json
import re
import struct
import subprocess
from pathlib import Path

try:
    from tools.script_recovery.audit_barrel_reward_component import (
        FORGE, GAME_ROOT, SCHEMA, _name_at_ref,
    )
except ModuleNotFoundError:
    from audit_barrel_reward_component import FORGE, GAME_ROOT, SCHEMA, _name_at_ref


OBJECT_INDEX = 4644
GOLD_DEF_INDEX = 13648
GOLD_DEF_LINK_OFFSET = 0x45


def _run(game_root: Path, *args: str) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        [str(FORGE), "defs", *args], capture_output=True, text=True,
        encoding="utf-8", errors="replace", check=False,
    )


def audit(workspace: Path, game_root: Path = GAME_ROOT) -> dict:
    del workspace  # Installed retail assets are the authority for this definition audit.
    errors: list[str] = []
    decoded = _run(game_root, "decode", str(game_root), str(SCHEMA),
                   "OBJECT_GOLD_1", "game.bin.retail-bak")
    shown = _run(game_root, "show", str(game_root), "OBJECT_GOLD_1", "game.bin.retail-bak")
    gold_def = _run(game_root, "decode", str(game_root), str(SCHEMA),
                    str(GOLD_DEF_INDEX), "game.bin.retail-bak")
    for label, process in (("object decode", decoded), ("object show", shown), ("CGoldDef decode", gold_def)):
        if process.returncode:
            errors.append(f"{label} failed: {process.stderr.strip()}")

    components: list[str] = []
    linked_index = None
    if not errors:
        header = re.search(r"^entry (\d+): OBJECT / OBJECT_GOLD_1", decoded.stdout, re.MULTILINE)
        if not header or int(header.group(1)) != OBJECT_INDEX:
            errors.append("OBJECT_GOLD_1 does not resolve to retail entry 4644")
        match = re.search(r"^\s*Components\s+CThingComponentSet\s+([0-9a-fA-F]+)$", decoded.stdout, re.MULTILINE)
        if not match:
            errors.append("OBJECT_GOLD_1 component set is missing")
        else:
            raw = bytes.fromhex(match.group(1))
            count = struct.unpack_from("<I", raw)[0]
            names = (game_root / "data/CompiledDefs/names.bin").read_bytes()
            components = [_name_at_ref(names, struct.unpack_from("<I", raw, 4 + i * 9)[0])
                          for i in range(count)]
            if components != ["CTCHitLocations", "CTCPhysicsStandard", "CTCGraphicAppearance",
                              "CTCGraphicAppearanceStaticMesh", "CTCGold"]:
                errors.append(f"OBJECT_GOLD_1 components differ: {components}")

        hex_bytes = []
        for line in shown.stdout.splitlines():
            if re.match(r"^[0-9a-fA-F]{6}\s+", line):
                hex_bytes.extend(int(value, 16) for value in re.findall(r"\b[0-9a-fA-F]{2}\b", line.split("  ", 1)[1]))
        if len(hex_bytes) <= GOLD_DEF_LINK_OFFSET + 3:
            errors.append("OBJECT_GOLD_1 raw payload dump is truncated")
        else:
            linked_index = struct.unpack_from("<I", bytes(hex_bytes), GOLD_DEF_LINK_OFFSET)[0]
            if linked_index != GOLD_DEF_INDEX:
                errors.append(f"gold sub-definition link is {linked_index}, expected {GOLD_DEF_INDEX}")

        amount = re.search(r"^\s*<unnamed>\s+int32\s+(-?\d+)", gold_def.stdout, re.MULTILINE)
        if not amount or int(amount.group(1)) != 1:
            errors.append("linked CGoldDef does not encode amount 1")

    return {
        "schema": "new-oakvale-gold-piece-definition-audit/0.1",
        "objectIndex": OBJECT_INDEX,
        "linkedGoldDefIndex": linked_index,
        "linkedGoldAmount": 1 if "linked CGoldDef does not encode amount 1" not in errors else None,
        "components": components,
        "isCollectibleGold": "CTCGold" in components,
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
