#!/usr/bin/env python3
"""Rank native script IR for evidence-gated ForgeFSE Lua reconstruction."""

from __future__ import annotations

import argparse
import csv
import json
import re
from pathlib import Path
from typing import Any


OPAQUE_RE = re.compile(r"^(?:FUN_|LAB_|SUB\d*$|thunk_FUN_)", re.IGNORECASE)
INFRASTRUCTURE_RE = re.compile(
    r"^(?:operator_(?:new|delete)|CCharString::|std::|CGuiVarTransferStruct::Add|"
    r"CScriptBase::IsActiveThreadTerminating|CSpawnedFunc<|Compare$|CONCAT\d+|CTimer$)"
)


def api_matcher(api_names: list[str]) -> re.Pattern[str]:
    alternatives = "|".join(re.escape(name) for name in sorted(api_names, key=len, reverse=True))
    return re.compile(rf"(?:^|::|_)({alternatives})(?=$|_)")


def api_match(callee: str, api_names: list[str]) -> str | None:
    match = api_matcher(api_names).search(callee)
    return match.group(1) if match else None


def load_interface_methods(slots_path: Path | None, catalog_path: Path | None) -> dict[str, dict[str, str]]:
    if not slots_path or not catalog_path:
        return {}
    addresses = {}
    with catalog_path.open(encoding="utf-8-sig") as stream:
        for line in stream:
            address, name = line.rstrip("\r\n").split("\t", 1)
            addresses[address.lower()] = name
    with slots_path.open(encoding="utf-8-sig", newline="") as stream:
        result = {}
        for row in csv.DictReader(stream, delimiter="\t"):
            if row["executable"].lower() != "true":
                continue
            name = addresses.get(row["target_address"].lower(), row["current_name"])
            decorated = re.match(r"^\?([^@]+)@CGameScriptInterface@@", name)
            if decorated:
                name = decorated.group(1)
            result[row["offset"].lower()] = {"name": name,
                                    "targetAddress": f"0x{row['target_address'].upper()}",
                                    "slotAddress": f"0x{row['slot_address'].upper()}"}
        return result


def analyze(catalog_path: Path, ir_dir: Path, manifest_path: Path,
            slots_path: Path | None = None, interface_catalog_path: Path | None = None,
            interface_field_evidence_path: Path | None = None) -> dict[str, Any]:
    catalog = json.loads(catalog_path.read_text(encoding="utf-8-sig"))
    manifest = json.loads(manifest_path.read_text(encoding="utf-8-sig"))
    api_names = sorted({row["name"] for row in manifest["functions"]}, key=len, reverse=True)
    match_api = api_matcher(api_names)
    interface_methods = load_interface_methods(slots_path, interface_catalog_path)
    verified_interface_fields = set()
    if interface_field_evidence_path:
        with interface_field_evidence_path.open(encoding="utf-8-sig", newline="") as stream:
            verified_interface_fields = {
                row["script"] for row in csv.DictReader(stream, delimiter="\t")
                if row["verified"].lower() == "true"
            }
    kinds = {row["name"]: row["kind"] for row in catalog["scripts"]}
    rows = []
    for path in sorted(ir_dir.glob("*.json")):
        ir = json.loads(path.read_text(encoding="utf-8-sig"))
        calls = [call["callee"] for life in ir["lifecycle"] for call in life["calls"]]
        indirect = [call for life in ir["lifecycle"] for call in life.get("indirectCalls", [])]
        interface_calls = []
        for call in indirect:
            method = interface_methods.get(call.get("vtableOffset"))
            provenance = call.get("interfaceProvenance")
            singleton_provenance = provenance in {
                "direct-gamescriptinterface-singleton",
                "local-copy-of-gamescriptinterface-singleton",
            }
            field_provenance = provenance in {
                "script-instance-gamescriptinterface-field",
                "local-copy-of-script-interface-vtable",
            } and ir["script"] in verified_interface_fields
            if (singleton_provenance or field_provenance) and method:
                interface_calls.append({**call, **method, "forgeManifestMatch": method["name"] in api_names})
        opaque = sorted({callee for callee in calls if OPAQUE_RE.match(callee)})
        matches = {callee: match_api.search(callee) for callee in set(calls)}
        mapped = sorted({match.group(1) for match in matches.values() if match})
        infrastructure = {callee for callee in calls if INFRASTRUCTURE_RE.match(callee)}
        unresolved = sorted(set(calls) - set(opaque) - infrastructure -
                            {callee for callee, match in matches.items() if match})
        decompile_complete = len(ir["lifecycle"]) == 5 and all(
            life.get("address") and isinstance(life.get("calls"), list) for life in ir["lifecycle"]
        )
        if not decompile_complete:
            stage = "repair-native-ir"
        elif opaque:
            stage = "resolve-opaque-callees"
        elif unresolved:
            stage = "map-native-helpers"
        else:
            stage = "manual-lua-reconstruction"
        rows.append({
            "name": ir["script"], "kind": kinds.get(ir["script"], "unknown"),
            "allocatorAddress": ir["allocatorAddress"], "vtableAddress": ir["vtableAddress"],
            "anchored": bool(ir.get("evidenceAnchors")), "lifecycleFunctions": len(ir["lifecycle"]),
            "calls": len(calls), "distinctCalls": len(set(calls)), "mappedForgeApis": mapped,
            "indirectCalls": len(indirect),
            "resolvedInterfaceCalls": interface_calls,
            "resolvedInterfaceMethods": sorted({call["name"] for call in interface_calls}),
            "indirectVtableOffsets": sorted({call["vtableOffset"] for call in indirect
                                              if call.get("vtableOffset")}),
            "opaqueCallees": opaque, "unresolvedNativeHelpers": unresolved, "stage": stage,
            "nativeIr": str(path.resolve()),
        })
    rows.sort(key=lambda row: (
        {"manual-lua-reconstruction": 0, "map-native-helpers": 1,
         "resolve-opaque-callees": 2, "repair-native-ir": 3}[row["stage"]],
        len(row["opaqueCallees"]) + len(row["unresolvedNativeHelpers"]), row["name"]
    ))
    stages: dict[str, int] = {}
    for row in rows:
        stages[row["stage"]] = stages.get(row["stage"], 0) + 1
    resolved_methods = {call["name"] for row in rows for call in row["resolvedInterfaceCalls"]}
    missing_methods = {call["name"] for row in rows for call in row["resolvedInterfaceCalls"]
                       if not call["forgeManifestMatch"]}
    return {
        "schema": "forgefse-native-conversion-readiness/0.1",
        "sources": {"catalog": str(catalog_path.resolve()), "ir": str(ir_dir.resolve()),
                    "apiManifest": str(manifest_path.resolve()),
                    "interfaceFieldEvidence": (str(interface_field_evidence_path.resolve())
                                               if interface_field_evidence_path else None)},
        "summary": {"scripts": len(rows), "anchored": sum(row["anchored"] for row in rows),
                    "lifecycleComplete": sum(row["lifecycleFunctions"] == 5 for row in rows),
                    "verifiedScriptInterfaceFields": len(verified_interface_fields),
                    "resolvedInterfaceCalls": sum(len(row["resolvedInterfaceCalls"]) for row in rows),
                    "resolvedForgeInterfaceCalls": sum(
                        sum(call["forgeManifestMatch"] for call in row["resolvedInterfaceCalls"])
                        for row in rows),
                    "resolvedInterfaceMethods": len(resolved_methods),
                    "missingForgeInterfaceMethods": sorted(missing_methods),
                    "scriptsWithResolvedInterfaceCalls": sum(bool(row["resolvedInterfaceCalls"]) for row in rows),
                    "stages": dict(sorted(stages.items()))},
        "scripts": rows,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--catalog", type=Path, required=True)
    parser.add_argument("--ir", type=Path, required=True)
    parser.add_argument("--api-manifest", type=Path, required=True)
    parser.add_argument("--vtable-slots", type=Path)
    parser.add_argument("--interface-catalog", type=Path)
    parser.add_argument("--interface-field-evidence", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    result = analyze(args.catalog, args.ir, args.api_manifest, args.vtable_slots,
                     args.interface_catalog, args.interface_field_evidence)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result["summary"], sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
