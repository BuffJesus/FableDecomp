#!/usr/bin/env python3
"""Extract conservative operation/state evidence from native lifecycle decompiles."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path
from typing import Any


STRING_RE = re.compile(r'"((?:[^"\\]|\\.)*)"')
CALL_RE = re.compile(r'(?<![\w])((?:[A-Za-z_]\w*::)*(?:~?[A-Za-z_]\w*))\s*\(')
STATE_WRITE_RE = re.compile(r'\*\([^)]*\*\)\([^\n]*?\+\s*(0x[0-9a-fA-F]+)\)\s*=\s*([^;]+);')
INDIRECT_CALL_RE = re.compile(r'\(\*\*\(code \*\*\)\((.*?)\)\)\s*\(', re.DOTALL)
VTABLE_OFFSET_RE = re.compile(r'\+\s*(0x[0-9a-fA-F]+|[0-9]+)\s*$')
ENTITY_BINDING_CALL_RE = re.compile(r'CScriptBase::AddEntityScriptBinding\s*\(')
CONTROL_WORDS = {"if", "for", "while", "switch", "sizeof", "return"}
GHIDRA_PCODE_RE = re.compile(r"^(?:SUB\d+|CONCAT\d+|ZEXT\d*|SEXT\d*|CARRY\d*|SCARRY\d*|SBORROW\d*)$")


def strings(text: str) -> list[str]:
    return [bytes(match.group(1), "utf-8").decode("unicode_escape") for match in STRING_RE.finditer(text)]


def calls(text: str) -> list[dict[str, Any]]:
    result = []
    # Ghidra's decompile text includes the function declaration before the opening
    # brace. CALL_RE also matches that declaration, which previously made every
    # default-named lifecycle function appear to call itself (for example
    # `void FUN_00cbd4e0(void)`). Only expressions in the function body are calls.
    body_start = text.find("{")
    for match in CALL_RE.finditer(text):
        if body_start >= 0 and match.start() < body_start:
            continue
        callee = match.group(1)
        if callee in CONTROL_WORDS or GHIDRA_PCODE_RE.match(callee):
            continue
        result.append({"operation": "call", "callee": callee, "offset": match.start()})
    return result


def persistence(text: str) -> list[dict[str, Any]]:
    result = []
    for call in re.finditer(r'CPersistContext::Transfer<([^>]+)>\s*\((.*?)\);', text, re.DOTALL):
        literals = strings(call.group(2))
        result.append({"key": literals[0] if literals else None, "nativeType": call.group(1).strip(),
                       "offset": call.start()})
    return result


def indirect_calls(text: str) -> list[dict[str, Any]]:
    result = []
    for match in INDIRECT_CALL_RE.finditer(text):
        expression = " ".join(match.group(1).split())
        offset = VTABLE_OFFSET_RE.search(expression)
        provenance = None
        if "DAT_0143e8f8" in expression:
            provenance = "direct-gamescriptinterface-singleton"
        elif "**(int **)(this + 0x40)" in expression:
            provenance = "script-instance-gamescriptinterface-field"
        else:
            base = re.match(r"([A-Za-z_]\w*)\s*\+", expression)
            if base:
                assignments = list(re.finditer(
                    rf"\b{re.escape(base.group(1))}\s*=\s*([^;]+);", text[:match.start()]
                ))
                if assignments:
                    source = assignments[-1].group(1).strip()
                    if source == "*DAT_0143e8f8":
                        provenance = "local-copy-of-gamescriptinterface-singleton"
                    elif source == "**(int **)(this + 0x40)":
                        provenance = "local-copy-of-script-interface-vtable"
        normalized_offset = f"0x{int(offset.group(1), 0):x}" if offset else None
        result.append({"operation": "indirect-call", "targetExpression": expression,
                       "vtableOffset": normalized_offset,
                       "interfaceProvenance": provenance,
                       "offset": match.start()})
    return result


def entity_bindings(text: str) -> list[dict[str, Any]]:
    """Recover the retail 0x1c-byte entity-binding records built before registration."""
    result = []
    previous_end = 0
    for call in ENTITY_BINDING_CALL_RE.finditer(text):
        # A binding is assembled immediately before AddEntityScriptBinding. Bound
        # the search by the preceding registration so adjacent records cannot mix.
        window_start = max(previous_end, call.start() - 3000)
        window = text[window_start:call.start()]
        names = list(re.finditer(
            r'CCharString::CCharString\s*\([^;]*?"((?:[^"\\]|\\.)*)"\s*,\s*-1\s*\)',
            window, re.DOTALL))
        allocations = list(re.finditer(r'operator_new\s*\(\s*(0x[0-9a-fA-F]+|\d+)\s*\)', window))
        vtables = list(re.finditer(r'=\s*&([^;\r\n]*PTR_[^;\r\n]+);', window))
        allocators = list(re.finditer(
            r'\*\([^)]*\)\s*\([^;\r\n]*?\+\s*0x10\)\s*=\s*([^;]+);', window))
        template_constructors = list(re.finditer(
            r'CEntityScriptBinding<([^>]+)>::\s*CEntityScriptBinding<[^>]+>\s*'
            r'\(.*?,\s*([A-Za-z_]\w*(?:::[A-Za-z_]\w*)*)\s*,\s*'
            r'(0x[0-9a-fA-F]+|\d+)\s*\);', window, re.DOTALL))
        enabled = list(re.finditer(
            r'\[0x14\]\s*=\s*\(CEntityScriptBindingBase\)(0x[0-9a-fA-F]+|\d+)', window))
        trailing = list(re.finditer(r'\+\s*0x18\)\s*=\s*(0x[0-9a-fA-F]+|\d+)\s*;', window))
        name = strings(f'"{names[-1].group(1)}"')[0] if names else None
        template_constructor = template_constructors[-1] if template_constructors else None
        allocator_expression = (allocators[-1].group(1).strip() if allocators else
                                template_constructor.group(2) if template_constructor else None)
        row = {
            "operation": "entity-binding-registration",
            "entityName": name,
            "allocationSize": int(allocations[-1].group(1), 0) if allocations else None,
            "bindingVtable": vtables[-1].group(1).strip() if vtables else None,
            "allocatorExpression": allocator_expression,
            "constructionMode": "explicit-layout" if allocators else
                                "template-constructor" if template_constructor else "unresolved",
            "constructorScriptType": template_constructor.group(1) if template_constructor else None,
            "enabled": bool(int(enabled[-1].group(1), 0)) if enabled else None,
            "trailingValue": int(trailing[-1].group(1), 0) if trailing else None,
            "offset": call.start(),
        }
        row["complete"] = all(row[key] is not None for key in (
            "entityName", "allocationSize", "allocatorExpression"))
        row["layoutComplete"] = all(row[key] is not None for key in (
            "bindingVtable", "enabled", "trailingValue"))
        result.append(row)
        previous_end = call.end()
    return result


def normalize_address(value: str) -> str:
    return f"0x{int(value, 16):08X}"


def load_direct_call_evidence(path: Path) -> dict[str, dict[str, Any]]:
    """Index Ghidra instruction-level calls by allocator, the stable retail identity."""
    document = json.loads(path.read_text(encoding="utf-8-sig"))
    result = {}
    for script in document["scripts"]:
        key = normalize_address(script["allocatorAddress"])
        if key in result:
            raise ValueError(f"duplicate allocator in direct-call evidence: {key}")
        result[key] = script
    return result


def extract(cluster_path: Path,
            direct_call_evidence: dict[str, dict[str, Any]] | None = None) -> dict[str, Any]:
    cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
    evidence = None
    if direct_call_evidence is not None:
        allocator = normalize_address(cluster["allocatorAddress"])
        evidence = direct_call_evidence.get(allocator)
        if evidence is None:
            raise ValueError(f"no direct-call evidence for allocator {allocator} ({cluster['script']})")
        evidence_lifecycle = {row["role"]: row for row in evidence["lifecycle"]}
    lifecycle = []
    for function in cluster["lifecycle"]:
        text = function.get("decompile") or ""
        direct_calls = []
        if evidence is not None:
            direct = evidence_lifecycle.get(function["role"])
            if direct is None:
                raise ValueError(f"missing {function['role']} direct-call evidence for {cluster['script']}")
            if normalize_address(direct["address"]) != normalize_address(function["address"]):
                raise ValueError(
                    f"{cluster['script']} {function['role']} address mismatch: "
                    f"{function['address']} != {direct['address']}"
                )
            direct_calls = direct["directCalls"]
        lifecycle.append({
            "role": function["role"], "address": function["address"],
            "calls": calls(text), "indirectCalls": indirect_calls(text), "strings": strings(text),
            "directCallTargets": direct_calls,
            "entityBindings": entity_bindings(text),
            "stateWrites": [{"fieldOffset": match.group(1).lower(), "valueExpression": match.group(2).strip(),
                              "offset": match.start()} for match in STATE_WRITE_RE.finditer(text)],
            "persistenceTransfers": persistence(text),
        })
    return {
        "schema": "fable-native-script-operation-ir/0.1",
        "script": cluster["script"], "cluster": str(cluster_path.as_posix()),
        "evidenceLevel": "native-decompile", "allocatorAddress": cluster["allocatorAddress"],
        "vtableAddress": cluster["vtableAddress"], "evidenceAnchors": cluster.get("evidenceAnchors", []),
        "lifecycle": lifecycle,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("cluster", type=Path)
    parser.add_argument("--direct-calls", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    evidence = load_direct_call_evidence(args.direct_calls) if args.direct_calls else None
    result = extract(args.cluster, evidence)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"script": result["script"], "functions": len(result["lifecycle"]),
                      "output": str(args.output.resolve())}, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
