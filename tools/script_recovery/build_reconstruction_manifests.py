#!/usr/bin/env python3
"""Compile the design/coverage artifacts of a reconstructed script package from its evidence files.

Inputs (all under one evidence directory, e.g. refs/script_recovery/new_oakvale_intro/):
  entities/*.json        per-script operation inventories (quest + entities, same schema)
  state_phases.json      hand-authored quest-level phase model
  fixtures/*.json        mock fixtures (with `__meta__.function`)
  validation_report.json output of validate_reconstructed_package.py

Outputs (same directory):
  state_manifest.json, api_requirements.json, persistence_manifest.json, test_matrix.json,
  coverage.json and coverage.md (native operation/function -> Lua -> fixture -> status).

The builder never invents evidence: every row cites the operation entry it came from, and binding
existence is checked against the names registered in ForgeFSE's LuaManager.cpp.
"""
from __future__ import annotations

import argparse
import json
import re
import sys
from collections import OrderedDict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from audit_forgefse_runtime import extract_bindings  # noqa: E402

EMPTY_RETAIL_BODIES = {"0x00CDEBB0", "0x00CDEBD0", "0x00DB8260"}  # shared empty Init / OnPredicateFail slots
FIELD_RE = re.compile(r"^F\.(\w+)\s*=\s*key\(\"(\w+)\",\s*(BOOL|INT|STRING),\s*(0x[0-9a-f]+),\s*([^)]*)\)", re.M)
NO_BINDING_SENTINELS = {"n/a", "n/a (data)", "host-managed", "unknown", "none", "missing"}


def load_json(path: Path):
    return json.loads(path.read_text(encoding="utf-8-sig"))


def split_forge_bindings(binding: str) -> list[str]:
    """Split inventory composites without corrupting sentinel values such as ``n/a``."""
    raw = str(binding or "").strip()
    if not raw:
        return []
    if raw.lower() in NO_BINDING_SENTINELS:
        return [raw]
    parts, current, depth = [], [], 0
    index = 0
    while index < len(raw):
        char = raw[index]
        if char == "(":
            depth += 1
        elif char == ")" and depth:
            depth -= 1
        is_word_separator = depth == 0 and raw[index:index + 5].lower() == " and "
        if depth == 0 and (char in "|+/ ," and char != " " or is_word_separator):
            part = "".join(current).strip()
            if part:
                parts.append(part)
            current = []
            index += 5 if is_word_separator else 1
            continue
        current.append(char)
        index += 1
    part = "".join(current).strip()
    if part:
        parts.append(part)
    return parts


def normalize_forge_binding(call: str) -> str | None:
    """Turn inventory annotations into a binding name or an explicit non-binding sentinel.

    ``forgeBinding`` predates the generated manifest and some inventories include explanatory text
    alongside real calls.  Never count that prose (actor-table descriptions, cross references, or
    quest-local helpers) as a missing Forge API.
    """
    value = str(call or "").strip()
    lower = value.lower()
    if not value or lower.startswith("see seq") or value.isdigit():
        return None
    if lower in NO_BINDING_SENTINELS:
        return value
    if lower == "quest helper" or lower.startswith("quest helper "):
        return "n/a"
    if lower.startswith("host-managed") or lower.startswith("luaentityhost vtable"):
        return "host-managed"
    if lower.startswith("runcutscenewithsetup "):
        return "RunCutsceneWithSetup"
    return re.sub(r"\s*\(.*$", "", value)


def include_in_api_inventory(call_name: str | None) -> bool:
    """Return true only for callable bindings or the explicit host-managed category."""
    if call_name is None:
        return False
    return call_name.lower() not in {"n/a", "n/a (data)", "none", "unknown", "missing"}


def binding_scope(call_name: str, quest_names: set[str], entity_names: set[str]) -> str | None:
    """Describe every Forge registration scope without losing dual-registered methods."""
    in_quest = call_name in quest_names
    in_entity = call_name in entity_names
    if in_quest and in_entity:
        return "Quest|Entity"
    if in_quest:
        return "Quest"
    if in_entity:
        return "Entity"
    return None


def build_lua_call_surface(validation: dict, bindings: set[tuple[str, str]],
                           operation_scopes: dict[str, set[str]]) -> list[dict]:
    """Build the executable Lua adapter surface observed by package validation."""
    rows = []
    for scoped_name, locations in sorted(validation.get("apiCalls", {}).items()):
        if "." not in scoped_name:
            continue
        scope, name = scoped_name.split(".", 1)
        files = locations if isinstance(locations, list) else [locations]
        rows.append({
            "scopedName": scoped_name,
            "scope": scope,
            "operation": name,
            "bindingExists": (scope, name) in bindings,
            "files": sorted(set(files)),
            "mappedByOperationInventory": scope in operation_scopes.get(name, set()),
        })
    return rows


def api_gap_is_blocking(gap: dict) -> bool:
    """Old inventories default to blocking; proven host/folded deviations opt out explicitly."""
    return bool(gap.get("blocking", True))


def native_function_name(operation: dict) -> str | None:
    """Map a split Lua helper label such as Main/judge_hero to its native owner Main."""
    name = operation.get("function")
    return name.split("/", 1)[0] if isinstance(name, str) else name


def parent_field_names(declaration: str) -> set[str]:
    """Expand slash-combined, annotated parent-field declarations into canonical names."""
    names = set()
    for component in declaration.split("/"):
        match = re.match(r"([A-Za-z_][A-Za-z0-9_]*)", component.strip())
        if match:
            names.add(match.group(1))
    return names


def status_for(op: dict, traced_functions: set[str], unsupported: bool) -> str:
    binding = str(op.get("forgeBinding", ""))
    if unsupported or "missing" in binding.lower():
        return "API-blocked"
    evidence = str(op.get("evidence", ""))
    if op.get("argsKnown") is False or "inference" in evidence or "unknown" in evidence:
        if native_function_name(op) in traced_functions:
            return "implemented-and-traced (uncertain args)"
        return "uncertain"
    if native_function_name(op) in traced_functions:
        return "implemented-and-traced"
    return "implemented-but-untraced"


def fixture_covers(entity_kind: str, lua_file: str, function: str, fixture_meta: dict,
                   fixture_result: dict) -> bool:
    """Return true only for a passing trace of this exact source/function pair."""
    fixture_source = fixture_meta.get("source")
    normalized_source = (fixture_source or "").replace("\\", "/")
    source_matches = ((entity_kind == "quest" and
                       (fixture_source is None or normalized_source == lua_file)) or
                      (entity_kind != "quest" and normalized_source == lua_file))
    trace_passed = (fixture_result.get("status") == "ran" and
                    fixture_result.get("trace") in (None, "match", "written"))
    covered_functions = fixture_meta.get("covers", [fixture_meta.get("function")])
    if function not in covered_functions or not source_matches or not trace_passed:
        return False
    if function != fixture_meta.get("function"):
        evidence = fixture_meta.get("coverEvidence", {}).get(function, [])
        if not evidence or not set(evidence) <= set(fixture_meta.get("expect", [])):
            return False
    return True


def aggregate_operation_status(statuses: list[str]) -> str:
    if any(status == "API-blocked" for status in statuses):
        return "partially API-blocked"
    if any("uncertain" in status for status in statuses):
        return "implemented (uncertain args)"
    if statuses and all(status == "implemented-and-traced" for status in statuses):
        return "implemented-and-traced"
    return "implemented-but-untraced"


def build(evidence_dir: Path, fse_root: Path, lua_manager: Path) -> dict:
    entities = {}
    for path in sorted((evidence_dir / "entities").glob("*.json")):
        entities[path.stem] = load_json(path)
    phases = load_json(evidence_dir / "state_phases.json")
    validation = load_json(evidence_dir / "validation_report.json") if (evidence_dir / "validation_report.json").exists() else {}
    fixtures = {}
    for path in sorted((evidence_dir / "fixtures").glob("*.json")):
        spec = load_json(path)
        meta = spec.get("__meta__", {})
        fixtures[path.stem] = {"function": meta.get("function", "Main"), "kind": meta.get("kind", "quest"),
                               "source": meta.get("source"), "note": meta.get("note", ""),
                               "expect": meta.get("expect", []), "forbid": meta.get("forbid", []),
                               "coverEvidence": meta.get("coverEvidence", {}),
                               "covers": meta.get("covers", [meta.get("function", "Main")])}
    fixture_status = {f["fixture"].replace(".json", ""): f for f in validation.get("fixtures", [])}
    bindings = extract_bindings(lua_manager) if lua_manager.exists() else set()
    quest_names = {n for s, n in bindings if s == "Quest"}
    entity_names = {n for s, n in bindings if s == "Entity"}

    # ---- state manifest -------------------------------------------------------------------------
    state = {"schema": "fable-script-state-manifest/0.1", "script": phases["script"],
             "evidenceLevel": "reconstructed-source", "questPhases": phases["phases"],
             "deviations": phases.get("deviations", []), "entityPhases": {}}
    for name, ent in entities.items():
        if ent.get("kind") == "quest":
            continue
        state["entityPhases"][name] = {
            "nativeClass": ent.get("nativeClass"),
            "phases": ent.get("phases", []),
            "parentFieldsRead": ent.get("parentFieldsRead", []),
            "parentFieldsWritten": ent.get("parentFieldsWritten", []),
            "confidence": "medium" if ent.get("uncertainties") else "high",
            "runtimeObservations": ent.get("runtimeObservations", []),
        }

    # ---- api requirements -----------------------------------------------------------------------
    api = OrderedDict()
    for name, ent in entities.items():
        source_function_scope = "quest" if ent.get("kind") == "quest" else "entity"
        for op in ent.get("operations", []):
            binding = str(op.get("forgeBinding", "") or "")
            for call in split_forge_bindings(binding):
                call_name = normalize_forge_binding(call)
                if not include_in_api_inventory(call_name):
                    continue
                key = call_name
                row = api.setdefault(key, {
                    "operation": call_name,
                    "proposedForgeCall": call_name,
                    "bindingExists": (call_name in quest_names) or (call_name in entity_names),
                    "bindingScope": binding_scope(call_name, quest_names, entity_names),
                    "nativeEvidence": [],
                    "signatureConfidence": "high",
                    "evidenceNotes": [],
                    "semanticDifferences": [],
                    "fallbackStatus": "direct",
                    "blocked": False,
                })
                row["nativeEvidence"].append({"script": name, "function": op.get("function"), "seq": op.get("seq"),
                                              "native": op.get("native"), "evidence": op.get("evidence")})
                if op.get("argsKnown") is False:
                    row["signatureConfidence"] = "low"
                if op.get("note"):
                    row["evidenceNotes"].append(f"{name}#{op.get('seq')}: {op['note']}")
                call_lower = call.lower()
                if "unsupported" in call_lower or "missing" in call_lower:
                    row["fallbackStatus"] = "explicit-gap (NOVI.unsupported)"
                    row["blocked"] = True
                if "host-managed" in call_lower:
                    row["fallbackStatus"] = "host-managed"
                if not row["bindingExists"] and call_name.lower() not in ("n/a", "n/a (data)", "host-managed", "none"):
                    row["blocked"] = row["blocked"] or "registered" not in call_lower
        for gap in ent.get("apiGaps", []):
            key = "GAP:" + str(gap.get("retail"))
            is_blocking = api_gap_is_blocking(gap)
            api.setdefault(key, {"operation": gap.get("retail"), "proposedForgeCall": None, "bindingExists": False,
                                  "bindingScope": None, "nativeEvidence": [], "signatureConfidence": "n/a",
                                  "evidenceNotes": [],
                                  "semanticDifferences": [],
                                  "fallbackStatus": "explicit-gap" if is_blocking else "documented-deviation",
                                  "blocked": is_blocking})
            api[key]["nativeEvidence"].append({"script": name, "reason": gap.get("reason"), "luaLocation": gap.get("luaLocation")})
    operation_scopes = {}
    for row in api.values():
        if row.get("fallbackStatus") != "direct":
            continue
        operation_scopes[row["operation"]] = set((row.get("bindingScope") or "").split("|"))
    lua_call_surface = build_lua_call_surface(validation, bindings, operation_scopes)
    api_doc = {"schema": "fable-script-api-requirements/0.3", "script": phases["script"],
               "luaManager": str(lua_manager), "registeredQuestBindings": len(quest_names),
               "registeredEntityBindings": len(entity_names), "requirements": list(api.values()),
               "luaCallSurface": lua_call_surface}

    # ---- persistence manifest -------------------------------------------------------------------
    fields_src = (fse_root / "NewOakValeIntro" / "fields.lua").read_text(encoding="utf-8-sig") if (fse_root / "NewOakValeIntro" / "fields.lua").exists() else ""
    fields = []
    readers, writers = {}, {}
    for name, ent in entities.items():
        for declaration in ent.get("parentFieldsRead", []):
            for field in parent_field_names(declaration):
                readers.setdefault(field, set()).add(name)
        for declaration in ent.get("parentFieldsWritten", []):
            for field in parent_field_names(declaration):
                writers.setdefault(field, set()).add(name)
    quest_entry = next((e for e in entities.values() if e.get("kind") == "quest"), {})
    persisted = {p["name"] for p in quest_entry.get("persistedFields", [])}
    master_data = []
    for name, ent in entities.items():
        for access in ent.get("masterDataAccess", []):
            master_data.append({"script": name, **access})
    for m in FIELD_RE.finditer(fields_src):
        fname, pdb, kind, offset, init = m.groups()
        fields.append({
            "name": pdb, "type": kind.lower(), "retailOffset": offset, "initValue": init.strip(),
            "survivesSaveLoad": pdb in persisted,
            "retailPersistence": "OnPersist Transfer<signed char>" if pdb in persisted else "reset by Init on load",
            "writtenBy": sorted(writers.get(pdb, set())), "readBy": sorted(readers.get(pdb, set())),
            "phasesWhereItMatters": [p["id"] for p in phases["phases"] if pdb in " ".join(map(str, p.get("persistentFields", []) + p.get("emittedEvents", [])))],
        })
    if "F.WhichBadDeedsPerformed = {}" in fields_src:
        pdb = "WhichBadDeedsPerformed"
        fields.append({
            "name": pdb, "type": "bool[5]", "retailOffset": "0xfc..0x100",
            "initValue": "false[5]", "survivesSaveLoad": False,
            "retailPersistence": "reset by Init on load",
            "writtenBy": sorted(writers.get(pdb, set())),
            "readBy": sorted(readers.get(pdb, set())),
            "phasesWhereItMatters": [
                p["id"] for p in phases["phases"]
                if pdb in " ".join(map(str, p.get("persistentFields", []) + p.get("emittedEvents", [])))
            ],
        })
    persistence = {"schema": "fable-script-persistence-manifest/0.2", "script": phases["script"],
                   "persistedByRetail": sorted(persisted), "fields": fields,
                   "masterData": master_data,
                   "note": "Retail persists only AttackOver. Everything else restarts from Init on load; a save during the childhood therefore resumes the childhood from its beginning of state (entities re-run their Init/Main), which is retail behaviour."}

    # ---- test matrix ----------------------------------------------------------------------------
    matrix_rows = []
    for stem, meta in fixtures.items():
        st = fixture_status.get(stem, {})
        matrix_rows.append({"fixture": stem, "function": meta["function"], "kind": meta["kind"], "source": meta["source"],
                            "purpose": meta["note"], "expects": meta["expect"], "forbids": meta["forbid"],
                            "covers": meta["covers"], "coverEvidence": meta["coverEvidence"],
                            "result": st.get("status", "not-run"), "trace": st.get("trace"), "events": st.get("events")})
    test_matrix = {"schema": "fable-script-test-matrix/0.2", "script": phases["script"], "fixtures": matrix_rows,
                   "runtimeOnlyBranches": [
                       "rendered camera/animation presentation parity for CS_OAKVALEINTRO_HESDEADJIM (the 89-command retail macro, 13-use/11-record camera sequence, six-command skip cleanup, runtime execution, camera-fix bracket, and state restoration are proven)",
                       "visual and non-scripted world-population parity after the proven section swap (post-attack Dead Father allocation/init, pre-attack Live Father unwind, and Dead Father Main entry are runtime-observed in three runs)",
                       "engine-side Hero morph result after the proven TurnCreatureInto(CREATURE_HERO_CHILD) out-buffer and post-frame handle-reacquisition boundary",
                       "visual endpoint/pathfinding parity for the intermittent Affair Wife route and Bully run-off compatibility fallback (successful Wife arrival and blocking Speak completion are runtime-observed)",
                       "repeated mid-childhood region unload/reload re-entry timing (normal quest-completion entity unwind, persistence, host destruction, coroutine cleanup, and retained-reference release are proven)"]}

    # ---- coverage -------------------------------------------------------------------------------
    unsupported_by_file = {}
    for u in validation.get("unsupported", []):
        unsupported_by_file.setdefault(Path(u["file"]).stem, []).append(u["retail"])
    coverage = []
    for name, ent in entities.items():
        lua_file = ent.get("luaFiles", {}).get("quest") if ent.get("kind") == "quest" else f"NewOakValeIntro/Entities/{name}.lua"
        for fn, meta in ent.get("functions", {}).items():
            addr = meta.get("address") if isinstance(meta, dict) else meta
            ops = [o for o in ent.get("operations", []) if native_function_name(o) == fn]
            covering = []
            for stem, fixture_meta in fixtures.items():
                result = fixture_status.get(stem, {})
                if fixture_covers(ent.get("kind", "entity"), lua_file, fn, fixture_meta, result):
                    covering.append(stem)
            traced_here = {fn} if covering else set()
            coverage_alias = meta.get("coverageAlias") if isinstance(meta, dict) else None
            if not ops and coverage_alias:
                status = f"not-applicable (covered by {coverage_alias})"
            elif not ops and fn in ("destructor", "RegisterMain"):
                status = "not-applicable (host lifecycle)"
            elif not ops and (str(addr) in EMPTY_RETAIL_BODIES or (isinstance(meta, dict) and meta.get("emptyRetail"))):
                status = "not-applicable (retail body empty / data only)"
            elif not ops:
                status = "not-implemented"
            else:
                statuses = [status_for(o, traced_here, name in unsupported_by_file and "unsupported" in str(o.get("forgeBinding", "")).lower()) for o in ops]
                status = aggregate_operation_status(statuses)
            coverage.append({"script": name, "nativeFunction": fn, "address": addr, "operations": len(ops),
                             "luaLocation": lua_file, "fixtures": covering,
                             "evidence": (meta.get("evidence") if isinstance(meta, dict) else None) or "native-decompile",
                             "status": status})
    coverage_doc = {"schema": "fable-script-coverage/0.1", "script": phases["script"], "rows": coverage,
                    "summary": {
                        "functions": len(coverage),
                        "operations": sum(r["operations"] for r in coverage),
                        "implementedAndTraced": sum(1 for r in coverage if r["status"] == "implemented-and-traced"),
                        "implementedUntraced": sum(1 for r in coverage if r["status"] == "implemented-but-untraced"),
                        "uncertain": sum(1 for r in coverage if "uncertain" in r["status"]),
                        "apiBlocked": sum(1 for r in coverage if "API-blocked" in r["status"]),
                        "notImplemented": sum(1 for r in coverage if r["status"] == "not-implemented"),
                        "notApplicable": sum(1 for r in coverage if r["status"].startswith("not-applicable")),
                    }}
    md = ["| script | native function | address | ops | Lua | fixtures | evidence | status |", "|---|---|---|---:|---|---|---|---|"]
    for r in coverage:
        md.append(f"| {r['script']} | {r['nativeFunction']} | {r['address']} | {r['operations']} | `{r['luaLocation']}` | {', '.join(r['fixtures']) or '-'} | {r['evidence']} | {r['status']} |")

    outputs = {
        "state_manifest.json": state, "api_requirements.json": api_doc, "persistence_manifest.json": persistence,
        "test_matrix.json": test_matrix, "coverage.json": coverage_doc,
    }
    for fname, doc in outputs.items():
        (evidence_dir / fname).write_text(json.dumps(doc, indent=2, sort_keys=False) + "\n", encoding="utf-8")
    (evidence_dir / "coverage.md").write_text("\n".join(md) + "\n", encoding="utf-8")
    return {"entities": len(entities), "apiRequirements": len(api_doc["requirements"]),
            "blockedApis": sum(1 for r in api_doc["requirements"] if r["blocked"]),
            "fields": len(fields), "fixtures": len(matrix_rows), "coverage": coverage_doc["summary"]}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--evidence-dir", type=Path, required=True)
    parser.add_argument("--fse-root", type=Path, required=True)
    parser.add_argument("--lua-manager", type=Path,
                        default=Path(r"D:/Code/ForgeFSE-retail-shadow/FableScriptExtender/LuaManager.cpp"))
    args = parser.parse_args()
    print(json.dumps(build(args.evidence_dir, args.fse_root, args.lua_manager), indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
