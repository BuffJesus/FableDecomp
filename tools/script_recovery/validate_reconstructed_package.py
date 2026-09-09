#!/usr/bin/env python3
"""Static + trace validation for a reconstructed ForgeFSE package (e.g. NewOakValeIntro).

Checks, without loading ForgeFSE or the game:
  * every Lua file compiles (lupa `load`);
  * quests.lua registration is complete and consistent: every entity file under Entities/ is
    registered, every registered file exists, ids are unique and entity ids stay below ForgeFSE's
    MAX_ENTITY_SCRIPTS, and the quest module's AddEntityBinding names equal the registered names;
  * every `quest:Name(` / `me:Name(` call site uses a name registered in ForgeFSE's LuaManager.cpp
    (scope-aware), reporting missing and wrong-scope names;
  * explicit `NOVI.unsupported(` gaps are inventoried;
  * every fixture in the fixture directory runs through run_lua_trace and, when a stored trace exists,
    matches it exactly (source SHA-256 mismatches are reported as stale traces).

A passing run proves Lua structure and mock behaviour only. It is not retail parity evidence.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))

from audit_forgefse_runtime import extract_bindings  # noqa: E402
from run_lua_trace import run_trace  # noqa: E402

MAX_ENTITY_SCRIPTS = 300  # ForgeFSE dllmain.cpp g_entityAllocatorPool size
CALL_RE = re.compile(r"\b(quest|Quest|me|hero|thing|target|father|trader|theresa|village|beetle|barrel|victim|bully|girl|man|woman|wife)\s*:\s*([A-Za-z_][A-Za-z0-9_]*)\s*\(")
UNSUPPORTED_RE = re.compile(r"NOVI\.unsupported\s*\(\s*[^,]+,\s*\"([^\"]+)\"")
BINDING_RE = re.compile(r"AddEntityBinding\(\s*\"([^\"]+)\"\s*,\s*\"([^\"]+)\"")
QUEST_RECEIVERS = {"quest", "Quest"}


def strip_lua_comments(source: str) -> str:
    """Remove Lua line and ``--[[...]]`` comments while preserving quoted strings."""
    output = []
    index = 0
    quote = None
    escaped = False
    while index < len(source):
        char = source[index]
        if quote:
            output.append(char)
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == quote:
                quote = None
            index += 1
            continue
        if char in {'"', "'"}:
            quote = char
            output.append(char)
            index += 1
            continue
        if source.startswith("--[[", index):
            end = source.find("]]", index + 4)
            comment = source[index:end + 2 if end >= 0 else len(source)]
            output.extend("\n" for c in comment if c == "\n")
            index += len(comment)
            continue
        if source.startswith("--", index):
            end = source.find("\n", index + 2)
            if end < 0:
                break
            output.append("\n")
            index = end + 1
            continue
        output.append(char)
        index += 1
    return "".join(output)


def executable_matches(pattern: re.Pattern, source: str):
    """Yield regex matches from Lua code, excluding comment text."""
    yield from pattern.finditer(strip_lua_comments(source))


def lua_compiles(path: Path) -> str | None:
    try:
        from lupa import LuaRuntime
    except ImportError:
        return "lupa not installed"
    runtime = LuaRuntime()
    check = runtime.eval("function(src, name) local f, e = load(src, name); return f ~= nil, e end")
    ok, err = check(path.read_text(encoding="utf-8-sig"), path.name)
    return None if ok else str(err)


def parse_quests_lua(path: Path) -> dict:
    from lupa import LuaRuntime
    runtime = LuaRuntime()
    runtime.execute(path.read_text(encoding="utf-8-sig"))
    quests = runtime.globals().Quests
    result = {}
    for key, entry in quests.items():
        entities = []
        for _, es in (entry.entity_scripts or {}).items():
            entities.append({"name": es.name, "file": es.file, "id": int(es.id)})
        result[str(key)] = {"name": entry.name, "file": entry.file, "id": int(entry.id), "entity_scripts": entities}
    return result


def validate(fse_root: Path, package_key: str, fixtures_dir: Path | None, traces_dir: Path | None,
             lua_manager: Path, update_traces: bool) -> dict:
    report = {"errors": [], "warnings": [], "files": [], "fixtures": [], "unsupported": [], "apiCalls": {}}
    lua_files = sorted(fse_root.rglob("*.lua"))
    for f in lua_files:
        err = lua_compiles(f)
        report["files"].append({"file": f.relative_to(fse_root).as_posix(), "compiles": err is None, "error": err})
        if err:
            report["errors"].append(f"syntax: {f.name}: {err}")

    quests = parse_quests_lua(fse_root / "quests.lua")
    if package_key not in quests:
        report["errors"].append(f"quests.lua has no entry {package_key}")
        return report
    pkg = quests[package_key]
    registered = {e["name"]: e for e in pkg["entity_scripts"]}
    ids = [pkg["id"]] + [e["id"] for e in pkg["entity_scripts"]]
    for other_key, other in quests.items():
        if other_key == package_key:
            continue
        for e in other["entity_scripts"]:
            ids.append(e["id"])
        ids.append(other["id"])
    if len(ids) != len(set(ids)):
        report["errors"].append("duplicate ids in quests.lua")
    for e in pkg["entity_scripts"]:
        if e["id"] >= MAX_ENTITY_SCRIPTS:
            report["errors"].append(f"entity id {e['id']} for {e['name']} exceeds MAX_ENTITY_SCRIPTS")
        if not (fse_root / (e["file"] + ".lua")).exists():
            report["errors"].append(f"registered entity file missing: {e['file']}.lua")
    if not (fse_root / (pkg["file"] + ".lua")).exists():
        report["errors"].append(f"quest file missing: {pkg['file']}.lua")
    entity_dir = fse_root / pkg["file"].split("/")[0] / "Entities"
    if entity_dir.exists():
        for f in sorted(entity_dir.glob("*.lua")):
            if f.stem not in registered:
                report["errors"].append(f"entity file not registered in quests.lua: {f.name}")
    main_path = fse_root / (pkg["file"] + ".lua")
    main_src = main_path.read_text(encoding="utf-8-sig")
    bound = [(m.group(1), m.group(2)) for m in BINDING_RE.finditer(main_src)]
    if not bound:
        # table-driven registration: observe the actual AddEntityBinding calls in a mock run of Main
        try:
            probe = run_trace(main_path, "Main", "quest", {}, 200_000, fse_root)
            bound = [tuple(e["arguments"][:2]) for e in probe["events"] if e["name"] == "AddEntityBinding"]
        except Exception as exc:  # noqa: BLE001
            report["errors"].append(f"could not probe Main for AddEntityBinding calls: {exc}")
    bound_names = [b[0] for b in bound]
    if bound_names != [e["name"] for e in pkg["entity_scripts"]]:
        report["errors"].append("AddEntityBinding order/names differ from quests.lua registration")
    for name, file in bound:
        if registered.get(name, {}).get("file") != file:
            report["errors"].append(f"AddEntityBinding file for {name} ({file}) differs from quests.lua")
    report["registration"] = {"quest": pkg["name"], "questId": pkg["id"], "entities": len(registered),
                              "bindingsInMain": len(bound)}

    bindings = extract_bindings(lua_manager) if lua_manager.exists() else set()
    quest_names = {n for s, n in bindings if s == "Quest"}
    entity_names = {n for s, n in bindings if s == "Entity"}
    missing, wrong_scope = {}, {}
    for f in lua_files:
        rel = f.relative_to(fse_root).as_posix()
        src = f.read_text(encoding="utf-8-sig")
        for m in executable_matches(CALL_RE, src):
            receiver, name = m.group(1), m.group(2)
            scope = "Quest" if receiver in QUEST_RECEIVERS else "Entity"
            report["apiCalls"].setdefault(f"{scope}.{name}", []).append(rel)
            if not bindings:
                continue
            ok = name in (quest_names if scope == "Quest" else entity_names)
            if not ok:
                other = name in (entity_names if scope == "Quest" else quest_names)
                (wrong_scope if other else missing).setdefault(f"{scope}.{name}", set()).add(rel)
        for m in executable_matches(UNSUPPORTED_RE, src):
            report["unsupported"].append({"retail": m.group(1), "file": rel})
    report["missingApis"] = {k: sorted(v) for k, v in sorted(missing.items())}
    report["wrongScopeApis"] = {k: sorted(v) for k, v in sorted(wrong_scope.items())}
    for k in report["missingApis"]:
        report["errors"].append(f"missing ForgeFSE binding: {k}")
    for k in report["wrongScopeApis"]:
        report["warnings"].append(f"wrong-scope call: {k}")

    if fixtures_dir and fixtures_dir.exists():
        for fixture in sorted(fixtures_dir.glob("*.json")):
            spec = json.loads(fixture.read_text(encoding="utf-8-sig"))
            meta = spec.get("__meta__", {})
            source = fse_root / meta.get("source", pkg["file"] + ".lua")
            function = meta.get("function", "Main")
            kind = meta.get("kind", "quest")
            budget = int(meta.get("instructionBudget", 200_000))
            fixtures = {k: v for k, v in spec.items() if k != "__meta__"}
            entry = {"fixture": fixture.name, "function": function, "kind": kind}
            try:
                trace = run_trace(source, function, kind, fixtures, budget, fse_root)
                entry["events"] = len(trace["events"])
                entry["status"] = "ran"
                expectations = meta.get("expect", [])
                names = [e["name"] for e in trace["events"]]
                for expected in expectations:
                    if expected not in names:
                        entry["status"] = "expectation-failed"
                        report["errors"].append(f"{fixture.name}: expected event {expected} not traced")
                for forbidden in meta.get("forbid", []):
                    if forbidden in names:
                        entry["status"] = "expectation-failed"
                        report["errors"].append(f"{fixture.name}: forbidden event {forbidden} traced")
                if traces_dir:
                    stored = traces_dir / (fixture.stem + ".json")
                    if update_traces or not stored.exists():
                        stored.parent.mkdir(parents=True, exist_ok=True)
                        stored.write_text(json.dumps(trace, indent=2, sort_keys=True) + "\n", encoding="utf-8")
                        entry["trace"] = "written"
                    else:
                        old = json.loads(stored.read_text(encoding="utf-8-sig"))
                        if old.get("sourceSha256") != trace["sourceSha256"]:
                            entry["trace"] = "stale-source"
                            report["warnings"].append(f"{fixture.name}: stored trace built from a different source hash")
                        if old.get("events") != trace["events"]:
                            entry["trace"] = "differs"
                            report["errors"].append(f"{fixture.name}: trace differs from stored trace")
                        else:
                            entry["trace"] = entry.get("trace", "match")
            except Exception as exc:  # noqa: BLE001 - report every failure loudly
                entry["status"] = "error"
                entry["error"] = str(exc)
                report["errors"].append(f"{fixture.name}: {exc}")
            report["fixtures"].append(entry)
    report["summary"] = {
        "luaFiles": len(lua_files),
        "syntaxErrors": sum(1 for f in report["files"] if not f["compiles"]),
        "fixtures": len(report["fixtures"]),
        "fixturesPassed": sum(1 for f in report["fixtures"] if f.get("status") == "ran" and f.get("trace") in (None, "match", "written")),
        "missingApis": len(report["missingApis"]),
        "wrongScopeApis": len(report["wrongScopeApis"]),
        "unsupportedCallSites": len(report["unsupported"]),
        "errors": len(report["errors"]),
        "warnings": len(report["warnings"]),
    }
    return report


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--fse-root", type=Path, required=True)
    parser.add_argument("--package", required=True, help="key in quests.lua, e.g. Q_NewOakValeIntro")
    parser.add_argument("--fixtures", type=Path)
    parser.add_argument("--traces", type=Path)
    parser.add_argument("--lua-manager", type=Path,
                        default=Path(r"D:/Code/ForgeFSE-retail-shadow/FableScriptExtender/LuaManager.cpp"))
    parser.add_argument("--update-traces", action="store_true")
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    report = validate(args.fse_root, args.package, args.fixtures, args.traces, args.lua_manager, args.update_traces)
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    print(json.dumps(report["summary"], indent=2))
    for e in report["errors"]:
        print("ERROR:", e)
    for w in report["warnings"]:
        print("WARN:", w)
    return 1 if report["errors"] else 0


if __name__ == "__main__":
    raise SystemExit(main())
