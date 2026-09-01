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
except ModuleNotFoundError:  # direct execution from this directory
    from extract_native_operation_ir import (
        STATE_WRITE_RE, calls, correlate_direct_call_targets, indirect_calls, strings,
    )


INTEGER_RE = r"(?:0x[0-9a-fA-F]+|-?\d+)"


def semantic_patterns(text: str, direct: list[dict[str, Any]],
                      indirect: list[dict[str, Any]]) -> list[dict[str, Any]]:
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
            cluster_dir: Path | None = None) -> dict[str, Any]:
    source = json.loads(source_path.read_text(encoding="utf-8-sig"))
    consumers_by_target = load_consumers(ir_dir, cluster_dir)
    rows = []
    for helper in source["helpers"]:
        text = helper.get("decompile") or ""
        direct = helper.get("directCalls", [])
        parsed = correlate_direct_call_targets(calls(text), direct) if text else []
        indirect = indirect_calls(text)
        semantics = semantic_patterns(text, direct, indirect)
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
            "strings": strings(text), "stateWrites": writes,
            "controlFlow": dict(sorted(controls.items())),
            "semanticPatterns": semantics,
            "parentInitializerEvidence": parent_initializer,
            "luaEmissionReady": any(pattern["kind"] in {
                "constant-return-switch", "native-field-initializer"
            } and pattern["complete"] for pattern in semantics),
        })
    stages = Counter(row["stage"] for row in rows)
    categories = Counter(row["category"] for row in rows)
    direct_calls = [call for row in rows for call in row["directCallTargets"]]
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
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    result = analyze(args.source, args.script_ir, args.clusters)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(result["summary"], sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
