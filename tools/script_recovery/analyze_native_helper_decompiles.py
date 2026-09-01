#!/usr/bin/env python3
"""Reduce native helper decompiles into address-backed conversion evidence."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
from collections import Counter
from pathlib import Path
from typing import Any

try:
    from tools.script_recovery.extract_native_operation_ir import (
        STATE_WRITE_RE, calls, correlate_direct_call_targets, indirect_calls, strings,
    )
    from tools.script_recovery.analyze_native_conversion_readiness import (
        HOST_MANAGED_METHODS, LUA_QUEST_BINDING_RE, RUNTIME_ABI_BLOCKERS, RUNTIME_ALIASES,
        load_interface_methods,
    )
except ModuleNotFoundError:  # direct execution from this directory
    from extract_native_operation_ir import (
        STATE_WRITE_RE, calls, correlate_direct_call_targets, indirect_calls, strings,
    )
    from analyze_native_conversion_readiness import (
        HOST_MANAGED_METHODS, LUA_QUEST_BINDING_RE, RUNTIME_ABI_BLOCKERS, RUNTIME_ALIASES,
        load_interface_methods,
    )


INTEGER_RE = r"(?:0x[0-9a-fA-F]+|-?\d+)"


def semantic_patterns(text: str, direct: list[dict[str, Any]],
                      indirect: list[dict[str, Any]], current_name: str = "") -> list[dict[str, Any]]:
    """Recognize only complete helper shapes that can be preserved losslessly."""
    patterns = []
    switch = re.search(r"switch\s*\(\s*([A-Za-z_]\w*)\s*\)\s*\{(.*?)\}", text, re.DOTALL)
    if switch and not direct and not indirect:
        body = switch.group(2)
        cases = [(int(key, 0), int(value, 0)) for key, value in re.findall(
            rf"case\s+({INTEGER_RE})\s*:\s*return\s+({INTEGER_RE})\s*;", body)]
        default = re.search(rf"default\s*:\s*return\s+({INTEGER_RE})\s*;", body)
        residue = re.sub(rf"(?:case\s+{INTEGER_RE}|default)\s*:\s*return\s+{INTEGER_RE}\s*;", "", body)
        if cases and default and not residue.strip():
            patterns.append({"kind": "constant-return-switch", "parameter": switch.group(1),
                             "cases": [{"input": key, "return": value} for key, value in cases],
                             "defaultReturn": int(default.group(1), 0), "complete": True})
    initializers = []
    for match in re.finditer(rf"this\[(0x[0-9a-fA-F]+)\]\s*=\s*\([^)]*\)({INTEGER_RE})\s*;", text):
        initializers.append({"fieldOffset": match.group(1).lower(), "width": 1,
                             "valueExpression": match.group(2)})
    for match in re.finditer(
            rf"\*\(undefined4 \*\)\(this \+ (0x[0-9a-fA-F]+)\)\s*=\s*({INTEGER_RE})\s*;", text):
        initializers.append({"fieldOffset": match.group(1).lower(), "width": 4,
                             "valueExpression": match.group(2)})
    if initializers and not direct and not indirect:
        patterns.append({"kind": "native-field-initializer", "writes": initializers,
                         "complete": True, "luaFieldNamesResolved": False})
    returns = re.findall(r"\breturn\s+([^;]+)\s*;", text)
    if len(returns) == 1 and not direct and not indirect:
        expression = returns[0].strip()
        if re.fullmatch(INTEGER_RE, expression):
            patterns.append({"kind": "constant-return", "return": int(expression, 0),
                             "complete": True})
        field = re.fullmatch(
            rf"\*\(([^)]+)\)\(this \+ ({INTEGER_RE})\)(?:\s*(!=)\s*0)?",
            expression)
        if field:
            native_type, comparison = " ".join(field.group(1).split()), field.group(3)
            accessor = "read_i32" if native_type == "int *" else (
                "read_ptr" if native_type.endswith(" **") else None)
            if accessor:
                patterns.append({"kind": "native-field-return",
                                 "fieldOffset": f"0x{int(field.group(2), 0):x}",
                                 "accessor": accessor,
                                 "resultTransform": "not-zero" if comparison else "identity",
                                 "complete": True, "luaFieldNameResolved": False})
        global_read = re.fullmatch(r"DAT_([0-9a-fA-F]{8})", expression)
        if global_read:
            patterns.append({"kind": "native-global-return",
                             "globalAddress": "0x" + global_read.group(1).upper(),
                             "complete": True})
    offsets = [call.get("vtableOffset") for call in indirect]
    if not direct and current_name.endswith("::RemoveArcheryQuestInfo") and offsets == [
            "0x504", "0x548", "0x548", "0x548"]:
        fields = re.findall(
            r"\+ 0x548\)\)\(\*\(undefined4 \*\)\(this \+ (0x[0-9a-fA-F]+)\)\);", text)
        if fields == ["0x58", "0x5c", "0x60"] and re.search(
                r"\+ 0x504\)\)\(0\);", text):
            patterns.append({"kind": "quest-interface-sequence", "complete": True,
                             "parameters": [], "operations": [
                {"method": "DisplayQuestInfo", "arguments": [
                    {"kind": "literal", "value": False}]},
                *({"method": "RemoveQuestInfoElement", "arguments": [
                    {"kind": "field-i32", "offset": field}]} for field in fields),
            ]})
    if not direct and current_name.endswith("::CChunkCollectionIndicator::OnPredicateFail") and offsets == ["0x53c"]:
        match = re.search(
            r"\+ 0x53c\)\)\s*\(\*\(undefined4 \*\)\(this \+ (0x[0-9a-fA-F]+)\),"
            r"\*\(undefined4 \*\)\(this \+ (0x[0-9a-fA-F]+)\),0xffffffff\);", text)
        if match:
            patterns.append({"kind": "quest-interface-sequence", "complete": True,
                             "parameters": [], "operations": [{
                "method": "UpdateQuestInfoCounter", "arguments": [
                    {"kind": "field-i32", "offset": match.group(1)},
                    {"kind": "field-i32", "offset": match.group(2)},
                    {"kind": "literal", "value": -1},
                ]}]})
    if not direct and current_name.endswith("::UpdateHighScore") and offsets == ["0x53c", "0xb54"]:
        match = re.search(
            r"\+ 0x53c\)\)\(\*\(undefined4 \*\)\(this \+ (0x[0-9a-fA-F]+)\),param_1,0xffffffff\);", text)
        if match and re.search(r"iVar1\s*=\s*-1;", text) and re.search(
                r"\+ 0xb54\)\)\(\(float\)iVar1\);", text):
            patterns.append({"kind": "quest-interface-sequence", "complete": True,
                             "parameters": ["param_1"], "operations": [
                {"method": "UpdateQuestInfoCounter", "arguments": [
                    {"kind": "field-i32", "offset": match.group(1)},
                    {"kind": "parameter", "name": "param_1"},
                    {"kind": "literal", "value": -1}]},
                {"method": "UpdateOnlineScore_Archery", "arguments": [
                    {"kind": "literal", "value": -1}]},
            ]})
    return patterns


def load_consumers(ir_dir: Path | None, cluster_dir: Path | None) -> dict[str, list[dict[str, Any]]]:
    result: dict[str, list[dict[str, Any]]] = {}
    if ir_dir is None:
        return result
    for path in sorted(ir_dir.glob("*.json")):
        ir = json.loads(path.read_text(encoding="utf-8-sig"))
        cluster = None
        if cluster_dir is not None:
            cluster_path = cluster_dir / path.name
            if cluster_path.is_file():
                cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
        cluster_lifecycle = ({row["role"]: row for row in cluster["lifecycle"]}
                             if cluster else {})
        for life in ir["lifecycle"]:
            text = cluster_lifecycle.get(life["role"], {}).get("decompile", "")
            for call in life["calls"]:
                target = call.get("targetAddress")
                if not target:
                    continue
                statement = None
                offset = call.get("offset")
                if text and isinstance(offset, int):
                    end = text.find(";", offset)
                    if end >= 0:
                        statement = " ".join(text[offset:end + 1].split())
                result.setdefault(target, []).append({
                    "script": ir["script"], "role": life["role"],
                    "site": call.get("directCallSite"), "callee": call["callee"],
                    "statement": statement,
                })
    return result


def analyze(source_path: Path, ir_dir: Path | None = None,
            cluster_dir: Path | None = None, slots_path: Path | None = None,
            interface_catalog_path: Path | None = None,
            api_manifest_path: Path | None = None,
            lua_manager_path: Path | None = None) -> dict[str, Any]:
    source = json.loads(source_path.read_text(encoding="utf-8-sig"))
    consumers_by_target = load_consumers(ir_dir, cluster_dir)
    interface_methods = load_interface_methods(slots_path, interface_catalog_path)
    api_names = set()
    if api_manifest_path:
        api_names = {row["name"] for row in json.loads(
            api_manifest_path.read_text(encoding="utf-8-sig"))["functions"]}
    runtime_bindings = set()
    if lua_manager_path:
        runtime_bindings = set(LUA_QUEST_BINDING_RE.findall(
            lua_manager_path.read_text(encoding="utf-8-sig", errors="replace")))
    rows = []
    for helper in source["helpers"]:
        text = helper.get("decompile") or ""
        direct = helper.get("directCalls", [])
        parsed = correlate_direct_call_targets(calls(text), direct) if text else []
        indirect = indirect_calls(text)
        resolved_interface_calls = []
        for call in indirect:
            if call.get("interfaceProvenance") in {
                "direct-gamescriptinterface-singleton",
                "local-copy-of-gamescriptinterface-singleton",
                "script-instance-gamescriptinterface-field",
                "local-copy-of-script-interface-vtable",
            } and call.get("vtableOffset") in interface_methods:
                method = interface_methods[call["vtableOffset"]]
                runtime_name = RUNTIME_ALIASES.get(method["name"], method["name"])
                resolved_interface_calls.append({
                    **call, **method,
                    "forgeManifestMatch": method["name"] in api_names,
                    "forgeRuntimeName": runtime_name,
                    "forgeRuntimeMatch": runtime_name in runtime_bindings,
                    "forgeHostManaged": method["name"] in HOST_MANAGED_METHODS,
                    "forgeRuntimeAbiBlocker": RUNTIME_ABI_BLOCKERS.get(method["name"]),
                })
        semantics = semantic_patterns(text, direct, indirect, helper["currentName"])
        consumers = consumers_by_target.get(helper["targetAddress"], [])
        initializer = next((pattern for pattern in semantics
                            if pattern["kind"] == "native-field-initializer"), None)
        parent_initializer = None
        if initializer and consumers and all(
                row["role"] == "Main" and row.get("statement") and
                re.search(r"\bthis\s*\)\s*;\s*$", row["statement"])
                for row in consumers):
            owners = sorted({row["script"] for row in consumers})
            if len(owners) == 1:
                parent_initializer = {
                    "script": owners[0], "role": "parent-field-initializer",
                    "evidence": "exact Main call target passes parent this pointer",
                    "callSites": sorted({row["site"] for row in consumers}),
                }
        controls = Counter(match.group(1) for match in re.finditer(
            r"\b(if|while|for|switch|goto)\b", text))
        writes = [{"fieldOffset": match.group(1).lower(),
                   "valueExpression": match.group(2).strip(), "offset": match.start()}
                  for match in STATE_WRITE_RE.finditer(text)]
        status = helper["status"]
        if status != "decompiled":
            stage = "repair-helper-decompile"
        elif not direct and not indirect:
            stage = "inspect-leaf-helper"
        else:
            stage = "inspect-helper-dependencies"
        rows.append({
            "targetAddress": helper["targetAddress"], "currentName": helper["currentName"],
            "helperNames": [name.strip() for name in helper["helperNames"].split("|")],
            "category": helper["category"], "consumerCalls": helper["calls"],
            "consumerScriptCount": helper["scripts"], "consumers": consumers,
            "consumerRoles": [role for role in helper["roles"].split(",") if role],
            "status": status, "error": helper.get("error"), "stage": stage,
            "decompileSha256": hashlib.sha256(text.encode("utf-8")).hexdigest() if text else None,
            "decompileLines": len(text.splitlines()) if text else 0,
            "calls": parsed, "directCallTargets": direct, "indirectCalls": indirect,
            "resolvedInterfaceCalls": resolved_interface_calls,
            "strings": strings(text), "stateWrites": writes,
            "controlFlow": dict(sorted(controls.items())),
            "semanticPatterns": semantics,
            "parentInitializerEvidence": parent_initializer,
            "luaEmissionReady": any(pattern["kind"] in {
                "constant-return-switch", "native-field-initializer", "constant-return",
                "native-field-return", "native-global-return", "quest-interface-sequence",
            } and pattern["complete"] for pattern in semantics),
        })
    stages = Counter(row["stage"] for row in rows)
    categories = Counter(row["category"] for row in rows)
    direct_calls = [call for row in rows for call in row["directCallTargets"]]
    resolved_interface_calls = [call for row in rows for call in row["resolvedInterfaceCalls"]]
    missing_manifest = sorted({call["name"] for call in resolved_interface_calls
                               if not call["forgeManifestMatch"]})
    missing_runtime = sorted({call["name"] for call in resolved_interface_calls
                              if not call["forgeRuntimeMatch"] and not call["forgeHostManaged"]})
    abi_blockers = {call["name"]: call["forgeRuntimeAbiBlocker"]
                    for call in resolved_interface_calls if call["forgeRuntimeAbiBlocker"]}
    return {
        "schema": "fable-native-helper-operation-ir/0.1",
        "source": str(source_path.resolve()),
        "summary": {
            "targets": len(rows),
            "decompiled": sum(row["status"] == "decompiled" for row in rows),
            "failed": sum(row["status"] != "decompiled" for row in rows),
            "consumerCalls": sum(row["consumerCalls"] for row in rows),
            "directDependencyCalls": len(direct_calls),
            "directDependencyTargets": len({call["target"] for call in direct_calls}),
            "semanticPatterns": sum(len(row["semanticPatterns"]) for row in rows),
            "luaEmissionReady": sum(row["luaEmissionReady"] for row in rows),
            "resolvedParentInitializers": sum(row["parentInitializerEvidence"] is not None
                                              for row in rows),
            "resolvedInterfaceCalls": len(resolved_interface_calls),
            "resolvedInterfaceMethods": len({call["name"] for call in resolved_interface_calls}),
            "resolvedForgeInterfaceCalls": sum(call["forgeManifestMatch"]
                                               for call in resolved_interface_calls),
            "resolvedForgeRuntimeCalls": sum(call["forgeRuntimeMatch"] or call["forgeHostManaged"]
                                             for call in resolved_interface_calls),
            "missingForgeInterfaceMethods": missing_manifest,
            "missingForgeRuntimeMethods": missing_runtime,
            "abiBlockedInterfaceMethods": dict(sorted(abi_blockers.items())),
            "stages": dict(sorted(stages.items())),
            "categories": dict(sorted(categories.items())),
        },
        "helpers": rows,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", type=Path, required=True)
    parser.add_argument("--script-ir", type=Path)
    parser.add_argument("--clusters", type=Path)
    parser.add_argument("--vtable-slots", type=Path)
    parser.add_argument("--interface-catalog", type=Path)
    parser.add_argument("--api-manifest", type=Path)
    parser.add_argument("--lua-manager", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    result = analyze(args.source, args.script_ir, args.clusters,
                     args.vtable_slots, args.interface_catalog,
                     args.api_manifest, args.lua_manager)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result["summary"], sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
