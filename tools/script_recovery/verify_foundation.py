#!/usr/bin/env python3
"""Audit the complete script-recovery foundation from its generated evidence."""

from __future__ import annotations

import argparse
import json
from pathlib import Path
from typing import Any


def load(path: Path) -> Any:
    return json.loads(path.read_text(encoding="utf-8-sig"))


def verify(root: Path) -> dict[str, Any]:
    checks: list[dict[str, Any]] = []

    def check(name: str, condition: bool, evidence: Any) -> None:
        checks.append({"name": name, "passed": bool(condition), "evidence": evidence})

    corpus = root / "seed_corpus"
    index = load(corpus / "corpus_index.json")
    packages = index["packages"]
    lua_files = [row for package in packages for row in package["files"] if row["path"].lower().endswith(".lua")]
    registry_files = [row for row in lua_files if row["path"].lower().endswith("/quests.lua")]
    script_files = [row for row in lua_files if row not in registry_files]
    ir_files = [row for row in script_files if "ir" in row]
    check("six seed packages ingested", len(packages) == 6, [row["name"] for row in packages])
    hashes_ok = all(row.get("archiveSha256") and len(row["archiveSha256"]) == 64 for row in packages)
    check("archive provenance hashed", hashes_ok, [row.get("archiveSha256") for row in packages])
    check("all executable Lua scripts have normalized IR",
          len(registry_files) == 6 and len(script_files) == len(ir_files) == 16,
          {"registries": len(registry_files), "scripts": len(script_files), "ir": len(ir_files)})
    check("merged registry emitted", (corpus / "combined_registry.lua").is_file(), "combined_registry.lua")

    validation = load(corpus / "validation.json")
    audit = load(root / "forgefse_runtime_audit.json")
    unresolved = {(row["status"], row["scope"].lower(), row["name"])
                  for row in audit["calls"] if row["status"] != "callable"}
    validation_api = set()
    for row in validation:
        if row["code"] not in {"unknown-api", "wrong-api-scope"}:
            continue
        quoted = row["message"].split("'")[1]
        scope = "entity" if "entity API" in row["message"] or "called on me" in row["message"] else "quest"
        status = "wrong-scope" if row["code"] == "wrong-api-scope" else "missing"
        validation_api.add((status, scope, quoted))
    check("static API findings reconciled with live bindings", validation_api == unresolved,
          {"validation": sorted(validation_api), "runtimeAudit": sorted(unresolved)})

    catalog = load(root / "native_catalog.json")
    kinds = catalog["summary"]["byKind"]
    check("retail catalog includes non-quest scripts",
          catalog["summary"]["total"] == 161 and len(kinds) >= 10 and kinds.get("quest") == 66,
          catalog["summary"])
    queue = load(root / "conversion_queue.json")["queue"]
    check("conversion queue covers retail catalog", len(queue) == 161, len(queue))

    manifest = load(root / "shadow_manifest.json")
    safe = all(row["mode"] == "shadow" and row["evidenceLevel"] == "reconstructed-source"
               and not row["mutatingCallsAllowed"] and not row["saveWritesAllowed"]
               for row in manifest["entries"])
    identities = [row["nativeName"] for row in manifest["entries"]]
    entity_count = sum(row.get("kind") == "entity" for row in manifest["entries"])
    check("reconstructed scripts are shadow-only",
          len(manifest["entries"]) == len(script_files) == 16 and entity_count == 10 and
          len(set(identities)) == len(identities) and safe,
          {"default": manifest["defaultMode"], "entries": len(manifest["entries"]),
           "entities": entity_count})

    traces = sorted((root / "traces").glob("*.json"))
    trace_rows = [load(path) for path in traces]
    trace_ok = len(trace_rows) >= 2 and all(row["schema"] == "forgefse-lua-event-trace/0.1"
        and row["events"] and len(row.get("sourceSha256", "")) == 64 for row in trace_rows)
    check("executable Lua traces recorded", trace_ok,
          [{"file": path.name, "events": len(row["events"])} for path, row in zip(traces, trace_rows)])

    clusters = sorted((root / "native_clusters").glob("*.json"))
    cluster_rows = [load(path) for path in clusters]
    anchored = [row for row in cluster_rows if row.get("evidenceAnchors")]
    seed_names = {row["nativeName"] for row in catalog.get("seedCorrelations", [])
                  if row.get("status") == "matched"}
    check("anchored native decompilation proven",
          len(cluster_rows) >= 16 and {row["script"] for row in anchored} == seed_names,
          [{"script": row["script"], "allocator": row["allocatorAddress"],
            "anchors": row.get("evidenceAnchors", [])} for row in cluster_rows])

    operation_irs = sorted((root / "native_operation_ir").glob("*.json"))
    operation_rows = [load(path) for path in operation_irs]
    check("native lifecycle clusters have operation IR",
          len(operation_rows) >= 16 and all(len(row.get("lifecycle", [])) == 5 for row in operation_rows),
          [row.get("script") for row in operation_rows])
    comparison = load(root / "seed_native_comparison.json")
    bindings_missing = {row["package"]: row["luaBindingsMissingNativeLifecycle"]
                        for row in comparison["scripts"] if row["luaBindingsMissingNativeLifecycle"]}
    check("all reconstructed parent bindings correlate with native lifecycle evidence",
          len(comparison["scripts"]) == 6 and not bindings_missing, bindings_missing)

    passed = sum(row["passed"] for row in checks)
    return {"schema": "fable-script-recovery-foundation-audit/0.1",
            "summary": {"passed": passed, "total": len(checks), "complete": passed == len(checks)},
            "checks": checks}


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", type=Path, default=Path("refs/script_recovery"))
    parser.add_argument("--output", type=Path)
    args = parser.parse_args()
    result = verify(args.root)
    text = json.dumps(result, indent=2, sort_keys=True) + "\n"
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(text, encoding="utf-8")
    print(json.dumps(result["summary"], sort_keys=True))
    return 0 if result["summary"]["complete"] else 1


if __name__ == "__main__":
    raise SystemExit(main())
