#!/usr/bin/env python3
"""Decode the retail HESDEADJIM macro and certify every referenced camera record."""
from __future__ import annotations

import hashlib
import json
import re
import sys
from pathlib import Path

try:
    from tools.parse_frontend import decode_entry, load_all
except ModuleNotFoundError:
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
    from parse_frontend import decode_entry, load_all


def _strings(hex_value: str) -> list[str]:
    raw = bytes.fromhex(hex_value)
    count = int.from_bytes(raw[:4], "little")
    values = [item.decode("latin1") for item in raw[4:].split(b"\0") if item]
    if len(values) != count:
        raise ValueError(f"declared {count} strings, decoded {len(values)}")
    return values


def audit(workspace: Path) -> dict:
    compiled_root = workspace / "work/ui_proto/base"
    entries, schema, _ = load_all(str(compiled_root), str(workspace / "ghidra_out/def_schema.json"), "script.bin")
    entry = next(item for item in entries if item["name"] == "CS_OAKVALEINTRO_HESDEADJIM")
    decoded, leftover = decode_entry(entry, schema)
    macro = _strings(decoded["Macro"])
    skip = _strings(decoded["SkipCond"])
    camera_sequence = []
    for command in macro:
        match = re.match(r"(?:NoLoad)?UseCamera (CAM_OVID_[^, ]+)", command)
        if match:
            camera_sequence.append(match.group(1))
    tng_path = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\data\Levels\FinalAlbion\StartOakValeWest.tng")
    tng_bytes = tng_path.read_bytes()
    tng = tng_bytes.decode("latin1")
    records = {}
    for name in sorted(set(camera_sequence)):
        match = re.search(r"NewThing Thing;(?:(?!EndThing;)[\s\S])*?ScriptName " + re.escape(name) + r";(?:(?!EndThing;)[\s\S])*?EndThing;", tng)
        definition = re.search(r'DefinitionType "([^"]+)";', match.group(0)).group(1) if match else None
        records[name] = definition
    checks = {
        "retailScriptBinHash": hashlib.sha256((compiled_root / "data/CompiledDefs/script.bin").read_bytes()).hexdigest().upper() == "3D6D09939B612478F70C6AA1EB45A5BE0E850B27D534F78D996A00466501C1D9",
        "exactCutsceneEntry": entry["index"] == 485 and entry["definition"] == "CCutsceneDef" and len(entry["payload"]) == 3057,
        "payloadHash": hashlib.sha256(entry["payload"]).hexdigest().upper() == "74041FE346B9EA079BDA15B6DF728F045F6EFF85D7F172171096C21FE6ED1F78",
        "schemaDecodeConsumedPayload": not leftover,
        "macroHasExactCommandCount": len(macro) == 89,
        "skipHasExactCommandCount": len(skip) == 6,
        "cameraSequenceExact": camera_sequence == ["CAM_OVID_SHOT1", "CAM_OVID_SHOT1", "CAM_OVID_SHOT2NEW", "CAM_OVID_SHOT3NEW", "CAM_OVID_SHOT4", "CAM_OVID_SHOT7NEW", "CAM_OVID_SHOT6NEW", "CAM_OVID_SHOT7NEW", "CAM_OVID_SHOT8NEW", "CAM_OVID_SHOT9NEW", "CAM_OVID_SHOT9A", "CAM_OVID_SHOT10", "CAM_OVID_SHOT12A"],
        "skipCleanupExact": skip == ["FadeOut", "StayFadedOut", "GamePause 0.5", "HERO.FadeIn 0", "MAZE.FadeIn 0", "PlayMusic MUSIC_SET_NULL,FALSE"],
        "retailTngHash": hashlib.sha256(tng_bytes).hexdigest().upper() == "A444FCC91B17EA72CBAAC5012439420F540D099C68529FB5E717B4D02C3B48EA",
        "allReferencedCamerasHaveAuthoredRecords": (
            len(records) == 11
            and all(value in {"CAMERA_POINT_SCRIPTED", "CAMERA_POINT_SCRIPTED_SPLINE"} for value in records.values())),
    }
    return {"schema": "dead-father-cutscene-asset-audit/0.1", "ok": all(checks.values()),
            "macroCommandCount": len(macro), "cameraSequence": camera_sequence,
            "cameraRecords": records, "checks": checks}


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
