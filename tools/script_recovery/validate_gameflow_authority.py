#!/usr/bin/env python3
"""Validate the disabled-by-default, single-authority Lua Gameflow profile."""

from __future__ import annotations

import argparse
import json
from pathlib import Path

from lupa import LuaRuntime


def _load_table(path: Path, name: str):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.execute(path.read_text(encoding="utf-8-sig"))
    return lua.globals()[name]


def validate(profile_fse: Path, source_fse: Path) -> dict:
    override_path = profile_fse / "retail_override.lua"
    quests_path = profile_fse / "quests.lua"
    errors: list[str] = []
    override = _load_table(override_path, "RetailOverrides")
    quests = _load_table(quests_path, "Quests")

    if bool(override["enabled"]):
        errors.append("authority switch must be disabled in the repository profile")
    entries = list(override["entries"].values())
    gameflow = [entry for entry in entries if entry["nativeName"] == "Gameflow"]
    if len(gameflow) != 1:
        errors.append("profile must contain exactly one native Gameflow override")
        entry = None
    else:
        entry = gameflow[0]
        if entry["mode"] != "override":
            errors.append("Gameflow entry is not an identity-preserving override")
        if not bool(entry["mutatingCallsAllowed"]) or not bool(entry["saveWritesAllowed"]):
            errors.append("Gameflow override lacks explicit mutation/save authorization")
        source = source_fse / (str(entry["file"]) + ".lua")
        if not source.is_file():
            errors.append(f"override source is missing: {source}")

    custom_names = [str(value["name"]) for value in quests.values()]
    forbidden = sorted(name for name in custom_names if name in {"Gameflow", "LUAGameflow"})
    if forbidden:
        errors.append("custom quest table creates a competing gameflow authority: " + ", ".join(forbidden))

    return {
        "schema": "fable-gameflow-authority-validation/0.1",
        "profile": str(profile_fse),
        "defaultEnabled": bool(override["enabled"]),
        "nativeOverrideCount": len(gameflow),
        "nativeIdentity": str(entry["nativeName"]) if entry else None,
        "allocatorFile": str(entry["file"]) if entry else None,
        "customQuestNames": sorted(custom_names),
        "competingAuthorities": forbidden,
        "mechanism": "Forge AddScript allocator replacement on the existing retail CScriptInfo",
        "errors": errors,
        "ok": not errors,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--profile-fse", type=Path, required=True)
    parser.add_argument("--source-fse", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    report = validate(args.profile_fse, args.source_fse)
    payload = json.dumps(report, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(payload, encoding="utf-8")
    else:
        print(payload, end="")
    return 0 if report["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
