#!/usr/bin/env python3
"""Extract conservative operation/state evidence from native lifecycle decompiles."""

from __future__ import annotations

import argparse
import json
import re
from pathlib import Path
from typing import Any


STRING_RE = re.compile(r'"((?:[^"\\]|\\.)*)"')
CALL_RE = re.compile(r'(?<![\w])((?:[A-Za-z_]\w*::)*[A-Za-z_]\w*)\s*\(')
STATE_WRITE_RE = re.compile(r'\*\([^)]*\*\)\([^\n]*?\+\s*(0x[0-9a-fA-F]+)\)\s*=\s*([^;]+);')
INDIRECT_CALL_RE = re.compile(r'\(\*\*\(code \*\*\)\((.*?)\)\)\s*\(', re.DOTALL)
VTABLE_OFFSET_RE = re.compile(r'\+\s*(0x[0-9a-fA-F]+|[0-9]+)\s*$')
CONTROL_WORDS = {"if", "for", "while", "switch", "sizeof", "return"}


def strings(text: str) -> list[str]:
    return [bytes(match.group(1), "utf-8").decode("unicode_escape") for match in STRING_RE.finditer(text)]


def calls(text: str) -> list[dict[str, Any]]:
    result = []
    for match in CALL_RE.finditer(text):
        callee = match.group(1)
        if callee in CONTROL_WORDS:
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


def extract(cluster_path: Path) -> dict[str, Any]:
    cluster = json.loads(cluster_path.read_text(encoding="utf-8-sig"))
    lifecycle = []
    for function in cluster["lifecycle"]:
        text = function.get("decompile") or ""
        lifecycle.append({
            "role": function["role"], "address": function["address"],
            "calls": calls(text), "indirectCalls": indirect_calls(text), "strings": strings(text),
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
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    result = extract(args.cluster)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n", encoding="utf-8")
    print(json.dumps({"script": result["script"], "functions": len(result["lifecycle"]),
                      "output": str(args.output.resolve())}, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
