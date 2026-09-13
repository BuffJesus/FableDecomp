#!/usr/bin/env python3
"""Three-way audit of native, inventory, and Lua New Oakvale Villager speech vectors."""
from __future__ import annotations

import json
import re
from pathlib import Path

from lupa import LuaRuntime

KEY_RE = re.compile(r"TEXT_QST_048_VILLAGER_DONE_[A-Z0-9_]+")
EXPECTED = {
    "MaleGood": ("0x9c", 6), "MaleBad": ("0xa8", 6),
    "MaleBoth": ("0xb4", 4), "MaleNeither": ("0xc0", 5),
    "FemaleGood": ("0xcc", 6), "FemaleBad": ("0xd8", 6),
    "FemaleBoth": ("0xe4", 4), "FemaleNeither": ("0xf0", 5),
}
EXPECTED_INIT_ORDER = [
    "FemaleBad", "MaleBad", "FemaleBoth", "MaleBoth",
    "FemaleGood", "MaleGood", "FemaleNeither", "MaleNeither",
]


def lua_array(table) -> list[str]:
    return [str(table[index]) for index in range(1, len(table) + 1)]


def audit(inventory_file: Path, lua_file: Path, native_decompile: Path) -> dict:
    inventory = json.loads(inventory_file.read_text(encoding="utf-8-sig"))["textKeys"]
    runtime = LuaRuntime(unpack_returned_tuples=True)
    speech = runtime.execute(f'return dofile("{lua_file.as_posix()}")')
    errors: list[str] = []
    inventory_keys: set[str] = set()
    lua_keys: set[str] = set()
    for vector, (offset, count) in EXPECTED.items():
        row = inventory.get("VillagerSpeech" + vector, {})
        expected_keys = list(row.get("keys", []))
        actual_keys = lua_array(speech[vector])
        inventory_keys.update(expected_keys)
        lua_keys.update(actual_keys)
        if row.get("offset") != offset:
            errors.append(f"{vector}: inventory offset {row.get('offset')} != {offset}")
        if len(expected_keys) != count:
            errors.append(f"{vector}: inventory has {len(expected_keys)} keys, expected {count}")
        if actual_keys != expected_keys:
            errors.append(f"{vector}: Lua vector differs from native inventory order")
    init_order = lua_array(speech["INIT_ORDER"])
    if init_order != EXPECTED_INIT_ORDER:
        errors.append("Lua INIT_ORDER differs from retail vector construction order")
    native_keys = set(KEY_RE.findall(native_decompile.read_text(encoding="utf-8", errors="replace")))
    if native_keys != inventory_keys:
        errors.append("native Init literal set differs from structured inventory")
    if lua_keys != inventory_keys:
        errors.append("evaluated Lua literal set differs from structured inventory")
    return {
        "schema": "new-oakvale-villager-speech-vector-audit/0.1",
        "vectors": len(EXPECTED), "keys": len(inventory_keys),
        "nativeKeys": len(native_keys), "luaKeys": len(lua_keys),
        "errors": errors, "ok": not errors,
    }


if __name__ == "__main__":
    root = Path(__file__).resolve().parents[2]
    print(json.dumps(audit(
        root / "refs/script_recovery/new_oakvale_intro/entities/NOVI_Villager.json",
        root / "refs/script_recovery/reconstructed/NewOakValeIntro/FSE/NewOakValeIntro/villager_speech.lua",
        root / "ghidra_out/script_recovery/native_decompiles/0x00DAADD0.c"), indent=2))
