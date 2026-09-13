#!/usr/bin/env python3
"""Deterministically audit the downloaded Lua Gameflow's resume-stage contract.

This is a static evidence check.  The native stage values come from writes to
CQ_SunnyvaleMasterData::PostSavePosition in the recovered retail Gameflow Main
decompile; the Lua values and dispatch table come from Aeon's downloaded port.
"""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path


LUA_STAGE = re.compile(r"^local\s+(EGP_[A-Z0-9_]+)\s*=\s*(\d+)\s*$", re.MULTILINE)
LUA_DISPATCH = re.compile(r"\[(EGP_[A-Z0-9_]+)\]\s*=\s*(RunStage_[A-Za-z0-9_]+)")
NATIVE_STAGE_WRITE = re.compile(r"\+ 4\)\s*=\s*(0x[0-9a-fA-F]+|\d+);")
LUA_HANDLER = re.compile(
    r"function\s+(RunStage_[A-Za-z0-9_]+)\(quest\)(.*?)(?=\nfunction\s+(?:RunStage_|Main\())",
    re.DOTALL,
)


def audit(lua_path: Path, native_cluster_path: Path) -> dict:
    lua = lua_path.read_text(encoding="utf-8-sig")
    cluster = json.loads(native_cluster_path.read_text(encoding="utf-8-sig"))
    main = next(item for item in cluster["lifecycle"] if item["role"] == "Main")
    persist = next(item for item in cluster["lifecycle"] if item["role"] == "OnPersist")

    lua_stages = {name: int(value) for name, value in LUA_STAGE.findall(lua)}
    dispatch = dict(LUA_DISPATCH.findall(lua))
    handlers = {}
    for handler_name, body in LUA_HANDLER.findall(lua):
        own = re.findall(r'SetMasterGameState\("PostSavePosition",\s*(EGP_[A-Z0-9_]+)\)', body)
        successors = re.findall(r"return\s+(EGP_[A-Z0-9_]+)", body)
        handlers[handler_name] = {"stageWrites": own, "successors": successors}
    native_values = sorted({int(raw, 0) for raw in NATIVE_STAGE_WRITE.findall(main["decompile"])})
    lua_values = sorted(lua_stages.values())

    persistence_fields = ["PostSavePosition", "CoreQuestWaiting",
                          "SavedScriptNames", "SavedCardDefNames"]
    native_persist = {field: field in persist["decompile"] for field in persistence_fields}
    lua_persist = {field: field in lua[lua.find("function OnPersist"):lua.find("function CoreQuestReminder")]
                   for field in persistence_fields}

    errors: list[str] = []
    if lua_values != native_values:
        errors.append("Lua EGP values differ from native PostSavePosition writes")
    missing_dispatch = sorted(set(lua_stages) - set(dispatch))
    extra_dispatch = sorted(set(dispatch) - set(lua_stages))
    if missing_dispatch:
        errors.append("stages missing dispatch handlers: " + ", ".join(missing_dispatch))
    if extra_dispatch:
        errors.append("dispatch entries lack constants: " + ", ".join(extra_dispatch))
    for stage, handler_name in dispatch.items():
        contract = handlers.get(handler_name)
        if not contract:
            errors.append(f"dispatch handler body missing: {handler_name}")
        elif contract["stageWrites"] != [stage]:
            errors.append(f"{handler_name} does not write exactly its own stage {stage}")

    ordered_names = [name for name, _ in sorted(lua_stages.items(), key=lambda item: item[1])]
    expected_edges = {ordered_names[index]: ordered_names[index + 1]
                      for index in range(len(ordered_names) - 1)}
    transition_edges = {}
    for stage, handler_name in dispatch.items():
        successors = handlers.get(handler_name, {}).get("successors", [])
        transition_edges[stage] = successors
        expected = [] if stage == ordered_names[-1] else [expected_edges[stage]]
        if successors != expected:
            errors.append(f"{handler_name} successors {successors} differ from {expected}")

    # The two saved vectors exist in retail's object and persistence format, but
    # neither is referenced by retail Main.  Report rather than pretending Lua
    # has reproduced an observable behavior that the recovered code does not use.
    native_main_mentions_saved_vectors = {
        field: field in main["decompile"] for field in ("SavedScriptNames", "SavedCardDefNames")
    }

    return {
        "schema": "fable-lua-gameflow-audit/0.1",
        "lua": str(lua_path),
        "nativeCluster": str(native_cluster_path),
        "stageContract": {
            "nativeValues": native_values,
            "luaValues": lua_values,
            "exactValueMatch": lua_values == native_values,
            "luaConstants": lua_stages,
            "dispatch": dispatch,
            "missingDispatch": missing_dispatch,
            "extraDispatch": extra_dispatch,
            "handlers": handlers,
            "transitionEdges": transition_edges,
            "linearRetailStageOrder": ordered_names,
        },
        "persistenceContract": {
            "nativeFields": native_persist,
            "luaFields": lua_persist,
            "nativeMainMentionsSavedVectors": native_main_mentions_saved_vectors,
            "finding": "SavedScriptNames/SavedCardDefNames are persisted by retail OnPersist but are not referenced by the recovered retail Main; Lua currently persists only the two live Main fields.",
        },
        "errors": errors,
        "ok": not errors,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--lua", type=Path, required=True)
    parser.add_argument("--native-cluster", type=Path, required=True)
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    report = audit(args.lua, args.native_cluster)
    text = json.dumps(report, indent=2) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(text, encoding="utf-8")
    else:
        print(text, end="")
    return 0 if report["ok"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
