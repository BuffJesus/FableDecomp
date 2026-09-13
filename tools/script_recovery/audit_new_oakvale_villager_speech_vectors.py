#!/usr/bin/env python3
"""Cross-check retail, metadata, Lua, layout, and teardown for villager speech vectors."""
from __future__ import annotations

import json
import re
from pathlib import Path


KEY_RE = re.compile(
    r'"(?P<key>TEXT_QST_048_VILLAGER_DONE_[A-Z0-9_]+)"[^\n]*\n'
    r'\s*\w+\s*=\s*[^\n]*\(param_1 \+ (?P<end>0x[0-9a-f]+|\d+)\);'
)


def _load(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8-sig"))


def audit(workspace: Path) -> dict:
    villager = _load(workspace / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Villager.json")
    layout = _load(workspace / "ghidra_out/script_recovery/new_oakvale_quest_layout.json")
    teardown = _load(workspace / "ghidra_out/script_recovery/new_oakvale_quest_teardown.json")
    native = (workspace / "ghidra_out/script_recovery/native_quest_construction/0x00DAADD0.c").read_text(encoding="utf-8")
    lua = (workspace / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/villager_speech.lua").read_text(encoding="utf-8")
    errors: list[str] = []

    vectors = {name: row for name, row in villager["textKeys"].items()
               if name.startswith("VillagerSpeech")}
    native_by_offset: dict[int, list[str]] = {}
    native_init_order: list[int] = []
    for match in KEY_RE.finditer(native):
        base = int(match.group("end"), 0) - 4
        if base not in native_by_offset:
            native_by_offset[base] = []
            native_init_order.append(base)
        native_by_offset[base].append(match.group("key"))

    metadata_by_offset = {int(row["offset"], 0): (name, row["keys"])
                          for name, row in vectors.items()}
    if set(native_by_offset) != set(metadata_by_offset):
        errors.append(f"native/metadata vector offsets differ: native={sorted(native_by_offset)}, metadata={sorted(metadata_by_offset)}")
    for offset, (name, keys) in metadata_by_offset.items():
        if native_by_offset.get(offset) != keys:
            errors.append(f"{name}: native key sequence differs from inventory")

    suffix_sets = {name: re.findall(r'"(_\d+)"', body) for name, body in
                   re.findall(r"local\s+(\w+)\s*=\s*\{([^}]*)\}", lua)}
    lua_vectors: dict[str, list[str]] = {}
    for short_name, prefix, suffix_name in re.findall(
            r"^\s*(\w+)\s*=\s*keys\(\"([^\"]+)\",\s*(\w+)\)", lua, re.MULTILINE):
        lua_vectors["VillagerSpeech" + short_name] = [prefix + suffix for suffix in suffix_sets.get(suffix_name, [])]
    if set(lua_vectors) != set(vectors):
        errors.append("Lua and inventory speech-vector names differ")
    for name, row in vectors.items():
        if lua_vectors.get(name) != row["keys"]:
            errors.append(f"{name}: Lua keys differ from inventory")

    init_match = re.search(r"Speech\.INIT_ORDER\s*=\s*\{([^}]*)\}", lua, re.DOTALL)
    lua_init_order = re.findall(r'"([^"]+)"', init_match.group(1)) if init_match else []
    native_names = [metadata_by_offset[offset][0].removeprefix("VillagerSpeech")
                    for offset in native_init_order if offset in metadata_by_offset]
    if lua_init_order != native_names:
        errors.append(f"Lua INIT_ORDER differs from retail: lua={lua_init_order}, retail={native_names}")

    region = next(row for row in layout["regions"] if row["name"] == "villager-speech-vectors")
    region_start = int(region["offset"], 0)
    expected_offsets = list(range(region_start, region_start + region["size"], 12))
    if sorted(metadata_by_offset) != expected_offsets:
        errors.append("speech vectors do not exactly cover the 96-byte layout region")
    teardown_offsets = [int(offset, 0) for offset in teardown["implementation"]["vectorDestructionOrder"]]
    if teardown_offsets != list(reversed(expected_offsets)):
        errors.append("destructor does not reverse-cover speech vector construction layout")

    all_keys = [key for offset in expected_offsets for key in metadata_by_offset[offset][1]]
    if len(all_keys) != len(set(all_keys)):
        errors.append("speech text keys are not unique")
    return {
        "schema": "new-oakvale-villager-speech-vector-audit/0.1",
        "vectors": len(vectors),
        "textKeys": len(all_keys),
        "nativeInitOrder": native_names,
        "layoutBytes": region["size"],
        "errors": errors,
        "ok": not errors,
    }


if __name__ == "__main__":
    print(json.dumps(audit(Path(__file__).resolve().parents[2]), indent=2))
