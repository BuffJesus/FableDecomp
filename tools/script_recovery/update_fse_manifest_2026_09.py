#!/usr/bin/env python3
"""Add the FSE bindings introduced upstream between 2026-08-31 and 2026-09-02.

Source of truth: eeeeeAeoN/FableScriptExtender @ master, files
`FableScriptExtender/LuaManager.cpp` (binding table), `LuaQuestState.h` and
`EntityScriptingAPI.h` (declarations).  Every row below is transcribed from a
declaration; none are inferred.  Retail addresses are deliberately absent - this
manifest describes the Lua surface, not verified native targets.
"""

from __future__ import annotations

import argparse
import json
from pathlib import Path

UPSTREAM_COMMIT_RANGE = "2026-08-31..2026-09-02"


def p(name: str, type_: str, optional: bool = False) -> dict:
    return {"name": name, "type": type_, "optional": optional}


NEW_FUNCTIONS = [
    # --- Entity scope -----------------------------------------------------
    {
        "name": "GetAngleXY", "scope": "Entity", "returnType": "float",
        "parameters": [p("pMe", "CScriptThing*")],
        "blocking": False, "category": "Entity API",
        "description": "Returns this entity's facing angle in the XY plane.",
    },
    {
        "name": "FadeOutAndKillEntity", "scope": "Entity", "returnType": "void",
        "parameters": [
            p("pMe", "CScriptThing*"),
            p("bKillAfterFade", "bool", True),
            p("fOverThisManySeconds", "float", True),
            p("bKillPersistent", "bool", True),
        ],
        "blocking": False, "category": "Entity API",
        "description": "Fades this entity out and optionally kills it (defaults true/1.0s/true).",
    },
    # --- Quest scope ------------------------------------------------------
    {
        "name": "SetIsPushableByHero", "scope": "Entity", "returnType": "void",
        "parameters": [p("pMe", "CScriptThing*"), p("bIsPushable", "bool")],
        "blocking": False, "category": "Entity API",
        "description": "Entity-scope pushable toggle added upstream alongside the quest-scope setter.",
    },
    {
        "name": "StartCutscene", "scope": "Quest", "returnType": "bool",
        "parameters": [p("actors", "table"), p("flags", "table", True), p("fixCamera", "bool", True)],
        "blocking": False, "category": "Cutscene",
        "description": "Opens a manual cutscene session: registers actors, starts the movie sequence.",
    },
    {
        "name": "RunCutscene", "scope": "Quest", "returnType": "bool",
        "parameters": [p("macroName", "string"), p("skippable", "bool", True), p("setupcond", "bool", True)],
        "blocking": True, "category": "Cutscene",
        "description": "Runs one cutscene macro inside an open session; blocks until the macro ends.",
    },
    {
        "name": "EndCutscene", "scope": "Quest", "returnType": "void",
        "parameters": [],
        "blocking": False, "category": "Cutscene",
        "description": "Closes the manual cutscene session and releases actors, camera and buffers.",
    },
    {
        "name": "RunCutsceneWithSetup", "scope": "Quest", "returnType": "void",
        "parameters": [
            p("cutsceneName", "string"), p("actors", "table"), p("flags", "table", True),
            p("setupcond", "bool", True), p("skippable", "bool", True), p("fixCamera", "bool", True),
        ],
        "blocking": True, "category": "Cutscene",
        "description": "One-shot cutscene: session setup, macro run and teardown in a single call.",
    },
    {
        "name": "GetCutsceneFlag", "scope": "Quest", "returnType": "bool",
        "parameters": [p("flagName", "string")],
        "blocking": False, "category": "Cutscene",
        "description": "Reads a flag from the active cutscene flag map.",
    },
    {
        "name": "SetCutsceneActionMode", "scope": "Quest", "returnType": "void",
        "parameters": [p("isActionMode", "bool"), p("actionName", "string")],
        "blocking": False, "category": "Cutscene",
        "description": "Toggles cutscene action mode and names the action.",
    },
    {
        "name": "FadeOutAndKillEntity", "scope": "Quest", "returnType": "void",
        "parameters": [
            p("thing", "CScriptThing*"), p("bKillAfterFade", "bool", True),
            p("fOverThisManySeconds", "float", True), p("bKillPersistent", "bool", True),
        ],
        "blocking": False, "category": "Entity Control",
        "description": "Fades the given thing out and optionally kills it.",
    },
    {
        "name": "AddLogbookStoryEntry", "scope": "Quest", "returnType": "void",
        "parameters": [p("entry", "int|string")],
        "blocking": False, "category": "UI",
        "description": "Adds a logbook story entry by numeric ID or by text key (sol::overload).",
    },
    {
        "name": "AddLogbookStoryEntryString", "scope": "Quest", "returnType": "void",
        "parameters": [p("textKey", "string")],
        "blocking": False, "category": "UI",
        "description": "Adds a logbook story entry by text key.",
    },
    {
        "name": "GetDistanceBetweenThings", "scope": "Quest", "returnType": "float",
        "parameters": [p("obj1", "CScriptThing*"), p("obj2", "CScriptThing*")],
        "blocking": False, "category": "World Query",
        "description": "Returns the distance between two things.",
    },
    {
        "name": "SetMasterGameState", "scope": "Quest", "returnType": "void",
        "parameters": [p("flagName", "string"), p("value", "any")],
        "blocking": False, "category": "Master Script",
        "description": "Writes a flag into the master (Gameflow) script state.",
    },
]


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--manifest", type=Path, default=Path("refs/fse_api_manifest.json"))
    parser.add_argument("--fse-version", default="1.1.0")
    parser.add_argument("--generated-at", default="2026-09-02T00:00:00Z")
    parser.add_argument("--check", action="store_true", help="report only, do not write")
    args = parser.parse_args()

    payload = json.loads(args.manifest.read_text(encoding="utf-8-sig"))
    existing = {(row["scope"], row["name"]) for row in payload["functions"]}

    added = [row for row in NEW_FUNCTIONS if (row["scope"], row["name"]) not in existing]
    skipped = [row for row in NEW_FUNCTIONS if (row["scope"], row["name"]) in existing]
    for row in skipped:
        print(f"already present: {row['scope']}.{row['name']}")
    for row in added:
        print(f"adding: {row['scope']}.{row['name']}")
    if args.check or not added:
        print(f"{len(added)} to add, {len(payload['functions'])} currently in manifest")
        return 0

    payload["functions"].extend(added)
    payload["functions"].sort(key=lambda row: (row["scope"], row["name"]))
    payload["fseVersion"] = args.fse_version
    payload["apiVersion"] = args.fse_version
    payload["generatedAtUtc"] = args.generated_at
    payload["buildId"] = f"upstream-{UPSTREAM_COMMIT_RANGE}"
    payload["fseBuildId"] = f"upstream-{UPSTREAM_COMMIT_RANGE}"
    args.manifest.write_text(json.dumps(payload, indent=4), encoding="utf-8")
    print(f"wrote {args.manifest}: {len(payload['functions'])} functions")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
