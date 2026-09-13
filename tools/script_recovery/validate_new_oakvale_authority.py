#!/usr/bin/env python3
"""Validate the disabled-by-default, single-authority New Oakvale Intro profile."""

from __future__ import annotations

import argparse
import json
from pathlib import Path

from lupa import LuaRuntime


NATIVE_NAME = "Q_NewOakValeIntro"
LEGACY_NAME = "NewOakValeIntro"
EXPECTED_ENTITIES = (
    "NOVI_LiveFather", "NOVI_Theresa", "NOVI_Guard", "NOVI_Villager",
    "NOVI_Bully", "NOVI_Victim", "NOVI_TeddyGirl", "NOVI_AffairMan",
    "NOVI_AffairWoman", "NOVI_AffairWife", "NOVI_BookTrader",
    "NOVI_BarrelMan", "NOVI_BarrelThug", "NOVI_Barrel",
    "NOVI_CreatedBeetle", "OVI_DeadFather",
)


def _load_table(path: Path, name: str):
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.execute(path.read_text(encoding="utf-8-sig"))
    return lua.globals()[name]


def validate(profile_fse: Path, source_fse: Path) -> dict:
    override = _load_table(profile_fse / "retail_override.lua", "RetailOverrides")
    quests = _load_table(profile_fse / "quests.lua", "Quests")
    errors: list[str] = []

    if bool(override["enabled"]):
        errors.append("authority switch must be disabled in the repository profile")

    entries = list(override["entries"].values())
    matches = [entry for entry in entries if entry["nativeName"] == NATIVE_NAME]
    entry = matches[0] if len(matches) == 1 else None
    if len(matches) != 1:
        errors.append(f"profile must contain exactly one native {NATIVE_NAME} override")
    else:
        if entry["mode"] != "override":
            errors.append("entry is not an identity-preserving override")
        if not bool(entry["mutatingCallsAllowed"]) or not bool(entry["saveWritesAllowed"]):
            errors.append("override lacks explicit mutation/save authorization")
        source = source_fse / (str(entry["file"]) + ".lua")
        if not source.is_file():
            errors.append(f"override source is missing: {source}")

    custom_names = sorted(str(value["name"]) for value in quests.values())
    competing = sorted(name for name in custom_names if name in {NATIVE_NAME, LEGACY_NAME})
    if competing:
        errors.append("custom quest table creates competing authority: " + ", ".join(competing))

    entity_names: list[str] = []
    duplicate_entities: list[str] = []
    missing_sources: list[str] = []
    if entry is not None:
        seen: set[str] = set()
        for entity in entry["entity_scripts"].values():
            name = str(entity["name"])
            entity_names.append(name)
            if name in seen:
                duplicate_entities.append(name)
            seen.add(name)
            source = source_fse / (str(entity["file"]) + ".lua")
            if not source.is_file():
                missing_sources.append(str(source))
        if tuple(entity_names) != EXPECTED_ENTITIES:
            errors.append("override entity binding order/set differs from retail Main")
        if duplicate_entities:
            errors.append("duplicate entity bindings: " + ", ".join(duplicate_entities))
        if missing_sources:
            errors.append("missing entity allocator sources: " + ", ".join(missing_sources))

    return {
        "schema": "fable-new-oakvale-authority-validation/0.1",
        "profile": str(profile_fse),
        "defaultEnabled": bool(override["enabled"]),
        "nativeOverrideCount": len(matches),
        "nativeIdentity": str(entry["nativeName"]) if entry else None,
        "allocatorFile": str(entry["file"]) if entry else None,
        "customQuestNames": custom_names,
        "competingAuthorities": competing,
        "entityNames": entity_names,
        "expectedEntityNames": list(EXPECTED_ENTITIES),
        "duplicateEntityNames": duplicate_entities,
        "missingSources": missing_sources,
        "mechanism": "Forge AddScript allocator replacement on existing retail CScriptInfo",
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
