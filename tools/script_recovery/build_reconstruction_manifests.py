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


def load_json(path: Path):
    return json.loads(path.read_text(encoding="utf-8-sig"))


def status_for(op: dict, traced_functions: set[str], unsupported: bool) -> str:
    binding = str(op.get("forgeBinding", ""))
    if unsupported or "missing" in binding.lower():
        return "API-blocked"
    evidence = str(op.get("evidence", ""))
    if op.get("argsKnown") is False or "inference" in evidence or "unknown" in evidence:
        if op.get("function") in traced_functions:
            return "implemented-and-traced (uncertain args)"
        return "uncertain"
    if op.get("function") in traced_functions:
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
                               "expect": meta.get("expect", []), "forbid": meta.get("forbid", [])}
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
        }

    # ---- api requirements -----------------------------------------------------------------------
    api = OrderedDict()
    for name, ent in entities.items():
        source_function_scope = "quest" if ent.get("kind") == "quest" else "entity"
        for op in ent.get("operations", []):
            binding = str(op.get("forgeBinding", "") or "")
            for call in [b.strip() for b in re.split(r"[,/]| and ", binding) if b.strip()]:
                call_name = re.sub(r"\s*\(.*$", "", call)
                if call_name.lower() in ("n/a", "missing", "host-managed", "unknown", "none", "n/a (data)"):
                    call_name = call
                key = call_name
                row = api.setdefault(key, {
                    "operation": call_name,
                    "proposedForgeCall": call_name,
                    "bindingExists": (call_name in quest_names) or (call_name in entity_names),
                    "bindingScope": "Quest" if call_name in quest_names else ("Entity" if call_name in entity_names else None),
                    "nativeEvidence": [],
                    "signatureConfidence": "high",
                    "semanticDifferences": [],
                    "fallbackStatus": "direct",
                    "blocked": False,
                })
                row["nativeEvidence"].append({"script": name, "function": op.get("function"), "seq": op.get("seq"),
                                              "native": op.get("native"), "evidence": op.get("evidence")})
                if op.get("argsKnown") is False:
                    row["signatureConfidence"] = "low"
                if op.get("note"):
                    row["semanticDifferences"].append(f"{name}#{op.get('seq')}: {op['note']}")
                if "unsupported" in binding.lower() or "missing" in binding.lower():
                    row["fallbackStatus"] = "explicit-gap (NOVI.unsupported)"
                    row["blocked"] = True
                if "host-managed" in binding.lower():
                    row["fallbackStatus"] = "host-managed"
                if not row["bindingExists"] and call_name.lower() not in ("n/a", "n/a (data)", "host-managed", "none"):
                    row["blocked"] = row["blocked"] or "registered" not in binding
        for gap in ent.get("apiGaps", []):
            key = "GAP:" + str(gap.get("retail"))
            api.setdefault(key, {"operation": gap.get("retail"), "proposedForgeCall": None, "bindingExists": False,
                                 "bindingScope": None, "nativeEvidence": [], "signatureConfidence": "n/a",
                                 "semanticDifferences": [], "fallbackStatus": "explicit-gap", "blocked": True})
            api[key]["nativeEvidence"].append({"script": name, "reason": gap.get("reason"), "luaLocation": gap.get("luaLocation")})
    api_doc = {"schema": "fable-script-api-requirements/0.1", "script": phases["script"],
               "luaManager": str(lua_manager), "registeredQuestBindings": len(quest_names),
               "registeredEntityBindings": len(entity_names), "requirements": list(api.values())}

    # ---- persistence manifest -------------------------------------------------------------------
    fields_src = (fse_root / "NewOakValeIntro" / "fields.lua").read_text(encoding="utf-8-sig") if (fse_root / "NewOakValeIntro" / "fields.lua").exists() else ""
    fields = []
    readers, writers = {}, {}
    for name, ent in entities.items():
        for f in ent.get("parentFieldsRead", []):
            readers.setdefault(f, set()).add(name)
        for f in ent.get("parentFieldsWritten", []):
            writers.setdefault(f, set()).add(name)
    quest_entry = next((e for e in entities.values() if e.get("kind") == "quest"), {})
    persisted = {p["name"] for p in quest_entry.get("persistedFields", [])}
    for m in FIELD_RE.finditer(fields_src):
        fname, pdb, kind, offset, init = m.groups()
        fields.append({
            "name": pdb, "type": kind.lower(), "retailOffset": offset, "initValue": init.strip(),
            "survivesSaveLoad": pdb in persisted,
            "retailPersistence": "OnPersist Transfer<signed char>" if pdb in persisted else "reset by Init on load",
            "writtenBy": sorted(writers.get(pdb, set())), "readBy": sorted(readers.get(pdb, set())),
            "phasesWhereItMatters": [p["id"] for p in phases["phases"] if pdb in " ".join(map(str, p.get("persistentFields", []) + p.get("emittedEvents", [])))],
        })
    persistence = {"schema": "fable-script-persistence-manifest/0.1", "script": phases["script"],
                   "persistedByRetail": sorted(persisted), "fields": fields,
                   "masterData": quest_entry.get("masterData", ["TeddySolution (CQ_SunnyvaleMasterData+0x54, written by NOVI_TeddyGirl/NOVI_Bully paths)"]),
                   "note": "Retail persists only AttackOver. Everything else restarts from Init on load; a save during the childhood therefore resumes the childhood from its beginning of state (entities re-run their Init/Main), which is retail behaviour."}

    # ---- test matrix ----------------------------------------------------------------------------
    matrix_rows = []
    for stem, meta in fixtures.items():
        st = fixture_status.get(stem, {})
        matrix_rows.append({"fixture": stem, "function": meta["function"], "kind": meta["kind"], "source": meta["source"],
                            "purpose": meta["note"], "expects": meta["expect"], "forbids": meta["forbid"],
                            "result": st.get("status", "not-run"), "trace": st.get("trace"), "events": st.get("events")})
    test_matrix = {"schema": "fable-script-test-matrix/0.1", "script": phases["script"], "fixtures": matrix_rows,
                   "runtimeOnlyBranches": [
                       "actual cutscene playback / camera fixing (CS_OAKVALEINTRO_HESDEADJIM)",
                       "section swap side effects (Q_NewOakValeIntro_PreAttack -> Q__OakValeIntro_PostAttack thing lifetimes)",
                       "TurnCreatureInto hero morph and CREATURE_HERO_CHILD controls",
                       "info-bar colour rendering and timer cadence",
                       "morality amounts (game-data float)",
                       "entity AI actions (FollowThing, Speak, conversations) and their blocking semantics",
                       "region unload/reload thread termination timing"]}

    # ---- coverage -------------------------------------------------------------------------------
    traced_functions = {meta["function"] for stem, meta in fixtures.items() if fixture_status.get(stem, {}).get("status") == "ran"}
    unsupported_by_file = {}
    for u in validation.get("unsupported", []):
        unsupported_by_file.setdefault(Path(u["file"]).stem, []).append(u["retail"])
    coverage = []
    for name, ent in entities.items():
        lua_file = ent.get("luaFiles", {}).get("quest") if ent.get("kind") == "quest" else f"NewOakValeIntro/Entities/{name}.lua"
        for fn, meta in ent.get("functions", {}).items():
            addr = meta.get("address") if isinstance(meta, dict) else meta
            ops = [o for o in ent.get("operations", []) if o.get("function") == fn]
            if not ops and fn in ("destructor", "RegisterMain"):
                status = "not-applicable (host lifecycle)"
            elif not ops and (str(addr) in EMPTY_RETAIL_BODIES or "vectors" in fn or (isinstance(meta, dict) and meta.get("emptyRetail"))):
                status = "not-applicable (retail body empty / data only)"
            elif not ops:
                status = "not-implemented"
            else:
                statuses = [status_for(o, traced_functions if ent.get("kind") == "quest" else set(), name in unsupported_by_file and "unsupported" in str(o.get("forgeBinding", "")).lower()) for o in ops]
                if any(s == "API-blocked" for s in statuses):
                    status = "partially API-blocked"
                elif all("traced" in s for s in statuses):
                    status = "implemented-and-traced"
                elif any(s == "uncertain" for s in statuses):
                    status = "implemented (uncertain args)"
                else:
                    status = "implemented-but-untraced"
            covering = [stem for stem, m in fixtures.items() if m["function"] == fn and (ent.get("kind") == "quest" or (m.get("source") or "").endswith(f"{name}.lua"))]
            coverage.append({"script": name, "nativeFunction": fn, "address": addr, "operations": len(ops),
                             "luaLocation": lua_file, "fixtures": covering,
                             "evidence": (meta.get("evidence") if isinstance(meta, dict) else None) or "native-decompile",
                             "status": status})
    coverage_doc = {"schema": "fable-script-coverage/0.1", "script": phases["script"], "rows": coverage,
                    "summary": {
                        "functions": len(coverage),
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
